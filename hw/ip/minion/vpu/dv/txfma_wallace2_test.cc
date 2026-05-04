// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_wallace2_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vtxfma_wallace2_tb;

namespace {

constexpr uint64_t kMask33 = (1ull << 33) - 1ull;

struct Csa42Outputs {
    uint64_t c = 0;
    uint64_t s = 0;
};

struct Wallace2Outputs {
    uint64_t c41 = 0;
    uint64_t s41 = 0;
};

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

uint64_t rand33(uint32_t& state) {
    return ((static_cast<uint64_t>(xorshift32(state)) << 1) | (xorshift32(state) & 1u)) & kMask33;
}

bool majority(bool a, bool b, bool c) {
    return (a && b) || (a && c) || (b && c);
}

Csa42Outputs model_csa42_array(uint64_t x0, uint64_t x1, uint64_t x2, uint64_t x3) {
    Csa42Outputs out;
    bool carry = false;

    for (int i = 0; i < 64; ++i) {
        const bool x0b = ((x0 >> i) & 1ull) != 0ull;
        const bool x1b = ((x1 >> i) & 1ull) != 0ull;
        const bool x2b = ((x2 >> i) & 1ull) != 0ull;
        const bool x3b = ((x3 >> i) & 1ull) != 0ull;

        const bool s0 = x0b ^ x1b ^ x2b;
        const bool cout = majority(x0b, x1b, x2b);
        const bool s = s0 ^ x3b ^ carry;
        const bool c = majority(s0, x3b, carry);

        out.c |= static_cast<uint64_t>(c) << i;
        out.s |= static_cast<uint64_t>(s) << i;
        carry = cout;
    }

    return out;
}

Wallace2Outputs model(uint32_t sxt, uint32_t cin, uint64_t pp17, uint64_t c21, uint64_t c22, uint64_t s21, uint64_t s22) {
    Wallace2Outputs out;
    const uint64_t pp17_ext = (pp17 & 0xffff'ffffull) << 32;
    const uint64_t sxt_cin = (static_cast<uint64_t>(sxt) << 32) | cin;

    const Csa42Outputs l31 = model_csa42_array(c21 << 1, s21, c22 << 1, s22);
    const Csa42Outputs l41 = model_csa42_array(l31.c << 1, l31.s, pp17_ext, sxt_cin);

    out.c41 = l41.c;
    out.s41 = l41.s;
    return out;
}

void check_case(SimCtrl<DUT>& sim, uint32_t sxt, uint32_t cin, uint64_t pp17,
                uint64_t c21, uint64_t c22, uint64_t s21, uint64_t s22, const char* name) {
    const Wallace2Outputs exp = model(sxt, cin, pp17, c21, c22, s21, s22);
    sim.dut->sxt_i = sxt;
    sim.dut->cin_i = cin;
    sim.dut->pp17_i = pp17;
    sim.dut->c21_i = c21;
    sim.dut->c22_i = c22;
    sim.dut->s21_i = s21;
    sim.dut->s22_i = s22;
    sim.dut->eval();

    sim.check(sim.dut->c41_o == exp.c41, name);
    sim.check(sim.dut->s41_o == exp.s41, name);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    check_case(sim, 0, 0, 0, 0, 0, 0, 0, "all_zero");
    check_case(sim, 0xffff'ffffu, 0xffff'ffffu, kMask33, ~0ull, ~0ull, ~0ull, ~0ull, "all_one");
    check_case(sim, 0x1234'5678u, 0x9abc'def0u, 1ull << 32, 0, 0, 0, 0, "pp17_msb_dropped");
    check_case(sim, 0x89ab'cdefu, 0x1357'9bdfu, 0x1ffff'ffffull,
               0x0123'4567'89ab'cdefull, 0xfedc'ba98'7654'3210ull,
               0x55aa'55aa'55aa'55aaull, 0xaa55'aa55'aa55'aa55ull, "mixed");

    uint32_t seed = 0x50d7d34bu;
    for (int i = 0; i < 4096; ++i) {
        check_case(sim,
                   xorshift32(seed),
                   xorshift32(seed),
                   rand33(seed),
                   (static_cast<uint64_t>(xorshift32(seed)) << 32) | xorshift32(seed),
                   (static_cast<uint64_t>(xorshift32(seed)) << 32) | xorshift32(seed),
                   (static_cast<uint64_t>(xorshift32(seed)) << 32) | xorshift32(seed),
                   (static_cast<uint64_t>(xorshift32(seed)) << 32) | xorshift32(seed),
                   "random");
    }

    return sim.finish();
}
