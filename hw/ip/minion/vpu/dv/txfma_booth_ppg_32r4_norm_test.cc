// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_booth_ppg_32r4_norm_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vtxfma_booth_ppg_32r4_norm_tb;

namespace {

constexpr uint64_t kMask33 = (1ull << 33) - 1ull;

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

uint64_t sign_extend33(uint32_t value, bool sign) {
    if (!sign) return static_cast<uint64_t>(value);
    return (static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(value))) & kMask33);
}

uint64_t model_pp(uint32_t mulcand, uint32_t r4input, bool sign) {
    const uint64_t signed_mulcand = sign_extend33(mulcand, sign);
    switch (r4input & 0x7u) {
      case 0u:
      case 7u:
        return 0;
      case 1u:
      case 2u:
        return signed_mulcand;
      case 5u:
      case 6u:
        return (~signed_mulcand) & kMask33;
      case 3u:
        return (static_cast<uint64_t>(mulcand) << 1) & kMask33;
      case 4u:
        return (~((static_cast<uint64_t>(mulcand) << 1) & kMask33)) & kMask33;
      default:
        return 0;
    }
}

void check_case(SimCtrl<DUT>& sim, uint32_t mulcand, uint32_t r4input, bool sign, const char* name) {
    sim.dut->mulcand_i = mulcand;
    sim.dut->r4input_i = r4input;
    sim.dut->sign_i = sign;
    sim.dut->eval();
    sim.check(static_cast<uint64_t>(sim.dut->pp_o) == model_pp(mulcand, r4input, sign), name);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    const uint32_t patterns[] = {
        0x00000000u, 0x00000001u, 0x80000000u, 0xffffffffu, 0x12345678u, 0x87654321u,
    };

    for (uint32_t pattern : patterns) {
        for (uint32_t r4input = 0; r4input < 8; ++r4input) {
            check_case(sim, pattern, r4input, false, "direct_unsigned");
            check_case(sim, pattern, r4input, true, "direct_signed");
        }
    }

    uint32_t seed = 0x2f43b51eu;
    for (int i = 0; i < 512; ++i) {
        check_case(sim, xorshift32(seed), xorshift32(seed) & 0x7u, (xorshift32(seed) & 1u) != 0u, "random");
    }

    return sim.finish();
}
