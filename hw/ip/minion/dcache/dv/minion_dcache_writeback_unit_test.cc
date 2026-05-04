// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test for minion_dcache_writeback_unit.

#include "Vminion_dcache_writeback_unit_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vminion_dcache_writeback_unit_tb;

namespace {

constexpr uint64_t kBank0DataA = 0x1111222233334444ULL;
constexpr uint64_t kBank1DataA = 0x5555666677778888ULL;
constexpr uint64_t kBank2DataA = 0x9999AAAABBBBCCCCULL;
constexpr uint64_t kBank3DataA = 0xDDDDEEEEFFFF0001ULL;

constexpr uint64_t kBank0DataB = 0x0123456789ABCDEFULL;
constexpr uint64_t kBank1DataB = 0xFEDCBA9876543210ULL;
constexpr uint64_t kBank2DataB = 0x0F0E0D0C0B0A0908ULL;
constexpr uint64_t kBank3DataB = 0x8070605040302010ULL;

void clear_inputs(DUT* dut) {
  dut->req_valid_i = 0;
  dut->req_tag_i = 0;
  dut->req_set_i = 0;
  dut->req_way_en_i = 0;
  dut->req_is_tbox_i = 0;
  dut->req_mem_idx_i = 0;
  dut->req_addr_low_i = 0;
  dut->data_req_grant_i = 0;
  dut->data_req_nack_i = 0;
  dut->data_resp_bank0_i = 0;
  dut->data_resp_bank1_i = 0;
  dut->data_resp_bank2_i = 0;
  dut->data_resp_bank3_i = 0;
  dut->l2_req_ready_i = 0;
}

void tick_and_eval(SimCtrl<DUT>& sim) {
  sim.dut->eval();
  sim.tick();
  sim.dut->eval();
}

void reset_and_clear(SimCtrl<DUT>& sim) {
  clear_inputs(sim.dut.get());
  sim.reset(2);
  clear_inputs(sim.dut.get());
  sim.dut->eval();
}

void load_resp_a(DUT* dut) {
  dut->data_resp_bank0_i = kBank0DataA;
  dut->data_resp_bank1_i = kBank1DataA;
  dut->data_resp_bank2_i = kBank2DataA;
  dut->data_resp_bank3_i = kBank3DataA;
}

void load_resp_b(DUT* dut) {
  dut->data_resp_bank0_i = kBank0DataB;
  dut->data_resp_bank1_i = kBank1DataB;
  dut->data_resp_bank2_i = kBank2DataB;
  dut->data_resp_bank3_i = kBank3DataB;
}

}  // namespace

