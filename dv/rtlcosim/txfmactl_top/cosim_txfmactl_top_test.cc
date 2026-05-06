// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


#include "Vcosim_txfmactl_top_tb.h"
#include "cosim_ctrl.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using DUT = Vcosim_txfmactl_top_tb;

namespace {

bool g_saw_clk_low = false;
bool g_saw_clk_high = false;

uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

int input_words(CosimCtrl<DUT>& sim) {
    return static_cast<int>(sizeof(sim.dut->inputs_i) / sizeof(sim.dut->inputs_i[0]));
}

void drive_words(CosimCtrl<DUT>& sim, const std::vector<uint32_t>& words) {
    const int n = input_words(sim);
    for (int i = 0; i < n; ++i) sim.dut->inputs_i[i] = (i < static_cast<int>(words.size())) ? words[i] : 0u;
}

std::vector<uint32_t> filled_words(CosimCtrl<DUT>& sim, uint32_t value) {
    return std::vector<uint32_t>(input_words(sim), value);
}

std::vector<uint32_t> random_words(CosimCtrl<DUT>& sim, uint32_t& rng) {
    std::vector<uint32_t> words(input_words(sim));
    for (uint32_t& word : words) word = xorshift32(rng);
    return words;
}

void compare_all(CosimCtrl<DUT>& sim, const std::string& phase) {
    sim.compare("is_trans_rfa_h " + phase, sim.dut->orig_is_trans_rfa_h, sim.dut->new_is_trans_rfa_h);
    sim.compare("sel_eblo_ph_exa_h " + phase, sim.dut->orig_sel_eblo_ph_exa_h, sim.dut->new_sel_eblo_ph_exa_h);
    sim.compare("sel_ec5bdet_exa_h " + phase, sim.dut->orig_sel_ec5bdet_exa_h, sim.dut->new_sel_ec5bdet_exa_h);
    sim.compare("elo_eb_en_exa_h " + phase, sim.dut->orig_elo_eb_en_exa_h, sim.dut->new_elo_eb_en_exa_h);
    sim.compare("ehi_eb_en_exa_h " + phase, sim.dut->orig_ehi_eb_en_exa_h, sim.dut->new_ehi_eb_en_exa_h);
    sim.compare("elo_ea_en_exa_h " + phase, sim.dut->orig_elo_ea_en_exa_h, sim.dut->new_elo_ea_en_exa_h);
    sim.compare("fa_en_exa_h " + phase, sim.dut->orig_fa_en_exa_h, sim.dut->new_fa_en_exa_h);
    sim.compare("fb_en_exa_h " + phase, sim.dut->orig_fb_en_exa_h, sim.dut->new_fb_en_exa_h);
    sim.compare("fc_en_exa_h " + phase, sim.dut->orig_fc_en_exa_h, sim.dut->new_fc_en_exa_h);
    sim.compare("sel_ealo_ph_exa_h " + phase, sim.dut->orig_sel_ealo_ph_exa_h, sim.dut->new_sel_ealo_ph_exa_h);
    sim.compare("sa_hi_en_exa_h " + phase, sim.dut->orig_sa_hi_en_exa_h, sim.dut->new_sa_hi_en_exa_h);
    sim.compare("sa_lo_en_exa_h " + phase, sim.dut->orig_sa_lo_en_exa_h, sim.dut->new_sa_lo_en_exa_h);
    sim.compare("sb_hi_en_exa_h " + phase, sim.dut->orig_sb_hi_en_exa_h, sim.dut->new_sb_hi_en_exa_h);
    sim.compare("sb_lo_en_exa_h " + phase, sim.dut->orig_sb_lo_en_exa_h, sim.dut->new_sb_lo_en_exa_h);
    sim.compare("sc_en_exa_h " + phase, sim.dut->orig_sc_en_exa_h, sim.dut->new_sc_en_exa_h);
    sim.compare("ehi_ea_en_exa_h " + phase, sim.dut->orig_ehi_ea_en_exa_h, sim.dut->new_ehi_ea_en_exa_h);
    sim.compare("ec_en_exa_h " + phase, sim.dut->orig_ec_en_exa_h, sim.dut->new_ec_en_exa_h);
    sim.compare("neg_fmaop_exa_h " + phase, sim.dut->orig_neg_fmaop_exa_h, sim.dut->new_neg_fmaop_exa_h);
    sim.compare("trans_op2_exa_l " + phase, sim.dut->orig_trans_op2_exa_l, sim.dut->new_trans_op2_exa_l);
    sim.compare("sa1_nr2_frcpfxp_exa_h " + phase, sim.dut->orig_sa1_nr2_frcpfxp_exa_h, sim.dut->new_sa1_nr2_frcpfxp_exa_h);
    sim.compare("sel_sign_sinp2_exa_h " + phase, sim.dut->orig_sel_sign_sinp2_exa_h, sim.dut->new_sel_sign_sinp2_exa_h);
    sim.compare("sa_zap_exa_l " + phase, sim.dut->orig_sa_zap_exa_l, sim.dut->new_sa_zap_exa_l);
    sim.compare("sb_zap_exa_l " + phase, sim.dut->orig_sb_zap_exa_l, sim.dut->new_sb_zap_exa_l);
    sim.compare("sel_f10_exa_h " + phase, sim.dut->orig_sel_f10_exa_h, sim.dut->new_sel_f10_exa_h);
    sim.compare("sel_f11_exa_h " + phase, sim.dut->orig_sel_f11_exa_h, sim.dut->new_sel_f11_exa_h);
    sim.compare("fc_dtps_exa_h " + phase, sim.dut->orig_fc_dtps_exa_h, sim.dut->new_fc_dtps_exa_h);
    sim.compare("sel_snorm_exa_h " + phase, sim.dut->orig_sel_snorm_exa_h, sim.dut->new_sel_snorm_exa_h);
    sim.compare("sel_g1sk2_exa_h " + phase, sim.dut->orig_sel_g1sk2_exa_h, sim.dut->new_sel_g1sk2_exa_h);
    sim.compare("sel_g1sk8_exa_h " + phase, sim.dut->orig_sel_g1sk8_exa_h, sim.dut->new_sel_g1sk8_exa_h);
    sim.compare("sel_g1sk16_exa_h " + phase, sim.dut->orig_sel_g1sk16_exa_h, sim.dut->new_sel_g1sk16_exa_h);
    sim.compare("sel_g1sk24_exa_h " + phase, sim.dut->orig_sel_g1sk24_exa_h, sim.dut->new_sel_g1sk24_exa_h);
    sim.compare("sel_xexpfrac_exa_h " + phase, sim.dut->orig_sel_xexpfrac_exa_h, sim.dut->new_sel_xexpfrac_exa_h);
    sim.compare("sel_ebin_hi_exa_h " + phase, sim.dut->orig_sel_ebin_hi_exa_h, sim.dut->new_sel_ebin_hi_exa_h);
    sim.compare("sel_eainmux_exa_h " + phase, sim.dut->orig_sel_eainmux_exa_h, sim.dut->new_sel_eainmux_exa_h);
    sim.compare("sel_ec5b_exa_h " + phase, sim.dut->orig_sel_ec5b_exa_h, sim.dut->new_sel_ec5b_exa_h);
    sim.compare("sel_ecin_exa_h " + phase, sim.dut->orig_sel_ecin_exa_h, sim.dut->new_sel_ecin_exa_h);
    sim.compare("sel_sc_exa_h " + phase, sim.dut->orig_sel_sc_exa_h, sim.dut->new_sel_sc_exa_h);
    sim.compare("sel_opcgfx_exa_h " + phase, sim.dut->orig_sel_opcgfx_exa_h, sim.dut->new_sel_opcgfx_exa_h);
    sim.compare("sel_xopc_exa_h " + phase, sim.dut->orig_sel_xopc_exa_h, sim.dut->new_sel_xopc_exa_h);
    sim.compare("sel_opcfmt_exa_h " + phase, sim.dut->orig_sel_opcfmt_exa_h, sim.dut->new_sel_opcfmt_exa_h);
    sim.compare("sel_transfa_exa_h " + phase, sim.dut->orig_sel_transfa_exa_h, sim.dut->new_sel_transfa_exa_h);
    sim.compare("sel_transfb_exa_h " + phase, sim.dut->orig_sel_transfb_exa_h, sim.dut->new_sel_transfb_exa_h);
    sim.compare("sel_opafmt_exa_h " + phase, sim.dut->orig_sel_opafmt_exa_h, sim.dut->new_sel_opafmt_exa_h);
    sim.compare("sel_opbfmt_exa_h " + phase, sim.dut->orig_sel_opbfmt_exa_h, sim.dut->new_sel_opbfmt_exa_h);
    sim.compare("opa_valid_rfa_h " + phase, sim.dut->orig_opa_valid_rfa_h, sim.dut->new_opa_valid_rfa_h);
    sim.compare("opb_valid_rfa_h " + phase, sim.dut->orig_opb_valid_rfa_h, sim.dut->new_opb_valid_rfa_h);
    sim.compare("opc_valid_rfa_h " + phase, sim.dut->orig_opc_valid_rfa_h, sim.dut->new_opc_valid_rfa_h);
    sim.compare("dtps_f0a_h " + phase, sim.dut->orig_dtps_f0a_h, sim.dut->new_dtps_f0a_h);
    sim.compare("ctrl_valid_f0a_h " + phase, sim.dut->orig_ctrl_valid_f0a_h, sim.dut->new_ctrl_valid_f0a_h);
    sim.compare("ctrl_valid_f1a_h " + phase, sim.dut->orig_ctrl_valid_f1a_h, sim.dut->new_ctrl_valid_f1a_h);
    sim.compare("ctrl_valid_f2a_h " + phase, sim.dut->orig_ctrl_valid_f2a_h, sim.dut->new_ctrl_valid_f2a_h);
    sim.compare("ctrl_valid_f3a_h " + phase, sim.dut->orig_ctrl_valid_f3a_h, sim.dut->new_ctrl_valid_f3a_h);
    sim.compare("ctrl_valid_f4a_h " + phase, sim.dut->orig_ctrl_valid_f4a_h, sim.dut->new_ctrl_valid_f4a_h);
    sim.compare("ctrl_valid_f5a_h " + phase, sim.dut->orig_ctrl_valid_f5a_h, sim.dut->new_ctrl_valid_f5a_h);
    sim.compare("ctrl_valid_f6a_h " + phase, sim.dut->orig_ctrl_valid_f6a_h, sim.dut->new_ctrl_valid_f6a_h);
    sim.compare("mul_sign_f0a_h " + phase, sim.dut->orig_mul_sign_f0a_h, sim.dut->new_mul_sign_f0a_h);
    sim.compare("mul_hp_mode_f0a_h " + phase, sim.dut->orig_mul_hp_mode_f0a_h, sim.dut->new_mul_hp_mode_f0a_h);
    sim.compare("eb_hi_k_exa_h " + phase, sim.dut->orig_eb_hi_k_exa_h, sim.dut->new_eb_hi_k_exa_h);
    sim.compare("use_mul_f0a_h " + phase, sim.dut->orig_use_mul_f0a_h, sim.dut->new_use_mul_f0a_h);
    sim.compare("ec_zero_f1a_h " + phase, sim.dut->orig_ec_zero_f1a_h, sim.dut->new_ec_zero_f1a_h);
    sim.compare("ea_hi_zero_f1a_h " + phase, sim.dut->orig_ea_hi_zero_f1a_h, sim.dut->new_ea_hi_zero_f1a_h);
    sim.compare("fc_zero30_8_f1a_h " + phase, sim.dut->orig_fc_zero30_8_f1a_h, sim.dut->new_fc_zero30_8_f1a_h);
    sim.compare("paddin1_sels_f1a_h " + phase, sim.dut->orig_paddin1_sels_f1a_h, sim.dut->new_paddin1_sels_f1a_h);
    sim.compare("paddin2_selc_f1a_h " + phase, sim.dut->orig_paddin2_selc_f1a_h, sim.dut->new_paddin2_selc_f1a_h);
    sim.compare("paddin2_selfcneg_f1a_h " + phase, sim.dut->orig_paddin2_selfcneg_f1a_h, sim.dut->new_paddin2_selfcneg_f1a_h);
    sim.compare("padd_cin_f1a_h " + phase, sim.dut->orig_padd_cin_f1a_h, sim.dut->new_padd_cin_f1a_h);
    sim.compare("sel_famask_f1a_h " + phase, sim.dut->orig_sel_famask_f1a_h, sim.dut->new_sel_famask_f1a_h);
    sim.compare("fa_en_f1a_h " + phase, sim.dut->orig_fa_en_f1a_h, sim.dut->new_fa_en_f1a_h);
    sim.compare("cmd_f1a_h " + phase, sim.dut->orig_cmd_f1a_h, sim.dut->new_cmd_f1a_h);
    sim.compare("sigs_f1a_h " + phase, sim.dut->orig_sigs_f1a_h, sim.dut->new_sigs_f1a_h);
    sim.compare("op_dtype_f1a_h " + phase, sim.dut->orig_op_dtype_f1a_h, sim.dut->new_op_dtype_f1a_h);
    sim.compare("use_mul_f1a_h " + phase, sim.dut->orig_use_mul_f1a_h, sim.dut->new_use_mul_f1a_h);
    sim.compare("ec_zero_f2a_h " + phase, sim.dut->orig_ec_zero_f2a_h, sim.dut->new_ec_zero_f2a_h);
    sim.compare("eprod_hi_zero_f2a_h " + phase, sim.dut->orig_eprod_hi_zero_f2a_h, sim.dut->new_eprod_hi_zero_f2a_h);
    sim.compare("eprod_lo_zero_f2a_h " + phase, sim.dut->orig_eprod_lo_zero_f2a_h, sim.dut->new_eprod_lo_zero_f2a_h);
    sim.compare("op3p1c_sel_p_f2a_h " + phase, sim.dut->orig_op3p1c_sel_p_f2a_h, sim.dut->new_op3p1c_sel_p_f2a_h);
    sim.compare("op3p1c_sel_r_f2a_h " + phase, sim.dut->orig_op3p1c_sel_r_f2a_h, sim.dut->new_op3p1c_sel_r_f2a_h);
    sim.compare("op3g_sel_f2a_h " + phase, sim.dut->orig_op3g_sel_f2a_h, sim.dut->new_op3g_sel_f2a_h);
    sim.compare("rm_rne_f2a_h " + phase, sim.dut->orig_rm_rne_f2a_h, sim.dut->new_rm_rne_f2a_h);
    sim.compare("rm_rmm_f2a_h " + phase, sim.dut->orig_rm_rmm_f2a_h, sim.dut->new_rm_rmm_f2a_h);
    sim.compare("frnd_rupnsc_f2a_h " + phase, sim.dut->orig_frnd_rupnsc_f2a_h, sim.dut->new_frnd_rupnsc_f2a_h);
    sim.compare("frnd_rdnsc_f2a_h " + phase, sim.dut->orig_frnd_rdnsc_f2a_h, sim.dut->new_frnd_rdnsc_f2a_h);
    sim.compare("frnd_t1_f2a_h " + phase, sim.dut->orig_frnd_t1_f2a_h, sim.dut->new_frnd_t1_f2a_h);
    sim.compare("frnd_t2_f2a_h " + phase, sim.dut->orig_frnd_t2_f2a_h, sim.dut->new_frnd_t2_f2a_h);
    sim.compare("al1c_zap_f2a_h " + phase, sim.dut->orig_al1c_zap_f2a_h, sim.dut->new_al1c_zap_f2a_h);
    sim.compare("op1c_log_mul_clr_f2a_h " + phase, sim.dut->orig_op1c_log_mul_clr_f2a_h, sim.dut->new_op1c_log_mul_clr_f2a_h);
    sim.compare("negop1c_int_logmul_clr_f2a_h " + phase, sim.dut->orig_negop1c_int_logmul_clr_f2a_h, sim.dut->new_negop1c_int_logmul_clr_f2a_h);
    sim.compare("psc_op1_clr_f2a_h " + phase, sim.dut->orig_psc_op1_clr_f2a_h, sim.dut->new_psc_op1_clr_f2a_h);
    sim.compare("align2_zap_f2a_h " + phase, sim.dut->orig_align2_zap_f2a_h, sim.dut->new_align2_zap_f2a_h);
    sim.compare("align2c_neg_f2a_h " + phase, sim.dut->orig_align2c_neg_f2a_h, sim.dut->new_align2c_neg_f2a_h);
    sim.compare("trans_exp_fma2_exc_f2a_h " + phase, sim.dut->orig_trans_exp_fma2_exc_f2a_h, sim.dut->new_trans_exp_fma2_exc_f2a_h);
    sim.compare("al1p1c_sel_f2a_h " + phase, sim.dut->orig_al1p1c_sel_f2a_h, sim.dut->new_al1p1c_sel_f2a_h);
    sim.compare("op2_sxtto_g_f2a_h " + phase, sim.dut->orig_op2_sxtto_g_f2a_h, sim.dut->new_op2_sxtto_g_f2a_h);
    sim.compare("op3p3spc_sel_f2a_h " + phase, sim.dut->orig_op3p3spc_sel_f2a_h, sim.dut->new_op3p3spc_sel_f2a_h);
    sim.compare("op3p2c_sel_f2a_h " + phase, sim.dut->orig_op3p2c_sel_f2a_h, sim.dut->new_op3p2c_sel_f2a_h);
    sim.compare("ugfxrplc_sel_f2a_h " + phase, sim.dut->orig_ugfxrplc_sel_f2a_h, sim.dut->new_ugfxrplc_sel_f2a_h);
    sim.compare("sgfxrplc_sel_f2a_h " + phase, sim.dut->orig_sgfxrplc_sel_f2a_h, sim.dut->new_sgfxrplc_sel_f2a_h);
    sim.compare("icgfxc_sel_f2a_h " + phase, sim.dut->orig_icgfxc_sel_f2a_h, sim.dut->new_icgfxc_sel_f2a_h);
    sim.compare("al1znp2c_sel_f2a_h " + phase, sim.dut->orig_al1znp2c_sel_f2a_h, sim.dut->new_al1znp2c_sel_f2a_h);
    sim.compare("al2c_sel_f2a_h " + phase, sim.dut->orig_al2c_sel_f2a_h, sim.dut->new_al2c_sel_f2a_h);
    sim.compare("al1p2c_sel_f2a_h " + phase, sim.dut->orig_al1p2c_sel_f2a_h, sim.dut->new_al1p2c_sel_f2a_h);
    sim.compare("op_dtype_f2a_h " + phase, sim.dut->orig_op_dtype_f2a_h, sim.dut->new_op_dtype_f2a_h);
    sim.compare("sigs_f2a_h " + phase, sim.dut->orig_sigs_f2a_h, sim.dut->new_sigs_f2a_h);
    sim.compare("cmd_f2a_h " + phase, sim.dut->orig_cmd_f2a_h, sim.dut->new_cmd_f2a_h);
    sim.compare("sin_ediff2_f2a_h " + phase, sim.dut->orig_sin_ediff2_f2a_h, sim.dut->new_sin_ediff2_f2a_h);
    sim.compare("fma_subopc3_f3a_h " + phase, sim.dut->orig_fma_subopc3_f3a_h, sim.dut->new_fma_subopc3_f3a_h);
    sim.compare("s_res_f3a_h " + phase, sim.dut->orig_s_res_f3a_h, sim.dut->new_s_res_f3a_h);
    sim.compare("imxc_sel_f3a_h " + phase, sim.dut->orig_imxc_sel_f3a_h, sim.dut->new_imxc_sel_f3a_h);
    sim.compare("al2c3_neg_cin_ps_f3a_h " + phase, sim.dut->orig_al2c3_neg_cin_ps_f3a_h, sim.dut->new_al2c3_neg_cin_ps_f3a_h);
    sim.compare("al1c3_neg_cin_f3a_h " + phase, sim.dut->orig_al1c3_neg_cin_f3a_h, sim.dut->new_al1c3_neg_cin_f3a_h);
    sim.compare("neg_norm_a3_f4a_h " + phase, sim.dut->orig_neg_norm_a3_f4a_h, sim.dut->new_neg_norm_a3_f4a_h);
    sim.compare("neg_norm_na3_f4a_h " + phase, sim.dut->orig_neg_norm_na3_f4a_h, sim.dut->new_neg_norm_na3_f4a_h);
    sim.compare("clearphps_en_f4a_h " + phase, sim.dut->orig_clearphps_en_f4a_h, sim.dut->new_clearphps_en_f4a_h);
    sim.compare("clear16frnd_nshin_f4a_l " + phase, sim.dut->orig_clear16frnd_nshin_f4a_l, sim.dut->new_clear16frnd_nshin_f4a_l);
    sim.compare("ph_ps_clear_f4a_l " + phase, sim.dut->orig_ph_ps_clear_f4a_l, sim.dut->new_ph_ps_clear_f4a_l);
    sim.compare("elxd_add_in2_mux_sel_f4a_h " + phase, sim.dut->orig_elxd_add_in2_mux_sel_f4a_h, sim.dut->new_elxd_add_in2_mux_sel_f4a_h);
    sim.compare("use_elxd_f4a_h " + phase, sim.dut->orig_use_elxd_f4a_h, sim.dut->new_use_elxd_f4a_h);
    sim.compare("fb_hi_zero_f4a_h " + phase, sim.dut->orig_fb_hi_zero_f4a_h, sim.dut->new_fb_hi_zero_f4a_h);
    sim.compare("enc_nsamt_f4a_h " + phase, sim.dut->orig_enc_nsamt_f4a_h, sim.dut->new_enc_nsamt_f4a_h);
    sim.compare("int_minmax_mux_sel_a_f5a_h " + phase, sim.dut->orig_int_minmax_mux_sel_a_f5a_h, sim.dut->new_int_minmax_mux_sel_a_f5a_h);
    sim.compare("int_minmax_mux_sel_c_f5a_h " + phase, sim.dut->orig_int_minmax_mux_sel_c_f5a_h, sim.dut->new_int_minmax_mux_sel_c_f5a_h);
    sim.compare("rc_ps_f5a_h " + phase, sim.dut->orig_rc_ps_f5a_h, sim.dut->new_rc_ps_f5a_h);
    sim.compare("rc_ph_f5a_h " + phase, sim.dut->orig_rc_ph_f5a_h, sim.dut->new_rc_ph_f5a_h);
    sim.compare("rc6_ps_f5a_h " + phase, sim.dut->orig_rc6_ps_f5a_h, sim.dut->new_rc6_ps_f5a_h);
    sim.compare("rc6_ph_f5a_h " + phase, sim.dut->orig_rc6_ph_f5a_h, sim.dut->new_rc6_ph_f5a_h);
    sim.compare("rc_int32_f5a_h " + phase, sim.dut->orig_rc_int32_f5a_h, sim.dut->new_rc_int32_f5a_h);
    sim.compare("rc_3rmxr_f5a_h " + phase, sim.dut->orig_rc_3rmxr_f5a_h, sim.dut->new_rc_3rmxr_f5a_h);
    sim.compare("rc_rl01_f5a_h " + phase, sim.dut->orig_rc_rl01_f5a_h, sim.dut->new_rc_rl01_f5a_h);
    sim.compare("rm_f5a_h " + phase, sim.dut->orig_rm_f5a_h, sim.dut->new_rm_f5a_h);
    sim.compare("rc_rmm_f5a_h " + phase, sim.dut->orig_rc_rmm_f5a_h, sim.dut->new_rc_rmm_f5a_h);
    sim.compare("rc_rup_f5a_h " + phase, sim.dut->orig_rc_rup_f5a_h, sim.dut->new_rc_rup_f5a_h);
    sim.compare("rc_rne_f5a_h " + phase, sim.dut->orig_rc_rne_f5a_h, sim.dut->new_rc_rne_f5a_h);
    sim.compare("rc_rdn_f5a_h " + phase, sim.dut->orig_rc_rdn_f5a_h, sim.dut->new_rc_rdn_f5a_h);
    sim.compare("rc_res_sign_f5a_l " + phase, sim.dut->orig_rc_res_sign_f5a_l, sim.dut->new_rc_res_sign_f5a_l);
    sim.compare("rc_transrnd_f5a_h " + phase, sim.dut->orig_rc_transrnd_f5a_h, sim.dut->new_rc_transrnd_f5a_h);
    sim.compare("rc_transrnd_log_fma2_f5a_h " + phase, sim.dut->orig_rc_transrnd_log_fma2_f5a_h, sim.dut->new_rc_transrnd_log_fma2_f5a_h);
    sim.compare("nshcen_sel1b_f5a_h " + phase, sim.dut->orig_nshcen_sel1b_f5a_h, sim.dut->new_nshcen_sel1b_f5a_h);
    sim.compare("en_logrrsel1b_f5a_l " + phase, sim.dut->orig_en_logrrsel1b_f5a_l, sim.dut->new_en_logrrsel1b_f5a_l);
    sim.compare("int32_stky2r_f5a_h " + phase, sim.dut->orig_int32_stky2r_f5a_h, sim.dut->new_int32_stky2r_f5a_h);
    sim.compare("mmx_sel_immres_f5a_h " + phase, sim.dut->orig_mmx_sel_immres_f5a_h, sim.dut->new_mmx_sel_immres_f5a_h);
    sim.compare("comp_op_true_f5a_h " + phase, sim.dut->orig_comp_op_true_f5a_h, sim.dut->new_comp_op_true_f5a_h);
    sim.compare("comp_op_f5a_h " + phase, sim.dut->orig_comp_op_f5a_h, sim.dut->new_comp_op_f5a_h);
    sim.compare("trans2_ps_roundup_r_f5a_h " + phase, sim.dut->orig_trans2_ps_roundup_r_f5a_h, sim.dut->new_trans2_ps_roundup_r_f5a_h);
    sim.compare("e_force_to_zero_f5a_h " + phase, sim.dut->orig_e_force_to_zero_f5a_h, sim.dut->new_e_force_to_zero_f5a_h);
    sim.compare("s_res_out_f5a_h " + phase, sim.dut->orig_s_res_out_f5a_h, sim.dut->new_s_res_out_f5a_h);
    sim.compare("op_dtype_f5a_h " + phase, sim.dut->orig_op_dtype_f5a_h, sim.dut->new_op_dtype_f5a_h);
    sim.compare("sigs_f5a_h " + phase, sim.dut->orig_sigs_f5a_h, sim.dut->new_sigs_f5a_h);
    sim.compare("cmd_f5a_h " + phase, sim.dut->orig_cmd_f5a_h, sim.dut->new_cmd_f5a_h);
    sim.compare("sel_gmaxu24_f6a_h " + phase, sim.dut->orig_sel_gmaxu24_f6a_h, sim.dut->new_sel_gmaxu24_f6a_h);
    sim.compare("sel_gmaxu16_f6a_h " + phase, sim.dut->orig_sel_gmaxu16_f6a_h, sim.dut->new_sel_gmaxu16_f6a_h);
    sim.compare("sel_gmaxs16_f6a_h " + phase, sim.dut->orig_sel_gmaxs16_f6a_h, sim.dut->new_sel_gmaxs16_f6a_h);
    sim.compare("sel_gmins16_f6a_h " + phase, sim.dut->orig_sel_gmins16_f6a_h, sim.dut->new_sel_gmins16_f6a_h);
    sim.compare("sel_gmaxs8_f6a_h " + phase, sim.dut->orig_sel_gmaxs8_f6a_h, sim.dut->new_sel_gmaxs8_f6a_h);
    sim.compare("sel_gmaxu2_f6a_h " + phase, sim.dut->orig_sel_gmaxu2_f6a_h, sim.dut->new_sel_gmaxu2_f6a_h);
    sim.compare("sel_gmins8_f6a_h " + phase, sim.dut->orig_sel_gmins8_f6a_h, sim.dut->new_sel_gmins8_f6a_h);
    sim.compare("sel_gmaxu8_f6a_h " + phase, sim.dut->orig_sel_gmaxu8_f6a_h, sim.dut->new_sel_gmaxu8_f6a_h);
    sim.compare("sel_gfxmax_f6a_h " + phase, sim.dut->orig_sel_gfxmax_f6a_h, sim.dut->new_sel_gfxmax_f6a_h);
    sim.compare("sel_psexc_sr_f6a_h " + phase, sim.dut->orig_sel_psexc_sr_f6a_h, sim.dut->new_sel_psexc_sr_f6a_h);
    sim.compare("sel_bit31_one_f6a_h " + phase, sim.dut->orig_sel_bit31_one_f6a_h, sim.dut->new_sel_bit31_one_f6a_h);
    sim.compare("sel_b30to0_ones_f6a_h " + phase, sim.dut->orig_sel_b30to0_ones_f6a_h, sim.dut->new_sel_b30to0_ones_f6a_h);
    sim.compare("sel_zerores_f6a_h " + phase, sim.dut->orig_sel_zerores_f6a_h, sim.dut->new_sel_zerores_f6a_h);
    sim.compare("sel_ps_unf_f6a_h " + phase, sim.dut->orig_sel_ps_unf_f6a_h, sim.dut->new_sel_ps_unf_f6a_h);
    sim.compare("sel_ps_normone_f6a_h " + phase, sim.dut->orig_sel_ps_normone_f6a_h, sim.dut->new_sel_ps_normone_f6a_h);
    sim.compare("sel_ps_inf_f6a_h " + phase, sim.dut->orig_sel_ps_inf_f6a_h, sim.dut->new_sel_ps_inf_f6a_h);
    sim.compare("sel_ps_cqn_f6a_h " + phase, sim.dut->orig_sel_ps_cqn_f6a_h, sim.dut->new_sel_ps_cqn_f6a_h);
    sim.compare("sel_ps_large_f6a_h " + phase, sim.dut->orig_sel_ps_large_f6a_h, sim.dut->new_sel_ps_large_f6a_h);
    sim.compare("sel_phsp_f6a_h " + phase, sim.dut->orig_sel_phsp_f6a_h, sim.dut->new_sel_phsp_f6a_h);
    sim.compare("sel_phhi_sr_f6a_h " + phase, sim.dut->orig_sel_phhi_sr_f6a_h, sim.dut->new_sel_phhi_sr_f6a_h);
    sim.compare("sel_phhi_large_f6a_h " + phase, sim.dut->orig_sel_phhi_large_f6a_h, sim.dut->new_sel_phhi_large_f6a_h);
    sim.compare("sel_phhi_unf_f6a_h " + phase, sim.dut->orig_sel_phhi_unf_f6a_h, sim.dut->new_sel_phhi_unf_f6a_h);
    sim.compare("sel_phhi_infres_f6a_h " + phase, sim.dut->orig_sel_phhi_infres_f6a_h, sim.dut->new_sel_phhi_infres_f6a_h);
    sim.compare("sel_f11inf_f6a_h " + phase, sim.dut->orig_sel_f11inf_f6a_h, sim.dut->new_sel_f11inf_f6a_h);
    sim.compare("sel_f10cqnan_f6a_h " + phase, sim.dut->orig_sel_f10cqnan_f6a_h, sim.dut->new_sel_f10cqnan_f6a_h);
    sim.compare("sel_f11sp_f6a_h " + phase, sim.dut->orig_sel_f11sp_f6a_h, sim.dut->new_sel_f11sp_f6a_h);
    sim.compare("sel_f10sp_f6a_h " + phase, sim.dut->orig_sel_f10sp_f6a_h, sim.dut->new_sel_f10sp_f6a_h);
    sim.compare("sel_f11large_f6a_h " + phase, sim.dut->orig_sel_f11large_f6a_h, sim.dut->new_sel_f11large_f6a_h);
    sim.compare("sel_f10large_f6a_h " + phase, sim.dut->orig_sel_f10large_f6a_h, sim.dut->new_sel_f10large_f6a_h);
    sim.compare("ps_rndovf_f6a_h " + phase, sim.dut->orig_ps_rndovf_f6a_h, sim.dut->new_ps_rndovf_f6a_h);
    sim.compare("ph_rndovf_f6a_h " + phase, sim.dut->orig_ph_rndovf_f6a_h, sim.dut->new_ph_rndovf_f6a_h);
    sim.compare("sel_set_bit0_one_f6a_h " + phase, sim.dut->orig_sel_set_bit0_one_f6a_h, sim.dut->new_sel_set_bit0_one_f6a_h);
    sim.compare("effsub1_set_sign1_f6a_h " + phase, sim.dut->orig_effsub1_set_sign1_f6a_h, sim.dut->new_effsub1_set_sign1_f6a_h);
    sim.compare("normmask1_f6a_h " + phase, sim.dut->orig_normmask1_f6a_h, sim.dut->new_normmask1_f6a_h);
    sim.compare("normmask2_f6a_h " + phase, sim.dut->orig_normmask2_f6a_h, sim.dut->new_normmask2_f6a_h);
    sim.compare("sel_nr1_frcpres_f6a_h " + phase, sim.dut->orig_sel_nr1_frcpres_f6a_h, sim.dut->new_sel_nr1_frcpres_f6a_h);
    sim.compare("trans_sinp2m_f6a_h " + phase, sim.dut->orig_trans_sinp2m_f6a_h, sim.dut->new_trans_sinp2m_f6a_h);
    sim.compare("f10f11mux_sel_f6a_h " + phase, sim.dut->orig_f10f11mux_sel_f6a_h, sim.dut->new_f10f11mux_sel_f6a_h);
    sim.compare("rrmux16l_sel_f6a_l " + phase, sim.dut->orig_rrmux16l_sel_f6a_l, sim.dut->new_rrmux16l_sel_f6a_l);
    sim.compare("sel_classify_out_f6a_h " + phase, sim.dut->orig_sel_classify_out_f6a_h, sim.dut->new_sel_classify_out_f6a_h);
    sim.compare("rrmux32_sel_f6a_h " + phase, sim.dut->orig_rrmux32_sel_f6a_h, sim.dut->new_rrmux32_sel_f6a_h);
    sim.compare("sel_excreshi_f6a_h " + phase, sim.dut->orig_sel_excreshi_f6a_h, sim.dut->new_sel_excreshi_f6a_h);
    sim.compare("sel_excreslo_f6a_h " + phase, sim.dut->orig_sel_excreslo_f6a_h, sim.dut->new_sel_excreslo_f6a_h);
    sim.compare("sel_fmares_hi_f6a_h " + phase, sim.dut->orig_sel_fmares_hi_f6a_h, sim.dut->new_sel_fmares_hi_f6a_h);
    sim.compare("sel_fmares_lo_f6a_h " + phase, sim.dut->orig_sel_fmares_lo_f6a_h, sim.dut->new_sel_fmares_lo_f6a_h);
    sim.compare("cmd_f6a_h " + phase, sim.dut->orig_cmd_f6a_h, sim.dut->new_cmd_f6a_h);
    sim.compare("sigs_f6a_h " + phase, sim.dut->orig_sigs_f6a_h, sim.dut->new_sigs_f6a_h);
    sim.compare("op_dtype_f6a_h " + phase, sim.dut->orig_op_dtype_f6a_h, sim.dut->new_op_dtype_f6a_h);
    sim.compare("classify_f6a_h " + phase, sim.dut->orig_classify_f6a_h, sim.dut->new_classify_f6a_h);
    sim.compare("fma_flags_f6a_h " + phase, sim.dut->orig_fma_flags_f6a_h, sim.dut->new_fma_flags_f6a_h);
    sim.compare("comp_op_true_f6a_h " + phase, sim.dut->orig_comp_op_true_f6a_h, sim.dut->new_comp_op_true_f6a_h);
}

void eval_phase(CosimCtrl<DUT>& sim, bool clk, const std::string& phase) {
    sim.dut->clk_i = clk;
    g_saw_clk_low |= !clk;
    g_saw_clk_high |= clk;
    sim.dut->eval();
    compare_all(sim, phase);
    sim.sim_time++;
    if (sim.sim_time >= sim.max_time) {
        std::printf("TIMEOUT at sim_time=%lu\n", static_cast<unsigned long>(sim.sim_time));
        std::exit(2);
    }
}

void run_cycle(CosimCtrl<DUT>& sim, const std::vector<uint32_t>& words,
               const std::string& label) {
    drive_words(sim, words);
    eval_phase(sim, false, label + " low");
    eval_phase(sim, true, label + " high");
}

}  // namespace

