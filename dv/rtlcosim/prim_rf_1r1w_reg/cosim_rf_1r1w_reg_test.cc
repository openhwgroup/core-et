// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_rf_1r1w_reg_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_rf_1r1w_reg_tb;

static uint32_t next_rand(uint32_t& state) {
  state ^= state << 13;
  state ^= state >> 17;
  state ^= state << 5;
  return state;
}

static void compare_rd(CosimCtrl<DUT>& sim) {
  sim.compare("rd_data", sim.dut->orig_rd_data_o, sim.dut->new_rd_data_o);
}

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);

  sim.dut->clk_i = 0;
  sim.dut->rd_addr_i = 0;
  sim.dut->rd_en_i = 0;
  sim.dut->wr_data_i = 0;
  sim.dut->wr_data_en_1p_i = 0;
  sim.dut->wr_addr_i = 0;
  sim.dut->wr_en_i = 0;

  sim.tick();
  compare_rd(sim);

  for (int i = 0; i < 32; i++) {
    sim.dut->wr_data_en_1p_i = 1;
    sim.dut->wr_en_i = 0;
    sim.dut->wr_data_i = 0x40000000u + i;
    sim.tick();
    compare_rd(sim);

    sim.dut->wr_data_en_1p_i = 0;
    sim.dut->wr_en_i = 1;
    sim.dut->wr_addr_i = i;
    sim.tick();
    compare_rd(sim);
  }
  sim.dut->wr_en_i = 0;

  uint32_t seed = 0x1badc0deu;
  for (int i = 0; i < 2200; i++) {
    seed = next_rand(seed);
    sim.dut->rd_addr_i = (seed >> 0) & 0x1fu;
    sim.dut->rd_en_i = (seed >> 5) & 1u;
    sim.dut->wr_addr_i = (seed >> 6) & 0x1fu;
    sim.dut->wr_en_i = (seed >> 11) & 1u;
    sim.dut->wr_data_en_1p_i = (seed >> 12) & 1u;
    sim.dut->wr_data_i = seed ^ 0xa5a55a5au;
    sim.tick();
    compare_rd(sim);
  }

  return sim.finish();
}