double sc_time_stamp() { return 0.0; }

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);

  reset_and_clear(sim);
  sim.check(sim.dut->req_ready_o == 1, "reset leaves writeback unit ready");
  sim.check(sim.dut->data_req_valid_o == 0, "reset clears data request valid");
  sim.check(sim.dut->l2_req_valid_o == 0, "reset clears L2 request valid");

  sim.dut->req_valid_i = 1;
  sim.dut->req_tag_i = 0x123456789ULL & ((1ULL << 33) - 1);
  sim.dut->req_set_i = 6;
  sim.dut->req_way_en_i = 4;
  sim.dut->eval();
  sim.check(sim.dut->req_ready_o == 1, "idle unit accepts normal writeback request");
  tick_and_eval(sim);
  sim.check(sim.dut->req_ready_o == 0, "accepted request makes unit busy");
  sim.check(sim.dut->data_req_bid_o == 1, "active normal request bids for first data pass");
  sim.check(sim.dut->data_req_addr_o == (6u << 6), "first normal pass addresses low half-line");
  sim.check(sim.dut->data_req_way_o == 2, "way encoder selects requested way");

  sim.dut->data_req_grant_i = 1;
  load_resp_a(sim.dut.get());
  tick_and_eval(sim);
  sim.check(sim.dut->data_req_valid_o == 1, "grant launches following-cycle data request");
  sim.check(sim.dut->data_req_bid_o == 1, "normal request bids for second data pass");
  sim.check(sim.dut->data_req_addr_o == (6u << 6), "second normal pass still sees previous pass index");

  sim.dut->data_req_grant_i = 1;
  load_resp_b(sim.dut.get());
  tick_and_eval(sim);
  sim.check(sim.dut->data_req_valid_o == 1, "second grant keeps request pipeline full");
  sim.check(sim.dut->data_req_addr_o == ((6u << 6) | (1u << 5)), "third cycle exposes toggled pass index bit");

  sim.dut->data_req_grant_i = 0;
  sim.dut->l2_req_ready_i = 1;
  load_resp_b(sim.dut.get());
  tick_and_eval(sim);
  tick_and_eval(sim);
  sim.check(sim.dut->l2_req_valid_o == 1, "normal writeback emits L2 evict after pipeline delay");
  sim.check(sim.dut->l2_req_opcode_o == 0, "normal writeback uses write opcode");
  sim.check(sim.dut->l2_req_size_o == 6, "normal writeback uses line-sized evict");
  sim.check(sim.dut->l2_req_qwen_o == 0xF, "normal writeback enables all qwords");
  sim.check(sim.dut->l2_req_address_o == ((sim.dut->req_tag_i << 7) | (0u << 6)), "normal writeback uses reduced-set address");

  tick_and_eval(sim);
  sim.check(sim.dut->req_ready_o == 0, "normal writeback remains busy until pipeline drains");
  tick_and_eval(sim);
  sim.check(sim.dut->req_ready_o == 1, "normal writeback retires after both passes drain");

  reset_and_clear(sim);
  sim.dut->req_valid_i = 1;
  sim.dut->req_tag_i = 0x111111111ULL & ((1ULL << 33) - 1);
  sim.dut->req_set_i = 3;
  sim.dut->req_way_en_i = 2;
  sim.dut->req_is_tbox_i = 1;
  sim.dut->req_mem_idx_i = 1;
  sim.dut->req_addr_low_i = 0x22;
  sim.dut->eval();
  tick_and_eval(sim);
  sim.check(sim.dut->data_req_bid_o == 1, "tbox request bids for data");
  sim.check(sim.dut->data_req_addr_o == ((3u << 6) | (1u << 5)), "tbox request uses mem_idx instead of pass count");
  sim.dut->data_req_grant_i = 1;
  load_resp_a(sim.dut.get());
  tick_and_eval(sim);
  sim.dut->data_req_grant_i = 0;
  sim.dut->l2_req_ready_i = 1;
  tick_and_eval(sim);
  tick_and_eval(sim);
  sim.check(sim.dut->l2_req_valid_o == 0, "tbox request has not reached the release stage yet");
  tick_and_eval(sim);
  sim.check(sim.dut->l2_req_valid_o == 1, "tbox request emits a single L2 message");
  sim.check(sim.dut->l2_req_opcode_o == 4, "tbox request uses message-send opcode");
  sim.check(sim.dut->l2_req_size_o == 5, "tbox request uses half-line size");
  sim.check(sim.dut->l2_req_qwen_o == 0x3, "tbox request enables low half qwords only");
  sim.check(sim.dut->l2_req_address_o == ((sim.dut->req_tag_i << 7) | 0x22u), "tbox request uses tag plus addr_low");
  tick_and_eval(sim);
  sim.check(sim.dut->req_ready_o == 1, "tbox request retires after single pass");

  reset_and_clear(sim);
  sim.dut->req_valid_i = 1;
  sim.dut->req_tag_i = 0x200000001ULL & ((1ULL << 33) - 1);
  sim.dut->req_set_i = 1;
  sim.dut->req_way_en_i = 1;
  sim.dut->eval();
  tick_and_eval(sim);
  sim.dut->data_req_grant_i = 1;
  tick_and_eval(sim);
  sim.dut->data_req_grant_i = 0;
  sim.dut->data_req_nack_i = 1;
  tick_and_eval(sim);
  sim.dut->data_req_nack_i = 0;
  sim.dut->l2_req_ready_i = 1;
  tick_and_eval(sim);
  sim.check(sim.dut->l2_req_valid_o == 0, "nacked data pass suppresses L2 request");
  sim.check(sim.dut->data_req_bid_o == 1, "nacked data pass causes request replay bidding");

  reset_and_clear(sim);
  sim.dut->req_valid_i = 1;
  sim.dut->req_tag_i = 0x300000001ULL & ((1ULL << 33) - 1);
  sim.dut->req_set_i = 2;
  sim.dut->req_way_en_i = 8;
  sim.dut->eval();
  tick_and_eval(sim);
  sim.dut->data_req_grant_i = 1;
  tick_and_eval(sim);
  sim.dut->data_req_grant_i = 1;
  tick_and_eval(sim);
  sim.dut->data_req_grant_i = 0;
  sim.dut->l2_req_ready_i = 0;
  tick_and_eval(sim);
  tick_and_eval(sim);
  sim.check(sim.dut->l2_req_valid_o == 1, "backpressured R3 stage still asks L2 for release");
  sim.check(sim.dut->data_req_valid_o == 0, "L2 backpressure cancels further data requests");
  tick_and_eval(sim);
  sim.check(sim.dut->data_req_bid_o == 1, "L2 backpressure rolls request count back for replay");

  return sim.finish();
}
