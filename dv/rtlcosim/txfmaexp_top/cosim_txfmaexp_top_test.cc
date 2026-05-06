// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_txfmaexp_top_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>

using DUT = Vcosim_txfmaexp_top_tb;

namespace {

struct Inputs {
    bool ec_zero_f1 = false;
    bool ec_all1_f1 = false;
    bool ctrl_valid_f1 = false;
    bool ctrl_valid_f2 = false;
    bool ctrl_valid_f3 = false;
    bool ctrl_valid_f4 = false;
    bool ctrl_valid_f5 = false;
    uint32_t op_dtype_f1 = 0;
    uint32_t op_dtype_f2 = 0;
    uint32_t op_dtype_f5 = 0;
    uint32_t op_dtype_f6 = 0;
    uint32_t sigs_f1 = 0;
    uint32_t sigs_f2 = 0;
    uint32_t sigs_f5 = 0;
    uint32_t sigs_f6 = 0;
    uint32_t cmd_f1 = 0;
    uint32_t cmd_f2 = 0;
    uint32_t cmd_f5 = 0;
    uint32_t cmd_f6 = 0;
    uint32_t ea_hi_f1 = 0;
    uint32_t ea_lo_f1 = 0;
    uint32_t eb_hi_f1 = 0;
    uint32_t eb_lo_f1 = 0;
    uint32_t ec_f1 = 0;
    uint32_t etrz_fa_hi_ph_f1 = 0;
    uint32_t etrz_fa_lo_ps_f1 = 0;
    uint32_t etrz_fb_hi_ph_f1 = 0;
    uint32_t etrz_fb_lo_ps_f1 = 0;
    uint32_t etrz_fc_f1 = 0;
    bool sin_ediff2_f2 = false;
    uint32_t elxd_f4 = 0;
    uint32_t etrz50_f4 = 0;
    uint32_t rm_f5 = 0;
    uint32_t ea_hi_f5 = 0;
    bool ea_hi_zero_f1 = false;
    bool fc_zero30_8_f1 = false;
    bool eprod_hi_zero_f2 = false;
    bool eprod_lo_zero_f2 = false;
    bool ec_zero_f2 = false;
    bool exp_fma2_exc_f2 = false;
    bool eb_hi_lsb_f4 = false;
    bool use_elxd_f4 = false;
    bool elxd_add_in2_mux_sel_f4 = false;
    bool fb_hi_zero_f4 = false;
    bool comp_op_f5 = false;
    bool comp_op_true_f5 = false;
    bool int_minmax_mux_sel_a_f5 = false;
    bool int_minmax_mux_sel_c_f5 = false;
    bool nshc_f5 = false;
    bool s_res_f5 = false;
    bool e_force_to_zero_f5 = false;
    bool ps_rndovf_f6 = false;
    bool ph_rndovf_f6 = false;
};

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

void drive_inputs(CosimCtrl<DUT>& sim, const Inputs& in) {
    sim.dut->ec_zero_f1a_h_i = in.ec_zero_f1;
    sim.dut->ec_all1_f1a_h_i = in.ec_all1_f1;
    sim.dut->ctrl_valid_f1a_h_i = in.ctrl_valid_f1;
    sim.dut->ctrl_valid_f2a_h_i = in.ctrl_valid_f2;
    sim.dut->ctrl_valid_f3a_h_i = in.ctrl_valid_f3;
    sim.dut->ctrl_valid_f4a_h_i = in.ctrl_valid_f4;
    sim.dut->ctrl_valid_f5a_h_i = in.ctrl_valid_f5;
    sim.dut->op_dtype_f1a_h_i = in.op_dtype_f1;
    sim.dut->op_dtype_f2a_h_i = in.op_dtype_f2;
    sim.dut->op_dtype_f5a_h_i = in.op_dtype_f5;
    sim.dut->op_dtype_f6a_h_i = in.op_dtype_f6;
    sim.dut->sigs_f1a_h_i = in.sigs_f1;
    sim.dut->sigs_f2a_h_i = in.sigs_f2;
    sim.dut->sigs_f5a_h_i = in.sigs_f5;
    sim.dut->sigs_f6a_h_i = in.sigs_f6;
    sim.dut->cmd_f1a_h_i = in.cmd_f1;
    sim.dut->cmd_f2a_h_i = in.cmd_f2;
    sim.dut->cmd_f5a_h_i = in.cmd_f5;
    sim.dut->cmd_f6a_h_i = in.cmd_f6;
    sim.dut->ea_hi_f1a_h_i = in.ea_hi_f1;
    sim.dut->ea_lo_f1a_h_i = in.ea_lo_f1;
    sim.dut->eb_hi_f1a_h_i = in.eb_hi_f1;
    sim.dut->eb_lo_f1a_h_i = in.eb_lo_f1;
    sim.dut->ec_f1a_h_i = in.ec_f1;
    sim.dut->etrz_fa_hi_ph_f1a_h_i = in.etrz_fa_hi_ph_f1;
    sim.dut->etrz_fa_lo_ps_f1a_h_i = in.etrz_fa_lo_ps_f1;
    sim.dut->etrz_fb_hi_ph_f1a_h_i = in.etrz_fb_hi_ph_f1;
    sim.dut->etrz_fb_lo_ps_f1a_h_i = in.etrz_fb_lo_ps_f1;
    sim.dut->etrz_fc_f1a_h_i = in.etrz_fc_f1;
    sim.dut->sin_ediff2_f2a_h_i = in.sin_ediff2_f2;
    sim.dut->elxd_f4a_h_i = in.elxd_f4;
    sim.dut->etrz50_f4a_h_i = in.etrz50_f4;
    sim.dut->rm_f5a_h_i = in.rm_f5;
    sim.dut->ea_hi_f5a_h_i = in.ea_hi_f5;
    sim.dut->ea_hi_zero_f1a_h_i = in.ea_hi_zero_f1;
    sim.dut->fc_zero30_8_f1a_h_i = in.fc_zero30_8_f1;
    sim.dut->eprod_hi_zero_f2a_h_i = in.eprod_hi_zero_f2;
    sim.dut->eprod_lo_zero_f2a_h_i = in.eprod_lo_zero_f2;
    sim.dut->ec_zero_f2a_h_i = in.ec_zero_f2;
    sim.dut->exp_fma2_exc_f2a_h_i = in.exp_fma2_exc_f2;
    sim.dut->eb_hi_lsb_f4a_h_i = in.eb_hi_lsb_f4;
    sim.dut->use_elxd_f4a_h_i = in.use_elxd_f4;
    sim.dut->elxd_add_in2_mux_sel_f4a_h_i = in.elxd_add_in2_mux_sel_f4;
    sim.dut->fb_hi_zero_f4a_h_i = in.fb_hi_zero_f4;
    sim.dut->comp_op_f5a_h_i = in.comp_op_f5;
    sim.dut->comp_op_true_f5a_h_i = in.comp_op_true_f5;
    sim.dut->int_minmax_mux_sel_a_f5a_h_i = in.int_minmax_mux_sel_a_f5;
    sim.dut->int_minmax_mux_sel_c_f5a_h_i = in.int_minmax_mux_sel_c_f5;
    sim.dut->nshc_f5a_h_i = in.nshc_f5;
    sim.dut->s_res_f5a_h_i = in.s_res_f5;
    sim.dut->e_force_to_zero_f5a_h_i = in.e_force_to_zero_f5;
    sim.dut->ps_rndovf_f6a_h_i = in.ps_rndovf_f6;
    sim.dut->ph_rndovf_f6a_h_i = in.ph_rndovf_f6;
}

void compare_outputs(CosimCtrl<DUT>& sim) {
    sim.compare("ec_f2", sim.dut->orig_ec_f2a_h_o, sim.dut->new_ec_f2a_h_o);
    sim.compare("ec_f3", sim.dut->orig_ec_f3a_h_o, sim.dut->new_ec_f3a_h_o);
    sim.compare("e_sticky1_f2", sim.dut->orig_e_sticky1_f2a_h_o, sim.dut->new_e_sticky1_f2a_h_o);
    sim.compare("e_sticky2_f2", sim.dut->orig_e_sticky2_f2a_h_o, sim.dut->new_e_sticky2_f2a_h_o);
    sim.compare("e_sticky1_f3", sim.dut->orig_e_sticky1_f3a_h_o, sim.dut->new_e_sticky1_f3a_h_o);
    sim.compare("align1_mux_sel_f2", sim.dut->orig_align1_mux_sel_f2a_h_o, sim.dut->new_align1_mux_sel_f2a_h_o);
    sim.compare("align2_mux_sel_f2", sim.dut->orig_align2_mux_sel_f2a_h_o, sim.dut->new_align2_mux_sel_f2a_h_o);
    sim.compare("opd3_mux_sel_f2", sim.dut->orig_opd3_mux_sel_f2a_h_o, sim.dut->new_opd3_mux_sel_f2a_h_o);
    sim.compare("ediff1_f2", sim.dut->orig_ediff1_f2a_h_o, sim.dut->new_ediff1_f2a_h_o);
    sim.compare("ediff2_f3", sim.dut->orig_ediff2_f3a_h_o, sim.dut->new_ediff2_f3a_h_o);
    sim.compare("ediff1_sel_f2", sim.dut->orig_ediff1_sel_f2a_h_o, sim.dut->new_ediff1_sel_f2a_h_o);
    sim.compare("ediff2_sel_f2", sim.dut->orig_ediff2_sel_f2a_h_o, sim.dut->new_ediff2_sel_f2a_h_o);
    sim.compare("ediff1_abs_f2", sim.dut->orig_ediff1_abs_f2a_h_o, sim.dut->new_ediff1_abs_f2a_h_o);
    sim.compare("ediff2_abs_f2", sim.dut->orig_ediff2_abs_f2a_h_o, sim.dut->new_ediff2_abs_f2a_h_o);
    sim.compare("ediff_abs_diff_hi_f2", sim.dut->orig_ediff_abs_diff_hi_f2a_h_o, sim.dut->new_ediff_abs_diff_hi_f2a_h_o);
    sim.compare("align2_rshamt_c3_f2", sim.dut->orig_align2_rshamt_c3_f2a_h_o, sim.dut->new_align2_rshamt_c3_f2a_h_o);
    sim.compare("ph_comb_stiky_f4", sim.dut->orig_ph_comb_stiky_f4a_h_o, sim.dut->new_ph_comb_stiky_f4a_h_o);
    sim.compare("exp_res_2f3_f2", sim.dut->orig_exp_res_2f3_f2a_h_o, sim.dut->new_exp_res_2f3_f2a_h_o);
    sim.compare("stky2g_f5", sim.dut->orig_stky2g_f5a_h_o, sim.dut->new_stky2g_f5a_h_o);
    sim.compare("exp_res_f6", sim.dut->orig_exp_res_f6a_h_o, sim.dut->new_exp_res_f6a_h_o);
    sim.compare("exp_res_zero_f6", sim.dut->orig_exp_res_zero_f6a_h_o, sim.dut->new_exp_res_zero_f6a_h_o);
    sim.compare("rmode_f5", sim.dut->orig_rmode_f5a_h_o, sim.dut->new_rmode_f5a_h_o);
    sim.compare("ediff1_zap_f5", sim.dut->orig_ediff1_zap_f5a_h_o, sim.dut->new_ediff1_zap_f5a_h_o);
    sim.compare("def_ovf_ps_f6", sim.dut->orig_def_ovf_ps_f6a_h_o, sim.dut->new_def_ovf_ps_f6a_h_o);
    sim.compare("pot_ovf_ps_f6", sim.dut->orig_pot_ovf_ps_f6a_h_o, sim.dut->new_pot_ovf_ps_f6a_h_o);
    sim.compare("def_unf_ps_f6", sim.dut->orig_def_unf_ps_f6a_h_o, sim.dut->new_def_unf_ps_f6a_h_o);
    sim.compare("pot_unf_ps_f6", sim.dut->orig_pot_unf_ps_f6a_h_o, sim.dut->new_pot_unf_ps_f6a_h_o);
    sim.compare("def_ovf_ph_f6", sim.dut->orig_def_ovf_ph_f6a_h_o, sim.dut->new_def_ovf_ph_f6a_h_o);
    sim.compare("pot_ovf_ph_f6", sim.dut->orig_pot_ovf_ph_f6a_h_o, sim.dut->new_pot_ovf_ph_f6a_h_o);
    sim.compare("def_unf_ph_f6", sim.dut->orig_def_unf_ph_f6a_h_o, sim.dut->new_def_unf_ph_f6a_h_o);
    sim.compare("pot_unf_ph_f6", sim.dut->orig_pot_unf_ph_f6a_h_o, sim.dut->new_pot_unf_ph_f6a_h_o);
}

void run_cycle(CosimCtrl<DUT>& sim, const Inputs& in) {
    drive_inputs(sim, in);
    sim.tick();
    compare_outputs(sim);
}

void update_bool(bool& saw0, bool& saw1, bool v) {
    saw0 |= !v;
    saw1 |= v;
}

void update_mask(uint32_t& ones, uint32_t& zeros, uint32_t value, uint32_t mask) {
    ones |= value & mask;
    zeros |= (~value) & mask;
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 50000;

    Inputs zeros{};
    for (int i = 0; i < 8; ++i) {
      zeros.ctrl_valid_f1 = true;
      zeros.ctrl_valid_f2 = true;
      zeros.ctrl_valid_f3 = true;
      zeros.ctrl_valid_f4 = true;
      zeros.ctrl_valid_f5 = true;
      run_cycle(sim, zeros);
    }

    bool ec_zero_f1_0=false, ec_zero_f1_1=false, ec_all1_f1_0=false, ec_all1_f1_1=false;
    bool ctrl_valid_f1_0=false, ctrl_valid_f1_1=false, ctrl_valid_f2_0=false, ctrl_valid_f2_1=false;
    bool ctrl_valid_f3_0=false, ctrl_valid_f3_1=false, ctrl_valid_f4_0=false, ctrl_valid_f4_1=false;
    bool ctrl_valid_f5_0=false, ctrl_valid_f5_1=false, sin_ediff2_f2_0=false, sin_ediff2_f2_1=false;
    bool ea_hi_zero_f1_0=false, ea_hi_zero_f1_1=false, fc_zero30_8_f1_0=false, fc_zero30_8_f1_1=false;
    bool eprod_hi_zero_f2_0=false, eprod_hi_zero_f2_1=false, eprod_lo_zero_f2_0=false, eprod_lo_zero_f2_1=false;
    bool ec_zero_f2_0=false, ec_zero_f2_1=false, exp_fma2_exc_f2_0=false, exp_fma2_exc_f2_1=false;
    bool eb_hi_lsb_f4_0=false, eb_hi_lsb_f4_1=false, use_elxd_f4_0=false, use_elxd_f4_1=false;
    bool elxd_add_in2_mux_sel_f4_0=false, elxd_add_in2_mux_sel_f4_1=false, fb_hi_zero_f4_0=false, fb_hi_zero_f4_1=false;
    bool comp_op_f5_0=false, comp_op_f5_1=false, comp_op_true_f5_0=false, comp_op_true_f5_1=false;
    bool int_minmax_mux_sel_a_f5_0=false, int_minmax_mux_sel_a_f5_1=false;
    bool int_minmax_mux_sel_c_f5_0=false, int_minmax_mux_sel_c_f5_1=false;
    bool nshc_f5_0=false, nshc_f5_1=false, s_res_f5_0=false, s_res_f5_1=false;
    bool e_force_to_zero_f5_0=false, e_force_to_zero_f5_1=false;
    bool ps_rndovf_f6_0=false, ps_rndovf_f6_1=false, ph_rndovf_f6_0=false, ph_rndovf_f6_1=false;

    uint32_t op_dtype_f1_ones=0, op_dtype_f1_zeros=0, op_dtype_f2_ones=0, op_dtype_f2_zeros=0;
    uint32_t op_dtype_f5_ones=0, op_dtype_f5_zeros=0, op_dtype_f6_ones=0, op_dtype_f6_zeros=0;
    uint32_t sigs_f1_ones=0, sigs_f1_zeros=0, sigs_f2_ones=0, sigs_f2_zeros=0;
    uint32_t sigs_f5_ones=0, sigs_f5_zeros=0, sigs_f6_ones=0, sigs_f6_zeros=0;
    uint32_t cmd_f1_ones=0, cmd_f1_zeros=0, cmd_f2_ones=0, cmd_f2_zeros=0;
    uint32_t cmd_f5_ones=0, cmd_f5_zeros=0, cmd_f6_ones=0, cmd_f6_zeros=0;
    uint32_t ea_hi_f1_ones=0, ea_hi_f1_zeros=0, ea_lo_f1_ones=0, ea_lo_f1_zeros=0;
    uint32_t eb_hi_f1_ones=0, eb_hi_f1_zeros=0, eb_lo_f1_ones=0, eb_lo_f1_zeros=0;
    uint32_t ec_f1_ones=0, ec_f1_zeros=0, etrz_fa_hi_ph_f1_ones=0, etrz_fa_hi_ph_f1_zeros=0;
    uint32_t etrz_fa_lo_ps_f1_ones=0, etrz_fa_lo_ps_f1_zeros=0, etrz_fb_hi_ph_f1_ones=0, etrz_fb_hi_ph_f1_zeros=0;
    uint32_t etrz_fb_lo_ps_f1_ones=0, etrz_fb_lo_ps_f1_zeros=0, etrz_fc_f1_ones=0, etrz_fc_f1_zeros=0;
    uint32_t elxd_f4_ones=0, elxd_f4_zeros=0, etrz50_f4_ones=0, etrz50_f4_zeros=0;
    uint32_t rm_f5_ones=0, rm_f5_zeros=0, ea_hi_f5_ones=0, ea_hi_f5_zeros=0;

    uint32_t seed = 0x4d7f92a1u;
    for (int i = 0; i < 16384; ++i) {
        Inputs in;
        in.ec_zero_f1 = (xorshift32(seed) & 1u) != 0u;
        in.ec_all1_f1 = (xorshift32(seed) & 1u) != 0u;
        in.ctrl_valid_f1 = (xorshift32(seed) & 1u) != 0u;
        in.ctrl_valid_f2 = (xorshift32(seed) & 1u) != 0u;
        in.ctrl_valid_f3 = (xorshift32(seed) & 1u) != 0u;
        in.ctrl_valid_f4 = (xorshift32(seed) & 1u) != 0u;
        in.ctrl_valid_f5 = (xorshift32(seed) & 1u) != 0u;
        in.op_dtype_f1 = xorshift32(seed) & 0xfu;
        in.op_dtype_f2 = xorshift32(seed) & 0xfu;
        in.op_dtype_f5 = xorshift32(seed) & 0xfu;
        in.op_dtype_f6 = xorshift32(seed) & 0xfu;
        in.sigs_f1 = xorshift32(seed) & 0x7ffffu;
        in.sigs_f2 = xorshift32(seed) & 0x7ffffu;
        in.sigs_f5 = xorshift32(seed) & 0x7ffffu;
        in.sigs_f6 = xorshift32(seed) & 0x7ffffu;
        in.cmd_f1 = xorshift32(seed) & 0x7fu;
        in.cmd_f2 = xorshift32(seed) & 0x7fu;
        in.cmd_f5 = xorshift32(seed) & 0x7fu;
        in.cmd_f6 = xorshift32(seed) & 0x7fu;
        in.ea_hi_f1 = xorshift32(seed) & 0xffu;
        in.ea_lo_f1 = xorshift32(seed) & 0x7fu;
        in.eb_hi_f1 = xorshift32(seed) & 0xffu;
        in.eb_lo_f1 = xorshift32(seed) & 0x7fu;
        in.ec_f1 = xorshift32(seed) & 0xffu;
        in.etrz_fa_hi_ph_f1 = xorshift32(seed) & 0xfu;
        in.etrz_fa_lo_ps_f1 = xorshift32(seed) & 0x1fu;
        in.etrz_fb_hi_ph_f1 = xorshift32(seed) & 0xfu;
        in.etrz_fb_lo_ps_f1 = xorshift32(seed) & 0x1fu;
        in.etrz_fc_f1 = xorshift32(seed) & 0x1fu;
        in.sin_ediff2_f2 = (xorshift32(seed) & 1u) != 0u;
        in.elxd_f4 = xorshift32(seed) & 0x7fu;
        in.etrz50_f4 = xorshift32(seed) & 0x3fu;
        in.rm_f5 = xorshift32(seed) & 0x7u;
        in.ea_hi_f5 = xorshift32(seed) & 0xffu;
        in.ea_hi_zero_f1 = (xorshift32(seed) & 1u) != 0u;
        in.fc_zero30_8_f1 = (xorshift32(seed) & 1u) != 0u;
        in.eprod_hi_zero_f2 = (xorshift32(seed) & 1u) != 0u;
        in.eprod_lo_zero_f2 = (xorshift32(seed) & 1u) != 0u;
        in.ec_zero_f2 = (xorshift32(seed) & 1u) != 0u;
        in.exp_fma2_exc_f2 = (xorshift32(seed) & 1u) != 0u;
        in.eb_hi_lsb_f4 = (xorshift32(seed) & 1u) != 0u;
        in.use_elxd_f4 = (xorshift32(seed) & 1u) != 0u;
        in.elxd_add_in2_mux_sel_f4 = (xorshift32(seed) & 1u) != 0u;
        in.fb_hi_zero_f4 = (xorshift32(seed) & 1u) != 0u;
        in.comp_op_f5 = (xorshift32(seed) & 1u) != 0u;
        in.comp_op_true_f5 = (xorshift32(seed) & 1u) != 0u;
        in.int_minmax_mux_sel_a_f5 = (xorshift32(seed) & 1u) != 0u;
        in.int_minmax_mux_sel_c_f5 = (xorshift32(seed) & 1u) != 0u;
        in.nshc_f5 = (xorshift32(seed) & 1u) != 0u;
        in.s_res_f5 = (xorshift32(seed) & 1u) != 0u;
        in.e_force_to_zero_f5 = (xorshift32(seed) & 1u) != 0u;
        in.ps_rndovf_f6 = (xorshift32(seed) & 1u) != 0u;
        in.ph_rndovf_f6 = (xorshift32(seed) & 1u) != 0u;

        update_bool(ec_zero_f1_0, ec_zero_f1_1, in.ec_zero_f1);
        update_bool(ec_all1_f1_0, ec_all1_f1_1, in.ec_all1_f1);
        update_bool(ctrl_valid_f1_0, ctrl_valid_f1_1, in.ctrl_valid_f1);
        update_bool(ctrl_valid_f2_0, ctrl_valid_f2_1, in.ctrl_valid_f2);
        update_bool(ctrl_valid_f3_0, ctrl_valid_f3_1, in.ctrl_valid_f3);
        update_bool(ctrl_valid_f4_0, ctrl_valid_f4_1, in.ctrl_valid_f4);
        update_bool(ctrl_valid_f5_0, ctrl_valid_f5_1, in.ctrl_valid_f5);
        update_bool(sin_ediff2_f2_0, sin_ediff2_f2_1, in.sin_ediff2_f2);
        update_bool(ea_hi_zero_f1_0, ea_hi_zero_f1_1, in.ea_hi_zero_f1);
        update_bool(fc_zero30_8_f1_0, fc_zero30_8_f1_1, in.fc_zero30_8_f1);
        update_bool(eprod_hi_zero_f2_0, eprod_hi_zero_f2_1, in.eprod_hi_zero_f2);
        update_bool(eprod_lo_zero_f2_0, eprod_lo_zero_f2_1, in.eprod_lo_zero_f2);
        update_bool(ec_zero_f2_0, ec_zero_f2_1, in.ec_zero_f2);
        update_bool(exp_fma2_exc_f2_0, exp_fma2_exc_f2_1, in.exp_fma2_exc_f2);
        update_bool(eb_hi_lsb_f4_0, eb_hi_lsb_f4_1, in.eb_hi_lsb_f4);
        update_bool(use_elxd_f4_0, use_elxd_f4_1, in.use_elxd_f4);
        update_bool(elxd_add_in2_mux_sel_f4_0, elxd_add_in2_mux_sel_f4_1, in.elxd_add_in2_mux_sel_f4);
        update_bool(fb_hi_zero_f4_0, fb_hi_zero_f4_1, in.fb_hi_zero_f4);
        update_bool(comp_op_f5_0, comp_op_f5_1, in.comp_op_f5);
        update_bool(comp_op_true_f5_0, comp_op_true_f5_1, in.comp_op_true_f5);
        update_bool(int_minmax_mux_sel_a_f5_0, int_minmax_mux_sel_a_f5_1, in.int_minmax_mux_sel_a_f5);
        update_bool(int_minmax_mux_sel_c_f5_0, int_minmax_mux_sel_c_f5_1, in.int_minmax_mux_sel_c_f5);
        update_bool(nshc_f5_0, nshc_f5_1, in.nshc_f5);
        update_bool(s_res_f5_0, s_res_f5_1, in.s_res_f5);
        update_bool(e_force_to_zero_f5_0, e_force_to_zero_f5_1, in.e_force_to_zero_f5);
        update_bool(ps_rndovf_f6_0, ps_rndovf_f6_1, in.ps_rndovf_f6);
        update_bool(ph_rndovf_f6_0, ph_rndovf_f6_1, in.ph_rndovf_f6);

        update_mask(op_dtype_f1_ones, op_dtype_f1_zeros, in.op_dtype_f1, 0xfu);
        update_mask(op_dtype_f2_ones, op_dtype_f2_zeros, in.op_dtype_f2, 0xfu);
        update_mask(op_dtype_f5_ones, op_dtype_f5_zeros, in.op_dtype_f5, 0xfu);
        update_mask(op_dtype_f6_ones, op_dtype_f6_zeros, in.op_dtype_f6, 0xfu);
        update_mask(sigs_f1_ones, sigs_f1_zeros, in.sigs_f1, 0x7ffffu);
        update_mask(sigs_f2_ones, sigs_f2_zeros, in.sigs_f2, 0x7ffffu);
        update_mask(sigs_f5_ones, sigs_f5_zeros, in.sigs_f5, 0x7ffffu);
        update_mask(sigs_f6_ones, sigs_f6_zeros, in.sigs_f6, 0x7ffffu);
        update_mask(cmd_f1_ones, cmd_f1_zeros, in.cmd_f1, 0x7fu);
        update_mask(cmd_f2_ones, cmd_f2_zeros, in.cmd_f2, 0x7fu);
        update_mask(cmd_f5_ones, cmd_f5_zeros, in.cmd_f5, 0x7fu);
        update_mask(cmd_f6_ones, cmd_f6_zeros, in.cmd_f6, 0x7fu);
        update_mask(ea_hi_f1_ones, ea_hi_f1_zeros, in.ea_hi_f1, 0xffu);
        update_mask(ea_lo_f1_ones, ea_lo_f1_zeros, in.ea_lo_f1, 0x7fu);
        update_mask(eb_hi_f1_ones, eb_hi_f1_zeros, in.eb_hi_f1, 0xffu);
        update_mask(eb_lo_f1_ones, eb_lo_f1_zeros, in.eb_lo_f1, 0x7fu);
        update_mask(ec_f1_ones, ec_f1_zeros, in.ec_f1, 0xffu);
        update_mask(etrz_fa_hi_ph_f1_ones, etrz_fa_hi_ph_f1_zeros, in.etrz_fa_hi_ph_f1, 0xfu);
        update_mask(etrz_fa_lo_ps_f1_ones, etrz_fa_lo_ps_f1_zeros, in.etrz_fa_lo_ps_f1, 0x1fu);
        update_mask(etrz_fb_hi_ph_f1_ones, etrz_fb_hi_ph_f1_zeros, in.etrz_fb_hi_ph_f1, 0xfu);
        update_mask(etrz_fb_lo_ps_f1_ones, etrz_fb_lo_ps_f1_zeros, in.etrz_fb_lo_ps_f1, 0x1fu);
        update_mask(etrz_fc_f1_ones, etrz_fc_f1_zeros, in.etrz_fc_f1, 0x1fu);
        update_mask(elxd_f4_ones, elxd_f4_zeros, in.elxd_f4, 0x7fu);
        update_mask(etrz50_f4_ones, etrz50_f4_zeros, in.etrz50_f4, 0x3fu);
        update_mask(rm_f5_ones, rm_f5_zeros, in.rm_f5, 0x7u);
        update_mask(ea_hi_f5_ones, ea_hi_f5_zeros, in.ea_hi_f5, 0xffu);

        run_cycle(sim, in);
    }

    sim.check(ec_zero_f1_0 && ec_zero_f1_1, "ec_zero_f1 exercised both values");
    sim.check(ec_all1_f1_0 && ec_all1_f1_1, "ec_all1_f1 exercised both values");
    sim.check(ctrl_valid_f1_0 && ctrl_valid_f1_1, "ctrl_valid_f1 exercised both values");
    sim.check(ctrl_valid_f2_0 && ctrl_valid_f2_1, "ctrl_valid_f2 exercised both values");
    sim.check(ctrl_valid_f3_0 && ctrl_valid_f3_1, "ctrl_valid_f3 exercised both values");
    sim.check(ctrl_valid_f4_0 && ctrl_valid_f4_1, "ctrl_valid_f4 exercised both values");
    sim.check(ctrl_valid_f5_0 && ctrl_valid_f5_1, "ctrl_valid_f5 exercised both values");
    sim.check(sin_ediff2_f2_0 && sin_ediff2_f2_1, "sin_ediff2_f2 exercised both values");
    sim.check(ea_hi_zero_f1_0 && ea_hi_zero_f1_1, "ea_hi_zero_f1 exercised both values");
    sim.check(fc_zero30_8_f1_0 && fc_zero30_8_f1_1, "fc_zero30_8_f1 exercised both values");
    sim.check(eprod_hi_zero_f2_0 && eprod_hi_zero_f2_1, "eprod_hi_zero_f2 exercised both values");
    sim.check(eprod_lo_zero_f2_0 && eprod_lo_zero_f2_1, "eprod_lo_zero_f2 exercised both values");
    sim.check(ec_zero_f2_0 && ec_zero_f2_1, "ec_zero_f2 exercised both values");
    sim.check(exp_fma2_exc_f2_0 && exp_fma2_exc_f2_1, "exp_fma2_exc_f2 exercised both values");
    sim.check(eb_hi_lsb_f4_0 && eb_hi_lsb_f4_1, "eb_hi_lsb_f4 exercised both values");
    sim.check(use_elxd_f4_0 && use_elxd_f4_1, "use_elxd_f4 exercised both values");
    sim.check(elxd_add_in2_mux_sel_f4_0 && elxd_add_in2_mux_sel_f4_1, "elxd_add_in2_mux_sel_f4 exercised both values");
    sim.check(fb_hi_zero_f4_0 && fb_hi_zero_f4_1, "fb_hi_zero_f4 exercised both values");
    sim.check(comp_op_f5_0 && comp_op_f5_1, "comp_op_f5 exercised both values");
    sim.check(comp_op_true_f5_0 && comp_op_true_f5_1, "comp_op_true_f5 exercised both values");
    sim.check(int_minmax_mux_sel_a_f5_0 && int_minmax_mux_sel_a_f5_1, "int_minmax_mux_sel_a_f5 exercised both values");
    sim.check(int_minmax_mux_sel_c_f5_0 && int_minmax_mux_sel_c_f5_1, "int_minmax_mux_sel_c_f5 exercised both values");
    sim.check(nshc_f5_0 && nshc_f5_1, "nshc_f5 exercised both values");
    sim.check(s_res_f5_0 && s_res_f5_1, "s_res_f5 exercised both values");
    sim.check(e_force_to_zero_f5_0 && e_force_to_zero_f5_1, "e_force_to_zero_f5 exercised both values");
    sim.check(ps_rndovf_f6_0 && ps_rndovf_f6_1, "ps_rndovf_f6 exercised both values");
    sim.check(ph_rndovf_f6_0 && ph_rndovf_f6_1, "ph_rndovf_f6 exercised both values");

    sim.check(op_dtype_f1_ones == 0xfu && op_dtype_f1_zeros == 0xfu, "op_dtype_f1 exercised all bits both ways");
    sim.check(op_dtype_f2_ones == 0xfu && op_dtype_f2_zeros == 0xfu, "op_dtype_f2 exercised all bits both ways");
    sim.check(op_dtype_f5_ones == 0xfu && op_dtype_f5_zeros == 0xfu, "op_dtype_f5 exercised all bits both ways");
    sim.check(op_dtype_f6_ones == 0xfu && op_dtype_f6_zeros == 0xfu, "op_dtype_f6 exercised all bits both ways");
    sim.check(sigs_f1_ones == 0x7ffffu && sigs_f1_zeros == 0x7ffffu, "sigs_f1 exercised all bits both ways");
    sim.check(sigs_f2_ones == 0x7ffffu && sigs_f2_zeros == 0x7ffffu, "sigs_f2 exercised all bits both ways");
    sim.check(sigs_f5_ones == 0x7ffffu && sigs_f5_zeros == 0x7ffffu, "sigs_f5 exercised all bits both ways");
    sim.check(sigs_f6_ones == 0x7ffffu && sigs_f6_zeros == 0x7ffffu, "sigs_f6 exercised all bits both ways");
    sim.check(cmd_f1_ones == 0x7fu && cmd_f1_zeros == 0x7fu, "cmd_f1 exercised all bits both ways");
    sim.check(cmd_f2_ones == 0x7fu && cmd_f2_zeros == 0x7fu, "cmd_f2 exercised all bits both ways");
    sim.check(cmd_f5_ones == 0x7fu && cmd_f5_zeros == 0x7fu, "cmd_f5 exercised all bits both ways");
    sim.check(cmd_f6_ones == 0x7fu && cmd_f6_zeros == 0x7fu, "cmd_f6 exercised all bits both ways");
    sim.check(ea_hi_f1_ones == 0xffu && ea_hi_f1_zeros == 0xffu, "ea_hi_f1 exercised all bits both ways");
    sim.check(ea_lo_f1_ones == 0x7fu && ea_lo_f1_zeros == 0x7fu, "ea_lo_f1 exercised all bits both ways");
    sim.check(eb_hi_f1_ones == 0xffu && eb_hi_f1_zeros == 0xffu, "eb_hi_f1 exercised all bits both ways");
    sim.check(eb_lo_f1_ones == 0x7fu && eb_lo_f1_zeros == 0x7fu, "eb_lo_f1 exercised all bits both ways");
    sim.check(ec_f1_ones == 0xffu && ec_f1_zeros == 0xffu, "ec_f1 exercised all bits both ways");
    sim.check(etrz_fa_hi_ph_f1_ones == 0xfu && etrz_fa_hi_ph_f1_zeros == 0xfu, "etrz_fa_hi_ph_f1 exercised all bits both ways");
    sim.check(etrz_fa_lo_ps_f1_ones == 0x1fu && etrz_fa_lo_ps_f1_zeros == 0x1fu, "etrz_fa_lo_ps_f1 exercised all bits both ways");
    sim.check(etrz_fb_hi_ph_f1_ones == 0xfu && etrz_fb_hi_ph_f1_zeros == 0xfu, "etrz_fb_hi_ph_f1 exercised all bits both ways");
    sim.check(etrz_fb_lo_ps_f1_ones == 0x1fu && etrz_fb_lo_ps_f1_zeros == 0x1fu, "etrz_fb_lo_ps_f1 exercised all bits both ways");
    sim.check(etrz_fc_f1_ones == 0x1fu && etrz_fc_f1_zeros == 0x1fu, "etrz_fc_f1 exercised all bits both ways");
    sim.check(elxd_f4_ones == 0x7fu && elxd_f4_zeros == 0x7fu, "elxd_f4 exercised all bits both ways");
    sim.check(etrz50_f4_ones == 0x3fu && etrz50_f4_zeros == 0x3fu, "etrz50_f4 exercised all bits both ways");
    sim.check(rm_f5_ones == 0x7u && rm_f5_zeros == 0x7u, "rm_f5 exercised all bits both ways");
    sim.check(ea_hi_f5_ones == 0xffu && ea_hi_f5_zeros == 0xffu, "ea_hi_f5 exercised all bits both ways");

    sim.check(sim.comparisons >= 500000, "txfmaexp_top cosim should generate at least 500000 comparisons");

    return sim.finish();
}
