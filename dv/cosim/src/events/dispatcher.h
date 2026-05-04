/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_

#include <chrono>
#include <limits>
#include <map>
#include <memory>
#include <unordered_map>

#include <spdlog/spdlog.h>

#include "cosim_evt.h"

struct dispatcher {
    static dispatcher* instance();
    static void deleteInstance();

    using duration_type = std::chrono::duration<unsigned long long, std::nano>;
    using handler_type = cosim_evt::handler;

    template <typename EventT>
    void dispatch(const EventT* evt)
    {
        const auto start = std::chrono::steady_clock::now();
        for (auto handler : handlers_) {
            handler->on(evt);
        }
        const auto end = std::chrono::steady_clock::now();
        const auto delta_time = std::chrono::duration_cast<duration_type>(end - start);
        update_stats(evt->event_id(), delta_time.count());
    }

    void dispatch(const cosim_evt::event_union* evt);

    void add_handler(handler_type* handler);
    void remove_handler(handler_type* handler);

    void write_stats(const std::string& path);

private:
    template <typename T>
    using event_map = std::unordered_map<cosim_evt::event_id, T, cosim_evt::hash_id>;

    void update_stats(cosim_evt::event_id evt_id, unsigned long long dt);

    std::vector<handler_type*> handlers_;

    struct stats {
        unsigned long count;
        double mean;
        double stdev;
        unsigned long long min = std::numeric_limits<unsigned long long>::max();
        unsigned long long max;
        unsigned long long total;
        double M2; // for precision
    };

    event_map<stats> stats_;

    dispatcher();
    ~dispatcher();
};

#endif // _DISPATCHER_H_
