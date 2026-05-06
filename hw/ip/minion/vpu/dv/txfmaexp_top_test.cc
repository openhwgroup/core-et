// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfmaexp_top_tb.h"
#include "sim_ctrl.h"

using DUT = Vtxfmaexp_top_tb;

namespace {

void drive_zero(DUT* dut) {
    dut->ec_zero_f1a_h_i = 0;
    dut->ec_all1_f1a_h_i = 0;
    dut->ctrl_valid_f1a_h_i = 1;
    dut->ctrl_valid_f2a_h_i = 1;
    dut->ctrl_valid_f3a_h_i = 1;
    dut->ctrl_valid_f4a_h_i = 1;
    dut->ctrl_valid_f5a_h_i = 1;
    dut->op_dtype_f1a_h_i = 0;
    dut->op_dtype_f2a_h_i = 0;
    dut->op_dtype_f5a_h_i = 0;
    dut->op_dtype_f6a_h_i = 0;
    dut->sigs_f1a_h_i = 0;
    dut->sigs_f2a_h_i = 0;
    dut->sigs_f5a_h_i = 0;
    dut->sigs_f6a_h_i = 0;
    dut->cmd_f1a_h_i = 0;
    dut->cmd_f2a_h_i = 0;
    dut->cmd_f5a_h_i = 0;
    dut->cmd_f6a_h_i = 0;
    dut->ea_hi_f1a_h_i = 0;
    dut->ea_lo_f1a_h_i = 0;
    dut->eb_hi_f1a_h_i = 0;
    dut->eb_lo_f1a_h_i = 0;
    dut->ec_f1a_h_i = 0;
    dut->etrz_fa_hi_ph_f1a_h_i = 0;
    dut->etrz_fa_lo_ps_f1a_h_i = 0;
    dut->etrz_fb_hi_ph_f1a_h_i = 0;
    dut->etrz_fb_lo_ps_f1a_h_i = 0;
    dut->etrz_fc_f1a_h_i = 0;
    dut->sin_ediff2_f2a_h_i = 0;
    dut->elxd_f4a_h_i = 0;
    dut->etrz50_f4a_h_i = 0;
    dut->rm_f5a_h_i = 0;
    dut->ea_hi_f5a_h_i = 0;
    dut->ea_hi_zero_f1a_h_i = 0;
    dut->fc_zero30_8_f1a_h_i = 0;
    dut->eprod_hi_zero_f2a_h_i = 0;
    dut->eprod_lo_zero_f2a_h_i = 0;
    dut->ec_zero_f2a_h_i = 0;
    dut->exp_fma2_exc_f2a_h_i = 0;
    dut->eb_hi_lsb_f4a_h_i = 0;
    dut->use_elxd_f4a_h_i = 0;
    dut->elxd_add_in2_mux_sel_f4a_h_i = 0;
    dut->fb_hi_zero_f4a_h_i = 0;
    dut->comp_op_f5a_h_i = 0;
    dut->comp_op_true_f5a_h_i = 0;
    dut->int_minmax_mux_sel_a_f5a_h_i = 0;
    dut->int_minmax_mux_sel_c_f5a_h_i = 0;
    dut->nshc_f5a_h_i = 0;
    dut->s_res_f5a_h_i = 0;
    dut->e_force_to_zero_f5a_h_i = 0;
    dut->ps_rndovf_f6a_h_i = 0;
    dut->ph_rndovf_f6a_h_i = 0;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);
    sim.max_time = 200;

    drive_zero(sim.dut.get());
    for (int i = 0; i < 8; ++i) sim.tick();

    sim.check(sim.dut->ec_f2a_h_o == 0x70, "zero flush produces expected ec_f2");
    sim.check(sim.dut->ec_f3a_h_o == 0x70, "zero flush produces expected ec_f3");
    sim.check(sim.dut->e_sticky1_f2a_h_o == 0, "zero flush leaves e_sticky1_f2 clear");
    sim.check(sim.dut->e_sticky2_f2a_h_o == 0, "zero flush leaves e_sticky2_f2 clear");
    sim.check(sim.dut->e_sticky1_f3a_h_o == 0, "zero flush leaves e_sticky1_f3 clear");
    sim.check(sim.dut->align1_mux_sel_f2a_h_o == 0, "zero flush selects align1 source 0");
    sim.check(sim.dut->align2_mux_sel_f2a_h_o == 1, "zero flush selects align2 source 1");
    sim.check(sim.dut->opd3_mux_sel_f2a_h_o == 4, "zero flush selects opd3 source 4");
    sim.check(sim.dut->ediff1_f2a_h_o == 0x390, "zero flush produces expected ediff1");
    sim.check(sim.dut->ediff2_f3a_h_o == 0x39f, "zero flush produces expected ediff2");
    sim.check(sim.dut->ediff1_sel_f2a_h_o == 0, "zero flush produces expected ediff1 select");
    sim.check(sim.dut->ediff2_sel_f2a_h_o == 1, "zero flush produces expected ediff2 select");
    sim.check(sim.dut->ediff1_abs_f2a_h_o == 0x70, "zero flush produces expected ediff1_abs");
    sim.check(sim.dut->ediff2_abs_f2a_h_o == 0x61, "zero flush produces expected ediff2_abs");
    sim.check(sim.dut->ediff_abs_diff_hi_f2a_h_o == 0x30, "zero flush produces expected ediff_abs_diff_hi");
    sim.check(sim.dut->align2_rshamt_c3_f2a_h_o == 0x1, "zero flush produces expected align2 shift amount");
    sim.check(sim.dut->ph_comb_stiky_f4a_h_o == 0, "zero flush keeps ph_comb_stiky low");
    sim.check(sim.dut->exp_res_2f3_f2a_h_o == 0x70, "zero flush produces expected exp_res_2f3");
    sim.check(sim.dut->stky2g_f5a_h_o == 1, "zero flush produces expected stky2g");
    sim.check(sim.dut->exp_res_f6a_h_o == 0x70, "zero flush produces expected exp_res_f6");
    sim.check(sim.dut->exp_res_zero_f6a_h_o == 0, "zero flush keeps exp_res_zero low");
    sim.check(sim.dut->rmode_f5a_h_o == 1, "zero flush produces expected rmode");
    sim.check(sim.dut->ediff1_zap_f5a_h_o == 0, "zero flush keeps ediff1_zap low");
    sim.check(sim.dut->def_ovf_ps_f6a_h_o == 0, "zero flush keeps default ps overflow low");
    sim.check(sim.dut->pot_ovf_ps_f6a_h_o == 0, "zero flush keeps potential ps overflow low");
    sim.check(sim.dut->def_unf_ps_f6a_h_o == 0, "zero flush keeps default ps underflow low");
    sim.check(sim.dut->pot_unf_ps_f6a_h_o == 0, "zero flush keeps potential ps underflow low");
    sim.check(sim.dut->def_ovf_ph_f6a_h_o == 0, "zero flush keeps default ph overflow low");
    sim.check(sim.dut->pot_ovf_ph_f6a_h_o == 0, "zero flush keeps potential ph overflow low");
    sim.check(sim.dut->def_unf_ph_f6a_h_o == 0, "zero flush keeps default ph underflow low");
    sim.check(sim.dut->pot_unf_ph_f6a_h_o == 1, "zero flush produces expected potential ph underflow");

    return sim.finish();
}
