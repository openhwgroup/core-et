// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_top
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
logic [`VPU_DATA_S_SZ-1:0]                   txfma0_opa_f0a_h; 
logic [`VPU_DATA_S_SZ-1:0]                   txfma0_opb_f0a_h; 
logic [`VPU_DATA_S_SZ-1:0]                   txfma0_opc_f0a_h;
logic [`TRANS_C0_MAX_SIZE-1:0]               txfma0_trans_c0_c1_f0a_h;
logic [`TRANS_C2_MAX_SIZE-1:0]               txfma0_trans_c2_f0a_h;
logic [`TRANS_MASK_SIZE-1:0]                 txfma0_trans_mask_f0a_h;
logic [`TXFMA_A0:`TXFMA_B30]                 txfma0_fa_f0a_h;
logic [`TXFMA_B28:0]                         txfma0_fb_f0a_h;
logic [`TXFMA_A0:`TXFMA_B30]                 txfma0_fc_f0a_h;
logic [`FLOAT32_M-1:0]                       txfma0_fb_trans_f0a_h;
logic [`FLOAT32_M-1:0]                       txfma0_fb_raw_trans_f1a_h;
logic [`TXFMA_A0:`TXFMA_B30]                 txfma0_fa_f1a_h;
logic [`VPU_DATA_S_SZ-1:0]                   txfma0_fb_f1a_h;
logic [`TXFMA_A0:`TXFMA_B30]                 txfma0_fc_f1a_h;
logic [`TXFMA_A0:`TXFMA_B30]                 txfma0_fa_f2a_h; 
logic [`TXFMA_A0:`TXFMA_B30]                 txfma0_fa_out_f2a_h; 
logic [`TXFMA_A0:`TXFMA_B30]                 txfma0_fc_f2a_h;
logic [`TXFMA_A0:`TXFMA_B22]                 txfma0_fa_f3a_h;
logic [`TXFMA_A0:`TXFMA_B30]                 txfma0_fc_f3a_h;
logic                                        txfma0_eb_hi_lsb_f2a_h;
logic                                        txfma0_eb_hi_lsb_f3a_h;
logic                                        txfma0_eb_hi_lsb_f4a_h;
logic                                        txfma0_prod_sign_hi_f1a_h;    
logic                                        txfma0_prod_sign_hi_f2a_h; 
logic                                        txfma0_prod_sign_hi_f3a_h;
logic                                        txfma0_prod_sign_hi_f4a_h;
logic                                        txfma0_prod_sign_lo_f1a_h;    
logic                                        txfma0_prod_sign_lo_f2a_h;
logic                                        txfma0_prod_sign_lo_f3a_h;
logic                                        txfma0_prod_sign_lo_f4a_h;
logic                                        txfma0_s_res_check_cadd_a3_f3a_h;
logic                                        txfma0_s_res_check_cadd_a3_f4a_h;
logic                                        txfma0_mul_sign_f1a_h;
logic                                        txfma0_eff_sign1_f2a_h;
logic                                        txfma0_eff_sign1_f3a_h;
logic                                        txfma0_ctrl_valid_f0a_h; 
logic                                        txfma0_ctrl_valid_f1a_h; 
logic                                        txfma0_ctrl_valid_f2a_h; 
logic                                        txfma0_ctrl_valid_f3a_h; 
logic                                        txfma0_ctrl_valid_f4a_h;
logic                                        txfma0_ctrl_valid_f5a_h;
vpu_ctrl_sigs_t                              txfma0_ctrl_sigs_f0a_h;
logic [`TXFMA_EXP_PH_SZ-1:0]                 txfma0_ea_lo_f1a_h;
logic [`TXFMA_EXP_PH_SZ-1:0]                 txfma0_eb_lo_f1a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_ea_hi_f1a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_ea_hi_f2a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_ea_hi_f3a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_ea_hi_f4a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_eb_hi_f1a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_ec_f1a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_eb_hi_f2a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_ec_f2a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_ec_f3a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_ec_f4a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_ec_out_f1a_h;
logic [`TXFMA_A0:`TXFMA_B31]                 txfma0_mergd_frac_res_f4a_h; 
logic [`TXFMA_A0:`TXFMA_B31]                 txfma0_mergd_frac_res_f5a_h;
logic [`TXFMA_A0:`TXFMA_B0]                  txfma0_norm_shift_out_neg_a0b0_f4a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_e_f4a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_exp_frac_e_shift_f4a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_e_res_f2a_h; 
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_e_res_f3a_h; 
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_e_res_f4a_h; 
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_e_res_f5a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_e_f5a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_exp_frac_e_shift_f2a_h; 
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_exp_frac_e_shift_f3a_h; 
logic [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]          txfma0_ediff1_sel_f2a_h;
logic [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]          txfma0_ediff2_sel_f2a_h;
logic                                        txfma0_e_sticky1_until_r_f4a_h;
logic                                        txfma0_e_sticky1_until_g_f4a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_norm_shfamnt_f4a_h;
logic                                        txfma0_e_force_to_zero_f4a_h;
logic                                        txfma0_e_sticky1_f2a_h;
logic                                        txfma0_e_sticky1_f3a_h;
logic                                        txfma0_e_sticky1_f4a_h;
logic                                        txfma0_e_sticky2_f2a_h;
logic                                        txfma0_e_sticky2_f3a_h;
logic                                        txfma0_e_sticky2_f4a_h;
logic [`TXFMA_LXD_PS_SZ-1:0]                 txfma0_lxd_f3a_h; 
logic [`TXFMA_LXD_PS_SZ-1:0]                 txfma0_lxd_f4a_h;
logic [`TXFMA_WSPH_ETRZ_SZ-1:0]              txfma0_comb_add_hi_ph_trz_f3a_h;
logic [`TXFMA_WSPH_ETRZ_SZ-1:0]              txfma0_comb_add_hi_ph_trz_f4a_h;
logic [`TXFMA_WSPS_ETRZ_SZ-1:0]              txfma0_comb_add_lo_ps_trz_f3a_h;
logic [`TXFMA_WSPS_ETRZ_SZ-1:0]              txfma0_comb_add_lo_ps_trz_f4a_h;
logic [`TXFMA_A3:0]                          txfma0_wallace_fC21_f1a_h; 
logic [`TXFMA_A3:0]                          txfma0_wallace_fC22_f1a_h; 
logic [`TXFMA_A3:0]                          txfma0_wallace_fS21_f1a_h; 
logic [`TXFMA_A3:0]                          txfma0_wallace_fS22_f1a_h; 
logic [`TXFMA_A3:0]                          txfma0_wallace_fC21_f2a_h; 
logic [`TXFMA_A3:0]                          txfma0_wallace_fC22_f2a_h; 
logic [`TXFMA_A3:0]                          txfma0_wallace_fS21_f2a_h; 
logic [`TXFMA_A3:0]                          txfma0_wallace_fS22_f2a_h;
logic                                        txfma0_eprod_hi_zero_f1a_h;
logic                                        txfma0_eprod_hi_zero_f2a_h;
logic                                        txfma0_eprod_hi_zero_f3a_h;
logic                                        txfma0_eprod_hi_zero_f4a_h;
logic                                        txfma0_eprod_lo_zero_f1a_h;
logic                                        txfma0_eprod_lo_zero_f2a_h;
logic                                        txfma0_eprod_lo_zero_f3a_h;
logic                                        txfma0_eprod_lo_zero_f4a_h;
logic [`VPU_DATA_S_SZ-1:0]                   txfma0_booth_fCin_f1a_h; 
logic [`VPU_DATA_S_SZ-1:0]                   txfma0_booth_fSxt_f1a_h; 
logic [32:0]                                 txfma0_booth_fpp17_f1a_h; 
logic [`VPU_DATA_S_SZ-1:0]                   txfma0_booth_fCin_f2a_h; 
logic [`VPU_DATA_S_SZ-1:0]                   txfma0_booth_fSxt_f2a_h; 
logic [32:0]                                 txfma0_booth_fpp17_f2a_h;
logic                                        txfma0_s_res_f3a_h; 
logic                                        txfma0_s_res_f4a_h; 
logic                                        txfma0_s_res_out_f4a_h; 
logic                                        txfma0_s_res_f5a_h;
logic                                        txfma0_s_f5a_h;
logic [`VPU_DTYPE_SZ-1:0]                    txfma0_op_dtype_f0a_h; 
logic [`VPU_DTYPE_SZ-1:0]                    txfma0_op_dtype_out_f0a_h; 
logic [`VPU_DTYPE_SZ-1:0]                    txfma0_op_dtype_f1a_h;
logic [`VPU_DTYPE_SZ-1:0]                    txfma0_op_dtype_f2a_h;
logic [`VPU_DTYPE_SZ-1:0]                    txfma0_op_dtype_f3a_h;
logic [`VPU_DTYPE_SZ-1:0]                    txfma0_op_dtype_f4a_h; 
logic [`VPU_DTYPE_SZ-1:0]                    txfma0_op_dtype_f5a_h;
logic                                        txfma0_use_prev_sigs_f0a_h;
logic                                        txfma0_use_prev_sigs_f1a_h;
logic                                        txfma0_use_prev_sigs_f2a_h;
logic                                        txfma0_use_prev_sigs_f3a_h;
logic                                        txfma0_use_prev_sigs_f4a_h;
logic [`VPU_FCMD_SZ-1:0]                     txfma0_cmd_f0a_h; 
logic [`VPU_FCMD_SZ-1:0]                     txfma0_cmd_f1a_h;
logic [`VPU_FCMD_SZ-1:0]                     txfma0_cmd_f2a_h;
logic [`VPU_FCMD_SZ-1:0]                     txfma0_cmd_f3a_h;
logic [`VPU_FCMD_SZ-1:0]                     txfma0_cmd_f4a_h; 
logic [`VPU_FCMD_SZ-1:0]                     txfma0_cmd_f5a_h;
logic                                        txfma0_sa_hi_f0a_h; 
logic                                        txfma0_sb_hi_f0a_h; 
logic                                        txfma0_sc_f0a_h;
logic                                        txfma0_sa_lo_f0a_h; 
logic                                        txfma0_sb_lo_f0a_h; 
logic                                        txfma0_sa_hi_f1a_h; 
logic                                        txfma0_sb_hi_f1a_h; 
logic                                        txfma0_sc_f1a_h;
logic                                        txfma0_sa_lo_f1a_h; 
logic                                        txfma0_sb_lo_f1a_h; 
logic                                        txfma0_sc_f2a_h;
logic                                        txfma0_sc_f3a_h;
logic                                        txfma0_sc_f4a_h;
logic [`TXFMA_ETRZ_PH_SZ-1:0]                txfma0_etrz_fa_hi_ph_f1a_h;
logic [`TXFMA_ETRZ_PS_SZ-1:0]                txfma0_etrz_fa_lo_ps_f1a_h;
logic [`TXFMA_ETRZ_PH_SZ-1:0]                txfma0_etrz_fb_hi_ph_f1a_h;
logic [`TXFMA_ETRZ_PS_SZ-1:0]                txfma0_etrz_fb_lo_ps_f1a_h;
logic [`TXFMA_ETRZ_PS_SZ-1:0]                txfma0_etrz_fc_lo_ps_f1a_h;
logic [`TXFMA_SIGS_SZ-1:0]                   txfma0_sigs_f0a_h; 
logic [`TXFMA_SIGS_SZ-1:0]                   txfma0_sigs_f1a_h; 
logic [`TXFMA_SIGS_SZ-1:0]                   txfma0_sigs_f2a_h;
logic [`TXFMA_SIGS_SZ-1:0]                   txfma0_sigs_f3a_h;
logic [`TXFMA_SIGS_SZ-1:0]                   txfma0_sigs_f4a_h; 
logic                                        txfma0_align1_neg_cin_f3a_h;
logic                                        txfma0_align2_neg_cin_f3a_h;
logic                                        txfma0_fround_l_f3a_h;
logic                                        txfma0_fround_f4a_h;
logic                                        txfma0_fc_sel_f3a_h;
logic                                        txfma0_fc_sel_f4a_h;
logic [`TXFMA_SIGS_SZ-1:0]                   txfma0_sigs_f5a_h;
logic                                        txfma0_ea_hi_zero_clr_f0a_h;
logic                                        txfma0_eb_hi_zero_clr_f0a_h;
logic                                        txfma0_ec_zero_clr_f0a_h; 
logic                                        txfma0_ea_lo_zero_clr_f0a_h;
logic                                        txfma0_eb_lo_zero_clr_f0a_h;
logic                                        txfma0_ea_hi_zero_f1a_h;
logic                                        txfma0_eb_hi_zero_f1a_h;
logic                                        txfma0_ec_zero_f1a_h; 
logic                                        txfma0_ea_lo_zero_f1a_h;
logic                                        txfma0_eb_lo_zero_f1a_h;
logic                                        txfma0_ec_zero_f2a_h;
logic                                        txfma0_ec_zero_f3a_h;
logic                                        txfma0_ec_zero_f4a_h; 
logic                                        txfma0_ec_zero_f5a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_eprod_hi_f1a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_eprod_lo_f1a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_eprod_hi_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_eprod_lo_f2a_h;
logic [`TXFMA_ETRZ_PROD_PH_SZ-1:0]           txfma0_etrz_prod_hi_ph_f1a_h;
logic [`TXFMA_ETRZ_PROD_PS_SZ-1:0]           txfma0_etrz_prod_lo_ps_f1a_h;
logic [`TXFMA_ETRZ_PROD_PH_SZ-1:0]           txfma0_etrz_prod_hi_ph_f2a_h;
logic [`TXFMA_ETRZ_PROD_PS_SZ-1:0]           txfma0_etrz_prod_lo_ps_f2a_h;
logic [`TXFMA_ETRZ_PS_SZ-1:0]                txfma0_etrz_fa_lo_ps_f2a_h;
logic [`TXFMA_ETRZ_PS_SZ-1:0]                txfma0_etrz_fc_lo_ps_f2a_h;
logic                                        txfma0_eff_sub1_f2a_h;
logic                                        txfma0_eff_sub1_f3a_h;
logic                                        txfma0_eff_sub1_out_f3a_h;
logic                                        txfma0_eff_sub1_f4a_h;
logic                                        txfma0_eff_sub1_f5a_h;
logic                                        txfma0_eff_sub2_f2a_h;
logic                                        txfma0_eff_sub2_f3a_h;
logic                                        txfma0_eff_sub2_f4a_h;
logic                                        txfma0_lxd_sub_f3a_h;
logic                                        txfma0_trans_log_rr_neg_f3a_h;
logic                                        txfma0_trans_log_rr_neg_f4a_h;
logic                                        txfma0_use_lxd_f4a_h;
logic                                        txfma0_e_lxd_add_in2_mux_sel_f4a_h;
logic                                        txfma0_e_lxd_add_cin_f4a_h;
logic                                        txfma0_nsh_correction_f4a_h;
logic [`TXFMA_RM_SZ-1:0]                     txfma0_rm_f0a_h; 
logic [`TXFMA_RM_SZ-1:0]                     txfma0_rm_f1a_h; 
logic [`TXFMA_RM_SZ-1:0]                     txfma0_rm_f2a_h;
logic [`TXFMA_RM_SZ-1:0]                     txfma0_rm_f3a_h;
logic [`TXFMA_RM_SZ-1:0]                     txfma0_rm_f4a_h; 
logic [`TXFMA_RM_SZ-1:0]                     txfma0_rm_f5a_h; 
logic                                        txfma0_rm_clr_f4a_h; 
logic                                        txfma0_rm_rne_f4a_h;
logic                                        txfma0_rm_rmm_f4a_h;
logic [`TXFMA_RND_CTL_SZ-1:0]                txfma0_rnd_ctl_f4a_h; 
logic [`TXFMA_RND_CTL_SZ-1:0]                txfma0_rnd_ctl_f5a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_ea_hi_f0a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_eb_hi_f0a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_ec_f0a_h;
logic [`TXFMA_EXP_PH_SZ-1:0]                 txfma0_ea_lo_f0a_h;
logic [`TXFMA_EXP_PH_SZ-1:0]                 txfma0_eb_lo_f0a_h;
logic [`TXFMA_EXP_PS_SZ-1:0]                 txfma0_eb_hi_clr_f1a_h;
logic [`TXFMA_FTZ_SZ-1:0]                    txfma0_ftz_f4a_h;
logic [`TXFMA_FTZ_SZ-1:0]                    txfma0_ftz_f5a_h;
logic                                        txfma0_use_product_f2a_h;
logic                                        txfma0_res_inf_f0a_h;
logic                                        txfma0_res_inf_f1a_h;
logic                                        txfma0_res_inf_f2a_h;
logic                                        txfma0_res_inf_f3a_h;
logic                                        txfma0_res_inf_out_f3a_h;
logic                                        txfma0_res_inf_f4a_h;
logic                                        txfma0_res_inf_f5a_h;
logic                                        txfma0_res_snan_f0a_h;
logic                                        txfma0_res_snan_f1a_h;
logic                                        txfma0_res_snan_f2a_h;
logic                                        txfma0_res_snan_f3a_h;
logic                                        txfma0_res_snan_out_f3a_h;
logic                                        txfma0_res_snan_f4a_h;
logic                                        txfma0_res_snan_f5a_h;
logic                                        txfma0_res_qnan_f0a_h;
logic                                        txfma0_res_qnan_f1a_h;
logic                                        txfma0_res_qnan_f2a_h;
logic                                        txfma0_res_qnan_f3a_h;
logic                                        txfma0_res_qnan_out_f3a_h;
logic                                        txfma0_res_qnan_f4a_h;
logic                                        txfma0_res_qnan_f5a_h;
logic                                        txfma0_rnd_ovf_f5a_h;
logic [`FP_CLASS_BITS-1:0]                   txfma0_classify_f0a_h;
logic [`FP_CLASS_BITS-1:0]                   txfma0_classify_f1a_h;
logic [`FP_CLASS_BITS-1:0]                   txfma0_classify_f2a_h;
logic [`FP_CLASS_BITS-1:0]                   txfma0_classify_f3a_h;
logic [`FP_CLASS_BITS-1:0]                   txfma0_classify_f4a_h;
logic [`FP_CLASS_BITS-1:0]                   txfma0_classify_f5a_h;
logic                                        txfma0_prod_inf_hi_f0a_h;
logic                                        txfma0_prod_inf_hi_f1a_h;
logic                                        txfma0_prod_inf_hi_f2a_h;
logic                                        txfma0_prod_inf_hi_f3a_h;
logic                                        txfma0_c_inf_f0a_h;
logic                                        txfma0_c_inf_f1a_h;
logic                                        txfma0_c_inf_f2a_h;
logic                                        txfma0_c_inf_f3a_h;
logic                                        txfma0_a_inf_f0a_h;
logic                                        txfma0_a_inf_f1a_h;
logic                                        txfma0_a_inf_f2a_h;
logic                                        txfma0_a_inf_f3a_h;
logic                                        txfma0_a_inf_f4a_h;
logic                                        txfma0_a_inf_f5a_h;
logic                                        txfma0_prod_inf_lo_f0a_h;
logic                                        txfma0_prod_inf_lo_f1a_h;
logic                                        txfma0_prod_inf_lo_f2a_h;
logic                                        txfma0_prod_inf_lo_f3a_h;
logic                                        txfma0_opa_valid_f0a_h;
logic                                        txfma0_opb_valid_f0a_h;
logic                                        txfma0_opc_valid_f0a_h;
logic [`VPU_FLAGS_SZ-1:0]                    txfma0_flags_f0a_h;
logic [`VPU_FLAGS_SZ-1:0]                    txfma0_flags_f1a_h;
logic [`VPU_FLAGS_SZ-1:0]                    txfma0_flags_f2a_h;
logic [`VPU_FLAGS_SZ-1:0]                    txfma0_flags_f3a_h;
logic [`VPU_FLAGS_SZ-1:0]                    txfma0_flags_f4a_h;
logic [`VPU_FLAGS_SZ-1:0]                    txfma0_flags_f5a_h;
logic                                        txfma0_sticky_until_l_f4a_h;
logic                                        txfma0_sticky_until_l_f5a_h;
logic                                        txfma0_pot_ovf_f4a_h;
logic                                        txfma0_def_ovf_f4a_h;
logic                                        txfma0_pot_ovf_f5a_h;
logic                                        txfma0_def_ovf_f5a_h;
logic                                        txfma0_pot_unf_f4a_h;
logic                                        txfma0_def_unf_f4a_h;
logic                                        txfma0_pot_unf_f5a_h;
logic                                        txfma0_def_unf_f5a_h;
logic                                        txfma0_comp_op_f2a_h;
logic                                        txfma0_comp_op_f3a_h;
logic                                        txfma0_comp_op_f4a_h;
logic                                        txfma0_comp_op_f5a_h;
logic                                        txfma0_rmode_f4a_h;
logic                                        txfma0_rmode_f5a_h;
logic                                        txfma0_minmax_op_f2a_h;
logic                                        txfma0_minmax_op_f3a_h;
logic                                        txfma0_minmax_op_f4a_h;
logic                                        txfma0_minmax_op_f5a_h;
logic                                        txfma0_a_is_qnan_hi_f0a_h;
logic                                        txfma0_a_is_qnan_hi_f1a_h;
logic                                        txfma0_c_is_qnan_f0a_h;
logic                                        txfma0_c_is_qnan_f1a_h;
logic                                        txfma0_a_is_qnan_hi_f2a_h;
logic                                        txfma0_c_is_qnan_f2a_h;
logic                                        txfma0_a_is_qnan_hi_f3a_h;
logic                                        txfma0_c_is_qnan_f3a_h;
logic                                        txfma0_a_is_snan_hi_f0a_h;
logic                                        txfma0_a_is_snan_hi_f1a_h;
logic                                        txfma0_c_is_snan_f0a_h;
logic                                        txfma0_c_is_snan_f1a_h;
logic                                        txfma0_a_is_snan_hi_f2a_h;
logic                                        txfma0_c_is_snan_f2a_h;
logic                                        txfma0_a_is_snan_hi_f3a_h;
logic                                        txfma0_c_is_snan_f3a_h;
logic                                        txfma0_res_pos_int_nan_f0a_h;
logic                                        txfma0_res_pos_int_nan_f1a_h;
logic                                        txfma0_res_pos_int_nan_f2a_h;
logic                                        txfma0_res_pos_int_nan_f3a_h;
logic                                        txfma0_res_pos_int_nan_f4a_h;
logic                                        txfma0_res_pos_int_nan_f5a_h;
logic                                        txfma0_res_neg_int_nan_f0a_h;
logic                                        txfma0_res_neg_int_nan_f1a_h;
logic                                        txfma0_res_neg_int_nan_f2a_h;
logic                                        txfma0_res_neg_int_nan_f3a_h;
logic                                        txfma0_res_neg_int_nan_f4a_h;
logic                                        txfma0_res_neg_int_nan_f5a_h;
logic                                        txfma0_fc_zero30_8_f0a_h;
logic                                        txfma0_fc_zero30_8_f1a_h;
logic                                        txfma0_fc_zero30_8_f2a_h;
logic                                        txfma0_cvt_fi_pos_ovf_f2a_h;
logic                                        txfma0_cvt_fi_pos_ovf_f3a_h;
logic                                        txfma0_cvt_fi_neg_ovf_f2a_h;
logic                                        txfma0_cvt_fi_neg_ovf_f3a_h;
logic                                        txfma0_cvt_fi_min_neg_f2a_h;
logic                                        txfma0_cvt_fi_min_neg_f3a_h;
logic                                        txfma0_cvt_fi_pos_ovf_f4a_h;
logic                                        txfma0_cvt_fi_neg_ovf_f4a_h;
logic                                        txfma0_cvt_fi_min_neg_f4a_h; 
logic                                        txfma0_cvt_fi_pos_ovf_f5a_h;
logic                                        txfma0_cvt_fi_neg_ovf_f5a_h;
logic                                        txfma0_cvt_fi_min_neg_f5a_h; 
logic                                        txfma0_cvt_fui_pos_ovf_f2a_h;
logic                                        txfma0_cvt_fui_pos_ovf_f3a_h;
logic                                        txfma0_cvt_fui_pos_ovf_f4a_h;
logic                                        txfma0_cvt_fui_pos_ovf_f5a_h;
logic                                        txfma0_res_uint_nan_f0a_h;
logic                                        txfma0_res_uint_nan_f1a_h;
logic                                        txfma0_res_uint_nan_f2a_h;
logic                                        txfma0_res_uint_nan_f3a_h;
logic                                        txfma0_res_uint_nan_f4a_h;
logic                                        txfma0_res_uint_nan_f5a_h;
logic                                        txfma0_res_zero_f0a_h;
logic                                        txfma0_res_zero_f1a_h;
logic                                        txfma0_res_zero_f2a_h;
logic                                        txfma0_res_zero_f3a_h;
logic                                        txfma0_res_zero_out_f3a_h;
logic                                        txfma0_res_zero_f4a_h;
logic                                        txfma0_res_zero_f5a_h;
logic                                        txfma0_res_one_f0a_h;
logic                                        txfma0_res_one_f1a_h;
logic                                        txfma0_res_one_f2a_h;
logic                                        txfma0_res_one_f3a_h;
logic                                        txfma0_res_one_f4a_h;
logic                                        txfma0_res_one_f5a_h;
logic                                        txfma0_ediff1_lt_31_f2a_h;
logic                                        txfma0_ediff1_lt_31_f3a_h;
logic                                        txfma0_ediff1_lt_31_f4a_h;
logic                                        txfma0_ediff1_lt_31_f5a_h;
logic                                        txfma0_fc_neg_f2a_h;
logic [`TXFMA_A3:0]                          txfma0_prodsum_f2a_h;
logic [`TXFMA_A3:0]                          txfma0_prodsum_f3a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_ediff_ec_eph_f1a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_ediff_ec_epl_f1a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_ediff_eph_epl_f1a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_ediff_ec_eph_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_ediff_ec_epl_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_ediff_eph_epl_f2a_h;
logic [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]          txfma0_align1_mux_sel_f2a_h;
logic [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]          txfma0_align2_mux_sel_f2a_h;
logic [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]          txfma0_opd3_mux_sel_f2a_h;
logic [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]          txfma0_align1_mux_sel_f3a_h;
logic [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]          txfma0_align2_mux_sel_f3a_h;
logic [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]          txfma0_opd3_mux_sel_f3a_h;
logic [`TXFMA_MINMAX_OPD_SEL_SZ-1:0]         txfma0_int_minmax_mux_sel_f3a_h;
logic [`TXFMA_MINMAX_OPD_SEL_SZ-1:0]         txfma0_int_minmax_mux_sel_f4a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_ediff1_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_ediff2_f2a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_ediff1_f3a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_ediff2_f3a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]            txfma0_ediff1_abs_f3a_h;
logic [`TXFMA_EXP_SEXT_PH_SZ-1:0]            txfma0_ediff2_abs_f3a_h;
logic                                        txfma0_ediff1_zap_f2a_h;
logic                                        txfma0_ediff1_zap_f3a_h;
logic                                        txfma0_ediff1_zap_f4a_h;
logic                                        txfma0_align1_zap_f3a_h;
logic                                        txfma0_align2_zap_f3a_h;
logic                                        txfma0_align1_neg_f3a_h;
logic                                        txfma0_align2_neg_f3a_h;
logic                                        txfma0_op2_sxtto_g_f3a_h;
logic [31:0]                                 txfma0_int_minmax_res_f3a_h;
logic [31:0]                                 txfma0_int_minmax_res_f4a_h;
logic [`TXFMA_A3:`TXFMA_B47]                 txfma0_comb_add_f3a_h;
logic [`TXFMA_A3:`TXFMA_B47]                 txfma0_comb_add_f4a_h;
logic                                        txfma0_aligned_48_int_r_f3a_h;
logic                                        txfma0_aligned_48_int_l_f3a_h;
logic                                        txfma0_neg_norm_f4a_h;
logic [`TXFMA_MERGE_MUX_SEL_SZ-1:0]          txfma0_mergd_mux_sel_f4a_h;
logic                                        txfma0_comp_res_sel_f3a_h;
logic                                        txfma0_comp_res_sel_f4a_h; 
logic                                        txfma0_comp_op_true_f4a_h;
logic                                        txfma0_comp_op_true_f5a_h;
logic                                        txfma0_comp_op_true_f6a_h;
logic                                        txfma0_fa_eq_fc_res_check_f3a_h;
logic                                        txfma0_fa_eq_fc_res_check_f4a_h;
logic                                        txfma0_fa_hi_zero_f0a_h;
logic                                        txfma0_fa_hi_zero_f1a_h;
logic                                        txfma0_fb_hi_zero_f0a_h;
logic                                        txfma0_fb_hi_zero_f1a_h;
logic                                        txfma0_fb_hi_zero_f2a_h;
logic                                        txfma0_fb_hi_zero_f3a_h;
logic                                        txfma0_fb_hi_zero_f4a_h;
logic                                        txfma0_trans_exp_fma2_exc_f0a_h;
logic                                        txfma0_trans_exp_fma2_exc_f1a_h;
logic                                        txfma0_trans_exp_fma2_exc_f2a_h;
logic                                        txfma0_trans_taylor_f0a_h;
logic [2:0]                                  txfma0_sin_res_shf_idx_f0a_h;
logic [2:0]                                  txfma0_sin_res_shf_idx_f1a_h;
logic [2:0]                                  txfma0_sin_res_shf_idx_f2a_h;
logic [2:0]                                  txfma0_sin_res_shf_idx_f3a_h;
logic [2:0]                                  txfma0_sin_res_shf_idx_f4a_h;
logic                                        txfma0_res_unorm_one_f0a_h;
logic                                        txfma0_res_unorm_one_f1a_h;
logic                                        txfma0_res_unorm_one_f2a_h;
logic                                        txfma0_res_unorm_one_f3a_h;
logic                                        txfma0_res_unorm_one_f4a_h;
logic                                        txfma0_res_unorm_one_f5a_h;
logic                                        txfma0_res_snorm_one_f0a_h;
logic                                        txfma0_res_snorm_one_f1a_h;
logic                                        txfma0_res_snorm_one_f2a_h;
logic                                        txfma0_res_snorm_one_f3a_h;
logic                                        txfma0_res_snorm_one_f4a_h;
logic                                        txfma0_res_snorm_one_f5a_h;
logic                                        txfma0_norm_f32_ovf_f1a_h;
logic                                        txfma0_norm_one_out_f1a_h;
logic                                        txfma0_norm_f32_ovf_f2a_h;
logic                                        txfma0_norm_one_out_f2a_h;
logic                                        txfma0_norm_f32_ovf_f3a_h;
logic                                        txfma0_norm_one_out_f3a_h;
logic                                        txfma0_norm_f32_ovf_f4a_h;
logic                                        txfma0_norm_one_out_f4a_h;
logic                                        txfma0_norm_f32_ovf_f5a_h;
logic                                        txfma0_norm_one_out_f5a_h;
logic                                        txfma0_fa_en_f1a_h;
logic                                        txfma0_fa_en_f2a_h;
logic                                        txfma0_flags_en_f0a_h;
logic                                        txfma0_flags_en_f1a_h;
logic                                        txfma0_flags_en_f2a_h;
logic                                        txfma0_flags_en_f3a_h;
logic                                        txfma0_flags_en_f4a_h;
logic                                        txfma0_flags_en_f5a_h;
logic                                        txfma0_spec_res_en_f0a_h;
logic                                        txfma0_spec_res_en_f1a_h;
logic                                        txfma0_spec_res_en_f2a_h;
logic                                        txfma0_spec_res_en_f3a_h;
logic                                        txfma0_spec_res_en_f4a_h;
logic                                        txfma0_spec_res_en_f5a_h;
vpu_output_t                                 txfma0_f6_out_data;
logic  [`VPU_DATA_S_SZ-1:0]                  txfma0_fma_data_f5a_h;
logic  [`VPU_FLAGS_SZ-1:0]                   txfma0_fma_flags_f5a_h;
logic                                        txfma0_is_trans_rfa_h;
logic                                        txfma0_opa_valid_rfa_h;
logic                                        txfma0_opb_valid_rfa_h;
logic                                        txfma0_opc_valid_rfa_h;
logic                                        txfma0_opa_trans_valid_rfa_h;
logic                                        txfma0_opb_trans_valid_rfa_h;
logic                                        txfma0_opc_trans_valid_rfa_h;
logic                                        txfma0_cvt_norm_trans_valid_rfa_h;
logic                                        txfma0_sel_gmaxu24_f5a_h;
logic                                        txfma0_sel_gmaxu16_f5a_h;
logic                                        txfma0_sel_gmaxu10_f5a_h;
logic                                        txfma0_sel_gmaxu8_f5a_h;
logic                                        txfma0_sel_gmaxu2_f5a_h;
logic                                        txfma0_sel_gminany_f5a_h;
logic                                        txfma0_sel_gmaxs16_f5a_h;
logic                                        txfma0_sel_gmins16_f5a_h;
logic                                        txfma0_sel_gmaxs8_f5a_h;
logic                                        txfma0_sel_gmins8_f5a_h;
logic                                        txfma0_cmp_invalid_f3a_h;
logic                                        txfma0_cmp_invalid_f4a_h;
logic                                        txfma0_cmp_invalid_f5a_h;
logic                                        txfma0_fround_sticky_until_l_f3a_h;
logic                                        txfma0_fround_sticky_until_l_f4a_h;


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// F0 Stage
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

  // operands trans valid enable
assign txfma0_opa_trans_valid_rfa_h = in_data.sigs.trans | (in_data.sigs.cmd==`VPU_TRANS_EXP_FRAC) 
                                    | (in_data.sigs.cmd==`VPU_TRANS_SIN_P2) | (in_data.sigs.cmd==`VPU_FCMD_NR2_FRCPFXP);
