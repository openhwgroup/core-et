// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_c4
(
  input  [`VPU_DTYPE_SZ-1:0]                 op_dtype_f4a_h, 
  input  [`VPU_FCMD_SZ-1:0]                  cmd_f4a_h, 
  input  [`TXFMA_SIGS_SZ-1:0]                sigs_f4a_h, 
  input  [`TXFMA_LXD_PS_SZ-1:0]              lxd_f4a_h,
  input                                      eff_sub1_f4a_h,
  input                                      eff_sub2_f4a_h,
  input  [`TXFMA_A0:`TXFMA_B0]               norm_shift_out_neg_a0b0_f4a_h,
  input                                      sc_f4a_h,
  input                                      eprod_hi_zero_f4a_h,
  input                                      ec_zero_f4a_h,
  input                                      comb_add_a3_hi_f4a_h,
  input                                      s_res_in_f4a_h,
  input                                      prod_sign_hi_f4a_h,
  input                                      s_res_check_cadd_a3_f4a_h,
  input                                      comp_res_sel_in_f4a_h,
  input                                      comp_op_f4a_h,
  input                                      minmax_op_f4a_h,
  input  [`TXFMA_WSPS_ETRZ_SZ-1:0]           comb_add_lo_ps_trz_f4a_h,
  input                                      fa_eq_fc_res_check_f4a_h,
  input                                      fc_sel_f4a_h,
  input                                      trans_log_rr_neg_f4a_h,
  input [`TXFMA_EXP_SEXT_PS_SZ-1:0]          exp_frac_e_shift_f4a_h,
  input [2:0]                                sin_res_shf_idx_f4a_h,    
  input                                      fround_f4a_h,
  input                                      ediff1_zap_f4a_h,
  input                                      sticky_until_l_f4a_h,
  output logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]   norm_shfamnt_f4a_h,
  output logic                               e_force_to_zero_f4a_h,
  output                                     use_lxd_f4a_h,
  output                                     e_lxd_add_in2_mux_sel_f4a_h,
  output                                     e_lxd_add_cin_f4a_h,
  output                                     nsh_correction_f4a_h,
  output logic [`TXFMA_MERGE_MUX_SEL_SZ-1:0] mergd_mux_sel_f4a_h,
  output logic                               neg_norm_f4a_h,
  output logic                               s_res_out_f4a_h,
  output logic                               rm_clr_f4a_h,
  output logic                               rm_rne_f4a_h,
  output logic                               rm_rmm_f4a_h,
  output logic                               comp_op_true_f4a_h
);

logic norm_eff_sub_f4a_h;

  //force exponent hi to 0 when mantissa is '0'
always_comb begin
  e_force_to_zero_f4a_h = 1'b0;

    //mantissa is zero
  if (!comp_op_f4a_h & !minmax_op_f4a_h & !fc_sel_f4a_h)
    e_force_to_zero_f4a_h = ($signed(lxd_f4a_h)==`TXFMA_LXD_HI_MAX_VALUE);

    //fround
  if ((cmd_f4a_h==`VPU_FCMD_FROUND) && !fround_f4a_h)
    e_force_to_zero_f4a_h = ((lxd_f4a_h>=`TXFMA_CVT_F32INT_OFFSET) | ec_zero_f4a_h) & ~ediff1_zap_f4a_h;
end

  //clear rounding mode for compare,minmax,fround,ffrc instructions
