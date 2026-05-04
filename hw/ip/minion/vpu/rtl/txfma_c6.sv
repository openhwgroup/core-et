// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off IMPORTSTAR */
import dft_pkg::*;
import etlink_pkg::*;
import minion_pkg::*;
import minion_dcache_pkg::*;
import vpu_pkg::*;
import vpu_defs_pkg::*;
/* verilator lint_on IMPORTSTAR */

/* verilator lint_off UNUSEDSIGNAL */  // Imported original TXFMA control stage keeps the packed signal bus unchanged.
module txfma_c6
(
  input     [VPU_DTYPE_SZ-1:0]                 op_dtype_f6a_h,
  input     [VPU_FCMD_SZ-1:0]                  cmd_f6a_h,
  input                                         classify_zero_f6a_h,
  input     [TXFMA_SIGS_SZ-1:0]                sigs_f6a_h,
  input     [VPU_FLAGS_SZ-1:0]                 in_flags_f6a_h,
  input     [TXFMA_RM_SZ-1:0]                  rm_f6a_h,
  input                                         rmode_f6a_h,
  input                                         eff_sub1_f6a_h,
  input                                         exp_res_zero_f6a_h,
  input                                         s_res_f6a_h,
  input                                         flags_en_f6a_h,
  input                                         ediff1_lt_31_f6a_h,
  input                                         rc_ph_f6a_h,
  input                                         rc_ps_f6a_h,
  input                                         rc_int32_f6a_h,
  input                                         ph_stky2l_f6a_h,
  input                                         ps_stky2l_f6a_h,
  input                                         int32_stky2l_f6a_h,
  input                                         fround_stky2l_f6a_h,
  input                                         minmax_op_f6a_h,
  input                                         comp_op_f6a_h,
  input                                         comp_op_true_f6a_h,
  input                                         spec_res_en_f6a_h,
  input                                         norm_f32_ovf_f6a_h,
  input                                         norm_one_out_f6a_h,
  input                                         ec_zero_f6a_h,
  input                                         res_inf_f6a_h,
  input                                         res_neg_inf_f6a_h,
  input                                         res_qnan_f6a_h,
  input                                         res_snan_f6a_h,
  input                                         pot_ovf_ps_f6a_h,
  input                                         def_ovf_ps_f6a_h,
  input                                         pot_unf_ps_f6a_h,
  input                                         def_unf_ps_f6a_h,
  input                                         pot_ovf_ph_f6a_h,
  input                                         def_ovf_ph_f6a_h,
  input                                         pot_unf_ph_f6a_h,
  input                                         def_unf_ph_f6a_h,
  input                                         cvt_fui_pos_ovf_f6a_h,
  input                                         cvt_fi_pos_ovf_f6a_h,
  input                                         cvt_fi_neg_ovf_f6a_h,
  input                                         cvt_fi_min_neg_f6a_h,
  input                                         res_unorm_one_f6a_h,
  input                                         res_snorm_one_f6a_h,
  input                                         cmp_invalid_f6a_h,
  input                                         res_pos_int_nan_f6a_h,
  input                                         res_neg_int_nan_f6a_h,
  input                                         res_uint_nan_f6a_h,
  input                                         res_zero_f6a_h,
  input                                         res_one_f6a_h,
  input                                         a_inf_f6a_h,
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
  output                                        sel_f11large_f6a_h,
  output                                        sel_f10large_f6a_h,
  output                                        sel_f11inf_f6a_h,
  output                                        sel_f10cqnan_f6a_h,
  output                                        sel_f11sp_f6a_h,
  output                                        sel_f10sp_f6a_h,
  output                                        ps_rndovf_f6a_h,
  output                                        ph_rndovf_f6a_h,
  output                                        sel_set_bit0_one_f6a_h,
  output                                        effsub1_set_sign1_f6a_h,
  output                                        normmask1_f6a_h,
  output                                        normmask2_f6a_h,
  output                                        sel_classify_out_f6a_h,
  output                                        sel_nr1_frcpres_f6a_h,
  output                                        trans_sinp2m_f6a_h,
  output                                        f10f11mux_sel_f6a_h,
  output                                        rrmux16l_sel_f6a_l,
  output       [1:0]                            rrmux32_sel_f6a_h,
  output       [1:0]                            sel_excreshi_f6a_h,
  output       [1:0]                            sel_excreslo_f6a_h,
  output       [1:0]                            sel_fmares_hi_f6a_h,
  output       [1:0]                            sel_fmares_lo_f6a_h,
  output       [VPU_FLAGS_SZ-1:0]              fma_flags_f6a_h

);