assign txfma0_opb_trans_valid_rfa_h = (in_data.sigs.trans & (in_data.sigs.cmd!=`VPU_TRANS_SIN_TRANSFORM));
assign txfma0_opc_trans_valid_rfa_h = in_data.sigs.trans | in_data.sigs.rom | (in_data.sigs.cmd==`VPU_TRANS_SIN_P2) 
                                    | (in_data.sigs.cmd==`VPU_TRANS_SIN_TRANSFORM) | (in_data.sigs.cmd==`VPU_FCMD_NR2_FRCPFXP);

assign txfma0_cvt_norm_trans_valid_rfa_h = (in_data.sigs.cmd == `VPU_FCMD_CVT_UN24F32) |
                                           (in_data.sigs.cmd == `VPU_FCMD_CVT_UN16F32) |
                                           (in_data.sigs.cmd == `VPU_FCMD_CVT_UN10F32) |
                                           (in_data.sigs.cmd == `VPU_FCMD_CVT_UN8F32) |
                                           (in_data.sigs.cmd == `VPU_FCMD_CVT_UN2F32) |
                                           (in_data.sigs.cmd == `VPU_FCMD_CVT_SN16F32) |
                                           (in_data.sigs.cmd == `VPU_FCMD_CVT_SN8F32);                                    

  // operands FF enable
assign txfma0_opa_valid_rfa_h = in_valid & (in_data.sigs.ren1 | txfma0_opa_trans_valid_rfa_h);
assign txfma0_opb_valid_rfa_h = in_valid & (in_data.sigs.ren2 | txfma0_opb_trans_valid_rfa_h | txfma0_cvt_norm_trans_valid_rfa_h);
assign txfma0_opc_valid_rfa_h = in_valid & (in_data.sigs.ren3 | txfma0_opc_trans_valid_rfa_h);

  // trans enable
assign txfma0_is_trans_rfa_h = (in_valid & in_data.sigs.trans);

rst_en_ff 
#(
  .width( $bits(in_data.in1))
)                        
F0A_FF
(
  .clock      ( clock                   ), 
  .reset      ( reset                   ), 
  .en         ( txfma0_opa_valid_rfa_h  ),
  .D          ( in_data.in1             ),
  .Q          ( txfma0_opa_f0a_h        )
);

rst_en_ff 
#(
  .width( $bits(in_data.in2))
)                        
F0B_FF
(
  .clock      ( clock                   ), 
  .reset      ( reset                   ), 
  .en         ( txfma0_opb_valid_rfa_h  ),
  .D          ( in_data.in2             ),
  .Q          ( txfma0_opb_f0a_h        )
);

rst_en_ff 
#(
  .width( $bits(in_data.in3))
)                        
F0C_FF
(
  .clock      ( clock                   ), 
  .reset      ( reset                   ), 
  .en         ( txfma0_opc_valid_rfa_h  ),
  .D          ( in_data.in3             ),
  .Q          ( txfma0_opc_f0a_h        )
);

en_ff 
#(
  .width(`TRANS_C0_MAX_SIZE+`TRANS_C2_MAX_SIZE +`TRANS_MASK_SIZE+1+1)
)                        
F0_TRANS_FF
(
  .clock      ( clock                             ), 
  .en         ( txfma0_is_trans_rfa_h             ),
  .D          ( {trans_coefficients.c0,
                 trans_coefficients.c2,
                 trans_coefficients.mask,
                 trans_coefficients.taylor,
                 trans_coefficients.exc}     ),
  .Q          ( {txfma0_trans_c0_c1_f0a_h,
                 txfma0_trans_c2_f0a_h,
                 txfma0_trans_mask_f0a_h,
                 txfma0_trans_taylor_f0a_h,
                 txfma0_trans_exp_fma2_exc_f0a_h} )
);

