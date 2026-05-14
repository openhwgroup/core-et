// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_ediff_opdorder_logic_tb.h"
#include "sim_ctrl.h"
#include "txfma_e_stage_model.h"

#include <string>

using DUT = Vtxfma_ediff_opdorder_logic_tb;
using namespace txfma_e_model;

namespace {

struct CovIdx { int cmd, op_dtype, sigs, sign_c_ph, sign_c_pl, sign_ph_pl, exp_fma2_exc, exp_res_2f3; };

CovIdx add_cov(Coverage& cov) {
    return CovIdx{cov.add("cmd_i", 7), cov.add("op_dtype_i", 4), cov.add("sigs_i", 19),
                  cov.add("sign_c_ph_i", 1), cov.add("sign_c_pl_i", 1),
                  cov.add("sign_ph_pl_i", 1), cov.add("exp_fma2_exc_i", 1),
                  cov.add("exp_res_2f3_i", 7)};
}

void drive_inputs(DUT* dut, const EdiffIn& in) {
    dut->cmd_i = in.cmd;
    dut->op_dtype_i = in.op_dtype;
    dut->sigs_i = in.sigs;
    dut->sign_c_ph_i = in.sign_c_ph;
    dut->sign_c_pl_i = in.sign_c_pl;
    dut->sign_ph_pl_i = in.sign_ph_pl;
    dut->exp_fma2_exc_i = in.exp_fma2_exc;
    dut->exp_res_2f3_i = in.exp_res_2f3;
}

void observe(Coverage& cov, const CovIdx& idx, const EdiffIn& in) {
    cov.observe(idx.cmd, in.cmd);
    cov.observe(idx.op_dtype, in.op_dtype);
    cov.observe(idx.sigs, in.sigs);
    cov.observe(idx.sign_c_ph, in.sign_c_ph);
    cov.observe(idx.sign_c_pl, in.sign_c_pl);
    cov.observe(idx.sign_ph_pl, in.sign_ph_pl);
    cov.observe(idx.exp_fma2_exc, in.exp_fma2_exc);
    cov.observe(idx.exp_res_2f3, in.exp_res_2f3);
}

void check_case(SimCtrl<DUT>& sim, Coverage& cov, const CovIdx& idx, const EdiffIn& in,
                const char* name) {
    drive_inputs(sim.dut.get(), in);
    observe(cov, idx, in);
    sim.dut->eval();
    const EdiffOut exp = model_ediff(in);
    const std::string p = std::string(name) + ": ";
    sim.check(sim.dut->align1_mux_sel_o == exp.align1_mux_sel, p + "align1_mux_sel");
    sim.check(sim.dut->align2_mux_sel_o == exp.align2_mux_sel, p + "align2_mux_sel");
    sim.check(sim.dut->opd3_mux_sel_o == exp.opd3_mux_sel, p + "opd3_mux_sel");
    sim.check(sim.dut->ediff1_sel_o == exp.ediff1_sel, p + "ediff1_sel");
    sim.check(sim.dut->ediff2_sel_o == exp.ediff2_sel, p + "ediff2_sel");
    sim.check(sim.dut->res_exp_hi_sel_o == exp.res_exp_hi_sel, p + "res_exp_hi_sel");
    sim.check(sim.dut->res_exp_lo_sel_o == exp.res_exp_lo_sel, p + "res_exp_lo_sel");
}

EdiffIn random_input(uint32_t& seed) {
    EdiffIn in;
    in.cmd = xorshift32(seed) & 0x7fu;
    in.op_dtype = xorshift32(seed) & 0xfu;
    in.sigs = xorshift32(seed) & 0x7ffffu;
    in.sign_c_ph = (xorshift32(seed) & 1u) != 0u;
    in.sign_c_pl = (xorshift32(seed) & 1u) != 0u;
    in.sign_ph_pl = (xorshift32(seed) & 1u) != 0u;
    in.exp_fma2_exc = (xorshift32(seed) & 1u) != 0u;
    in.exp_res_2f3 = xorshift32(seed) & 0x7fu;
    return in;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    Coverage cov;
    const CovIdx idx = add_cov(cov);

    check_case(sim, cov, idx, EdiffIn{VPU_FCMD_ADD, VPU_DTYPE_F32, 0, false, false, false, false, 1},
               "f32_c_greater_add");
    check_case(sim, cov, idx, EdiffIn{VPU_FCMD_SUB, VPU_DTYPE_F32, 0, true, false, false, false, 1},
               "f32_product_greater_sub");
    check_case(sim, cov, idx, EdiffIn{VPU_FCMD_FROUND, VPU_DTYPE_F32, 1u << TXFMA_CVT, false, false, false, false, 1},
               "f32_cvt_round");
    check_case(sim, cov, idx, EdiffIn{VPU_TRANS_LOG_MUL, VPU_DTYPE_F32, 0, false, false, false, false, 0},
               "log_mul_zero_exp_res");
    check_case(sim, cov, idx, EdiffIn{VPU_TRANS_LOG_MUL, VPU_DTYPE_F32, 0, false, false, false, false, 3},
               "log_mul_nonzero_exp_res");
    check_case(sim, cov, idx, EdiffIn{VPU_TRANS_EXP_FRAC, VPU_DTYPE_F32, 0, false, false, false, false, 1},
               "exp_frac_selectors");
    check_case(sim, cov, idx, EdiffIn{VPU_TRANS_SIN_TRANSFORM, VPU_DTYPE_F32, 0, false, false, false, false, 1},
               "sin_transform_selectors");
    check_case(sim, cov, idx, EdiffIn{VPU_FCMD_ADD, VPU_DTYPE_F32, 1u << TXFMA_NORM_F32, false, false, false, false, 1},
               "norm_f32_selectors");
    check_case(sim, cov, idx, EdiffIn{VPU_FCMD_ADD, VPU_DTYPE_F32, 1u << TXFMA_FX_F32, false, false, false, false, 1},
               "fx_f32_selectors");
    check_case(sim, cov, idx, EdiffIn{VPU_FCMD_CVT_F32RAST, VPU_DTYPE_INT, 1u << TXFMA_CVT, false, false, false, false, 1},
               "int_cvt_f32rast");
    check_case(sim, cov, idx, EdiffIn{VPU_TRANS_EXP_FMA2, VPU_DTYPE_INT, 1u << TXFMA_TRANS2, false, false, false, true, 1},
               "int_trans_exp_exception");
    check_case(sim, cov, idx, EdiffIn{VPU_FCMD_NR2_FRCPFXP, VPU_DTYPE_INT, 0, false, false, false, false, 1},
               "int_nr2_frcpfxp");
    check_case(sim, cov, idx, EdiffIn{VPU_FCMD_ADD, VPU_DTYPE_F16_F32, 1u << TXFMA_MUL, false, false, false, false, 1},
               "f16_f32_mul_ph_greater");
    check_case(sim, cov, idx, EdiffIn{VPU_FCMD_ADD, VPU_DTYPE_F16_F32, 1u << TXFMA_MUL, false, false, true, false, 1},
               "f16_f32_mul_pl_greater");

    for (uint32_t order = 0; order < 8; ++order) {
        check_case(sim, cov, idx,
                   EdiffIn{VPU_FCMD_ADD, VPU_DTYPE_F16_F32, 0, (order & 4u) != 0u,
                           (order & 2u) != 0u, (order & 1u) != 0u, false, order},
                   "f16_f32_order_sweep");
    }
    check_case(sim, cov, idx, EdiffIn{VPU_FCMD_ADD, VPU_DTYPE_F16, 0, false, false, false, false, 1},
               "f16_res_exp_c");
    check_case(sim, cov, idx, EdiffIn{VPU_FCMD_ADD, VPU_DTYPE_F11, 0, false, false, false, false, 1},
               "f11_res_exp_c");
    check_case(sim, cov, idx, EdiffIn{VPU_FCMD_ADD, VPU_DTYPE_F10, 0, false, false, false, false, 1},
               "f10_res_exp_c");
    check_case(sim, cov, idx, EdiffIn{0x7f, 0xf, 0x7ffff, true, true, true, true, 0x7f},
               "all_ones_coverage");

    uint32_t seed = 0xed1ff123u;
    for (int i = 0; i < 2048; ++i) {
        check_case(sim, cov, idx, random_input(seed), "random");
    }

    cov.check(sim);
    return sim.finish();
}
