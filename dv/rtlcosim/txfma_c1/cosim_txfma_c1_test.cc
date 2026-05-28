// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_c1_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using DUT = Vcosim_txfma_c1_tb;

namespace {

constexpr int kNumInputs = 16;
constexpr int kRandomIters = 4096;
constexpr int kTargetComparisons = 25000;
constexpr std::array<uint64_t, kNumInputs> kInputMasks = {0x1ull, 0x7ffffull, 0x7full, 0xfull, 0xffull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull};
const char* const kInputNames[kNumInputs] = {"ctrl_valid_f1a_h", "sigs_f1a_h", "cmd_in_f1a_h", "op_dtype_f1a_h", "ea_hi_f1a_h", "fa_hi_zero_f1a_h", "sa_hi_f1a_h", "sb_hi_f1a_h", "sc_f1a_h", "sa_lo_f1a_h", "sb_lo_f1a_h", "ea_hi_zero_f1a_h", "eb_hi_zero_f1a_h", "ea_lo_zero_f1a_h", "eb_lo_zero_f1a_h", "flog_mul_prod_zero_f1a_h"};

struct Inputs {
    uint64_t ctrl_valid_f1a_h = 0;
    uint64_t sigs_f1a_h = 0;
    uint64_t cmd_in_f1a_h = 0;
    uint64_t op_dtype_f1a_h = 0;
    uint64_t ea_hi_f1a_h = 0;
    uint64_t fa_hi_zero_f1a_h = 0;
    uint64_t sa_hi_f1a_h = 0;
    uint64_t sb_hi_f1a_h = 0;
    uint64_t sc_f1a_h = 0;
    uint64_t sa_lo_f1a_h = 0;
    uint64_t sb_lo_f1a_h = 0;
    uint64_t ea_hi_zero_f1a_h = 0;
    uint64_t eb_hi_zero_f1a_h = 0;
    uint64_t ea_lo_zero_f1a_h = 0;
    uint64_t eb_lo_zero_f1a_h = 0;
    uint64_t flog_mul_prod_zero_f1a_h = 0;
};

struct InputCoverage {
    std::array<uint64_t, kNumInputs> ones{};
    std::array<uint64_t, kNumInputs> zeros{};
};

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

uint64_t random64(uint32_t& state) {
    const uint64_t lo = xorshift32(state);
    const uint64_t hi = xorshift32(state);
    return (hi << 32) | lo;
}

std::array<uint64_t, kNumInputs> input_values(const Inputs& in) {
    return {in.ctrl_valid_f1a_h, in.sigs_f1a_h, in.cmd_in_f1a_h, in.op_dtype_f1a_h, in.ea_hi_f1a_h, in.fa_hi_zero_f1a_h, in.sa_hi_f1a_h, in.sb_hi_f1a_h, in.sc_f1a_h, in.sa_lo_f1a_h, in.sb_lo_f1a_h, in.ea_hi_zero_f1a_h, in.eb_hi_zero_f1a_h, in.ea_lo_zero_f1a_h, in.eb_lo_zero_f1a_h, in.flog_mul_prod_zero_f1a_h};
}

void apply_inputs(DUT* dut, const Inputs& in) {
    dut->ctrl_valid_f1a_h = in.ctrl_valid_f1a_h & 0x1ull;
    dut->sigs_f1a_h = in.sigs_f1a_h & 0x7ffffull;
    dut->cmd_in_f1a_h = in.cmd_in_f1a_h & 0x7full;
    dut->op_dtype_f1a_h = in.op_dtype_f1a_h & 0xfull;
    dut->ea_hi_f1a_h = in.ea_hi_f1a_h & 0xffull;
    dut->fa_hi_zero_f1a_h = in.fa_hi_zero_f1a_h & 0x1ull;
    dut->sa_hi_f1a_h = in.sa_hi_f1a_h & 0x1ull;
    dut->sb_hi_f1a_h = in.sb_hi_f1a_h & 0x1ull;
    dut->sc_f1a_h = in.sc_f1a_h & 0x1ull;
    dut->sa_lo_f1a_h = in.sa_lo_f1a_h & 0x1ull;
    dut->sb_lo_f1a_h = in.sb_lo_f1a_h & 0x1ull;
    dut->ea_hi_zero_f1a_h = in.ea_hi_zero_f1a_h & 0x1ull;
    dut->eb_hi_zero_f1a_h = in.eb_hi_zero_f1a_h & 0x1ull;
    dut->ea_lo_zero_f1a_h = in.ea_lo_zero_f1a_h & 0x1ull;
    dut->eb_lo_zero_f1a_h = in.eb_lo_zero_f1a_h & 0x1ull;
    dut->flog_mul_prod_zero_f1a_h = in.flog_mul_prod_zero_f1a_h & 0x1ull;
}

Inputs random_inputs(uint32_t& state) {
    return Inputs{(random64(state) & 0x1ull), (random64(state) & 0x7ffffull), (random64(state) & 0x7full), (random64(state) & 0xfull), (random64(state) & 0xffull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull)};
}

std::vector<Inputs> directed_inputs() {
    return {
        Inputs{0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull},  // zero
        Inputs{0x1ull, 0x7ffffull, 0x7full, 0xfull, 0xffull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull},  // ones
        Inputs{0x1ull, 0x6e1c4ull, 0x2aull, 0x6ull, 0x1eull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull},  // pattern_a
        Inputs{0x0ull, 0x1be69ull, 0x56ull, 0xeull, 0x9dull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull},  // pattern_b
        Inputs{0x1ull, 0xaull, 0x7ull, 0x1ull, 0x7full, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull},  // fma_sign_norm
        Inputs{0x1ull, 0x88ull, 0x74ull, 0xeull, 0x7full, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull},  // log_mul_zero
        Inputs{0x1ull, 0x4800ull, 0x21ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull},  // compare_sgcvt
    };
}

void observe(InputCoverage& cov, const Inputs& in) {
    const auto vals = input_values(in);
    for (int i = 0; i < kNumInputs; ++i) {
        const uint64_t masked = vals[i] & kInputMasks[i];
        cov.ones[i] |= masked;
        cov.zeros[i] |= (~masked) & kInputMasks[i];
    }
}

void compare_all(CosimCtrl<DUT>& sim) {
    sim.compare("minmax_op_f1a_h", sim.dut->orig_minmax_op_f1a_h, sim.dut->new_minmax_op_f1a_h);
    sim.compare("prod_sign_hi_f1a_h", sim.dut->orig_prod_sign_hi_f1a_h, sim.dut->new_prod_sign_hi_f1a_h);
    sim.compare("prod_sign_lo_f1a_h", sim.dut->orig_prod_sign_lo_f1a_h, sim.dut->new_prod_sign_lo_f1a_h);
    sim.compare("eprod_hi_zero_f1a_h", sim.dut->orig_eprod_hi_zero_f1a_h, sim.dut->new_eprod_hi_zero_f1a_h);
    sim.compare("eprod_lo_zero_f1a_h", sim.dut->orig_eprod_lo_zero_f1a_h, sim.dut->new_eprod_lo_zero_f1a_h);
    sim.compare("norm_f32_ovf_f1a_h", sim.dut->orig_norm_f32_ovf_f1a_h, sim.dut->new_norm_f32_ovf_f1a_h);
    sim.compare("norm_one_out_f1a_h", sim.dut->orig_norm_one_out_f1a_h, sim.dut->new_norm_one_out_f1a_h);
    sim.compare("paddin1_sels_f1a_h", sim.dut->orig_paddin1_sels_f1a_h, sim.dut->new_paddin1_sels_f1a_h);
    sim.compare("paddin2_selc_f1a_h", sim.dut->orig_paddin2_selc_f1a_h, sim.dut->new_paddin2_selc_f1a_h);
    sim.compare("paddin2_selfcneg_f1a_h", sim.dut->orig_paddin2_selfcneg_f1a_h, sim.dut->new_paddin2_selfcneg_f1a_h);
    sim.compare("padd_cin_f1a_h", sim.dut->orig_padd_cin_f1a_h, sim.dut->new_padd_cin_f1a_h);
    sim.compare("sel_famask_f1a_h", sim.dut->orig_sel_famask_f1a_h, sim.dut->new_sel_famask_f1a_h);
    sim.compare("sel_fcmask_f1a_h", sim.dut->orig_sel_fcmask_f1a_h, sim.dut->new_sel_fcmask_f1a_h);
    sim.compare("fa_en_f1a_h", sim.dut->orig_fa_en_f1a_h, sim.dut->new_fa_en_f1a_h);
    sim.compare("cmd_f1a_h", sim.dut->orig_cmd_f1a_h, sim.dut->new_cmd_f1a_h);
    sim.compare("use_mul_f1a_h", sim.dut->orig_use_mul_f1a_h, sim.dut->new_use_mul_f1a_h);
}

void drive_and_compare(CosimCtrl<DUT>& sim, InputCoverage& cov, const Inputs& in) {
    apply_inputs(sim.dut.get(), in);
    observe(cov, in);
    sim.dut->eval();
    compare_all(sim);
    sim.sim_time++;
}

void check_coverage(CosimCtrl<DUT>& sim, const InputCoverage& cov) {
    for (int i = 0; i < kNumInputs; ++i) {
        sim.check(cov.ones[i] == kInputMasks[i], std::string(kInputNames[i]) + " drove all bits high");
        sim.check(cov.zeros[i] == kInputMasks[i], std::string(kInputNames[i]) + " drove all bits low");
    }
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    InputCoverage cov;
    auto directed = directed_inputs();
    for (const auto& in : directed) {
        drive_and_compare(sim, cov, in);
    }
    uint32_t state = 0x2468ace0u;
    for (int i = 0; i < kRandomIters; ++i) {
        drive_and_compare(sim, cov, random_inputs(state));
    }
    check_coverage(sim, cov);
    sim.check(sim.comparisons >= kTargetComparisons, "cosim comparison target met");
    return sim.finish();
}
