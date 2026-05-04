// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_fill_ff_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>
#include <cstdio>

using DUT = Vcosim_neigh_fill_ff_tb;

namespace {

uint32_t rng_state = 0x46696c6cU;

uint32_t xorshift32() {
  rng_state ^= rng_state << 13;
  rng_state ^= rng_state >> 17;
  rng_state ^= rng_state << 5;
  return rng_state;
}

uint64_t rand64() {
  return (uint64_t(xorshift32()) << 32) | xorshift32();
}

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

void compare_outputs(CosimCtrl<DUT>& sim) {
  auto* dut = sim.dut.get();
  sim.compare("fill_fifo_ready", dut->orig_fill_fifo_ready_o, dut->new_fill_fifo_ready_o);
  sim.compare("tstore_ready", dut->orig_tstore_ready_o, dut->new_tstore_ready_o);
  sim.compare("local_ready", dut->orig_local_ready_o, dut->new_local_ready_o);
  sim.compare("tbox_ready", dut->orig_tbox_ready_o, dut->new_tbox_ready_o);
  sim.compare("l2_resp_id", dut->orig_l2_resp_id_o, dut->new_l2_resp_id_o);
  sim.compare("l2_resp_dest", dut->orig_l2_resp_dest_o, dut->new_l2_resp_dest_o);
  sim.compare("l2_resp_wdata", dut->orig_l2_resp_wdata_o, dut->new_l2_resp_wdata_o);
  sim.compare("l2_resp_opcode", dut->orig_l2_resp_opcode_o, dut->new_l2_resp_opcode_o);
  sim.compare("l2_resp_data0", dut->orig_l2_resp_data0_o, dut->new_l2_resp_data0_o);
  sim.compare("l2_resp_data1", dut->orig_l2_resp_data1_o, dut->new_l2_resp_data1_o);
  sim.compare("l2_resp_data2", dut->orig_l2_resp_data2_o, dut->new_l2_resp_data2_o);
  sim.compare("l2_resp_data3", dut->orig_l2_resp_data3_o, dut->new_l2_resp_data3_o);
  sim.compare("l2_resp_size", dut->orig_l2_resp_size_o, dut->new_l2_resp_size_o);
  sim.compare("l2_resp_qwen", dut->orig_l2_resp_qwen_o, dut->new_l2_resp_qwen_o);
  sim.compare("l2_resp_valid", dut->orig_l2_resp_valid_o, dut->new_l2_resp_valid_o);
  sim.compare("tstore_min_ack", dut->orig_tstore_min_ack_o, dut->new_tstore_min_ack_o);
  sim.compare("dbg_sm_signals", dut->orig_dbg_sm_signals_o, dut->new_dbg_sm_signals_o);
}

void tick_and_compare(CosimCtrl<DUT>& sim) {
  sim.tick();
  compare_outputs(sim);
}

}  // namespace

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  auto* dut = sim.dut.get();

  clear_inputs(dut);
  sim.reset(2);
  compare_outputs(sim);

  std::printf("phase 1: directed fill/local/tstore flows\n");

  dut->fill_fifo_id_i = 0x12;
  dut->fill_fifo_dest_i = 0x1;
  dut->fill_fifo_wdata_i = 1;
  dut->fill_fifo_opcode_i = 1;
  dut->fill_fifo_data0_i = 0x1111111111111111ull;
  dut->fill_fifo_data1_i = 0x2222222222222222ull;
  dut->fill_fifo_data2_i = 0x3333333333333333ull;
  dut->fill_fifo_data3_i = 0x4444444444444444ull;
  dut->fill_fifo_data4_i = 0xaaaaaaaaaaaaaaaaull;
  dut->fill_fifo_data5_i = 0xbbbbbbbbbbbbbbbbull;
  dut->fill_fifo_data6_i = 0xccccccccccccccccull;
  dut->fill_fifo_data7_i = 0xddddddddddddddddull;
  dut->fill_fifo_size_i = 6;
  dut->fill_fifo_qwen_i = 0x3;
  dut->fill_fifo_valid_i = 1;
  tick_and_compare(sim);

  dut->fill_fifo_valid_i = 0;
  tick_and_compare(sim);
  tick_and_compare(sim);
  tick_and_compare(sim);

  dut->local_id_i = 0x44;
  dut->local_dest_i = 0x1;
  dut->local_wdata_i = 1;
  dut->local_opcode_i = 2;
  dut->local_data0_i = 0xdeadbeefcafef00dull;
  dut->local_data1_i = 0x0123456789abcdefull;
  dut->local_data2_i = 0x55aa55aa55aa55aaull;
  dut->local_data3_i = 0xaa55aa55aa55aa55ull;
  dut->local_size_i = 5;
  dut->local_qwen_i = 0xf;
  dut->local_valid_i = 1;
  tick_and_compare(sim);
  tick_and_compare(sim);
  dut->local_valid_i = 0;
  tick_and_compare(sim);
  tick_and_compare(sim);

  dut->tstore_id_i = 0x01;
  dut->tstore_dest_i = 0;
  dut->tstore_wdata_i = 0;
  dut->tstore_opcode_i = 0;
  dut->tstore_size_i = 2;
  dut->tstore_qwen_i = 0;
  dut->tstore_valid_i = 1;
  tick_and_compare(sim);
  dut->tstore_valid_i = 0;
  tick_and_compare(sim);
  tick_and_compare(sim);

  std::printf("phase 2: mixed random traffic (4000 cycles)\n");
  for (int i = 0; i < 4000; ++i) {
    dut->fill_fifo_id_i = xorshift32() & 0xff;
    dut->fill_fifo_dest_i = xorshift32() & 0xf;
    dut->fill_fifo_wdata_i = xorshift32() & 1;
    dut->fill_fifo_opcode_i = xorshift32() & 0x3;
    dut->fill_fifo_data0_i = rand64();
    dut->fill_fifo_data1_i = rand64();
    dut->fill_fifo_data2_i = rand64();
    dut->fill_fifo_data3_i = rand64();
    dut->fill_fifo_data4_i = rand64();
    dut->fill_fifo_data5_i = rand64();
    dut->fill_fifo_data6_i = rand64();
    dut->fill_fifo_data7_i = rand64();
    dut->fill_fifo_size_i = xorshift32() % 7;
    dut->fill_fifo_qwen_i = xorshift32() & 0xf;
    dut->fill_fifo_valid_i = xorshift32() & 1;

    dut->tstore_id_i = xorshift32() & 0xff;
    dut->tstore_dest_i = xorshift32() & 1;
    dut->tstore_wdata_i = xorshift32() & 1;
    dut->tstore_opcode_i = xorshift32() & 0x3;
    dut->tstore_data0_i = rand64();
    dut->tstore_data1_i = rand64();
    dut->tstore_data2_i = rand64();
    dut->tstore_data3_i = rand64();
    dut->tstore_size_i = xorshift32() % 7;
    dut->tstore_qwen_i = xorshift32() & 0xf;
    dut->tstore_valid_i = xorshift32() & 1;

    dut->local_id_i = xorshift32() & 0xff;
    dut->local_dest_i = xorshift32() & 0xf;
    dut->local_wdata_i = xorshift32() & 1;
    dut->local_opcode_i = xorshift32() & 0x3;
    dut->local_data0_i = rand64();
    dut->local_data1_i = rand64();
    dut->local_data2_i = rand64();
    dut->local_data3_i = rand64();
    dut->local_size_i = xorshift32() % 7;
    dut->local_qwen_i = xorshift32() & 0xf;
    dut->local_valid_i = xorshift32() & 1;

    dut->tbox_id_i = xorshift32() & 0xff;
    dut->tbox_dest_i = xorshift32() & 0xf;
    dut->tbox_wdata_i = xorshift32() & 1;
    dut->tbox_opcode_i = xorshift32() & 0x3;
    dut->tbox_data0_i = rand64();
    dut->tbox_data1_i = rand64();
    dut->tbox_data2_i = rand64();
    dut->tbox_data3_i = rand64();
    dut->tbox_size_i = xorshift32() % 7;
    dut->tbox_qwen_i = xorshift32() & 0xf;
    dut->tbox_valid_i = xorshift32() & 1;

    dut->l2_resp_ready_i = xorshift32() & 1;
    tick_and_compare(sim);
    if (sim.mismatches > 20) break;
  }

  return sim.finish();
}
