// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: minion_tlb vs tlb_top.

#include "Vcosim_minion_tlb_tb.h"
#include "cosim_ctrl.h"

#include <cstdlib>
#include <cstdint>

using DUT = Vcosim_minion_tlb_tb;

namespace {

constexpr uint8_t kModeBare = 0;
constexpr uint8_t kModeSv39 = 8;
constexpr uint8_t kModeSv48 = 9;
constexpr uint8_t kPrvU = 0;
constexpr uint8_t kPrvS = 1;
constexpr uint8_t kPrvM = 3;
constexpr uint8_t kPage4K = 0;
constexpr uint8_t kPage2M = 2;
constexpr uint8_t kPage1G = 3;

struct BoolCoverage {
    bool seen0 = false;
    bool seen1 = false;

    void sample(bool value) {
        if (value) seen1 = true;
        else seen0 = true;
    }

    bool full() const {
        return seen0 && seen1;
    }
};

struct ValueCoverage {
    bool seen_zero = false;
    bool seen_nonzero = false;

    template <typename T>
    void sample(T value) {
        if (value == 0) seen_zero = true;
        else seen_nonzero = true;
    }

    bool full() const {
        return seen_zero && seen_nonzero;
    }
};

struct Coverage {
    BoolCoverage dft_scanmode;
    BoolCoverage coop_mode;
    BoolCoverage req_mprv;
    BoolCoverage req_sum;
    BoolCoverage req_mxr;
    BoolCoverage req_debug;
    BoolCoverage req_passthrough;
    BoolCoverage req_instruction;
    BoolCoverage req_store;
    BoolCoverage req_msb_err;
    BoolCoverage req_valid;
    BoolCoverage ptw_req_ready;
    BoolCoverage ptw_resp_canceled_req;
    BoolCoverage ptw_resp_access_fault;
    BoolCoverage ptw_resp_d;
    BoolCoverage ptw_resp_a;
    BoolCoverage ptw_resp_g;
    BoolCoverage ptw_resp_u;
    BoolCoverage ptw_resp_x;
    BoolCoverage ptw_resp_w;
    BoolCoverage ptw_resp_r;
    BoolCoverage ptw_resp_v;
    BoolCoverage ptw_resp_valid;
    ValueCoverage req_vpn;
    ValueCoverage satp_ppn0;
    ValueCoverage satp_ppn1;
    ValueCoverage matp_ppn0;
    ValueCoverage matp_ppn1;
    ValueCoverage ptw_resp_rsvd_for_hw;
    ValueCoverage ptw_resp_ppn;
    ValueCoverage ptw_resp_rsvd_for_sw;
    uint8_t vmspagesize_mask = 0;
    uint8_t req_min_id_mask = 0;
    uint8_t req_prv_mask = 0;
    uint8_t req_mpp_mask = 0;
    uint8_t tlb_invalidate_mask = 0;
    bool seen_satp_bare = false;
    bool seen_satp_sv39 = false;
    bool seen_satp_sv48 = false;
    bool seen_matp_bare = false;
    bool seen_matp_sv39 = false;
    bool seen_matp_sv48 = false;
};

void sample_mode(uint8_t mode, bool& bare, bool& sv39, bool& sv48) {
    if (mode == kModeBare) bare = true;
    if (mode == kModeSv39) sv39 = true;
    if (mode == kModeSv48) sv48 = true;
}

void sample_coverage(DUT* dut, Coverage& cov) {
    cov.dft_scanmode.sample(dut->dft_scanmode_i);
    cov.coop_mode.sample(dut->coop_mode_i);
    cov.req_mprv.sample(dut->req_mprv_i);
    cov.req_sum.sample(dut->req_sum_i);
    cov.req_mxr.sample(dut->req_mxr_i);
    cov.req_debug.sample(dut->req_debug_i);
    cov.req_passthrough.sample(dut->req_passthrough_i);
    cov.req_instruction.sample(dut->req_instruction_i);
    cov.req_store.sample(dut->req_store_i);
    cov.req_msb_err.sample(dut->req_msb_err_i);
    cov.req_valid.sample(dut->req_valid_i);
    cov.ptw_req_ready.sample(dut->ptw_req_ready_i);
    cov.ptw_resp_canceled_req.sample(dut->ptw_resp_canceled_req_i);
    cov.ptw_resp_access_fault.sample(dut->ptw_resp_access_fault_i);
    cov.ptw_resp_d.sample(dut->ptw_resp_d_i);
    cov.ptw_resp_a.sample(dut->ptw_resp_a_i);
    cov.ptw_resp_g.sample(dut->ptw_resp_g_i);
    cov.ptw_resp_u.sample(dut->ptw_resp_u_i);
    cov.ptw_resp_x.sample(dut->ptw_resp_x_i);
    cov.ptw_resp_w.sample(dut->ptw_resp_w_i);
    cov.ptw_resp_r.sample(dut->ptw_resp_r_i);
    cov.ptw_resp_v.sample(dut->ptw_resp_v_i);
    cov.ptw_resp_valid.sample(dut->ptw_resp_valid_i);
    cov.req_vpn.sample(dut->req_vpn_i);
    cov.satp_ppn0.sample(dut->satp_ppn0_i);
    cov.satp_ppn1.sample(dut->satp_ppn1_i);
    cov.matp_ppn0.sample(dut->matp_ppn0_i);
    cov.matp_ppn1.sample(dut->matp_ppn1_i);
    cov.ptw_resp_rsvd_for_hw.sample(dut->ptw_resp_rsvd_for_hw_i);
    cov.ptw_resp_ppn.sample(dut->ptw_resp_ppn_i);
    cov.ptw_resp_rsvd_for_sw.sample(dut->ptw_resp_rsvd_for_sw_i);
    cov.vmspagesize_mask |= uint8_t(1u << dut->vmspagesize_i);
    cov.req_min_id_mask |= uint8_t(1u << dut->req_min_id_i);
    cov.req_prv_mask |= uint8_t(1u << dut->req_prv_i);
    cov.req_mpp_mask |= uint8_t(1u << dut->req_mpp_i);
    cov.tlb_invalidate_mask |= uint8_t(1u << dut->tlb_invalidate_i);
    sample_mode(dut->satp_mode0_i, cov.seen_satp_bare, cov.seen_satp_sv39, cov.seen_satp_sv48);
    sample_mode(dut->satp_mode1_i, cov.seen_satp_bare, cov.seen_satp_sv39, cov.seen_satp_sv48);
    sample_mode(dut->matp_mode0_i, cov.seen_matp_bare, cov.seen_matp_sv39, cov.seen_matp_sv48);
    sample_mode(dut->matp_mode1_i, cov.seen_matp_bare, cov.seen_matp_sv39, cov.seen_matp_sv48);
}

void check_coverage(CosimCtrl<DUT>& sim, const Coverage& cov) {
    sim.check(cov.dft_scanmode.full(), "coverage: dft_scanmode exercised 0/1");
    sim.check(cov.coop_mode.full(), "coverage: coop_mode exercised 0/1");
    sim.check(cov.req_mprv.full(), "coverage: req_mprv exercised 0/1");
    sim.check(cov.req_sum.full(), "coverage: req_sum exercised 0/1");
    sim.check(cov.req_mxr.full(), "coverage: req_mxr exercised 0/1");
    sim.check(cov.req_debug.full(), "coverage: req_debug exercised 0/1");
    sim.check(cov.req_passthrough.full(), "coverage: req_passthrough exercised 0/1");
    sim.check(cov.req_instruction.full(), "coverage: req_instruction exercised 0/1");
    sim.check(cov.req_store.full(), "coverage: req_store exercised 0/1");
    sim.check(cov.req_msb_err.full(), "coverage: req_msb_err exercised 0/1");
    sim.check(cov.req_valid.full(), "coverage: req_valid exercised 0/1");
    sim.check(cov.ptw_req_ready.full(), "coverage: ptw_req_ready exercised 0/1");
    sim.check(cov.ptw_resp_canceled_req.full(), "coverage: ptw_resp_canceled_req exercised 0/1");
    sim.check(cov.ptw_resp_access_fault.full(), "coverage: ptw_resp_access_fault exercised 0/1");
    sim.check(cov.ptw_resp_d.full(), "coverage: ptw_resp_d exercised 0/1");
    sim.check(cov.ptw_resp_a.full(), "coverage: ptw_resp_a exercised 0/1");
    sim.check(cov.ptw_resp_g.full(), "coverage: ptw_resp_g exercised 0/1");
    sim.check(cov.ptw_resp_u.full(), "coverage: ptw_resp_u exercised 0/1");
    sim.check(cov.ptw_resp_x.full(), "coverage: ptw_resp_x exercised 0/1");
    sim.check(cov.ptw_resp_w.full(), "coverage: ptw_resp_w exercised 0/1");
    sim.check(cov.ptw_resp_r.full(), "coverage: ptw_resp_r exercised 0/1");
    sim.check(cov.ptw_resp_v.full(), "coverage: ptw_resp_v exercised 0/1");
    sim.check(cov.ptw_resp_valid.full(), "coverage: ptw_resp_valid exercised 0/1");
    sim.check(cov.req_vpn.full(), "coverage: req_vpn exercised zero/non-zero");
    sim.check(cov.satp_ppn0.full(), "coverage: satp_ppn0 exercised zero/non-zero");
    sim.check(cov.satp_ppn1.full(), "coverage: satp_ppn1 exercised zero/non-zero");
    sim.check(cov.matp_ppn0.full(), "coverage: matp_ppn0 exercised zero/non-zero");
    sim.check(cov.matp_ppn1.full(), "coverage: matp_ppn1 exercised zero/non-zero");
    sim.check(cov.ptw_resp_rsvd_for_hw.full(), "coverage: ptw_resp_rsvd_for_hw exercised zero/non-zero");
    sim.check(cov.ptw_resp_ppn.full(), "coverage: ptw_resp_ppn exercised zero/non-zero");
    sim.check(cov.ptw_resp_rsvd_for_sw.full(), "coverage: ptw_resp_rsvd_for_sw exercised zero/non-zero");
    sim.check(cov.vmspagesize_mask == 0x0F, "coverage: all vmspagesize values exercised");
    sim.check((cov.req_min_id_mask & 0x3) == 0x3, "coverage: both req_min_id values exercised");
    sim.check((cov.req_prv_mask & ((1u << kPrvU) | (1u << kPrvS) | (1u << kPrvM))) ==
                  ((1u << kPrvU) | (1u << kPrvS) | (1u << kPrvM)),
              "coverage: req_prv exercised U/S/M");
    sim.check((cov.req_mpp_mask & ((1u << kPrvU) | (1u << kPrvS) | (1u << kPrvM))) ==
                  ((1u << kPrvU) | (1u << kPrvS) | (1u << kPrvM)),
              "coverage: req_mpp exercised U/S/M");
    sim.check(cov.tlb_invalidate_mask == 0x0F, "coverage: all tlb_invalidate patterns exercised");
    sim.check(cov.seen_satp_bare && cov.seen_satp_sv39 && cov.seen_satp_sv48,
              "coverage: satp modes exercised bare/sv39/sv48");
    sim.check(cov.seen_matp_bare && cov.seen_matp_sv39 && cov.seen_matp_sv48,
              "coverage: matp modes exercised bare/sv39/sv48");
}

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

uint64_t vpn_for_index(unsigned idx, unsigned tag) {
    return (uint64_t(tag) << 4) | uint64_t(idx & 0xF);
}

void clear_ptw_resp(DUT* dut) {
    dut->ptw_resp_canceled_req_i = 0;
    dut->ptw_resp_access_fault_i = 0;
    dut->ptw_resp_rsvd_for_hw_i = 0;
    dut->ptw_resp_ppn_i = 0;
    dut->ptw_resp_rsvd_for_sw_i = 0;
    dut->ptw_resp_d_i = 0;
    dut->ptw_resp_a_i = 0;
    dut->ptw_resp_g_i = 0;
    dut->ptw_resp_u_i = 0;
    dut->ptw_resp_x_i = 0;
    dut->ptw_resp_w_i = 0;
    dut->ptw_resp_r_i = 0;
    dut->ptw_resp_v_i = 0;
    dut->ptw_resp_valid_i = 0;
}

void clear_inputs(DUT* dut) {
    dut->dft_scanmode_i = 0;
    dut->vmspagesize_i = 0;
    dut->coop_mode_i = 0;
    dut->req_min_id_i = 0;
    dut->req_prv_i = kPrvS;
    dut->req_mprv_i = 0;
    dut->req_mpp_i = kPrvM;
    dut->req_sum_i = 0;
    dut->req_mxr_i = 0;
    dut->req_debug_i = 0;
    dut->req_vpn_i = 0;
    dut->req_passthrough_i = 0;
    dut->req_instruction_i = 0;
    dut->req_store_i = 0;
    dut->req_msb_err_i = 0;
    dut->req_valid_i = 0;
    dut->satp_mode0_i = kModeBare;
    dut->satp_ppn0_i = 0;
    dut->satp_mode1_i = kModeBare;
    dut->satp_ppn1_i = 0;
    dut->matp_mode0_i = kModeBare;
    dut->matp_ppn0_i = 0;
    dut->matp_mode1_i = kModeBare;
    dut->matp_ppn1_i = 0;
    dut->tlb_invalidate_i = 0;
    dut->ptw_req_ready_i = 0;
    clear_ptw_resp(dut);
}

void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("resp_fill_pending", sim.dut->orig_resp_fill_pending_o, sim.dut->new_resp_fill_pending_o);
    sim.compare("resp_miss", sim.dut->orig_resp_miss_o, sim.dut->new_resp_miss_o);
    sim.compare("resp_ppn", sim.dut->orig_resp_ppn_o, sim.dut->new_resp_ppn_o);
    sim.compare("resp_xcpt_ld", sim.dut->orig_resp_xcpt_ld_o, sim.dut->new_resp_xcpt_ld_o);
    sim.compare("resp_xcpt_st", sim.dut->orig_resp_xcpt_st_o, sim.dut->new_resp_xcpt_st_o);
    sim.compare("resp_xcpt_if", sim.dut->orig_resp_xcpt_if_o, sim.dut->new_resp_xcpt_if_o);
    sim.compare("resp_access_fault", sim.dut->orig_resp_access_fault_o, sim.dut->new_resp_access_fault_o);
    sim.compare("ptw_req_satp_mode", sim.dut->orig_ptw_req_satp_mode_o, sim.dut->new_ptw_req_satp_mode_o);
    sim.compare("ptw_req_satp_ppn", sim.dut->orig_ptw_req_satp_ppn_o, sim.dut->new_ptw_req_satp_ppn_o);
    sim.compare("ptw_req_prv", sim.dut->orig_ptw_req_prv_o, sim.dut->new_ptw_req_prv_o);
    sim.compare("ptw_req_addr", sim.dut->orig_ptw_req_addr_o, sim.dut->new_ptw_req_addr_o);
    sim.compare("ptw_req_valid", sim.dut->orig_ptw_req_valid_o, sim.dut->new_ptw_req_valid_o);
    sim.compare("ptw_invalidate", sim.dut->orig_ptw_invalidate_o, sim.dut->new_ptw_invalidate_o);
    sim.compare("vm_enabled", sim.dut->orig_vm_enabled_o, sim.dut->new_vm_enabled_o);
    sim.compare("state_q", sim.dut->orig_state_o, sim.dut->new_state_o);
    sim.compare("ptw_req_valid_q", sim.dut->orig_ptw_req_valid_q_o, sim.dut->new_ptw_req_valid_q_o);
    sim.compare("ptw_resp_valid_q", sim.dut->orig_ptw_resp_valid_q_o, sim.dut->new_ptw_resp_valid_q_o);
    sim.compare("rf_latch_wr_en", sim.dut->orig_rf_latch_wr_en_o, sim.dut->new_rf_latch_wr_en_o);
    sim.compare("rf_latch_wr_en_early", sim.dut->orig_rf_latch_wr_en_early_o, sim.dut->new_rf_latch_wr_en_early_o);
    sim.compare("req_min_id_q", sim.dut->orig_req_min_id_q_o, sim.dut->new_req_min_id_q_o);
    sim.compare("req_vpn_q", sim.dut->orig_req_vpn_q_o, sim.dut->new_req_vpn_q_o);
    sim.compare("req_tag_q", sim.dut->orig_req_tag_q_o, sim.dut->new_req_tag_q_o);
    sim.compare("tlb_cache_waddr", sim.dut->orig_tlb_cache_waddr_o, sim.dut->new_tlb_cache_waddr_o);
    sim.compare("tlb_cache_raddr", sim.dut->orig_tlb_cache_raddr_o, sim.dut->new_tlb_cache_raddr_o);
}

