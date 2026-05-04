// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_align_shf_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <string>

using DUT = Vcosim_txfma_align_shf_tb;

namespace {

constexpr uint64_t kWidthMask = (1ull << 48) - 1ull;
constexpr int kRandomIters = 25000;

struct WordCoverage {
    uint64_t or_mask = 0;
    uint64_t and_mask = kWidthMask;

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

uint64_t model_align(uint64_t op, uint32_t shfamnt) {
    if (shfamnt >= 48u) {
        return 0;
    }
    return (op & kWidthMask) >> shfamnt;
}

void drive_and_compare(CosimCtrl<DUT>& sim, WordCoverage& op_cov, WordCoverage& shfamnt_cov,
                       uint64_t op, uint32_t shfamnt, const char* name) {
    sim.dut->op_i = op & kWidthMask;
    sim.dut->shfamnt_i = shfamnt;
    op_cov.observe(op & kWidthMask);
    shfamnt_cov.observe(shfamnt);
    sim.dut->eval();
    sim.compare("op_align", sim.dut->orig_op_align_o, sim.dut->new_op_align_o);
    sim.check(sim.dut->new_op_align_o == model_align(op, shfamnt), name);
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
    WordCoverage op_cov;
    WordCoverage shfamnt_cov;

    drive_and_compare(sim, op_cov, shfamnt_cov, 0x000000000000ull, 0u, "all_zero");
    drive_and_compare(sim, op_cov, shfamnt_cov, 0xffffffffffffull, 0u, "no_shift");
    drive_and_compare(sim, op_cov, shfamnt_cov, 0xffffffffffffull, 1u, "shift_one");
    drive_and_compare(sim, op_cov, shfamnt_cov, 0x123456789abcull, 47u, "shift_last_bit");
    drive_and_compare(sim, op_cov, shfamnt_cov, 0x123456789abcull, 48u, "shift_out_all_bits");
    drive_and_compare(sim, op_cov, shfamnt_cov, 0x123456789abcull, 255u, "large_shift");
    for (uint32_t sh = 0; sh < 256u; ++sh) {
        drive_and_compare(sim, op_cov, shfamnt_cov, 0xffffffffffffull, sh, "sweep_ones");
        drive_and_compare(sim, op_cov, shfamnt_cov, 0xaaaaaaaaaaaauLL, sh, "sweep_alt");
    }
    for (int bit = 0; bit < 48; ++bit) {
        drive_and_compare(sim, op_cov, shfamnt_cov, 1ull << bit, 0u, "walk_input");
    }

    uint32_t seed = 0x13579bdfu;
    for (int i = 0; i < kRandomIters; ++i) {
        uint64_t op = (static_cast<uint64_t>(xorshift32(seed)) << 16) | (xorshift32(seed) & 0xffffu);
        drive_and_compare(sim, op_cov, shfamnt_cov, op, xorshift32(seed) & 0xffu, "random");
    }

    check_coverage(sim, op_cov, kWidthMask, "op_i");
    check_coverage(sim, shfamnt_cov, 0xffu, "shfamnt_i");
    sim.check(sim.comparisons >= 25000, "txfma_align_shf cosim should exceed 25k comparisons");
    return sim.finish();
}
