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

/* verilator lint_off CASEINCOMPLETE */  // Imported original TXFMA operand-select decode preserves sparse selector coverage.
/* verilator lint_off UNUSEDSIGNAL */  // Packed stage-control fields intentionally keep unused bits in the original layout.
module txfma_c3
(
  input  [VPU_DTYPE_SZ-1:0]                  op_dtype_f3a_h,
  input  [VPU_FCMD_SZ-1:0]                   cmd_f3a_h,
  input  [TXFMA_SIGS_SZ-1:0]                 sigs_f3a_h,
  input  [TXFMA_EXP_SEXT_PS_SZ-1:0]          ediff1_f3a_h,
  input  [TXFMA_EXP_SEXT_PS_SZ-1:0]          ediff2_f3a_h,
  input  [TXFMA_EXP_PS_SZ-1:0]               ec_f3a_h,
  input  [TXFMA_RM_SZ-1:0]                   rm_f3a_h,
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
  input                                       e_sticky1_f3a_h,
  input                                       fround_rbit_f3a_h,
  input                                       prodsum_a2_f3a_h,
  input                                       ediff1_0_f3a_h,
  input                                       fa_ge_fc_comp_sign_hi_res_f3a_h,
  input                                       fa_eq_fc_comp_sign_hi_res_f3a_h,
  input                                       int_minmax_mux_sel_fc_f3a_h,
  input                                       minmax_op_f3a_h,
  input                                       comp_op_f3a_h,
  input                                       res_inf_f3a_h,
  input                                       res_qnan_f3a_h,
  input                                       res_snan_f3a_h,
  input                                       spec_res_en_f3a_h,
  input                                       flags_en_f3a_h,
  input                                       trans_log_rr_neg_f3a_h,
  input                                       sin_align1_neg_f3a_h,
  input                                       sin_align2_neg_f3a_h,
  input                                       sin_eff_sub1_f3a_h,
  input                                       sin_check_fc_sign_f3a_h,
  input                                       fc_sel_f3a_h,
  input [TXFMA_EDIFF_OPD_SEL_SZ-1:0]         opd3_mux_sel_f3a_h,
  output [1:0]                                imxc_sel_f3a_h,
  output                                      res_snan_out_f3a_h,
  output                                      res_qnan_out_f3a_h,
  output                                      res_inf_out_f3a_h,
  output                                      res_neg_inf_out_f3a_h,
  output                                      comp_res_sel_f3a_h,
  output                                      fa_eq_fc_res_check_hi_f3a_h,
  output                                      eff_sub1_out_f3a_h,
  output                                      cmp_invalid_f3a_h,
  output                                      fma_subopc3_f3a_h,
  output logic                                s_res_f3a_h,
  output logic                                s_res_check_cadd_a3_f3a_h,
  output logic                                int_minmax_mux_sel_a_f3a_h,
  output logic                                int_minmax_mux_sel_c_f3a_h,
  output logic                                fround_stky2l_f3a_h
);


///////////////////////////////////////////////////////////////////////////////
// Internal Signals
///////////////////////////////////////////////////////////////////////////////
logic [TXFMA_EXP_SEXT_PS_SZ-1:0]      ediff1_abs_f3a_h;
logic                                  ediff_hi_is_1_f3a_h;
logic                                  ediff1_is_1_f3a_h;
logic                                  eprod_ge_ec_1_f3a_h;
logic                                  eprod_ge_ec_hi_f3a_h;
logic                                  eff_sub_f3a_h;
logic                                  eprod_lt_ec_f3a_h;
logic                                  eprod_lt_ec_1_f3a_h;
logic                                  eprod_lte_ec_2_f3a_h;
logic                                  ediff1_is_0_f3a_h;
logic                                  exp_frac_neg_inf_f3a_h;
logic                                  exp_frac_inf_f3a_h;
logic                                  res_inf_mux_f3a_h;
logic                                  res_qnan_mux_f3a_h;
logic                                  res_snan_mux_f3a_h;
logic                                  ec_hi_gte_127_f3a_h;
logic                                  res_qnan_minmax_f3a_h;
logic                                  res_snan_minmax_f3a_h;
logic                                  fa_ge_fc_f32_frac_hi_f3a_h;
logic                                  fa_ge_fc_comp_exp_hi_f3a_h;
logic                                  fa_ge_fc_res_sel_hi_f3a_h;
logic                                  minmax_res_sel_f3a_h;
logic [TXFMA_MINMAX_OPD_SEL_SZ-1:0]   int_minmax_mux_sel_f3a_h;

