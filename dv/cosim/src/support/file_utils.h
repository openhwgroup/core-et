/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <array>
#include <streambuf>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

#include <lz4frame.h>

using filename_t = std::string;

namespace file_utils {

// "abc/file.txt" => ("abc", "file.txt")
std::tuple<filename_t, filename_t> split(const filename_t& path);
// "abc/file.txt" => ("abc/file", ".txt")
std::tuple<filename_t, filename_t> split_ext(const filename_t& path);

filename_t dirname(filename_t path);
filename_t basename(filename_t path, bool remove_ext = false);
filename_t extension(filename_t path);

bool exists(const filename_t& fname);
int remove(const filename_t& fname);
int rename(const filename_t& fname1, const filename_t& fname2);

bool create_dir(const filename_t& path);

bool is_zipfile(const filename_t& fname);

// Compresses a std::ostream
class ozip_buffer final : public std::streambuf {
public:
    ozip_buffer(std::streambuf* sink);
    ozip_buffer(const ozip_buffer&) = delete;
    ozip_buffer& operator=(const ozip_buffer&) = delete;
    ~ozip_buffer();

private:
    int_type overflow(int_type c) override;
    int_type sync() override;

    void compress_and_write();
    void write_header();
    void write_footer();

    std::streambuf* sink_;
    std::array<char, 256> src_buf_;
    std::vector<char> dest_buf_;
    LZ4F_compressionContext_t ctx_;
};

// Decompresses a std::istream
class izip_buffer final : public std::streambuf {
public:
    izip_buffer(std::streambuf* src);
    izip_buffer(const izip_buffer&) = delete;
    izip_buffer& operator=(const izip_buffer&) = delete;
    ~izip_buffer();

private:
    int_type underflow() override;

    std::streambuf* src_;
    std::array<char, 256> src_buf_;
    std::array<char, 256> dest_buf_;
    size_t offset_;
    size_t src_buf_size_;
    LZ4F_decompressionContext_t ctx_;
};

// Wraps a std::ofstream. Can be compressed
class ofile final : public std::ostream {
public:
    ofile() : monitor_(new monitor())
    {
    }
    ofile(filename_t fname, bool truncate = false, bool zipped = false);
    ofile(const ofile&) = delete;
    ofile& operator=(const ofile&) = delete;
    ~ofile();

    bool is_open() const
    {
        return ofs_.is_open();
    }
    bool is_zipped() const
    {
        return ozip_ != nullptr;
    }
    void open(filename_t fname, bool truncate = false, bool zipped = false);
    void reopen(bool truncate);
    void close();

    inline std::size_t size() const
    {
        return monitor_->size();
    }

    const filename_t& filename() const
    {
        return filename_;
    }

private:
    class monitor : public std::streambuf {
    public:
        explicit monitor() = default;
        monitor(const monitor&) = delete;
        monitor& operator=(const monitor&) = delete;

        void open(std::streambuf* sink)
        {
            sink_ = sink;
            size_ = 0;
        }

        inline std::size_t size() const noexcept
        {
            return size_;
        }

    protected:
        int_type overflow(int_type ch) override
        {
            if (std::streambuf::traits_type::eq_int_type(ch, std::streambuf::traits_type::eof())) {
                return ch;
            }
            ++size_;
            return sink_->sputc(static_cast<std::streambuf::char_type>(ch));
        }

        int_type sync() override
        {
            return sink_->pubsync();
        }

    private:
        std::streambuf* sink_;
        std::size_t size_;
    };

    std::ofstream ofs_;
    ozip_buffer* ozip_ = nullptr;
    filename_t filename_;
    monitor* monitor_;
};

// Wraps a std::ifstream. Can be compressed
class ifile final : public std::istream {
public:
    explicit ifile() = default;
    ifile(const filename_t& fname, bool zipped = false);
    ifile(const ifile&) = delete;
    ifile& operator=(const ifile&) = delete;
    ~ifile();

    bool is_open() const
    {
        return ifs_.is_open();
    }
    bool is_zipped() const
    {
        return izip_ != nullptr;
    }

    void open(const filename_t& fname, bool zipped = false);
    void close();

private:
    std::ifstream ifs_;
    izip_buffer* izip_ = nullptr;
    filename_t filename_;
};
}