logic                       classify_zero_mux_f6a_h;
logic  [VPU_FLAGS_SZ-1:0]  out_flags_f6a_h;
logic                       res_uint_nan_mux_f6a_h;
logic                       res_inf_mux_f6a_h;
logic                       res_qnan_mux_f6a_h;
logic                       res_snan_mux_f6a_h;
logic                       res_pos_int_nan_mux_f6a_h;
logic                       res_pos_int_nan_out_f6a_h;
logic                       res_neg_int_nan_mux_f6a_h;
logic                       res_neg_int_nan_out_f6a_h;
logic                       res_unorm_one_mux_f6a_h;
logic                       res_snorm_one_mux_f6a_h;
logic                       res_denormal_f6a_h;
logic                       inexact_clr_f6a_h;
logic                       res_uint_nan_out_f6a_h;
logic                       res_inf_out_f6a_h;
logic                       res_nan_f6a_h;
logic                       ffrc_op_f6a_h;
logic                       int32_res_f6a_h;
logic                       mul_int32_res_f6a_h;
logic                       fp_ovf_ps_f6a_h;
logic                       fp_ovf_phf_f6a_h;
logic                       fp_ps_ovf_res_inf_f6a_h;
logic                       fp_ph_ovf_res_inf_f6a_h;
logic                       rne_f6a_h;
logic                       rup_f6a_h;
logic                       rdn_f6a_h;
logic                       rmm_f6a_h;
logic                       rtz_f6a_h;
logic                       cqnan_res_f6a_h;
logic                       invalid_op_f6a_h;
logic                       sel_gfxmaxmin_res_f6a_h;
logic                       rc_phf_f6a_h;
logic                       sel_cmptrue_f6a_h;
logic                       sel_cmpfalse_f6a_h;
logic                       res_zero_mux_f6a_h;
logic                       res_one_mux_f6a_h;
logic                       qual_ph_stky2l_f6a_h;
logic                       qual_ps_stky2l_f6a_h;
logic                       qual_int32_stky2l_f6a_h;
logic                       ps_exc_res_en_f6a_h;
logic                       ps_exc_res_en2_f6a_h;
logic                       ph_exc_res_en_f6a_h;
logic                       sel_transop_snan_f6a_h;
logic                       sat_to_zero_f6a_h;
logic                       unf_f6a_h;
logic                       sel_sin_snan_f6a_h;
logic                       sel_log_snan_f6a_h;
logic                       sel_exp_snan_f6a_h;
logic                       fp_no_ovf_ps_f6a_h;
logic                       fp_trans_no_ovf_ps_f6a_h;
logic                       sel_ps_no_unf_f6a_h;
logic                       exc_lo_f6a_h;
logic                       exc_hi_f6a_h;
logic                       rr16_rndovf_f6a_h;


//========================================================================================================================
// Pipestage 6 Rounding & Determination of Round Overflow
//========================================================================================================================

assign rne_f6a_h   = (rm_f6a_h == RNE);
assign rup_f6a_h   = (rm_f6a_h == RUP);
assign rdn_f6a_h   = (rm_f6a_h == RDN);
assign rmm_f6a_h   = (rm_f6a_h == RMM);
assign rtz_f6a_h   = (rm_f6a_h == RTZ);

assign effsub1_set_sign1_f6a_h =  (sigs_f6a_h[TXFMA_FP32_ADD_SUB] & rdn_f6a_h & eff_sub1_f6a_h & !ec_zero_f6a_h & exp_res_zero_f6a_h);

assign sel_sin_snan_f6a_h = (cmd_f6a_h==VPU_TRANS_SIN_P1) | (cmd_f6a_h==VPU_TRANS_SIN_P2) | (cmd_f6a_h==VPU_FCMD_FFRC_SIN) | (cmd_f6a_h==VPU_TRANS_SIN_TRANSFORM) | (cmd_f6a_h==VPU_TRANS_SIN_RR);
assign sel_log_snan_f6a_h = (cmd_f6a_h==VPU_TRANS_LOG_RR) | (cmd_f6a_h==VPU_TRANS_LOG_FMA1);
assign sel_exp_snan_f6a_h = (cmd_f6a_h==VPU_TRANS_EXP_FRAC) | (cmd_f6a_h==VPU_TRANS_EXP_RR) | (cmd_f6a_h==VPU_TRANS_EXP_FMA1);

assign sel_transop_snan_f6a_h = res_snan_mux_f6a_h & (sel_sin_snan_f6a_h | sel_log_snan_f6a_h | sel_exp_snan_f6a_h | (cmd_f6a_h==VPU_TRANS_RCP_FMA1) | (cmd_f6a_h==VPU_TRANS_RSQRT_FMA1));

assign sat_to_zero_f6a_h = s_res_f6a_h & ((cmd_f6a_h == VPU_FCMD_CVT_F10F32) | (cmd_f6a_h == VPU_FCMD_CVT_F11F32));

assign ps_rndovf_f6a_h = rc_ps_f6a_h & ((ps_allone2r_f6a_h & rnd_r_ps_f6a_h) | (ps_allone2l_f6a_h & rnd_l_ps_f6a_h));
assign ph_rndovf_f6a_h = rc_ph_f6a_h & ((ph_allone2r_f6a_h & rnd_r_ph_f6a_h) | (ph_allone2l_f6a_h & rnd_l_ph_f6a_h));

