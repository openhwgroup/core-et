// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_wallace1_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>

using DUT = Vtxfma_wallace1_tb;

namespace {

constexpr uint64_t kMask33 = (1ull << 33) - 1ull;

struct Csa42Outputs {
    uint64_t c = 0;
    uint64_t s = 0;
};

struct Wallace1Outputs {
    uint64_t c21 = 0;
    uint64_t c22 = 0;
    uint64_t s21 = 0;
    uint64_t s22 = 0;
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

Wallace1Outputs model(std::array<uint64_t, 16> pp, bool mode) {
    Wallace1Outputs out;
    std::array<uint64_t, 16> pp_ext{};
    const bool ph = mode;
    const bool ps_int32 = !mode;

    pp_ext[0]  = (pp[0] & kMask33) | (static_cast<uint64_t>(ps_int32) << 33) | (static_cast<uint64_t>(ph) << 13);
    pp_ext[1]  = (pp[1] & kMask33) << 2;
    pp_ext[2]  = (pp[2] & kMask33) << 4;
    pp_ext[3]  = (pp[3] & kMask33) << 6;
    pp_ext[4]  = (pp[4] & kMask33) << 8;
    pp_ext[5]  = (pp[5] & kMask33) << 10;
    pp_ext[6]  = (pp[6] & kMask33) << 12;
    pp_ext[7]  = (pp[7] & kMask33) << 14;
    pp_ext[8]  = (pp[8] & kMask33) << 16;
    pp_ext[9]  = (pp[9] & kMask33) << 18;
    pp_ext[10] = ((pp[10] & kMask33) << 20) | (static_cast<uint64_t>(ph) << 53);
    pp_ext[11] = (pp[11] & kMask33) << 22;
    pp_ext[12] = (pp[12] & kMask33) << 24;
    pp_ext[13] = (pp[13] & kMask33) << 26;
    pp_ext[14] = (pp[14] & kMask33) << 28;
    pp_ext[15] = (pp[15] & kMask33) << 30;

    const Csa42Outputs l11 = model_csa42_array(pp_ext[0], pp_ext[1], pp_ext[2], pp_ext[3]);
    const Csa42Outputs l12 = model_csa42_array(pp_ext[4], pp_ext[5], pp_ext[6], pp_ext[7]);
    const Csa42Outputs l13 = model_csa42_array(pp_ext[8], pp_ext[9], pp_ext[10], pp_ext[11]);
    const Csa42Outputs l14 = model_csa42_array(pp_ext[12], pp_ext[13], pp_ext[14], pp_ext[15]);

    const Csa42Outputs l21 = model_csa42_array((l11.c << 1), l11.s, (l12.c << 1), l12.s);
    const Csa42Outputs l22 = model_csa42_array((l13.c << 1), l13.s, (l14.c << 1), l14.s);

    out.c21 = l21.c;
    out.s21 = l21.s;
    out.c22 = l22.c;
    out.s22 = l22.s;
    return out;
}

void drive_inputs(DUT* dut, const std::array<uint64_t, 16>& pp, bool mode) {
    dut->pp1_i = pp[0];
    dut->pp2_i = pp[1];
    dut->pp3_i = pp[2];
    dut->pp4_i = pp[3];
    dut->pp5_i = pp[4];
    dut->pp6_i = pp[5];
    dut->pp7_i = pp[6];
    dut->pp8_i = pp[7];
    dut->pp9_i = pp[8];
    dut->pp10_i = pp[9];
    dut->pp11_i = pp[10];
    dut->pp12_i = pp[11];
    dut->pp13_i = pp[12];
    dut->pp14_i = pp[13];
    dut->pp15_i = pp[14];
    dut->pp16_i = pp[15];
    dut->mode_i = mode;
}

void check_case(SimCtrl<DUT>& sim, const std::array<uint64_t, 16>& pp, bool mode, const char* name) {
    const Wallace1Outputs exp = model(pp, mode);
    drive_inputs(sim.dut.get(), pp, mode);
    sim.dut->eval();

    sim.check(sim.dut->c21_o == exp.c21, name);
    sim.check(sim.dut->c22_o == exp.c22, name);
    sim.check(sim.dut->s21_o == exp.s21, name);
    sim.check(sim.dut->s22_o == exp.s22, name);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    std::array<uint64_t, 16> pp{};
    check_case(sim, pp, false, "all_zero_ps");
    check_case(sim, pp, true, "all_zero_ph");

    pp.fill(kMask33);
    check_case(sim, pp, false, "all_ones_ps");
    check_case(sim, pp, true, "all_ones_ph");

    pp.fill(0);
    pp[0] = 1ull << 5;
    pp[10] = 1ull << 10;
    check_case(sim, pp, false, "magic_one_ps");
    check_case(sim, pp, true, "magic_one_ph");

    uint32_t seed = 0x1b7d93a5u;
    for (int i = 0; i < 2048; ++i) {
        for (int j = 0; j < 16; ++j) {
            pp[j] = rand33(seed);
        }
        check_case(sim, pp, (xorshift32(seed) & 1u) != 0u, "random");
    }

    return sim.finish();
}
