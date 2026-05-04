// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_neigh_fill_fifo_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_neigh_fill_fifo_tb;

static uint32_t next_rand(uint32_t& state) {
  state ^= state << 13;
  state ^= state >> 17;
  state ^= state << 5;
  return state;
}

static void compare_outputs(CosimCtrl<DUT>& sim) {
  sim.compare("full", sim.dut->orig_full_o, sim.dut->new_full_o);
  sim.compare("out_rsp_valid", sim.dut->orig_out_rsp_valid_o, sim.dut->new_out_rsp_valid_o);
  sim.compare("out_rsp_wdata", sim.dut->orig_out_rsp_wdata_o, sim.dut->new_out_rsp_wdata_o);
  for (int i = 0; i < 12; i++) {
    const auto idx = std::to_string(i);
    sim.compare("out_rsp_id[" + idx + "]", sim.dut->orig_out_rsp_id_o[i], sim.dut->new_out_rsp_id_o[i]);
    sim.compare("out_rsp_dest[" + idx + "]", sim.dut->orig_out_rsp_dest_o[i], sim.dut->new_out_rsp_dest_o[i]);
    sim.compare("out_rsp_opcode[" + idx + "]", sim.dut->orig_out_rsp_opcode_o[i], sim.dut->new_out_rsp_opcode_o[i]);
    sim.compare("out_rsp_data0[" + idx + "]", sim.dut->orig_out_rsp_data0_o[i], sim.dut->new_out_rsp_data0_o[i]);
    sim.compare("out_rsp_data7[" + idx + "]", sim.dut->orig_out_rsp_data7_o[i], sim.dut->new_out_rsp_data7_o[i]);
    sim.compare("out_rsp_size[" + idx + "]", sim.dut->orig_out_rsp_size_o[i], sim.dut->new_out_rsp_size_o[i]);
    sim.compare("out_rsp_qwen[" + idx + "]", sim.dut->orig_out_rsp_qwen_o[i], sim.dut->new_out_rsp_qwen_o[i]);
  }
}

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.dut->clk_i = 0;
  sim.dut->rst_ni = 1;
  sim.dut->push_i = 0;
  sim.dut->push_id_i = 0;
  sim.dut->push_dest_i = 0;
  sim.dut->push_wdata_i = 0;
  sim.dut->push_opcode_i = 0;
  sim.dut->push_data0_i = 0;
  sim.dut->push_data1_i = 0;
  sim.dut->push_data2_i = 0;
  sim.dut->push_data3_i = 0;
  sim.dut->push_data4_i = 0;
  sim.dut->push_data5_i = 0;
  sim.dut->push_data6_i = 0;
  sim.dut->push_data7_i = 0;
  sim.dut->push_size_i = 0;
  sim.dut->push_qwen_i = 0;
  sim.dut->coop_mask_i = 0;
  sim.dut->coop_min_miss_id_i = 0;
  sim.dut->out_rsp_ready_i = 0;
  sim.reset(10);
  compare_outputs(sim);

  sim.dut->out_rsp_ready_i = 0xfff;
  sim.dut->push_i = 1;
  sim.dut->push_id_i = 0x21;
  sim.dut->push_dest_i = 3;
  sim.dut->push_wdata_i = 1;
  sim.dut->push_opcode_i = 1;
  sim.dut->push_data0_i = 0x1111;
  sim.dut->push_data7_i = 0x8888;
  sim.dut->push_size_i = 6;
  sim.dut->push_qwen_i = 0xf;
  sim.tick();
  compare_outputs(sim);
  sim.dut->push_i = 0;

  sim.dut->push_i = 1;
  sim.dut->push_id_i = 0x33;
  sim.dut->push_dest_i = 12;
  sim.dut->push_wdata_i = 1;
  sim.dut->push_opcode_i = 1;
  sim.dut->push_data0_i = 0xaaaa;
  sim.dut->push_data7_i = 0xbbbb;
  sim.dut->push_size_i = 6;
  sim.dut->push_qwen_i = 0xf;
  sim.dut->coop_mask_i = 0x3;
  sim.dut->coop_min_miss_id_i = (5u << 3) | 1u;
  sim.tick();
  compare_outputs(sim);
  sim.dut->push_i = 0;

  uint32_t seed = 0x62a1f09du;
  for (int i = 0; i < 1500; i++) {
    seed = next_rand(seed);
    sim.dut->push_i = (seed >> 0) & 1u;
    sim.dut->push_id_i = (seed >> 1) & 0xffu;
    sim.dut->push_dest_i = (seed >> 9) % 13u;
    sim.dut->push_wdata_i = (seed >> 13) & 1u;
    sim.dut->push_opcode_i = (seed >> 14) & 0x3u;
    sim.dut->push_data0_i = 0x1111111100000000ull ^ seed;
    sim.dut->push_data1_i = 0x2222222200000000ull ^ (seed << 1);
    sim.dut->push_data2_i = 0x3333333300000000ull ^ (seed << 2);
    sim.dut->push_data3_i = 0x4444444400000000ull ^ (seed << 3);
    sim.dut->push_data4_i = 0x5555555500000000ull ^ (seed << 4);
    sim.dut->push_data5_i = 0x6666666600000000ull ^ (seed << 5);
    sim.dut->push_data6_i = 0x7777777700000000ull ^ (seed << 6);
    sim.dut->push_data7_i = 0x8888888800000000ull ^ (seed << 7);
    sim.dut->push_size_i = (seed >> 16) & 0x7u;
    sim.dut->push_qwen_i = (seed >> 19) & 0xfu;
    sim.dut->coop_mask_i = (seed >> 23) & 0xffu;
    sim.dut->coop_min_miss_id_i =
        (((seed >> 0) & 0x7u) << 0) |
        (((seed >> 3) & 0x7u) << 3) |
        (((seed >> 6) & 0x7u) << 6) |
        (((seed >> 9) & 0x7u) << 9) |
        (((seed >> 12) & 0x7u) << 12) |
        (((seed >> 15) & 0x7u) << 15) |
        (((seed >> 18) & 0x7u) << 18) |
        (((seed >> 21) & 0x7u) << 21);
    sim.dut->out_rsp_ready_i = (seed >> 24) & 0xfffu;
    sim.tick();
    compare_outputs(sim);
  }

  return sim.finish();
}
