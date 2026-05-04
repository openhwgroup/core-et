// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_lxd_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vtxfma_lxd_tb;

namespace {

constexpr uint64_t kInWidth = 52u;
constexpr uint64_t kAftA0BitsNum = 4u;
constexpr uint64_t kOutDefault = kInWidth - kAftA0BitsNum;
constexpr uint64_t kOutMask = (1ull << 7) - 1ull;
constexpr uint64_t kInMask = (1ull << kInWidth) - 1ull;

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

void check_case(SimCtrl<DUT>& sim, uint64_t in1, uint64_t in2, bool sub_op, const char* name) {
    sim.dut->in1_i = in1 & kInMask;
    sim.dut->in2_i = in2 & kInMask;
    sim.dut->sub_op_i = sub_op;
    sim.dut->eval();
    sim.check(sim.dut->lxd_o == model_lxd(in1, in2, sub_op), name);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    check_case(sim, 0x0000000000000ull, 0x0000000000000ull, false, "all_zero_add");
    check_case(sim, 0x0000000000000ull, 0x0000000000000ull, true, "all_zero_sub");
    check_case(sim, 0x0000000000001ull, 0x0000000000000ull, false, "lsb_propagate");
    check_case(sim, 0x0000000000001ull, 0x0000000000001ull, false, "lsb_generate");
    check_case(sim, 0x8000000000000ull, 0x0000000000000ull, false, "msb_propagate");
    check_case(sim, 0xfffffffffffffull, 0x5555555555555ull, true, "mixed_pattern");

    uint32_t seed = 0x31415926u;
    for (int i = 0; i < 4096; ++i) {
        const uint64_t in1 = (static_cast<uint64_t>(xorshift32(seed)) << 20) |
                             (xorshift32(seed) & 0xfffffull);
        const uint64_t in2 = (static_cast<uint64_t>(xorshift32(seed)) << 20) |
                             (xorshift32(seed) & 0xfffffull);
        check_case(sim, in1, in2, (xorshift32(seed) & 1u) != 0u, "random");
    }

    return sim.finish();
}
