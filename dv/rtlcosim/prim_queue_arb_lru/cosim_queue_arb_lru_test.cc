// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_queue_arb_lru_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_queue_arb_lru_tb;

static uint32_t next_rand(uint32_t& state) {
  state ^= state << 13;
  state ^= state >> 17;
  state ^= state << 5;
  return state;
}

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.dut->clk_i = 0;
  sim.dut->rst_ni = 1;
  sim.dut->prio_up_i = 0;
  sim.dut->prio_entry_i = 0;
  sim.dut->bid_i = 0;
  sim.reset(10);
  sim.compare("winner", sim.dut->orig_winner_o, sim.dut->new_winner_o);

  sim.dut->bid_i = 0xf;
  for (int i = 0; i < 8; i++) {
    sim.tick();
    sim.compare("winner", sim.dut->orig_winner_o, sim.dut->new_winner_o);
  }

  uint32_t seed = 0x8128af63u;
  for (int i = 0; i < 2200; i++) {
    seed = next_rand(seed);
    sim.dut->prio_up_i = (seed >> 0) & 1u;
    sim.dut->prio_entry_i = (seed >> 1) & 0x3u;
    sim.dut->bid_i = (seed >> 3) & 0xfu;
    sim.tick();
    sim.compare("winner", sim.dut->orig_winner_o, sim.dut->new_winner_o);
  }

  return sim.finish();
}
