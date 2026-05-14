// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_e1_tb.h"
#include "sim_ctrl.h"
#include "txfma_e_stage_model.h"

#include <string>

using DUT = Vtxfma_e1_tb;
using namespace txfma_e_model;

namespace {

struct CovIdx {
    int op_dtype;
    int sigs;
    int cmd;
    int ea_hi;
    int eb_hi;
    int ea_lo;
    int eb_lo;
    int ec;
    int ec_zero;
    int ec_all1;
    int ea_hi_zero;
    int etrz_fa_hi_ph;
    int etrz_fa_lo_ps;
    int etrz_fb_hi_ph;
    int etrz_fb_lo_ps;
    int fc_zero30_8;
};

CovIdx add_cov(Coverage& cov) {
    return CovIdx{cov.add("op_dtype_i", 4),
                  cov.add("sigs_i", 19),
                  cov.add("cmd_i", 7),
                  cov.add("ea_hi_i", 8),
                  cov.add("eb_hi_i", 8),
                  cov.add("ea_lo_i", 5),
                  cov.add("eb_lo_i", 5),
                  cov.add("ec_i", 8),
                  cov.add("ec_zero_i", 1),
                  cov.add("ec_all1_i", 1),
                  cov.add("ea_hi_zero_i", 1),
                  cov.add("etrz_fa_hi_ph_i", 4),
                  cov.add("etrz_fa_lo_ps_i", 5),
                  cov.add("etrz_fb_hi_ph_i", 4),
                  cov.add("etrz_fb_lo_ps_i", 5),
                  cov.add("fc_zero30_8_i", 1)};
}

void drive_inputs(DUT* dut, const E1In& in) {
    dut->op_dtype_i = in.op_dtype;
    dut->sigs_i = in.sigs;
    dut->cmd_i = in.cmd;
    dut->ea_hi_i = in.ea_hi;
    dut->eb_hi_i = in.eb_hi;
    dut->ea_lo_i = in.ea_lo;
    dut->eb_lo_i = in.eb_lo;
    dut->ec_i = in.ec;
    dut->ec_zero_i = in.ec_zero;
    dut->ec_all1_i = in.ec_all1;
    dut->ea_hi_zero_i = in.ea_hi_zero;
    dut->etrz_fa_hi_ph_i = in.etrz_fa_hi_ph;
    dut->etrz_fa_lo_ps_i = in.etrz_fa_lo_ps;
    dut->etrz_fb_hi_ph_i = in.etrz_fb_hi_ph;
    dut->etrz_fb_lo_ps_i = in.etrz_fb_lo_ps;
    dut->fc_zero30_8_i = in.fc_zero30_8;
}

void observe(Coverage& cov, const CovIdx& idx, const E1In& in) {
    cov.observe(idx.op_dtype, in.op_dtype);
    cov.observe(idx.sigs, in.sigs);
    cov.observe(idx.cmd, in.cmd);
    cov.observe(idx.ea_hi, in.ea_hi);
    cov.observe(idx.eb_hi, in.eb_hi);
    cov.observe(idx.ea_lo, in.ea_lo);
    cov.observe(idx.eb_lo, in.eb_lo);
    cov.observe(idx.ec, in.ec);
    cov.observe(idx.ec_zero, in.ec_zero);
    cov.observe(idx.ec_all1, in.ec_all1);
    cov.observe(idx.ea_hi_zero, in.ea_hi_zero);
    cov.observe(idx.etrz_fa_hi_ph, in.etrz_fa_hi_ph);
    cov.observe(idx.etrz_fa_lo_ps, in.etrz_fa_lo_ps);
    cov.observe(idx.etrz_fb_hi_ph, in.etrz_fb_hi_ph);
    cov.observe(idx.etrz_fb_lo_ps, in.etrz_fb_lo_ps);
    cov.observe(idx.fc_zero30_8, in.fc_zero30_8);
}

void check_case(SimCtrl<DUT>& sim, Coverage& cov, const CovIdx& idx, const E1In& in,
                const char* name) {
    drive_inputs(sim.dut.get(), in);
    observe(cov, idx, in);
    sim.dut->eval();
    const E1Out exp = model_e1(in);
    const std::string p = std::string(name) + ": ";
    sim.check(sim.dut->etrz_prod_hi_ph_o == exp.etrz_prod_hi_ph, p + "etrz_prod_hi_ph");
    sim.check(sim.dut->etrz_prod_lo_ps_o == exp.etrz_prod_lo_ps, p + "etrz_prod_lo_ps");
    sim.check(sim.dut->ediff_ec_eph_o == exp.ediff_ec_eph, p + "ediff_ec_eph");
    sim.check(sim.dut->ediff_ec_epl_o == exp.ediff_ec_epl, p + "ediff_ec_epl");
    sim.check(sim.dut->ediff_eph_epl_o == exp.ediff_eph_epl, p + "ediff_eph_epl");
    sim.check(sim.dut->eprod_hi_o == exp.eprod_hi, p + "eprod_hi");
    sim.check(sim.dut->eprod_lo_o == exp.eprod_lo, p + "eprod_lo");
    sim.check(sim.dut->ec_out_o == exp.ec_out, p + "ec_out");
}

E1In random_input(uint32_t& seed) {
    E1In in;
    in.op_dtype = xorshift32(seed) & 0xfu;
    in.sigs = xorshift32(seed) & 0x7ffffu;
    in.cmd = xorshift32(seed) & 0x7fu;
    in.ea_hi = xorshift32(seed) & 0xffu;
    in.eb_hi = xorshift32(seed) & 0xffu;
    in.ea_lo = xorshift32(seed) & 0x1fu;
    in.eb_lo = xorshift32(seed) & 0x1fu;
    in.ec = xorshift32(seed) & 0xffu;
    in.ec_zero = (xorshift32(seed) & 1u) != 0u;
    in.ec_all1 = (xorshift32(seed) & 1u) != 0u;
    in.ea_hi_zero = (xorshift32(seed) & 1u) != 0u;
    in.etrz_fa_hi_ph = xorshift32(seed) & 0xfu;
    in.etrz_fa_lo_ps = xorshift32(seed) & 0x1fu;
    in.etrz_fb_hi_ph = xorshift32(seed) & 0xfu;
    in.etrz_fb_lo_ps = xorshift32(seed) & 0x1fu;
    in.fc_zero30_8 = (xorshift32(seed) & 1u) != 0u;
    return in;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    Coverage cov;
    const CovIdx idx = add_cov(cov);

    check_case(sim, cov, idx,
               E1In{VPU_DTYPE_F32, 0, VPU_FCMD_ADD, 130, 4, 2, 3, 127, false, false, false,
                    1, 2, 3, 4, true},
               "f32_add_bias_cancel");
    check_case(sim, cov, idx,
               E1In{VPU_DTYPE_F16, 0, VPU_FCMD_ADD, 0x0f, 0x10, 0x1, 0x1e, 0x0f, false, false,
                    false, 15, 31, 1, 2, true},
               "f16_expansion");
    check_case(sim, cov, idx,
               E1In{VPU_DTYPE_F16, 0, VPU_FCMD_ADD, 0, 0, 0, 0, 0, true, false, true,
                    0, 0, 0, 0, true},
               "half_ec_zero_forces_zero");
    check_case(sim, cov, idx,
               E1In{VPU_DTYPE_F16, 0, VPU_FCMD_ADD, 0x1f, 0x1f, 0, 0, 0x1f, false, true,
                    false, 2, 3, 4, 5, true},
               "half_ec_all1_forces_ff");
    check_case(sim, cov, idx,
               E1In{VPU_DTYPE_INT, 1u << TXFMA_CVT, VPU_FCMD_CVT_INTF32, 1, 2, 3, 4, 5, false,
                    false, false, 7, 8, 9, 10, true},
               "cvt_intf32_offset");
    check_case(sim, cov, idx,
               E1In{VPU_DTYPE_F32, 0, VPU_TRANS_RCP_FMA2, 0x40, 0x55, 0, 0, 0x7f, false, false,
                    false, 1, 2, 3, 4, true},
               "trans_rcp_complement_b");
    check_case(sim, cov, idx,
               E1In{VPU_DTYPE_F32, 0, VPU_TRANS_RSQRT_FMA2, 0x40, 0x80, 0, 0, 0x7f, false, false,
                    false, 1, 2, 3, 4, true},
               "trans_rsqrt_allones_a");
    check_case(sim, cov, idx,
               E1In{VPU_DTYPE_F32, 1u << TXFMA_TRANS2, VPU_TRANS_EXP_FMA2, 2, 0xfe, 1, 1, 0x80,
                    false, false, false, 1, 2, 3, 4, true},
               "trans_exp_fma2_signext_b");
    check_case(sim, cov, idx,
               E1In{VPU_DTYPE_F32, 0, VPU_TRANS_LOG_MUL, 3, 4, 5, 6, 0x40, false, false, false,
                    1, 2, 3, 4, false},
               "log_mul_negative_ec_incdec");
    check_case(sim, cov, idx,
               E1In{VPU_DTYPE_F32, 0, VPU_TRANS_LOG_MUL, 3, 4, 5, 6, 0x90, false, false, false,
                    1, 2, 3, 4, true},
               "log_mul_positive_ec_incdec");
    check_case(sim, cov, idx,
               E1In{VPU_DTYPE_F16_F32, 1u << TXFMA_NORM_F32, VPU_TRANS_EXP_FRAC, 1, 2, 3, 4, 5,
                    false, false, false, 1, 2, 3, 4, true},
               "norm_f32_ec_and_exp_frac_lo");
    check_case(sim, cov, idx,
               E1In{0xf, 0x7ffff, 0x7f, 0xff, 0xff, 0x1f, 0x1f, 0xff, true, true, true,
                    0xf, 0x1f, 0xf, 0x1f, true},
               "all_ones_coverage");

    uint32_t seed = 0x1eaf1234u;
    for (int i = 0; i < 512; ++i) {
        check_case(sim, cov, idx, random_input(seed), "random");
    }

    cov.check(sim);
    return sim.finish();
}
