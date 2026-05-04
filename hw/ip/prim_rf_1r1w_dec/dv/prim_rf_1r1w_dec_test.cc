// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vprim_rf_1r1w_dec.h"
#include "sim_ctrl.h"

using DUT = Vprim_rf_1r1w_dec;

static constexpr int kWidth = 16;
static constexpr int kDepth = 8;

static uint32_t onehot(int idx) {
  return 1u << idx;
}

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);

  sim.dut->clk_i = 0;
  sim.dut->rd_addr_i = 0;
  sim.dut->wr_data_i = 0;
  sim.dut->wr_data_en_1p_i = 0;
  sim.dut->wr_addr_i = 0;
  sim.dut->wr_en_i = 0;

  sim.tick();
  sim.check(sim.dut->rd_data_o == 0u, "init read zero");

  for (int i = 0; i < kDepth; ++i) {
    const uint32_t value = 0x5100u + static_cast<uint32_t>(i);

    sim.dut->wr_data_i = value;
    sim.dut->wr_data_en_1p_i = 1;
    sim.dut->wr_en_i = 0;
    sim.tick();

    sim.dut->wr_data_en_1p_i = 0;
    sim.dut->wr_en_i = 1;
    sim.dut->wr_addr_i = onehot(i);
    sim.tick();

    sim.dut->wr_en_i = 0;
    sim.dut->rd_addr_i = onehot(i);
    sim.tick();
    sim.check(sim.dut->rd_data_o == value, "readback written entry");
  }

  sim.dut->rd_addr_i = onehot(2) | onehot(5);
  sim.tick();
  sim.check(sim.dut->rd_data_o == ((0x5100u + 2u) | (0x5100u + 5u)),
            "multi-hot read ors selected entries");

  sim.dut->wr_data_i = 0x6aa6u;
  sim.dut->wr_data_en_1p_i = 1;
  sim.dut->wr_en_i = 0;
  sim.tick();
  sim.dut->wr_data_en_1p_i = 0;
  sim.dut->wr_en_i = 1;
  sim.dut->wr_addr_i = onehot(1) | onehot(4);
  sim.tick();
  sim.dut->wr_en_i = 0;

  sim.dut->rd_addr_i = onehot(1);
  sim.tick();
  sim.check(sim.dut->rd_data_o == 0x6aa6u, "entry 1 multi-write");

  sim.dut->rd_addr_i = onehot(4);
  sim.tick();
  sim.check(sim.dut->rd_data_o == 0x6aa6u, "entry 4 multi-write");

  return sim.finish();
}
