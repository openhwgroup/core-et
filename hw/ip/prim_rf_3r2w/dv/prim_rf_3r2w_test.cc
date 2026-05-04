// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// prim_rf_3r2w -- Verilator test.

#include "Vprim_rf_3r2w_tb.h"
#include "verilated.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

#ifdef TRACE
#include "verilated_vcd_c.h"
#endif

namespace {

constexpr int kEntries = 64;

struct Sim {
  Vprim_rf_3r2w_tb* dut;
  uint64_t sim_time = 0;
  int errors = 0;
  int checks = 0;
#ifdef TRACE
  VerilatedVcdC* tfp = nullptr;
#endif

  Sim(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    dut = new Vprim_rf_3r2w_tb;
#ifdef TRACE
    Verilated::traceEverOn(true);
    tfp = new VerilatedVcdC;
    dut->trace(tfp, 99);
    tfp->open("sim.vcd");
#endif
  }

  ~Sim() {
#ifdef TRACE
    if (tfp) {
      tfp->close();
      delete tfp;
    }
#endif
    delete dut;
  }

  void dump() {
#ifdef TRACE
    if (tfp) tfp->dump(sim_time);
#endif
  }

  void tick() {
    dut->clk_i = 0;
    dut->eval();
    dump();
    sim_time++;

    dut->clk_i = 1;
    dut->eval();
    dump();
    sim_time++;
  }

