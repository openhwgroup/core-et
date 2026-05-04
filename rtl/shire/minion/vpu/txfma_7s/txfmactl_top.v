// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfmactl_top
(
  input                                         clock,
  input                                         reset,
  // F0 Inputs
  input                                         in_valid_exa_h,
  input vpu_ctrl_sigs_t                         sigs_in_exa_h,
  input [`TXFMA_RM_SZ-1:0]                      rm_in_exa_h,
  input [`TRANS_MASK_SIZE-1:0]                  trans_mask_exa_h,
  input                                         trans_taylor_exa_h,
  input                                         trans_exp_fma2_exc_exa_h,
  input                                         use_prev_sigs_exa_h,
  input                                         sa_hi_f0a_h,
  input                                         sa_lo_f0a_h,
  input                                         sb_hi_f0a_h,
  input                                         sb_lo_f0a_h,
  input                                         sc_f0a_h,
  input                                         ea_hi_zero_f0a_l,
  input                                         ea_hi_all1_f0a_h,
  input                                         ea_lo_zero_f0a_l,
  input                                         ea_lo_all1_f0a_h,
  input                                         eb_hi_zero_f0a_l,
  input                                         eb_hi_all1_f0a_h,
  input                                         eb_lo_zero_f0a_l,
  input                                         eb_lo_all1_f0a_h,
  input                                         ec_zero_f0a_l,
  input                                         ec_all1_f0a_h,
  input                                         eb_eql_127_f0a_h,
  input                                         eb_eql_126_f0a_h,
  input                                         eb_eql_125_f0a_h,
  input                                         fcf10_zero_f0a_l,
  input                                         fcf11_zero_f0a_l,
  input                                         fcps_zero_f0a_h,
  input                                         fcph_zero_f0a_l,
  input                                         fcnorm_zero_29_8_f0a_h,
  input                                         fcnorm16_eq_1_f0a_h,
  input                                         fcnorm8_eq_1_f0a_h,
  input                                         fcnorm_30_f0a_h,
  input                                         faph_hi_b0_f0a_h,
  input                                         fbph_hi_b0_f0a_h,
  input                                         fcph_b0_f0a_h,
  input                                         fcf11_b0_f0a_h,
  input                                         fcf10_b0_f0a_h,
  input                                         fcn8_one_30_24_f0a_h,
  input                                         fcn10_one_30_22_f0a_h,
  input                                         fcn16_one_30_16_f0a_h,
  input                                         fcn24_one_30_8_f0a_h,
  input                                         faps_zero_f0a_l,
  input                                         fbps_zero_f0a_l,
  input                                         faph_hi_zero_f0a_l,
  input                                         fbph_hi_zero_f0a_l,
  input                                         faph_lo_zero_f0a_l,
  input                                         fbph_lo_zero_f0a_l,
  input                                         faph_lo_b0_f0a_h,
  input                                         fbph_lo_b0_f0a_h,
  input                                         faps_b0_f0a_h,
  input                                         fbps_b0_f0a_h,
  input                                         fc_31_f0a_h,
  input                                         fcps_b0_f0a_h,
  // F1 Inputs
  input  [`TXFMA_EXP_PS_SZ-1:0]                 ea_hi_f1a_h,
  // F2 Inputs
  input [`TXFMA_EXP_SEXT_PS_SZ-1:0]             ediff1_f2a_h,
  input [`TXFMA_EXP_SEXT_PS_SZ-1:0]             ediff1_abs_f2a_h,
  input [`TXFMA_EXP_SEXT_PS_SZ-1:0]             ediff2_abs_f2a_h,
  input [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]           ediff1_sel_f2a_h,
  input [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]           ediff2_sel_f2a_h,
  input [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]           align1_mux_sel_f2a_h,
  input [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]           align2_mux_sel_f2a_h,
  input [`TXFMA_EXP_PS_SZ-1:0]                  ec_f2a_h,
  input                                         fc_b0_f2a_h,
  input                                         e_sticky1_f2a_h,
  input                                         e_sticky2_f2a_h,
  input [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]           opd3_mux_sel_f2a_h,
  // F3 Inputs
  input  [`TXFMA_EXP_SEXT_PS_SZ-1:0]            ediff1_f3a_h,
  input  [`TXFMA_EXP_SEXT_PS_SZ-1:0]            ediff2_f3a_h,
  input  [`TXFMA_EXP_PS_SZ-1:0]                 ec_f3a_h,
  input                                         e_sticky1_f3a_h,
  input                                         prodsum_a2_f3a_h,
  input                                         fround_rbit_f3a_h,
  // F4 Inputs
  input                                         ph_comb_stiky_f4a_h,
  input [`TXFMA_LXD_PS_SZ-1:0]                  elxd_f4a_h,
  input [`TXFMA_WSPS_ETRZ_SZ-1:0]               etrz50_f4a_h,
  // F5 Input
  input                                         stky2g_f5a_h,  
  input                                         s_res_f5a_h,
  input                                         comb_add_a3_f5a_h,
  input                                         ediff1_zap_f5a_h,
  // F6 Inputs
  input                                         rmode_f6a_h,
  input                                         exp_res_zero_f6a_h, 
  input                                         s_res_f6a_h,
  input                                         ph_stky2l_f6a_h,
  input                                         ps_stky2l_f6a_h,
  input                                         int32_stky2l_f6a_h,
  input                                         pot_ovf_ps_f6a_h,
  input                                         def_ovf_ps_f6a_h,
  input                                         pot_unf_ps_f6a_h,
  input                                         def_unf_ps_f6a_h, 
  input                                         pot_ovf_ph_f6a_h,
  input                                         def_ovf_ph_f6a_h,
  input                                         pot_unf_ph_f6a_h,
  input                                         def_unf_ph_f6a_h, 
  input                                         frac_zero_f6a_h,
  input                                         rnd_l_ph_f6a_h,
  input                                         rnd_r_ph_f6a_h,
  input                                         rnd_l_ps_f6a_h,
  input                                         rnd_r_ps_f6a_h,
  input                                         int_res_l_bit_f6a_h,
  input                                         ph_allone2r_f6a_h,
  input                                         ph_allone2l_f6a_h,
  input                                         ps_allone2r_f6a_h,
  input                                         ps_allone2l_f6a_h, 
  // F0 Outputs
  output                                        is_trans_rfa_h, 
  output                                        sel_eblo_ph_exa_h,
  output                                        sel_ec5bdet_exa_h,
  output                                        elo_eb_en_exa_h,
  output                                        ehi_eb_en_exa_h,
  output                                        elo_ea_en_exa_h,
  output                                        fa_en_exa_h,
  output                                        fb_en_exa_h,
  output                                        fc_en_exa_h,
  output                                        sel_ealo_ph_exa_h,
  output                                        sa_hi_en_exa_h,
  output                                        sa_lo_en_exa_h,
  output                                        sb_hi_en_exa_h,
  output                                        sb_lo_en_exa_h,
  output                                        sc_en_exa_h,
  output                                        ehi_ea_en_exa_h,
  output                                        ec_en_exa_h,
  output                                        neg_fmaop_exa_h,
  output                                        trans_op2_exa_l,
  output                                        sa1_nr2_frcpfxp_exa_h,
  output                                        sel_sign_sinp2_exa_h,
  output                                        sa_zap_exa_l,
  output                                        sb_zap_exa_l,
  output                                        sel_f10_exa_h,
  output                                        sel_f11_exa_h,
  output                                        fc_dtps_exa_h,
  output                                        sel_snorm_exa_h,
  output                                        sel_g1sk2_exa_h,
  output                                        sel_g1sk8_exa_h,
  output                                        sel_g1sk16_exa_h,
  output                                        sel_g1sk24_exa_h,
  output                                        sel_xexpfrac_exa_h,
  output [1:0]                                  sel_ebin_hi_exa_h,
  output [1:0]                                  sel_eainmux_exa_h,
  output [1:0]                                  sel_ec5b_exa_h,
  output [1:0]                                  sel_ecin_exa_h,
  output [1:0]                                  sel_sc_exa_h,
  output [1:0]                                  sel_opcgfx_exa_h,
  output [1:0]                                  sel_xopc_exa_h,
  output [1:0]                                  sel_opcfmt_exa_h,
  output [1:0]                                  sel_transfa_exa_h,
  output [1:0]                                  sel_transfb_exa_h,
  output [1:0]                                  sel_opafmt_exa_h,
  output [1:0]                                  sel_opbfmt_exa_h,
  output logic                                  opa_valid_rfa_h,
  output logic                                  opb_valid_rfa_h,
  output logic                                  opc_valid_rfa_h,
  output logic                                  dtps_f0a_h,
  output logic                                  ctrl_valid_f0a_h,
  output logic                                  ctrl_valid_f1a_h,
  output logic                                  ctrl_valid_f2a_h,
  output logic                                  ctrl_valid_f3a_h,
  output logic                                  ctrl_valid_f4a_h,
  output logic                                  ctrl_valid_f5a_h,
  output logic                                  ctrl_valid_f6a_h,
  output logic                                  mul_sign_f0a_h,
  output logic                                  mul_hp_mode_f0a_h,
  output logic [5:0]                            eb_hi_k_exa_h,
  output logic                                  use_mul_f0a_h,    
  // F1 Outputs
  output logic                                  ec_zero_f1a_h,
  output logic                                  ea_hi_zero_f1a_h,
  output logic                                  fc_zero30_8_f1a_h,
  output logic                                  paddin1_sels_f1a_h,
  output logic                                  paddin2_selc_f1a_h,
  output logic                                  paddin2_selfcneg_f1a_h,
  output logic                                  padd_cin_f1a_h,
  output logic                                  sel_famask_f1a_h,
  output logic                                  fa_en_f1a_h,
  output logic [`VPU_FCMD_SZ-1:0]               cmd_f1a_h, 
  output logic [`TXFMA_SIGS_SZ-1:0]             sigs_f1a_h,
  output logic[`VPU_DTYPE_SZ-1:0]               op_dtype_f1a_h,
  output logic                                  use_mul_f1a_h,  
  // F2 Output
  output                                        ec_zero_f2a_h,
  output                                        eprod_hi_zero_f2a_h,
  output                                        eprod_lo_zero_f2a_h,
  output                                        op3p1c_sel_p_f2a_h,
  output                                        op3p1c_sel_r_f2a_h,
  output                                        op3g_sel_f2a_h,
  output                                        rm_rne_f2a_h,
  output                                        rm_rmm_f2a_h,
  output                                        frnd_rupnsc_f2a_h,
  output                                        frnd_rdnsc_f2a_h,
  output                                        frnd_t1_f2a_h,
  output                                        frnd_t2_f2a_h,
  output                                        al1c_zap_f2a_h,
  output                                        op1c_log_mul_clr_f2a_h,
  output                                        negop1c_int_logmul_clr_f2a_h,
  output                                        psc_op1_clr_f2a_h,
  output                                        align2_zap_f2a_h,
  output                                        align2c_neg_f2a_h,
  output                                        trans_exp_fma2_exc_f2a_h,
  output                                        al1p1c_sel_f2a_h,
  output                                        op2_sxtto_g_f2a_h,
  output [1:0]                                  op3p3spc_sel_f2a_h,
  output [2:0]                                  op3p2c_sel_f2a_h,
  output [1:0]                                  ugfxrplc_sel_f2a_h,
  output [1:0]                                  sgfxrplc_sel_f2a_h,
  output [1:0]                                  icgfxc_sel_f2a_h,
  output [1:0]                                  al1znp2c_sel_f2a_h,
  output [1:0]                                  al2c_sel_f2a_h,
  output logic [1:0]                            al1p2c_sel_f2a_h,
  output logic [`VPU_DTYPE_SZ-1:0]              op_dtype_f2a_h,
  output logic [`TXFMA_SIGS_SZ-1:0]             sigs_f2a_h,
  output logic [`VPU_FCMD_SZ-1:0]               cmd_f2a_h, 
  output logic                                  sin_ediff2_f2a_h,
  // F3 Outputs  
  output                                        fma_subopc3_f3a_h,
  output logic                                  s_res_f3a_h,
  output [1:0]                                  imxc_sel_f3a_h,
  output                                        al2c3_neg_cin_ps_f3a_h,
  output                                        al1c3_neg_cin_f3a_h,
  // F4 Outputs
  output                                        neg_norm_a3_f4a_h,
  output                                        neg_norm_na3_f4a_h,
  output                                        clearphps_en_f4a_h,
  output                                        clear16frnd_nshin_f4a_l,
  output                                        ph_ps_clear_f4a_l,
  output                                        elxd_add_in2_mux_sel_f4a_h,
  output                                        use_elxd_f4a_h,
  output                                        fb_hi_zero_f4a_h,
  output [`TXFMA_NORM_SHF_AMNT_SZ-1:0]          enc_nsamt_f4a_h,
  // F5 Outputs
  output logic                                  int_minmax_mux_sel_a_f5a_h,
  output logic                                  int_minmax_mux_sel_c_f5a_h,
  output                                        rc_ps_f5a_h,
  output                                        rc_ph_f5a_h,
  output                                        rc6_ps_f5a_h,
  output                                        rc6_ph_f5a_h,
  output                                        rc_int32_f5a_h,
  output                                        rc_3rmxr_f5a_h,
  output                                        rc_rl01_f5a_h,
  output [`TXFMA_RM_SZ-1:0]                     rm_f5a_h,
  output                                        rc_rmm_f5a_h,
  output                                        rc_rup_f5a_h,
  output                                        rc_rne_f5a_h,
  output                                        rc_rdn_f5a_h,
  output                                        rc_res_sign_f5a_l,
  output                                        rc_transrnd_f5a_h,
  output                                        rc_transrnd_log_fma2_f5a_h,
  output                                        nshcen_sel1b_f5a_h,
  output                                        en_logrrsel1b_f5a_l,
  output                                        int32_stky2r_f5a_h,
  output                                        mmx_sel_immres_f5a_h,
  output                                        comp_op_true_f5a_h,
  output                                        comp_op_f5a_h,
  output                                        trans2_ps_roundup_r_f5a_h,
  output logic                                  e_force_to_zero_f5a_h,
  output logic                                  s_res_out_f5a_h,
  output logic [`VPU_DTYPE_SZ-1:0]              op_dtype_f5a_h,
  output logic [`TXFMA_SIGS_SZ-1:0]             sigs_f5a_h,
  output logic [`VPU_FCMD_SZ-1:0]               cmd_f5a_h, 
// F6 Outputs
  output                                        sel_gmaxu24_f6a_h,
  output                                        sel_gmaxu16_f6a_h,
  output                                        sel_gmaxs16_f6a_h,
  output                                        sel_gmins16_f6a_h,
  output                                        sel_gmaxs8_f6a_h,
  output                                        sel_gmaxu2_f6a_h,
  output                                        sel_gmins8_f6a_h,
  output                                        sel_gmaxu8_f6a_h,
  output                                        sel_gfxmax_f6a_h,
  output                                        sel_psexc_sr_f6a_h,         
  output                                        sel_bit31_one_f6a_h,
  output                                        sel_b30to0_ones_f6a_h,         
  output                                        sel_zerores_f6a_h,      
  output                                        sel_ps_unf_f6a_h,         
  output                                        sel_ps_normone_f6a_h,   
  output                                        sel_ps_inf_f6a_h,      
  output                                        sel_ps_cqn_f6a_h,     
  output                                        sel_ps_large_f6a_h,  
  output                                        sel_phsp_f6a_h,  
  output                                        sel_phhi_sr_f6a_h,  
  output                                        sel_phhi_large_f6a_h,     
  output                                        sel_phhi_unf_f6a_h,      
  output                                        sel_phhi_infres_f6a_h,      
  output                                        sel_f11inf_f6a_h,     
  output                                        sel_f10cqnan_f6a_h,      
  output                                        sel_f11sp_f6a_h,      
  output                                        sel_f10sp_f6a_h,     
  output                                        sel_f11large_f6a_h,      
  output                                        sel_f10large_f6a_h,     
  output                                        ps_rndovf_f6a_h,
  output                                        ph_rndovf_f6a_h,
  output                                        sel_set_bit0_one_f6a_h,
  output                                        effsub1_set_sign1_f6a_h,
  output                                        normmask1_f6a_h,       
  output                                        normmask2_f6a_h, 
  output                                        sel_nr1_frcpres_f6a_h,
  output                                        trans_sinp2m_f6a_h,    
  output                                        f10f11mux_sel_f6a_h, 
  output                                        rrmux16l_sel_f6a_l,    
  output                                        sel_classify_out_f6a_h,
  output [1:0]                                  rrmux32_sel_f6a_h,      
  output [1:0]                                  sel_excreshi_f6a_h,
  output [1:0]                                  sel_excreslo_f6a_h,
  output [1:0]                                  sel_fmares_hi_f6a_h,
  output [1:0]                                  sel_fmares_lo_f6a_h,
  output [`VPU_FCMD_SZ-1:0]                     cmd_f6a_h,
  output logic [`TXFMA_SIGS_SZ-1:0]             sigs_f6a_h,
  output logic [`VPU_DTYPE_SZ-1:0]              op_dtype_f6a_h,
  output [`FP_CLASS_BITS-1:0]                   classify_f6a_h,
  output [`VPU_FLAGS_SZ-1:0]                    fma_flags_f6a_h,
  output                                        comp_op_true_f6a_h  
);

