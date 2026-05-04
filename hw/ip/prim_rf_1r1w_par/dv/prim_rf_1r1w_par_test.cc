// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vprim_rf_1r1w_par.h"
#include "verilated.h"

#include <cstdio>
#include <cstdlib>
#include <string>

struct Sim {
  Vprim_rf_1r1w_par* dut;
  uint64_t sim_time = 0;
  int errors = 0;
  int checks = 0;

  Sim(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    dut = new Vprim_rf_1r1w_par;
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

  uint32_t entry(int idx) const {
    return dut->rd_data_o[idx];
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

static void write_rf(Sim& sim, int addr, uint32_t data) {
  sim.dut->wr_data_en_1p_i = 1;
  sim.dut->wr_en_i = 0;
  sim.dut->wr_data_i = data;
  sim.tick();

  sim.dut->wr_data_en_1p_i = 0;
  sim.dut->wr_en_i = 1;
  sim.dut->wr_addr_i = addr;
  sim.tick();

  sim.dut->wr_en_i = 0;
}

int main(int argc, char** argv) {
  Sim sim(argc, argv);

  sim.dut->wr_data_en_1p_i = 0;
  sim.dut->wr_en_i = 0;
  sim.dut->wr_addr_i = 0;
  sim.dut->wr_data_i = 0;
  sim.tick();
  sim.tick();

  std::printf("test 1: seed entries\n");
  for (int i = 0; i < 8; i++) write_rf(sim, i, 0x3000u + i);
  for (int i = 0; i < 8; i++) {
    sim.check(sim.entry(i) == (0x3000u + (uint32_t)i),
              "entry " + std::to_string(i) + " seeded");
  }

  std::printf("test 2: wr_en_i=0 does not write\n");
  sim.dut->wr_data_en_1p_i = 1;
  sim.dut->wr_en_i = 0;
  sim.dut->wr_data_i = 0xabcdef01u;
  sim.tick();
  sim.dut->wr_data_en_1p_i = 0;
  sim.tick();
  sim.check(sim.entry(0) == 0x3000u, "entry 0 unchanged");

  std::printf("test 3: stale capture behavior\n");
  sim.dut->wr_data_en_1p_i = 0;
  sim.dut->wr_data_i = 0x11111111u;
  sim.tick();
  sim.dut->wr_en_i = 1;
  sim.dut->wr_addr_i = 4;
  sim.tick();
  sim.dut->wr_en_i = 0;
  sim.check(sim.entry(4) == 0xabcdef01u, "write uses previous captured value");

  std::printf("test 4: overwrite different entries\n");
  write_rf(sim, 2, 0x5555aaaau);
  write_rf(sim, 7, 0x12345678u);
  sim.check(sim.entry(2) == 0x5555aaaau, "entry 2 overwritten");
  sim.check(sim.entry(7) == 0x12345678u, "entry 7 overwritten");

  return sim.finish();
}
