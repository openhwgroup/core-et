// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_txfmactl_top_tb
  import minion_pkg::*;
  import vpu_pkg::*;
  import vpu_defs_pkg::*;
#(
  parameter int unsigned InputWidth =
      (1) +
      ($bits(vpu_ctrl_sigs_t)) +
      (((TXFMA_RM_SZ-1) - (0) + 1)) +
      (((TRANS_MASK_SIZE-1) - (0) + 1)) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (((TXFMA_EXP_PS_SZ-1) - (0) + 1)) +
      (((TXFMA_EXP_SEXT_PS_SZ-1) - (0) + 1)) +
      (((TXFMA_EXP_SEXT_PS_SZ-1) - (0) + 1)) +
      (((TXFMA_EXP_SEXT_PS_SZ-1) - (0) + 1)) +
      (((TXFMA_EDIFF_OPD_SEL_SZ-1) - (0) + 1)) +
      (((TXFMA_EDIFF_OPD_SEL_SZ-1) - (0) + 1)) +
      (((TXFMA_EDIFF_OPD_SEL_SZ-1) - (0) + 1)) +
      (((TXFMA_EDIFF_OPD_SEL_SZ-1) - (0) + 1)) +
      (((TXFMA_EXP_PS_SZ-1) - (0) + 1)) +
      (1) +
      (1) +
      (1) +
      (((TXFMA_EDIFF_OPD_SEL_SZ-1) - (0) + 1)) +
      (((TXFMA_EXP_SEXT_PS_SZ-1) - (0) + 1)) +
      (((TXFMA_EXP_SEXT_PS_SZ-1) - (0) + 1)) +
      (((TXFMA_EXP_PS_SZ-1) - (0) + 1)) +
      (1) +
      (1) +
      (1) +
      (1) +
      (((TXFMA_LXD_PS_SZ-1) - (0) + 1)) +
      (((TXFMA_WSPS_ETRZ_SZ-1) - (0) + 1)) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1) +
      (1),
  parameter int unsigned InputWords = (InputWidth + 31) / 32
) (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [InputWords*32-1:0] inputs_i,
  output logic [is_trans_rfa_h_W-1:0] orig_is_trans_rfa_h,
  output logic [is_trans_rfa_h_W-1:0] new_is_trans_rfa_h,
  output logic [sel_eblo_ph_exa_h_W-1:0] orig_sel_eblo_ph_exa_h,
  output logic [sel_eblo_ph_exa_h_W-1:0] new_sel_eblo_ph_exa_h,
  output logic [sel_ec5bdet_exa_h_W-1:0] orig_sel_ec5bdet_exa_h,
  output logic [sel_ec5bdet_exa_h_W-1:0] new_sel_ec5bdet_exa_h,
  output logic [elo_eb_en_exa_h_W-1:0] orig_elo_eb_en_exa_h,
  output logic [elo_eb_en_exa_h_W-1:0] new_elo_eb_en_exa_h,
  output logic [ehi_eb_en_exa_h_W-1:0] orig_ehi_eb_en_exa_h,
  output logic [ehi_eb_en_exa_h_W-1:0] new_ehi_eb_en_exa_h,
  output logic [elo_ea_en_exa_h_W-1:0] orig_elo_ea_en_exa_h,
  output logic [elo_ea_en_exa_h_W-1:0] new_elo_ea_en_exa_h,
  output logic [fa_en_exa_h_W-1:0] orig_fa_en_exa_h,
  output logic [fa_en_exa_h_W-1:0] new_fa_en_exa_h,
  output logic [fb_en_exa_h_W-1:0] orig_fb_en_exa_h,
  output logic [fb_en_exa_h_W-1:0] new_fb_en_exa_h,
  output logic [fc_en_exa_h_W-1:0] orig_fc_en_exa_h,
  output logic [fc_en_exa_h_W-1:0] new_fc_en_exa_h,
  output logic [sel_ealo_ph_exa_h_W-1:0] orig_sel_ealo_ph_exa_h,
  output logic [sel_ealo_ph_exa_h_W-1:0] new_sel_ealo_ph_exa_h,
  output logic [sa_hi_en_exa_h_W-1:0] orig_sa_hi_en_exa_h,
  output logic [sa_hi_en_exa_h_W-1:0] new_sa_hi_en_exa_h,
  output logic [sa_lo_en_exa_h_W-1:0] orig_sa_lo_en_exa_h,
  output logic [sa_lo_en_exa_h_W-1:0] new_sa_lo_en_exa_h,
  output logic [sb_hi_en_exa_h_W-1:0] orig_sb_hi_en_exa_h,
  output logic [sb_hi_en_exa_h_W-1:0] new_sb_hi_en_exa_h,
  output logic [sb_lo_en_exa_h_W-1:0] orig_sb_lo_en_exa_h,
  output logic [sb_lo_en_exa_h_W-1:0] new_sb_lo_en_exa_h,
  output logic [sc_en_exa_h_W-1:0] orig_sc_en_exa_h,
  output logic [sc_en_exa_h_W-1:0] new_sc_en_exa_h,
  output logic [ehi_ea_en_exa_h_W-1:0] orig_ehi_ea_en_exa_h,
  output logic [ehi_ea_en_exa_h_W-1:0] new_ehi_ea_en_exa_h,
  output logic [ec_en_exa_h_W-1:0] orig_ec_en_exa_h,
  output logic [ec_en_exa_h_W-1:0] new_ec_en_exa_h,
  output logic [neg_fmaop_exa_h_W-1:0] orig_neg_fmaop_exa_h,
  output logic [neg_fmaop_exa_h_W-1:0] new_neg_fmaop_exa_h,
  output logic [trans_op2_exa_l_W-1:0] orig_trans_op2_exa_l,
  output logic [trans_op2_exa_l_W-1:0] new_trans_op2_exa_l,
  output logic [sa1_nr2_frcpfxp_exa_h_W-1:0] orig_sa1_nr2_frcpfxp_exa_h,
  output logic [sa1_nr2_frcpfxp_exa_h_W-1:0] new_sa1_nr2_frcpfxp_exa_h,
  output logic [sel_sign_sinp2_exa_h_W-1:0] orig_sel_sign_sinp2_exa_h,
  output logic [sel_sign_sinp2_exa_h_W-1:0] new_sel_sign_sinp2_exa_h,
  output logic [sa_zap_exa_l_W-1:0] orig_sa_zap_exa_l,
  output logic [sa_zap_exa_l_W-1:0] new_sa_zap_exa_l,
  output logic [sb_zap_exa_l_W-1:0] orig_sb_zap_exa_l,
  output logic [sb_zap_exa_l_W-1:0] new_sb_zap_exa_l,
  output logic [sel_f10_exa_h_W-1:0] orig_sel_f10_exa_h,
  output logic [sel_f10_exa_h_W-1:0] new_sel_f10_exa_h,
  output logic [sel_f11_exa_h_W-1:0] orig_sel_f11_exa_h,
  output logic [sel_f11_exa_h_W-1:0] new_sel_f11_exa_h,
  output logic [fc_dtps_exa_h_W-1:0] orig_fc_dtps_exa_h,
  output logic [fc_dtps_exa_h_W-1:0] new_fc_dtps_exa_h,
  output logic [sel_snorm_exa_h_W-1:0] orig_sel_snorm_exa_h,
  output logic [sel_snorm_exa_h_W-1:0] new_sel_snorm_exa_h,
  output logic [sel_g1sk2_exa_h_W-1:0] orig_sel_g1sk2_exa_h,
  output logic [sel_g1sk2_exa_h_W-1:0] new_sel_g1sk2_exa_h,
  output logic [sel_g1sk8_exa_h_W-1:0] orig_sel_g1sk8_exa_h,
  output logic [sel_g1sk8_exa_h_W-1:0] new_sel_g1sk8_exa_h,
  output logic [sel_g1sk16_exa_h_W-1:0] orig_sel_g1sk16_exa_h,
  output logic [sel_g1sk16_exa_h_W-1:0] new_sel_g1sk16_exa_h,
  output logic [sel_g1sk24_exa_h_W-1:0] orig_sel_g1sk24_exa_h,
  output logic [sel_g1sk24_exa_h_W-1:0] new_sel_g1sk24_exa_h,
  output logic [sel_xexpfrac_exa_h_W-1:0] orig_sel_xexpfrac_exa_h,
  output logic [sel_xexpfrac_exa_h_W-1:0] new_sel_xexpfrac_exa_h,
  output logic [sel_ebin_hi_exa_h_W-1:0] orig_sel_ebin_hi_exa_h,
  output logic [sel_ebin_hi_exa_h_W-1:0] new_sel_ebin_hi_exa_h,
  output logic [sel_eainmux_exa_h_W-1:0] orig_sel_eainmux_exa_h,
  output logic [sel_eainmux_exa_h_W-1:0] new_sel_eainmux_exa_h,
  output logic [sel_ec5b_exa_h_W-1:0] orig_sel_ec5b_exa_h,
  output logic [sel_ec5b_exa_h_W-1:0] new_sel_ec5b_exa_h,
  output logic [sel_ecin_exa_h_W-1:0] orig_sel_ecin_exa_h,
  output logic [sel_ecin_exa_h_W-1:0] new_sel_ecin_exa_h,
  output logic [sel_sc_exa_h_W-1:0] orig_sel_sc_exa_h,
  output logic [sel_sc_exa_h_W-1:0] new_sel_sc_exa_h,
  output logic [sel_opcgfx_exa_h_W-1:0] orig_sel_opcgfx_exa_h,
  output logic [sel_opcgfx_exa_h_W-1:0] new_sel_opcgfx_exa_h,
  output logic [sel_xopc_exa_h_W-1:0] orig_sel_xopc_exa_h,
  output logic [sel_xopc_exa_h_W-1:0] new_sel_xopc_exa_h,
  output logic [sel_opcfmt_exa_h_W-1:0] orig_sel_opcfmt_exa_h,
  output logic [sel_opcfmt_exa_h_W-1:0] new_sel_opcfmt_exa_h,
  output logic [sel_transfa_exa_h_W-1:0] orig_sel_transfa_exa_h,
  output logic [sel_transfa_exa_h_W-1:0] new_sel_transfa_exa_h,
  output logic [sel_transfb_exa_h_W-1:0] orig_sel_transfb_exa_h,
  output logic [sel_transfb_exa_h_W-1:0] new_sel_transfb_exa_h,
  output logic [sel_opafmt_exa_h_W-1:0] orig_sel_opafmt_exa_h,
  output logic [sel_opafmt_exa_h_W-1:0] new_sel_opafmt_exa_h,
  output logic [sel_opbfmt_exa_h_W-1:0] orig_sel_opbfmt_exa_h,
  output logic [sel_opbfmt_exa_h_W-1:0] new_sel_opbfmt_exa_h,
  output logic [opa_valid_rfa_h_W-1:0] orig_opa_valid_rfa_h,
  output logic [opa_valid_rfa_h_W-1:0] new_opa_valid_rfa_h,
  output logic [opb_valid_rfa_h_W-1:0] orig_opb_valid_rfa_h,
  output logic [opb_valid_rfa_h_W-1:0] new_opb_valid_rfa_h,
  output logic [opc_valid_rfa_h_W-1:0] orig_opc_valid_rfa_h,
  output logic [opc_valid_rfa_h_W-1:0] new_opc_valid_rfa_h,
  output logic [dtps_f0a_h_W-1:0] orig_dtps_f0a_h,
  output logic [dtps_f0a_h_W-1:0] new_dtps_f0a_h,
  output logic [ctrl_valid_f0a_h_W-1:0] orig_ctrl_valid_f0a_h,
  output logic [ctrl_valid_f0a_h_W-1:0] new_ctrl_valid_f0a_h,
  output logic [ctrl_valid_f1a_h_W-1:0] orig_ctrl_valid_f1a_h,
  output logic [ctrl_valid_f1a_h_W-1:0] new_ctrl_valid_f1a_h,
  output logic [ctrl_valid_f2a_h_W-1:0] orig_ctrl_valid_f2a_h,
  output logic [ctrl_valid_f2a_h_W-1:0] new_ctrl_valid_f2a_h,
  output logic [ctrl_valid_f3a_h_W-1:0] orig_ctrl_valid_f3a_h,
  output logic [ctrl_valid_f3a_h_W-1:0] new_ctrl_valid_f3a_h,
  output logic [ctrl_valid_f4a_h_W-1:0] orig_ctrl_valid_f4a_h,
  output logic [ctrl_valid_f4a_h_W-1:0] new_ctrl_valid_f4a_h,
  output logic [ctrl_valid_f5a_h_W-1:0] orig_ctrl_valid_f5a_h,
  output logic [ctrl_valid_f5a_h_W-1:0] new_ctrl_valid_f5a_h,
  output logic [ctrl_valid_f6a_h_W-1:0] orig_ctrl_valid_f6a_h,
  output logic [ctrl_valid_f6a_h_W-1:0] new_ctrl_valid_f6a_h,
  output logic [mul_sign_f0a_h_W-1:0] orig_mul_sign_f0a_h,
  output logic [mul_sign_f0a_h_W-1:0] new_mul_sign_f0a_h,
  output logic [mul_hp_mode_f0a_h_W-1:0] orig_mul_hp_mode_f0a_h,
  output logic [mul_hp_mode_f0a_h_W-1:0] new_mul_hp_mode_f0a_h,
  output logic [eb_hi_k_exa_h_W-1:0] orig_eb_hi_k_exa_h,
  output logic [eb_hi_k_exa_h_W-1:0] new_eb_hi_k_exa_h,
  output logic [use_mul_f0a_h_W-1:0] orig_use_mul_f0a_h,
  output logic [use_mul_f0a_h_W-1:0] new_use_mul_f0a_h,
  output logic [ec_zero_f1a_h_W-1:0] orig_ec_zero_f1a_h,
  output logic [ec_zero_f1a_h_W-1:0] new_ec_zero_f1a_h,
  output logic [ea_hi_zero_f1a_h_W-1:0] orig_ea_hi_zero_f1a_h,
  output logic [ea_hi_zero_f1a_h_W-1:0] new_ea_hi_zero_f1a_h,
  output logic [fc_zero30_8_f1a_h_W-1:0] orig_fc_zero30_8_f1a_h,
  output logic [fc_zero30_8_f1a_h_W-1:0] new_fc_zero30_8_f1a_h,
  output logic [paddin1_sels_f1a_h_W-1:0] orig_paddin1_sels_f1a_h,
  output logic [paddin1_sels_f1a_h_W-1:0] new_paddin1_sels_f1a_h,
  output logic [paddin2_selc_f1a_h_W-1:0] orig_paddin2_selc_f1a_h,
  output logic [paddin2_selc_f1a_h_W-1:0] new_paddin2_selc_f1a_h,
  output logic [paddin2_selfcneg_f1a_h_W-1:0] orig_paddin2_selfcneg_f1a_h,
  output logic [paddin2_selfcneg_f1a_h_W-1:0] new_paddin2_selfcneg_f1a_h,
  output logic [padd_cin_f1a_h_W-1:0] orig_padd_cin_f1a_h,
  output logic [padd_cin_f1a_h_W-1:0] new_padd_cin_f1a_h,
  output logic [sel_famask_f1a_h_W-1:0] orig_sel_famask_f1a_h,
  output logic [sel_famask_f1a_h_W-1:0] new_sel_famask_f1a_h,
  output logic [fa_en_f1a_h_W-1:0] orig_fa_en_f1a_h,
  output logic [fa_en_f1a_h_W-1:0] new_fa_en_f1a_h,
  output logic [cmd_f1a_h_W-1:0] orig_cmd_f1a_h,
  output logic [cmd_f1a_h_W-1:0] new_cmd_f1a_h,
  output logic [sigs_f1a_h_W-1:0] orig_sigs_f1a_h,
  output logic [sigs_f1a_h_W-1:0] new_sigs_f1a_h,
  output logic [op_dtype_f1a_h_W-1:0] orig_op_dtype_f1a_h,
  output logic [op_dtype_f1a_h_W-1:0] new_op_dtype_f1a_h,
  output logic [use_mul_f1a_h_W-1:0] orig_use_mul_f1a_h,
  output logic [use_mul_f1a_h_W-1:0] new_use_mul_f1a_h,
  output logic [ec_zero_f2a_h_W-1:0] orig_ec_zero_f2a_h,
  output logic [ec_zero_f2a_h_W-1:0] new_ec_zero_f2a_h,
  output logic [eprod_hi_zero_f2a_h_W-1:0] orig_eprod_hi_zero_f2a_h,
  output logic [eprod_hi_zero_f2a_h_W-1:0] new_eprod_hi_zero_f2a_h,
  output logic [eprod_lo_zero_f2a_h_W-1:0] orig_eprod_lo_zero_f2a_h,
  output logic [eprod_lo_zero_f2a_h_W-1:0] new_eprod_lo_zero_f2a_h,
  output logic [op3p1c_sel_p_f2a_h_W-1:0] orig_op3p1c_sel_p_f2a_h,
  output logic [op3p1c_sel_p_f2a_h_W-1:0] new_op3p1c_sel_p_f2a_h,
  output logic [op3p1c_sel_r_f2a_h_W-1:0] orig_op3p1c_sel_r_f2a_h,
  output logic [op3p1c_sel_r_f2a_h_W-1:0] new_op3p1c_sel_r_f2a_h,
  output logic [op3g_sel_f2a_h_W-1:0] orig_op3g_sel_f2a_h,
  output logic [op3g_sel_f2a_h_W-1:0] new_op3g_sel_f2a_h,
  output logic [rm_rne_f2a_h_W-1:0] orig_rm_rne_f2a_h,
  output logic [rm_rne_f2a_h_W-1:0] new_rm_rne_f2a_h,
  output logic [rm_rmm_f2a_h_W-1:0] orig_rm_rmm_f2a_h,
  output logic [rm_rmm_f2a_h_W-1:0] new_rm_rmm_f2a_h,
  output logic [frnd_rupnsc_f2a_h_W-1:0] orig_frnd_rupnsc_f2a_h,
  output logic [frnd_rupnsc_f2a_h_W-1:0] new_frnd_rupnsc_f2a_h,
  output logic [frnd_rdnsc_f2a_h_W-1:0] orig_frnd_rdnsc_f2a_h,
  output logic [frnd_rdnsc_f2a_h_W-1:0] new_frnd_rdnsc_f2a_h,
  output logic [frnd_t1_f2a_h_W-1:0] orig_frnd_t1_f2a_h,
  output logic [frnd_t1_f2a_h_W-1:0] new_frnd_t1_f2a_h,
  output logic [frnd_t2_f2a_h_W-1:0] orig_frnd_t2_f2a_h,
  output logic [frnd_t2_f2a_h_W-1:0] new_frnd_t2_f2a_h,
  output logic [al1c_zap_f2a_h_W-1:0] orig_al1c_zap_f2a_h,
  output logic [al1c_zap_f2a_h_W-1:0] new_al1c_zap_f2a_h,
  output logic [op1c_log_mul_clr_f2a_h_W-1:0] orig_op1c_log_mul_clr_f2a_h,
  output logic [op1c_log_mul_clr_f2a_h_W-1:0] new_op1c_log_mul_clr_f2a_h,
  output logic [negop1c_int_logmul_clr_f2a_h_W-1:0] orig_negop1c_int_logmul_clr_f2a_h,
  output logic [negop1c_int_logmul_clr_f2a_h_W-1:0] new_negop1c_int_logmul_clr_f2a_h,
  output logic [psc_op1_clr_f2a_h_W-1:0] orig_psc_op1_clr_f2a_h,
  output logic [psc_op1_clr_f2a_h_W-1:0] new_psc_op1_clr_f2a_h,
  output logic [align2_zap_f2a_h_W-1:0] orig_align2_zap_f2a_h,
  output logic [align2_zap_f2a_h_W-1:0] new_align2_zap_f2a_h,
  output logic [align2c_neg_f2a_h_W-1:0] orig_align2c_neg_f2a_h,
  output logic [align2c_neg_f2a_h_W-1:0] new_align2c_neg_f2a_h,
  output logic [trans_exp_fma2_exc_f2a_h_W-1:0] orig_trans_exp_fma2_exc_f2a_h,
  output logic [trans_exp_fma2_exc_f2a_h_W-1:0] new_trans_exp_fma2_exc_f2a_h,
  output logic [al1p1c_sel_f2a_h_W-1:0] orig_al1p1c_sel_f2a_h,
  output logic [al1p1c_sel_f2a_h_W-1:0] new_al1p1c_sel_f2a_h,
  output logic [op2_sxtto_g_f2a_h_W-1:0] orig_op2_sxtto_g_f2a_h,
  output logic [op2_sxtto_g_f2a_h_W-1:0] new_op2_sxtto_g_f2a_h,
  output logic [op3p3spc_sel_f2a_h_W-1:0] orig_op3p3spc_sel_f2a_h,
  output logic [op3p3spc_sel_f2a_h_W-1:0] new_op3p3spc_sel_f2a_h,
  output logic [op3p2c_sel_f2a_h_W-1:0] orig_op3p2c_sel_f2a_h,
  output logic [op3p2c_sel_f2a_h_W-1:0] new_op3p2c_sel_f2a_h,
  output logic [ugfxrplc_sel_f2a_h_W-1:0] orig_ugfxrplc_sel_f2a_h,
  output logic [ugfxrplc_sel_f2a_h_W-1:0] new_ugfxrplc_sel_f2a_h,
  output logic [sgfxrplc_sel_f2a_h_W-1:0] orig_sgfxrplc_sel_f2a_h,
  output logic [sgfxrplc_sel_f2a_h_W-1:0] new_sgfxrplc_sel_f2a_h,
  output logic [icgfxc_sel_f2a_h_W-1:0] orig_icgfxc_sel_f2a_h,
  output logic [icgfxc_sel_f2a_h_W-1:0] new_icgfxc_sel_f2a_h,
  output logic [al1znp2c_sel_f2a_h_W-1:0] orig_al1znp2c_sel_f2a_h,
  output logic [al1znp2c_sel_f2a_h_W-1:0] new_al1znp2c_sel_f2a_h,
  output logic [al2c_sel_f2a_h_W-1:0] orig_al2c_sel_f2a_h,
  output logic [al2c_sel_f2a_h_W-1:0] new_al2c_sel_f2a_h,
  output logic [al1p2c_sel_f2a_h_W-1:0] orig_al1p2c_sel_f2a_h,
  output logic [al1p2c_sel_f2a_h_W-1:0] new_al1p2c_sel_f2a_h,
  output logic [op_dtype_f2a_h_W-1:0] orig_op_dtype_f2a_h,
  output logic [op_dtype_f2a_h_W-1:0] new_op_dtype_f2a_h,
  output logic [sigs_f2a_h_W-1:0] orig_sigs_f2a_h,
  output logic [sigs_f2a_h_W-1:0] new_sigs_f2a_h,
  output logic [cmd_f2a_h_W-1:0] orig_cmd_f2a_h,
  output logic [cmd_f2a_h_W-1:0] new_cmd_f2a_h,
  output logic [sin_ediff2_f2a_h_W-1:0] orig_sin_ediff2_f2a_h,
  output logic [sin_ediff2_f2a_h_W-1:0] new_sin_ediff2_f2a_h,
  output logic [fma_subopc3_f3a_h_W-1:0] orig_fma_subopc3_f3a_h,
  output logic [fma_subopc3_f3a_h_W-1:0] new_fma_subopc3_f3a_h,
  output logic [s_res_f3a_h_W-1:0] orig_s_res_f3a_h,
  output logic [s_res_f3a_h_W-1:0] new_s_res_f3a_h,
  output logic [imxc_sel_f3a_h_W-1:0] orig_imxc_sel_f3a_h,
  output logic [imxc_sel_f3a_h_W-1:0] new_imxc_sel_f3a_h,
  output logic [al2c3_neg_cin_ps_f3a_h_W-1:0] orig_al2c3_neg_cin_ps_f3a_h,
  output logic [al2c3_neg_cin_ps_f3a_h_W-1:0] new_al2c3_neg_cin_ps_f3a_h,
  output logic [al1c3_neg_cin_f3a_h_W-1:0] orig_al1c3_neg_cin_f3a_h,
  output logic [al1c3_neg_cin_f3a_h_W-1:0] new_al1c3_neg_cin_f3a_h,
  output logic [neg_norm_a3_f4a_h_W-1:0] orig_neg_norm_a3_f4a_h,
  output logic [neg_norm_a3_f4a_h_W-1:0] new_neg_norm_a3_f4a_h,
  output logic [neg_norm_na3_f4a_h_W-1:0] orig_neg_norm_na3_f4a_h,
  output logic [neg_norm_na3_f4a_h_W-1:0] new_neg_norm_na3_f4a_h,
  output logic [clearphps_en_f4a_h_W-1:0] orig_clearphps_en_f4a_h,
  output logic [clearphps_en_f4a_h_W-1:0] new_clearphps_en_f4a_h,
  output logic [clear16frnd_nshin_f4a_l_W-1:0] orig_clear16frnd_nshin_f4a_l,
  output logic [clear16frnd_nshin_f4a_l_W-1:0] new_clear16frnd_nshin_f4a_l,
  output logic [ph_ps_clear_f4a_l_W-1:0] orig_ph_ps_clear_f4a_l,
  output logic [ph_ps_clear_f4a_l_W-1:0] new_ph_ps_clear_f4a_l,
  output logic [elxd_add_in2_mux_sel_f4a_h_W-1:0] orig_elxd_add_in2_mux_sel_f4a_h,
  output logic [elxd_add_in2_mux_sel_f4a_h_W-1:0] new_elxd_add_in2_mux_sel_f4a_h,
  output logic [use_elxd_f4a_h_W-1:0] orig_use_elxd_f4a_h,
  output logic [use_elxd_f4a_h_W-1:0] new_use_elxd_f4a_h,
  output logic [fb_hi_zero_f4a_h_W-1:0] orig_fb_hi_zero_f4a_h,
  output logic [fb_hi_zero_f4a_h_W-1:0] new_fb_hi_zero_f4a_h,
  output logic [enc_nsamt_f4a_h_W-1:0] orig_enc_nsamt_f4a_h,
  output logic [enc_nsamt_f4a_h_W-1:0] new_enc_nsamt_f4a_h,
  output logic [int_minmax_mux_sel_a_f5a_h_W-1:0] orig_int_minmax_mux_sel_a_f5a_h,
  output logic [int_minmax_mux_sel_a_f5a_h_W-1:0] new_int_minmax_mux_sel_a_f5a_h,
  output logic [int_minmax_mux_sel_c_f5a_h_W-1:0] orig_int_minmax_mux_sel_c_f5a_h,
  output logic [int_minmax_mux_sel_c_f5a_h_W-1:0] new_int_minmax_mux_sel_c_f5a_h,
  output logic [rc_ps_f5a_h_W-1:0] orig_rc_ps_f5a_h,
  output logic [rc_ps_f5a_h_W-1:0] new_rc_ps_f5a_h,
  output logic [rc_ph_f5a_h_W-1:0] orig_rc_ph_f5a_h,
  output logic [rc_ph_f5a_h_W-1:0] new_rc_ph_f5a_h,
  output logic [rc6_ps_f5a_h_W-1:0] orig_rc6_ps_f5a_h,
  output logic [rc6_ps_f5a_h_W-1:0] new_rc6_ps_f5a_h,
  output logic [rc6_ph_f5a_h_W-1:0] orig_rc6_ph_f5a_h,
  output logic [rc6_ph_f5a_h_W-1:0] new_rc6_ph_f5a_h,
  output logic [rc_int32_f5a_h_W-1:0] orig_rc_int32_f5a_h,
  output logic [rc_int32_f5a_h_W-1:0] new_rc_int32_f5a_h,
  output logic [rc_3rmxr_f5a_h_W-1:0] orig_rc_3rmxr_f5a_h,
  output logic [rc_3rmxr_f5a_h_W-1:0] new_rc_3rmxr_f5a_h,
  output logic [rc_rl01_f5a_h_W-1:0] orig_rc_rl01_f5a_h,
  output logic [rc_rl01_f5a_h_W-1:0] new_rc_rl01_f5a_h,
  output logic [rm_f5a_h_W-1:0] orig_rm_f5a_h,
  output logic [rm_f5a_h_W-1:0] new_rm_f5a_h,
  output logic [rc_rmm_f5a_h_W-1:0] orig_rc_rmm_f5a_h,
  output logic [rc_rmm_f5a_h_W-1:0] new_rc_rmm_f5a_h,
  output logic [rc_rup_f5a_h_W-1:0] orig_rc_rup_f5a_h,
  output logic [rc_rup_f5a_h_W-1:0] new_rc_rup_f5a_h,
  output logic [rc_rne_f5a_h_W-1:0] orig_rc_rne_f5a_h,
  output logic [rc_rne_f5a_h_W-1:0] new_rc_rne_f5a_h,
  output logic [rc_rdn_f5a_h_W-1:0] orig_rc_rdn_f5a_h,
  output logic [rc_rdn_f5a_h_W-1:0] new_rc_rdn_f5a_h,
  output logic [rc_res_sign_f5a_l_W-1:0] orig_rc_res_sign_f5a_l,
  output logic [rc_res_sign_f5a_l_W-1:0] new_rc_res_sign_f5a_l,
  output logic [rc_transrnd_f5a_h_W-1:0] orig_rc_transrnd_f5a_h,
  output logic [rc_transrnd_f5a_h_W-1:0] new_rc_transrnd_f5a_h,
  output logic [rc_transrnd_log_fma2_f5a_h_W-1:0] orig_rc_transrnd_log_fma2_f5a_h,
  output logic [rc_transrnd_log_fma2_f5a_h_W-1:0] new_rc_transrnd_log_fma2_f5a_h,
  output logic [nshcen_sel1b_f5a_h_W-1:0] orig_nshcen_sel1b_f5a_h,
  output logic [nshcen_sel1b_f5a_h_W-1:0] new_nshcen_sel1b_f5a_h,
  output logic [en_logrrsel1b_f5a_l_W-1:0] orig_en_logrrsel1b_f5a_l,
  output logic [en_logrrsel1b_f5a_l_W-1:0] new_en_logrrsel1b_f5a_l,
  output logic [int32_stky2r_f5a_h_W-1:0] orig_int32_stky2r_f5a_h,
  output logic [int32_stky2r_f5a_h_W-1:0] new_int32_stky2r_f5a_h,
  output logic [mmx_sel_immres_f5a_h_W-1:0] orig_mmx_sel_immres_f5a_h,
  output logic [mmx_sel_immres_f5a_h_W-1:0] new_mmx_sel_immres_f5a_h,
  output logic [comp_op_true_f5a_h_W-1:0] orig_comp_op_true_f5a_h,
  output logic [comp_op_true_f5a_h_W-1:0] new_comp_op_true_f5a_h,
  output logic [comp_op_f5a_h_W-1:0] orig_comp_op_f5a_h,
  output logic [comp_op_f5a_h_W-1:0] new_comp_op_f5a_h,
  output logic [trans2_ps_roundup_r_f5a_h_W-1:0] orig_trans2_ps_roundup_r_f5a_h,
  output logic [trans2_ps_roundup_r_f5a_h_W-1:0] new_trans2_ps_roundup_r_f5a_h,
  output logic [e_force_to_zero_f5a_h_W-1:0] orig_e_force_to_zero_f5a_h,
  output logic [e_force_to_zero_f5a_h_W-1:0] new_e_force_to_zero_f5a_h,
  output logic [s_res_out_f5a_h_W-1:0] orig_s_res_out_f5a_h,
  output logic [s_res_out_f5a_h_W-1:0] new_s_res_out_f5a_h,
  output logic [op_dtype_f5a_h_W-1:0] orig_op_dtype_f5a_h,
  output logic [op_dtype_f5a_h_W-1:0] new_op_dtype_f5a_h,
  output logic [sigs_f5a_h_W-1:0] orig_sigs_f5a_h,
  output logic [sigs_f5a_h_W-1:0] new_sigs_f5a_h,
  output logic [cmd_f5a_h_W-1:0] orig_cmd_f5a_h,
  output logic [cmd_f5a_h_W-1:0] new_cmd_f5a_h,
  output logic [sel_gmaxu24_f6a_h_W-1:0] orig_sel_gmaxu24_f6a_h,
  output logic [sel_gmaxu24_f6a_h_W-1:0] new_sel_gmaxu24_f6a_h,
  output logic [sel_gmaxu16_f6a_h_W-1:0] orig_sel_gmaxu16_f6a_h,
  output logic [sel_gmaxu16_f6a_h_W-1:0] new_sel_gmaxu16_f6a_h,
  output logic [sel_gmaxs16_f6a_h_W-1:0] orig_sel_gmaxs16_f6a_h,
  output logic [sel_gmaxs16_f6a_h_W-1:0] new_sel_gmaxs16_f6a_h,
  output logic [sel_gmins16_f6a_h_W-1:0] orig_sel_gmins16_f6a_h,
  output logic [sel_gmins16_f6a_h_W-1:0] new_sel_gmins16_f6a_h,
  output logic [sel_gmaxs8_f6a_h_W-1:0] orig_sel_gmaxs8_f6a_h,
  output logic [sel_gmaxs8_f6a_h_W-1:0] new_sel_gmaxs8_f6a_h,
  output logic [sel_gmaxu2_f6a_h_W-1:0] orig_sel_gmaxu2_f6a_h,
  output logic [sel_gmaxu2_f6a_h_W-1:0] new_sel_gmaxu2_f6a_h,
  output logic [sel_gmins8_f6a_h_W-1:0] orig_sel_gmins8_f6a_h,
  output logic [sel_gmins8_f6a_h_W-1:0] new_sel_gmins8_f6a_h,
  output logic [sel_gmaxu8_f6a_h_W-1:0] orig_sel_gmaxu8_f6a_h,
  output logic [sel_gmaxu8_f6a_h_W-1:0] new_sel_gmaxu8_f6a_h,
  output logic [sel_gfxmax_f6a_h_W-1:0] orig_sel_gfxmax_f6a_h,
  output logic [sel_gfxmax_f6a_h_W-1:0] new_sel_gfxmax_f6a_h,
  output logic [sel_psexc_sr_f6a_h_W-1:0] orig_sel_psexc_sr_f6a_h,
  output logic [sel_psexc_sr_f6a_h_W-1:0] new_sel_psexc_sr_f6a_h,
  output logic [sel_bit31_one_f6a_h_W-1:0] orig_sel_bit31_one_f6a_h,
  output logic [sel_bit31_one_f6a_h_W-1:0] new_sel_bit31_one_f6a_h,
  output logic [sel_b30to0_ones_f6a_h_W-1:0] orig_sel_b30to0_ones_f6a_h,
  output logic [sel_b30to0_ones_f6a_h_W-1:0] new_sel_b30to0_ones_f6a_h,
  output logic [sel_zerores_f6a_h_W-1:0] orig_sel_zerores_f6a_h,
  output logic [sel_zerores_f6a_h_W-1:0] new_sel_zerores_f6a_h,
  output logic [sel_ps_unf_f6a_h_W-1:0] orig_sel_ps_unf_f6a_h,
  output logic [sel_ps_unf_f6a_h_W-1:0] new_sel_ps_unf_f6a_h,
  output logic [sel_ps_normone_f6a_h_W-1:0] orig_sel_ps_normone_f6a_h,
  output logic [sel_ps_normone_f6a_h_W-1:0] new_sel_ps_normone_f6a_h,
  output logic [sel_ps_inf_f6a_h_W-1:0] orig_sel_ps_inf_f6a_h,
  output logic [sel_ps_inf_f6a_h_W-1:0] new_sel_ps_inf_f6a_h,
  output logic [sel_ps_cqn_f6a_h_W-1:0] orig_sel_ps_cqn_f6a_h,
  output logic [sel_ps_cqn_f6a_h_W-1:0] new_sel_ps_cqn_f6a_h,
  output logic [sel_ps_large_f6a_h_W-1:0] orig_sel_ps_large_f6a_h,
  output logic [sel_ps_large_f6a_h_W-1:0] new_sel_ps_large_f6a_h,
  output logic [sel_phsp_f6a_h_W-1:0] orig_sel_phsp_f6a_h,
  output logic [sel_phsp_f6a_h_W-1:0] new_sel_phsp_f6a_h,
  output logic [sel_phhi_sr_f6a_h_W-1:0] orig_sel_phhi_sr_f6a_h,
  output logic [sel_phhi_sr_f6a_h_W-1:0] new_sel_phhi_sr_f6a_h,
  output logic [sel_phhi_large_f6a_h_W-1:0] orig_sel_phhi_large_f6a_h,
  output logic [sel_phhi_large_f6a_h_W-1:0] new_sel_phhi_large_f6a_h,
  output logic [sel_phhi_unf_f6a_h_W-1:0] orig_sel_phhi_unf_f6a_h,
  output logic [sel_phhi_unf_f6a_h_W-1:0] new_sel_phhi_unf_f6a_h,
  output logic [sel_phhi_infres_f6a_h_W-1:0] orig_sel_phhi_infres_f6a_h,
  output logic [sel_phhi_infres_f6a_h_W-1:0] new_sel_phhi_infres_f6a_h,
  output logic [sel_f11inf_f6a_h_W-1:0] orig_sel_f11inf_f6a_h,
  output logic [sel_f11inf_f6a_h_W-1:0] new_sel_f11inf_f6a_h,
  output logic [sel_f10cqnan_f6a_h_W-1:0] orig_sel_f10cqnan_f6a_h,
  output logic [sel_f10cqnan_f6a_h_W-1:0] new_sel_f10cqnan_f6a_h,
  output logic [sel_f11sp_f6a_h_W-1:0] orig_sel_f11sp_f6a_h,
  output logic [sel_f11sp_f6a_h_W-1:0] new_sel_f11sp_f6a_h,
  output logic [sel_f10sp_f6a_h_W-1:0] orig_sel_f10sp_f6a_h,
  output logic [sel_f10sp_f6a_h_W-1:0] new_sel_f10sp_f6a_h,
  output logic [sel_f11large_f6a_h_W-1:0] orig_sel_f11large_f6a_h,
  output logic [sel_f11large_f6a_h_W-1:0] new_sel_f11large_f6a_h,
  output logic [sel_f10large_f6a_h_W-1:0] orig_sel_f10large_f6a_h,
  output logic [sel_f10large_f6a_h_W-1:0] new_sel_f10large_f6a_h,
  output logic [ps_rndovf_f6a_h_W-1:0] orig_ps_rndovf_f6a_h,
  output logic [ps_rndovf_f6a_h_W-1:0] new_ps_rndovf_f6a_h,
  output logic [ph_rndovf_f6a_h_W-1:0] orig_ph_rndovf_f6a_h,
  output logic [ph_rndovf_f6a_h_W-1:0] new_ph_rndovf_f6a_h,
  output logic [sel_set_bit0_one_f6a_h_W-1:0] orig_sel_set_bit0_one_f6a_h,
  output logic [sel_set_bit0_one_f6a_h_W-1:0] new_sel_set_bit0_one_f6a_h,
  output logic [effsub1_set_sign1_f6a_h_W-1:0] orig_effsub1_set_sign1_f6a_h,
  output logic [effsub1_set_sign1_f6a_h_W-1:0] new_effsub1_set_sign1_f6a_h,
  output logic [normmask1_f6a_h_W-1:0] orig_normmask1_f6a_h,
  output logic [normmask1_f6a_h_W-1:0] new_normmask1_f6a_h,
  output logic [normmask2_f6a_h_W-1:0] orig_normmask2_f6a_h,
  output logic [normmask2_f6a_h_W-1:0] new_normmask2_f6a_h,
  output logic [sel_nr1_frcpres_f6a_h_W-1:0] orig_sel_nr1_frcpres_f6a_h,
  output logic [sel_nr1_frcpres_f6a_h_W-1:0] new_sel_nr1_frcpres_f6a_h,
  output logic [trans_sinp2m_f6a_h_W-1:0] orig_trans_sinp2m_f6a_h,
  output logic [trans_sinp2m_f6a_h_W-1:0] new_trans_sinp2m_f6a_h,
  output logic [f10f11mux_sel_f6a_h_W-1:0] orig_f10f11mux_sel_f6a_h,
  output logic [f10f11mux_sel_f6a_h_W-1:0] new_f10f11mux_sel_f6a_h,
  output logic [rrmux16l_sel_f6a_l_W-1:0] orig_rrmux16l_sel_f6a_l,
  output logic [rrmux16l_sel_f6a_l_W-1:0] new_rrmux16l_sel_f6a_l,
  output logic [sel_classify_out_f6a_h_W-1:0] orig_sel_classify_out_f6a_h,
  output logic [sel_classify_out_f6a_h_W-1:0] new_sel_classify_out_f6a_h,
  output logic [rrmux32_sel_f6a_h_W-1:0] orig_rrmux32_sel_f6a_h,
  output logic [rrmux32_sel_f6a_h_W-1:0] new_rrmux32_sel_f6a_h,
  output logic [sel_excreshi_f6a_h_W-1:0] orig_sel_excreshi_f6a_h,
  output logic [sel_excreshi_f6a_h_W-1:0] new_sel_excreshi_f6a_h,
  output logic [sel_excreslo_f6a_h_W-1:0] orig_sel_excreslo_f6a_h,
  output logic [sel_excreslo_f6a_h_W-1:0] new_sel_excreslo_f6a_h,
  output logic [sel_fmares_hi_f6a_h_W-1:0] orig_sel_fmares_hi_f6a_h,
  output logic [sel_fmares_hi_f6a_h_W-1:0] new_sel_fmares_hi_f6a_h,
  output logic [sel_fmares_lo_f6a_h_W-1:0] orig_sel_fmares_lo_f6a_h,
  output logic [sel_fmares_lo_f6a_h_W-1:0] new_sel_fmares_lo_f6a_h,
  output logic [cmd_f6a_h_W-1:0] orig_cmd_f6a_h,
  output logic [cmd_f6a_h_W-1:0] new_cmd_f6a_h,
  output logic [sigs_f6a_h_W-1:0] orig_sigs_f6a_h,
  output logic [sigs_f6a_h_W-1:0] new_sigs_f6a_h,
  output logic [op_dtype_f6a_h_W-1:0] orig_op_dtype_f6a_h,
  output logic [op_dtype_f6a_h_W-1:0] new_op_dtype_f6a_h,
  output logic [classify_f6a_h_W-1:0] orig_classify_f6a_h,
  output logic [classify_f6a_h_W-1:0] new_classify_f6a_h,
  output logic [fma_flags_f6a_h_W-1:0] orig_fma_flags_f6a_h,
  output logic [fma_flags_f6a_h_W-1:0] new_fma_flags_f6a_h,
  output logic [comp_op_true_f6a_h_W-1:0] orig_comp_op_true_f6a_h,
  output logic [comp_op_true_f6a_h_W-1:0] new_comp_op_true_f6a_h
);

  generate
    if (InputWords * 32 > InputWidth) begin : gen_input_padding_used
      /* verilator lint_off UNUSEDSIGNAL */  // Packed stimulus words are rounded to 32 bits; high padding bits are intentionally ignored.
      logic unused_input_padding;
      assign unused_input_padding = ^inputs_i[InputWords*32-1:InputWidth];
      /* verilator lint_on UNUSEDSIGNAL */
    end
  endgenerate

  localparam int unsigned in_valid_exa_h_W = 1;
  localparam int unsigned sigs_in_exa_h_W = $bits(vpu_ctrl_sigs_t);
  localparam int unsigned rm_in_exa_h_W = ((TXFMA_RM_SZ-1) - (0) + 1);
  localparam int unsigned trans_mask_exa_h_W = ((TRANS_MASK_SIZE-1) - (0) + 1);
  localparam int unsigned trans_taylor_exa_h_W = 1;
  localparam int unsigned trans_exp_fma2_exc_exa_h_W = 1;
  localparam int unsigned use_prev_sigs_exa_h_W = 1;
  localparam int unsigned sa_hi_f0a_h_W = 1;
  localparam int unsigned sa_lo_f0a_h_W = 1;
  localparam int unsigned sb_hi_f0a_h_W = 1;
  localparam int unsigned sb_lo_f0a_h_W = 1;
  localparam int unsigned sc_f0a_h_W = 1;
  localparam int unsigned ea_hi_zero_f0a_l_W = 1;
  localparam int unsigned ea_hi_all1_f0a_h_W = 1;
  localparam int unsigned ea_lo_zero_f0a_l_W = 1;
  localparam int unsigned ea_lo_all1_f0a_h_W = 1;
  localparam int unsigned eb_hi_zero_f0a_l_W = 1;
  localparam int unsigned eb_hi_all1_f0a_h_W = 1;
  localparam int unsigned eb_lo_zero_f0a_l_W = 1;
  localparam int unsigned eb_lo_all1_f0a_h_W = 1;
  localparam int unsigned ec_zero_f0a_l_W = 1;
  localparam int unsigned ec_all1_f0a_h_W = 1;
  localparam int unsigned eb_eql_127_f0a_h_W = 1;
  localparam int unsigned eb_eql_126_f0a_h_W = 1;
  localparam int unsigned eb_eql_125_f0a_h_W = 1;
  localparam int unsigned fcf10_zero_f0a_l_W = 1;
  localparam int unsigned fcf11_zero_f0a_l_W = 1;
  localparam int unsigned fcps_zero_f0a_h_W = 1;
  localparam int unsigned fcph_zero_f0a_l_W = 1;
  localparam int unsigned fcnorm_zero_29_8_f0a_h_W = 1;
  localparam int unsigned fcnorm16_eq_1_f0a_h_W = 1;
  localparam int unsigned fcnorm8_eq_1_f0a_h_W = 1;
  localparam int unsigned fcnorm_30_f0a_h_W = 1;
  localparam int unsigned faph_hi_b0_f0a_h_W = 1;
  localparam int unsigned fbph_hi_b0_f0a_h_W = 1;
  localparam int unsigned fcph_b0_f0a_h_W = 1;
  localparam int unsigned fcf11_b0_f0a_h_W = 1;
  localparam int unsigned fcf10_b0_f0a_h_W = 1;
  localparam int unsigned fcn8_one_30_24_f0a_h_W = 1;
  localparam int unsigned fcn10_one_30_22_f0a_h_W = 1;
  localparam int unsigned fcn16_one_30_16_f0a_h_W = 1;
  localparam int unsigned fcn24_one_30_8_f0a_h_W = 1;
  localparam int unsigned faps_zero_f0a_l_W = 1;
  localparam int unsigned fbps_zero_f0a_l_W = 1;
  localparam int unsigned faph_hi_zero_f0a_l_W = 1;
  localparam int unsigned fbph_hi_zero_f0a_l_W = 1;
  localparam int unsigned faph_lo_zero_f0a_l_W = 1;
  localparam int unsigned fbph_lo_zero_f0a_l_W = 1;
  localparam int unsigned faph_lo_b0_f0a_h_W = 1;
  localparam int unsigned fbph_lo_b0_f0a_h_W = 1;
  localparam int unsigned faps_b0_f0a_h_W = 1;
  localparam int unsigned fbps_b0_f0a_h_W = 1;
  localparam int unsigned fc_31_f0a_h_W = 1;
  localparam int unsigned fcps_b0_f0a_h_W = 1;
  localparam int unsigned ea_hi_f1a_h_W = ((TXFMA_EXP_PS_SZ-1) - (0) + 1);
  localparam int unsigned ediff1_f2a_h_W = ((TXFMA_EXP_SEXT_PS_SZ-1) - (0) + 1);
  localparam int unsigned ediff1_abs_f2a_h_W = ((TXFMA_EXP_SEXT_PS_SZ-1) - (0) + 1);
  localparam int unsigned ediff2_abs_f2a_h_W = ((TXFMA_EXP_SEXT_PS_SZ-1) - (0) + 1);
  localparam int unsigned ediff1_sel_f2a_h_W = ((TXFMA_EDIFF_OPD_SEL_SZ-1) - (0) + 1);
  localparam int unsigned ediff2_sel_f2a_h_W = ((TXFMA_EDIFF_OPD_SEL_SZ-1) - (0) + 1);
  localparam int unsigned align1_mux_sel_f2a_h_W = ((TXFMA_EDIFF_OPD_SEL_SZ-1) - (0) + 1);
  localparam int unsigned align2_mux_sel_f2a_h_W = ((TXFMA_EDIFF_OPD_SEL_SZ-1) - (0) + 1);
  localparam int unsigned ec_f2a_h_W = ((TXFMA_EXP_PS_SZ-1) - (0) + 1);
  localparam int unsigned fc_b0_f2a_h_W = 1;
  localparam int unsigned e_sticky1_f2a_h_W = 1;
  localparam int unsigned e_sticky2_f2a_h_W = 1;
  localparam int unsigned opd3_mux_sel_f2a_h_W = ((TXFMA_EDIFF_OPD_SEL_SZ-1) - (0) + 1);
  localparam int unsigned ediff1_f3a_h_W = ((TXFMA_EXP_SEXT_PS_SZ-1) - (0) + 1);
  localparam int unsigned ediff2_f3a_h_W = ((TXFMA_EXP_SEXT_PS_SZ-1) - (0) + 1);
  localparam int unsigned ec_f3a_h_W = ((TXFMA_EXP_PS_SZ-1) - (0) + 1);
  localparam int unsigned e_sticky1_f3a_h_W = 1;
  localparam int unsigned prodsum_a2_f3a_h_W = 1;
  localparam int unsigned fround_rbit_f3a_h_W = 1;
  localparam int unsigned ph_comb_stiky_f4a_h_W = 1;
  localparam int unsigned elxd_f4a_h_W = ((TXFMA_LXD_PS_SZ-1) - (0) + 1);
  localparam int unsigned etrz50_f4a_h_W = ((TXFMA_WSPS_ETRZ_SZ-1) - (0) + 1);
  localparam int unsigned stky2g_f5a_h_W = 1;
  localparam int unsigned s_res_f5a_h_W = 1;
  localparam int unsigned comb_add_a3_f5a_h_W = 1;
  localparam int unsigned ediff1_zap_f5a_h_W = 1;
  localparam int unsigned rmode_f6a_h_W = 1;
  localparam int unsigned exp_res_zero_f6a_h_W = 1;
  localparam int unsigned s_res_f6a_h_W = 1;
  localparam int unsigned ph_stky2l_f6a_h_W = 1;
  localparam int unsigned ps_stky2l_f6a_h_W = 1;
  localparam int unsigned int32_stky2l_f6a_h_W = 1;
  localparam int unsigned pot_ovf_ps_f6a_h_W = 1;
  localparam int unsigned def_ovf_ps_f6a_h_W = 1;
  localparam int unsigned pot_unf_ps_f6a_h_W = 1;
  localparam int unsigned def_unf_ps_f6a_h_W = 1;
  localparam int unsigned pot_ovf_ph_f6a_h_W = 1;
  localparam int unsigned def_ovf_ph_f6a_h_W = 1;
  localparam int unsigned pot_unf_ph_f6a_h_W = 1;
  localparam int unsigned def_unf_ph_f6a_h_W = 1;
  localparam int unsigned frac_zero_f6a_h_W = 1;
  localparam int unsigned rnd_l_ph_f6a_h_W = 1;
  localparam int unsigned rnd_r_ph_f6a_h_W = 1;
  localparam int unsigned rnd_l_ps_f6a_h_W = 1;
  localparam int unsigned rnd_r_ps_f6a_h_W = 1;
  localparam int unsigned int_res_l_bit_f6a_h_W = 1;
  localparam int unsigned ph_allone2r_f6a_h_W = 1;
  localparam int unsigned ph_allone2l_f6a_h_W = 1;
  localparam int unsigned ps_allone2r_f6a_h_W = 1;
  localparam int unsigned ps_allone2l_f6a_h_W = 1;
  localparam int unsigned is_trans_rfa_h_W = 1;
  localparam int unsigned sel_eblo_ph_exa_h_W = 1;
  localparam int unsigned sel_ec5bdet_exa_h_W = 1;
  localparam int unsigned elo_eb_en_exa_h_W = 1;
  localparam int unsigned ehi_eb_en_exa_h_W = 1;
  localparam int unsigned elo_ea_en_exa_h_W = 1;
  localparam int unsigned fa_en_exa_h_W = 1;
  localparam int unsigned fb_en_exa_h_W = 1;
  localparam int unsigned fc_en_exa_h_W = 1;
  localparam int unsigned sel_ealo_ph_exa_h_W = 1;
  localparam int unsigned sa_hi_en_exa_h_W = 1;
  localparam int unsigned sa_lo_en_exa_h_W = 1;
  localparam int unsigned sb_hi_en_exa_h_W = 1;
  localparam int unsigned sb_lo_en_exa_h_W = 1;
  localparam int unsigned sc_en_exa_h_W = 1;
  localparam int unsigned ehi_ea_en_exa_h_W = 1;
  localparam int unsigned ec_en_exa_h_W = 1;
  localparam int unsigned neg_fmaop_exa_h_W = 1;
  localparam int unsigned trans_op2_exa_l_W = 1;
  localparam int unsigned sa1_nr2_frcpfxp_exa_h_W = 1;
  localparam int unsigned sel_sign_sinp2_exa_h_W = 1;
  localparam int unsigned sa_zap_exa_l_W = 1;
  localparam int unsigned sb_zap_exa_l_W = 1;
  localparam int unsigned sel_f10_exa_h_W = 1;
  localparam int unsigned sel_f11_exa_h_W = 1;
  localparam int unsigned fc_dtps_exa_h_W = 1;
  localparam int unsigned sel_snorm_exa_h_W = 1;
  localparam int unsigned sel_g1sk2_exa_h_W = 1;
  localparam int unsigned sel_g1sk8_exa_h_W = 1;
  localparam int unsigned sel_g1sk16_exa_h_W = 1;
  localparam int unsigned sel_g1sk24_exa_h_W = 1;
  localparam int unsigned sel_xexpfrac_exa_h_W = 1;
  localparam int unsigned sel_ebin_hi_exa_h_W = ((1) - (0) + 1);
  localparam int unsigned sel_eainmux_exa_h_W = ((1) - (0) + 1);
  localparam int unsigned sel_ec5b_exa_h_W = ((1) - (0) + 1);
  localparam int unsigned sel_ecin_exa_h_W = ((1) - (0) + 1);
  localparam int unsigned sel_sc_exa_h_W = ((1) - (0) + 1);
  localparam int unsigned sel_opcgfx_exa_h_W = ((1) - (0) + 1);
  localparam int unsigned sel_xopc_exa_h_W = ((1) - (0) + 1);
  localparam int unsigned sel_opcfmt_exa_h_W = ((1) - (0) + 1);
  localparam int unsigned sel_transfa_exa_h_W = ((1) - (0) + 1);
  localparam int unsigned sel_transfb_exa_h_W = ((1) - (0) + 1);
  localparam int unsigned sel_opafmt_exa_h_W = ((1) - (0) + 1);
  localparam int unsigned sel_opbfmt_exa_h_W = ((1) - (0) + 1);
  localparam int unsigned opa_valid_rfa_h_W = 1;
  localparam int unsigned opb_valid_rfa_h_W = 1;
  localparam int unsigned opc_valid_rfa_h_W = 1;
  localparam int unsigned dtps_f0a_h_W = 1;
  localparam int unsigned ctrl_valid_f0a_h_W = 1;
  localparam int unsigned ctrl_valid_f1a_h_W = 1;
  localparam int unsigned ctrl_valid_f2a_h_W = 1;
  localparam int unsigned ctrl_valid_f3a_h_W = 1;
  localparam int unsigned ctrl_valid_f4a_h_W = 1;
  localparam int unsigned ctrl_valid_f5a_h_W = 1;
  localparam int unsigned ctrl_valid_f6a_h_W = 1;
  localparam int unsigned mul_sign_f0a_h_W = 1;
  localparam int unsigned mul_hp_mode_f0a_h_W = 1;
  localparam int unsigned eb_hi_k_exa_h_W = ((5) - (0) + 1);
  localparam int unsigned use_mul_f0a_h_W = 1;
  localparam int unsigned ec_zero_f1a_h_W = 1;
  localparam int unsigned ea_hi_zero_f1a_h_W = 1;
  localparam int unsigned fc_zero30_8_f1a_h_W = 1;
  localparam int unsigned paddin1_sels_f1a_h_W = 1;
  localparam int unsigned paddin2_selc_f1a_h_W = 1;
  localparam int unsigned paddin2_selfcneg_f1a_h_W = 1;
  localparam int unsigned padd_cin_f1a_h_W = 1;
  localparam int unsigned sel_famask_f1a_h_W = 1;
  localparam int unsigned fa_en_f1a_h_W = 1;
  localparam int unsigned cmd_f1a_h_W = ((VPU_FCMD_SZ-1) - (0) + 1);
  localparam int unsigned sigs_f1a_h_W = ((TXFMA_SIGS_SZ-1) - (0) + 1);
  localparam int unsigned op_dtype_f1a_h_W = ((VPU_DTYPE_SZ-1) - (0) + 1);
  localparam int unsigned use_mul_f1a_h_W = 1;
  localparam int unsigned ec_zero_f2a_h_W = 1;
  localparam int unsigned eprod_hi_zero_f2a_h_W = 1;
  localparam int unsigned eprod_lo_zero_f2a_h_W = 1;
  localparam int unsigned op3p1c_sel_p_f2a_h_W = 1;
  localparam int unsigned op3p1c_sel_r_f2a_h_W = 1;
  localparam int unsigned op3g_sel_f2a_h_W = 1;
  localparam int unsigned rm_rne_f2a_h_W = 1;
  localparam int unsigned rm_rmm_f2a_h_W = 1;
  localparam int unsigned frnd_rupnsc_f2a_h_W = 1;
  localparam int unsigned frnd_rdnsc_f2a_h_W = 1;
  localparam int unsigned frnd_t1_f2a_h_W = 1;
  localparam int unsigned frnd_t2_f2a_h_W = 1;
  localparam int unsigned al1c_zap_f2a_h_W = 1;
  localparam int unsigned op1c_log_mul_clr_f2a_h_W = 1;
  localparam int unsigned negop1c_int_logmul_clr_f2a_h_W = 1;
  localparam int unsigned psc_op1_clr_f2a_h_W = 1;
  localparam int unsigned align2_zap_f2a_h_W = 1;
  localparam int unsigned align2c_neg_f2a_h_W = 1;
  localparam int unsigned trans_exp_fma2_exc_f2a_h_W = 1;
  localparam int unsigned al1p1c_sel_f2a_h_W = 1;
  localparam int unsigned op2_sxtto_g_f2a_h_W = 1;
  localparam int unsigned op3p3spc_sel_f2a_h_W = ((1) - (0) + 1);
  localparam int unsigned op3p2c_sel_f2a_h_W = ((2) - (0) + 1);
  localparam int unsigned ugfxrplc_sel_f2a_h_W = ((1) - (0) + 1);
  localparam int unsigned sgfxrplc_sel_f2a_h_W = ((1) - (0) + 1);
  localparam int unsigned icgfxc_sel_f2a_h_W = ((1) - (0) + 1);
  localparam int unsigned al1znp2c_sel_f2a_h_W = ((1) - (0) + 1);
  localparam int unsigned al2c_sel_f2a_h_W = ((1) - (0) + 1);
  localparam int unsigned al1p2c_sel_f2a_h_W = ((1) - (0) + 1);
  localparam int unsigned op_dtype_f2a_h_W = ((VPU_DTYPE_SZ-1) - (0) + 1);
  localparam int unsigned sigs_f2a_h_W = ((TXFMA_SIGS_SZ-1) - (0) + 1);
  localparam int unsigned cmd_f2a_h_W = ((VPU_FCMD_SZ-1) - (0) + 1);
  localparam int unsigned sin_ediff2_f2a_h_W = 1;
  localparam int unsigned fma_subopc3_f3a_h_W = 1;
  localparam int unsigned s_res_f3a_h_W = 1;
  localparam int unsigned imxc_sel_f3a_h_W = ((1) - (0) + 1);
  localparam int unsigned al2c3_neg_cin_ps_f3a_h_W = 1;
  localparam int unsigned al1c3_neg_cin_f3a_h_W = 1;
  localparam int unsigned neg_norm_a3_f4a_h_W = 1;
  localparam int unsigned neg_norm_na3_f4a_h_W = 1;
  localparam int unsigned clearphps_en_f4a_h_W = 1;
  localparam int unsigned clear16frnd_nshin_f4a_l_W = 1;
  localparam int unsigned ph_ps_clear_f4a_l_W = 1;
  localparam int unsigned elxd_add_in2_mux_sel_f4a_h_W = 1;
  localparam int unsigned use_elxd_f4a_h_W = 1;
  localparam int unsigned fb_hi_zero_f4a_h_W = 1;
  localparam int unsigned enc_nsamt_f4a_h_W = ((TXFMA_NORM_SHF_AMNT_SZ-1) - (0) + 1);
  localparam int unsigned int_minmax_mux_sel_a_f5a_h_W = 1;
  localparam int unsigned int_minmax_mux_sel_c_f5a_h_W = 1;
  localparam int unsigned rc_ps_f5a_h_W = 1;
  localparam int unsigned rc_ph_f5a_h_W = 1;
  localparam int unsigned rc6_ps_f5a_h_W = 1;
  localparam int unsigned rc6_ph_f5a_h_W = 1;
  localparam int unsigned rc_int32_f5a_h_W = 1;
  localparam int unsigned rc_3rmxr_f5a_h_W = 1;
  localparam int unsigned rc_rl01_f5a_h_W = 1;
  localparam int unsigned rm_f5a_h_W = ((TXFMA_RM_SZ-1) - (0) + 1);
  localparam int unsigned rc_rmm_f5a_h_W = 1;
  localparam int unsigned rc_rup_f5a_h_W = 1;
  localparam int unsigned rc_rne_f5a_h_W = 1;
  localparam int unsigned rc_rdn_f5a_h_W = 1;
  localparam int unsigned rc_res_sign_f5a_l_W = 1;
  localparam int unsigned rc_transrnd_f5a_h_W = 1;
  localparam int unsigned rc_transrnd_log_fma2_f5a_h_W = 1;
  localparam int unsigned nshcen_sel1b_f5a_h_W = 1;
  localparam int unsigned en_logrrsel1b_f5a_l_W = 1;
  localparam int unsigned int32_stky2r_f5a_h_W = 1;
  localparam int unsigned mmx_sel_immres_f5a_h_W = 1;
  localparam int unsigned comp_op_true_f5a_h_W = 1;
  localparam int unsigned comp_op_f5a_h_W = 1;
  localparam int unsigned trans2_ps_roundup_r_f5a_h_W = 1;
  localparam int unsigned e_force_to_zero_f5a_h_W = 1;
  localparam int unsigned s_res_out_f5a_h_W = 1;
  localparam int unsigned op_dtype_f5a_h_W = ((VPU_DTYPE_SZ-1) - (0) + 1);
  localparam int unsigned sigs_f5a_h_W = ((TXFMA_SIGS_SZ-1) - (0) + 1);
  localparam int unsigned cmd_f5a_h_W = ((VPU_FCMD_SZ-1) - (0) + 1);
  localparam int unsigned sel_gmaxu24_f6a_h_W = 1;
  localparam int unsigned sel_gmaxu16_f6a_h_W = 1;
  localparam int unsigned sel_gmaxs16_f6a_h_W = 1;
  localparam int unsigned sel_gmins16_f6a_h_W = 1;
  localparam int unsigned sel_gmaxs8_f6a_h_W = 1;
  localparam int unsigned sel_gmaxu2_f6a_h_W = 1;
  localparam int unsigned sel_gmins8_f6a_h_W = 1;
  localparam int unsigned sel_gmaxu8_f6a_h_W = 1;
  localparam int unsigned sel_gfxmax_f6a_h_W = 1;
  localparam int unsigned sel_psexc_sr_f6a_h_W = 1;
  localparam int unsigned sel_bit31_one_f6a_h_W = 1;
  localparam int unsigned sel_b30to0_ones_f6a_h_W = 1;
  localparam int unsigned sel_zerores_f6a_h_W = 1;
  localparam int unsigned sel_ps_unf_f6a_h_W = 1;
  localparam int unsigned sel_ps_normone_f6a_h_W = 1;
  localparam int unsigned sel_ps_inf_f6a_h_W = 1;
  localparam int unsigned sel_ps_cqn_f6a_h_W = 1;
  localparam int unsigned sel_ps_large_f6a_h_W = 1;
  localparam int unsigned sel_phsp_f6a_h_W = 1;
  localparam int unsigned sel_phhi_sr_f6a_h_W = 1;
  localparam int unsigned sel_phhi_large_f6a_h_W = 1;
  localparam int unsigned sel_phhi_unf_f6a_h_W = 1;
  localparam int unsigned sel_phhi_infres_f6a_h_W = 1;
  localparam int unsigned sel_f11inf_f6a_h_W = 1;
  localparam int unsigned sel_f10cqnan_f6a_h_W = 1;
  localparam int unsigned sel_f11sp_f6a_h_W = 1;
  localparam int unsigned sel_f10sp_f6a_h_W = 1;
  localparam int unsigned sel_f11large_f6a_h_W = 1;
  localparam int unsigned sel_f10large_f6a_h_W = 1;
  localparam int unsigned ps_rndovf_f6a_h_W = 1;
  localparam int unsigned ph_rndovf_f6a_h_W = 1;
  localparam int unsigned sel_set_bit0_one_f6a_h_W = 1;
  localparam int unsigned effsub1_set_sign1_f6a_h_W = 1;
  localparam int unsigned normmask1_f6a_h_W = 1;
  localparam int unsigned normmask2_f6a_h_W = 1;
  localparam int unsigned sel_nr1_frcpres_f6a_h_W = 1;
  localparam int unsigned trans_sinp2m_f6a_h_W = 1;
  localparam int unsigned f10f11mux_sel_f6a_h_W = 1;
  localparam int unsigned rrmux16l_sel_f6a_l_W = 1;
  localparam int unsigned sel_classify_out_f6a_h_W = 1;
  localparam int unsigned rrmux32_sel_f6a_h_W = ((1) - (0) + 1);
  localparam int unsigned sel_excreshi_f6a_h_W = ((1) - (0) + 1);
  localparam int unsigned sel_excreslo_f6a_h_W = ((1) - (0) + 1);
  localparam int unsigned sel_fmares_hi_f6a_h_W = ((1) - (0) + 1);
  localparam int unsigned sel_fmares_lo_f6a_h_W = ((1) - (0) + 1);
  localparam int unsigned cmd_f6a_h_W = ((VPU_FCMD_SZ-1) - (0) + 1);
  localparam int unsigned sigs_f6a_h_W = ((TXFMA_SIGS_SZ-1) - (0) + 1);
  localparam int unsigned op_dtype_f6a_h_W = ((VPU_DTYPE_SZ-1) - (0) + 1);
  localparam int unsigned classify_f6a_h_W = ((FP_CLASS_BITS-1) - (0) + 1);
  localparam int unsigned fma_flags_f6a_h_W = ((VPU_FLAGS_SZ-1) - (0) + 1);
  localparam int unsigned comp_op_true_f6a_h_W = 1;

  logic [in_valid_exa_h_W-1:0] in_valid_exa_h;
  vpu_ctrl_sigs_t sigs_in_exa_h;
  logic [rm_in_exa_h_W-1:0] rm_in_exa_h;
  logic [trans_mask_exa_h_W-1:0] trans_mask_exa_h;
  logic [trans_taylor_exa_h_W-1:0] trans_taylor_exa_h;
  logic [trans_exp_fma2_exc_exa_h_W-1:0] trans_exp_fma2_exc_exa_h;
  logic [use_prev_sigs_exa_h_W-1:0] use_prev_sigs_exa_h;
  logic [sa_hi_f0a_h_W-1:0] sa_hi_f0a_h;
  logic [sa_lo_f0a_h_W-1:0] sa_lo_f0a_h;
  logic [sb_hi_f0a_h_W-1:0] sb_hi_f0a_h;
  logic [sb_lo_f0a_h_W-1:0] sb_lo_f0a_h;
  logic [sc_f0a_h_W-1:0] sc_f0a_h;
  logic [ea_hi_zero_f0a_l_W-1:0] ea_hi_zero_f0a_l;
  logic [ea_hi_all1_f0a_h_W-1:0] ea_hi_all1_f0a_h;
  logic [ea_lo_zero_f0a_l_W-1:0] ea_lo_zero_f0a_l;
  logic [ea_lo_all1_f0a_h_W-1:0] ea_lo_all1_f0a_h;
  logic [eb_hi_zero_f0a_l_W-1:0] eb_hi_zero_f0a_l;
  logic [eb_hi_all1_f0a_h_W-1:0] eb_hi_all1_f0a_h;
  logic [eb_lo_zero_f0a_l_W-1:0] eb_lo_zero_f0a_l;
  logic [eb_lo_all1_f0a_h_W-1:0] eb_lo_all1_f0a_h;
  logic [ec_zero_f0a_l_W-1:0] ec_zero_f0a_l;
  logic [ec_all1_f0a_h_W-1:0] ec_all1_f0a_h;
  logic [eb_eql_127_f0a_h_W-1:0] eb_eql_127_f0a_h;
  logic [eb_eql_126_f0a_h_W-1:0] eb_eql_126_f0a_h;
  logic [eb_eql_125_f0a_h_W-1:0] eb_eql_125_f0a_h;
  logic [fcf10_zero_f0a_l_W-1:0] fcf10_zero_f0a_l;
  logic [fcf11_zero_f0a_l_W-1:0] fcf11_zero_f0a_l;
  logic [fcps_zero_f0a_h_W-1:0] fcps_zero_f0a_h;
  logic [fcph_zero_f0a_l_W-1:0] fcph_zero_f0a_l;
  logic [fcnorm_zero_29_8_f0a_h_W-1:0] fcnorm_zero_29_8_f0a_h;
  logic [fcnorm16_eq_1_f0a_h_W-1:0] fcnorm16_eq_1_f0a_h;
  logic [fcnorm8_eq_1_f0a_h_W-1:0] fcnorm8_eq_1_f0a_h;
  logic [fcnorm_30_f0a_h_W-1:0] fcnorm_30_f0a_h;
  logic [faph_hi_b0_f0a_h_W-1:0] faph_hi_b0_f0a_h;
  logic [fbph_hi_b0_f0a_h_W-1:0] fbph_hi_b0_f0a_h;
  logic [fcph_b0_f0a_h_W-1:0] fcph_b0_f0a_h;
  logic [fcf11_b0_f0a_h_W-1:0] fcf11_b0_f0a_h;
  logic [fcf10_b0_f0a_h_W-1:0] fcf10_b0_f0a_h;
  logic [fcn8_one_30_24_f0a_h_W-1:0] fcn8_one_30_24_f0a_h;
  logic [fcn10_one_30_22_f0a_h_W-1:0] fcn10_one_30_22_f0a_h;
  logic [fcn16_one_30_16_f0a_h_W-1:0] fcn16_one_30_16_f0a_h;
  logic [fcn24_one_30_8_f0a_h_W-1:0] fcn24_one_30_8_f0a_h;
  logic [faps_zero_f0a_l_W-1:0] faps_zero_f0a_l;
  logic [fbps_zero_f0a_l_W-1:0] fbps_zero_f0a_l;
  logic [faph_hi_zero_f0a_l_W-1:0] faph_hi_zero_f0a_l;
  logic [fbph_hi_zero_f0a_l_W-1:0] fbph_hi_zero_f0a_l;
  logic [faph_lo_zero_f0a_l_W-1:0] faph_lo_zero_f0a_l;
  logic [fbph_lo_zero_f0a_l_W-1:0] fbph_lo_zero_f0a_l;
  logic [faph_lo_b0_f0a_h_W-1:0] faph_lo_b0_f0a_h;
  logic [fbph_lo_b0_f0a_h_W-1:0] fbph_lo_b0_f0a_h;
  logic [faps_b0_f0a_h_W-1:0] faps_b0_f0a_h;
  logic [fbps_b0_f0a_h_W-1:0] fbps_b0_f0a_h;
  logic [fc_31_f0a_h_W-1:0] fc_31_f0a_h;
  logic [fcps_b0_f0a_h_W-1:0] fcps_b0_f0a_h;
  logic [ea_hi_f1a_h_W-1:0] ea_hi_f1a_h;
  logic [ediff1_f2a_h_W-1:0] ediff1_f2a_h;
  logic [ediff1_abs_f2a_h_W-1:0] ediff1_abs_f2a_h;
  logic [ediff2_abs_f2a_h_W-1:0] ediff2_abs_f2a_h;
  logic [ediff1_sel_f2a_h_W-1:0] ediff1_sel_f2a_h;
  logic [ediff2_sel_f2a_h_W-1:0] ediff2_sel_f2a_h;
  logic [align1_mux_sel_f2a_h_W-1:0] align1_mux_sel_f2a_h;
  logic [align2_mux_sel_f2a_h_W-1:0] align2_mux_sel_f2a_h;
  logic [ec_f2a_h_W-1:0] ec_f2a_h;
  logic [fc_b0_f2a_h_W-1:0] fc_b0_f2a_h;
  logic [e_sticky1_f2a_h_W-1:0] e_sticky1_f2a_h;
  logic [e_sticky2_f2a_h_W-1:0] e_sticky2_f2a_h;
  logic [opd3_mux_sel_f2a_h_W-1:0] opd3_mux_sel_f2a_h;
  logic [ediff1_f3a_h_W-1:0] ediff1_f3a_h;
  logic [ediff2_f3a_h_W-1:0] ediff2_f3a_h;
  logic [ec_f3a_h_W-1:0] ec_f3a_h;
  logic [e_sticky1_f3a_h_W-1:0] e_sticky1_f3a_h;
  logic [prodsum_a2_f3a_h_W-1:0] prodsum_a2_f3a_h;
  logic [fround_rbit_f3a_h_W-1:0] fround_rbit_f3a_h;
  logic [ph_comb_stiky_f4a_h_W-1:0] ph_comb_stiky_f4a_h;
  logic [elxd_f4a_h_W-1:0] elxd_f4a_h;
  logic [etrz50_f4a_h_W-1:0] etrz50_f4a_h;
  logic [stky2g_f5a_h_W-1:0] stky2g_f5a_h;
  logic [s_res_f5a_h_W-1:0] s_res_f5a_h;
  logic [comb_add_a3_f5a_h_W-1:0] comb_add_a3_f5a_h;
  logic [ediff1_zap_f5a_h_W-1:0] ediff1_zap_f5a_h;
  logic [rmode_f6a_h_W-1:0] rmode_f6a_h;
  logic [exp_res_zero_f6a_h_W-1:0] exp_res_zero_f6a_h;
  logic [s_res_f6a_h_W-1:0] s_res_f6a_h;
  logic [ph_stky2l_f6a_h_W-1:0] ph_stky2l_f6a_h;
  logic [ps_stky2l_f6a_h_W-1:0] ps_stky2l_f6a_h;
  logic [int32_stky2l_f6a_h_W-1:0] int32_stky2l_f6a_h;
  logic [pot_ovf_ps_f6a_h_W-1:0] pot_ovf_ps_f6a_h;
  logic [def_ovf_ps_f6a_h_W-1:0] def_ovf_ps_f6a_h;
  logic [pot_unf_ps_f6a_h_W-1:0] pot_unf_ps_f6a_h;
  logic [def_unf_ps_f6a_h_W-1:0] def_unf_ps_f6a_h;
  logic [pot_ovf_ph_f6a_h_W-1:0] pot_ovf_ph_f6a_h;
  logic [def_ovf_ph_f6a_h_W-1:0] def_ovf_ph_f6a_h;
  logic [pot_unf_ph_f6a_h_W-1:0] pot_unf_ph_f6a_h;
  logic [def_unf_ph_f6a_h_W-1:0] def_unf_ph_f6a_h;
  logic [frac_zero_f6a_h_W-1:0] frac_zero_f6a_h;
  logic [rnd_l_ph_f6a_h_W-1:0] rnd_l_ph_f6a_h;
  logic [rnd_r_ph_f6a_h_W-1:0] rnd_r_ph_f6a_h;
  logic [rnd_l_ps_f6a_h_W-1:0] rnd_l_ps_f6a_h;
  logic [rnd_r_ps_f6a_h_W-1:0] rnd_r_ps_f6a_h;
  logic [int_res_l_bit_f6a_h_W-1:0] int_res_l_bit_f6a_h;
  logic [ph_allone2r_f6a_h_W-1:0] ph_allone2r_f6a_h;
  logic [ph_allone2l_f6a_h_W-1:0] ph_allone2l_f6a_h;
  logic [ps_allone2r_f6a_h_W-1:0] ps_allone2r_f6a_h;
  logic [ps_allone2l_f6a_h_W-1:0] ps_allone2l_f6a_h;

  assign in_valid_exa_h = inputs_i[0 +: in_valid_exa_h_W];
  assign sigs_in_exa_h = vpu_ctrl_sigs_t'(inputs_i[(0 + in_valid_exa_h_W) +: sigs_in_exa_h_W]);
  assign rm_in_exa_h = inputs_i[((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) +: rm_in_exa_h_W];
  assign trans_mask_exa_h = inputs_i[(((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) +: trans_mask_exa_h_W];
  assign trans_taylor_exa_h = inputs_i[((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) +: trans_taylor_exa_h_W];
  assign trans_exp_fma2_exc_exa_h = inputs_i[(((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) +: trans_exp_fma2_exc_exa_h_W];
  assign use_prev_sigs_exa_h = inputs_i[((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) +: use_prev_sigs_exa_h_W];
  assign sa_hi_f0a_h = inputs_i[(((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) +: sa_hi_f0a_h_W];
  assign sa_lo_f0a_h = inputs_i[((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) +: sa_lo_f0a_h_W];
  assign sb_hi_f0a_h = inputs_i[(((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) +: sb_hi_f0a_h_W];
  assign sb_lo_f0a_h = inputs_i[((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) +: sb_lo_f0a_h_W];
  assign sc_f0a_h = inputs_i[(((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) +: sc_f0a_h_W];
  assign ea_hi_zero_f0a_l = inputs_i[((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) +: ea_hi_zero_f0a_l_W];
  assign ea_hi_all1_f0a_h = inputs_i[(((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) +: ea_hi_all1_f0a_h_W];
  assign ea_lo_zero_f0a_l = inputs_i[((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) +: ea_lo_zero_f0a_l_W];
  assign ea_lo_all1_f0a_h = inputs_i[(((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) +: ea_lo_all1_f0a_h_W];
  assign eb_hi_zero_f0a_l = inputs_i[((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) +: eb_hi_zero_f0a_l_W];
  assign eb_hi_all1_f0a_h = inputs_i[(((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) +: eb_hi_all1_f0a_h_W];
  assign eb_lo_zero_f0a_l = inputs_i[((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) +: eb_lo_zero_f0a_l_W];
  assign eb_lo_all1_f0a_h = inputs_i[(((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) +: eb_lo_all1_f0a_h_W];
  assign ec_zero_f0a_l = inputs_i[((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) +: ec_zero_f0a_l_W];
  assign ec_all1_f0a_h = inputs_i[(((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) +: ec_all1_f0a_h_W];
  assign eb_eql_127_f0a_h = inputs_i[((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) +: eb_eql_127_f0a_h_W];
  assign eb_eql_126_f0a_h = inputs_i[(((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) +: eb_eql_126_f0a_h_W];
  assign eb_eql_125_f0a_h = inputs_i[((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) +: eb_eql_125_f0a_h_W];
  assign fcf10_zero_f0a_l = inputs_i[(((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) +: fcf10_zero_f0a_l_W];
  assign fcf11_zero_f0a_l = inputs_i[((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) +: fcf11_zero_f0a_l_W];
  assign fcps_zero_f0a_h = inputs_i[(((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) +: fcps_zero_f0a_h_W];
  assign fcph_zero_f0a_l = inputs_i[((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) +: fcph_zero_f0a_l_W];
  assign fcnorm_zero_29_8_f0a_h = inputs_i[(((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) +: fcnorm_zero_29_8_f0a_h_W];
  assign fcnorm16_eq_1_f0a_h = inputs_i[((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) +: fcnorm16_eq_1_f0a_h_W];
  assign fcnorm8_eq_1_f0a_h = inputs_i[(((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) +: fcnorm8_eq_1_f0a_h_W];
  assign fcnorm_30_f0a_h = inputs_i[((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) +: fcnorm_30_f0a_h_W];
  assign faph_hi_b0_f0a_h = inputs_i[(((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) +: faph_hi_b0_f0a_h_W];
  assign fbph_hi_b0_f0a_h = inputs_i[((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) +: fbph_hi_b0_f0a_h_W];
  assign fcph_b0_f0a_h = inputs_i[(((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) +: fcph_b0_f0a_h_W];
  assign fcf11_b0_f0a_h = inputs_i[((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) +: fcf11_b0_f0a_h_W];
  assign fcf10_b0_f0a_h = inputs_i[(((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) +: fcf10_b0_f0a_h_W];
  assign fcn8_one_30_24_f0a_h = inputs_i[((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) +: fcn8_one_30_24_f0a_h_W];
  assign fcn10_one_30_22_f0a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) +: fcn10_one_30_22_f0a_h_W];
  assign fcn16_one_30_16_f0a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) +: fcn16_one_30_16_f0a_h_W];
  assign fcn24_one_30_8_f0a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) +: fcn24_one_30_8_f0a_h_W];
  assign faps_zero_f0a_l = inputs_i[((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) +: faps_zero_f0a_l_W];
  assign fbps_zero_f0a_l = inputs_i[(((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) +: fbps_zero_f0a_l_W];
  assign faph_hi_zero_f0a_l = inputs_i[((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) +: faph_hi_zero_f0a_l_W];
  assign fbph_hi_zero_f0a_l = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) +: fbph_hi_zero_f0a_l_W];
  assign faph_lo_zero_f0a_l = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) +: faph_lo_zero_f0a_l_W];
  assign fbph_lo_zero_f0a_l = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) +: fbph_lo_zero_f0a_l_W];
  assign faph_lo_b0_f0a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) +: faph_lo_b0_f0a_h_W];
  assign fbph_lo_b0_f0a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) +: fbph_lo_b0_f0a_h_W];
  assign faps_b0_f0a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) +: faps_b0_f0a_h_W];
  assign fbps_b0_f0a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) +: fbps_b0_f0a_h_W];
  assign fc_31_f0a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) +: fc_31_f0a_h_W];
  assign fcps_b0_f0a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) +: fcps_b0_f0a_h_W];
  assign ea_hi_f1a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) +: ea_hi_f1a_h_W];
  assign ediff1_f2a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) +: ediff1_f2a_h_W];
  assign ediff1_abs_f2a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) +: ediff1_abs_f2a_h_W];
  assign ediff2_abs_f2a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) +: ediff2_abs_f2a_h_W];
  assign ediff1_sel_f2a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) +: ediff1_sel_f2a_h_W];
  assign ediff2_sel_f2a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) +: ediff2_sel_f2a_h_W];
  assign align1_mux_sel_f2a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) +: align1_mux_sel_f2a_h_W];
  assign align2_mux_sel_f2a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) +: align2_mux_sel_f2a_h_W];
  assign ec_f2a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) +: ec_f2a_h_W];
  assign fc_b0_f2a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) +: fc_b0_f2a_h_W];
  assign e_sticky1_f2a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) +: e_sticky1_f2a_h_W];
  assign e_sticky2_f2a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) +: e_sticky2_f2a_h_W];
  assign opd3_mux_sel_f2a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) +: opd3_mux_sel_f2a_h_W];
  assign ediff1_f3a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) +: ediff1_f3a_h_W];
  assign ediff2_f3a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) +: ediff2_f3a_h_W];
  assign ec_f3a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) +: ec_f3a_h_W];
  assign e_sticky1_f3a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) +: e_sticky1_f3a_h_W];
  assign prodsum_a2_f3a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) +: prodsum_a2_f3a_h_W];
  assign fround_rbit_f3a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) +: fround_rbit_f3a_h_W];
  assign ph_comb_stiky_f4a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) +: ph_comb_stiky_f4a_h_W];
  assign elxd_f4a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) +: elxd_f4a_h_W];
  assign etrz50_f4a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) +: etrz50_f4a_h_W];
  assign stky2g_f5a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) +: stky2g_f5a_h_W];
  assign s_res_f5a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) +: s_res_f5a_h_W];
  assign comb_add_a3_f5a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) +: comb_add_a3_f5a_h_W];
  assign ediff1_zap_f5a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) +: ediff1_zap_f5a_h_W];
  assign rmode_f6a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) +: rmode_f6a_h_W];
  assign exp_res_zero_f6a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) +: exp_res_zero_f6a_h_W];
  assign s_res_f6a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) +: s_res_f6a_h_W];
  assign ph_stky2l_f6a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) + s_res_f6a_h_W) +: ph_stky2l_f6a_h_W];
  assign ps_stky2l_f6a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) + s_res_f6a_h_W) + ph_stky2l_f6a_h_W) +: ps_stky2l_f6a_h_W];
  assign int32_stky2l_f6a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) + s_res_f6a_h_W) + ph_stky2l_f6a_h_W) + ps_stky2l_f6a_h_W) +: int32_stky2l_f6a_h_W];
  assign pot_ovf_ps_f6a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) + s_res_f6a_h_W) + ph_stky2l_f6a_h_W) + ps_stky2l_f6a_h_W) + int32_stky2l_f6a_h_W) +: pot_ovf_ps_f6a_h_W];
  assign def_ovf_ps_f6a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) + s_res_f6a_h_W) + ph_stky2l_f6a_h_W) + ps_stky2l_f6a_h_W) + int32_stky2l_f6a_h_W) + pot_ovf_ps_f6a_h_W) +: def_ovf_ps_f6a_h_W];
  assign pot_unf_ps_f6a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) + s_res_f6a_h_W) + ph_stky2l_f6a_h_W) + ps_stky2l_f6a_h_W) + int32_stky2l_f6a_h_W) + pot_ovf_ps_f6a_h_W) + def_ovf_ps_f6a_h_W) +: pot_unf_ps_f6a_h_W];
  assign def_unf_ps_f6a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) + s_res_f6a_h_W) + ph_stky2l_f6a_h_W) + ps_stky2l_f6a_h_W) + int32_stky2l_f6a_h_W) + pot_ovf_ps_f6a_h_W) + def_ovf_ps_f6a_h_W) + pot_unf_ps_f6a_h_W) +: def_unf_ps_f6a_h_W];
  assign pot_ovf_ph_f6a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) + s_res_f6a_h_W) + ph_stky2l_f6a_h_W) + ps_stky2l_f6a_h_W) + int32_stky2l_f6a_h_W) + pot_ovf_ps_f6a_h_W) + def_ovf_ps_f6a_h_W) + pot_unf_ps_f6a_h_W) + def_unf_ps_f6a_h_W) +: pot_ovf_ph_f6a_h_W];
  assign def_ovf_ph_f6a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) + s_res_f6a_h_W) + ph_stky2l_f6a_h_W) + ps_stky2l_f6a_h_W) + int32_stky2l_f6a_h_W) + pot_ovf_ps_f6a_h_W) + def_ovf_ps_f6a_h_W) + pot_unf_ps_f6a_h_W) + def_unf_ps_f6a_h_W) + pot_ovf_ph_f6a_h_W) +: def_ovf_ph_f6a_h_W];
  assign pot_unf_ph_f6a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) + s_res_f6a_h_W) + ph_stky2l_f6a_h_W) + ps_stky2l_f6a_h_W) + int32_stky2l_f6a_h_W) + pot_ovf_ps_f6a_h_W) + def_ovf_ps_f6a_h_W) + pot_unf_ps_f6a_h_W) + def_unf_ps_f6a_h_W) + pot_ovf_ph_f6a_h_W) + def_ovf_ph_f6a_h_W) +: pot_unf_ph_f6a_h_W];
  assign def_unf_ph_f6a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) + s_res_f6a_h_W) + ph_stky2l_f6a_h_W) + ps_stky2l_f6a_h_W) + int32_stky2l_f6a_h_W) + pot_ovf_ps_f6a_h_W) + def_ovf_ps_f6a_h_W) + pot_unf_ps_f6a_h_W) + def_unf_ps_f6a_h_W) + pot_ovf_ph_f6a_h_W) + def_ovf_ph_f6a_h_W) + pot_unf_ph_f6a_h_W) +: def_unf_ph_f6a_h_W];
  assign frac_zero_f6a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) + s_res_f6a_h_W) + ph_stky2l_f6a_h_W) + ps_stky2l_f6a_h_W) + int32_stky2l_f6a_h_W) + pot_ovf_ps_f6a_h_W) + def_ovf_ps_f6a_h_W) + pot_unf_ps_f6a_h_W) + def_unf_ps_f6a_h_W) + pot_ovf_ph_f6a_h_W) + def_ovf_ph_f6a_h_W) + pot_unf_ph_f6a_h_W) + def_unf_ph_f6a_h_W) +: frac_zero_f6a_h_W];
  assign rnd_l_ph_f6a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) + s_res_f6a_h_W) + ph_stky2l_f6a_h_W) + ps_stky2l_f6a_h_W) + int32_stky2l_f6a_h_W) + pot_ovf_ps_f6a_h_W) + def_ovf_ps_f6a_h_W) + pot_unf_ps_f6a_h_W) + def_unf_ps_f6a_h_W) + pot_ovf_ph_f6a_h_W) + def_ovf_ph_f6a_h_W) + pot_unf_ph_f6a_h_W) + def_unf_ph_f6a_h_W) + frac_zero_f6a_h_W) +: rnd_l_ph_f6a_h_W];
  assign rnd_r_ph_f6a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) + s_res_f6a_h_W) + ph_stky2l_f6a_h_W) + ps_stky2l_f6a_h_W) + int32_stky2l_f6a_h_W) + pot_ovf_ps_f6a_h_W) + def_ovf_ps_f6a_h_W) + pot_unf_ps_f6a_h_W) + def_unf_ps_f6a_h_W) + pot_ovf_ph_f6a_h_W) + def_ovf_ph_f6a_h_W) + pot_unf_ph_f6a_h_W) + def_unf_ph_f6a_h_W) + frac_zero_f6a_h_W) + rnd_l_ph_f6a_h_W) +: rnd_r_ph_f6a_h_W];
  assign rnd_l_ps_f6a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) + s_res_f6a_h_W) + ph_stky2l_f6a_h_W) + ps_stky2l_f6a_h_W) + int32_stky2l_f6a_h_W) + pot_ovf_ps_f6a_h_W) + def_ovf_ps_f6a_h_W) + pot_unf_ps_f6a_h_W) + def_unf_ps_f6a_h_W) + pot_ovf_ph_f6a_h_W) + def_ovf_ph_f6a_h_W) + pot_unf_ph_f6a_h_W) + def_unf_ph_f6a_h_W) + frac_zero_f6a_h_W) + rnd_l_ph_f6a_h_W) + rnd_r_ph_f6a_h_W) +: rnd_l_ps_f6a_h_W];
  assign rnd_r_ps_f6a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) + s_res_f6a_h_W) + ph_stky2l_f6a_h_W) + ps_stky2l_f6a_h_W) + int32_stky2l_f6a_h_W) + pot_ovf_ps_f6a_h_W) + def_ovf_ps_f6a_h_W) + pot_unf_ps_f6a_h_W) + def_unf_ps_f6a_h_W) + pot_ovf_ph_f6a_h_W) + def_ovf_ph_f6a_h_W) + pot_unf_ph_f6a_h_W) + def_unf_ph_f6a_h_W) + frac_zero_f6a_h_W) + rnd_l_ph_f6a_h_W) + rnd_r_ph_f6a_h_W) + rnd_l_ps_f6a_h_W) +: rnd_r_ps_f6a_h_W];
  assign int_res_l_bit_f6a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) + s_res_f6a_h_W) + ph_stky2l_f6a_h_W) + ps_stky2l_f6a_h_W) + int32_stky2l_f6a_h_W) + pot_ovf_ps_f6a_h_W) + def_ovf_ps_f6a_h_W) + pot_unf_ps_f6a_h_W) + def_unf_ps_f6a_h_W) + pot_ovf_ph_f6a_h_W) + def_ovf_ph_f6a_h_W) + pot_unf_ph_f6a_h_W) + def_unf_ph_f6a_h_W) + frac_zero_f6a_h_W) + rnd_l_ph_f6a_h_W) + rnd_r_ph_f6a_h_W) + rnd_l_ps_f6a_h_W) + rnd_r_ps_f6a_h_W) +: int_res_l_bit_f6a_h_W];
  assign ph_allone2r_f6a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) + s_res_f6a_h_W) + ph_stky2l_f6a_h_W) + ps_stky2l_f6a_h_W) + int32_stky2l_f6a_h_W) + pot_ovf_ps_f6a_h_W) + def_ovf_ps_f6a_h_W) + pot_unf_ps_f6a_h_W) + def_unf_ps_f6a_h_W) + pot_ovf_ph_f6a_h_W) + def_ovf_ph_f6a_h_W) + pot_unf_ph_f6a_h_W) + def_unf_ph_f6a_h_W) + frac_zero_f6a_h_W) + rnd_l_ph_f6a_h_W) + rnd_r_ph_f6a_h_W) + rnd_l_ps_f6a_h_W) + rnd_r_ps_f6a_h_W) + int_res_l_bit_f6a_h_W) +: ph_allone2r_f6a_h_W];
  assign ph_allone2l_f6a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) + s_res_f6a_h_W) + ph_stky2l_f6a_h_W) + ps_stky2l_f6a_h_W) + int32_stky2l_f6a_h_W) + pot_ovf_ps_f6a_h_W) + def_ovf_ps_f6a_h_W) + pot_unf_ps_f6a_h_W) + def_unf_ps_f6a_h_W) + pot_ovf_ph_f6a_h_W) + def_ovf_ph_f6a_h_W) + pot_unf_ph_f6a_h_W) + def_unf_ph_f6a_h_W) + frac_zero_f6a_h_W) + rnd_l_ph_f6a_h_W) + rnd_r_ph_f6a_h_W) + rnd_l_ps_f6a_h_W) + rnd_r_ps_f6a_h_W) + int_res_l_bit_f6a_h_W) + ph_allone2r_f6a_h_W) +: ph_allone2l_f6a_h_W];
  assign ps_allone2r_f6a_h = inputs_i[((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) + s_res_f6a_h_W) + ph_stky2l_f6a_h_W) + ps_stky2l_f6a_h_W) + int32_stky2l_f6a_h_W) + pot_ovf_ps_f6a_h_W) + def_ovf_ps_f6a_h_W) + pot_unf_ps_f6a_h_W) + def_unf_ps_f6a_h_W) + pot_ovf_ph_f6a_h_W) + def_ovf_ph_f6a_h_W) + pot_unf_ph_f6a_h_W) + def_unf_ph_f6a_h_W) + frac_zero_f6a_h_W) + rnd_l_ph_f6a_h_W) + rnd_r_ph_f6a_h_W) + rnd_l_ps_f6a_h_W) + rnd_r_ps_f6a_h_W) + int_res_l_bit_f6a_h_W) + ph_allone2r_f6a_h_W) + ph_allone2l_f6a_h_W) +: ps_allone2r_f6a_h_W];
  assign ps_allone2l_f6a_h = inputs_i[(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((0 + in_valid_exa_h_W) + sigs_in_exa_h_W) + rm_in_exa_h_W) + trans_mask_exa_h_W) + trans_taylor_exa_h_W) + trans_exp_fma2_exc_exa_h_W) + use_prev_sigs_exa_h_W) + sa_hi_f0a_h_W) + sa_lo_f0a_h_W) + sb_hi_f0a_h_W) + sb_lo_f0a_h_W) + sc_f0a_h_W) + ea_hi_zero_f0a_l_W) + ea_hi_all1_f0a_h_W) + ea_lo_zero_f0a_l_W) + ea_lo_all1_f0a_h_W) + eb_hi_zero_f0a_l_W) + eb_hi_all1_f0a_h_W) + eb_lo_zero_f0a_l_W) + eb_lo_all1_f0a_h_W) + ec_zero_f0a_l_W) + ec_all1_f0a_h_W) + eb_eql_127_f0a_h_W) + eb_eql_126_f0a_h_W) + eb_eql_125_f0a_h_W) + fcf10_zero_f0a_l_W) + fcf11_zero_f0a_l_W) + fcps_zero_f0a_h_W) + fcph_zero_f0a_l_W) + fcnorm_zero_29_8_f0a_h_W) + fcnorm16_eq_1_f0a_h_W) + fcnorm8_eq_1_f0a_h_W) + fcnorm_30_f0a_h_W) + faph_hi_b0_f0a_h_W) + fbph_hi_b0_f0a_h_W) + fcph_b0_f0a_h_W) + fcf11_b0_f0a_h_W) + fcf10_b0_f0a_h_W) + fcn8_one_30_24_f0a_h_W) + fcn10_one_30_22_f0a_h_W) + fcn16_one_30_16_f0a_h_W) + fcn24_one_30_8_f0a_h_W) + faps_zero_f0a_l_W) + fbps_zero_f0a_l_W) + faph_hi_zero_f0a_l_W) + fbph_hi_zero_f0a_l_W) + faph_lo_zero_f0a_l_W) + fbph_lo_zero_f0a_l_W) + faph_lo_b0_f0a_h_W) + fbph_lo_b0_f0a_h_W) + faps_b0_f0a_h_W) + fbps_b0_f0a_h_W) + fc_31_f0a_h_W) + fcps_b0_f0a_h_W) + ea_hi_f1a_h_W) + ediff1_f2a_h_W) + ediff1_abs_f2a_h_W) + ediff2_abs_f2a_h_W) + ediff1_sel_f2a_h_W) + ediff2_sel_f2a_h_W) + align1_mux_sel_f2a_h_W) + align2_mux_sel_f2a_h_W) + ec_f2a_h_W) + fc_b0_f2a_h_W) + e_sticky1_f2a_h_W) + e_sticky2_f2a_h_W) + opd3_mux_sel_f2a_h_W) + ediff1_f3a_h_W) + ediff2_f3a_h_W) + ec_f3a_h_W) + e_sticky1_f3a_h_W) + prodsum_a2_f3a_h_W) + fround_rbit_f3a_h_W) + ph_comb_stiky_f4a_h_W) + elxd_f4a_h_W) + etrz50_f4a_h_W) + stky2g_f5a_h_W) + s_res_f5a_h_W) + comb_add_a3_f5a_h_W) + ediff1_zap_f5a_h_W) + rmode_f6a_h_W) + exp_res_zero_f6a_h_W) + s_res_f6a_h_W) + ph_stky2l_f6a_h_W) + ps_stky2l_f6a_h_W) + int32_stky2l_f6a_h_W) + pot_ovf_ps_f6a_h_W) + def_ovf_ps_f6a_h_W) + pot_unf_ps_f6a_h_W) + def_unf_ps_f6a_h_W) + pot_ovf_ph_f6a_h_W) + def_ovf_ph_f6a_h_W) + pot_unf_ph_f6a_h_W) + def_unf_ph_f6a_h_W) + frac_zero_f6a_h_W) + rnd_l_ph_f6a_h_W) + rnd_r_ph_f6a_h_W) + rnd_l_ps_f6a_h_W) + rnd_r_ps_f6a_h_W) + int_res_l_bit_f6a_h_W) + ph_allone2r_f6a_h_W) + ph_allone2l_f6a_h_W) + ps_allone2r_f6a_h_W) +: ps_allone2l_f6a_h_W];

  txfmactl_top_orig u_orig (
    .clock                                    ( clk_i ),
    .reset                                    ( !rst_ni ),
    .in_valid_exa_h                           ( in_valid_exa_h ),
    .sigs_in_exa_h                            ( sigs_in_exa_h ),
    .rm_in_exa_h                              ( rm_in_exa_h ),
    .trans_mask_exa_h                         ( trans_mask_exa_h ),
    .trans_taylor_exa_h                       ( trans_taylor_exa_h ),
    .trans_exp_fma2_exc_exa_h                 ( trans_exp_fma2_exc_exa_h ),
    .use_prev_sigs_exa_h                      ( use_prev_sigs_exa_h ),
    .sa_hi_f0a_h                              ( sa_hi_f0a_h ),
    .sa_lo_f0a_h                              ( sa_lo_f0a_h ),
    .sb_hi_f0a_h                              ( sb_hi_f0a_h ),
    .sb_lo_f0a_h                              ( sb_lo_f0a_h ),
    .sc_f0a_h                                 ( sc_f0a_h ),
    .ea_hi_zero_f0a_l                         ( ea_hi_zero_f0a_l ),
    .ea_hi_all1_f0a_h                         ( ea_hi_all1_f0a_h ),
    .ea_lo_zero_f0a_l                         ( ea_lo_zero_f0a_l ),
    .ea_lo_all1_f0a_h                         ( ea_lo_all1_f0a_h ),
    .eb_hi_zero_f0a_l                         ( eb_hi_zero_f0a_l ),
    .eb_hi_all1_f0a_h                         ( eb_hi_all1_f0a_h ),
    .eb_lo_zero_f0a_l                         ( eb_lo_zero_f0a_l ),
    .eb_lo_all1_f0a_h                         ( eb_lo_all1_f0a_h ),
    .ec_zero_f0a_l                            ( ec_zero_f0a_l ),
    .ec_all1_f0a_h                            ( ec_all1_f0a_h ),
    .eb_eql_127_f0a_h                         ( eb_eql_127_f0a_h ),
    .eb_eql_126_f0a_h                         ( eb_eql_126_f0a_h ),
    .eb_eql_125_f0a_h                         ( eb_eql_125_f0a_h ),
    .fcf10_zero_f0a_l                         ( fcf10_zero_f0a_l ),
    .fcf11_zero_f0a_l                         ( fcf11_zero_f0a_l ),
    .fcps_zero_f0a_h                          ( fcps_zero_f0a_h ),
    .fcph_zero_f0a_l                          ( fcph_zero_f0a_l ),
    .fcnorm_zero_29_8_f0a_h                   ( fcnorm_zero_29_8_f0a_h ),
    .fcnorm16_eq_1_f0a_h                      ( fcnorm16_eq_1_f0a_h ),
    .fcnorm8_eq_1_f0a_h                       ( fcnorm8_eq_1_f0a_h ),
    .fcnorm_30_f0a_h                          ( fcnorm_30_f0a_h ),
    .faph_hi_b0_f0a_h                         ( faph_hi_b0_f0a_h ),
    .fbph_hi_b0_f0a_h                         ( fbph_hi_b0_f0a_h ),
    .fcph_b0_f0a_h                            ( fcph_b0_f0a_h ),
    .fcf11_b0_f0a_h                           ( fcf11_b0_f0a_h ),
    .fcf10_b0_f0a_h                           ( fcf10_b0_f0a_h ),
    .fcn8_one_30_24_f0a_h                     ( fcn8_one_30_24_f0a_h ),
    .fcn10_one_30_22_f0a_h                    ( fcn10_one_30_22_f0a_h ),
    .fcn16_one_30_16_f0a_h                    ( fcn16_one_30_16_f0a_h ),
    .fcn24_one_30_8_f0a_h                     ( fcn24_one_30_8_f0a_h ),
    .faps_zero_f0a_l                          ( faps_zero_f0a_l ),
    .fbps_zero_f0a_l                          ( fbps_zero_f0a_l ),
    .faph_hi_zero_f0a_l                       ( faph_hi_zero_f0a_l ),
    .fbph_hi_zero_f0a_l                       ( fbph_hi_zero_f0a_l ),
    .faph_lo_zero_f0a_l                       ( faph_lo_zero_f0a_l ),
    .fbph_lo_zero_f0a_l                       ( fbph_lo_zero_f0a_l ),
    .faph_lo_b0_f0a_h                         ( faph_lo_b0_f0a_h ),
    .fbph_lo_b0_f0a_h                         ( fbph_lo_b0_f0a_h ),
    .faps_b0_f0a_h                            ( faps_b0_f0a_h ),
    .fbps_b0_f0a_h                            ( fbps_b0_f0a_h ),
    .fc_31_f0a_h                              ( fc_31_f0a_h ),
    .fcps_b0_f0a_h                            ( fcps_b0_f0a_h ),
    .ea_hi_f1a_h                              ( ea_hi_f1a_h ),
    .ediff1_f2a_h                             ( ediff1_f2a_h ),
    .ediff1_abs_f2a_h                         ( ediff1_abs_f2a_h ),
    .ediff2_abs_f2a_h                         ( ediff2_abs_f2a_h ),
    .ediff1_sel_f2a_h                         ( ediff1_sel_f2a_h ),
    .ediff2_sel_f2a_h                         ( ediff2_sel_f2a_h ),
    .align1_mux_sel_f2a_h                     ( align1_mux_sel_f2a_h ),
    .align2_mux_sel_f2a_h                     ( align2_mux_sel_f2a_h ),
    .ec_f2a_h                                 ( ec_f2a_h ),
    .fc_b0_f2a_h                              ( fc_b0_f2a_h ),
    .e_sticky1_f2a_h                          ( e_sticky1_f2a_h ),
    .e_sticky2_f2a_h                          ( e_sticky2_f2a_h ),
    .opd3_mux_sel_f2a_h                       ( opd3_mux_sel_f2a_h ),
    .ediff1_f3a_h                             ( ediff1_f3a_h ),
    .ediff2_f3a_h                             ( ediff2_f3a_h ),
    .ec_f3a_h                                 ( ec_f3a_h ),
    .e_sticky1_f3a_h                          ( e_sticky1_f3a_h ),
    .prodsum_a2_f3a_h                         ( prodsum_a2_f3a_h ),
    .fround_rbit_f3a_h                        ( fround_rbit_f3a_h ),
    .ph_comb_stiky_f4a_h                      ( ph_comb_stiky_f4a_h ),
    .elxd_f4a_h                               ( elxd_f4a_h ),
    .etrz50_f4a_h                             ( etrz50_f4a_h ),
    .stky2g_f5a_h                             ( stky2g_f5a_h ),
    .s_res_f5a_h                              ( s_res_f5a_h ),
    .comb_add_a3_f5a_h                        ( comb_add_a3_f5a_h ),
    .ediff1_zap_f5a_h                         ( ediff1_zap_f5a_h ),
    .rmode_f6a_h                              ( rmode_f6a_h ),
    .exp_res_zero_f6a_h                       ( exp_res_zero_f6a_h ),
    .s_res_f6a_h                              ( s_res_f6a_h ),
    .ph_stky2l_f6a_h                          ( ph_stky2l_f6a_h ),
    .ps_stky2l_f6a_h                          ( ps_stky2l_f6a_h ),
    .int32_stky2l_f6a_h                       ( int32_stky2l_f6a_h ),
    .pot_ovf_ps_f6a_h                         ( pot_ovf_ps_f6a_h ),
    .def_ovf_ps_f6a_h                         ( def_ovf_ps_f6a_h ),
    .pot_unf_ps_f6a_h                         ( pot_unf_ps_f6a_h ),
    .def_unf_ps_f6a_h                         ( def_unf_ps_f6a_h ),
    .pot_ovf_ph_f6a_h                         ( pot_ovf_ph_f6a_h ),
    .def_ovf_ph_f6a_h                         ( def_ovf_ph_f6a_h ),
    .pot_unf_ph_f6a_h                         ( pot_unf_ph_f6a_h ),
    .def_unf_ph_f6a_h                         ( def_unf_ph_f6a_h ),
    .frac_zero_f6a_h                          ( frac_zero_f6a_h ),
    .rnd_l_ph_f6a_h                           ( rnd_l_ph_f6a_h ),
    .rnd_r_ph_f6a_h                           ( rnd_r_ph_f6a_h ),
    .rnd_l_ps_f6a_h                           ( rnd_l_ps_f6a_h ),
    .rnd_r_ps_f6a_h                           ( rnd_r_ps_f6a_h ),
    .int_res_l_bit_f6a_h                      ( int_res_l_bit_f6a_h ),
    .ph_allone2r_f6a_h                        ( ph_allone2r_f6a_h ),
    .ph_allone2l_f6a_h                        ( ph_allone2l_f6a_h ),
    .ps_allone2r_f6a_h                        ( ps_allone2r_f6a_h ),
    .ps_allone2l_f6a_h                        ( ps_allone2l_f6a_h ),
    .is_trans_rfa_h                           ( orig_is_trans_rfa_h ),
    .sel_eblo_ph_exa_h                        ( orig_sel_eblo_ph_exa_h ),
    .sel_ec5bdet_exa_h                        ( orig_sel_ec5bdet_exa_h ),
    .elo_eb_en_exa_h                          ( orig_elo_eb_en_exa_h ),
    .ehi_eb_en_exa_h                          ( orig_ehi_eb_en_exa_h ),
    .elo_ea_en_exa_h                          ( orig_elo_ea_en_exa_h ),
    .fa_en_exa_h                              ( orig_fa_en_exa_h ),
    .fb_en_exa_h                              ( orig_fb_en_exa_h ),
    .fc_en_exa_h                              ( orig_fc_en_exa_h ),
    .sel_ealo_ph_exa_h                        ( orig_sel_ealo_ph_exa_h ),
    .sa_hi_en_exa_h                           ( orig_sa_hi_en_exa_h ),
    .sa_lo_en_exa_h                           ( orig_sa_lo_en_exa_h ),
    .sb_hi_en_exa_h                           ( orig_sb_hi_en_exa_h ),
    .sb_lo_en_exa_h                           ( orig_sb_lo_en_exa_h ),
    .sc_en_exa_h                              ( orig_sc_en_exa_h ),
    .ehi_ea_en_exa_h                          ( orig_ehi_ea_en_exa_h ),
    .ec_en_exa_h                              ( orig_ec_en_exa_h ),
    .neg_fmaop_exa_h                          ( orig_neg_fmaop_exa_h ),
    .trans_op2_exa_l                          ( orig_trans_op2_exa_l ),
    .sa1_nr2_frcpfxp_exa_h                    ( orig_sa1_nr2_frcpfxp_exa_h ),
    .sel_sign_sinp2_exa_h                     ( orig_sel_sign_sinp2_exa_h ),
    .sa_zap_exa_l                             ( orig_sa_zap_exa_l ),
    .sb_zap_exa_l                             ( orig_sb_zap_exa_l ),
    .sel_f10_exa_h                            ( orig_sel_f10_exa_h ),
    .sel_f11_exa_h                            ( orig_sel_f11_exa_h ),
    .fc_dtps_exa_h                            ( orig_fc_dtps_exa_h ),
    .sel_snorm_exa_h                          ( orig_sel_snorm_exa_h ),
    .sel_g1sk2_exa_h                          ( orig_sel_g1sk2_exa_h ),
    .sel_g1sk8_exa_h                          ( orig_sel_g1sk8_exa_h ),
    .sel_g1sk16_exa_h                         ( orig_sel_g1sk16_exa_h ),
    .sel_g1sk24_exa_h                         ( orig_sel_g1sk24_exa_h ),
    .sel_xexpfrac_exa_h                       ( orig_sel_xexpfrac_exa_h ),
    .sel_ebin_hi_exa_h                        ( orig_sel_ebin_hi_exa_h ),
    .sel_eainmux_exa_h                        ( orig_sel_eainmux_exa_h ),
    .sel_ec5b_exa_h                           ( orig_sel_ec5b_exa_h ),
    .sel_ecin_exa_h                           ( orig_sel_ecin_exa_h ),
    .sel_sc_exa_h                             ( orig_sel_sc_exa_h ),
    .sel_opcgfx_exa_h                         ( orig_sel_opcgfx_exa_h ),
    .sel_xopc_exa_h                           ( orig_sel_xopc_exa_h ),
    .sel_opcfmt_exa_h                         ( orig_sel_opcfmt_exa_h ),
    .sel_transfa_exa_h                        ( orig_sel_transfa_exa_h ),
    .sel_transfb_exa_h                        ( orig_sel_transfb_exa_h ),
    .sel_opafmt_exa_h                         ( orig_sel_opafmt_exa_h ),
    .sel_opbfmt_exa_h                         ( orig_sel_opbfmt_exa_h ),
    .opa_valid_rfa_h                          ( orig_opa_valid_rfa_h ),
    .opb_valid_rfa_h                          ( orig_opb_valid_rfa_h ),
    .opc_valid_rfa_h                          ( orig_opc_valid_rfa_h ),
    .dtps_f0a_h                               ( orig_dtps_f0a_h ),
    .ctrl_valid_f0a_h                         ( orig_ctrl_valid_f0a_h ),
    .ctrl_valid_f1a_h                         ( orig_ctrl_valid_f1a_h ),
    .ctrl_valid_f2a_h                         ( orig_ctrl_valid_f2a_h ),
    .ctrl_valid_f3a_h                         ( orig_ctrl_valid_f3a_h ),
    .ctrl_valid_f4a_h                         ( orig_ctrl_valid_f4a_h ),
    .ctrl_valid_f5a_h                         ( orig_ctrl_valid_f5a_h ),
    .ctrl_valid_f6a_h                         ( orig_ctrl_valid_f6a_h ),
    .mul_sign_f0a_h                           ( orig_mul_sign_f0a_h ),
    .mul_hp_mode_f0a_h                        ( orig_mul_hp_mode_f0a_h ),
    .eb_hi_k_exa_h                            ( orig_eb_hi_k_exa_h ),
    .use_mul_f0a_h                            ( orig_use_mul_f0a_h ),
    .ec_zero_f1a_h                            ( orig_ec_zero_f1a_h ),
    .ea_hi_zero_f1a_h                         ( orig_ea_hi_zero_f1a_h ),
    .fc_zero30_8_f1a_h                        ( orig_fc_zero30_8_f1a_h ),
    .paddin1_sels_f1a_h                       ( orig_paddin1_sels_f1a_h ),
    .paddin2_selc_f1a_h                       ( orig_paddin2_selc_f1a_h ),
    .paddin2_selfcneg_f1a_h                   ( orig_paddin2_selfcneg_f1a_h ),
    .padd_cin_f1a_h                           ( orig_padd_cin_f1a_h ),
    .sel_famask_f1a_h                         ( orig_sel_famask_f1a_h ),
    .fa_en_f1a_h                              ( orig_fa_en_f1a_h ),
    .cmd_f1a_h                                ( orig_cmd_f1a_h ),
    .sigs_f1a_h                               ( orig_sigs_f1a_h ),
    .op_dtype_f1a_h                           ( orig_op_dtype_f1a_h ),
    .use_mul_f1a_h                            ( orig_use_mul_f1a_h ),
    .ec_zero_f2a_h                            ( orig_ec_zero_f2a_h ),
    .eprod_hi_zero_f2a_h                      ( orig_eprod_hi_zero_f2a_h ),
    .eprod_lo_zero_f2a_h                      ( orig_eprod_lo_zero_f2a_h ),
    .op3p1c_sel_p_f2a_h                       ( orig_op3p1c_sel_p_f2a_h ),
    .op3p1c_sel_r_f2a_h                       ( orig_op3p1c_sel_r_f2a_h ),
    .op3g_sel_f2a_h                           ( orig_op3g_sel_f2a_h ),
    .rm_rne_f2a_h                             ( orig_rm_rne_f2a_h ),
    .rm_rmm_f2a_h                             ( orig_rm_rmm_f2a_h ),
    .frnd_rupnsc_f2a_h                        ( orig_frnd_rupnsc_f2a_h ),
    .frnd_rdnsc_f2a_h                         ( orig_frnd_rdnsc_f2a_h ),
    .frnd_t1_f2a_h                            ( orig_frnd_t1_f2a_h ),
    .frnd_t2_f2a_h                            ( orig_frnd_t2_f2a_h ),
    .al1c_zap_f2a_h                           ( orig_al1c_zap_f2a_h ),
    .op1c_log_mul_clr_f2a_h                   ( orig_op1c_log_mul_clr_f2a_h ),
    .negop1c_int_logmul_clr_f2a_h             ( orig_negop1c_int_logmul_clr_f2a_h ),
    .psc_op1_clr_f2a_h                        ( orig_psc_op1_clr_f2a_h ),
    .align2_zap_f2a_h                         ( orig_align2_zap_f2a_h ),
    .align2c_neg_f2a_h                        ( orig_align2c_neg_f2a_h ),
    .trans_exp_fma2_exc_f2a_h                 ( orig_trans_exp_fma2_exc_f2a_h ),
    .al1p1c_sel_f2a_h                         ( orig_al1p1c_sel_f2a_h ),
    .op2_sxtto_g_f2a_h                        ( orig_op2_sxtto_g_f2a_h ),
    .op3p3spc_sel_f2a_h                       ( orig_op3p3spc_sel_f2a_h ),
    .op3p2c_sel_f2a_h                         ( orig_op3p2c_sel_f2a_h ),
    .ugfxrplc_sel_f2a_h                       ( orig_ugfxrplc_sel_f2a_h ),
    .sgfxrplc_sel_f2a_h                       ( orig_sgfxrplc_sel_f2a_h ),
    .icgfxc_sel_f2a_h                         ( orig_icgfxc_sel_f2a_h ),
    .al1znp2c_sel_f2a_h                       ( orig_al1znp2c_sel_f2a_h ),
    .al2c_sel_f2a_h                           ( orig_al2c_sel_f2a_h ),
    .al1p2c_sel_f2a_h                         ( orig_al1p2c_sel_f2a_h ),
    .op_dtype_f2a_h                           ( orig_op_dtype_f2a_h ),
    .sigs_f2a_h                               ( orig_sigs_f2a_h ),
    .cmd_f2a_h                                ( orig_cmd_f2a_h ),
    .sin_ediff2_f2a_h                         ( orig_sin_ediff2_f2a_h ),
    .fma_subopc3_f3a_h                        ( orig_fma_subopc3_f3a_h ),
    .s_res_f3a_h                              ( orig_s_res_f3a_h ),
    .imxc_sel_f3a_h                           ( orig_imxc_sel_f3a_h ),
    .al2c3_neg_cin_ps_f3a_h                   ( orig_al2c3_neg_cin_ps_f3a_h ),
    .al1c3_neg_cin_f3a_h                      ( orig_al1c3_neg_cin_f3a_h ),
    .neg_norm_a3_f4a_h                        ( orig_neg_norm_a3_f4a_h ),
    .neg_norm_na3_f4a_h                       ( orig_neg_norm_na3_f4a_h ),
    .clearphps_en_f4a_h                       ( orig_clearphps_en_f4a_h ),
    .clear16frnd_nshin_f4a_l                  ( orig_clear16frnd_nshin_f4a_l ),
    .ph_ps_clear_f4a_l                        ( orig_ph_ps_clear_f4a_l ),
    .elxd_add_in2_mux_sel_f4a_h               ( orig_elxd_add_in2_mux_sel_f4a_h ),
    .use_elxd_f4a_h                           ( orig_use_elxd_f4a_h ),
    .fb_hi_zero_f4a_h                         ( orig_fb_hi_zero_f4a_h ),
    .enc_nsamt_f4a_h                          ( orig_enc_nsamt_f4a_h ),
    .int_minmax_mux_sel_a_f5a_h               ( orig_int_minmax_mux_sel_a_f5a_h ),
    .int_minmax_mux_sel_c_f5a_h               ( orig_int_minmax_mux_sel_c_f5a_h ),
    .rc_ps_f5a_h                              ( orig_rc_ps_f5a_h ),
    .rc_ph_f5a_h                              ( orig_rc_ph_f5a_h ),
    .rc6_ps_f5a_h                             ( orig_rc6_ps_f5a_h ),
    .rc6_ph_f5a_h                             ( orig_rc6_ph_f5a_h ),
    .rc_int32_f5a_h                           ( orig_rc_int32_f5a_h ),
    .rc_3rmxr_f5a_h                           ( orig_rc_3rmxr_f5a_h ),
    .rc_rl01_f5a_h                            ( orig_rc_rl01_f5a_h ),
    .rm_f5a_h                                 ( orig_rm_f5a_h ),
    .rc_rmm_f5a_h                             ( orig_rc_rmm_f5a_h ),
    .rc_rup_f5a_h                             ( orig_rc_rup_f5a_h ),
    .rc_rne_f5a_h                             ( orig_rc_rne_f5a_h ),
    .rc_rdn_f5a_h                             ( orig_rc_rdn_f5a_h ),
    .rc_res_sign_f5a_l                        ( orig_rc_res_sign_f5a_l ),
    .rc_transrnd_f5a_h                        ( orig_rc_transrnd_f5a_h ),
    .rc_transrnd_log_fma2_f5a_h               ( orig_rc_transrnd_log_fma2_f5a_h ),
    .nshcen_sel1b_f5a_h                       ( orig_nshcen_sel1b_f5a_h ),
    .en_logrrsel1b_f5a_l                      ( orig_en_logrrsel1b_f5a_l ),
    .int32_stky2r_f5a_h                       ( orig_int32_stky2r_f5a_h ),
    .mmx_sel_immres_f5a_h                     ( orig_mmx_sel_immres_f5a_h ),
    .comp_op_true_f5a_h                       ( orig_comp_op_true_f5a_h ),
    .comp_op_f5a_h                            ( orig_comp_op_f5a_h ),
    .trans2_ps_roundup_r_f5a_h                ( orig_trans2_ps_roundup_r_f5a_h ),
    .e_force_to_zero_f5a_h                    ( orig_e_force_to_zero_f5a_h ),
    .s_res_out_f5a_h                          ( orig_s_res_out_f5a_h ),
    .op_dtype_f5a_h                           ( orig_op_dtype_f5a_h ),
    .sigs_f5a_h                               ( orig_sigs_f5a_h ),
    .cmd_f5a_h                                ( orig_cmd_f5a_h ),
    .sel_gmaxu24_f6a_h                        ( orig_sel_gmaxu24_f6a_h ),
    .sel_gmaxu16_f6a_h                        ( orig_sel_gmaxu16_f6a_h ),
    .sel_gmaxs16_f6a_h                        ( orig_sel_gmaxs16_f6a_h ),
    .sel_gmins16_f6a_h                        ( orig_sel_gmins16_f6a_h ),
    .sel_gmaxs8_f6a_h                         ( orig_sel_gmaxs8_f6a_h ),
    .sel_gmaxu2_f6a_h                         ( orig_sel_gmaxu2_f6a_h ),
    .sel_gmins8_f6a_h                         ( orig_sel_gmins8_f6a_h ),
    .sel_gmaxu8_f6a_h                         ( orig_sel_gmaxu8_f6a_h ),
    .sel_gfxmax_f6a_h                         ( orig_sel_gfxmax_f6a_h ),
    .sel_psexc_sr_f6a_h                       ( orig_sel_psexc_sr_f6a_h ),
    .sel_bit31_one_f6a_h                      ( orig_sel_bit31_one_f6a_h ),
    .sel_b30to0_ones_f6a_h                    ( orig_sel_b30to0_ones_f6a_h ),
    .sel_zerores_f6a_h                        ( orig_sel_zerores_f6a_h ),
    .sel_ps_unf_f6a_h                         ( orig_sel_ps_unf_f6a_h ),
    .sel_ps_normone_f6a_h                     ( orig_sel_ps_normone_f6a_h ),
    .sel_ps_inf_f6a_h                         ( orig_sel_ps_inf_f6a_h ),
    .sel_ps_cqn_f6a_h                         ( orig_sel_ps_cqn_f6a_h ),
    .sel_ps_large_f6a_h                       ( orig_sel_ps_large_f6a_h ),
    .sel_phsp_f6a_h                           ( orig_sel_phsp_f6a_h ),
    .sel_phhi_sr_f6a_h                        ( orig_sel_phhi_sr_f6a_h ),
    .sel_phhi_large_f6a_h                     ( orig_sel_phhi_large_f6a_h ),
    .sel_phhi_unf_f6a_h                       ( orig_sel_phhi_unf_f6a_h ),
    .sel_phhi_infres_f6a_h                    ( orig_sel_phhi_infres_f6a_h ),
    .sel_f11inf_f6a_h                         ( orig_sel_f11inf_f6a_h ),
    .sel_f10cqnan_f6a_h                       ( orig_sel_f10cqnan_f6a_h ),
    .sel_f11sp_f6a_h                          ( orig_sel_f11sp_f6a_h ),
    .sel_f10sp_f6a_h                          ( orig_sel_f10sp_f6a_h ),
    .sel_f11large_f6a_h                       ( orig_sel_f11large_f6a_h ),
    .sel_f10large_f6a_h                       ( orig_sel_f10large_f6a_h ),
    .ps_rndovf_f6a_h                          ( orig_ps_rndovf_f6a_h ),
    .ph_rndovf_f6a_h                          ( orig_ph_rndovf_f6a_h ),
    .sel_set_bit0_one_f6a_h                   ( orig_sel_set_bit0_one_f6a_h ),
    .effsub1_set_sign1_f6a_h                  ( orig_effsub1_set_sign1_f6a_h ),
    .normmask1_f6a_h                          ( orig_normmask1_f6a_h ),
    .normmask2_f6a_h                          ( orig_normmask2_f6a_h ),
    .sel_nr1_frcpres_f6a_h                    ( orig_sel_nr1_frcpres_f6a_h ),
    .trans_sinp2m_f6a_h                       ( orig_trans_sinp2m_f6a_h ),
    .f10f11mux_sel_f6a_h                      ( orig_f10f11mux_sel_f6a_h ),
    .rrmux16l_sel_f6a_l                       ( orig_rrmux16l_sel_f6a_l ),
    .sel_classify_out_f6a_h                   ( orig_sel_classify_out_f6a_h ),
    .rrmux32_sel_f6a_h                        ( orig_rrmux32_sel_f6a_h ),
    .sel_excreshi_f6a_h                       ( orig_sel_excreshi_f6a_h ),
    .sel_excreslo_f6a_h                       ( orig_sel_excreslo_f6a_h ),
    .sel_fmares_hi_f6a_h                      ( orig_sel_fmares_hi_f6a_h ),
    .sel_fmares_lo_f6a_h                      ( orig_sel_fmares_lo_f6a_h ),
    .cmd_f6a_h                                ( orig_cmd_f6a_h ),
    .sigs_f6a_h                               ( orig_sigs_f6a_h ),
    .op_dtype_f6a_h                           ( orig_op_dtype_f6a_h ),
    .classify_f6a_h                           ( orig_classify_f6a_h ),
    .fma_flags_f6a_h                          ( orig_fma_flags_f6a_h ),
    .comp_op_true_f6a_h                       ( orig_comp_op_true_f6a_h )
  );

  txfmactl_top u_new (
    .clock                                    ( clk_i ),
    .reset                                    ( !rst_ni ),
    .in_valid_exa_h                           ( in_valid_exa_h ),
    .sigs_in_exa_h                            ( sigs_in_exa_h ),
    .rm_in_exa_h                              ( rm_in_exa_h ),
    .trans_mask_exa_h                         ( trans_mask_exa_h ),
    .trans_taylor_exa_h                       ( trans_taylor_exa_h ),
    .trans_exp_fma2_exc_exa_h                 ( trans_exp_fma2_exc_exa_h ),
    .use_prev_sigs_exa_h                      ( use_prev_sigs_exa_h ),
    .sa_hi_f0a_h                              ( sa_hi_f0a_h ),
    .sa_lo_f0a_h                              ( sa_lo_f0a_h ),
    .sb_hi_f0a_h                              ( sb_hi_f0a_h ),
    .sb_lo_f0a_h                              ( sb_lo_f0a_h ),
    .sc_f0a_h                                 ( sc_f0a_h ),
    .ea_hi_zero_f0a_l                         ( ea_hi_zero_f0a_l ),
    .ea_hi_all1_f0a_h                         ( ea_hi_all1_f0a_h ),
    .ea_lo_zero_f0a_l                         ( ea_lo_zero_f0a_l ),
    .ea_lo_all1_f0a_h                         ( ea_lo_all1_f0a_h ),
    .eb_hi_zero_f0a_l                         ( eb_hi_zero_f0a_l ),
    .eb_hi_all1_f0a_h                         ( eb_hi_all1_f0a_h ),
    .eb_lo_zero_f0a_l                         ( eb_lo_zero_f0a_l ),
    .eb_lo_all1_f0a_h                         ( eb_lo_all1_f0a_h ),
    .ec_zero_f0a_l                            ( ec_zero_f0a_l ),
    .ec_all1_f0a_h                            ( ec_all1_f0a_h ),
    .eb_eql_127_f0a_h                         ( eb_eql_127_f0a_h ),
    .eb_eql_126_f0a_h                         ( eb_eql_126_f0a_h ),
    .eb_eql_125_f0a_h                         ( eb_eql_125_f0a_h ),
    .fcf10_zero_f0a_l                         ( fcf10_zero_f0a_l ),
    .fcf11_zero_f0a_l                         ( fcf11_zero_f0a_l ),
    .fcps_zero_f0a_h                          ( fcps_zero_f0a_h ),
    .fcph_zero_f0a_l                          ( fcph_zero_f0a_l ),
    .fcnorm_zero_29_8_f0a_h                   ( fcnorm_zero_29_8_f0a_h ),
    .fcnorm16_eq_1_f0a_h                      ( fcnorm16_eq_1_f0a_h ),
    .fcnorm8_eq_1_f0a_h                       ( fcnorm8_eq_1_f0a_h ),
    .fcnorm_30_f0a_h                          ( fcnorm_30_f0a_h ),
    .faph_hi_b0_f0a_h                         ( faph_hi_b0_f0a_h ),
    .fbph_hi_b0_f0a_h                         ( fbph_hi_b0_f0a_h ),
    .fcph_b0_f0a_h                            ( fcph_b0_f0a_h ),
    .fcf11_b0_f0a_h                           ( fcf11_b0_f0a_h ),
    .fcf10_b0_f0a_h                           ( fcf10_b0_f0a_h ),
    .fcn8_one_30_24_f0a_h                     ( fcn8_one_30_24_f0a_h ),
    .fcn10_one_30_22_f0a_h                    ( fcn10_one_30_22_f0a_h ),
    .fcn16_one_30_16_f0a_h                    ( fcn16_one_30_16_f0a_h ),
    .fcn24_one_30_8_f0a_h                     ( fcn24_one_30_8_f0a_h ),
    .faps_zero_f0a_l                          ( faps_zero_f0a_l ),
    .fbps_zero_f0a_l                          ( fbps_zero_f0a_l ),
    .faph_hi_zero_f0a_l                       ( faph_hi_zero_f0a_l ),
    .fbph_hi_zero_f0a_l                       ( fbph_hi_zero_f0a_l ),
    .faph_lo_zero_f0a_l                       ( faph_lo_zero_f0a_l ),
    .fbph_lo_zero_f0a_l                       ( fbph_lo_zero_f0a_l ),
    .faph_lo_b0_f0a_h                         ( faph_lo_b0_f0a_h ),
    .fbph_lo_b0_f0a_h                         ( fbph_lo_b0_f0a_h ),
    .faps_b0_f0a_h                            ( faps_b0_f0a_h ),
    .fbps_b0_f0a_h                            ( fbps_b0_f0a_h ),
    .fc_31_f0a_h                              ( fc_31_f0a_h ),
    .fcps_b0_f0a_h                            ( fcps_b0_f0a_h ),
    .ea_hi_f1a_h                              ( ea_hi_f1a_h ),
    .ediff1_f2a_h                             ( ediff1_f2a_h ),
    .ediff1_abs_f2a_h                         ( ediff1_abs_f2a_h ),
    .ediff2_abs_f2a_h                         ( ediff2_abs_f2a_h ),
    .ediff1_sel_f2a_h                         ( ediff1_sel_f2a_h ),
    .ediff2_sel_f2a_h                         ( ediff2_sel_f2a_h ),
    .align1_mux_sel_f2a_h                     ( align1_mux_sel_f2a_h ),
    .align2_mux_sel_f2a_h                     ( align2_mux_sel_f2a_h ),
    .ec_f2a_h                                 ( ec_f2a_h ),
    .fc_b0_f2a_h                              ( fc_b0_f2a_h ),
    .e_sticky1_f2a_h                          ( e_sticky1_f2a_h ),
    .e_sticky2_f2a_h                          ( e_sticky2_f2a_h ),
    .opd3_mux_sel_f2a_h                       ( opd3_mux_sel_f2a_h ),
    .ediff1_f3a_h                             ( ediff1_f3a_h ),
    .ediff2_f3a_h                             ( ediff2_f3a_h ),
    .ec_f3a_h                                 ( ec_f3a_h ),
    .e_sticky1_f3a_h                          ( e_sticky1_f3a_h ),
    .prodsum_a2_f3a_h                         ( prodsum_a2_f3a_h ),
    .fround_rbit_f3a_h                        ( fround_rbit_f3a_h ),
    .ph_comb_stiky_f4a_h                      ( ph_comb_stiky_f4a_h ),
    .elxd_f4a_h                               ( elxd_f4a_h ),
    .etrz50_f4a_h                             ( etrz50_f4a_h ),
    .stky2g_f5a_h                             ( stky2g_f5a_h ),
    .s_res_f5a_h                              ( s_res_f5a_h ),
    .comb_add_a3_f5a_h                        ( comb_add_a3_f5a_h ),
    .ediff1_zap_f5a_h                         ( ediff1_zap_f5a_h ),
    .rmode_f6a_h                              ( rmode_f6a_h ),
    .exp_res_zero_f6a_h                       ( exp_res_zero_f6a_h ),
    .s_res_f6a_h                              ( s_res_f6a_h ),
    .ph_stky2l_f6a_h                          ( ph_stky2l_f6a_h ),
    .ps_stky2l_f6a_h                          ( ps_stky2l_f6a_h ),
    .int32_stky2l_f6a_h                       ( int32_stky2l_f6a_h ),
    .pot_ovf_ps_f6a_h                         ( pot_ovf_ps_f6a_h ),
    .def_ovf_ps_f6a_h                         ( def_ovf_ps_f6a_h ),
    .pot_unf_ps_f6a_h                         ( pot_unf_ps_f6a_h ),
    .def_unf_ps_f6a_h                         ( def_unf_ps_f6a_h ),
    .pot_ovf_ph_f6a_h                         ( pot_ovf_ph_f6a_h ),
    .def_ovf_ph_f6a_h                         ( def_ovf_ph_f6a_h ),
    .pot_unf_ph_f6a_h                         ( pot_unf_ph_f6a_h ),
    .def_unf_ph_f6a_h                         ( def_unf_ph_f6a_h ),
    .frac_zero_f6a_h                          ( frac_zero_f6a_h ),
    .rnd_l_ph_f6a_h                           ( rnd_l_ph_f6a_h ),
    .rnd_r_ph_f6a_h                           ( rnd_r_ph_f6a_h ),
    .rnd_l_ps_f6a_h                           ( rnd_l_ps_f6a_h ),
    .rnd_r_ps_f6a_h                           ( rnd_r_ps_f6a_h ),
    .int_res_l_bit_f6a_h                      ( int_res_l_bit_f6a_h ),
    .ph_allone2r_f6a_h                        ( ph_allone2r_f6a_h ),
    .ph_allone2l_f6a_h                        ( ph_allone2l_f6a_h ),
    .ps_allone2r_f6a_h                        ( ps_allone2r_f6a_h ),
    .ps_allone2l_f6a_h                        ( ps_allone2l_f6a_h ),
    .is_trans_rfa_h                           ( new_is_trans_rfa_h ),
    .sel_eblo_ph_exa_h                        ( new_sel_eblo_ph_exa_h ),
    .sel_ec5bdet_exa_h                        ( new_sel_ec5bdet_exa_h ),
    .elo_eb_en_exa_h                          ( new_elo_eb_en_exa_h ),
    .ehi_eb_en_exa_h                          ( new_ehi_eb_en_exa_h ),
    .elo_ea_en_exa_h                          ( new_elo_ea_en_exa_h ),
    .fa_en_exa_h                              ( new_fa_en_exa_h ),
    .fb_en_exa_h                              ( new_fb_en_exa_h ),
    .fc_en_exa_h                              ( new_fc_en_exa_h ),
    .sel_ealo_ph_exa_h                        ( new_sel_ealo_ph_exa_h ),
    .sa_hi_en_exa_h                           ( new_sa_hi_en_exa_h ),
    .sa_lo_en_exa_h                           ( new_sa_lo_en_exa_h ),
    .sb_hi_en_exa_h                           ( new_sb_hi_en_exa_h ),
    .sb_lo_en_exa_h                           ( new_sb_lo_en_exa_h ),
    .sc_en_exa_h                              ( new_sc_en_exa_h ),
    .ehi_ea_en_exa_h                          ( new_ehi_ea_en_exa_h ),
    .ec_en_exa_h                              ( new_ec_en_exa_h ),
    .neg_fmaop_exa_h                          ( new_neg_fmaop_exa_h ),
    .trans_op2_exa_l                          ( new_trans_op2_exa_l ),
    .sa1_nr2_frcpfxp_exa_h                    ( new_sa1_nr2_frcpfxp_exa_h ),
    .sel_sign_sinp2_exa_h                     ( new_sel_sign_sinp2_exa_h ),
    .sa_zap_exa_l                             ( new_sa_zap_exa_l ),
    .sb_zap_exa_l                             ( new_sb_zap_exa_l ),
    .sel_f10_exa_h                            ( new_sel_f10_exa_h ),
    .sel_f11_exa_h                            ( new_sel_f11_exa_h ),
    .fc_dtps_exa_h                            ( new_fc_dtps_exa_h ),
    .sel_snorm_exa_h                          ( new_sel_snorm_exa_h ),
    .sel_g1sk2_exa_h                          ( new_sel_g1sk2_exa_h ),
    .sel_g1sk8_exa_h                          ( new_sel_g1sk8_exa_h ),
    .sel_g1sk16_exa_h                         ( new_sel_g1sk16_exa_h ),
    .sel_g1sk24_exa_h                         ( new_sel_g1sk24_exa_h ),
    .sel_xexpfrac_exa_h                       ( new_sel_xexpfrac_exa_h ),
    .sel_ebin_hi_exa_h                        ( new_sel_ebin_hi_exa_h ),
    .sel_eainmux_exa_h                        ( new_sel_eainmux_exa_h ),
    .sel_ec5b_exa_h                           ( new_sel_ec5b_exa_h ),
    .sel_ecin_exa_h                           ( new_sel_ecin_exa_h ),
    .sel_sc_exa_h                             ( new_sel_sc_exa_h ),
    .sel_opcgfx_exa_h                         ( new_sel_opcgfx_exa_h ),
    .sel_xopc_exa_h                           ( new_sel_xopc_exa_h ),
    .sel_opcfmt_exa_h                         ( new_sel_opcfmt_exa_h ),
    .sel_transfa_exa_h                        ( new_sel_transfa_exa_h ),
    .sel_transfb_exa_h                        ( new_sel_transfb_exa_h ),
    .sel_opafmt_exa_h                         ( new_sel_opafmt_exa_h ),
    .sel_opbfmt_exa_h                         ( new_sel_opbfmt_exa_h ),
    .opa_valid_rfa_h                          ( new_opa_valid_rfa_h ),
    .opb_valid_rfa_h                          ( new_opb_valid_rfa_h ),
    .opc_valid_rfa_h                          ( new_opc_valid_rfa_h ),
    .dtps_f0a_h                               ( new_dtps_f0a_h ),
    .ctrl_valid_f0a_h                         ( new_ctrl_valid_f0a_h ),
    .ctrl_valid_f1a_h                         ( new_ctrl_valid_f1a_h ),
    .ctrl_valid_f2a_h                         ( new_ctrl_valid_f2a_h ),
    .ctrl_valid_f3a_h                         ( new_ctrl_valid_f3a_h ),
    .ctrl_valid_f4a_h                         ( new_ctrl_valid_f4a_h ),
    .ctrl_valid_f5a_h                         ( new_ctrl_valid_f5a_h ),
    .ctrl_valid_f6a_h                         ( new_ctrl_valid_f6a_h ),
    .mul_sign_f0a_h                           ( new_mul_sign_f0a_h ),
    .mul_hp_mode_f0a_h                        ( new_mul_hp_mode_f0a_h ),
    .eb_hi_k_exa_h                            ( new_eb_hi_k_exa_h ),
    .use_mul_f0a_h                            ( new_use_mul_f0a_h ),
    .ec_zero_f1a_h                            ( new_ec_zero_f1a_h ),
    .ea_hi_zero_f1a_h                         ( new_ea_hi_zero_f1a_h ),
    .fc_zero30_8_f1a_h                        ( new_fc_zero30_8_f1a_h ),
    .paddin1_sels_f1a_h                       ( new_paddin1_sels_f1a_h ),
    .paddin2_selc_f1a_h                       ( new_paddin2_selc_f1a_h ),
    .paddin2_selfcneg_f1a_h                   ( new_paddin2_selfcneg_f1a_h ),
    .padd_cin_f1a_h                           ( new_padd_cin_f1a_h ),
    .sel_famask_f1a_h                         ( new_sel_famask_f1a_h ),
    .fa_en_f1a_h                              ( new_fa_en_f1a_h ),
    .cmd_f1a_h                                ( new_cmd_f1a_h ),
    .sigs_f1a_h                               ( new_sigs_f1a_h ),
    .op_dtype_f1a_h                           ( new_op_dtype_f1a_h ),
    .use_mul_f1a_h                            ( new_use_mul_f1a_h ),
    .ec_zero_f2a_h                            ( new_ec_zero_f2a_h ),
    .eprod_hi_zero_f2a_h                      ( new_eprod_hi_zero_f2a_h ),
    .eprod_lo_zero_f2a_h                      ( new_eprod_lo_zero_f2a_h ),
    .op3p1c_sel_p_f2a_h                       ( new_op3p1c_sel_p_f2a_h ),
    .op3p1c_sel_r_f2a_h                       ( new_op3p1c_sel_r_f2a_h ),
    .op3g_sel_f2a_h                           ( new_op3g_sel_f2a_h ),
    .rm_rne_f2a_h                             ( new_rm_rne_f2a_h ),
    .rm_rmm_f2a_h                             ( new_rm_rmm_f2a_h ),
    .frnd_rupnsc_f2a_h                        ( new_frnd_rupnsc_f2a_h ),
    .frnd_rdnsc_f2a_h                         ( new_frnd_rdnsc_f2a_h ),
    .frnd_t1_f2a_h                            ( new_frnd_t1_f2a_h ),
    .frnd_t2_f2a_h                            ( new_frnd_t2_f2a_h ),
    .al1c_zap_f2a_h                           ( new_al1c_zap_f2a_h ),
    .op1c_log_mul_clr_f2a_h                   ( new_op1c_log_mul_clr_f2a_h ),
    .negop1c_int_logmul_clr_f2a_h             ( new_negop1c_int_logmul_clr_f2a_h ),
    .psc_op1_clr_f2a_h                        ( new_psc_op1_clr_f2a_h ),
    .align2_zap_f2a_h                         ( new_align2_zap_f2a_h ),
    .align2c_neg_f2a_h                        ( new_align2c_neg_f2a_h ),
    .trans_exp_fma2_exc_f2a_h                 ( new_trans_exp_fma2_exc_f2a_h ),
    .al1p1c_sel_f2a_h                         ( new_al1p1c_sel_f2a_h ),
    .op2_sxtto_g_f2a_h                        ( new_op2_sxtto_g_f2a_h ),
    .op3p3spc_sel_f2a_h                       ( new_op3p3spc_sel_f2a_h ),
    .op3p2c_sel_f2a_h                         ( new_op3p2c_sel_f2a_h ),
    .ugfxrplc_sel_f2a_h                       ( new_ugfxrplc_sel_f2a_h ),
    .sgfxrplc_sel_f2a_h                       ( new_sgfxrplc_sel_f2a_h ),
    .icgfxc_sel_f2a_h                         ( new_icgfxc_sel_f2a_h ),
    .al1znp2c_sel_f2a_h                       ( new_al1znp2c_sel_f2a_h ),
    .al2c_sel_f2a_h                           ( new_al2c_sel_f2a_h ),
    .al1p2c_sel_f2a_h                         ( new_al1p2c_sel_f2a_h ),
    .op_dtype_f2a_h                           ( new_op_dtype_f2a_h ),
    .sigs_f2a_h                               ( new_sigs_f2a_h ),
    .cmd_f2a_h                                ( new_cmd_f2a_h ),
    .sin_ediff2_f2a_h                         ( new_sin_ediff2_f2a_h ),
    .fma_subopc3_f3a_h                        ( new_fma_subopc3_f3a_h ),
    .s_res_f3a_h                              ( new_s_res_f3a_h ),
    .imxc_sel_f3a_h                           ( new_imxc_sel_f3a_h ),
    .al2c3_neg_cin_ps_f3a_h                   ( new_al2c3_neg_cin_ps_f3a_h ),
    .al1c3_neg_cin_f3a_h                      ( new_al1c3_neg_cin_f3a_h ),
    .neg_norm_a3_f4a_h                        ( new_neg_norm_a3_f4a_h ),
    .neg_norm_na3_f4a_h                       ( new_neg_norm_na3_f4a_h ),
    .clearphps_en_f4a_h                       ( new_clearphps_en_f4a_h ),
    .clear16frnd_nshin_f4a_l                  ( new_clear16frnd_nshin_f4a_l ),
    .ph_ps_clear_f4a_l                        ( new_ph_ps_clear_f4a_l ),
    .elxd_add_in2_mux_sel_f4a_h               ( new_elxd_add_in2_mux_sel_f4a_h ),
    .use_elxd_f4a_h                           ( new_use_elxd_f4a_h ),
    .fb_hi_zero_f4a_h                         ( new_fb_hi_zero_f4a_h ),
    .enc_nsamt_f4a_h                          ( new_enc_nsamt_f4a_h ),
    .int_minmax_mux_sel_a_f5a_h               ( new_int_minmax_mux_sel_a_f5a_h ),
    .int_minmax_mux_sel_c_f5a_h               ( new_int_minmax_mux_sel_c_f5a_h ),
    .rc_ps_f5a_h                              ( new_rc_ps_f5a_h ),
    .rc_ph_f5a_h                              ( new_rc_ph_f5a_h ),
    .rc6_ps_f5a_h                             ( new_rc6_ps_f5a_h ),
    .rc6_ph_f5a_h                             ( new_rc6_ph_f5a_h ),
    .rc_int32_f5a_h                           ( new_rc_int32_f5a_h ),
    .rc_3rmxr_f5a_h                           ( new_rc_3rmxr_f5a_h ),
    .rc_rl01_f5a_h                            ( new_rc_rl01_f5a_h ),
    .rm_f5a_h                                 ( new_rm_f5a_h ),
    .rc_rmm_f5a_h                             ( new_rc_rmm_f5a_h ),
    .rc_rup_f5a_h                             ( new_rc_rup_f5a_h ),
    .rc_rne_f5a_h                             ( new_rc_rne_f5a_h ),
    .rc_rdn_f5a_h                             ( new_rc_rdn_f5a_h ),
    .rc_res_sign_f5a_l                        ( new_rc_res_sign_f5a_l ),
    .rc_transrnd_f5a_h                        ( new_rc_transrnd_f5a_h ),
    .rc_transrnd_log_fma2_f5a_h               ( new_rc_transrnd_log_fma2_f5a_h ),
    .nshcen_sel1b_f5a_h                       ( new_nshcen_sel1b_f5a_h ),
    .en_logrrsel1b_f5a_l                      ( new_en_logrrsel1b_f5a_l ),
    .int32_stky2r_f5a_h                       ( new_int32_stky2r_f5a_h ),
    .mmx_sel_immres_f5a_h                     ( new_mmx_sel_immres_f5a_h ),
    .comp_op_true_f5a_h                       ( new_comp_op_true_f5a_h ),
    .comp_op_f5a_h                            ( new_comp_op_f5a_h ),
    .trans2_ps_roundup_r_f5a_h                ( new_trans2_ps_roundup_r_f5a_h ),
    .e_force_to_zero_f5a_h                    ( new_e_force_to_zero_f5a_h ),
    .s_res_out_f5a_h                          ( new_s_res_out_f5a_h ),
    .op_dtype_f5a_h                           ( new_op_dtype_f5a_h ),
    .sigs_f5a_h                               ( new_sigs_f5a_h ),
    .cmd_f5a_h                                ( new_cmd_f5a_h ),
    .sel_gmaxu24_f6a_h                        ( new_sel_gmaxu24_f6a_h ),
    .sel_gmaxu16_f6a_h                        ( new_sel_gmaxu16_f6a_h ),
    .sel_gmaxs16_f6a_h                        ( new_sel_gmaxs16_f6a_h ),
    .sel_gmins16_f6a_h                        ( new_sel_gmins16_f6a_h ),
    .sel_gmaxs8_f6a_h                         ( new_sel_gmaxs8_f6a_h ),
    .sel_gmaxu2_f6a_h                         ( new_sel_gmaxu2_f6a_h ),
    .sel_gmins8_f6a_h                         ( new_sel_gmins8_f6a_h ),
    .sel_gmaxu8_f6a_h                         ( new_sel_gmaxu8_f6a_h ),
    .sel_gfxmax_f6a_h                         ( new_sel_gfxmax_f6a_h ),
    .sel_psexc_sr_f6a_h                       ( new_sel_psexc_sr_f6a_h ),
    .sel_bit31_one_f6a_h                      ( new_sel_bit31_one_f6a_h ),
    .sel_b30to0_ones_f6a_h                    ( new_sel_b30to0_ones_f6a_h ),
    .sel_zerores_f6a_h                        ( new_sel_zerores_f6a_h ),
    .sel_ps_unf_f6a_h                         ( new_sel_ps_unf_f6a_h ),
    .sel_ps_normone_f6a_h                     ( new_sel_ps_normone_f6a_h ),
    .sel_ps_inf_f6a_h                         ( new_sel_ps_inf_f6a_h ),
    .sel_ps_cqn_f6a_h                         ( new_sel_ps_cqn_f6a_h ),
    .sel_ps_large_f6a_h                       ( new_sel_ps_large_f6a_h ),
    .sel_phsp_f6a_h                           ( new_sel_phsp_f6a_h ),
    .sel_phhi_sr_f6a_h                        ( new_sel_phhi_sr_f6a_h ),
    .sel_phhi_large_f6a_h                     ( new_sel_phhi_large_f6a_h ),
    .sel_phhi_unf_f6a_h                       ( new_sel_phhi_unf_f6a_h ),
    .sel_phhi_infres_f6a_h                    ( new_sel_phhi_infres_f6a_h ),
    .sel_f11inf_f6a_h                         ( new_sel_f11inf_f6a_h ),
    .sel_f10cqnan_f6a_h                       ( new_sel_f10cqnan_f6a_h ),
    .sel_f11sp_f6a_h                          ( new_sel_f11sp_f6a_h ),
    .sel_f10sp_f6a_h                          ( new_sel_f10sp_f6a_h ),
    .sel_f11large_f6a_h                       ( new_sel_f11large_f6a_h ),
    .sel_f10large_f6a_h                       ( new_sel_f10large_f6a_h ),
    .ps_rndovf_f6a_h                          ( new_ps_rndovf_f6a_h ),
    .ph_rndovf_f6a_h                          ( new_ph_rndovf_f6a_h ),
    .sel_set_bit0_one_f6a_h                   ( new_sel_set_bit0_one_f6a_h ),
    .effsub1_set_sign1_f6a_h                  ( new_effsub1_set_sign1_f6a_h ),
    .normmask1_f6a_h                          ( new_normmask1_f6a_h ),
    .normmask2_f6a_h                          ( new_normmask2_f6a_h ),
    .sel_nr1_frcpres_f6a_h                    ( new_sel_nr1_frcpres_f6a_h ),
    .trans_sinp2m_f6a_h                       ( new_trans_sinp2m_f6a_h ),
    .f10f11mux_sel_f6a_h                      ( new_f10f11mux_sel_f6a_h ),
    .rrmux16l_sel_f6a_l                       ( new_rrmux16l_sel_f6a_l ),
    .sel_classify_out_f6a_h                   ( new_sel_classify_out_f6a_h ),
    .rrmux32_sel_f6a_h                        ( new_rrmux32_sel_f6a_h ),
    .sel_excreshi_f6a_h                       ( new_sel_excreshi_f6a_h ),
    .sel_excreslo_f6a_h                       ( new_sel_excreslo_f6a_h ),
    .sel_fmares_hi_f6a_h                      ( new_sel_fmares_hi_f6a_h ),
    .sel_fmares_lo_f6a_h                      ( new_sel_fmares_lo_f6a_h ),
    .cmd_f6a_h                                ( new_cmd_f6a_h ),
    .sigs_f6a_h                               ( new_sigs_f6a_h ),
    .op_dtype_f6a_h                           ( new_op_dtype_f6a_h ),
    .classify_f6a_h                           ( new_classify_f6a_h ),
    .fma_flags_f6a_h                          ( new_fma_flags_f6a_h ),
    .comp_op_true_f6a_h                       ( new_comp_op_true_f6a_h )
  );

endmodule : cosim_txfmactl_top_tb
