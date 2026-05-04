// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_trz2_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_txfma_trz2_tb;

namespace {

constexpr uint64_t kInWidth = 52u;
constexpr uint64_t kInMask = (1ull << kInWidth) - 1ull;
constexpr int kRandomIters = 25000;

struct WordCoverage {
    uint64_t or_mask = 0ull;
    uint64_t and_mask = kInMask;

    void observe(uint64_t value) {
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

uint64_t model_trz2(uint64_t in1, uint64_t in2, bool add_cin) {
    const uint64_t p = (in1 ^ in2) & kInMask;
    const uint64_t k = (~in1 & ~in2) & kInMask;

    if ((((p >> 0) & 1ull) ^ static_cast<uint64_t>(add_cin)) != 0ull) {
        return 0;
    }
    for (uint64_t i = 1; i < kInWidth; ++i) {
        const bool p_i = ((p >> i) & 1ull) != 0ull;
        const bool k_prev = ((k >> (i - 1)) & 1ull) != 0ull;
        if ((p_i && k_prev) || (!p_i && !k_prev)) {
            return i;
        }
    }
    return kInWidth;
}

void drive_and_compare(CosimCtrl<DUT>& sim, WordCoverage& in1_cov, WordCoverage& in2_cov,
                       bool& cin_seen_0, bool& cin_seen_1, uint64_t in1, uint64_t in2,
                       bool add_cin, const char* name) {
    sim.dut->in1_i = in1 & kInMask;
    sim.dut->in2_i = in2 & kInMask;
    sim.dut->add_cin_i = add_cin;
    in1_cov.observe(in1 & kInMask);
    in2_cov.observe(in2 & kInMask);
    cin_seen_0 |= !add_cin;
    cin_seen_1 |= add_cin;
    sim.dut->eval();
    sim.compare("trz", sim.dut->orig_trz_o, sim.dut->new_trz_o);
    sim.check(sim.dut->new_trz_o == model_trz2(in1, in2, add_cin), name);
    sim.sim_time++;
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    WordCoverage in1_cov;
    WordCoverage in2_cov;
    bool cin_seen_0 = false;
    bool cin_seen_1 = false;

    drive_and_compare(sim, in1_cov, in2_cov, cin_seen_0, cin_seen_1,
                      0x0000000000000ull, 0x0000000000000ull, false, "all_zero_no_cin");
    drive_and_compare(sim, in1_cov, in2_cov, cin_seen_0, cin_seen_1,
                      0x0000000000000ull, 0x0000000000000ull, true, "all_zero_with_cin");
    drive_and_compare(sim, in1_cov, in2_cov, cin_seen_0, cin_seen_1,
                      0x0000000000001ull, 0x0000000000000ull, false, "lsb_propagate");
    drive_and_compare(sim, in1_cov, in2_cov, cin_seen_0, cin_seen_1,
                      0x0000000000001ull, 0x0000000000001ull, false, "lsb_kill");
    drive_and_compare(sim, in1_cov, in2_cov, cin_seen_0, cin_seen_1,
                      0x8000000000000ull, 0x0000000000000ull, false, "msb_propagate");
    drive_and_compare(sim, in1_cov, in2_cov, cin_seen_0, cin_seen_1,
                      0xfffffffffffffull, 0x5555555555555ull, true, "mixed_pattern");

    for (uint64_t bit = 0; bit < kInWidth; ++bit) {
        drive_and_compare(sim, in1_cov, in2_cov, cin_seen_0, cin_seen_1,
                          1ull << bit, 0ull, false, "walk_in1");
        drive_and_compare(sim, in1_cov, in2_cov, cin_seen_0, cin_seen_1,
                          0ull, 1ull << bit, false, "walk_in2");
        drive_and_compare(sim, in1_cov, in2_cov, cin_seen_0, cin_seen_1,
                          1ull << bit, 1ull << bit, true, "walk_both");
    }

    uint32_t seed = 0x5a6b7c8du;
    for (int i = 0; i < kRandomIters; ++i) {
        const uint64_t in1 = (static_cast<uint64_t>(xorshift32(seed)) << 20) |
                             (xorshift32(seed) & 0xfffffull);
        const uint64_t in2 = (static_cast<uint64_t>(xorshift32(seed)) << 20) |
                             (xorshift32(seed) & 0xfffffull);
        drive_and_compare(sim, in1_cov, in2_cov, cin_seen_0, cin_seen_1,
                          in1, in2, (xorshift32(seed) & 1u) != 0u, "random");
    }

    sim.check(in1_cov.or_mask == kInMask, "in1_i should drive every bit high");
    sim.check(in1_cov.and_mask == 0ull, "in1_i should drive every bit low");
    sim.check(in2_cov.or_mask == kInMask, "in2_i should drive every bit high");
    sim.check(in2_cov.and_mask == 0ull, "in2_i should drive every bit low");
    sim.check(cin_seen_0, "add_cin_i should be driven low");
    sim.check(cin_seen_1, "add_cin_i should be driven high");
    sim.check(sim.comparisons >= 25000, "txfma_trz2 cosim should exceed 25k comparisons");
    return sim.finish();
}
