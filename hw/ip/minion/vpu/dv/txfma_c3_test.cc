// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_c3_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

using DUT = Vtxfma_c3_tb;

namespace {

constexpr int kNumInputs = 44;
constexpr int kNumChunks = 1;
constexpr std::array<uint64_t, kNumInputs> kInputMasks = {0xfull, 0x7full, 0x7ffffull, 0x3ffull, 0x3ffull, 0xffull, 0x7ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1full};
const char* const kInputNames[kNumInputs] = {"op_dtype_f3a_h", "cmd_f3a_h", "sigs_f3a_h", "ediff1_f3a_h", "ediff2_f3a_h", "ec_f3a_h", "rm_f3a_h", "eff_sub1_f3a_h", "eff_sub2_f3a_h", "eprod_hi_zero_f3a_h", "eprod_lo_zero_f3a_h", "prod_inf_hi_f3a_h", "prod_inf_lo_f3a_h", "eff_sign1_f3a_h", "prod_sign_hi_f3a_h", "prod_sign_lo_f3a_h", "sc_f3a_h", "c_inf_f3a_h", "ec_zero_f3a_h", "a_is_qnan_hi_f3a_h", "c_is_qnan_f3a_h", "a_is_snan_hi_f3a_h", "c_is_snan_f3a_h", "e_sticky1_f3a_h", "fround_rbit_f3a_h", "prodsum_a2_f3a_h", "ediff1_0_f3a_h", "fa_ge_fc_comp_sign_hi_res_f3a_h", "fa_eq_fc_comp_sign_hi_res_f3a_h", "int_minmax_mux_sel_fc_f3a_h", "minmax_op_f3a_h", "comp_op_f3a_h", "res_inf_f3a_h", "res_qnan_f3a_h", "res_snan_f3a_h", "spec_res_en_f3a_h", "flags_en_f3a_h", "trans_log_rr_neg_f3a_h", "sin_align1_neg_f3a_h", "sin_align2_neg_f3a_h", "sin_eff_sub1_f3a_h", "sin_check_fc_sign_f3a_h", "fc_sel_f3a_h", "opd3_mux_sel_f3a_h"};

struct Inputs {
    uint64_t op_dtype_f3a_h = 0;
    uint64_t cmd_f3a_h = 0;
    uint64_t sigs_f3a_h = 0;
    uint64_t ediff1_f3a_h = 0;
    uint64_t ediff2_f3a_h = 0;
    uint64_t ec_f3a_h = 0;
    uint64_t rm_f3a_h = 0;
    uint64_t eff_sub1_f3a_h = 0;
    uint64_t eff_sub2_f3a_h = 0;
    uint64_t eprod_hi_zero_f3a_h = 0;
    uint64_t eprod_lo_zero_f3a_h = 0;
    uint64_t prod_inf_hi_f3a_h = 0;
    uint64_t prod_inf_lo_f3a_h = 0;
    uint64_t eff_sign1_f3a_h = 0;
    uint64_t prod_sign_hi_f3a_h = 0;
    uint64_t prod_sign_lo_f3a_h = 0;
    uint64_t sc_f3a_h = 0;
    uint64_t c_inf_f3a_h = 0;
    uint64_t ec_zero_f3a_h = 0;
    uint64_t a_is_qnan_hi_f3a_h = 0;
    uint64_t c_is_qnan_f3a_h = 0;
    uint64_t a_is_snan_hi_f3a_h = 0;
    uint64_t c_is_snan_f3a_h = 0;
    uint64_t e_sticky1_f3a_h = 0;
    uint64_t fround_rbit_f3a_h = 0;
    uint64_t prodsum_a2_f3a_h = 0;
    uint64_t ediff1_0_f3a_h = 0;
    uint64_t fa_ge_fc_comp_sign_hi_res_f3a_h = 0;
    uint64_t fa_eq_fc_comp_sign_hi_res_f3a_h = 0;
    uint64_t int_minmax_mux_sel_fc_f3a_h = 0;
    uint64_t minmax_op_f3a_h = 0;
    uint64_t comp_op_f3a_h = 0;
    uint64_t res_inf_f3a_h = 0;
    uint64_t res_qnan_f3a_h = 0;
    uint64_t res_snan_f3a_h = 0;
    uint64_t spec_res_en_f3a_h = 0;
    uint64_t flags_en_f3a_h = 0;
    uint64_t trans_log_rr_neg_f3a_h = 0;
    uint64_t sin_align1_neg_f3a_h = 0;
    uint64_t sin_align2_neg_f3a_h = 0;
    uint64_t sin_eff_sub1_f3a_h = 0;
    uint64_t sin_check_fc_sign_f3a_h = 0;
    uint64_t fc_sel_f3a_h = 0;
    uint64_t opd3_mux_sel_f3a_h = 0;
};

struct TestCase {
    const char* name;
    Inputs in;
    std::array<uint64_t, kNumChunks> expected;
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
    return {in.op_dtype_f3a_h, in.cmd_f3a_h, in.sigs_f3a_h, in.ediff1_f3a_h, in.ediff2_f3a_h, in.ec_f3a_h, in.rm_f3a_h, in.eff_sub1_f3a_h, in.eff_sub2_f3a_h, in.eprod_hi_zero_f3a_h, in.eprod_lo_zero_f3a_h, in.prod_inf_hi_f3a_h, in.prod_inf_lo_f3a_h, in.eff_sign1_f3a_h, in.prod_sign_hi_f3a_h, in.prod_sign_lo_f3a_h, in.sc_f3a_h, in.c_inf_f3a_h, in.ec_zero_f3a_h, in.a_is_qnan_hi_f3a_h, in.c_is_qnan_f3a_h, in.a_is_snan_hi_f3a_h, in.c_is_snan_f3a_h, in.e_sticky1_f3a_h, in.fround_rbit_f3a_h, in.prodsum_a2_f3a_h, in.ediff1_0_f3a_h, in.fa_ge_fc_comp_sign_hi_res_f3a_h, in.fa_eq_fc_comp_sign_hi_res_f3a_h, in.int_minmax_mux_sel_fc_f3a_h, in.minmax_op_f3a_h, in.comp_op_f3a_h, in.res_inf_f3a_h, in.res_qnan_f3a_h, in.res_snan_f3a_h, in.spec_res_en_f3a_h, in.flags_en_f3a_h, in.trans_log_rr_neg_f3a_h, in.sin_align1_neg_f3a_h, in.sin_align2_neg_f3a_h, in.sin_eff_sub1_f3a_h, in.sin_check_fc_sign_f3a_h, in.fc_sel_f3a_h, in.opd3_mux_sel_f3a_h};
}

void apply_inputs(DUT* dut, const Inputs& in) {
    dut->op_dtype_f3a_h = in.op_dtype_f3a_h & 0xfull;
    dut->cmd_f3a_h = in.cmd_f3a_h & 0x7full;
    dut->sigs_f3a_h = in.sigs_f3a_h & 0x7ffffull;
    dut->ediff1_f3a_h = in.ediff1_f3a_h & 0x3ffull;
    dut->ediff2_f3a_h = in.ediff2_f3a_h & 0x3ffull;
    dut->ec_f3a_h = in.ec_f3a_h & 0xffull;
    dut->rm_f3a_h = in.rm_f3a_h & 0x7ull;
    dut->eff_sub1_f3a_h = in.eff_sub1_f3a_h & 0x1ull;
    dut->eff_sub2_f3a_h = in.eff_sub2_f3a_h & 0x1ull;
    dut->eprod_hi_zero_f3a_h = in.eprod_hi_zero_f3a_h & 0x1ull;
    dut->eprod_lo_zero_f3a_h = in.eprod_lo_zero_f3a_h & 0x1ull;
    dut->prod_inf_hi_f3a_h = in.prod_inf_hi_f3a_h & 0x1ull;
    dut->prod_inf_lo_f3a_h = in.prod_inf_lo_f3a_h & 0x1ull;
    dut->eff_sign1_f3a_h = in.eff_sign1_f3a_h & 0x1ull;
    dut->prod_sign_hi_f3a_h = in.prod_sign_hi_f3a_h & 0x1ull;
    dut->prod_sign_lo_f3a_h = in.prod_sign_lo_f3a_h & 0x1ull;
    dut->sc_f3a_h = in.sc_f3a_h & 0x1ull;
    dut->c_inf_f3a_h = in.c_inf_f3a_h & 0x1ull;
    dut->ec_zero_f3a_h = in.ec_zero_f3a_h & 0x1ull;
    dut->a_is_qnan_hi_f3a_h = in.a_is_qnan_hi_f3a_h & 0x1ull;
    dut->c_is_qnan_f3a_h = in.c_is_qnan_f3a_h & 0x1ull;
    dut->a_is_snan_hi_f3a_h = in.a_is_snan_hi_f3a_h & 0x1ull;
    dut->c_is_snan_f3a_h = in.c_is_snan_f3a_h & 0x1ull;
    dut->e_sticky1_f3a_h = in.e_sticky1_f3a_h & 0x1ull;
    dut->fround_rbit_f3a_h = in.fround_rbit_f3a_h & 0x1ull;
    dut->prodsum_a2_f3a_h = in.prodsum_a2_f3a_h & 0x1ull;
    dut->ediff1_0_f3a_h = in.ediff1_0_f3a_h & 0x1ull;
    dut->fa_ge_fc_comp_sign_hi_res_f3a_h = in.fa_ge_fc_comp_sign_hi_res_f3a_h & 0x1ull;
    dut->fa_eq_fc_comp_sign_hi_res_f3a_h = in.fa_eq_fc_comp_sign_hi_res_f3a_h & 0x1ull;
    dut->int_minmax_mux_sel_fc_f3a_h = in.int_minmax_mux_sel_fc_f3a_h & 0x1ull;
    dut->minmax_op_f3a_h = in.minmax_op_f3a_h & 0x1ull;
    dut->comp_op_f3a_h = in.comp_op_f3a_h & 0x1ull;
    dut->res_inf_f3a_h = in.res_inf_f3a_h & 0x1ull;
    dut->res_qnan_f3a_h = in.res_qnan_f3a_h & 0x1ull;
    dut->res_snan_f3a_h = in.res_snan_f3a_h & 0x1ull;
    dut->spec_res_en_f3a_h = in.spec_res_en_f3a_h & 0x1ull;
    dut->flags_en_f3a_h = in.flags_en_f3a_h & 0x1ull;
    dut->trans_log_rr_neg_f3a_h = in.trans_log_rr_neg_f3a_h & 0x1ull;
    dut->sin_align1_neg_f3a_h = in.sin_align1_neg_f3a_h & 0x1ull;
    dut->sin_align2_neg_f3a_h = in.sin_align2_neg_f3a_h & 0x1ull;
    dut->sin_eff_sub1_f3a_h = in.sin_eff_sub1_f3a_h & 0x1ull;
    dut->sin_check_fc_sign_f3a_h = in.sin_check_fc_sign_f3a_h & 0x1ull;
    dut->fc_sel_f3a_h = in.fc_sel_f3a_h & 0x1ull;
    dut->opd3_mux_sel_f3a_h = in.opd3_mux_sel_f3a_h & 0x1full;
}

Inputs random_inputs(uint32_t& state) {
    return Inputs{(random64(state) & 0xfull), (random64(state) & 0x7full), (random64(state) & 0x7ffffull), (random64(state) & 0x3ffull), (random64(state) & 0x3ffull), (random64(state) & 0xffull), (random64(state) & 0x7ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1full)};
}

std::vector<TestCase> directed_cases() {
    return {
        {"zero", Inputs{0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x000000000000c100ull}},
        {"ones", Inputs{0xfull, 0x7full, 0x7ffffull, 0x3ffull, 0x3ffull, 0xffull, 0x7ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1full}, std::array<uint64_t, kNumChunks>{0x00000000000050b3ull}},
        {"pattern_a", Inputs{0x7ull, 0x46ull, 0x68130ull, 0x28full, 0x234ull, 0xa6ull, 0x6ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x12ull}, std::array<uint64_t, kNumChunks>{0x0000000000004033ull}},
        {"pattern_b", Inputs{0x0ull, 0x6bull, 0x326ccull, 0x1c7ull, 0x2b7ull, 0xb8ull, 0x7ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x9ull}, std::array<uint64_t, kNumChunks>{0x0000000000002814ull}},
        {"max_equal_zero", Inputs{0x1ull, 0x10ull, 0x20ull, 0x0ull, 0x0ull, 0x7full, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x00000000000041a2ull}},
        {"flt_snan_invalid", Inputs{0x1ull, 0x21ull, 0x20ull, 0x1ull, 0x0ull, 0x80ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x000000000000c240ull}},
        {"exp_frac_bound", Inputs{0x1ull, 0x7aull, 0x10000ull, 0x3feull, 0x0ull, 0x7full, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x000000000000cc10ull}},
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

std::array<uint64_t, kNumChunks> capture_outputs(DUT* dut) {
    return {dut->out_chunk0_o};
}

void check_outputs(SimCtrl<DUT>& sim, const TestCase& tc) {
    const auto got = capture_outputs(sim.dut.get());
    for (int i = 0; i < kNumChunks; ++i) {
        sim.check(got[i] == tc.expected[i], std::string(tc.name) + " output chunk " + std::to_string(i));
    }
}

void check_coverage(SimCtrl<DUT>& sim, const InputCoverage& cov) {
    for (int i = 0; i < kNumInputs; ++i) {
        sim.check(cov.ones[i] == kInputMasks[i], std::string(kInputNames[i]) + " drove all bits high");
        sim.check(cov.zeros[i] == kInputMasks[i], std::string(kInputNames[i]) + " drove all bits low");
    }
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    InputCoverage cov;
    auto cases = directed_cases();
    for (const auto& tc : cases) {
        apply_inputs(sim.dut.get(), tc.in);
        observe(cov, tc.in);
        sim.dut->eval();
        sim.sim_time++;
        check_outputs(sim, tc);
    }

    uint32_t state = 0x13579bdfu;
    for (int i = 0; i < 256; ++i) {
        const Inputs in = random_inputs(state);
        apply_inputs(sim.dut.get(), in);
        observe(cov, in);
        sim.dut->eval();
        const auto first = capture_outputs(sim.dut.get());
        sim.dut->eval();
        const auto second = capture_outputs(sim.dut.get());
        for (int chunk = 0; chunk < kNumChunks; ++chunk) {
            sim.check(first[chunk] == second[chunk], "random deterministic output chunk " + std::to_string(chunk));
        }
    }

    check_coverage(sim, cov);
    return sim.finish();
}
