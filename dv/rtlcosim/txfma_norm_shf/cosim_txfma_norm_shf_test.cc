// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_norm_shf_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vcosim_txfma_norm_shf_tb;

namespace {

constexpr uint64_t kInputMask = (1ull << 49) - 1ull;
constexpr uint64_t kOutputMask = (1ull << 33) - 1ull;
constexpr uint32_t kShiftMask = (1u << 10) - 1u;
constexpr int kRandomIters = 25000;

struct WordCoverage {
    uint64_t or_mask = 0;
    uint64_t and_mask = ~0ull;

    void observe(uint64_t value) {
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

void drive_and_compare(CosimCtrl<DUT>& sim, WordCoverage& op_cov, WordCoverage& shfamnt_cov,
                       uint64_t op, uint32_t shfamnt, const char* name) {
    sim.dut->op_i = op & kInputMask;
    sim.dut->shfamnt_i = shfamnt & kShiftMask;
    op_cov.observe(op & kInputMask);
    shfamnt_cov.observe(shfamnt & kShiftMask);
    sim.dut->eval();
    sim.compare("op_norm", sim.dut->orig_op_norm_o, sim.dut->new_op_norm_o);
    sim.check(sim.dut->new_op_norm_o == model_norm(op, shfamnt), name);
    sim.sim_time++;
}

void check_coverage(CosimCtrl<DUT>& sim, const WordCoverage& coverage, uint64_t mask,
                    const char* name) {
    sim.check(coverage.or_mask == mask, std::string(name) + " should drive every bit high");
    sim.check(coverage.and_mask == 0ull, std::string(name) + " should drive every bit low");
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    WordCoverage op_cov{0, kInputMask};
    WordCoverage shfamnt_cov{0, kShiftMask};

    drive_and_compare(sim, op_cov, shfamnt_cov, 0x0000000000000ull, 0x000u, "all_zero");
    drive_and_compare(sim, op_cov, shfamnt_cov, 0x1ffffffffffffull, 0x000u, "no_shift");
    drive_and_compare(sim, op_cov, shfamnt_cov, 0x123456789abcdull, 0x001u, "left_shift_one");
    drive_and_compare(sim, op_cov, shfamnt_cov, 0x123456789abcdull, 0x3ffu, "right_shift_one");
    drive_and_compare(sim, op_cov, shfamnt_cov, 0x123456789abcdull, 0x010u, "left_shift_sixteen");
    drive_and_compare(sim, op_cov, shfamnt_cov, 0x123456789abcdull, 0x3f0u, "right_shift_sixteen");
    drive_and_compare(sim, op_cov, shfamnt_cov, 0x123456789abcdull, 0x040u, "left_shift_out_all_bits");
    drive_and_compare(sim, op_cov, shfamnt_cov, 0x123456789abcdull, 0x3c0u, "right_shift_out_all_bits");
    for (uint32_t sh = 0; sh < 64u; ++sh) {
        drive_and_compare(sim, op_cov, shfamnt_cov, 0x1ffffffffffffull, sh, "left_shift_sweep");
        drive_and_compare(sim, op_cov, shfamnt_cov, 0x1ffffffffffffull, ((~sh) + 1u) & kShiftMask, "right_shift_sweep");
    }
    for (int bit = 0; bit < 49; ++bit) {
        drive_and_compare(sim, op_cov, shfamnt_cov, 1ull << bit, 0x000u, "walk_input");
        drive_and_compare(sim, op_cov, shfamnt_cov, 1ull << bit, 0x3ffu, "walk_input_neg1");
    }

    uint32_t seed = 0x2468ace1u;
    for (int i = 0; i < kRandomIters; ++i) {
        uint64_t op = (static_cast<uint64_t>(xorshift32(seed)) << 17) | (xorshift32(seed) & 0x1ffffu);
        drive_and_compare(sim, op_cov, shfamnt_cov, op, xorshift32(seed) & kShiftMask, "random");
    }

    check_coverage(sim, op_cov, kInputMask, "op_i");
    check_coverage(sim, shfamnt_cov, kShiftMask, "shfamnt_i");
    sim.check(sim.comparisons >= 25000, "txfma_norm_shf cosim should exceed 25k comparisons");
    return sim.finish();
}
