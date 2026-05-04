// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_c5
(
  input [`VPU_DTYPE_SZ-1:0]                        op_dtype_f5a_h,
  input [`VPU_FCMD_SZ-1:0]                         cmd_f5a_h,
  input [`TXFMA_SIGS_SZ-1:0]                       sigs_f5a_h,
  input [`FP_RM_BITS-1:0]                          rm_f5a_h,
  input [`TXFMA_LXD_PS_SZ-1:0]                     elxd_f5a_h,    
  input [`TXFMA_WSPS_ETRZ_SZ-1:0]                  etrz50_f5a_h,
  input                                            ec_zero_f5a_h,
  input                                            prod_sign_hi_f5a_h,
  input                                            s_res_f5a_h,
  input                                            s_res_check_cadd_a3_f5a_h,
  input                                            comp_res_sel_f5a_h,
  input                                            comp_op_f5a_h,
  input                                            minmax_op_f5a_h,
  input                                            trans_log_rr_neg_f5a_h,
  input                                            ediff1_zap_f5a_h,
  input                                            fa_eq_fc_res_check_hi_f5a_h,
  input                                            fc_sel_f5a_h,
  input                                            stky2g_f5a_h,
  input                                            comb_add_a3_f5a_h,
  output logic                                     rc_ps_f5a_h,
  output logic                                     rc_ph_f5a_h,
  output                                           rc6_ps_f5a_h,
  output                                           rc6_ph_f5a_h,
  output logic                                     rc_int32_f5a_h,
  output                                           rc_3rmxr_f5a_h,
  output                                           rc_rl01_f5a_h,
  output                                           rc_rmm_f5a_h,
  output                                           rc_rup_f5a_h,
  output                                           rc_rne_f5a_h,
  output                                           rc_rdn_f5a_h,
  output                                           rc_res_sign_f5a_l,
  output                                           rc_transrnd_f5a_h,
  output                                           rc_transrnd_log_fma2_f5a_h,
  output                                           nshcen_sel1b_f5a_h,
  output                                           en_logrrsel1b_f5a_l,
  output                                           int32_stky2r_f5a_h,
  output                                           mmx_sel_immres_f5a_h,
  output logic                                     comp_op_true_f5a_h,
  output                                           trans2_ps_roundup_r_f5a_h,
  output logic                                     e_force_to_zero_f5a_h,
  output logic                                     s_res_out_f5a_h
);

logic rm_rne_f5a_h;
logic rm_rup_f5a_h;
logic rm_rdn_f5a_h;
logic rm_rmm_f5a_h;
logic rm_rtz_f5a_h;
logic trans1_int_roundup_l_f5a_h;
logic cvt_rastf32_roundup_l_f5a_h;
logic [`FP_RM_BITS-1:0]  rm_mod_f5a_h;
logic rm_rmm_f32_to_norm_cvt_f5a_h;
logic rm_rne_cvt_f32rast_f5a_h;
logic rm_clr_f5a_h;
logic rc_ps_clr_f5a_h;
logic comp_flt_true_f5a_h;
logic comp_fle_true_f5a_h;
logic comp_feq_true_f5a_h;


//================================================================================================================
// Clear rounding mode for compare,minmax,fround,ffrc instructions
//================================================================================================================
assign rm_clr_f5a_h = comp_op_f5a_h | minmax_op_f5a_h | (cmd_f5a_h==`VPU_FCMD_FROUND) | (cmd_f5a_h==`VPU_FCMD_FFRC) | (cmd_f5a_h==`VPU_FCMD_FFRC_SIN);

//================================================================================================================
// Set RNE rounding mode for f32 rast
//================================================================================================================
assign rm_rne_cvt_f32rast_f5a_h = (cmd_f5a_h==`VPU_FCMD_CVT_F32RAST);

//================================================================================================================
// Set RMM rounding mode for f32 to norm convert instructions
//================================================================================================================
assign rm_rmm_f32_to_norm_cvt_f5a_h = sigs_f5a_h[`TXFMA_NORM_F32];

