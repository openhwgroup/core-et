// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_icache_tag_array_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_icache_tag_array_tb;

static uint32_t next_rand(uint32_t& state) {
  state ^= state << 13;
  state ^= state >> 17;
  state ^= state << 5;
  return state;
}

int main(int argc, char** argv) {
  CosimCtrl<DUT> sim(argc, argv);
  sim.max_time = 500000;

  sim.dut->read_valid_i = 0;
  sim.dut->read_set_i = 0;
  sim.dut->read_tag_i = 0;
  sim.dut->write_early_valid_i = 0;
  sim.dut->write_valid_i = 0;
  sim.dut->write_req_valid_i = 0;
  sim.dut->write_req_set_i = 0;
  sim.dut->write_req_way_i = 0;
  sim.dut->write_req_tag_i = 0;
  sim.dut->f1_miss_i = 0;
  sim.dut->f1_miss_idx_i = 0;
  sim.dut->f1_miss_way_i = 0;
  sim.dut->miss_state_i = 0;
  sim.dut->flush_data_i = 0;
  sim.dut->apb_pair_i = 0;
  sim.dut->apb_set_i = 0;
  sim.dut->apb_pwrite_i = 0;
  sim.dut->apb_psel_i = 0;
  sim.dut->apb_penable_i = 0;
  sim.dut->apb_pwdata_i = 0;
  sim.reset();

  uint32_t rng = 0x6c0ffee1u;
  sim.dut->miss_state_i = 0;
  sim.dut->read_valid_i = 1;
  sim.dut->read_set_i = 0;
  sim.dut->read_tag_i = 0;
  sim.tick();
  sim.compare("f1_read_tag_hit", sim.dut->orig_f1_read_tag_hit_o, sim.dut->new_f1_read_tag_hit_o);
  sim.compare("apb_pready", sim.dut->orig_apb_pready_o, sim.dut->new_apb_pready_o);
  sim.compare("apb_prdata", sim.dut->orig_apb_prdata_o, sim.dut->new_apb_prdata_o);
  sim.compare("apb_pslverr", sim.dut->orig_apb_pslverr_o, sim.dut->new_apb_pslverr_o);

  for (int i = 0; i < 20000; ++i) {
    uint32_t r0 = next_rand(rng);
    uint32_t r1 = next_rand(rng);
    uint32_t r2 = next_rand(rng);
    uint32_t r3 = next_rand(rng);

    sim.dut->read_valid_i = (r0 & 0x3) != 0;
    sim.dut->read_set_i = r1 & 0x7f;
    sim.dut->read_tag_i = r2 & ((1u << 27) - 1);

    sim.dut->write_valid_i = (r0 & 0x10) != 0;
    sim.dut->write_early_valid_i = sim.dut->write_valid_i || ((r0 & 0x20) != 0);
    sim.dut->write_req_valid_i = (r0 & 0x40) != 0;
    sim.dut->write_req_set_i = r2 & 0x7f;
    sim.dut->write_req_way_i = r3 & 0x3;
    sim.dut->write_req_tag_i = next_rand(rng) & ((1u << 27) - 1);

    sim.dut->f1_miss_i = (r1 & 0x7) == 0;
    sim.dut->f1_miss_idx_i = r3 & 0x7f;
    sim.dut->f1_miss_way_i = (r3 >> 7) & 0x3;
    sim.dut->miss_state_i = r1 % 3;
    sim.dut->flush_data_i = (r2 & 0x1ff) == 0;

    sim.dut->apb_set_i = (r2 >> 8) & 0x7f;
    sim.dut->apb_pair_i = (r2 >> 15) & 0x1;
    sim.dut->apb_pwrite_i = (r3 & 0x20) != 0;
    sim.dut->apb_psel_i = (r3 & 0xc0) == 0xc0;
    sim.dut->apb_penable_i = (r3 & 0x100) != 0;
    sim.dut->apb_pwdata_i =
        (uint64_t{next_rand(rng)} << 32) | uint64_t{next_rand(rng)};

    sim.tick();
    sim.compare("f1_read_tag_hit", sim.dut->orig_f1_read_tag_hit_o, sim.dut->new_f1_read_tag_hit_o);
    sim.compare("apb_pready", sim.dut->orig_apb_pready_o, sim.dut->new_apb_pready_o);
    sim.compare("apb_prdata", sim.dut->orig_apb_prdata_o, sim.dut->new_apb_prdata_o);
    sim.compare("apb_pslverr", sim.dut->orig_apb_pslverr_o, sim.dut->new_apb_pslverr_o);
  }

  return sim.finish();
}
