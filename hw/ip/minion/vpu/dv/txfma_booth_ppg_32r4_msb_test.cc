// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_booth_ppg_32r4_msb_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vtxfma_booth_ppg_32r4_msb_tb;

namespace {

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

uint64_t model_pp(uint32_t mulcand, bool msb, bool sign) {
    return (!sign && msb) ? static_cast<uint64_t>(mulcand) : 0ull;
}

void check_case(SimCtrl<DUT>& sim, uint32_t mulcand, bool msb, bool sign, const char* name) {
    sim.dut->mulcand_i = mulcand;
    sim.dut->msb_i = msb;
    sim.dut->sign_i = sign;
    sim.dut->eval();
    sim.check(static_cast<uint64_t>(sim.dut->pp_o) == model_pp(mulcand, msb, sign), name);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    const uint32_t patterns[] = {
        0x00000000u, 0x00000001u, 0x80000000u, 0xffffffffu, 0x13579bdfu, 0x2468ace0u,
    };

    for (uint32_t pattern : patterns) {
        check_case(sim, pattern, false, false, "msb0_sign0");
        check_case(sim, pattern, true, false, "msb1_sign0");
        check_case(sim, pattern, false, true, "msb0_sign1");
        check_case(sim, pattern, true, true, "msb1_sign1");
    }

    uint32_t seed = 0x79d1ab43u;
    for (int i = 0; i < 512; ++i) {
        check_case(sim, xorshift32(seed), (xorshift32(seed) & 1u) != 0u,
                   (xorshift32(seed) & 1u) != 0u, "random");
    }

    return sim.finish();
}