en_ff 
#(
  .width($bits(in_data.use_prev_sigs))
)                        
F0_PREV_CTRL_FF
(
  .clock      ( clock                      ), 
  .en         ( in_valid                   ),
  .D          ( in_data.use_prev_sigs      ),
  .Q          ( txfma0_use_prev_sigs_f0a_h )
);


en_ff 
#(
  .width($bits( {in_data.sigs, 
                 in_data.rm, 
                 in_data.sigs.cmd, 
                 in_data.sigs.dtype} ))
)                        
F0_CTRL_FF
(
  .clock      ( clock                    ), 
  .en         ( in_valid & 
                !in_data.use_prev_sigs   ),
  .D          ( {in_data.sigs, 
                 in_data.rm, 
                 in_data.sigs.cmd, 
                 in_data.sigs.dtype }    ),  
  .Q          ( {txfma0_ctrl_sigs_f0a_h, 
                 txfma0_rm_f0a_h, 
                 txfma0_cmd_f0a_h, 
                 txfma0_op_dtype_f0a_h}  )
);

rst_ff 
#(
  .width( $bits({in_valid,
                 txfma0_opa_valid_rfa_h,
                 txfma0_opb_valid_rfa_h,
                 txfma0_opc_valid_rfa_h}) )
)                        
F0_CTRL_EN_FF
(
  .clock      ( clock                    ), 
  .reset      ( reset                    ), 
  .D          ( {in_valid,
                 txfma0_opa_valid_rfa_h,
                 txfma0_opb_valid_rfa_h,
                 txfma0_opc_valid_rfa_h} ),
  .Q          ( {txfma0_ctrl_valid_f0a_h,
                 txfma0_opa_valid_f0a_h,
                 txfma0_opb_valid_f0a_h,
                 txfma0_opc_valid_f0a_h} )
);


///////////////////////////////////////////////////////////////////////////////
// C0 component
///////////////////////////////////////////////////////////////////////////////
txfma_c0 txfma0_c0 
(
  .op_dtype_f0a_h        ( txfma0_op_dtype_f0a_h             ),
  .ctrl_valid_f0a_h      ( txfma0_ctrl_valid_f0a_h           ),
  .vpu_ctrl_ren1_f0a_h   ( txfma0_ctrl_sigs_f0a_h.ren1       ),
  .vpu_ctrl_ren2_f0a_h   ( txfma0_ctrl_sigs_f0a_h.ren2       ),
  .vpu_ctrl_ren3_f0a_h   ( txfma0_ctrl_sigs_f0a_h.ren3       ),
  .vpu_ctrl_fma_f0a_h    ( txfma0_ctrl_sigs_f0a_h.fma        ),
  .vpu_ctrl_fms_f0a_h    ( txfma0_ctrl_sigs_f0a_h.fms        ),
  .vpu_ctrl_add_f0a_h    ( txfma0_ctrl_sigs_f0a_h.add        ),
  .vpu_ctrl_sub_f0a_h    ( txfma0_ctrl_sigs_f0a_h.sub        ),
  .vpu_ctrl_mul_f0a_h    ( txfma0_ctrl_sigs_f0a_h.mul        ),
  .vpu_ctrl_cvt_f0a_h    ( txfma0_ctrl_sigs_f0a_h.cvt        ),
  .vpu_ctrl_gcvt_f0a_h   ( txfma0_ctrl_sigs_f0a_h.gcvt       ),
  .vpu_ctrl_trans_f0a_h  ( txfma0_ctrl_sigs_f0a_h.trans      ),
  .vpu_ctrl_rom_f0a_h    ( txfma0_ctrl_sigs_f0a_h.rom        ),
  .vpu_ctrl_cmd_f0a_h    ( txfma0_ctrl_sigs_f0a_h.cmd        ),
  .trans_taylor          ( txfma0_trans_taylor_f0a_h         ),
  .txfma_sigs_f0a_h      ( txfma0_sigs_f0a_h                 ),
  .trans_fma1_mask       ( txfma0_trans_mask_f0a_h[5]        ),
  .trans_fma2_mask       ( txfma0_trans_mask_f0a_h[6:2]      ),
  .op_dtype_out_f0a_h    ( txfma0_op_dtype_out_f0a_h         ),
  .sin_res_shf_idx_f0a_h ( txfma0_sin_res_shf_idx_f0a_h      )
);


///////////////////////////////////////////////////////////////////////////////
// txfma_f0: INMUX for mantissa and exponent datapaths & special detect logic
///////////////////////////////////////////////////////////////////////////////
txfma_f0 txfma0_f0
(
  .opa_f0a_h                      ( txfma0_opa_f0a_h                ),
  .opb_f0a_h                      ( txfma0_opb_f0a_h                ),
  .opc_f0a_h                      ( txfma0_opc_f0a_h                ),
  .sigs_f0a_h                     ( txfma0_sigs_f0a_h               ),
  .op_dtype_f0a_h                 ( txfma0_op_dtype_f0a_h           ),
  .cmd_f0a_h                      ( txfma0_cmd_f0a_h                ),
  .trans_c0_c1_f0a_h              ( txfma0_trans_c0_c1_f0a_h        ),
  .trans_c2_f0a_h                 ( txfma0_trans_c2_f0a_h           ),
  .trans_mask_f0a_h               ( txfma0_trans_mask_f0a_h         ),
  .trans_taylor_f0a_h             ( txfma0_trans_taylor_f0a_h       ),
  .trans_exp_fma2_exc_f0a_h       ( txfma0_trans_exp_fma2_exc_f0a_h ),
  .sa_hi_f0a_h                    ( txfma0_sa_hi_f0a_h              ),
  .sa_lo_f0a_h                    ( txfma0_sa_lo_f0a_h              ),
  .sb_hi_f0a_h                    ( txfma0_sb_hi_f0a_h              ),
  .sb_lo_f0a_h                    ( txfma0_sb_lo_f0a_h              ),
  .sc_f0a_h                       ( txfma0_sc_f0a_h              	),
  .ea_hi_f0a_h                    ( txfma0_ea_hi_f0a_h              ),
  .ea_lo_f0a_h                    ( txfma0_ea_lo_f0a_h              ),
  .eb_hi_f0a_h                    ( txfma0_eb_hi_f0a_h              ),
  .eb_lo_f0a_h                    ( txfma0_eb_lo_f0a_h              ),
  .ec_f0a_h                       ( txfma0_ec_f0a_h              	),
  .fa_f0a_h                       ( txfma0_fa_f0a_h                 ),
  .fb_f0a_h                       ( txfma0_fb_f0a_h                 ),
  .fc_f0a_h                       ( txfma0_fc_f0a_h                 ),
  .fb_trans_f0a_h                 ( txfma0_fb_trans_f0a_h           ),
  .res_inf_f0a_h                  ( txfma0_res_inf_f0a_h         	),
  .res_snan_f0a_h                 ( txfma0_res_snan_f0a_h        	),
  .res_qnan_f0a_h                 ( txfma0_res_qnan_f0a_h        	),
  .classify_f0a_h                 ( txfma0_classify_f0a_h        	),
  .prod_inf_hi_f0a_h              ( txfma0_prod_inf_hi_f0a_h        ),
  .a_inf_f0a_h                    ( txfma0_a_inf_f0a_h           	),
  .c_inf_f0a_h                    ( txfma0_c_inf_f0a_h           	),
  .flags_f0a_h                    ( txfma0_flags_f0a_h           	),
  .a_is_qnan_hi_f0a_h             ( txfma0_a_is_qnan_hi_f0a_h       ),
  .c_is_qnan_f0a_h                ( txfma0_c_is_qnan_f0a_h       	),
  .a_is_snan_hi_f0a_h             ( txfma0_a_is_snan_hi_f0a_h       ),
  .c_is_snan_f0a_h                ( txfma0_c_is_snan_f0a_h       	),
  .res_pos_int_nan_f0a_h          ( txfma0_res_pos_int_nan_f0a_h 	),
  .res_neg_int_nan_f0a_h          ( txfma0_res_neg_int_nan_f0a_h 	),
  .fc_zero30_8_f0a_h              ( txfma0_fc_zero30_8_f0a_h     	),
  .res_uint_nan_f0a_h             ( txfma0_res_uint_nan_f0a_h    	),
  .res_zero_f0a_h                 ( txfma0_res_zero_f0a_h        	),
  .res_one_f0a_h                  ( txfma0_res_one_f0a_h          	),
  .prod_inf_lo_f0a_h              ( txfma0_prod_inf_lo_f0a_h        ),
  .fa_hi_zero_f0a_h               ( txfma0_fa_hi_zero_f0a_h         ),
  .fb_hi_zero_f0a_h               ( txfma0_fb_hi_zero_f0a_h         ),
  .res_unorm_one_f0a_h            ( txfma0_res_unorm_one_f0a_h      ),  
  .res_snorm_one_f0a_h            ( txfma0_res_snorm_one_f0a_h      ),
  .flags_en_f0a_h                 ( txfma0_flags_en_f0a_h           ),
  .spec_res_en_f0a_h              ( txfma0_spec_res_en_f0a_h        ),
  .ea_hi_zero_clr_f0a_h           ( txfma0_ea_hi_zero_clr_f0a_h     ),
  .eb_hi_zero_clr_f0a_h           ( txfma0_eb_hi_zero_clr_f0a_h     ),
  .ec_zero_clr_f0a_h              ( txfma0_ec_zero_clr_f0a_h        ),
  .ea_lo_zero_clr_f0a_h           ( txfma0_ea_lo_zero_clr_f0a_h     ),
  .eb_lo_zero_clr_f0a_h           ( txfma0_eb_lo_zero_clr_f0a_h     )
);


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// F1 Stage
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// F1FF
///////////////////////////////////////////////////////////////////////////////

en_ff 
#(
  .width($bits( txfma0_fa_f0a_h ))
)                        
F1A_FF
(
  .clock      ( clock                   ), 
  .en         ( txfma0_ctrl_valid_f0a_h ),
  .D          ( txfma0_fa_f0a_h         ),
  .Q          ( txfma0_fa_f1a_h         )
);

en_ff 
#(
  .width($bits( {txfma0_fb_f0a_h,
                 txfma0_fb_trans_f0a_h,
                 txfma0_sin_res_shf_idx_f0a_h} ) )
)                        
F1B_FF
(
  .clock      ( clock                          ), 
  .en         ( txfma0_ctrl_valid_f0a_h        ),
  .D          ( {txfma0_fb_f0a_h,
                 txfma0_fb_trans_f0a_h,
                 txfma0_sin_res_shf_idx_f0a_h} ),
  .Q          ( {txfma0_fb_f1a_h,
                 txfma0_fb_raw_trans_f1a_h,  
                 txfma0_sin_res_shf_idx_f1a_h} )
);

en_ff 
#(
  .width($bits( txfma0_fc_f0a_h ))
)                        
F1C_FF
(
  .clock      ( clock                   ), 
  .en         ( txfma0_ctrl_valid_f0a_h ),
  .D          ( txfma0_fc_f0a_h         ),
  .Q          ( txfma0_fc_f1a_h         )
);

en_ff 
#(
  .width($bits( {txfma0_ea_lo_f0a_h,
                 txfma0_eb_lo_f0a_h,
                 txfma0_ea_hi_f0a_h} ))
)                        
E1A_FF
(
  .clock      ( clock                   ), 
  .en         ( txfma0_ctrl_valid_f0a_h ),
  .D          ( {txfma0_ea_lo_f0a_h,
                 txfma0_eb_lo_f0a_h,
                 txfma0_ea_hi_f0a_h}    ),
  .Q          ( {txfma0_ea_lo_f1a_h,
                 txfma0_eb_lo_f1a_h,
                 txfma0_ea_hi_f1a_h}    )
);

en_ff 
#(
  .width( $bits(txfma0_eb_hi_f0a_h) )
)                        
E1B_FF
(
  .clock      ( clock                   ), 
  .en         ( txfma0_ctrl_valid_f0a_h ),
  .D          ( txfma0_eb_hi_f0a_h      ),
  .Q          ( txfma0_eb_hi_f1a_h      )
);

en_ff 
#(
  .width( $bits(txfma0_ec_f0a_h) )
)                        
E1C_FF
(
  .clock      ( clock                   ), 
  .en         ( txfma0_ctrl_valid_f0a_h ),
  .D          ( txfma0_ec_f0a_h         ),
  .Q          ( txfma0_ec_f1a_h         )
);

en_ff 
#(
  .width($bits( {txfma0_sa_hi_f0a_h,
                 txfma0_sa_lo_f0a_h} ))
)                        
S1A_FF
(
  .clock      ( clock                   ), 
  .en         ( txfma0_ctrl_valid_f0a_h ),
  .D          ( {txfma0_sa_hi_f0a_h,
                 txfma0_sa_lo_f0a_h}    ),
  .Q          ( {txfma0_sa_hi_f1a_h,
                 txfma0_sa_lo_f1a_h}    )
);

en_ff 
#(
  .width($bits( {txfma0_sb_hi_f0a_h,
                 txfma0_sb_lo_f0a_h} ))
)                        
S1B_FF
(
  .clock      ( clock                   ), 
  .en         ( txfma0_ctrl_valid_f0a_h ),
  .D          ( {txfma0_sb_hi_f0a_h,
                 txfma0_sb_lo_f0a_h}    ),
  .Q          ( {txfma0_sb_hi_f1a_h,
                 txfma0_sb_lo_f1a_h}    )
);

en_ff 
#(
  .width($bits( txfma0_sc_f0a_h ))
)                        
S1C_FF
(
  .clock      ( clock                   ), 
  .en         ( txfma0_ctrl_valid_f0a_h ),
  .D          ( txfma0_sc_f0a_h         ),
  .Q          ( txfma0_sc_f1a_h         )
);

en_ff 
#(
  .width($bits( {txfma0_op_dtype_out_f0a_h, 
                 txfma0_cmd_f0a_h, 
                 txfma0_sigs_f0a_h, 
                 txfma0_rm_f0a_h,
                 txfma0_trans_exp_fma2_exc_f0a_h} ))
)                        
F1_CTRL_FF
(
  .clock      ( clock                              ),
  .en         ( txfma0_ctrl_valid_f0a_h & 
                !txfma0_use_prev_sigs_f0a_h        ),
  .D          ( {txfma0_op_dtype_out_f0a_h, 
                 txfma0_cmd_f0a_h, 
                 txfma0_sigs_f0a_h, 
                 txfma0_rm_f0a_h,
                 txfma0_trans_exp_fma2_exc_f0a_h } ),  
  .Q          ( {txfma0_op_dtype_f1a_h, 
                 txfma0_cmd_f1a_h, 
                 txfma0_sigs_f1a_h, 
                 txfma0_rm_f1a_h,
                 txfma0_trans_exp_fma2_exc_f1a_h}  )
);

en_ff 
#(
  .width( $bits(txfma0_use_prev_sigs_f0a_h ))
)                        
F1_PREV_CTRL_FF
(
  .clock      ( clock                      ), 
  .en         ( txfma0_ctrl_valid_f0a_h    ),
  .D          ( txfma0_use_prev_sigs_f0a_h ),
  .Q          ( txfma0_use_prev_sigs_f1a_h )
);

rst_ff 
#(
  .width( $bits(txfma0_ctrl_valid_f0a_h) )
)                        
F1_CTRL_EN_FF
(
  .clock      ( clock                    ), 
  .reset      ( reset                    ), 
  .D          ( txfma0_ctrl_valid_f0a_h  ),
  .Q          ( txfma0_ctrl_valid_f1a_h  )
);


en_ff 
#(
  .width($bits( {txfma0_ea_hi_zero_clr_f0a_h,
                 txfma0_eb_hi_zero_clr_f0a_h,
                 txfma0_ec_zero_clr_f0a_h, 
                 txfma0_ea_lo_zero_clr_f0a_h,
                 txfma0_eb_lo_zero_clr_f0a_h}))
)
F0_CLR_FF
(
  .clock      ( clock                         ),
  .en         ( txfma0_ctrl_valid_f0a_h       ),
  .D          ( {txfma0_ea_hi_zero_clr_f0a_h,
                 txfma0_eb_hi_zero_clr_f0a_h,
                 txfma0_ec_zero_clr_f0a_h, 
                 txfma0_ea_lo_zero_clr_f0a_h,
                 txfma0_eb_lo_zero_clr_f0a_h} ),
  .Q          ( {txfma0_ea_hi_zero_f1a_h,
                 txfma0_eb_hi_zero_f1a_h,
                 txfma0_ec_zero_f1a_h, 
                 txfma0_ea_lo_zero_f1a_h,
                 txfma0_eb_lo_zero_f1a_h}     )
);  


en_ff 
#(
  .width($bits( {txfma0_res_inf_f0a_h,
                 txfma0_res_snan_f0a_h,
                 txfma0_res_qnan_f0a_h,
                 txfma0_classify_f0a_h,
                 txfma0_prod_inf_hi_f0a_h,
                 txfma0_c_inf_f0a_h,
                 txfma0_a_inf_f0a_h,
                 txfma0_flags_f0a_h,
                 txfma0_a_is_qnan_hi_f0a_h,
                 txfma0_c_is_qnan_f0a_h,
                 txfma0_a_is_snan_hi_f0a_h,
                 txfma0_c_is_snan_f0a_h,
                 txfma0_res_pos_int_nan_f0a_h,
                 txfma0_res_neg_int_nan_f0a_h,
                 txfma0_fc_zero30_8_f0a_h,
                 txfma0_res_uint_nan_f0a_h,
                 txfma0_res_zero_f0a_h,
                 txfma0_res_one_f0a_h,
                 txfma0_prod_inf_lo_f0a_h,
                 txfma0_fa_hi_zero_f0a_h,
                 txfma0_fb_hi_zero_f0a_h,
                 txfma0_res_unorm_one_f0a_h,  
                 txfma0_res_snorm_one_f0a_h,
                 txfma0_flags_en_f0a_h,
                 txfma0_spec_res_en_f0a_h} ))
)
F1_SPEC_DET_FF
(
  .clock      ( clock                         ),
  .en         ( txfma0_ctrl_valid_f0a_h       ),
  .D          ( {txfma0_res_inf_f0a_h,
                 txfma0_res_snan_f0a_h,
                 txfma0_res_qnan_f0a_h,
                 txfma0_classify_f0a_h,
                 txfma0_prod_inf_hi_f0a_h,
                 txfma0_c_inf_f0a_h,
                 txfma0_a_inf_f0a_h,
                 txfma0_flags_f0a_h,
                 txfma0_a_is_qnan_hi_f0a_h,
                 txfma0_c_is_qnan_f0a_h,
                 txfma0_a_is_snan_hi_f0a_h,
                 txfma0_c_is_snan_f0a_h,
                 txfma0_res_pos_int_nan_f0a_h,
                 txfma0_res_neg_int_nan_f0a_h,
                 txfma0_fc_zero30_8_f0a_h,
                 txfma0_res_uint_nan_f0a_h,
                 txfma0_res_zero_f0a_h,
                 txfma0_res_one_f0a_h,
                 txfma0_prod_inf_lo_f0a_h,            
                 txfma0_fa_hi_zero_f0a_h,
                 txfma0_fb_hi_zero_f0a_h,
                 txfma0_res_unorm_one_f0a_h,  
                 txfma0_res_snorm_one_f0a_h,
                 txfma0_flags_en_f0a_h,
                 txfma0_spec_res_en_f0a_h}    ),
  .Q          ( {txfma0_res_inf_f1a_h,
                 txfma0_res_snan_f1a_h,
                 txfma0_res_qnan_f1a_h,
                 txfma0_classify_f1a_h,
                 txfma0_prod_inf_hi_f1a_h,
                 txfma0_c_inf_f1a_h,
                 txfma0_a_inf_f1a_h,
                 txfma0_flags_f1a_h,
                 txfma0_a_is_qnan_hi_f1a_h,
                 txfma0_c_is_qnan_f1a_h,
                 txfma0_a_is_snan_hi_f1a_h,
                 txfma0_c_is_snan_f1a_h,
                 txfma0_res_pos_int_nan_f1a_h,
                 txfma0_res_neg_int_nan_f1a_h,
                 txfma0_fc_zero30_8_f1a_h,
                 txfma0_res_uint_nan_f1a_h,
                 txfma0_res_zero_f1a_h,
                 txfma0_res_one_f1a_h,
                 txfma0_prod_inf_lo_f1a_h,                 
                 txfma0_fa_hi_zero_f1a_h,
                 txfma0_fb_hi_zero_f1a_h,
                 txfma0_res_unorm_one_f1a_h,  
                 txfma0_res_snorm_one_f1a_h,
                 txfma0_flags_en_f1a_h,
                 txfma0_spec_res_en_f1a_h}    )
);

