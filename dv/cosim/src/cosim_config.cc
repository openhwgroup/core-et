/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "cosim_config.h"

#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <ios>
#include <unordered_map>

#include <spdlog/fmt/fmt.h>
#include <toml.hpp>

#include "testBase.h"

#include "support/file_utils.h"

extern const char cosim_toml[]; // see `docs/cosim.toml`

using size_spec = cosim_config::size_spec;

spdlog::level::level_enum level_from_str(std::string s)
{
    // Transform to lowercase
    std::transform(s.begin(), s.end(), s.begin(), [](char c) { return std::tolower(c); });
    // Remove uvm prefix
    const auto prefix = s.substr(0, std::min(s.size(), sizeof("uvm_")));
    if (prefix == "uvm_") s = s.substr(sizeof("uvm_"));
    // Support non-spdlog level specs (i.e. plusargs)
    if (s == "none" || s == "quiet") return spdlog::level::off;
    if (s == "2" || s == "low") return spdlog::level::warn;
    if (s == "1" || s == "med" || s == "medium") return spdlog::level::info;
    if (s == "0" || s == "high") return spdlog::level::debug;
    if (s == "full") return spdlog::level::trace;
    // Fallback to spdlog
    return spdlog::level::from_str(s);
}

logLevel cosim_config::get_log_level(spdlog::level::level_enum lvl)
{
    if (lvl <= spdlog::level::debug) return LOG_DEBUG;
    if (lvl <= spdlog::level::info) return LOG_INFO;
    if (lvl <= spdlog::level::warn) return LOG_WARN;
    return LOG_ERR;
}

spdlog::level::level_enum cosim_config::get_level_enum(logLevel lvl)
{
    if (lvl >= LOG_DEBUG) return spdlog::level::debug;
    if (lvl >= LOG_INFO) return spdlog::level::info;
    if (lvl >= LOG_WARN) return spdlog::level::warn;
    return spdlog::level::err;
}

// Parse extra datatypes from TOML
//  - spdlog::level::level_enum
//  - size_spec
namespace toml {
template <>
struct from<spdlog::level::level_enum> {
    static spdlog::level::level_enum from_toml(const value& v)
    {
        const std::string& s = toml::get<std::string>(v);
        return level_from_str(s); // spdlog::level::from_str
    }
};

template <>
struct from<size_spec> {
    static size_spec from_toml(const value& v)
    {
        if (v.is_integer()) {
            return size_spec{ static_cast<size_t>(toml::get<toml::integer>(v)) };
        }
        if (v.is_string()) {
            return size_spec::from_str(toml::get<std::string>(v));
        }
        return size_spec{ 0 };
    }
};
} // namespace toml

std::ostream& operator<<(std::ostream& os, const size_spec& size)
{
    const size_t value = static_cast<size_t>(size);
    if (value < (1u << 10)) return os << value;
    if (value < (1u << 20)) return os << fmt::format("{:.2f}K", (value + 0.0) / (1 << 10));
    if (value < (1u << 30)) return os << fmt::format("{:.2f}M", (value + 0.0) / (1 << 20));
    return os << fmt::format("{:.2f}G", (value + 0.0) / (1 << 30));
}

// size_spec is a floating point number followed by an optional suffix (K/M/G)
size_spec size_spec::from_str(const std::string& s)
{
    size_t parsed;
    const double base = std::stof(s, &parsed);
    const std::string suffix = s.substr(parsed);
    if (suffix.empty()) return size_spec{ static_cast<size_t>(base) };
    if (suffix[0] == 'K') return size_spec{ static_cast<size_t>(base * (1u << 10)) };
    if (suffix[0] == 'M') return size_spec{ static_cast<size_t>(base * (1u << 20)) };
    if (suffix[0] == 'G') return size_spec{ static_cast<size_t>(base * (1u << 30)) };
    return size_spec{ static_cast<size_t>(base) };
}

