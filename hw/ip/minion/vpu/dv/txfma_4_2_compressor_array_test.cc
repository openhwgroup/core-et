// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_4_2_compressor_array_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vtxfma_4_2_compressor_array_tb;

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

void model(uint64_t x0, uint64_t x1, uint64_t x2, uint64_t x3, uint64_t& c, uint64_t& s) {
    c = 0;
    s = 0;
    bool carry = false;
    for (int bit = 0; bit < 64; ++bit) {
        const bool b0 = ((x0 >> bit) & 1u) != 0u;
        const bool b1 = ((x1 >> bit) & 1u) != 0u;
        const bool b2 = ((x2 >> bit) & 1u) != 0u;
        const bool b3 = ((x3 >> bit) & 1u) != 0u;
        const bool s0 = b0 ^ b1 ^ b2;
        const bool next_carry = majority(b0, b1, b2);
        const bool c_bit = majority(s0, b3, carry);
        const bool s_bit = s0 ^ b3 ^ carry;
        c |= static_cast<uint64_t>(c_bit) << bit;
        s |= static_cast<uint64_t>(s_bit) << bit;
        carry = next_carry;
    }
}

void check_case(SimCtrl<DUT>& sim, uint64_t x0, uint64_t x1, uint64_t x2, uint64_t x3, const char* name) {
    uint64_t expected_c = 0;
    uint64_t expected_s = 0;
    model(x0, x1, x2, x3, expected_c, expected_s);

    sim.dut->x0_i = x0;
    sim.dut->x1_i = x1;
    sim.dut->x2_i = x2;
    sim.dut->x3_i = x3;
    sim.dut->eval();

    sim.check(sim.dut->c_o == expected_c, name);
    sim.check(sim.dut->s_o == expected_s, name);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    check_case(sim, 0, 0, 0, 0, "all_zero");
    check_case(sim, ~0ull, 0, 0, 0, "x0_only");
    check_case(sim, 0, ~0ull, 0, 0, "x1_only");
    check_case(sim, 0, 0, ~0ull, 0, "x2_only");
    check_case(sim, 0, 0, 0, ~0ull, "x3_only");
    check_case(sim, 0xaaaaaaaaaaaaaaaaull, 0x5555555555555555ull,
               0x3333333333333333ull, 0x0f0f0f0f0f0f0f0full, "mixed_pattern");

    uint32_t seed = 0x17c4e28du;
    for (int i = 0; i < 256; ++i) {
        const uint64_t x0 = (static_cast<uint64_t>(xorshift32(seed)) << 32) | xorshift32(seed);
        const uint64_t x1 = (static_cast<uint64_t>(xorshift32(seed)) << 32) | xorshift32(seed);
        const uint64_t x2 = (static_cast<uint64_t>(xorshift32(seed)) << 32) | xorshift32(seed);
        const uint64_t x3 = (static_cast<uint64_t>(xorshift32(seed)) << 32) | xorshift32(seed);
        check_case(sim, x0, x1, x2, x3, "random");
    }

    return sim.finish();
}
