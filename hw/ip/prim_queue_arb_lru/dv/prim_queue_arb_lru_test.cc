// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vprim_queue_arb_lru.h"
#include "verilated.h"

#include <cstdio>
#include <string>

struct Sim {
  Vprim_queue_arb_lru* dut;
  uint64_t sim_time = 0;
  int errors = 0;
  int checks = 0;

  Sim(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    dut = new Vprim_queue_arb_lru;
  }

  ~Sim() { delete dut; }

  void tick() {
    dut->clk_i = 0;
    dut->eval();
    sim_time++;

    dut->clk_i = 1;
    dut->eval();
    sim_time++;
  }

  void check(bool cond, const std::string& msg) {
    checks++;
    if (!cond) {
      std::printf("  FAIL [t=%lu]: %s\n", (unsigned long)sim_time, msg.c_str());
      errors++;
    }
  }

  int finish() {
    std::printf("\n%d/%d checks passed\n", checks - errors, checks);
    if (errors) {
      std::printf("TEST FAILED: %d error(s)\n", errors);
      return 1;
    }
    std::printf("TEST PASSED\n");
    return 0;
  }
};

static void push_entry(Sim& sim, int entry) {
  sim.dut->prio_up_i = 1;
  sim.dut->prio_entry_i = entry;
  sim.tick();
  sim.dut->prio_up_i = 0;
}

int main(int argc, char** argv) {
  Sim sim(argc, argv);

  sim.dut->rst_ni = 0;
  sim.dut->prio_up_i = 0;
  sim.dut->prio_entry_i = 0;
  sim.dut->bid_i = 0;
  sim.tick();
  sim.tick();
  sim.dut->rst_ni = 1;

  std::printf("test 1: initial lowest index wins\n");
  sim.dut->bid_i = 0xf;
  sim.dut->eval();
  sim.check(sim.dut->winner_o == 0, "winner is client 0 after reset");

  std::printf("test 2: inserted entries become MRU\n");
  push_entry(sim, 0);
  sim.dut->bid_i = 0xf;
  sim.dut->eval();
  sim.check(sim.dut->winner_o == 1, "after pushing 0, client 1 is oldest");

  push_entry(sim, 1);
  sim.dut->eval();
  sim.check(sim.dut->winner_o == 2, "after pushing 1, client 2 is oldest");

  push_entry(sim, 2);
  sim.dut->eval();
  sim.check(sim.dut->winner_o == 3, "after pushing 2, client 3 is oldest");

  std::printf("test 3: subset bidding obeys stored order\n");
  sim.dut->bid_i = 0b1001;
  sim.dut->eval();
  sim.check(sim.dut->winner_o == 3, "between clients 3 and 0, client 3 wins");

  push_entry(sim, 3);
  sim.dut->bid_i = 0xf;
  sim.dut->eval();
  sim.check(sim.dut->winner_o == 0, "after pushing 3, order wraps back to client 0");

  return sim.finish();
}
