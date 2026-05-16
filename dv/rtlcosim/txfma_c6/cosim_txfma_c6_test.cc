// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_c6_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using DUT = Vcosim_txfma_c6_tb;

namespace {

constexpr int kNumInputs = 61;
constexpr int kRandomIters = 4096;
constexpr int kTargetComparisons = 50000;
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

std::vector<Inputs> directed_inputs() {
    return {
        Inputs{0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull},  // zero
        Inputs{0xfull, 0x7full, 0x1ull, 0x7ffffull, 0x3full, 0x7ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull},  // ones
        Inputs{0x2ull, 0x7ull, 0x1ull, 0x6fe6cull, 0x31ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull},  // pattern_a
        Inputs{0x5ull, 0x2aull, 0x1ull, 0x5f124ull, 0x32ull, 0x7ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull},  // pattern_b
        Inputs{0x1ull, 0x7ull, 0x0ull, 0x40008ull, 0x0ull, 0x2ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull},  // f32_round_ovf
        Inputs{0x2ull, 0x7bull, 0x0ull, 0x100ull, 0x0ull, 0x3ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull},  // ph_cvt_unf
        Inputs{0x0ull, 0x15ull, 0x0ull, 0x8100ull, 0x3full, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull},  // int_cvt_invalid
        Inputs{0x1ull, 0x1full, 0x0ull, 0x20ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull},  // compare_true_nan
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
    sim.compare("sel_gmaxu24_f6a_h", sim.dut->orig_sel_gmaxu24_f6a_h, sim.dut->new_sel_gmaxu24_f6a_h);
    sim.compare("sel_gmaxu16_f6a_h", sim.dut->orig_sel_gmaxu16_f6a_h, sim.dut->new_sel_gmaxu16_f6a_h);
    sim.compare("sel_gmaxs16_f6a_h", sim.dut->orig_sel_gmaxs16_f6a_h, sim.dut->new_sel_gmaxs16_f6a_h);
    sim.compare("sel_gmins16_f6a_h", sim.dut->orig_sel_gmins16_f6a_h, sim.dut->new_sel_gmins16_f6a_h);
    sim.compare("sel_gmaxs8_f6a_h", sim.dut->orig_sel_gmaxs8_f6a_h, sim.dut->new_sel_gmaxs8_f6a_h);
    sim.compare("sel_gmaxu2_f6a_h", sim.dut->orig_sel_gmaxu2_f6a_h, sim.dut->new_sel_gmaxu2_f6a_h);
    sim.compare("sel_gmins8_f6a_h", sim.dut->orig_sel_gmins8_f6a_h, sim.dut->new_sel_gmins8_f6a_h);
    sim.compare("sel_gmaxu8_f6a_h", sim.dut->orig_sel_gmaxu8_f6a_h, sim.dut->new_sel_gmaxu8_f6a_h);
    sim.compare("sel_gfxmax_f6a_h", sim.dut->orig_sel_gfxmax_f6a_h, sim.dut->new_sel_gfxmax_f6a_h);
    sim.compare("sel_psexc_sr_f6a_h", sim.dut->orig_sel_psexc_sr_f6a_h, sim.dut->new_sel_psexc_sr_f6a_h);
    sim.compare("sel_bit31_one_f6a_h", sim.dut->orig_sel_bit31_one_f6a_h, sim.dut->new_sel_bit31_one_f6a_h);
    sim.compare("sel_b30to0_ones_f6a_h", sim.dut->orig_sel_b30to0_ones_f6a_h, sim.dut->new_sel_b30to0_ones_f6a_h);
    sim.compare("sel_zerores_f6a_h", sim.dut->orig_sel_zerores_f6a_h, sim.dut->new_sel_zerores_f6a_h);
    sim.compare("sel_ps_unf_f6a_h", sim.dut->orig_sel_ps_unf_f6a_h, sim.dut->new_sel_ps_unf_f6a_h);
    sim.compare("sel_ps_normone_f6a_h", sim.dut->orig_sel_ps_normone_f6a_h, sim.dut->new_sel_ps_normone_f6a_h);
    sim.compare("sel_ps_inf_f6a_h", sim.dut->orig_sel_ps_inf_f6a_h, sim.dut->new_sel_ps_inf_f6a_h);
    sim.compare("sel_ps_cqn_f6a_h", sim.dut->orig_sel_ps_cqn_f6a_h, sim.dut->new_sel_ps_cqn_f6a_h);
    sim.compare("sel_ps_large_f6a_h", sim.dut->orig_sel_ps_large_f6a_h, sim.dut->new_sel_ps_large_f6a_h);
    sim.compare("sel_phsp_f6a_h", sim.dut->orig_sel_phsp_f6a_h, sim.dut->new_sel_phsp_f6a_h);
    sim.compare("sel_phhi_sr_f6a_h", sim.dut->orig_sel_phhi_sr_f6a_h, sim.dut->new_sel_phhi_sr_f6a_h);
    sim.compare("sel_phhi_large_f6a_h", sim.dut->orig_sel_phhi_large_f6a_h, sim.dut->new_sel_phhi_large_f6a_h);
    sim.compare("sel_phhi_unf_f6a_h", sim.dut->orig_sel_phhi_unf_f6a_h, sim.dut->new_sel_phhi_unf_f6a_h);
    sim.compare("sel_phhi_infres_f6a_h", sim.dut->orig_sel_phhi_infres_f6a_h, sim.dut->new_sel_phhi_infres_f6a_h);
    sim.compare("sel_f11large_f6a_h", sim.dut->orig_sel_f11large_f6a_h, sim.dut->new_sel_f11large_f6a_h);
    sim.compare("sel_f10large_f6a_h", sim.dut->orig_sel_f10large_f6a_h, sim.dut->new_sel_f10large_f6a_h);
    sim.compare("sel_f11inf_f6a_h", sim.dut->orig_sel_f11inf_f6a_h, sim.dut->new_sel_f11inf_f6a_h);
    sim.compare("sel_f10cqnan_f6a_h", sim.dut->orig_sel_f10cqnan_f6a_h, sim.dut->new_sel_f10cqnan_f6a_h);
    sim.compare("sel_f11sp_f6a_h", sim.dut->orig_sel_f11sp_f6a_h, sim.dut->new_sel_f11sp_f6a_h);
    sim.compare("sel_f10sp_f6a_h", sim.dut->orig_sel_f10sp_f6a_h, sim.dut->new_sel_f10sp_f6a_h);
    sim.compare("ps_rndovf_f6a_h", sim.dut->orig_ps_rndovf_f6a_h, sim.dut->new_ps_rndovf_f6a_h);
    sim.compare("ph_rndovf_f6a_h", sim.dut->orig_ph_rndovf_f6a_h, sim.dut->new_ph_rndovf_f6a_h);
    sim.compare("sel_set_bit0_one_f6a_h", sim.dut->orig_sel_set_bit0_one_f6a_h, sim.dut->new_sel_set_bit0_one_f6a_h);
    sim.compare("effsub1_set_sign1_f6a_h", sim.dut->orig_effsub1_set_sign1_f6a_h, sim.dut->new_effsub1_set_sign1_f6a_h);
    sim.compare("normmask1_f6a_h", sim.dut->orig_normmask1_f6a_h, sim.dut->new_normmask1_f6a_h);
    sim.compare("normmask2_f6a_h", sim.dut->orig_normmask2_f6a_h, sim.dut->new_normmask2_f6a_h);
    sim.compare("sel_classify_out_f6a_h", sim.dut->orig_sel_classify_out_f6a_h, sim.dut->new_sel_classify_out_f6a_h);
    sim.compare("sel_nr1_frcpres_f6a_h", sim.dut->orig_sel_nr1_frcpres_f6a_h, sim.dut->new_sel_nr1_frcpres_f6a_h);
    sim.compare("trans_sinp2m_f6a_h", sim.dut->orig_trans_sinp2m_f6a_h, sim.dut->new_trans_sinp2m_f6a_h);
    sim.compare("f10f11mux_sel_f6a_h", sim.dut->orig_f10f11mux_sel_f6a_h, sim.dut->new_f10f11mux_sel_f6a_h);
    sim.compare("rrmux16l_sel_f6a_l", sim.dut->orig_rrmux16l_sel_f6a_l, sim.dut->new_rrmux16l_sel_f6a_l);
    sim.compare("rrmux32_sel_f6a_h", sim.dut->orig_rrmux32_sel_f6a_h, sim.dut->new_rrmux32_sel_f6a_h);
    sim.compare("sel_excreshi_f6a_h", sim.dut->orig_sel_excreshi_f6a_h, sim.dut->new_sel_excreshi_f6a_h);
    sim.compare("sel_excreslo_f6a_h", sim.dut->orig_sel_excreslo_f6a_h, sim.dut->new_sel_excreslo_f6a_h);
    sim.compare("sel_fmares_hi_f6a_h", sim.dut->orig_sel_fmares_hi_f6a_h, sim.dut->new_sel_fmares_hi_f6a_h);
    sim.compare("sel_fmares_lo_f6a_h", sim.dut->orig_sel_fmares_lo_f6a_h, sim.dut->new_sel_fmares_lo_f6a_h);
    sim.compare("fma_flags_f6a_h", sim.dut->orig_fma_flags_f6a_h, sim.dut->new_fma_flags_f6a_h);
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
