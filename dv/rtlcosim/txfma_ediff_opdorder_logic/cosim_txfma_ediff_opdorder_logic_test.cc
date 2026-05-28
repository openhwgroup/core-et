// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_ediff_opdorder_logic_tb.h"
#include "cosim_ctrl.h"
#include "../../../hw/ip/minion/vpu/dv/txfma_e_stage_model.h"

using DUT = Vcosim_txfma_ediff_opdorder_logic_tb;
using namespace txfma_e_model;

namespace {

struct CovIdx { int cmd, op_dtype, sigs, sign_c_ph, sign_c_pl, sign_ph_pl, exp_fma2_exc, exp_res_2f3; };

CovIdx add_cov(Coverage& cov) {
    return CovIdx{cov.add("cmd_i", 7), cov.add("op_dtype_i", 4), cov.add("sigs_i", 19),
                  cov.add("sign_c_ph_i", 1), cov.add("sign_c_pl_i", 1), cov.add("sign_ph_pl_i", 1),
                  cov.add("exp_fma2_exc_i", 1), cov.add("exp_res_2f3_i", 7)};
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

void drive_and_compare(CosimCtrl<DUT>& sim, Coverage& cov, const CovIdx& idx, const EdiffIn& in) {
    drive_inputs(sim.dut.get(), in);
    observe(cov, idx, in);
    sim.dut->eval();
    const EdiffOut exp = model_ediff(in);
    sim.compare("align1_mux_sel", sim.dut->orig_align1_mux_sel_o, sim.dut->new_align1_mux_sel_o);
    sim.compare("align2_mux_sel", sim.dut->orig_align2_mux_sel_o, sim.dut->new_align2_mux_sel_o);
    sim.compare("opd3_mux_sel", sim.dut->orig_opd3_mux_sel_o, sim.dut->new_opd3_mux_sel_o);
    sim.compare("ediff1_sel", sim.dut->orig_ediff1_sel_o, sim.dut->new_ediff1_sel_o);
    sim.compare("ediff2_sel", sim.dut->orig_ediff2_sel_o, sim.dut->new_ediff2_sel_o);
    sim.compare("res_exp_hi_sel", sim.dut->orig_res_exp_hi_sel_o, sim.dut->new_res_exp_hi_sel_o);
    sim.compare("res_exp_lo_sel", sim.dut->orig_res_exp_lo_sel_o, sim.dut->new_res_exp_lo_sel_o);
    sim.check(sim.dut->new_align1_mux_sel_o == exp.align1_mux_sel, "model align1_mux_sel");
    sim.check(sim.dut->new_align2_mux_sel_o == exp.align2_mux_sel, "model align2_mux_sel");
    sim.check(sim.dut->new_opd3_mux_sel_o == exp.opd3_mux_sel, "model opd3_mux_sel");
    sim.check(sim.dut->new_ediff1_sel_o == exp.ediff1_sel, "model ediff1_sel");
    sim.check(sim.dut->new_ediff2_sel_o == exp.ediff2_sel, "model ediff2_sel");
    sim.check(sim.dut->new_res_exp_hi_sel_o == exp.res_exp_hi_sel, "model res_exp_hi_sel");
    sim.check(sim.dut->new_res_exp_lo_sel_o == exp.res_exp_lo_sel, "model res_exp_lo_sel");
    sim.sim_time++;
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
    CosimCtrl<DUT> sim(argc, argv);
    Coverage cov;
    const CovIdx idx = add_cov(cov);
    const EdiffIn directed[] = {
        EdiffIn{VPU_FCMD_ADD, VPU_DTYPE_F32, 0, false, false, false, false, 1},
        EdiffIn{VPU_FCMD_SUB, VPU_DTYPE_F32, 0, true, false, false, false, 1},
        EdiffIn{VPU_FCMD_FROUND, VPU_DTYPE_F32, 1u << TXFMA_CVT, false, false, false, false, 1},
        EdiffIn{VPU_TRANS_LOG_MUL, VPU_DTYPE_F32, 0, false, false, false, false, 0},
        EdiffIn{VPU_TRANS_LOG_MUL, VPU_DTYPE_F32, 0, false, false, false, false, 3},
        EdiffIn{VPU_TRANS_EXP_FRAC, VPU_DTYPE_F32, 0, false, false, false, false, 1},
        EdiffIn{VPU_TRANS_SIN_TRANSFORM, VPU_DTYPE_F32, 0, false, false, false, false, 1},
        EdiffIn{VPU_FCMD_ADD, VPU_DTYPE_F32, 1u << TXFMA_NORM_F32, false, false, false, false, 1},
        EdiffIn{VPU_FCMD_ADD, VPU_DTYPE_F32, 1u << TXFMA_FX_F32, false, false, false, false, 1},
        EdiffIn{VPU_FCMD_CVT_F32RAST, VPU_DTYPE_INT, 1u << TXFMA_CVT, false, false, false, false, 1},
        EdiffIn{VPU_TRANS_EXP_FMA2, VPU_DTYPE_INT, 1u << TXFMA_TRANS2, false, false, false, true, 1},
        EdiffIn{VPU_FCMD_NR2_FRCPFXP, VPU_DTYPE_INT, 0, false, false, false, false, 1},
        EdiffIn{VPU_FCMD_ADD, VPU_DTYPE_F16_F32, 1u << TXFMA_MUL, false, false, false, false, 1},
        EdiffIn{VPU_FCMD_ADD, VPU_DTYPE_F16_F32, 1u << TXFMA_MUL, false, false, true, false, 1},
        EdiffIn{VPU_FCMD_ADD, VPU_DTYPE_F16, 0, false, false, false, false, 1},
        EdiffIn{VPU_FCMD_ADD, VPU_DTYPE_F11, 0, false, false, false, false, 1},
        EdiffIn{VPU_FCMD_ADD, VPU_DTYPE_F10, 0, false, false, false, false, 1},
        EdiffIn{0x7f, 0xf, 0x7ffff, true, true, true, true, 0x7f},
    };
    for (const EdiffIn& in : directed) {
        drive_and_compare(sim, cov, idx, in);
    }
    for (uint32_t order = 0; order < 8; ++order) {
        drive_and_compare(sim, cov, idx,
                          EdiffIn{VPU_FCMD_ADD, VPU_DTYPE_F16_F32, 0, (order & 4u) != 0u,
                                  (order & 2u) != 0u, (order & 1u) != 0u, false, order});
    }
    uint32_t seed = 0xed1ff567u;
    for (int i = 0; i < 10000; ++i) {
        drive_and_compare(sim, cov, idx, random_input(seed));
    }

    cov.check(sim);
    sim.check(sim.comparisons >= 70000, "txfma_ediff_opdorder_logic should exceed 70000 comparisons");
    return sim.finish();
}
