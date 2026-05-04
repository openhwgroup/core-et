// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_trz_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vcosim_txfma_trz_tb;

namespace {

constexpr uint32_t kMax = 23u;
constexpr uint32_t kFracWidth = kMax + 1u;
constexpr uint32_t kFracMask = (1u << kFracWidth) - 1u;
constexpr int kRandomIters = 25000;

struct WordCoverage {
    uint32_t or_mask = 0u;
    uint32_t and_mask = kFracMask;

    void observe(uint32_t value) {
        or_mask |= value;
        and_mask &= value;
    }
};

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

void drive_and_compare(CosimCtrl<DUT>& sim, WordCoverage& frac_cov, uint32_t frac, const char* name) {
    sim.dut->frac_i = frac & kFracMask;
    frac_cov.observe(frac & kFracMask);
    sim.dut->eval();
    sim.compare("trz", sim.dut->orig_trz_o, sim.dut->new_trz_o);
    sim.check(sim.dut->new_trz_o == model_trz(frac), name);
    sim.sim_time++;
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    WordCoverage frac_cov;

    drive_and_compare(sim, frac_cov, 0x000000u, "all_zero");
    drive_and_compare(sim, frac_cov, 0x000001u, "lsb_set");
    drive_and_compare(sim, frac_cov, 0x800000u, "msb_set");
    drive_and_compare(sim, frac_cov, 0x800204u, "lowest_set_bit_wins");
    drive_and_compare(sim, frac_cov, 0x000800u, "middle_set");
    drive_and_compare(sim, frac_cov, 0xffffffu, "all_ones");
    for (uint32_t bit = 0; bit < kFracWidth; ++bit) {
        drive_and_compare(sim, frac_cov, 1u << bit, "walk_single");
        drive_and_compare(sim, frac_cov, (1u << bit) | (1u << ((bit + 5u) % kFracWidth)), "walk_twohot");
    }

    uint32_t seed = 0x1a2b3c4du;
    for (int i = 0; i < kRandomIters; ++i) {
        drive_and_compare(sim, frac_cov, xorshift32(seed), "random");
    }

    sim.check(frac_cov.or_mask == kFracMask, "frac_i should drive every bit high");
    sim.check(frac_cov.and_mask == 0u, "frac_i should drive every bit low");
    sim.check(sim.comparisons >= 25000, "txfma_trz cosim should exceed 25k comparisons");
    return sim.finish();
}
