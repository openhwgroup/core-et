// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_icache_tlb_array_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <random>

using DUT = Vcosim_icache_tlb_array_tb;

namespace {

constexpr uint8_t kModeBare = 0;
constexpr uint8_t kModeSv39 = 8;
constexpr uint8_t kModeSv48 = 9;
constexpr uint8_t kPrvS = 1;
constexpr uint8_t kPrvM = 3;
constexpr uint8_t kPage4K = 0;

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
  dut->vmspagesize_i = kPage4K;
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
  sim.compare("resp_xcpt_if", sim.dut->orig_resp_xcpt_if_o, sim.dut->new_resp_xcpt_if_o);
  sim.compare("resp_access_fault", sim.dut->orig_resp_access_fault_o, sim.dut->new_resp_access_fault_o);
  sim.compare("ptw_req_satp_mode", sim.dut->orig_ptw_req_satp_mode_o, sim.dut->new_ptw_req_satp_mode_o);
  sim.compare("ptw_req_satp_ppn", sim.dut->orig_ptw_req_satp_ppn_o, sim.dut->new_ptw_req_satp_ppn_o);
  sim.compare("ptw_req_prv", sim.dut->orig_ptw_req_prv_o, sim.dut->new_ptw_req_prv_o);
  sim.compare("ptw_req_addr", sim.dut->orig_ptw_req_addr_o, sim.dut->new_ptw_req_addr_o);
  sim.compare("ptw_req_valid", sim.dut->orig_ptw_req_valid_o, sim.dut->new_ptw_req_valid_o);
  sim.compare("ptw_invalidate", sim.dut->orig_ptw_invalidate_o, sim.dut->new_ptw_invalidate_o);
}

void tick_and_compare(CosimCtrl<DUT>& sim) {
  sim.tick();
  compare_outputs(sim);
}

void apply_reset(CosimCtrl<DUT>& sim) {
  clear_inputs(sim.dut.get());
  sim.reset(1);
  clear_inputs(sim.dut.get());
  compare_outputs(sim);
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  apply_reset(sim);

  sim.dut->req_valid_i = 1;
  sim.dut->req_vpn_i = 0x123456789ULL;
  tick_and_compare(sim);

  clear_inputs(sim.dut.get());
  sim.dut->satp_mode0_i = kModeSv48;
  sim.dut->req_valid_i = 1;
  sim.dut->req_msb_err_i = 1;
  sim.dut->req_vpn_i = 0x22222ULL;
  tick_and_compare(sim);

  clear_inputs(sim.dut.get());
  sim.dut->satp_mode0_i = kModeSv39;
  sim.dut->satp_ppn0_i = 0x0010000;
  sim.dut->req_valid_i = 1;
  sim.dut->req_vpn_i = 0x00123456ULL;
  tick_and_compare(sim);
  sim.dut->ptw_req_ready_i = 1;
  tick_and_compare(sim);
  clear_ptw_resp(sim.dut.get());
  sim.dut->ptw_req_ready_i = 0;
  sim.dut->ptw_resp_access_fault_i = 1;
  sim.dut->ptw_resp_r_i = 1;
  sim.dut->ptw_resp_v_i = 1;
  sim.dut->ptw_resp_valid_i = 1;
  tick_and_compare(sim);

  std::mt19937_64 rng(0x1CA7E7B5AULL);
  for (int cycle = 0; cycle < 4000; ++cycle) {
    sim.dut->vmspagesize_i = rng() & 0x3;
    sim.dut->coop_mode_i = rng() & 0x1;
    sim.dut->req_min_id_i = rng() & 0x1;
    sim.dut->req_prv_i = rng() & 0x3;
    sim.dut->req_mprv_i = rng() & 0x1;
    sim.dut->req_mpp_i = rng() & 0x3;
    sim.dut->req_sum_i = rng() & 0x1;
    sim.dut->req_mxr_i = rng() & 0x1;
    sim.dut->req_debug_i = rng() & 0x1;
    sim.dut->req_vpn_i = rng() & ((1ULL << 36) - 1);
    sim.dut->req_passthrough_i = rng() & 0x1;
    sim.dut->req_msb_err_i = rng() & 0x1;
    sim.dut->req_valid_i = rng() & 0x1;

    sim.dut->satp_mode0_i = (rng() & 0x3) == 0 ? kModeBare : ((rng() & 0x1) ? kModeSv39 : kModeSv48);
    sim.dut->satp_mode1_i = (rng() & 0x3) == 0 ? kModeBare : ((rng() & 0x1) ? kModeSv39 : kModeSv48);
    sim.dut->matp_mode0_i = (rng() & 0x3) == 0 ? kModeBare : ((rng() & 0x1) ? kModeSv39 : kModeSv48);
    sim.dut->matp_mode1_i = (rng() & 0x3) == 0 ? kModeBare : ((rng() & 0x1) ? kModeSv39 : kModeSv48);
    sim.dut->satp_ppn0_i = rng() & ((1ULL << 28) - 1);
    sim.dut->satp_ppn1_i = rng() & ((1ULL << 28) - 1);
    sim.dut->matp_ppn0_i = rng() & ((1ULL << 28) - 1);
    sim.dut->matp_ppn1_i = rng() & ((1ULL << 28) - 1);
    sim.dut->tlb_invalidate_i = rng() & 0x3;
    sim.dut->ptw_req_ready_i = rng() & 0x1;

    sim.dut->ptw_resp_canceled_req_i = rng() & 0x1;
    sim.dut->ptw_resp_access_fault_i = rng() & 0x1;
    sim.dut->ptw_resp_rsvd_for_hw_i = rng() & 0xff;
    sim.dut->ptw_resp_ppn_i = rng() & ((1ULL << 44) - 1);
    sim.dut->ptw_resp_rsvd_for_sw_i = rng() & 0x3;
    sim.dut->ptw_resp_d_i = rng() & 0x1;
    sim.dut->ptw_resp_a_i = rng() & 0x1;
    sim.dut->ptw_resp_g_i = rng() & 0x1;
    sim.dut->ptw_resp_u_i = rng() & 0x1;
    sim.dut->ptw_resp_x_i = rng() & 0x1;
    sim.dut->ptw_resp_w_i = rng() & 0x1;
    sim.dut->ptw_resp_r_i = rng() & 0x1;
    sim.dut->ptw_resp_v_i = rng() & 0x1;
    sim.dut->ptw_resp_valid_i = rng() & 0x1;

    tick_and_compare(sim);
  }

  return sim.finish();
}
