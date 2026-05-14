// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_c2_tb.h"
#include "cosim_ctrl.h"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using DUT = Vcosim_txfma_c2_tb;

namespace {

constexpr int kNumInputs = 24;
constexpr int kRandomIters = 4096;
constexpr int kTargetComparisons = 50000;
constexpr std::array<uint64_t, kNumInputs> kInputMasks = {0xfull, 0x7full, 0x7ffffull, 0x7ull, 0x3ffull, 0x3ffull, 0x3ffull, 0x1full, 0x1full, 0x1full, 0x1full, 0x1full, 0xffull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull};
const char* const kInputNames[kNumInputs] = {"op_dtype_f2a_h", "cmd_f2a_h", "sigs_f2a_h", "rm_f2a_h", "ediff1_f2a_h", "ediff1_abs_f2a_h", "ediff2_abs_f2a_h", "ediff1_sel_f2a_h", "ediff2_sel_f2a_h", "align1_mux_sel_f2a_h", "align2_mux_sel_f2a_h", "opd3_mux_sel_f2a_h", "ec_f2a_h", "ec_zero_f2a_h", "eprod_hi_zero_f2a_h", "fc_b0_f2a_h", "e_sticky1_f2a_h", "e_sticky2_f2a_h", "sc_f2a_h", "sel_fcmask_f2a_h", "prod_sign_hi_f2a_h", "prod_sign_lo_f2a_h", "fc_zero30_8_f2a_h", "minmax_op_f2a_h"};

struct Inputs {
    uint64_t op_dtype_f2a_h = 0;
    uint64_t cmd_f2a_h = 0;
    uint64_t sigs_f2a_h = 0;
    uint64_t rm_f2a_h = 0;
    uint64_t ediff1_f2a_h = 0;
    uint64_t ediff1_abs_f2a_h = 0;
    uint64_t ediff2_abs_f2a_h = 0;
    uint64_t ediff1_sel_f2a_h = 0;
    uint64_t ediff2_sel_f2a_h = 0;
    uint64_t align1_mux_sel_f2a_h = 0;
    uint64_t align2_mux_sel_f2a_h = 0;
    uint64_t opd3_mux_sel_f2a_h = 0;
    uint64_t ec_f2a_h = 0;
    uint64_t ec_zero_f2a_h = 0;
    uint64_t eprod_hi_zero_f2a_h = 0;
    uint64_t fc_b0_f2a_h = 0;
    uint64_t e_sticky1_f2a_h = 0;
    uint64_t e_sticky2_f2a_h = 0;
    uint64_t sc_f2a_h = 0;
    uint64_t sel_fcmask_f2a_h = 0;
    uint64_t prod_sign_hi_f2a_h = 0;
    uint64_t prod_sign_lo_f2a_h = 0;
    uint64_t fc_zero30_8_f2a_h = 0;
    uint64_t minmax_op_f2a_h = 0;
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
    return {in.op_dtype_f2a_h, in.cmd_f2a_h, in.sigs_f2a_h, in.rm_f2a_h, in.ediff1_f2a_h, in.ediff1_abs_f2a_h, in.ediff2_abs_f2a_h, in.ediff1_sel_f2a_h, in.ediff2_sel_f2a_h, in.align1_mux_sel_f2a_h, in.align2_mux_sel_f2a_h, in.opd3_mux_sel_f2a_h, in.ec_f2a_h, in.ec_zero_f2a_h, in.eprod_hi_zero_f2a_h, in.fc_b0_f2a_h, in.e_sticky1_f2a_h, in.e_sticky2_f2a_h, in.sc_f2a_h, in.sel_fcmask_f2a_h, in.prod_sign_hi_f2a_h, in.prod_sign_lo_f2a_h, in.fc_zero30_8_f2a_h, in.minmax_op_f2a_h};
}

void apply_inputs(DUT* dut, const Inputs& in) {
    dut->op_dtype_f2a_h = in.op_dtype_f2a_h & 0xfull;
    dut->cmd_f2a_h = in.cmd_f2a_h & 0x7full;
    dut->sigs_f2a_h = in.sigs_f2a_h & 0x7ffffull;
    dut->rm_f2a_h = in.rm_f2a_h & 0x7ull;
    dut->ediff1_f2a_h = in.ediff1_f2a_h & 0x3ffull;
    dut->ediff1_abs_f2a_h = in.ediff1_abs_f2a_h & 0x3ffull;
    dut->ediff2_abs_f2a_h = in.ediff2_abs_f2a_h & 0x3ffull;
    dut->ediff1_sel_f2a_h = in.ediff1_sel_f2a_h & 0x1full;
    dut->ediff2_sel_f2a_h = in.ediff2_sel_f2a_h & 0x1full;
    dut->align1_mux_sel_f2a_h = in.align1_mux_sel_f2a_h & 0x1full;
    dut->align2_mux_sel_f2a_h = in.align2_mux_sel_f2a_h & 0x1full;
    dut->opd3_mux_sel_f2a_h = in.opd3_mux_sel_f2a_h & 0x1full;
    dut->ec_f2a_h = in.ec_f2a_h & 0xffull;
    dut->ec_zero_f2a_h = in.ec_zero_f2a_h & 0x1ull;
    dut->eprod_hi_zero_f2a_h = in.eprod_hi_zero_f2a_h & 0x1ull;
    dut->fc_b0_f2a_h = in.fc_b0_f2a_h & 0x1ull;
    dut->e_sticky1_f2a_h = in.e_sticky1_f2a_h & 0x1ull;
    dut->e_sticky2_f2a_h = in.e_sticky2_f2a_h & 0x1ull;
    dut->sc_f2a_h = in.sc_f2a_h & 0x1ull;
    dut->sel_fcmask_f2a_h = in.sel_fcmask_f2a_h & 0x1ull;
    dut->prod_sign_hi_f2a_h = in.prod_sign_hi_f2a_h & 0x1ull;
    dut->prod_sign_lo_f2a_h = in.prod_sign_lo_f2a_h & 0x1ull;
    dut->fc_zero30_8_f2a_h = in.fc_zero30_8_f2a_h & 0x1ull;
    dut->minmax_op_f2a_h = in.minmax_op_f2a_h & 0x1ull;
}

Inputs random_inputs(uint32_t& state) {
    return Inputs{(random64(state) & 0xfull), (random64(state) & 0x7full), (random64(state) & 0x7ffffull), (random64(state) & 0x7ull), (random64(state) & 0x3ffull), (random64(state) & 0x3ffull), (random64(state) & 0x3ffull), (random64(state) & 0x1full), (random64(state) & 0x1full), (random64(state) & 0x1full), (random64(state) & 0x1full), (random64(state) & 0x1full), (random64(state) & 0xffull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull), (random64(state) & 0x1ull)};
}

std::vector<Inputs> directed_inputs() {
    return {
        Inputs{0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull},  // zero
        Inputs{0xfull, 0x7full, 0x7ffffull, 0x7ull, 0x3ffull, 0x3ffull, 0x3ffull, 0x1full, 0x1full, 0x1full, 0x1full, 0x1full, 0xffull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull, 0x1ull},  // ones
        Inputs{0x6ull, 0x3ull, 0x796e1ull, 0x5ull, 0x311ull, 0x36dull, 0x2dcull, 0x7ull, 0x2ull, 0x14ull, 0x18ull, 0x11ull, 0x12ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull},  // pattern_a
        Inputs{0x1ull, 0x2eull, 0x2311dull, 0x5ull, 0x392ull, 0x373ull, 0x2a5ull, 0x19ull, 0x9ull, 0x10ull, 0x6ull, 0x15ull, 0x3ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x1ull},  // pattern_b
        Inputs{0x1ull, 0x1ull, 0x40ull, 0x0ull, 0x3ffull, 0x1ull, 0x2ull, 0x6ull, 0x1ull, 0x0ull, 0x1ull, 0x2ull, 0x7full, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull},  // f32_sub_ediff_neg1
        Inputs{0x0ull, 0x15ull, 0x2100ull, 0x0ull, 0x1ull, 0x1ull, 0x3ull, 0x2ull, 0x7ull, 0x8ull, 0x9ull, 0xaull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull},  // int_cvt_signed
        Inputs{0x1ull, 0x75ull, 0x10000ull, 0x0ull, 0x3ffull, 0x1ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x7eull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull},  // sin_transform_negative
        Inputs{0x1ull, 0x4cull, 0x20ull, 0x3ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull, 0x1ull, 0x0ull, 0x0ull, 0x0ull, 0x0ull},  // ffrc_mask
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
    sim.compare("eff_sub1_f2a_h", sim.dut->orig_eff_sub1_f2a_h, sim.dut->new_eff_sub1_f2a_h);
    sim.compare("eff_sub2_f2a_h", sim.dut->orig_eff_sub2_f2a_h, sim.dut->new_eff_sub2_f2a_h);
    sim.compare("comp_op_f2a_h", sim.dut->orig_comp_op_f2a_h, sim.dut->new_comp_op_f2a_h);
    sim.compare("ediff1_0_f2a_h", sim.dut->orig_ediff1_0_f2a_h, sim.dut->new_ediff1_0_f2a_h);
    sim.compare("fa_ge_fc_comp_sign_hi_res_f2a_h", sim.dut->orig_fa_ge_fc_comp_sign_hi_res_f2a_h, sim.dut->new_fa_ge_fc_comp_sign_hi_res_f2a_h);
    sim.compare("fa_eq_fc_comp_sign_hi_res_f2a_h", sim.dut->orig_fa_eq_fc_comp_sign_hi_res_f2a_h, sim.dut->new_fa_eq_fc_comp_sign_hi_res_f2a_h);
    sim.compare("ediff1_lt_31_f2a_h", sim.dut->orig_ediff1_lt_31_f2a_h, sim.dut->new_ediff1_lt_31_f2a_h);
    sim.compare("cvt_fui_pos_ovf_f2a_h", sim.dut->orig_cvt_fui_pos_ovf_f2a_h, sim.dut->new_cvt_fui_pos_ovf_f2a_h);
    sim.compare("cvt_fi_pos_ovf_f2a_h", sim.dut->orig_cvt_fi_pos_ovf_f2a_h, sim.dut->new_cvt_fi_pos_ovf_f2a_h);
    sim.compare("cvt_fi_neg_ovf_f2a_h", sim.dut->orig_cvt_fi_neg_ovf_f2a_h, sim.dut->new_cvt_fi_neg_ovf_f2a_h);
    sim.compare("cvt_fi_min_neg_f2a_h", sim.dut->orig_cvt_fi_min_neg_f2a_h, sim.dut->new_cvt_fi_min_neg_f2a_h);
    sim.compare("fc_sel_f2a_h", sim.dut->orig_fc_sel_f2a_h, sim.dut->new_fc_sel_f2a_h);
    sim.compare("op3p1c_sel_p_f2a_h", sim.dut->orig_op3p1c_sel_p_f2a_h, sim.dut->new_op3p1c_sel_p_f2a_h);
    sim.compare("op3p1c_sel_r_f2a_h", sim.dut->orig_op3p1c_sel_r_f2a_h, sim.dut->new_op3p1c_sel_r_f2a_h);
    sim.compare("op3g_sel_f2a_h", sim.dut->orig_op3g_sel_f2a_h, sim.dut->new_op3g_sel_f2a_h);
    sim.compare("rm_rne_f2a_h", sim.dut->orig_rm_rne_f2a_h, sim.dut->new_rm_rne_f2a_h);
    sim.compare("rm_rmm_f2a_h", sim.dut->orig_rm_rmm_f2a_h, sim.dut->new_rm_rmm_f2a_h);
    sim.compare("frnd_rupnsc_f2a_h", sim.dut->orig_frnd_rupnsc_f2a_h, sim.dut->new_frnd_rupnsc_f2a_h);
    sim.compare("frnd_rdnsc_f2a_h", sim.dut->orig_frnd_rdnsc_f2a_h, sim.dut->new_frnd_rdnsc_f2a_h);
    sim.compare("frnd_t1_f2a_h", sim.dut->orig_frnd_t1_f2a_h, sim.dut->new_frnd_t1_f2a_h);
    sim.compare("frnd_t2_f2a_h", sim.dut->orig_frnd_t2_f2a_h, sim.dut->new_frnd_t2_f2a_h);
    sim.compare("al1c_zap_f2a_h", sim.dut->orig_al1c_zap_f2a_h, sim.dut->new_al1c_zap_f2a_h);
    sim.compare("op1c_log_mul_clr_f2a_h", sim.dut->orig_op1c_log_mul_clr_f2a_h, sim.dut->new_op1c_log_mul_clr_f2a_h);
    sim.compare("negop1c_int_logmul_clr_f2a_h", sim.dut->orig_negop1c_int_logmul_clr_f2a_h, sim.dut->new_negop1c_int_logmul_clr_f2a_h);
    sim.compare("psc_op1_clr_f2a_h", sim.dut->orig_psc_op1_clr_f2a_h, sim.dut->new_psc_op1_clr_f2a_h);
    sim.compare("align2_zap_f2a_h", sim.dut->orig_align2_zap_f2a_h, sim.dut->new_align2_zap_f2a_h);
    sim.compare("align2c_neg_f2a_h", sim.dut->orig_align2c_neg_f2a_h, sim.dut->new_align2c_neg_f2a_h);
    sim.compare("trans_log_rr_neg_f2a_h", sim.dut->orig_trans_log_rr_neg_f2a_h, sim.dut->new_trans_log_rr_neg_f2a_h);
    sim.compare("sin_eff_sub1_f2a_h", sim.dut->orig_sin_eff_sub1_f2a_h, sim.dut->new_sin_eff_sub1_f2a_h);
    sim.compare("al2c3_neg_cin_ps_f2a_h", sim.dut->orig_al2c3_neg_cin_ps_f2a_h, sim.dut->new_al2c3_neg_cin_ps_f2a_h);
    sim.compare("al1c3_neg_cin_f2a_h", sim.dut->orig_al1c3_neg_cin_f2a_h, sim.dut->new_al1c3_neg_cin_f2a_h);
    sim.compare("op2_sxtto_g_f2a_h", sim.dut->orig_op2_sxtto_g_f2a_h, sim.dut->new_op2_sxtto_g_f2a_h);
    sim.compare("op3p3spc_sel_f2a_h", sim.dut->orig_op3p3spc_sel_f2a_h, sim.dut->new_op3p3spc_sel_f2a_h);
    sim.compare("op3p2c_sel_f2a_h", sim.dut->orig_op3p2c_sel_f2a_h, sim.dut->new_op3p2c_sel_f2a_h);
    sim.compare("ugfxrplc_sel_f2a_h", sim.dut->orig_ugfxrplc_sel_f2a_h, sim.dut->new_ugfxrplc_sel_f2a_h);
    sim.compare("sgfxrplc_sel_f2a_h", sim.dut->orig_sgfxrplc_sel_f2a_h, sim.dut->new_sgfxrplc_sel_f2a_h);
    sim.compare("icgfxc_sel_f2a_h", sim.dut->orig_icgfxc_sel_f2a_h, sim.dut->new_icgfxc_sel_f2a_h);
    sim.compare("al1znp2c_sel_f2a_h", sim.dut->orig_al1znp2c_sel_f2a_h, sim.dut->new_al1znp2c_sel_f2a_h);
    sim.compare("al2c_sel_f2a_h", sim.dut->orig_al2c_sel_f2a_h, sim.dut->new_al2c_sel_f2a_h);
    sim.compare("al1p2c_sel_f2a_h", sim.dut->orig_al1p2c_sel_f2a_h, sim.dut->new_al1p2c_sel_f2a_h);
    sim.compare("sin_ediff2_f2a_h", sim.dut->orig_sin_ediff2_f2a_h, sim.dut->new_sin_ediff2_f2a_h);
    sim.compare("al1p1c_sel_f2a_h", sim.dut->orig_al1p1c_sel_f2a_h, sim.dut->new_al1p1c_sel_f2a_h);
    sim.compare("sin_align1_neg_f2a_h", sim.dut->orig_sin_align1_neg_f2a_h, sim.dut->new_sin_align1_neg_f2a_h);
    sim.compare("sin_align2_neg_f2a_h", sim.dut->orig_sin_align2_neg_f2a_h, sim.dut->new_sin_align2_neg_f2a_h);
    sim.compare("sin_check_fc_sign_f2a_h", sim.dut->orig_sin_check_fc_sign_f2a_h, sim.dut->new_sin_check_fc_sign_f2a_h);
    sim.compare("int_minmax_mux_sel_fc_f2a_h", sim.dut->orig_int_minmax_mux_sel_fc_f2a_h, sim.dut->new_int_minmax_mux_sel_fc_f2a_h);
    sim.compare("eff_sign1_f2a_h", sim.dut->orig_eff_sign1_f2a_h, sim.dut->new_eff_sign1_f2a_h);
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
