// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_c3
(
  input  [`VPU_DTYPE_SZ-1:0]                  op_dtype_f3a_h, 
  input  [`VPU_FCMD_SZ-1:0]                   cmd_f3a_h,
  input  [`TXFMA_SIGS_SZ-1:0]                 sigs_f3a_h,
  input  [`TXFMA_EXP_SEXT_PS_SZ-1:0]          ediff1_f3a_h,
  input  [`TXFMA_EXP_SEXT_PS_SZ-1:0]          ediff2_f3a_h,
  input  [`TXFMA_RM_SZ-1:0]                   rm_f3a_h,
  input                                       eff_sub1_f3a_h,
  input                                       eff_sub2_f3a_h,
  input                                       eprod_hi_zero_f3a_h,
  input                                       eprod_lo_zero_f3a_h,
  input                                       prod_inf_hi_f3a_h,
  input                                       prod_inf_lo_f3a_h,
  input                                       eff_sign1_f3a_h,
  input                                       prod_sign_hi_f3a_h,
  input                                       prod_sign_lo_f3a_h,
  input                                       sc_f3a_h,
  input                                       c_inf_f3a_h,
  input                                       ec_zero_f3a_h,
  input                                       a_is_qnan_hi_f3a_h,
  input                                       c_is_qnan_f3a_h,
  input                                       a_is_snan_hi_f3a_h,
  input                                       c_is_snan_f3a_h,
  input                                       prodsum_a2_f3a_h,
  input                                       e_sticky1_f3a_h,
  input                                       e_sticky2_f3a_h,
  input                                       minmax_op_f3a_h,
  input                                       comp_op_f3a_h,
  input                                       res_inf_f3a_h,
  input                                       res_qnan_f3a_h,
  input                                       res_snan_f3a_h,
  input                                       aligned_48_int_r_f3a_h,
  input                                       aligned_48_int_l_f3a_h,
  input   [`TXFMA_EDIFF_OPD_SEL_SZ-1:0]       opd3_mux_sel_f3a_h,
  input                                       spec_res_en_f3a_h,
  input                                       fc_b0_f3a_h,
  input                                       res_zero_f3a_h,
  input  [`TXFMA_EXP_PS_SZ-1:0]               ec_f3a_h,
  input  [`FP_CLASS_BITS-1:0]                 classify_f3a_h,
  input                                       flags_en_f3a_h,
  input                                       fb_hi_zero_f3a_h,
  input [`TXFMA_EXP_SEXT_PS_SZ-1:0]           exp_frac_e_shift_f3a_h, 
  output logic                                fround_l_f3a_h,
  output logic                                res_snan_out_f3a_h,
  output logic                                res_qnan_out_f3a_h,
  output logic                                res_inf_out_f3a_h,
  output logic                                align1_neg_cin_f3a_h,
  output logic                                align2_neg_cin_f3a_h,
  output logic                                align1_zap_f3a_h,
  output logic                                align2_zap_f3a_h,
  output logic                                align1_neg_f3a_h,
  output logic                                align2_neg_f3a_h,
  output logic                                op2_sxtto_g_f3a_h,  
  output logic [`TXFMA_MINMAX_OPD_SEL_SZ-1:0] int_minmax_mux_sel_f3a_h,
  output logic                                s_res_f3a_h,
  output logic                                s_res_check_cadd_a3_f3a_h,
  output logic                                comp_res_sel_f3a_h,
  output logic                                fa_eq_fc_res_check_f3a_h,
  output logic                                lxd_sub_f3a_h,
  output logic                                trans_log_rr_neg_f3a_h,
  output logic                                res_zero_out_f3a_h,
  output logic                                fc_sel_f3a_h,
  output logic [`TXFMA_EXP_SEXT_PS_SZ-1:0]    ediff1_abs_f3a_h,
  output logic [`TXFMA_EXP_SEXT_PH_SZ-1:0]    ediff2_abs_f3a_h,
  output logic                                eff_sub1_out_f3a_h,
  output logic                                cmp_invalid_f3a_h,
  output logic                                fround_sticky_until_l_f3a_h
);


logic                             use_align1_f3a_h;
logic                             use_align2_f3a_h;
logic                             fa_ge_fc_comp_sign_res_f3a_h;
logic                             fa_eq_fc_comp_sign_res_f3a_h;
logic                             fa_ge_fc_comp_exp_hi_f3a_h;
logic                             fa_ge_fc_comp_frac_hi_f3a_h;
logic                             fa_ge_fc_trans_exp_frac_hi_f3a_h;
logic                             fa_ge_fc_f32_frac_hi_f3a_h;
logic                             fa_ge_fc_res_sel_hi_f3a_h;  
logic                             minmax_res_sel_f3a_h;
logic                             out_of_range_1_f3a_h;
logic                             out_of_range_2_f3a_h;
logic                             ediff_hi_is_1_f3a_h;
logic                             ediff1_is_1_f3a_h;
logic                             ediff2_is_1_f3a_h;
logic                             ediff1_lte_0_f3a_h;
logic                             ediff1_gte_m1_f3a_h;
logic                             ediff1_gte_m2_f3a_h;
logic                             eprod_ge_ec_f3a_h;
logic                             eprod_ge_ec_1_f3a_h;
logic                             eprod_ge_ec_2_f3a_h;
logic                             eff_sub_f3a_h;
logic                             eprod_lt_ec_f3a_h;
logic                             eprod_lt_ec_1_f3a_h;
logic                             eprod_lt_ec_2_f3a_h;
logic                             eprod_lte_ec_2_f3a_h;
logic                             ediff1_is_0_f3a_h;
logic                             ediff2_is_0_f3a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0] ediff1_abs_int_f3a_h;
logic [`TXFMA_EXP_SEXT_PS_SZ-1:0] ediff2_abs_int_f3a_h;
logic                             exp_frac_zero_f3a_h;
logic                             exp_frac_inf_f3a_h;
logic                             fc_lt_0_f3a_h;
logic                             fc_gt_0_f3a_h;
logic                             fc_lte_m05_f3a_h;
logic                             fc_lte_m075_f3a_h;
logic                             fc_lte_m025_f3a_h;
logic                             fc_gte_025_f3a_h;
logic                             fc_gte_05_f3a_h;
logic                             fc_gte_075_f3a_h;
logic                             sin_align1_neg_f3a_h;
logic                             sin_align2_neg_f3a_h;
logic                             int_minmax_mux_sel_fc_f3a_h;
logic [`TXFMA_EXP_SEXT_PH_SZ-1:0] sin_ediff2_f3a_h;
logic                             sin_eff_sub1_f3a_h;
logic                             sin_check_fc_sign_f3a_h;
logic                             ediff_lte_m31;
logic                             ediff_gt_m53;
logic                             res_inf_mux_f3a_h;
logic                             res_qnan_mux_f3a_h;
logic                             res_snan_mux_f3a_h;
logic                             trans_log_mul_align1_neg_f3a_h;
logic                             ec_gte_127_f3a_h;
logic                             res_qnan_minmax_hi_f3a_h;
logic                             res_snan_minmax_hi_f3a_h;


///////////////////////////////////////////////////////////////////////////////
// ediff control logic
///////////////////////////////////////////////////////////////////////////////
  //exponent product and ec comparations
assign eprod_ge_ec_1_f3a_h  = ($signed(ediff1_f3a_h)<0);
assign eprod_ge_ec_2_f3a_h  = ($signed(ediff2_f3a_h)<0);
assign eprod_lt_ec_1_f3a_h  = ($signed(ediff1_f3a_h)>0);
assign eprod_lt_ec_2_f3a_h  = ($signed(ediff2_f3a_h)>0);
assign eprod_lte_ec_2_f3a_h = ($signed(ediff2_f3a_h)>=0);
assign ediff1_is_0_f3a_h    = ($signed(ediff1_f3a_h)==0);
assign ediff2_is_0_f3a_h    = ($signed(ediff2_f3a_h)==0);
assign ediff1_is_1_f3a_h    = ($signed(ediff1_abs_int_f3a_h)==1);
assign ediff2_is_1_f3a_h    = ($signed(ediff2_abs_int_f3a_h)==1);
assign ediff1_lte_0_f3a_h   = ($signed(ediff1_f3a_h)<=0);
assign ediff1_gte_m1_f3a_h  = ($signed(ediff1_f3a_h)>=-1);
assign ediff1_gte_m2_f3a_h  = ($signed(ediff1_f3a_h)>=-2);
assign ediff_lte_m31        = $signed(ediff1_f3a_h) <= $signed(-31);
assign ediff_gt_m53         = $signed(ediff1_f3a_h) > $signed(-53);
assign ec_gte_127_f3a_h     = (ec_f3a_h >= 127);
  
  //ediff absolute values
assign ediff1_abs_int_f3a_h = ediff1_f3a_h[`TXFMA_EXP_SEXT_PS_SZ-1] ? -ediff1_f3a_h : ediff1_f3a_h;
assign ediff2_abs_int_f3a_h = ediff2_f3a_h[`TXFMA_EXP_SEXT_PS_SZ-1] ? -ediff2_f3a_h : ediff2_f3a_h;

  //trans log rr negate
assign trans_log_rr_neg_f3a_h = fc_b0_f3a_h & (cmd_f3a_h==`VPU_TRANS_LOG_RR);

  //trans log mul, neg if ec>127 ^ fc[b0]!=0
assign trans_log_mul_align1_neg_f3a_h = (cmd_f3a_h==`VPU_TRANS_LOG_MUL) & !(ec_gte_127_f3a_h ^ fc_b0_f3a_h);


///////////////////////////////////////////////////////////////////////////////
// aligner control logic
///////////////////////////////////////////////////////////////////////////////

// Use aligner control
assign use_align1_f3a_h = sigs_f3a_h[`TXFMA_FMA] | sigs_f3a_h[`TXFMA_FMS] | sigs_f3a_h[`TXFMA_ADD] | 
                          sigs_f3a_h[`TXFMA_SUB] | sigs_f3a_h[`TXFMA_CVT] | sigs_f3a_h[`TXFMA_NORM_F32] | (cmd_f3a_h==`VPU_TRANS_LOG_RR) |
                          sigs_f3a_h[`TXFMA_TRANS1] | sigs_f3a_h[`TXFMA_TRANS2] | (cmd_f3a_h==`VPU_TRANS_SIN_TRANSFORM) | (cmd_f3a_h==`VPU_TRANS_EXP_FRAC);

assign use_align2_f3a_h = op_dtype_f3a_h == `VPU_DTYPE_F16_F32 | (cmd_f3a_h==`VPU_TRANS_SIN_TRANSFORM);                        

