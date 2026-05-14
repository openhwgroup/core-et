// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_frac_zero_detect_tb.h"
#include "sim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vtxfma_frac_zero_detect_tb;

namespace {

constexpr uint64_t kMask33 = (1ull << 33) - 1ull;

uint64_t xorshift64(uint64_t& state) {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state;
}

void check_case(SimCtrl<DUT>& sim, uint64_t value, const char* name) {
    sim.dut->in_data = value & kMask33;
    sim.dut->eval();
    sim.check(sim.dut->zero == ((value & kMask33) == 0), name);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    check_case(sim, 0, "all_zero_detects_zero");
    check_case(sim, 1ull, "lsb_nonzero");
    check_case(sim, 1ull << 16, "middle_bit_nonzero");
    check_case(sim, 1ull << 32, "msb_nonzero");
    check_case(sim, kMask33, "all_ones_nonzero");

    uint64_t seed = 0x6eed1234cafef00dull;
    for (int i = 0; i < 4096; ++i) {
        check_case(sim, xorshift64(seed), "random_zero_detect");
    }

    return sim.finish();
}