assign rm_clr_f4a_h = comp_op_f4a_h | minmax_op_f4a_h | (cmd_f4a_h==`VPU_FCMD_FROUND) | ((cmd_f4a_h == `VPU_FCMD_FFRC) | (cmd_f4a_h==`VPU_FCMD_FFRC_SIN));


  //norm eff sub hi mux
always_comb begin
  case (op_dtype_f4a_h)
    `VPU_DTYPE_F16_F32: begin
       norm_eff_sub_f4a_h = eff_sub1_f4a_h | eff_sub2_f4a_h;
    end
    default: begin
       norm_eff_sub_f4a_h = eff_sub1_f4a_h;
    end
  endcase
end

  //negate prod adder in effective sub: (ediff>=-2 AND sign bit) OR (ediff<-2 AND carry)
assign neg_norm_f4a_h = ((norm_eff_sub_f4a_h & comb_add_a3_hi_f4a_h)  |
                           (s_res_in_f4a_h & sigs_f4a_h[`TXFMA_NORM_F32]) |
                           ((cmd_f4a_h==`VPU_TRANS_LOG_MUL) & comb_add_a3_hi_f4a_h) |
                           ((cmd_f4a_h==`VPU_TRANS_SIN_P1) & (op_dtype_f4a_h==`VPU_DTYPE_INT)) |
                           (sc_f4a_h & (cmd_f4a_h==`VPU_FCMD_CVT_INTF32 | cmd_f4a_h==`VPU_FCMD_CVT_UINTF32 | 
                            cmd_f4a_h==`VPU_FCMD_CVT_RASTF32 | cmd_f4a_h==`VPU_TRANS_EXP_FRAC)));
           

 // calculate normalization hi shifter amount
always_comb begin
    //no shift     
  norm_shfamnt_f4a_h = `TXFMA_EXP_SEXT_PS_SZ'b0;
  
  if ((op_dtype_f4a_h != `VPU_DTYPE_F32) & sigs_f4a_h[`TXFMA_GCVT]) begin
    norm_shfamnt_f4a_h = `SX(`TXFMA_EXP_SEXT_PS_SZ, lxd_f4a_h);    
  end
  

  case (op_dtype_f4a_h)
    `VPU_DTYPE_INT: begin
      if (sigs_f4a_h[`TXFMA_CVT])
        norm_shfamnt_f4a_h = `SX(`TXFMA_EXP_SEXT_PS_SZ, lxd_f4a_h);
      if (cmd_f4a_h==`VPU_TRANS_RCP_FMA2) 
        norm_shfamnt_f4a_h = $unsigned(`TXFMA_A0-`TXFMA_B5);
      if (cmd_f4a_h==`VPU_TRANS_RSQRT_FMA2) 
        norm_shfamnt_f4a_h = $unsigned(`TXFMA_A0-`TXFMA_B4);
      if (cmd_f4a_h==`VPU_TRANS_LOG_FMA2)
        norm_shfamnt_f4a_h = `TXFMA_EXP_SEXT_PS_SZ'h3fe; //-2
      if (cmd_f4a_h==`VPU_TRANS_EXP_FMA2)
        norm_shfamnt_f4a_h = $unsigned(`TXFMA_A0-`TXFMA_B1);   

      if (cmd_f4a_h==`VPU_TRANS_SIN_P1 || cmd_f4a_h==`VPU_TRANS_SIN_P2) begin
        case (sin_res_shf_idx_f4a_h)
          3'd0: norm_shfamnt_f4a_h = `TXFMA_EXP_SEXT_PS_SZ'h3fe; //-2
          3'd1: norm_shfamnt_f4a_h =  0;
          3'd2: norm_shfamnt_f4a_h =  1;
          3'd3: norm_shfamnt_f4a_h =  3;
          3'd4: norm_shfamnt_f4a_h =  4;
          3'd5: norm_shfamnt_f4a_h =  5;          
        endcase
      end
    end
    `VPU_DTYPE_F32,`VPU_DTYPE_F16_F32: begin
      if (~(sigs_f4a_h[`TXFMA_CVT] | sigs_f4a_h[`TXFMA_NORM_F32] | (cmd_f4a_h==`VPU_TRANS_LOG_RR) | ~use_lxd_f4a_h) | (cmd_f4a_h == `VPU_FCMD_FROUND)) begin
        norm_shfamnt_f4a_h = `SX(`TXFMA_EXP_SEXT_PS_SZ, lxd_f4a_h);
      end 
    end
  endcase
end

  //use lxd c hi
assign use_lxd_f4a_h = ((op_dtype_f4a_h == `VPU_DTYPE_F32) & ~(sigs_f4a_h[`TXFMA_CVT] | sigs_f4a_h[`TXFMA_NORM_F32] | 
                                                                  ((cmd_f4a_h==`VPU_TRANS_EXP_FRAC) && ($signed(exp_frac_e_shift_f4a_h)>=0)))) |
                           (cmd_f4a_h==`VPU_FCMD_CVT_F32INT | cmd_f4a_h==`VPU_FCMD_CVT_F32UINT | cmd_f4a_h==`VPU_FCMD_CVT_F32RAST) |
                           (op_dtype_f4a_h == `VPU_DTYPE_F16_F32);

  //exp hi lxd in2 mux sel
assign e_lxd_add_in2_mux_sel_f4a_h = (cmd_f4a_h!=`VPU_FCMD_CVT_F32INT & cmd_f4a_h!=`VPU_FCMD_CVT_F32UINT & !sigs_f4a_h[`TXFMA_MUL] & cmd_f4a_h!=`VPU_FCMD_CVT_F32RAST) & 
                                        (cmd_f4a_h==`VPU_FCMD_CVT_INTF32 | cmd_f4a_h==`VPU_FCMD_CVT_UINTF32 | cmd_f4a_h==`VPU_FCMD_CVT_RASTF32);

  //nsh correction hi
assign nsh_correction_f4a_h = (norm_shift_out_neg_a0b0_f4a_h==2'b01);

  //lxd adder cin: exp-1 when there is nsh correction
assign e_lxd_add_cin_f4a_h  = ~(nsh_correction_f4a_h | sigs_f4a_h[`TXFMA_TRANS2] | (cmd_f4a_h==`VPU_TRANS_EXP_FRAC) | sigs_f4a_h[`TXFMA_F32_FX]);

                                                         
  //generate compare true selector with signalS comming from f3 + final mantissa equality check (TRZ==`TXFMA_PH_DPATH_SZ)
assign comp_op_true_f4a_h = ((cmd_f4a_h==`VPU_FCMD_FLT) & comp_res_sel_in_f4a_h & !(fa_eq_fc_res_check_f4a_h & (comb_add_lo_ps_trz_f4a_h==$unsigned(`TXFMA_PS_DPATH_SZ))))  | 
                            ((cmd_f4a_h==`VPU_FCMD_FLE) & (comp_res_sel_in_f4a_h | (fa_eq_fc_res_check_f4a_h & (comb_add_lo_ps_trz_f4a_h==$unsigned(`TXFMA_PS_DPATH_SZ))))) | 
                            ((cmd_f4a_h==`VPU_FCMD_FEQ) & (fa_eq_fc_res_check_f4a_h & (comb_add_lo_ps_trz_f4a_h==$unsigned(`TXFMA_PS_DPATH_SZ))));


 // mergd mux selector hi