void cosim_config::write(const std::string& fname) const
{
    std::ofstream ofs(fname);

#define print_opt(section, name) ofs << #name " = " << section name << "\n"

    ofs << "# CoSim Configuration\n";
    ofs << std::hex << std::showbase;
    print_opt(, start_pc);
    print_opt(, spio_start_pc);
    print_opt(, ctrl_start_pc);
    ofs << std::dec << std::noshowbase;
    print_opt(, mem_desc);
    print_opt(, max_commits);
    print_opt(, max_errors);
    print_opt(, log_level);
    print_opt(, log_path);
    print_opt(, log_limit);
    print_opt(, sample_pending);
    print_opt(, tload_debug);
    print_opt(, minion_stats);
    print_opt(, event_stats);
    print_opt(, sigterm_shutdown);
    print_opt(, dump_config);

    ofs << "\n[checker]\n";
    print_opt(checker., enabled);
    print_opt(checker., bypassed);
    print_opt(checker., log_level);
    ofs << "log_window = [" << checker.log_window[0] << ", " << checker.log_window[1] << "]\n";

    ofs << "\n[monitor]\n";
    print_opt(monitor., enabled);
    print_opt(monitor., trace_path);

#undef print_opt
}

void cosim_config::parse(const std::string& fname)
{
    const auto data = toml::parse(fname);
    const auto& tab = toml::get<toml::table>(data);

    // General
    start_pc = toml::find_or(data, "start_pc", start_pc);
    spio_start_pc = toml::find_or(data, "spio_start_pc", spio_start_pc);
    ctrl_start_pc = toml::find_or(data, "ctrl_start_pc", ctrl_start_pc);
    mem_desc = toml::find_or(data, "mem_desc", mem_desc);
    max_commits = toml::find_or(data, "max_commits", max_commits);
    max_errors = toml::find_or(data, "max_errors", max_errors);
    if (tab.count("log_level")) { // Set both log_level and checker.log_level
        log_level = toml::find<level_enum>(data, "log_level");
        checker.log_level = get_log_level(log_level);
    }
    log_path = toml::find_or(data, "log_path", log_path);
    log_limit = toml::find_or(data, "log_limit", log_limit);
    log_files = toml::find_or(data, "log_files", log_files);
    sample_pending = toml::find_or(data, "sample_pending", sample_pending);
    tload_debug = toml::find_or(data, "tload_debug", tload_debug);
    minion_stats = toml::find_or(data, "minion_stats", minion_stats);
    event_stats = toml::find_or(data, "event_stats", event_stats);
    sigterm_shutdown = toml::find_or(data, "sigterm_shutdown", sigterm_shutdown);
    dump_config = toml::find_or(data, "dump_config", dump_config);

    // Checker
    if (tab.count("checker")) {
        const auto& sect = toml::find(data, "checker");
        checker.enabled = toml::find_or(sect, "enabled", checker.enabled);
        checker.bypassed = toml::find_or(sect, "bypassed", checker.bypassed);
        if (sect.as_table().count("log_level")) {
            const auto lvl = toml::find<level_enum>(sect, "log_level");
            checker.log_level = get_log_level(lvl);
        }
        if (sect.as_table().count("log_window")) {
            const auto& val = toml::find(sect, "log_window");
            if (val.is_integer()) {
                checker.log_window[0] = val.as_integer();
            }
            else if (val.is_array()) {
                const auto win = toml::get<std::vector<uint64_t> >(val);
                for (size_t i = 0; i < checker.log_window.size() && i < win.size(); ++i) {
                    checker.log_window[i] = win[i];
                }
            }
        }
    }

    // Monitor
    if (tab.count("monitor")) {
        const auto& sect = toml::find(data, "monitor");
        monitor.enabled = toml::find_or(sect, "enabled", monitor.enabled);
        monitor.trace_path = toml::find_or(sect, "trace_path", monitor.trace_path);
    }
}

template <typename T>
bool env_variable(const char* key, const char* fmt, T* v)
{
    const char* env = std::getenv(key);
    if (!env) return false;
    return sscanf(env, fmt, v) > 0;
}