///////////////////////////////////////////////////////////////////////////////
// ediff control logic
///////////////////////////////////////////////////////////////////////////////
  //exponent product and ec comparations
assign eprod_ge_ec_1_f3a_h  = ($signed(ediff1_f3a_h)<0);
assign eprod_lt_ec_1_f3a_h  = ($signed(ediff1_f3a_h)>0);
assign eprod_lte_ec_2_f3a_h = ($signed(ediff2_f3a_h)>=0);
assign ediff1_is_0_f3a_h    = ($signed(ediff1_f3a_h)==0);
assign ediff1_is_1_f3a_h    = ($signed(ediff1_abs_f3a_h)==1);

assign ec_hi_gte_127_f3a_h    = (ec_f3a_h >= 127);

  //ediff absolute values
assign ediff1_abs_f3a_h = ediff1_f3a_h[TXFMA_EXP_SEXT_PS_SZ-1] ? -ediff1_f3a_h : ediff1_f3a_h;

assign int_minmax_mux_sel_a_f3a_h = (int_minmax_mux_sel_f3a_h == TXFMA_INT_MINMAX_SEL_A);
assign int_minmax_mux_sel_c_f3a_h = (int_minmax_mux_sel_f3a_h == TXFMA_INT_MINMAX_SEL_C);

///////////////////////////////////////////////////////////////////////////////
// elxd control
///////////////////////////////////////////////////////////////////////////////
assign fma_subopc3_f3a_h = (op_dtype_f3a_h==VPU_DTYPE_F16_F32) ? s_res_check_cadd_a3_f3a_h
                                                                : (eff_sub1_f3a_h | trans_log_rr_neg_f3a_h);

assign eff_sub1_out_f3a_h = (cmd_f3a_h==VPU_TRANS_SIN_TRANSFORM) ? sin_eff_sub1_f3a_h : eff_sub1_f3a_h;


///////////////////////////////////////////////////////////////////////////////
// fround sticky 2
///////////////////////////////////////////////////////////////////////////////
assign fround_stky2l_f3a_h = (fround_rbit_f3a_h | e_sticky1_f3a_h);


///////////////////////////////////////////////////////////////////////////////
// min/max and compare logic
///////////////////////////////////////////////////////////////////////////////

  //FA is equal to FC if any of the compares bellow is set.
  //This signal is passed to F4 for final mantissa equal check (TRZ)
assign fa_eq_fc_res_check_hi_f3a_h  = ((eprod_hi_zero_f3a_h & ec_zero_f3a_h) | (!eff_sign1_f3a_h & ediff1_is_0_f3a_h)) &
                                       !(a_is_qnan_hi_f3a_h | c_is_qnan_f3a_h | res_snan_mux_f3a_h);

  //exp frac bound compare conditions
    //opc<-166
assign exp_frac_neg_inf_f3a_h = ((sc_f3a_h & fa_ge_fc_comp_exp_hi_f3a_h) | (ediff1_is_0_f3a_h & fa_ge_fc_res_sel_hi_f3a_h)) & (cmd_f3a_h==VPU_TRANS_EXP_FRAC) & !flags_en_f3a_h & ~(res_qnan_f3a_h | res_snan_f3a_h);
    //opc>128
assign exp_frac_inf_f3a_h  = !sc_f3a_h & eprod_lte_ec_2_f3a_h & (cmd_f3a_h==VPU_TRANS_EXP_FRAC);

  //comp op selector:
    //'1': fa==fc (feq), fa<=fc (fle), fa<fc (flt), fc!=0
    // note: final mantissa equal check is done at f4
assign comp_res_sel_f3a_h = ((!fa_ge_fc_res_sel_hi_f3a_h &(cmd_f3a_h==VPU_FCMD_FLT))    |
                            (!fa_ge_fc_res_sel_hi_f3a_h & (cmd_f3a_h==VPU_FCMD_FLE)))  &
                              //return always false when there is a NaN
                            !(a_is_qnan_hi_f3a_h | c_is_qnan_f3a_h | res_snan_mux_f3a_h);


  // This logic has been moved here due to the timing of prodsum_a2 which is
  // a very late F2 signal

  //fa greater than fc compare by fraction
