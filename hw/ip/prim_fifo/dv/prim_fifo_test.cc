// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vprim_fifo_tb.h"
#include "sim_ctrl.h"

using DUT = Vprim_fifo_tb;

namespace {

void drive(DUT* dut, bool push_early, bool push, uint8_t data, bool pop) {
  dut->push_early_i = push_early;
  dut->push_i = push;
  dut->push_data_i = data;
  dut->pop_i = pop;
}

void check_pair_equal(SimCtrl<DUT>& sim, const char* what) {
  sim.check(sim.dut->ff_full_o == sim.dut->lat_full_o, std::string(what) + ": full matches");
  sim.check(sim.dut->ff_full_next_o == sim.dut->lat_full_next_o, std::string(what) + ": full_next matches");
  sim.check(sim.dut->ff_valid_o == sim.dut->lat_valid_o, std::string(what) + ": valid matches");
  sim.check(sim.dut->ff_valid_next_o == sim.dut->lat_valid_next_o, std::string(what) + ": valid_next matches");
  if (sim.dut->ff_valid_o || sim.dut->lat_valid_o) {
    sim.check(sim.dut->ff_pop_data_o == sim.dut->lat_pop_data_o, std::string(what) + ": pop_data matches");
  }
}

void check_state(SimCtrl<DUT>& sim,
                 const char* what,
                 bool full,
                 bool valid,
                 uint8_t head) {
  check_pair_equal(sim, what);
  sim.check(sim.dut->ff_full_o == full, std::string(what) + ": full");
  sim.check(sim.dut->ff_valid_o == valid, std::string(what) + ": valid");
  if (valid) {
    sim.check(sim.dut->ff_pop_data_o == head, std::string(what) + ": head data");
  }
}

}  // namespace

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);

  drive(sim.dut.get(), false, false, 0, false);
  sim.reset();
  check_state(sim, "reset", false, false, 0);
  sim.check(sim.dut->ff_full_next_o == 0, "reset: full_next low");
  sim.check(sim.dut->ff_valid_next_o == 0, "reset: valid_next low");

  // Push A into empty FIFO.
  drive(sim.dut.get(), true, true, 0xA1, false);
  sim.tick();
  check_state(sim, "push A", false, true, 0xA1);

  // Hold.
  drive(sim.dut.get(), false, false, 0, false);
  sim.tick();
  check_state(sim, "hold A", false, true, 0xA1);

  // Push B, C, D.
  drive(sim.dut.get(), true, true, 0xB2, false);
  sim.tick();
  check_state(sim, "push B", false, true, 0xA1);

  drive(sim.dut.get(), true, true, 0xC3, false);
  sim.tick();
  check_state(sim, "push C", false, true, 0xA1);

  drive(sim.dut.get(), true, true, 0xD4, false);
  sim.tick();
  check_state(sim, "push D", true, true, 0xA1);

  // Drain in order.
  drive(sim.dut.get(), false, false, 0, true);
  sim.tick();
  check_state(sim, "pop A", false, true, 0xB2);

  drive(sim.dut.get(), false, false, 0, true);
  sim.tick();
  check_state(sim, "pop B", false, true, 0xC3);

  drive(sim.dut.get(), false, false, 0, true);
  sim.tick();
  check_state(sim, "pop C", false, true, 0xD4);

  drive(sim.dut.get(), false, false, 0, true);
  sim.tick();
  check_state(sim, "pop D", false, false, 0);

  // Empty bypass.
  drive(sim.dut.get(), true, true, 0xE5, false);
  sim.tick();
  check_state(sim, "push E", false, true, 0xE5);

  // Push+pop with only the output element occupied should bypass the new data.
  drive(sim.dut.get(), true, true, 0xF6, true);
  sim.tick();
  check_state(sim, "push F pop E", false, true, 0xF6);

  // Final drain.
  drive(sim.dut.get(), false, false, 0, true);
  sim.tick();
  check_state(sim, "final pop", false, false, 0);

  return sim.finish();
}
