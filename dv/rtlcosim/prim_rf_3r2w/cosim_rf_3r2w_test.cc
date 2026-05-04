// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: prim_rf_3r2w vs rf_latch_3r_2w.

#include "Vcosim_rf_3r2w_tb.h"
#include "verilated.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

namespace {

struct Cosim {
  Vcosim_rf_3r2w_tb* dut;
  uint64_t sim_time = 0;
  int errors = 0;
  int compares = 0;

  Cosim(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    dut = new Vcosim_rf_3r2w_tb;
  }

  ~Cosim() { delete dut; }

  void compare(uint32_t orig, uint32_t nw, const char* name) {
    compares++;
    if (orig != nw) {
      std::printf("  MISMATCH [t=%lu] %s: orig=0x%08x new=0x%08x\n",
                  static_cast<unsigned long>(sim_time), name, orig, nw);
      errors++;
    }
  }

  void eval_and_compare() {
    dut->eval();
    compare(dut->orig_rd_data_a_o, dut->new_rd_data_a_o, "rd_data_a");
    compare(dut->orig_rd_data_b_o, dut->new_rd_data_b_o, "rd_data_b");
    compare(dut->orig_rd_data_c_o, dut->new_rd_data_c_o, "rd_data_c");
  }

  void step_half(int clk_level) {
    dut->clk_i = clk_level;
    eval_and_compare();
    sim_time++;
  }

  int finish() {
    std::printf("\n%d comparisons, %d mismatches\n", compares, errors);
    if (errors) {
      std::printf("COSIM FAILED\n");
      return 1;
    }
    std::printf("COSIM PASSED\n");
    return 0;
  }
};

uint32_t seed = 0x3A2A5678u;

uint32_t xorshift32() {
  seed ^= seed << 13;
  seed ^= seed >> 17;
  seed ^= seed << 5;
  return seed;
}

void drive_idle(Cosim& sim) {
  sim.dut->wr_en_a_i = 0;
  sim.dut->wr_en_b_i = 0;
  sim.dut->wr_data_a_en_1p_i = 0;
  sim.dut->wr_data_b_en_1p_i = 0;
}

void full_cycle(Cosim& sim) {
  sim.step_half(0);
  sim.step_half(1);
}

void write_dual(Cosim& sim, int addr_a, uint32_t data_a, int addr_b, uint32_t data_b) {
  sim.dut->wr_data_a_en_1p_i = 1;
  sim.dut->wr_data_b_en_1p_i = 1;
  sim.dut->wr_en_a_i = 0;
  sim.dut->wr_en_b_i = 0;
  sim.dut->wr_data_a_i = data_a;
  sim.dut->wr_data_b_i = data_b;
  full_cycle(sim);

  sim.dut->wr_data_a_en_1p_i = 0;
  sim.dut->wr_data_b_en_1p_i = 0;
  sim.dut->wr_en_a_i = 1;
  sim.dut->wr_en_b_i = 1;
  sim.dut->wr_addr_a_i = addr_a;
  sim.dut->wr_addr_b_i = addr_b;
  full_cycle(sim);
  drive_idle(sim);
}

}  // namespace