assign fa_ge_fc_f32_frac_hi_f3a_h       = (!prod_sign_hi_f3a_h & !prodsum_a2_f3a_h) | (prod_sign_hi_f3a_h & prodsum_a2_f3a_h) |
                                          (eprod_hi_zero_f3a_h & !ec_zero_f3a_h);

    //fa greater than fc compared by exponent
assign fa_ge_fc_comp_exp_hi_f3a_h  = (!prod_sign_hi_f3a_h & eprod_ge_ec_hi_f3a_h & !eprod_hi_zero_f3a_h) |
                                     (prod_sign_hi_f3a_h & !(eprod_ge_ec_hi_f3a_h & !eprod_hi_zero_f3a_h) & !ediff1_0_f3a_h);


  //FA is greather than FC if any of the compares bellow is set
assign fa_ge_fc_res_sel_hi_f3a_h  =  (fa_ge_fc_comp_sign_hi_res_f3a_h                                |                  //sign
                                     (fa_eq_fc_comp_sign_hi_res_f3a_h & fa_ge_fc_comp_exp_hi_f3a_h)  |                  //exp
                                     (fa_eq_fc_comp_sign_hi_res_f3a_h & ediff1_0_f3a_h & fa_ge_fc_f32_frac_hi_f3a_h));  //frac

  //minmax op selector:
    //'1': fa>=fc (max), fa<=fc (min)
    //'0': fa<fc  (max), fa>fc (min)
always_comb
begin
  minmax_res_sel_f3a_h = 1'b0;

    //fmax
  if  (cmd_f3a_h==VPU_FCMD_MAX) begin
    minmax_res_sel_f3a_h  = fa_ge_fc_res_sel_hi_f3a_h;                  // opa greater than opc
    minmax_res_sel_f3a_h &= !(a_is_qnan_hi_f3a_h | a_is_snan_hi_f3a_h); // opc greater if opa is NaN
    minmax_res_sel_f3a_h |= (c_is_qnan_f3a_h | c_is_snan_f3a_h);  // opa greater if opc is NaN
  end

    //fmin
  if  (cmd_f3a_h==VPU_FCMD_MIN) begin
    minmax_res_sel_f3a_h  = !fa_ge_fc_res_sel_hi_f3a_h;                 // opa smaller than opc
    minmax_res_sel_f3a_h &= !(a_is_qnan_hi_f3a_h | a_is_snan_hi_f3a_h); // opa smaller if opc is NaN
    minmax_res_sel_f3a_h |= (c_is_qnan_f3a_h | c_is_snan_f3a_h);  // opa smaller if opa is NaN
  end

end

always_comb
begin
  int_minmax_mux_sel_f3a_h = TXFMA_INT_MINMAX_SEL_PH;

    //int add
  if ((op_dtype_f3a_h == VPU_DTYPE_INT) & (cmd_f3a_h!=VPU_FCMD_CVT_F32INT) & (cmd_f3a_h!=VPU_FCMD_CVT_F32RAST)) begin
      //int mul low
    if ((cmd_f3a_h==VPU_FCMD_MULL) | (cmd_f3a_h==VPU_FCMD_NR1_FRCPFXP))
      int_minmax_mux_sel_f3a_h = TXFMA_INT_MINMAX_SEL_PL;
  end

    //min max
  if (minmax_op_f3a_h) begin
    if (minmax_res_sel_f3a_h) begin
      int_minmax_mux_sel_f3a_h = TXFMA_INT_MINMAX_SEL_A;
    end else begin
      int_minmax_mux_sel_f3a_h = TXFMA_INT_MINMAX_SEL_C;
    end
  end

    //trans log rr
  if (cmd_f3a_h==VPU_TRANS_LOG_RR)
    int_minmax_mux_sel_f3a_h = TXFMA_INT_MINMAX_SEL_S;

    //trans sin: select fa in case of beeing at I Quartile
  if (cmd_f3a_h==VPU_TRANS_SIN_TRANSFORM && int_minmax_mux_sel_fc_f3a_h)
    int_minmax_mux_sel_f3a_h = TXFMA_INT_MINMAX_SEL_C;

    //ffrc
  if (fc_sel_f3a_h)
    int_minmax_mux_sel_f3a_h = TXFMA_INT_MINMAX_SEL_C;

    //float32 to floatX CVT
  if (sigs_f3a_h[TXFMA_FX_F32])
    int_minmax_mux_sel_f3a_h = TXFMA_INT_MINMAX_SEL_A;

    //floatX to float32 CVT
  if (sigs_f3a_h[TXFMA_F32_FX])
    int_minmax_mux_sel_f3a_h = TXFMA_INT_MINMAX_SEL_C;