assign fp_trans_no_ovf_ps_f6a_h = sigs_f6a_h[TXFMA_TRANS2] | (cmd_f6a_h == VPU_TRANS_LOG_MUL) | (cmd_f6a_h == VPU_TRANS_SIN_TRANSFORM) | (cmd_f6a_h == VPU_TRANS_LOG_RR);

assign fp_no_ovf_ps_f6a_h = comp_op_f6a_h | minmax_op_f6a_h | sigs_f6a_h[TXFMA_F32_NORM] | fp_trans_no_ovf_ps_f6a_h
                          | ffrc_op_f6a_h | cqnan_res_f6a_h  | res_inf_mux_f6a_h  | sel_transop_snan_f6a_h;

assign fp_ovf_ps_f6a_h  = (def_ovf_ps_f6a_h | (pot_ovf_ps_f6a_h & ps_rndovf_f6a_h)) & ~fp_no_ovf_ps_f6a_h;

assign fp_ovf_phf_f6a_h = (def_ovf_ph_f6a_h | (pot_ovf_ph_f6a_h & ph_rndovf_f6a_h)) & rc_phf_f6a_h & ~(res_inf_f6a_h | cqnan_res_f6a_h);

assign qual_ph_stky2l_f6a_h = ph_stky2l_f6a_h & rc_ph_f6a_h;
assign qual_ps_stky2l_f6a_h = (ps_stky2l_f6a_h & rc_ps_f6a_h) | (fround_stky2l_f6a_h & (cmd_f6a_h==VPU_FCMD_FROUND));
assign qual_int32_stky2l_f6a_h = int32_stky2l_f6a_h & rc_int32_f6a_h;

//========================================================================================================================
// Result MUX Control
//========================================================================================================================

// Invalid Operation
assign invalid_op_f6a_h = out_flags_f6a_h[VPU_FLAG_INVALID];

// Masks to force '0 for SNORM8/SNORM16
assign mul_int32_res_f6a_h = (cmd_f6a_h ==VPU_FCMD_MULL) | (cmd_f6a_h ==VPU_FCMD_MULLU) | (cmd_f6a_h ==VPU_FCMD_MULH) | (cmd_f6a_h ==VPU_FCMD_MULHU);

assign int32_res_f6a_h =  (cmd_f6a_h ==VPU_FCMD_CVT_INTF32) | (cmd_f6a_h == VPU_FCMD_CVT_UINTF32) | sigs_f6a_h[TXFMA_NORM_F32] | mul_int32_res_f6a_h;

assign normmask1_f6a_h = ~((cmd_f6a_h == VPU_FCMD_CVT_SN8F32) | (cmd_f6a_h == VPU_FCMD_CVT_SN16F32));
assign normmask2_f6a_h = ~(cmd_f6a_h == VPU_FCMD_CVT_SN8F32);

assign sel_classify_out_f6a_h = (cmd_f6a_h == VPU_FCMD_CLASS);
assign sel_nr1_frcpres_f6a_h  = (cmd_f6a_h == VPU_FCMD_NR1_FRCPFXP);

assign trans_sinp2m_f6a_h = (cmd_f6a_h == VPU_TRANS_SIN_P2);

assign rrmux16l_sel_f6a_l   = (cmd_f6a_h != VPU_FCMD_CVT_F16F32);
assign rrmux32_sel_f6a_h[1] = ((cmd_f6a_h==VPU_TRANS_LOG_FMA2) | (cmd_f6a_h==VPU_TRANS_LOG_RR) | ((cmd_f6a_h==VPU_TRANS_SIN_P2) & (op_dtype_f6a_h == VPU_DTYPE_INT)) |
                               (cmd_f6a_h==VPU_FCMD_NR1_FRCPFXP) |  (cmd_f6a_h==VPU_FCMD_CLASS));
assign rrmux32_sel_f6a_h[0] = ((cmd_f6a_h==VPU_TRANS_EXP_FRAC) | sigs_f6a_h[TXFMA_NORM_F32] | int32_res_f6a_h | (cmd_f6a_h==VPU_TRANS_LOG_RR) | ((cmd_f6a_h==VPU_TRANS_SIN_P2) & (op_dtype_f6a_h == VPU_DTYPE_INT)) | (sigs_f6a_h[TXFMA_TRANS1] & (cmd_f6a_h!=VPU_TRANS_LOG_FMA2)) | (cmd_f6a_h==VPU_FCMD_CVT_RASTF32) | (cmd_f6a_h==VPU_FCMD_NR2_FRCPFXP));


// 0 --> f10 (default) 1 --> f11
assign f10f11mux_sel_f6a_h  = (cmd_f6a_h == VPU_FCMD_CVT_F11F32);

