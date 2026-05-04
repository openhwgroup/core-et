// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_adder_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vtxfma_adder_tb;

namespace {

uint64_t xorshift64(uint64_t& state) {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state;
}

void check_case(SimCtrl<DUT>& sim, uint64_t a, uint64_t b, bool cin, const char* name) {
    sim.dut->a_i = a;
    sim.dut->b_i = b;
    sim.dut->cin_i = cin;
    sim.dut->eval();
    sim.check(sim.dut->sum_o == a + b + static_cast<uint64_t>(cin), name);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    check_case(sim, 0x0000000000000000ull, 0x0000000000000000ull, false, "all_zero");
    check_case(sim, 0x0000000000000000ull, 0x0000000000000000ull, true, "cin_only");
    check_case(sim, 0xffffffffffffffffull, 0x0000000000000001ull, false, "wrap_no_cin");
    check_case(sim, 0xffffffffffffffffull, 0x0000000000000000ull, true, "wrap_with_cin");
    check_case(sim, 0x123456789abcdef0ull, 0x0fedcba987654321ull, true, "mixed_pattern");

    uint64_t seed = 0x4c3b2a1908172635ull;
    for (int i = 0; i < 4096; ++i) {
        check_case(sim, xorshift64(seed), xorshift64(seed), xorshift64(seed) & 1ull, "random");
    }

    return sim.finish();
}
