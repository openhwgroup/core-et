/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "dispatcher.h"

#include <cmath>
#include <fstream>

#include <spdlog/fmt/bundled/printf.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "recorder.h"

#include "support/file_utils.h"

static dispatcher* dispatcher_inst = NULL;

dispatcher* dispatcher::instance()
{
    if (dispatcher_inst == NULL) {
        dispatcher_inst = new dispatcher;
    }
    return dispatcher_inst;
}

void dispatcher::deleteInstance()
{
    if (dispatcher_inst != NULL) {
        delete dispatcher_inst;
    }
}

dispatcher::dispatcher()
{
}

dispatcher::~dispatcher()
{
    dispatcher_inst = NULL;
}

void dispatcher::add_handler(handler_type* handler)
{
    const auto it = std::find(handlers_.begin(), handlers_.end(), handler);
    if (it == handlers_.end()) handlers_.push_back(handler);
}

void dispatcher::remove_handler(handler_type* handler)
{
    const auto it = std::find(handlers_.begin(), handlers_.end(), handler);
    handlers_.erase(it);
}

void dispatcher::dispatch(const cosim_evt::event_union* evt)
{
#define DISPATCH(EVT)                                                                                                  \
    case cosim_evt::event_id::EVT:                                                                                     \
        return dispatch(evt->get<cosim_evt::EVT##_t>());
    switch (evt->id) {
        COSIM_EVT_FOREACH(DISPATCH)
    default:
        assert(0 && "Invalid event");
    }

#undef DISPATCH
}

void dispatcher::update_stats(cosim_evt::event_id evt_id, unsigned long long dt)
{
    auto& s = stats_[evt_id];
    s.count += 1;
    s.total += dt;
    const auto delta = dt - s.mean;
    s.mean += delta / s.count;
    const auto delta2 = dt - s.mean;
    s.M2 += delta * delta2;
    s.min = std::min(dt, s.min);
    s.max = std::max(dt, s.max);
}

void dispatcher::write_stats(const std::string& path)
{
    file_utils::create_dir(file_utils::dirname(path));
    std::ofstream ofs(path);
    fmt::print(ofs, "event\tcount\tmean\tstdev\tmin\tmax\ttotal\n");
    for (auto e : cosim_evt::all_events) {
        const auto& s = stats_[e];
        const auto& name = cosim_evt::event_name(e);
        switch (s.count) {
        case 0: {
            fmt::print(ofs, "{}\t0\tNA\tNA\tNA\tNA\t0\n", name);
            break;
        }
        case 1: {
            fmt::print(ofs, "{}\t1\t{}\tNA\t{}\t{}\t{}\n", name, s.mean, s.min, s.max, s.total);
            break;
        }
        default: {
            const auto stdev = std::sqrt(s.M2 / s.count);
            fmt::print(ofs, "{}\t{}\t{}\t{}\t{}\t{}\t{}\n", name, s.count, s.mean, stdev, s.min, s.max, s.total);
            break;
        }
        }
    }
}