end

assign imxc_sel_f3a_h[1]     = ((int_minmax_mux_sel_f3a_h == TXFMA_INT_MINMAX_SEL_PL) |  (int_minmax_mux_sel_f3a_h == TXFMA_INT_MINMAX_SEL_PH) | (int_minmax_mux_sel_f3a_h == TXFMA_INT_MINMAX_SEL_S));
assign imxc_sel_f3a_h[0]     = ((int_minmax_mux_sel_f3a_h == TXFMA_INT_MINMAX_SEL_C) |  (int_minmax_mux_sel_f3a_h == TXFMA_INT_MINMAX_SEL_PH) | (int_minmax_mux_sel_f3a_h == TXFMA_INT_MINMAX_SEL_S));


///////////////////////////////////////////////////////////////////////////////
// clear special cases for minmax and compare operations
///////////////////////////////////////////////////////////////////////////////
assign res_qnan_minmax_f3a_h = minmax_op_f3a_h & (a_is_qnan_hi_f3a_h | a_is_snan_hi_f3a_h) & (c_is_qnan_f3a_h | c_is_snan_f3a_h);
assign res_snan_minmax_f3a_h = minmax_op_f3a_h & (a_is_snan_hi_f3a_h & c_is_snan_f3a_h);
assign res_snan_out_f3a_h = (!comp_op_f3a_h & !minmax_op_f3a_h & res_snan_mux_f3a_h);
assign res_qnan_out_f3a_h = (!comp_op_f3a_h & !minmax_op_f3a_h & res_qnan_mux_f3a_h) | (res_qnan_minmax_f3a_h | res_snan_minmax_f3a_h);

assign res_inf_out_f3a_h  = (!comp_op_f3a_h & !minmax_op_f3a_h & res_inf_mux_f3a_h)  | exp_frac_inf_f3a_h;
assign res_neg_inf_out_f3a_h  = exp_frac_neg_inf_f3a_h;

// result is invalid if any input is NaN in case of compare
assign cmp_invalid_f3a_h = (((cmd_f3a_h==VPU_FCMD_FLE) | (cmd_f3a_h==VPU_FCMD_FLT)) & (a_is_qnan_hi_f3a_h | a_is_snan_hi_f3a_h | c_is_qnan_f3a_h | c_is_snan_f3a_h)) |
                            ((cmd_f3a_h==VPU_FCMD_FEQ) & (a_is_snan_hi_f3a_h | c_is_snan_f3a_h));


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
  eprod_ge_ec_hi_f3a_h = eprod_ge_ec_1_f3a_h;
  eff_sub_f3a_h        = eff_sub1_f3a_h;
  eprod_lt_ec_f3a_h    = eprod_lt_ec_1_f3a_h;
  ediff_hi_is_1_f3a_h  = ediff1_is_1_f3a_h;
end

  //sign hi
always_comb
begin
  s_res_f3a_h               = 1'b0;
  s_res_check_cadd_a3_f3a_h = 1'b0;

//mul
  if (sigs_f3a_h[TXFMA_MUL] & !(op_dtype_f3a_h==VPU_DTYPE_F16_F32)) begin
    s_res_f3a_h = prod_sign_hi_f3a_h;

    if (cmd_f3a_h==VPU_TRANS_LOG_MUL) begin
      s_res_f3a_h = !ec_hi_gte_127_f3a_h;
      s_res_f3a_h |= res_inf_f3a_h & ec_zero_f3a_h;
    end
  end
// end mul

//min
  else if (cmd_f3a_h==VPU_FCMD_MIN) begin
    s_res_f3a_h = ((minmax_res_sel_f3a_h & prod_sign_hi_f3a_h) | (!minmax_res_sel_f3a_h & sc_f3a_h));
  end
