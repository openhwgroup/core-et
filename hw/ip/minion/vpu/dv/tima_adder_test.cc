// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtima_adder_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vtima_adder_tb;

namespace {

void check_sum(SimCtrl<DUT>& sim, uint32_t a, uint32_t b, uint32_t expected, const char* name) {
    sim.dut->a_i = a;
    sim.dut->b_i = b;
    sim.dut->eval();
    sim.check(sim.dut->sum_o == expected, name);
}

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    check_sum(sim, 0x00000000u, 0x00000000u, 0x00000000u, "zero");
    check_sum(sim, 0x00000001u, 0x00000002u, 0x00000003u, "small add");
    check_sum(sim, 0xffffffffu, 0x00000001u, 0x00000000u, "wraparound");
    check_sum(sim, 0x80000000u, 0x80000000u, 0x00000000u, "high-bit wrap");
    check_sum(sim, 0x12345678u, 0x87654321u, 0x99999999u, "mixed pattern");

    uint32_t seed = 0x71adda32u;
    for (int i = 0; i < 64; ++i) {
        uint32_t a = xorshift32(seed);
        uint32_t b = xorshift32(seed);
        check_sum(sim, a, b, a + b, "random add");
    }

    return sim.finish();
}
