// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_f3_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vtxfma_f3_tb;

namespace {

void clear_inputs(SimCtrl<DUT>& sim) {
    sim.dut->ctrl_f2_clk = 0;
    sim.dut->ctrl_f3_clk = 0;
    sim.dut->fa_f3a_h = 0;
    sim.dut->fc_f3a_h = 0;
    sim.dut->prodsum_f3a_h = 0;
    sim.dut->exp_res_f2a_h = 0;
    sim.dut->ediff_abs_diff_hi_f2a_h = 0;
    sim.dut->align2_rshamt_c3_f2a_h = 0;
    sim.dut->op3p3spc_sel_f2a_h = 0;
    sim.dut->op3p2c_sel_f2a_h = 0;
    sim.dut->imxc_sel_f3a_h = 0;
    sim.dut->ugfxrplc_sel_f2a_h = 0;
    sim.dut->sgfxrplc_sel_f2a_h = 0;
    sim.dut->icgfxc_sel_f2a_h = 0;
    sim.dut->al1znp2c_sel_f2a_h = 0;
    sim.dut->al2c_sel_f2a_h = 0;
    sim.dut->al1p2c_sel_f2a_h = 0;
    sim.dut->al1p1c_sel_f2a_h = 0;
    sim.dut->op3p1c_sel_r_f2a_h = 0;
    sim.dut->op3p1c_sel_p_f2a_h = 0;
    sim.dut->op3g_sel_f2a_h = 0;
    sim.dut->op2_sxtto_g_f2a_h = 0;
    sim.dut->rm_rne_f2a_h = 0;
    sim.dut->rm_rmm_f2a_h = 0;
    sim.dut->frnd_rupnsc_f2a_h = 0;
    sim.dut->frnd_rdnsc_f2a_h = 0;
    sim.dut->frnd_t1_f2a_h = 0;
    sim.dut->frnd_t2_f2a_h = 0;
    sim.dut->al1c_zap_f2a_h = 0;
    sim.dut->op1c_log_mul_clr_f2a_h = 0;
    sim.dut->negop1c_int_logmul_clr_f2a_h = 0;
    sim.dut->psc_op1_clr_f2a_h = 0;
    sim.dut->align2_zap_f2a_h = 0;
    sim.dut->align2c_neg_f2a_h = 0;
    sim.dut->al2c3_neg_cin_ps_f3a_h = 0;
}

void step_all_clocks(SimCtrl<DUT>& sim) {
    sim.dut->ctrl_f2_clk = 0;
    sim.dut->ctrl_f3_clk = 0;
    sim.dut->eval();
    sim.sim_time++;
    sim.dut->ctrl_f2_clk = 1;
    sim.dut->ctrl_f3_clk = 1;
    sim.dut->eval();
    sim.sim_time++;
    sim.dut->ctrl_f2_clk = 0;
    sim.dut->ctrl_f3_clk = 0;
    sim.dut->eval();
    sim.sim_time++;
}

void check_int_mux(SimCtrl<DUT>& sim, uint8_t sel, uint32_t expected, const char* name) {
    sim.dut->imxc_sel_f3a_h = sel;
    step_all_clocks(sim);
    sim.check(sim.dut->int_minmax_res_f4a_h == expected, name);
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    clear_inputs(sim);
    step_all_clocks(sim);
    sim.check(sim.dut->comb_s_f4a_h == 0, "zero_comb_sum");
    sim.check(sim.dut->comb_c_f4a_h == 0, "zero_comb_carry");
    sim.check(sim.dut->int_minmax_res_f4a_h == 0, "zero_int_minmax");
    sim.check(sim.dut->fround_rbit_f3a_h == 0, "zero_fround_rbit");

    const uint32_t fa = 0x11112222u;
    const uint32_t fc = 0x33334444u;
    const uint64_t prodsum = 0xcafebabe87654321ull;
    sim.dut->fa_f3a_h = fa;
    sim.dut->fc_f3a_h = fc;
    sim.dut->prodsum_f3a_h = prodsum;

    check_int_mux(sim, 0, fa, "int_minmax_select_fa");
    check_int_mux(sim, 1, fc, "int_minmax_select_fc");
    check_int_mux(sim, 2, static_cast<uint32_t>(prodsum), "int_minmax_select_prodsum_low32");
    check_int_mux(sim, 3, static_cast<uint32_t>(prodsum >> 32), "int_minmax_select_prodsum_high32");

    sim.dut->prodsum_f3a_h = 1ull << 62;
    sim.dut->op2_sxtto_g_f2a_h = 1;
    step_all_clocks(sim);
    sim.check(sim.dut->prodsum_a2_f3a_h == 1, "prodsum_a2_immediate_bit");
    sim.check(sim.dut->op2_sxtto_g_f3a_h == 1, "op2_sxtto_g_control_flop");

    clear_inputs(sim);
    sim.dut->fa_f3a_h = 0x00ff00ffu;
    sim.dut->fc_f3a_h = 0x0f0f0f0fu;
    sim.dut->prodsum_f3a_h = 0;
    sim.dut->al1c_zap_f2a_h = 1;
    sim.dut->align2_zap_f2a_h = 1;
    step_all_clocks(sim);  // load local F3 controls
    step_all_clocks(sim);  // observe zapped CSA result at F4 flops
    sim.check(sim.dut->comb_s_f4a_h == 0, "zapped_operands_keep_csa_sum_zero");
    sim.check(sim.dut->comb_c_f4a_h == 0, "zapped_operands_keep_csa_carry_zero");

    return sim.finish();
}
