/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "inflight_monitor.h"

#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/fmt/ostr.h"
#include "support/file_utils.h"
#include "checker/riscv_disasm.h"

using namespace coverage;

using int_type = typename std::underlying_type<coverage::group>::type;

std::ostream& operator<<(std::ostream& os, const inflight_monitor::info_t& info)
{
    os << info.cyc[0] << "\t"; //
    if (info.cyc[1]) {
        os << info.cyc[1] << "\t";
    } else {
        os << "pending\t";
    }
    os << info.hart << "\t"                              //
       << info.desc << "\t"                              //
       << "[" << coverage::group_name(info.grp) << "]\t" //
       << riscv_disasm(info.insn.bits);
    return os;
}

void inflight_monitor::on(const cosim_evt::inst_retire_t* evt)
{
    info_t info{};
    info.cyc[0] = evt->cycle;
    info.hart = get_hart(evt);
    info.pc = evt->pc;
    info.desc = insn_desc(get_hart(evt), evt->pc);
    info.insn = bemu::Instruction{ evt->inst_bits, 0 };
    info.grp = coverage::get_group(evt->inst_bits, evt->store_addr);
    info.thread_id = evt->thread_id;

    info.pending.w_int = evt->delayed_write && evt->wbx;
    info.pending.w_fp = evt->delayed_write && evt->wbf;
    info.pending.w_mask = evt->delayed_write && (evt->wbm || evt->wbmall);
    info.pending.store = evt->store && evt->delayed_store;
    info.store_id = evt->store_age_id;
    info.pending.load = evt->load_miss;
    info.pending.flb = evt->flb_valid;
    info.pending.tensor_fma = evt->tensor_fma;
    info.pending.tensor_quant = evt->tensor_quant;
    info.pending.tensor_reduce = evt->reduce;
    info.pending.tensor_store = evt->tensor_store;
    info.pending.tensor_load = evt->tensor_load;
    info.pending.tensor_load_tenb = evt->tenb;

    auto& q = get_queue(evt);
    q.push_back(info);

    // std::cout << "+\t" << info << "\n";

    if (info.done()) commit(evt->cycle, q, std::prev(q.end()));
}

void inflight_monitor::on(const cosim_evt::flb_update_t* evt)
{
    auto& q = get_queue(evt);
    const auto pending_flb = [evt](const info_t& in) {
        return in.thread_id == evt->thread_id && in.pending.flb && in.insn.rd() == evt->wbx_rd;
    };
    auto it = std::find_if(q.begin(), q.end(), pending_flb);
    assert(it != q.end());

    it->pending.w_int = false;
    it->pending.flb = false;
    assert(it->done());

    commit(evt->cycle, q, it);
}

void inflight_monitor::on(const cosim_evt::delayed_write_int_t* evt)
{
    auto& q = get_queue(evt);
    const auto pending_wint = [evt](const info_t& in) {
        return in.thread_id == evt->thread_id && in.pending.w_int && in.insn.rd() == evt->wbx_rd;
    };
    auto it = std::find_if(q.begin(), q.end(), pending_wint);
    assert(it != q.end());

    it->pending.w_int = false;
    it->pending.load = false; // in case of load
    assert(it->done());

    commit(evt->cycle, q, it);
}

void inflight_monitor::on(const cosim_evt::delayed_write_rm_t* evt)
{
    auto& q = get_queue(evt);
    const auto pending_wmask = [evt](const info_t& in) {
        return in.thread_id == evt->thread_id && in.pending.w_mask && in.insn.md() == evt->wbm_rd;
    };
    auto it = std::find_if(q.begin(), q.end(), pending_wmask);
    assert(it != q.end());

    it->pending.w_mask = false;
    assert(it->done());

    commit(evt->cycle, q, it);
}

void inflight_monitor::on(const cosim_evt::delayed_write_fp_t* evt)
{
    auto& q = get_queue(evt);
    const auto pending_wfp = [evt](const info_t& in) {
        return in.thread_id == evt->thread_id && in.pending.w_fp && in.insn.fd() == evt->wbf_rd;
    };
    auto it = std::find_if(q.begin(), q.end(), pending_wfp);
    assert(it != q.end());

    it->pending.w_fp = false;
    assert(it->done());

    if (it->done()) commit(evt->cycle, q, it);
}