assign sel_psexc_sr_f6a_h   = rc_ps_f6a_h & ~(res_unorm_one_mux_f6a_h | invalid_op_f6a_h | res_pos_int_nan_out_f6a_h | cqnan_res_f6a_h | (cmd_f6a_h==VPU_TRANS_EXP_FMA2 && res_one_mux_f6a_h) ); // | sel_transop_snan_f6a_h;  // pass sign for trans_snan sr, 7f80 0001

assign sel_set_bit0_one_f6a_h = sel_transop_snan_f6a_h | res_denormal_f6a_h;
assign sel_bit31_one_f6a_h = res_neg_inf_f6a_h | comp_op_true_f6a_h | res_uint_nan_out_f6a_h | res_neg_int_nan_out_f6a_h | (sigs_f6a_h[TXFMA_FP32_ADD_SUB] & rdn_f6a_h & eff_sub1_f6a_h & !ec_zero_f6a_h & frac_zero_f6a_h & sel_ps_unf_f6a_h);

assign sel_cmptrue_f6a_h      = (comp_op_f6a_h & comp_op_true_f6a_h);
assign sel_b30to0_ones_f6a_h  = sel_cmptrue_f6a_h | res_pos_int_nan_out_f6a_h | res_uint_nan_out_f6a_h;
assign sel_cmpfalse_f6a_h     = (comp_op_f6a_h & ~comp_op_true_f6a_h);

assign sel_zerores_f6a_h    = sel_cmpfalse_f6a_h | res_zero_mux_f6a_h | res_neg_int_nan_out_f6a_h | (sigs_f6a_h[TXFMA_F32_NORM] & (classify_zero_mux_f6a_h)) | res_denormal_f6a_h;

assign sel_ps_no_unf_f6a_h  = comp_op_f6a_h  | minmax_op_f6a_h | sigs_f6a_h[TXFMA_F32_NORM] | (cmd_f6a_h == VPU_TRANS_LOG_MUL)
                            | sigs_f6a_h[TXFMA_TRANS2] | (cmd_f6a_h == VPU_TRANS_SIN_TRANSFORM) | ffrc_op_f6a_h | cqnan_res_f6a_h  | res_inf_mux_f6a_h  | sel_transop_snan_f6a_h;
assign sel_ps_unf_f6a_h     = (rc_ps_f6a_h & (def_unf_ps_f6a_h | (pot_unf_ps_f6a_h & ~ps_rndovf_f6a_h))) & ~sel_ps_no_unf_f6a_h;

assign sel_ps_normone_f6a_h = ((sigs_f6a_h[TXFMA_F32_NORM] & (res_unorm_one_mux_f6a_h | res_snorm_one_mux_f6a_h)) |  res_one_mux_f6a_h); // | (cmd_f6a_h==VPU_TRANS_EXP_FMA2 && res_one_mux_f6a_h) );

assign fp_ps_ovf_res_inf_f6a_h = fp_ovf_ps_f6a_h & ( rne_f6a_h | rmm_f6a_h | ((rup_f6a_h & ~s_res_f6a_h) | (rdn_f6a_h & s_res_f6a_h)));


assign fp_ph_ovf_res_inf_f6a_h = fp_ovf_phf_f6a_h & ( rne_f6a_h | rmm_f6a_h | ((rup_f6a_h & ~s_res_f6a_h) | (rdn_f6a_h & s_res_f6a_h)));

// For passing SNAN for TRANS Ops, sel_inf_result and set bit0
assign sel_ps_inf_f6a_h     = sel_ps_normone_f6a_h | sel_transop_snan_f6a_h | ((res_inf_mux_f6a_h | fp_ps_ovf_res_inf_f6a_h | res_neg_inf_f6a_h) & (rc_ps_f6a_h | (cmd_f6a_h==VPU_TRANS_EXP_FRAC)) & ~comp_op_f6a_h & ~((cmd_f6a_h==VPU_FCMD_FFRC)));

assign cqnan_res_f6a_h  = (res_qnan_mux_f6a_h | res_snan_mux_f6a_h & !sel_transop_snan_f6a_h);
assign sel_ps_cqn_f6a_h = cqnan_res_f6a_h & (rc_ps_f6a_h | (cmd_f6a_h==VPU_FCMD_FROUND) | (cmd_f6a_h==VPU_TRANS_EXP_FRAC));

