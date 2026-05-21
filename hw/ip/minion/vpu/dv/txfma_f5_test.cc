// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_f5_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vtxfma_f5_tb;

namespace {

constexpr uint64_t kMask33 = (1ull << 33) - 1ull;

void clear_inputs(SimCtrl<DUT>& sim) {
    sim.dut->ctrl_f4_clk = 0;
    sim.dut->ctrl_f5_clk = 0;
    sim.dut->comb_add_f5a_h = 0;
    sim.dut->int_minmax_res_f5a_h = 0;
    sim.dut->enc_nsamt_f4a_h = 0;
    sim.dut->neg_norm_f5a_h = 0;
    sim.dut->en_logrrsel1b_f5a_l = 0;
    sim.dut->mmx_sel_immres_f5a_h = 0;
    sim.dut->nshcen_sel1b_f5a_h = 0;
    sim.dut->rc_ps_f5a_h = 0;
    sim.dut->rc_ph_f5a_h = 0;
    sim.dut->rc_int32_f5a_h = 0;
    sim.dut->int32_stky2r_f5a_h = 0;
    sim.dut->stky2g_f5a_h = 0;
    sim.dut->rc_3rmxr_f5a_h = 0;
    sim.dut->rc_rl01_f5a_h = 0;
    sim.dut->rc_rmm_f5a_h = 0;
    sim.dut->rc_rup_f5a_h = 0;
    sim.dut->rc_rne_f5a_h = 0;
    sim.dut->rc_rdn_f5a_h = 0;
    sim.dut->rc_res_sign_f5a_l = 0;
    sim.dut->rc_transrnd_f5a_h = 0;
    sim.dut->rc_transrnd_log_fma2_f5a_h = 0;
}

void step_all_clocks(SimCtrl<DUT>& sim) {
    sim.dut->ctrl_f4_clk = 0;
    sim.dut->ctrl_f5_clk = 0;
    sim.dut->eval();
    sim.sim_time++;
    sim.dut->ctrl_f4_clk = 1;
    sim.dut->ctrl_f5_clk = 1;
    sim.dut->eval();
    sim.sim_time++;
    sim.dut->ctrl_f4_clk = 0;
    sim.dut->ctrl_f5_clk = 0;
    sim.dut->eval();
    sim.sim_time++;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    clear_inputs(sim);
    step_all_clocks(sim);
    sim.check(sim.dut->mergd_frac_res_hi_f6a_h == 0, "zero_merged_fraction");
    sim.check(sim.dut->frac_zero_f6a_h == 1, "zero_fraction_detected");
    sim.check(sim.dut->ftz_f6a_l == 3, "default_ftz_not_forced");
    sim.check(sim.dut->comb_add_a3_f5a_h == 0, "zero_comb_add_a3");

    clear_inputs(sim);
    sim.dut->mmx_sel_immres_f5a_h = 1;
    sim.dut->int_minmax_res_f5a_h = 0xffffffffu;
    sim.dut->stky2g_f5a_h = 1;
    sim.dut->int32_stky2r_f5a_h = 1;
    sim.dut->rc_transrnd_f5a_h = 1;
    sim.dut->rc_transrnd_log_fma2_f5a_h = 1;
    step_all_clocks(sim);
    sim.check(sim.dut->mergd_frac_res_hi_f6a_h == ((0xffffffffull << 1) & kMask33),
              "immediate_result_merges_int_minmax_with_zero_lsb");
    sim.check(sim.dut->frac_zero_f6a_h == 0, "nonzero_immediate_fraction_not_zero");
    sim.check(sim.dut->ph_allone2l_f5a_h == 1 && sim.dut->ph_allone2r_f5a_h == 1,
              "ph_allone_reduction_from_immediate_result");
    sim.check(sim.dut->ps_allone2l_f5a_h == 1 && sim.dut->ps_allone2r_f5a_h == 1,
              "ps_allone_reduction_from_immediate_result");
    sim.check(sim.dut->ph_stky2l_f5a_h == 1, "ph_left_sticky_from_global_sticky");
    sim.check(sim.dut->ps_stky2l_f5a_h == 1, "ps_left_sticky_from_global_sticky");
    sim.check(sim.dut->int32_stky2l_f5a_h == 1, "int32_left_sticky_from_right_sticky");
    sim.check(sim.dut->rnd_l_int_f6a_h == 1, "integer_left_round_from_transrnd");
    sim.check(sim.dut->rnd_r_int_f6a_h == 1, "integer_right_round_from_log_fma2");

    clear_inputs(sim);
    sim.dut->comb_add_f5a_h = 1ull << 51;
    step_all_clocks(sim);
    sim.check(sim.dut->comb_add_a3_f5a_h == 1, "comb_add_a3_reports_sign_bit");

    clear_inputs(sim);
    sim.dut->mmx_sel_immres_f5a_h = 1;
    sim.dut->int_minmax_res_f5a_h = 0;
    step_all_clocks(sim);
    sim.check(sim.dut->mergd_frac_res_hi_f6a_h == 0, "zero_immediate_result_merges_to_zero");
    sim.check(sim.dut->frac_zero_f6a_h == 1, "zero_immediate_result_detects_zero_fraction");

    return sim.finish();
}