void eval_and_compare(CosimCtrl<DUT>& sim, Coverage* cov = nullptr) {
    if (cov != nullptr) sample_coverage(sim.dut.get(), *cov);
    sim.dut->eval();
    compare_outputs(sim);
}

void step_half_and_compare(CosimCtrl<DUT>& sim, int clk_level, Coverage* cov = nullptr) {
    if (cov != nullptr) sample_coverage(sim.dut.get(), *cov);
    sim.dut->clk_i = clk_level;
    sim.dut->eval();
    compare_outputs(sim);
    sim.sim_time++;
    if (sim.sim_time >= sim.max_time) {
        printf("TIMEOUT at sim_time=%lu\n", (unsigned long)sim.sim_time);
        std::exit(2);
    }
}

void tick_and_compare(CosimCtrl<DUT>& sim, Coverage* cov = nullptr) {
    if (cov != nullptr) sample_coverage(sim.dut.get(), *cov);
    sim.dut->eval();
    sim.tick();
    sim.dut->eval();
    compare_outputs(sim);
}

void fill_entry(CosimCtrl<DUT>& sim,
                uint8_t min_id,
                uint64_t vpn,
                uint64_t ppn,
                bool user,
                bool read,
                bool write,
                bool exec,
                bool accessed,
                bool dirty,
                bool expected_m_mode,
                Coverage* cov = nullptr) {
    auto* dut = sim.dut.get();
    dut->satp_mode0_i = kModeSv39;
    dut->satp_mode1_i = kModeSv39;
    dut->satp_ppn0_i = 0x0010000;
    dut->satp_ppn1_i = 0x0020000;
    clear_ptw_resp(dut);
    dut->ptw_req_ready_i = 0;
    dut->tlb_invalidate_i = 0;

    dut->req_min_id_i = min_id;
    dut->req_prv_i = kPrvS;
    dut->req_mprv_i = 0;
    dut->req_mpp_i = kPrvM;
    dut->req_sum_i = 0;
    dut->req_mxr_i = 0;
    dut->req_debug_i = 0;
    dut->req_vpn_i = vpn;
    dut->req_passthrough_i = 0;
    dut->req_instruction_i = 0;
    dut->req_store_i = 0;
    dut->req_msb_err_i = 0;
    dut->req_valid_i = 1;
    tick_and_compare(sim, cov);

    dut->ptw_req_ready_i = 1;
    tick_and_compare(sim, cov);

    dut->ptw_req_ready_i = 0;
    clear_ptw_resp(dut);
    dut->ptw_resp_rsvd_for_hw_i = expected_m_mode ? 1 : 0;
    dut->ptw_resp_ppn_i = ppn;
    dut->ptw_resp_d_i = dirty;
    dut->ptw_resp_a_i = accessed;
    dut->ptw_resp_u_i = user;
    dut->ptw_resp_x_i = exec;
    dut->ptw_resp_w_i = write;
    dut->ptw_resp_r_i = read;
    dut->ptw_resp_v_i = 1;
    dut->ptw_resp_valid_i = 1;
    tick_and_compare(sim, cov);

    clear_ptw_resp(dut);
    tick_and_compare(sim, cov);
}