//================================================================================================================
// Round Control signals for use in F5 & F6/C6 
//================================================================================================================
assign rm_mod_f5a_h = rm_clr_f5a_h ?                  RTZ 
                    :  rm_rmm_f32_to_norm_cvt_f5a_h ? RMM
                    : rm_rne_cvt_f32rast_f5a_h      ? RNE
                    :                                 rm_f5a_h;

assign rm_rne_f5a_h   = (rm_mod_f5a_h == RNE);
assign rm_rup_f5a_h   = (rm_mod_f5a_h == RUP);
assign rm_rdn_f5a_h   = (rm_mod_f5a_h == RDN);
assign rm_rmm_f5a_h   = (rm_mod_f5a_h == RMM);
assign rm_rtz_f5a_h   = (rm_mod_f5a_h == RTZ);

assign trans1_int_roundup_l_f5a_h  = sigs_f5a_h[`TXFMA_TRANS1];
assign trans2_ps_roundup_r_f5a_h   = sigs_f5a_h[`TXFMA_TRANS2];
assign cvt_rastf32_roundup_l_f5a_h = (cmd_f5a_h == `VPU_FCMD_CVT_RASTF32);

assign rc_ps_clr_f5a_h = sigs_f5a_h[`TXFMA_FX_F32] | sigs_f5a_h[`TXFMA_CVT] | sigs_f5a_h[`TXFMA_NORM_F32] | (cmd_f5a_h==`VPU_TRANS_EXP_FRAC) | (op_dtype_f5a_h == `VPU_DTYPE_INT);
assign rc_ps_f5a_h     = ~rc_ps_clr_f5a_h | ((op_dtype_f5a_h == `VPU_DTYPE_INT) & sigs_f5a_h[`TXFMA_CVT]) | ((op_dtype_f5a_h != `VPU_DTYPE_F32) & sigs_f5a_h[`TXFMA_F32_NORM])
                       | sigs_f5a_h[`TXFMA_TRANS2] | (cmd_f5a_h==`VPU_FCMD_CVT_F32RAST);

assign rc6_ps_f5a_h   = rc_ps_f5a_h;

assign rc_ph_f5a_h    = (cmd_f5a_h == `VPU_FCMD_CVT_F16F32);
assign rc6_ph_f5a_h   = rc_ph_f5a_h;

assign rc_int32_f5a_h = (op_dtype_f5a_h == `VPU_DTYPE_F32) & (sigs_f5a_h[`TXFMA_CVT] | sigs_f5a_h[`TXFMA_NORM_F32] | (cmd_f5a_h==`VPU_TRANS_EXP_FRAC)) | sigs_f5a_h[`TXFMA_TRANS1] & (cmd_f5a_h!=`VPU_TRANS_LOG_FMA2);


assign rc_3rmxr_f5a_h    = rm_rup_f5a_h | rm_rdn_f5a_h | rm_rtz_f5a_h;
assign rc_rl01_f5a_h     = ((rm_rdn_f5a_h & s_res_out_f5a_h) | (rm_rup_f5a_h & ~s_res_out_f5a_h));
assign rc_rmm_f5a_h      = rm_rmm_f5a_h;
assign rc_rup_f5a_h      = rm_rup_f5a_h;
assign rc_rne_f5a_h      = rm_rne_f5a_h;
assign rc_rdn_f5a_h      = rm_rdn_f5a_h;
assign rc_res_sign_f5a_l = ~s_res_out_f5a_h;
assign rc_transrnd_f5a_h = trans1_int_roundup_l_f5a_h | cvt_rastf32_roundup_l_f5a_h;

assign rc_transrnd_log_fma2_f5a_h = (cmd_f5a_h==`VPU_TRANS_LOG_FMA2);

assign int32_stky2r_f5a_h = stky2g_f5a_h & rc_int32_f5a_h;



//================================================================================================================
// Zeroing out of Exponent 
//================================================================================================================
  //force exponent hi to 0 when mantissa is '0'
always_comb begin
  e_force_to_zero_f5a_h = 1'b0;

    //mantissa is zero
  if (!comp_op_f5a_h & !minmax_op_f5a_h & !fc_sel_f5a_h)
    e_force_to_zero_f5a_h = ($signed(elxd_f5a_h)==`TXFMA_LXD_HI_MAX_VALUE);

    //fround
  if (cmd_f5a_h==`VPU_FCMD_FROUND)
    e_force_to_zero_f5a_h = ((elxd_f5a_h>=`TXFMA_CVT_F32INT_OFFSET) | ec_zero_f5a_h) & ~ediff1_zap_f5a_h;
end


//================================================================================================================
// Merging MUX Selectors
//================================================================================================================
  //generate compare true selector with signalS comming from f3 + final mantissa equality check (TRZ==`TXFMA_PH_DPATH_SZ)
assign comp_flt_true_f5a_h = ((cmd_f5a_h==`VPU_FCMD_FLT) & comp_res_sel_f5a_h & !(fa_eq_fc_res_check_hi_f5a_h & (etrz50_f5a_h==$unsigned(`TXFMA_PS_DPATH_SZ))));
assign comp_fle_true_f5a_h = ((cmd_f5a_h==`VPU_FCMD_FLE) & (comp_res_sel_f5a_h | (fa_eq_fc_res_check_hi_f5a_h & (etrz50_f5a_h==$unsigned(`TXFMA_PS_DPATH_SZ)))));
assign comp_feq_true_f5a_h = ((cmd_f5a_h==`VPU_FCMD_FEQ) & (fa_eq_fc_res_check_hi_f5a_h & (etrz50_f5a_h==$unsigned(`TXFMA_PS_DPATH_SZ))));
assign comp_op_true_f5a_h = comp_flt_true_f5a_h | comp_fle_true_f5a_h | comp_feq_true_f5a_h; 

