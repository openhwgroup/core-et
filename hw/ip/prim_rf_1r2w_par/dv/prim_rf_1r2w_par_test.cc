// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vprim_rf_1r2w_par.h"
#include "verilated.h"

#include <cstdio>
#include <string>

struct Sim {
  Vprim_rf_1r2w_par* dut;
  uint64_t sim_time = 0;
  int errors = 0;
  int checks = 0;

  Sim(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    dut = new Vprim_rf_1r2w_par;
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

  uint32_t entry(int idx) const { return dut->rd_data_o[idx]; }

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

static void phase_write_a(Sim& sim, int addr, uint32_t data) {
  sim.dut->wr_data_a_en_1p_i = 1;
  sim.dut->wr_en_a_i = 0;
  sim.dut->wr_data_a_i = data;
  sim.tick();

  sim.dut->wr_data_a_en_1p_i = 0;
  sim.dut->wr_en_a_i = 1;
  sim.dut->wr_addr_a_i = addr;
  sim.tick();

  sim.dut->wr_en_a_i = 0;
}

static void phase_write_b(Sim& sim, int addr, uint32_t data) {
  sim.dut->wr_data_b_en_1p_i = 1;
  sim.dut->wr_en_b_i = 0;
  sim.dut->wr_data_b_i = data;
  sim.tick();

  sim.dut->wr_data_b_en_1p_i = 0;
  sim.dut->wr_en_b_i = 1;
  sim.dut->wr_addr_b_i = addr;
  sim.tick();

  sim.dut->wr_en_b_i = 0;
}

int main(int argc, char** argv) {
  Sim sim(argc, argv);

  sim.dut->wr_data_a_en_1p_i = 0;
  sim.dut->wr_en_a_i = 0;
  sim.dut->wr_addr_a_i = 0;
  sim.dut->wr_data_a_i = 0;
  sim.dut->wr_data_b_en_1p_i = 0;
  sim.dut->wr_en_b_i = 0;
  sim.dut->wr_addr_b_i = 0;
  sim.dut->wr_data_b_i = 0;
  sim.tick();
  sim.tick();

  std::printf("test 1: independent writes\n");
  phase_write_a(sim, 1, 0x11111111u);
  phase_write_b(sim, 2, 0x22222222u);
  sim.check(sim.entry(1) == 0x11111111u, "port A writes entry 1");
  sim.check(sim.entry(2) == 0x22222222u, "port B writes entry 2");

  std::printf("test 2: simultaneous different-address writes\n");
  sim.dut->wr_data_a_en_1p_i = 1;
  sim.dut->wr_data_b_en_1p_i = 1;
  sim.dut->wr_data_a_i = 0xaaaa0001u;
  sim.dut->wr_data_b_i = 0xbbbb0002u;
  sim.tick();
  sim.dut->wr_data_a_en_1p_i = 0;
  sim.dut->wr_data_b_en_1p_i = 0;
  sim.dut->wr_en_a_i = 1;
  sim.dut->wr_en_b_i = 1;
  sim.dut->wr_addr_a_i = 4;
  sim.dut->wr_addr_b_i = 5;
  sim.tick();
  sim.dut->wr_en_a_i = 0;
  sim.dut->wr_en_b_i = 0;
  sim.check(sim.entry(4) == 0xaaaa0001u, "simultaneous port A write lands");
  sim.check(sim.entry(5) == 0xbbbb0002u, "simultaneous port B write lands");

  std::printf("test 3: same-address collision uses port A\n");
  sim.dut->wr_data_a_en_1p_i = 1;
  sim.dut->wr_data_b_en_1p_i = 1;
  sim.dut->wr_data_a_i = 0x12345678u;
  sim.dut->wr_data_b_i = 0x87654321u;
  sim.tick();
  sim.dut->wr_data_a_en_1p_i = 0;
  sim.dut->wr_data_b_en_1p_i = 0;
  sim.dut->wr_en_a_i = 1;
  sim.dut->wr_en_b_i = 1;
  sim.dut->wr_addr_a_i = 3;
  sim.dut->wr_addr_b_i = 3;
  sim.tick();
  sim.dut->wr_en_a_i = 0;
  sim.dut->wr_en_b_i = 0;
  sim.check(sim.entry(3) == 0x12345678u, "port A wins same-address collision");

  std::printf("test 4: stale capture is per-port\n");
  sim.dut->wr_data_a_en_1p_i = 1;
  sim.dut->wr_data_a_i = 0xdeadc0deu;
  sim.tick();
  sim.dut->wr_data_a_en_1p_i = 0;
  sim.dut->wr_data_a_i = 0x99999999u;
  sim.dut->wr_en_a_i = 1;
  sim.dut->wr_addr_a_i = 6;
  sim.tick();
  sim.dut->wr_en_a_i = 0;
  sim.check(sim.entry(6) == 0xdeadc0deu, "port A uses previously captured data");

  sim.dut->wr_data_b_en_1p_i = 1;
  sim.dut->wr_data_b_i = 0xcafebabeu;
  sim.tick();
  sim.dut->wr_data_b_en_1p_i = 0;
  sim.dut->wr_data_b_i = 0x11111111u;
  sim.dut->wr_en_b_i = 1;
  sim.dut->wr_addr_b_i = 7;
  sim.tick();
  sim.dut->wr_en_b_i = 0;
  sim.check(sim.entry(7) == 0xcafebabeu, "port B uses previously captured data");

  return sim.finish();
}