//  Negate in case of effective SUB and integer SUB (if prod is not zero), negative int to float convert or min/max operation
assign align1_neg_f3a_h = ((eff_sub1_f3a_h | (sc_f3a_h & (cmd_f3a_h==`VPU_FCMD_CVT_F32INT | cmd_f3a_h==`VPU_FCMD_CVT_F32RAST))) & use_align1_f3a_h
                        |  trans_log_rr_neg_f3a_h | sin_align1_neg_f3a_h | trans_log_mul_align1_neg_f3a_h) & ~(cmd_f3a_h==`VPU_TRANS_EXP_FRAC);
assign align2_neg_f3a_h = eff_sub2_f3a_h & use_align2_f3a_h | sin_align2_neg_f3a_h;

  // out of range signals used to zap operands
always_comb begin
  if (op_dtype_f3a_h==`VPU_DTYPE_F32) begin
    out_of_range_1_f3a_h = (ediff1_abs_int_f3a_h>`TXFMA_F32_EDIFF_OUT_OF_RANGE);
  end
  else if (op_dtype_f3a_h==`VPU_DTYPE_F16_F32) begin
    //ph goes to align 1
    if (opd3_mux_sel_f3a_h==`TXFMA_SEL_C)
      out_of_range_1_f3a_h = (ediff1_abs_int_f3a_h>`TXFMA_PH_PS_P_EDIFF_OUT_OF_RANGE);
      //c goes to align 1
    else
      out_of_range_1_f3a_h = (ediff1_abs_int_f3a_h>`TXFMA_PH_PS_C_EDIFF_OUT_OF_RANGE);
  end
    //other cases
  else begin
    out_of_range_1_f3a_h = (ediff1_abs_int_f3a_h>`TXFMA_PH_EDIFF_OUT_OF_RANGE);        
  end
