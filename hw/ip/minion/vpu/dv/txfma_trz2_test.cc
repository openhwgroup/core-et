// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_trz2_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vtxfma_trz2_tb;

namespace {

constexpr uint64_t kInWidth = 52u;
constexpr uint64_t kInMask = (1ull << kInWidth) - 1ull;

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

void check_case(SimCtrl<DUT>& sim, uint64_t in1, uint64_t in2, bool add_cin, const char* name) {
    sim.dut->in1_i = in1 & kInMask;
    sim.dut->in2_i = in2 & kInMask;
    sim.dut->add_cin_i = add_cin;
    sim.dut->eval();
    sim.check(sim.dut->trz_o == model_trz2(in1, in2, add_cin), name);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    check_case(sim, 0x0000000000000ull, 0x0000000000000ull, false, "all_zero_no_cin");
    check_case(sim, 0x0000000000000ull, 0x0000000000000ull, true, "all_zero_with_cin");
    check_case(sim, 0x0000000000001ull, 0x0000000000000ull, false, "lsb_propagate");
    check_case(sim, 0x0000000000001ull, 0x0000000000001ull, false, "lsb_kill");
    check_case(sim, 0x8000000000000ull, 0x0000000000000ull, false, "msb_propagate");
    check_case(sim, 0xfffffffffffffull, 0x5555555555555ull, true, "mixed_pattern");

    uint32_t seed = 0x5a6b7c8du;
    for (int i = 0; i < 4096; ++i) {
        const uint64_t in1 = (static_cast<uint64_t>(xorshift32(seed)) << 20) |
                             (xorshift32(seed) & 0xfffffull);
        const uint64_t in2 = (static_cast<uint64_t>(xorshift32(seed)) << 20) |
                             (xorshift32(seed) & 0xfffffull);
        check_case(sim, in1, in2, (xorshift32(seed) & 1u) != 0u, "random");
    }

    return sim.finish();
}
