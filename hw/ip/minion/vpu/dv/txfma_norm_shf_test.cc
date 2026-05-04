// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_norm_shf_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vtxfma_norm_shf_tb;

namespace {

constexpr uint64_t kInputMask = (1ull << 49) - 1ull;
constexpr uint64_t kOutputMask = (1ull << 33) - 1ull;
constexpr uint32_t kShiftMask = (1u << 10) - 1u;

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

uint64_t model_norm(uint64_t op, uint32_t shfamnt) {
    const bool is_negative = ((shfamnt >> 9) & 1u) != 0u;
    uint32_t shfamnt_abs = shfamnt & kShiftMask;
    if (is_negative) {
        shfamnt_abs = (~shfamnt_abs + 1u) & kShiftMask;
    }

    uint64_t op_norm_ext = 0;
    if (is_negative) {
        op_norm_ext = (shfamnt_abs >= 49u) ? 0ull : ((op & kInputMask) >> shfamnt_abs);
    } else {
        op_norm_ext = (shfamnt_abs >= 49u) ? 0ull : (((op & kInputMask) << shfamnt_abs) & kInputMask);
    }
    return (op_norm_ext >> 14) & kOutputMask;
}

void check_case(SimCtrl<DUT>& sim, uint64_t op, uint32_t shfamnt, const char* name) {
    sim.dut->op_i = op & kInputMask;
    sim.dut->shfamnt_i = shfamnt & kShiftMask;
    sim.dut->eval();
    sim.check(sim.dut->op_norm_o == model_norm(op, shfamnt), name);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    check_case(sim, 0x0000000000000ull, 0x000u, "all_zero");
    check_case(sim, 0x1ffffffffffffull, 0x000u, "no_shift");
    check_case(sim, 0x123456789abcdull, 0x001u, "left_shift_one");
    check_case(sim, 0x123456789abcdull, 0x3ffu, "right_shift_one");
    check_case(sim, 0x123456789abcdull, 0x010u, "left_shift_sixteen");
    check_case(sim, 0x123456789abcdull, 0x3f0u, "right_shift_sixteen");
    check_case(sim, 0x123456789abcdull, 0x040u, "left_shift_out_all_bits");
    check_case(sim, 0x123456789abcdull, 0x3c0u, "right_shift_out_all_bits");

    uint32_t seed = 0x2468ace1u;
    for (int i = 0; i < 4096; ++i) {
        uint64_t op = (static_cast<uint64_t>(xorshift32(seed)) << 17) | (xorshift32(seed) & 0x1ffffu);
        check_case(sim, op, xorshift32(seed) & kShiftMask, "random");
    }

    return sim.finish();
}
