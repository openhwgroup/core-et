// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_pseudo_lru_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <random>

using DUT = Vcosim_pseudo_lru_tb;

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  std::mt19937 rng(0x1u);

  sim.dut->clk_i = 0;
  sim.dut->rst_ni = 1;
  sim.dut->access_i = 0;
  sim.dut->index_in_i = 0;

  sim.reset();

  for (int cycle = 0; cycle < 5000; ++cycle) {
    sim.dut->access_i = (rng() & 1U) ? 1 : 0;
    sim.dut->index_in_i = rng() & 0x7U;
    sim.tick();
    sim.compare("index_out", sim.dut->orig_index_out_o, sim.dut->new_index_out_o);
  }

  return sim.finish();
}
