// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vlru_sel.h"
#include "sim_ctrl.h"

using DUT = Vlru_sel;

static uint32_t onehot(int idx) {
  return 1u << idx;
}

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);
  sim.reset(2);

  sim.check(sim.dut->victim_o == onehot(0), "initial victim is client 0");

  sim.dut->access_entry_i = onehot(0);
  sim.tick();
  sim.check(sim.dut->victim_o == onehot(1), "after access 0 victim is 1");

  sim.dut->access_entry_i = onehot(1);
  sim.tick();
  sim.check(sim.dut->victim_o == onehot(2), "after access 1 victim is 2");

  sim.dut->access_entry_i = onehot(2);
  sim.tick();
  sim.check(sim.dut->victim_o == onehot(3), "after access 2 victim is 3");

  sim.dut->access_entry_i = onehot(3);
  sim.tick();
  sim.check(sim.dut->victim_o == onehot(4), "after access 3 victim advances to 4");

  sim.dut->access_entry_i = onehot(4);
  sim.tick();
  sim.check(sim.dut->victim_o == onehot(5), "after access 4 victim is 5");

  sim.dut->access_entry_i = onehot(5);
  sim.tick();
  sim.check(sim.dut->victim_o == onehot(6), "after access 5 victim is 6");

  sim.dut->access_entry_i = onehot(6);
  sim.tick();
  sim.check(sim.dut->victim_o == onehot(7), "after access 6 victim is 7");

  sim.dut->access_entry_i = onehot(7);
  sim.tick();
  sim.check(sim.dut->victim_o == onehot(0), "after access 7 victim wraps to 0");

  sim.dut->access_entry_i = 0;
  sim.tick();
  sim.check(sim.dut->victim_o == onehot(0), "idle cycle preserves victim");

  sim.dut->access_entry_i = onehot(0) | onehot(2);
  sim.tick();
  sim.check((sim.dut->victim_o & (onehot(0) | onehot(2))) == 0u,
            "multi-access does not immediately choose accessed entries");

  return sim.finish();
}
