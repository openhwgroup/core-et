// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vptw_top_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vptw_top_tb;

namespace {

constexpr uint8_t kSatpModeSv39 = 8;
constexpr uint8_t kPrvS = 1;
constexpr uint8_t kVmPage4K = 0;
constexpr uint8_t kVmPage1G = 3;
constexpr uint8_t kReqRead = 1;
constexpr uint8_t kRspAckData = 1;
constexpr uint8_t kSizeLine = 6;
constexpr uint32_t kRootPpn = 0x40480u;
constexpr uint32_t kNextPpn = 0x40481u;
constexpr uint32_t kLeafPpn1G = 0x40000u;
constexpr uint32_t kLeafPpn4K = 0x30000u;
constexpr uint64_t kPpnMask = (1ULL << 44) - 1ULL;

uint64_t make_pte(uint64_t ppn, bool r, bool w, bool x, bool a = false, bool d = false) {
  return (ppn << 10)
       | (static_cast<uint64_t>(d) << 7)
       | (static_cast<uint64_t>(a) << 6)
       | (static_cast<uint64_t>(x) << 3)
       | (static_cast<uint64_t>(w) << 2)
       | (static_cast<uint64_t>(r) << 1)
       | 1ULL;
}

uint64_t resp_ppn(uint64_t resp) {
  return (resp >> 10) & kPpnMask;
}

bool resp_access_fault(uint64_t resp) {
  return ((resp >> 62) & 1ULL) != 0;
}

bool resp_canceled(uint64_t resp) {
  return ((resp >> 63) & 1ULL) != 0;
}

void clear_fill(DUT* dut) {
  dut->mem_fill_req_id_i = 0;
  dut->mem_fill_req_dest_i = 0;
  dut->mem_fill_req_wdata_i = 0;
  dut->mem_fill_req_opcode_i = 0;
  dut->mem_fill_req_data_0_i = 0;
  dut->mem_fill_req_data_1_i = 0;
  dut->mem_fill_req_data_2_i = 0;
  dut->mem_fill_req_data_3_i = 0;
  dut->mem_fill_req_data_4_i = 0;
  dut->mem_fill_req_data_5_i = 0;
  dut->mem_fill_req_data_6_i = 0;
  dut->mem_fill_req_data_7_i = 0;
  dut->mem_fill_req_size_i = 0;
  dut->mem_fill_req_qwen_i = 0;
  dut->mem_fill_req_valid_i = 0;
}

void clear_req(DUT* dut) {
  dut->ioshire_i = 0;
  dut->req_satp_mode_i = 0;
  dut->req_satp_ppn_i = 0;
  dut->req_prv_i = 0;
  dut->req_addr_i = 0;
  dut->req_valid_i = 0;
  dut->invalidate_i = 0;
  dut->mprot_enable_secure_memory_i = 0;
  dut->mprot_dram_size_i = 0;
  dut->mprot_disable_osbox_access_i = 0;
  dut->mprot_disable_pcie_access_i = 0;
  dut->mprot_io_access_mode_i = 0;
  dut->vmspagesize_i = 0;
  dut->mem_miss_req_disable_next_i = 0;
  dut->mem_miss_req_ready_i = 0;
}

void clear_inputs(DUT* dut) {
  clear_req(dut);
  clear_fill(dut);
}

void set_request(DUT* dut, bool ioshire, uint8_t vmspagesize, uint32_t satp_ppn) {
  dut->ioshire_i = ioshire ? 1 : 0;
  dut->req_satp_mode_i = kSatpModeSv39;
  dut->req_satp_ppn_i = satp_ppn;
  dut->req_prv_i = kPrvS;
  dut->req_addr_i = 0;
  dut->req_valid_i = 1;
  dut->vmspagesize_i = vmspagesize;
}

void set_fill_line(DUT* dut, uint64_t pte0) {
  dut->mem_fill_req_id_i = 0;
  dut->mem_fill_req_dest_i = 0;
  dut->mem_fill_req_wdata_i = 0;
  dut->mem_fill_req_opcode_i = kRspAckData;
  dut->mem_fill_req_data_0_i = pte0;
  dut->mem_fill_req_data_1_i = 0;
  dut->mem_fill_req_data_2_i = 0;
  dut->mem_fill_req_data_3_i = 0;
  dut->mem_fill_req_data_4_i = 0;
  dut->mem_fill_req_data_5_i = 0;
  dut->mem_fill_req_data_6_i = 0;
  dut->mem_fill_req_data_7_i = 0;
  dut->mem_fill_req_size_i = kSizeLine;
  dut->mem_fill_req_qwen_i = 0;
  dut->mem_fill_req_valid_i = 1;
}

void reset_and_idle(SimCtrl<DUT>& sim) {
  clear_inputs(sim.dut.get());
  sim.reset(2);
  clear_inputs(sim.dut.get());
  sim.dut->eval();
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  auto* dut = sim.dut.get();

  reset_and_idle(sim);

  sim.check(dut->req_ready_o == 1, "reset leaves PTW ready");
  sim.check(dut->resp_valid_o == 0, "reset clears response valid");
  sim.check(dut->mem_miss_req_valid_o == 0, "reset clears miss valid");
  sim.check(dut->mem_fill_req_ready_o == 1, "fill side is always ready");

  set_request(dut, false, kVmPage1G, kRootPpn);
  sim.tick();
  sim.check(dut->req_ready_o == 0, "accepted request leaves ready state");
  dut->req_valid_i = 0;
  sim.tick();
  sim.check(dut->mem_miss_req_valid_o == 1, "faulting request still exposes the original transient miss pulse");
  sim.tick();
  sim.check(dut->resp_valid_o == 1, "faulting request still returns a response");
  sim.check(resp_access_fault(dut->resp_data_o), "fault response sets access_fault");
  sim.check(!resp_canceled(dut->resp_data_o), "fault response is not marked canceled");

  reset_and_idle(sim);

  set_request(dut, true, kVmPage1G, kRootPpn);
  sim.tick();
  dut->req_valid_i = 0;
  dut->mem_miss_req_ready_i = 1;
  sim.tick();
  sim.check(dut->mem_miss_req_valid_o == 1, "legal request issues miss");
  sim.check(dut->mem_miss_req_opcode_o == kReqRead, "PTW miss opcode is Read");
  sim.check(dut->mem_miss_req_address_o == 0x40480000ULL, "PTW miss targets root table line");
  sim.check(dut->mem_miss_req_size_o == kSizeLine, "PTW miss requests a full line");
  sim.check(dut->mem_miss_req_qwen_o == 0, "PTW miss has zero qwen");
  sim.check(dut->mem_miss_req_subopcode_o == 0, "PTW miss selects the L2 read subopcode");
  sim.check(dut->mem_miss_req_data_0_o == 0, "PTW miss carries no payload data chunk 0");
  sim.check(dut->mem_miss_req_data_1_o == 0, "PTW miss carries no payload data chunk 1");
  sim.check(dut->mem_miss_req_data_2_o == 0, "PTW miss carries no payload data chunk 2");
  sim.check(dut->mem_miss_req_data_3_o == 0, "PTW miss carries no payload data chunk 3");

  set_fill_line(dut, make_pte(kLeafPpn1G, true, false, false));
  sim.tick();
  clear_fill(dut);
  sim.tick();
  sim.tick();
  sim.check(dut->resp_valid_o == 1, "1G leaf walk completes");
  sim.check(!resp_access_fault(dut->resp_data_o), "1G leaf walk has no access fault");
  sim.check(resp_ppn(dut->resp_data_o) == kLeafPpn1G, "1G leaf walk returns the expected PPN");

  reset_and_idle(sim);

  set_request(dut, true, kVmPage4K, kRootPpn);
  sim.tick();
  dut->req_valid_i = 0;
  dut->mem_miss_req_ready_i = 1;
  sim.tick();
  sim.check(dut->mem_miss_req_valid_o == 1, "4K walk issues root-table miss");
  sim.check(dut->mem_miss_req_address_o == 0x40480000ULL, "4K walk first miss uses root table");

  set_fill_line(dut, make_pte(kNextPpn, false, false, false));
  sim.tick();
  clear_fill(dut);
  sim.tick();

  dut->mem_miss_req_ready_i = 1;
  sim.tick();
  sim.check(dut->mem_miss_req_valid_o == 1, "table PTE causes second-level miss");
  sim.check(dut->mem_miss_req_address_o == 0x40481000ULL, "4K walk second miss uses next-level table");

  set_fill_line(dut, make_pte(kLeafPpn4K, true, false, false));
  sim.tick();
  clear_fill(dut);
  sim.tick();
  sim.tick();
  sim.check(dut->resp_valid_o == 1, "4K walk completes after second-level fill");
  sim.check(resp_ppn(dut->resp_data_o) == kLeafPpn4K, "4K walk returns final leaf PPN");

  set_request(dut, true, kVmPage4K, kRootPpn);
  sim.tick();
  dut->req_valid_i = 0;
  dut->mem_miss_req_ready_i = 0;
  sim.tick();
  sim.check(dut->mem_miss_req_valid_o == 0, "cached root PTE suppresses a repeated root miss");
  sim.tick();
  sim.check(dut->mem_miss_req_valid_o == 0, "cached L1 line suppresses the repeated second-level miss");
  sim.tick();
  sim.tick();
  sim.check(dut->resp_valid_o == 1, "cached walk still returns a response");
  sim.check(resp_ppn(dut->resp_data_o) == kLeafPpn4K, "cached walk returns the same leaf PPN");

  return sim.finish();
}
