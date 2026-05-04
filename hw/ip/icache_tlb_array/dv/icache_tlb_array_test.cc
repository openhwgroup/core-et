// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for icache_tlb_array.

#include "Vicache_tlb_array_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vicache_tlb_array_tb;

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
  sim.check(sim.dut->resp_miss_o == 0, "bare mode is passthrough");
  sim.check(sim.dut->resp_fill_pending_o == 0, "bare mode fill pending clear");
  sim.check(sim.dut->resp_ppn_o == (0x123456789ULL & ((1ULL << 28) - 1)), "bare-mode PPN matches VPN low bits");
  sim.check(sim.dut->resp_xcpt_if_o == 0, "bare mode no instruction fault");
  sim.check(sim.dut->resp_access_fault_o == 0, "bare mode no access fault");
  sim.check(sim.dut->ptw_req_valid_o == 0, "bare mode does not issue PTW request");

  reset_and_clear(sim);
  sim.dut->satp_mode0_i = kModeSv48;
  sim.dut->req_valid_i = 1;
  sim.dut->req_msb_err_i = 1;
  sim.dut->req_vpn_i = 0x22222ULL;
  tick_and_eval(sim);
  sim.check(sim.dut->resp_xcpt_if_o == 1, "invalid address raises icache instruction fault");
  sim.check(sim.dut->resp_miss_o == 1, "invalid address preserves the underlying TLB miss indication");
  sim.check(sim.dut->ptw_req_valid_o == 0, "invalid address does not issue PTW request");

  reset_and_clear(sim);
  sim.dut->satp_mode0_i = kModeSv39;
  sim.dut->satp_ppn0_i = 0x0010000;
  sim.dut->req_valid_i = 1;
  sim.dut->req_vpn_i = 0x00123456ULL;
  tick_and_eval(sim);
  sim.check(sim.dut->resp_miss_o == 1, "active VM request misses before PTW fill");
  sim.check(sim.dut->resp_fill_pending_o == 0, "initial miss is not yet fill-pending");
  tick_and_eval(sim);
  sim.check(sim.dut->ptw_req_valid_o == 1, "miss issues PTW request");
  sim.check(sim.dut->ptw_req_satp_mode_o == kModeSv39, "PTW request forwards SATP mode");
  sim.check(sim.dut->ptw_req_satp_ppn_o == 0x0010000, "PTW request forwards SATP PPN");
  sim.check(sim.dut->ptw_req_prv_o == kPrvS, "PTW request forwards privilege");
  sim.check(sim.dut->ptw_req_addr_o == 0x00123456ULL, "PTW request forwards VPN");

  return sim.finish();
}