end


assign out_of_range_2_f3a_h = (ediff2_abs_int_f3a_h>`TXFMA_PH_EDIFF_OUT_OF_RANGE);

//  Zap if we don't use aligner or out of range
assign  align1_zap_f3a_h = out_of_range_1_f3a_h | !use_align1_f3a_h;
assign  align2_zap_f3a_h = out_of_range_2_f3a_h | !use_align2_f3a_h;  

  //aligner Cin
assign align1_neg_cin_f3a_h = align1_neg_f3a_h & ~e_sticky1_f3a_h | trans_log_mul_align1_neg_f3a_h;
assign align2_neg_cin_f3a_h = (align2_neg_f3a_h & ~e_sticky2_f3a_h) & (op_dtype_f3a_h == `VPU_DTYPE_F16_F32);
 
  //op2 sign extend to g bit
assign op2_sxtto_g_f3a_h = align2_neg_f3a_h & (cmd_f3a_h==`VPU_TRANS_SIN_TRANSFORM);


//lxd control
assign lxd_sub_f3a_h = (op_dtype_f3a_h==`VPU_DTYPE_F16_F32) ? s_res_check_cadd_a3_f3a_h  
                        :                                        (eff_sub1_f3a_h | trans_log_rr_neg_f3a_h);

assign ediff1_abs_f3a_h = (cmd_f3a_h==`VPU_TRANS_EXP_FRAC) ? exp_frac_e_shift_f3a_h : ediff1_abs_int_f3a_h;

assign ediff2_abs_f3a_h = (cmd_f3a_h==`VPU_TRANS_SIN_TRANSFORM) ? sin_ediff2_f3a_h : ediff2_abs_int_f3a_h[`TXFMA_EXP_SEXT_PH_SZ-1:0];

assign eff_sub1_out_f3a_h = (cmd_f3a_h==`VPU_TRANS_SIN_TRANSFORM) ? sin_eff_sub1_f3a_h : eff_sub1_f3a_h;