///////////////////////////////////////////////////////////////////////////////
// F1 component
///////////////////////////////////////////////////////////////////////////////
txfma_f1 txfma0_f1 
(
  .op_dtype_f1a_h           ( txfma0_op_dtype_f1a_h           ),
  .mul_sign_f1a_h           ( txfma0_mul_sign_f1a_h           ),
  .fa_f1a_h                 ( txfma0_fa_f1a_h                 ),
  .fb_f1a_h                 ( txfma0_fb_f1a_h                 ),
  .fc_f1a_h                 ( txfma0_fc_f1a_h                 ),
  .wallace_fC21_f1a_h       ( txfma0_wallace_fC21_f1a_h       ),
  .wallace_fC22_f1a_h       ( txfma0_wallace_fC22_f1a_h       ),
  .wallace_fS21_f1a_h       ( txfma0_wallace_fS21_f1a_h       ),
  .wallace_fS22_f1a_h       ( txfma0_wallace_fS22_f1a_h       ),  
  .booth_fCin_f1a_h         ( txfma0_booth_fCin_f1a_h         ),
  .booth_fSxt_f1a_h         ( txfma0_booth_fSxt_f1a_h         ),
  .booth_fpp17_f1a_h        ( txfma0_booth_fpp17_f1a_h        ),
  .etrz_fa_hi_ph_f1a_h      ( txfma0_etrz_fa_hi_ph_f1a_h      ),
  .etrz_fa_lo_ps_f1a_h      ( txfma0_etrz_fa_lo_ps_f1a_h      ),
  .etrz_fb_hi_ph_f1a_h      ( txfma0_etrz_fb_hi_ph_f1a_h      ),
  .etrz_fb_lo_ps_f1a_h      ( txfma0_etrz_fb_lo_ps_f1a_h      ),      
  .etrz_fc_lo_ps_f1a_h      ( txfma0_etrz_fc_lo_ps_f1a_h      )
);



///////////////////////////////////////////////////////////////////////////////
// E1 component
///////////////////////////////////////////////////////////////////////////////
txfma_e1 txfma0_e1 
(
  .ea_hi_f1a_h           ( txfma0_ea_hi_f1a_h           ),
  .eb_hi_f1a_h           ( txfma0_eb_hi_f1a_h           ),
  .ec_f1a_h              ( txfma0_ec_f1a_h              ),
  .ea_lo_f1a_h           ( txfma0_ea_lo_f1a_h           ),
  .eb_lo_f1a_h           ( txfma0_eb_lo_f1a_h           ),
  .ea_hi_zero_f1a_h      ( txfma0_ea_hi_zero_f1a_h      ),
  .op_dtype_f1a_h        ( txfma0_op_dtype_f1a_h        ),
  .sigs_f1a_h            ( txfma0_sigs_f1a_h            ),
  .cmd_f1a_h             ( txfma0_cmd_f1a_h             ),
  .etrz_fa_hi_ph_f1a_h   ( txfma0_etrz_fa_hi_ph_f1a_h   ),
  .etrz_fa_lo_ps_f1a_h   ( txfma0_etrz_fa_lo_ps_f1a_h   ),
  .etrz_fb_hi_ph_f1a_h   ( txfma0_etrz_fb_hi_ph_f1a_h   ),
  .etrz_fb_lo_ps_f1a_h   ( txfma0_etrz_fb_lo_ps_f1a_h   ),      
  .fc_zero30_8_f1a_h     ( txfma0_fc_zero30_8_f1a_h     ),
  .ediff_ec_eph_f1a_h    ( txfma0_ediff_ec_eph_f1a_h    ),
  .ediff_ec_epl_f1a_h    ( txfma0_ediff_ec_epl_f1a_h    ),
  .ediff_eph_epl_f1a_h   ( txfma0_ediff_eph_epl_f1a_h   ),
  .eprod_hi_f1a_h        ( txfma0_eprod_hi_f1a_h        ),
  .eprod_lo_f1a_h        ( txfma0_eprod_lo_f1a_h        ),
  .etrz_prod_hi_ph_f1a_h ( txfma0_etrz_prod_hi_ph_f1a_h ),
  .etrz_prod_lo_ps_f1a_h ( txfma0_etrz_prod_lo_ps_f1a_h ),
  .ec_out_f1a_h          ( txfma0_ec_out_f1a_h          )
);


///////////////////////////////////////////////////////////////////////////////
// C1 component
///////////////////////////////////////////////////////////////////////////////
txfma_c1 txfma0_c1 
(
  .sigs_f1a_h          ( txfma0_sigs_f1a_h              ),
  .op_dtype_f1a_h      ( txfma0_op_dtype_f1a_h          ),
  .cmd_f1a_h           ( txfma0_cmd_f1a_h               ),
  .ea_hi_f1a_h         ( txfma0_ea_hi_f1a_h             ),
  .fa_hi_zero_f1a_h    ( txfma0_fa_hi_zero_f1a_h        ),
  .fb_hi_zero_f1a_h    ( txfma0_fb_hi_zero_f1a_h        ),
  .sa_hi_f1a_h         ( txfma0_sa_hi_f1a_h             ),
  .sb_hi_f1a_h         ( txfma0_sb_hi_f1a_h             ),
  .sa_lo_f1a_h         ( txfma0_sa_lo_f1a_h             ),
  .sb_lo_f1a_h         ( txfma0_sb_lo_f1a_h             ),
  .ea_hi_zero_f1a_h    ( txfma0_ea_hi_zero_f1a_h        ),
  .eb_hi_zero_f1a_h    ( txfma0_eb_hi_zero_f1a_h        ),
  .ea_lo_zero_f1a_h    ( txfma0_ea_lo_zero_f1a_h        ),
  .eb_lo_zero_f1a_h    ( txfma0_eb_lo_zero_f1a_h        ),
  .trans_taylor        ( txfma0_trans_taylor_f0a_h      ),
  .prod_sign_hi_f1a_h  ( txfma0_prod_sign_hi_f1a_h      ),
  .prod_sign_lo_f1a_h  ( txfma0_prod_sign_lo_f1a_h      ),
  .mul_sign_f1a_h      ( txfma0_mul_sign_f1a_h          ),
  .eprod_hi_zero_f1a_h ( txfma0_eprod_hi_zero_f1a_h     ),
  .eprod_lo_zero_f1a_h ( txfma0_eprod_lo_zero_f1a_h     ),
  .norm_f32_ovf_f1a_h  ( txfma0_norm_f32_ovf_f1a_h      ),
  .norm_one_out_f1a_h  ( txfma0_norm_one_out_f1a_h      ),
  .fa_en_f1a_h         ( txfma0_fa_en_f1a_h             )
);


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// F2 Stage
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// F2FF
///////////////////////////////////////////////////////////////////////////////

en_ff 
#(
  .width($bits( {txfma0_booth_fCin_f1a_h, 
                 txfma0_booth_fSxt_f1a_h, 
                 txfma0_booth_fpp17_f1a_h, 
                 txfma0_wallace_fC21_f1a_h, 
                 txfma0_wallace_fC22_f1a_h, 
                 txfma0_wallace_fS21_f1a_h, 
                 txfma0_wallace_fS22_f1a_h} ))
)                        
F2_FF
(
  .clock      ( clock                       ), 
  .en         ( txfma0_ctrl_valid_f1a_h     ),
  .D          ( {txfma0_booth_fCin_f1a_h, 
                 txfma0_booth_fSxt_f1a_h, 
                 txfma0_booth_fpp17_f1a_h, 
                 txfma0_wallace_fC21_f1a_h, 
                 txfma0_wallace_fC22_f1a_h, 
                 txfma0_wallace_fS21_f1a_h, 
                 txfma0_wallace_fS22_f1a_h} ),
  .Q          ( {txfma0_booth_fCin_f2a_h, 
                 txfma0_booth_fSxt_f2a_h, 
                 txfma0_booth_fpp17_f2a_h, 
                 txfma0_wallace_fC21_f2a_h, 
                 txfma0_wallace_fC22_f2a_h, 
                 txfma0_wallace_fS21_f2a_h, 
                 txfma0_wallace_fS22_f2a_h} )
);

en_ff 
#(
  .width( $bits( txfma0_fa_f1a_h )      )
)                        
F2_FA_FF
(
  .clock      ( clock                     ), 
  .en         ( txfma0_ctrl_valid_f1a_h &
                txfma0_fa_en_f1a_h        ),
  .D          ( txfma0_fa_f1a_h           ),
  .Q          ( txfma0_fa_f2a_h           )
);

en_ff 
#(
  .width( $bits( txfma0_eb_hi_f1a_h[0] ) )
)                        
F2_FB_FF
(
  .clock      ( clock                     ), 
  .en         ( txfma0_ctrl_valid_f1a_h &
                txfma0_fa_en_f1a_h        ),
  .D          ( txfma0_eb_hi_f1a_h[0]     ),
  .Q          ( txfma0_eb_hi_lsb_f2a_h    )
);


en_ff 
#(
  .width( $bits( {txfma0_fc_f1a_h,
                  txfma0_sin_res_shf_idx_f1a_h} ) )
)                        
F2_FC_FF
(
  .clock      ( clock                          ), 
  .en         ( txfma0_ctrl_valid_f1a_h        ),
  .D          ( {txfma0_fc_f1a_h,
                 txfma0_sin_res_shf_idx_f1a_h} ),
  .Q          ( {txfma0_fc_f2a_h,
                 txfma0_sin_res_shf_idx_f2a_h} )
);

en_ff 
#(
  .width( $bits(txfma0_use_prev_sigs_f0a_h ))
)                        
F2_FRAC_EN_FF
(
  .clock      ( clock                   ), 
  .en         ( txfma0_ctrl_valid_f1a_h ),
  .D          ( txfma0_fa_en_f1a_h      ),
  .Q          ( txfma0_fa_en_f2a_h      )
);


en_ff 
#(
  .width($bits( {txfma0_prod_sign_hi_f1a_h, 
                 txfma0_prod_sign_lo_f1a_h, 
                 txfma0_sc_f1a_h}        ))
)                        
S2_FF
(
  .clock      ( clock                       ),
  .en         ( txfma0_ctrl_valid_f1a_h     ),
  .D          ( {txfma0_prod_sign_hi_f1a_h, 
                 txfma0_prod_sign_lo_f1a_h, 
                 txfma0_sc_f1a_h}           ),
  .Q          ( {txfma0_prod_sign_hi_f2a_h, 
                 txfma0_prod_sign_lo_f2a_h, 
                 txfma0_sc_f2a_h}           )
);


en_ff 
#(
  .width($bits( {txfma0_ec_out_f1a_h,
                 txfma0_ea_hi_f1a_h,
                 txfma0_eb_hi_f1a_h,
                 txfma0_ediff_ec_eph_f1a_h,
                 txfma0_ediff_ec_epl_f1a_h,
                 txfma0_ediff_eph_epl_f1a_h,
                 txfma0_eprod_hi_f1a_h,
                 txfma0_eprod_lo_f1a_h,
                 txfma0_etrz_prod_hi_ph_f1a_h,
                 txfma0_etrz_prod_lo_ps_f1a_h,
                 txfma0_etrz_fa_lo_ps_f1a_h,
                 txfma0_etrz_fc_lo_ps_f1a_h}   ))
)                        
E2_FF
(
  .clock      ( clock                          ), 
  .en         ( txfma0_ctrl_valid_f1a_h        ),
  .D          ( {txfma0_ec_out_f1a_h,
                 txfma0_ea_hi_f1a_h,
                 txfma0_eb_hi_f1a_h,
                 txfma0_ediff_ec_eph_f1a_h,
                 txfma0_ediff_ec_epl_f1a_h,
                 txfma0_ediff_eph_epl_f1a_h,
                 txfma0_eprod_hi_f1a_h,
                 txfma0_eprod_lo_f1a_h,
                 txfma0_etrz_prod_hi_ph_f1a_h,
                 txfma0_etrz_prod_lo_ps_f1a_h,
                 txfma0_etrz_fa_lo_ps_f1a_h,
                 txfma0_etrz_fc_lo_ps_f1a_h}   ),
  .Q          ( {txfma0_ec_f2a_h,
                 txfma0_ea_hi_f2a_h,
                 txfma0_eb_hi_f2a_h,
                 txfma0_ediff_ec_eph_f2a_h,
                 txfma0_ediff_ec_epl_f2a_h,
                 txfma0_ediff_eph_epl_f2a_h,
                 txfma0_eprod_hi_f2a_h,
                 txfma0_eprod_lo_f2a_h,
                 txfma0_etrz_prod_hi_ph_f2a_h,
                 txfma0_etrz_prod_lo_ps_f2a_h,
                 txfma0_etrz_fa_lo_ps_f2a_h,
                 txfma0_etrz_fc_lo_ps_f2a_h}   )
);

en_ff 
#(
  .width($bits( {txfma0_op_dtype_f1a_h,
                 txfma0_cmd_f1a_h, 
                 txfma0_sigs_f1a_h, 
                 txfma0_rm_f1a_h,
                 txfma0_trans_exp_fma2_exc_f1a_h} ))
)                        
F2_CTRL_FF
(
  .clock      ( clock                             ), 
  .en         ( txfma0_ctrl_valid_f1a_h &
                !txfma0_use_prev_sigs_f1a_h ),
  .D          ( {txfma0_op_dtype_f1a_h,
                 txfma0_cmd_f1a_h, 
                 txfma0_sigs_f1a_h, 
                 txfma0_rm_f1a_h,
                 txfma0_trans_exp_fma2_exc_f1a_h} ),
  .Q          ( {txfma0_op_dtype_f2a_h, 
                 txfma0_cmd_f2a_h, 
                 txfma0_sigs_f2a_h, 
                 txfma0_rm_f2a_h,
                 txfma0_trans_exp_fma2_exc_f2a_h} )
);

en_ff 
#(
  .width( $bits(txfma0_use_prev_sigs_f1a_h ))
)                        
F2_PREV_CTRL_FF
(
  .clock      ( clock                      ), 
  .en         ( txfma0_ctrl_valid_f1a_h    ),
  .D          ( txfma0_use_prev_sigs_f1a_h ),
  .Q          ( txfma0_use_prev_sigs_f2a_h )
);



en_ff 
#(
  .width($bits( {txfma0_ec_zero_f1a_h,
                 txfma0_eprod_hi_zero_f1a_h,
                 txfma0_eprod_lo_zero_f1a_h,
                 txfma0_fc_zero30_8_f1a_h,
                 txfma0_res_zero_f1a_h,
                 txfma0_res_one_f1a_h,
                 txfma0_fb_hi_zero_f1a_h}) )
)
F2_ZER0_FF
(
  .clock      ( clock                        ), 
  .en         ( txfma0_ctrl_valid_f1a_h      ),
  .D          ( {txfma0_ec_zero_f1a_h,
                 txfma0_eprod_hi_zero_f1a_h,
                 txfma0_eprod_lo_zero_f1a_h,
                 txfma0_fc_zero30_8_f1a_h,
                 txfma0_res_zero_f1a_h,
                 txfma0_res_one_f1a_h,
                 txfma0_fb_hi_zero_f1a_h}    ),
  .Q          ( {txfma0_ec_zero_f2a_h, 
                 txfma0_eprod_hi_zero_f2a_h,
                 txfma0_eprod_lo_zero_f2a_h,
                 txfma0_fc_zero30_8_f2a_h,
                 txfma0_res_zero_f2a_h,
                 txfma0_res_one_f2a_h,
                 txfma0_fb_hi_zero_f2a_h}    )
);

en_ff 
#(
  .width($bits( {txfma0_res_inf_f1a_h, 
                 txfma0_res_snan_f1a_h, 
                 txfma0_res_qnan_f1a_h, 
                 txfma0_classify_f1a_h,
                 txfma0_res_unorm_one_f1a_h,
                 txfma0_res_snorm_one_f1a_h,
                 txfma0_res_pos_int_nan_f1a_h,
                 txfma0_res_neg_int_nan_f1a_h,
                 txfma0_res_uint_nan_f1a_h}) )
)
F2_SPEC_FF
(
  .clock      ( clock                          ), 
  .en         ( txfma0_ctrl_valid_f1a_h &
                txfma0_spec_res_en_f1a_h       ),
  .D          ( {txfma0_res_inf_f1a_h, 
                 txfma0_res_snan_f1a_h,  
                 txfma0_res_qnan_f1a_h, 
                 txfma0_classify_f1a_h, 
                 txfma0_res_unorm_one_f1a_h,
                 txfma0_res_snorm_one_f1a_h,
                 txfma0_res_pos_int_nan_f1a_h,
                 txfma0_res_neg_int_nan_f1a_h,
                 txfma0_res_uint_nan_f1a_h}    ),
  .Q          ( {txfma0_res_inf_f2a_h, 
                 txfma0_res_snan_f2a_h, 
                 txfma0_res_qnan_f2a_h, 
                 txfma0_classify_f2a_h, 
                 txfma0_res_unorm_one_f2a_h,
                 txfma0_res_snorm_one_f2a_h,                 
                 txfma0_res_pos_int_nan_f2a_h,
                 txfma0_res_neg_int_nan_f2a_h,
                 txfma0_res_uint_nan_f2a_h}    )
);


