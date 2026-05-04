/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "file_utils.h"

#include <cassert>
#include <exception>
#include <algorithm>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

inline void handle_lz4_error(size_t ret, std::string msg = "LZ4 Error")
{
    if (LZ4F_isError(ret) != 0) {
        throw std::runtime_error(msg + ": " + LZ4F_getErrorName(ret));
    }
}

namespace file_utils {

using int_type = ozip_buffer::int_type;

std::tuple<filename_t, filename_t> split(const filename_t& path)
{
    const auto pos = path.rfind('/');
    return pos == filename_t::npos ? std::make_tuple(filename_t(), path)
                                   : std::make_tuple(path.substr(0, pos), path.substr(pos + 1));
}

std::tuple<filename_t, filename_t> split_ext(const filename_t& path)
{
    const auto ext_pos = path.rfind('.');
    if (ext_pos == filename_t::npos || ext_pos == 0 || ext_pos == path.size() - 1) {
        return std::make_tuple(path, filename_t());
    }

    const auto folder_pos = path.rfind('/');
    if (folder_pos != filename_t::npos && folder_pos >= ext_pos - 1) {
        return std::make_tuple(path, filename_t());
    }

    return std::make_tuple(path.substr(0, ext_pos), path.substr(ext_pos));
}

filename_t dirname(filename_t path)
{
    return std::get<0>(split(path));
}

filename_t basename(filename_t path, bool remove_ext)
{
    return std::get<1>(split(remove_ext ? std::get<0>(split_ext(path)) : path));
}

filename_t extension(filename_t path)
{
    return std::get<1>(split_ext(path));
}

bool exists(const filename_t& fname)
{
    struct stat buffer;
    return (::stat(fname.c_str(), &buffer) == 0);
}

int remove(const filename_t& fname)
{
    return std::remove(fname.c_str());
}

int rename(const filename_t& fname1, const filename_t& fname2)
{
    return std::rename(fname1.c_str(), fname2.c_str());
}

bool create_dir(const filename_t& path)
{
    if (exists(path)) return true;
    if (path.empty()) return false;

    size_t offset = 0;
    do {
        auto pos = path.find('/', offset);
        if (pos == filename_t::npos) {
            pos = path.size();
        }

        const auto subdir = path.substr(0, pos);
        if (!subdir.empty() && !exists(subdir) && ::mkdir(subdir.c_str(), mode_t(0775))) {
            return false;
        }
        offset = pos + 1;
    } while (offset < path.size());

    return true;
}

bool is_zipfile(const filename_t& path)
{
    const auto ext = extension(path);
    return ext == ".zip" || ext == ".lz4" || ext == ".logz";
}

ozip_buffer::ozip_buffer(std::streambuf* sink)
    : sink_(sink)
    , dest_buf_(LZ4F_compressBound(src_buf_.size(), nullptr))
    , ctx_(nullptr)
{
    const auto base = &src_buf_.front();
    setp(base, base + src_buf_.size() - 1);

    const auto ret = LZ4F_createCompressionContext(&ctx_, LZ4F_VERSION);
    handle_lz4_error(ret);
    write_header();
}

ozip_buffer::~ozip_buffer()
{
    sync();
    write_footer();
    LZ4F_freeCompressionContext(ctx_);
}

int_type ozip_buffer::overflow(int_type ch)
{
    assert(std::less_equal<char*>()(pptr(), epptr()));

    *pptr() = static_cast<char_type>(ch);
    pbump(1);
    compress_and_write();

    return ch;
}

int_type ozip_buffer::sync()
{
    compress_and_write();
    return 0;
}

void ozip_buffer::compress_and_write()
{
    const auto orig_size = static_cast<int>(pptr() - pbase());
    pbump(-orig_size);
    const auto ret = LZ4F_compressUpdate(ctx_, &dest_buf_.front(), dest_buf_.capacity(), pbase(), orig_size, nullptr);
    handle_lz4_error(ret);
    sink_->sputn(&dest_buf_.front(), ret);
}

void ozip_buffer::write_header()
{
    const auto ret = LZ4F_compressBegin(ctx_, &dest_buf_.front(), dest_buf_.capacity(), nullptr);
    handle_lz4_error(ret);
    sink_->sputn(&dest_buf_.front(), ret);
}

void ozip_buffer::write_footer()
{
    const auto ret = LZ4F_compressEnd(ctx_, &dest_buf_.front(), dest_buf_.capacity(), nullptr);
    handle_lz4_error(ret);
    sink_->sputn(&dest_buf_.front(), ret);
}

izip_buffer::izip_buffer(std::streambuf* src)
    : src_(src)
    , offset_(0)
    , src_buf_size_(0)
    , ctx_(nullptr)
{
    const auto ret = LZ4F_createDecompressionContext(&ctx_, LZ4F_VERSION);
    handle_lz4_error(ret);
    setg(&src_buf_.front(), &src_buf_.front(), &src_buf_.front());
}

izip_buffer::~izip_buffer()
{
    LZ4F_freeDecompressionContext(ctx_);
}

int_type izip_buffer::underflow()
{
    size_t written = 0;
    while (written == 0) {
        if (offset_ == src_buf_size_) {
            src_buf_size_ = src_->sgetn(&src_buf_.front(), src_buf_.size());
            offset_ = 0;
        }

        if (src_buf_size_ == 0) {
            return traits_type::eof();
        }

        auto src_size = src_buf_size_ - offset_;
        auto dest_size = dest_buf_.size();
        const auto ret
            = LZ4F_decompress(ctx_, &dest_buf_.front(), &dest_size, &src_buf_.front() + offset_, &src_size, nullptr);
        handle_lz4_error(ret);
        written = dest_size;
        offset_ += src_size;
    }
    setg(&dest_buf_.front(), &dest_buf_.front(), &dest_buf_.front() + written);
    return traits_type::to_int_type(*gptr());
}

ofile::ofile(filename_t fname, bool truncate, bool zipped) : monitor_(new monitor())
{
    open(fname, truncate, zipped);
}

ofile::~ofile()
{
    close();
    delete monitor_;
}

void ofile::open(filename_t fname, bool truncate, bool zipped)
{
    if (is_open()) close();
    filename_ = fname;
    auto flags = std::ios::binary;
    if (truncate) flags |= std::ios::trunc;
    create_dir(dirname(fname));
    ofs_.open(fname, flags);
    monitor_->open(ofs_.rdbuf());
    if (zipped || is_zipfile(fname)) {
        ozip_ = new ozip_buffer(monitor_);
        rdbuf(ozip_);
    } else {
        rdbuf(monitor_);
    }
}

void ofile::reopen(bool truncate)
{
    open(filename(), truncate, is_zipped());
}

void ofile::close()
{
    delete ozip_;
    ozip_ = nullptr;
    ofs_.close();
}

ifile::ifile(const filename_t& fname, bool zipped)
{
    open(fname, zipped);
}

ifile::~ifile()
{
    close();
}

void ifile::open(const filename_t& fname, bool zipped)
{
    if (is_open()) close();
    ifs_.open(fname, std::ios::binary);
    if (zipped || is_zipfile(fname)) {
        izip_ = new izip_buffer(ifs_.rdbuf());
        rdbuf(izip_);
    } else {
        rdbuf(ifs_.rdbuf());
    }
}

void ifile::close()
{
    delete izip_;
    izip_ = nullptr;
    ifs_.close();
}

} // namespace file_utils
