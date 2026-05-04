// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_rf_single_1r1w_par_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_rf_single_1r1w_par_tb;

static uint32_t next_rand(uint32_t& state) {
  state ^= state << 13;
  state ^= state >> 17;
  state ^= state << 5;
  return state;
}

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);

  sim.dut->clk_i = 0;
  sim.dut->wr_data_i = 0;
  sim.dut->wr_data_en_1p_i = 0;
  sim.dut->wr_en_i = 0;

  sim.tick();
  sim.compare("rd_data", sim.dut->orig_rd_data_o, sim.dut->new_rd_data_o);

  uint32_t seed = 0x89abcdefu;
  for (int i = 0; i < 2200; i++) {
    seed = next_rand(seed);
    sim.dut->wr_en_i = (seed >> 0) & 1u;
    sim.dut->wr_data_en_1p_i = (seed >> 1) & 1u;
    sim.dut->wr_data_i = seed ^ 0x55aa00ffu;
    sim.tick();
    sim.compare("rd_data", sim.dut->orig_rd_data_o, sim.dut->new_rd_data_o);
  }

  return sim.finish();
}
