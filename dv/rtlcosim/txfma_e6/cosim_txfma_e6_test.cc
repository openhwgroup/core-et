// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfma_e6_tb.h"
#include "cosim_ctrl.h"
#include "../../../hw/ip/minion/vpu/dv/txfma_e_stage_model.h"

using DUT = Vcosim_txfma_e6_tb;
using namespace txfma_e_model;

namespace {

struct CovIdx { int exp_res_in, ps_rndovf, ph_rndovf, cmd, sigs, op_dtype; };

CovIdx add_cov(Coverage& cov) {
    return CovIdx{cov.add("exp_res_in_i", 10), cov.add("ps_rndovf_i", 1),
                  cov.add("ph_rndovf_i", 1), cov.add("cmd_i", 7),
                  cov.add("sigs_i", 19), cov.add("op_dtype_i", 4)};
}

void drive_inputs(DUT* dut, const E6In& in) {
    dut->exp_res_in_i = in.exp_res_in;
    dut->ps_rndovf_i = in.ps_rndovf;
    dut->ph_rndovf_i = in.ph_rndovf;
    dut->cmd_i = in.cmd;
    dut->sigs_i = in.sigs;
    dut->op_dtype_i = in.op_dtype;
}

void observe(Coverage& cov, const CovIdx& idx, const E6In& in) {
    cov.observe(idx.exp_res_in, in.exp_res_in);
    cov.observe(idx.ps_rndovf, in.ps_rndovf);
    cov.observe(idx.ph_rndovf, in.ph_rndovf);
    cov.observe(idx.cmd, in.cmd);
    cov.observe(idx.sigs, in.sigs);
    cov.observe(idx.op_dtype, in.op_dtype);
}

void drive_and_compare(CosimCtrl<DUT>& sim, Coverage& cov, const CovIdx& idx, const E6In& in) {
    drive_inputs(sim.dut.get(), in);
    observe(cov, idx, in);
    sim.dut->eval();
    const E6Out exp = model_e6(in);
    sim.compare("def_ovf_ps", sim.dut->orig_def_ovf_ps_o, sim.dut->new_def_ovf_ps_o);
    sim.compare("pot_ovf_ps", sim.dut->orig_pot_ovf_ps_o, sim.dut->new_pot_ovf_ps_o);
    sim.compare("def_unf_ps", sim.dut->orig_def_unf_ps_o, sim.dut->new_def_unf_ps_o);
    sim.compare("pot_unf_ps", sim.dut->orig_pot_unf_ps_o, sim.dut->new_pot_unf_ps_o);
    sim.compare("def_ovf_ph", sim.dut->orig_def_ovf_ph_o, sim.dut->new_def_ovf_ph_o);
    sim.compare("pot_ovf_ph", sim.dut->orig_pot_ovf_ph_o, sim.dut->new_pot_ovf_ph_o);
    sim.compare("def_unf_ph", sim.dut->orig_def_unf_ph_o, sim.dut->new_def_unf_ph_o);
    sim.compare("pot_unf_ph", sim.dut->orig_pot_unf_ph_o, sim.dut->new_pot_unf_ph_o);
    sim.compare("exp_res", sim.dut->orig_exp_res_o, sim.dut->new_exp_res_o);
    sim.compare("exp_res_zero", sim.dut->orig_exp_res_zero_o, sim.dut->new_exp_res_zero_o);
    sim.check(sim.dut->new_def_ovf_ps_o == exp.def_ovf_ps, "model def_ovf_ps");
    sim.check(sim.dut->new_pot_ovf_ps_o == exp.pot_ovf_ps, "model pot_ovf_ps");
    sim.check(sim.dut->new_def_unf_ps_o == exp.def_unf_ps, "model def_unf_ps");
    sim.check(sim.dut->new_pot_unf_ps_o == exp.pot_unf_ps, "model pot_unf_ps");
    sim.check(sim.dut->new_def_ovf_ph_o == exp.def_ovf_ph, "model def_ovf_ph");
    sim.check(sim.dut->new_pot_ovf_ph_o == exp.pot_ovf_ph, "model pot_ovf_ph");
    sim.check(sim.dut->new_def_unf_ph_o == exp.def_unf_ph, "model def_unf_ph");
    sim.check(sim.dut->new_pot_unf_ph_o == exp.pot_unf_ph, "model pot_unf_ph");
    sim.check(sim.dut->new_exp_res_o == exp.exp_res, "model exp_res");
    sim.check(sim.dut->new_exp_res_zero_o == exp.exp_res_zero, "model exp_res_zero");
    sim.sim_time++;
}

E6In random_input(uint32_t& seed) {
    E6In in;
    in.exp_res_in = xorshift32(seed) & 0x3ffu;
    in.ps_rndovf = (xorshift32(seed) & 1u) != 0u;
    in.ph_rndovf = (xorshift32(seed) & 1u) != 0u;
    in.cmd = xorshift32(seed) & 0x7fu;
    in.sigs = xorshift32(seed) & 0x7ffffu;
    in.op_dtype = xorshift32(seed) & 0xfu;
    return in;
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    Coverage cov;
    const CovIdx idx = add_cov(cov);
    const E6In directed[] = {
        E6In{0x0fe, false, false, VPU_FCMD_ADD, 0, VPU_DTYPE_F32},
        E6In{0x0ff, false, false, VPU_FCMD_ADD, 0, VPU_DTYPE_F32},
        E6In{0x000, false, false, VPU_FCMD_ADD, 0, VPU_DTYPE_F32},
        E6In{0x3ff, false, false, VPU_FCMD_ADD, 0, VPU_DTYPE_F32},
        E6In{0x0fe, false, false, VPU_FCMD_ADD, 1u << TXFMA_MUL, VPU_DTYPE_F16_F32},
        E6In{FLOAT16_32_MAX, false, false, VPU_FCMD_ADD, 0, VPU_DTYPE_F16},
        E6In{FLOAT16_32_MAX + 1, false, false, VPU_FCMD_ADD, 0, VPU_DTYPE_F16},
        E6In{TXFMA_BIAS_DIFF, false, false, VPU_FCMD_ADD, 0, VPU_DTYPE_F16},
        E6In{TXFMA_BIAS_DIFF - 1, false, false, VPU_FCMD_ADD, 0, VPU_DTYPE_F16},
        E6In{0x07f, true, false, VPU_FCMD_ADD, 0, VPU_DTYPE_F32},
        E6In{0x07f, false, true, VPU_FCMD_ADD, 0, VPU_DTYPE_F16},
        E6In{0x081, false, false, VPU_TRANS_RSQRT_FMA2, 0, VPU_DTYPE_F32},
        E6In{0x3ff, true, true, 0x7f, 0x7ffff, 0xf},
    };
    for (const E6In& in : directed) {
        drive_and_compare(sim, cov, idx, in);
    }
    uint32_t seed = 0xe6a65678u;
    for (int i = 0; i < 5000; ++i) {
        drive_and_compare(sim, cov, idx, random_input(seed));
    }

    cov.check(sim);
    sim.check(sim.comparisons >= 50000, "txfma_e6 should exceed 50000 comparisons");
    return sim.finish();
}