///////////////////////////////////////////////////////////////////////////////
// min/max and compare logic
///////////////////////////////////////////////////////////////////////////////

  //fa greater than fc compare by sign
assign fa_ge_fc_comp_sign_res_f3a_h = (eff_sign1_f3a_h & !prod_sign_hi_f3a_h) & (cmd_f3a_h!=`VPU_TRANS_EXP_FRAC);

  //fa equal to fc compared by sign
assign fa_eq_fc_comp_sign_res_f3a_h = !(prod_sign_hi_f3a_h ^ sc_f3a_h);

    //fa greater than fc compared by exponent
assign fa_ge_fc_comp_exp_hi_f3a_h  = (!prod_sign_hi_f3a_h & eprod_ge_ec_f3a_h&!eprod_hi_zero_f3a_h) | (prod_sign_hi_f3a_h & !(eprod_ge_ec_f3a_h&!eprod_hi_zero_f3a_h) & !ediff1_is_0_f3a_h);
  
  //fa greater than fc compare by fraction
assign fa_ge_fc_f32_frac_hi_f3a_h = (!prod_sign_hi_f3a_h & !prodsum_a2_f3a_h) | (prod_sign_hi_f3a_h & prodsum_a2_f3a_h) | 
                                    (eprod_hi_zero_f3a_h & !ec_zero_f3a_h);
assign fa_ge_fc_trans_exp_frac_hi_f3a_h = !prodsum_a2_f3a_h;
assign fa_ge_fc_comp_frac_hi_f3a_h = (cmd_f3a_h==`VPU_TRANS_EXP_FRAC) ? fa_ge_fc_trans_exp_frac_hi_f3a_h
                                   :                                    fa_ge_fc_f32_frac_hi_f3a_h;


  //FA is greather than FC if any of the compares bellow is set
assign fa_ge_fc_res_sel_hi_f3a_h  =  (fa_ge_fc_comp_sign_res_f3a_h                                |                  //sign
                                     (fa_eq_fc_comp_sign_res_f3a_h & fa_ge_fc_comp_exp_hi_f3a_h)  |                  //exp
                                     (fa_eq_fc_comp_sign_res_f3a_h&ediff1_is_0_f3a_h&fa_ge_fc_comp_frac_hi_f3a_h));  //frac

  //FA is equal to FC if any of the compares bellow is set. 
  //This signal is passed to F4 for final mantissa equal check (TRZ)                                 
assign fa_eq_fc_res_check_f3a_h  = ((eprod_hi_zero_f3a_h & ec_zero_f3a_h) | (!eff_sign1_f3a_h & ediff1_is_0_f3a_h)) & 
                                       !(a_is_qnan_hi_f3a_h | c_is_qnan_f3a_h | res_snan_mux_f3a_h);


  //minmax op selector:
    //'1': fa>=fc (max), fa<=fc (min)
    //'0': fa<fc  (max), fa>fc (min)
always_comb begin
  minmax_res_sel_f3a_h = 1'b0;

    //fmax
  if  (cmd_f3a_h==`VPU_FCMD_MAX) begin
    minmax_res_sel_f3a_h  = fa_ge_fc_res_sel_hi_f3a_h;                  // opa greater than opc
    minmax_res_sel_f3a_h &= !(a_is_qnan_hi_f3a_h | a_is_snan_hi_f3a_h); // opc greater if opa is NaN
    minmax_res_sel_f3a_h |= (c_is_qnan_f3a_h | c_is_snan_f3a_h);  // opa greater if opc is NaN
  end

    //fmin
  if  (cmd_f3a_h==`VPU_FCMD_MIN) begin
    minmax_res_sel_f3a_h  = !fa_ge_fc_res_sel_hi_f3a_h;                 // opa smaller than opc
    minmax_res_sel_f3a_h &= !(a_is_qnan_hi_f3a_h | a_is_snan_hi_f3a_h); // opa smaller if opc is NaN    
    minmax_res_sel_f3a_h |= (c_is_qnan_f3a_h | c_is_snan_f3a_h);  // opa smaller if opa is NaN
  end

end


  //exp frac
