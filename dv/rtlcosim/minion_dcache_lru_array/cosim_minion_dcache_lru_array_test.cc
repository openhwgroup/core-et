// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation test: dcache_lru_array (original) vs minion_dcache_lru_array (new).

#include "Vcosim_minion_dcache_lru_array_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_minion_dcache_lru_array_tb;

namespace {

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("read_data", sim.dut->orig_read_data_o, sim.dut->new_read_data_o);
}

void tick_and_compare(CosimCtrl<DUT>& sim) {
    sim.tick();
    compare_outputs(sim);
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);

    sim.dut->read_set_i = 0;
    sim.dut->write_en_i = 0;
    sim.dut->write_set_i = 0;
    sim.dut->write_data_i = 0;
    sim.reset(4);
    compare_outputs(sim);

    for (int i = 0; i < 32; ++i) {
        sim.dut->read_set_i = i & 0xF;
        sim.dut->write_en_i = 1;
        sim.dut->write_set_i = (i * 5) & 0xF;
        sim.dut->write_data_i = uint16_t(0x1100u + i);
        tick_and_compare(sim);
    }

    uint32_t seed = 0xC001D00Du;
    for (int i = 0; i < 4096; ++i) {
        sim.dut->read_set_i = xorshift32(seed) & 0xF;
        sim.dut->write_en_i = xorshift32(seed) & 1u;
        sim.dut->write_set_i = xorshift32(seed) & 0xF;
        sim.dut->write_data_i = uint16_t(xorshift32(seed));
        tick_and_compare(sim);
    }

    return sim.finish();
}