  void check(bool cond, const std::string& msg) {
    checks++;
    if (!cond) {
      std::printf("  FAIL [t=%lu]: %s\n", static_cast<unsigned long>(sim_time),
                  msg.c_str());
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

uint32_t seed = 0x3A2A1234u;

uint32_t xorshift32() {
  seed ^= seed << 13;
  seed ^= seed >> 17;
  seed ^= seed << 5;
  return seed;
}

void drive_idle(Sim& sim) {
  sim.dut->wr_en_a_i = 0;
  sim.dut->wr_en_b_i = 0;
  sim.dut->wr_data_a_en_1p_i = 0;
  sim.dut->wr_data_b_en_1p_i = 0;
}

void write_dual(Sim& sim, int addr_a, uint32_t data_a, int addr_b, uint32_t data_b) {
  sim.dut->wr_data_a_en_1p_i = 1;
  sim.dut->wr_data_b_en_1p_i = 1;
  sim.dut->wr_en_a_i = 0;
  sim.dut->wr_en_b_i = 0;
  sim.dut->wr_data_a_i = data_a;
  sim.dut->wr_data_b_i = data_b;
  sim.tick();

  sim.dut->wr_data_a_en_1p_i = 0;
  sim.dut->wr_data_b_en_1p_i = 0;
  sim.dut->wr_en_a_i = 1;
  sim.dut->wr_en_b_i = 1;
  sim.dut->wr_addr_a_i = addr_a;
  sim.dut->wr_addr_b_i = addr_b;
  sim.tick();

  drive_idle(sim);
}

void model_tick(std::array<uint32_t, kEntries>& rf, uint32_t& latched_a, uint32_t& latched_b,
                bool wr_data_a_en, uint32_t wr_data_a, bool wr_data_b_en, uint32_t wr_data_b,
                bool wr_en_a, int wr_addr_a, bool wr_en_b, int wr_addr_b) {
  if (wr_data_a_en) latched_a = wr_data_a;
  if (wr_data_b_en) latched_b = wr_data_b;
  if (wr_en_b) rf[wr_addr_b] = latched_b;
  if (wr_en_a) rf[wr_addr_a] = latched_a;
}

}  // namespace

int main(int argc, char** argv) {
  Sim sim(argc, argv);

  sim.dut->rd_addr_a_i = 0;
  sim.dut->rd_addr_b_i = 0;
  sim.dut->rd_addr_c_i = 0;
  sim.dut->wr_addr_a_i = 0;
  sim.dut->wr_addr_b_i = 0;
  sim.dut->wr_data_a_i = 0;
  sim.dut->wr_data_b_i = 0;
  drive_idle(sim);

  // Establish a known clock phase, then prime both delayed write-data registers.
  sim.tick();
  sim.tick();
  sim.dut->wr_data_a_en_1p_i = 1;
  sim.dut->wr_data_b_en_1p_i = 1;
  sim.dut->wr_data_a_i = 0x11111111u;
  sim.dut->wr_data_b_i = 0x22222222u;
  sim.tick();
  drive_idle(sim);

  std::array<uint32_t, kEntries> model{};
  uint32_t latched_a = 0x11111111u;
  uint32_t latched_b = 0x22222222u;

  std::printf("test 1: deterministic initialization by dual writes\n");
  for (int pair = 0; pair < kEntries / 2; pair++) {
    const int addr_a = pair * 2;
    const int addr_b = pair * 2 + 1;
    const uint32_t data_a = 0x10000000u + static_cast<uint32_t>(addr_a);
    const uint32_t data_b = 0x20000000u + static_cast<uint32_t>(addr_b);
    write_dual(sim, addr_a, data_a, addr_b, data_b);
    model[addr_a] = data_a;
    model[addr_b] = data_b;
    latched_a = data_a;
    latched_b = data_b;
  }

  for (int idx = 0; idx < kEntries; idx++) {
    sim.dut->rd_addr_a_i = idx;
    sim.dut->rd_addr_b_i = (idx + 13) & 63;
    sim.dut->rd_addr_c_i = (idx + 29) & 63;
    sim.dut->eval();
    sim.check(sim.dut->rd_data_a_o == model[sim.dut->rd_addr_a_i], "rd_data_a seeded");
    sim.check(sim.dut->rd_data_b_o == model[sim.dut->rd_addr_b_i], "rd_data_b seeded");
    sim.check(sim.dut->rd_data_c_o == model[sim.dut->rd_addr_c_i], "rd_data_c seeded");
  }

  std::printf("test 2: same-address dual write uses port A data\n");
  sim.dut->wr_data_a_en_1p_i = 1;
  sim.dut->wr_data_b_en_1p_i = 1;
  sim.dut->wr_en_a_i = 0;
  sim.dut->wr_en_b_i = 0;
  sim.dut->wr_data_a_i = 0xAAAA5555u;
  sim.dut->wr_data_b_i = 0xBBBB6666u;
  sim.tick();
  sim.dut->wr_data_a_en_1p_i = 0;
  sim.dut->wr_data_b_en_1p_i = 0;
  sim.dut->wr_en_a_i = 1;
  sim.dut->wr_en_b_i = 1;
  sim.dut->wr_addr_a_i = 7;
  sim.dut->wr_addr_b_i = 7;
  sim.tick();
  drive_idle(sim);
  model[7] = 0xAAAA5555u;
  latched_a = 0xAAAA5555u;
  latched_b = 0xBBBB6666u;
  sim.dut->rd_addr_a_i = 7;
  sim.dut->rd_addr_b_i = 7;
  sim.dut->rd_addr_c_i = 7;
  sim.dut->eval();
  sim.check(sim.dut->rd_data_a_o == 0xAAAA5555u, "same-address conflict: port A wins");
  sim.check(sim.dut->rd_data_b_o == 0xAAAA5555u, "same-address conflict visible on port B");
  sim.check(sim.dut->rd_data_c_o == 0xAAAA5555u, "same-address conflict visible on port C");

  std::printf("test 3: stale captured data is reused when commit arrives without fresh capture\n");
  sim.dut->wr_data_a_en_1p_i = 1;
  sim.dut->wr_data_b_en_1p_i = 1;
  sim.dut->wr_data_a_i = 0x13572468u;
  sim.dut->wr_data_b_i = 0x24681357u;
  sim.tick();
  sim.dut->wr_data_a_en_1p_i = 0;
  sim.dut->wr_data_b_en_1p_i = 0;
  sim.dut->wr_data_a_i = 0xFFFFFFFFu;
  sim.dut->wr_data_b_i = 0xEEEEEEEEu;
  sim.dut->wr_en_a_i = 1;
  sim.dut->wr_en_b_i = 1;
  sim.dut->wr_addr_a_i = 8;
  sim.dut->wr_addr_b_i = 9;
  sim.tick();
  drive_idle(sim);
  model[8] = 0x13572468u;
  model[9] = 0x24681357u;
  latched_a = 0x13572468u;
  latched_b = 0x24681357u;
  sim.dut->rd_addr_a_i = 8;
  sim.dut->rd_addr_b_i = 9;
  sim.dut->rd_addr_c_i = 8;
  sim.dut->eval();
  sim.check(sim.dut->rd_data_a_o == 0x13572468u, "port A stale captured data reused");
  sim.check(sim.dut->rd_data_b_o == 0x24681357u, "port B stale captured data reused");
  sim.check(sim.dut->rd_data_c_o == 0x13572468u, "port C sees stale-captured A write");

  std::printf("test 4: low-phase capture survives later data mutation before commit\n");
  sim.dut->wr_data_a_en_1p_i = 1;
  sim.dut->wr_data_b_en_1p_i = 1;
  sim.dut->wr_en_a_i = 0;
  sim.dut->wr_en_b_i = 0;
  sim.dut->wr_data_a_i = 0x0BADF00Du;
  sim.dut->wr_data_b_i = 0x00C0FFEEu;
  sim.dut->clk_i = 0;
  sim.dut->eval();
  sim.sim_time++;
  sim.dut->wr_data_a_i = 0x11111111u;
  sim.dut->wr_data_b_i = 0x22222222u;
  sim.dut->clk_i = 1;
  sim.dut->wr_data_a_en_1p_i = 0;
  sim.dut->wr_data_b_en_1p_i = 0;
  sim.dut->wr_en_a_i = 1;
  sim.dut->wr_en_b_i = 1;
  sim.dut->wr_addr_a_i = 10;
  sim.dut->wr_addr_b_i = 11;
  sim.dut->eval();
  sim.sim_time++;
  drive_idle(sim);
  model[10] = 0x0BADF00Du;
  model[11] = 0x00C0FFEEu;
  latched_a = 0x0BADF00Du;
  latched_b = 0x00C0FFEEu;
  sim.dut->rd_addr_a_i = 10;
  sim.dut->rd_addr_b_i = 11;
  sim.dut->rd_addr_c_i = 10;
  sim.dut->eval();
  sim.check(sim.dut->rd_data_a_o == 0x0BADF00Du, "port A commit uses low-phase captured data");
  sim.check(sim.dut->rd_data_b_o == 0x00C0FFEEu, "port B commit uses low-phase captured data");
  sim.check(sim.dut->rd_data_c_o == 0x0BADF00Du, "port C sees low-phase captured A write");

  std::printf("test 5: random full-cycle stimulus against software model\n");
  for (int i = 0; i < 2000; i++) {
    const uint32_t ctrl = xorshift32();
    const bool wr_data_a_en = (ctrl >> 0) & 1;
    const bool wr_data_b_en = (ctrl >> 1) & 1;
    const bool wr_en_a = (ctrl >> 2) & 1;
    const bool wr_en_b = (ctrl >> 3) & 1;
    const int rd_addr_a = (ctrl >> 4) & 63;
    const int rd_addr_b = (ctrl >> 10) & 63;
    const int rd_addr_c = (ctrl >> 16) & 63;
    const int wr_addr_a = (ctrl >> 22) & 63;
    const int wr_addr_b = (ctrl >> 26) & 63;
    const uint32_t wr_data_a = xorshift32();
    const uint32_t wr_data_b = xorshift32();

    sim.dut->rd_addr_a_i = rd_addr_a;
    sim.dut->rd_addr_b_i = rd_addr_b;
    sim.dut->rd_addr_c_i = rd_addr_c;
    sim.dut->wr_data_a_en_1p_i = wr_data_a_en;
    sim.dut->wr_data_b_en_1p_i = wr_data_b_en;
    sim.dut->wr_en_a_i = wr_en_a;
    sim.dut->wr_en_b_i = wr_en_b;
    sim.dut->wr_addr_a_i = wr_addr_a;
    sim.dut->wr_addr_b_i = wr_addr_b;
    sim.dut->wr_data_a_i = wr_data_a;
    sim.dut->wr_data_b_i = wr_data_b;
    sim.tick();

    model_tick(model, latched_a, latched_b, wr_data_a_en, wr_data_a, wr_data_b_en,
               wr_data_b, wr_en_a, wr_addr_a, wr_en_b, wr_addr_b);
    sim.dut->eval();
    sim.check(sim.dut->rd_data_a_o == model[rd_addr_a], "random rd_data_a matches model");
    sim.check(sim.dut->rd_data_b_o == model[rd_addr_b], "random rd_data_b matches model");
    sim.check(sim.dut->rd_data_c_o == model[rd_addr_c], "random rd_data_c matches model");
  }

  return sim.finish();
}
