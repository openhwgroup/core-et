// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_trz_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vtxfma_trz_tb;

namespace {

constexpr uint32_t kMax = 23u;
constexpr uint32_t kFracWidth = kMax + 1u;
constexpr uint32_t kFracMask = (1u << kFracWidth) - 1u;

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

uint32_t model_trz(uint32_t frac) {
    frac &= kFracMask;
    if (frac == 0u) {
        return kFracWidth;
    }
    for (uint32_t i = 0; i < kFracWidth; ++i) {
        if (((frac >> i) & 1u) != 0u) {
            return i;
        }
    }
    return kFracWidth;
}

void check_case(SimCtrl<DUT>& sim, uint32_t frac, const char* name) {
    sim.dut->frac_i = frac & kFracMask;
    sim.dut->eval();
    sim.check(sim.dut->trz_o == model_trz(frac), name);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    check_case(sim, 0x000000u, "all_zero");
    check_case(sim, 0x000001u, "lsb_set");
    check_case(sim, 0x800000u, "msb_set");
    check_case(sim, 0x800204u, "lowest_set_bit_wins");
    check_case(sim, 0x000800u, "middle_set");
    check_case(sim, 0xffffffu, "all_ones");

    uint32_t seed = 0x1a2b3c4du;
    for (int i = 0; i < 4096; ++i) {
        check_case(sim, xorshift32(seed), "random");
    }

    return sim.finish();
}