assign exp_frac_zero_f3a_h = fa_ge_fc_res_sel_hi_f3a_h & (cmd_f3a_h==`VPU_TRANS_EXP_FRAC) & !flags_en_f3a_h & ~(res_qnan_f3a_h | res_snan_f3a_h);
assign exp_frac_inf_f3a_h  = !sc_f3a_h & eprod_lte_ec_2_f3a_h & (cmd_f3a_h==`VPU_TRANS_EXP_FRAC);



  //comp op selector:
    //'1': fa==fc (feq), fa<=fc (fle), fa<fc (flt), fc!=0
    // note: final mantissa equal check is done at f4
assign comp_res_sel_f3a_h = ((!fa_ge_fc_res_sel_hi_f3a_h &(cmd_f3a_h==`VPU_FCMD_FLT))    |
                            (!fa_ge_fc_res_sel_hi_f3a_h & (cmd_f3a_h==`VPU_FCMD_FLE)))  &
                              //return always false when there is a NaN
                            !(a_is_qnan_hi_f3a_h | c_is_qnan_f3a_h | res_snan_mux_f3a_h);
                           

///////////////////////////////////////////////////////////////////////////////
// min max mux selector
///////////////////////////////////////////////////////////////////////////////
always_comb
begin
  int_minmax_mux_sel_f3a_h = `TXFMA_INT_MINMAX_SEL_PH;

    //int add
  if ((op_dtype_f3a_h == `VPU_DTYPE_INT) & (cmd_f3a_h!=`VPU_FCMD_CVT_F32INT) & (cmd_f3a_h!=`VPU_FCMD_CVT_F32RAST)) begin
      //int mul low
    if ((cmd_f3a_h==`VPU_FCMD_MULL) | (cmd_f3a_h==`VPU_FCMD_NR1_FRCPFXP))
      int_minmax_mux_sel_f3a_h = `TXFMA_INT_MINMAX_SEL_PL;
  end

    //min max
  if (minmax_op_f3a_h) begin
    if (minmax_res_sel_f3a_h) begin
      int_minmax_mux_sel_f3a_h = `TXFMA_INT_MINMAX_SEL_A;  
    end else begin
      int_minmax_mux_sel_f3a_h = `TXFMA_INT_MINMAX_SEL_C;       
    end
  end

    //trans log rr
  if (cmd_f3a_h==`VPU_TRANS_LOG_RR)
    int_minmax_mux_sel_f3a_h = `TXFMA_INT_MINMAX_SEL_S;

    //trans sin: select fa in case of beeing at I Quartile
  if (cmd_f3a_h==`VPU_TRANS_SIN_TRANSFORM && int_minmax_mux_sel_fc_f3a_h)
    int_minmax_mux_sel_f3a_h = `TXFMA_INT_MINMAX_SEL_C;

    //ffrc
  if (fc_sel_f3a_h)
    int_minmax_mux_sel_f3a_h = `TXFMA_INT_MINMAX_SEL_C;
  
    //float32 to floatX CVT
  if (sigs_f3a_h[`TXFMA_FX_F32])
    int_minmax_mux_sel_f3a_h = `TXFMA_INT_MINMAX_SEL_A;

    //floatX to float32 CVT   
  if (sigs_f3a_h[`TXFMA_F32_FX])
    int_minmax_mux_sel_f3a_h = `TXFMA_INT_MINMAX_SEL_C;
end


///////////////////////////////////////////////////////////////////////////////
// clear special cases for minmax and compare operations
///////////////////////////////////////////////////////////////////////////////
assign res_qnan_minmax_hi_f3a_h = minmax_op_f3a_h & (a_is_qnan_hi_f3a_h | a_is_snan_hi_f3a_h) & (c_is_qnan_f3a_h | c_is_snan_f3a_h);
assign res_snan_minmax_hi_f3a_h = minmax_op_f3a_h & (a_is_snan_hi_f3a_h & c_is_snan_f3a_h);
assign res_snan_out_f3a_h = (!comp_op_f3a_h & !minmax_op_f3a_h & res_snan_mux_f3a_h);
assign res_qnan_out_f3a_h = (!comp_op_f3a_h & !minmax_op_f3a_h & res_qnan_mux_f3a_h) | (res_qnan_minmax_hi_f3a_h | res_snan_minmax_hi_f3a_h);
assign res_inf_out_f3a_h  = (!comp_op_f3a_h & !minmax_op_f3a_h & res_inf_mux_f3a_h)  | exp_frac_inf_f3a_h;

assign res_zero_out_f3a_h = res_zero_f3a_h | exp_frac_zero_f3a_h;

// result is invalid if any input is NaN in case of compare
assign cmp_invalid_f3a_h = (((cmd_f3a_h==`VPU_FCMD_FLE) | (cmd_f3a_h==`VPU_FCMD_FLT)) & (a_is_qnan_hi_f3a_h | a_is_snan_hi_f3a_h | c_is_qnan_f3a_h | c_is_snan_f3a_h)) |
                            ((cmd_f3a_h==`VPU_FCMD_FEQ) & (a_is_snan_hi_f3a_h | c_is_snan_f3a_h));


always_comb begin
  res_inf_mux_f3a_h  = 1'b0;
  res_qnan_mux_f3a_h = 1'b0;
  res_snan_mux_f3a_h = 1'b0;

  if (spec_res_en_f3a_h) begin
    res_inf_mux_f3a_h  = res_inf_f3a_h;
    res_qnan_mux_f3a_h = res_qnan_f3a_h;
    res_snan_mux_f3a_h = res_snan_f3a_h;
  end
end

///////////////////////////////////////////////////////////////////////////////
//  Sign generation: 
//  In case of INT this is done implicitly, i.e. MSB is actually the sign 
//  Therefore this signal applies only to FP
///////////////////////////////////////////////////////////////////////////////

  //hi mux signals to generate op sign
always_comb
begin
  eprod_ge_ec_f3a_h = eprod_ge_ec_1_f3a_h;
  eff_sub_f3a_h     = eff_sub1_f3a_h;
  eprod_lt_ec_f3a_h = eprod_lt_ec_1_f3a_h;
  ediff_hi_is_1_f3a_h  = ediff1_is_1_f3a_h;
end


  //sign hi
always_comb
begin
  s_res_f3a_h               = 1'b0;
  s_res_check_cadd_a3_f3a_h = 1'b0; 
 
    //mul
  if (sigs_f3a_h[`TXFMA_MUL] & !(op_dtype_f3a_h==`VPU_DTYPE_F16_F32)) begin
    s_res_f3a_h = prod_sign_hi_f3a_h;

    if (cmd_f3a_h==`VPU_TRANS_LOG_MUL) begin
      s_res_f3a_h = !ec_gte_127_f3a_h;
      s_res_f3a_h |= res_inf_f3a_h & ec_zero_f3a_h;
    end
  end 
    //min
  else if (cmd_f3a_h==`VPU_FCMD_MIN) begin
    s_res_f3a_h = minmax_res_sel_f3a_h&prod_sign_hi_f3a_h | !minmax_res_sel_f3a_h&sc_f3a_h;
  end 
    //max
  else if (cmd_f3a_h==`VPU_FCMD_MAX) begin
    s_res_f3a_h = minmax_res_sel_f3a_h&prod_sign_hi_f3a_h | !minmax_res_sel_f3a_h&sc_f3a_h;
  end 
    //cvt
  else if (sigs_f3a_h[`TXFMA_CVT] || ((cmd_f3a_h == `VPU_FCMD_FFRC) | (cmd_f3a_h==`VPU_FCMD_FFRC_SIN))) begin
    s_res_f3a_h = sc_f3a_h & (cmd_f3a_h!=`VPU_FCMD_CVT_F32UINT);
    if (cmd_f3a_h==`VPU_FCMD_CVT_INTF32 || cmd_f3a_h==`VPU_FCMD_CVT_RASTF32)
      s_res_f3a_h = sc_f3a_h;
  end
    // gcvt
  else if (sigs_f3a_h[`TXFMA_SGCVT]) begin
    if (sigs_f3a_h[`TXFMA_F32_NORM] || op_dtype_f3a_h==`VPU_DTYPE_F16)
      s_res_f3a_h = sc_f3a_h;
    else
      s_res_f3a_h = prod_sign_hi_f3a_h;
  end
    //trans fma2
  else if (sigs_f3a_h[`TXFMA_TRANS2]) begin
    s_res_f3a_h = prod_sign_hi_f3a_h;

    if (cmd_f3a_h==`VPU_TRANS_EXP_FMA2)
      s_res_f3a_h = 1'b0;
  end
    //exp  
  else if (cmd_f3a_h==`VPU_TRANS_EXP_FRAC) begin
    s_res_f3a_h = sc_f3a_h & !exp_frac_zero_f3a_h;
  end
    //sin p2
  else if (cmd_f3a_h==`VPU_TRANS_SIN_P2) begin
      s_res_f3a_h = 1'b0; 
  end
    //sin transfrom
  else if (cmd_f3a_h==`VPU_TRANS_SIN_TRANSFORM) begin
    s_res_f3a_h = sin_align2_neg_f3a_h;

    if (sin_check_fc_sign_f3a_h)
      s_res_f3a_h = sin_align1_neg_f3a_h;

    if (fc_sel_f3a_h)
      s_res_f3a_h = sc_f3a_h;
  end
    //FMA/MUL PH PS
  else if (op_dtype_f3a_h==`VPU_DTYPE_F16_F32) begin
    case (opd3_mux_sel_f3a_h)
      `TXFMA_SEL_PH: s_res_f3a_h = prod_sign_hi_f3a_h;
      `TXFMA_SEL_PL: s_res_f3a_h = prod_sign_lo_f3a_h;
      `TXFMA_SEL_C:  s_res_f3a_h = sc_f3a_h;
    endcase

      //inf
    if (res_inf_mux_f3a_h & prod_inf_hi_f3a_h)
      s_res_f3a_h = prod_sign_hi_f3a_h;
    else if (res_inf_mux_f3a_h & prod_inf_lo_f3a_h)
      s_res_f3a_h = prod_sign_lo_f3a_h;
    else if (res_inf_mux_f3a_h & c_inf_f3a_h)
      s_res_f3a_h = sc_f3a_h;
      //check comb adder A3 bit in eff sub     
    else begin
      s_res_check_cadd_a3_f3a_h = eff_sub1_f3a_h | eff_sub2_f3a_h;  

        //eprodh and eprodl and ec zero
      if ((eprod_hi_zero_f3a_h && eprod_lo_zero_f3a_h && !(!ec_zero_f3a_h && sigs_f3a_h[`TXFMA_FMA])) && (eff_sub1_f3a_h | eff_sub2_f3a_h)) begin
        s_res_f3a_h = (rm_f3a_h==RDN);
      end
    end
  end 
    //PS
  else begin
      //effective sub
    if (eff_sub_f3a_h) begin
        //eprod greater than ec
      if (eprod_ge_ec_f3a_h&!eprod_hi_zero_f3a_h) begin
        s_res_f3a_h = prod_sign_hi_f3a_h;
        if (res_inf_mux_f3a_h & c_inf_f3a_h)
          s_res_f3a_h = sc_f3a_h ^ (sigs_f3a_h[`TXFMA_SUB] | sigs_f3a_h[`TXFMA_FMS]);        
      end
        //eprod and ec are zero
      else if (eprod_hi_zero_f3a_h & ec_zero_f3a_h) begin  
        s_res_f3a_h = (rm_f3a_h==RDN);
      end
        //ec is zero  
      else if (ec_zero_f3a_h) 
          s_res_f3a_h = prod_sign_hi_f3a_h;
        //eprod smaller than ec or eprod is zero 
      else if ((eprod_lt_ec_f3a_h & !ediff_hi_is_1_f3a_h) | eprod_hi_zero_f3a_h) begin
        if (res_inf_mux_f3a_h & prod_inf_hi_f3a_h)
          s_res_f3a_h = prod_sign_hi_f3a_h;
        else 
          s_res_f3a_h = sc_f3a_h ^ (sigs_f3a_h[`TXFMA_SUB] | sigs_f3a_h[`TXFMA_FMS]);         
      end 
        //result is infinity and ediff==1
      else if (res_inf_mux_f3a_h) begin
        s_res_f3a_h = sc_f3a_h ^ (sigs_f3a_h[`TXFMA_SUB] | sigs_f3a_h[`TXFMA_FMS]);       
        if (prod_inf_hi_f3a_h)
          s_res_f3a_h = prod_sign_hi_f3a_h;
      end
        //check comb adder A2 bit when ediff==0      
      else begin
        s_res_check_cadd_a3_f3a_h = 1'b1; 
      end
    end 
      //effective add
    else begin
      s_res_f3a_h = prod_sign_hi_f3a_h;
    end
  end
end




  //fc mask selection
assign fc_sel_f3a_h = ((cmd_f3a_h == `VPU_FCMD_FFRC) | (cmd_f3a_h==`VPU_FCMD_FFRC_SIN)) & eprod_ge_ec_1_f3a_h | int_minmax_mux_sel_fc_f3a_h;


///////////////////////////////////////////////////////////////////////////////
// sin transform compare logic
///////////////////////////////////////////////////////////////////////////////

  //fc<0
assign fc_lt_0_f3a_h = sc_f3a_h & !ec_zero_f3a_h;
  //fc<=-0.5
assign fc_lte_m05_f3a_h = sc_f3a_h & ediff1_gte_m1_f3a_h;
  //fc<=-0.75
assign fc_lte_m075_f3a_h = sc_f3a_h & ediff1_gte_m1_f3a_h & fc_b0_f3a_h;
  //fc<=-0.25
assign fc_lte_m025_f3a_h = sc_f3a_h & ediff1_gte_m2_f3a_h;
  //fc>=0.5
assign fc_gte_05_f3a_h = !sc_f3a_h & ediff1_gte_m1_f3a_h;
  //fc>=0.75
assign fc_gte_075_f3a_h = !sc_f3a_h & ediff1_gte_m1_f3a_h & fc_b0_f3a_h;
  //fc>=0.25
assign fc_gte_025_f3a_h = !sc_f3a_h & ediff1_gte_m2_f3a_h;
  //fc>0
assign fc_gt_0_f3a_h = !sc_f3a_h & ediff1_lte_0_f3a_h & !ec_zero_f3a_h;

  //sin eff sub
assign sin_eff_sub1_f3a_h = sin_align1_neg_f3a_h ^ sin_align2_neg_f3a_h;

always_comb begin
    //I Quartile 
  sin_align1_neg_f3a_h   = 1'b0;
  sin_align2_neg_f3a_h   = 1'b0;
  sin_ediff2_f3a_h       = '0;
  int_minmax_mux_sel_fc_f3a_h = 1'b0;
  sin_check_fc_sign_f3a_h = 1'b0;

    //trans sin transform
  if (cmd_f3a_h==`VPU_TRANS_SIN_TRANSFORM) begin
      //IV Quartile: fc < 0 && fc <= -0.5 && fc <= -0.75; res = fc+1  
    if (fc_lt_0_f3a_h && fc_lte_m05_f3a_h && fc_lte_m075_f3a_h) begin
      sin_align1_neg_f3a_h = sc_f3a_h;
      //III Quartile: fc < 0 && fc <= -0.5; res = -0.5-fc     
    end else if (fc_lt_0_f3a_h && fc_lte_m05_f3a_h) begin
      sin_align1_neg_f3a_h = !sc_f3a_h;
      sin_align2_neg_f3a_h = 1'b1;
      sin_check_fc_sign_f3a_h = 1'b1;
      sin_ediff2_f3a_h = 1;      
      //II Quartile: fc < 0 && fc <= -0.25; res = -0.5-fc
    end else if (fc_lt_0_f3a_h && fc_lte_m025_f3a_h) begin
      sin_align1_neg_f3a_h = !sc_f3a_h;
      sin_align2_neg_f3a_h = 1'b1;      
      sin_ediff2_f3a_h = 1;            
    end else if (!fc_lt_0_f3a_h) begin
        //IV Quartile: fc > 0 && fc >= 0.5 && fc >= 0.75; res = fc-1
      if (fc_gt_0_f3a_h && fc_gte_05_f3a_h && fc_gte_075_f3a_h) begin
        sin_align1_neg_f3a_h = sc_f3a_h;
        sin_align2_neg_f3a_h = 1'b1;        
        //III Quartile: fc > 0 && fc >= 0.5; res = 0.5-fc
      end else if (fc_gt_0_f3a_h && fc_gte_05_f3a_h) begin
        sin_check_fc_sign_f3a_h = 1'b1;
        sin_align1_neg_f3a_h = !sc_f3a_h; 
        sin_ediff2_f3a_h = 1;              
      end
        //II Quartile: fc > 0 && fc >= 0.25; res = 0.5-fc
      else if (fc_gt_0_f3a_h && fc_gte_025_f3a_h) begin
        sin_align1_neg_f3a_h = !sc_f3a_h;   
        sin_ediff2_f3a_h = 1;      
        //I Quartile   
      end 
      else begin
        int_minmax_mux_sel_fc_f3a_h = 1'b1;
      end
      //I Quartile    
    end else begin
      int_minmax_mux_sel_fc_f3a_h = 1'b1;
    end
  end
end


///////////////////////////////////////////////////////////////////////////////
// FROUND R control bit
//   We convert the single-precision FP number into an Integer; round and
//   then convert back to SP FP. Due to the timing, we cannot use the main
//   datapath rounding logic. For that reason, we calculate the round bit
//   here for the FROUND instruction only.
///////////////////////////////////////////////////////////////////////////////

always_comb begin
  fround_l_f3a_h = ((rm_f3a_h==RNE) & aligned_48_int_r_f3a_h & !(aligned_48_int_r_f3a_h & !e_sticky1_f3a_h & !aligned_48_int_l_f3a_h))  |
                   ((rm_f3a_h==RMM) & aligned_48_int_r_f3a_h)                                                                           |
                   ((rm_f3a_h==RDN) & sc_f3a_h & (e_sticky1_f3a_h | aligned_48_int_r_f3a_h))                                         |
                   ((rm_f3a_h==RUP) & !sc_f3a_h & (e_sticky1_f3a_h | aligned_48_int_r_f3a_h));                 

    // do not round if operand is zero
  fround_l_f3a_h &= !ec_zero_f3a_h;
end

assign fround_sticky_until_l_f3a_h = (e_sticky1_f3a_h | aligned_48_int_r_f3a_h) & (cmd_f3a_h==`VPU_FCMD_FROUND);

endmodule
