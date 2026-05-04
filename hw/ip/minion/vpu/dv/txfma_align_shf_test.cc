// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_align_shf_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vtxfma_align_shf_tb;

namespace {

constexpr uint64_t kWidthMask = (1ull << 48) - 1ull;

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

uint64_t model_align(uint64_t op, uint32_t shfamnt) {
    if (shfamnt >= 48u) {
        return 0;
    }
    return (op & kWidthMask) >> shfamnt;
}

void check_case(SimCtrl<DUT>& sim, uint64_t op, uint32_t shfamnt, const char* name) {
    sim.dut->op_i = op & kWidthMask;
    sim.dut->shfamnt_i = shfamnt;
    sim.dut->eval();
    sim.check(sim.dut->op_align_o == model_align(op, shfamnt), name);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    check_case(sim, 0x000000000000ull, 0u, "all_zero");
    check_case(sim, 0xffffffffffffull, 0u, "no_shift");
    check_case(sim, 0xffffffffffffull, 1u, "shift_one");
    check_case(sim, 0x123456789abcll, 47u, "shift_last_bit");
    check_case(sim, 0x123456789abcll, 48u, "shift_out_all_bits");
    check_case(sim, 0x123456789abcll, 255u, "large_shift");

    uint32_t seed = 0x13579bdfu;
    for (int i = 0; i < 4096; ++i) {
        uint64_t op = (static_cast<uint64_t>(xorshift32(seed)) << 16) | (xorshift32(seed) & 0xffffu);
        check_case(sim, op, xorshift32(seed) & 0xffu, "random");
    }

    return sim.finish();
}
