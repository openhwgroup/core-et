// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_uncached_arb_lru_grant_tb.h"
#include "cosim_ctrl.h"
#include <cstdint>

static uint32_t xorshift32(uint32_t& s) {
    s ^= s << 13;
    s ^= s >> 17;
    s ^= s << 5;
    return s;
}

int main(int argc, char** argv) {
    CosimCtrl<Vcosim_uncached_arb_lru_grant_tb> sim(argc, argv);
    sim.dut->bid_i = 0;
    sim.dut->stall_i = 0;
    sim.reset();

    uint32_t seed = 0x75a5f00du;
    for (int i = 0; i < 4096; ++i) {
        if (i < 64) {
            sim.dut->bid_i = static_cast<uint8_t>(i & 0x1f);
            sim.dut->stall_i = static_cast<uint8_t>((i >> 5) & 1);
        } else {
            uint32_t r = xorshift32(seed);
            sim.dut->bid_i = static_cast<uint8_t>(r & 0x1f);
            sim.dut->stall_i = static_cast<uint8_t>((r >> 8) & 1);
        }
        sim.tick();
        sim.compare("grant", sim.dut->orig_grant_o, sim.dut->new_grant_o);
        sim.compare("winner", sim.dut->orig_winner_o, sim.dut->new_winner_o);
    }

    return sim.finish();
}
