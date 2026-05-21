// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_e6_tb.h"
#include "sim_ctrl.h"
#include "txfma_e_stage_model.h"

#include <string>

using DUT = Vtxfma_e6_tb;
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

void check_case(SimCtrl<DUT>& sim, Coverage& cov, const CovIdx& idx, const E6In& in,
                const char* name) {
    drive_inputs(sim.dut.get(), in);
    observe(cov, idx, in);
    sim.dut->eval();
    const E6Out exp = model_e6(in);
    const std::string p = std::string(name) + ": ";
    sim.check(sim.dut->def_ovf_ps_o == exp.def_ovf_ps, p + "def_ovf_ps");
    sim.check(sim.dut->pot_ovf_ps_o == exp.pot_ovf_ps, p + "pot_ovf_ps");
    sim.check(sim.dut->def_unf_ps_o == exp.def_unf_ps, p + "def_unf_ps");
    sim.check(sim.dut->pot_unf_ps_o == exp.pot_unf_ps, p + "pot_unf_ps");
    sim.check(sim.dut->def_ovf_ph_o == exp.def_ovf_ph, p + "def_ovf_ph");
    sim.check(sim.dut->pot_ovf_ph_o == exp.pot_ovf_ph, p + "pot_ovf_ph");
    sim.check(sim.dut->def_unf_ph_o == exp.def_unf_ph, p + "def_unf_ph");
    sim.check(sim.dut->pot_unf_ph_o == exp.pot_unf_ph, p + "pot_unf_ph");
    sim.check(sim.dut->exp_res_o == exp.exp_res, p + "exp_res");
    sim.check(sim.dut->exp_res_zero_o == exp.exp_res_zero, p + "exp_res_zero");
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
    SimCtrl<DUT> sim(argc, argv);
    Coverage cov;
    const CovIdx idx = add_cov(cov);

    check_case(sim, cov, idx, E6In{0x0fe, false, false, VPU_FCMD_ADD, 0, VPU_DTYPE_F32},
               "f32_potential_overflow");
    check_case(sim, cov, idx, E6In{0x0ff, false, false, VPU_FCMD_ADD, 0, VPU_DTYPE_F32},
               "f32_definite_overflow");
    check_case(sim, cov, idx, E6In{0x000, false, false, VPU_FCMD_ADD, 0, VPU_DTYPE_F32},
               "f32_potential_underflow_zero_result");
    check_case(sim, cov, idx, E6In{0x3ff, false, false, VPU_FCMD_ADD, 0, VPU_DTYPE_F32},
               "f32_definite_underflow_negative_exp");
    check_case(sim, cov, idx,
               E6In{0x0fe, false, false, VPU_FCMD_ADD, 1u << TXFMA_MUL, VPU_DTYPE_F16_F32},
               "f16_f32_mul_suppresses_ps_flags");
    check_case(sim, cov, idx, E6In{FLOAT16_32_MAX, false, false, VPU_FCMD_ADD, 0, VPU_DTYPE_F16},
               "ph_potential_overflow");
    check_case(sim, cov, idx, E6In{FLOAT16_32_MAX + 1, false, false, VPU_FCMD_ADD, 0, VPU_DTYPE_F16},
               "ph_definite_overflow");
    check_case(sim, cov, idx, E6In{TXFMA_BIAS_DIFF, false, false, VPU_FCMD_ADD, 0, VPU_DTYPE_F16},
               "ph_potential_underflow");
    check_case(sim, cov, idx, E6In{TXFMA_BIAS_DIFF - 1, false, false, VPU_FCMD_ADD, 0, VPU_DTYPE_F16},
               "ph_definite_underflow");
    check_case(sim, cov, idx, E6In{0x07f, true, false, VPU_FCMD_ADD, 0, VPU_DTYPE_F32},
               "ps_round_overflow_increments");
    check_case(sim, cov, idx, E6In{0x07f, false, true, VPU_FCMD_ADD, 0, VPU_DTYPE_F16},
               "ph_round_overflow_increments");
    check_case(sim, cov, idx, E6In{0x081, false, false, VPU_TRANS_RSQRT_FMA2, 0, VPU_DTYPE_F32},
               "rsqrt_decrements_by_one");
    check_case(sim, cov, idx, E6In{0x3ff, true, true, 0x7f, 0x7ffff, 0xf},
               "all_ones_coverage");

    uint32_t seed = 0xe6a61234u;
    for (int i = 0; i < 1024; ++i) {
        check_case(sim, cov, idx, random_input(seed), "random");
    }

    cov.check(sim);
    return sim.finish();
}
