// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_icache_micro_tag_array_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_icache_micro_tag_array_tb;

static uint32_t next_rand(uint32_t& state) {
  state ^= state << 13;
  state ^= state >> 17;
  state ^= state << 5;
  return state;
}

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.max_time = 500000;

  sim.dut->rd_tag_i = 0;
  sim.dut->wr_early_valid_i = 0;
  sim.dut->wr_valid_i = 0;
  sim.dut->wr_entry_i = 0;
  sim.dut->wr_tag_i = 0;
  sim.dut->miss_i = 0;
  sim.dut->miss_entry_i = 0;
  sim.dut->miss_state_i = 0;
  sim.dut->flush_data_i = 0;
  sim.dut->apb_paddr_i = 0;
  sim.dut->apb_pwrite_i = 0;
  sim.dut->apb_psel_i = 0;
  sim.dut->apb_penable_i = 0;
  sim.dut->apb_pwdata_i = 0;
  sim.reset();

  uint32_t rng = 0x1234cafeu;
  sim.dut->miss_state_i = 0;
  sim.tick();
  sim.compare("rd_hit_array", sim.dut->orig_rd_hit_array_o, sim.dut->new_rd_hit_array_o);
  sim.compare("apb_pready", sim.dut->orig_apb_pready_o, sim.dut->new_apb_pready_o);
  sim.compare("apb_prdata", sim.dut->orig_apb_prdata_o, sim.dut->new_apb_prdata_o);
  sim.compare("apb_pslverr", sim.dut->orig_apb_pslverr_o, sim.dut->new_apb_pslverr_o);

  for (int i = 0; i < 20000; ++i) {
    uint32_t r0 = next_rand(rng);
    uint32_t r1 = next_rand(rng);
    uint32_t r2 = next_rand(rng);

    sim.dut->rd_tag_i = ((uint64_t{next_rand(rng)} << 2) | (r0 & 0x3)) & ((uint64_t{1} << 34) - 1);
    sim.dut->wr_valid_i = (r0 & 0x3) != 0;
    sim.dut->wr_early_valid_i = sim.dut->wr_valid_i || ((r0 & 0x10) != 0);
    sim.dut->wr_entry_i = r1 & 0xf;
    sim.dut->wr_tag_i = ((uint64_t{next_rand(rng)} << 2) | (r1 & 0x3)) & ((uint64_t{1} << 34) - 1);
    sim.dut->miss_i = (r0 & 0x40) != 0;
    sim.dut->miss_entry_i = (r1 >> 4) & 0xf;
    sim.dut->miss_state_i = r1 % 3;
    sim.dut->flush_data_i = (r2 & 0x1ff) == 0;
    sim.dut->apb_paddr_i = r2 & 0xf;
    sim.dut->apb_pwrite_i = (r2 & 0x20) != 0;
    sim.dut->apb_psel_i = (r2 & 0xc0) == 0xc0;
    sim.dut->apb_penable_i = (r2 & 0x100) != 0;
    sim.dut->apb_pwdata_i =
        (uint64_t{next_rand(rng)} << 32) | uint64_t{next_rand(rng)};

    sim.tick();
    sim.compare("rd_hit_array", sim.dut->orig_rd_hit_array_o, sim.dut->new_rd_hit_array_o);
    sim.compare("apb_pready", sim.dut->orig_apb_pready_o, sim.dut->new_apb_pready_o);
    sim.compare("apb_prdata", sim.dut->orig_apb_prdata_o, sim.dut->new_apb_prdata_o);
    sim.compare("apb_pslverr", sim.dut->orig_apb_pslverr_o, sim.dut->new_apb_pslverr_o);
  }

  return sim.finish();
}
