// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vprim_rf_1r1w_reg.h"
#include "verilated.h"

#include <cstdio>
#include <cstdlib>
#include <string>

struct Sim {
  Vprim_rf_1r1w_reg* dut;
  uint64_t sim_time = 0;
  int errors = 0;
  int checks = 0;

  Sim(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    dut = new Vprim_rf_1r1w_reg;
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

static uint32_t read_rf(Sim& sim, int addr) {
  sim.dut->rd_en_i = 1;
  sim.dut->rd_addr_i = addr;
  sim.tick();
  sim.dut->rd_en_i = 0;
  sim.dut->eval();
  return sim.dut->rd_data_o;
}

int main(int argc, char** argv) {
  Sim sim(argc, argv);

  sim.dut->wr_data_en_1p_i = 0;
  sim.dut->wr_en_i = 0;
  sim.dut->wr_addr_i = 0;
  sim.dut->wr_data_i = 0;
  sim.dut->rd_addr_i = 0;
  sim.dut->rd_en_i = 0;

  sim.tick();
  sim.tick();

  std::printf("test 1: deterministic initialization\n");
  for (int i = 0; i < 8; i++) write_rf(sim, i, 0x2000u + i);
  for (int i = 0; i < 8; i++) {
    sim.check(read_rf(sim, i) == (0x2000u + (uint32_t)i),
              "rf[" + std::to_string(i) + "] seeded value");
  }

  std::printf("test 2: read address hold\n");
  (void)read_rf(sim, 3);
  sim.dut->rd_addr_i = 7;
  sim.dut->rd_en_i = 0;
  sim.dut->eval();
  sim.check(sim.dut->rd_data_o == 0x2003u, "rd_data holds previous selected address when rd_en_i=0");

  std::printf("test 3: wr_en_i=0 does not write\n");
  sim.dut->wr_data_en_1p_i = 1;
  sim.dut->wr_en_i = 0;
  sim.dut->wr_data_i = 0xdeadbeefu;
  sim.tick();
  sim.dut->wr_data_en_1p_i = 0;
  sim.tick();
  sim.check(read_rf(sim, 3) == 0x2003u, "entry unchanged when wr_en_i stays low");

  std::printf("test 4: wr_data_en_1p_i=0 preserves captured data\n");
  sim.dut->wr_data_en_1p_i = 0;
  sim.dut->wr_data_i = 0xaaaaaaaau;
  sim.tick();
  sim.dut->wr_en_i = 1;
  sim.dut->wr_addr_i = 9;
  sim.tick();
  sim.dut->wr_en_i = 0;
  sim.check(read_rf(sim, 9) == 0xdeadbeefu, "write uses previous captured value");

  std::printf("test 5: overwrite and held read\n");
  write_rf(sim, 3, 0x55aa7711u);
  sim.check(read_rf(sim, 3) == 0x55aa7711u, "entry overwritten");
  sim.dut->rd_addr_i = 1;
  sim.dut->rd_en_i = 0;
  sim.dut->eval();
  sim.check(sim.dut->rd_data_o == 0x55aa7711u, "held read remains stable");

  return sim.finish();
}