en_ff 
#(
  .width($bits( {txfma0_prod_inf_hi_f1a_h, 
                 txfma0_c_inf_f1a_h, 
                 txfma0_a_inf_f1a_h, 
                 txfma0_a_is_qnan_hi_f1a_h,
                 txfma0_c_is_qnan_f1a_h,
                 txfma0_a_is_snan_hi_f1a_h,
                 txfma0_c_is_snan_f1a_h,                 
                 txfma0_norm_f32_ovf_f1a_h,
                 txfma0_norm_one_out_f1a_h,
                 txfma0_prod_inf_lo_f1a_h}) )
)
F2_SPEC2_FF
(
  .clock      ( clock                       ), 
  .en         ( txfma0_ctrl_valid_f1a_h     ),
  .D          ( {txfma0_prod_inf_hi_f1a_h, 
                 txfma0_c_inf_f1a_h, 
                 txfma0_a_inf_f1a_h, 
                 txfma0_a_is_qnan_hi_f1a_h,
                 txfma0_c_is_qnan_f1a_h,
                 txfma0_a_is_snan_hi_f1a_h,
                 txfma0_c_is_snan_f1a_h,
                 txfma0_norm_f32_ovf_f1a_h,
                 txfma0_norm_one_out_f1a_h, 
                 txfma0_prod_inf_lo_f1a_h}  ),
  .Q          ( {txfma0_prod_inf_hi_f2a_h, 
                 txfma0_c_inf_f2a_h, 
                 txfma0_a_inf_f2a_h, 
                 txfma0_a_is_qnan_hi_f2a_h,
                 txfma0_c_is_qnan_f2a_h,
                 txfma0_a_is_snan_hi_f2a_h,
                 txfma0_c_is_snan_f2a_h,
                 txfma0_norm_f32_ovf_f2a_h,
                 txfma0_norm_one_out_f2a_h,
                 txfma0_prod_inf_lo_f2a_h}  )
);
  

en_ff 
#(
  .width($bits(txfma0_flags_f1a_h))
)                        
F2_FLG_FF
(
  .clock      ( clock                     ), 
  .en         ( txfma0_ctrl_valid_f1a_h &
                txfma0_flags_en_f1a_h     ),
  .D          ( txfma0_flags_f1a_h        ),
  .Q          ( txfma0_flags_f2a_h        )
);


rst_en_ff 
#(
  .width( $bits({txfma0_spec_res_en_f1a_h,
                 txfma0_flags_en_f1a_h}))
)                        
F2_SPEC_EN_FF
(
  .clock      ( clock                    ), 
  .reset      ( reset                    ), 
  .en         ( txfma0_ctrl_valid_f1a_h  ), 
  .D          ({txfma0_spec_res_en_f1a_h,
                txfma0_flags_en_f1a_h}   ),
  .Q          ({txfma0_spec_res_en_f2a_h,
                txfma0_flags_en_f2a_h}   )
);


rst_ff 
#(
  .width( $bits(txfma0_ctrl_valid_f1a_h) )
)                        
F2_CTRL_EN_FF
(
  .clock      ( clock                   ), 
  .reset      ( reset                   ), 
  .D          ( txfma0_ctrl_valid_f1a_h ),
  .Q          ( txfma0_ctrl_valid_f2a_h )
);


///////////////////////////////////////////////////////////////////////////////
// F2 component
///////////////////////////////////////////////////////////////////////////////
txfma_f2 txfma0_f2
(
  .fa_f2a_h           ( txfma0_fa_f2a_h           ),
  .fc_f2a_h           ( txfma0_fc_f2a_h           ),
  .cmd_f2a_h          ( txfma0_cmd_f2a_h          ),
  .booth_fCin_f2a_h   ( txfma0_booth_fCin_f2a_h   ),
  .booth_fSxt_f2a_h   ( txfma0_booth_fSxt_f2a_h   ),
  .booth_fpp17_f2a_h  ( txfma0_booth_fpp17_f2a_h  ),  
  .wallace_fC21_f2a_h ( txfma0_wallace_fC21_f2a_h ),
  .wallace_fC22_f2a_h ( txfma0_wallace_fC22_f2a_h ),
  .wallace_fS21_f2a_h ( txfma0_wallace_fS21_f2a_h ),
  .wallace_fS22_f2a_h ( txfma0_wallace_fS22_f2a_h ),  
  .use_product_f2a_h  ( txfma0_use_product_f2a_h  ),
  .fc_neg_f2a_h       ( txfma0_fc_neg_f2a_h       ),
  .prodsum_f2a_h      ( txfma0_prodsum_f2a_h      ),
  .fa_out_f2a_h       ( txfma0_fa_out_f2a_h       )
);



///////////////////////////////////////////////////////////////////////////////
// E2 component
///////////////////////////////////////////////////////////////////////////////
txfma_e2 txfma0_e2 
(
  .cmd_f2a_h              ( txfma0_cmd_f2a_h                ), 
  .op_dtype_f2a_h         ( txfma0_op_dtype_f2a_h           ),
  .sigs_f2a_h             ( txfma0_sigs_f2a_h               ),
  .ediff_ec_eph_f2a_h     ( txfma0_ediff_ec_eph_f2a_h       ),
  .ediff_ec_epl_f2a_h     ( txfma0_ediff_ec_epl_f2a_h       ),
  .ediff_eph_epl_f2a_h    ( txfma0_ediff_eph_epl_f2a_h      ),
  .eprod_hi_f2a_h         ( txfma0_eprod_hi_f2a_h           ),
  .eprod_lo_f2a_h         ( txfma0_eprod_lo_f2a_h           ),
  .ec_f2a_h               ( txfma0_ec_f2a_h                 ),
  .etrz_fa_lo_ps_f2a_h    ( txfma0_etrz_fa_lo_ps_f2a_h      ),
  .etrz_fc_lo_ps_f2a_h    ( txfma0_etrz_fc_lo_ps_f2a_h      ),
  .etrz_prod_hi_ph_f2a_h  ( txfma0_etrz_prod_hi_ph_f2a_h    ),
  .etrz_prod_lo_ps_f2a_h  ( txfma0_etrz_prod_lo_ps_f2a_h    ),
  .eprod_hi_zero_f2a_h    ( txfma0_eprod_hi_zero_f2a_h      ),
  .eprod_lo_zero_f2a_h    ( txfma0_eprod_lo_zero_f2a_h      ),
  .ec_zero_f2a_h          ( txfma0_ec_zero_f2a_h            ),
  .exp_fma2_exc_f2a_h     ( txfma0_trans_exp_fma2_exc_f2a_h ),
  .eb_hi_f2a_h            ( txfma0_eb_hi_f2a_h              ),
  .e_sticky1_f2a_h        ( txfma0_e_sticky1_f2a_h          ),
  .e_sticky2_f2a_h        ( txfma0_e_sticky2_f2a_h          ),
  .align1_mux_sel_f2a_h   ( txfma0_align1_mux_sel_f2a_h     ),
  .align2_mux_sel_f2a_h   ( txfma0_align2_mux_sel_f2a_h     ),
  .opd3_mux_sel_f2a_h     ( txfma0_opd3_mux_sel_f2a_h       ),
  .ediff1_f2a_h           ( txfma0_ediff1_f2a_h             ),
  .ediff2_f2a_h           ( txfma0_ediff2_f2a_h             ),
  .e_res_f2a_h            ( txfma0_e_res_f2a_h              ),
  .exp_frac_e_shift_f2a_h ( txfma0_exp_frac_e_shift_f2a_h   ),
  .ediff1_sel_f2a_h       ( txfma0_ediff1_sel_f2a_h         ),
  .ediff2_sel_f2a_h       ( txfma0_ediff2_sel_f2a_h         ),
  .ediff1_zap_f2a_h       ( txfma0_ediff1_zap_f2a_h         )

);

///////////////////////////////////////////////////////////////////////////////
// C2 component
///////////////////////////////////////////////////////////////////////////////
txfma_c2 txfma0_c2 
(
  .op_dtype_f2a_h        ( txfma0_op_dtype_f2a_h        ),
  .cmd_f2a_h             ( txfma0_cmd_f2a_h             ),
  .sigs_f2a_h            ( txfma0_sigs_f2a_h            ), 
  .ediff1_f2a_h          ( txfma0_ediff1_f2a_h          ),
  .sc_f2a_h              ( txfma0_sc_f2a_h              ),
  .prod_sign_hi_f2a_h    ( txfma0_prod_sign_hi_f2a_h    ),
  .prod_sign_lo_f2a_h    ( txfma0_prod_sign_lo_f2a_h    ),
  .use_product_f2a_h     ( txfma0_use_product_f2a_h     ),
  .ediff1_sel_f2a_h      ( txfma0_ediff1_sel_f2a_h      ),
  .ediff2_sel_f2a_h      ( txfma0_ediff2_sel_f2a_h      ),
  .eff_sub1_f2a_h        ( txfma0_eff_sub1_f2a_h        ),
  .eff_sub2_f2a_h        ( txfma0_eff_sub2_f2a_h        ),
  .comp_op_f2a_h         ( txfma0_comp_op_f2a_h         ),
  .minmax_op_f2a_h       ( txfma0_minmax_op_f2a_h       ),
  .fc_zero30_8_f2a_h     ( txfma0_fc_zero30_8_f2a_h     ),
  .cvt_fi_pos_ovf_f2a_h  ( txfma0_cvt_fi_pos_ovf_f2a_h  ), 
  .cvt_fui_pos_ovf_f2a_h ( txfma0_cvt_fui_pos_ovf_f2a_h ),   
  .cvt_fi_neg_ovf_f2a_h  ( txfma0_cvt_fi_neg_ovf_f2a_h  ),
  .cvt_fi_min_neg_f2a_h  ( txfma0_cvt_fi_min_neg_f2a_h  ),  
  .ediff1_lt_31_f2a_h    ( txfma0_ediff1_lt_31_f2a_h    ),
  .fc_neg_f2a_h          ( txfma0_fc_neg_f2a_h          ),
  .eff_sign1_f2a_h       ( txfma0_eff_sign1_f2a_h       )
);


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// F3 Stage
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// F3FF
///////////////////////////////////////////////////////////////////////////////
en_ff 
#(
  .width($bits( {txfma0_eff_sub1_f2a_h,
                 txfma0_eff_sub2_f2a_h,
                 txfma0_align1_mux_sel_f2a_h,
                 txfma0_align2_mux_sel_f2a_h,
                 txfma0_opd3_mux_sel_f2a_h,
                 txfma0_prodsum_f2a_h} ) )
)                        
F3_FF
(
  .clock      ( clock                         ), 
  .en         ( txfma0_ctrl_valid_f2a_h       ),
  .D          ( {txfma0_eff_sub1_f2a_h,
                 txfma0_eff_sub2_f2a_h,
                 txfma0_align1_mux_sel_f2a_h,
                 txfma0_align2_mux_sel_f2a_h,
                 txfma0_opd3_mux_sel_f2a_h,
                 txfma0_prodsum_f2a_h}        ),                 
  .Q          ( {txfma0_eff_sub1_f3a_h, 
                 txfma0_eff_sub2_f3a_h,
                 txfma0_align1_mux_sel_f3a_h,
                 txfma0_align2_mux_sel_f3a_h,
                 txfma0_opd3_mux_sel_f3a_h,
                 txfma0_prodsum_f3a_h}        ) 
);


en_ff 
#(
  .width( $bits(txfma0_fa_out_f2a_h[`TXFMA_A0:`TXFMA_B22]) )
)                        
F3_FA_FF
(
  .clock      ( clock                                      ), 
  .en         ( txfma0_ctrl_valid_f2a_h &
                txfma0_fa_en_f2a_h                         ),
  .D          ( txfma0_fa_out_f2a_h[`TXFMA_A0:`TXFMA_B22]  ),
  .Q          ( txfma0_fa_f3a_h                            )
);

en_ff 
#(
  .width( $bits( txfma0_eb_hi_lsb_f2a_h ) )
)                        
F3_FB_FF
(
  .clock      ( clock                      ), 
  .en         ( txfma0_ctrl_valid_f2a_h &
                txfma0_fa_en_f2a_h         ),
  .D          ( txfma0_eb_hi_lsb_f2a_h     ),
  .Q          ( txfma0_eb_hi_lsb_f3a_h     )
);

en_ff 
#(
  .width( $bits( {txfma0_fc_f2a_h,
                  txfma0_sin_res_shf_idx_f2a_h} ) )
)                        
F3_FC_FF
(
  .clock      ( clock                           ), 
  .en         ( txfma0_ctrl_valid_f2a_h         ),
  .D          ( {txfma0_fc_f2a_h,
                 txfma0_sin_res_shf_idx_f2a_h}  ),
  .Q          ( {txfma0_fc_f3a_h,
                 txfma0_sin_res_shf_idx_f3a_h}  )
);

en_ff 
#(
  .width($bits({txfma0_ediff1_f2a_h,
                txfma0_ediff2_f2a_h,
                txfma0_e_res_f2a_h, 
                txfma0_exp_frac_e_shift_f2a_h, 
                txfma0_e_sticky1_f2a_h,
                txfma0_e_sticky2_f2a_h,
                txfma0_ec_f2a_h,
                txfma0_ea_hi_f2a_h,
                txfma0_ediff1_zap_f2a_h
                }))
)                        
E3_FF
(
  .clock      ( clock                           ), 
  .en         ( txfma0_ctrl_valid_f2a_h         ),
  .D          ( {txfma0_ediff1_f2a_h,
                 txfma0_ediff2_f2a_h,
                 txfma0_e_res_f2a_h, 
                 txfma0_exp_frac_e_shift_f2a_h, 
                 txfma0_e_sticky1_f2a_h,
                 txfma0_e_sticky2_f2a_h,
                 txfma0_ec_f2a_h,
                 txfma0_ea_hi_f2a_h,
                 txfma0_ediff1_zap_f2a_h}       ),
  .Q          ( {txfma0_ediff1_f3a_h,
                 txfma0_ediff2_f3a_h,
                 txfma0_e_res_f3a_h, 
                 txfma0_exp_frac_e_shift_f3a_h, 
                 txfma0_e_sticky1_f3a_h,
                 txfma0_e_sticky2_f3a_h,
                 txfma0_ec_f3a_h,
                 txfma0_ea_hi_f3a_h,
                 txfma0_ediff1_zap_f3a_h}       )
);


en_ff 
#(
  .width($bits( {txfma0_sc_f2a_h,
                 txfma0_prod_sign_hi_f2a_h,
                 txfma0_prod_sign_lo_f2a_h,
                 txfma0_eff_sign1_f2a_h} ))
)                        
S3_FF
(
  .clock      ( clock                       ), 
  .en         ( txfma0_ctrl_valid_f2a_h     ),
  .D          ( {txfma0_sc_f2a_h,
                 txfma0_prod_sign_hi_f2a_h,
                 txfma0_prod_sign_lo_f2a_h,
                 txfma0_eff_sign1_f2a_h}    ),
  .Q          ( {txfma0_sc_f3a_h,
                 txfma0_prod_sign_hi_f3a_h,
                 txfma0_prod_sign_lo_f3a_h,
                 txfma0_eff_sign1_f3a_h}    )
);


en_ff 
#(
  .width($bits({txfma0_op_dtype_f2a_h, 
                txfma0_cmd_f2a_h, 
                txfma0_sigs_f2a_h, 
                txfma0_rm_f2a_h, 
                txfma0_comp_op_f2a_h, 
                txfma0_minmax_op_f2a_h}))
)                        
F3_CTRL_FF
(
  .clock      ( clock                       ), 
  .en         ( txfma0_ctrl_valid_f2a_h &
                !txfma0_use_prev_sigs_f2a_h ),
  .D          ( {txfma0_op_dtype_f2a_h, 
                 txfma0_cmd_f2a_h,
                 txfma0_sigs_f2a_h,
                 txfma0_rm_f2a_h,
                 txfma0_comp_op_f2a_h,
                 txfma0_minmax_op_f2a_h}    ),
  .Q          ( {txfma0_op_dtype_f3a_h, 
                 txfma0_cmd_f3a_h, 
                 txfma0_sigs_f3a_h, 
                 txfma0_rm_f3a_h, 
                 txfma0_comp_op_f3a_h, 
                 txfma0_minmax_op_f3a_h}    )
);

en_ff 
#(
  .width( $bits(txfma0_use_prev_sigs_f2a_h ))
)                        
F3_PREV_CTRL_FF
(
  .clock      ( clock                      ), 
  .en         ( txfma0_ctrl_valid_f2a_h    ),
  .D          ( txfma0_use_prev_sigs_f2a_h ),
  .Q          ( txfma0_use_prev_sigs_f3a_h )
);


en_ff 
#(
  .width($bits( {txfma0_ec_zero_f2a_h,
                 txfma0_res_zero_f2a_h,
                 txfma0_res_one_f2a_h,                 
                 txfma0_eprod_hi_zero_f2a_h,
                 txfma0_eprod_lo_zero_f2a_h,
                 txfma0_fb_hi_zero_f2a_h})   )
)
F3_ZER0_FF
(
  .clock      ( clock                        ), 
  .en         ( txfma0_ctrl_valid_f2a_h      ),
  .D          ( {txfma0_ec_zero_f2a_h,
                 txfma0_res_zero_f2a_h,
                 txfma0_res_one_f2a_h,                 
                 txfma0_eprod_hi_zero_f2a_h,
                 txfma0_eprod_lo_zero_f2a_h,
                 txfma0_fb_hi_zero_f2a_h}    ),
  .Q          ( {txfma0_ec_zero_f3a_h, 
                 txfma0_res_zero_f3a_h,
                 txfma0_res_one_f3a_h,                
                 txfma0_eprod_hi_zero_f3a_h,
                 txfma0_eprod_lo_zero_f3a_h,
                 txfma0_fb_hi_zero_f3a_h}    )
);


