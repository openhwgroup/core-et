// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vneigh_fill_ff_tb.h"
#include "sim_ctrl.h"

using DUT = Vneigh_fill_ff_tb;

namespace {

constexpr uint64_t kLineLo0 = 0x1111111111111111ull;
constexpr uint64_t kLineLo1 = 0x2222222222222222ull;
constexpr uint64_t kLineLo2 = 0x3333333333333333ull;
constexpr uint64_t kLineLo3 = 0x4444444444444444ull;
constexpr uint64_t kLineHi0 = 0xaaaaaaaaaaaaaaaaull;
constexpr uint64_t kLineHi1 = 0xbbbbbbbbbbbbbbbbull;
constexpr uint64_t kLineHi2 = 0xccccccccccccccccull;
constexpr uint64_t kLineHi3 = 0xddddddddddddddddull;

void clear_inputs(DUT* dut) {
  dut->fill_fifo_id_i = 0;
  dut->fill_fifo_dest_i = 0;
  dut->fill_fifo_wdata_i = 0;
  dut->fill_fifo_opcode_i = 0;
  dut->fill_fifo_data0_i = 0;
  dut->fill_fifo_data1_i = 0;
  dut->fill_fifo_data2_i = 0;
  dut->fill_fifo_data3_i = 0;
  dut->fill_fifo_data4_i = 0;
  dut->fill_fifo_data5_i = 0;
  dut->fill_fifo_data6_i = 0;
  dut->fill_fifo_data7_i = 0;
  dut->fill_fifo_size_i = 0;
  dut->fill_fifo_qwen_i = 0;
  dut->fill_fifo_valid_i = 0;

  dut->tstore_id_i = 0;
  dut->tstore_dest_i = 0;
  dut->tstore_wdata_i = 0;
  dut->tstore_opcode_i = 0;
  dut->tstore_data0_i = 0;
  dut->tstore_data1_i = 0;
  dut->tstore_data2_i = 0;
  dut->tstore_data3_i = 0;
  dut->tstore_size_i = 0;
  dut->tstore_qwen_i = 0;
  dut->tstore_valid_i = 0;

  dut->local_id_i = 0;
  dut->local_dest_i = 0;
  dut->local_wdata_i = 0;
  dut->local_opcode_i = 0;
  dut->local_data0_i = 0;
  dut->local_data1_i = 0;
  dut->local_data2_i = 0;
  dut->local_data3_i = 0;
  dut->local_size_i = 0;
  dut->local_qwen_i = 0;
  dut->local_valid_i = 0;

  dut->tbox_id_i = 0;
  dut->tbox_dest_i = 0;
  dut->tbox_wdata_i = 0;
  dut->tbox_opcode_i = 0;
  dut->tbox_data0_i = 0;
  dut->tbox_data1_i = 0;
  dut->tbox_data2_i = 0;
  dut->tbox_data3_i = 0;
  dut->tbox_size_i = 0;
  dut->tbox_qwen_i = 0;
  dut->tbox_valid_i = 0;

  dut->l2_resp_ready_i = 1;
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  clear_inputs(sim.dut.get());
  sim.reset();

  sim.check(sim.dut->fill_fifo_ready_o == 1, "fill fifo ready after reset");
  sim.check(sim.dut->tstore_ready_o == 1, "tstore ready after reset");
  sim.check(sim.dut->local_ready_o == 0, "local message not pre-reserved after reset");
  sim.check(sim.dut->tbox_ready_o == 0, "tbox message not pre-reserved after reset");
  sim.check(sim.dut->l2_resp_valid_o == 0, "no output valid after reset");

  // Line fill: two 256-bit responses to minion.
  sim.dut->fill_fifo_id_i = 0x12;
  sim.dut->fill_fifo_dest_i = 0x1;  // thread_id = 1
  sim.dut->fill_fifo_wdata_i = 1;
  sim.dut->fill_fifo_opcode_i = 1;  // AckData
  sim.dut->fill_fifo_data0_i = kLineLo0;
  sim.dut->fill_fifo_data1_i = kLineLo1;
  sim.dut->fill_fifo_data2_i = kLineLo2;
  sim.dut->fill_fifo_data3_i = kLineLo3;
  sim.dut->fill_fifo_data4_i = kLineHi0;
  sim.dut->fill_fifo_data5_i = kLineHi1;
  sim.dut->fill_fifo_data6_i = kLineHi2;
  sim.dut->fill_fifo_data7_i = kLineHi3;
  sim.dut->fill_fifo_size_i = 6;  // line
  sim.dut->fill_fifo_qwen_i = 0x3;
  sim.dut->fill_fifo_valid_i = 1;
  sim.tick();
  sim.check(sim.dut->l2_resp_valid_o == 0, "line fill is queued before first response appears");

  sim.dut->fill_fifo_valid_i = 0;
  sim.tick();
  sim.check(sim.dut->l2_resp_valid_o == 1, "first line chunk becomes valid");
  sim.check(sim.dut->l2_resp_id_o == 0x12, "first chunk keeps id");
  sim.check(sim.dut->l2_resp_dest_o == 1, "first chunk uses thread id from dest");
  sim.check(sim.dut->l2_resp_opcode_o == 1, "first chunk keeps opcode");
  sim.check(sim.dut->l2_resp_data0_o == kLineLo0, "first chunk word0");
  sim.check(sim.dut->l2_resp_data1_o == kLineLo1, "first chunk word1");
  sim.check(sim.dut->l2_resp_data2_o == kLineLo2, "first chunk word2");
  sim.check(sim.dut->l2_resp_data3_o == kLineLo3, "first chunk word3");

  sim.tick();
  sim.check(sim.dut->l2_resp_valid_o == 1, "second line chunk stays valid");
  sim.check(sim.dut->l2_resp_data0_o == kLineHi0, "second chunk word0");
  sim.check(sim.dut->l2_resp_data1_o == kLineHi1, "second chunk word1");
  sim.check(sim.dut->l2_resp_data2_o == kLineHi2, "second chunk word2");
  sim.check(sim.dut->l2_resp_data3_o == kLineHi3, "second chunk word3");

  sim.tick();
  sim.check(sim.dut->l2_resp_valid_o == 0, "line fill drains after second chunk");

  // Local message reservation path.
  sim.dut->local_id_i = 0x44;
  sim.dut->local_dest_i = 0x1;  // thread_id = 1
  sim.dut->local_wdata_i = 1;
  sim.dut->local_opcode_i = 2;
  sim.dut->local_data0_i = 0xdeadbeefcafef00dull;
  sim.dut->local_data1_i = 0x0123456789abcdefull;
  sim.dut->local_data2_i = 0x55aa55aa55aa55aaull;
  sim.dut->local_data3_i = 0xaa55aa55aa55aa55ull;
  sim.dut->local_size_i = 5;  // hline / 256b neighborhood data
  sim.dut->local_qwen_i = 0xf;
  sim.dut->local_valid_i = 1;
  sim.tick();
  sim.check(sim.dut->local_ready_o == 1, "local message reservation goes active");
  sim.check(sim.dut->l2_resp_valid_o == 0, "reserved local message not yet output");

  sim.tick();
  sim.dut->local_valid_i = 0;
  sim.tick();
  sim.check(sim.dut->l2_resp_valid_o == 1, "local message delivered");
  sim.check(sim.dut->l2_resp_opcode_o == 2, "local message converts to msg opcode");
  sim.check(sim.dut->l2_resp_dest_o == 1, "local message uses encoded thread id");
  sim.check(sim.dut->l2_resp_data0_o == 0xdeadbeefcafef00dull, "local message data word0");
  sim.check(sim.dut->l2_resp_data3_o == 0xaa55aa55aa55aa55ull, "local message data word3");

  sim.tick();
  sim.check(sim.dut->l2_resp_valid_o == 0, "local message drains in one pass");

  // Tensor-store cooperative ACK.
  sim.dut->tstore_id_i = 0x01;
  sim.dut->tstore_dest_i = 0;
  sim.dut->tstore_wdata_i = 0;
  sim.dut->tstore_opcode_i = 0;  // Ack
  sim.dut->tstore_size_i = 2;
  sim.dut->tstore_qwen_i = 0;
  sim.dut->tstore_valid_i = 1;
  sim.tick();
  sim.dut->tstore_valid_i = 0;
  sim.tick();
  sim.check(sim.dut->l2_resp_valid_o == 1, "tstore ack becomes visible");
  sim.check(sim.dut->l2_resp_opcode_o == 0, "tstore opcode is ack");
  sim.check(sim.dut->tstore_min_ack_o == 1, "tstore ack pulse asserted");

  sim.tick();
  sim.check(sim.dut->l2_resp_valid_o == 0, "tstore ack drains");
  sim.check(sim.dut->tstore_min_ack_o == 0, "tstore ack pulse clears");

  return sim.finish();
}