void cosim_config::parse_env()
{
    // Local variables to hold temporary option values
    char buf[1024];

    // General
    if (env_variable("COSIM_DEFAULT_LOG_LEVEL", "%s", &buf)) {
        log_level = level_from_str(buf);
        checker.log_level = get_log_level(log_level);
    }
    env_variable("COSIM_START_PC", "%llx", &start_pc);
    env_variable("COSIM_CONTROL_START_PC", "%llx", &ctrl_start_pc);
    env_variable("COSIM_SPIO_START_PC", "%llx", &spio_start_pc);
    env_variable("COSIM_MEM_DESC", "%s", &mem_desc);
    env_variable("COSIM_MAX_COMMITS", "%u", &max_commits);
    env_variable("COSIM_MAX_NUM_ERR", "%u", &max_errors);
    if (env_variable("COSIM_LOG_LEVEL", "%s", &buf)) {
        log_level = level_from_str(buf);
    }
    if (env_variable("COSIM_LOG_PATH", "%s", &buf)) log_path = buf;
    if (env_variable("COSIM_LOG_LIMIT", "%s", &buf)) {
        log_limit = size_spec::from_str(buf);
    }
    env_variable("COSIM_LOG_FILES", "%lu", &log_files);
    env_variable("COSIM_MAX_NUM_PENDING_INSTR", "%d", &sample_pending);
    env_variable("COSIM_TENSOR_LOAD_DBG", "%i", &tload_debug);
    env_variable("COSIM_SIGTERM", "%i", &sigterm_shutdown);
    if (env_variable("COSIM_DUMP_CONFIG", "%s", &buf)) dump_config = buf;

    // Checker
    env_variable("COSIM_CHECKER", "%i", &checker.enabled);
    env_variable("COSIM_CHECKER_BYPASSED", "%i", &checker.bypassed);
    if (env_variable("COSIM_EMU_LOG_LEVEL", "%s", &buf)) {
        checker.log_level = get_log_level(level_from_str(buf));
    }
    env_variable("COSIM_EMU_LOG_LEVEL_START_CYCLE", "%i", &checker.log_window[0]);
    env_variable("COSIM_EMU_LOG_LEVEL_END_CYCLE", "%i", &checker.log_window[1]);

#ifndef COSIM_REPLAY
    // Monitor
    if (env_variable("COSIM_RECORD", "%s", buf)) { // For backward-compatibility
        monitor.enabled = true;
        monitor.trace_path = buf;
    }
#endif // COSIM_REPLAY
    env_variable("COSIM_MONITOR", "%i", &monitor.enabled);
    if (env_variable("COSIM_MONITOR_TRACE", "%s", buf)) monitor.trace_path = buf;
}

void cosim_config::parse_plusargs()
{
#ifndef ZEBU_DPI

    // Local variables to hold temporary option values
    char buf[1024];

    // General
    if (valuePlusArgs("DEFAULT_LOG_LEVEL", "%s", &buf) || valuePlusArgs("verbosity", "%s", &buf)) {
        log_level = level_from_str(buf);
        checker.log_level = get_log_level(log_level);
    }
    valuePlusArgs("START_PC", "%llx", &start_pc);
    valuePlusArgs("CONTROL_START_PC", "%llx", &ctrl_start_pc);
    valuePlusArgs("SPIO_START_PC", "%llx", &spio_start_pc);
    valuePlusArgs("MEM_DESC", "%s", &mem_desc);
    valuePlusArgs("MAX_COMMITS", "%u", &max_commits);
    valuePlusArgs("COSIM_MAX_NUM_ERR", "%u", &max_errors);
    if (valuePlusArgs("COSIM_LOG_LEVEL", "%s", &buf)) {
        log_level = level_from_str(buf);
    }
    if (valuePlusArgs("CoSIM_LOG_LEVEL", "%s", &buf)) { // backward-compatibility
        log_level = level_from_str(buf);
    }
    if (valuePlusArgs("COSIM_LOG_PATH", "%s", &buf)) log_path = buf;
    if (valuePlusArgs("COSIM_LOG_LIMIT", "%s", &buf)) {
        log_limit = size_spec::from_str(buf);
    }
    valuePlusArgs("COSIM_LOG_FILES", "%lu", &log_files);
    valuePlusArgs("MAX_NUM_PENDING_INSTR", "%d", &sample_pending);
    valuePlusArgs("COSIM_TENSOR_LOAD_DBG", "%i", &tload_debug);
    valuePlusArgs("COSIM_SIGTERM", "%i", &sigterm_shutdown);
    if (valuePlusArgs("COSIM_DUMP_CONFIG", "%s", &buf)) dump_config = buf;

    // Checker
    valuePlusArgs("CHECKER", "%i", &checker.enabled);
    valuePlusArgs("CHECKER_BYPASSED", "%i", &checker.bypassed);
    if (valuePlusArgs("EMU_LOG_LEVEL", "%s", &buf)) {
        checker.log_level = get_log_level(level_from_str(buf));
    }
    valuePlusArgs("EMU_LOG_LEVEL_START_CYCLE", "%i", &checker.log_window[0]);
    valuePlusArgs("EMU_LOG_LEVEL_END_CYCLE", "%i", &checker.log_window[1]);

#ifndef COSIM_REPLAY
    // Monitor
    if (valuePlusArgs("RECORD", "%s", buf)) { // For backward-compatibility
        monitor.enabled = true;
        monitor.trace_path = buf;
    }
#endif // COSIM_REPLAY
    valuePlusArgs("MONITOR", "%i", &monitor.enabled);
    if (valuePlusArgs("MONITOR_TRACE", "%s", buf)) monitor.trace_path = buf;

#endif // ZEBU_DPI
}