en_ff 
#(
  .width($bits({txfma0_res_inf_f2a_h,
                txfma0_res_snan_f2a_h, 
                txfma0_res_qnan_f2a_h,           
                txfma0_classify_f2a_h,
                txfma0_res_unorm_one_f2a_h,
                txfma0_res_snorm_one_f2a_h,                    
                txfma0_res_pos_int_nan_f2a_h,
                txfma0_res_neg_int_nan_f2a_h,
                txfma0_res_uint_nan_f2a_h}))
)                        
F3_SPEC_FF
(
  .clock      ( clock                          ), 
  .en         ( txfma0_ctrl_valid_f2a_h &
                txfma0_spec_res_en_f2a_h       ),
  .D          ( {txfma0_res_inf_f2a_h,
                 txfma0_res_snan_f2a_h, 
                 txfma0_res_qnan_f2a_h,
                 txfma0_classify_f2a_h,
                 txfma0_res_unorm_one_f2a_h,
                 txfma0_res_snorm_one_f2a_h,  
                 txfma0_res_pos_int_nan_f2a_h,
                 txfma0_res_neg_int_nan_f2a_h,
                 txfma0_res_uint_nan_f2a_h}    ),
  .Q          ( {txfma0_res_inf_f3a_h, 
                 txfma0_res_snan_f3a_h, 
                 txfma0_res_qnan_f3a_h,
                 txfma0_classify_f3a_h,
                 txfma0_res_unorm_one_f3a_h,
                 txfma0_res_snorm_one_f3a_h,                
                 txfma0_res_pos_int_nan_f3a_h,
                 txfma0_res_neg_int_nan_f3a_h,
                 txfma0_res_uint_nan_f3a_h}    )
);


en_ff 
#(
  .width($bits({txfma0_prod_inf_hi_f2a_h,
                txfma0_prod_inf_lo_f2a_h,
                txfma0_c_inf_f2a_h,
                txfma0_a_inf_f2a_h,
                txfma0_cvt_fui_pos_ovf_f2a_h,                
                txfma0_cvt_fi_pos_ovf_f2a_h,
                txfma0_cvt_fi_neg_ovf_f2a_h,
                txfma0_cvt_fi_min_neg_f2a_h,
                txfma0_ediff1_lt_31_f2a_h,
                txfma0_norm_f32_ovf_f2a_h,
                txfma0_norm_one_out_f2a_h, 
                txfma0_a_is_snan_hi_f2a_h,
                txfma0_c_is_snan_f2a_h,
                txfma0_a_is_qnan_hi_f2a_h,
                txfma0_c_is_qnan_f2a_h}))
)                        
F3_SPEC2_FF
(
  .clock      ( clock                          ), 
  .en         ( txfma0_ctrl_valid_f2a_h        ),
  .D          ( {txfma0_prod_inf_hi_f2a_h,
                 txfma0_prod_inf_lo_f2a_h,
                 txfma0_c_inf_f2a_h,
                 txfma0_a_inf_f2a_h,
                 txfma0_cvt_fui_pos_ovf_f2a_h,                 
                 txfma0_cvt_fi_pos_ovf_f2a_h,
                 txfma0_cvt_fi_neg_ovf_f2a_h,
                 txfma0_cvt_fi_min_neg_f2a_h,
                 txfma0_ediff1_lt_31_f2a_h,
                 txfma0_norm_f32_ovf_f2a_h,
                 txfma0_norm_one_out_f2a_h,  
                 txfma0_a_is_snan_hi_f2a_h,
                 txfma0_c_is_snan_f2a_h,                
                 txfma0_a_is_qnan_hi_f2a_h,
                 txfma0_c_is_qnan_f2a_h}       ),
  .Q          ( {txfma0_prod_inf_hi_f3a_h,
                 txfma0_prod_inf_lo_f3a_h,
                 txfma0_c_inf_f3a_h,
                 txfma0_a_inf_f3a_h,
                 txfma0_cvt_fui_pos_ovf_f3a_h,                 
                 txfma0_cvt_fi_pos_ovf_f3a_h,
                 txfma0_cvt_fi_neg_ovf_f3a_h,
                 txfma0_cvt_fi_min_neg_f3a_h,
                 txfma0_ediff1_lt_31_f3a_h,
                 txfma0_norm_f32_ovf_f3a_h,
                 txfma0_norm_one_out_f3a_h,
                 txfma0_a_is_snan_hi_f3a_h,
                 txfma0_c_is_snan_f3a_h,                 
                 txfma0_a_is_qnan_hi_f3a_h,
                 txfma0_c_is_qnan_f3a_h}       )
);


en_ff 
#(
  .width($bits(txfma0_flags_f2a_h))
)                        
F3_FLG_FF
(
  .clock      ( clock                     ), 
  .en         ( txfma0_ctrl_valid_f2a_h &
                txfma0_flags_en_f2a_h     ),
  .D          ( txfma0_flags_f2a_h        ),
  .Q          ( txfma0_flags_f3a_h        )
);

rst_en_ff 
#(
  .width( $bits(txfma0_flags_en_f1a_h) )
)                        
F3_FLG_EN_FF
(
  .clock      ( clock                   ), 
  .reset      ( reset                   ), 
  .en         ( txfma0_ctrl_valid_f2a_h ), 
  .D          ( txfma0_flags_en_f2a_h   ),
  .Q          ( txfma0_flags_en_f3a_h   )
);

rst_en_ff 
#(
  .width( $bits(txfma0_spec_res_en_f2a_h) )
)                        
F3_SPEC_EN_FF
(
  .clock      ( clock                    ), 
  .reset      ( reset                    ), 
  .en         ( txfma0_ctrl_valid_f2a_h  ), 
  .D          ( txfma0_spec_res_en_f2a_h ),
  .Q          ( txfma0_spec_res_en_f3a_h )
);


rst_ff 
#(
  .width( $bits(txfma0_ctrl_valid_f2a_h) )
)                        
F3_CTRL_EN_FF
(
  .clock      ( clock                   ), 
  .reset      ( reset                   ), 
  .D          ( txfma0_ctrl_valid_f2a_h ),
  .Q          ( txfma0_ctrl_valid_f3a_h )
);


///////////////////////////////////////////////////////////////////////////////
// F3 component
///////////////////////////////////////////////////////////////////////////////
txfma_f3 txfma0_f3
(
  .op_dtype_f3a_h               ( txfma0_op_dtype_f3a_h            ), 
  .cmd_f3a_h                    ( txfma0_cmd_f3a_h                 ),                        
  .align1_zap_f3a_h             ( txfma0_align1_zap_f3a_h          ),
  .align2_zap_f3a_h             ( txfma0_align2_zap_f3a_h          ),
  .align1_neg_f3a_h             ( txfma0_align1_neg_f3a_h          ),
  .align2_neg_f3a_h             ( txfma0_align2_neg_f3a_h          ),
  .op2_sxtto_g_f3a_h            ( txfma0_op2_sxtto_g_f3a_h         ),
  .fa_f3a_h                     ( txfma0_fa_f3a_h                  ),
  .fc_f3a_h                     ( txfma0_fc_f3a_h                  ),
  .prodsum_f3a_h                ( txfma0_prodsum_f3a_h             ),
  .sigs_f3a_h                   ( txfma0_sigs_f3a_h                ),
  .align1_neg_cin_f3a_h         ( txfma0_align1_neg_cin_f3a_h      ),
  .align2_neg_cin_f3a_h         ( txfma0_align2_neg_cin_f3a_h      ),
  .fround_l_f3a_h               ( txfma0_fround_l_f3a_h            ),
  .eprod_hi_zero_f3a_h          ( txfma0_eprod_hi_zero_f3a_h       ),  
  .e_res_f3a_h                  ( txfma0_e_res_f3a_h               ),
  .lxd_sub_f3a_h                ( txfma0_lxd_sub_f3a_h             ),
  .align1_mux_sel_f3a_h         ( txfma0_align1_mux_sel_f3a_h      ),  
  .align2_mux_sel_f3a_h         ( txfma0_align2_mux_sel_f3a_h      ),
  .opd3_mux_sel_f3a_h           ( txfma0_opd3_mux_sel_f3a_h        ),
  .int_minmax_mux_sel_f3a_h     ( txfma0_int_minmax_mux_sel_f3a_h  ),  
  .ediff1_abs_f3a_h             ( txfma0_ediff1_abs_f3a_h          ),
  .ediff2_abs_f3a_h             ( txfma0_ediff2_abs_f3a_h          ), 
  .lxd_f3a_h                    ( txfma0_lxd_f3a_h                 ),  
  .comb_add_hi_ph_trz_f3a_h     ( txfma0_comb_add_hi_ph_trz_f3a_h  ),
  .comb_add_lo_ps_trz_f3a_h     ( txfma0_comb_add_lo_ps_trz_f3a_h  ),
  .int_minmax_res_f3a_h         ( txfma0_int_minmax_res_f3a_h      ),
  .comb_add_f3a_h               ( txfma0_comb_add_f3a_h            ),
  .aligned_48_int_r_f3a_h       ( txfma0_aligned_48_int_r_f3a_h    ),
  .aligned_48_int_l_f3a_h       ( txfma0_aligned_48_int_l_f3a_h    )
);


///////////////////////////////////////////////////////////////////////////////
// C3 component
///////////////////////////////////////////////////////////////////////////////
txfma_c3 txfma0_c3 
(
  .op_dtype_f3a_h              ( txfma0_op_dtype_f3a_h              ),
  .cmd_f3a_h                   ( txfma0_cmd_f3a_h                   ),
  .rm_f3a_h                    ( txfma0_rm_f3a_h                    ),  
  .ediff1_f3a_h                ( txfma0_ediff1_f3a_h                ),
  .ediff2_f3a_h                ( txfma0_ediff2_f3a_h                ),
  .sigs_f3a_h                  ( txfma0_sigs_f3a_h                  ),
  .eff_sub1_f3a_h              ( txfma0_eff_sub1_f3a_h              ),
  .eff_sub2_f3a_h              ( txfma0_eff_sub2_f3a_h              ),
  .eprod_hi_zero_f3a_h         ( txfma0_eprod_hi_zero_f3a_h         ),
  .eprod_lo_zero_f3a_h         ( txfma0_eprod_lo_zero_f3a_h         ),
  .prodsum_a2_f3a_h            ( txfma0_prodsum_f3a_h[`TXFMA_A2]    ),
  .e_sticky1_f3a_h             ( txfma0_e_sticky1_f3a_h             ),
  .e_sticky2_f3a_h             ( txfma0_e_sticky2_f3a_h             ),
  .minmax_op_f3a_h             ( txfma0_minmax_op_f3a_h             ),
  .comp_op_f3a_h               ( txfma0_comp_op_f3a_h               ),
  .align1_neg_cin_f3a_h        ( txfma0_align1_neg_cin_f3a_h        ),
  .align2_neg_cin_f3a_h        ( txfma0_align2_neg_cin_f3a_h        ),
  .fround_l_f3a_h              ( txfma0_fround_l_f3a_h              ),  
  .prod_inf_hi_f3a_h           ( txfma0_prod_inf_hi_f3a_h           ),
  .prod_inf_lo_f3a_h           ( txfma0_prod_inf_lo_f3a_h           ),
  .c_inf_f3a_h                 ( txfma0_c_inf_f3a_h                 ),
  .eff_sign1_f3a_h             ( txfma0_eff_sign1_f3a_h             ),
  .prod_sign_hi_f3a_h          ( txfma0_prod_sign_hi_f3a_h          ),
  .prod_sign_lo_f3a_h          ( txfma0_prod_sign_lo_f3a_h          ),
  .sc_f3a_h                    ( txfma0_sc_f3a_h                    ),
  .opd3_mux_sel_f3a_h          ( txfma0_opd3_mux_sel_f3a_h          ),
  .fc_b0_f3a_h                 ( txfma0_fc_f3a_h[`TXFMA_B0]         ),
  .fb_hi_zero_f3a_h            ( txfma0_fb_hi_zero_f3a_h            ),
  .exp_frac_e_shift_f3a_h      ( txfma0_exp_frac_e_shift_f3a_h      ),
  .res_zero_f3a_h              ( txfma0_res_zero_f3a_h              ),
  .ec_f3a_h                    ( txfma0_ec_f3a_h                    ),
  .classify_f3a_h              ( txfma0_classify_f3a_h              ),
  .flags_en_f3a_h              ( txfma0_flags_en_f3a_h              ),
  .spec_res_en_f3a_h           ( txfma0_spec_res_en_f3a_h           ),  
  .aligned_48_int_r_f3a_h      ( txfma0_aligned_48_int_r_f3a_h      ),
  .aligned_48_int_l_f3a_h      ( txfma0_aligned_48_int_l_f3a_h      ),
  .res_inf_f3a_h               ( txfma0_res_inf_f3a_h               ),
  .res_qnan_f3a_h              ( txfma0_res_qnan_f3a_h              ),
  .res_snan_f3a_h              ( txfma0_res_snan_f3a_h              ),
  .res_snan_out_f3a_h          ( txfma0_res_snan_out_f3a_h          ),
  .res_qnan_out_f3a_h          ( txfma0_res_qnan_out_f3a_h          ),
  .res_inf_out_f3a_h           ( txfma0_res_inf_out_f3a_h           ),
  .ec_zero_f3a_h               ( txfma0_ec_zero_f3a_h               ),
  .a_is_qnan_hi_f3a_h          ( txfma0_a_is_qnan_hi_f3a_h          ),
  .c_is_qnan_f3a_h             ( txfma0_c_is_qnan_f3a_h             ),
  .a_is_snan_hi_f3a_h          ( txfma0_a_is_snan_hi_f3a_h          ),
  .c_is_snan_f3a_h             ( txfma0_c_is_snan_f3a_h             ),
  .align1_zap_f3a_h            ( txfma0_align1_zap_f3a_h            ),
  .align2_zap_f3a_h            ( txfma0_align2_zap_f3a_h            ),
  .align1_neg_f3a_h            ( txfma0_align1_neg_f3a_h            ),
  .align2_neg_f3a_h            ( txfma0_align2_neg_f3a_h            ),
  .op2_sxtto_g_f3a_h           ( txfma0_op2_sxtto_g_f3a_h           ),
  .int_minmax_mux_sel_f3a_h    ( txfma0_int_minmax_mux_sel_f3a_h    ),
  .s_res_f3a_h                 ( txfma0_s_res_f3a_h                 ),
  .s_res_check_cadd_a3_f3a_h   ( txfma0_s_res_check_cadd_a3_f3a_h   ),
  .comp_res_sel_f3a_h          ( txfma0_comp_res_sel_f3a_h          ),
  .fa_eq_fc_res_check_f3a_h    ( txfma0_fa_eq_fc_res_check_f3a_h    ),
  .lxd_sub_f3a_h               ( txfma0_lxd_sub_f3a_h               ),
  .trans_log_rr_neg_f3a_h      ( txfma0_trans_log_rr_neg_f3a_h      ),
  .res_zero_out_f3a_h          ( txfma0_res_zero_out_f3a_h          ),
  .fc_sel_f3a_h                ( txfma0_fc_sel_f3a_h                ),
  .ediff1_abs_f3a_h            ( txfma0_ediff1_abs_f3a_h            ),
  .ediff2_abs_f3a_h            ( txfma0_ediff2_abs_f3a_h            ), 
  .eff_sub1_out_f3a_h          ( txfma0_eff_sub1_out_f3a_h          ),
  .cmp_invalid_f3a_h           ( txfma0_cmp_invalid_f3a_h           ),
  .fround_sticky_until_l_f3a_h ( txfma0_fround_sticky_until_l_f3a_h )
);

  
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// F4 Stage
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// F4FF
///////////////////////////////////////////////////////////////////////////////

en_ff 
#(
  .width($bits( {txfma0_lxd_f3a_h, 
                 txfma0_eff_sub2_f3a_h,
                 txfma0_eff_sub1_out_f3a_h,
                 txfma0_sin_res_shf_idx_f3a_h,
                 txfma0_eb_hi_lsb_f3a_h} ) )
)                        
F4_FF
(
  .clock      ( clock                          ), 
  .en         ( txfma0_ctrl_valid_f3a_h        ),
  .D          ( {txfma0_lxd_f3a_h,
                 txfma0_eff_sub2_f3a_h,
                 txfma0_eff_sub1_out_f3a_h,
                 txfma0_sin_res_shf_idx_f3a_h,
                 txfma0_eb_hi_lsb_f3a_h} ),                 
  .Q          ( {txfma0_lxd_f4a_h,
                 txfma0_eff_sub2_f4a_h,
                 txfma0_eff_sub1_f4a_h,
                 txfma0_sin_res_shf_idx_f4a_h,
                 txfma0_eb_hi_lsb_f4a_h} ) 
);



en_ff 
#(
  .width($bits({txfma0_e_res_f3a_h, 
                txfma0_exp_frac_e_shift_f3a_h, 
                txfma0_e_sticky1_f3a_h, 
                txfma0_e_sticky2_f3a_h, 
                txfma0_ec_f3a_h,
                txfma0_ea_hi_f3a_h,
                txfma0_ediff1_zap_f3a_h,
                txfma0_fround_sticky_until_l_f3a_h}))
)
E4_FF
(
  .clock      ( clock                                ), 
  .en         ( txfma0_ctrl_valid_f3a_h              ),
  .D          ( {txfma0_e_res_f3a_h, 
                 txfma0_exp_frac_e_shift_f3a_h, 
                 txfma0_e_sticky1_f3a_h, 
                 txfma0_e_sticky2_f3a_h, 
                 txfma0_ec_f3a_h,
                 txfma0_ea_hi_f3a_h,
                 txfma0_ediff1_zap_f3a_h,
                 txfma0_fround_sticky_until_l_f3a_h} ),
  .Q          ( {txfma0_e_f4a_h, 
                 txfma0_exp_frac_e_shift_f4a_h, 
                 txfma0_e_sticky1_f4a_h,  
                 txfma0_e_sticky2_f4a_h,  
                 txfma0_ec_f4a_h,
                 txfma0_ea_hi_f4a_h,
                 txfma0_ediff1_zap_f4a_h,
                 txfma0_fround_sticky_until_l_f4a_h} )
);

en_ff 
#(
  .width($bits( {txfma0_s_res_f3a_h,
                 txfma0_prod_sign_hi_f3a_h,
                 txfma0_prod_sign_lo_f3a_h,                 
                 txfma0_s_res_check_cadd_a3_f3a_h,
                 txfma0_sc_f3a_h}             ))
)                        
S4_FF
(
  .clock      ( clock                               ), 
  .en         ( txfma0_ctrl_valid_f3a_h             ),
  .D          ( {txfma0_s_res_f3a_h,
                 txfma0_prod_sign_hi_f3a_h,
                 txfma0_prod_sign_lo_f3a_h,
                 txfma0_s_res_check_cadd_a3_f3a_h,
                 txfma0_sc_f3a_h}    ),
  .Q          ( {txfma0_s_res_f4a_h,
                 txfma0_prod_sign_hi_f4a_h,
                 txfma0_prod_sign_lo_f4a_h,
                 txfma0_s_res_check_cadd_a3_f4a_h,
                 txfma0_sc_f4a_h}                  )
);