assign sel_ps_large_f6a_h    = fp_ovf_ps_f6a_h & (rtz_f6a_h | (rup_f6a_h & s_res_f6a_h) | (rdn_f6a_h & ~s_res_f6a_h));
assign sel_phhi_sr_f6a_h     = ~((cmd_f6a_h == VPU_FCMD_CVT_F10F32) | (cmd_f6a_h == VPU_FCMD_CVT_F11F32) | invalid_op_f6a_h | cqnan_res_f6a_h);
assign sel_phhi_large_f6a_h  = (fp_ovf_phf_f6a_h & rc_ph_f6a_h & (rtz_f6a_h | (rup_f6a_h & s_res_f6a_h) | (rdn_f6a_h & ~s_res_f6a_h))) | (sel_f10large_f6a_h) ;
assign sel_phhi_unf_f6a_h    = sat_to_zero_f6a_h | ((def_unf_ph_f6a_h | (pot_unf_ph_f6a_h & ~ph_rndovf_f6a_h)) & rc_phf_f6a_h & ~(cqnan_res_f6a_h | res_inf_f6a_h))  ;
assign sel_f10cqnan_f6a_h    = (cmd_f6a_h == VPU_FCMD_CVT_F10F32) & cqnan_res_f6a_h;
assign sel_f11inf_f6a_h      = (cmd_f6a_h == VPU_FCMD_CVT_F11F32) & res_inf_f6a_h;

assign sel_f10sp_f6a_h       = (cmd_f6a_h == VPU_FCMD_CVT_F10F32) & (cqnan_res_f6a_h | res_inf_f6a_h | fp_ovf_phf_f6a_h);
assign sel_f11sp_f6a_h       = (cmd_f6a_h == VPU_FCMD_CVT_F11F32) & (cqnan_res_f6a_h | res_inf_f6a_h | fp_ovf_phf_f6a_h);
assign sel_f11large_f6a_h    = fp_ovf_phf_f6a_h & (cmd_f6a_h == VPU_FCMD_CVT_F11F32);
assign sel_f10large_f6a_h    = fp_ovf_phf_f6a_h & (cmd_f6a_h == VPU_FCMD_CVT_F10F32);
assign sel_phhi_infres_f6a_h = (fp_ph_ovf_res_inf_f6a_h | res_inf_f6a_h) & rc_ph_f6a_h;
assign sel_phsp_f6a_h        = fp_ph_ovf_res_inf_f6a_h | (rc_ph_f6a_h & (cqnan_res_f6a_h | res_inf_f6a_h)) | (rc_phf_f6a_h & sel_phhi_unf_f6a_h);


// generating min/max MUX select for NORM Data Types
//
assign sel_gmaxu24_f6a_h = (cmd_f6a_h == VPU_FCMD_CVT_UN24F32);
assign sel_gmaxu16_f6a_h = (cmd_f6a_h == VPU_FCMD_CVT_UN16F32);
assign sel_gmins16_f6a_h = (cmd_f6a_h == VPU_FCMD_CVT_SN16F32);
assign sel_gmaxs16_f6a_h = (cmd_f6a_h == VPU_FCMD_CVT_SN16F32);
assign sel_gmaxs8_f6a_h  = (cmd_f6a_h == VPU_FCMD_CVT_SN8F32);
assign sel_gmins8_f6a_h  = (cmd_f6a_h == VPU_FCMD_CVT_SN8F32);
assign sel_gmaxu8_f6a_h  = (cmd_f6a_h == VPU_FCMD_CVT_UN8F32);
assign sel_gmaxu2_f6a_h  = (cmd_f6a_h == VPU_FCMD_CVT_UN2F32);

assign sel_gfxmax_f6a_h = (sigs_f6a_h[TXFMA_NORM_F32]) & ( res_nan_f6a_h | (res_inf_mux_f6a_h | norm_f32_ovf_f6a_h | norm_one_out_f6a_h) & ~s_res_f6a_h);

assign sel_gfxmaxmin_res_f6a_h = sel_gfxmax_f6a_h | (sigs_f6a_h[TXFMA_NORM_F32]) &  (res_inf_mux_f6a_h | norm_f32_ovf_f6a_h | norm_one_out_f6a_h) & s_res_f6a_h;

assign ps_exc_res_en_f6a_h  = (sel_bit31_one_f6a_h | sel_b30to0_ones_f6a_h | sel_zerores_f6a_h | sel_set_bit0_one_f6a_h | sel_ps_normone_f6a_h)  & ~sel_classify_out_f6a_h;
assign ps_exc_res_en2_f6a_h = (sel_ps_unf_f6a_h | sel_ps_cqn_f6a_h | sel_ps_large_f6a_h | sel_ps_inf_f6a_h | sel_gfxmaxmin_res_f6a_h) & ~sel_classify_out_f6a_h;
assign ph_exc_res_en_f6a_h = sel_phhi_large_f6a_h | sel_phhi_unf_f6a_h | sel_f10cqnan_f6a_h | sel_f11sp_f6a_h | sel_f10sp_f6a_h
                           | sel_f11inf_f6a_h | sel_f10large_f6a_h | sel_phsp_f6a_h | sel_f11large_f6a_h | sel_phhi_infres_f6a_h;
//ss
assign rc_phf_f6a_h = rc_ph_f6a_h | (cmd_f6a_h == VPU_FCMD_CVT_F10F32) | (cmd_f6a_h == VPU_FCMD_CVT_F11F32);
assign sel_excreshi_f6a_h[1] = sel_gfxmaxmin_res_f6a_h;
assign sel_excreshi_f6a_h[0] = (rc_ps_f6a_h | rc_int32_f6a_h) & (ps_exc_res_en_f6a_h | ps_exc_res_en2_f6a_h) & ~sel_gfxmaxmin_res_f6a_h; // ss int32 and ps_exc_res_en can fire for gfxmaxmin