assign mmx_sel_immres_f5a_h = (op_dtype_f5a_h == `VPU_DTYPE_INT)  & (((cmd_f5a_h==`VPU_FCMD_MULL | cmd_f5a_h==`VPU_FCMD_MULLU | cmd_f5a_h==`VPU_FCMD_MULH | cmd_f5a_h==`VPU_FCMD_MULHU) | (cmd_f5a_h==`VPU_FCMD_NR1_FRCPFXP))) |
                                                                    (minmax_op_f5a_h | fc_sel_f5a_h | sigs_f5a_h[`TXFMA_FX_F32] | sigs_f5a_h[`TXFMA_F32_FX]);

assign nshcen_sel1b_f5a_h  = ~mmx_sel_immres_f5a_h & ((sigs_f5a_h[`TXFMA_CVT] & (op_dtype_f5a_h==`VPU_DTYPE_INT)) | 
                                                      ((op_dtype_f5a_h !=`VPU_DTYPE_INT) & ~(sigs_f5a_h[`TXFMA_CVT] | sigs_f5a_h[`TXFMA_NORM_F32] | (cmd_f5a_h==`VPU_TRANS_EXP_FRAC) |  (cmd_f5a_h==`VPU_TRANS_LOG_RR))) |  
                                                      (cmd_f5a_h==`VPU_FCMD_FROUND)); 

assign en_logrrsel1b_f5a_l = ~( ~mmx_sel_immres_f5a_h & ((cmd_f5a_h==`VPU_TRANS_LOG_RR) && !trans_log_rr_neg_f5a_h));


//================================================================================================================
// Sign Correction 
//================================================================================================================
  //sign correction depending on sign check signal coming from c3 and A2 result
always_comb
begin
  s_res_out_f5a_h = s_res_f5a_h;
 
  if (s_res_check_cadd_a3_f5a_h) begin
    if (op_dtype_f5a_h == `VPU_DTYPE_F16_F32)
      s_res_out_f5a_h = (comb_add_a3_f5a_h ^ s_res_f5a_h) & ~(($signed(elxd_f5a_h)==`TXFMA_LXD_HI_MAX_VALUE) & !stky2g_f5a_h);
    else
      s_res_out_f5a_h = (!comb_add_a3_f5a_h ^ prod_sign_hi_f5a_h) & ($signed(elxd_f5a_h)!=`TXFMA_LXD_HI_MAX_VALUE);

    if (cmd_f5a_h == `VPU_TRANS_LOG_MUL)
      s_res_out_f5a_h = comb_add_a3_f5a_h;   
  end

  if (comp_op_f5a_h)
    s_res_out_f5a_h = comp_op_true_f5a_h;
end
endmodule 
