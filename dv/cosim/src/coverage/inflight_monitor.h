/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <deque>
#include <map>
#include <bitset>

#include "spdlog/spdlog.h"

#include "coverage/group.h"
#include "events/cosim_evt.h"
#include "support/stats.h"
#include "support/insn_desc.h"

#include "insn.h"

namespace coverage {

struct inflight_monitor : cosim_evt::handler {

    inflight_monitor(bool per_thread, bool debug)
        : per_thread_(per_thread)
        , debug_(debug)
    {
    }

    void on(const cosim_evt::inst_retire_t* evt);
    // void on(const cosim_evt::inst_exception_t* evt);
    // void on(const cosim_evt::inst_interrupt_t* evt);

    void on(const cosim_evt::flb_update_t* evt);
    void on(const cosim_evt::delayed_write_int_t* evt);
    void on(const cosim_evt::delayed_write_rm_t* evt);
    void on(const cosim_evt::delayed_write_fp_t* evt);
    void on(const cosim_evt::delayed_store_t* evt);
    void on(const cosim_evt::load_miss_bus_err_t* evt);
    // void on(const cosim_evt::update_fcsr_flags_t* evt);
    void on(const cosim_evt::tensorfma_done_t* evt);
    void on(const cosim_evt::tensorquant_done_t* evt);
    void on(const cosim_evt::reduce_done_t* evt);
    void on(const cosim_evt::tensorstore_done_t* evt);

    void on(const cosim_evt::tl_mon_op_start_t* evt);
    void on(const cosim_evt::tl_mon_process_busy_t* evt);
    void on(const cosim_evt::tl_mon_process_req_t* evt);
    void on(const cosim_evt::tl_mon_process_resp_t* evt);
    void on(const cosim_evt::tl_mon_process_tenb_flush_t* evt);

    // void on(const cosim_evt::esr_write_t* evt); // to match esr writes?

    void write(const std::string& path);
    void write(std::ostream& ofs);

    struct info_t {
        struct {
            bool w_int : 1;
            bool w_fp : 1;
            bool w_mask : 1;
            bool store : 1;
            bool load : 1;
            bool flb : 1;
            bool tensor_fma : 1;
            bool tensor_quant : 1;
            bool tensor_reduce : 1;
            bool tensor_store : 1;
            bool tensor_load : 1;
            bool tensor_load_tenb : 1;
        } pending;

        uint64_t cyc[2];
        hart_id hart;
        uint64_t pc;
        insn_desc desc;
        bemu::Instruction insn;
        group grp;

        uint8_t thread_id;

        unsigned store_id;
        uint32_t tl_id;

        bool done() const
        {
            return !(pending.w_int || pending.w_fp || pending.w_mask || pending.store || pending.flb
                     || pending.tensor_fma || pending.tensor_quant || pending.tensor_reduce || pending.tensor_store
                     || pending.tensor_load);
        }
    };

private:
    struct tensor_load_t {
        uint32_t id;
        uint32_t minion_id;
        uint8_t module;
        uint8_t state;
        uint8_t req[4];
        bool is_trans;
        uint32_t n_req;
        uint32_t n_resp;
        bool tenb_flush;
        insn_desc desc;
    };

    using tl_list = std::list<tensor_load_t>;
    void advance(uint64_t cyc, tl_list::iterator tl);

    using queue_t = std::deque<info_t>;

    template <typename Evt>
    hart_id get_hart(const Evt* evt)
    {
        return hart_id(evt->minion_id << 1 | evt->thread_id);
    }

    template <typename Evt>
    hart_id get_hart0(const Evt* evt)
    {
        return hart_id(evt->minion_id << 1);
    }

    template <typename Evt>
    queue_t& get_queue(const Evt* evt)
    {
        const auto hart = per_thread_ ? get_hart(evt) : get_hart0(evt);
        return qs_[hart];
    }

    template <typename Evt>
    queue_t& get_queue0(const Evt* evt)
    {
        return qs_[get_hart0(evt)];
    }

    void commit(uint64_t cyc, queue_t& q, queue_t::iterator it);

    bool per_thread_ = false;
    bool debug_ = false;

    std::map<hart_id, queue_t> qs_;

    using group_stats_t = std::array<stats_t, coverage::num_groups>;

    group_stats_t cycles_{};
    group_stats_t qlen_{};

    using ulong_t = unsigned long long;

    using cover_t = std::bitset<coverage::num_groups>;
    using row_t = std::array<ulong_t, coverage::num_groups>;

    std::array<row_t, coverage::num_groups> cross_{};
    row_t commits_{};
    std::shared_ptr<spdlog::logger> log_;

    tl_list open_tls_;

    uint32_t next_tl_id_ = 0;
};

} // namespace coverage
