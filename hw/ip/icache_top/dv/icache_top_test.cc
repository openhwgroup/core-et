// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vicache_top_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>

using DUT = Vicache_top_tb;

namespace {

constexpr uint8_t kModeBare = 0;
constexpr uint8_t kModeSv39 = 8;
constexpr uint8_t kPrvS = 1;
constexpr uint8_t kPrvM = 3;
constexpr uint8_t kPage4K = 0;
constexpr uint8_t kRspAckData = 1;

constexpr uint64_t kSpRomAddr = 0x0040000000ULL;
constexpr uint64_t kVmReqAddr = 0x00000123456000ULL;

using BlockWords = std::array<uint64_t, 8>;

void clear_l2_resp(DUT* dut) {
  dut->l2_miss_resp_valid_i = 0;
  dut->l2_miss_resp_opcode_i = 0;
}

void clear_ptw0_resp(DUT* dut) {
  dut->ptw0_resp_canceled_req_i = 0;
  dut->ptw0_resp_access_fault_i = 0;
  dut->ptw0_resp_rsvd_for_hw_i = 0;
  dut->ptw0_resp_ppn_i = 0;
  dut->ptw0_resp_rsvd_for_sw_i = 0;
  dut->ptw0_resp_d_i = 0;
  dut->ptw0_resp_a_i = 0;
  dut->ptw0_resp_g_i = 0;
  dut->ptw0_resp_u_i = 0;
  dut->ptw0_resp_x_i = 0;
  dut->ptw0_resp_w_i = 0;
  dut->ptw0_resp_r_i = 0;
  dut->ptw0_resp_v_i = 0;
  dut->ptw0_resp_valid_i = 0;
}

void clear_ptw1_resp(DUT* dut) {
  dut->ptw1_resp_canceled_req_i = 0;
  dut->ptw1_resp_access_fault_i = 0;
  dut->ptw1_resp_rsvd_for_hw_i = 0;
  dut->ptw1_resp_ppn_i = 0;
  dut->ptw1_resp_rsvd_for_sw_i = 0;
  dut->ptw1_resp_d_i = 0;
  dut->ptw1_resp_a_i = 0;
  dut->ptw1_resp_g_i = 0;
  dut->ptw1_resp_u_i = 0;
  dut->ptw1_resp_x_i = 0;
  dut->ptw1_resp_w_i = 0;
  dut->ptw1_resp_r_i = 0;
  dut->ptw1_resp_v_i = 0;
  dut->ptw1_resp_valid_i = 0;
}

void clear_sram_resp(DUT* dut) {
  dut->sram_resp_valid_i = 0;
  dut->sram_resp_qw0_i = 0;
  dut->sram_resp_qw1_i = 0;
  dut->sram_resp_qw2_i = 0;
  dut->sram_resp_qw3_i = 0;
  dut->sram_resp_qw4_i = 0;
  dut->sram_resp_qw5_i = 0;
  dut->sram_resp_qw6_i = 0;
  dut->sram_resp_qw7_i = 0;
}

void clear_inputs(DUT* dut) {
  dut->ioshire_i = 1;
  dut->prefetch_prv_i = kPrvS;
  dut->prefetch_start_addr_i = 0;
  dut->prefetch_num_lines_i = 0;
  dut->prefetch_start_i = 0;
  dut->err_rsp_enable_i = 0;
  dut->err_interrupt_enable_i = 0;
  dut->mprot_i = 0;
  dut->vmspagesize_i = kPage4K;
  dut->bypass_icache_i = 0;
  dut->coop_mode_i = 0;
  dut->req0_valid_i = 0;
  dut->req0_min_id_i = 0;
  dut->req0_addr_i = 0;
  dut->req0_prv_i = kPrvS;
  dut->req0_mprv_i = 0;
  dut->req0_mpp_i = kPrvM;
  dut->req0_sum_i = 0;
  dut->req0_mxr_i = 0;
  dut->req0_debug_i = 0;
  dut->req1_valid_i = 0;
  dut->req1_min_id_i = 0;
  dut->req1_addr_i = 0;
  dut->req1_prv_i = kPrvS;
  dut->req1_mprv_i = 0;
  dut->req1_mpp_i = kPrvM;
  dut->req1_sum_i = 0;
  dut->req1_mxr_i = 0;
  dut->req1_debug_i = 0;
  dut->flush_data_i = 0;
  dut->l2_miss_req_disable_next_i = 0;
  dut->l2_miss_req_ready_i = 0;
  clear_l2_resp(dut);
  dut->satp_mode0_i = kModeBare;
  dut->satp_ppn0_i = 0;
  dut->satp_mode1_i = kModeBare;
  dut->satp_ppn1_i = 0;
  dut->matp_mode0_i = kModeBare;
  dut->matp_ppn0_i = 0;
  dut->matp_mode1_i = kModeBare;
  dut->matp_ppn1_i = 0;
  dut->tlb_invalidate_i = 0;
  dut->ptw0_req_ready_i = 0;
  dut->ptw1_req_ready_i = 0;
  clear_ptw0_resp(dut);
  clear_ptw1_resp(dut);
  dut->sram_req_ready_i = 1;
  clear_sram_resp(dut);
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

void pulse_req0(SimCtrl<DUT>& sim, uint64_t addr, uint8_t prv) {
  sim.dut->req0_addr_i = addr;
  sim.dut->req0_prv_i = prv;
  sim.dut->req0_valid_i = 1;
  sim.tick();
  sim.dut->req0_valid_i = 0;
}

void pulse_req1(SimCtrl<DUT>& sim, uint64_t addr, uint8_t prv) {
  sim.dut->req1_addr_i = addr;
  sim.dut->req1_prv_i = prv;
  sim.dut->req1_valid_i = 1;
  sim.tick();
  sim.dut->req1_valid_i = 0;
}

void drive_l2_ack(SimCtrl<DUT>& sim) {
  sim.dut->l2_miss_resp_opcode_i = kRspAckData;
  sim.dut->l2_miss_resp_valid_i = 1;
  sim.tick();
  clear_l2_resp(sim.dut.get());
}

void drive_sram_resp(SimCtrl<DUT>& sim, const BlockWords& words) {
  sim.dut->sram_resp_valid_i = 1;
  sim.dut->sram_resp_qw0_i = words[0];
  sim.dut->sram_resp_qw1_i = words[1];
  sim.dut->sram_resp_qw2_i = words[2];
  sim.dut->sram_resp_qw3_i = words[3];
  sim.dut->sram_resp_qw4_i = words[4];
  sim.dut->sram_resp_qw5_i = words[5];
  sim.dut->sram_resp_qw6_i = words[6];
  sim.dut->sram_resp_qw7_i = words[7];
  sim.tick();
  clear_sram_resp(sim.dut.get());
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

  const BlockWords zero_fill = {0, 0, 0, 0, 0, 0, 0, 0};

  sim.check(sim.dut->prefetch_done_o == 1, "prefetch done high after reset");
  sim.check(sim.dut->req0_ready_o == 1, "requestor 0 ready after reset");
  sim.check(sim.dut->req1_ready_o == 1, "requestor 1 ready after reset");
  sim.check(sim.dut->resp0_valid_o == 0, "no requestor 0 response after reset");
  sim.check(sim.dut->resp1_valid_o == 0, "no requestor 1 response after reset");
  sim.check(sim.dut->l2_miss_req_valid_o == 0, "no L2 miss request after reset");

  pulse_req0(sim, kSpRomAddr, kPrvS);

  if (wait_until(sim, [](DUT* dut) { return dut->resp0_valid_o; }, 16, "cold requestor 0 access responds")) {
    sim.check(sim.dut->resp0_miss_o == 1, "cold requestor 0 access reports miss");
  }

  if (wait_until(sim, [](DUT* dut) { return dut->l2_miss_req_valid_o; }, 16, "cold requestor 0 access issues L2 miss request")) {
    sim.check(sim.dut->l2_miss_req_addr_o == kSpRomAddr, "L2 miss request forwards requestor 0 address");
    sim.check(sim.dut->l2_miss_req_opcode_o == 1, "L2 miss request uses read opcode");
    sim.check(sim.dut->l2_miss_req_size_o == 6, "L2 miss request uses line size");
    sim.check(sim.dut->l2_miss_req_subopcode_o == 0, "L2 miss request uses Read_L2 subopcode");
  }

  sim.dut->l2_miss_req_ready_i = 1;
  sim.tick();
  sim.dut->l2_miss_req_ready_i = 0;

  if (wait_until(sim, [](DUT* dut) { return dut->sram_req_valid_o; }, 8, "miss flow issues shared SRAM request")) {
    sim.check(sim.dut->sram_req_write_o == 1, "miss fill writes shared SRAM");
  }

  drive_l2_ack(sim);
  drive_sram_resp(sim, zero_fill);

  if (wait_until(sim, [](DUT* dut) { return dut->resp0_fill_done_o; }, 16, "miss flow completes fill")) {
    sim.check(sim.dut->resp0_fill_done_o == 1, "requestor 0 fill done pulses");
  }

  pulse_req0(sim, kSpRomAddr, kPrvS);

  bool saw_shared_sram_hit = false;
  bool saw_replay_hit = false;
  for (int i = 0; i < 16; ++i) {
    if (sim.dut->sram_req_valid_o) saw_shared_sram_hit = true;
    if (sim.dut->resp0_valid_o && !sim.dut->resp0_miss_o) {
      saw_replay_hit = true;
      break;
    }
    sim.tick();
  }
  sim.check(!saw_shared_sram_hit, "replay hit stays inside the micro-cache");
  sim.check(saw_replay_hit, "requestor 0 replay hits");

  if (saw_replay_hit) {
    sim.check(sim.dut->resp0_data_0_o == 0, "requestor 0 hit returns qword 0");
    sim.check(sim.dut->resp0_data_1_o == 0, "requestor 0 hit returns qword 1");
    sim.check(sim.dut->resp0_data_2_o == 0, "requestor 0 hit returns qword 2");
    sim.check(sim.dut->resp0_data_3_o == 0, "requestor 0 hit returns qword 3");
    sim.check(sim.dut->resp0_cacheable_o == 1, "requestor 0 hit is cacheable");
    sim.check(sim.dut->resp0_page_fault_o == 0, "requestor 0 hit has no page fault");
    sim.check(sim.dut->resp0_access_fault_o == 0, "requestor 0 hit has no access fault");
    sim.check(sim.dut->resp0_bus_err_o == 0, "requestor 0 hit has no bus error");
    sim.check(sim.dut->resp0_ecc_err_o == 0, "requestor 0 hit has no ECC error");
  }

  reset_and_clear(sim);
  sim.dut->ioshire_i = 0;
  sim.dut->satp_mode1_i = kModeSv39;
  sim.dut->satp_ppn1_i = 0x0010000;
  pulse_req1(sim, kVmReqAddr, kPrvS);

  if (wait_until(sim, [](DUT* dut) { return dut->ptw1_req_valid_o; }, 16, "requestor 1 VM access issues PTW request")) {
    sim.check(sim.dut->ptw1_req_satp_mode_o == kModeSv39, "requestor 1 PTW forwards SATP mode");
    sim.check(sim.dut->ptw1_req_satp_ppn_o == 0x0010000, "requestor 1 PTW forwards SATP ppn");
    sim.check(sim.dut->ptw1_req_prv_o == kPrvS, "requestor 1 PTW forwards privilege");
    sim.check(sim.dut->ptw1_req_addr_o == (kVmReqAddr >> 12), "requestor 1 PTW forwards VPN");
    sim.check(sim.dut->ptw0_req_valid_o == 0, "requestor 0 PTW stays idle");
  }

  return sim.finish();
}
