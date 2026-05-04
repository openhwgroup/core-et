// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_icache_micro_data_array_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_icache_micro_data_array_tb;

static uint32_t next_rand(uint32_t& state) {
  state ^= state << 13;
  state ^= state >> 17;
  state ^= state << 5;
  return state;
}

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);

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
  sim.compare("f3_rd_data", sim.dut->orig_f3_rd_data_o, sim.dut->new_f3_rd_data_o);
  sim.compare("dbg_write_ready", sim.dut->orig_dbg_write_ready_o, sim.dut->new_dbg_write_ready_o);
  sim.compare("dbg_read_data", sim.dut->orig_dbg_read_data_o, sim.dut->new_dbg_read_data_o);
  sim.compare("dbg_read_ready", sim.dut->orig_dbg_read_ready_o, sim.dut->new_dbg_read_ready_o);

  uint32_t seed = 0x13579bdfu;
  for (int i = 0; i < 20000; i++) {
    const uint32_t r0 = next_rand(seed);
    const uint32_t r1 = next_rand(seed);

    sim.dut->f3_rd_en_i = (r0 >> 0) & 1u;
    sim.dut->f3_rd_addr_dec_i = (r0 >> 1) & 0xfu;
    sim.dut->f0_wr_en_i = (r0 >> 5) & 1u;
    sim.dut->f0_wr_en_early_i = (r0 >> 6) & 1u;
    sim.dut->f0_wr_addr_dec_i = (r0 >> 7) & 0xfu;
    sim.dut->f0_wr_data_i = r1 & 0x3ffffu;
    sim.dut->dbg_addr_dec_i = (r1 >> 18) & 0xfu;
    sim.dut->dbg_write_en_i = (r0 >> 11) & 1u;
    sim.dut->dbg_write_en_early_i = (r0 >> 12) & 1u;
    sim.dut->dbg_write_data_i = (next_rand(seed) ^ 0x2a55u) & 0x3ffffu;
    sim.dut->dbg_read_en_i = (r0 >> 13) & 1u;

    sim.tick();
    sim.compare("f3_rd_data", sim.dut->orig_f3_rd_data_o, sim.dut->new_f3_rd_data_o);
    sim.compare("dbg_write_ready", sim.dut->orig_dbg_write_ready_o, sim.dut->new_dbg_write_ready_o);
    sim.compare("dbg_read_data", sim.dut->orig_dbg_read_data_o, sim.dut->new_dbg_read_data_o);
    sim.compare("dbg_read_ready", sim.dut->orig_dbg_read_ready_o, sim.dut->new_dbg_read_ready_o);
  }

  return sim.finish();
}
