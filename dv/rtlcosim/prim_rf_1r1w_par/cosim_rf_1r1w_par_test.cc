// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_rf_1r1w_par_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_rf_1r1w_par_tb;

static uint32_t next_rand(uint32_t& state) {
  state ^= state << 13;
  state ^= state >> 17;
  state ^= state << 5;
  return state;
}

static void compare_rd_data(CosimCtrl<DUT>& sim) {
  for (int i = 0; i < 8; i++) {
    sim.compare("rd_data[" + std::to_string(i) + "]",
                sim.dut->orig_rd_data_o[i],
                sim.dut->new_rd_data_o[i]);
  }
}

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);

  sim.dut->clk_i = 0;
  sim.dut->wr_data_i = 0;
  sim.dut->wr_data_en_1p_i = 0;
  sim.dut->wr_addr_i = 0;
  sim.dut->wr_en_i = 0;

  sim.tick();
  compare_rd_data(sim);

  for (int i = 0; i < 8; i++) {
    sim.dut->wr_data_en_1p_i = 1;
    sim.dut->wr_en_i = 0;
    sim.dut->wr_data_i = 0x50000000u + i;
    sim.tick();
    compare_rd_data(sim);

    sim.dut->wr_data_en_1p_i = 0;
    sim.dut->wr_en_i = 1;
    sim.dut->wr_addr_i = i;
    sim.tick();
    compare_rd_data(sim);
  }
  sim.dut->wr_en_i = 0;

  uint32_t seed = 0x31415926u;
  for (int i = 0; i < 2200; i++) {
    seed = next_rand(seed);
    sim.dut->wr_addr_i = (seed >> 0) & 0x7u;
    sim.dut->wr_en_i = (seed >> 3) & 1u;
    sim.dut->wr_data_en_1p_i = (seed >> 4) & 1u;
    sim.dut->wr_data_i = seed ^ 0x96aa55c3u;
    sim.tick();
    compare_rd_data(sim);
  }

  return sim.finish();
}
