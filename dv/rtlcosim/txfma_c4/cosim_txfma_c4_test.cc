// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_c4_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using DUT = Vcosim_txfma_c4_tb;

namespace {

constexpr int kNumInputs = 10;
constexpr int kRandomIters = 4096;
constexpr int kTargetComparisons = 25000;
constexpr std::array<uint64_t, kNumInputs> kInputMasks = {0xfull, 0x7full, 0x7ffffull, 0x7full, 0x7ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull};
const char* const kInputNames[kNumInputs] = {"op_dtype_f4a_h", "cmd_f4a_h", "sigs_f4a_h", "elxd_f4a_h", "sin_res_shf_idx_f4a_h", "ph_comb_stiky_f4a_h", "eff_sub1_f4a_h", "eff_sub2_f4a_h", "s_res_f4a_h", "sc_f4a_h"};

struct Inputs {
    uint64_t op_dtype_f4a_h = 0;
    uint64_t cmd_f4a_h = 0;
    uint64_t sigs_f4a_h = 0;
    uint64_t elxd_f4a_h = 0;
    uint64_t sin_res_shf_idx_f4a_h = 0;
    uint64_t ph_comb_stiky_f4a_h = 0;
    uint64_t eff_sub1_f4a_h = 0;
    uint64_t eff_sub2_f4a_h = 0;
    uint64_t s_res_f4a_h = 0;
    uint64_t sc_f4a_h = 0;
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
    return {in.op_dtype_f4a_h, in.cmd_f4a_h, in.sigs_f4a_h, in.elxd_f4a_h, in.sin_res_shf_idx_f4a_h, in.ph_comb_stiky_f4a_h, in.eff_sub1_f4a_h, in.eff_sub2_f4a_h, in.s_res_f4a_h, in.sc_f4a_h};
}

void apply_inputs(DUT* dut, const Inputs& in) {
    dut->op_dtype_f4a_h = in.op_dtype_f4a_h & 0xfull;
    dut->cmd_f4a_h = in.cmd_f4a_h & 0x7full;
    dut->sigs_f4a_h = in.sigs_f4a_h & 0x7ffffull;
    dut->elxd_f4a_h = in.elxd_f4a_h & 0x7full;
    dut->sin_res_shf_idx_f4a_h = in.sin_res_shf_idx_f4a_h & 0x7ull;
    dut->ph_comb_stiky_f4a_h = in.ph_comb_stiky_f4a_h & 0x1ull;
    dut->eff_sub1_f4a_h = in.eff_sub1_f4a_h & 0x1ull;
    dut->eff_sub2_f4a_h = in.eff_sub2_f4a_h & 0x1ull;
    dut->s_res_f4a_h = in.s_res_f4a_h & 0x1ull;
    dut->sc_f4a_h = in.sc_f4a_h & 0x1ull;
}

Inputs random_inputs(uint32_t& state) {
    return Inputs{(random64(state) & 0xfull), (random64(state) & 0x7full), (random64(state) & 0x7ffffull), (random64(state) & 0x7full), (random64(state) & 0x7ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull)};
}

std::vector<Inputs> directed_inputs() {
    return {
        Inputs{0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull},  // zero
        Inputs{0xfull, 0x7full, 0x7ffffull, 0x7full, 0x7ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull},  // ones
        Inputs{0x0ull, 0x5ull, 0x7eecfull, 0x35ull, 0x3ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull},  // pattern_a
        Inputs{0x7ull, 0x28ull, 0x24933ull, 0x7dull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull},  // pattern_b
        Inputs{0x1ull, 0x7ull, 0x8ull, 0xcull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull},  // f32_elxd_effsub
        Inputs{0x0ull, 0x77ull, 0x10000ull, 0x0ull, 0x3ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull},  // sin_shift_idx3
        Inputs{0xeull, 0x7bull, 0x100ull, 0x1full, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull},  // f16f32_clear
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
    sim.compare("neg_norm_a3_f4a_h", sim.dut->orig_neg_norm_a3_f4a_h, sim.dut->new_neg_norm_a3_f4a_h);
    sim.compare("neg_norm_na3_f4a_h", sim.dut->orig_neg_norm_na3_f4a_h, sim.dut->new_neg_norm_na3_f4a_h);
    sim.compare("clearphps_en_f4a_h", sim.dut->orig_clearphps_en_f4a_h, sim.dut->new_clearphps_en_f4a_h);
    sim.compare("clear16frnd_nshin_f4a_l", sim.dut->orig_clear16frnd_nshin_f4a_l, sim.dut->new_clear16frnd_nshin_f4a_l);
    sim.compare("ph_ps_clear_f4a_l", sim.dut->orig_ph_ps_clear_f4a_l, sim.dut->new_ph_ps_clear_f4a_l);
    sim.compare("enc_nsamt_f4a_h", sim.dut->orig_enc_nsamt_f4a_h, sim.dut->new_enc_nsamt_f4a_h);
    sim.compare("elxd_add_in2_mux_sel_f4a_h", sim.dut->orig_elxd_add_in2_mux_sel_f4a_h, sim.dut->new_elxd_add_in2_mux_sel_f4a_h);
    sim.compare("use_elxd_f4a_h", sim.dut->orig_use_elxd_f4a_h, sim.dut->new_use_elxd_f4a_h);
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
