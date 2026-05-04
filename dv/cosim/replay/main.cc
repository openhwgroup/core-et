/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <unistd.h>
#include <getopt.h>

#include <iostream>
#include <fstream>
#include <string>

#include <cereal/archives/binary.hpp>
#include <spdlog/fmt/fmt.h>

#include "lz4_stream.h"

#include "cosim.h"
#include "events/cosim_evt.h"
#include "events/dispatcher.h"
#include "support/file_utils.h"
#include "coverage/inflight_monitor.h"

#define streq(A, B) !strcmp((A), (B))

static const char* help_msg = "Usage: run-replay [options] <trace>\n\n"
                              "Replay trace recorded from VCS/ZEBU run.\n"
                              "By default this will list all events in the trace.\n"
                              "You can enable checking/coverage with the options below.\n"
                              "\n"
                              "Checker Options:\n"
                              "    -check       Enable checking with cosim\n"
                              "    -info        Set verbosity for CoSim logger\n"
                              "    -debug\n"
                              "\n"
                              "Coverage Options:\n"
                              "    -cov <path>  Enable coverage collection\n"
                              "    -info        Print commits to stdout\n"
                              "    -per_thread  Collect coverage per thread\n"
                              "    -per_minion  Collect coverage per minion (default)\n"
                              "\n"
                              "Other Options:\n"
                              "    -stats    Show event dispatcher stats\n"
                              "    -help     Show this help and exit\n";

struct cmd_options {
    bool check_en;
    bool cov_en;
    bool show_stats;
    spdlog::level::level_enum log_level = spdlog::level::warn;
    const char* trace_file = nullptr;
    std::string cov_out;
    bool cov_per_thread;
};

cmd_options parse_args(int argc, char* const* argv)
{
    cmd_options opts{};
    static const struct option long_options[] = { { "check", no_argument, nullptr, 0 },
                                                  { "stats", no_argument, nullptr, 0 },
                                                  { "cov", required_argument, nullptr, 0 },
                                                  { "per_thread", no_argument, nullptr, 0 },
                                                  { "per_minion", no_argument, nullptr, 0 },
                                                  { "info", no_argument, nullptr, 0 },
                                                  { "debug", no_argument, nullptr, 0 },
                                                  { "help", no_argument, nullptr, 0 },
                                                  { NULL, 0, 0, 0 } };
    int ret, index;
    opterr = 0;

    while ((ret = getopt_long_only(argc, argv, "", long_options, &index)) != -1) {
        if (ret == '?') {
            const auto unknown_opt = argv[optind - 1];
            throw std::runtime_error(fmt::format("Unknown option '{}'", unknown_opt));
        }

        const auto name = long_options[index].name;

        if (streq(name, "check")) {
            opts.check_en = true;
        } else if (streq(name, "stats")) {
            opts.show_stats = true;
        } else if (streq(name, "cov")) {
            opts.cov_en = true;
            opts.cov_out = optarg;
        } else if (streq(name, "per_minion")) {
            opts.cov_per_thread = false;
        } else if (streq(name, "per_thread")) {
            opts.cov_per_thread = true;
        } else if (streq(name, "info")) {
            opts.log_level = spdlog::level::info;
        } else if (streq(name, "debug")) {
            opts.log_level = spdlog::level::debug;
        } else if (streq(name, "help")) {
            printf("%s", help_msg);
            exit(0);
        }
    }

    if (optind == argc) {
        throw std::runtime_error("No argument for trace");
    }

    opts.trace_file = argv[optind++];

    if (optind < argc) {
        throw std::runtime_error("Trailing arguments");
    }

    return opts;
}

struct event_decoder : cosim_evt::handler {
#define DECODE(EVT)                                                                                                    \
    void on(const cosim_evt::EVT##_t* evt)                                                                             \
    {                                                                                                                  \
        std::cout << evt->describe() << std::endl;                                                                     \
    }
    COSIM_EVT_FOREACH(DECODE)
#undef DECODE
};

int main(int argc, char* const* argv) try
{
    auto opts = parse_args(argc, argv);

    std::ifstream ifs(opts.trace_file, std::ios::binary);
    lz4_stream::basic_istream<1024> lz4(ifs);
    cereal::BinaryInputArchive read(lz4);

    cosim_evt::handler* cos = &cosim::getInstance();
    dispatcher* disp = dispatcher::instance();

    event_decoder dec;
    coverage::inflight_monitor cov(opts.cov_per_thread, opts.log_level != spdlog::level::warn);

    spdlog::get("cosim")->set_level(opts.log_level); // Rest is configured in arg_desc.txt..

    if (opts.check_en) {
        if (!file_utils::exists("mem_desc.txt")) std::cerr << "Warning: No mem_desc.txt" << std::endl;
        if (!file_utils::exists("arg_desc.txt")) std::cerr << "Warning: No arg_desc.txt" << std::endl;
    } else {
        disp->remove_handler(cos); // Cosim adds itself so we remove it here
        spdlog::get("cosim")->set_level(spdlog::level::off);
    }
    if (opts.cov_en) disp->add_handler(&cov);
    if (!(opts.cov_en || opts.check_en || opts.show_stats)) disp->add_handler(&dec);

    bool eot = false;
    cosim::getInstance().on_fail([&eot](unsigned, unsigned, int) { eot = true; });

    cosim_evt::event_union evt;
    while (lz4.peek() != EOF && !eot) {
        try
        {
            read(evt);
            disp->dispatch(&evt);
        }
        catch (const cereal::Exception& err)
        {
            std::cerr << "Error: Incomplete event trace" << std::endl;
            eot = true;
        }
    }

    const bool cosim_shows_stats = opts.check_en && cosim::getInstance().config().event_stats;
    if (opts.show_stats && !cosim_shows_stats) {
        disp->write_stats("cosim_evt.stats");
    }
    if (opts.cov_en) cov.write(opts.cov_out);
    if (opts.check_en) cosim::deleteInstance();
    dispatcher::deleteInstance();
}
catch (const std::exception& err)
{
    std::cerr << "Error: " << err.what() << std::endl;
}
