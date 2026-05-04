/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <mutex>
#include <ostream>
#include <string>

#include <spdlog/details/null_mutex.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/synchronous_factory.h>
#include <spdlog/fmt/fmt.h>

#include "file_utils.h"

template <typename Mutex>
class file_sink final : public spdlog::sinks::base_sink<Mutex> {
public:
    explicit file_sink(filename_t filename, bool truncate = false, bool zipped = false)
        : file_(filename, truncate, zipped)
    {
    }

    inline const filename_t& filename() const noexcept
    {
        return file_.filename();
    }

protected:
    void sink_it_(const spdlog::details::log_msg& msg) override
    {
        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
        file_.write(formatted.data(), static_cast<std::streamsize>(formatted.size()));
    }

    inline void flush_() override
    {
        file_.flush();
    }

private:
    file_utils::ofile file_;
};

using file_sink_mt = file_sink<std::mutex>;
using file_sink_st = file_sink<spdlog::details::null_mutex>;

// Based on spdlog::sinks::rotating_file_sink
template <typename Mutex>
class rotating_file_sink final : public spdlog::sinks::base_sink<Mutex> {
public:
    explicit rotating_file_sink(filename_t base_filename, std::size_t max_size, std::size_t max_files)
        : base_filename_(std::move(base_filename))
        , max_size_(max_size)
        , max_files_(max_files)
    {
        const auto fname = calc_filename(base_filename_, 0);
        const auto do_rotate = file_utils::exists(fname);
        file_.open(fname);
        if (do_rotate) rotate();
    }

    static filename_t calc_filename(const filename_t& filename, std::size_t index)
    {
        return index == 0 ? filename : fmt::format("{}.{}", filename, index);
    }

    inline const filename_t& filename() const noexcept
    {
        return file_.filename();
    }

protected:
    void sink_it_(const spdlog::details::log_msg& msg) override
    {
        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
        file_.write(formatted.data(), static_cast<std::streamsize>(formatted.size()));
        if (file_.size() > max_size_) rotate();
    }

    inline void flush_() override
    {
        file_.flush();
    }

private:
    void rotate()
    {
        using file_utils::exists;
        using file_utils::rename;
        file_.close();
        for (auto i = max_files_; i > 0; --i) {
            const auto src = calc_filename(base_filename_, i - 1);
            if (!exists(src)) continue;
            const auto target = calc_filename(base_filename_, i);
            rename(src, target);
        }
        file_.reopen(true);
    }

    filename_t base_filename_;
    std::size_t max_size_;
    std::size_t max_files_;
    std::size_t current_size_;
    file_utils::ofile file_;
};

using rotating_file_sink_mt = rotating_file_sink<std::mutex>;
using rotating_file_sink_st = rotating_file_sink<spdlog::details::null_mutex>;

template <typename Factory = spdlog::synchronous_factory>
inline std::shared_ptr<spdlog::logger> file_logger_mt(std::string logger_name, filename_t filename,
                                                      bool truncate = false, bool zipped = false)
{
    return Factory::template create<file_sink_mt>(logger_name, filename, truncate, zipped);
}

template <typename Factory = spdlog::synchronous_factory>
inline std::shared_ptr<spdlog::logger> file_logger_st(std::string logger_name, filename_t filename,
                                                      bool truncate = false, bool zipped = false)
{
    return Factory::template create<file_sink_st>(logger_name, filename, truncate, zipped);
}

template <typename Factory = spdlog::synchronous_factory>
inline std::shared_ptr<spdlog::logger> rotating_logger_mt(std::string logger_name, filename_t filename, size_t max_size,
                                                          size_t max_files)
{
    return Factory::template create<rotating_file_sink_mt>(logger_name, filename, max_size, max_files);
}

template <typename Factory = spdlog::synchronous_factory>
inline std::shared_ptr<spdlog::logger> rotating_logger_st(std::string logger_name, filename_t filename, size_t max_size,
                                                          size_t max_files)
{
    return Factory::template create<rotating_file_sink_st>(logger_name, filename, max_size, max_files);
}
