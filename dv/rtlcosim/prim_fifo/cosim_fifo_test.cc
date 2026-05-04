// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_fifo_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_fifo_tb;

static void compare_outputs(CosimCtrl<DUT>& sim) {
  sim.compare("ff_full", sim.dut->orig_ff_full_o, sim.dut->new_ff_full_o);
  sim.compare("ff_full_next", sim.dut->orig_ff_full_next_o, sim.dut->new_ff_full_next_o);
  sim.compare("ff_valid", sim.dut->orig_ff_valid_o, sim.dut->new_ff_valid_o);
  sim.compare("ff_valid_next", sim.dut->orig_ff_valid_next_o, sim.dut->new_ff_valid_next_o);
  if (sim.dut->orig_ff_valid_o || sim.dut->new_ff_valid_o) {
    sim.compare("ff_pop_data", sim.dut->orig_ff_pop_data_o, sim.dut->new_ff_pop_data_o);
  }

  sim.compare("lat_full", sim.dut->orig_lat_full_o, sim.dut->new_lat_full_o);
  sim.compare("lat_full_next", sim.dut->orig_lat_full_next_o, sim.dut->new_lat_full_next_o);
  sim.compare("lat_valid", sim.dut->orig_lat_valid_o, sim.dut->new_lat_valid_o);
  sim.compare("lat_valid_next", sim.dut->orig_lat_valid_next_o, sim.dut->new_lat_valid_next_o);
  if (sim.dut->orig_lat_valid_o || sim.dut->new_lat_valid_o) {
    sim.compare("lat_pop_data", sim.dut->orig_lat_pop_data_o, sim.dut->new_lat_pop_data_o);
  }
}

static void tick_and_compare(CosimCtrl<DUT>& sim) {
  sim.tick();
  compare_outputs(sim);
}

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  int fill = 0;

  sim.dut->push_early_i = 0;
  sim.dut->push_i = 0;
  sim.dut->push_data_i = 0;
  sim.dut->pop_i = 0;
  sim.reset(10);
  compare_outputs(sim);

  // Directed fill / drain.
  for (int i = 0; i < 4; ++i) {
    sim.dut->push_early_i = 1;
    sim.dut->push_i = 1;
    sim.dut->push_data_i = 0x10 + i;
    sim.dut->pop_i = 0;
    tick_and_compare(sim);
    fill++;
  }

  sim.dut->push_early_i = 0;
  sim.dut->push_i = 0;
  tick_and_compare(sim);

  for (int i = 0; i < 4; ++i) {
    sim.dut->pop_i = 1;
    tick_and_compare(sim);
    fill--;
  }

  sim.dut->pop_i = 0;
  tick_and_compare(sim);

  // Simultaneous push/pop.
  for (int i = 0; i < 2; ++i) {
    sim.dut->push_early_i = 1;
    sim.dut->push_i = 1;
    sim.dut->push_data_i = 0x80 + i;
    tick_and_compare(sim);
    fill++;
  }

  for (int i = 0; i < 16; ++i) {
    sim.dut->push_early_i = 1;
    sim.dut->push_i = 1;
    sim.dut->push_data_i = 0xC0 + i;
    sim.dut->pop_i = 1;
    tick_and_compare(sim);
  }

  sim.dut->push_early_i = 0;
  sim.dut->push_i = 0;
  sim.dut->pop_i = 0;
  tick_and_compare(sim);

  while (fill > 0) {
    sim.dut->push_early_i = 0;
    sim.dut->push_i = 0;
    sim.dut->pop_i = 1;
    tick_and_compare(sim);
    fill--;
  }

  sim.dut->pop_i = 0;
  tick_and_compare(sim);

  // Random legal traffic.
  uint32_t seed = 0x2134a5f1u;
  for (int i = 0; i < 700; ++i) {
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;

    const int want_push = (seed >> 0) & 1;
    const int want_pop = (seed >> 1) & 1;
    const int do_push = want_push && (fill < 4);
    const int do_pop = want_pop && (fill > 0);

    sim.dut->push_early_i = do_push;
    sim.dut->push_i = do_push;
    sim.dut->push_data_i = seed & 0xFF;
    sim.dut->pop_i = do_pop;
    tick_and_compare(sim);

    fill += do_push - do_pop;
  }

  sim.dut->push_early_i = 0;
  sim.dut->push_i = 0;
  sim.dut->pop_i = 0;
  tick_and_compare(sim);

  return sim.finish();
}
