// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vicache_micro_cache_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vicache_micro_cache_tb;

namespace {

constexpr uint8_t kModeBare = 0;
constexpr uint8_t kModeSv39 = 8;
constexpr uint8_t kPrvS = 1;
constexpr uint8_t kPrvM = 3;
constexpr uint8_t kPage4K = 0;

constexpr uint64_t kSpRomAddr = 0x0040000000ULL;
constexpr uint64_t kDramMcodeAddr = 0x8000001000ULL;
constexpr uint64_t kVmReqAddr = 0x00000123456000ULL;

using BlockWords = std::array<uint64_t, 8>;

void clear_l1_resp(DUT* dut) {
  dut->l1_miss_resp_early_valid_i = 0;
  dut->l1_miss_resp_valid_i = 0;
  dut->l1_miss_resp_data_0_i = 0;
  dut->l1_miss_resp_data_1_i = 0;
  dut->l1_miss_resp_data_2_i = 0;
  dut->l1_miss_resp_data_3_i = 0;
  dut->l1_miss_resp_data_4_i = 0;
  dut->l1_miss_resp_data_5_i = 0;
  dut->l1_miss_resp_data_6_i = 0;
  dut->l1_miss_resp_data_7_i = 0;
  dut->l1_miss_resp_ecc_err_i = 0;
  dut->l1_miss_resp_l2_err_i = 0;
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
  dut->ioshire_i = 1;
  dut->prefetch_prv_i = kPrvS;
  dut->prefetch_start_addr_i = 0;
  dut->prefetch_num_lines_i = 0;
  dut->prefetch_start_i = 0;
  dut->mprot_i = 0;
  dut->vmspagesize_i = kPage4K;
  dut->bypass_icache_i = 0;
  dut->coop_mode_i = 0;
  dut->req_valid_i = 0;
  dut->req_min_id_i = 0;
  dut->req_addr_i = 0;
  dut->req_prv_i = kPrvS;
  dut->req_mprv_i = 0;
  dut->req_mpp_i = kPrvM;
  dut->req_sum_i = 0;
  dut->req_mxr_i = 0;
  dut->req_debug_i = 0;
  dut->flush_data_i = 0;
  dut->l1_miss_req_ready_i = 0;
  clear_l1_resp(dut);
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
  dut->apb_paddr_i = 0;
  dut->apb_pwrite_i = 0;
  dut->apb_psel_i = 0;
  dut->apb_penable_i = 0;
  dut->apb_pwdata_i = 0;
}

template <typename Pred>
bool wait_until(SimCtrl<DUT>& sim, Pred pred, int max_cycles, const std::string& label) {
  for (int i = 0; i < max_cycles; ++i) {
    if (pred(sim.dut.get())) return true;
    sim.tick();
  }
  sim.check(false, label);
  return false;
}

void pulse_request(SimCtrl<DUT>& sim, uint64_t addr, uint8_t prv, bool bypass) {
  sim.dut->bypass_icache_i = bypass;
  sim.dut->req_prv_i = prv;
  sim.dut->req_addr_i = addr;
  sim.dut->req_valid_i = 1;
  sim.tick();
  sim.dut->req_valid_i = 0;
}

void accept_miss_request(SimCtrl<DUT>& sim) {
  sim.dut->l1_miss_req_ready_i = 1;
  sim.tick();
  sim.dut->l1_miss_req_ready_i = 0;
}

void drive_fill_response(SimCtrl<DUT>& sim, const BlockWords& words, bool ecc_err, bool l2_err) {
  sim.dut->l1_miss_resp_early_valid_i = 1;
  sim.dut->l1_miss_resp_valid_i = 1;
  sim.dut->l1_miss_resp_data_0_i = words[0];
  sim.dut->l1_miss_resp_data_1_i = words[1];
  sim.dut->l1_miss_resp_data_2_i = words[2];
  sim.dut->l1_miss_resp_data_3_i = words[3];
  sim.dut->l1_miss_resp_data_4_i = words[4];
  sim.dut->l1_miss_resp_data_5_i = words[5];
  sim.dut->l1_miss_resp_data_6_i = words[6];
  sim.dut->l1_miss_resp_data_7_i = words[7];
  sim.dut->l1_miss_resp_ecc_err_i = ecc_err;
  sim.dut->l1_miss_resp_l2_err_i = l2_err;
  sim.tick();
  clear_l1_resp(sim.dut.get());
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

  sim.check(sim.dut->prefetch_done_o == 1, "prefetch done set after reset");
  sim.check(sim.dut->req_ready_o == 1, "request path ready after reset");
  sim.check(sim.dut->resp_valid_o == 0, "no frontend response after reset");
  sim.check(sim.dut->l1_miss_req_valid_o == 0, "no L1 miss request after reset");

  const BlockWords hit_fill = {
      0x1111111111111111ULL, 0x2222222222222222ULL, 0x3333333333333333ULL, 0x4444444444444444ULL,
      0x5555555555555555ULL, 0x6666666666666666ULL, 0x7777777777777777ULL, 0x8888888888888888ULL};

  pulse_request(sim, kSpRomAddr, kPrvS, false);

  if (wait_until(sim, [](DUT* dut) { return dut->resp_valid_o; }, 12, "first access produces a miss response")) {
    sim.check(sim.dut->resp_miss_o == 1, "cold access reports miss");
  }

  if (wait_until(sim, [](DUT* dut) { return dut->l1_miss_req_valid_o; }, 12, "cold access issues L1 miss request")) {
    sim.check(sim.dut->l1_miss_req_addr_o == kSpRomAddr, "miss request forwards SP ROM address");
  }

  accept_miss_request(sim);
  drive_fill_response(sim, hit_fill, false, false);

  if (wait_until(sim, [](DUT* dut) { return dut->resp_fill_done_o; }, 12, "fill response completes")) {
    sim.check(sim.dut->resp_fill_done_o == 1, "fill done pulses high");
  }

  pulse_request(sim, kSpRomAddr, kPrvS, false);

  if (wait_until(sim, [](DUT* dut) { return dut->resp_valid_o && !dut->resp_miss_o; }, 12, "refilled access hits")) {
    sim.check(sim.dut->resp_data_0_o == hit_fill[0], "hit returns chunk word 0");
    sim.check(sim.dut->resp_data_1_o == hit_fill[1], "hit returns chunk word 1");
    sim.check(sim.dut->resp_data_2_o == hit_fill[2], "hit returns chunk word 2");
    sim.check(sim.dut->resp_data_3_o == hit_fill[3], "hit returns chunk word 3");
    sim.check(sim.dut->resp_cacheable_o == 1, "SP ROM hit is cacheable");
    sim.check(sim.dut->resp_page_fault_o == 0, "SP ROM hit has no page fault");
    sim.check(sim.dut->resp_access_fault_o == 0, "SP ROM hit has no access fault");
    sim.check(sim.dut->resp_bus_err_o == 0, "SP ROM hit has no bus error");
    sim.check(sim.dut->resp_ecc_err_o == 0, "SP ROM hit has no ECC error");
  }

  reset_and_clear(sim);
  sim.dut->ioshire_i = 0;

  const BlockWords bypass_fill = {
      0xABABABABABABABABULL, 0xCDCDCDCDCDCDCDCDULL, 0xEFEFEFEFEFEFEFEFULL, 0x0123456789ABCDEFULL,
      0x13579BDF2468ACE0ULL, 0x0F0E0D0C0B0A0908ULL, 0x1122334455667788ULL, 0x99AABBCCDDEEFF00ULL};

  pulse_request(sim, kDramMcodeAddr, kPrvM, true);

  if (wait_until(sim, [](DUT* dut) { return dut->l1_miss_req_valid_o; }, 12, "bypass access issues L1 miss request")) {
    sim.check(sim.dut->l1_miss_req_addr_o == kDramMcodeAddr, "bypass miss request forwards DRAM address");
  }

  accept_miss_request(sim);
  drive_fill_response(sim, bypass_fill, true, true);

  if (wait_until(sim, [](DUT* dut) { return dut->resp_fill_done_o; }, 12, "bypass fill response completes")) {
    sim.check(sim.dut->resp_fill_done_o == 1, "bypass fill done pulses high");
  }

  pulse_request(sim, kDramMcodeAddr, kPrvM, true);

  if (wait_until(sim, [](DUT* dut) { return dut->resp_valid_o && !dut->resp_miss_o; }, 12, "bypass replay hits")) {
    sim.check(sim.dut->resp_data_0_o == bypass_fill[0], "bypass hit returns chunk word 0");
    sim.check(sim.dut->resp_data_1_o == bypass_fill[1], "bypass hit returns chunk word 1");
    sim.check(sim.dut->resp_data_2_o == bypass_fill[2], "bypass hit returns chunk word 2");
    sim.check(sim.dut->resp_data_3_o == bypass_fill[3], "bypass hit returns chunk word 3");
    sim.check(sim.dut->resp_bus_err_o == 1, "bypass hit forwards L2 error");
    sim.check(sim.dut->resp_ecc_err_o == 1, "bypass hit forwards ECC error");
  }

  reset_and_clear(sim);
  sim.dut->ioshire_i = 0;
  sim.dut->satp_mode0_i = kModeSv39;
  sim.dut->satp_ppn0_i = 0x0010000;
  pulse_request(sim, kVmReqAddr, kPrvS, false);

  if (wait_until(sim, [](DUT* dut) { return dut->ptw_req_valid_o; }, 12, "VM access issues PTW request")) {
    sim.check(sim.dut->ptw_req_satp_mode_o == kModeSv39, "PTW request forwards SATP mode");
    sim.check(sim.dut->ptw_req_satp_ppn_o == 0x0010000, "PTW request forwards SATP ppn");
    sim.check(sim.dut->ptw_req_prv_o == kPrvS, "PTW request forwards privilege");
    sim.check(sim.dut->ptw_req_addr_o == (kVmReqAddr >> 12), "PTW request forwards VPN");
  }

  return sim.finish();
}
