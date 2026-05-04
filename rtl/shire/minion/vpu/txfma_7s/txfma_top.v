// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"


module txfma_top
#(
    parameter USE_MMI = 1
)
(
  input                         clock,
  input                         reset, 
  input                         in_valid,
  input  vpu_input_t            in_data,
  input  trans_fma_coefficients trans_coefficients,
  output vpu_output_t           out_data_res,
  output logic                  out_comp_res  
);

 
///////////////////////////////////////////////////////////////////////////////
// Internal logic
///////////////////////////////////////////////////////////////////////////////
logic                                        txfma0_eb_hi_lsb_f2a_h;
logic                                        txfma0_eb_hi_lsb_f3a_h;
logic                                        txfma0_eb_hi_lsb_f4a_h;
logic                                        txfma0_mul_sign_f0a_h;
logic                                        txfma0_mul_hp_mode_f0a_h;
logic                                        txfma0_ctrl_valid_f0a_h; 
logic                                        txfma0_ctrl_valid_f1a_h; 
logic                                        txfma0_ctrl_valid_f2a_h; 
logic                                        txfma0_ctrl_valid_f3a_h; 
logic                                        txfma0_ctrl_valid_f4a_h;
logic                                        txfma0_ctrl_valid_f5a_h;
logic                                        txfma0_ctrl_valid_f6a_h;
logic [`TXFMA_EXP_PH_SZ-1:0]                 txfma0_ea_lo_f1a_h;
logic [`TXFMA_EXP_PH_SZ-1:0]                 txfma0_eb_lo_f1a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_ea_hi_f0a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_ea_hi_f1a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_ea_hi_f2a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_ea_hi_f3a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_ea_hi_f4a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_ea_hi_f5a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_eb_hi_f1a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_ec_f0a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_ec_f1a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_ec_f2a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_ec_f3a_h;
logic [6:0]                                  txfma0_exp_res_2f3_f2a_h; 
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_exp_res_f6a_h; 
logic                                        txfma0_exp_res_zero_f6a_h; 
logic [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]          txfma0_ediff1_sel_f2a_h;
logic [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]          txfma0_ediff2_sel_f2a_h;
logic [`TXFMA_NORM_SHF_AMNT_SZ-1:0]          txfma0_enc_nsamt_f4a_h;
logic                                        txfma0_e_force_to_zero_f5a_h;
logic                                        txfma0_e_sticky1_f2a_h;
logic                                        txfma0_e_sticky1_f3a_h;
logic                                        txfma0_e_sticky2_f2a_h;
logic [`TXFMA_LXD_PS_SZ-1:0]                 txfma0_elxd_f4a_h;
logic                                        txfma0_use_elxd_f4a_h;
logic                                        txfma0_elxd_add_in2_mux_sel_f4a_h;
logic                                        txfma0_eprod_hi_zero_f2a_h;
logic                                        txfma0_eprod_lo_zero_f2a_h;
logic                                        txfma0_s_res_f3a_h; 
logic                                        txfma0_s_res_f4a_h; 
logic                                        txfma0_s_res_f5a_h;
logic                                        txfma0_s_res_out_f5a_h;
logic                                        txfma0_s_res_f6a_h;
logic [`VPU_DTYPE_SZ-1:0]                    txfma0_op_dtype_f1a_h;
logic [`VPU_DTYPE_SZ-1:0]                    txfma0_op_dtype_f2a_h;
logic [`VPU_DTYPE_SZ-1:0]                    txfma0_op_dtype_f5a_h;
logic [`VPU_DTYPE_SZ-1:0]                    txfma0_op_dtype_f6a_h;
logic [`VPU_FCMD_SZ-1:0]                     txfma0_cmd_f1a_h;
logic [`VPU_FCMD_SZ-1:0]                     txfma0_cmd_f2a_h;
logic [`VPU_FCMD_SZ-1:0]                     txfma0_cmd_f5a_h;
logic [`VPU_FCMD_SZ-1:0]                     txfma0_cmd_f6a_h;
logic [`TXFMA_SIGS_SZ-1:0]                   txfma0_sigs_f1a_h; 
logic [`TXFMA_SIGS_SZ-1:0]                   txfma0_sigs_f2a_h;
logic [`TXFMA_SIGS_SZ-1:0]                   txfma0_sigs_f5a_h;
logic [`TXFMA_SIGS_SZ-1:0]                   txfma0_sigs_f6a_h;
logic                                        txfma0_sa_hi_f0a_h; 
logic                                        txfma0_sa_lo_f0a_h; 
logic                                        txfma0_sb_hi_f0a_h; 
logic                                        txfma0_sb_lo_f0a_h; 
logic                                        txfma0_sc_f0a_h;
logic [3:0]                                  txfma0_etrz_fa_hi_ph_f1a_h;
logic [4:0]                                  txfma0_etrz_fa_lo_ps_f1a_h;
logic [3:0]                                  txfma0_etrz_fb_hi_ph_f1a_h;
logic [4:0]                                  txfma0_etrz_fb_lo_ps_f1a_h;
logic [4:0]                                  txfma0_etrz_fc_f1a_h;
logic                                        txfma0_int_minmax_mux_sel_a_f5a_h;
logic                                        txfma0_int_minmax_mux_sel_c_f5a_h;
logic                                        txfma0_ea_hi_zero_f0a_l;
logic                                        txfma0_ea_hi_zero_f1a_h;
logic                         	             txfma0_ea_hi_all1_f0a_h;
logic				                         txfma0_ea_lo_zero_f0a_l;
logic                         	             txfma0_ea_lo_all1_f0a_h;
logic				                         txfma0_eb_hi_zero_f0a_l;
logic                         	             txfma0_eb_hi_all1_f0a_h;
logic				                         txfma0_eb_lo_zero_f0a_l;
logic                         	             txfma0_eb_lo_all1_f0a_h;
logic                         	             txfma0_ec_all1_f0a_h;
logic                         	             txfma0_ec_all1_f1a_h;
logic                                        txfma0_ec_zero_f0a_l;
logic                                        txfma0_ec_zero_f1a_h;
logic                                        txfma0_ec_zero_f2a_h;
logic                                        txfma0_trans_exp_fma2_exc_f2a_h;
logic                                        txfma0_eb_eql_127_f0a_h;
logic                                        txfma0_eb_eql_126_f0a_h;
logic                                        txfma0_eb_eql_125_f0a_h;
logic [`TXFMA_RM_SZ-1:0]                     txfma0_rm_f5a_h; 
logic                                        txfma0_rnd_l_ph_f6a_h;
logic                                        txfma0_rnd_r_ph_f6a_h;
logic                                        txfma0_rnd_l_ps_f6a_h;
logic                                        txfma0_rnd_r_ps_f6a_h;
logic                                        txfma0_ph_allone2r_f5a_h;
logic                                        txfma0_ph_allone2l_f5a_h;
logic                                        txfma0_ps_allone2r_f5a_h;
logic                                        txfma0_ps_allone2l_f5a_h;
logic                                        txfma0_ph_allone2r_f6a_h;
logic                                        txfma0_ph_allone2l_f6a_h;
logic                                        txfma0_ps_allone2r_f6a_h;
logic                                        txfma0_ps_allone2l_f6a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_eb_hi_f0a_h;
logic [`TXFMA_EXP_PH_SZ-1:0]                 txfma0_ea_lo_f0a_h;
logic [`TXFMA_EXP_PH_SZ-1:0]                 txfma0_eb_lo_f0a_h;
logic                                        txfma0_ps_rndovf_f6a_h;
logic                                        txfma0_ph_rndovf_f6a_h;
logic                                        txfma0_sel_set_bit0_one_f6a_h;
logic                                        txfma0_effsub1_set_sign1_f6a_h;
logic [`FP_CLASS_BITS-1:0]                   txfma0_classify_f6a_h;
logic                                        txfma0_sel_classify_out_f6a_h;
logic [`VPU_FLAGS_SZ-1:0]                    txfma0_fma_flags_f6a_h;
logic                                        txfma0_pot_ovf_ps_f6a_h;
logic                                        txfma0_pot_ovf_ph_f6a_h;
logic                                        txfma0_def_ovf_ps_f6a_h;
logic                                        txfma0_def_ovf_ph_f6a_h;
logic                                        txfma0_pot_unf_ps_f6a_h;
logic                                        txfma0_pot_unf_ph_f6a_h;
logic                                        txfma0_def_unf_ps_f6a_h;
logic                                        txfma0_def_unf_ph_f6a_h;
logic                                        txfma0_comp_op_f5a_h;
logic                                        txfma0_rmode_f5a_h;
logic                                        txfma0_rmode_f6a_h;
logic                                        txfma0_fc_zero30_8_f1a_h;
logic                                        txfma0_prodsum_a2_f3a_h;
logic                                        txfma0_fround_rbit_f3a_h;
logic [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]          txfma0_align1_mux_sel_f2a_h;
logic [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]          txfma0_align2_mux_sel_f2a_h;
logic [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]          txfma0_opd3_mux_sel_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_ediff1_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_ediff1_f3a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_ediff2_f3a_h;
logic                                        txfma0_ediff1_zap_f5a_h;
logic                                        txfma0_neg_norm_a3_f4a_h;
logic                                        txfma0_neg_norm_na3_f4a_h;
logic                                        txfma0_clearphps_en_f4a_h;
logic                                        txfma0_ph_ps_clear_f4a_l;
logic                                        txfma0_comp_op_true_f5a_h;
logic                                        txfma0_comp_op_true_f6a_h;
logic                                        txfma0_fa_en_f1a_h;
logic                                        txfma0_fa_en_f2a_h;
logic  [`VPU_DATA_S_SZ-1:0]                  txfma0_fma_res_f6a_h;
logic                                        txfma0_is_trans_rfa_h;
logic                                        txfma0_opa_valid_rfa_h;
logic                                        txfma0_opb_valid_rfa_h;
logic                                        txfma0_opc_valid_rfa_h;
logic                                        txfma0_dtps_f0a_h;
logic                                        txfma0_normmask2_f6a_h;
logic                                        txfma0_normmask1_f6a_h;
logic                                        txfma0_sel_nr1_frcpres_f6a_h;
logic                                        txfma0_trans_sinp2m_f6a_h;
logic                                        txfma0_trans2_ps_roundup_r_f5a_h;
logic [1:0]                                  txfma0_rrmux32_sel_f6a_h;
logic                                        txfma0_rrmux16l_sel_f6a_l;
logic                                        txfma0_f10f11mux_sel_f6a_h;
logic                                        txfma0_sel_psexc_sr_f6a_h;
logic                                        txfma0_sel_bit31_one_f6a_h;
logic                                        txfma0_sel_b30to0_ones_f6a_h;
logic                                        txfma0_sel_ps_unf_f6a_h;
logic                                        txfma0_sel_zerores_f6a_h;
logic                                        txfma0_sel_ps_normone_f6a_h;
logic                                        txfma0_sel_ps_inf_f6a_h;
logic                                        txfma0_sel_ps_cqn_f6a_h;
logic                                        txfma0_sel_ps_large_f6a_h;
logic                                        txfma0_sel_phsp_f6a_h;
logic                                        txfma0_sel_phhi_sr_f6a_h;
logic                                        txfma0_sel_phhi_large_f6a_h;
logic                                        txfma0_sel_phhi_unf_f6a_h;
logic                                        txfma0_sel_phhi_infres_f6a_h;
logic                                        txfma0_sel_f11inf_f6a_h;
logic                                        txfma0_sel_f10cqnan_f6a_h;
logic                                        txfma0_sel_f11sp_f6a_h;
logic                                        txfma0_sel_f10sp_f6a_h;
logic                                        txfma0_sel_f11large_f6a_h;
logic                                        txfma0_sel_f10large_f6a_h;
logic                                        txfma0_rc_ph_f5a_h;
logic                                        txfma0_rc_ps_f5a_h;
logic                                        txfma0_rc6_ph_f5a_h;
logic                                        txfma0_rc6_ps_f5a_h;
logic                                        txfma0_rc_int32_f5a_h;
logic                                        txfma0_sel_gmaxu24_f6a_h;
logic                                        txfma0_sel_gmaxu16_f6a_h;
logic                                        txfma0_sel_gmaxs16_f6a_h;
logic                                        txfma0_sel_gmins16_f6a_h;
logic                                        txfma0_sel_gmaxs8_f6a_h;
logic                                        txfma0_sel_gmaxu2_f6a_h;
logic                                        txfma0_sel_gmaxu8_f6a_h;
logic                                        txfma0_sel_gmins8_f6a_h;
logic                                        txfma0_sel_gfxmax_f6a_h;
logic [1:0]                                  txfma0_sel_excreshi_f6a_h;
logic [1:0]                                  txfma0_sel_excreslo_f6a_h;
logic [1:0]                                  txfma0_sel_fmares_hi_f6a_h;
logic [1:0]                                  txfma0_sel_fmares_lo_f6a_h;
logic                                        txfma0_frac_zero_f6a_h;
logic                                        txfma0_al2c3_neg_cin_ps_f3a_h;
logic                                        txfma0_al1c3_neg_cin_f3a_h;
logic                                        txfma0_fma_subopc3_f3a_h;
logic [`TXFMA_WSPS_ETRZ_SZ-1:0]              txfma0_etrz50_f4a_h;
logic                                        txfma0_paddin1_sels_f1a_h;
logic                                        txfma0_paddin2_selc_f1a_h;
logic                                        txfma0_paddin2_selfcneg_f1a_h;
logic                                        txfma0_padd_cin_f1a_h;
logic                                        txfma0_sel_famask_f1a_h;
logic                                        txfma0_clear16frnd_nshin_f4a_l;
logic [5:0]                                  txfma0_eb_hi_k_exa_h;
logic                                        txfma0_multype_clken_f0a_h;
logic                                        txfma0_multype_clken_f1a_h;
logic                                        txfma0_sel_eblo_ph_exa_h;
logic                                        txfma0_sel_ec5bdet_exa_h;
logic [1:0]                                  txfma0_sel_ebin_hi_exa_h;
logic                                        txfma0_elo_eb_en_exa_h;
logic                                        txfma0_ehi_eb_en_exa_h;
logic                                        txfma0_elo_ea_en_exa_h;
logic                                        txfma0_fa_en_exa_h;
logic                                        txfma0_fb_en_exa_h;
logic                                        txfma0_fc_en_exa_h;
logic                                        txfma0_sel_ealo_ph_exa_h;
logic [1:0]                                  txfma0_sel_eainmux_exa_h;
logic [1:0]                                  txfma0_sel_ec5b_exa_h;
logic [1:0]                                  txfma0_sel_ecin_exa_h;
logic                                        txfma0_sa_hi_en_exa_h;
logic                                        txfma0_sa_lo_en_exa_h;
logic                                        txfma0_sb_hi_en_exa_h;
logic                                        txfma0_sb_lo_en_exa_h;
logic                                        txfma0_sc_en_exa_h;
logic                                        txfma0_ehi_ea_en_exa_h;
logic                                        txfma0_ec_en_exa_h;
logic                                        txfma0_neg_fmaop_exa_h;
logic                                        txfma0_trans_op2_exa_l;
logic                                        txfma0_sa1_nr2_frcpfxp_exa_h;
logic [1:0]                                  txfma0_sel_sc_exa_h;
logic                                        txfma0_sel_sign_sinp2_exa_h;
logic                                        txfma0_sa_zap_exa_l;
logic                                        txfma0_sb_zap_exa_l;
logic                                        txfma0_sel_f10_exa_h;
logic                                        txfma0_sel_f11_exa_h;
logic [1:0]                                  txfma0_sel_opcgfx_exa_h;
logic                                        txfma0_fc_dtps_exa_h;
logic [1:0]                                  txfma0_sel_xopc_exa_h;
logic [1:0]                                  txfma0_sel_opcfmt_exa_h;
logic                                        txfma0_sel_snorm_exa_h;
logic                                        txfma0_sel_g1sk2_exa_h;
logic                                        txfma0_sel_g1sk8_exa_h;
logic                                        txfma0_sel_g1sk16_exa_h;
logic                                        txfma0_sel_g1sk24_exa_h;
logic                                        txfma0_sel_xexpfrac_exa_h;
logic [1:0]                                  txfma0_sel_transfa_exa_h;
logic [1:0]                                  txfma0_sel_transfb_exa_h;
logic [1:0]                                  txfma0_sel_opafmt_exa_h;
logic [1:0]                                  txfma0_sel_opbfmt_exa_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_ediff1_abs_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_ediff2_abs_f2a_h;
logic [5:0]                                  txfma0_ediff_abs_diff_hi_f2a_h;
logic [4:0]                                  txfma0_align2_rshamt_c3_f2a_h;
logic [1:0]                                  txfma0_op3p3spc_sel_f2a_h;
logic [2:0]                                  txfma0_op3p2c_sel_f2a_h;
logic                                        txfma0_op3g_sel_f2a_h;
logic [1:0]                                  txfma0_imxc_sel_f3a_h;
logic [1:0]                                  txfma0_ugfxrplc_sel_f2a_h;
logic [1:0]                                  txfma0_sgfxrplc_sel_f2a_h;
logic [1:0]                                  txfma0_icgfxc_sel_f2a_h;
logic [1:0]                                  txfma0_al1znp2c_sel_f2a_h;
logic [1:0]                                  txfma0_al2c_sel_f2a_h;
logic [1:0]                                  txfma0_al1p2c_sel_f2a_h;
logic                                        txfma0_al1p1c_sel_f2a_h;
logic                                        txfma0_op3p1c_sel_p_f2a_h;
logic                                        txfma0_op3p1c_sel_r_f2a_h;
logic                                        txfma0_rm_rne_f2a_h;
logic                                        txfma0_rm_rmm_f2a_h;
logic                                        txfma0_frnd_rupnsc_f2a_h;
logic                                        txfma0_frnd_rdnsc_f2a_h;
logic                                        txfma0_frnd_t1_f2a_h;
logic                                        txfma0_frnd_t2_f2a_h;
logic                                        txfma0_al1c_zap_f2a_h;
logic                                        txfma0_op1c_log_mul_clr_f2a_h;
logic                                        txfma0_negop1c_int_logmul_clr_f2a_h;
logic                                        txfma0_psc_op1_clr_f2a_h;
logic                                        txfma0_align2_zap_f2a_h;
logic                                        txfma0_align2c_neg_f2a_h;
logic                                        txfma0_sin_ediff2_f2a_h;
logic                                        txfma0_nshcen_sel1b_f5a_h;
logic                                        txfma0_en_logrrsel1b_f5a_l;
logic                                        txfma0_mmx_sel_immres_f5a_h;
logic                                        txfma0_int32_stky2r_f5a_h;
logic                                        txfma0_stky2g_f5a_h;
logic                                        txfma0_ph_comb_stiky_f4a_h;
logic                                        txfma0_rc_3rmxr_f5a_h;
logic                                        txfma0_rc_rl01_f5a_h;
logic                                        txfma0_rc_rmm_f5a_h;
logic                                        txfma0_rc_rup_f5a_h;
logic                                        txfma0_rc_rne_f5a_h;
logic                                        txfma0_rc_rdn_f5a_h;
logic                                        txfma0_rc_res_sign_f5a_l;
logic                                        txfma0_rc_transrnd_f5a_h;
logic                                        txfma0_rc_transrnd_log_fma2_f5a_h;
logic                                        txfma0_nshc_f5a_h;
logic                                        txfma0_ph_stky2l_f5a_h;
logic                                        txfma0_ps_stky2l_f5a_h;
logic                                        txfma0_int32_stky2l_f5a_h;
logic                                        txfma0_ph_stky2l_f6a_h;
logic                                        txfma0_ps_stky2l_f6a_h;
logic                                        txfma0_int32_stky2l_f6a_h;
logic                                        txfma0_fcf10_zero_f0a_l;
logic                                        txfma0_fcf11_zero_f0a_l;
logic                                        txfma0_fcps_zero_f0a_h;
logic                                        txfma0_fcph_zero_f0a_l;
logic                                        txfma0_fcnorm_zero_29_8_f0a_h;
logic                                        txfma0_fcnorm16_eq_1_f0a_h;
logic                                        txfma0_fcnorm8_eq_1_f0a_h;
logic                                        txfma0_fcnorm_30_f0a_h;
logic                                        txfma0_faph_hi_b0_f0a_h;
logic                                        txfma0_fbph_hi_b0_f0a_h;
logic                                        txfma0_fcph_b0_f0a_h;
logic                                        txfma0_fcf11_b0_f0a_h;
logic                                        txfma0_fcf10_b0_f0a_h;
logic                                        txfma0_fcn8_one_30_24_f0a_h;
logic                                        txfma0_fcn10_one_30_22_f0a_h;
logic                                        txfma0_fcn16_one_30_16_f0a_h;
logic                                        txfma0_fcn24_one_30_8_f0a_h;
logic                                        txfma0_faps_zero_f0a_l;
logic                                        txfma0_fbps_zero_f0a_l;
logic                                        txfma0_faph_hi_zero_f0a_l;
logic                                        txfma0_fbph_hi_zero_f0a_l;
logic                                        txfma0_faph_lo_zero_f0a_l;
logic                                        txfma0_fbph_lo_zero_f0a_l;
logic                                        txfma0_faph_lo_b0_f0a_h;
logic                                        txfma0_fbph_lo_b0_f0a_h;
logic                                        txfma0_faps_b0_f0a_h;
logic                                        txfma0_fbps_b0_f0a_h;
logic                                        txfma0_fc_31_f0a_h;
logic                                        txfma0_fcps_b0_f0a_h;
logic                                        txfma0_int_res_l_bit_f6a_h;
logic                                        txfma0_fc_b0_f2a_h;
logic                                        txfma0_fb_hi_zero_f4a_h;
logic                                        txfma0_comb_add_a3_f5a_h;
logic                                        txfma0_op2_sxtto_g_f2a_h;
vpu_output_t                                 txfma0_f6_out_data;


///////////////////////////////////////////////////////////////////////////////
// F0FFs
///////////////////////////////////////////////////////////////////////////////

// NOTE: System Verilog does not allow "." in instance names
// Asssignment of sub-fields is done here so as not to drive unnecessary
// signals down to txfmactl/txfmafrac 


en_ff 
#(
  .width($bits( {txfma0_ea_hi_f0a_h,
                 txfma0_eb_hi_f0a_h,
                 txfma0_ea_lo_f0a_h,
                 txfma0_eb_lo_f0a_h,
                 txfma0_ec_f0a_h} )       )
)                        
E1A_FF
(
  .clock      ( clock                     ), 
  .en         ( txfma0_ctrl_valid_f0a_h   ),
  .D          ( {txfma0_ea_hi_f0a_h,
                 txfma0_eb_hi_f0a_h,
                 txfma0_ea_lo_f0a_h,
                 txfma0_eb_lo_f0a_h,
                 txfma0_ec_f0a_h}         ),
  .Q          ( {txfma0_ea_hi_f1a_h,
                 txfma0_eb_hi_f1a_h,
                 txfma0_ea_lo_f1a_h,
                 txfma0_eb_lo_f1a_h,
                 txfma0_ec_f1a_h}         )
);

en_ff 
#(
  .width( $bits(txfma0_ec_all1_f0a_h )     )
)                        
F2_EC_ALL1_EN_FF
(
  .clock      ( clock                      ), 
  .en         ( txfma0_ctrl_valid_f0a_h    ),
  .D          ( txfma0_ec_all1_f0a_h       ),
  .Q          ( txfma0_ec_all1_f1a_h       )
);


///////////////////////////////////////////////////////////////////////////////
// F2FFs
///////////////////////////////////////////////////////////////////////////////

en_ff 
#(
  .width( $bits( txfma0_eb_hi_f1a_h[0] ) )
)                        
F2_FB_FF
(
  .clock      ( clock                       ), 
  .en         ( txfma0_ctrl_valid_f1a_h &
                txfma0_fa_en_f1a_h          ),
  .D          ( txfma0_eb_hi_f1a_h[0]       ),
  .Q          ( txfma0_eb_hi_lsb_f2a_h      )
);

en_ff 
#(
  .width( $bits(txfma0_fa_en_f1a_h )       )
)                        
F2_FRAC_EN_FF
(
  .clock      ( clock                      ), 
  .en         ( txfma0_ctrl_valid_f1a_h    ),
  .D          ( txfma0_fa_en_f1a_h         ),
  .Q          ( txfma0_fa_en_f2a_h         )
);

en_ff 
#(
  .width($bits( txfma0_ea_hi_f1a_h)         )
)                        
E2_FF
(
  .clock      ( clock                       ), 
  .en         ( txfma0_ctrl_valid_f1a_h     ),
  .D          ( txfma0_ea_hi_f1a_h          ),
  .Q          ( txfma0_ea_hi_f2a_h          )
);
  
///////////////////////////////////////////////////////////////////////////////
// F3FFs
///////////////////////////////////////////////////////////////////////////////
en_ff 
#(
  .width( $bits( txfma0_eb_hi_lsb_f2a_h ) )
)                        
F3_FB_FF
(
  .clock      ( clock                      ), 
  .en         ( txfma0_ctrl_valid_f2a_h &
                txfma0_fa_en_f2a_h         ),
  .D          ( txfma0_eb_hi_lsb_f2a_h      ),
  .Q          ( txfma0_eb_hi_lsb_f3a_h      )
);

en_ff 
#(
  .width($bits({txfma0_ediff1_f2a_h,
                txfma0_ea_hi_f2a_h}))
)                        
E3_FF
(
  .clock      ( clock                        ), 
  .en         ( txfma0_ctrl_valid_f2a_h      ),
  .D          ( {txfma0_ediff1_f2a_h,
                 txfma0_ea_hi_f2a_h}    ),
  .Q          ( {txfma0_ediff1_f3a_h,
                 txfma0_ea_hi_f3a_h}         )
);


///////////////////////////////////////////////////////////////////////////////
// F4FFs
///////////////////////////////////////////////////////////////////////////////

en_ff 
#(
  .width($bits( txfma0_eb_hi_lsb_f3a_h ) )
)                        
F4_FF
(
  .clock      ( clock                          ), 
  .en         ( txfma0_ctrl_valid_f3a_h        ),
  .D          ( txfma0_eb_hi_lsb_f3a_h         ),                 
  .Q          ( txfma0_eb_hi_lsb_f4a_h         ) 
);

en_ff 
#(
  .width($bits(txfma0_ea_hi_f3a_h))
)
E4_FF
(
  .clock      ( clock                    ), 
  .en         ( txfma0_ctrl_valid_f3a_h  ),
  .D          ( txfma0_ea_hi_f3a_h       ),
  .Q          ( txfma0_ea_hi_f4a_h       )
);

en_ff 
#(
  .width($bits( txfma0_s_res_f3a_h)       )
)                        
S4_FF
(
  .clock      ( clock                     ), 
  .en         ( txfma0_ctrl_valid_f3a_h   ),
  .D          ( txfma0_s_res_f3a_h        ),
  .Q          ( txfma0_s_res_f4a_h        )
);



///////////////////////////////////////////////////////////////////////////////
// F5FFs
///////////////////////////////////////////////////////////////////////////////

en_ff 
#(
  .width($bits({txfma0_ea_hi_f4a_h,
                txfma0_s_res_f4a_h})             )
)                        
F5_RES_FF
(
  .clock      ( clock                            ), 
  .en         ( txfma0_ctrl_valid_f4a_h          ),
  .D          ( {txfma0_ea_hi_f4a_h,
                 txfma0_s_res_f4a_h }            ),
  .Q          ( {txfma0_ea_hi_f5a_h,
                 txfma0_s_res_f5a_h }            )
);



///////////////////////////////////////////////////////////////////////////////
// F6 FFs
///////////////////////////////////////////////////////////////////////////////


rst_en_ff 
#(
  .width( $bits({ txfma0_rmode_f5a_h,
                  txfma0_ph_stky2l_f5a_h,
                  txfma0_ps_stky2l_f5a_h,
                  txfma0_int32_stky2l_f5a_h }) )
)                        
F6_CTL_FF
(
  .clock      ( clock                    ), 
  .reset      ( reset                    ), 
  .en         ( txfma0_ctrl_valid_f5a_h  ), 
  .D          ( {txfma0_rmode_f5a_h,
                 txfma0_ph_stky2l_f5a_h,
                 txfma0_ps_stky2l_f5a_h,
                 txfma0_int32_stky2l_f5a_h }),
  .Q          ( {txfma0_rmode_f6a_h,
                 txfma0_ph_stky2l_f6a_h,
                 txfma0_ps_stky2l_f6a_h,
                 txfma0_int32_stky2l_f6a_h }) 
);


rst_en_ff
#(
  .width( $bits( txfma0_s_res_out_f5a_h ))
)
F6_SIGN_FF
(
  .clock      ( clock                     ), 
  .reset      ( reset                     ), 
  .en         ( txfma0_ctrl_valid_f5a_h   ), 
  .D          ( txfma0_s_res_out_f5a_h    ),
  .Q          ( txfma0_s_res_f6a_h        )
);

rst_en_ff 
#(
  .width( $bits({txfma0_ph_allone2r_f5a_h,
                  txfma0_ph_allone2l_f5a_h,
                  txfma0_ps_allone2r_f5a_h,
                  txfma0_ps_allone2l_f5a_h}))
)                        
F6_MANT_FF
(
  .clock      ( clock                    ), 
  .reset      ( reset                    ), 
  .en         ( txfma0_ctrl_valid_f5a_h  ), 
  .D          ( {txfma0_ph_allone2r_f5a_h,
                 txfma0_ph_allone2l_f5a_h,
                 txfma0_ps_allone2r_f5a_h,
                 txfma0_ps_allone2l_f5a_h}),
  .Q          ( {txfma0_ph_allone2r_f6a_h,
                 txfma0_ph_allone2l_f6a_h,
                 txfma0_ps_allone2r_f6a_h,
                 txfma0_ps_allone2l_f6a_h})
);

///////////////////////////////////////////////////////////////////////////////
// TXFMA Control Blocks 
///////////////////////////////////////////////////////////////////////////////
txfmactl_top txfmactl0_top 
(
  //TXFMA_C0
  .clock                        ( clock                               ),
  .reset                        ( reset                               ),
  .in_valid_exa_h               ( in_valid                            ),
  .use_prev_sigs_exa_h          ( in_data.use_prev_sigs               ),
  .sigs_in_exa_h                ( in_data.sigs                        ), 
  .rm_in_exa_h                  ( in_data.rm                          ),
  .ctrl_valid_f0a_h             ( txfma0_ctrl_valid_f0a_h             ),
  .ctrl_valid_f1a_h             ( txfma0_ctrl_valid_f1a_h             ),
  .ctrl_valid_f2a_h             ( txfma0_ctrl_valid_f2a_h             ),
  .ctrl_valid_f3a_h             ( txfma0_ctrl_valid_f3a_h             ),
  .ctrl_valid_f4a_h             ( txfma0_ctrl_valid_f4a_h             ),
  .ctrl_valid_f5a_h             ( txfma0_ctrl_valid_f5a_h             ),
  .ctrl_valid_f6a_h             ( txfma0_ctrl_valid_f6a_h             ),
  .ea_hi_zero_f0a_l             ( txfma0_ea_hi_zero_f0a_l             ),
  .ea_hi_all1_f0a_h             ( txfma0_ea_hi_all1_f0a_h             ),
  .ea_lo_zero_f0a_l             ( txfma0_ea_lo_zero_f0a_l             ),
  .ea_lo_all1_f0a_h             ( txfma0_ea_lo_all1_f0a_h             ),
  .eb_hi_zero_f0a_l             ( txfma0_eb_hi_zero_f0a_l             ),
  .eb_hi_all1_f0a_h             ( txfma0_eb_hi_all1_f0a_h             ),
  .eb_lo_zero_f0a_l             ( txfma0_eb_lo_zero_f0a_l             ),
  .eb_lo_all1_f0a_h             ( txfma0_eb_lo_all1_f0a_h             ),
  .ec_zero_f0a_l                ( txfma0_ec_zero_f0a_l                ),
  .ec_zero_f1a_h                ( txfma0_ec_zero_f1a_h                ),
  .ec_all1_f0a_h                ( txfma0_ec_all1_f0a_h                ),
  .eb_eql_127_f0a_h             ( txfma0_eb_eql_127_f0a_h             ),
  .eb_eql_126_f0a_h             ( txfma0_eb_eql_126_f0a_h             ),
  .eb_eql_125_f0a_h             ( txfma0_eb_eql_125_f0a_h             ),
  .fcf10_zero_f0a_l             ( txfma0_fcf10_zero_f0a_l             ),
  .fcf11_zero_f0a_l             ( txfma0_fcf11_zero_f0a_l             ),
  .fcps_zero_f0a_h              ( txfma0_fcps_zero_f0a_h              ),
  .fcph_zero_f0a_l              ( txfma0_fcph_zero_f0a_l              ),
  .fcnorm_zero_29_8_f0a_h       ( txfma0_fcnorm_zero_29_8_f0a_h       ),
  .fcnorm16_eq_1_f0a_h          ( txfma0_fcnorm16_eq_1_f0a_h          ),
  .fcnorm8_eq_1_f0a_h           ( txfma0_fcnorm8_eq_1_f0a_h           ),
  .fcnorm_30_f0a_h              ( txfma0_fcnorm_30_f0a_h              ),
  .faph_hi_b0_f0a_h             ( txfma0_faph_hi_b0_f0a_h             ),
  .fbph_hi_b0_f0a_h             ( txfma0_fbph_hi_b0_f0a_h             ),
  .fcph_b0_f0a_h                ( txfma0_fcph_b0_f0a_h                ),
  .fcf11_b0_f0a_h               ( txfma0_fcf11_b0_f0a_h               ),
  .fcf10_b0_f0a_h               ( txfma0_fcf10_b0_f0a_h               ),
  .fcn8_one_30_24_f0a_h         ( txfma0_fcn8_one_30_24_f0a_h         ),
  .fcn10_one_30_22_f0a_h        ( txfma0_fcn10_one_30_22_f0a_h        ),
  .fcn16_one_30_16_f0a_h        ( txfma0_fcn16_one_30_16_f0a_h        ),
  .fcn24_one_30_8_f0a_h         ( txfma0_fcn24_one_30_8_f0a_h         ),
  .faps_zero_f0a_l              ( txfma0_faps_zero_f0a_l              ),
  .fbps_zero_f0a_l              ( txfma0_fbps_zero_f0a_l              ),
  .faph_hi_zero_f0a_l           ( txfma0_faph_hi_zero_f0a_l           ),
  .fbph_hi_zero_f0a_l           ( txfma0_fbph_hi_zero_f0a_l           ),
  .faph_lo_zero_f0a_l		( txfma0_faph_lo_zero_f0a_l           ),
  .fbph_lo_zero_f0a_l		( txfma0_fbph_lo_zero_f0a_l           ),
  .faph_lo_b0_f0a_h             ( txfma0_faph_lo_b0_f0a_h             ),
  .fbph_lo_b0_f0a_h             ( txfma0_fbph_lo_b0_f0a_h             ),
  .faps_b0_f0a_h                ( txfma0_faps_b0_f0a_h                ),
  .fbps_b0_f0a_h                ( txfma0_fbps_b0_f0a_h                ),
  .fc_31_f0a_h                  ( txfma0_fc_31_f0a_h                  ),
  .fcps_b0_f0a_h                ( txfma0_fcps_b0_f0a_h                ),
  .trans_taylor_exa_h           ( trans_coefficients.taylor           ),
  .trans_exp_fma2_exc_exa_h     ( trans_coefficients.exc              ),
  .trans_mask_exa_h             ( trans_coefficients.mask             ),
  .sa_hi_f0a_h                  ( txfma0_sa_hi_f0a_h                  ),
  .sa_lo_f0a_h                  ( txfma0_sa_lo_f0a_h                  ),
  .sb_hi_f0a_h                  ( txfma0_sb_hi_f0a_h                  ),
  .sb_lo_f0a_h                  ( txfma0_sb_lo_f0a_h                  ),
  .sc_f0a_h                     ( txfma0_sc_f0a_h              	      ),
  .opa_valid_rfa_h              ( txfma0_opa_valid_rfa_h              ),
  .opb_valid_rfa_h              ( txfma0_opb_valid_rfa_h              ),
  .opc_valid_rfa_h              ( txfma0_opc_valid_rfa_h              ),
  .dtps_f0a_h                   ( txfma0_dtps_f0a_h                   ),
  .is_trans_rfa_h               ( txfma0_is_trans_rfa_h               ),
  .eb_hi_k_exa_h                ( txfma0_eb_hi_k_exa_h                ),
  .sel_eblo_ph_exa_h            ( txfma0_sel_eblo_ph_exa_h            ),
  .sel_ec5bdet_exa_h            ( txfma0_sel_ec5bdet_exa_h            ),
  .sel_ebin_hi_exa_h            ( txfma0_sel_ebin_hi_exa_h            ),
  .elo_eb_en_exa_h              ( txfma0_elo_eb_en_exa_h              ),
  .ehi_eb_en_exa_h              ( txfma0_ehi_eb_en_exa_h              ),
  .elo_ea_en_exa_h              ( txfma0_elo_ea_en_exa_h              ),
  .fa_en_exa_h                  ( txfma0_fa_en_exa_h                  ),
  .fb_en_exa_h                  ( txfma0_fb_en_exa_h                  ),
  .fc_en_exa_h                  ( txfma0_fc_en_exa_h                  ),
  .sel_ealo_ph_exa_h            ( txfma0_sel_ealo_ph_exa_h            ),
  .sel_eainmux_exa_h            ( txfma0_sel_eainmux_exa_h            ),
  .sel_ec5b_exa_h               ( txfma0_sel_ec5b_exa_h               ),
  .sel_ecin_exa_h               ( txfma0_sel_ecin_exa_h               ),
  .sa_hi_en_exa_h               ( txfma0_sa_hi_en_exa_h               ),
  .sa_lo_en_exa_h               ( txfma0_sa_lo_en_exa_h               ),
  .sb_hi_en_exa_h               ( txfma0_sb_hi_en_exa_h               ),
  .sb_lo_en_exa_h               ( txfma0_sb_lo_en_exa_h               ),
  .sc_en_exa_h                  ( txfma0_sc_en_exa_h                  ),
  .ehi_ea_en_exa_h              ( txfma0_ehi_ea_en_exa_h              ),
  .ec_en_exa_h                  ( txfma0_ec_en_exa_h                  ),
  .neg_fmaop_exa_h              ( txfma0_neg_fmaop_exa_h              ),
  .trans_op2_exa_l              ( txfma0_trans_op2_exa_l              ),
  .sa1_nr2_frcpfxp_exa_h        ( txfma0_sa1_nr2_frcpfxp_exa_h        ),
  .sel_sc_exa_h                 ( txfma0_sel_sc_exa_h                 ),
  .sel_sign_sinp2_exa_h         ( txfma0_sel_sign_sinp2_exa_h         ),
  .sa_zap_exa_l                 ( txfma0_sa_zap_exa_l                 ),
  .sb_zap_exa_l                 ( txfma0_sb_zap_exa_l                 ),
  .sel_f10_exa_h                ( txfma0_sel_f10_exa_h                ),
  .sel_f11_exa_h                ( txfma0_sel_f11_exa_h                ),
  .sel_opcgfx_exa_h             ( txfma0_sel_opcgfx_exa_h             ),
  .fc_dtps_exa_h                ( txfma0_fc_dtps_exa_h                ),
  .sel_xopc_exa_h               ( txfma0_sel_xopc_exa_h               ),
  .sel_opcfmt_exa_h             ( txfma0_sel_opcfmt_exa_h             ),
  .sel_snorm_exa_h              ( txfma0_sel_snorm_exa_h              ),
  .sel_g1sk2_exa_h              ( txfma0_sel_g1sk2_exa_h              ),
  .sel_g1sk8_exa_h              ( txfma0_sel_g1sk8_exa_h              ),
  .sel_g1sk16_exa_h             ( txfma0_sel_g1sk16_exa_h             ),
  .sel_g1sk24_exa_h             ( txfma0_sel_g1sk24_exa_h             ),
  .sel_xexpfrac_exa_h           ( txfma0_sel_xexpfrac_exa_h           ),
  .sel_transfa_exa_h            ( txfma0_sel_transfa_exa_h            ),
  .sel_transfb_exa_h            ( txfma0_sel_transfb_exa_h            ),
  .sel_opafmt_exa_h             ( txfma0_sel_opafmt_exa_h             ),
  .sel_opbfmt_exa_h             ( txfma0_sel_opbfmt_exa_h             ),
  .mul_sign_f0a_h               ( txfma0_mul_sign_f0a_h               ),
  .mul_hp_mode_f0a_h            ( txfma0_mul_hp_mode_f0a_h            ),
  .use_mul_f0a_h                ( txfma0_multype_clken_f0a_h           ),    
  //TXFMA_C1
  .sigs_f1a_h                   ( txfma0_sigs_f1a_h                   ),
  .op_dtype_f1a_h               ( txfma0_op_dtype_f1a_h               ),
  .cmd_f1a_h                    ( txfma0_cmd_f1a_h                    ),
  .ea_hi_f1a_h                  ( txfma0_ea_hi_f1a_h                  ),
  .paddin1_sels_f1a_h           ( txfma0_paddin1_sels_f1a_h           ),
  .paddin2_selc_f1a_h           ( txfma0_paddin2_selc_f1a_h           ),
  .paddin2_selfcneg_f1a_h       ( txfma0_paddin2_selfcneg_f1a_h       ),
  .padd_cin_f1a_h               ( txfma0_padd_cin_f1a_h               ),
  .ea_hi_zero_f1a_h             ( txfma0_ea_hi_zero_f1a_h             ),
  .fc_zero30_8_f1a_h            ( txfma0_fc_zero30_8_f1a_h     	      ),
  .sel_famask_f1a_h             ( txfma0_sel_famask_f1a_h             ),
  .fa_en_f1a_h                  ( txfma0_fa_en_f1a_h                  ),
  .use_mul_f1a_h                ( txfma0_multype_clken_f1a_h           ),  
//TXFMA_C2
  .op_dtype_f2a_h               ( txfma0_op_dtype_f2a_h               ),
  .cmd_f2a_h                    ( txfma0_cmd_f2a_h                    ),
  .sigs_f2a_h                   ( txfma0_sigs_f2a_h                   ), 
  .ediff1_f2a_h                 ( txfma0_ediff1_f2a_h                 ),
  .e_sticky1_f2a_h              ( txfma0_e_sticky1_f2a_h              ),
  .e_sticky2_f2a_h              ( txfma0_e_sticky2_f2a_h              ),
  .ec_f2a_h                     ( txfma0_ec_f2a_h                     ),
  .ec_zero_f2a_h                ( txfma0_ec_zero_f2a_h                ),
  .eprod_hi_zero_f2a_h          ( txfma0_eprod_hi_zero_f2a_h          ),
  .eprod_lo_zero_f2a_h          ( txfma0_eprod_lo_zero_f2a_h          ),
  .ediff1_sel_f2a_h             ( txfma0_ediff1_sel_f2a_h             ),
  .ediff2_sel_f2a_h             ( txfma0_ediff2_sel_f2a_h             ),
  .sin_ediff2_f2a_h             ( txfma0_sin_ediff2_f2a_h             ),
  .fc_b0_f2a_h                  ( txfma0_fc_b0_f2a_h                  ),
  .align1_mux_sel_f2a_h         ( txfma0_align1_mux_sel_f2a_h         ),
  .align2_mux_sel_f2a_h         ( txfma0_align2_mux_sel_f2a_h         ),
  .opd3_mux_sel_f2a_h           ( txfma0_opd3_mux_sel_f2a_h           ),
  .op3p3spc_sel_f2a_h           ( txfma0_op3p3spc_sel_f2a_h           ),
  .op3p2c_sel_f2a_h             ( txfma0_op3p2c_sel_f2a_h             ),
  .op3p1c_sel_p_f2a_h           ( txfma0_op3p1c_sel_p_f2a_h           ),
  .op3p1c_sel_r_f2a_h           ( txfma0_op3p1c_sel_r_f2a_h           ),
  .op3g_sel_f2a_h               ( txfma0_op3g_sel_f2a_h               ),
  .ugfxrplc_sel_f2a_h           ( txfma0_ugfxrplc_sel_f2a_h           ),
  .sgfxrplc_sel_f2a_h           ( txfma0_sgfxrplc_sel_f2a_h           ),
  .icgfxc_sel_f2a_h             ( txfma0_icgfxc_sel_f2a_h             ),
  .al1p1c_sel_f2a_h             ( txfma0_al1p1c_sel_f2a_h             ),
  .al1p2c_sel_f2a_h             ( txfma0_al1p2c_sel_f2a_h             ),
  .rm_rne_f2a_h                 ( txfma0_rm_rne_f2a_h                 ),
  .rm_rmm_f2a_h                 ( txfma0_rm_rmm_f2a_h                 ),
  .frnd_rupnsc_f2a_h            ( txfma0_frnd_rupnsc_f2a_h            ),
  .frnd_rdnsc_f2a_h             ( txfma0_frnd_rdnsc_f2a_h             ),
  .frnd_t1_f2a_h                ( txfma0_frnd_t1_f2a_h                ),
  .frnd_t2_f2a_h                ( txfma0_frnd_t2_f2a_h                ),
  .al1znp2c_sel_f2a_h           ( txfma0_al1znp2c_sel_f2a_h           ),
  .al1c_zap_f2a_h               ( txfma0_al1c_zap_f2a_h               ),
  .op1c_log_mul_clr_f2a_h       ( txfma0_op1c_log_mul_clr_f2a_h       ),
  .op2_sxtto_g_f2a_h            ( txfma0_op2_sxtto_g_f2a_h            ),
  .negop1c_int_logmul_clr_f2a_h ( txfma0_negop1c_int_logmul_clr_f2a_h ),
  .psc_op1_clr_f2a_h            ( txfma0_psc_op1_clr_f2a_h            ),
  .al2c_sel_f2a_h               ( txfma0_al2c_sel_f2a_h               ),
  .align2_zap_f2a_h             ( txfma0_align2_zap_f2a_h             ),
  .align2c_neg_f2a_h            ( txfma0_align2c_neg_f2a_h            ),
  .ediff1_abs_f2a_h             ( txfma0_ediff1_abs_f2a_h             ),
  .ediff2_abs_f2a_h             ( txfma0_ediff2_abs_f2a_h             ),
  .trans_exp_fma2_exc_f2a_h     ( txfma0_trans_exp_fma2_exc_f2a_h     ),
  //TXFMA_C3
  .ediff1_f3a_h                 ( txfma0_ediff1_f3a_h                 ),
  .ediff2_f3a_h                 ( txfma0_ediff2_f3a_h                 ),
  .e_sticky1_f3a_h              ( txfma0_e_sticky1_f3a_h              ),
  .prodsum_a2_f3a_h             ( txfma0_prodsum_a2_f3a_h             ),
  .fround_rbit_f3a_h            ( txfma0_fround_rbit_f3a_h            ),
  .imxc_sel_f3a_h               ( txfma0_imxc_sel_f3a_h               ),
  .ec_f3a_h                     ( txfma0_ec_f3a_h                     ),
  .s_res_f3a_h                  ( txfma0_s_res_f3a_h                  ),
  .fma_subopc3_f3a_h            ( txfma0_fma_subopc3_f3a_h            ),
  .al1c3_neg_cin_f3a_h          ( txfma0_al1c3_neg_cin_f3a_h          ),  
  .al2c3_neg_cin_ps_f3a_h       ( txfma0_al2c3_neg_cin_ps_f3a_h       ),
  //TXFMA_C4
  .elxd_f4a_h                   ( txfma0_elxd_f4a_h                   ), 
  .ph_comb_stiky_f4a_h          ( txfma0_ph_comb_stiky_f4a_h          ),
  .enc_nsamt_f4a_h              ( txfma0_enc_nsamt_f4a_h              ),
  .elxd_add_in2_mux_sel_f4a_h   ( txfma0_elxd_add_in2_mux_sel_f4a_h   ),
  .neg_norm_a3_f4a_h            ( txfma0_neg_norm_a3_f4a_h            ),
  .neg_norm_na3_f4a_h           ( txfma0_neg_norm_na3_f4a_h           ),
  .clearphps_en_f4a_h           ( txfma0_clearphps_en_f4a_h           ),
  .clear16frnd_nshin_f4a_l      ( txfma0_clear16frnd_nshin_f4a_l      ),
  .ph_ps_clear_f4a_l            ( txfma0_ph_ps_clear_f4a_l            ),
  .use_elxd_f4a_h               ( txfma0_use_elxd_f4a_h               ),
  .fb_hi_zero_f4a_h             ( txfma0_fb_hi_zero_f4a_h             ),
  .etrz50_f4a_h                 ( txfma0_etrz50_f4a_h                 ),  
  //TXFMA_C5
  .stky2g_f5a_h                 ( txfma0_stky2g_f5a_h                 ),   
  .int_minmax_mux_sel_a_f5a_h   ( txfma0_int_minmax_mux_sel_a_f5a_h   ),
  .int_minmax_mux_sel_c_f5a_h   ( txfma0_int_minmax_mux_sel_c_f5a_h   ),
  .op_dtype_f5a_h               ( txfma0_op_dtype_f5a_h               ),
  .cmd_f5a_h                    ( txfma0_cmd_f5a_h                    ),
  .sigs_f5a_h                   ( txfma0_sigs_f5a_h                   ),
  .rm_f5a_h                     ( txfma0_rm_f5a_h                     ),
  .s_res_f5a_h                  ( txfma0_s_res_f5a_h                  ),
  .comb_add_a3_f5a_h            ( txfma0_comb_add_a3_f5a_h            ),
  .comp_op_f5a_h                ( txfma0_comp_op_f5a_h                ),
  .ediff1_zap_f5a_h             ( txfma0_ediff1_zap_f5a_h             ),
  .rc_ph_f5a_h                  ( txfma0_rc_ph_f5a_h                  ),
  .rc_ps_f5a_h                  ( txfma0_rc_ps_f5a_h                  ),
  .rc6_ph_f5a_h                 ( txfma0_rc6_ph_f5a_h                 ),
  .rc6_ps_f5a_h                 ( txfma0_rc6_ps_f5a_h                 ),
  .rc_int32_f5a_h               ( txfma0_rc_int32_f5a_h               ),
  .rc_3rmxr_f5a_h               ( txfma0_rc_3rmxr_f5a_h               ),
  .rc_rl01_f5a_h                ( txfma0_rc_rl01_f5a_h                ),
  .rc_rmm_f5a_h                 ( txfma0_rc_rmm_f5a_h                 ),
  .rc_rup_f5a_h                 ( txfma0_rc_rup_f5a_h                 ),
  .rc_rne_f5a_h                 ( txfma0_rc_rne_f5a_h                 ),
  .rc_rdn_f5a_h                 ( txfma0_rc_rdn_f5a_h                 ),
  .rc_res_sign_f5a_l            ( txfma0_rc_res_sign_f5a_l            ),
  .rc_transrnd_f5a_h            ( txfma0_rc_transrnd_f5a_h            ),
  .rc_transrnd_log_fma2_f5a_h   ( txfma0_rc_transrnd_log_fma2_f5a_h   ),
  .s_res_out_f5a_h              ( txfma0_s_res_out_f5a_h              ),
  .nshcen_sel1b_f5a_h           ( txfma0_nshcen_sel1b_f5a_h           ),
  .en_logrrsel1b_f5a_l          ( txfma0_en_logrrsel1b_f5a_l          ),
  .int32_stky2r_f5a_h           ( txfma0_int32_stky2r_f5a_h           ),
  .mmx_sel_immres_f5a_h         ( txfma0_mmx_sel_immres_f5a_h         ),
  .comp_op_true_f5a_h           ( txfma0_comp_op_true_f5a_h           ),
  .e_force_to_zero_f5a_h        ( txfma0_e_force_to_zero_f5a_h        ), 
  .trans2_ps_roundup_r_f5a_h    ( txfma0_trans2_ps_roundup_r_f5a_h    ),
  //TXFMA_C6
  .cmd_f6a_h                    ( txfma0_cmd_f6a_h                    ), 
  .op_dtype_f6a_h               ( txfma0_op_dtype_f6a_h               ),   
  .sigs_f6a_h                   ( txfma0_sigs_f6a_h                   ),     
  .sel_nr1_frcpres_f6a_h        ( txfma0_sel_nr1_frcpres_f6a_h        ),
  .rmode_f6a_h                  ( txfma0_rmode_f6a_h                  ),
  .exp_res_zero_f6a_h           ( txfma0_exp_res_zero_f6a_h           ),
  .s_res_f6a_h                  ( txfma0_s_res_f6a_h                  ),
  .ph_stky2l_f6a_h              ( txfma0_ph_stky2l_f6a_h              ),
  .ps_stky2l_f6a_h              ( txfma0_ps_stky2l_f6a_h              ),
  .int32_stky2l_f6a_h           ( txfma0_int32_stky2l_f6a_h           ),
  .pot_ovf_ps_f6a_h             ( txfma0_pot_ovf_ps_f6a_h             ),
  .def_ovf_ps_f6a_h             ( txfma0_def_ovf_ps_f6a_h             ),  
  .pot_unf_ps_f6a_h             ( txfma0_pot_unf_ps_f6a_h             ),
  .def_unf_ps_f6a_h             ( txfma0_def_unf_ps_f6a_h             ),
  .pot_ovf_ph_f6a_h             ( txfma0_pot_ovf_ph_f6a_h             ),
  .def_ovf_ph_f6a_h             ( txfma0_def_ovf_ph_f6a_h             ),  
  .pot_unf_ph_f6a_h             ( txfma0_pot_unf_ph_f6a_h             ),
  .def_unf_ph_f6a_h             ( txfma0_def_unf_ph_f6a_h             ),
  .frac_zero_f6a_h              ( txfma0_frac_zero_f6a_h              ),
  .rnd_l_ph_f6a_h               ( txfma0_rnd_l_ph_f6a_h               ),
  .rnd_r_ph_f6a_h               ( txfma0_rnd_r_ph_f6a_h               ),
  .rnd_l_ps_f6a_h               ( txfma0_rnd_l_ps_f6a_h               ),
  .rnd_r_ps_f6a_h               ( txfma0_rnd_r_ps_f6a_h               ),
  .int_res_l_bit_f6a_h          ( txfma0_int_res_l_bit_f6a_h          ),
  .ph_allone2r_f6a_h            ( txfma0_ph_allone2r_f6a_h            ),
  .ph_allone2l_f6a_h            ( txfma0_ph_allone2l_f6a_h            ),
  .ps_allone2r_f6a_h            ( txfma0_ps_allone2r_f6a_h            ),
  .ps_allone2l_f6a_h            ( txfma0_ps_allone2l_f6a_h            ),
  .sel_gmaxu24_f6a_h            ( txfma0_sel_gmaxu24_f6a_h            ),
  .sel_gmaxu16_f6a_h            ( txfma0_sel_gmaxu16_f6a_h            ),
  .sel_gmaxs16_f6a_h            ( txfma0_sel_gmaxs16_f6a_h            ),
  .sel_gmins16_f6a_h            ( txfma0_sel_gmins16_f6a_h            ),
  .sel_gmaxs8_f6a_h             ( txfma0_sel_gmaxs8_f6a_h             ),
  .sel_gmaxu2_f6a_h             ( txfma0_sel_gmaxu2_f6a_h             ),
  .sel_gmaxu8_f6a_h             ( txfma0_sel_gmaxu8_f6a_h             ),
  .sel_gmins8_f6a_h             ( txfma0_sel_gmins8_f6a_h             ),
  .sel_gfxmax_f6a_h             ( txfma0_sel_gfxmax_f6a_h             ),
  .sel_psexc_sr_f6a_h           ( txfma0_sel_psexc_sr_f6a_h           ),
  .sel_bit31_one_f6a_h          ( txfma0_sel_bit31_one_f6a_h          ),
  .sel_b30to0_ones_f6a_h        ( txfma0_sel_b30to0_ones_f6a_h        ),
  .sel_zerores_f6a_h            ( txfma0_sel_zerores_f6a_h            ),
  .sel_ps_unf_f6a_h             ( txfma0_sel_ps_unf_f6a_h             ),
  .sel_ps_normone_f6a_h         ( txfma0_sel_ps_normone_f6a_h         ),
  .sel_ps_inf_f6a_h             ( txfma0_sel_ps_inf_f6a_h             ),
  .sel_ps_cqn_f6a_h             ( txfma0_sel_ps_cqn_f6a_h             ),
  .sel_ps_large_f6a_h           ( txfma0_sel_ps_large_f6a_h           ),
  .sel_phsp_f6a_h               ( txfma0_sel_phsp_f6a_h               ),
  .sel_phhi_sr_f6a_h            ( txfma0_sel_phhi_sr_f6a_h            ),
  .sel_phhi_large_f6a_h         ( txfma0_sel_phhi_large_f6a_h         ),
  .sel_phhi_unf_f6a_h           ( txfma0_sel_phhi_unf_f6a_h           ),
  .sel_phhi_infres_f6a_h        ( txfma0_sel_phhi_infres_f6a_h        ),
  .sel_f11inf_f6a_h             ( txfma0_sel_f11inf_f6a_h             ),
  .sel_f10cqnan_f6a_h           ( txfma0_sel_f10cqnan_f6a_h           ),
  .sel_f11sp_f6a_h              ( txfma0_sel_f11sp_f6a_h              ),
  .sel_f10sp_f6a_h              ( txfma0_sel_f10sp_f6a_h              ),
  .sel_f11large_f6a_h           ( txfma0_sel_f11large_f6a_h           ),
  .sel_f10large_f6a_h           ( txfma0_sel_f10large_f6a_h           ),
  .ps_rndovf_f6a_h              ( txfma0_ps_rndovf_f6a_h              ),
  .ph_rndovf_f6a_h              ( txfma0_ph_rndovf_f6a_h              ),
  .sel_set_bit0_one_f6a_h       ( txfma0_sel_set_bit0_one_f6a_h       ),
  .effsub1_set_sign1_f6a_h      ( txfma0_effsub1_set_sign1_f6a_h      ),
  .normmask2_f6a_h              ( txfma0_normmask2_f6a_h              ),
  .normmask1_f6a_h              ( txfma0_normmask1_f6a_h              ),
  .trans_sinp2m_f6a_h           ( txfma0_trans_sinp2m_f6a_h           ),
  .f10f11mux_sel_f6a_h          ( txfma0_f10f11mux_sel_f6a_h          ),
  .rrmux32_sel_f6a_h            ( txfma0_rrmux32_sel_f6a_h            ),
  .rrmux16l_sel_f6a_l           ( txfma0_rrmux16l_sel_f6a_l           ),
  .sel_excreshi_f6a_h           ( txfma0_sel_excreshi_f6a_h           ),
  .sel_excreslo_f6a_h           ( txfma0_sel_excreslo_f6a_h           ),
  .sel_fmares_hi_f6a_h          ( txfma0_sel_fmares_hi_f6a_h          ),
  .sel_fmares_lo_f6a_h          ( txfma0_sel_fmares_lo_f6a_h          ),
  .classify_f6a_h               ( txfma0_classify_f6a_h               ),
  .sel_classify_out_f6a_h       ( txfma0_sel_classify_out_f6a_h       ),
  .fma_flags_f6a_h              ( txfma0_fma_flags_f6a_h              ),
  .comp_op_true_f6a_h           ( txfma0_comp_op_true_f6a_h           )
);


///////////////////////////////////////////////////////////////////////////////
// TXFMA Fraction Blocks 
///////////////////////////////////////////////////////////////////////////////
`ifdef MINION_USE_MMI_TXFMA
generate if (USE_MMI)
begin: TXFMA_MMI
    txfmafrac_top_mmi txfmafrac0_top_mmi
    (
    .txfma_clk                    ( clock                               ), 
    .txfmac0_se                   ( 1'b0                                ),
    .txfma_se                     ( 1'b0                                ),
    .si_txfmac0                   ( 1'b0                                ),
    .si_txfma_byp_abc_opd         ( 1'b0                                ),
    .test_enable                  ( 1'b0                                ),
    // clock en  
    .opa_clken_frfa_h             ( txfma0_opa_valid_rfa_h              ),
    .opb_clken_frfa_h             ( txfma0_opb_valid_rfa_h              ),
    .opc_clken_frfa_h             ( txfma0_opc_valid_rfa_h              ),
    .transop_clken_frfa_h         ( txfma0_is_trans_rfa_h               ),
    .txfma_clken_frfa_h           ( in_valid                            ),  
    .txfma_clken_f0a_h            ( txfma0_ctrl_valid_f0a_h             ),
    .multype_clken_f0a_h          ( txfma0_multype_clken_f0a_h          ), 
    .txfma_clken_f1a_h            ( txfma0_ctrl_valid_f1a_h             ),
    .multype_clken_f1a_h          ( txfma0_multype_clken_f1a_h          ),   
    .txfma_clken_f2a_h            ( txfma0_ctrl_valid_f2a_h             ),
    .txfma_clken_f3a_h            ( txfma0_ctrl_valid_f3a_h             ),
    .txfma_clken_f4a_h            ( txfma0_ctrl_valid_f4a_h             ),
    .txfma_clken_f5a_h            ( txfma0_ctrl_valid_f5a_h             ),
    //TXFMA_F0
    .opa_byp_frfa_h               ( in_data.in1                         ),
    .opb_byp_frfa_h               ( in_data.in2                         ),
    .opc_byp_frfa_h               ( in_data.in3                         ),
    .xrom_c1c0k_ra_h              ( trans_coefficients.c0               ),
    .xrom_c2_ra_h                 ( trans_coefficients.c2               ),
    .trans_mask_ra_h              ( trans_coefficients.mask             ),
    .eb_hi_k_exa_h                ( txfma0_eb_hi_k_exa_h                ),
    .sel_eblo_ph_exa_h            ( txfma0_sel_eblo_ph_exa_h            ),
    .sel_ec5bdet_exa_h            ( txfma0_sel_ec5bdet_exa_h            ),
    .sel_ebin_hi_exa_h            ( txfma0_sel_ebin_hi_exa_h            ),
    .elo_eb_en_exa_h              ( txfma0_elo_eb_en_exa_h              ),
    .ehi_eb_en_exa_h              ( txfma0_ehi_eb_en_exa_h              ),
    .elo_ea_en_exa_h              ( txfma0_elo_ea_en_exa_h              ),
    .fa_en_exa_h                  ( txfma0_fa_en_exa_h                  ),
    .fb_en_exa_h                  ( txfma0_fb_en_exa_h                  ),
    .fc_en_exa_h                  ( txfma0_fc_en_exa_h                  ),
    .sel_ealo_ph_exa_h            ( txfma0_sel_ealo_ph_exa_h            ),
    .sel_eainmux_exa_h            ( txfma0_sel_eainmux_exa_h            ),
    .sel_ec5b_exa_h               ( txfma0_sel_ec5b_exa_h               ),
    .sel_ecin_exa_h               ( txfma0_sel_ecin_exa_h               ),
    .sa_hi_en_exa_h               ( txfma0_sa_hi_en_exa_h               ),
    .sa_lo_en_exa_h               ( txfma0_sa_lo_en_exa_h               ),
    .sb_hi_en_exa_h               ( txfma0_sb_hi_en_exa_h               ),
    .sb_lo_en_exa_h               ( txfma0_sb_lo_en_exa_h               ),
    .sc_en_exa_h                  ( txfma0_sc_en_exa_h                  ),
    .ehi_ea_en_exa_h              ( txfma0_ehi_ea_en_exa_h              ),
    .ec_en_exa_h                  ( txfma0_ec_en_exa_h                  ),
    .neg_fmaop_exa_h              ( txfma0_neg_fmaop_exa_h              ),
    .trans_op2_exa_l              ( txfma0_trans_op2_exa_l              ),
    .sa1_nr2_frcpfxp_exa_h        ( txfma0_sa1_nr2_frcpfxp_exa_h        ),
    .sel_sc_exa_h                 ( txfma0_sel_sc_exa_h                 ),
    .sel_sign_sinp2_exa_h         ( txfma0_sel_sign_sinp2_exa_h         ),
    .sa_zap_exa_l                 ( txfma0_sa_zap_exa_l                 ),
    .sb_zap_exa_l                 ( txfma0_sb_zap_exa_l                 ),
    .sel_f10_exa_h                ( txfma0_sel_f10_exa_h                ),
    .sel_f11_exa_h                ( txfma0_sel_f11_exa_h                ),
    .sel_opcgfx_exa_h             ( txfma0_sel_opcgfx_exa_h             ),
    .fc_dtps_exa_h                ( txfma0_fc_dtps_exa_h                ),
    .sel_xopc_exa_h               ( txfma0_sel_xopc_exa_h               ),
    .sel_opcfmt_exa_h             ( txfma0_sel_opcfmt_exa_h             ),
    .sel_snorm_exa_h              ( txfma0_sel_snorm_exa_h              ),
    .sel_g1sk2_exa_h              ( txfma0_sel_g1sk2_exa_h              ),
    .sel_g1sk8_exa_h              ( txfma0_sel_g1sk8_exa_h              ),
    .sel_g1sk16_exa_h             ( txfma0_sel_g1sk16_exa_h             ),
    .sel_g1sk24_exa_h             ( txfma0_sel_g1sk24_exa_h             ),
    .sel_xexpfrac_exa_h           ( txfma0_sel_xexpfrac_exa_h           ),
    .sel_transfa_exa_h            ( txfma0_sel_transfa_exa_h            ),
    .sel_transfb_exa_h            ( txfma0_sel_transfb_exa_h            ),
    .sel_opafmt_exa_h             ( txfma0_sel_opafmt_exa_h             ),
    .sel_opbfmt_exa_h             ( txfma0_sel_opbfmt_exa_h             ),
    .sa_hi_f0a_h                  ( txfma0_sa_hi_f0a_h                  ),
    .sa_lo_f0a_h                  ( txfma0_sa_lo_f0a_h                  ),
    .sb_hi_f0a_h                  ( txfma0_sb_hi_f0a_h                  ),
    .sb_lo_f0a_h                  ( txfma0_sb_lo_f0a_h                  ),
    .sc_f0a_h                     ( txfma0_sc_f0a_h                     ),
    .ea_hi_f0a_h                  ( txfma0_ea_hi_f0a_h                  ),
    .ea_lo_f0a_h                  ( txfma0_ea_lo_f0a_h                  ),
    .eb_hi_f0a_h                  ( txfma0_eb_hi_f0a_h                  ),
    .eb_lo_f0a_h                  ( txfma0_eb_lo_f0a_h                  ),
    .ec_f0a_h                     ( txfma0_ec_f0a_h                     ),
    .ea_hi_zero_f0a_l             ( txfma0_ea_hi_zero_f0a_l             ),
    .ea_hi_all1_f0a_h             ( txfma0_ea_hi_all1_f0a_h             ),
    .ea_lo_zero_f0a_l             ( txfma0_ea_lo_zero_f0a_l             ),
    .ea_lo_all1_f0a_h             ( txfma0_ea_lo_all1_f0a_h             ),
    .eb_hi_zero_f0a_l             ( txfma0_eb_hi_zero_f0a_l             ),
    .eb_hi_all1_f0a_h             ( txfma0_eb_hi_all1_f0a_h             ),
    .eb_lo_zero_f0a_l             ( txfma0_eb_lo_zero_f0a_l             ),
    .eb_lo_all1_f0a_h             ( txfma0_eb_lo_all1_f0a_h             ),
    .ec_zero_f0a_l                ( txfma0_ec_zero_f0a_l                ),
    .ec_all1_f0a_h                ( txfma0_ec_all1_f0a_h                ),
    .eb_eql_127_f0a_h             ( txfma0_eb_eql_127_f0a_h             ),
    .eb_eql_126_f0a_h             ( txfma0_eb_eql_126_f0a_h             ),
    .eb_eql_125_f0a_h             ( txfma0_eb_eql_125_f0a_h             ),
    .fcf10_zero_f0a_l             ( txfma0_fcf10_zero_f0a_l             ),
    .fcf11_zero_f0a_l             ( txfma0_fcf11_zero_f0a_l             ),
    .fcps_zero_f0a_h              ( txfma0_fcps_zero_f0a_h              ),
    .fcph_zero_f0a_l              ( txfma0_fcph_zero_f0a_l              ),
    .fcnorm_zero_29_8_f0a_h       ( txfma0_fcnorm_zero_29_8_f0a_h       ),
    .fcnorm16_eq_1_f0a_h          ( txfma0_fcnorm16_eq_1_f0a_h          ),
    .fcnorm8_eq_1_f0a_h           ( txfma0_fcnorm8_eq_1_f0a_h           ),
    .fcnorm_30_f0a_h              ( txfma0_fcnorm_30_f0a_h              ),
    .faph_hi_b0_f0a_h             ( txfma0_faph_hi_b0_f0a_h             ),
    .fbph_hi_b0_f0a_h             ( txfma0_fbph_hi_b0_f0a_h             ),
    .fcph_b0_f0a_h                ( txfma0_fcph_b0_f0a_h                ),
    .fcf11_b0_f0a_h               ( txfma0_fcf11_b0_f0a_h               ),
    .fcf10_b0_f0a_h               ( txfma0_fcf10_b0_f0a_h               ),
    .fcn8_one_30_24_f0a_h         ( txfma0_fcn8_one_30_24_f0a_h         ),
    .fcn10_one_30_22_f0a_h        ( txfma0_fcn10_one_30_22_f0a_h        ),
    .fcn16_one_30_16_f0a_h        ( txfma0_fcn16_one_30_16_f0a_h        ),
    .fcn24_one_30_8_f0a_h         ( txfma0_fcn24_one_30_8_f0a_h         ),
    .faps_zero_f0a_l              ( txfma0_faps_zero_f0a_l              ),
    .fbps_zero_f0a_l              ( txfma0_fbps_zero_f0a_l              ),
    .faph_hi_zero_f0a_l           ( txfma0_faph_hi_zero_f0a_l           ),
    .fbph_hi_zero_f0a_l           ( txfma0_fbph_hi_zero_f0a_l           ),
    .faph_lo_zero_f0a_l           ( txfma0_faph_lo_zero_f0a_l           ),
    .fbph_lo_zero_f0a_l           ( txfma0_fbph_lo_zero_f0a_l           ),
    .faph_lo_b0_f0a_h             ( txfma0_faph_lo_b0_f0a_h             ),
    .fbph_lo_b0_f0a_h             ( txfma0_fbph_lo_b0_f0a_h             ),
    .faps_b0_f0a_h                ( txfma0_faps_b0_f0a_h                ),
    .fbps_b0_f0a_h                ( txfma0_fbps_b0_f0a_h                ),
    .fc_31_f0a_h                  ( txfma0_fc_31_f0a_h                  ),
    .fcps_b0_f0a_h                ( txfma0_fcps_b0_f0a_h                ),
  //TXFMA_F1
    .mul_sign_f0a_h               ( txfma0_mul_sign_f0a_h               ),
    .mul_hp_mode_f0a_h            ( txfma0_mul_hp_mode_f0a_h            ),
    .dtps_f0a_h                   ( txfma0_dtps_f0a_h                   ),
    .fc_b0_f2a_h                  ( txfma0_fc_b0_f2a_h                  ),
    .etrz_fa_hi_ph_f1a_h          ( txfma0_etrz_fa_hi_ph_f1a_h          ),
    .etrz_fa_lo_ps_f1a_h          ( txfma0_etrz_fa_lo_ps_f1a_h          ),
    .etrz_fb_hi_ph_f1a_h          ( txfma0_etrz_fb_hi_ph_f1a_h          ),
    .etrz_fb_lo_ps_f1a_h          ( txfma0_etrz_fb_lo_ps_f1a_h          ),      
    .etrz_fc_f1a_h                ( txfma0_etrz_fc_f1a_h                ), 
  //TXFMA_F2 
    .paddin1_sels_f1a_h           ( txfma0_paddin1_sels_f1a_h           ),
    .paddin2_selc_f1a_h           ( txfma0_paddin2_selc_f1a_h           ),
    .paddin2_selfcneg_f1a_h       ( txfma0_paddin2_selfcneg_f1a_h       ),
    .padd_cin_f1a_h               ( txfma0_padd_cin_f1a_h               ),
    .sel_famask_f1a_h             ( txfma0_sel_famask_f1a_h             ),
  //TXFMA_F3
    .exp_res_f2a_h                ( txfma0_exp_res_2f3_f2a_h            ),
    .ediff_abs_diff_hi_f2a_h      ( txfma0_ediff_abs_diff_hi_f2a_h      ),
    .align2_rshamt_c3_f2a_h       ( txfma0_align2_rshamt_c3_f2a_h       ),
    .op3p3spc_sel_f2a_h           ( txfma0_op3p3spc_sel_f2a_h           ),
    .op3p1c_sel_p_f2a_h           ( txfma0_op3p1c_sel_p_f2a_h           ),
    .op3p2c_sel_f2a_h             ( txfma0_op3p2c_sel_f2a_h             ),
    .op3p1c_sel_r_f2a_h           ( txfma0_op3p1c_sel_r_f2a_h           ),
    .op3g_sel_f2a_h               ( txfma0_op3g_sel_f2a_h               ),
    .op2_sxtto_g_f2a_h            ( txfma0_op2_sxtto_g_f2a_h            ),
    .imxc_sel_f3a_h               ( txfma0_imxc_sel_f3a_h               ),
    .fround_rbit_f3a_h            ( txfma0_fround_rbit_f3a_h            ),
    .ugfxrplc_sel_f2a_h           ( txfma0_ugfxrplc_sel_f2a_h           ),
    .sgfxrplc_sel_f2a_h           ( txfma0_sgfxrplc_sel_f2a_h           ),
    .icgfxc_sel_f2a_h             ( txfma0_icgfxc_sel_f2a_h             ),
    .al1p1c_sel_f2a_h             ( txfma0_al1p1c_sel_f2a_h             ),
    .al1p2c_sel_f2a_h             ( txfma0_al1p2c_sel_f2a_h             ),
    .rm_rne_f2a_h                 ( txfma0_rm_rne_f2a_h                 ),
    .rm_rmm_f2a_h                 ( txfma0_rm_rmm_f2a_h                 ),
    .frnd_rupnsc_f2a_h            ( txfma0_frnd_rupnsc_f2a_h            ),
    .frnd_rdnsc_f2a_h             ( txfma0_frnd_rdnsc_f2a_h             ),
    .frnd_t1_f2a_h                ( txfma0_frnd_t1_f2a_h                ),
    .frnd_t2_f2a_h                ( txfma0_frnd_t2_f2a_h                ),
    .al1c_zap_f2a_h               ( txfma0_al1c_zap_f2a_h               ),
    .op1c_log_mul_clr_f2a_h       ( txfma0_op1c_log_mul_clr_f2a_h       ),
    .negop1c_int_logmul_clr_f2a_h ( txfma0_negop1c_int_logmul_clr_f2a_h ),
    .psc_op1_clr_f2a_h            ( txfma0_psc_op1_clr_f2a_h            ),
    .al1znp2c_sel_f2a_h           ( txfma0_al1znp2c_sel_f2a_h           ),
    .al2c_sel_f2a_h               ( txfma0_al2c_sel_f2a_h               ),
    .align2_zap_f2a_h             ( txfma0_align2_zap_f2a_h             ),
    .align2c_neg_f2a_h            ( txfma0_align2c_neg_f2a_h            ),
    .prodsum_a2_f3a_h             ( txfma0_prodsum_a2_f3a_h             ),
    .al2c3_neg_cin_ps_f3a_h       ( txfma0_al2c3_neg_cin_ps_f3a_h       ),  
    .fma_subopc3_f3a_h            ( txfma0_fma_subopc3_f3a_h            ),  
  //TXFMA_F4
    .al1c3_neg_cin_f3a_h          ( txfma0_al1c3_neg_cin_f3a_h          ),
    .neg_norm_a3_f4a_h            ( txfma0_neg_norm_a3_f4a_h            ),
    .neg_norm_na3_f4a_h           ( txfma0_neg_norm_na3_f4a_h           ),
    .clearphps_en_f4a_h           ( txfma0_clearphps_en_f4a_h           ),
    .clear16frnd_nshin_f4a_l      ( txfma0_clear16frnd_nshin_f4a_l      ),
    .ph_ps_clear_f4a_l            ( txfma0_ph_ps_clear_f4a_l            ),
    .elxd_f4a_h                   ( txfma0_elxd_f4a_h                   ),
    .etrz50_f4a_h                 ( txfma0_etrz50_f4a_h                 ),
  //TXFMA_F5
    .enc_nsamt_f4a_h              ( txfma0_enc_nsamt_f4a_h              ),
    .en_logrrsel1b_f5a_l          ( txfma0_en_logrrsel1b_f5a_l          ),
    .mmx_sel_immres_f5a_h         ( txfma0_mmx_sel_immres_f5a_h         ),
    .nshcen_sel1b_f5a_h           ( txfma0_nshcen_sel1b_f5a_h           ),
    .rc_ph_f5a_h                  ( txfma0_rc_ph_f5a_h                  ),
    .rc_ps_f5a_h                  ( txfma0_rc_ps_f5a_h                  ),
    .rc_int32_f5a_h               ( txfma0_rc_int32_f5a_h               ),
    .int32_stky2r_f5a_h           ( txfma0_int32_stky2r_f5a_h           ),
    .stky2g_f5a_h                 ( txfma0_stky2g_f5a_h                 ),
    .rc_3rmxr_f5a_h               ( txfma0_rc_3rmxr_f5a_h               ),
    .rc_rl01_f5a_h                ( txfma0_rc_rl01_f5a_h                ),
    .rc_rmm_f5a_h                 ( txfma0_rc_rmm_f5a_h                 ),
    .rc_rup_f5a_h                 ( txfma0_rc_rup_f5a_h                 ),
    .rc_rne_f5a_h                 ( txfma0_rc_rne_f5a_h                 ),
    .rc_rdn_f5a_h                 ( txfma0_rc_rdn_f5a_h                 ),
    .rc_res_sign_f5a_l            ( txfma0_rc_res_sign_f5a_l            ),
    .rc_transrnd_f5a_h            ( txfma0_rc_transrnd_f5a_h            ),
    .rc_transrnd_log_fma2_f5a_h   ( txfma0_rc_transrnd_log_fma2_f5a_h   ),
    .ph_stky2l_f5a_h              ( txfma0_ph_stky2l_f5a_h              ),
    .ps_stky2l_f5a_h              ( txfma0_ps_stky2l_f5a_h              ),
    .int32_stky2l_f5a_h           ( txfma0_int32_stky2l_f5a_h           ),
    .nshc_f5a_h                   ( txfma0_nshc_f5a_h                   ),
    .frac_zero_f6a_h              ( txfma0_frac_zero_f6a_h              ),
    .ph_allone2r_f5a_h            ( txfma0_ph_allone2r_f5a_h            ),
    .ph_allone2l_f5a_h            ( txfma0_ph_allone2l_f5a_h            ),
    .ps_allone2r_f5a_h            ( txfma0_ps_allone2r_f5a_h            ),
    .ps_allone2l_f5a_h            ( txfma0_ps_allone2l_f5a_h            ),
    .comb_add_a3_f5a_h            ( txfma0_comb_add_a3_f5a_h            ),
  //TXFMA_F6
    .rnd_l_ph_f6a_h               ( txfma0_rnd_l_ph_f6a_h               ),
    .rnd_r_ph_f6a_h               ( txfma0_rnd_r_ph_f6a_h               ),
    .rnd_l_ps_f6a_h               ( txfma0_rnd_l_ps_f6a_h               ),
    .rnd_r_ps_f6a_h               ( txfma0_rnd_r_ps_f6a_h               ),
    .classify_f6a_h               ( txfma0_classify_f6a_h               ),
    .trans2_ps_roundup_r_f5a_h    ( txfma0_trans2_ps_roundup_r_f5a_h    ),
    .normmask2_f6a_h              ( txfma0_normmask2_f6a_h              ),
    .normmask1_f6a_h              ( txfma0_normmask1_f6a_h              ),
    .sel_classify_out_f6a_h       ( txfma0_sel_classify_out_f6a_h       ),
    .sel_nr1_frcpres_f6a_h        ( txfma0_sel_nr1_frcpres_f6a_h        ),
    .trans_sinp2m_f6a_h           ( txfma0_trans_sinp2m_f6a_h           ),
    .s_res_f6a_h                  ( txfma0_s_res_f6a_h                  ),
    .exp_res_f6a_h                ( txfma0_exp_res_f6a_h                ),
    .rrmux32_sel_f6a_h            ( txfma0_rrmux32_sel_f6a_h            ),
    .rrmux16l_sel_f6a_l           ( txfma0_rrmux16l_sel_f6a_l           ),
    .rc6_ph_f5a_h                 ( txfma0_rc6_ph_f5a_h                 ),
    .rc6_ps_f5a_h                 ( txfma0_rc6_ps_f5a_h                 ),
    .ps_rndovf_f6a_h              ( txfma0_ps_rndovf_f6a_h              ),
    .ph_rndovf_f6a_h              ( txfma0_ph_rndovf_f6a_h              ),
    .sel_set_bit0_one_f6a_h       ( txfma0_sel_set_bit0_one_f6a_h       ),
    .effsub1_set_sign1_f6a_h      ( txfma0_effsub1_set_sign1_f6a_h      ),
    .f10f11mux_sel_f6a_h          ( txfma0_f10f11mux_sel_f6a_h          ),
    .sel_psexc_sr_f6a_h           ( txfma0_sel_psexc_sr_f6a_h           ),
    .sel_bit31_one_f6a_h          ( txfma0_sel_bit31_one_f6a_h          ),
    .sel_b30to0_ones_f6a_h        ( txfma0_sel_b30to0_ones_f6a_h        ),
    .sel_ps_unf_f6a_h             ( txfma0_sel_ps_unf_f6a_h             ),
    .sel_zerores_f6a_h            ( txfma0_sel_zerores_f6a_h            ),
    .sel_ps_normone_f6a_h         ( txfma0_sel_ps_normone_f6a_h         ),
    .sel_ps_inf_f6a_h             ( txfma0_sel_ps_inf_f6a_h             ),
    .sel_ps_cqn_f6a_h             ( txfma0_sel_ps_cqn_f6a_h             ),
    .sel_ps_large_f6a_h           ( txfma0_sel_ps_large_f6a_h           ),
    .sel_phsp_f6a_h               ( txfma0_sel_phsp_f6a_h               ),
    .sel_phhi_sr_f6a_h            ( txfma0_sel_phhi_sr_f6a_h            ),
    .sel_phhi_large_f6a_h         ( txfma0_sel_phhi_large_f6a_h         ),
    .sel_phhi_unf_f6a_h           ( txfma0_sel_phhi_unf_f6a_h           ),
    .sel_phhi_infres_f6a_h        ( txfma0_sel_phhi_infres_f6a_h        ),
    .sel_f11inf_f6a_h             ( txfma0_sel_f11inf_f6a_h             ),
    .sel_f10cqnan_f6a_h           ( txfma0_sel_f10cqnan_f6a_h           ),
    .sel_f11sp_f6a_h              ( txfma0_sel_f11sp_f6a_h              ),
    .sel_f10sp_f6a_h              ( txfma0_sel_f10sp_f6a_h              ),
    .sel_f11large_f6a_h           ( txfma0_sel_f11large_f6a_h           ),
    .sel_f10large_f6a_h           ( txfma0_sel_f10large_f6a_h           ),
    .sel_gmaxu24_f6a_h            ( txfma0_sel_gmaxu24_f6a_h            ),
    .sel_gmaxu16_f6a_h            ( txfma0_sel_gmaxu16_f6a_h            ),
    .sel_gmaxs16_f6a_h            ( txfma0_sel_gmaxs16_f6a_h            ),
    .sel_gmins16_f6a_h            ( txfma0_sel_gmins16_f6a_h            ),
    .sel_gmaxs8_f6a_h             ( txfma0_sel_gmaxs8_f6a_h             ),
    .sel_gmaxu2_f6a_h             ( txfma0_sel_gmaxu2_f6a_h             ),
    .sel_gmaxu8_f6a_h             ( txfma0_sel_gmaxu8_f6a_h             ),
    .sel_gmins8_f6a_h             ( txfma0_sel_gmins8_f6a_h             ),
    .sel_gfxmax_f6a_h             ( txfma0_sel_gfxmax_f6a_h             ),
    .sel_excreshi_f6a_h           ( txfma0_sel_excreshi_f6a_h           ),
    .sel_excreslo_f6a_h           ( txfma0_sel_excreslo_f6a_h           ),
    .sel_fmares_hi_f6a_h          ( txfma0_sel_fmares_hi_f6a_h          ),
    .sel_fmares_lo_f6a_h          ( txfma0_sel_fmares_lo_f6a_h          ),
    .int_res_l_bit_f6a_h          ( txfma0_int_res_l_bit_f6a_h          ),
    .fma_res_f6a_h                ( txfma0_fma_res_f6a_h                )
  );
end
else
begin : TXFMA_NO_MMI
`endif
    txfmafrac_top txfmafrac0_top 
    (
    .txfma_clk                    ( clock                               ), 
    .txfmac0_se                   ( 1'b0                                ),
    .txfma_se                     ( 1'b0                                ),
    .si_txfmac0                   ( 1'b0                                ),
    .si_txfma_byp_abc_opd         ( 1'b0                                ),
    .test_enable                  ( 1'b0                                ),
    // clock en  
    .opa_clken_frfa_h             ( txfma0_opa_valid_rfa_h              ),
    .opb_clken_frfa_h             ( txfma0_opb_valid_rfa_h              ),
    .opc_clken_frfa_h             ( txfma0_opc_valid_rfa_h              ),
    .transop_clken_frfa_h         ( txfma0_is_trans_rfa_h               ),
    .txfma_clken_frfa_h           ( in_valid                            ),  
    .txfma_clken_f0a_h            ( txfma0_ctrl_valid_f0a_h             ),
    .multype_clken_f0a_h          ( txfma0_multype_clken_f0a_h          ), 
    .txfma_clken_f1a_h            ( txfma0_ctrl_valid_f1a_h             ),
    .multype_clken_f1a_h          ( txfma0_multype_clken_f1a_h          ),   
    .txfma_clken_f2a_h            ( txfma0_ctrl_valid_f2a_h             ),
    .txfma_clken_f3a_h            ( txfma0_ctrl_valid_f3a_h             ),
    .txfma_clken_f4a_h            ( txfma0_ctrl_valid_f4a_h             ),
    .txfma_clken_f5a_h            ( txfma0_ctrl_valid_f5a_h             ),
    //TXFMA_F0
    .opa_byp_frfa_h               ( in_data.in1                         ),
    .opb_byp_frfa_h               ( in_data.in2                         ),
    .opc_byp_frfa_h               ( in_data.in3                         ),
    .xrom_c1c0k_ra_h              ( trans_coefficients.c0               ),
    .xrom_c2_ra_h                 ( trans_coefficients.c2               ),
    .trans_mask_ra_h              ( trans_coefficients.mask             ),
    .eb_hi_k_exa_h                ( txfma0_eb_hi_k_exa_h                ),
    .sel_eblo_ph_exa_h            ( txfma0_sel_eblo_ph_exa_h            ),
    .sel_ec5bdet_exa_h            ( txfma0_sel_ec5bdet_exa_h            ),
    .sel_ebin_hi_exa_h            ( txfma0_sel_ebin_hi_exa_h            ),
    .elo_eb_en_exa_h              ( txfma0_elo_eb_en_exa_h              ),
    .ehi_eb_en_exa_h              ( txfma0_ehi_eb_en_exa_h              ),
    .elo_ea_en_exa_h              ( txfma0_elo_ea_en_exa_h              ),
    .fa_en_exa_h                  ( txfma0_fa_en_exa_h                  ),
    .fb_en_exa_h                  ( txfma0_fb_en_exa_h                  ),
    .fc_en_exa_h                  ( txfma0_fc_en_exa_h                  ),
    .sel_ealo_ph_exa_h            ( txfma0_sel_ealo_ph_exa_h            ),
    .sel_eainmux_exa_h            ( txfma0_sel_eainmux_exa_h            ),
    .sel_ec5b_exa_h               ( txfma0_sel_ec5b_exa_h               ),
    .sel_ecin_exa_h               ( txfma0_sel_ecin_exa_h               ),
    .sa_hi_en_exa_h               ( txfma0_sa_hi_en_exa_h               ),
    .sa_lo_en_exa_h               ( txfma0_sa_lo_en_exa_h               ),
    .sb_hi_en_exa_h               ( txfma0_sb_hi_en_exa_h               ),
    .sb_lo_en_exa_h               ( txfma0_sb_lo_en_exa_h               ),
    .sc_en_exa_h                  ( txfma0_sc_en_exa_h                  ),
    .ehi_ea_en_exa_h              ( txfma0_ehi_ea_en_exa_h              ),
    .ec_en_exa_h                  ( txfma0_ec_en_exa_h                  ),
    .neg_fmaop_exa_h              ( txfma0_neg_fmaop_exa_h              ),
    .trans_op2_exa_l              ( txfma0_trans_op2_exa_l              ),
    .sa1_nr2_frcpfxp_exa_h        ( txfma0_sa1_nr2_frcpfxp_exa_h        ),
    .sel_sc_exa_h                 ( txfma0_sel_sc_exa_h                 ),
    .sel_sign_sinp2_exa_h         ( txfma0_sel_sign_sinp2_exa_h         ),
    .sa_zap_exa_l                 ( txfma0_sa_zap_exa_l                 ),
    .sb_zap_exa_l                 ( txfma0_sb_zap_exa_l                 ),
    .sel_f10_exa_h                ( txfma0_sel_f10_exa_h                ),
    .sel_f11_exa_h                ( txfma0_sel_f11_exa_h                ),
    .sel_opcgfx_exa_h             ( txfma0_sel_opcgfx_exa_h             ),
    .fc_dtps_exa_h                ( txfma0_fc_dtps_exa_h                ),
    .sel_xopc_exa_h               ( txfma0_sel_xopc_exa_h               ),
    .sel_opcfmt_exa_h             ( txfma0_sel_opcfmt_exa_h             ),
    .sel_snorm_exa_h              ( txfma0_sel_snorm_exa_h              ),
    .sel_g1sk2_exa_h              ( txfma0_sel_g1sk2_exa_h              ),
    .sel_g1sk8_exa_h              ( txfma0_sel_g1sk8_exa_h              ),
    .sel_g1sk16_exa_h             ( txfma0_sel_g1sk16_exa_h             ),
    .sel_g1sk24_exa_h             ( txfma0_sel_g1sk24_exa_h             ),
    .sel_xexpfrac_exa_h           ( txfma0_sel_xexpfrac_exa_h           ),
    .sel_transfa_exa_h            ( txfma0_sel_transfa_exa_h            ),
    .sel_transfb_exa_h            ( txfma0_sel_transfb_exa_h            ),
    .sel_opafmt_exa_h             ( txfma0_sel_opafmt_exa_h             ),
    .sel_opbfmt_exa_h             ( txfma0_sel_opbfmt_exa_h             ),
    .sa_hi_f0a_h                  ( txfma0_sa_hi_f0a_h                  ),
    .sa_lo_f0a_h                  ( txfma0_sa_lo_f0a_h                  ),
    .sb_hi_f0a_h                  ( txfma0_sb_hi_f0a_h                  ),
    .sb_lo_f0a_h                  ( txfma0_sb_lo_f0a_h                  ),
    .sc_f0a_h                     ( txfma0_sc_f0a_h                     ),
    .ea_hi_f0a_h                  ( txfma0_ea_hi_f0a_h                  ),
    .ea_lo_f0a_h                  ( txfma0_ea_lo_f0a_h                  ),
    .eb_hi_f0a_h                  ( txfma0_eb_hi_f0a_h                  ),
    .eb_lo_f0a_h                  ( txfma0_eb_lo_f0a_h                  ),
    .ec_f0a_h                     ( txfma0_ec_f0a_h                     ),
    .ea_hi_zero_f0a_l             ( txfma0_ea_hi_zero_f0a_l             ),
    .ea_hi_all1_f0a_h             ( txfma0_ea_hi_all1_f0a_h             ),
    .ea_lo_zero_f0a_l             ( txfma0_ea_lo_zero_f0a_l             ),
    .ea_lo_all1_f0a_h             ( txfma0_ea_lo_all1_f0a_h             ),
    .eb_hi_zero_f0a_l             ( txfma0_eb_hi_zero_f0a_l             ),
    .eb_hi_all1_f0a_h             ( txfma0_eb_hi_all1_f0a_h             ),
    .eb_lo_zero_f0a_l             ( txfma0_eb_lo_zero_f0a_l             ),
    .eb_lo_all1_f0a_h             ( txfma0_eb_lo_all1_f0a_h             ),
    .ec_zero_f0a_l                ( txfma0_ec_zero_f0a_l                ),
    .ec_all1_f0a_h                ( txfma0_ec_all1_f0a_h                ),
    .eb_eql_127_f0a_h             ( txfma0_eb_eql_127_f0a_h             ),
    .eb_eql_126_f0a_h             ( txfma0_eb_eql_126_f0a_h             ),
    .eb_eql_125_f0a_h             ( txfma0_eb_eql_125_f0a_h             ),
    .fcf10_zero_f0a_l             ( txfma0_fcf10_zero_f0a_l             ),
    .fcf11_zero_f0a_l             ( txfma0_fcf11_zero_f0a_l             ),
    .fcps_zero_f0a_h              ( txfma0_fcps_zero_f0a_h              ),
    .fcph_zero_f0a_l              ( txfma0_fcph_zero_f0a_l              ),
    .fcnorm_zero_29_8_f0a_h       ( txfma0_fcnorm_zero_29_8_f0a_h       ),
    .fcnorm16_eq_1_f0a_h          ( txfma0_fcnorm16_eq_1_f0a_h          ),
    .fcnorm8_eq_1_f0a_h           ( txfma0_fcnorm8_eq_1_f0a_h           ),
    .fcnorm_30_f0a_h              ( txfma0_fcnorm_30_f0a_h              ),
    .faph_hi_b0_f0a_h             ( txfma0_faph_hi_b0_f0a_h             ),
    .fbph_hi_b0_f0a_h             ( txfma0_fbph_hi_b0_f0a_h             ),
    .fcph_b0_f0a_h                ( txfma0_fcph_b0_f0a_h                ),
    .fcf11_b0_f0a_h               ( txfma0_fcf11_b0_f0a_h               ),
    .fcf10_b0_f0a_h               ( txfma0_fcf10_b0_f0a_h               ),
    .fcn8_one_30_24_f0a_h         ( txfma0_fcn8_one_30_24_f0a_h         ),
    .fcn10_one_30_22_f0a_h        ( txfma0_fcn10_one_30_22_f0a_h        ),
    .fcn16_one_30_16_f0a_h        ( txfma0_fcn16_one_30_16_f0a_h        ),
    .fcn24_one_30_8_f0a_h         ( txfma0_fcn24_one_30_8_f0a_h         ),
    .faps_zero_f0a_l              ( txfma0_faps_zero_f0a_l              ),
    .fbps_zero_f0a_l              ( txfma0_fbps_zero_f0a_l              ),
    .faph_hi_zero_f0a_l           ( txfma0_faph_hi_zero_f0a_l           ),
    .fbph_hi_zero_f0a_l           ( txfma0_fbph_hi_zero_f0a_l           ),
    .faph_lo_zero_f0a_l           ( txfma0_faph_lo_zero_f0a_l           ),
    .fbph_lo_zero_f0a_l           ( txfma0_fbph_lo_zero_f0a_l           ),
    .faph_lo_b0_f0a_h             ( txfma0_faph_lo_b0_f0a_h             ),
    .fbph_lo_b0_f0a_h             ( txfma0_fbph_lo_b0_f0a_h             ),
    .faps_b0_f0a_h                ( txfma0_faps_b0_f0a_h                ),
    .fbps_b0_f0a_h                ( txfma0_fbps_b0_f0a_h                ),
    .fc_31_f0a_h                  ( txfma0_fc_31_f0a_h                  ),
    .fcps_b0_f0a_h                ( txfma0_fcps_b0_f0a_h                ),
  //TXFMA_F1
    .mul_sign_f0a_h               ( txfma0_mul_sign_f0a_h               ),
    .mul_hp_mode_f0a_h            ( txfma0_mul_hp_mode_f0a_h            ),
    .dtps_f0a_h                   ( txfma0_dtps_f0a_h                   ),
    .fc_b0_f2a_h                  ( txfma0_fc_b0_f2a_h                  ),
    .etrz_fa_hi_ph_f1a_h          ( txfma0_etrz_fa_hi_ph_f1a_h          ),
    .etrz_fa_lo_ps_f1a_h          ( txfma0_etrz_fa_lo_ps_f1a_h          ),
    .etrz_fb_hi_ph_f1a_h          ( txfma0_etrz_fb_hi_ph_f1a_h          ),
    .etrz_fb_lo_ps_f1a_h          ( txfma0_etrz_fb_lo_ps_f1a_h          ),      
    .etrz_fc_f1a_h                ( txfma0_etrz_fc_f1a_h                ), 
  //TXFMA_F2 
    .paddin1_sels_f1a_h           ( txfma0_paddin1_sels_f1a_h           ),
    .paddin2_selc_f1a_h           ( txfma0_paddin2_selc_f1a_h           ),
    .paddin2_selfcneg_f1a_h       ( txfma0_paddin2_selfcneg_f1a_h       ),
    .padd_cin_f1a_h               ( txfma0_padd_cin_f1a_h               ),
    .sel_famask_f1a_h             ( txfma0_sel_famask_f1a_h             ),
  //TXFMA_F3
    .exp_res_f2a_h                ( txfma0_exp_res_2f3_f2a_h            ),
    .ediff_abs_diff_hi_f2a_h      ( txfma0_ediff_abs_diff_hi_f2a_h      ),
    .align2_rshamt_c3_f2a_h       ( txfma0_align2_rshamt_c3_f2a_h       ),
    .op3p3spc_sel_f2a_h           ( txfma0_op3p3spc_sel_f2a_h           ),
    .op3p1c_sel_p_f2a_h           ( txfma0_op3p1c_sel_p_f2a_h           ),
    .op3p2c_sel_f2a_h             ( txfma0_op3p2c_sel_f2a_h             ),
    .op3p1c_sel_r_f2a_h           ( txfma0_op3p1c_sel_r_f2a_h           ),
    .op3g_sel_f2a_h               ( txfma0_op3g_sel_f2a_h               ),
    .op2_sxtto_g_f2a_h            ( txfma0_op2_sxtto_g_f2a_h            ),
    .imxc_sel_f3a_h               ( txfma0_imxc_sel_f3a_h               ),
    .fround_rbit_f3a_h            ( txfma0_fround_rbit_f3a_h            ),
    .ugfxrplc_sel_f2a_h           ( txfma0_ugfxrplc_sel_f2a_h           ),
    .sgfxrplc_sel_f2a_h           ( txfma0_sgfxrplc_sel_f2a_h           ),
    .icgfxc_sel_f2a_h             ( txfma0_icgfxc_sel_f2a_h             ),
    .al1p1c_sel_f2a_h             ( txfma0_al1p1c_sel_f2a_h             ),
    .al1p2c_sel_f2a_h             ( txfma0_al1p2c_sel_f2a_h             ),
    .rm_rne_f2a_h                 ( txfma0_rm_rne_f2a_h                 ),
    .rm_rmm_f2a_h                 ( txfma0_rm_rmm_f2a_h                 ),
    .frnd_rupnsc_f2a_h            ( txfma0_frnd_rupnsc_f2a_h            ),
    .frnd_rdnsc_f2a_h             ( txfma0_frnd_rdnsc_f2a_h             ),
    .frnd_t1_f2a_h                ( txfma0_frnd_t1_f2a_h                ),
    .frnd_t2_f2a_h                ( txfma0_frnd_t2_f2a_h                ),
    .al1c_zap_f2a_h               ( txfma0_al1c_zap_f2a_h               ),
    .op1c_log_mul_clr_f2a_h       ( txfma0_op1c_log_mul_clr_f2a_h       ),
    .negop1c_int_logmul_clr_f2a_h ( txfma0_negop1c_int_logmul_clr_f2a_h ),
    .psc_op1_clr_f2a_h            ( txfma0_psc_op1_clr_f2a_h            ),
    .al1znp2c_sel_f2a_h           ( txfma0_al1znp2c_sel_f2a_h           ),
    .al2c_sel_f2a_h               ( txfma0_al2c_sel_f2a_h               ),
    .align2_zap_f2a_h             ( txfma0_align2_zap_f2a_h             ),
    .align2c_neg_f2a_h            ( txfma0_align2c_neg_f2a_h            ),
    .prodsum_a2_f3a_h             ( txfma0_prodsum_a2_f3a_h             ),
    .al2c3_neg_cin_ps_f3a_h       ( txfma0_al2c3_neg_cin_ps_f3a_h       ),  
    .fma_subopc3_f3a_h            ( txfma0_fma_subopc3_f3a_h            ),  
  //TXFMA_F4
    .al1c3_neg_cin_f3a_h          ( txfma0_al1c3_neg_cin_f3a_h          ),
    .neg_norm_a3_f4a_h            ( txfma0_neg_norm_a3_f4a_h            ),
    .neg_norm_na3_f4a_h           ( txfma0_neg_norm_na3_f4a_h           ),
    .clearphps_en_f4a_h           ( txfma0_clearphps_en_f4a_h           ),
    .clear16frnd_nshin_f4a_l      ( txfma0_clear16frnd_nshin_f4a_l      ),
    .ph_ps_clear_f4a_l            ( txfma0_ph_ps_clear_f4a_l            ),
    .elxd_f4a_h                   ( txfma0_elxd_f4a_h                   ),
    .etrz50_f4a_h                 ( txfma0_etrz50_f4a_h                 ),
  //TXFMA_F5
    .enc_nsamt_f4a_h              ( txfma0_enc_nsamt_f4a_h              ),
    .en_logrrsel1b_f5a_l          ( txfma0_en_logrrsel1b_f5a_l          ),
    .mmx_sel_immres_f5a_h         ( txfma0_mmx_sel_immres_f5a_h         ),
    .nshcen_sel1b_f5a_h           ( txfma0_nshcen_sel1b_f5a_h           ),
    .rc_ph_f5a_h                  ( txfma0_rc_ph_f5a_h                  ),
    .rc_ps_f5a_h                  ( txfma0_rc_ps_f5a_h                  ),
    .rc_int32_f5a_h               ( txfma0_rc_int32_f5a_h               ),
    .int32_stky2r_f5a_h           ( txfma0_int32_stky2r_f5a_h           ),
    .stky2g_f5a_h                 ( txfma0_stky2g_f5a_h                 ),
    .rc_3rmxr_f5a_h               ( txfma0_rc_3rmxr_f5a_h               ),
    .rc_rl01_f5a_h                ( txfma0_rc_rl01_f5a_h                ),
    .rc_rmm_f5a_h                 ( txfma0_rc_rmm_f5a_h                 ),
    .rc_rup_f5a_h                 ( txfma0_rc_rup_f5a_h                 ),
    .rc_rne_f5a_h                 ( txfma0_rc_rne_f5a_h                 ),
    .rc_rdn_f5a_h                 ( txfma0_rc_rdn_f5a_h                 ),
    .rc_res_sign_f5a_l            ( txfma0_rc_res_sign_f5a_l            ),
    .rc_transrnd_f5a_h            ( txfma0_rc_transrnd_f5a_h            ),
    .rc_transrnd_log_fma2_f5a_h   ( txfma0_rc_transrnd_log_fma2_f5a_h   ),
    .ph_stky2l_f5a_h              ( txfma0_ph_stky2l_f5a_h              ),
    .ps_stky2l_f5a_h              ( txfma0_ps_stky2l_f5a_h              ),
    .int32_stky2l_f5a_h           ( txfma0_int32_stky2l_f5a_h           ),
    .nshc_f5a_h                   ( txfma0_nshc_f5a_h                   ),
    .frac_zero_f6a_h              ( txfma0_frac_zero_f6a_h              ),
    .ph_allone2r_f5a_h            ( txfma0_ph_allone2r_f5a_h            ),
    .ph_allone2l_f5a_h            ( txfma0_ph_allone2l_f5a_h            ),
    .ps_allone2r_f5a_h            ( txfma0_ps_allone2r_f5a_h            ),
    .ps_allone2l_f5a_h            ( txfma0_ps_allone2l_f5a_h            ),
    .comb_add_a3_f5a_h            ( txfma0_comb_add_a3_f5a_h            ),
  //TXFMA_F6
    .rnd_l_ph_f6a_h               ( txfma0_rnd_l_ph_f6a_h               ),
    .rnd_r_ph_f6a_h               ( txfma0_rnd_r_ph_f6a_h               ),
    .rnd_l_ps_f6a_h               ( txfma0_rnd_l_ps_f6a_h               ),
    .rnd_r_ps_f6a_h               ( txfma0_rnd_r_ps_f6a_h               ),
    .classify_f6a_h               ( txfma0_classify_f6a_h               ),
    .trans2_ps_roundup_r_f5a_h    ( txfma0_trans2_ps_roundup_r_f5a_h    ),
    .normmask2_f6a_h              ( txfma0_normmask2_f6a_h              ),
    .normmask1_f6a_h              ( txfma0_normmask1_f6a_h              ),
    .sel_classify_out_f6a_h       ( txfma0_sel_classify_out_f6a_h       ),
    .sel_nr1_frcpres_f6a_h        ( txfma0_sel_nr1_frcpres_f6a_h        ),
    .trans_sinp2m_f6a_h           ( txfma0_trans_sinp2m_f6a_h           ),
    .s_res_f6a_h                  ( txfma0_s_res_f6a_h                  ),
    .exp_res_f6a_h                ( txfma0_exp_res_f6a_h                ),
    .rrmux32_sel_f6a_h            ( txfma0_rrmux32_sel_f6a_h            ),
    .rrmux16l_sel_f6a_l           ( txfma0_rrmux16l_sel_f6a_l           ),
    .rc6_ph_f5a_h                 ( txfma0_rc6_ph_f5a_h                 ),
    .rc6_ps_f5a_h                 ( txfma0_rc6_ps_f5a_h                 ),
    .ps_rndovf_f6a_h              ( txfma0_ps_rndovf_f6a_h              ),
    .ph_rndovf_f6a_h              ( txfma0_ph_rndovf_f6a_h              ),
    .sel_set_bit0_one_f6a_h       ( txfma0_sel_set_bit0_one_f6a_h       ),
    .effsub1_set_sign1_f6a_h      ( txfma0_effsub1_set_sign1_f6a_h      ),
    .f10f11mux_sel_f6a_h          ( txfma0_f10f11mux_sel_f6a_h          ),
    .sel_psexc_sr_f6a_h           ( txfma0_sel_psexc_sr_f6a_h           ),
    .sel_bit31_one_f6a_h          ( txfma0_sel_bit31_one_f6a_h          ),
    .sel_b30to0_ones_f6a_h        ( txfma0_sel_b30to0_ones_f6a_h        ),
    .sel_ps_unf_f6a_h             ( txfma0_sel_ps_unf_f6a_h             ),
    .sel_zerores_f6a_h            ( txfma0_sel_zerores_f6a_h            ),
    .sel_ps_normone_f6a_h         ( txfma0_sel_ps_normone_f6a_h         ),
    .sel_ps_inf_f6a_h             ( txfma0_sel_ps_inf_f6a_h             ),
    .sel_ps_cqn_f6a_h             ( txfma0_sel_ps_cqn_f6a_h             ),
    .sel_ps_large_f6a_h           ( txfma0_sel_ps_large_f6a_h           ),
    .sel_phsp_f6a_h               ( txfma0_sel_phsp_f6a_h               ),
    .sel_phhi_sr_f6a_h            ( txfma0_sel_phhi_sr_f6a_h            ),
    .sel_phhi_large_f6a_h         ( txfma0_sel_phhi_large_f6a_h         ),
    .sel_phhi_unf_f6a_h           ( txfma0_sel_phhi_unf_f6a_h           ),
    .sel_phhi_infres_f6a_h        ( txfma0_sel_phhi_infres_f6a_h        ),
    .sel_f11inf_f6a_h             ( txfma0_sel_f11inf_f6a_h             ),
    .sel_f10cqnan_f6a_h           ( txfma0_sel_f10cqnan_f6a_h           ),
    .sel_f11sp_f6a_h              ( txfma0_sel_f11sp_f6a_h              ),
    .sel_f10sp_f6a_h              ( txfma0_sel_f10sp_f6a_h              ),
    .sel_f11large_f6a_h           ( txfma0_sel_f11large_f6a_h           ),
    .sel_f10large_f6a_h           ( txfma0_sel_f10large_f6a_h           ),
    .sel_gmaxu24_f6a_h            ( txfma0_sel_gmaxu24_f6a_h            ),
    .sel_gmaxu16_f6a_h            ( txfma0_sel_gmaxu16_f6a_h            ),
    .sel_gmaxs16_f6a_h            ( txfma0_sel_gmaxs16_f6a_h            ),
    .sel_gmins16_f6a_h            ( txfma0_sel_gmins16_f6a_h            ),
    .sel_gmaxs8_f6a_h             ( txfma0_sel_gmaxs8_f6a_h             ),
    .sel_gmaxu2_f6a_h             ( txfma0_sel_gmaxu2_f6a_h             ),
    .sel_gmaxu8_f6a_h             ( txfma0_sel_gmaxu8_f6a_h             ),
    .sel_gmins8_f6a_h             ( txfma0_sel_gmins8_f6a_h             ),
    .sel_gfxmax_f6a_h             ( txfma0_sel_gfxmax_f6a_h             ),
    .sel_excreshi_f6a_h           ( txfma0_sel_excreshi_f6a_h           ),
    .sel_excreslo_f6a_h           ( txfma0_sel_excreslo_f6a_h           ),
    .sel_fmares_hi_f6a_h          ( txfma0_sel_fmares_hi_f6a_h          ),
    .sel_fmares_lo_f6a_h          ( txfma0_sel_fmares_lo_f6a_h          ),
    .int_res_l_bit_f6a_h          ( txfma0_int_res_l_bit_f6a_h          ),
    .fma_res_f6a_h                ( txfma0_fma_res_f6a_h                )
  );
`ifdef MINION_USE_MMI_TXFMA
end
endgenerate
`endif

///////////////////////////////////////////////////////////////////////////////
// TXFMA Exponent Blocks 
///////////////////////////////////////////////////////////////////////////////
txfmaexp_top txfmaexp0_top 
(
  .clock                      (  clock                             ),
  .ctrl_valid_f1a_h           ( txfma0_ctrl_valid_f1a_h            ),
  .ctrl_valid_f2a_h           ( txfma0_ctrl_valid_f2a_h            ),
  .ctrl_valid_f3a_h           ( txfma0_ctrl_valid_f3a_h            ),
  .ctrl_valid_f4a_h           ( txfma0_ctrl_valid_f4a_h            ),
  .ctrl_valid_f5a_h           ( txfma0_ctrl_valid_f5a_h            ),
  .ec_zero_f1a_h              ( txfma0_ec_zero_f1a_h               ),
  .ec_all1_f1a_h              ( txfma0_ec_all1_f1a_h               ),
// TXFMA E1
  .op_dtype_f1a_h             ( txfma0_op_dtype_f1a_h              ),
  .sigs_f1a_h                 ( txfma0_sigs_f1a_h                  ),
  .cmd_f1a_h                  ( txfma0_cmd_f1a_h                   ),
  .ea_hi_f1a_h                ( txfma0_ea_hi_f1a_h                 ),
  .ea_lo_f1a_h                ( txfma0_ea_lo_f1a_h                 ),
  .eb_hi_f1a_h                ( txfma0_eb_hi_f1a_h                 ),
  .eb_lo_f1a_h                ( txfma0_eb_lo_f1a_h                 ),
  .ec_f1a_h                   ( txfma0_ec_f1a_h                    ),
  .ea_hi_zero_f1a_h           ( txfma0_ea_hi_zero_f1a_h            ),
  .etrz_fa_hi_ph_f1a_h        ( txfma0_etrz_fa_hi_ph_f1a_h         ),
  .etrz_fa_lo_ps_f1a_h        ( txfma0_etrz_fa_lo_ps_f1a_h         ),
  .etrz_fb_hi_ph_f1a_h        ( txfma0_etrz_fb_hi_ph_f1a_h         ),
  .etrz_fb_lo_ps_f1a_h        ( txfma0_etrz_fb_lo_ps_f1a_h         ),      
  .etrz_fc_f1a_h              ( txfma0_etrz_fc_f1a_h               ),      
  .fc_zero30_8_f1a_h          ( txfma0_fc_zero30_8_f1a_h           ),
// TXFMA E2
  .cmd_f2a_h                  ( txfma0_cmd_f2a_h                   ), 
  .op_dtype_f2a_h             ( txfma0_op_dtype_f2a_h              ),
  .sigs_f2a_h                 ( txfma0_sigs_f2a_h                  ),
  .ec_f2a_h                   ( txfma0_ec_f2a_h                    ),
  .ec_f3a_h                   ( txfma0_ec_f3a_h                    ),
  .sin_ediff2_f2a_h           ( txfma0_sin_ediff2_f2a_h            ),
  .eprod_hi_zero_f2a_h        ( txfma0_eprod_hi_zero_f2a_h         ),
  .eprod_lo_zero_f2a_h        ( txfma0_eprod_lo_zero_f2a_h         ),
  .ec_zero_f2a_h              ( txfma0_ec_zero_f2a_h               ),
  .exp_fma2_exc_f2a_h         ( txfma0_trans_exp_fma2_exc_f2a_h    ),
  .e_sticky1_f2a_h            ( txfma0_e_sticky1_f2a_h             ),
  .e_sticky2_f2a_h            ( txfma0_e_sticky2_f2a_h             ),
  .e_sticky1_f3a_h            ( txfma0_e_sticky1_f3a_h             ),
  .ediff_abs_diff_hi_f2a_h    ( txfma0_ediff_abs_diff_hi_f2a_h     ),
  .align2_rshamt_c3_f2a_h     ( txfma0_align2_rshamt_c3_f2a_h      ),
  .align1_mux_sel_f2a_h       ( txfma0_align1_mux_sel_f2a_h        ),
  .align2_mux_sel_f2a_h       ( txfma0_align2_mux_sel_f2a_h        ),
  .opd3_mux_sel_f2a_h         ( txfma0_opd3_mux_sel_f2a_h          ),
  .ediff1_f2a_h               ( txfma0_ediff1_f2a_h                ),
  .ediff1_abs_f2a_h           ( txfma0_ediff1_abs_f2a_h            ),
  .ediff2_abs_f2a_h           ( txfma0_ediff2_abs_f2a_h            ),
  .exp_res_2f3_f2a_h          ( txfma0_exp_res_2f3_f2a_h           ),
  .ediff1_sel_f2a_h           ( txfma0_ediff1_sel_f2a_h            ),
  .ediff2_sel_f2a_h           ( txfma0_ediff2_sel_f2a_h            ),
  .ediff2_f3a_h               ( txfma0_ediff2_f3a_h                ),
// TXFMA_E4
  .elxd_f4a_h                 ( txfma0_elxd_f4a_h                  ), 
  .etrz50_f4a_h               ( txfma0_etrz50_f4a_h                ),
  .use_elxd_f4a_h             ( txfma0_use_elxd_f4a_h              ),
  .elxd_add_in2_mux_sel_f4a_h ( txfma0_elxd_add_in2_mux_sel_f4a_h  ),
  .fb_hi_zero_f4a_h           ( txfma0_fb_hi_zero_f4a_h            ),
  .eb_hi_lsb_f4a_h            ( txfma0_eb_hi_lsb_f4a_h             ),
  .ph_comb_stiky_f4a_h        ( txfma0_ph_comb_stiky_f4a_h         ),
// TXFMA E5
  .stky2g_f5a_h               ( txfma0_stky2g_f5a_h                ),
  .int_minmax_mux_sel_a_f5a_h ( txfma0_int_minmax_mux_sel_a_f5a_h  ),
  .int_minmax_mux_sel_c_f5a_h ( txfma0_int_minmax_mux_sel_c_f5a_h  ),
  .cmd_f5a_h                  ( txfma0_cmd_f5a_h                   ),
  .s_res_f5a_h                ( txfma0_s_res_f5a_h                 ),
  .rm_f5a_h                   ( txfma0_rm_f5a_h                    ),
  .op_dtype_f5a_h             ( txfma0_op_dtype_f5a_h              ),
  .sigs_f5a_h                 ( txfma0_sigs_f5a_h                  ),
  .ea_hi_f5a_h                ( txfma0_ea_hi_f5a_h                 ),
  .comp_op_f5a_h              ( txfma0_comp_op_f5a_h               ),
  .comp_op_true_f5a_h         ( txfma0_comp_op_true_f5a_h          ),
  .nshc_f5a_h                 ( txfma0_nshc_f5a_h                  ),
  .e_force_to_zero_f5a_h      ( txfma0_e_force_to_zero_f5a_h       ),
  .ediff1_zap_f5a_h           ( txfma0_ediff1_zap_f5a_h            ),
  .rmode_f5a_h                ( txfma0_rmode_f5a_h                 ),
// TXFMA E6
  .def_ovf_ps_f6a_h           ( txfma0_def_ovf_ps_f6a_h            ),
  .pot_ovf_ps_f6a_h           ( txfma0_pot_ovf_ps_f6a_h            ),
  .def_unf_ps_f6a_h           ( txfma0_def_unf_ps_f6a_h            ),
  .pot_unf_ps_f6a_h           ( txfma0_pot_unf_ps_f6a_h            ),
  .def_ovf_ph_f6a_h           ( txfma0_def_ovf_ph_f6a_h            ),
  .pot_ovf_ph_f6a_h           ( txfma0_pot_ovf_ph_f6a_h            ),
  .def_unf_ph_f6a_h           ( txfma0_def_unf_ph_f6a_h            ),
  .pot_unf_ph_f6a_h           ( txfma0_pot_unf_ph_f6a_h            ),
  .cmd_f6a_h                  ( txfma0_cmd_f6a_h                   ),
  .sigs_f6a_h                 ( txfma0_sigs_f6a_h                  ),
  .op_dtype_f6a_h             ( txfma0_op_dtype_f6a_h              ),  
  .ps_rndovf_f6a_h            ( txfma0_ps_rndovf_f6a_h             ),
  .ph_rndovf_f6a_h            ( txfma0_ph_rndovf_f6a_h             ),
  .exp_res_f6a_h              ( txfma0_exp_res_f6a_h               ),
  .exp_res_zero_f6a_h         ( txfma0_exp_res_zero_f6a_h          )
);

///////////////////////////////////////////////////////////////////////////////
// TXFMA Final Output (Data, Flags) 
///////////////////////////////////////////////////////////////////////////////

assign txfma0_f6_out_data = { txfma0_fma_res_f6a_h, txfma0_fma_flags_f6a_h };
assign out_data_res       = txfma0_f6_out_data & {($bits(txfma0_f6_out_data)){txfma0_ctrl_valid_f6a_h}};
assign out_comp_res       = txfma0_comp_op_true_f6a_h & txfma0_ctrl_valid_f6a_h;

endmodule