en_ff 
#(
  .width($bits({txfma0_op_dtype_f3a_h, 
                txfma0_cmd_f3a_h, 
                txfma0_sigs_f3a_h, 
                txfma0_rm_f3a_h, 
                txfma0_comp_op_f3a_h, 
                txfma0_minmax_op_f3a_h,
                txfma0_comp_res_sel_f3a_h}) )
)                        
F4_CTRL_FF
(
  .clock      ( clock                               ), 
  .en         ( txfma0_ctrl_valid_f3a_h &
                !txfma0_use_prev_sigs_f3a_h         ),
  .D          ( {txfma0_op_dtype_f3a_h, 
                 txfma0_cmd_f3a_h, 
                 txfma0_sigs_f3a_h, 
                 txfma0_rm_f3a_h, 
                 txfma0_comp_op_f3a_h, 
                 txfma0_minmax_op_f3a_h,
                 txfma0_comp_res_sel_f3a_h}     ),
  .Q          ( {txfma0_op_dtype_f4a_h, 
                 txfma0_cmd_f4a_h, 
                 txfma0_sigs_f4a_h, 
                 txfma0_rm_f4a_h, 
                 txfma0_comp_op_f4a_h, 
                 txfma0_minmax_op_f4a_h,
                 txfma0_comp_res_sel_f4a_h}     )
);


en_ff 
#(
  .width($bits({txfma0_a_inf_f3a_h,
                txfma0_int_minmax_mux_sel_f3a_h,
                txfma0_fa_eq_fc_res_check_f3a_h,
                txfma0_fc_sel_f3a_h,
                txfma0_fround_l_f3a_h,
                txfma0_cmp_invalid_f3a_h,
                txfma0_trans_log_rr_neg_f3a_h}) )
)                        
F4_CTRL2_FF
(
  .clock      ( clock                               ), 
  .en         ( txfma0_ctrl_valid_f3a_h             ),
  .D          ( {txfma0_a_inf_f3a_h,
                 txfma0_int_minmax_mux_sel_f3a_h,
                 txfma0_fa_eq_fc_res_check_f3a_h,
                 txfma0_fc_sel_f3a_h,
                 txfma0_fround_l_f3a_h,
                 txfma0_cmp_invalid_f3a_h,
                 txfma0_trans_log_rr_neg_f3a_h}     ),
  .Q          ( {txfma0_a_inf_f4a_h,
                 txfma0_int_minmax_mux_sel_f4a_h, 
                 txfma0_fa_eq_fc_res_check_f4a_h,
                 txfma0_fc_sel_f4a_h,
                 txfma0_fround_f4a_h,
                 txfma0_cmp_invalid_f4a_h,
                 txfma0_trans_log_rr_neg_f4a_h}     )
);



en_ff 
#(
  .width( $bits(txfma0_use_prev_sigs_f3a_h ))
)                        
F4_PREV_CTRL_FF
(
  .clock      ( clock                      ), 
  .en         ( txfma0_ctrl_valid_f3a_h    ),
  .D          ( txfma0_use_prev_sigs_f3a_h ),
  .Q          ( txfma0_use_prev_sigs_f4a_h )
);


en_ff 
#(
  .width( $bits({txfma0_comb_add_hi_ph_trz_f3a_h, 
                 txfma0_comb_add_lo_ps_trz_f3a_h}) )
)                        
F4_TRZ_FF
(
  .clock      ( clock                              ), 
  .en         ( txfma0_ctrl_valid_f3a_h            ),
  .D          ( {txfma0_comb_add_hi_ph_trz_f3a_h, 
                 txfma0_comb_add_lo_ps_trz_f3a_h}  ),
  .Q          ( {txfma0_comb_add_hi_ph_trz_f4a_h, 
                 txfma0_comb_add_lo_ps_trz_f4a_h}  )
);

en_ff 
#(
  .width($bits( {txfma0_ec_zero_f3a_h,
                 txfma0_res_zero_out_f3a_h,
                 txfma0_res_one_f3a_h,                
                 txfma0_fb_hi_zero_f3a_h,
                 txfma0_eprod_hi_zero_f3a_h,
                 txfma0_eprod_lo_zero_f3a_h}) )
)
F4_ZER0_FF
(
  .clock      ( clock                        ), 
  .en         ( txfma0_ctrl_valid_f3a_h      ),
  .D          ( {txfma0_ec_zero_f3a_h,
                 txfma0_res_zero_out_f3a_h,
                 txfma0_res_one_f3a_h,                                  
                 txfma0_fb_hi_zero_f3a_h,
                 txfma0_eprod_hi_zero_f3a_h,
                 txfma0_eprod_lo_zero_f3a_h} ),
  .Q          ( {txfma0_ec_zero_f4a_h, 
                 txfma0_res_zero_f4a_h,
                 txfma0_res_one_f4a_h,                                  
                 txfma0_fb_hi_zero_f4a_h,
                 txfma0_eprod_hi_zero_f4a_h,
                 txfma0_eprod_lo_zero_f4a_h} )
);

en_ff 
#(
  .width($bits({txfma0_res_inf_out_f3a_h,
                txfma0_res_snan_out_f3a_h, 
                txfma0_res_qnan_out_f3a_h, 
                txfma0_classify_f3a_h,
                txfma0_res_unorm_one_f3a_h,
                txfma0_res_snorm_one_f3a_h,                
                txfma0_res_pos_int_nan_f3a_h,
                txfma0_res_neg_int_nan_f3a_h,
                txfma0_res_uint_nan_f3a_h}))
)                        
F4_SPEC_FF
(
  .clock      ( clock                          ), 
  .en         ( txfma0_ctrl_valid_f3a_h &
                txfma0_spec_res_en_f3a_h       ),
  .D          ( {txfma0_res_inf_out_f3a_h,
                 txfma0_res_snan_out_f3a_h, 
                 txfma0_res_qnan_out_f3a_h, 
                 txfma0_classify_f3a_h,
                 txfma0_res_unorm_one_f3a_h,
                 txfma0_res_snorm_one_f3a_h,              
                 txfma0_res_pos_int_nan_f3a_h,
                 txfma0_res_neg_int_nan_f3a_h,
                 txfma0_res_uint_nan_f3a_h}    ),
  .Q          ( {txfma0_res_inf_f4a_h,
                 txfma0_res_snan_f4a_h, 
                 txfma0_res_qnan_f4a_h, 
                 txfma0_classify_f4a_h,
                 txfma0_res_unorm_one_f4a_h,
                 txfma0_res_snorm_one_f4a_h,                 
                 txfma0_res_pos_int_nan_f4a_h,
                 txfma0_res_neg_int_nan_f4a_h,
                 txfma0_res_uint_nan_f4a_h}    )
);


en_ff 
#(
  .width($bits({txfma0_cvt_fui_pos_ovf_f3a_h,                
                txfma0_cvt_fi_pos_ovf_f3a_h,
                txfma0_cvt_fi_neg_ovf_f3a_h,
                txfma0_cvt_fi_min_neg_f3a_h,
                txfma0_ediff1_lt_31_f3a_h,
                txfma0_norm_f32_ovf_f3a_h,
                txfma0_norm_one_out_f3a_h}))
)                        
F4_SPEC2_FF
(
  .clock      ( clock                          ), 
  .en         ( txfma0_ctrl_valid_f3a_h        ),
  .D          ( {txfma0_cvt_fui_pos_ovf_f3a_h,                 
                 txfma0_cvt_fi_pos_ovf_f3a_h,
                 txfma0_cvt_fi_neg_ovf_f3a_h,
                 txfma0_cvt_fi_min_neg_f3a_h,
                 txfma0_ediff1_lt_31_f3a_h,
                 txfma0_norm_f32_ovf_f3a_h,
                 txfma0_norm_one_out_f3a_h}    ),
  .Q          ( {txfma0_cvt_fui_pos_ovf_f4a_h,                 
                 txfma0_cvt_fi_pos_ovf_f4a_h,
                 txfma0_cvt_fi_neg_ovf_f4a_h,
                 txfma0_cvt_fi_min_neg_f4a_h,
                 txfma0_ediff1_lt_31_f4a_h,
                 txfma0_norm_f32_ovf_f4a_h,
                 txfma0_norm_one_out_f4a_h}   )
);




en_ff 
#(
  .width( $bits(txfma0_flags_f3a_h)  )
)                        
F4_FLG_FF
(
  .clock      ( clock                     ), 
  .en         ( txfma0_ctrl_valid_f3a_h &
                txfma0_flags_en_f3a_h     ),
  .D          ( txfma0_flags_f3a_h        ),
  .Q          ( txfma0_flags_f4a_h        )
);


rst_en_ff 
#(
  .width( $bits(txfma0_flags_en_f3a_h) )
)                        
F4_FLG_EN_FF
(
  .clock      ( clock                   ), 
  .reset      ( reset                   ), 
  .en         ( txfma0_ctrl_valid_f3a_h ), 
  .D          ( txfma0_flags_en_f3a_h   ),
  .Q          ( txfma0_flags_en_f4a_h   )
);

rst_en_ff 
#(
  .width( $bits(txfma0_spec_res_en_f3a_h) )
)                        
F4_SPEC_EN_FF
(
  .clock      ( clock                    ), 
  .reset      ( reset                    ), 
  .en         ( txfma0_ctrl_valid_f3a_h  ), 
  .D          ( txfma0_spec_res_en_f3a_h ),
  .Q          ( txfma0_spec_res_en_f4a_h )
);



en_ff 
#(
  .width($bits({txfma0_comb_add_f3a_h,
                txfma0_int_minmax_res_f3a_h}))
)                        
F4_DATA_FF
(
  .clock      ( clock                         ), 
  .en         ( txfma0_ctrl_valid_f3a_h       ),
  .D          ( {txfma0_comb_add_f3a_h,
                 txfma0_int_minmax_res_f3a_h} ),
  .Q          ( {txfma0_comb_add_f4a_h,
                 txfma0_int_minmax_res_f4a_h} )
);


rst_ff 
#(
  .width( $bits(txfma0_ctrl_valid_f3a_h) )
)                        
F4_CTRL_EN_FF
(
  .clock      ( clock                   ), 
  .reset      ( reset                   ), 
  .D          ( txfma0_ctrl_valid_f3a_h ),
  .Q          ( txfma0_ctrl_valid_f4a_h )
);



///////////////////////////////////////////////////////////////////////////////
// F4 component
///////////////////////////////////////////////////////////////////////////////
txfma_f4 txfma0_f4 
(
  .op_dtype_f4a_h                ( txfma0_op_dtype_f4a_h                ),
  .cmd_f4a_h                     ( txfma0_cmd_f4a_h                     ),
  .mergd_mux_sel_f4a_h           ( txfma0_mergd_mux_sel_f4a_h           ),
  .neg_norm_f4a_h                ( txfma0_neg_norm_f4a_h                ),
  .comb_add_f4a_h                ( txfma0_comb_add_f4a_h                ),
  .int_minmax_res_f4a_h          ( txfma0_int_minmax_res_f4a_h          ),
  .sign_hi_f4a_h                 ( txfma0_s_res_out_f4a_h               ),
  .sigs_f4a_h                    ( txfma0_sigs_f4a_h                    ),  
  .e_sticky1_until_r_f4a_h       ( txfma0_e_sticky1_until_r_f4a_h       ),
  .e_sticky1_until_g_f4a_h       ( txfma0_e_sticky1_until_g_f4a_h       ),  
  .fround_sticky_until_l_f4a_h   ( txfma0_fround_sticky_until_l_f4a_h   ),
  .rm_f4a_h                      ( txfma0_rm_f4a_h                      ),  
  .rm_clr_f4a_h                  ( txfma0_rm_clr_f4a_h                  ),  
  .rm_rne_f4a_h                  ( txfma0_rm_rne_f4a_h                  ),
  .rm_rmm_f4a_h                  ( txfma0_rm_rmm_f4a_h                  ),
  .rnd_ctl_f4a_h                 ( txfma0_rnd_ctl_f4a_h                 ),
  .mergd_frac_res_f4a_h          ( txfma0_mergd_frac_res_f4a_h          ),
  .norm_shift_out_neg_a0b0_f4a_h ( txfma0_norm_shift_out_neg_a0b0_f4a_h ),
  .ftz_f4a_h                     ( txfma0_ftz_f4a_h                     ),
  .norm_shfamnt_f4a_h            ( txfma0_norm_shfamnt_f4a_h            ),
  .sticky_until_l_f4a_h          ( txfma0_sticky_until_l_f4a_h          )
);

///////////////////////////////////////////////////////////////////////////////
// E4 component
///////////////////////////////////////////////////////////////////////////////
txfma_e4 txfma0_e4 
(
  .cmd_f4a_h                   ( txfma0_cmd_f4a_h                   ),
  .rm_f4a_h                    ( txfma0_rm_f4a_h                    ),
  .sign_hi_f4a_h               ( txfma0_s_res_out_f4a_h             ),
  .e_sticky1_f4a_h             ( txfma0_e_sticky1_f4a_h             ),
  .e_sticky2_f4a_h             ( txfma0_e_sticky2_f4a_h             ),
  .op_dtype_f4a_h              ( txfma0_op_dtype_f4a_h              ),  
  .sigs_f4a_h                  ( txfma0_sigs_f4a_h                  ),    
  .e_f4a_h                     ( txfma0_e_f4a_h                     ),
  .lxd_f4a_h                   ( txfma0_lxd_f4a_h                   ),
  .comb_add_hi_ph_trz_f4a_h    ( txfma0_comb_add_hi_ph_trz_f4a_h    ),
  .comb_add_lo_ps_trz_f4a_h    ( txfma0_comb_add_lo_ps_trz_f4a_h    ),
  .e_res_f4a_h                 ( txfma0_e_res_f4a_h                 ),
  .e_sticky1_until_r_f4a_h     ( txfma0_e_sticky1_until_r_f4a_h     ),
  .e_sticky1_until_g_f4a_h     ( txfma0_e_sticky1_until_g_f4a_h     ),
  .use_lxd_f4a_h               ( txfma0_use_lxd_f4a_h               ),
  .e_lxd_add_in2_mux_sel_f4a_h ( txfma0_e_lxd_add_in2_mux_sel_f4a_h ),
  .e_lxd_add_cin_f4a_h         ( txfma0_e_lxd_add_cin_f4a_h         ),
  .nsh_correction_f4a_h        ( txfma0_nsh_correction_f4a_h        ),
  .int_minmax_mux_sel_f4a_h    ( txfma0_int_minmax_mux_sel_f4a_h    ),
  .mergd_mux_sel_f4a_h         ( txfma0_mergd_mux_sel_f4a_h         ),
  .ea_hi_f4a_h                 ( txfma0_ea_hi_f4a_h                 ),
  .e_force_to_zero_f4a_h       ( txfma0_e_force_to_zero_f4a_h       ), 
  .fb_hi_zero_f4a_h            ( txfma0_fb_hi_zero_f4a_h            ),
  .eb_hi_lsb_f4a_h             ( txfma0_eb_hi_lsb_f4a_h             ),
  .ediff1_zap_f4a_h            ( txfma0_ediff1_zap_f4a_h            ),
  .ec_f4a_h                    ( txfma0_ec_f4a_h                    ),
  .rmode_f4a_h                 ( txfma0_rmode_f4a_h                 ),
  .def_ovf_f4a_h               ( txfma0_def_ovf_f4a_h               ),
  .pot_ovf_f4a_h               ( txfma0_pot_ovf_f4a_h               ),
  .def_unf_f4a_h               ( txfma0_def_unf_f4a_h               ),
  .pot_unf_f4a_h               ( txfma0_pot_unf_f4a_h               )
);