assign sel_excreslo_f6a_h[1] = sel_gfxmaxmin_res_f6a_h | (ph_exc_res_en_f6a_h & (rc_phf_f6a_h ));
assign sel_excreslo_f6a_h[0] = sel_excreshi_f6a_h[0]  | (rc_phf_f6a_h & ph_exc_res_en_f6a_h);

assign exc_hi_f6a_h = (|sel_excreshi_f6a_h[1:0] | (cmd_f6a_h == VPU_FCMD_CVT_F10F32) | f10f11mux_sel_f6a_h | ph_rndovf_f6a_h);
assign rr16_rndovf_f6a_h = (ps_rndovf_f6a_h | ~rrmux16l_sel_f6a_l);
assign sel_fmares_hi_f6a_h[0] = rr16_rndovf_f6a_h & ~exc_hi_f6a_h ;
assign sel_fmares_hi_f6a_h[1] = ~exc_hi_f6a_h  & ~rr16_rndovf_f6a_h;

////assign sel_fmares_lo_f6a_h[1] = |sel_excreslo_f6a_h[1:0] | (f10f11mux_sel_f6a_h | (cmd_f6a_h == VPU_FCMD_CVT_F10F32) | ph_rndovf_f6a_h ;
assign exc_lo_f6a_h = (|sel_excreslo_f6a_h[1:0] | (f10f11mux_sel_f6a_h | (cmd_f6a_h == VPU_FCMD_CVT_F10F32) | ph_rndovf_f6a_h));
assign sel_fmares_lo_f6a_h[0] = rr16_rndovf_f6a_h & ~ exc_lo_f6a_h; // ps_rndovf_f6a_h | ~rrmux16l_sel_f6a_l;

assign sel_fmares_lo_f6a_h[1] = ~exc_lo_f6a_h & ~rr16_rndovf_f6a_h;





//========================================================================================================================
// Flag Generation
//========================================================================================================================

assign res_nan_f6a_h = res_qnan_mux_f6a_h | res_snan_mux_f6a_h;

assign ffrc_op_f6a_h   = ((cmd_f6a_h==VPU_FCMD_FFRC) || (cmd_f6a_h == VPU_FCMD_FFRC_SIN));


  //gen special special
always_comb begin
  classify_zero_mux_f6a_h      = 1'b0;
  res_uint_nan_mux_f6a_h       = 1'b0;
  res_inf_mux_f6a_h            = 1'b0;
  res_qnan_mux_f6a_h           = 1'b0;
  res_snan_mux_f6a_h           = 1'b0;
  res_pos_int_nan_mux_f6a_h    = 1'b0;
  res_neg_int_nan_mux_f6a_h    = 1'b0;
  res_unorm_one_mux_f6a_h      = 1'b0;
  res_snorm_one_mux_f6a_h      = 1'b0;
  res_zero_mux_f6a_h           = 1'b0;
  res_one_mux_f6a_h            = 1'b0;

  if (spec_res_en_f6a_h) begin
    classify_zero_mux_f6a_h      = classify_zero_f6a_h;
    res_uint_nan_mux_f6a_h       = res_uint_nan_f6a_h;
    res_inf_mux_f6a_h            = res_inf_f6a_h;
    res_qnan_mux_f6a_h           = res_qnan_f6a_h;
    res_snan_mux_f6a_h           = res_snan_f6a_h;
    res_pos_int_nan_mux_f6a_h    = res_pos_int_nan_f6a_h;
    res_neg_int_nan_mux_f6a_h    = res_neg_int_nan_f6a_h;
    res_unorm_one_mux_f6a_h      = res_unorm_one_f6a_h;
    res_snorm_one_mux_f6a_h      = res_snorm_one_f6a_h;
    res_zero_mux_f6a_h           = res_zero_f6a_h;
    res_one_mux_f6a_h            = res_one_f6a_h;
  end
end


