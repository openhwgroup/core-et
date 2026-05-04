// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for minion_tlb.

#include "Vminion_tlb_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vminion_tlb_tb;

namespace {

constexpr uint8_t kModeBare = 0;
constexpr uint8_t kModeSv39 = 8;
constexpr uint8_t kModeSv48 = 9;
constexpr uint8_t kPrvU = 0;
constexpr uint8_t kPrvS = 1;
constexpr uint8_t kPrvM = 3;
constexpr uint8_t kPage4K = 0;

void clear_req(DUT* dut) {
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
    dut->vmspagesize_i = kPage4K;
    dut->coop_mode_i = 0;
    clear_req(dut);
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

void eval(DUT* dut) {
    dut->eval();
}

void tick_and_eval(SimCtrl<DUT>& sim) {
    sim.dut->eval();
    sim.tick();
    sim.dut->eval();
}

void reset_and_clear(SimCtrl<DUT>& sim) {
    clear_inputs(sim.dut.get());
    sim.reset(1);
    clear_inputs(sim.dut.get());
    sim.dut->eval();
}

uint64_t vpn_for_idx(unsigned idx, unsigned tag) {
    return (uint64_t(tag) << 4) | uint64_t(idx & 0xF);
}

void set_split_req(DUT* dut, uint8_t min_id, uint64_t vpn, bool store = false) {
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
    dut->req_store_i = store;
    dut->req_msb_err_i = 0;
    dut->req_valid_i = 1;
}

void set_ptw_leaf(DUT* dut,
                  uint64_t ppn,
                  bool user,
                  bool read,
                  bool write,
                  bool exec,
                  bool accessed,
                  bool dirty,
                  bool expected_m_mode,
                  bool access_fault = false) {
    dut->ptw_resp_canceled_req_i = 0;
    dut->ptw_resp_access_fault_i = access_fault;
    dut->ptw_resp_rsvd_for_hw_i = expected_m_mode ? 1 : 0;
    dut->ptw_resp_ppn_i = ppn;
    dut->ptw_resp_rsvd_for_sw_i = 0;
    dut->ptw_resp_d_i = dirty;
    dut->ptw_resp_a_i = accessed;
    dut->ptw_resp_g_i = 0;
    dut->ptw_resp_u_i = user;
    dut->ptw_resp_x_i = exec;
    dut->ptw_resp_w_i = write;
    dut->ptw_resp_r_i = read;
    dut->ptw_resp_v_i = 1;
}

void fill_entry(SimCtrl<DUT>& sim,
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
                bool access_fault = false) {
    auto* dut = sim.dut.get();
    dut->satp_mode0_i = kModeSv39;
    dut->satp_mode1_i = kModeSv39;
    dut->satp_ppn0_i = 0x0010000;
    dut->satp_ppn1_i = 0x0020000;
    clear_ptw_resp(dut);
    dut->ptw_req_ready_i = 0;
    dut->tlb_invalidate_i = 0;

    set_split_req(dut, min_id, vpn);
    eval(dut);
    sim.check(dut->resp_miss_o == 1, "miss asserted before PTW request");
    tick_and_eval(sim);
    sim.check(dut->ptw_req_valid_o == 1, "PTW request asserted after miss");
    sim.check(dut->ptw_req_addr_o == vpn, "PTW request address matches VPN");

    dut->ptw_req_ready_i = 1;
    tick_and_eval(sim);
    sim.check(dut->resp_fill_pending_o == 1, "fill pending in wait state");
    sim.check(dut->ptw_req_valid_o == 0, "PTW request dropped after ready");

    dut->ptw_req_ready_i = 0;
    set_ptw_leaf(dut, ppn, user, read, write, exec, accessed, dirty, expected_m_mode, access_fault);
    dut->ptw_resp_valid_i = 1;
    tick_and_eval(sim);
    sim.check(dut->resp_fill_pending_o == 1, "fill pending while PTW response is buffered");

    // Preserve the original latch-style RF write timing: a one-cycle PTW pulse
    // can leave stale RF contents visible, while a two-cycle hold commits the
    // translated entry that the standalone TLB then returns on the next hit.
    dut->ptw_resp_valid_i = 1;
    tick_and_eval(sim);

    clear_ptw_resp(dut);
    tick_and_eval(sim);
}

}  // namespace

double sc_time_stamp() {
    return 0.0;
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    reset_and_clear(sim);

    sim.check(sim.dut->ptw_req_valid_o == 0, "PTW request clear after reset");
    sim.check(sim.dut->resp_fill_pending_o == 0, "fill pending clear after reset");

    sim.dut->req_valid_i = 1;
    sim.dut->req_vpn_i = 0x123456789ULL;
    eval(sim.dut.get());
    sim.check(sim.dut->vm_enabled_o == 0, "bare mode disables VM");
    sim.check(sim.dut->resp_miss_o == 0, "bare mode is passthrough");
    sim.check(sim.dut->resp_ppn_o == (0x123456789ULL & ((1ULL << 28) - 1)), "passthrough PPN matches VPN low bits");
    sim.check(sim.dut->ptw_req_valid_o == 0, "bare mode does not issue PTW request");

    reset_and_clear(sim);
    sim.dut->satp_mode0_i = kModeSv39;
    sim.dut->req_valid_i = 1;
    sim.dut->req_vpn_i = 0x00ABCDEFULL;
    sim.dut->req_passthrough_i = 1;
    eval(sim.dut.get());
    sim.check(sim.dut->vm_enabled_o == 0, "passthrough disables VM even when SATP is active");
    sim.check(sim.dut->resp_miss_o == 0, "passthrough suppresses TLB misses");
    tick_and_eval(sim);
    sim.check(sim.dut->ptw_req_valid_o == 0, "passthrough does not issue PTW request");

    reset_and_clear(sim);
    sim.dut->satp_mode0_i = kModeSv39;
    sim.dut->req_valid_i = 1;
    sim.dut->req_vpn_i = 0x00123456ULL;
    sim.dut->req_debug_i = 1;
    sim.dut->req_instruction_i = 1;
    eval(sim.dut.get());
    sim.check(sim.dut->vm_enabled_o == 0, "debug instruction fetch bypasses VM");
    sim.check(sim.dut->resp_miss_o == 0, "debug instruction fetch is passthrough");
    tick_and_eval(sim);
    sim.check(sim.dut->ptw_req_valid_o == 0, "debug instruction fetch does not issue PTW request");

    reset_and_clear(sim);
    sim.dut->satp_mode0_i = kModeBare;
    sim.dut->matp_mode0_i = kModeSv39;
    sim.dut->matp_ppn0_i = 0x0032100;
    sim.dut->req_valid_i = 1;
    sim.dut->req_vpn_i = 0x00345678ULL;
    sim.dut->req_prv_i = kPrvS;
    sim.dut->req_mprv_i = 1;
    sim.dut->req_mpp_i = kPrvM;
    sim.dut->req_debug_i = 1;
    eval(sim.dut.get());
    sim.check(sim.dut->vm_enabled_o == 1, "debug mprv request uses MATP VM mode");
    sim.check(sim.dut->resp_miss_o == 1, "MATP path miss detected");
    tick_and_eval(sim);
    sim.check(sim.dut->ptw_req_valid_o == 1, "MATP path issues PTW request");
    sim.check(sim.dut->ptw_req_satp_mode_o == kModeSv39, "PTW request takes MATP mode");
    sim.check(sim.dut->ptw_req_satp_ppn_o == 0x0032100, "PTW request takes MATP PPN");
    sim.check(sim.dut->ptw_req_prv_o == kPrvS, "PTW request preserves original request privilege");
    sim.check(sim.dut->ptw_req_addr_o == 0x00345678ULL, "PTW request preserves VPN");
    sim.dut->req_valid_i = 0;
    sim.dut->ptw_req_ready_i = 1;
    tick_and_eval(sim);
    sim.dut->ptw_req_ready_i = 0;
    sim.dut->ptw_resp_canceled_req_i = 1;
    sim.dut->ptw_resp_r_i = 1;
    sim.dut->ptw_resp_v_i = 1;
    sim.dut->ptw_resp_valid_i = 1;
    tick_and_eval(sim);
    clear_ptw_resp(sim.dut.get());
    tick_and_eval(sim);
    sim.dut->req_valid_i = 1;
    eval(sim.dut.get());
    sim.check(sim.dut->resp_miss_o == 1, "canceled PTW response does not populate the TLB");
    tick_and_eval(sim);
    sim.check(sim.dut->ptw_req_valid_o == 1, "same request reissues after canceled PTW response");

    reset_and_clear(sim);
    sim.dut->satp_mode0_i = kModeSv39;
    sim.dut->req_valid_i = 1;
    sim.dut->req_vpn_i = 0x002468ACULL;
    eval(sim.dut.get());
    tick_and_eval(sim);
    sim.check(sim.dut->ptw_req_valid_o == 1, "request enters PTW request state");
    sim.dut->req_valid_i = 0;
    sim.dut->tlb_invalidate_i = 0x1;
    tick_and_eval(sim);
    sim.dut->tlb_invalidate_i = 0;
    tick_and_eval(sim);
    sim.check(sim.dut->ptw_req_valid_o == 0, "invalidate clears outstanding PTW request");
    sim.check(sim.dut->resp_fill_pending_o == 0, "invalidate returns FSM to ready");
    sim.dut->req_valid_i = 1;
    eval(sim.dut.get());
    sim.check(sim.dut->resp_miss_o == 1, "invalidated request still misses when retried");
    tick_and_eval(sim);
    sim.check(sim.dut->ptw_req_valid_o == 1, "invalidated request can be retried");

    reset_and_clear(sim);
    sim.dut->satp_mode0_i = kModeSv48;
    sim.dut->req_valid_i = 1;
    sim.dut->req_vpn_i = vpn_for_idx(1, 0x55);
    sim.dut->req_msb_err_i = 1;
    eval(sim.dut.get());
    sim.check(sim.dut->vm_enabled_o == 1, "SV48 enables VM");
    sim.check(sim.dut->resp_xcpt_ld_o == 1, "invalid SV48 address raises load exception");
    sim.check(sim.dut->resp_xcpt_st_o == 1, "invalid SV48 address raises store exception");
    sim.check(sim.dut->resp_xcpt_if_o == 1, "invalid SV48 address raises fetch exception");
    tick_and_eval(sim);
    sim.check(sim.dut->ptw_req_valid_o == 0, "invalid address does not issue PTW request");

    reset_and_clear(sim);
    fill_entry(sim, 0, vpn_for_idx(1, 0x11), 0x0000ABCDEULL, false, true, true, false, true, true, false);
    eval(sim.dut.get());
    sim.check(sim.dut->resp_miss_o == 0, "filled entry hits");
    sim.check(sim.dut->resp_ppn_o == (0x0000ABCDEULL & ((1ULL << 28) - 1)), "filled entry returns PTW PPN");
    sim.check(sim.dut->resp_xcpt_ld_o == 0, "clean load hit has no exception");

    reset_and_clear(sim);
    fill_entry(sim, 0, vpn_for_idx(2, 0x12), 0x000012345ULL, false, true, true, false, true, true, false);
    sim.dut->req_prv_i = kPrvU;
    eval(sim.dut.get());
    sim.check(sim.dut->resp_miss_o == 0, "user privilege mismatch still hits entry");
    sim.check(sim.dut->resp_xcpt_ld_o == 1, "user privilege mismatch raises load exception");

    reset_and_clear(sim);
    fill_entry(sim, 0, vpn_for_idx(3, 0x13), 0x00006789AULL, false, false, false, true, true, true, false);
    sim.dut->req_mxr_i = 1;
    eval(sim.dut.get());
    sim.check(sim.dut->resp_xcpt_ld_o == 0, "mxr allows execute-only page to be read");

    reset_and_clear(sim);
    fill_entry(sim, 0, vpn_for_idx(0, 0x14), 0x0000FEDCBULL, false, true, true, false, false, false, false);
    sim.dut->req_store_i = 1;
    eval(sim.dut.get());
    sim.check(sim.dut->resp_xcpt_st_o == 1, "store to A=0/D=0 page raises store exception");

    reset_and_clear(sim);
    fill_entry(sim, 0, vpn_for_idx(1, 0x15), 0x000045678ULL, false, true, true, false, true, true, false, true);
    eval(sim.dut.get());
    sim.check(sim.dut->resp_access_fault_o == 1, "PTW access fault propagates on hit");

    reset_and_clear(sim);
    sim.dut->vmspagesize_i = 2;
    fill_entry(sim, 0, 0x00155400ULL, 0x000011111ULL, false, true, true, false, true, true, false);
    sim.dut->req_vpn_i = 0x00155400ULL ^ 0x1A5ULL;
    eval(sim.dut.get());
    sim.check(sim.dut->resp_miss_o == 0, "2M page ignores low VPN bits");
    sim.check(sim.dut->resp_ppn_o == (0x000011111ULL & ((1ULL << 28) - 1)), "2M page hit keeps cached PPN");

    reset_and_clear(sim);
    sim.dut->vmspagesize_i = 3;
    fill_entry(sim, 0, 0x00280000ULL, 0x000022222ULL, false, true, true, false, true, true, false);
    sim.dut->req_vpn_i = 0x00280000ULL ^ 0x2AAAAULL;
    eval(sim.dut.get());
    sim.check(sim.dut->resp_miss_o == 0, "1G page ignores low VPN bits");
    sim.check(sim.dut->resp_ppn_o == (0x000022222ULL & ((1ULL << 28) - 1)), "1G page hit keeps cached PPN");

    reset_and_clear(sim);
    fill_entry(sim, 0, vpn_for_idx(0, 0x20), 0x000000111ULL, false, true, true, false, true, true, false);
    fill_entry(sim, 1, vpn_for_idx(0, 0x21), 0x000000222ULL, false, true, true, false, true, true, false);
    sim.dut->tlb_invalidate_i = 0x2;
    tick_and_eval(sim);
    sim.dut->tlb_invalidate_i = 0;
    tick_and_eval(sim);
    sim.dut->req_min_id_i = 0;
    sim.dut->req_vpn_i = vpn_for_idx(0, 0x20);
    sim.dut->req_valid_i = 1;
    eval(sim.dut.get());
    sim.check(sim.dut->resp_miss_o == 0, "split invalidate keeps other minion entries");
    sim.dut->req_min_id_i = 1;
    sim.dut->req_vpn_i = vpn_for_idx(0, 0x21);
    eval(sim.dut.get());
    sim.check(sim.dut->resp_miss_o == 1, "split invalidate clears matching minion entries");

    reset_and_clear(sim);
    sim.dut->coop_mode_i = 1;
    tick_and_eval(sim);
    fill_entry(sim, 0, vpn_for_idx(5, 0x30), 0x000000333ULL, false, true, true, false, true, true, false);
    sim.dut->req_min_id_i = 1;
    sim.dut->req_vpn_i = vpn_for_idx(5, 0x30);
    eval(sim.dut.get());
    sim.check(sim.dut->resp_miss_o == 0, "cooperative mode shares entries across minions");
    sim.dut->coop_mode_i = 1;
    sim.dut->tlb_invalidate_i = 0x2;
    tick_and_eval(sim);
    sim.dut->tlb_invalidate_i = 0;
    tick_and_eval(sim);
    sim.dut->req_min_id_i = 0;
    sim.dut->req_vpn_i = vpn_for_idx(5, 0x30);
    sim.dut->req_valid_i = 1;
    eval(sim.dut.get());
    sim.check(sim.dut->resp_miss_o == 1, "cooperative invalidate clears whole cache");

    reset_and_clear(sim);
    fill_entry(sim, 0, vpn_for_idx(6, 0x31), 0x000000444ULL, true, true, true, false, true, true, false);
    sim.dut->req_sum_i = 0;
    eval(sim.dut.get());
    sim.check(sim.dut->resp_xcpt_ld_o == 1, "SUM=0 blocks supervisor access to user page");
    sim.dut->req_sum_i = 1;
    eval(sim.dut.get());
    sim.check(sim.dut->resp_xcpt_ld_o == 0, "SUM=1 allows supervisor access to user page");

    reset_and_clear(sim);
    fill_entry(sim, 0, vpn_for_idx(7, 0x32), 0x000000555ULL, false, true, true, false, true, true, true);
    eval(sim.dut.get());
    sim.check(sim.dut->resp_miss_o == 1, "S-mode request misses when expected execution mode is machine");
    sim.dut->matp_mode0_i = kModeSv39;
    sim.dut->matp_ppn0_i = 0x0010000;
    sim.dut->req_debug_i = 1;
    sim.dut->req_mprv_i = 1;
    sim.dut->req_mpp_i = kPrvM;
    eval(sim.dut.get());
    sim.check(sim.dut->resp_miss_o == 0, "M-mode request hits machine-only translation");

    return sim.finish();
}