void inflight_monitor::on(const cosim_evt::delayed_store_t* evt)
{
    auto& q = get_queue(evt);
    const auto pending_store = [evt](const info_t& in) {
        return in.thread_id == evt->thread_id && in.pending.store && in.store_id == evt->store_age_id;
    };
    auto it = std::find_if(q.begin(), q.end(), pending_store);
    assert(it != q.end());

    it->pending.store = false;
    assert(it->done());
}

void inflight_monitor::on(const cosim_evt::load_miss_bus_err_t* evt)
{
    auto& q = get_queue(evt);
    const auto pending_load
        = [evt](const info_t& in) { return in.thread_id == evt->thread_id && in.pending.load && in.pc == evt->pc; };
    auto it = std::find_if(q.begin(), q.end(), pending_load);
    assert(it != q.end());

    it->pending.load = false;
    it->pending.w_int = false; // pending load implies w_int
    assert(it->done());

    commit(evt->cycle, q, it);
}

void inflight_monitor::on(const cosim_evt::tensorfma_done_t* evt)
{
    auto& q = get_queue0(evt);
    const auto pending_tfma = [evt](const info_t& in) { return in.thread_id == 0 && in.pending.tensor_fma; };
    auto it = std::find_if(q.begin(), q.end(), pending_tfma);
    assert(it != q.end());

    it->pending.tensor_fma = false;
    assert(it->done());

    commit(evt->cycle, q, it);
}

void inflight_monitor::on(const cosim_evt::tensorquant_done_t* evt)
{
    auto& q = get_queue0(evt);
    const auto pending_tquant = [evt](const info_t& in) { return in.thread_id == 0 && in.pending.tensor_quant; };
    auto it = std::find_if(q.begin(), q.end(), pending_tquant);
    assert(it != q.end());

    it->pending.tensor_quant = false;
    assert(it->done());

    commit(evt->cycle, q, it);
}

void inflight_monitor::on(const cosim_evt::reduce_done_t* evt)
{
    auto& q = get_queue0(evt);
    const auto pending_treduce = [evt](const info_t& in) { return in.thread_id == 0 && in.pending.tensor_reduce; };
    auto it = std::find_if(q.begin(), q.end(), pending_treduce);
    assert(it != q.end());

    it->pending.tensor_reduce = false;
    assert(it->done());

    commit(evt->cycle, q, it);
}

void inflight_monitor::on(const cosim_evt::tensorstore_done_t* evt)
{
    auto& q = get_queue0(evt);
    const auto pending_tstore = [evt](const info_t& in) { return in.thread_id == 0 && in.pending.tensor_store; };
    auto it = std::find_if(q.begin(), q.end(), pending_tstore);
    assert(it != q.end());

    it->pending.tensor_store = false;
    assert(it->done());

    commit(evt->cycle, q, it);
}

void inflight_monitor::on(const cosim_evt::tl_mon_op_start_t* evt)
{
    auto& q = get_queue0(evt);
    const auto pending_tload = [evt](const info_t& in) {
        return !in.tl_id && in.pending.tensor_load && in.pending.tensor_load_tenb == evt->tl_module;
    };
    auto it = std::find_if(q.begin(), q.end(), pending_tload);
    assert(it != q.end());

    tensor_load_t tl{};
    tl.id = ++next_tl_id_;
    tl.minion_id = evt->minion_id;
    tl.module = evt->tl_module;
    tl.state = 1;
    tl.is_trans = tl.module ? 0 : evt->transform;

    tl.desc = it->desc;
    it->tl_id = tl.id;

    open_tls_.push_back(tl);
}

void inflight_monitor::on(const cosim_evt::tl_mon_process_tenb_flush_t* evt)
{
    const auto matches_evt
        = [evt](const tensor_load_t& tl) { return tl.minion_id == evt->minion_id && tl.module == 1; };
    auto tl = std::find_if(open_tls_.begin(), open_tls_.end(), matches_evt);
    if (tl == open_tls_.end()) return;

    tl->tenb_flush = true;
    advance(evt->cycle, tl);
}

