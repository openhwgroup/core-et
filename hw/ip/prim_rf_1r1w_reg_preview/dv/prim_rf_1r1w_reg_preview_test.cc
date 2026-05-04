// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vprim_rf_1r1w_reg_preview_tb.h"
#include "verilated.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

struct Sim {
  Vprim_rf_1r1w_reg_preview_tb* dut;
  uint64_t sim_time = 0;
  int errors = 0;
  int checks = 0;

  explicit Sim(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    dut = new Vprim_rf_1r1w_reg_preview_tb;
  }

  ~Sim() { delete dut; }

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

static uint32_t next_rand(uint32_t& state) {
  state ^= state << 13;
  state ^= state >> 17;
  state ^= state << 5;
  return state;
}

int main(int argc, char** argv) {
  Sim sim(argc, argv);

  std::array<uint16_t, 8> rf_ref = {};
  uint16_t wr_data_del_ref = 0;
  bool wr_data_en_1p_ref = false;
  uint8_t rd_addr_ref = 0;
  uint8_t prev_rf_clk = 1;

  auto apply_model = [&]() {
    if (!sim.dut->preview_clk_i) {
      wr_data_en_1p_ref = sim.dut->wr_data_en_1p_next_i;
    }
    if (prev_rf_clk == 1 && sim.dut->rf_clk_i == 0) {
      if (wr_data_en_1p_ref) {
        wr_data_del_ref = sim.dut->wr_data_i;
      }
    }
    if (prev_rf_clk == 0 && sim.dut->rf_clk_i == 1) {
      if (sim.dut->rd_en_i) {
        rd_addr_ref = sim.dut->rd_addr_i & 0x7;
      }
      if (sim.dut->wr_en_i) {
        rf_ref[sim.dut->wr_addr_i & 0x7] = wr_data_del_ref;
      }
    }
    prev_rf_clk = sim.dut->rf_clk_i;
  };

  auto step = [&](uint8_t preview_clk,
                  uint8_t rf_clk,
                  uint8_t preview_next,
                  uint16_t wr_data,
                  uint8_t wr_addr,
                  uint8_t wr_en,
                  uint8_t rd_addr,
                  uint8_t rd_en,
                  const std::string& msg) {
    sim.dut->preview_clk_i = preview_clk;
    sim.dut->rf_clk_i = rf_clk;
    sim.dut->wr_data_en_1p_next_i = preview_next;
    sim.dut->wr_data_i = wr_data;
    sim.dut->wr_addr_i = wr_addr;
    sim.dut->wr_en_i = wr_en;
    sim.dut->rd_addr_i = rd_addr;
    sim.dut->rd_en_i = rd_en;
    apply_model();
    sim.dut->eval();
    sim.sim_time++;
    sim.check(sim.dut->rd_data_o == rf_ref[rd_addr_ref], msg);
  };

  step(1, 1, 0, 0x0000, 0, 0, 0, 0, "initial state");
  step(0, 0, 1, 0x2003, 0, 0, 0, 0, "preview arms first registered write");
  step(1, 1, 0, 0x4444, 1, 1, 1, 1, "write commits previewed data and captures read address");
  step(0, 0, 0, 0xaaaa, 2, 0, 3, 0, "dropping preview next while low updates preview latch");
  step(1, 1, 1, 0xbbbb, 2, 1, 1, 0, "write uses previously previewed data when preview is low");
  step(0, 0, 1, 0x55aa, 3, 0, 4, 0, "preview can arm while read address is held");
  step(1, 1, 0, 0xeeee, 3, 0, 3, 1, "read enable updates held read address without write");
  step(0, 0, 1, 0x1234, 5, 1, 5, 0, "preview re-arms on same local clock");
  step(1, 1, 0, 0x5678, 5, 1, 5, 1, "write commits only after matching write enable");

  uint32_t rng = 0x4b1d2a39u;
  for (int i = 0; i < 2048; ++i) {
    const uint8_t clk = next_rand(rng) & 0x1;
    step(clk,
         clk,
         next_rand(rng) & 0x1,
         static_cast<uint16_t>(next_rand(rng) & 0xffff),
         next_rand(rng) & 0x7,
         next_rand(rng) & 0x1,
         next_rand(rng) & 0x7,
         next_rand(rng) & 0x1,
         "random step " + std::to_string(i));
  }

  return sim.finish();
}
