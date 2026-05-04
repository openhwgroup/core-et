// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_booth_ppg_32r4_norm_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vcosim_txfma_booth_ppg_32r4_norm_tb;

namespace {

constexpr uint64_t kMask33 = (1ull << 33) - 1ull;
constexpr int kRandomIters = 25000;

struct BoolCoverage {
    bool seen0 = false;
    bool seen1 = false;
    void observe(bool value) { value ? seen1 = true : seen0 = true; }
    bool full() const { return seen0 && seen1; }
};

struct WordCoverage {
    uint32_t or_mask = 0;
    uint32_t and_mask = 0xffffffffu;
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

void drive_and_compare(CosimCtrl<DUT>& sim, WordCoverage& mulcand_cov, uint8_t& r4input_seen,
                       BoolCoverage& sign_cov, uint32_t mulcand, uint32_t r4input,
                       bool sign, const char* name) {
    sim.dut->mulcand_i = mulcand;
    sim.dut->r4input_i = r4input;
    sim.dut->sign_i = sign;
    mulcand_cov.observe(mulcand);
    r4input_seen |= static_cast<uint8_t>(1u << (r4input & 0x7u));
    sign_cov.observe(sign);
    sim.dut->eval();
    sim.compare("pp", static_cast<uint64_t>(sim.dut->orig_pp_o), static_cast<uint64_t>(sim.dut->new_pp_o));
    sim.check(static_cast<uint64_t>(sim.dut->new_pp_o) == model_pp(mulcand, r4input, sign), name);
    sim.sim_time++;
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    WordCoverage mulcand_cov;
    uint8_t r4input_seen = 0;
    BoolCoverage sign_cov;

    const uint32_t patterns[] = {
        0x00000000u, 0x00000001u, 0x80000000u, 0xffffffffu, 0x12345678u, 0x87654321u,
    };
    for (uint32_t pattern : patterns) {
        for (uint32_t r4input = 0; r4input < 8; ++r4input) {
            drive_and_compare(sim, mulcand_cov, r4input_seen, sign_cov, pattern, r4input, false, "direct_unsigned");
            drive_and_compare(sim, mulcand_cov, r4input_seen, sign_cov, pattern, r4input, true, "direct_signed");
        }
    }
    for (int bit = 0; bit < 32; ++bit) {
        const uint32_t one_hot = 1u << bit;
        for (uint32_t r4input = 0; r4input < 8; ++r4input) {
            drive_and_compare(sim, mulcand_cov, r4input_seen, sign_cov, one_hot, r4input, false, "walk_unsigned");
            drive_and_compare(sim, mulcand_cov, r4input_seen, sign_cov, one_hot, r4input, true, "walk_signed");
        }
    }

    uint32_t seed = 0x2f43b51eu;
    for (int i = 0; i < kRandomIters; ++i) {
        drive_and_compare(sim, mulcand_cov, r4input_seen, sign_cov,
                          xorshift32(seed), xorshift32(seed) & 0x7u, (xorshift32(seed) & 1u) != 0u, "random");
    }

    sim.check(mulcand_cov.or_mask == 0xffffffffu, "mulcand_i should drive every bit high");
    sim.check(mulcand_cov.and_mask == 0x00000000u, "mulcand_i should drive every bit low");
    sim.check(r4input_seen == 0xffu, "r4input_i should cover all 8 Booth encodings");
    sim.check(sign_cov.full(), "sign_i should be driven to both 0 and 1");
    sim.check(sim.comparisons >= 25000, "txfma_booth_ppg_32r4_norm cosim should exceed 25k comparisons");
    return sim.finish();
}
