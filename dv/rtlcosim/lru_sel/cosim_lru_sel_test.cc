// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_lru_sel_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_lru_sel_tb;

static uint32_t xorshift32(uint32_t& s) {
  s ^= s << 13;
  s ^= s >> 17;
  s ^= s << 5;
  return s;
}

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.reset(2);

  sim.compare("victim", sim.dut->orig_victim_o, sim.dut->new_victim_o);

  uint32_t seed = 0x31415926u;
  for (int i = 0; i < 5000; i++) {
    sim.dut->access_entry_i = xorshift32(seed) & 0xffu;
    sim.tick();
    sim.compare("victim", sim.dut->orig_victim_o, sim.dut->new_victim_o);
  }

  return sim.finish();
}
