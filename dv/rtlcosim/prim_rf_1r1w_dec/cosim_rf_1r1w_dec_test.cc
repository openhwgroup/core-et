// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_rf_1r1w_dec_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_rf_1r1w_dec_tb;

static uint32_t next_rand(uint32_t& state) {
  state ^= state << 13;
  state ^= state >> 17;
  state ^= state << 5;
  return state;
}

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);

  sim.dut->clk_i = 0;
  sim.dut->rd_addr_i = 0;
  sim.dut->wr_data_i = 0;
  sim.dut->wr_data_en_1p_i = 0;
  sim.dut->wr_addr_i = 0;
  sim.dut->wr_en_i = 0;

  sim.tick();
  sim.compare("rd_data", sim.dut->orig_rd_data_o, sim.dut->new_rd_data_o);

  for (int i = 0; i < 8; i++) {
    sim.dut->wr_data_en_1p_i = 1;
    sim.dut->wr_en_i = 0;
    sim.dut->wr_data_i = 0x4400u + i;
    sim.tick();
    sim.compare("rd_data", sim.dut->orig_rd_data_o, sim.dut->new_rd_data_o);

    sim.dut->wr_data_en_1p_i = 0;
    sim.dut->wr_en_i = 1;
    sim.dut->wr_addr_i = 1u << i;
    sim.tick();
    sim.compare("rd_data", sim.dut->orig_rd_data_o, sim.dut->new_rd_data_o);
  }

  uint32_t seed = 0x5a17f00du;
  for (int i = 0; i < 2500; i++) {
    const uint32_t r0 = next_rand(seed);
    const uint32_t r1 = next_rand(seed);

    sim.dut->rd_addr_i = r0 & 0xffu;
    sim.dut->wr_en_i = (r0 >> 8) & 1u;
    sim.dut->wr_data_en_1p_i = (r0 >> 9) & 1u;
    sim.dut->wr_addr_i = (r0 >> 10) & 0xffu;
    sim.dut->wr_data_i = static_cast<uint16_t>(r1);

    sim.tick();
    sim.compare("rd_data", sim.dut->orig_rd_data_o, sim.dut->new_rd_data_o);
  }

  return sim.finish();
}
