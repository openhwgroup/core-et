// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_lxd_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_txfma_lxd_tb;

namespace {

constexpr uint64_t kInWidth = 52u;
constexpr uint64_t kAftA0BitsNum = 4u;
constexpr uint64_t kOutDefault = kInWidth - kAftA0BitsNum;
constexpr uint64_t kOutMask = (1ull << 7) - 1ull;
constexpr uint64_t kInMask = (1ull << kInWidth) - 1ull;
constexpr int kRandomIters = 25000;

struct WordCoverage {
    uint64_t or_mask = 0ull;
    uint64_t and_mask = kInMask;

    void observe(uint64_t value) {
        const uint64_t masked = value & kInMask;
        or_mask |= masked;
        and_mask &= masked;
    }
};

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

uint64_t model_lxd(uint64_t in1, uint64_t in2, bool sub_op) {
    const uint64_t in1_m = in1 & kInMask;
    const uint64_t in2_m = in2 & kInMask;
    uint64_t p[kInWidth + 1] = {};
    uint64_t g[kInWidth + 1] = {};
    uint64_t k[kInWidth + 1] = {};
    bool indicator[kInWidth] = {};

    p[kInWidth] = static_cast<uint64_t>(sub_op);
    for (uint64_t i = 0; i < kInWidth; ++i) {
        const uint64_t bit1 = (in1_m >> i) & 1ull;
        const uint64_t bit2 = (in2_m >> i) & 1ull;
        g[i] = bit1 & bit2;
        p[i] = bit1 ^ bit2;
        k[i] = (~bit1 & ~bit2) & 1ull;
    }

    indicator[0] = true;
    for (uint64_t i = 1; i < kInWidth; ++i) {
        indicator[i] =
            (p[i + 1] != 0ull &&
             (((g[i] != 0ull) && (k[i - 1] == 0ull)) || ((k[i] != 0ull) && (g[i - 1] == 0ull)))) ||
            (p[i + 1] == 0ull &&
             (((k[i] != 0ull) && (k[i - 1] == 0ull)) || ((g[i] != 0ull) && (g[i - 1] == 0ull))));
    }

    uint64_t lxd = kOutDefault;
    for (uint64_t i = 0; i < kInWidth; ++i) {
        if (indicator[i]) {
            lxd = (kOutDefault - i) & kOutMask;
        }
    }
    return lxd & kOutMask;
}

void drive_and_compare(CosimCtrl<DUT>& sim, WordCoverage& in1_cov, WordCoverage& in2_cov,
                       bool& sub_seen_0, bool& sub_seen_1, uint64_t in1, uint64_t in2,
                       bool sub_op, const char* name) {
    sim.dut->in1_i = in1 & kInMask;
    sim.dut->in2_i = in2 & kInMask;
    sim.dut->sub_op_i = sub_op;
    in1_cov.observe(in1);
    in2_cov.observe(in2);
    sub_seen_0 |= !sub_op;
    sub_seen_1 |= sub_op;
    sim.dut->eval();
    sim.compare("lxd", sim.dut->orig_lxd_o, sim.dut->new_lxd_o);
    sim.check(sim.dut->new_lxd_o == model_lxd(in1, in2, sub_op), name);
    sim.sim_time++;
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    WordCoverage in1_cov;
    WordCoverage in2_cov;
    bool sub_seen_0 = false;
    bool sub_seen_1 = false;

    drive_and_compare(sim, in1_cov, in2_cov, sub_seen_0, sub_seen_1,
                      0x0000000000000ull, 0x0000000000000ull, false, "all_zero_add");
    drive_and_compare(sim, in1_cov, in2_cov, sub_seen_0, sub_seen_1,
                      0x0000000000000ull, 0x0000000000000ull, true, "all_zero_sub");
    drive_and_compare(sim, in1_cov, in2_cov, sub_seen_0, sub_seen_1,
                      0x0000000000001ull, 0x0000000000000ull, false, "lsb_propagate");
    drive_and_compare(sim, in1_cov, in2_cov, sub_seen_0, sub_seen_1,
                      0x0000000000001ull, 0x0000000000001ull, false, "lsb_generate");
    drive_and_compare(sim, in1_cov, in2_cov, sub_seen_0, sub_seen_1,
                      0x8000000000000ull, 0x0000000000000ull, false, "msb_propagate");
    drive_and_compare(sim, in1_cov, in2_cov, sub_seen_0, sub_seen_1,
                      0xfffffffffffffull, 0x5555555555555ull, true, "mixed_pattern");
    for (int bit = 0; bit < 52; ++bit) {
        const uint64_t one_hot = 1ull << bit;
        drive_and_compare(sim, in1_cov, in2_cov, sub_seen_0, sub_seen_1,
                          one_hot, 0ull, false, "walk_in1");
        drive_and_compare(sim, in1_cov, in2_cov, sub_seen_0, sub_seen_1,
                          0ull, one_hot, false, "walk_in2");
        drive_and_compare(sim, in1_cov, in2_cov, sub_seen_0, sub_seen_1,
                          one_hot, one_hot, true, "walk_sub");
    }

    uint32_t seed = 0x31415926u;
    for (int i = 0; i < kRandomIters; ++i) {
        const uint64_t in1 = (static_cast<uint64_t>(xorshift32(seed)) << 20) |
                             (xorshift32(seed) & 0xfffffull);
        const uint64_t in2 = (static_cast<uint64_t>(xorshift32(seed)) << 20) |
                             (xorshift32(seed) & 0xfffffull);
        drive_and_compare(sim, in1_cov, in2_cov, sub_seen_0, sub_seen_1,
                          in1, in2, (xorshift32(seed) & 1u) != 0u, "random");
    }

    sim.check(in1_cov.or_mask == kInMask, "in1_i should drive every bit high");
    sim.check(in1_cov.and_mask == 0ull, "in1_i should drive every bit low");
    sim.check(in2_cov.or_mask == kInMask, "in2_i should drive every bit high");
    sim.check(in2_cov.and_mask == 0ull, "in2_i should drive every bit low");
    sim.check(sub_seen_0, "sub_op_i should be driven low");
    sim.check(sub_seen_1, "sub_op_i should be driven high");
    sim.check(sim.comparisons >= 25000, "txfma_lxd cosim should exceed 25k comparisons");
    return sim.finish();
}
