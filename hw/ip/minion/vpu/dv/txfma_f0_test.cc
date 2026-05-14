// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vtxfma_f0_tb.h"
#include "sim_ctrl.h"

#include <cstdint>

using DUT = Vtxfma_f0_tb;

namespace {

void clear_inputs(SimCtrl<DUT>& sim) {
    sim.dut->opa_frf_clk = 0;
    sim.dut->opb_frf_clk = 0;
    sim.dut->opc_frf_clk = 0;
    sim.dut->trans_frf_clk = 0;
    sim.dut->ctrl_frf_clk = 0;
    sim.dut->opa_byp_frfa_h = 0;
    sim.dut->opc_byp_frfa_h = 0;
    sim.dut->opb_byp_frfa_h = 0;
    sim.dut->xrom_c1c0k_ra_h = 0;
    sim.dut->xrom_c2_ra_h = 0;
    sim.dut->trans_mask_ra_h = 0;
    sim.dut->eb_hi_k_exa_h = 0;
    sim.dut->sel_ebin_hi_exa_h = 0;
    sim.dut->sel_eainmux_exa_h = 0;
    sim.dut->sel_ec5b_exa_h = 0;
    sim.dut->sel_ecin_exa_h = 0;
    sim.dut->sel_ec5bdet_exa_h = 0;
    sim.dut->sel_sc_exa_h = 0;
    sim.dut->sel_opcgfx_exa_h = 0;
    sim.dut->sel_xopc_exa_h = 0;
    sim.dut->sel_opcfmt_exa_h = 0;
    sim.dut->sel_transfa_exa_h = 0;
    sim.dut->sel_transfb_exa_h = 0;
    sim.dut->sel_opafmt_exa_h = 0;
    sim.dut->sel_opbfmt_exa_h = 0;
    sim.dut->sel_eblo_ph_exa_h = 0;
    sim.dut->elo_eb_en_exa_h = 0;
    sim.dut->ehi_eb_en_exa_h = 0;
    sim.dut->elo_ea_en_exa_h = 0;
    sim.dut->fa_en_exa_h = 0;
    sim.dut->fb_en_exa_h = 0;
    sim.dut->fc_en_exa_h = 0;
    sim.dut->sel_ealo_ph_exa_h = 0;
    sim.dut->sa_hi_en_exa_h = 0;
    sim.dut->sa_lo_en_exa_h = 0;
    sim.dut->sb_hi_en_exa_h = 0;
    sim.dut->sb_lo_en_exa_h = 0;
    sim.dut->sc_en_exa_h = 0;
    sim.dut->ehi_ea_en_exa_h = 0;
    sim.dut->ec_en_exa_h = 0;
    sim.dut->neg_fmaop_exa_h = 0;
    sim.dut->trans_op2_exa_l = 0;
    sim.dut->sa1_nr2_frcpfxp_exa_h = 0;
    sim.dut->sel_sign_sinp2_exa_h = 0;
    sim.dut->sa_zap_exa_l = 0;
    sim.dut->sb_zap_exa_l = 0;
    sim.dut->sel_f10_exa_h = 0;
    sim.dut->sel_f11_exa_h = 0;
    sim.dut->fc_dtps_exa_h = 0;
    sim.dut->sel_snorm_exa_h = 0;
    sim.dut->sel_g1sk2_exa_h = 0;
    sim.dut->sel_g1sk8_exa_h = 0;
    sim.dut->sel_g1sk16_exa_h = 0;
    sim.dut->sel_g1sk24_exa_h = 0;
    sim.dut->sel_xexpfrac_exa_h = 0;
}

void step_all_clocks(SimCtrl<DUT>& sim) {
    sim.dut->opa_frf_clk = 0;
    sim.dut->opb_frf_clk = 0;
    sim.dut->opc_frf_clk = 0;
    sim.dut->trans_frf_clk = 0;
    sim.dut->ctrl_frf_clk = 0;
    sim.dut->eval();
    sim.sim_time++;
    sim.dut->opa_frf_clk = 1;
    sim.dut->opb_frf_clk = 1;
    sim.dut->opc_frf_clk = 1;
    sim.dut->trans_frf_clk = 1;
    sim.dut->ctrl_frf_clk = 1;
    sim.dut->eval();
    sim.sim_time++;
    sim.dut->opa_frf_clk = 0;
    sim.dut->opb_frf_clk = 0;
    sim.dut->opc_frf_clk = 0;
    sim.dut->trans_frf_clk = 0;
    sim.dut->ctrl_frf_clk = 0;
    sim.dut->eval();
    sim.sim_time++;
}

void enable_passthrough_controls(SimCtrl<DUT>& sim) {
    sim.dut->sel_eainmux_exa_h = 1;
    sim.dut->sel_ebin_hi_exa_h = 1;
    sim.dut->sel_ecin_exa_h = 1;
    sim.dut->sel_opafmt_exa_h = 0;
    sim.dut->sel_opbfmt_exa_h = 0;
    sim.dut->sel_opcfmt_exa_h = 0;
    sim.dut->sel_ealo_ph_exa_h = 1;
    sim.dut->sel_eblo_ph_exa_h = 1;
    sim.dut->ehi_ea_en_exa_h = 1;
    sim.dut->elo_ea_en_exa_h = 1;
    sim.dut->ehi_eb_en_exa_h = 1;
    sim.dut->elo_eb_en_exa_h = 1;
    sim.dut->ec_en_exa_h = 1;
    sim.dut->fa_en_exa_h = 1;
    sim.dut->fb_en_exa_h = 1;
    sim.dut->fc_en_exa_h = 1;
    sim.dut->sa_hi_en_exa_h = 1;
    sim.dut->sa_lo_en_exa_h = 1;
    sim.dut->sb_hi_en_exa_h = 1;
    sim.dut->sb_lo_en_exa_h = 1;
    sim.dut->sc_en_exa_h = 1;
    sim.dut->trans_op2_exa_l = 1;
    sim.dut->sa_zap_exa_l = 1;
    sim.dut->sb_zap_exa_l = 1;
}

}  // namespace

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    clear_inputs(sim);
    step_all_clocks(sim);
    sim.check(sim.dut->fa_f0a_h == 0, "zero_controls_mask_fa");
    sim.check(sim.dut->fb_f0a_h == 0, "zero_controls_mask_fb");
    sim.check(sim.dut->fc_f0a_h == 0, "zero_controls_mask_fc");
    sim.check(sim.dut->ea_hi_f0a_h == 0 && sim.dut->eb_hi_f0a_h == 0 && sim.dut->ec_f0a_h == 0,
              "zero_controls_mask_exponents");
    sim.check(sim.dut->fcps_zero_f0a_h == 1, "zero_opc_reports_ps_fraction_zero");
    sim.check(sim.dut->fcnorm_zero_29_8_f0a_h == 1, "zero_fc_reports_norm_fraction_zero");

    clear_inputs(sim);
    const uint32_t opa = 0xc0a48601u;  // sign=1, exp=0x81, PH-low exponent/fraction bits set.
    const uint32_t opb = 0x7f208a02u;  // exp=0xfe exercises eb_eql_127_f0a_h.
    const uint32_t opc = 0xbf91a425u;  // sign=1 and nonzero F10/F11/PH/PS fractions.
    sim.dut->opa_byp_frfa_h = opa;
    sim.dut->opb_byp_frfa_h = opb;
    sim.dut->opc_byp_frfa_h = opc;
    enable_passthrough_controls(sim);
    step_all_clocks(sim);

    sim.check(sim.dut->fa_f0a_h == opa, "integer_format_fa_passthrough");
    sim.check(sim.dut->fb_f0a_h == opb, "integer_format_fb_passthrough");
    sim.check(sim.dut->fc_f0a_h == opc, "integer_format_fc_passthrough");
    sim.check(sim.dut->sa_hi_f0a_h == ((opa >> 31) & 1u), "sa_hi_sign_xor_path");
    sim.check(sim.dut->sa_lo_f0a_h == ((opa >> 15) & 1u), "sa_lo_ph_sign_bit");
    sim.check(sim.dut->sb_hi_f0a_h == ((opb >> 31) & 1u), "sb_hi_sign_path");
    sim.check(sim.dut->sb_lo_f0a_h == ((opb >> 15) & 1u), "sb_lo_ph_sign_bit");
    sim.check(sim.dut->sc_f0a_h == ((opc >> 31) & 1u), "sc_sign_path");
    sim.check(sim.dut->ea_hi_f0a_h == ((opa >> 23) & 0xffu), "ea_hi_exp_passthrough");
    sim.check(sim.dut->ea_lo_f0a_h == ((opa >> 10) & 0x1fu), "ea_lo_ph_exp_passthrough");
    sim.check(sim.dut->eb_hi_f0a_h == ((opb >> 23) & 0xffu), "eb_hi_exp_passthrough");
    sim.check(sim.dut->eb_lo_f0a_h == ((opb >> 10) & 0x1fu), "eb_lo_ph_exp_passthrough");
    sim.check(sim.dut->ec_f0a_h == ((opc >> 23) & 0xffu), "ec_exp_passthrough");
    sim.check(sim.dut->eb_eql_127_f0a_h == 1, "eb_exp_fe_special_detect");
    sim.check(sim.dut->faps_b0_f0a_h == ((opa >> 22) & 1u), "faps_b0_detect");
    sim.check(sim.dut->fbps_b0_f0a_h == ((opb >> 22) & 1u), "fbps_b0_detect");
    sim.check(sim.dut->fcps_b0_f0a_h == ((opc >> 22) & 1u), "fcps_b0_detect");
    sim.check(sim.dut->fcf10_zero_f0a_l == ((opc & 0xfu) != 0), "f10_fraction_zero_low_true_when_nonzero");
    sim.check(sim.dut->fcf11_zero_f0a_l == ((opc & 0x1fu) != 0), "f11_fraction_zero_low_true_when_nonzero");

    clear_inputs(sim);
    sim.dut->opb_byp_frfa_h = 0x7e800000u;  // exp=0xfd
    step_all_clocks(sim);
    sim.check(sim.dut->eb_eql_126_f0a_h == 1, "eb_exp_fd_special_detect");

    clear_inputs(sim);
    sim.dut->opb_byp_frfa_h = 0x3e800000u;  // exp=0x7d
    step_all_clocks(sim);
    sim.check(sim.dut->eb_eql_125_f0a_h == 1, "eb_exp_7d_special_detect");

    return sim.finish();
}