void fill_entry_posedge_only_resp(CosimCtrl<DUT>& sim,
                                  uint8_t min_id,
                                  uint64_t vpn,
                                  uint64_t ppn,
                                  bool user,
                                  bool read,
                                  bool write,
                                  bool exec,
                                  bool accessed,
                                  bool dirty,
                                  bool expected_m_mode,
                                  Coverage* cov = nullptr) {
    auto* dut = sim.dut.get();
    dut->satp_mode0_i = kModeSv39;
    dut->satp_mode1_i = kModeSv39;
    dut->satp_ppn0_i = 0x0010000;
    dut->satp_ppn1_i = 0x0020000;
    clear_ptw_resp(dut);
    dut->ptw_req_ready_i = 0;
    dut->tlb_invalidate_i = 0;

    dut->req_min_id_i = min_id;
    dut->req_prv_i = kPrvS;
    dut->req_mprv_i = 0;
    dut->req_mpp_i = kPrvM;
    dut->req_sum_i = 0;
    dut->req_mxr_i = 0;
    dut->req_debug_i = 0;
    dut->req_vpn_i = vpn;
    dut->req_passthrough_i = 0;
    dut->req_instruction_i = 0;
    dut->req_store_i = 0;
    dut->req_msb_err_i = 0;
    dut->req_valid_i = 1;
    tick_and_compare(sim, cov);

    dut->ptw_req_ready_i = 1;
    tick_and_compare(sim, cov);

    dut->ptw_req_ready_i = 0;
    clear_ptw_resp(dut);
    step_half_and_compare(sim, 0, cov);

    dut->ptw_resp_rsvd_for_hw_i = expected_m_mode ? 1 : 0;
    dut->ptw_resp_ppn_i = ppn;
    dut->ptw_resp_d_i = dirty;
    dut->ptw_resp_a_i = accessed;
    dut->ptw_resp_u_i = user;
    dut->ptw_resp_x_i = exec;
    dut->ptw_resp_w_i = write;
    dut->ptw_resp_r_i = read;
    dut->ptw_resp_v_i = 1;
    dut->ptw_resp_valid_i = 1;
    step_half_and_compare(sim, 1, cov);

    clear_ptw_resp(dut);
    step_half_and_compare(sim, 0, cov);
    step_half_and_compare(sim, 1, cov);
    tick_and_compare(sim, cov);
}