int main(int argc, char** argv) {
    CosimCtrl<DUT> sim(argc, argv);
    sim.max_time = 20000;

    bool saw_reset_asserted = false;
    bool saw_reset_deasserted = false;
    bool saw_all_zero_stimulus = false;
    bool saw_all_one_stimulus = false;

    sim.dut->rst_ni = 0;
    saw_reset_asserted = true;
    run_cycle(sim, filled_words(sim, 0u), "reset");
    sim.dut->rst_ni = 1;
    saw_reset_deasserted = true;
    Verilated::assertOn(true);

    run_cycle(sim, filled_words(sim, 0u), "directed zero");
    saw_all_zero_stimulus = true;
    run_cycle(sim, filled_words(sim, 0xffff'ffffu), "directed one");
    saw_all_one_stimulus = true;

    uint32_t rng = 0x3141'5926u;
    for (int cycle = 0; cycle < 2500; ++cycle) {
        run_cycle(sim, random_words(sim, rng), "random " + std::to_string(cycle));
    }

    sim.check(g_saw_clk_low && g_saw_clk_high, "clock toggled low/high every cycle");
    sim.check(saw_reset_asserted && saw_reset_deasserted, "reset driven asserted and deasserted");
    sim.check(saw_all_zero_stimulus && saw_all_one_stimulus,
              "all packed non-clock inputs driven by all-zero and all-one directed cycles");
    sim.check(sim.comparisons >= 995000,
              "txfmactl_top cosim should generate proportional all-output comparisons");
    return sim.finish();
}
