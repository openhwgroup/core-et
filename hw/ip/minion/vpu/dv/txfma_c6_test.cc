// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_c6_tb.h"
#include "sim_ctrl.h"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

using DUT = Vtxfma_c6_tb;

namespace {

constexpr int kNumInputs = 61;
constexpr int kNumChunks = 1;
constexpr std::array<uint64_t, kNumInputs> kInputMasks = {0xfull, 0x7full, 0x1ull, 0x7ffffull, 0x3full, 0x7ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull};
const char* const kInputNames[kNumInputs] = {"op_dtype_f6a_h", "cmd_f6a_h", "classify_zero_f6a_h", "sigs_f6a_h", "in_flags_f6a_h", "rm_f6a_h", "rmode_f6a_h", "eff_sub1_f6a_h", "exp_res_zero_f6a_h", "s_res_f6a_h", "flags_en_f6a_h", "ediff1_lt_31_f6a_h", "rc_ph_f6a_h", "rc_ps_f6a_h", "rc_int32_f6a_h", "ph_stky2l_f6a_h", "ps_stky2l_f6a_h", "int32_stky2l_f6a_h", "fround_stky2l_f6a_h", "minmax_op_f6a_h", "comp_op_f6a_h", "comp_op_true_f6a_h", "spec_res_en_f6a_h", "norm_f32_ovf_f6a_h", "norm_one_out_f6a_h", "ec_zero_f6a_h", "res_inf_f6a_h", "res_neg_inf_f6a_h", "res_qnan_f6a_h", "res_snan_f6a_h", "pot_ovf_ps_f6a_h", "def_ovf_ps_f6a_h", "pot_unf_ps_f6a_h", "def_unf_ps_f6a_h", "pot_ovf_ph_f6a_h", "def_ovf_ph_f6a_h", "pot_unf_ph_f6a_h", "def_unf_ph_f6a_h", "cvt_fui_pos_ovf_f6a_h", "cvt_fi_pos_ovf_f6a_h", "cvt_fi_neg_ovf_f6a_h", "cvt_fi_min_neg_f6a_h", "res_unorm_one_f6a_h", "res_snorm_one_f6a_h", "cmp_invalid_f6a_h", "res_pos_int_nan_f6a_h", "res_neg_int_nan_f6a_h", "res_uint_nan_f6a_h", "res_zero_f6a_h", "res_one_f6a_h", "a_inf_f6a_h", "frac_zero_f6a_h", "rnd_l_ph_f6a_h", "rnd_r_ph_f6a_h", "rnd_l_ps_f6a_h", "rnd_r_ps_f6a_h", "int_res_l_bit_f6a_h", "ph_allone2r_f6a_h", "ph_allone2l_f6a_h", "ps_allone2r_f6a_h", "ps_allone2l_f6a_h"};

struct Inputs {
    uint64_t op_dtype_f6a_h = 0;
    uint64_t cmd_f6a_h = 0;
    uint64_t classify_zero_f6a_h = 0;
    uint64_t sigs_f6a_h = 0;
    uint64_t in_flags_f6a_h = 0;
    uint64_t rm_f6a_h = 0;
    uint64_t rmode_f6a_h = 0;
    uint64_t eff_sub1_f6a_h = 0;
    uint64_t exp_res_zero_f6a_h = 0;
    uint64_t s_res_f6a_h = 0;
    uint64_t flags_en_f6a_h = 0;
    uint64_t ediff1_lt_31_f6a_h = 0;
    uint64_t rc_ph_f6a_h = 0;
    uint64_t rc_ps_f6a_h = 0;
    uint64_t rc_int32_f6a_h = 0;
    uint64_t ph_stky2l_f6a_h = 0;
    uint64_t ps_stky2l_f6a_h = 0;
    uint64_t int32_stky2l_f6a_h = 0;
    uint64_t fround_stky2l_f6a_h = 0;
    uint64_t minmax_op_f6a_h = 0;
    uint64_t comp_op_f6a_h = 0;
    uint64_t comp_op_true_f6a_h = 0;
    uint64_t spec_res_en_f6a_h = 0;
    uint64_t norm_f32_ovf_f6a_h = 0;
    uint64_t norm_one_out_f6a_h = 0;
    uint64_t ec_zero_f6a_h = 0;
    uint64_t res_inf_f6a_h = 0;
    uint64_t res_neg_inf_f6a_h = 0;
    uint64_t res_qnan_f6a_h = 0;
    uint64_t res_snan_f6a_h = 0;
    uint64_t pot_ovf_ps_f6a_h = 0;
    uint64_t def_ovf_ps_f6a_h = 0;
    uint64_t pot_unf_ps_f6a_h = 0;
    uint64_t def_unf_ps_f6a_h = 0;
    uint64_t pot_ovf_ph_f6a_h = 0;
    uint64_t def_ovf_ph_f6a_h = 0;
    uint64_t pot_unf_ph_f6a_h = 0;
    uint64_t def_unf_ph_f6a_h = 0;
    uint64_t cvt_fui_pos_ovf_f6a_h = 0;
    uint64_t cvt_fi_pos_ovf_f6a_h = 0;
    uint64_t cvt_fi_neg_ovf_f6a_h = 0;
    uint64_t cvt_fi_min_neg_f6a_h = 0;
    uint64_t res_unorm_one_f6a_h = 0;
    uint64_t res_snorm_one_f6a_h = 0;
    uint64_t cmp_invalid_f6a_h = 0;
    uint64_t res_pos_int_nan_f6a_h = 0;
    uint64_t res_neg_int_nan_f6a_h = 0;
    uint64_t res_uint_nan_f6a_h = 0;
    uint64_t res_zero_f6a_h = 0;
    uint64_t res_one_f6a_h = 0;
    uint64_t a_inf_f6a_h = 0;
    uint64_t frac_zero_f6a_h = 0;
    uint64_t rnd_l_ph_f6a_h = 0;
    uint64_t rnd_r_ph_f6a_h = 0;
    uint64_t rnd_l_ps_f6a_h = 0;
    uint64_t rnd_r_ps_f6a_h = 0;
    uint64_t int_res_l_bit_f6a_h = 0;
    uint64_t ph_allone2r_f6a_h = 0;
    uint64_t ph_allone2l_f6a_h = 0;
    uint64_t ps_allone2r_f6a_h = 0;
    uint64_t ps_allone2l_f6a_h = 0;
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
    return {in.op_dtype_f6a_h, in.cmd_f6a_h, in.classify_zero_f6a_h, in.sigs_f6a_h, in.in_flags_f6a_h, in.rm_f6a_h, in.rmode_f6a_h, in.eff_sub1_f6a_h, in.exp_res_zero_f6a_h, in.s_res_f6a_h, in.flags_en_f6a_h, in.ediff1_lt_31_f6a_h, in.rc_ph_f6a_h, in.rc_ps_f6a_h, in.rc_int32_f6a_h, in.ph_stky2l_f6a_h, in.ps_stky2l_f6a_h, in.int32_stky2l_f6a_h, in.fround_stky2l_f6a_h, in.minmax_op_f6a_h, in.comp_op_f6a_h, in.comp_op_true_f6a_h, in.spec_res_en_f6a_h, in.norm_f32_ovf_f6a_h, in.norm_one_out_f6a_h, in.ec_zero_f6a_h, in.res_inf_f6a_h, in.res_neg_inf_f6a_h, in.res_qnan_f6a_h, in.res_snan_f6a_h, in.pot_ovf_ps_f6a_h, in.def_ovf_ps_f6a_h, in.pot_unf_ps_f6a_h, in.def_unf_ps_f6a_h, in.pot_ovf_ph_f6a_h, in.def_ovf_ph_f6a_h, in.pot_unf_ph_f6a_h, in.def_unf_ph_f6a_h, in.cvt_fui_pos_ovf_f6a_h, in.cvt_fi_pos_ovf_f6a_h, in.cvt_fi_neg_ovf_f6a_h, in.cvt_fi_min_neg_f6a_h, in.res_unorm_one_f6a_h, in.res_snorm_one_f6a_h, in.cmp_invalid_f6a_h, in.res_pos_int_nan_f6a_h, in.res_neg_int_nan_f6a_h, in.res_uint_nan_f6a_h, in.res_zero_f6a_h, in.res_one_f6a_h, in.a_inf_f6a_h, in.frac_zero_f6a_h, in.rnd_l_ph_f6a_h, in.rnd_r_ph_f6a_h, in.rnd_l_ps_f6a_h, in.rnd_r_ps_f6a_h, in.int_res_l_bit_f6a_h, in.ph_allone2r_f6a_h, in.ph_allone2l_f6a_h, in.ps_allone2r_f6a_h, in.ps_allone2l_f6a_h};
}

void apply_inputs(DUT* dut, const Inputs& in) {
    dut->op_dtype_f6a_h = in.op_dtype_f6a_h & 0xfull;
    dut->cmd_f6a_h = in.cmd_f6a_h & 0x7full;
    dut->classify_zero_f6a_h = in.classify_zero_f6a_h & 0x1ull;
    dut->sigs_f6a_h = in.sigs_f6a_h & 0x7ffffull;
    dut->in_flags_f6a_h = in.in_flags_f6a_h & 0x3full;
    dut->rm_f6a_h = in.rm_f6a_h & 0x7ull;
    dut->rmode_f6a_h = in.rmode_f6a_h & 0x1ull;
    dut->eff_sub1_f6a_h = in.eff_sub1_f6a_h & 0x1ull;
    dut->exp_res_zero_f6a_h = in.exp_res_zero_f6a_h & 0x1ull;
    dut->s_res_f6a_h = in.s_res_f6a_h & 0x1ull;
    dut->flags_en_f6a_h = in.flags_en_f6a_h & 0x1ull;
    dut->ediff1_lt_31_f6a_h = in.ediff1_lt_31_f6a_h & 0x1ull;
    dut->rc_ph_f6a_h = in.rc_ph_f6a_h & 0x1ull;
    dut->rc_ps_f6a_h = in.rc_ps_f6a_h & 0x1ull;
    dut->rc_int32_f6a_h = in.rc_int32_f6a_h & 0x1ull;
    dut->ph_stky2l_f6a_h = in.ph_stky2l_f6a_h & 0x1ull;
    dut->ps_stky2l_f6a_h = in.ps_stky2l_f6a_h & 0x1ull;
    dut->int32_stky2l_f6a_h = in.int32_stky2l_f6a_h & 0x1ull;
    dut->fround_stky2l_f6a_h = in.fround_stky2l_f6a_h & 0x1ull;
    dut->minmax_op_f6a_h = in.minmax_op_f6a_h & 0x1ull;
    dut->comp_op_f6a_h = in.comp_op_f6a_h & 0x1ull;
    dut->comp_op_true_f6a_h = in.comp_op_true_f6a_h & 0x1ull;
    dut->spec_res_en_f6a_h = in.spec_res_en_f6a_h & 0x1ull;
    dut->norm_f32_ovf_f6a_h = in.norm_f32_ovf_f6a_h & 0x1ull;
    dut->norm_one_out_f6a_h = in.norm_one_out_f6a_h & 0x1ull;
    dut->ec_zero_f6a_h = in.ec_zero_f6a_h & 0x1ull;
    dut->res_inf_f6a_h = in.res_inf_f6a_h & 0x1ull;
    dut->res_neg_inf_f6a_h = in.res_neg_inf_f6a_h & 0x1ull;
    dut->res_qnan_f6a_h = in.res_qnan_f6a_h & 0x1ull;
    dut->res_snan_f6a_h = in.res_snan_f6a_h & 0x1ull;
    dut->pot_ovf_ps_f6a_h = in.pot_ovf_ps_f6a_h & 0x1ull;
    dut->def_ovf_ps_f6a_h = in.def_ovf_ps_f6a_h & 0x1ull;
    dut->pot_unf_ps_f6a_h = in.pot_unf_ps_f6a_h & 0x1ull;
    dut->def_unf_ps_f6a_h = in.def_unf_ps_f6a_h & 0x1ull;
    dut->pot_ovf_ph_f6a_h = in.pot_ovf_ph_f6a_h & 0x1ull;
    dut->def_ovf_ph_f6a_h = in.def_ovf_ph_f6a_h & 0x1ull;
    dut->pot_unf_ph_f6a_h = in.pot_unf_ph_f6a_h & 0x1ull;
    dut->def_unf_ph_f6a_h = in.def_unf_ph_f6a_h & 0x1ull;
    dut->cvt_fui_pos_ovf_f6a_h = in.cvt_fui_pos_ovf_f6a_h & 0x1ull;
    dut->cvt_fi_pos_ovf_f6a_h = in.cvt_fi_pos_ovf_f6a_h & 0x1ull;
    dut->cvt_fi_neg_ovf_f6a_h = in.cvt_fi_neg_ovf_f6a_h & 0x1ull;
    dut->cvt_fi_min_neg_f6a_h = in.cvt_fi_min_neg_f6a_h & 0x1ull;
    dut->res_unorm_one_f6a_h = in.res_unorm_one_f6a_h & 0x1ull;
    dut->res_snorm_one_f6a_h = in.res_snorm_one_f6a_h & 0x1ull;
    dut->cmp_invalid_f6a_h = in.cmp_invalid_f6a_h & 0x1ull;
    dut->res_pos_int_nan_f6a_h = in.res_pos_int_nan_f6a_h & 0x1ull;
    dut->res_neg_int_nan_f6a_h = in.res_neg_int_nan_f6a_h & 0x1ull;
    dut->res_uint_nan_f6a_h = in.res_uint_nan_f6a_h & 0x1ull;
    dut->res_zero_f6a_h = in.res_zero_f6a_h & 0x1ull;
    dut->res_one_f6a_h = in.res_one_f6a_h & 0x1ull;
    dut->a_inf_f6a_h = in.a_inf_f6a_h & 0x1ull;
    dut->frac_zero_f6a_h = in.frac_zero_f6a_h & 0x1ull;
    dut->rnd_l_ph_f6a_h = in.rnd_l_ph_f6a_h & 0x1ull;
    dut->rnd_r_ph_f6a_h = in.rnd_r_ph_f6a_h & 0x1ull;
    dut->rnd_l_ps_f6a_h = in.rnd_l_ps_f6a_h & 0x1ull;
    dut->rnd_r_ps_f6a_h = in.rnd_r_ps_f6a_h & 0x1ull;
    dut->int_res_l_bit_f6a_h = in.int_res_l_bit_f6a_h & 0x1ull;
    dut->ph_allone2r_f6a_h = in.ph_allone2r_f6a_h & 0x1ull;
    dut->ph_allone2l_f6a_h = in.ph_allone2l_f6a_h & 0x1ull;
    dut->ps_allone2r_f6a_h = in.ps_allone2r_f6a_h & 0x1ull;
    dut->ps_allone2l_f6a_h = in.ps_allone2l_f6a_h & 0x1ull;
}

Inputs random_inputs(uint32_t& state) {
    return Inputs{(random64(state) & 0xfull), (random64(state) & 0x7full), (random64(state) & 0x1ull), (random64(state) & 0x7ffffull), (random64(state) & 0x3full), (random64(state) & 0x7ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull)};
}

std::vector<TestCase> directed_cases() {
    return {
        {"zero", Inputs{0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x0000001000610280ull}},
        {"ones", Inputs{0xfull, 0x7full, 0x1ull, 0x7ffffull, 0x3full, 0x7ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x0000bba206616c2aull}},
        {"pattern_a", Inputs{0x2ull, 0x7ull, 0x1ull, 0x6fe6cull, 0x31ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x00008ba200616c32ull}},
        {"pattern_b", Inputs{0x5ull, 0x2aull, 0x1ull, 0x5f124ull, 0x32ull, 0x7ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x00008b1000616822ull}},
        {"f32_round_ovf", Inputs{0x1ull, 0x7ull, 0x0ull, 0x40008ull, 0x0ull, 0x2ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull}, std::array<uint64_t, kNumChunks>{0x0000411004e11405ull}},
        {"ph_cvt_unf", Inputs{0x2ull, 0x7bull, 0x0ull, 0x100ull, 0x0ull, 0x3ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x0000003402610c01ull}},
        {"int_cvt_invalid", Inputs{0x0ull, 0x15ull, 0x0ull, 0x8100ull, 0x3full, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x0000000000614290ull}},
        {"compare_true_nan", Inputs{0x1ull, 0x1full, 0x0ull, 0x20ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull}, std::array<uint64_t, kNumChunks>{0x0000300000610290ull}},
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