void drive_random_inputs(DUT* dut, uint32_t& seed) {
    dut->dft_scanmode_i = xorshift32(seed) & 1u;
    dut->vmspagesize_i = xorshift32(seed) & 0x3u;
    dut->coop_mode_i = xorshift32(seed) & 1u;
    dut->req_min_id_i = xorshift32(seed) & 1u;
    dut->req_prv_i = xorshift32(seed) & 0x3u;
    dut->req_mprv_i = xorshift32(seed) & 1u;
    dut->req_mpp_i = xorshift32(seed) & 0x3u;
    dut->req_sum_i = xorshift32(seed) & 1u;
    dut->req_mxr_i = xorshift32(seed) & 1u;
    dut->req_debug_i = xorshift32(seed) & 1u;
    dut->req_vpn_i = ((uint64_t(xorshift32(seed)) << 4) ^ xorshift32(seed)) & ((1ULL << 36) - 1);
    dut->req_passthrough_i = xorshift32(seed) & 1u;
    dut->req_instruction_i = xorshift32(seed) & 1u;
    dut->req_store_i = xorshift32(seed) & 1u;
    dut->req_msb_err_i = xorshift32(seed) & 1u;
    dut->req_valid_i = xorshift32(seed) & 1u;

    dut->satp_mode0_i = (xorshift32(seed) & 1u) ? kModeSv39 : kModeBare;
    dut->satp_ppn0_i = xorshift32(seed) & ((1u << 28) - 1);
    dut->satp_mode1_i = (xorshift32(seed) & 1u) ? kModeSv48 : kModeBare;
    dut->satp_ppn1_i = xorshift32(seed) & ((1u << 28) - 1);
    dut->matp_mode0_i = (xorshift32(seed) & 1u) ? kModeSv39 : kModeBare;
    dut->matp_ppn0_i = xorshift32(seed) & ((1u << 28) - 1);
    dut->matp_mode1_i = (xorshift32(seed) & 1u) ? kModeSv48 : kModeBare;
    dut->matp_ppn1_i = xorshift32(seed) & ((1u << 28) - 1);

    dut->tlb_invalidate_i = xorshift32(seed) & 0x3u;
    dut->ptw_req_ready_i = xorshift32(seed) & 1u;
    dut->ptw_resp_canceled_req_i = xorshift32(seed) & 1u;
    dut->ptw_resp_access_fault_i = xorshift32(seed) & 1u;
    dut->ptw_resp_rsvd_for_hw_i = xorshift32(seed) & 0xFFu;
    dut->ptw_resp_ppn_i =
        ((uint64_t(xorshift32(seed)) << 12) ^ xorshift32(seed)) & ((1ULL << 44) - 1);
    dut->ptw_resp_rsvd_for_sw_i = xorshift32(seed) & 0x3u;
    dut->ptw_resp_d_i = xorshift32(seed) & 1u;
    dut->ptw_resp_a_i = xorshift32(seed) & 1u;
    dut->ptw_resp_g_i = xorshift32(seed) & 1u;
    dut->ptw_resp_u_i = xorshift32(seed) & 1u;
    dut->ptw_resp_x_i = xorshift32(seed) & 1u;
    dut->ptw_resp_w_i = xorshift32(seed) & 1u;
    dut->ptw_resp_r_i = xorshift32(seed) & 1u;
    dut->ptw_resp_v_i = xorshift32(seed) & 1u;
    dut->ptw_resp_valid_i = xorshift32(seed) & 1u;
}

}  // namespace