///////////////////////////////////////////////////////////////////////////////
// C4 component
///////////////////////////////////////////////////////////////////////////////
txfma_c4 txfma0_c4 
(
  .op_dtype_f4a_h                ( txfma0_op_dtype_f4a_h                ), 
  .cmd_f4a_h                     ( txfma0_cmd_f4a_h                     ), 
  .sigs_f4a_h                    ( txfma0_sigs_f4a_h                    ),
  .lxd_f4a_h                     ( txfma0_lxd_f4a_h                     ), 
  .eff_sub1_f4a_h                ( txfma0_eff_sub1_f4a_h                ),
  .eff_sub2_f4a_h                ( txfma0_eff_sub2_f4a_h                ),
  .norm_shift_out_neg_a0b0_f4a_h ( txfma0_norm_shift_out_neg_a0b0_f4a_h ),
  .sc_f4a_h                      ( txfma0_sc_f4a_h                      ),
  .eprod_hi_zero_f4a_h           ( txfma0_eprod_hi_zero_f4a_h           ),
  .ec_zero_f4a_h                 ( txfma0_ec_zero_f4a_h                 ),
  .comb_add_a3_hi_f4a_h          ( txfma0_comb_add_f4a_h[`TXFMA_A3]     ),
  .s_res_check_cadd_a3_f4a_h     ( txfma0_s_res_check_cadd_a3_f4a_h     ),
  .comp_op_f4a_h                 ( txfma0_comp_op_f4a_h                 ),
  .minmax_op_f4a_h               ( txfma0_minmax_op_f4a_h               ),
  .comb_add_lo_ps_trz_f4a_h      ( txfma0_comb_add_lo_ps_trz_f4a_h      ),
  .fa_eq_fc_res_check_f4a_h      ( txfma0_fa_eq_fc_res_check_f4a_h      ),
  .fc_sel_f4a_h                  ( txfma0_fc_sel_f4a_h                  ),
  .exp_frac_e_shift_f4a_h        ( txfma0_exp_frac_e_shift_f4a_h        ),
  .sin_res_shf_idx_f4a_h         ( txfma0_sin_res_shf_idx_f4a_h         ),
  .trans_log_rr_neg_f4a_h        ( txfma0_trans_log_rr_neg_f4a_h        ),
  .fround_f4a_h                  ( txfma0_fround_f4a_h                  ),
  .ediff1_zap_f4a_h              ( txfma0_ediff1_zap_f4a_h              ),
  .sticky_until_l_f4a_h          ( txfma0_sticky_until_l_f4a_h          ),
  .comp_res_sel_in_f4a_h         ( txfma0_comp_res_sel_f4a_h            ),
  .prod_sign_hi_f4a_h            ( txfma0_prod_sign_hi_f4a_h            ),
  .s_res_in_f4a_h                ( txfma0_s_res_f4a_h                   ),
  .norm_shfamnt_f4a_h            ( txfma0_norm_shfamnt_f4a_h            ),
  .e_force_to_zero_f4a_h         ( txfma0_e_force_to_zero_f4a_h         ),
  .use_lxd_f4a_h                 ( txfma0_use_lxd_f4a_h                 ),
  .e_lxd_add_in2_mux_sel_f4a_h   ( txfma0_e_lxd_add_in2_mux_sel_f4a_h   ),
  .e_lxd_add_cin_f4a_h           ( txfma0_e_lxd_add_cin_f4a_h           ),
  .nsh_correction_f4a_h          ( txfma0_nsh_correction_f4a_h          ),
  .mergd_mux_sel_f4a_h           ( txfma0_mergd_mux_sel_f4a_h           ),
  .neg_norm_f4a_h                ( txfma0_neg_norm_f4a_h                ),
  .s_res_out_f4a_h               ( txfma0_s_res_out_f4a_h               ),
  .rm_clr_f4a_h                  ( txfma0_rm_clr_f4a_h                  ),
  .rm_rne_f4a_h                  ( txfma0_rm_rne_f4a_h                  ),
  .rm_rmm_f4a_h                  ( txfma0_rm_rmm_f4a_h                  ),
  .comp_op_true_f4a_h            ( txfma0_comp_op_true_f4a_h            )
);



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// F5 Stage
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// F5FF
///////////////////////////////////////////////////////////////////////////////

en_ff 
#(
  .width($bits({txfma0_mergd_frac_res_f4a_h,
                txfma0_e_res_f4a_h,
                txfma0_s_res_out_f4a_h,
                txfma0_rnd_ctl_f4a_h}))
)                        
F5_RES_FF
(
  .clock      ( clock                            ), 
  .en         ( txfma0_ctrl_valid_f4a_h          ),
  .D          ( {txfma0_mergd_frac_res_f4a_h,
                 txfma0_e_res_f4a_h,
                 txfma0_s_res_out_f4a_h,
                 txfma0_rnd_ctl_f4a_h}        ),
  .Q          ( {txfma0_mergd_frac_res_f5a_h,
                 txfma0_e_f5a_h,
                 txfma0_s_f5a_h,
                 txfma0_rnd_ctl_f5a_h }                )
);


en_ff 
#(
  .width($bits( {txfma0_op_dtype_f4a_h, 
                 txfma0_cmd_f4a_h, 
                 txfma0_sigs_f4a_h, 
                 txfma0_comp_op_f4a_h, 
                 txfma0_rm_f4a_h, 
                 txfma0_minmax_op_f4a_h} ))
)                        
F5_CTRL_FF
(
  .clock      ( clock                          ), 
  .en         ( txfma0_ctrl_valid_f4a_h &
                !txfma0_use_prev_sigs_f4a_h    ),
  .D          ( {txfma0_op_dtype_f4a_h, 
                 txfma0_cmd_f4a_h, 
                 txfma0_sigs_f4a_h, 
                 txfma0_comp_op_f4a_h,
                 txfma0_rm_f4a_h, 
                 txfma0_minmax_op_f4a_h}      ),
  .Q          ( {txfma0_op_dtype_f5a_h, 
                 txfma0_cmd_f5a_h, 
                 txfma0_sigs_f5a_h, 
                 txfma0_comp_op_f5a_h, 
                 txfma0_rm_f5a_h, 
                 txfma0_minmax_op_f5a_h}      )
);

en_ff 
#(
  .width($bits( {txfma0_a_inf_f4a_h,
                 txfma0_ftz_f4a_h, 
                 txfma0_eff_sub1_f4a_h,
                 txfma0_comp_op_true_f4a_h} ))
)                        
F5_CTRL2_FF
(
  .clock      ( clock                        ), 
  .en         ( txfma0_ctrl_valid_f4a_h      ),
  .D          ( {txfma0_a_inf_f4a_h,
                 txfma0_ftz_f4a_h, 
                 txfma0_eff_sub1_f4a_h,
                 txfma0_comp_op_true_f4a_h}      ),
  .Q          ( {txfma0_a_inf_f5a_h,
                 txfma0_ftz_f5a_h, 
                 txfma0_eff_sub1_f5a_h,
                 txfma0_comp_op_true_f5a_h}      )
);

 
en_ff 
#(
  .width($bits( {txfma0_ec_zero_f4a_h,
                 txfma0_res_one_f4a_h,
                 txfma0_res_zero_f4a_h}) )
)
F5_ZER0_FF
(
  .clock      ( clock                   ), 
  .en         ( txfma0_ctrl_valid_f4a_h ),
  .D          ( {txfma0_ec_zero_f4a_h,
                 txfma0_res_one_f4a_h,
                 txfma0_res_zero_f4a_h} ),
  .Q          ( {txfma0_ec_zero_f5a_h, 
                 txfma0_res_one_f5a_h,
                 txfma0_res_zero_f5a_h} )
);

en_ff 
#(
  .width($bits( {txfma0_res_inf_f4a_h,
                 txfma0_res_snan_f4a_h, 
                 txfma0_res_qnan_f4a_h, 
                 txfma0_classify_f4a_h,
                 txfma0_res_unorm_one_f4a_h,
                 txfma0_res_snorm_one_f4a_h,
                 txfma0_res_pos_int_nan_f4a_h,
                 txfma0_res_neg_int_nan_f4a_h,
                 txfma0_res_uint_nan_f4a_h} ))
)                        
F5_SPEC_FF
(
  .clock      ( clock                           ), 
  .en         ( txfma0_ctrl_valid_f4a_h &
                txfma0_spec_res_en_f4a_h        ),
  .D          ( {txfma0_res_inf_f4a_h,
                 txfma0_res_snan_f4a_h, 
                 txfma0_res_qnan_f4a_h, 
                 txfma0_classify_f4a_h,
                 txfma0_res_unorm_one_f4a_h,
                 txfma0_res_snorm_one_f4a_h,
                 txfma0_res_pos_int_nan_f4a_h,
                 txfma0_res_neg_int_nan_f4a_h,
                 txfma0_res_uint_nan_f4a_h}    ),
  .Q          ( {txfma0_res_inf_f5a_h, 
                 txfma0_res_snan_f5a_h, 
                 txfma0_res_qnan_f5a_h, 
                 txfma0_classify_f5a_h, 
                 txfma0_res_unorm_one_f5a_h,
                 txfma0_res_snorm_one_f5a_h,
                 txfma0_res_pos_int_nan_f5a_h,
                 txfma0_res_neg_int_nan_f5a_h,
                 txfma0_res_uint_nan_f5a_h}    )
);


en_ff 
#(
  .width($bits( {txfma0_pot_ovf_f4a_h, 
                 txfma0_def_ovf_f4a_h, 
                 txfma0_rmode_f4a_h, 
                 txfma0_cmp_invalid_f4a_h,                 
                 txfma0_pot_unf_f4a_h, 
                 txfma0_def_unf_f4a_h, 
                 txfma0_cvt_fui_pos_ovf_f4a_h,                 
                 txfma0_cvt_fi_pos_ovf_f4a_h,
                 txfma0_cvt_fi_neg_ovf_f4a_h,
                 txfma0_cvt_fi_min_neg_f4a_h,
                 txfma0_ediff1_lt_31_f4a_h,
                 txfma0_sticky_until_l_f4a_h, 
                 txfma0_norm_f32_ovf_f4a_h,
                 txfma0_norm_one_out_f4a_h} ))
)                        
F5_SPEC2_AND_CTL_FF
(
  .clock      ( clock                          ), 
  .en         ( txfma0_ctrl_valid_f4a_h        ),
  .D          ( {txfma0_pot_ovf_f4a_h, 
                 txfma0_def_ovf_f4a_h, 
                 txfma0_rmode_f4a_h, 
                 txfma0_cmp_invalid_f4a_h,                 
                 txfma0_pot_unf_f4a_h, 
                 txfma0_def_unf_f4a_h, 
                 txfma0_cvt_fui_pos_ovf_f4a_h,                 
                 txfma0_cvt_fi_pos_ovf_f4a_h,
                 txfma0_cvt_fi_neg_ovf_f4a_h,
                 txfma0_cvt_fi_min_neg_f4a_h,
                 txfma0_ediff1_lt_31_f4a_h,
                 txfma0_sticky_until_l_f4a_h, 
                 txfma0_norm_f32_ovf_f4a_h,
                 txfma0_norm_one_out_f4a_h}    ),
  .Q          ( {txfma0_pot_ovf_f5a_h, 
                 txfma0_def_ovf_f5a_h, 
                 txfma0_rmode_f5a_h, 
                 txfma0_cmp_invalid_f5a_h,                 
                 txfma0_pot_unf_f5a_h, 
                 txfma0_def_unf_f5a_h, 
                 txfma0_cvt_fui_pos_ovf_f5a_h,                 
                 txfma0_cvt_fi_pos_ovf_f5a_h,
                 txfma0_cvt_fi_neg_ovf_f5a_h,
                 txfma0_cvt_fi_min_neg_f5a_h,
                 txfma0_ediff1_lt_31_f5a_h,
                 txfma0_sticky_until_l_f5a_h, 
                 txfma0_norm_f32_ovf_f5a_h,
                 txfma0_norm_one_out_f5a_h}    )
);


en_ff 
#(
  .width($bits(txfma0_flags_f4a_h))
)                        
F5_FLG_FF
(
  .clock      ( clock                     ), 
  .en         ( txfma0_ctrl_valid_f4a_h &
                txfma0_flags_en_f4a_h     ),
  .D          ( txfma0_flags_f4a_h        ),
  .Q          ( txfma0_flags_f5a_h        )
);


rst_en_ff 
#(
  .width( $bits(txfma0_spec_res_en_f4a_h) )
)                        
F5_SPEC_EN_FF
(
  .clock      ( clock                    ), 
  .reset      ( reset                    ), 
  .en         ( txfma0_ctrl_valid_f4a_h  ), 
  .D          ( txfma0_spec_res_en_f4a_h ),
  .Q          ( txfma0_spec_res_en_f5a_h )
);


rst_en_ff 
#(
  .width( $bits(txfma0_flags_en_f4a_h) )
)                        
F5_FLG_EN_FF
(
  .clock      ( clock                   ), 
  .reset      ( reset                   ), 
  .en         ( txfma0_ctrl_valid_f4a_h ), 
  .D          ( txfma0_flags_en_f4a_h   ),
  .Q          ( txfma0_flags_en_f5a_h   )
);


rst_ff 
#(
  .width( $bits(txfma0_ctrl_valid_f3a_h) )
)                        
F5_CTRL_EN_FF
(
  .clock      ( clock                   ), 
  .reset      ( reset                   ), 
  .D          ( txfma0_ctrl_valid_f4a_h ),
  .Q          ( txfma0_ctrl_valid_f5a_h )
);


///////////////////////////////////////////////////////////////////////////////
// F5 component
///////////////////////////////////////////////////////////////////////////////
txfma_f5 txfma0_f5 
(
  .a_inf_f5a_h           ( txfma0_a_inf_f5a_h           ),
  .eff_sub1_f5a_h        ( txfma0_eff_sub1_f5a_h        ),
  .norm_f32_ovf_f5a_h    ( txfma0_norm_f32_ovf_f5a_h    ),
  .norm_one_out_f5a_h    ( txfma0_norm_one_out_f5a_h    ),
  .res_unorm_one_f5a_h   ( txfma0_res_unorm_one_f5a_h   ),
  .res_snorm_one_f5a_h   ( txfma0_res_snorm_one_f5a_h   ),   
  .ec_zero_f5a_h         ( txfma0_ec_zero_f5a_h         ),
  .ediff1_lt_31_f5a_h    ( txfma0_ediff1_lt_31_f5a_h    ),
  .res_uint_nan_f5a_h    ( txfma0_res_uint_nan_f5a_h    ),
  .res_zero_f5a_h        ( txfma0_res_zero_f5a_h        ),
  .res_one_f5a_h         ( txfma0_res_one_f5a_h         ),  
  .cvt_fui_pos_ovf_f5a_h ( txfma0_cvt_fui_pos_ovf_f5a_h ),
  .cvt_fi_pos_ovf_f5a_h  ( txfma0_cvt_fi_pos_ovf_f5a_h  ),
  .cvt_fi_neg_ovf_f5a_h  ( txfma0_cvt_fi_neg_ovf_f5a_h  ),
  .cvt_fi_min_neg_f5a_h  ( txfma0_cvt_fi_min_neg_f5a_h  ),
  .res_pos_int_nan_f5a_h ( txfma0_res_pos_int_nan_f5a_h ),
  .res_neg_int_nan_f5a_h ( txfma0_res_neg_int_nan_f5a_h ), 
  .minmax_op_f5a_h       ( txfma0_minmax_op_f5a_h       ),
  .rm_f5a_h              ( txfma0_rm_f5a_h              ),
  .rmode_f5a_h           ( txfma0_rmode_f5a_h           ),
  .comp_op_f5a_h         ( txfma0_comp_op_f5a_h         ),
  .cmp_invalid_f5a_h     ( txfma0_cmp_invalid_f5a_h     ),
  .rnd_ovf_f5a_h         ( txfma0_rnd_ovf_f5a_h         ),
  .pot_ovf_f5a_h         ( txfma0_pot_ovf_f5a_h         ),
  .def_ovf_f5a_h         ( txfma0_def_ovf_f5a_h         ),  
  .pot_unf_f5a_h         ( txfma0_pot_unf_f5a_h         ),
  .def_unf_f5a_h         ( txfma0_def_unf_f5a_h         ),
  .in_flags_f5a_h        ( txfma0_flags_f5a_h           ),
  .cmd_f5a_h             ( txfma0_cmd_f5a_h             ),
  .res_inf_f5a_h         ( txfma0_res_inf_f5a_h         ),
  .res_snan_f5a_h        ( txfma0_res_snan_f5a_h        ),
  .res_qnan_f5a_h        ( txfma0_res_qnan_f5a_h        ),
  .classify_f5a_h        ( txfma0_classify_f5a_h        ),
  .op_dtype_f5a_h        ( txfma0_op_dtype_f5a_h        ),
  .sigs_f5a_h            ( txfma0_sigs_f5a_h            ),
  .mergd_frac_res_f5a_h  ( txfma0_mergd_frac_res_f5a_h  ),
  .e_res_f5a_h           ( txfma0_e_res_f5a_h           ),
  .rnd_ctl_f5a_h         ( txfma0_rnd_ctl_f5a_h         ),
  .sticky_until_l_f5a_h  ( txfma0_sticky_until_l_f5a_h  ),
  .ftz_f5a_h             ( txfma0_ftz_f5a_h             ),
  .s_res_f5a_h           ( txfma0_s_res_f5a_h           ),
  .flags_en_f5a_h        ( txfma0_flags_en_f5a_h        ),
  .spec_res_en_f5a_h     ( txfma0_spec_res_en_f5a_h     ),
  .sel_gmaxu24_f5a_h     ( txfma0_sel_gmaxu24_f5a_h     ),
  .sel_gmaxu16_f5a_h     ( txfma0_sel_gmaxu16_f5a_h     ),
  .sel_gmaxu10_f5a_h     ( txfma0_sel_gmaxu10_f5a_h     ),
  .sel_gmaxu8_f5a_h      ( txfma0_sel_gmaxu8_f5a_h      ),
  .sel_gmaxu2_f5a_h      ( txfma0_sel_gmaxu2_f5a_h      ),
  .sel_gminany_f5a_h     ( txfma0_sel_gminany_f5a_h     ),
  .sel_gmaxs16_f5a_h     ( txfma0_sel_gmaxs16_f5a_h     ),
  .sel_gmins16_f5a_h     ( txfma0_sel_gmins16_f5a_h     ),
  .sel_gmaxs8_f5a_h      ( txfma0_sel_gmaxs8_f5a_h      ),
  .sel_gmins8_f5a_h      ( txfma0_sel_gmins8_f5a_h      ),
  .fma_data_f5a_h        ( txfma0_fma_data_f5a_h        ),
  .fma_flags_f5a_h       ( txfma0_fma_flags_f5a_h       )
);


///////////////////////////////////////////////////////////////////////////////
// E5 component
///////////////////////////////////////////////////////////////////////////////
txfma_e5 txfma0_e5
(
  .e_f5a_h         ( txfma0_e_f5a_h      ),
  .cmd_f5a_h       ( txfma0_cmd_f5a_h    ),
  .e_inc_sel_f5a_h ( txfma0_rnd_ovf_f5a_h ),
  .e_res_f5a_h     ( txfma0_e_res_f5a_h  )
);

///////////////////////////////////////////////////////////////////////////////
// C5 component
///////////////////////////////////////////////////////////////////////////////
txfma_c5 txfma0_c5 
(
  .op_dtype_f5a_h       ( txfma0_op_dtype_f5a_h       ),
  .cmd_f5a_h            ( txfma0_cmd_f5a_h            ), 
  .mergd_frac_res_f5a_h ( txfma0_mergd_frac_res_f5a_h ),
  .ec_zero_f5a_h        ( txfma0_ec_zero_f5a_h        ),  
  .s_f5a_h              ( txfma0_s_f5a_h              ),
  .rnd_ctl_f5a_h        ( txfma0_rnd_ctl_f5a_h        ),  
  .sel_gmaxu24_f5a_h    ( txfma0_sel_gmaxu24_f5a_h    ),
  .sel_gmaxu16_f5a_h    ( txfma0_sel_gmaxu16_f5a_h    ),
  .sel_gmaxu10_f5a_h    ( txfma0_sel_gmaxu10_f5a_h    ),
  .sel_gmaxu8_f5a_h     ( txfma0_sel_gmaxu8_f5a_h     ),
  .sel_gmaxu2_f5a_h     ( txfma0_sel_gmaxu2_f5a_h     ),
  .sel_gminany_f5a_h    ( txfma0_sel_gminany_f5a_h    ),
  .sel_gmaxs16_f5a_h    ( txfma0_sel_gmaxs16_f5a_h    ),
  .sel_gmins16_f5a_h    ( txfma0_sel_gmins16_f5a_h    ),
  .sel_gmaxs8_f5a_h     ( txfma0_sel_gmaxs8_f5a_h     ),
  .sel_gmins8_f5a_h     ( txfma0_sel_gmins8_f5a_h     ),
  .s_res_f5a_h          ( txfma0_s_res_f5a_h          ),
  .rnd_ovf_f5a_h        ( txfma0_rnd_ovf_f5a_h        )
);



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// F6 Stage
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


rst_en_ff 
#(
  .width( $bits({ txfma0_fma_data_f5a_h,
                  txfma0_fma_flags_f5a_h,
                  txfma0_comp_op_true_f5a_h }) )
)                        
F6_OUT_FF
(
  .clock      ( clock                       ), 
  .reset      ( reset                       ), 
  .en         ( txfma0_ctrl_valid_f5a_h     ), 
  .D          ( {txfma0_fma_data_f5a_h,
                 txfma0_fma_flags_f5a_h,
                 txfma0_comp_op_true_f5a_h} ),
  .Q          ( {txfma0_f6_out_data,
                 txfma0_comp_op_true_f6a_h} )
);

assign out_data_res = txfma0_f6_out_data;
assign out_comp_res = txfma0_comp_op_true_f6a_h;


endmodule
