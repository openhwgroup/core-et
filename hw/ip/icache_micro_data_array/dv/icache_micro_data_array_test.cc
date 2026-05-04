// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vicache_micro_data_array_tb.h"
#include "sim_ctrl.h"

using DUT = Vicache_micro_data_array_tb;

static uint32_t onehot(int idx) {
  return 1u << idx;
}

int main(int argc, char** argv) {
  SimCtrl<DUT> sim(argc, argv);

  sim.dut->clk_i = 0;
  sim.dut->f3_rd_en_i = 0;
  sim.dut->f3_rd_addr_dec_i = 0;
  sim.dut->f0_wr_en_i = 0;
  sim.dut->f0_wr_en_early_i = 0;
  sim.dut->f0_wr_addr_dec_i = 0;
  sim.dut->f0_wr_data_i = 0;
  sim.dut->dbg_addr_dec_i = 0;
  sim.dut->dbg_write_en_i = 0;
  sim.dut->dbg_write_en_early_i = 0;
  sim.dut->dbg_write_data_i = 0;
  sim.dut->dbg_read_en_i = 0;

  sim.tick();
  sim.check(sim.dut->f3_rd_data_o == 0u, "init read zero");
  sim.check(sim.dut->dbg_read_data_o == 0u, "init debug read zero");
  sim.check(sim.dut->dbg_read_ready_o == 0u, "init debug read ready");
  sim.check(sim.dut->dbg_write_ready_o == 0u, "init debug write ready");

  sim.dut->f0_wr_data_i = 0x12345u;
  sim.dut->f0_wr_en_early_i = 1;
  sim.dut->f0_wr_en_i = 1;
  sim.dut->f0_wr_addr_dec_i = onehot(2);
  sim.tick();
  sim.dut->f0_wr_en_early_i = 0;
  sim.dut->f0_wr_en_i = 0;

  sim.dut->f3_rd_en_i = 1;
  sim.dut->f3_rd_addr_dec_i = onehot(2);
  sim.tick();
  sim.check(sim.dut->f3_rd_data_o == 0x12345u, "normal readback");
  sim.check(sim.dut->dbg_read_ready_o == 0u, "debug read blocked by normal read");

  sim.dut->dbg_write_data_i = 0x2aa55u;
  sim.dut->dbg_write_en_early_i = 1;
  sim.dut->dbg_write_en_i = 1;
  sim.dut->dbg_addr_dec_i = onehot(1);
  sim.tick();
  sim.check(sim.dut->dbg_write_ready_o == 1u, "debug write ready");
  sim.dut->dbg_write_en_early_i = 0;
  sim.dut->dbg_write_en_i = 0;

  sim.dut->f3_rd_addr_dec_i = onehot(1);
  sim.tick();
  sim.check(sim.dut->f3_rd_data_o == 0x2aa55u, "debug write visible to normal read");

  sim.dut->f3_rd_en_i = 0;
  sim.dut->dbg_read_en_i = 1;
  sim.dut->dbg_addr_dec_i = onehot(1);
  sim.tick();
  sim.check(sim.dut->dbg_read_data_o == 0x2aa55u, "debug read data");
  sim.check(sim.dut->dbg_read_ready_o == 1u, "debug read ready when idle");

  sim.dut->dbg_read_en_i = 0;
  sim.dut->f3_rd_en_i = 1;
  sim.dut->f3_rd_addr_dec_i = onehot(2);
  sim.dut->dbg_read_en_i = 1;
  sim.dut->dbg_addr_dec_i = onehot(1);
  sim.tick();
  sim.check(sim.dut->f3_rd_data_o == 0x12345u, "normal read priority over debug read");
  sim.check(sim.dut->dbg_read_ready_o == 0u, "debug read blocked while f3 active");

  return sim.finish();
}