always_comb
begin
  mergd_mux_sel_f4a_h = `TXFMA_MERGE_MUX_SEL_NORM;

    //int
  if (op_dtype_f4a_h == `VPU_DTYPE_INT) begin
    if ((cmd_f4a_h==`VPU_FCMD_MULL) | (cmd_f4a_h==`VPU_FCMD_NR1_FRCPFXP))
      mergd_mux_sel_f4a_h = `TXFMA_MERGE_MUX_SEL_INTL;
    else if (sigs_f4a_h[`TXFMA_CVT]) begin
      if (nsh_correction_f4a_h)
        mergd_mux_sel_f4a_h = `TXFMA_MERGE_MUX_SEL_NORM_NSHC;
    end else if (sigs_f4a_h[`TXFMA_TRANS1] | sigs_f4a_h[`TXFMA_TRANS2]) begin
      mergd_mux_sel_f4a_h = `TXFMA_MERGE_MUX_SEL_NORM_TRANS;
    end else if (cmd_f4a_h!=`VPU_FCMD_NR2_FRCPFXP)
      mergd_mux_sel_f4a_h = `TXFMA_MERGE_MUX_SEL_INTH;
  end 
    //float
  else begin
    if(cmd_f4a_h==`VPU_TRANS_LOG_RR && !trans_log_rr_neg_f4a_h)
      mergd_mux_sel_f4a_h = `TXFMA_MERGE_MUX_SEL_NORM_LOG_RR;
    else if (nsh_correction_f4a_h & (~(sigs_f4a_h[`TXFMA_CVT] | sigs_f4a_h[`TXFMA_NORM_F32] | (cmd_f4a_h==`VPU_TRANS_EXP_FRAC) | (cmd_f4a_h==`VPU_TRANS_LOG_RR)) | (cmd_f4a_h==`VPU_FCMD_FROUND) ))
      mergd_mux_sel_f4a_h = `TXFMA_MERGE_MUX_SEL_NORM_NSHC;
      //compare
    if (comp_op_f4a_h) begin
      if (comp_op_true_f4a_h)
        mergd_mux_sel_f4a_h = `TXFMA_MERGE_MUX_SEL_COMP_TRUE;
      else
        mergd_mux_sel_f4a_h = `TXFMA_MERGE_MUX_SEL_COMP_FALSE;
    end

      //minmax op or force c operand OR float to float
    if (minmax_op_f4a_h | fc_sel_f4a_h | sigs_f4a_h[`TXFMA_FX_F32] | sigs_f4a_h[`TXFMA_F32_FX])
      mergd_mux_sel_f4a_h = `TXFMA_MERGE_MUX_SEL_INTH;      
  end
end

  //sign correction depending on sign check signal coming from c3 and A2 result
    //hi
always_comb
begin
  s_res_out_f4a_h = s_res_in_f4a_h;
 
  if (s_res_check_cadd_a3_f4a_h) begin
    if (op_dtype_f4a_h == `VPU_DTYPE_F16_F32)
      s_res_out_f4a_h = (comb_add_a3_hi_f4a_h ^ s_res_in_f4a_h) & ~(($signed(lxd_f4a_h)==`TXFMA_LXD_HI_MAX_VALUE) & !sticky_until_l_f4a_h);
    else
      s_res_out_f4a_h = (!comb_add_a3_hi_f4a_h ^ prod_sign_hi_f4a_h) & ($signed(lxd_f4a_h)!=`TXFMA_LXD_HI_MAX_VALUE);

    if (cmd_f4a_h == `VPU_TRANS_LOG_MUL)
      s_res_out_f4a_h = comb_add_a3_hi_f4a_h;   
  end

  if (comp_op_f4a_h)
    s_res_out_f4a_h = comp_op_true_f4a_h;
end

assign rm_rne_f4a_h = (cmd_f4a_h == `VPU_FCMD_CVT_F32RAST);
assign rm_rmm_f4a_h = sigs_f4a_h[`TXFMA_NORM_F32];

endmodule 