double sc_time_stamp() {
    return 0.0;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    Coverage coverage;
    clear_inputs(sim.dut.get());
    sample_coverage(sim.dut.get(), coverage);
    sim.reset(2);
    clear_inputs(sim.dut.get());
    sim.dut->eval();
    sample_coverage(sim.dut.get(), coverage);

    for (unsigned min_id = 0; min_id < 2; ++min_id) {
        for (unsigned idx = 0; idx < 4; ++idx) {
            fill_entry(sim,
                       uint8_t(min_id),
                       vpn_for_index(idx, 0x10 + (min_id * 4) + idx),
                       0x10000 + (min_id * 0x100) + idx,
                       false,
                       true,
                       true,
                       false,
                       true,
                       true,
                       false,
                       &coverage);
        }
    }

    clear_inputs(sim.dut.get());
    sim.dut->satp_mode0_i = kModeSv39;
    sim.dut->req_valid_i = 1;
    sim.dut->req_vpn_i = 0x000ABCDEULL;
    sim.dut->req_passthrough_i = 1;
    tick_and_compare(sim, &coverage);

    clear_inputs(sim.dut.get());
    sim.dut->satp_mode0_i = kModeSv39;
    sim.dut->req_valid_i = 1;
    sim.dut->req_vpn_i = 0x00123456ULL;
    sim.dut->req_debug_i = 1;
    sim.dut->req_instruction_i = 1;
    tick_and_compare(sim, &coverage);

    clear_inputs(sim.dut.get());
    sim.dut->satp_mode0_i = kModeBare;
    sim.dut->matp_mode0_i = kModeSv39;
    sim.dut->matp_ppn0_i = 0x0032100;
    sim.dut->req_valid_i = 1;
    sim.dut->req_vpn_i = 0x00345678ULL;
    sim.dut->req_prv_i = kPrvS;
    sim.dut->req_mprv_i = 1;
    sim.dut->req_mpp_i = kPrvM;
    sim.dut->req_debug_i = 1;
    tick_and_compare(sim, &coverage);
    sim.dut->req_valid_i = 0;
    sim.dut->ptw_req_ready_i = 1;
    tick_and_compare(sim, &coverage);
    sim.dut->ptw_req_ready_i = 0;
    sim.dut->ptw_resp_canceled_req_i = 1;
    sim.dut->ptw_resp_r_i = 1;
    sim.dut->ptw_resp_v_i = 1;
    sim.dut->ptw_resp_valid_i = 1;
    tick_and_compare(sim, &coverage);
    clear_inputs(sim.dut.get());
    sim.dut->satp_mode0_i = kModeBare;
    sim.dut->matp_mode0_i = kModeSv39;
    sim.dut->matp_ppn0_i = 0x0032100;
    sim.dut->req_valid_i = 1;
    sim.dut->req_vpn_i = 0x00345678ULL;
    sim.dut->req_prv_i = kPrvS;
    sim.dut->req_mprv_i = 1;
    sim.dut->req_mpp_i = kPrvM;
    sim.dut->req_debug_i = 1;
    tick_and_compare(sim, &coverage);

    clear_inputs(sim.dut.get());
    sim.dut->satp_mode0_i = kModeSv39;
    sim.dut->req_min_id_i = 1;
    sim.dut->req_valid_i = 1;
    sim.dut->req_vpn_i = 0x002468ACULL;
    tick_and_compare(sim, &coverage);
    sim.dut->req_valid_i = 0;
    sim.dut->tlb_invalidate_i = 0x2;
    tick_and_compare(sim, &coverage);
    sim.dut->tlb_invalidate_i = 0;
    tick_and_compare(sim, &coverage);

    clear_inputs(sim.dut.get());
    sim.dut->vmspagesize_i = kPage2M;
    fill_entry(sim, 0, 0x00155400ULL, 0x000011111ULL, false, true, true, false, true, true, false, &coverage);
    sim.dut->req_vpn_i = 0x00155400ULL ^ 0x1A5ULL;
    tick_and_compare(sim, &coverage);

    clear_inputs(sim.dut.get());
    sim.dut->vmspagesize_i = kPage4K;
    fill_entry_posedge_only_resp(
        sim,
        0,
        vpn_for_index(3, 0x34),
        0x000000666ULL,
        false,
        true,
        true,
        false,
        true,
        true,
        false,
        &coverage);

    clear_inputs(sim.dut.get());
    sim.dut->vmspagesize_i = kPage1G;
    fill_entry(sim, 0, 0x00280000ULL, 0x000022222ULL, false, true, true, false, true, true, false, &coverage);
    sim.dut->req_vpn_i = 0x00280000ULL ^ 0x2AAAAULL;
    tick_and_compare(sim, &coverage);

    clear_inputs(sim.dut.get());
    sim.dut->coop_mode_i = 1;
    fill_entry(sim, 0, vpn_for_index(5, 0x30), 0x000000333ULL, false, true, true, false, true, true, false, &coverage);
    sim.dut->req_min_id_i = 1;
    sim.dut->req_vpn_i = vpn_for_index(5, 0x30);
    tick_and_compare(sim, &coverage);

    clear_inputs(sim.dut.get());
    fill_entry(sim, 0, vpn_for_index(6, 0x31), 0x000000444ULL, true, true, true, false, true, true, false, &coverage);
    sim.dut->req_sum_i = 0;
    tick_and_compare(sim, &coverage);
    sim.dut->req_sum_i = 1;
    tick_and_compare(sim, &coverage);

    clear_inputs(sim.dut.get());
    fill_entry(sim, 0, vpn_for_index(7, 0x32), 0x000000555ULL, false, true, true, false, true, true, true, &coverage);
    sim.dut->matp_mode0_i = kModeSv39;
    sim.dut->matp_ppn0_i = 0x0010000;
    sim.dut->req_debug_i = 1;
    sim.dut->req_mprv_i = 1;
    sim.dut->req_mpp_i = kPrvM;
    tick_and_compare(sim, &coverage);

    sim.dut->coop_mode_i = 1;
    sim.dut->req_valid_i = 1;
    sim.dut->req_min_id_i = 0;
    sim.dut->req_vpn_i = vpn_for_index(5, 0x33);
    sim.dut->satp_mode0_i = kModeSv48;
    sim.dut->req_msb_err_i = 1;
    tick_and_compare(sim, &coverage);
    sim.dut->req_msb_err_i = 0;
    sim.dut->satp_mode0_i = kModeSv39;

    constexpr uint32_t kHalfSeeds[] = {0x89ABCDEFu, 0x13579BDFu};
    for (uint32_t seed_init : kHalfSeeds) {
        uint32_t seed = seed_init;
        for (int cycle = 0; cycle < 2048; ++cycle) {
            drive_random_inputs(sim.dut.get(), seed);
            step_half_and_compare(sim, 0, &coverage);
            drive_random_inputs(sim.dut.get(), seed);
            eval_and_compare(sim, &coverage);
            step_half_and_compare(sim, 1, &coverage);
        }
    }

    constexpr uint32_t kSeeds[] = {0x5F3759DFu, 0xC001CAFEu, 0x1234ABCDu};
    for (uint32_t seed_init : kSeeds) {
        uint32_t seed = seed_init;
        for (int cycle = 0; cycle < 8192; ++cycle) {
            drive_random_inputs(sim.dut.get(), seed);
            tick_and_compare(sim, &coverage);
        }
    }

    check_coverage(sim, coverage);
    return sim.finish();
}