void inflight_monitor::on(const cosim_evt::tl_mon_process_busy_t* evt)
{
    const auto matches_evt = [evt](const tensor_load_t& tl) {
        return tl.minion_id == evt->minion_id && tl.module == evt->tl_module && tl.state == evt->state;
    };
    auto tl = std::find_if(open_tls_.begin(), open_tls_.end(), matches_evt);
    if (tl == open_tls_.end()) return;

    tl->state++;
    advance(evt->cycle, tl);
}

void inflight_monitor::on(const cosim_evt::tl_mon_process_req_t* evt)
{
    const auto matches_evt = [evt](const tensor_load_t& tl) {
        return tl.minion_id == evt->minion_id && tl.module == evt->tl_module && tl.state == 2;
    };
    auto tl = std::find_if(open_tls_.begin(), open_tls_.end(), matches_evt);
    assert(tl != open_tls_.end());

    tl->req[evt->req_id_idx] = 2;
    tl->n_req += 2;
}

void inflight_monitor::on(const cosim_evt::tl_mon_process_resp_t* evt)
{
    const auto matches_evt = [evt](const tensor_load_t& tl) {
        return tl.minion_id == evt->minion_id && tl.module == evt->tl_module && (tl.state == 2 || tl.state == 3)
               && tl.req[evt->resp_id_idx] > 0;
    };
    auto tl = std::find_if(open_tls_.begin(), open_tls_.end(), matches_evt);
    assert(tl != open_tls_.end());

    tl->req[evt->resp_id_idx] -= 1;
    tl->n_resp += 1;
    advance(evt->cycle, tl);
}

void inflight_monitor::advance(uint64_t cyc, tl_list::iterator tl)
{

    const bool is_done = (tl->module == 1 && tl->tenb_flush) || (tl->state == 3 && tl->n_resp == tl->n_req);
    if (!is_done) return;

    auto& q = get_queue0(&*tl);
    const auto matches_tl = [&tl](const info_t& in) { return in.tl_id == tl->id; };

    auto ins = std::find_if(q.begin(), q.end(), matches_tl);
    assert(ins != q.end());

    ins->pending.tensor_load = false;
    ins->pending.tensor_load_tenb = false;

    open_tls_.erase(tl);
    commit(cyc, q, ins);
}

void inflight_monitor::commit(uint64_t cyc, queue_t& q, queue_t::iterator it)
{
    assert(it->done());

    it->cyc[1] = cyc;

    cover_t cover{};

    if (debug_) {
        std::cout << *it << "\n";
    }

    const auto update_cover = [&cover](const info_t& in) { cover.set(static_cast<int_type>(in.grp)); };
    std::for_each(std::next(it), q.end(), update_cover);

    const auto me = static_cast<int_type>(it->grp);

    auto& row = cross_[me];
    for (int_type g = 0; g < coverage::num_groups; ++g) {
        row[g] += cover[g];
    }
    commits_[me] += 1;
    cycles_[me].sample(cyc - it->cyc[0]);
    qlen_[me].sample(q.end() - std::next(it));

    // Drain queue from the front
    const auto open = std::find_if(q.begin(), q.end(), [](const info_t& in) { return !in.done(); });
    q.erase(q.begin(), open);
}

void inflight_monitor::write(const std::string& fname)
{
    using namespace file_utils;
    create_dir(dirname(fname));
    std::ofstream ofs(fname);
    write(ofs);
}

void inflight_monitor::write(std::ostream& os)
{

    for (int_type g = 0; g < coverage::num_groups; ++g) {
        const auto name = coverage::group_name(static_cast<coverage::group>(g));
        const auto& cross = cross_[g];
        const auto commits = commits_[g];
        const auto& cycles = cycles_[g];
        const auto& qlen = qlen_[g];

        os << "group " << name << "\n"
           << "commits " << commits << "\n"
           << "exec_time " << cycles.mean() << " " << cycles.stdev() << " " << cycles.min() << " " << cycles.max()
           << " " << cycles.sum() << "\n"
           << "queue_size " << qlen.mean() << " " << qlen.stdev() << " " << qlen.min() << " " << qlen.max() << " "
           << qlen.sum() << "\n";

        for (int_type g1 = 0; g1 < coverage::num_groups; ++g1) {
            const auto name1 = coverage::group_name(static_cast<coverage::group>(g1));
            os << "cross " << name1 << " " << cross[g1] << "\n";
        }

        os << std::endl;
    }
}
