// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_csa_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vtxfma_csa_tb;

namespace {

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

void check_case(SimCtrl<DUT>& sim, uint32_t a, uint32_t b, uint32_t cin, const char* name) {
    sim.dut->a_i = a;
    sim.dut->b_i = b;
    sim.dut->cin_i = cin;
    sim.dut->eval();
    sim.check(sim.dut->sum_o == (a ^ b ^ cin), name);
    sim.check(sim.dut->cout_o == ((a & b) | (a & cin) | (b & cin)), name);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    check_case(sim, 0x00000000u, 0x00000000u, 0x00000000u, "all_zero");
    check_case(sim, 0xffffffffu, 0x00000000u, 0x00000000u, "a_only");
    check_case(sim, 0x00000000u, 0xffffffffu, 0x00000000u, "b_only");
    check_case(sim, 0x00000000u, 0x00000000u, 0xffffffffu, "cin_only");
    check_case(sim, 0xaaaaaaaau, 0x55555555u, 0x33333333u, "mixed_pattern");

    uint32_t seed = 0x4c3b2a19u;
    for (int i = 0; i < 256; ++i) {
        check_case(sim, xorshift32(seed), xorshift32(seed), xorshift32(seed), "random");
    }

    return sim.finish();
}
