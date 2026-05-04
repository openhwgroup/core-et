/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <array>
#include <cinttypes>
#include <cctype>

#include <string>
#include <istream>

#include "testLog.h"

#include <spdlog/common.h>

// FUTURE Make this a namespace'd struct, i.e. cosim::config
struct cosim_config {
    // Currently there are two different logger implementations
    // These are utility functions to translate between their respective log levels
    static logLevel get_log_level(spdlog::level::level_enum lvl);
    static spdlog::level::level_enum get_level_enum(logLevel lvl);

    using level_enum = spdlog::level::level_enum;

    // File size in string format, e.g. "50MB"
    struct size_spec {
    public:
        static size_spec from_str(const std::string& s);

        size_spec(size_t value = 0) : value_(value)
        {
        }

        explicit operator size_t() const
        {
            return value_;
        }

        friend std::ostream& operator<<(std::ostream& os, const size_spec& size);

    private:
        size_t value_;
    };

    // Write to a TOML config file
    void write(const std::string& fname) const;

    // Parse from TOML config file
    void parse(const std::string& fname);

    void parse_env(); // Parse environment variables

    // Old interface
    void parse_plusargs();
    void parse_arg_desc();

    // General settings
    uint64_t start_pc{ 0x8000001000 };
    uint64_t spio_start_pc{ 0x0040000000 };
    uint64_t ctrl_start_pc{ 0x8000001000 };
    std::string mem_desc{ "mem_desc.txt" };
    // - limits
    size_t max_commits{ 0 };
    size_t max_errors{ 1 };
    // - log
    level_enum log_level{ spdlog::level::warn };
    std::string log_path{};
    size_spec log_limit{ 0 };
    size_t log_files{ 1 };
    // - misc
    size_t sample_pending{ 45 };
    bool tload_debug{ false };
    bool minion_stats{ true };
    bool event_stats{ false };
    bool sigterm_shutdown{ false };
    std::string dump_config;

    // Checker settings
    struct {
        bool enabled{ false };
        bool bypassed{ false };
        logLevel log_level{ LOG_WARN }; // FUTURE Migrate to spdlog as well?
        std::array<uint64_t, 2> log_window{};
    } checker;

    // Monitor settings
    struct {
        bool enabled{ false };
        std::string trace_path{ "cosim_evt.trace" };
    } monitor;
};
