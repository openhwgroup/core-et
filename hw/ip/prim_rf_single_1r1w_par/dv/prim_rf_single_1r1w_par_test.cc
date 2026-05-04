// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vprim_rf_single_1r1w_par.h"
#include "verilated.h"

#include <cstdio>
#include <cstdlib>
#include <string>

struct Sim {
  Vprim_rf_single_1r1w_par* dut;
  uint64_t sim_time = 0;
  int errors = 0;
  int checks = 0;

  Sim(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    dut = new Vprim_rf_single_1r1w_par;
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

static void write_reg(Sim& sim, uint32_t data) {
  sim.dut->wr_data_en_1p_i = 1;
  sim.dut->wr_en_i = 0;
  sim.dut->wr_data_i = data;
  sim.tick();

  sim.dut->wr_data_en_1p_i = 0;
  sim.dut->wr_en_i = 1;
  sim.tick();

  sim.dut->wr_en_i = 0;
}

int main(int argc, char** argv) {
  Sim sim(argc, argv);

  sim.dut->wr_data_en_1p_i = 0;
  sim.dut->wr_en_i = 0;
  sim.dut->wr_data_i = 0;
  sim.tick();
  sim.tick();

  std::printf("test 1: basic write\n");
  write_reg(sim, 0x11223344u);
  sim.check(sim.dut->rd_data_o == 0x11223344u, "basic write");

  std::printf("test 2: wr_en_i=0 does not write\n");
  sim.dut->wr_data_en_1p_i = 1;
  sim.dut->wr_en_i = 0;
  sim.dut->wr_data_i = 0x55667788u;
  sim.tick();
  sim.dut->wr_data_en_1p_i = 0;
  sim.tick();
  sim.check(sim.dut->rd_data_o == 0x11223344u, "no write without wr_en_i");

  std::printf("test 3: stale captured data\n");
  sim.dut->wr_data_en_1p_i = 0;
  sim.dut->wr_data_i = 0xdeadbeefu;
  sim.tick();
  sim.dut->wr_en_i = 1;
  sim.tick();
  sim.dut->wr_en_i = 0;
  sim.check(sim.dut->rd_data_o == 0x55667788u, "write uses previous captured data");

  std::printf("test 4: overwrite\n");
  write_reg(sim, 0xa5a55a5au);
  sim.check(sim.dut->rd_data_o == 0xa5a55a5au, "overwrite");

  return sim.finish();
}