int main(int argc, char** argv) {
  Cosim sim(argc, argv);

  sim.dut->rd_addr_a_i = 0;
  sim.dut->rd_addr_b_i = 0;
  sim.dut->rd_addr_c_i = 0;
  sim.dut->wr_addr_a_i = 0;
  sim.dut->wr_addr_b_i = 0;
  sim.dut->wr_data_a_i = 0;
  sim.dut->wr_data_b_i = 0;
  drive_idle(sim);

  // Establish a known phase and prime the delayed write-data registers.
  full_cycle(sim);
  full_cycle(sim);
  sim.dut->wr_data_a_en_1p_i = 1;
  sim.dut->wr_data_b_en_1p_i = 1;
  sim.dut->wr_data_a_i = 0x11111111u;
  sim.dut->wr_data_b_i = 0x22222222u;
  full_cycle(sim);
  drive_idle(sim);

  std::printf("──── cosim prim_rf_3r2w ────\n");
  std::printf("phase 1: deterministic initialization\n");
  for (int pair = 0; pair < 32; pair++) {
    write_dual(sim, pair * 2, 0x10000000u + static_cast<uint32_t>(pair * 2),
               pair * 2 + 1, 0x20000000u + static_cast<uint32_t>(pair * 2 + 1));
  }

  std::printf("phase 2: read sweep on all three ports\n");
  for (int idx = 0; idx < 64; idx++) {
    sim.dut->rd_addr_a_i = idx;
    sim.dut->rd_addr_b_i = (idx + 9) & 63;
    sim.dut->rd_addr_c_i = (idx + 21) & 63;
    sim.eval_and_compare();
  }
  full_cycle(sim);

  std::printf("phase 3: same-address dual write\n");
  sim.dut->wr_data_a_en_1p_i = 1;
  sim.dut->wr_data_b_en_1p_i = 1;
  sim.dut->wr_data_a_i = 0xAAAA5555u;
  sim.dut->wr_data_b_i = 0xBBBB6666u;
  full_cycle(sim);
  sim.dut->wr_data_a_en_1p_i = 0;
  sim.dut->wr_data_b_en_1p_i = 0;
  sim.dut->wr_en_a_i = 1;
  sim.dut->wr_en_b_i = 1;
  sim.dut->wr_addr_a_i = 5;
  sim.dut->wr_addr_b_i = 5;
  full_cycle(sim);
  drive_idle(sim);

  std::printf("phase 4: stale captured data reuse without fresh capture\n");
  sim.dut->wr_data_a_en_1p_i = 1;
  sim.dut->wr_data_b_en_1p_i = 1;
  sim.dut->wr_data_a_i = 0x12345678u;
  sim.dut->wr_data_b_i = 0x89ABCDEFu;
  full_cycle(sim);
  sim.dut->wr_data_a_en_1p_i = 0;
  sim.dut->wr_data_b_en_1p_i = 0;
  sim.dut->wr_data_a_i = 0xFFFFFFFFu;
  sim.dut->wr_data_b_i = 0xEEEEEEEEu;
  sim.dut->wr_en_a_i = 1;
  sim.dut->wr_en_b_i = 1;
  sim.dut->wr_addr_a_i = 6;
  sim.dut->wr_addr_b_i = 7;
  full_cycle(sim);
  drive_idle(sim);

  std::printf("phase 5: half-cycle capture mutation\n");
  sim.dut->wr_data_a_en_1p_i = 1;
  sim.dut->wr_data_b_en_1p_i = 1;
  sim.dut->wr_en_a_i = 0;
  sim.dut->wr_en_b_i = 0;
  sim.dut->wr_data_a_i = 0x0BADF00Du;
  sim.dut->wr_data_b_i = 0x00C0FFEEu;
  sim.step_half(0);
  sim.dut->wr_data_a_i = 0x11111111u;
  sim.dut->wr_data_b_i = 0x22222222u;
  sim.dut->wr_data_a_en_1p_i = 0;
  sim.dut->wr_data_b_en_1p_i = 0;
  sim.dut->wr_en_a_i = 1;
  sim.dut->wr_en_b_i = 1;
  sim.dut->wr_addr_a_i = 8;
  sim.dut->wr_addr_b_i = 9;
  sim.step_half(1);
  drive_idle(sim);

  std::printf("phase 6: random stimulus (2048 cycles)\n");
  for (int i = 0; i < 2048; i++) {
    const uint32_t ctrl = xorshift32();
    sim.dut->rd_addr_a_i = ctrl & 63;
    sim.dut->rd_addr_b_i = (ctrl >> 6) & 63;
    sim.dut->rd_addr_c_i = (ctrl >> 12) & 63;
    sim.dut->wr_data_a_en_1p_i = (ctrl >> 18) & 1;
    sim.dut->wr_data_b_en_1p_i = (ctrl >> 19) & 1;
    sim.dut->wr_en_a_i = (ctrl >> 20) & 1;
    sim.dut->wr_en_b_i = (ctrl >> 21) & 1;
    sim.dut->wr_addr_a_i = (ctrl >> 22) & 63;
    sim.dut->wr_addr_b_i = (ctrl >> 26) & 63;
    sim.dut->wr_data_a_i = xorshift32();
    sim.dut->wr_data_b_i = xorshift32();
    full_cycle(sim);
  }

  return sim.finish();
}
