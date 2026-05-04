// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_4_2_compressor_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vtxfma_4_2_compressor_tb;

namespace {

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

bool majority(bool a, bool b, bool c) {
    return (a && b) || (a && c) || (b && c);
}

void check_case(SimCtrl<DUT>& sim, bool x0, bool x1, bool x2, bool x3, bool cin, const char* name) {
    const bool s0 = x0 ^ x1 ^ x2;
    const bool cout = majority(x0, x1, x2);
    const bool c = majority(s0, x3, cin);
    const bool s = s0 ^ x3 ^ cin;

    sim.dut->x0_i = x0;
    sim.dut->x1_i = x1;
    sim.dut->x2_i = x2;
    sim.dut->x3_i = x3;
    sim.dut->cin_i = cin;
    sim.dut->eval();

    sim.check(sim.dut->cout_o == cout, name);
    sim.check(sim.dut->c_o == c, name);
    sim.check(sim.dut->s_o == s, name);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    for (int value = 0; value < 32; ++value) {
        check_case(sim,
                   (value & 0x01) != 0,
                   (value & 0x02) != 0,
                   (value & 0x04) != 0,
                   (value & 0x08) != 0,
                   (value & 0x10) != 0,
                   "exhaustive");
    }

    uint32_t seed = 0x8f61d24bu;
    for (int i = 0; i < 1024; ++i) {
        const uint32_t sample = xorshift32(seed);
        check_case(sim,
                   (sample & 0x01u) != 0u,
                   (sample & 0x02u) != 0u,
                   (sample & 0x04u) != 0u,
                   (sample & 0x08u) != 0u,
                   (sample & 0x10u) != 0u,
                   "random");
    }

    return sim.finish();
}
