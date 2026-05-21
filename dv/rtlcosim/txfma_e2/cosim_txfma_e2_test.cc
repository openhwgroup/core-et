// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_e2_tb.h"
#include "cosim_ctrl.h"
#include "../../../hw/ip/minion/vpu/dv/txfma_e_stage_model.h"

using DUT = Vcosim_txfma_e2_tb;
using namespace txfma_e_model;

namespace {

struct CovIdx {
    int cmd, op_dtype, sigs, ediff_ec_eph, ediff_ec_epl, ediff_eph_epl, eprod_hi, eprod_lo;
    int ec, eb_hi, etrz_fa_lo_ps, etrz_fc, etrz_prod_hi_ph, etrz_prod_lo_ps, sin_ediff2;
    int eprod_hi_zero, eprod_lo_zero, ec_zero, exp_fma2_exc;
};

CovIdx add_cov(Coverage& cov) {
    return CovIdx{cov.add("cmd_i", 7), cov.add("op_dtype_i", 4), cov.add("sigs_i", 19),
                  cov.add("ediff_ec_eph_i", 10), cov.add("ediff_ec_epl_i", 10),
                  cov.add("ediff_eph_epl_i", 10), cov.add("eprod_hi_i", 10),
                  cov.add("eprod_lo_i", 10), cov.add("ec_i", 8), cov.add("eb_hi_i", 8),
                  cov.add("etrz_fa_lo_ps_i", 5), cov.add("etrz_fc_i", 5),
                  cov.add("etrz_prod_hi_ph_i", 5), cov.add("etrz_prod_lo_ps_i", 6),
                  cov.add("sin_ediff2_i", 1), cov.add("eprod_hi_zero_i", 1),
                  cov.add("eprod_lo_zero_i", 1), cov.add("ec_zero_i", 1),
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

void drive_and_compare(CosimCtrl<DUT>& sim, Coverage& cov, const CovIdx& idx, const E2In& in) {
    drive_inputs(sim.dut.get(), in);
    observe(cov, idx, in);
    sim.dut->eval();
    const E2Out exp = model_e2(in);
    sim.compare("e_sticky1", sim.dut->orig_e_sticky1_o, sim.dut->new_e_sticky1_o);
    sim.compare("e_sticky2", sim.dut->orig_e_sticky2_o, sim.dut->new_e_sticky2_o);
    sim.compare("ediff1_zap", sim.dut->orig_ediff1_zap_o, sim.dut->new_ediff1_zap_o);
    sim.compare("ediff_abs_diff_hi", sim.dut->orig_ediff_abs_diff_hi_o, sim.dut->new_ediff_abs_diff_hi_o);
    sim.compare("align2_rshamt_c3", sim.dut->orig_align2_rshamt_c3_o, sim.dut->new_align2_rshamt_c3_o);
    sim.compare("exp_res_2f3", sim.dut->orig_exp_res_2f3_o, sim.dut->new_exp_res_2f3_o);
    sim.compare("align1_mux_sel", sim.dut->orig_align1_mux_sel_o, sim.dut->new_align1_mux_sel_o);
    sim.compare("align2_mux_sel", sim.dut->orig_align2_mux_sel_o, sim.dut->new_align2_mux_sel_o);
    sim.compare("opd3_mux_sel", sim.dut->orig_opd3_mux_sel_o, sim.dut->new_opd3_mux_sel_o);
    sim.compare("ediff1", sim.dut->orig_ediff1_o, sim.dut->new_ediff1_o);
    sim.compare("ediff2", sim.dut->orig_ediff2_o, sim.dut->new_ediff2_o);
    sim.compare("ediff1_sel", sim.dut->orig_ediff1_sel_o, sim.dut->new_ediff1_sel_o);
    sim.compare("ediff2_sel", sim.dut->orig_ediff2_sel_o, sim.dut->new_ediff2_sel_o);
    sim.compare("ediff1_abs", sim.dut->orig_ediff1_abs_o, sim.dut->new_ediff1_abs_o);
    sim.compare("ediff2_abs", sim.dut->orig_ediff2_abs_o, sim.dut->new_ediff2_abs_o);
    sim.compare("exp_res", sim.dut->orig_exp_res_o, sim.dut->new_exp_res_o);
    sim.check(sim.dut->new_e_sticky1_o == exp.e_sticky1, "model e_sticky1");
    sim.check(sim.dut->new_e_sticky2_o == exp.e_sticky2, "model e_sticky2");
    sim.check(sim.dut->new_ediff1_zap_o == exp.ediff1_zap, "model ediff1_zap");
    sim.check(sim.dut->new_ediff_abs_diff_hi_o == exp.ediff_abs_diff_hi, "model ediff_abs_diff_hi");
    sim.check(sim.dut->new_align2_rshamt_c3_o == exp.align2_rshamt_c3, "model align2_rshamt_c3");
    sim.check(sim.dut->new_exp_res_2f3_o == exp.exp_res_2f3, "model exp_res_2f3");
    sim.check(sim.dut->new_align1_mux_sel_o == exp.align1_mux_sel, "model align1_mux_sel");
    sim.check(sim.dut->new_align2_mux_sel_o == exp.align2_mux_sel, "model align2_mux_sel");
    sim.check(sim.dut->new_opd3_mux_sel_o == exp.opd3_mux_sel, "model opd3_mux_sel");
    sim.check(sim.dut->new_ediff1_o == exp.ediff1, "model ediff1");
    sim.check(sim.dut->new_ediff2_o == exp.ediff2, "model ediff2");
    sim.check(sim.dut->new_ediff1_sel_o == exp.ediff1_sel, "model ediff1_sel");
    sim.check(sim.dut->new_ediff2_sel_o == exp.ediff2_sel, "model ediff2_sel");
    sim.check(sim.dut->new_ediff1_abs_o == exp.ediff1_abs, "model ediff1_abs");
    sim.check(sim.dut->new_ediff2_abs_o == exp.ediff2_abs, "model ediff2_abs");
    sim.check(sim.dut->new_exp_res_o == exp.exp_res, "model exp_res");
    sim.sim_time++;
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
    CosimCtrl<DUT> sim(argc, argv);
    Coverage cov;
    const CovIdx idx = add_cov(cov);
    const E2In directed[] = {
        E2In{VPU_FCMD_ADD, VPU_DTYPE_F32, 0, 0x3fb, 0x3f0, 0x001, 0x081, 0x080, 0x7f, 0x40, 2, 3, 4, 5, false, false, false, false, false},
        E2In{VPU_FCMD_ADD, VPU_DTYPE_F32, 0, 0x005, 0x006, 0x001, 0x081, 0x080, 0x90, 0x40, 2, 3, 4, 5, false, false, false, false, false},
        E2In{VPU_FCMD_ADD, VPU_DTYPE_F32, 0, 0x000, 0x000, 0x000, 0x080, 0x080, 0x80, 0x40, 2, 3, 4, 5, false, false, false, false, false},
        E2In{VPU_FCMD_ADD, VPU_DTYPE_F32, 0, 0x000, 0x000, 0x000, 0x000, 0x080, 0x00, 0x40, 2, 3, 4, 5, false, true, false, false, false},
        E2In{VPU_FCMD_ADD, VPU_DTYPE_F16_F32, 0, 0x001, 0x001, 0x3ff, 0x080, 0x000, 0x00, 0x40, 2, 3, 4, 5, false, false, true, true, false},
        E2In{VPU_TRANS_SIN_TRANSFORM, VPU_DTYPE_F32, 0, 0x3fe, 0x002, 0x3fc, 0x085, 0x084, 0x80, 0x40, 2, 3, 4, 5, true, false, false, false, false},
        E2In{VPU_TRANS_EXP_FMA2, VPU_DTYPE_INT, 1u << TXFMA_TRANS2, 0x3fe, 0x002, 0x3fc, 0x085, 0x084, 0x80, 0x41, 2, 3, 4, 5, false, false, false, false, true},
        E2In{VPU_FCMD_FROUND, VPU_DTYPE_F32, 1u << TXFMA_CVT, 0x020, 0x000, 0x000, 0x090, 0x080, 0x80, 0x40, 2, 3, 4, 5, false, false, false, false, false},
        E2In{VPU_FCMD_CVT_F16F32, VPU_DTYPE_F32, 1u << TXFMA_CVT, 0x3f0, 0x000, 0x000, 0x090, 0x080, 0x80, 0x40, 2, 3, 4, 5, false, false, false, false, false},
        E2In{VPU_TRANS_EXP_FRAC, VPU_DTYPE_F32, 0, 0x3f0, 0x3e0, 0x000, 0x090, 0x080, 0x10, 0x40, 2, 3, 4, 5, false, false, false, false, false},
        E2In{VPU_FCMD_ADD, VPU_DTYPE_F16_F32, 1u << TXFMA_MUL, 0x3f0, 0x3e0, 0x3ff, 0x090, 0x080, 0x10, 0x40, 2, 3, 4, 5, false, false, false, false, false},
        E2In{0x7f, 0xf, 0x7ffff, 0x3ff, 0x3ff, 0x3ff, 0x3ff, 0x3ff, 0xff, 0xff, 0x1f, 0x1f, 0x1f, 0x3f, true, true, true, true, true},
    };
    for (const E2In& in : directed) {
        drive_and_compare(sim, cov, idx, in);
    }
    uint32_t seed = 0xe2a25678u;
    for (int i = 0; i < 5000; ++i) {
        drive_and_compare(sim, cov, idx, random_input(seed));
    }

    cov.check(sim);
    sim.check(sim.comparisons >= 80000, "txfma_e2 should exceed 80000 comparisons");
    return sim.finish();
}
