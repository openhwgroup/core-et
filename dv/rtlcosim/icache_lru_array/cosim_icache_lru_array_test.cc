// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_icache_lru_array_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vcosim_icache_lru_array_tb;

static uint32_t next_rand(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 200000;

    sim.dut->f0_valid_i = 0;
    sim.dut->f0_access_set_i = 0;
    sim.dut->f1_update_en_early_i = 0;
    sim.dut->f1_update_en_i = 0;
    sim.dut->f1_update_set_i = 0;
    sim.dut->f1_update_access_way_i = 0;
    sim.reset();

    uint32_t rng = 0x5eed1234u;
    uint8_t last_read_set = 0;
    bool have_f1_state = false;

    for (int i = 0; i < 5000; ++i) {
      sim.dut->f0_valid_i = 1;
      sim.dut->f0_access_set_i = next_rand(rng) & 0x7f;

      sim.dut->f1_update_en_i = have_f1_state && ((next_rand(rng) & 0x3) != 0);
      sim.dut->f1_update_en_early_i = sim.dut->f1_update_en_i;
      sim.dut->f1_update_set_i = have_f1_state ? last_read_set : 0;
      sim.dut->f1_update_access_way_i = 1u << (next_rand(rng) & 0x3);

      if (sim.dut->f1_update_en_i && ((next_rand(rng) & 0x1) != 0)) {
        sim.dut->f0_access_set_i = sim.dut->f1_update_set_i;
      }

      sim.tick();
      sim.compare("f1_victim", sim.dut->orig_f1_victim_o, sim.dut->new_f1_victim_o);

      last_read_set = sim.dut->f0_access_set_i;
      have_f1_state = true;
    }

    return sim.finish();
}
