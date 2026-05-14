// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_exp_special_detect_tb.h"
#include "cosim_ctrl.h"
#include "../../../hw/ip/minion/vpu/dv/txfma_e_stage_model.h"

using DUT = Vcosim_txfma_exp_special_detect_tb;
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

void drive_and_compare(CosimCtrl<DUT>& sim, Coverage& cov, const CovIdx& idx, const SpecialIn& in) {
    drive_inputs(sim.dut.get(), in);
    observe(cov, idx, in);
    sim.dut->eval();
    const SpecialOut exp = model_special(in);
    sim.compare("ea_hi_zero", sim.dut->orig_ea_hi_zero_o, sim.dut->new_ea_hi_zero_o);
    sim.compare("eb_hi_zero", sim.dut->orig_eb_hi_zero_o, sim.dut->new_eb_hi_zero_o);
    sim.compare("ec_zero", sim.dut->orig_ec_zero_o, sim.dut->new_ec_zero_o);
    sim.compare("ea_lo_zero", sim.dut->orig_ea_lo_zero_o, sim.dut->new_ea_lo_zero_o);
    sim.compare("eb_lo_zero", sim.dut->orig_eb_lo_zero_o, sim.dut->new_eb_lo_zero_o);
    sim.compare("ea_hi_all_one", sim.dut->orig_ea_hi_all_one_o, sim.dut->new_ea_hi_all_one_o);
    sim.compare("eb_hi_all_one", sim.dut->orig_eb_hi_all_one_o, sim.dut->new_eb_hi_all_one_o);
    sim.compare("ec_all_one", sim.dut->orig_ec_all_one_o, sim.dut->new_ec_all_one_o);
    sim.compare("ea_lo_all_one", sim.dut->orig_ea_lo_all_one_o, sim.dut->new_ea_lo_all_one_o);
    sim.compare("eb_lo_all_one", sim.dut->orig_eb_lo_all_one_o, sim.dut->new_eb_lo_all_one_o);
    sim.check(sim.dut->new_ea_hi_zero_o == exp.ea_hi_zero, "model ea_hi_zero");
    sim.check(sim.dut->new_eb_hi_zero_o == exp.eb_hi_zero, "model eb_hi_zero");
    sim.check(sim.dut->new_ec_zero_o == exp.ec_zero, "model ec_zero");
    sim.check(sim.dut->new_ea_lo_zero_o == exp.ea_lo_zero, "model ea_lo_zero");
    sim.check(sim.dut->new_eb_lo_zero_o == exp.eb_lo_zero, "model eb_lo_zero");
    sim.check(sim.dut->new_ea_hi_all_one_o == exp.ea_hi_all_one, "model ea_hi_all_one");
    sim.check(sim.dut->new_eb_hi_all_one_o == exp.eb_hi_all_one, "model eb_hi_all_one");
    sim.check(sim.dut->new_ec_all_one_o == exp.ec_all_one, "model ec_all_one");
    sim.check(sim.dut->new_ea_lo_all_one_o == exp.ea_lo_all_one, "model ea_lo_all_one");
    sim.check(sim.dut->new_eb_lo_all_one_o == exp.eb_lo_all_one, "model eb_lo_all_one");
    sim.sim_time++;
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
    CosimCtrl<DUT> sim(argc, argv);
    Coverage cov;
    const CovIdx idx = add_cov(cov);
    const SpecialIn directed[] = {
        SpecialIn{0x00000000, 0x00000001, 0x00000002, VPU_DTYPE_F32, 0, VPU_FCMD_ADD, false, false},
        SpecialIn{0x3f800000, 0x40000000, 0x40400000, VPU_DTYPE_F32, 0, VPU_FCMD_ADD, false, false},
        SpecialIn{0x7f800000, 0x7f800001, 0x7fc00000, VPU_DTYPE_F32, 0, VPU_FCMD_ADD, false, false},
        SpecialIn{0x00000000, 0x7c000000, 0x00007c00, VPU_DTYPE_F16_F32, 0, VPU_FCMD_ADD, false, false},
        SpecialIn{0x04000400, 0x03ff03ff, 0x00000000, VPU_DTYPE_F16_F32, 0, VPU_FCMD_ADD, false, false},
        SpecialIn{0, 0, 0x00007c00, VPU_DTYPE_F16, 0, VPU_FCMD_ADD, false, false},
        SpecialIn{0, 0, 0x000007c0, VPU_DTYPE_F11, 0, VPU_FCMD_ADD, false, false},
        SpecialIn{0, 0, 0x000003e0, VPU_DTYPE_F10, 0, VPU_FCMD_ADD, false, false},
        SpecialIn{0x00000000, 0xffffffff, 0x00000000, VPU_DTYPE_F32, 0, VPU_TRANS_SIN_P2, false, false},
        SpecialIn{0x00000000, 0xffffffff, 0x00000000, VPU_DTYPE_F32, 0, VPU_TRANS_SIN_P3, false, false},
        SpecialIn{0x00000000, 0xffffffff, 0x00000000, VPU_DTYPE_F32, 0, VPU_TRANS_SIN_P3, false, true},
        SpecialIn{0x00000000, 0x00000000, 0x00000000, VPU_DTYPE_F32, 1u << TXFMA_TRANS2, VPU_TRANS_EXP_FMA2, false, false},
        SpecialIn{0x00000000, 0x00000000, 0x00000000, VPU_DTYPE_F32, 1u << TXFMA_TRANS2, VPU_TRANS_EXP_FMA2, true, false},
        SpecialIn{0x00000000, 0x00000000, 0x7f800000, VPU_DTYPE_F32, 0, VPU_TRANS_LOG_MUL, false, false},
        SpecialIn{0x00000000, 0x7f800000, 0x00000000, VPU_DTYPE_F32, 0, VPU_TRANS_RCP_FMA2, false, false},
        SpecialIn{0xffffffff, 0xffffffff, 0xffffffff, 0xf, 0x7ffff, 0x7f, true, true},
    };
    for (const SpecialIn& in : directed) {
        drive_and_compare(sim, cov, idx, in);
    }
    uint32_t seed = 0x5ec1a567u;
    for (int i = 0; i < 5000; ++i) {
        drive_and_compare(sim, cov, idx, random_input(seed));
    }

    cov.check(sim);
    sim.check(sim.comparisons >= 50000, "txfma_exp_special_detect should exceed 50000 comparisons");
    return sim.finish();
}
