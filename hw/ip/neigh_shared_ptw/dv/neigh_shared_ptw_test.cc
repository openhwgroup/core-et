// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vneigh_shared_ptw_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vneigh_shared_ptw_tb;

namespace {

constexpr uint8_t kSatpModeSv39 = 8;
constexpr uint8_t kPrvS = 1;
constexpr uint8_t kVmPage1G = 3;
constexpr uint8_t kReqRead = 1;
constexpr uint8_t kRspAckData = 1;
constexpr uint8_t kSizeLine = 6;
constexpr uint32_t kRootPpn0 = 0x40480u;
constexpr uint32_t kRootPpn1 = 0x40490u;
constexpr uint32_t kLeafPpn0 = 0x40000u;
constexpr uint32_t kLeafPpn1 = 0x80000u;
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

void clear_requests(DUT* dut) {
  for (int i = 0; i < 5; ++i) {
    dut->req_satp_mode_i[i] = 0;
    dut->req_satp_ppn_i[i] = 0;
    dut->req_prv_i[i] = 0;
    dut->req_addr_i[i] = 0;
  }
  dut->req_valid_i = 0;
  dut->invalidate_i = 0;
}

void clear_inputs(DUT* dut) {
  dut->ioshire_i = 0;
  clear_requests(dut);
  dut->mprot_enable_secure_memory_i = 0;
  dut->mprot_dram_size_i = 0;
  dut->mprot_disable_osbox_access_i = 0;
  dut->mprot_disable_pcie_access_i = 0;
  dut->mprot_io_access_mode_i = 0;
  dut->vmspagesize_i = 0;
  dut->mem_miss_req_disable_next_i = 0;
  dut->mem_miss_req_ready_i = 0;
  clear_fill(dut);
}

void set_request(DUT* dut, int slot, uint32_t satp_ppn, uint32_t addr) {
  dut->req_satp_mode_i[slot] = kSatpModeSv39;
  dut->req_satp_ppn_i[slot] = satp_ppn;
  dut->req_prv_i[slot] = kPrvS;
  dut->req_addr_i[slot] = addr;
  dut->req_valid_i |= static_cast<uint8_t>(1u << slot);
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

  sim.check(dut->req_ready_o == 0, "reset leaves all request grants low");
  sim.check(dut->resp_valid_o == 0, "reset clears routed response valids");
  sim.check(dut->mem_fill_req_ready_o == 1, "shared PTW fill side is always ready");

  dut->ioshire_i = 1;
  dut->vmspagesize_i = kVmPage1G;
  set_request(dut, 1, kRootPpn0, 0x12);
  set_request(dut, 4, kRootPpn1, 0x34);
  dut->eval();
  sim.check(dut->req_ready_o == 0x02, "lowest-index requester wins first after reset");
  sim.check(dut->dbg_req_satp_mode_o == kSatpModeSv39, "debug mirrors the winning satp mode");
  sim.check(dut->dbg_req_prv_o == kPrvS, "debug mirrors the winning privilege");
  sim.check(dut->dbg_req_addr_o == 0x12, "debug mirrors the winning address");

  sim.tick();
  dut->req_valid_i = 0;
  dut->mem_miss_req_ready_i = 1;
  sim.tick();
  sim.check(dut->mem_miss_req_valid_o == 1, "first routed request issues a PTW miss");
  sim.check(dut->mem_miss_req_opcode_o == kReqRead, "shared PTW miss opcode is Read");
  sim.check(dut->mem_miss_req_address_o == 0x40480000ULL, "winner 1 uses its root table address");

  set_fill_line(dut, make_pte(kLeafPpn0, true, false, false));
  sim.tick();
  clear_fill(dut);
  sim.tick();
  sim.tick();
  sim.check(dut->resp_valid_o == 0x02, "first response is routed back to requester 1");
  sim.check(resp_ppn(dut->resp_data_o) == kLeafPpn0, "first routed response keeps PTW payload");

  clear_requests(dut);
  set_request(dut, 1, kRootPpn0, 0x55);
  set_request(dut, 4, kRootPpn1, 0x66);
  dut->eval();
  sim.check(dut->req_ready_o == 0x10, "LRU update makes requester 4 the next winner");
  sim.check(dut->dbg_req_addr_o == 0x66, "debug now mirrors requester 4");

  sim.tick();
  dut->req_valid_i = 0;
  dut->mem_miss_req_ready_i = 1;
  sim.tick();
  sim.check(dut->mem_miss_req_valid_o == 1, "second routed request also issues a PTW miss");
  sim.check(dut->mem_miss_req_address_o == 0x40490000ULL, "winner 4 uses its own root table address");

  set_fill_line(dut, make_pte(kLeafPpn1, true, false, false));
  sim.tick();
  clear_fill(dut);
  sim.tick();
  sim.tick();
  sim.check(dut->resp_valid_o == 0x10, "second response is routed back to requester 4");
  sim.check(resp_ppn(dut->resp_data_o) == kLeafPpn1, "second routed response keeps PTW payload");

  return sim.finish();
}
