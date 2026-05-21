// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_e5_tb.h"
#include "sim_ctrl.h"
#include "txfma_e_stage_model.h"

#include <string>

using DUT = Vtxfma_e5_tb;
using namespace txfma_e_model;

namespace {

struct CovIdx {
    int cmd, rm, op_dtype, sigs, exp_res_in, ea_hi, ec, s_res, comp_op, comp_op_true;
    int nshc, minmax_a, minmax_c, force_zero, ediff1_zap, e_sticky1, e_sticky2;
    int elxd, etrz50, use_elxd, elxd_in2_sel, fb_hi_zero, eb_hi_lsb;
};

CovIdx add_cov(Coverage& cov) {
    return CovIdx{cov.add("cmd_i", 7),
                  cov.add("rm_i", 3),
                  cov.add("op_dtype_i", 4),
                  cov.add("sigs_i", 19),
                  cov.add("exp_res_in_i", 10),
                  cov.add("ea_hi_i", 8),
                  cov.add("ec_i", 8),
                  cov.add("s_res_i", 1),
                  cov.add("comp_op_i", 1),
                  cov.add("comp_op_true_i", 1),
                  cov.add("nshc_i", 1),
                  cov.add("int_minmax_mux_sel_a_i", 1),
                  cov.add("int_minmax_mux_sel_c_i", 1),
                  cov.add("e_force_to_zero_i", 1),
                  cov.add("ediff1_zap_i", 1),
                  cov.add("e_sticky1_i", 1),
                  cov.add("e_sticky2_i", 1),
                  cov.add("elxd_i", 7),
                  cov.add("etrz50_i", 6),
                  cov.add("use_elxd_i", 1),
                  cov.add("elxd_add_in2_mux_sel_i", 1),
                  cov.add("fb_hi_zero_i", 1),
                  cov.add("eb_hi_lsb_i", 1)};
}

void drive_inputs(DUT* dut, const E5In& in) {
    dut->cmd_i = in.cmd;
    dut->rm_i = in.rm;
    dut->op_dtype_i = in.op_dtype;
    dut->sigs_i = in.sigs;
    dut->exp_res_in_i = in.exp_res_in;
    dut->ea_hi_i = in.ea_hi;
    dut->ec_i = in.ec;
    dut->s_res_i = in.s_res;
    dut->comp_op_i = in.comp_op;
    dut->comp_op_true_i = in.comp_op_true;
    dut->nshc_i = in.nshc;
    dut->int_minmax_mux_sel_a_i = in.int_minmax_mux_sel_a;
    dut->int_minmax_mux_sel_c_i = in.int_minmax_mux_sel_c;
    dut->e_force_to_zero_i = in.e_force_to_zero;
    dut->ediff1_zap_i = in.ediff1_zap;
    dut->e_sticky1_i = in.e_sticky1;
    dut->e_sticky2_i = in.e_sticky2;
    dut->elxd_i = in.elxd;
    dut->etrz50_i = in.etrz50;
    dut->use_elxd_i = in.use_elxd;
    dut->elxd_add_in2_mux_sel_i = in.elxd_add_in2_mux_sel;
    dut->fb_hi_zero_i = in.fb_hi_zero;
    dut->eb_hi_lsb_i = in.eb_hi_lsb;
}

void observe(Coverage& cov, const CovIdx& idx, const E5In& in) {
    cov.observe(idx.cmd, in.cmd);
    cov.observe(idx.rm, in.rm);
    cov.observe(idx.op_dtype, in.op_dtype);
    cov.observe(idx.sigs, in.sigs);
    cov.observe(idx.exp_res_in, in.exp_res_in);
    cov.observe(idx.ea_hi, in.ea_hi);
    cov.observe(idx.ec, in.ec);
    cov.observe(idx.s_res, in.s_res);
    cov.observe(idx.comp_op, in.comp_op);
    cov.observe(idx.comp_op_true, in.comp_op_true);
    cov.observe(idx.nshc, in.nshc);
    cov.observe(idx.minmax_a, in.int_minmax_mux_sel_a);
    cov.observe(idx.minmax_c, in.int_minmax_mux_sel_c);
    cov.observe(idx.force_zero, in.e_force_to_zero);
    cov.observe(idx.ediff1_zap, in.ediff1_zap);
    cov.observe(idx.e_sticky1, in.e_sticky1);
    cov.observe(idx.e_sticky2, in.e_sticky2);
    cov.observe(idx.elxd, in.elxd);
    cov.observe(idx.etrz50, in.etrz50);
    cov.observe(idx.use_elxd, in.use_elxd);
    cov.observe(idx.elxd_in2_sel, in.elxd_add_in2_mux_sel);
    cov.observe(idx.fb_hi_zero, in.fb_hi_zero);
    cov.observe(idx.eb_hi_lsb, in.eb_hi_lsb);
}

void check_case(SimCtrl<DUT>& sim, Coverage& cov, const CovIdx& idx, const E5In& in,
                const char* name) {
    drive_inputs(sim.dut.get(), in);
    observe(cov, idx, in);
    sim.dut->eval();
    const E5Out exp = model_e5(in);
    const std::string p = std::string(name) + ": ";
    sim.check(sim.dut->stky2g_o == exp.stky2g, p + "stky2g");
    sim.check(sim.dut->rmode_o == exp.rmode, p + "rmode");
    sim.check(sim.dut->exp_res_o == exp.exp_res, p + "exp_res");
}

E5In random_input(uint32_t& seed) {
    E5In in;
    in.cmd = xorshift32(seed) & 0x7fu;
    in.rm = xorshift32(seed) & 0x7u;
    in.op_dtype = xorshift32(seed) & 0xfu;
    in.sigs = xorshift32(seed) & 0x7ffffu;
    in.exp_res_in = xorshift32(seed) & 0x3ffu;
    in.ea_hi = xorshift32(seed) & 0xffu;
    in.ec = xorshift32(seed) & 0xffu;
    in.s_res = (xorshift32(seed) & 1u) != 0u;
    in.comp_op = (xorshift32(seed) & 1u) != 0u;
    in.comp_op_true = (xorshift32(seed) & 1u) != 0u;
    in.nshc = (xorshift32(seed) & 1u) != 0u;
    in.int_minmax_mux_sel_a = (xorshift32(seed) & 1u) != 0u;
    in.int_minmax_mux_sel_c = (xorshift32(seed) & 1u) != 0u;
    in.e_force_to_zero = (xorshift32(seed) & 1u) != 0u;
    in.ediff1_zap = (xorshift32(seed) & 1u) != 0u;
    in.e_sticky1 = (xorshift32(seed) & 1u) != 0u;
    in.e_sticky2 = (xorshift32(seed) & 1u) != 0u;
    in.elxd = xorshift32(seed) & 0x7fu;
    in.etrz50 = xorshift32(seed) & 0x3fu;
    in.use_elxd = (xorshift32(seed) & 1u) != 0u;
    in.elxd_add_in2_mux_sel = (xorshift32(seed) & 1u) != 0u;
    in.fb_hi_zero = (xorshift32(seed) & 1u) != 0u;
    in.eb_hi_lsb = (xorshift32(seed) & 1u) != 0u;
    return in;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    Coverage cov;
    const CovIdx idx = add_cov(cov);

    check_case(sim, cov, idx,
               E5In{VPU_FCMD_ADD, RNE, VPU_DTYPE_F32, 0, 0x82, 0x11, 0x22, false, false,
                    false, false, false, false, false, false, false, false, 4, 0, true,
                    false, false, false},
               "rne_use_elxd_f32");
    check_case(sim, cov, idx,
               E5In{VPU_FCMD_ADD, RUP, VPU_DTYPE_F32, 0, 0x82, 0x11, 0x22, false, false,
                    false, true, false, false, false, false, false, false, 4, 0, true,
                    false, false, false},
               "rup_positive_nshc");
    check_case(sim, cov, idx,
               E5In{VPU_FCMD_ADD, RDN, VPU_DTYPE_F32, 0, 0x82, 0x11, 0x22, true, false,
                    false, false, false, false, false, false, false, false, 4, 0, true,
                    false, false, false},
               "rdn_negative");
    check_case(sim, cov, idx,
               E5In{VPU_FCMD_ADD, RMM, VPU_DTYPE_F16_F32, 0, 0x70, 0x44, 0x55, false, false,
                    false, false, false, false, false, false, false, true, 0x7f, 0x3f, true,
                    false, false, false},
               "f16_f32_sticky2_propagates");
    check_case(sim, cov, idx,
               E5In{VPU_FCMD_ADD, RNE, VPU_DTYPE_F32, 0, 0x70, 0xaa, 0xbb, false, false,
                    false, false, false, false, true, false, false, false, 3, 1, false,
                    false, false, false},
               "force_to_zero");
    check_case(sim, cov, idx,
               E5In{VPU_FCMD_ADD, RNE, VPU_DTYPE_F32, 0, 0x70, 0xaa, 0xbb, false, false,
                    false, false, true, false, false, false, false, false, 3, 1, false,
                    false, false, false},
               "minmax_select_a");
    check_case(sim, cov, idx,
               E5In{VPU_FCMD_ADD, RNE, VPU_DTYPE_F32, 0, 0x70, 0xaa, 0xbb, false, false,
                    false, false, false, true, false, false, false, false, 3, 1, false,
                    false, false, false},
               "minmax_select_c");
    check_case(sim, cov, idx,
               E5In{VPU_FCMD_ADD, RNE, VPU_DTYPE_F32, 0, 0x70, 0xaa, 0xbb, false, true,
                    true, false, false, false, false, false, false, false, 3, 1, false,
                    false, false, false},
               "compare_true_m1");
    check_case(sim, cov, idx,
               E5In{VPU_TRANS_RCP_FMA2, RNE, VPU_DTYPE_F32, 0, 0x70, 0xaa, 0xbb, false, false,
                    false, false, false, false, false, false, false, false, 3, 1, false,
                    false, true, false},
               "rcp_fb_zero_increment");
    check_case(sim, cov, idx,
               E5In{VPU_TRANS_RSQRT_FMA2, RNE, VPU_DTYPE_F32, 0, 0x81, 0xaa, 0xbb, false,
                    false, false, false, false, false, false, false, false, false, 3, 1,
                    false, false, false, false},
               "rsqrt_compensate");
    check_case(sim, cov, idx,
               E5In{VPU_TRANS_EXP_FRAC, RNE, VPU_DTYPE_F32, 0, 0x90, 0xaa, 0x22, false,
                    false, false, false, false, false, false, false, false, false, 3, 1,
                    false, false, false, false},
               "exp_frac_uses_ec");
    check_case(sim, cov, idx,
               E5In{VPU_TRANS_EXP_FMA2, RNE, VPU_DTYPE_F32, 1u << TXFMA_TRANS2, 0x155, 0xaa,
                    0x22, false, false, false, false, false, false, false, false, false, false,
                    3, 1, false, false, false, false},
               "exp_fma2_preserves_input");
    check_case(sim, cov, idx,
               E5In{0x7f, 0x7, 0xf, 0x7ffff, 0x3ff, 0xff, 0xff, true, true, true, true, true,
                    true, true, true, true, true, 0x7f, 0x3f, true, true, true, true},
               "all_ones_coverage");

    uint32_t seed = 0xe5a51234u;
    for (int i = 0; i < 1024; ++i) {
        check_case(sim, cov, idx, random_input(seed), "random");
    }

    cov.check(sim);
    return sim.finish();
}