always_comb begin

  res_denormal_f6a_h= '0;
  out_flags_f6a_h= '0;
  res_uint_nan_out_f6a_h = '0;
  res_pos_int_nan_out_f6a_h = '0;
  res_neg_int_nan_out_f6a_h = '0;
  res_inf_out_f6a_h = '0;
  unf_f6a_h = '0;


   //inexact flag clear for trans ops
  inexact_clr_f6a_h = (cmd_f6a_h==VPU_TRANS_SIN_P3) | (cmd_f6a_h==VPU_TRANS_EXP_FMA2) | (cmd_f6a_h==VPU_FCMD_NR2_FRCPFXP) | (cmd_f6a_h==VPU_TRANS_LOG_MUL) | (cmd_f6a_h==VPU_TRANS_RSQRT_FMA2)
                    | (cmd_f6a_h==VPU_TRANS_RCP_FMA2) | (cmd_f6a_h == VPU_FCMD_FFRC) | (cmd_f6a_h == VPU_FCMD_FFRC_SIN);


  if (flags_en_f6a_h) begin
    out_flags_f6a_h = in_flags_f6a_h;
  end

  out_flags_f6a_h[VPU_FLAG_INEXACT] = (qual_ph_stky2l_f6a_h | qual_ps_stky2l_f6a_h)  & ~((op_dtype_f6a_h == VPU_DTYPE_INT) & sigs_f6a_h[TXFMA_MUL]);

      //trans invalid flag
  if (inexact_clr_f6a_h)
    out_flags_f6a_h[VPU_FLAG_INVALID] = (out_flags_f6a_h[VPU_FLAG_INVALID] | res_snan_mux_f6a_h);


    // F32 + TENSOR.PS + CVT + GCVT
  if ((op_dtype_f6a_h == VPU_DTYPE_F32) | (op_dtype_f6a_h == VPU_DTYPE_F16_F32) | sigs_f6a_h[TXFMA_NORM_F32] |
      ((op_dtype_f6a_h != VPU_DTYPE_F32) & sigs_f6a_h[TXFMA_F32_NORM]) | sigs_f6a_h[TXFMA_TRANS2] | sigs_f6a_h[TXFMA_F32_FX]) begin

      //internal flags
    res_pos_int_nan_out_f6a_h = (res_pos_int_nan_mux_f6a_h | cvt_fi_pos_ovf_f6a_h) & (cmd_f6a_h == VPU_FCMD_CVT_INTF32 | cmd_f6a_h == VPU_FCMD_CVT_RASTF32);
    res_neg_int_nan_out_f6a_h = (res_neg_int_nan_mux_f6a_h | (cvt_fi_min_neg_f6a_h & !res_pos_int_nan_out_f6a_h) ) & (cmd_f6a_h == VPU_FCMD_CVT_INTF32 | cmd_f6a_h == VPU_FCMD_CVT_RASTF32);

    res_uint_nan_out_f6a_h    = (res_uint_nan_mux_f6a_h | cvt_fui_pos_ovf_f6a_h) & (cmd_f6a_h == VPU_FCMD_CVT_UINTF32);

    res_inf_out_f6a_h = (res_inf_mux_f6a_h | fp_ovf_ps_f6a_h & rmode_f6a_h) & ~comp_op_f6a_h & ~((cmd_f6a_h==VPU_FCMD_FFRC) | (cmd_f6a_h==VPU_FCMD_FFRC_SIN)) | ((sigs_f6a_h[TXFMA_NORM_F32]) & (norm_f32_ovf_f6a_h | norm_one_out_f6a_h));

    unf_f6a_h = (def_unf_ps_f6a_h | pot_unf_ps_f6a_h & ~ps_rndovf_f6a_h) &
                ~(comp_op_f6a_h  | minmax_op_f6a_h | sigs_f6a_h[TXFMA_F32_NORM] | (cmd_f6a_h == VPU_TRANS_LOG_MUL) | sigs_f6a_h[TXFMA_TRANS2] | (cmd_f6a_h == VPU_TRANS_SIN_TRANSFORM) | ffrc_op_f6a_h);

     //output flags

    out_flags_f6a_h[VPU_FLAG_INEXACT] = (qual_ph_stky2l_f6a_h | qual_ps_stky2l_f6a_h | qual_int32_stky2l_f6a_h)  &
                                              ~( out_flags_f6a_h[VPU_FLAG_DIV0] | sigs_f6a_h[TXFMA_TRANS2] | res_inf_mux_f6a_h | minmax_op_f6a_h | res_nan_f6a_h |
                                             comp_op_f6a_h | sigs_f6a_h[TXFMA_NORM_F32] | sigs_f6a_h[TXFMA_F32_NORM]);

    out_flags_f6a_h[VPU_FLAG_INEXACT] |= (fp_ovf_ps_f6a_h & ~res_inf_mux_f6a_h & ~res_nan_f6a_h);

    out_flags_f6a_h[VPU_FLAG_INEXACT] |= fp_ovf_ps_f6a_h | (sel_ps_unf_f6a_h & ~(res_zero_mux_f6a_h | frac_zero_f6a_h));

    out_flags_f6a_h[VPU_FLAG_OVF] |= fp_ovf_ps_f6a_h & ~(out_flags_f6a_h[VPU_FLAG_DIV0] | sigs_f6a_h[TXFMA_TRANS2] | res_inf_mux_f6a_h | res_nan_f6a_h | sigs_f6a_h[TXFMA_CVT] |
                                                       sigs_f6a_h[TXFMA_NORM_F32] | sigs_f6a_h[TXFMA_F32_NORM] | sigs_f6a_h[TXFMA_F32_FX]);

    out_flags_f6a_h[VPU_FLAG_UNF] |= sel_ps_unf_f6a_h & ~(out_flags_f6a_h[VPU_FLAG_DIV0] | sigs_f6a_h[TXFMA_TRANS2] | res_inf_mux_f6a_h | res_nan_f6a_h | sigs_f6a_h[TXFMA_F32_FX] |
                                                      (frac_zero_f6a_h & !out_flags_f6a_h[VPU_FLAG_INEXACT]) | sigs_f6a_h[TXFMA_CVT] | sigs_f6a_h[TXFMA_NORM_F32] | sigs_f6a_h[TXFMA_F32_NORM] | (op_dtype_f6a_h == VPU_DTYPE_F16_F32));

    out_flags_f6a_h[VPU_FLAG_INVALID] = (out_flags_f6a_h[VPU_FLAG_INVALID] & ~(comp_op_f6a_h | sigs_f6a_h[TXFMA_NORM_F32] | sigs_f6a_h[TXFMA_F32_NORM])) | cmp_invalid_f6a_h;


      //convert
    if (sigs_f6a_h[TXFMA_CVT]) begin
      out_flags_f6a_h[VPU_FLAG_INVALID] = ((res_pos_int_nan_out_f6a_h | cvt_fi_neg_ovf_f6a_h | res_neg_int_nan_mux_f6a_h) &
                                               ((cmd_f6a_h == VPU_FCMD_CVT_INTF32) | (cmd_f6a_h == VPU_FCMD_CVT_RASTF32))) |
                                              ((res_uint_nan_out_f6a_h | res_zero_mux_f6a_h & ~(ediff1_lt_31_f6a_h & ~int_res_l_bit_f6a_h)) & ~ec_zero_f6a_h & (cmd_f6a_h == VPU_FCMD_CVT_UINTF32)) |
                                              ((cmd_f6a_h==VPU_FCMD_FROUND) & res_snan_mux_f6a_h);

      out_flags_f6a_h[VPU_FLAG_INEXACT] &= !out_flags_f6a_h[VPU_FLAG_INVALID];
    end


      //exp frac: generate invalid flag when generating zero, infinity or qnan
    if (cmd_f6a_h == VPU_TRANS_EXP_FRAC) begin
      res_denormal_f6a_h = out_flags_f6a_h[VPU_FLAG_DENORMAL];
      out_flags_f6a_h='0;
      out_flags_f6a_h[VPU_FLAG_INVALID] = res_zero_mux_f6a_h | res_inf_mux_f6a_h | res_nan_f6a_h | res_one_mux_f6a_h | res_denormal_f6a_h | res_neg_inf_f6a_h;
    end
      //ffrc sin: propagate denormal
    if ((cmd_f6a_h == VPU_TRANS_SIN_TRANSFORM) || (cmd_f6a_h == VPU_TRANS_SIN_P1) || (cmd_f6a_h == VPU_TRANS_SIN_P2) || (cmd_f6a_h == VPU_FCMD_FFRC_SIN)) begin
      res_denormal_f6a_h = out_flags_f6a_h[VPU_FLAG_DENORMAL];
    end

    if (sigs_f6a_h[TXFMA_F32_NORM] & (classify_zero_mux_f6a_h | res_unorm_one_mux_f6a_h | res_snorm_one_mux_f6a_h))
      out_flags_f6a_h[VPU_FLAG_INEXACT] = 1'b0;

  end // F32 + TENSOR.PS + CVT + GCVT


