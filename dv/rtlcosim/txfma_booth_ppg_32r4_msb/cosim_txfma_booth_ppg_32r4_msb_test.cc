// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_booth_ppg_32r4_msb_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_txfma_booth_ppg_32r4_msb_tb;

namespace {

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

uint64_t model_pp(uint32_t mulcand, bool msb, bool sign) {
    return (!sign && msb) ? static_cast<uint64_t>(mulcand) : 0ull;
}

void drive_and_compare(CosimCtrl<DUT>& sim, WordCoverage& mulcand_cov, BoolCoverage& msb_cov,
                       BoolCoverage& sign_cov, uint32_t mulcand, bool msb, bool sign,
                       const char* name) {
    sim.dut->mulcand_i = mulcand;
    sim.dut->msb_i = msb;
    sim.dut->sign_i = sign;
    mulcand_cov.observe(mulcand);
    msb_cov.observe(msb);
    sign_cov.observe(sign);
    sim.dut->eval();
    sim.compare("pp", static_cast<uint64_t>(sim.dut->orig_pp_o), static_cast<uint64_t>(sim.dut->new_pp_o));
    sim.check(static_cast<uint64_t>(sim.dut->new_pp_o) == model_pp(mulcand, msb, sign), name);
    sim.sim_time++;
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    WordCoverage mulcand_cov;
    BoolCoverage msb_cov;
    BoolCoverage sign_cov;

    const uint32_t patterns[] = {
        0x00000000u, 0x00000001u, 0x80000000u, 0xffffffffu, 0x13579bdfu, 0x2468ace0u,
    };
    for (uint32_t pattern : patterns) {
        drive_and_compare(sim, mulcand_cov, msb_cov, sign_cov, pattern, false, false, "msb0_sign0");
        drive_and_compare(sim, mulcand_cov, msb_cov, sign_cov, pattern, true, false, "msb1_sign0");
        drive_and_compare(sim, mulcand_cov, msb_cov, sign_cov, pattern, false, true, "msb0_sign1");
        drive_and_compare(sim, mulcand_cov, msb_cov, sign_cov, pattern, true, true, "msb1_sign1");
    }
    for (int bit = 0; bit < 32; ++bit) {
        const uint32_t one_hot = 1u << bit;
        drive_and_compare(sim, mulcand_cov, msb_cov, sign_cov, one_hot, false, false, "walk_msb0_sign0");
        drive_and_compare(sim, mulcand_cov, msb_cov, sign_cov, one_hot, true, false, "walk_msb1_sign0");
        drive_and_compare(sim, mulcand_cov, msb_cov, sign_cov, one_hot, false, true, "walk_msb0_sign1");
        drive_and_compare(sim, mulcand_cov, msb_cov, sign_cov, one_hot, true, true, "walk_msb1_sign1");
    }

    uint32_t seed = 0x79d1ab43u;
    for (int i = 0; i < kRandomIters; ++i) {
        drive_and_compare(sim, mulcand_cov, msb_cov, sign_cov,
                          xorshift32(seed), (xorshift32(seed) & 1u) != 0u,
                          (xorshift32(seed) & 1u) != 0u, "random");
    }

    sim.check(mulcand_cov.or_mask == 0xffffffffu, "mulcand_i should drive every bit high");
    sim.check(mulcand_cov.and_mask == 0x00000000u, "mulcand_i should drive every bit low");
    sim.check(msb_cov.full(), "msb_i should be driven to both 0 and 1");
    sim.check(sign_cov.full(), "sign_i should be driven to both 0 and 1");
    sim.check(sim.comparisons >= 25000, "txfma_booth_ppg_32r4_msb cosim should exceed 25k comparisons");
    return sim.finish();
}