///////////////////////////////////////////////////////////////////////////////
// Internal logic
///////////////////////////////////////////////////////////////////////////////

logic [2:0]                                  sin_res_shf_idx_f0a_h;
logic [2:0]                                  sin_res_shf_idx_f1a_h;
logic [2:0]                                  sin_res_shf_idx_f2a_h;
logic [2:0]                                  sin_res_shf_idx_f3a_h;
logic [2:0]                                  sin_res_shf_idx_f4a_h;
logic [`FP_CLASS_BITS-1:0]                   classify_f0a_h;
logic [`FP_CLASS_BITS-1:0]                   classify_f1a_h;
logic [`FP_CLASS_BITS-1:0]                   classify_f2a_h;
logic [`FP_CLASS_BITS-1:0]                   classify_f3a_h;
logic [`FP_CLASS_BITS-1:0]                   classify_f4a_h;
logic [`FP_CLASS_BITS-1:0]                   classify_f5a_h;
logic [`TXFMA_RM_SZ-1:0]                     rm_f0a_h;
logic [`TXFMA_RM_SZ-1:0]                     rm_f1a_h;
logic [`TXFMA_RM_SZ-1:0]                     rm_f2a_h;
logic [`TXFMA_RM_SZ-1:0]                     rm_f3a_h;
logic [`TXFMA_RM_SZ-1:0]                     rm_f4a_h;
logic [`TXFMA_RM_SZ-1:0]                     rm_f6a_h;
logic                                        fc_zero30_8_f0a_h;
logic                                        fc_zero30_8_f2a_h;
logic                                        fb_hi_zero_f0a_h;
logic                                        fb_hi_zero_f1a_h;
logic                                        fb_hi_zero_f2a_h;
logic                                        fb_hi_zero_f3a_h;
logic                         	             res_inf_f0a_h;
logic                         	             res_inf_f1a_h;
logic                         	             res_inf_f2a_h;
logic                         	             res_inf_f3a_h;
logic                         	             res_inf_out_f3a_h;
logic                         	             res_inf_f4a_h;
logic                         	             res_inf_f5a_h;
logic                         	             res_inf_f6a_h;
logic                         	             res_neg_inf_out_f3a_h;
logic                         	             res_neg_inf_f4a_h;
logic                         	             res_neg_inf_f5a_h;
logic                         	             res_neg_inf_f6a_h;
logic                                        res_snan_f0a_h;
logic                                        res_snan_f1a_h;
logic                                        res_snan_f2a_h;
logic                                        res_snan_f3a_h;
logic                                        res_snan_out_f3a_h;
logic                                        res_snan_f4a_h;
logic                                        res_snan_f5a_h;
logic                                        res_snan_f6a_h;
logic                                        res_qnan_f0a_h;
logic                                        res_qnan_f1a_h;
logic                                        res_qnan_f2a_h;
logic                                        res_qnan_f3a_h;
logic                                        res_qnan_out_f3a_h;
logic                                        res_qnan_f4a_h;
logic                                        res_qnan_f5a_h;
logic                                        res_qnan_f6a_h;
logic                                        prod_inf_hi_f0a_h;
logic                                        prod_inf_hi_f1a_h;
logic                                        prod_inf_hi_f2a_h;
logic                                        prod_inf_hi_f3a_h;
logic                                        prod_inf_lo_f0a_h;
logic                                        prod_inf_lo_f1a_h;
logic                                        prod_inf_lo_f2a_h;
logic                                        prod_inf_lo_f3a_h;
logic                                        a_inf_f0a_h;
logic                                        a_inf_f1a_h;
logic                                        a_inf_f2a_h;
logic                                        a_inf_f3a_h;
logic                                        a_inf_f4a_h;
logic                                        a_inf_f5a_h;
logic                                        a_inf_f6a_h;
logic                                        c_inf_f0a_h;
logic                                        c_inf_f1a_h;
logic                                        c_inf_f2a_h;
logic                                        c_inf_f3a_h;
logic [`VPU_FLAGS_SZ-1:0]                    flags_f0a_h;
logic [`VPU_FLAGS_SZ-1:0]                    flags_f1a_h;
logic [`VPU_FLAGS_SZ-1:0]                    flags_f2a_h;
logic [`VPU_FLAGS_SZ-1:0]                    flags_f3a_h;
logic [`VPU_FLAGS_SZ-1:0]                    flags_f4a_h;
logic [`VPU_FLAGS_SZ-1:0]                    flags_f5a_h;
logic [`VPU_FLAGS_SZ-1:0]                    flags_f6a_h;
logic                                        flags_en_f0a_h;
logic                                        flags_en_f1a_h;
logic                                        flags_en_f2a_h;
logic                                        flags_en_f3a_h;
logic                                        flags_en_f4a_h;
logic                                        flags_en_f5a_h;
logic                                        flags_en_f6a_h;
logic                                        res_unorm_one_f0a_h;
logic                                        res_unorm_one_f1a_h;
logic                                        res_unorm_one_f2a_h;
logic                                        res_unorm_one_f3a_h;
logic                                        res_unorm_one_f4a_h;
logic                                        res_unorm_one_f5a_h;
logic                                        res_unorm_one_f6a_h;
logic                                        res_snorm_one_f0a_h;
logic                                        res_snorm_one_f1a_h;
logic                                        res_snorm_one_f2a_h;
logic                                        res_snorm_one_f3a_h;
logic                                        res_snorm_one_f4a_h;
logic                                        res_snorm_one_f5a_h;
logic                                        res_snorm_one_f6a_h;
logic                                        cvt_fui_pos_ovf_f2a_h;
logic                                        cvt_fui_pos_ovf_f3a_h;
logic                                        cvt_fui_pos_ovf_f4a_h;
logic                                        cvt_fui_pos_ovf_f5a_h;
logic                                        cvt_fui_pos_ovf_f6a_h;
logic                                        cvt_fi_pos_ovf_f2a_h;
logic                                        cvt_fi_pos_ovf_f3a_h;
logic                                        cvt_fi_pos_ovf_f4a_h;
logic                                        cvt_fi_pos_ovf_f5a_h;
logic                                        cvt_fi_pos_ovf_f6a_h;
logic                                        cvt_fi_neg_ovf_f2a_h;
logic                                        cvt_fi_neg_ovf_f3a_h;
logic                                        cvt_fi_neg_ovf_f4a_h;
logic                                        cvt_fi_neg_ovf_f5a_h;
logic                                        cvt_fi_neg_ovf_f6a_h;
logic                                        cvt_fi_min_neg_f2a_h;
logic                                        cvt_fi_min_neg_f3a_h;
logic                                        cvt_fi_min_neg_f4a_h;
logic                                        cvt_fi_min_neg_f5a_h;
logic                                        cvt_fi_min_neg_f6a_h;
logic                                        spec_res_en_f0a_h;
logic                                        spec_res_en_f1a_h;
logic                                        spec_res_en_f2a_h;
logic                                        spec_res_en_f3a_h;
logic                                        spec_res_en_f4a_h;
logic                                        spec_res_en_f5a_h;
logic                                        spec_res_en_f6a_h;
logic                                        ec_zero_clr_f0a_h;
logic                                        res_pos_int_nan_f0a_h;
logic                                        res_pos_int_nan_f1a_h;
logic                                        res_pos_int_nan_f2a_h;
logic                                        res_pos_int_nan_f3a_h;
logic                                        res_pos_int_nan_f4a_h;
logic                                        res_pos_int_nan_f5a_h;
logic                                        res_pos_int_nan_f6a_h;
logic                                        res_neg_int_nan_f0a_h;
logic                                        res_neg_int_nan_f1a_h;
logic                                        res_neg_int_nan_f2a_h;
logic                                        res_neg_int_nan_f3a_h;
logic                                        res_neg_int_nan_f4a_h;
logic                                        res_neg_int_nan_f5a_h;
logic                                        res_neg_int_nan_f6a_h;
logic                                        res_uint_nan_f0a_h;
logic                                        res_uint_nan_f1a_h;
logic                                        res_uint_nan_f2a_h;
logic                                        res_uint_nan_f3a_h;
logic                                        res_uint_nan_f4a_h;
logic                                        res_uint_nan_f5a_h;
logic                                        res_uint_nan_f6a_h;
logic                                        res_zero_f0a_h;
logic                                        res_zero_f1a_h;
logic                                        res_zero_f2a_h;
logic                                        res_zero_f3a_h;
logic                                        res_zero_f4a_h;
logic                                        res_zero_f5a_h;
logic                                        res_zero_f6a_h;
logic                                        res_one_f0a_h;
logic                                        res_one_f1a_h;
logic                                        res_one_f2a_h;
logic                                        res_one_f3a_h;
logic                                        res_one_f4a_h;
logic                                        res_one_f5a_h;
logic                                        res_one_f6a_h;
logic                         	             a_is_qnan_hi_f0a_h;
logic                         	             a_is_snan_hi_f0a_h;
logic                         	             a_is_qnan_hi_f1a_h;
logic                         	             a_is_snan_hi_f1a_h;
logic                                        a_is_qnan_hi_f2a_h;
logic                                        a_is_snan_hi_f2a_h;
logic                                        a_is_qnan_hi_f3a_h;
logic                                        a_is_snan_hi_f3a_h;
logic                         	             c_is_qnan_f0a_h;
logic                         	             c_is_snan_f0a_h;
logic                         	             c_is_qnan_f1a_h;
logic                         	             c_is_snan_f1a_h;
logic                                        c_is_qnan_f2a_h;
logic                                        c_is_snan_f2a_h; 
logic                                        c_is_qnan_f3a_h;
logic                                        c_is_snan_f3a_h;
logic                                        sa_hi_f1a_h;
logic                                        sa_lo_f1a_h;
logic                                        sb_hi_f1a_h;
logic                                        sb_lo_f1a_h;
logic                                        sc_f1a_h;
logic                                        sc_f2a_h;
logic                                        sc_f3a_h;
logic                                        sc_f4a_h;
logic                                        ec_zero_f3a_h;
logic                                        ec_zero_f4a_h;
logic                                        ec_zero_f5a_h;
logic                                        ec_zero_f6a_h;
logic                                        eprod_hi_zero_f1a_h;
logic                                        eprod_lo_zero_f1a_h;
logic                                        eprod_hi_zero_f3a_h;
logic                                        eprod_lo_zero_f3a_h;
logic                                        norm_f32_ovf_f1a_h;
logic                                        norm_f32_ovf_f2a_h;
logic                                        norm_f32_ovf_f3a_h;
logic                                        norm_f32_ovf_f4a_h;
logic                                        norm_f32_ovf_f5a_h;
logic                                        norm_f32_ovf_f6a_h;
logic                                        norm_one_out_f1a_h;
logic                                        norm_one_out_f2a_h;
logic                                        norm_one_out_f3a_h;
logic                                        norm_one_out_f4a_h;
logic                                        norm_one_out_f5a_h;
logic                                        norm_one_out_f6a_h;
logic                                        ediff1_lt_31_f2a_h;
logic                                        ediff1_lt_31_f3a_h;
logic                                        ediff1_lt_31_f4a_h;
logic                                        ediff1_lt_31_f5a_h;
logic                                        ediff1_lt_31_f6a_h;
logic                                        eff_sub1_f2a_h;
logic                                        eff_sub1_f3a_h;
logic                                        eff_sub1_out_f3a_h;
logic                                        eff_sub1_f4a_h;
logic                                        eff_sub1_f5a_h;
logic                                        eff_sub1_f6a_h;
logic                                        eff_sub2_f2a_h;
logic                                        eff_sub2_f3a_h;
logic                                        eff_sub2_f4a_h;
logic                                        sin_eff_sub1_f2a_h;
logic                                        sin_eff_sub1_f3a_h;
logic                                        cmp_invalid_f3a_h;
logic                                        cmp_invalid_f4a_h;
logic                                        cmp_invalid_f5a_h;
logic                                        cmp_invalid_f6a_h;
logic                                        fc_sel_f2a_h;
logic                                        fc_sel_f3a_h;
logic                                        fc_sel_f4a_h;
logic                                        fc_sel_f5a_h;
logic                                        fa_eq_fc_res_check_hi_f3a_h;
logic                                        fa_eq_fc_res_check_hi_f4a_h;
logic                                        fa_eq_fc_res_check_hi_f5a_h;
logic                                        comp_res_sel_f3a_h;
logic                                        comp_res_sel_f4a_h;
logic                                        comp_res_sel_f5a_h;
logic                                        trans_log_rr_neg_f2a_h;
logic                                        trans_log_rr_neg_f3a_h;
logic                                        trans_log_rr_neg_f4a_h;
logic                                        trans_log_rr_neg_f5a_h;
logic                                        s_res_check_cadd_a3_f3a_h;
logic                                        s_res_check_cadd_a3_f4a_h;
logic                                        s_res_check_cadd_a3_f5a_h;
logic                                        prod_sign_hi_f1a_h;    
logic                                        prod_sign_hi_f2a_h; 
logic                                        prod_sign_hi_f3a_h;
logic                                        prod_sign_hi_f4a_h;
logic                                        prod_sign_hi_f5a_h;
logic                                        prod_sign_lo_f1a_h;    
logic                                        prod_sign_lo_f2a_h;
logic                                        prod_sign_lo_f3a_h;
logic                                        fa_hi_zero_f0a_h;
logic                                        fa_hi_zero_f1a_h;
logic                                        trans_exp_fma2_exc_f0a_h;
logic                                        trans_exp_fma2_exc_f1a_h;
logic  [`VPU_FCMD_SZ-1:0]                    cmd_f0a_h;  
logic  [`VPU_FCMD_SZ-1:0]                    cmd_in_f1a_h;  
logic  [`VPU_FCMD_SZ-1:0]                    cmd_f3a_h;  
logic  [`TXFMA_SIGS_SZ-1:0]                  sigs_f0a_h;
logic  [`TXFMA_SIGS_SZ-1:0]                  sigs_f3a_h;
logic  [`VPU_DTYPE_SZ-1:0]                   op_dtype_f0a_h;
logic  [`VPU_DTYPE_SZ-1:0]                   op_dtype_f3a_h;
logic                                        ediff1_0_f2a_h;
logic                                        ediff1_0_f3a_h;
logic                                        fa_ge_fc_comp_sign_hi_res_f2a_h;
logic                                        fa_eq_fc_comp_sign_hi_res_f2a_h;
logic                                        fa_ge_fc_comp_sign_hi_res_f3a_h;
logic                                        fa_eq_fc_comp_sign_hi_res_f3a_h;
logic                                        int_minmax_mux_sel_fc_f2a_h;
logic                                        int_minmax_mux_sel_fc_f3a_h;
logic                                        eff_sign1_f2a_h;
logic                                        eff_sign1_f3a_h;
logic                                        ea_hi_zero_clr_f0a_h;
logic                                        ea_lo_zero_clr_f0a_h;
logic                                        eb_hi_zero_clr_f0a_h;
logic                                        eb_lo_zero_clr_f0a_h;
logic                                        ea_lo_zero_f1a_h;
logic                                        eb_hi_zero_f1a_h;
logic                                        eb_lo_zero_f1a_h;
logic                                        minmax_op_f1a_h;
logic                                        minmax_op_f2a_h;
logic                                        minmax_op_f3a_h;
logic                                        minmax_op_f4a_h;
logic                                        minmax_op_f5a_h;
logic                                        minmax_op_f6a_h;
logic                                        sel_fcmask_f1a_h;
logic                                        sel_fcmask_f2a_h;
logic                                        s_res_f4a_h;
logic                                        int_minmax_mux_sel_a_f3a_h;
logic                                        int_minmax_mux_sel_c_f3a_h;
logic                                        int_minmax_mux_sel_a_f4a_h;
logic                                        int_minmax_mux_sel_c_f4a_h;
logic                                        comp_op_f2a_h;
logic                                        comp_op_f3a_h;
logic                                        comp_op_f4a_h;
logic                                        comp_op_f6a_h;
logic [`TXFMA_LXD_PS_SZ-1:0]                 elxd_f5a_h;
logic [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]          opd3_mux_sel_f3a_h;
logic                                        rc_int32_f6a_h;
logic                                        sin_align1_neg_f3a_h;
logic                                        sin_align2_neg_f3a_h;
logic                                        sin_align1_neg_f2a_h;
logic                                        sin_align2_neg_f2a_h;
logic                                        sin_check_fc_sign_f3a_h;
logic                                        sin_check_fc_sign_f2a_h;
logic                                        rc_ps_f6a_h;
logic                                        rc_ph_f6a_h;
logic [`TXFMA_WSPS_ETRZ_SZ-1:0]              etrz50_f5a_h;
logic                                        use_prev_sigs_f0a_h;
logic                                        use_prev_sigs_f1a_h;
logic                                        use_prev_sigs_f2a_h;
logic                                        use_prev_sigs_f3a_h;
logic                                        use_prev_sigs_f4a_h;
logic                                        use_prev_sigs_f5a_h;
logic                                        fround_stky2l_f3a_h;
logic                                        fround_stky2l_f4a_h;
logic                                        fround_stky2l_f5a_h;
logic                                        fround_stky2l_f6a_h;
logic                                        flog_mul_prod_zero_f0a_h;
logic                                        flog_mul_prod_zero_f1a_h;
logic                                        al2c3_neg_cin_ps_f2a_h;
logic                                        al1c3_neg_cin_f2a_h;
logic [`VPU_DTYPE_SZ-1:0]                    op_dtype_f4a_h;
logic [`TXFMA_SIGS_SZ-1:0]                   sigs_f4a_h;
logic [`VPU_FCMD_SZ-1:0]                     cmd_f4a_h; 


///////////////////////////////////////////////////////////////////////////////
// C0 component
///////////////////////////////////////////////////////////////////////////////
txfma_c0 txfma0_c0 
(
  .clock                          ( clock                        ),
  .reset                          ( reset                        ),
  .in_valid_exa_h                 ( in_valid_exa_h               ),
  .sigs_in_exa_h                  ( sigs_in_exa_h                ),
  .rm_in_exa_h                    ( rm_in_exa_h                  ),
  .use_prev_sigs_exa_h            ( use_prev_sigs_exa_h          ),
  .ea_hi_zero_f0a_l               ( ea_hi_zero_f0a_l             ),
  .ea_hi_all1_f0a_h               ( ea_hi_all1_f0a_h             ),
  .ea_lo_zero_f0a_l               ( ea_lo_zero_f0a_l             ),
  .ea_lo_all1_f0a_h               ( ea_lo_all1_f0a_h             ),
  .eb_hi_zero_f0a_l               ( eb_hi_zero_f0a_l             ),
  .eb_hi_all1_f0a_h               ( eb_hi_all1_f0a_h             ),
  .eb_lo_zero_f0a_l               ( eb_lo_zero_f0a_l             ),
  .eb_lo_all1_f0a_h               ( eb_lo_all1_f0a_h             ),
  .ec_zero_f0a_l                  ( ec_zero_f0a_l                ),
  .ec_all1_f0a_h                  ( ec_all1_f0a_h                ),
  .eb_eql_127_f0a_h               ( eb_eql_127_f0a_h             ),
  .eb_eql_126_f0a_h               ( eb_eql_126_f0a_h             ),
  .eb_eql_125_f0a_h               ( eb_eql_125_f0a_h             ),
  .fcf10_zero_f0a_l               ( fcf10_zero_f0a_l             ),
  .fcf11_zero_f0a_l               ( fcf11_zero_f0a_l             ),
  .fcps_zero_f0a_h                ( fcps_zero_f0a_h              ),
  .fcph_zero_f0a_l                ( fcph_zero_f0a_l              ),
  .fcnorm_zero_29_8_f0a_h         ( fcnorm_zero_29_8_f0a_h       ),
  .fcnorm16_eq_1_f0a_h            ( fcnorm16_eq_1_f0a_h          ),
  .fcnorm8_eq_1_f0a_h             ( fcnorm8_eq_1_f0a_h           ),
  .fcnorm_30_f0a_h                ( fcnorm_30_f0a_h              ),
  .faph_hi_b0_f0a_h               ( faph_hi_b0_f0a_h             ),
  .fbph_hi_b0_f0a_h               ( fbph_hi_b0_f0a_h             ),
  .fcph_b0_f0a_h                  ( fcph_b0_f0a_h                ),
  .fcf11_b0_f0a_h                 ( fcf11_b0_f0a_h               ),
  .fcf10_b0_f0a_h                 ( fcf10_b0_f0a_h               ),
  .fcn8_one_30_24_f0a_h           ( fcn8_one_30_24_f0a_h         ),
  .fcn10_one_30_22_f0a_h          ( fcn10_one_30_22_f0a_h        ),
  .fcn16_one_30_16_f0a_h          ( fcn16_one_30_16_f0a_h        ),
  .fcn24_one_30_8_f0a_h           ( fcn24_one_30_8_f0a_h         ),
  .faps_zero_f0a_l                ( faps_zero_f0a_l              ),
  .fbps_zero_f0a_l                ( fbps_zero_f0a_l              ),
  .faph_hi_zero_f0a_l             ( faph_hi_zero_f0a_l           ),
  .fbph_hi_zero_f0a_l             ( fbph_hi_zero_f0a_l           ),
  .faph_lo_zero_f0a_l             ( faph_lo_zero_f0a_l           ),
  .fbph_lo_zero_f0a_l             ( fbph_lo_zero_f0a_l           ),
  .faph_lo_b0_f0a_h               ( faph_lo_b0_f0a_h             ),
  .fbph_lo_b0_f0a_h               ( fbph_lo_b0_f0a_h             ),
  .faps_b0_f0a_h                  ( faps_b0_f0a_h                ),
  .fbps_b0_f0a_h                  ( fbps_b0_f0a_h                ),
  .fc_31_f0a_h                    ( fc_31_f0a_h                  ),
  .fcps_b0_f0a_h                  ( fcps_b0_f0a_h                ),
  .trans_taylor_exa_h             ( trans_taylor_exa_h           ),
  .trans_exp_fma2_exc_exa_h       ( trans_exp_fma2_exc_exa_h     ),
  .trans_exp_fma2_exc_f0a_h       ( trans_exp_fma2_exc_f0a_h     ),
  .trans_mask_exa_h               ( trans_mask_exa_h             ),
  .sa_hi_f0a_h                    ( sa_hi_f0a_h                  ),
  .sa_lo_f0a_h                    ( sa_lo_f0a_h                  ),
  .sb_hi_f0a_h                    ( sb_hi_f0a_h                  ),
  .sb_lo_f0a_h                    ( sb_lo_f0a_h                  ),
  .sc_f0a_h                       ( sc_f0a_h                     ),
  .opa_valid_rfa_h                ( opa_valid_rfa_h              ),
  .opb_valid_rfa_h                ( opb_valid_rfa_h              ),
  .opc_valid_rfa_h                ( opc_valid_rfa_h              ),
  .ctrl_valid_f0a_h               ( ctrl_valid_f0a_h             ),
  .is_trans_rfa_h                 ( is_trans_rfa_h               ),
  .eb_hi_k_exa_h                  ( eb_hi_k_exa_h                ),
  .sel_eblo_ph_exa_h              ( sel_eblo_ph_exa_h            ),
  .sel_ec5bdet_exa_h              ( sel_ec5bdet_exa_h            ),
  .sel_ebin_hi_exa_h              ( sel_ebin_hi_exa_h            ),
  .elo_eb_en_exa_h                ( elo_eb_en_exa_h              ),
  .ehi_eb_en_exa_h                ( ehi_eb_en_exa_h              ),
  .elo_ea_en_exa_h                ( elo_ea_en_exa_h              ),
  .fa_en_exa_h                    ( fa_en_exa_h                  ),
  .fb_en_exa_h                    ( fb_en_exa_h                  ),
  .fc_en_exa_h                    ( fc_en_exa_h                  ),
  .sel_ealo_ph_exa_h              ( sel_ealo_ph_exa_h            ),
  .sel_eainmux_exa_h              ( sel_eainmux_exa_h            ),
  .sel_ec5b_exa_h                 ( sel_ec5b_exa_h               ),
  .sel_ecin_exa_h                 ( sel_ecin_exa_h               ),
  .sa_hi_en_exa_h                 ( sa_hi_en_exa_h               ),
  .sa_lo_en_exa_h                 ( sa_lo_en_exa_h               ),
  .sb_hi_en_exa_h                 ( sb_hi_en_exa_h               ),
  .sb_lo_en_exa_h                 ( sb_lo_en_exa_h               ),
  .sc_en_exa_h                    ( sc_en_exa_h                  ),
  .ehi_ea_en_exa_h                ( ehi_ea_en_exa_h              ),
  .ec_en_exa_h                    ( ec_en_exa_h                  ),
  .neg_fmaop_exa_h                ( neg_fmaop_exa_h              ),
  .trans_op2_exa_l                ( trans_op2_exa_l              ),
  .sa1_nr2_frcpfxp_exa_h          ( sa1_nr2_frcpfxp_exa_h        ),
  .sel_sc_exa_h                   ( sel_sc_exa_h                 ),
  .sa_zap_exa_l                   ( sa_zap_exa_l                 ),
  .sb_zap_exa_l                   ( sb_zap_exa_l                 ),
  .sel_sign_sinp2_exa_h           ( sel_sign_sinp2_exa_h         ),
  .sel_f10_exa_h                  ( sel_f10_exa_h                ),
  .sel_f11_exa_h                  ( sel_f11_exa_h                ),
  .sel_opcgfx_exa_h               ( sel_opcgfx_exa_h             ),
  .fc_dtps_exa_h                  ( fc_dtps_exa_h                ),
  .sel_xopc_exa_h                 ( sel_xopc_exa_h               ),
  .sel_opcfmt_exa_h               ( sel_opcfmt_exa_h             ),
  .sel_snorm_exa_h                ( sel_snorm_exa_h              ),
  .sel_g1sk2_exa_h                ( sel_g1sk2_exa_h              ),
  .sel_g1sk8_exa_h                ( sel_g1sk8_exa_h              ),
  .sel_g1sk16_exa_h               ( sel_g1sk16_exa_h             ),
  .sel_g1sk24_exa_h               ( sel_g1sk24_exa_h             ),
  .sel_xexpfrac_exa_h             ( sel_xexpfrac_exa_h           ),
  .sel_transfa_exa_h              ( sel_transfa_exa_h            ),
  .sel_transfb_exa_h              ( sel_transfb_exa_h            ),
  .sel_opafmt_exa_h               ( sel_opafmt_exa_h             ),
  .sel_opbfmt_exa_h               ( sel_opbfmt_exa_h             ),
  .mul_sign_f0a_h                 ( mul_sign_f0a_h               ),
  .mul_hp_mode_f0a_h              ( mul_hp_mode_f0a_h            ),
  .op_dtype_f0a_h                 ( op_dtype_f0a_h               ),
  .cmd_f0a_h                      ( cmd_f0a_h                    ),
  .sigs_f0a_h                     ( sigs_f0a_h                   ),
  .use_prev_sigs_f0a_h            ( use_prev_sigs_f0a_h          ),
  .rm_f0a_h                       ( rm_f0a_h                     ),
  .sin_res_shf_idx_f0a_h          ( sin_res_shf_idx_f0a_h        ),
  .flog_mul_prod_zero_f0a_h       ( flog_mul_prod_zero_f0a_h     ),
  .res_inf_f0a_h                  ( res_inf_f0a_h              	 ),
  .res_snan_f0a_h                 ( res_snan_f0a_h               ),
  .res_qnan_f0a_h                 ( res_qnan_f0a_h               ),
  .classify_f0a_h                 ( classify_f0a_h               ),
  .prod_inf_hi_f0a_h              ( prod_inf_hi_f0a_h        	 ),
  .c_inf_f0a_h                    ( c_inf_f0a_h                  ),
  .a_inf_f0a_h                    ( a_inf_f0a_h                  ),
  .flags_f0a_h                 ( flags_f0a_h           	 ),
  .a_is_qnan_hi_f0a_h             ( a_is_qnan_hi_f0a_h       	 ),
  .c_is_qnan_f0a_h                ( c_is_qnan_f0a_h       	     ),
  .a_is_snan_hi_f0a_h             ( a_is_snan_hi_f0a_h       	 ),
  .c_is_snan_f0a_h                ( c_is_snan_f0a_h              ),
  .res_pos_int_nan_f0a_h          ( res_pos_int_nan_f0a_h        ),
  .res_neg_int_nan_f0a_h          ( res_neg_int_nan_f0a_h        ),
  .fc_zero30_8_f0a_h              ( fc_zero30_8_f0a_h            ),
  .res_uint_nan_f0a_h             ( res_uint_nan_f0a_h           ),
  .res_zero_f0a_h                 ( res_zero_f0a_h               ),
  .res_one_f0a_h                  ( res_one_f0a_h                ),
  .prod_inf_lo_f0a_h              ( prod_inf_lo_f0a_h            ),
  .ea_hi_zero_clr_f0a_h           ( ea_hi_zero_clr_f0a_h         ),
  .ea_lo_zero_clr_f0a_h           ( ea_lo_zero_clr_f0a_h         ),
  .eb_hi_zero_clr_f0a_h           ( eb_hi_zero_clr_f0a_h         ),
  .eb_lo_zero_clr_f0a_h           ( eb_lo_zero_clr_f0a_h         ),
  .fa_hi_zero_f0a_h               ( fa_hi_zero_f0a_h             ),
  .fb_hi_zero_f0a_h               ( fb_hi_zero_f0a_h             ),
  .res_unorm_one_f0a_h            ( res_unorm_one_f0a_h          ),  
  .res_snorm_one_f0a_h            ( res_snorm_one_f0a_h          ),
  .flags_en_f0a_h                 ( flags_en_f0a_h               ),
  .dtps_f0a_h                     ( dtps_f0a_h                   ),
  .spec_res_en_f0a_h              ( spec_res_en_f0a_h            ),
  .ec_zero_clr_f0a_h              ( ec_zero_clr_f0a_h            ),
  .use_mul_f0a_h                  ( use_mul_f0a_h                )  
);


///////////////////////////////////////////////////////////////////////////////
// F1FF
///////////////////////////////////////////////////////////////////////////////

rst_ff
#(
  .width($bits( ctrl_valid_f0a_h    ))
)
F1_CTRLVAL_FF
(
  .clock      ( clock                ),
  .reset      ( reset                ),
  .D          ( ctrl_valid_f0a_h     ),
  .Q          ( ctrl_valid_f1a_h     )
);

en_ff 
#(
  .width($bits( {trans_exp_fma2_exc_f0a_h,
                 sin_res_shf_idx_f0a_h,
                 flog_mul_prod_zero_f0a_h} ) )
)                        
F1_TRANS_FF
(
  .clock      ( clock                      ), 
  .en         ( ctrl_valid_f0a_h           ),
  .D          ( {trans_exp_fma2_exc_f0a_h,
                 sin_res_shf_idx_f0a_h,
                 flog_mul_prod_zero_f0a_h}    ),
  .Q          ( {trans_exp_fma2_exc_f1a_h,
                 sin_res_shf_idx_f1a_h,
                flog_mul_prod_zero_f1a_h}    )
);

en_ff 
#(
  .width($bits( {sa_hi_f0a_h,
                 sa_lo_f0a_h,
                 sb_hi_f0a_h,
                 sb_lo_f0a_h,
                 sc_f0a_h})       )
)                        
S1A_FF
(
  .clock      ( clock             ), 
  .en         ( ctrl_valid_f0a_h  ),
  .D          ( {sa_hi_f0a_h,
                 sa_lo_f0a_h,
                 sb_hi_f0a_h,
                 sb_lo_f0a_h,
                 sc_f0a_h}        ),
  .Q          ( {sa_hi_f1a_h,
                 sa_lo_f1a_h,
                 sb_hi_f1a_h,
                 sb_lo_f1a_h,
                 sc_f1a_h}        )
);

en_ff 
#(
  .width($bits( {op_dtype_f0a_h, 
                 cmd_f0a_h, 
                 sigs_f0a_h, 
                 rm_f0a_h})  )
)                        
F1_CTRL_FF
(
  .clock      ( clock                        ), 
  .en         ( ctrl_valid_f0a_h & 
                !use_prev_sigs_f0a_h         ),
  .D          ( {op_dtype_f0a_h, 
                 cmd_f0a_h, 
                 sigs_f0a_h, 
                 rm_f0a_h}   ),  
  .Q          ( {op_dtype_f1a_h, 
                 cmd_in_f1a_h, 
                 sigs_f1a_h, 
                 rm_f1a_h}   )
);



en_ff 
#(
  .width( $bits(use_prev_sigs_f0a_h))
)                        
F1_PREV_CTRL_FF
(
  .clock      ( clock               ), 
  .en         ( ctrl_valid_f0a_h    ),
  .D          ( use_prev_sigs_f0a_h ),
  .Q          ( use_prev_sigs_f1a_h )
);


en_ff 
#(
  .width($bits( {res_inf_f0a_h,
                 res_snan_f0a_h,
                 res_qnan_f0a_h,
                 res_zero_f0a_h,
                 res_one_f0a_h,
                 ec_zero_clr_f0a_h,
                 ea_hi_zero_clr_f0a_h,
                 ea_lo_zero_clr_f0a_h,
                 eb_hi_zero_clr_f0a_h,
                 eb_lo_zero_clr_f0a_h,
                 classify_f0a_h,
                 prod_inf_hi_f0a_h,
                 a_inf_f0a_h,
                 c_inf_f0a_h,
                 flags_f0a_h,
                 a_is_qnan_hi_f0a_h,
                 c_is_qnan_f0a_h,
                 a_is_snan_hi_f0a_h,
                 c_is_snan_f0a_h,
                 res_pos_int_nan_f0a_h,
                 res_neg_int_nan_f0a_h,
                 fc_zero30_8_f0a_h,
                 res_uint_nan_f0a_h,
                 prod_inf_lo_f0a_h,
                 fa_hi_zero_f0a_h,
                 fb_hi_zero_f0a_h,
                 res_unorm_one_f0a_h,  
                 res_snorm_one_f0a_h,
                 flags_en_f0a_h,
                 spec_res_en_f0a_h} ))
)
F1_SPEC_DET_FF
(
  .clock      ( clock                  ),
  .en         ( ctrl_valid_f0a_h       ),
  .D          ( {res_inf_f0a_h,
                 res_snan_f0a_h,
                 res_qnan_f0a_h,
                 res_zero_f0a_h,
                 res_one_f0a_h,
                 ec_zero_clr_f0a_h,
                 ea_hi_zero_clr_f0a_h,
                 ea_lo_zero_clr_f0a_h,
                 eb_hi_zero_clr_f0a_h,
                 eb_lo_zero_clr_f0a_h,
                 classify_f0a_h,
                 prod_inf_hi_f0a_h,
                 a_inf_f0a_h,
                 c_inf_f0a_h,
                 flags_f0a_h,
                 a_is_qnan_hi_f0a_h,
                 c_is_qnan_f0a_h,
                 a_is_snan_hi_f0a_h,
                 c_is_snan_f0a_h,
                 res_pos_int_nan_f0a_h,
                 res_neg_int_nan_f0a_h,
                 fc_zero30_8_f0a_h,
                 res_uint_nan_f0a_h,
                 prod_inf_lo_f0a_h,
                 fa_hi_zero_f0a_h,
                 fb_hi_zero_f0a_h,
                 res_unorm_one_f0a_h,  
                 res_snorm_one_f0a_h,
                 flags_en_f0a_h,
                 spec_res_en_f0a_h}    ),
  .Q          ( {res_inf_f1a_h,
                 res_snan_f1a_h,
                 res_qnan_f1a_h,
                 res_zero_f1a_h,
                 res_one_f1a_h,
                 ec_zero_f1a_h,
                 ea_hi_zero_f1a_h,
                 ea_lo_zero_f1a_h,
                 eb_hi_zero_f1a_h,
                 eb_lo_zero_f1a_h,
                 classify_f1a_h,
                 prod_inf_hi_f1a_h,
                 a_inf_f1a_h,
                 c_inf_f1a_h,
                 flags_f1a_h,
                 a_is_qnan_hi_f1a_h,
                 c_is_qnan_f1a_h,
                 a_is_snan_hi_f1a_h,
                 c_is_snan_f1a_h,
                 res_pos_int_nan_f1a_h,
                 res_neg_int_nan_f1a_h,
                 fc_zero30_8_f1a_h,
                 res_uint_nan_f1a_h,
                 prod_inf_lo_f1a_h,
                 fa_hi_zero_f1a_h,
                 fb_hi_zero_f1a_h,
                 res_unorm_one_f1a_h,  
                 res_snorm_one_f1a_h,
                 flags_en_f1a_h,
                 spec_res_en_f1a_h}    )
);

///////////////////////////////////////////////////////////////////////////////
// C1 component
///////////////////////////////////////////////////////////////////////////////
txfma_c1 txfma0_c1 
(
  .ctrl_valid_f1a_h          ( ctrl_valid_f1a_h        ),
  .sigs_f1a_h                ( sigs_f1a_h              ),
  .op_dtype_f1a_h            ( op_dtype_f1a_h          ),
  .cmd_in_f1a_h              ( cmd_in_f1a_h            ),
  .ea_hi_f1a_h               ( ea_hi_f1a_h             ),
  .fa_hi_zero_f1a_h          ( fa_hi_zero_f1a_h        ),
  .sa_hi_f1a_h               ( sa_hi_f1a_h             ),
  .sb_hi_f1a_h               ( sb_hi_f1a_h             ),
  .sc_f1a_h                  ( sc_f1a_h                ),
  .sa_lo_f1a_h               ( sa_lo_f1a_h             ),
  .sb_lo_f1a_h               ( sb_lo_f1a_h             ),
  .ea_hi_zero_f1a_h          ( ea_hi_zero_f1a_h        ),
  .eb_hi_zero_f1a_h          ( eb_hi_zero_f1a_h        ),
  .ea_lo_zero_f1a_h          ( ea_lo_zero_f1a_h        ),
  .eb_lo_zero_f1a_h          ( eb_lo_zero_f1a_h        ),
  .flog_mul_prod_zero_f1a_h  ( flog_mul_prod_zero_f1a_h ),
  .prod_sign_hi_f1a_h        ( prod_sign_hi_f1a_h      ),
  .prod_sign_lo_f1a_h        ( prod_sign_lo_f1a_h      ),
  .eprod_hi_zero_f1a_h       ( eprod_hi_zero_f1a_h     ),
  .eprod_lo_zero_f1a_h       ( eprod_lo_zero_f1a_h     ),
  .norm_f32_ovf_f1a_h        ( norm_f32_ovf_f1a_h      ),
  .norm_one_out_f1a_h        ( norm_one_out_f1a_h      ),
  .paddin1_sels_f1a_h        ( paddin1_sels_f1a_h      ),
  .paddin2_selc_f1a_h        ( paddin2_selc_f1a_h      ),
  .paddin2_selfcneg_f1a_h    ( paddin2_selfcneg_f1a_h  ),
  .padd_cin_f1a_h            ( padd_cin_f1a_h          ),
  .sel_famask_f1a_h          ( sel_famask_f1a_h        ),
  .sel_fcmask_f1a_h          ( sel_fcmask_f1a_h        ),
  .fa_en_f1a_h               ( fa_en_f1a_h             ),
  .minmax_op_f1a_h           ( minmax_op_f1a_h         ),
  .cmd_f1a_h                 ( cmd_f1a_h               ),
  .use_mul_f1a_h             ( use_mul_f1a_h           )
);



///////////////////////////////////////////////////////////////////////////////
// F2FF
///////////////////////////////////////////////////////////////////////////////

rst_ff
#(
  .width($bits( ctrl_valid_f1a_h    ))
)
F2_CTRLVAL_FF
(
  .clock      ( clock                ),
  .reset      ( reset                ),
  .D          ( ctrl_valid_f1a_h     ),
  .Q          ( ctrl_valid_f2a_h     )
);

en_ff 
#(
  .width($bits( sin_res_shf_idx_f1a_h ) )
)                        
F2_IDX_FF
(
  .clock      ( clock                      ), 
  .en         ( ctrl_valid_f1a_h           ),
  .D          ( sin_res_shf_idx_f1a_h      ),
  .Q          ( sin_res_shf_idx_f2a_h      )
);

en_ff 
#(
  .width($bits( {prod_sign_hi_f1a_h, 
                 prod_sign_lo_f1a_h, 
                 sc_f1a_h}        ))
)                        
S2_FF
(
  .clock      ( clock                       ),
  .en         ( ctrl_valid_f1a_h     ),
  .D          ( {prod_sign_hi_f1a_h, 
                 prod_sign_lo_f1a_h, 
                 sc_f1a_h}           ),
  .Q          ( {prod_sign_hi_f2a_h, 
                 prod_sign_lo_f2a_h, 
                 sc_f2a_h}           )
);

en_ff 
#(
  .width($bits( {op_dtype_f1a_h,
                 cmd_f1a_h, 
                 sigs_f1a_h, 
                 rm_f1a_h,
                 minmax_op_f1a_h} ) )
)                        
F2_CTRL_FF
(
  .clock      ( clock                        ), 
  .en         ( ctrl_valid_f1a_h &
                !use_prev_sigs_f1a_h         ),
  .D          ( {op_dtype_f1a_h,
                 cmd_f1a_h, 
                 sigs_f1a_h, 
                 rm_f1a_h,
                 minmax_op_f1a_h }   ),
  .Q          ( {op_dtype_f2a_h, 
                 cmd_f2a_h, 
                 sigs_f2a_h, 
                 rm_f2a_h,
                 minmax_op_f2a_h }   )
);

en_ff 
#(
  .width($bits( trans_exp_fma2_exc_f1a_h ) )
)                        
F2_NO_PREV_CTRL_FF
(
  .clock      ( clock                    ), 
  .en         ( ctrl_valid_f1a_h         ),
  .D          ( trans_exp_fma2_exc_f1a_h ),
  .Q          ( trans_exp_fma2_exc_f2a_h )
);


en_ff 
#(
  .width( $bits(use_prev_sigs_f1a_h ))
)                        
F2_PREV_CTRL_FF
(
  .clock      ( clock                ), 
  .en         ( ctrl_valid_f1a_h     ),
  .D          ( use_prev_sigs_f1a_h  ),
  .Q          ( use_prev_sigs_f2a_h  )
);



en_ff 
#(
  .width($bits( {ec_zero_f1a_h,
                 eprod_hi_zero_f1a_h,
                 eprod_lo_zero_f1a_h,
                 sel_fcmask_f1a_h,
                 fc_zero30_8_f1a_h,
                 fb_hi_zero_f1a_h})     )
)
F2_ZER0_FF
(
  .clock      ( clock                   ), 
  .en         ( ctrl_valid_f1a_h        ),
  .D          ( {ec_zero_f1a_h,
                 eprod_hi_zero_f1a_h,
                 eprod_lo_zero_f1a_h,
                 sel_fcmask_f1a_h,
                 fc_zero30_8_f1a_h,
                 fb_hi_zero_f1a_h}      ),
  .Q          ( {ec_zero_f2a_h, 
                 eprod_hi_zero_f2a_h,
                 eprod_lo_zero_f2a_h,
                 sel_fcmask_f2a_h,
                 fc_zero30_8_f2a_h,
                 fb_hi_zero_f2a_h}      )
);

en_ff 
#(
  .width($bits( {res_inf_f1a_h, 
                 res_snan_f1a_h, 
                 res_qnan_f1a_h, 
                 res_zero_f1a_h, 
                 res_one_f1a_h, 
                 classify_f1a_h,
                 res_unorm_one_f1a_h,
                 res_snorm_one_f1a_h,
                 res_pos_int_nan_f1a_h,
                 res_neg_int_nan_f1a_h,
                 res_uint_nan_f1a_h}) )
)
F2_SPEC_FF
(
  .clock      ( clock                          ), 
  .en         ( ctrl_valid_f1a_h               ),
  .D          ( {res_inf_f1a_h, 
                 res_snan_f1a_h,  
                 res_qnan_f1a_h, 
                 res_zero_f1a_h, 
                 res_one_f1a_h, 
                 classify_f1a_h, 
                 res_unorm_one_f1a_h,
                 res_snorm_one_f1a_h,
                 res_pos_int_nan_f1a_h,
                 res_neg_int_nan_f1a_h,
                 res_uint_nan_f1a_h}           ),
  .Q          ( {res_inf_f2a_h, 
                 res_snan_f2a_h, 
                 res_qnan_f2a_h, 
                 res_zero_f2a_h, 
                 res_one_f2a_h, 
                 classify_f2a_h, 
                 res_unorm_one_f2a_h,
                 res_snorm_one_f2a_h,                 
                 res_pos_int_nan_f2a_h,
                 res_neg_int_nan_f2a_h,
                 res_uint_nan_f2a_h}           )
);


en_ff 
#(
  .width($bits( {prod_inf_hi_f1a_h, 
                 a_inf_f1a_h, 
                 c_inf_f1a_h, 
                 a_is_qnan_hi_f1a_h,
                 c_is_qnan_f1a_h,
                 a_is_snan_hi_f1a_h,
                 c_is_snan_f1a_h,                 
                 norm_f32_ovf_f1a_h,
                 norm_one_out_f1a_h,
                 prod_inf_lo_f1a_h}) )
)
F2_SPEC2_FF
(
  .clock      ( clock                             ), 
  .en         ( ctrl_valid_f1a_h           ),
  .D          ( {prod_inf_hi_f1a_h, 
                 a_inf_f1a_h, 
                 c_inf_f1a_h, 
                 a_is_qnan_hi_f1a_h,
                 c_is_qnan_f1a_h,
                 a_is_snan_hi_f1a_h,
                 c_is_snan_f1a_h,
                 norm_f32_ovf_f1a_h,
                 norm_one_out_f1a_h, 
                 prod_inf_lo_f1a_h}           ),
  .Q          ( {prod_inf_hi_f2a_h, 
                 a_inf_f2a_h, 
                 c_inf_f2a_h, 
                 a_is_qnan_hi_f2a_h,
                 c_is_qnan_f2a_h,
                 a_is_snan_hi_f2a_h,
                 c_is_snan_f2a_h,
                 norm_f32_ovf_f2a_h,
                 norm_one_out_f2a_h,
                 prod_inf_lo_f2a_h}           )
);
  

en_ff 
#(
  .width($bits(flags_f1a_h))
)                        
F2_FLG_FF
(
  .clock      ( clock              ), 
  .en         ( ctrl_valid_f1a_h &
                flags_en_f1a_h     ),
  .D          ( flags_f1a_h     ),
  .Q          ( flags_f2a_h     )
);


rst_en_ff 
#(
  .width( $bits({spec_res_en_f1a_h,
                 flags_en_f1a_h}))
)                        
F2_SPEC_EN_FF
(
  .clock      ( clock                    ), 
  .reset      ( reset                    ), 
  .en         ( ctrl_valid_f1a_h  ), 
  .D          ({spec_res_en_f1a_h,
                flags_en_f1a_h} ),
  .Q          ({spec_res_en_f2a_h,
                flags_en_f2a_h} )
);

///////////////////////////////////////////////////////////////////////////////
// C2 component
///////////////////////////////////////////////////////////////////////////////
txfma_c2 txfma0_c2 
(
  .op_dtype_f2a_h                    ( op_dtype_f2a_h                   ),
  .cmd_f2a_h                         ( cmd_f2a_h                        ),
  .sigs_f2a_h                        ( sigs_f2a_h                       ), 
  .rm_f2a_h                          ( rm_f2a_h                         ),
  .ediff1_f2a_h                      ( ediff1_f2a_h                     ),
  .e_sticky1_f2a_h                   ( e_sticky1_f2a_h                  ),
  .e_sticky2_f2a_h                   ( e_sticky2_f2a_h                  ),
  .ec_f2a_h                          ( ec_f2a_h                         ),
  .ec_zero_f2a_h                     ( ec_zero_f2a_h                    ),
  .eprod_hi_zero_f2a_h               ( eprod_hi_zero_f2a_h              ),
  .sel_fcmask_f2a_h                  ( sel_fcmask_f2a_h                 ),
  .sc_f2a_h                          ( sc_f2a_h                         ),
  .prod_sign_hi_f2a_h                ( prod_sign_hi_f2a_h               ),
  .prod_sign_lo_f2a_h                ( prod_sign_lo_f2a_h               ),
  .ediff1_sel_f2a_h                  ( ediff1_sel_f2a_h                 ),
  .ediff2_sel_f2a_h                  ( ediff2_sel_f2a_h                 ),
  .sin_ediff2_f2a_h                  ( sin_ediff2_f2a_h                 ),
  .eff_sub1_f2a_h                    ( eff_sub1_f2a_h                   ),
  .eff_sub2_f2a_h                    ( eff_sub2_f2a_h                   ),
  .sin_eff_sub1_f2a_h                ( sin_eff_sub1_f2a_h               ),
  .comp_op_f2a_h                     ( comp_op_f2a_h                    ),
  .fc_zero30_8_f2a_h                 ( fc_zero30_8_f2a_h                ),
  .fc_b0_f2a_h                       ( fc_b0_f2a_h                      ),
  .align1_mux_sel_f2a_h              ( align1_mux_sel_f2a_h             ),
  .align2_mux_sel_f2a_h              ( align2_mux_sel_f2a_h             ),
  .opd3_mux_sel_f2a_h                ( opd3_mux_sel_f2a_h               ),
  .ediff1_0_f2a_h                    ( ediff1_0_f2a_h                   ),
  .fa_ge_fc_comp_sign_hi_res_f2a_h   ( fa_ge_fc_comp_sign_hi_res_f2a_h  ),
  .fa_eq_fc_comp_sign_hi_res_f2a_h   ( fa_eq_fc_comp_sign_hi_res_f2a_h  ),
  .int_minmax_mux_sel_fc_f2a_h       ( int_minmax_mux_sel_fc_f2a_h      ),
  .minmax_op_f2a_h                   ( minmax_op_f2a_h                  ),
  .cvt_fi_pos_ovf_f2a_h              ( cvt_fi_pos_ovf_f2a_h             ), 
  .cvt_fui_pos_ovf_f2a_h             ( cvt_fui_pos_ovf_f2a_h            ),   
  .cvt_fi_neg_ovf_f2a_h              ( cvt_fi_neg_ovf_f2a_h             ),
  .cvt_fi_min_neg_f2a_h              ( cvt_fi_min_neg_f2a_h             ),  
  .fc_sel_f2a_h                      ( fc_sel_f2a_h                     ),
  .op3p3spc_sel_f2a_h                ( op3p3spc_sel_f2a_h               ),
  .op3p2c_sel_f2a_h                  ( op3p2c_sel_f2a_h                 ),
  .op3p1c_sel_p_f2a_h                ( op3p1c_sel_p_f2a_h               ),
  .op3p1c_sel_r_f2a_h                ( op3p1c_sel_r_f2a_h               ),
  .op3g_sel_f2a_h                    ( op3g_sel_f2a_h                   ),
  .ugfxrplc_sel_f2a_h                ( ugfxrplc_sel_f2a_h               ),
  .sgfxrplc_sel_f2a_h                ( sgfxrplc_sel_f2a_h               ),
  .icgfxc_sel_f2a_h                  ( icgfxc_sel_f2a_h                 ),
  .al1p1c_sel_f2a_h                  ( al1p1c_sel_f2a_h                 ),
  .al1p2c_sel_f2a_h                  ( al1p2c_sel_f2a_h                 ),
  .op2_sxtto_g_f2a_h                 ( op2_sxtto_g_f2a_h                ),
  .rm_rne_f2a_h                      ( rm_rne_f2a_h                     ),
  .rm_rmm_f2a_h                      ( rm_rmm_f2a_h                     ),
  .frnd_rupnsc_f2a_h                 ( frnd_rupnsc_f2a_h                ),
  .frnd_rdnsc_f2a_h                  ( frnd_rdnsc_f2a_h                 ),
  .frnd_t1_f2a_h                     ( frnd_t1_f2a_h                    ),
  .frnd_t2_f2a_h                     ( frnd_t2_f2a_h                    ),
  .al1znp2c_sel_f2a_h                ( al1znp2c_sel_f2a_h               ),
  .al1c_zap_f2a_h                    ( al1c_zap_f2a_h                   ),
  .op1c_log_mul_clr_f2a_h            ( op1c_log_mul_clr_f2a_h           ),
  .negop1c_int_logmul_clr_f2a_h      ( negop1c_int_logmul_clr_f2a_h     ),
  .psc_op1_clr_f2a_h                 ( psc_op1_clr_f2a_h                ),
  .al2c_sel_f2a_h                    ( al2c_sel_f2a_h                   ),
  .align2_zap_f2a_h                  ( align2_zap_f2a_h                 ),
  .align2c_neg_f2a_h                 ( align2c_neg_f2a_h                ),
  .ediff1_lt_31_f2a_h                ( ediff1_lt_31_f2a_h               ),
  .ediff1_abs_f2a_h                  ( ediff1_abs_f2a_h                 ),
  .ediff2_abs_f2a_h                  ( ediff2_abs_f2a_h                 ),
  .sin_align1_neg_f2a_h              ( sin_align1_neg_f2a_h             ),
  .sin_align2_neg_f2a_h              ( sin_align2_neg_f2a_h             ),
  .sin_check_fc_sign_f2a_h           ( sin_check_fc_sign_f2a_h          ),
  .trans_log_rr_neg_f2a_h            ( trans_log_rr_neg_f2a_h           ),
  .al2c3_neg_cin_ps_f2a_h            ( al2c3_neg_cin_ps_f2a_h           ),
  .al1c3_neg_cin_f2a_h               ( al1c3_neg_cin_f2a_h              ),
  .eff_sign1_f2a_h                   ( eff_sign1_f2a_h                  )
);

///////////////////////////////////////////////////////////////////////////////
// F3FF
///////////////////////////////////////////////////////////////////////////////

rst_ff
#(
  .width($bits( ctrl_valid_f2a_h    ))
)
F3_CTRLVAL_FF
(
  .clock      ( clock                ),
  .reset      ( reset                ),
  .D          ( ctrl_valid_f2a_h     ),
  .Q          ( ctrl_valid_f3a_h     )
);

en_ff 
#(
  .width($bits( sin_res_shf_idx_f2a_h ) )
)                        
F3_IDX_FF
(
  .clock      ( clock                      ), 
  .en         ( ctrl_valid_f2a_h           ),
  .D          ( sin_res_shf_idx_f2a_h      ),
  .Q          ( sin_res_shf_idx_f3a_h      )
);

en_ff 
#(
  .width($bits( {eff_sub1_f2a_h,
                 eff_sub2_f2a_h} ) )
)                        
F3_FF
(
  .clock      ( clock                          ), 
  .en         ( ctrl_valid_f2a_h        ),
  .D          ( {eff_sub1_f2a_h,
                 eff_sub2_f2a_h}     ),                 
  .Q          ( {eff_sub1_f3a_h, 
                 eff_sub2_f3a_h}     ) 
);

en_ff 
#(
  .width($bits( {sc_f2a_h,
                 prod_sign_hi_f2a_h,
                 prod_sign_lo_f2a_h,
                 eff_sign1_f2a_h} ))
)                        
S3_FF
(
  .clock      ( clock                       ), 
  .en         ( ctrl_valid_f2a_h     ),
  .D          ( {sc_f2a_h,
                 prod_sign_hi_f2a_h,
                 prod_sign_lo_f2a_h,
                 eff_sign1_f2a_h}  ),
  .Q          ( {sc_f3a_h,
                 prod_sign_hi_f3a_h,
                 prod_sign_lo_f3a_h,
                 eff_sign1_f3a_h}  )
);


en_ff 
#(
  .width($bits({op_dtype_f2a_h, 
                cmd_f2a_h, 
                sigs_f2a_h, 
                rm_f2a_h, 
                minmax_op_f2a_h}))
)                        
F3_CTRL_FF
(
  .clock      ( clock                       ), 
  .en         ( ctrl_valid_f2a_h &
                !use_prev_sigs_f2a_h        ),
  .D          ( {op_dtype_f2a_h, 
                 cmd_f2a_h,
                 sigs_f2a_h,
                 rm_f2a_h,
                 minmax_op_f2a_h}    ),
  .Q          ( {op_dtype_f3a_h, 
                 cmd_f3a_h, 
                 sigs_f3a_h, 
                 rm_f3a_h,
                 minmax_op_f3a_h}     )
);

en_ff 
#(
  .width($bits( {sin_eff_sub1_f3a_h,
                 sin_align1_neg_f3a_h,
                 sin_align2_neg_f3a_h,
                 sin_check_fc_sign_f3a_h,
                 comp_op_f3a_h, 
                 opd3_mux_sel_f3a_h,
                 ediff1_0_f3a_h,
                 fa_ge_fc_comp_sign_hi_res_f3a_h,
                 fa_eq_fc_comp_sign_hi_res_f3a_h,
                 al2c3_neg_cin_ps_f2a_h,
                 al1c3_neg_cin_f2a_h,  
                 fc_sel_f2a_h,   
                 trans_log_rr_neg_f2a_h,
                 int_minmax_mux_sel_fc_f3a_h} ))
)                        
F3_NO_PREV_CTRL_FF
(
  .clock      ( clock                       ), 
  .en         ( ctrl_valid_f2a_h     ),
  .D          ( {sin_eff_sub1_f2a_h,
                 sin_align1_neg_f2a_h,
                 sin_align2_neg_f2a_h,
                 sin_check_fc_sign_f2a_h,
                 comp_op_f2a_h, 
                 opd3_mux_sel_f2a_h,
                 ediff1_0_f2a_h,
                 fa_ge_fc_comp_sign_hi_res_f2a_h,
                 fa_eq_fc_comp_sign_hi_res_f2a_h,
                 al2c3_neg_cin_ps_f2a_h,
                 al1c3_neg_cin_f2a_h,     
                 fc_sel_f2a_h,   
                 trans_log_rr_neg_f2a_h,
                 int_minmax_mux_sel_fc_f2a_h}  ),
  .Q          ( {sin_eff_sub1_f3a_h,
                 sin_align1_neg_f3a_h,
                 sin_align2_neg_f3a_h,
                 sin_check_fc_sign_f3a_h,
                 comp_op_f3a_h, 
                 opd3_mux_sel_f3a_h,
                 ediff1_0_f3a_h,
                 fa_ge_fc_comp_sign_hi_res_f3a_h,
                 fa_eq_fc_comp_sign_hi_res_f3a_h,
                 al2c3_neg_cin_ps_f3a_h,
                 al1c3_neg_cin_f3a_h,      
                 fc_sel_f3a_h,  
                 trans_log_rr_neg_f3a_h,                 
                 int_minmax_mux_sel_fc_f3a_h}  )
);

en_ff 
#(
  .width( $bits(use_prev_sigs_f2a_h ))
)                        
F3_PREV_CTRL_FF
(
  .clock      ( clock               ), 
  .en         ( ctrl_valid_f2a_h    ),
  .D          ( use_prev_sigs_f2a_h ),
  .Q          ( use_prev_sigs_f3a_h )
);


en_ff 
#(
  .width($bits( {ec_zero_f2a_h,
                 eprod_hi_zero_f2a_h,
                 eprod_lo_zero_f2a_h,
                 fb_hi_zero_f2a_h})   )
)
F3_ZER0_FF
(
  .clock      ( clock                 ), 
  .en         ( ctrl_valid_f2a_h      ),
  .D          ( {ec_zero_f2a_h,
                 eprod_hi_zero_f2a_h,
                 eprod_lo_zero_f2a_h,
                 fb_hi_zero_f2a_h}    ),
  .Q          ( {ec_zero_f3a_h, 
                 eprod_hi_zero_f3a_h,
                 eprod_lo_zero_f3a_h,
                 fb_hi_zero_f3a_h}    )
);


en_ff 
#(
  .width($bits({res_inf_f2a_h,
                res_snan_f2a_h, 
                res_qnan_f2a_h,           
                res_zero_f2a_h,           
                res_one_f2a_h,           
                classify_f2a_h,
                res_unorm_one_f2a_h,
                res_snorm_one_f2a_h,                    
                res_pos_int_nan_f2a_h,
                res_neg_int_nan_f2a_h,
                res_uint_nan_f2a_h}))
)                        
F3_SPEC_FF
(
  .clock      ( clock                        ), 
  .en         ( ctrl_valid_f2a_h             ),
  .D          ( {res_inf_f2a_h,
                 res_snan_f2a_h, 
                 res_qnan_f2a_h,
                 res_zero_f2a_h,
                 res_one_f2a_h,
                 classify_f2a_h,
                 res_unorm_one_f2a_h,
                 res_snorm_one_f2a_h,   
                 res_pos_int_nan_f2a_h,
                 res_neg_int_nan_f2a_h,
                 res_uint_nan_f2a_h}         ),
  .Q          ( {res_inf_f3a_h, 
                 res_snan_f3a_h, 
                 res_qnan_f3a_h,
                 res_zero_f3a_h,
                 res_one_f3a_h,
                 classify_f3a_h,
                 res_unorm_one_f3a_h,
                 res_snorm_one_f3a_h,                    
                 res_pos_int_nan_f3a_h,
                 res_neg_int_nan_f3a_h,
                 res_uint_nan_f3a_h}         )
);


en_ff 
#(
  .width($bits({prod_inf_hi_f2a_h,
                prod_inf_lo_f2a_h,
                a_inf_f2a_h,
                c_inf_f2a_h,
                cvt_fui_pos_ovf_f2a_h,                
                cvt_fi_pos_ovf_f2a_h,
                cvt_fi_neg_ovf_f2a_h,
                cvt_fi_min_neg_f2a_h,
                ediff1_lt_31_f2a_h,
                norm_f32_ovf_f2a_h,
                norm_one_out_f2a_h, 
                a_is_snan_hi_f2a_h,
                c_is_snan_f2a_h,
                a_is_qnan_hi_f2a_h,
                c_is_qnan_f2a_h}))
)                        
F3_SPEC2_FF
(
  .clock      ( clock                              ), 
  .en         ( ctrl_valid_f2a_h            ),
  .D          ( {prod_inf_hi_f2a_h,
                 prod_inf_lo_f2a_h,
                 a_inf_f2a_h,
                 c_inf_f2a_h,
                 cvt_fui_pos_ovf_f2a_h,                 
                 cvt_fi_pos_ovf_f2a_h,
                 cvt_fi_neg_ovf_f2a_h,
                 cvt_fi_min_neg_f2a_h,
                 ediff1_lt_31_f2a_h,
                 norm_f32_ovf_f2a_h,
                 norm_one_out_f2a_h,  
                 a_is_snan_hi_f2a_h,
                 c_is_snan_f2a_h,                
                 a_is_qnan_hi_f2a_h,
                 c_is_qnan_f2a_h}       ),
  .Q          ( {prod_inf_hi_f3a_h,
                 prod_inf_lo_f3a_h,
                 a_inf_f3a_h,
                 c_inf_f3a_h,
                 cvt_fui_pos_ovf_f3a_h,                 
                 cvt_fi_pos_ovf_f3a_h,
                 cvt_fi_neg_ovf_f3a_h,
                 cvt_fi_min_neg_f3a_h,
                 ediff1_lt_31_f3a_h,
                 norm_f32_ovf_f3a_h,
                 norm_one_out_f3a_h,
                 a_is_snan_hi_f3a_h,
                 c_is_snan_f3a_h,                 
                 a_is_qnan_hi_f3a_h,
                 c_is_qnan_f3a_h}       )
);


en_ff 
#(
  .width($bits(flags_f2a_h))
)                        
F3_FLG_FF
(
  .clock      ( clock              ), 
  .en         ( ctrl_valid_f2a_h &
                flags_en_f2a_h     ),
  .D          ( flags_f2a_h     ),
  .Q          ( flags_f3a_h     )
);

rst_en_ff 
#(
  .width( $bits(flags_en_f1a_h) )
)                        
F3_FLG_EN_FF
(
  .clock      ( clock                   ), 
  .reset      ( reset                   ), 
  .en         ( ctrl_valid_f2a_h ), 
  .D          ( flags_en_f2a_h   ),
  .Q          ( flags_en_f3a_h   )
);

rst_en_ff 
#(
  .width( $bits(spec_res_en_f2a_h) )
)                        
F3_SPEC_EN_FF
(
  .clock      ( clock                   ), 
  .reset      ( reset                   ), 
  .en         ( ctrl_valid_f2a_h ), 
  .D          ( spec_res_en_f2a_h   ),
  .Q          ( spec_res_en_f3a_h   )
);


///////////////////////////////////////////////////////////////////////////////
// C3 component
///////////////////////////////////////////////////////////////////////////////
txfma_c3 txfma0_c3 
(
  .op_dtype_f3a_h                    ( op_dtype_f3a_h                   ),
  .cmd_f3a_h                         ( cmd_f3a_h                        ),
  .rm_f3a_h                          ( rm_f3a_h                         ),  
  .ediff1_f3a_h                      ( ediff1_f3a_h                     ),
  .ediff2_f3a_h                      ( ediff2_f3a_h                     ),
  .sigs_f3a_h                        ( sigs_f3a_h                       ),
  .eff_sub1_f3a_h                    ( eff_sub1_f3a_h                   ),
  .eff_sub2_f3a_h                    ( eff_sub2_f3a_h                   ),
  .sin_eff_sub1_f3a_h                ( sin_eff_sub1_f3a_h               ),
  .eprod_hi_zero_f3a_h               ( eprod_hi_zero_f3a_h              ),
  .eprod_lo_zero_f3a_h               ( eprod_lo_zero_f3a_h              ),
  .sin_align1_neg_f3a_h              ( sin_align1_neg_f3a_h             ),
  .sin_align2_neg_f3a_h              ( sin_align2_neg_f3a_h             ),
  .sin_check_fc_sign_f3a_h           ( sin_check_fc_sign_f3a_h          ),
  .e_sticky1_f3a_h                   ( e_sticky1_f3a_h                  ),
  .prodsum_a2_f3a_h                  ( prodsum_a2_f3a_h                 ),
  .fround_rbit_f3a_h                 ( fround_rbit_f3a_h                ),
  .opd3_mux_sel_f3a_h                ( opd3_mux_sel_f3a_h               ),
  .ediff1_0_f3a_h                    ( ediff1_0_f3a_h                   ),
  .fa_ge_fc_comp_sign_hi_res_f3a_h   ( fa_ge_fc_comp_sign_hi_res_f3a_h  ),
  .fa_eq_fc_comp_sign_hi_res_f3a_h   ( fa_eq_fc_comp_sign_hi_res_f3a_h  ),
  .int_minmax_mux_sel_fc_f3a_h       ( int_minmax_mux_sel_fc_f3a_h      ),
  .imxc_sel_f3a_h                    ( imxc_sel_f3a_h                   ),
  .int_minmax_mux_sel_a_f3a_h        ( int_minmax_mux_sel_a_f3a_h       ),
  .int_minmax_mux_sel_c_f3a_h        ( int_minmax_mux_sel_c_f3a_h       ),
  .fround_stky2l_f3a_h               ( fround_stky2l_f3a_h              ),
  .minmax_op_f3a_h                   ( minmax_op_f3a_h                  ),
  .comp_op_f3a_h                     ( comp_op_f3a_h                    ),
  .prod_inf_hi_f3a_h                 ( prod_inf_hi_f3a_h                ),
  .prod_inf_lo_f3a_h                 ( prod_inf_lo_f3a_h                ),
  .c_inf_f3a_h                       ( c_inf_f3a_h                      ),
  .eff_sign1_f3a_h                   ( eff_sign1_f3a_h                  ),
  .prod_sign_hi_f3a_h                ( prod_sign_hi_f3a_h               ),
  .prod_sign_lo_f3a_h                ( prod_sign_lo_f3a_h               ),
  .sc_f3a_h                          ( sc_f3a_h                         ),
  .ec_f3a_h                          ( ec_f3a_h                         ),
  .flags_en_f3a_h                    ( flags_en_f3a_h                   ),
  .spec_res_en_f3a_h                 ( spec_res_en_f3a_h                ),  
  .res_inf_f3a_h                     ( res_inf_f3a_h                    ),
  .res_qnan_f3a_h                    ( res_qnan_f3a_h                   ),
  .res_snan_f3a_h                    ( res_snan_f3a_h                   ),
  .res_snan_out_f3a_h                ( res_snan_out_f3a_h               ),
  .res_qnan_out_f3a_h                ( res_qnan_out_f3a_h               ),
  .res_inf_out_f3a_h                 ( res_inf_out_f3a_h                ),
  .res_neg_inf_out_f3a_h             ( res_neg_inf_out_f3a_h            ),
  .ec_zero_f3a_h                     ( ec_zero_f3a_h                    ),
  .a_is_qnan_hi_f3a_h                ( a_is_qnan_hi_f3a_h               ),
  .c_is_qnan_f3a_h                   ( c_is_qnan_f3a_h                  ),
  .a_is_snan_hi_f3a_h                ( a_is_snan_hi_f3a_h               ),
  .c_is_snan_f3a_h                   ( c_is_snan_f3a_h                  ),
  .s_res_f3a_h                       ( s_res_f3a_h                      ),
  .s_res_check_cadd_a3_f3a_h         ( s_res_check_cadd_a3_f3a_h        ),
  .comp_res_sel_f3a_h                ( comp_res_sel_f3a_h               ),
  .fa_eq_fc_res_check_hi_f3a_h       ( fa_eq_fc_res_check_hi_f3a_h      ),
  .trans_log_rr_neg_f3a_h            ( trans_log_rr_neg_f3a_h           ),
  .fc_sel_f3a_h                      ( fc_sel_f3a_h                     ),
  .eff_sub1_out_f3a_h                ( eff_sub1_out_f3a_h               ),
  .cmp_invalid_f3a_h                 ( cmp_invalid_f3a_h                ),
  .fma_subopc3_f3a_h                 ( fma_subopc3_f3a_h                )
);


///////////////////////////////////////////////////////////////////////////////
// F4FF
///////////////////////////////////////////////////////////////////////////////
en_ff 
#(
  .width($bits(int_minmax_mux_sel_a_f3a_h))
)                        
SEL_A_E4_FF
(
  .clock      ( clock                 ), 
  .en         ( ctrl_valid_f3a_h      ),
  .D          ( int_minmax_mux_sel_a_f3a_h         ),
  .Q          ( int_minmax_mux_sel_a_f4a_h      )
); 
en_ff 
#(
  .width($bits(int_minmax_mux_sel_c_f3a_h))
)                        
SEL_C_E4_FF
(
  .clock      ( clock                 ), 
  .en         ( ctrl_valid_f3a_h      ),
  .D          ( int_minmax_mux_sel_c_f3a_h         ),
  .Q          ( int_minmax_mux_sel_c_f4a_h      )
); 

rst_ff
#(
  .width($bits( ctrl_valid_f3a_h    ))
)
F4_CTRLVAL_FF
(
  .clock      ( clock                ),
  .reset      ( reset                ),
  .D          ( ctrl_valid_f3a_h     ),
  .Q          ( ctrl_valid_f4a_h     )
);

en_ff 
#(
  .width($bits( sin_res_shf_idx_f3a_h ) )
)                        
F4_IDX_FF
(
  .clock      ( clock                      ), 
  .en         ( ctrl_valid_f3a_h           ),
  .D          ( sin_res_shf_idx_f3a_h      ),
  .Q          ( sin_res_shf_idx_f4a_h      )
);

en_ff 
#(
  .width($bits( {eff_sub2_f3a_h,
                 eff_sub1_out_f3a_h} ) )
)                        
F4_FF
(
  .clock      ( clock                ), 
  .en         ( ctrl_valid_f3a_h     ),
  .D          ( {eff_sub2_f3a_h,
                 eff_sub1_out_f3a_h} ),                 
  .Q          ( {eff_sub2_f4a_h,
                 eff_sub1_f4a_h}     ) 
);

en_ff 
#(
  .width($bits( {prod_sign_hi_f3a_h,
                 s_res_f3a_h,
                 s_res_check_cadd_a3_f3a_h,
                 sc_f3a_h}             ))
)                        
S4_FF
(
  .clock      ( clock                                 ), 
  .en         ( ctrl_valid_f3a_h               ),
  .D          ( {prod_sign_hi_f3a_h,
                 s_res_f3a_h,
                 s_res_check_cadd_a3_f3a_h,
                 sc_f3a_h}                     ),
  .Q          ( {prod_sign_hi_f4a_h,
                 s_res_f4a_h,
                 s_res_check_cadd_a3_f4a_h,
                 sc_f4a_h}                     )
);

en_ff 
#(
  .width($bits({op_dtype_f3a_h, 
                cmd_f3a_h, 
                sigs_f3a_h, 
                rm_f3a_h, 
                comp_op_f3a_h, 
                minmax_op_f3a_h})       )
)                        
F4_CTRL_FF
(
  .clock      ( clock                          ), 
  .en         ( ctrl_valid_f3a_h &
                !use_prev_sigs_f3a_h           ),
  .D          ( {op_dtype_f3a_h, 
                 cmd_f3a_h, 
                 sigs_f3a_h, 
                 rm_f3a_h, 
                 comp_op_f3a_h, 
                 minmax_op_f3a_h}       ),
  .Q          ( {op_dtype_f4a_h, 
                 cmd_f4a_h, 
                 sigs_f4a_h, 
                 rm_f4a_h, 
                 comp_op_f4a_h, 
                 minmax_op_f4a_h}       )
);


en_ff 
#(
  .width($bits({comp_res_sel_f3a_h,
                fa_eq_fc_res_check_hi_f3a_h,
                fc_sel_f3a_h,
                cmp_invalid_f3a_h,
                fround_stky2l_f3a_h,
                trans_log_rr_neg_f3a_h})       )
)                        
F4_NO_PREV_CTRL_FF
(
  .clock      ( clock                          ), 
  .en         ( ctrl_valid_f3a_h               ),
  .D          ( {comp_res_sel_f3a_h,
                 fa_eq_fc_res_check_hi_f3a_h,
                 fc_sel_f3a_h,
                 cmp_invalid_f3a_h,
                 fround_stky2l_f3a_h,
                 trans_log_rr_neg_f3a_h}       ),
  .Q          ( {comp_res_sel_f4a_h,
                 fa_eq_fc_res_check_hi_f4a_h,
                 fc_sel_f4a_h,
                 cmp_invalid_f4a_h,
                 fround_stky2l_f4a_h,
                 trans_log_rr_neg_f4a_h}       )
);


en_ff 
#(
  .width( $bits(use_prev_sigs_f3a_h ))
)                        
F4_PREV_CTRL_FF
(
  .clock      ( clock                ), 
  .en         ( ctrl_valid_f3a_h     ),
  .D          ( use_prev_sigs_f3a_h  ),
  .Q          ( use_prev_sigs_f4a_h  )
);

en_ff 
#(
  .width($bits( {ec_zero_f3a_h,
                 fb_hi_zero_f3a_h})   )
)
F4_ZER0_FF
(
  .clock      ( clock                 ), 
  .en         ( ctrl_valid_f3a_h      ),
  .D          ( {ec_zero_f3a_h,
                 fb_hi_zero_f3a_h}    ),
  .Q          ( {ec_zero_f4a_h, 
                 fb_hi_zero_f4a_h}    )
);

en_ff 
#(
  .width($bits({res_inf_out_f3a_h,
                res_neg_inf_out_f3a_h,
                res_snan_out_f3a_h, 
                res_qnan_out_f3a_h, 
                res_zero_f3a_h, 
                res_one_f3a_h, 
                a_inf_f3a_h,
                classify_f3a_h,
                res_unorm_one_f3a_h,
                res_snorm_one_f3a_h,                
                res_pos_int_nan_f3a_h,
                res_neg_int_nan_f3a_h,
                res_uint_nan_f3a_h})     )
)                        
F4_SPEC_FF
(
  .clock      ( clock                     ), 
  .en         ( ctrl_valid_f3a_h          ),
  .D          ( {res_inf_out_f3a_h,
                 res_neg_inf_out_f3a_h,
                 res_snan_out_f3a_h, 
                 res_qnan_out_f3a_h, 
                 res_zero_f3a_h, 
                 res_one_f3a_h, 
                 a_inf_f3a_h,
                 classify_f3a_h,
                 res_unorm_one_f3a_h,
                 res_snorm_one_f3a_h,              
                 res_pos_int_nan_f3a_h,
                 res_neg_int_nan_f3a_h,
                 res_uint_nan_f3a_h}     ),
  .Q          ( {res_inf_f4a_h,
                 res_neg_inf_f4a_h,
                 res_snan_f4a_h, 
                 res_qnan_f4a_h, 
                 res_zero_f4a_h, 
                 res_one_f4a_h, 
                 a_inf_f4a_h,
                 classify_f4a_h,
                 res_unorm_one_f4a_h,
                 res_snorm_one_f4a_h,                 
                 res_pos_int_nan_f4a_h,
                 res_neg_int_nan_f4a_h,
                 res_uint_nan_f4a_h}     )
);


en_ff 
#(
  .width($bits({cvt_fui_pos_ovf_f3a_h,                
                cvt_fi_pos_ovf_f3a_h,
                cvt_fi_neg_ovf_f3a_h,
                cvt_fi_min_neg_f3a_h,
                ediff1_lt_31_f3a_h,
                norm_f32_ovf_f3a_h,
                norm_one_out_f3a_h})      )
)                        
F4_SPEC2_FF
(
  .clock      ( clock                            ), 
  .en         ( ctrl_valid_f3a_h          ),
  .D          ( {cvt_fui_pos_ovf_f3a_h,                 
                 cvt_fi_pos_ovf_f3a_h,
                 cvt_fi_neg_ovf_f3a_h,
                 cvt_fi_min_neg_f3a_h,
                 ediff1_lt_31_f3a_h,
                 norm_f32_ovf_f3a_h,
                 norm_one_out_f3a_h}      ),
  .Q          ( {cvt_fui_pos_ovf_f4a_h,                 
                 cvt_fi_pos_ovf_f4a_h,
                 cvt_fi_neg_ovf_f4a_h,
                 cvt_fi_min_neg_f4a_h,
                 ediff1_lt_31_f4a_h,
                 norm_f32_ovf_f4a_h,
                 norm_one_out_f4a_h}      )
);




en_ff 
#(
  .width( $bits(flags_f3a_h)  )
)                        
F4_FLG_FF
(
  .clock      ( clock              ), 
  .en         ( ctrl_valid_f3a_h &
                flags_en_f3a_h     ),
  .D          ( flags_f3a_h     ),
  .Q          ( flags_f4a_h     )
);


rst_en_ff 
#(
  .width( $bits(flags_en_f3a_h) )
)                        
F4_FLG_EN_FF
(
  .clock      ( clock            ), 
  .reset      ( reset            ), 
  .en         ( ctrl_valid_f3a_h ), 
  .D          ( flags_en_f3a_h   ),
  .Q          ( flags_en_f4a_h   )
);

rst_en_ff 
#(
  .width( $bits(spec_res_en_f3a_h)  )
)                        
F4_SPEC_EN_FF
(
  .clock      ( clock               ), 
  .reset      ( reset               ), 
  .en         ( ctrl_valid_f3a_h    ), 
  .D          ( spec_res_en_f3a_h   ),
  .Q          ( spec_res_en_f4a_h   )
);

///////////////////////////////////////////////////////////////////////////////
// C4 component
///////////////////////////////////////////////////////////////////////////////
txfma_c4 txfma0_c4 
(
  .op_dtype_f4a_h                   ( op_dtype_f4a_h                   ), 
  .cmd_f4a_h                        ( cmd_f4a_h                        ), 
  .sigs_f4a_h                       ( sigs_f4a_h                       ),
  .elxd_f4a_h                       ( elxd_f4a_h                       ), 
  .ph_comb_stiky_f4a_h              ( ph_comb_stiky_f4a_h              ),
  .eff_sub1_f4a_h                   ( eff_sub1_f4a_h                   ),
  .eff_sub2_f4a_h                   ( eff_sub2_f4a_h                   ),
  .s_res_f4a_h                      ( s_res_f4a_h                      ),
  .sc_f4a_h                         ( sc_f4a_h                         ),
  .sin_res_shf_idx_f4a_h            ( sin_res_shf_idx_f4a_h            ),
  .enc_nsamt_f4a_h                  ( enc_nsamt_f4a_h                  ),
  .elxd_add_in2_mux_sel_f4a_h       ( elxd_add_in2_mux_sel_f4a_h       ),
  .neg_norm_a3_f4a_h                ( neg_norm_a3_f4a_h                ),
  .neg_norm_na3_f4a_h               ( neg_norm_na3_f4a_h               ),
  .clearphps_en_f4a_h               ( clearphps_en_f4a_h               ),
  .clear16frnd_nshin_f4a_l          ( clear16frnd_nshin_f4a_l          ),
  .ph_ps_clear_f4a_l                ( ph_ps_clear_f4a_l                ),
  .use_elxd_f4a_h                   ( use_elxd_f4a_h                   )
);
///////////////////////////////////////////////////////////////////////////////
// F5FF
///////////////////////////////////////////////////////////////////////////////
en_ff 
#(
  .width($bits(int_minmax_mux_sel_a_f4a_h))
)                        
SEL_A_E5_FF
(
  .clock      ( clock                 ), 
  .en         ( ctrl_valid_f4a_h      ),
  .D          ( int_minmax_mux_sel_a_f4a_h         ),
  .Q          ( int_minmax_mux_sel_a_f5a_h      )
); 
en_ff 
#(
  .width($bits(int_minmax_mux_sel_c_f3a_h))
)                        
SEL_C_E5_FF
(
  .clock      ( clock                 ), 
  .en         ( ctrl_valid_f4a_h      ),
  .D          ( int_minmax_mux_sel_c_f4a_h         ),
  .Q          ( int_minmax_mux_sel_c_f5a_h      )
); 

rst_ff
#(
  .width($bits( ctrl_valid_f4a_h    ))
)
F5_CTRLVAL_FF
(
  .clock      ( clock                ),
  .reset      ( reset                ),
  .D          ( ctrl_valid_f4a_h     ),
  .Q          ( ctrl_valid_f5a_h     )
);

en_ff 
#(
  .width($bits({prod_sign_hi_f4a_h,
                ec_zero_f4a_h}))
)                        
F5_RES_FF
(
  .clock      ( clock                     ), 
  .en         ( ctrl_valid_f4a_h          ),
  .D          ( {prod_sign_hi_f4a_h,
                 ec_zero_f4a_h}         ),
  .Q          ( {prod_sign_hi_f5a_h,
                 ec_zero_f5a_h}         )
);

en_ff 
#(
  .width($bits( {op_dtype_f4a_h, 
                 cmd_f4a_h, 
                 sigs_f4a_h, 
                 comp_op_f4a_h, 
                 rm_f4a_h, 
                 minmax_op_f4a_h} ))
)                        
F5_CTRL_FF
(
  .clock      ( clock                                     ), 
  .en         ( ctrl_valid_f4a_h &
                !use_prev_sigs_f4a_h               ),
  .D          ( {op_dtype_f4a_h, 
                 cmd_f4a_h, 
                 sigs_f4a_h, 
                 comp_op_f4a_h,
                 rm_f4a_h,
                 minmax_op_f4a_h}                  ),
  .Q          ( {op_dtype_f5a_h, 
                 cmd_f5a_h, 
                 sigs_f5a_h, 
                 comp_op_f5a_h, 
                 rm_f5a_h,                
                 minmax_op_f5a_h}                  )
);


en_ff 
#(
  .width($bits( {trans_log_rr_neg_f4a_h,
                 comp_res_sel_f4a_h,
                 fa_eq_fc_res_check_hi_f4a_h,
                 fc_sel_f4a_h,
                 elxd_f4a_h,
                 s_res_check_cadd_a3_f4a_h,
                 eff_sub1_f4a_h,
                 etrz50_f4a_h,
                 fround_stky2l_f4a_h} ))
)                        
F5_NO_PREV_CTRL_FF
(
  .clock      ( clock                              ), 
  .en         ( ctrl_valid_f4a_h                   ),
  .D          ( {trans_log_rr_neg_f4a_h,
                 comp_res_sel_f4a_h,
                 fa_eq_fc_res_check_hi_f4a_h,
                 fc_sel_f4a_h,
                 elxd_f4a_h,
                 s_res_check_cadd_a3_f4a_h,
                 eff_sub1_f4a_h,
                 etrz50_f4a_h,
                 fround_stky2l_f4a_h}                  ),
  .Q          ( {trans_log_rr_neg_f5a_h,
                 comp_res_sel_f5a_h,
                 fa_eq_fc_res_check_hi_f5a_h,
                 fc_sel_f5a_h,
                 elxd_f5a_h,
                 s_res_check_cadd_a3_f5a_h,
                 eff_sub1_f5a_h,
                 etrz50_f5a_h,
                 fround_stky2l_f5a_h}                  )
);



en_ff 
#(
  .width($bits( {res_inf_f4a_h,
                 res_neg_inf_f4a_h,
                 res_snan_f4a_h, 
                 res_qnan_f4a_h, 
                 res_zero_f4a_h, 
                 res_one_f4a_h, 
                 a_inf_f4a_h,
                 classify_f4a_h,
                 res_unorm_one_f4a_h,
                 res_snorm_one_f4a_h,
                 cmp_invalid_f4a_h,
                 res_pos_int_nan_f4a_h,
                 res_neg_int_nan_f4a_h,
                 res_uint_nan_f4a_h} ))
)                        
F5_SPEC_FF
(
  .clock      ( clock                      ), 
  .en         ( ctrl_valid_f4a_h           ),
  .D          ( {res_inf_f4a_h,
                 res_neg_inf_f4a_h,
                 res_snan_f4a_h, 
                 res_qnan_f4a_h, 
                 res_zero_f4a_h, 
                 res_one_f4a_h, 
                 a_inf_f4a_h,
                 classify_f4a_h,
                 res_unorm_one_f4a_h,
                 res_snorm_one_f4a_h,
                 cmp_invalid_f4a_h,
                 res_pos_int_nan_f4a_h,
                 res_neg_int_nan_f4a_h,
                 res_uint_nan_f4a_h}         ),
  .Q          ( {res_inf_f5a_h, 
                 res_neg_inf_f5a_h,
                 res_snan_f5a_h, 
                 res_qnan_f5a_h, 
                 res_zero_f5a_h, 
                 res_one_f5a_h, 
                 a_inf_f5a_h,
                 classify_f5a_h, 
                 res_unorm_one_f5a_h,
                 res_snorm_one_f5a_h,
                 cmp_invalid_f5a_h,
                 res_pos_int_nan_f5a_h,
                 res_neg_int_nan_f5a_h,
                 res_uint_nan_f5a_h}         )
);


en_ff 
#(
  .width($bits( {cvt_fui_pos_ovf_f4a_h,                 
                 cvt_fi_pos_ovf_f4a_h,
                 cvt_fi_neg_ovf_f4a_h,
                 cvt_fi_min_neg_f4a_h,
                 ediff1_lt_31_f4a_h,
                 norm_f32_ovf_f4a_h,
                 norm_one_out_f4a_h} ))
)                        
F5_SPEC2_FF
(
  .clock      ( clock                             ), 
  .en         ( ctrl_valid_f4a_h           ),
  .D          ( {cvt_fui_pos_ovf_f4a_h,                 
                 cvt_fi_pos_ovf_f4a_h,
                 cvt_fi_neg_ovf_f4a_h,
                 cvt_fi_min_neg_f4a_h,
                 ediff1_lt_31_f4a_h,
                 norm_f32_ovf_f4a_h,
                 norm_one_out_f4a_h}         ),
  .Q          ( {cvt_fui_pos_ovf_f5a_h,                 
                 cvt_fi_pos_ovf_f5a_h,
                 cvt_fi_neg_ovf_f5a_h,
                 cvt_fi_min_neg_f5a_h,
                 ediff1_lt_31_f5a_h,
                 norm_f32_ovf_f5a_h,
                 norm_one_out_f5a_h}         )
);


en_ff 
#(
  .width($bits(flags_f4a_h))
)                        
F5_FLG_FF
(
  .clock      ( clock              ), 
  .en         ( ctrl_valid_f4a_h &
                flags_en_f4a_h     ),
  .D          ( flags_f4a_h     ),
  .Q          ( flags_f5a_h     )
);


rst_en_ff 
#(
  .width( $bits(spec_res_en_f4a_h) )
)                        
F5_SPEC_EN_FF
(
  .clock      ( clock                      ), 
  .reset      ( reset                      ), 
  .en         ( ctrl_valid_f4a_h    ), 
  .D          ( spec_res_en_f4a_h   ),
  .Q          ( spec_res_en_f5a_h   )
);


rst_en_ff 
#(
  .width( $bits(flags_en_f4a_h) )
)                        
F5_FLG_EN_FF
(
  .clock      ( clock                   ), 
  .reset      ( reset                   ), 
  .en         ( ctrl_valid_f4a_h ), 
  .D          ( flags_en_f4a_h   ),
  .Q          ( flags_en_f5a_h   )
);

en_ff 
#(
  .width( $bits(use_prev_sigs_f4a_h ))
)                        
F5_PREV_CTRL_FF
(
  .clock      ( clock                ), 
  .en         ( ctrl_valid_f4a_h     ),
  .D          ( use_prev_sigs_f4a_h  ),
  .Q          ( use_prev_sigs_f5a_h  )
);


///////////////////////////////////////////////////////////////////////////////
// C5 component
///////////////////////////////////////////////////////////////////////////////
txfma_c5 txfma0_c5 
(
  .op_dtype_f5a_h                     ( op_dtype_f5a_h                   ),
  .cmd_f5a_h                          ( cmd_f5a_h                        ),
  .sigs_f5a_h                         ( sigs_f5a_h                       ),
  .rm_f5a_h                           ( rm_f5a_h                         ),
  .elxd_f5a_h                         ( elxd_f5a_h                       ), 
  .ec_zero_f5a_h                      ( ec_zero_f5a_h                    ),
  .prod_sign_hi_f5a_h                 ( prod_sign_hi_f5a_h               ),
  .s_res_f5a_h                        ( s_res_f5a_h                      ),
  .s_res_check_cadd_a3_f5a_h          ( s_res_check_cadd_a3_f5a_h        ),
  .comb_add_a3_f5a_h                  ( comb_add_a3_f5a_h                ),
  .comp_res_sel_f5a_h                 ( comp_res_sel_f5a_h               ),
  .comp_op_f5a_h                      ( comp_op_f5a_h                    ),
  .minmax_op_f5a_h                    ( minmax_op_f5a_h                  ),
  .stky2g_f5a_h                       ( stky2g_f5a_h                     ),
  .trans_log_rr_neg_f5a_h             ( trans_log_rr_neg_f5a_h           ),
  .ediff1_zap_f5a_h                   ( ediff1_zap_f5a_h                 ),
  .fa_eq_fc_res_check_hi_f5a_h        ( fa_eq_fc_res_check_hi_f5a_h      ),
  .etrz50_f5a_h                       ( etrz50_f5a_h                     ),
  .fc_sel_f5a_h                       ( fc_sel_f5a_h                     ),
  .rc_ph_f5a_h                        ( rc_ph_f5a_h                      ),
  .rc_ps_f5a_h                        ( rc_ps_f5a_h                      ),
  .rc6_ph_f5a_h                       ( rc6_ph_f5a_h                     ),
  .rc6_ps_f5a_h                       ( rc6_ps_f5a_h                     ),
  .rc_int32_f5a_h                     ( rc_int32_f5a_h                   ),
  .rc_3rmxr_f5a_h                     ( rc_3rmxr_f5a_h                   ),
  .rc_rl01_f5a_h                      ( rc_rl01_f5a_h                    ),
  .rc_rmm_f5a_h                       ( rc_rmm_f5a_h                     ),
  .rc_rup_f5a_h                       ( rc_rup_f5a_h                     ),
  .rc_rne_f5a_h                       ( rc_rne_f5a_h                     ),
  .rc_rdn_f5a_h                       ( rc_rdn_f5a_h                     ),
  .rc_res_sign_f5a_l                  ( rc_res_sign_f5a_l                ),
  .rc_transrnd_f5a_h                  ( rc_transrnd_f5a_h                ),
  .rc_transrnd_log_fma2_f5a_h         ( rc_transrnd_log_fma2_f5a_h       ),
  .s_res_out_f5a_h                    ( s_res_out_f5a_h                  ),
  .nshcen_sel1b_f5a_h                 ( nshcen_sel1b_f5a_h               ),
  .en_logrrsel1b_f5a_l                ( en_logrrsel1b_f5a_l              ),
  .int32_stky2r_f5a_h                 ( int32_stky2r_f5a_h               ),
  .mmx_sel_immres_f5a_h               ( mmx_sel_immres_f5a_h             ),
  .comp_op_true_f5a_h                 ( comp_op_true_f5a_h               ),
  .e_force_to_zero_f5a_h              ( e_force_to_zero_f5a_h            ), 
  .trans2_ps_roundup_r_f5a_h          ( trans2_ps_roundup_r_f5a_h        )
);

///////////////////////////////////////////////////////////////////////////////
// F6FF 
///////////////////////////////////////////////////////////////////////////////

rst_en_ff 
#(
  .width( $bits(flags_en_f5a_h) )
)                        
F6_FLG_EN_FF
(
  .clock      ( clock            ), 
  .reset      ( reset            ), 
  .en         ( ctrl_valid_f5a_h ), 
  .D          ( flags_en_f5a_h   ),
  .Q          ( flags_en_f6a_h   )
);

en_ff 
#(
  .width($bits(flags_f5a_h))
)                        
F6_FLG_FF
(
  .clock      ( clock              ), 
  .en         ( ctrl_valid_f5a_h &
                flags_en_f5a_h     ),
  .D          ( flags_f5a_h     ),
  .Q          ( flags_f6a_h     )
);

rst_en_ff 
#(
  .width( $bits({op_dtype_f5a_h,
                 sigs_f5a_h,
                 cmd_f5a_h,
                 rm_f5a_h,                 
                 minmax_op_f5a_h,
                 comp_op_f5a_h}) )
)                        
F6_CTL_FF
(
  .clock      ( clock                    ), 
  .reset      ( reset                    ), 
  .en         ( ctrl_valid_f5a_h & 
                !use_prev_sigs_f5a_h     ), 
  .D          ( {op_dtype_f5a_h,
                 sigs_f5a_h,
                 cmd_f5a_h,
                 rm_f5a_h,
                 minmax_op_f5a_h ,
                 comp_op_f5a_h}),
  .Q          ( {op_dtype_f6a_h,
                 sigs_f6a_h,
                 cmd_f6a_h,
                 rm_f6a_h,
                 minmax_op_f6a_h,
                 comp_op_f6a_h}) 
);



rst_en_ff 
#(
  .width( $bits({ eff_sub1_f5a_h,
                  rc_ph_f5a_h,
                  rc_ps_f5a_h,
                  rc_int32_f5a_h,
                  ediff1_lt_31_f5a_h,
                  comp_op_true_f5a_h,
                  spec_res_en_f5a_h,
                  cvt_fui_pos_ovf_f5a_h,
                  cvt_fi_pos_ovf_f5a_h,
                  cvt_fi_neg_ovf_f5a_h,
                  fround_stky2l_f5a_h,
                  cvt_fi_min_neg_f5a_h}) )
)                        
F6_NO_PREV_CTRL_FF
(
  .clock      ( clock                    ), 
  .reset      ( reset                    ), 
  .en         ( ctrl_valid_f5a_h         ), 
  .D          ( {eff_sub1_f5a_h,
                 rc_ph_f5a_h,
                 rc_ps_f5a_h,
                 rc_int32_f5a_h,
                 ediff1_lt_31_f5a_h,
                 comp_op_true_f5a_h,
                 spec_res_en_f5a_h,
                 cvt_fui_pos_ovf_f5a_h,
                 cvt_fi_pos_ovf_f5a_h,
                 cvt_fi_neg_ovf_f5a_h,
                 fround_stky2l_f5a_h,
                 cvt_fi_min_neg_f5a_h}),
  .Q          ( {eff_sub1_f6a_h,
                 rc_ph_f6a_h,
                 rc_ps_f6a_h,
                 rc_int32_f6a_h,
                 ediff1_lt_31_f6a_h,
                 comp_op_true_f6a_h,
                 spec_res_en_f6a_h,
                 cvt_fui_pos_ovf_f6a_h,
                 cvt_fi_pos_ovf_f6a_h,
                 cvt_fi_neg_ovf_f6a_h,
                 fround_stky2l_f6a_h,
                 cvt_fi_min_neg_f6a_h}) 
);


en_ff 
#(
  .width($bits( {res_inf_f5a_h,
                 res_neg_inf_f5a_h,
                 res_snan_f5a_h, 
                 res_qnan_f5a_h, 
                 res_zero_f5a_h,
                 res_one_f5a_h,
                 a_inf_f5a_h,
                 classify_f5a_h,
                 res_unorm_one_f5a_h,
                 res_snorm_one_f5a_h,
                 norm_f32_ovf_f5a_h,
                 norm_one_out_f5a_h,
                 cmp_invalid_f5a_h,
                 res_pos_int_nan_f5a_h,
                 res_neg_int_nan_f5a_h,
                 res_uint_nan_f5a_h} )     )
)                        
F6_SPEC_FF
(
  .clock      ( clock                      ), 
  .en         ( ctrl_valid_f5a_h           ),   
  .D          ( {res_inf_f5a_h,
                 res_neg_inf_f5a_h,
                 res_snan_f5a_h, 
                 res_qnan_f5a_h, 
                 res_zero_f5a_h,
                 res_one_f5a_h,
                 a_inf_f5a_h,
                 classify_f5a_h,
                 res_unorm_one_f5a_h,
                 res_snorm_one_f5a_h,
                 norm_f32_ovf_f5a_h,
                 norm_one_out_f5a_h,
                 cmp_invalid_f5a_h,
                 res_pos_int_nan_f5a_h,
                 res_neg_int_nan_f5a_h,
                 res_uint_nan_f5a_h}       ),
  .Q          ( {res_inf_f6a_h, 
                 res_neg_inf_f6a_h,
                 res_snan_f6a_h, 
                 res_qnan_f6a_h, 
                 res_zero_f6a_h,
                 res_one_f6a_h,
                 a_inf_f6a_h,
                 classify_f6a_h, 
                 res_unorm_one_f6a_h,
                 res_snorm_one_f6a_h,
                 norm_f32_ovf_f6a_h,
                 norm_one_out_f6a_h,
                 cmp_invalid_f6a_h,
                 res_pos_int_nan_f6a_h,
                 res_neg_int_nan_f6a_h,
                 res_uint_nan_f6a_h}       )
);

rst_en_ff
#(
  .width( $bits( ec_zero_f5a_h )   )
)
F6_EXP_FF
(
  .clock      ( clock              ), 
  .reset      ( reset              ), 
  .en         ( ctrl_valid_f5a_h   ), 
  .D          ( ec_zero_f5a_h      ),
  .Q          ( ec_zero_f6a_h      )
);


///////////////////////////////////////////////////////////////////////////////
// C6 component
///////////////////////////////////////////////////////////////////////////////
txfma_c6 txfma0_c6 
(
  .op_dtype_f6a_h               ( op_dtype_f6a_h             ),
  .cmd_f6a_h                    ( cmd_f6a_h                  ), 
  .sigs_f6a_h                   ( sigs_f6a_h                 ),
  .classify_zero_f6a_h          ( classify_f6a_h[`FP_CLASS_POS_ZERO]             ),
  .sel_classify_out_f6a_h       ( sel_classify_out_f6a_h     ),
  .sel_nr1_frcpres_f6a_h        ( sel_nr1_frcpres_f6a_h      ),
  .rmode_f6a_h                  ( rmode_f6a_h                ),
  .exp_res_zero_f6a_h           ( exp_res_zero_f6a_h         ),
  .s_res_f6a_h                  ( s_res_f6a_h                ),
  .eff_sub1_f6a_h               ( eff_sub1_f6a_h             ),
  .in_flags_f6a_h               ( flags_f6a_h                ),
  .rm_f6a_h                     ( rm_f6a_h                   ),
  .flags_en_f6a_h               ( flags_en_f6a_h             ),
  .ediff1_lt_31_f6a_h           ( ediff1_lt_31_f6a_h         ),
  .rc_ph_f6a_h                  ( rc_ph_f6a_h                ),
  .rc_ps_f6a_h                  ( rc_ps_f6a_h                ),
  .rc_int32_f6a_h               ( rc_int32_f6a_h             ),
  .ph_stky2l_f6a_h              ( ph_stky2l_f6a_h            ),
  .ps_stky2l_f6a_h              ( ps_stky2l_f6a_h            ),
  .fround_stky2l_f6a_h          ( fround_stky2l_f6a_h        ),
  .int32_stky2l_f6a_h           ( int32_stky2l_f6a_h         ),
  .minmax_op_f6a_h              ( minmax_op_f6a_h            ),
  .comp_op_f6a_h                ( comp_op_f6a_h              ),
  .comp_op_true_f6a_h           ( comp_op_true_f6a_h         ),
  .spec_res_en_f6a_h            ( spec_res_en_f6a_h          ),
  .ec_zero_f6a_h                ( ec_zero_f6a_h              ),  
  .res_inf_f6a_h                ( res_inf_f6a_h              ),
  .res_neg_inf_f6a_h            ( res_neg_inf_f6a_h          ),
  .res_snan_f6a_h               ( res_snan_f6a_h             ),
  .res_qnan_f6a_h               ( res_qnan_f6a_h             ),
  .res_zero_f6a_h               ( res_zero_f6a_h             ),
  .res_one_f6a_h                ( res_one_f6a_h              ),
  .a_inf_f6a_h                  ( a_inf_f6a_h                ),
  .norm_f32_ovf_f6a_h           ( norm_f32_ovf_f6a_h         ),
  .norm_one_out_f6a_h           ( norm_one_out_f6a_h         ),
  .pot_ovf_ps_f6a_h             ( pot_ovf_ps_f6a_h           ),
  .def_ovf_ps_f6a_h             ( def_ovf_ps_f6a_h           ),  
  .pot_unf_ps_f6a_h             ( pot_unf_ps_f6a_h           ),
  .def_unf_ps_f6a_h             ( def_unf_ps_f6a_h           ),
  .pot_ovf_ph_f6a_h             ( pot_ovf_ph_f6a_h           ),
  .def_ovf_ph_f6a_h             ( def_ovf_ph_f6a_h           ),  
  .pot_unf_ph_f6a_h             ( pot_unf_ph_f6a_h           ),
  .def_unf_ph_f6a_h             ( def_unf_ph_f6a_h           ),
  .cvt_fui_pos_ovf_f6a_h        ( cvt_fui_pos_ovf_f6a_h      ),
  .cvt_fi_pos_ovf_f6a_h         ( cvt_fi_pos_ovf_f6a_h       ),
  .cvt_fi_neg_ovf_f6a_h         ( cvt_fi_neg_ovf_f6a_h       ),
  .cvt_fi_min_neg_f6a_h         ( cvt_fi_min_neg_f6a_h       ),
  .res_unorm_one_f6a_h          ( res_unorm_one_f6a_h        ),
  .res_snorm_one_f6a_h          ( res_snorm_one_f6a_h        ),   
  .cmp_invalid_f6a_h            ( cmp_invalid_f6a_h          ),
  .res_pos_int_nan_f6a_h        ( res_pos_int_nan_f6a_h      ),
  .res_neg_int_nan_f6a_h        ( res_neg_int_nan_f6a_h      ),
  .res_uint_nan_f6a_h           ( res_uint_nan_f6a_h         ),
  .frac_zero_f6a_h              ( frac_zero_f6a_h            ),
  .rnd_l_ph_f6a_h               ( rnd_l_ph_f6a_h             ),
  .rnd_r_ph_f6a_h               ( rnd_r_ph_f6a_h             ),
  .rnd_l_ps_f6a_h               ( rnd_l_ps_f6a_h             ),
  .rnd_r_ps_f6a_h               ( rnd_r_ps_f6a_h             ),
  .int_res_l_bit_f6a_h          ( int_res_l_bit_f6a_h        ),
  .ph_allone2r_f6a_h            ( ph_allone2r_f6a_h          ),
  .ph_allone2l_f6a_h            ( ph_allone2l_f6a_h          ),
  .ps_allone2r_f6a_h            ( ps_allone2r_f6a_h          ),
  .ps_allone2l_f6a_h            ( ps_allone2l_f6a_h          ),
  .sel_gmaxu24_f6a_h            ( sel_gmaxu24_f6a_h          ),
  .sel_gmaxu16_f6a_h            ( sel_gmaxu16_f6a_h          ),
  .sel_gmaxs16_f6a_h            ( sel_gmaxs16_f6a_h          ),
  .sel_gmins16_f6a_h            ( sel_gmins16_f6a_h          ),
  .sel_gmaxs8_f6a_h             ( sel_gmaxs8_f6a_h           ),
  .sel_gmaxu2_f6a_h             ( sel_gmaxu2_f6a_h           ),
  .sel_gmins8_f6a_h             ( sel_gmins8_f6a_h           ),
  .sel_gmaxu8_f6a_h             ( sel_gmaxu8_f6a_h           ),
  .sel_gfxmax_f6a_h             ( sel_gfxmax_f6a_h           ),
  .sel_psexc_sr_f6a_h           ( sel_psexc_sr_f6a_h         ),
  .sel_bit31_one_f6a_h          ( sel_bit31_one_f6a_h        ),
  .sel_b30to0_ones_f6a_h        ( sel_b30to0_ones_f6a_h      ),
  .sel_zerores_f6a_h            ( sel_zerores_f6a_h          ),
  .sel_ps_unf_f6a_h             ( sel_ps_unf_f6a_h           ),
  .sel_ps_normone_f6a_h         ( sel_ps_normone_f6a_h       ),
  .sel_ps_inf_f6a_h             ( sel_ps_inf_f6a_h           ),
  .sel_ps_cqn_f6a_h             ( sel_ps_cqn_f6a_h           ),
  .sel_ps_large_f6a_h           ( sel_ps_large_f6a_h         ),
  .sel_phsp_f6a_h               ( sel_phsp_f6a_h             ),
  .sel_phhi_sr_f6a_h            ( sel_phhi_sr_f6a_h          ),
  .sel_phhi_large_f6a_h         ( sel_phhi_large_f6a_h       ),
  .sel_phhi_unf_f6a_h           ( sel_phhi_unf_f6a_h         ),
  .sel_phhi_infres_f6a_h        ( sel_phhi_infres_f6a_h      ),
  .sel_f11inf_f6a_h             ( sel_f11inf_f6a_h           ),
  .sel_f10cqnan_f6a_h           ( sel_f10cqnan_f6a_h         ),
  .sel_f11sp_f6a_h              ( sel_f11sp_f6a_h            ),
  .sel_f10sp_f6a_h              ( sel_f10sp_f6a_h            ),
  .sel_f11large_f6a_h           ( sel_f11large_f6a_h         ),
  .sel_f10large_f6a_h           ( sel_f10large_f6a_h         ),
  .ps_rndovf_f6a_h              ( ps_rndovf_f6a_h            ),
  .ph_rndovf_f6a_h              ( ph_rndovf_f6a_h            ),
  .sel_set_bit0_one_f6a_h       ( sel_set_bit0_one_f6a_h     ),
  .effsub1_set_sign1_f6a_h      ( effsub1_set_sign1_f6a_h    ),
  .normmask2_f6a_h              ( normmask2_f6a_h            ),
  .normmask1_f6a_h              ( normmask1_f6a_h            ),
  .trans_sinp2m_f6a_h           ( trans_sinp2m_f6a_h         ),
  .f10f11mux_sel_f6a_h          ( f10f11mux_sel_f6a_h        ),
  .rrmux32_sel_f6a_h            ( rrmux32_sel_f6a_h          ),
  .rrmux16l_sel_f6a_l           ( rrmux16l_sel_f6a_l         ),
  .sel_excreshi_f6a_h           ( sel_excreshi_f6a_h         ),
  .sel_excreslo_f6a_h           ( sel_excreslo_f6a_h         ),
  .sel_fmares_hi_f6a_h          ( sel_fmares_hi_f6a_h        ),
  .sel_fmares_lo_f6a_h          ( sel_fmares_lo_f6a_h        ),
  .fma_flags_f6a_h              ( fma_flags_f6a_h            )
);

///////////////////////////////////////////////////////////////////////////////
// F6FF
///////////////////////////////////////////////////////////////////////////////

// The final ctrl_valid signal is used to qualify the TXFMA Out Data
rst_ff
#(
  .width($bits( ctrl_valid_f5a_h    ))
)
F6_CTRLVAL_FF
(
  .clock      ( clock                ),
  .reset      ( reset                ),
  .D          ( ctrl_valid_f5a_h     ),
  .Q          ( ctrl_valid_f6a_h     )
);

endmodule
