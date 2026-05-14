// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_e2_tb.h"
#include "sim_ctrl.h"
#include "txfma_e_stage_model.h"

#include <string>

using DUT = Vtxfma_e2_tb;
using namespace txfma_e_model;

namespace {

struct CovIdx {
    int cmd, op_dtype, sigs, ediff_ec_eph, ediff_ec_epl, ediff_eph_epl, eprod_hi, eprod_lo;
    int ec, eb_hi, etrz_fa_lo_ps, etrz_fc, etrz_prod_hi_ph, etrz_prod_lo_ps, sin_ediff2;
    int eprod_hi_zero, eprod_lo_zero, ec_zero, exp_fma2_exc;
};

CovIdx add_cov(Coverage& cov) {
    return CovIdx{cov.add("cmd_i", 7),
                  cov.add("op_dtype_i", 4),
                  cov.add("sigs_i", 19),
                  cov.add("ediff_ec_eph_i", 10),
                  cov.add("ediff_ec_epl_i", 10),
                  cov.add("ediff_eph_epl_i", 10),
                  cov.add("eprod_hi_i", 10),
                  cov.add("eprod_lo_i", 10),
                  cov.add("ec_i", 8),
                  cov.add("eb_hi_i", 8),
                  cov.add("etrz_fa_lo_ps_i", 5),
                  cov.add("etrz_fc_i", 5),
                  cov.add("etrz_prod_hi_ph_i", 5),
                  cov.add("etrz_prod_lo_ps_i", 6),
                  cov.add("sin_ediff2_i", 1),
                  cov.add("eprod_hi_zero_i", 1),
                  cov.add("eprod_lo_zero_i", 1),
                  cov.add("ec_zero_i", 1),
                  cov.add("exp_fma2_exc_i", 1)};
}

void drive_inputs(DUT* dut, const E2In& in) {
    dut->cmd_i = in.cmd;
    dut->op_dtype_i = in.op_dtype;
    dut->sigs_i = in.sigs;
    dut->ediff_ec_eph_i = in.ediff_ec_eph;
    dut->ediff_ec_epl_i = in.ediff_ec_epl;
    dut->ediff_eph_epl_i = in.ediff_eph_epl;
    dut->eprod_hi_i = in.eprod_hi;
    dut->eprod_lo_i = in.eprod_lo;
    dut->ec_i = in.ec;
    dut->eb_hi_i = in.eb_hi;
    dut->etrz_fa_lo_ps_i = in.etrz_fa_lo_ps;
    dut->etrz_fc_i = in.etrz_fc;
    dut->etrz_prod_hi_ph_i = in.etrz_prod_hi_ph;
    dut->etrz_prod_lo_ps_i = in.etrz_prod_lo_ps;
    dut->sin_ediff2_i = in.sin_ediff2;
    dut->eprod_hi_zero_i = in.eprod_hi_zero;
    dut->eprod_lo_zero_i = in.eprod_lo_zero;
    dut->ec_zero_i = in.ec_zero;
    dut->exp_fma2_exc_i = in.exp_fma2_exc;
}

void observe(Coverage& cov, const CovIdx& idx, const E2In& in) {
    cov.observe(idx.cmd, in.cmd);
    cov.observe(idx.op_dtype, in.op_dtype);
    cov.observe(idx.sigs, in.sigs);
    cov.observe(idx.ediff_ec_eph, in.ediff_ec_eph);
    cov.observe(idx.ediff_ec_epl, in.ediff_ec_epl);
    cov.observe(idx.ediff_eph_epl, in.ediff_eph_epl);
    cov.observe(idx.eprod_hi, in.eprod_hi);
    cov.observe(idx.eprod_lo, in.eprod_lo);
    cov.observe(idx.ec, in.ec);
    cov.observe(idx.eb_hi, in.eb_hi);
    cov.observe(idx.etrz_fa_lo_ps, in.etrz_fa_lo_ps);
    cov.observe(idx.etrz_fc, in.etrz_fc);
    cov.observe(idx.etrz_prod_hi_ph, in.etrz_prod_hi_ph);
    cov.observe(idx.etrz_prod_lo_ps, in.etrz_prod_lo_ps);
    cov.observe(idx.sin_ediff2, in.sin_ediff2);
    cov.observe(idx.eprod_hi_zero, in.eprod_hi_zero);
    cov.observe(idx.eprod_lo_zero, in.eprod_lo_zero);
    cov.observe(idx.ec_zero, in.ec_zero);
    cov.observe(idx.exp_fma2_exc, in.exp_fma2_exc);
}

void check_case(SimCtrl<DUT>& sim, Coverage& cov, const CovIdx& idx, const E2In& in,
                const char* name) {
    drive_inputs(sim.dut.get(), in);
    observe(cov, idx, in);
    sim.dut->eval();
    const E2Out exp = model_e2(in);
    const std::string p = std::string(name) + ": ";
    sim.check(sim.dut->e_sticky1_o == exp.e_sticky1, p + "e_sticky1");
    sim.check(sim.dut->e_sticky2_o == exp.e_sticky2, p + "e_sticky2");
    sim.check(sim.dut->ediff1_zap_o == exp.ediff1_zap, p + "ediff1_zap");
    sim.check(sim.dut->ediff_abs_diff_hi_o == exp.ediff_abs_diff_hi, p + "ediff_abs_diff_hi");
    sim.check(sim.dut->align2_rshamt_c3_o == exp.align2_rshamt_c3, p + "align2_rshamt_c3");
    sim.check(sim.dut->exp_res_2f3_o == exp.exp_res_2f3, p + "exp_res_2f3");
    sim.check(sim.dut->align1_mux_sel_o == exp.align1_mux_sel, p + "align1_mux_sel");
    sim.check(sim.dut->align2_mux_sel_o == exp.align2_mux_sel, p + "align2_mux_sel");
    sim.check(sim.dut->opd3_mux_sel_o == exp.opd3_mux_sel, p + "opd3_mux_sel");
    sim.check(sim.dut->ediff1_o == exp.ediff1, p + "ediff1");
    sim.check(sim.dut->ediff2_o == exp.ediff2, p + "ediff2");
    sim.check(sim.dut->ediff1_sel_o == exp.ediff1_sel, p + "ediff1_sel");
    sim.check(sim.dut->ediff2_sel_o == exp.ediff2_sel, p + "ediff2_sel");
    sim.check(sim.dut->ediff1_abs_o == exp.ediff1_abs, p + "ediff1_abs");
    sim.check(sim.dut->ediff2_abs_o == exp.ediff2_abs, p + "ediff2_abs");
    sim.check(sim.dut->exp_res_o == exp.exp_res, p + "exp_res");
}

E2In random_input(uint32_t& seed) {
    E2In in;
    in.cmd = xorshift32(seed) & 0x7fu;
    in.op_dtype = xorshift32(seed) & 0xfu;
    in.sigs = xorshift32(seed) & 0x7ffffu;
    in.ediff_ec_eph = xorshift32(seed) & 0x3ffu;
    in.ediff_ec_epl = xorshift32(seed) & 0x3ffu;
    in.ediff_eph_epl = xorshift32(seed) & 0x3ffu;
    in.eprod_hi = xorshift32(seed) & 0x3ffu;
    in.eprod_lo = xorshift32(seed) & 0x3ffu;
    in.ec = xorshift32(seed) & 0xffu;
    in.eb_hi = xorshift32(seed) & 0xffu;
    in.etrz_fa_lo_ps = xorshift32(seed) & 0x1fu;
    in.etrz_fc = xorshift32(seed) & 0x1fu;
    in.etrz_prod_hi_ph = xorshift32(seed) & 0x1fu;
    in.etrz_prod_lo_ps = xorshift32(seed) & 0x3fu;
    in.sin_ediff2 = (xorshift32(seed) & 1u) != 0u;
    in.eprod_hi_zero = (xorshift32(seed) & 1u) != 0u;
    in.eprod_lo_zero = (xorshift32(seed) & 1u) != 0u;
    in.ec_zero = (xorshift32(seed) & 1u) != 0u;
    in.exp_fma2_exc = (xorshift32(seed) & 1u) != 0u;
    return in;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    Coverage cov;
    const CovIdx idx = add_cov(cov);

    check_case(sim, cov, idx,
               E2In{VPU_FCMD_ADD, VPU_DTYPE_F32, 0, 0x3fb, 0x3f0, 0x001, 0x081, 0x080,
                    0x7f, 0x40, 2, 3, 4, 5, false, false, false, false, false},
               "f32_product_gt_c_negative_ediff");
    check_case(sim, cov, idx,
               E2In{VPU_FCMD_ADD, VPU_DTYPE_F32, 0, 0x005, 0x006, 0x001, 0x081, 0x080,
                    0x90, 0x40, 2, 3, 4, 5, false, false, false, false, false},
               "f32_c_gt_product_positive_ediff");
    check_case(sim, cov, idx,
               E2In{VPU_FCMD_ADD, VPU_DTYPE_F32, 0, 0x000, 0x000, 0x000, 0x080, 0x080,
                    0x80, 0x40, 2, 3, 4, 5, false, false, false, false, false},
               "f32_tied_exponents");
    check_case(sim, cov, idx,
               E2In{VPU_FCMD_ADD, VPU_DTYPE_F32, 0, 0x000, 0x000, 0x000, 0x000, 0x080,
                    0x00, 0x40, 2, 3, 4, 5, false, true, false, false, false},
               "eprod_hi_zero_forces_sign");
    check_case(sim, cov, idx,
               E2In{VPU_FCMD_ADD, VPU_DTYPE_F16_F32, 0, 0x001, 0x001, 0x3ff, 0x080, 0x000,
                    0x00, 0x40, 2, 3, 4, 5, false, false, true, true, false},
               "ec_zero_forces_f16_order");
    check_case(sim, cov, idx,
               E2In{VPU_TRANS_SIN_TRANSFORM, VPU_DTYPE_F32, 0, 0x3fe, 0x002, 0x3fc, 0x085,
                    0x084, 0x80, 0x40, 2, 3, 4, 5, true, false, false, false, false},
               "sin_ediff2_forces_shift_one");
    check_case(sim, cov, idx,
               E2In{VPU_TRANS_EXP_FMA2, VPU_DTYPE_INT, 1u << TXFMA_TRANS2, 0x3fe, 0x002,
                    0x3fc, 0x085, 0x084, 0x80, 0x41, 2, 3, 4, 5, false, false, false,
                    false, true},
               "exp_fma2_exception_selects_eb");
    check_case(sim, cov, idx,
               E2In{VPU_FCMD_FROUND, VPU_DTYPE_F32, 1u << TXFMA_CVT, 0x020, 0x000, 0x000,
                    0x090, 0x080, 0x80, 0x40, 2, 3, 4, 5, false, false, false, false,
                    false},
               "fround_zaps_nonnegative_large_ediff");
    check_case(sim, cov, idx,
               E2In{VPU_FCMD_CVT_F16F32, VPU_DTYPE_F32, 1u << TXFMA_CVT, 0x3f0, 0x000, 0x000,
                    0x090, 0x080, 0x80, 0x40, 2, 3, 4, 5, false, false, false, false,
                    false},
               "cvt_f16f32_negative_sticky_distance");
    check_case(sim, cov, idx,
               E2In{VPU_TRANS_EXP_FRAC, VPU_DTYPE_F32, 0, 0x3f0, 0x3e0, 0x000, 0x090, 0x080,
                    0x10, 0x40, 2, 3, 4, 5, false, false, false, false, false},
               "exp_frac_uses_exp_shift");
    check_case(sim, cov, idx,
               E2In{VPU_FCMD_ADD, VPU_DTYPE_F16_F32, 1u << TXFMA_MUL, 0x3f0, 0x3e0,
                    0x3ff, 0x090, 0x080, 0x10, 0x40, 2, 3, 4, 5, false, false, false,
                    false, false},
               "f16_f32_mul_selects_products");
    check_case(sim, cov, idx,
               E2In{0x7f, 0xf, 0x7ffff, 0x3ff, 0x3ff, 0x3ff, 0x3ff, 0x3ff, 0xff, 0xff,
                    0x1f, 0x1f, 0x1f, 0x3f, true, true, true, true, true},
               "all_ones_coverage");

    uint32_t seed = 0xe2a21234u;
    for (int i = 0; i < 1024; ++i) {
        check_case(sim, cov, idx, random_input(seed), "random");
    }

    cov.check(sim);
    return sim.finish();
}
