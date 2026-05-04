// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vneigh_fill_fifo_tb.h"
#include "sim_ctrl.h"

using DUT = Vneigh_fill_fifo_tb;

namespace {

constexpr uint64_t kWord0 = 0x1111111111111111ull;
constexpr uint64_t kWord1 = 0x2222222222222222ull;
constexpr uint64_t kWord2 = 0x3333333333333333ull;
constexpr uint64_t kWord3 = 0x4444444444444444ull;
constexpr uint64_t kWord4 = 0xaaaaaaaaaaaaaaaaull;
constexpr uint64_t kWord5 = 0xbbbbbbbbbbbbbbbbull;
constexpr uint64_t kWord6 = 0xccccccccccccccccull;
constexpr uint64_t kWord7 = 0xddddddddddddddddull;

void clear_inputs(DUT* dut) {
  dut->push_i = 0;
  dut->push_id_i = 0;
  dut->push_dest_i = 0;
  dut->push_wdata_i = 0;
  dut->push_opcode_i = 0;
  dut->push_data0_i = 0;
  dut->push_data1_i = 0;
  dut->push_data2_i = 0;
  dut->push_data3_i = 0;
  dut->push_data4_i = 0;
  dut->push_data5_i = 0;
  dut->push_data6_i = 0;
  dut->push_data7_i = 0;
  dut->push_size_i = 0;
  dut->push_qwen_i = 0;
  dut->coop_mask_i = 0;
  dut->coop_min_miss_id_i = 0;
  dut->out_rsp_ready_i = 0;
}

void drive_regular_fill(DUT* dut, uint8_t id, uint8_t dest) {
  dut->push_i = 1;
  dut->push_id_i = id;
  dut->push_dest_i = dest;
  dut->push_wdata_i = 1;
  dut->push_opcode_i = 1;  // AckData
  dut->push_data0_i = kWord0;
  dut->push_data1_i = kWord1;
  dut->push_data2_i = kWord2;
  dut->push_data3_i = kWord3;
  dut->push_data4_i = kWord4;
  dut->push_data5_i = kWord5;
  dut->push_data6_i = kWord6;
  dut->push_data7_i = kWord7;
  dut->push_size_i = 6;
  dut->push_qwen_i = 0xf;
}

void drive_coop_fill(DUT* dut) {
  drive_regular_fill(dut, 0x33, 12);
  dut->coop_mask_i = 0x3;
  dut->coop_min_miss_id_i = (5u << 3) | 1u;
}

bool only_agent_valid(DUT* dut, int agent, uint32_t valid_mask) {
  return dut->out_rsp_valid_o == valid_mask && (((valid_mask >> agent) & 1u) == 1u);
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  clear_inputs(sim.dut.get());
  sim.reset();

  sim.check(sim.dut->full_o == 0, "fifo not full after reset");
  sim.check(sim.dut->out_rsp_valid_o == 0, "no outputs valid after reset");

  sim.dut->out_rsp_ready_i = 0xfff;
  drive_regular_fill(sim.dut.get(), 0x22, 3);
  sim.tick();
  sim.check(sim.dut->out_rsp_valid_o == 0, "push cycle does not immediately produce output");

  sim.dut->push_i = 0;
  sim.tick();
  sim.check(sim.dut->out_rsp_valid_o == (1u << 3), "regular fill targets only minion 3");
  sim.check(sim.dut->out_rsp_id_o[3] == 0x22, "regular fill keeps id");
  sim.check(sim.dut->out_rsp_dest_o[3] == 3, "regular fill keeps destination");
  sim.check(sim.dut->out_rsp_opcode_o[3] == 1, "regular fill keeps opcode");
  sim.check(sim.dut->out_rsp_data0_o[3] == kWord0, "regular fill word0 matches");
  sim.check(sim.dut->out_rsp_data7_o[3] == kWord7, "regular fill word7 matches");

  sim.tick();
  sim.check(sim.dut->out_rsp_valid_o == 0, "regular fill drains after one visible cycle");

  clear_inputs(sim.dut.get());
  sim.dut->out_rsp_ready_i = 1u << 0;
  drive_coop_fill(sim.dut.get());
  sim.tick();
  sim.dut->push_i = 0;
  sim.tick();
  sim.check(sim.dut->out_rsp_valid_o == 1u, "first partial coop fill goes to minion 0 only");
  sim.check(sim.dut->out_rsp_id_o[0] == 0x11, "first coop minion id is expanded");
  sim.check(sim.dut->out_rsp_data0_o[0] == kWord0, "first coop payload word0 matches");

  sim.dut->out_rsp_ready_i = 0x3;
  sim.tick();
  sim.check(sim.dut->out_rsp_valid_o == (1u << 1), "remaining partial coop fill goes to minion 1");
  sim.check(sim.dut->out_rsp_id_o[1] == 0x15, "second coop minion id is expanded");
  sim.check(sim.dut->out_rsp_data7_o[1] == kWord7, "second coop payload word7 matches");

  sim.tick();
  sim.check(sim.dut->out_rsp_valid_o == 0, "coop fill drains after second visible cycle");

  return sim.finish();
}
