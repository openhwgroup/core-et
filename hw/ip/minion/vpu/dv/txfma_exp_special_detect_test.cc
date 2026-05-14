// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_exp_special_detect_tb.h"
#include "sim_ctrl.h"
#include "txfma_e_stage_model.h"

#include <string>

using DUT = Vtxfma_exp_special_detect_tb;
using namespace txfma_e_model;

namespace {

struct CovIdx { int opa, opb, opc, op_dtype, sigs, cmd, trans_exp_fma2_exc, trans_taylor; };

CovIdx add_cov(Coverage& cov) {
    return CovIdx{cov.add("opa_i", 32), cov.add("opb_i", 32), cov.add("opc_i", 32),
                  cov.add("op_dtype_i", 4), cov.add("sigs_i", 19), cov.add("cmd_i", 7),
                  cov.add("trans_exp_fma2_exc_i", 1), cov.add("trans_taylor_i", 1)};
}

void drive_inputs(DUT* dut, const SpecialIn& in) {
    dut->opa_i = in.opa;
    dut->opb_i = in.opb;
    dut->opc_i = in.opc;
    dut->op_dtype_i = in.op_dtype;
    dut->sigs_i = in.sigs;
    dut->cmd_i = in.cmd;
    dut->trans_exp_fma2_exc_i = in.trans_exp_fma2_exc;
    dut->trans_taylor_i = in.trans_taylor;
}

void observe(Coverage& cov, const CovIdx& idx, const SpecialIn& in) {
    cov.observe(idx.opa, in.opa);
    cov.observe(idx.opb, in.opb);
    cov.observe(idx.opc, in.opc);
    cov.observe(idx.op_dtype, in.op_dtype);
    cov.observe(idx.sigs, in.sigs);
    cov.observe(idx.cmd, in.cmd);
    cov.observe(idx.trans_exp_fma2_exc, in.trans_exp_fma2_exc);
    cov.observe(idx.trans_taylor, in.trans_taylor);
}

void check_case(SimCtrl<DUT>& sim, Coverage& cov, const CovIdx& idx, const SpecialIn& in,
                const char* name) {
    drive_inputs(sim.dut.get(), in);
    observe(cov, idx, in);
    sim.dut->eval();
    const SpecialOut exp = model_special(in);
    const std::string p = std::string(name) + ": ";
    sim.check(sim.dut->ea_hi_zero_o == exp.ea_hi_zero, p + "ea_hi_zero");
    sim.check(sim.dut->eb_hi_zero_o == exp.eb_hi_zero, p + "eb_hi_zero");
    sim.check(sim.dut->ec_zero_o == exp.ec_zero, p + "ec_zero");
    sim.check(sim.dut->ea_lo_zero_o == exp.ea_lo_zero, p + "ea_lo_zero");
    sim.check(sim.dut->eb_lo_zero_o == exp.eb_lo_zero, p + "eb_lo_zero");
    sim.check(sim.dut->ea_hi_all_one_o == exp.ea_hi_all_one, p + "ea_hi_all_one");
    sim.check(sim.dut->eb_hi_all_one_o == exp.eb_hi_all_one, p + "eb_hi_all_one");
    sim.check(sim.dut->ec_all_one_o == exp.ec_all_one, p + "ec_all_one");
    sim.check(sim.dut->ea_lo_all_one_o == exp.ea_lo_all_one, p + "ea_lo_all_one");
    sim.check(sim.dut->eb_lo_all_one_o == exp.eb_lo_all_one, p + "eb_lo_all_one");
}

SpecialIn random_input(uint32_t& seed) {
    SpecialIn in;
    in.opa = xorshift32(seed);
    in.opb = xorshift32(seed);
    in.opc = xorshift32(seed);
    in.op_dtype = xorshift32(seed) & 0xfu;
    in.sigs = xorshift32(seed) & 0x7ffffu;
    in.cmd = xorshift32(seed) & 0x7fu;
    in.trans_exp_fma2_exc = (xorshift32(seed) & 1u) != 0u;
    in.trans_taylor = (xorshift32(seed) & 1u) != 0u;
    return in;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    Coverage cov;
    const CovIdx idx = add_cov(cov);

    check_case(sim, cov, idx, SpecialIn{0x00000000, 0x00000001, 0x00000002, VPU_DTYPE_F32, 0,
                                        VPU_FCMD_ADD, false, false},
               "f32_zero_denorm_inputs");
    check_case(sim, cov, idx, SpecialIn{0x3f800000, 0x40000000, 0x40400000, VPU_DTYPE_F32, 0,
                                        VPU_FCMD_ADD, false, false},
               "f32_normal_inputs");
    check_case(sim, cov, idx, SpecialIn{0x7f800000, 0x7f800001, 0x7fc00000, VPU_DTYPE_F32, 0,
                                        VPU_FCMD_ADD, false, false},
               "f32_inf_nan_like_all_one_exp");
    check_case(sim, cov, idx, SpecialIn{0x00000000, 0x7c000000, 0x00007c00, VPU_DTYPE_F16_F32, 0,
                                        VPU_FCMD_ADD, false, false},
               "packed_half_zero_inf");
    check_case(sim, cov, idx, SpecialIn{0x04000400, 0x03ff03ff, 0x00000000, VPU_DTYPE_F16_F32, 0,
                                        VPU_FCMD_ADD, false, false},
               "packed_half_subnormal_normal");
    check_case(sim, cov, idx, SpecialIn{0, 0, 0x00007c00, VPU_DTYPE_F16, 0, VPU_FCMD_ADD, false,
                                        false},
               "f16_opc_all_one");
    check_case(sim, cov, idx, SpecialIn{0, 0, 0x000007c0, VPU_DTYPE_F11, 0, VPU_FCMD_ADD, false,
                                        false},
               "f11_opc_all_one");
    check_case(sim, cov, idx, SpecialIn{0, 0, 0x000003e0, VPU_DTYPE_F10, 0, VPU_FCMD_ADD, false,
                                        false},
               "f10_opc_all_one");
    check_case(sim, cov, idx,
               SpecialIn{0x00000000, 0xffffffff, 0x00000000, VPU_DTYPE_F32, 0,
                         VPU_TRANS_SIN_P2, false, false},
               "sin_p2_rom_clears_zero_detection");
    check_case(sim, cov, idx,
               SpecialIn{0x00000000, 0xffffffff, 0x00000000, VPU_DTYPE_F32, 0,
                         VPU_TRANS_SIN_P3, false, false},
               "sin_p3_non_taylor_clears_a_zero");
    check_case(sim, cov, idx,
               SpecialIn{0x00000000, 0xffffffff, 0x00000000, VPU_DTYPE_F32, 0,
                         VPU_TRANS_SIN_P3, false, true},
               "sin_p3_taylor_allows_a_zero");
    check_case(sim, cov, idx,
               SpecialIn{0x00000000, 0x00000000, 0x00000000, VPU_DTYPE_F32, 1u << TXFMA_TRANS2,
                         VPU_TRANS_EXP_FMA2, false, false},
               "trans2_exp_fma2_no_exception_keeps_zero_path_off");
    check_case(sim, cov, idx,
               SpecialIn{0x00000000, 0x00000000, 0x00000000, VPU_DTYPE_F32, 1u << TXFMA_TRANS2,
                         VPU_TRANS_EXP_FMA2, true, false},
               "trans2_exp_fma2_exception_clears_a_c_zero");
    check_case(sim, cov, idx,
               SpecialIn{0x00000000, 0x00000000, 0x7f800000, VPU_DTYPE_F32, 0,
                         VPU_TRANS_LOG_MUL, false, false},
               "log_mul_clears_a_b_zero");
    check_case(sim, cov, idx,
               SpecialIn{0x00000000, 0x7f800000, 0x00000000, VPU_DTYPE_F32, 0,
                         VPU_TRANS_RCP_FMA2, false, false},
               "rcp_detects_b_inf");
    check_case(sim, cov, idx,
               SpecialIn{0xffffffff, 0xffffffff, 0xffffffff, 0xf, 0x7ffff, 0x7f, true, true},
               "all_ones_coverage");

    uint32_t seed = 0x5ec1a123u;
    for (int i = 0; i < 1024; ++i) {
        check_case(sim, cov, idx, random_input(seed), "random");
    }

    cov.check(sim);
    return sim.finish();
}