// cvt ps to ph and cvt f32 to fx
  if (sigs_f6a_h[TXFMA_FX_F32]) begin
    unf_f6a_h = (def_unf_ph_f6a_h | pot_unf_ph_f6a_h & ~ph_rndovf_f6a_h);
    res_inf_out_f6a_h = (res_inf_f6a_h | fp_ovf_phf_f6a_h & rmode_f6a_h);

    out_flags_f6a_h[VPU_FLAG_INEXACT] |= fp_ovf_phf_f6a_h | (res_inf_out_f6a_h & (sigs_f6a_h[TXFMA_FX_F32] && ~a_inf_f6a_h));
    out_flags_f6a_h[VPU_FLAG_OVF]      = fp_ovf_phf_f6a_h;
    out_flags_f6a_h[VPU_FLAG_UNF]      = unf_f6a_h;

    if (unf_f6a_h) begin
      out_flags_f6a_h[VPU_FLAG_INEXACT] = (res_zero_mux_f6a_h | frac_zero_f6a_h) ? out_flags_f6a_h[VPU_FLAG_INEXACT] : 1'b1;
    end

  end

    //clear inexact flag for trans ops, assuming trans computation results are approximations
  out_flags_f6a_h[VPU_FLAG_INEXACT] &= !inexact_clr_f6a_h;

end //always_comb begin

assign fma_flags_f6a_h = out_flags_f6a_h;

endmodule
/* verilator lint_on UNUSEDSIGNAL */
