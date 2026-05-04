// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: dcache_metadata_array (original) vs minion_dcache_metadata_array (new).

#include "Vcosim_minion_dcache_metadata_array_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_minion_dcache_metadata_array_tb;

namespace {

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("valid", sim.dut->orig_s1_read_resp_valid_o, sim.dut->new_s1_read_resp_valid_o);
    sim.compare("way0_coh", sim.dut->orig_way0_coh_state_o, sim.dut->new_way0_coh_state_o);
    sim.compare("way0_tag", sim.dut->orig_way0_tag_o, sim.dut->new_way0_tag_o);
    sim.compare("way1_coh", sim.dut->orig_way1_coh_state_o, sim.dut->new_way1_coh_state_o);
    sim.compare("way1_tag", sim.dut->orig_way1_tag_o, sim.dut->new_way1_tag_o);
    sim.compare("way2_coh", sim.dut->orig_way2_coh_state_o, sim.dut->new_way2_coh_state_o);
    sim.compare("way2_tag", sim.dut->orig_way2_tag_o, sim.dut->new_way2_tag_o);
    sim.compare("way3_coh", sim.dut->orig_way3_coh_state_o, sim.dut->new_way3_coh_state_o);
    sim.compare("way3_tag", sim.dut->orig_way3_tag_o, sim.dut->new_way3_tag_o);
}

void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.dut->eval();  // Negedge write-data capture needs wrapper inputs settled before the clock edge.
    sim.tick();
    compare_outputs(sim);
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    sim.dut->s1_read_set_i = 0;
    sim.dut->s1_write_valid_i = 0;
    sim.dut->s1_write_valid_prev_i = 0;
    sim.dut->s1_write_set_i = 0;
    sim.dut->s1_write_way_en_i = 0;
    sim.dut->s1_write_coh_state_i = 0;
    sim.dut->s1_write_tag_i = 0;
    sim.dut->cfg_clear_all_i = 0;
    sim.dut->cfg_clear_low_i = 0;
    sim.reset(4);
    compare_outputs(sim);

    for (int i = 0; i < 32; ++i) {
      sim.dut->s1_read_set_i = i & 0xF;
      sim.dut->s1_write_valid_i = 1;
      sim.dut->s1_write_valid_prev_i = (i & 1) != 0;
      sim.dut->s1_write_set_i = (i * 3) & 0xF;
      sim.dut->s1_write_way_en_i = ((i << 1) | 1) & 0xF;
      sim.dut->s1_write_coh_state_i = i & 0x3;
      sim.dut->s1_write_tag_i = 0x100000000ULL | uint64_t(i * 17);
      sim.dut->cfg_clear_all_i = (i == 10);
      sim.dut->cfg_clear_low_i = (i == 20);
      tick_and_compare(sim);
    }

    uint32_t seed = 0xA341316Cu;
    for (int i = 0; i < 4096; ++i) {
      sim.dut->s1_read_set_i = xorshift32(seed) & 0xF;
      sim.dut->s1_write_valid_i = xorshift32(seed) & 1u;
      sim.dut->s1_write_valid_prev_i = xorshift32(seed) & 1u;
      sim.dut->s1_write_set_i = xorshift32(seed) & 0xF;
      sim.dut->s1_write_way_en_i = xorshift32(seed) & 0xF;
      sim.dut->s1_write_coh_state_i = xorshift32(seed) & 0x3;
      sim.dut->s1_write_tag_i = xorshift32(seed) & 0x1FFFFFFFFULL;
      sim.dut->cfg_clear_all_i = (xorshift32(seed) & 0x7F) == 0;
      sim.dut->cfg_clear_low_i = !sim.dut->cfg_clear_all_i && ((xorshift32(seed) & 0x3F) == 0);
      tick_and_compare(sim);
    }

    return sim.finish();
}