void cosim_config::parse_arg_desc()
{
    char fname[2048];
    strncpy(fname, "arg_desc.txt", sizeof(fname));

#ifndef ZEBU_DPI
    valuePlusArgs("ARG_DESC", "%s", fname);
#endif // ZEBU_DPI

    FILE* fp = fopen(fname, "r");
    if (fp == nullptr) return;

    char* line = nullptr;
    size_t len = 0;

    char buf[1024];

    // NOTE This is a hack to maintain backwards compatibility
    // When we run with ZEBU we assume that the checker is enabled
    // We only really parse arg_desc.txt when running with ZEBU
    checker.enabled = true;

    while (getline(&line, &len, fp) >= 0) {
        // General
        if (sscanf(line, "Compute Shire Start PC: 40'h%lX", &start_pc)) continue;
        if (sscanf(line, "Master Shire Start PC: 40'h%lX", &ctrl_start_pc)) continue;
        if (sscanf(line, "Service Processor Start PC: 40'h%lX", &spio_start_pc)) continue;
        if (sscanf(line, "MEM_DESC: %s", buf)) {
            mem_desc = buf;
            continue;
        }
        if (sscanf(line, "COSIM_MAX_NUM_ERR: %lu", &max_errors)) continue;
        if (sscanf(line, "MAX_COMMITS: %lu", &max_commits)) continue;
        if (sscanf(line, "COSIM_LOG_LEVEL: %s", buf)) {
            log_level = level_from_str(buf);
            continue;
        }
        if (sscanf(line, "COSIM_LOG_PATH: %s", buf)) {
            log_path = buf;
            continue;
        }
        if (sscanf(line, "COSIM_LOG_LIMIT: %s", buf)) {
            log_limit = size_spec::from_str(buf);
            continue;
        }
        if (sscanf(line, "COSIM_LOG_FILES: %lu", &log_files)) continue;
        if (sscanf(line, "MAX_NUM_PENDING_INSTR: %lu", &sample_pending)) continue;
        if (sscanf(line, "COSIM_TENSOR_LOAD_DBG: %d", (int*)&tload_debug)) continue;
        if (sscanf(line, "COSIM_SIGTERM: %d", (int*)&sigterm_shutdown)) continue;
        if (sscanf(line, "DUMP_CONFIG: %s", buf)) {
            dump_config = buf;
            continue;
        }

        // Checker
        if (sscanf(line, "CHECKER: %d", (int*)&checker.enabled)) continue;
        if (sscanf(line, "EMU_LOG_LEVEL: %s", buf)) {
            checker.log_level = get_log_level(level_from_str(buf));
            continue;
        }
        if (sscanf(line, "CHECKER_BYPASSED: %d", (int*)&checker.bypassed)) continue;

#ifndef COSIM_REPLAY
        // Monitor
        if (sscanf(line, "RECORD: %s", buf)) { // For backward-compatibility
            monitor.enabled = true;
            monitor.trace_path = buf;
        }
#endif // COSIM_REPLAY
        if (sscanf(line, "MONITOR: %d", (int*)&monitor.enabled)) continue;
        if (sscanf(line, "MONITOR_TRACE: %s", buf)) {
            monitor.trace_path = buf;
            continue;
        }
    }

    if (line) free(line);
    fclose(fp);
}