// end min

//max
  else if (cmd_f3a_h==VPU_FCMD_MAX) begin
    s_res_f3a_h = ((minmax_res_sel_f3a_h & prod_sign_hi_f3a_h) | (!minmax_res_sel_f3a_h & sc_f3a_h));
  end
// end max

//cvt
  else if (sigs_f3a_h[TXFMA_CVT] || ((cmd_f3a_h==VPU_FCMD_FFRC) | (cmd_f3a_h==VPU_FCMD_FFRC_SIN))) begin

    s_res_f3a_h = sc_f3a_h & (cmd_f3a_h!=VPU_FCMD_CVT_F32UINT);
    if (cmd_f3a_h==VPU_FCMD_CVT_INTF32 || cmd_f3a_h==VPU_FCMD_CVT_RASTF32)
      s_res_f3a_h = sc_f3a_h;
  end
// end cvt

// gcvt
  else if (sigs_f3a_h[TXFMA_SGCVT]) begin
    if (sigs_f3a_h[TXFMA_F32_NORM] || op_dtype_f3a_h==VPU_DTYPE_F16)
      s_res_f3a_h = sc_f3a_h;
    else
      s_res_f3a_h = prod_sign_hi_f3a_h;
  end
// end gcvt

//trans fma2
  else if (sigs_f3a_h[TXFMA_TRANS2]) begin
    s_res_f3a_h = prod_sign_hi_f3a_h;

    if (cmd_f3a_h==VPU_TRANS_EXP_FMA2)
      s_res_f3a_h = 1'b0;
  end
  //exp
  else if (cmd_f3a_h==VPU_TRANS_EXP_FRAC) begin
    s_res_f3a_h = sc_f3a_h;
  end
    //sin p2
  else if (cmd_f3a_h==VPU_TRANS_SIN_P2) begin
      s_res_f3a_h = 1'b0;
  end
    //sin transfrom
  else if (cmd_f3a_h==VPU_TRANS_SIN_TRANSFORM) begin
    s_res_f3a_h = sin_align2_neg_f3a_h;

    if (sin_check_fc_sign_f3a_h)
      s_res_f3a_h = sin_align1_neg_f3a_h;

    if (fc_sel_f3a_h)
      s_res_f3a_h = sc_f3a_h;
  end
//end trans fma2

// Tensor
  else if (op_dtype_f3a_h==VPU_DTYPE_F16_F32) begin
    case (opd3_mux_sel_f3a_h)
      TXFMA_SEL_PH: s_res_f3a_h     = prod_sign_hi_f3a_h;
      TXFMA_SEL_PL: s_res_f3a_h     = prod_sign_lo_f3a_h;
      TXFMA_SEL_C:  s_res_f3a_h     = sc_f3a_h;
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
      if ((eprod_hi_zero_f3a_h && eprod_lo_zero_f3a_h && !(!ec_zero_f3a_h && sigs_f3a_h[TXFMA_FMA])) && (eff_sub1_f3a_h | eff_sub2_f3a_h)) begin
        s_res_f3a_h = (rm_f3a_h==RDN);
      end
    end
  end
// end Tensor

//PS
  else begin
      //effective sub
    if (eff_sub_f3a_h) begin
        //eprod greater than ec
      if (eprod_ge_ec_hi_f3a_h & !eprod_hi_zero_f3a_h) begin
        s_res_f3a_h = prod_sign_hi_f3a_h;
        if (res_inf_mux_f3a_h & c_inf_f3a_h)
          s_res_f3a_h = sc_f3a_h ^ (sigs_f3a_h[TXFMA_SUB] | sigs_f3a_h[TXFMA_FMS]);
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
          s_res_f3a_h = sc_f3a_h ^ (sigs_f3a_h[TXFMA_SUB] | sigs_f3a_h[TXFMA_FMS]);
      end
        //result is infinity and ediff==1
      else if (res_inf_mux_f3a_h) begin
        s_res_f3a_h = sc_f3a_h ^ (sigs_f3a_h[TXFMA_SUB] | sigs_f3a_h[TXFMA_FMS]);
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
end  // always_comb

endmodule
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on CASEINCOMPLETE */
