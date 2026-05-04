// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_special_detect
(
  input        [`TXFMA_SIGS_SZ-1:0] sigs,
  input        [`VPU_FCMD_SZ-1:0]   cmd,
  input        [`VPU_DTYPE_SZ-1:0]  op_dtype,
  input                             trans_exp_fma2_exc,
  input        [`VPU_DATA_S_SZ-1:0] opa, 
  input        [`VPU_DATA_S_SZ-1:0] opb,
  input        [`VPU_DATA_S_SZ-1:0] opc,
  input                             ea_hi_zero, 
  input                             eb_hi_zero, 
  input                             ec_zero,  
  input                             ea_hi_all_one,
  input                             eb_hi_all_one,
  input                             ec_all_one,
  input                             sa_hi, 
  input                             sb_hi,
  input                             sc,
  input                             ea_lo_zero, 
  input                             eb_lo_zero, 
  input                             sa_lo, 
  input                             sb_lo,
  input                             ea_lo_all_one,
  input                             eb_lo_all_one,
  input                             eb_hi_eq_254,
  input                             eb_hi_eq_253,
  input                             eb_hi_eq_125,
  input                             ec_gt_133,
  input                             ec_eq_133,
  output logic                      res_zero,
  output logic                      res_one,
  output logic                      fa_hi_zero,
  output logic                      fb_hi_zero,
  output logic                      fc_zero30_8,
  output logic                      a_is_qnan_hi,
  output logic                      c_inf,
  output logic                      a_inf,
  output logic                      c_is_qnan,
  output logic                      a_is_snan_hi,
  output logic                      c_is_snan,
  output logic                      prod_inf_hi,
  output logic [`VPU_FLAGS_SZ-1:0]  flags,
  output logic                      prod_inf_lo,
  output logic                      res_unorm_one,
  output logic                      res_snorm_one,
  output logic                      res_inf,
  output logic                      res_snan,
  output logic                      res_qnan,
  output logic                      res_pos_int_nan,
  output logic                      res_neg_int_nan,
  output logic                      res_uint_nan,
  output logic [`FP_CLASS_BITS-1:0] classify_c,  
  output logic                      flags_en,
  output logic                      spec_res_en
);

logic fa_zero29_27;
logic fb_zero29_27;
logic fc_zero29_27;
logic fa_zero29_26;
logic fb_zero29_26;
logic fc_zero29_26;
logic fa_zero29_25;
logic fb_zero29_25;
logic fc_zero29_25;
logic fa_zero29_24;
logic fb_zero29_24;
logic fc_zero29_24; 
logic fa_zero29_22;
logic fb_zero29_22;
logic fc_zero29_22; 
logic fa_zero29_21;
logic fb_zero29_21;
logic fc_zero29_21;
logic fa_zero29_16;
logic fb_zero29_16;
logic fc_zero29_16;
logic fa_zero29_8;
logic fb_zero29_8;
logic fc_zero29_8;
logic fa_zero30_26;
logic fb_zero30_26;
logic fc_zero30_26;
logic fa_zero30_25;
logic fb_zero30_25;
logic fc_zero30_25;
logic fa_zero30_24;
logic fb_zero30_24;
logic fc_zero30_24; 
logic fc_zero30_23;
logic fa_zero30_22;
logic fb_zero30_22;
logic fc_zero30_22; 
logic fa_zero30_21;
logic fb_zero30_21;
logic fc_zero30_21;
logic fc_zero30_17;
logic fa_zero30_16;
logic fb_zero30_16;
logic fc_zero30_16;
logic fc_zero30_9;
logic fa_zero10_1;
logic fb_zero10_1;
logic fa_zero9_1;
logic fc_one30_24;
logic fc_one30_22;
logic fc_one30_16;
logic fc_one30_8;
logic fb_zero11_3;
logic fb_zero12_3;

logic a_zero_hi;   
logic b_zero_hi;
logic c_zero_hi;
logic res_pot_inf_hi;
logic res_pot_inf_lo;

logic res_pot_qnan_hi;
logic inf_nan1_hi;
logic inf_nan2_hi;
logic inf_nan1_lo;
logic inf_nan2_lo;
logic [`FP_CLASS_BITS-1:0] classify_a_hi;
logic [`FP_CLASS_BITS-1:0] classify_b_hi;
logic [`FP_CLASS_BITS-1:0] classify_a_lo;
logic [`FP_CLASS_BITS-1:0] classify_b_lo;
logic sign_prod_hi;
logic sign_prod_lo;
logic s_prod_hi_prod_lo;
logic s_prod_hi_c_hi;
logic s_prod_lo_c_hi;
logic flags_denorm;
logic res_inf_trans;
logic rsqrt_res_nan_inv_hi;
logic sin_res_nan_inv_hi;
logic log_res_nan_inv_hi;
logic fa_zero30_8;
logic fb_zero30_8;


  //fa/b zero detect signals
always_comb begin
  fa_hi_zero = fa_zero30_8;
  fb_hi_zero = fb_zero30_8;

    //trans log int mul zero detection 
  if (cmd==`VPU_TRANS_LOG_MUL) begin
    fa_hi_zero = !(|opa[31:0]);
    fb_hi_zero = !(|opb[31:0]);
  end
end

////////////////////////////////////////////////////

//bit mapping acoording to INMUX out

// zeros 
assign fa_zero29_27 = !(|opa[29:27]);
assign fb_zero29_27 = !(|opb[29:27]);
assign fc_zero29_27 = !(|opc[29:27]); 

assign fa_zero29_26 = fa_zero29_27 & ~opa[26];
assign fb_zero29_26 = fb_zero29_27 & ~opb[26];
assign fc_zero29_26 = fc_zero29_27 & ~opc[26];
assign fa_zero30_26 = ~opa[30] & fa_zero29_26;
assign fb_zero30_26 = ~opb[30] & fb_zero29_26;
assign fc_zero30_26 = ~opc[30] & fc_zero29_26;

assign fa_zero29_25 = fa_zero29_26 & !opa[25];
assign fb_zero29_25 = fb_zero29_26 & !opb[25];
assign fc_zero29_25 = fc_zero29_26 & !opc[25];
assign fa_zero30_25 = !opa[30] & fa_zero29_25;
assign fb_zero30_25 = !opb[30] & fb_zero29_25;
assign fc_zero30_25 = !opc[30] & fc_zero29_25;

assign fa_zero29_24 = fa_zero29_25 & !opa[24];
assign fb_zero29_24 = fb_zero29_25 & !opb[24];
assign fc_zero29_24 = fc_zero29_25 & !opc[24]; 
assign fa_zero30_24 = !opa[30] & fa_zero29_24;
assign fb_zero30_24 = !opb[30] & fb_zero29_24;
assign fc_zero30_24 = !opc[30] & fc_zero29_24; 

assign fc_zero30_23 = !opc[30] & fc_zero30_24 & !opc[23];

assign fa_zero29_22 = fa_zero29_24 & !(opa[23] | opa[22]);
assign fb_zero29_22 = fb_zero29_24 & !(opb[23] | opb[22]);
assign fc_zero29_22 = fc_zero29_24 & !(opc[23] | opc[22]); 
assign fa_zero30_22 = !opa[30] & fa_zero29_22;
assign fb_zero30_22 = !opb[30] & fb_zero29_22;
assign fc_zero30_22 = !opc[30] & fc_zero29_22; 

assign fa_zero29_21 = fa_zero29_22 & !opa[21];
assign fb_zero29_21 = fb_zero29_22 & !opb[21];
assign fc_zero29_21 = fc_zero29_22 & !opc[21];
assign fa_zero30_21 = !opa[30] & fa_zero29_21;
assign fb_zero30_21 = !opb[30] & fb_zero29_21;
assign fc_zero30_21 = !opc[30] & fc_zero29_21;

assign fc_zero30_17 = fc_zero30_21 & !(|opc[20:17]);

assign fa_zero29_16 = fa_zero29_21 & !(|opa[20:16]);
assign fb_zero29_16 = fb_zero29_21 & !(|opb[20:16]);
assign fc_zero29_16 = fc_zero29_21 & !(|opc[20:16]);
assign fa_zero30_16 = !opa[30] & fa_zero29_16;
assign fb_zero30_16 = !opb[30] & fb_zero29_16;
assign fc_zero30_16 = !opc[30] & fc_zero29_16;

assign fc_zero30_9 = fc_zero30_16 & !(|opc[15:9]);

assign fa_zero29_8 = fa_zero29_16 & !(|opa[15:8]);
assign fb_zero29_8 = fb_zero29_16 & !(|opb[15:8]);
assign fc_zero29_8 = fc_zero29_16 & !(|opc[15:8]);
assign fa_zero30_8 = !opa[30] & fa_zero29_8;
assign fb_zero30_8 = !opb[30] & fb_zero29_8;
assign fc_zero30_8 = !opc[30] & fc_zero29_8;
assign fa_zero9_1 = !(|opa[9:1]);
assign fa_zero10_1 = !opa[10] & !(|opa[9:1]);
assign fb_zero11_3 = !(|opb[11:3]);
assign fb_zero12_3 = !opb[12] & !(|opb[11:3]);


// ones
assign fc_one30_24 = &opc[30:24];
assign fc_one30_22 = fc_one30_24 & (&opc[23:22]);
assign fc_one30_16 = fc_one30_22 & (&opc[21:16]);
assign fc_one30_8 = fc_one30_16 & (&opc[15:8]);

assign sign_prod_hi = sa_hi ^ sb_hi;
assign sign_prod_lo = sa_lo ^ sb_lo;

assign s_prod_hi_prod_lo = sign_prod_hi ^ sign_prod_lo;
assign s_prod_hi_c_hi = sign_prod_hi ^ sc;
assign s_prod_lo_c_hi = sign_prod_lo ^ sc;

///////////////////////////////////////////////////////////////////////////////
// Output format mux
///////////////////////////////////////////////////////////////////////////////

always_comb begin
  res_uint_nan = 0;
  res_zero = 0;
  res_one = 0;
  res_snan=0;
  res_pot_inf_hi=0;
  res_pot_qnan_hi=0;  
  prod_inf_hi=0;
  c_inf=0;
  a_inf=0;
  a_zero_hi = 0;
  b_zero_hi = 0;
  c_zero_hi = 0;
  classify_a_hi = '0;
  classify_b_hi = '0;  
  classify_c = '0;
  inf_nan1_hi = '0;
  inf_nan2_hi = '0;
  inf_nan1_lo = '0;
  inf_nan2_lo = '0;
  res_pot_inf_lo=0;
  prod_inf_lo=0;
  classify_a_lo = '0;
  classify_b_lo = '0;  
  res_pos_int_nan = '0;
  res_neg_int_nan = '0;
  res_inf_trans = 0;  
  res_unorm_one = 0;
  res_snorm_one = 0;
  rsqrt_res_nan_inv_hi = 0;
  sin_res_nan_inv_hi = 0;
  log_res_nan_inv_hi = 0;


      //special cases for trans2 frcp/frsqrt/exp 
  if (op_dtype==`VPU_DTYPE_INT && sigs[`TXFMA_TRANS2]) begin
    b_zero_hi = (eb_hi_zero & fb_zero30_8);

    classify_b_hi[`FP_CLASS_NEG_ZERO] = sb_hi & b_zero_hi;
    classify_b_hi[`FP_CLASS_POS_ZERO] = !sb_hi & b_zero_hi;
    classify_b_hi[`FP_CLASS_POS_DENORMAL] = !sb_hi & eb_hi_zero & !fb_zero30_8;
    classify_b_hi[`FP_CLASS_NEG_DENORMAL] = sb_hi & eb_hi_zero & !fb_zero30_8;
    classify_b_hi[`FP_CLASS_POS_INF] = !sb_hi & eb_hi_all_one & fb_zero30_8;
    classify_b_hi[`FP_CLASS_NEG_INF] = sb_hi & eb_hi_all_one & fb_zero30_8;
    classify_b_hi[`FP_CLASS_QNAN] = eb_hi_all_one & opb[30];
    classify_b_hi[`FP_CLASS_SNAN] = eb_hi_all_one & !opb[30] & !fb_zero29_8;
    
      //inf when zero or denormal 
    res_inf_trans = classify_b_hi[`FP_CLASS_POS_ZERO]     | classify_b_hi[`FP_CLASS_NEG_ZERO]     |
                    classify_b_hi[`FP_CLASS_POS_DENORMAL] | classify_b_hi[`FP_CLASS_NEG_DENORMAL];

      //zero when inf and not rsqrt
    res_zero = classify_b_hi[`FP_CLASS_POS_INF] | (classify_b_hi[`FP_CLASS_NEG_INF] & (cmd == `VPU_TRANS_RCP_FMA2));
      //frcp is zero on a value greater than 2 ^126
    res_zero |= (eb_hi_eq_254 | (eb_hi_eq_253 & !fb_zero30_8)) & (cmd==`VPU_TRANS_RCP_FMA2);
      //trans are NaN
    res_pot_qnan_hi  = classify_b_hi[`FP_CLASS_QNAN];
    res_snan = classify_b_hi[`FP_CLASS_SNAN];

      //rsqrt is nan when opb is negative and not zero/denormal
    rsqrt_res_nan_inv_hi = (sb_hi & !(classify_b_hi[`FP_CLASS_NEG_ZERO] | classify_b_hi[`FP_CLASS_NEG_DENORMAL] | classify_b_hi[`FP_CLASS_QNAN]) & (cmd == `VPU_TRANS_RSQRT_FMA2));
    res_pot_qnan_hi |= rsqrt_res_nan_inv_hi;

      //clear special detection for exp fma2, as they were treated at trans exp frac operation   
    if (cmd==`VPU_TRANS_EXP_FMA2) begin
      res_inf_trans = 1'b0;
      res_one  = classify_b_hi[`FP_CLASS_NEG_DENORMAL] | classify_b_hi[`FP_CLASS_POS_DENORMAL];      
      res_zero = classify_b_hi[`FP_CLASS_NEG_INF];      
    end
  end

  if (op_dtype==`VPU_DTYPE_F32) begin
    // if opa[31] is 0 that means all exp bits are 0.This is done in InMux
    a_zero_hi = ea_hi_zero & fa_zero30_8; 
    b_zero_hi = eb_hi_zero & fb_zero30_8;
    c_zero_hi = ec_zero & fc_zero30_8;
    
    classify_a_hi[`FP_CLASS_NEG_INF] = sa_hi & ea_hi_all_one & fa_zero30_8;
    classify_a_hi[`FP_CLASS_NEG_NORMAL] = sa_hi & !ea_hi_zero & !ea_hi_all_one;
    classify_a_hi[`FP_CLASS_NEG_DENORMAL] = sa_hi & ea_hi_zero & !fa_zero30_8;
    classify_a_hi[`FP_CLASS_NEG_ZERO] = sa_hi & a_zero_hi;
    classify_a_hi[`FP_CLASS_POS_ZERO] = !sa_hi & a_zero_hi;
    classify_a_hi[`FP_CLASS_POS_DENORMAL] = !sa_hi & ea_hi_zero & !fa_zero30_8;
    classify_a_hi[`FP_CLASS_POS_NORMAL] = !sa_hi & !ea_hi_zero & !ea_hi_all_one;
    classify_a_hi[`FP_CLASS_POS_INF] = !sa_hi & ea_hi_all_one & fa_zero30_8;
    classify_a_hi[`FP_CLASS_SNAN] = ea_hi_all_one & !opa[30] & !fa_zero29_8;
    classify_a_hi[`FP_CLASS_QNAN] = ea_hi_all_one & opa[30];
    
    classify_b_hi[`FP_CLASS_NEG_INF] = sb_hi & eb_hi_all_one & fb_zero30_8;
    classify_b_hi[`FP_CLASS_NEG_NORMAL] = sb_hi & !eb_hi_zero & !eb_hi_all_one;
    classify_b_hi[`FP_CLASS_NEG_DENORMAL] = sb_hi & eb_hi_zero & !fb_zero30_8;
    classify_b_hi[`FP_CLASS_NEG_ZERO] = sb_hi & b_zero_hi;
    classify_b_hi[`FP_CLASS_POS_ZERO] = !sb_hi & b_zero_hi;
    classify_b_hi[`FP_CLASS_POS_DENORMAL] = !sb_hi & eb_hi_zero & !fb_zero30_8;
    classify_b_hi[`FP_CLASS_POS_NORMAL] = !sb_hi & !eb_hi_zero & !eb_hi_all_one;
    classify_b_hi[`FP_CLASS_POS_INF] = !sb_hi & eb_hi_all_one & fb_zero30_8;
    classify_b_hi[`FP_CLASS_SNAN] = eb_hi_all_one & !opb[30] & !fb_zero29_8;
    classify_b_hi[`FP_CLASS_QNAN] = eb_hi_all_one & opb[30];

    classify_c[`FP_CLASS_NEG_INF] = sc & ec_all_one & fc_zero30_8;
    classify_c[`FP_CLASS_NEG_NORMAL] = sc & !ec_zero & !ec_all_one;
    classify_c[`FP_CLASS_NEG_DENORMAL] = sc & ec_zero & !fc_zero30_8;
    classify_c[`FP_CLASS_NEG_ZERO] = sc & c_zero_hi;
    classify_c[`FP_CLASS_POS_ZERO] = !sc & c_zero_hi;
    classify_c[`FP_CLASS_POS_DENORMAL] = !sc & ec_zero & !fc_zero30_8;
    classify_c[`FP_CLASS_POS_NORMAL] = !sc & !ec_zero & !ec_all_one;
    classify_c[`FP_CLASS_POS_INF] = !sc & ec_all_one & fc_zero30_8;
    classify_c[`FP_CLASS_SNAN] = ec_all_one & !opc[30] & !fc_zero29_8;
    classify_c[`FP_CLASS_QNAN] = ec_all_one & opc[30];

    c_inf = ec_all_one & fc_zero30_8;

      //sin is enabling ren3 to perform internal operation, but opc is not used, so clear special detect for operand c
    if ((cmd==`VPU_TRANS_SIN_P1) || (cmd==`VPU_TRANS_SIN_P2) || (cmd==`VPU_TRANS_SIN_P3)) begin
      classify_c &= '0;
      c_inf      = 0;
    end

    // potential infinity (in case NaN doesn't occur)
    res_pot_inf_hi = ea_hi_all_one & fa_zero30_8 | eb_hi_all_one & fb_zero30_8 | c_inf;
    a_inf = ea_hi_all_one & fa_zero30_8;
    prod_inf_hi = ea_hi_all_one & fa_zero30_8 | eb_hi_all_one & fb_zero30_8;
    // potential qnan (in case the result isn't snan)
    res_pot_qnan_hi = classify_a_hi[`FP_CLASS_QNAN] | classify_b_hi[`FP_CLASS_QNAN] | classify_c[`FP_CLASS_QNAN];
    // 0*inf
    inf_nan1_hi = ea_hi_all_one & fa_zero30_8 & eb_hi_zero | eb_hi_all_one & fb_zero30_8 & ea_hi_zero;
    // inf-inf
    inf_nan2_hi = (prod_inf_hi & c_inf & (sign_prod_hi ^ sc ^ (sigs[`TXFMA_SUB] | sigs[`TXFMA_FMS]))) & ~res_pot_qnan_hi;
    res_snan = classify_a_hi[`FP_CLASS_SNAN] | classify_b_hi[`FP_CLASS_SNAN] | classify_c[`FP_CLASS_SNAN] | 
               (inf_nan1_hi & (sigs[`TXFMA_SUB] | sigs[`TXFMA_ADD] | sigs[`TXFMA_FMA] | sigs[`TXFMA_FMS] | sigs[`TXFMA_MUL])) | 
               (inf_nan2_hi & (sigs[`TXFMA_SUB] | sigs[`TXFMA_ADD] | sigs[`TXFMA_FMA] | sigs[`TXFMA_FMS] | sigs[`TXFMA_MUL]));
    // F32toINT
    res_pos_int_nan = (classify_c[`FP_CLASS_POS_INF] | classify_c[`FP_CLASS_SNAN] | classify_c[`FP_CLASS_QNAN]) & ((cmd == `VPU_FCMD_CVT_INTF32) | (cmd == `VPU_FCMD_CVT_RASTF32));
    res_neg_int_nan = classify_c[`FP_CLASS_NEG_INF] & ((cmd == `VPU_FCMD_CVT_INTF32) | (cmd == `VPU_FCMD_CVT_RASTF32));
    res_uint_nan = (classify_c[`FP_CLASS_POS_INF] | classify_c[`FP_CLASS_SNAN] | classify_c[`FP_CLASS_QNAN]) & (cmd == `VPU_FCMD_CVT_UINTF32);
    res_zero = sc & ~(sc & (classify_c[`FP_CLASS_SNAN] | classify_c[`FP_CLASS_QNAN])) & (cmd == `VPU_FCMD_CVT_UINTF32);


      //LOG
    if (cmd==`VPU_TRANS_LOG_MUL) begin
        //log result is nan when opc is negative or NaN
        //inf when opc is inf or zero
      res_inf_trans = classify_c[`FP_CLASS_POS_INF]  | classify_c[`FP_CLASS_POS_ZERO] | 
                      classify_c[`FP_CLASS_NEG_ZERO] | classify_c[`FP_CLASS_POS_DENORMAL] | classify_c[`FP_CLASS_NEG_DENORMAL];
      log_res_nan_inv_hi = (sc & !(res_inf_trans | classify_c[`FP_CLASS_QNAN]));
      res_pot_qnan_hi = log_res_nan_inv_hi | classify_c[`FP_CLASS_QNAN];                     
    end

      //EXP  
    if (cmd==`VPU_TRANS_EXP_FRAC) begin
      res_one       = classify_c[`FP_CLASS_NEG_ZERO] | classify_c[`FP_CLASS_POS_ZERO];
      res_inf_trans = classify_c[`FP_CLASS_POS_INF];
      res_pot_qnan_hi  = classify_c[`FP_CLASS_QNAN] | classify_c[`FP_CLASS_SNAN];

      //exp of neg inf input must result to +0.0
      //VPU_TRANS_EXP_FMA2 needs to detect the difference between an underflow special case and the neg inf.
      //both cases give +0.0 as result, so we let pass neg inf and VPU_TRANS_EXP_FMA2 will convert to +0.0
      res_inf_trans |= classify_c[`FP_CLASS_NEG_INF];
      res_pot_inf_hi = 1'b0;
      res_snan = classify_c[`FP_CLASS_SNAN];

      //any input value less than -126.0 is considered zero
      res_zero = ((sc & ec_gt_133) | (sc & ec_eq_133 & (&opc[30:26]) & (|opc[25:8]))) & !res_pot_qnan_hi;
    end
    
      //SIN
    if ((cmd==`VPU_TRANS_SIN_TRANSFORM) || (cmd==`VPU_FCMD_FFRC_SIN)) begin
      res_pot_qnan_hi = classify_c[`FP_CLASS_SNAN] | classify_c[`FP_CLASS_QNAN];
      res_inf_trans = classify_c[`FP_CLASS_POS_INF] | classify_c[`FP_CLASS_NEG_INF];
    end
    if (cmd==`VPU_TRANS_SIN_P3) begin
      sin_res_nan_inv_hi = classify_b_hi[`FP_CLASS_POS_INF] | classify_b_hi[`FP_CLASS_NEG_INF];     
      res_pot_qnan_hi = sin_res_nan_inv_hi | classify_b_hi[`FP_CLASS_SNAN] | classify_b_hi[`FP_CLASS_QNAN];

        //0.25
      res_one = eb_hi_eq_125 & fb_zero30_8;
    end
  end

  
    //other data types
  case (op_dtype)  
     `VPU_DTYPE_F16_F32 : begin

        // For VPU_DTYPE_F16_F32 OperandA & OperandB hold 2 16-bit half-precision FP numbers; 
        // OperandC holds 1 32-bit single-precision FP number.
        //
        classify_a_lo[`FP_CLASS_SNAN] = ea_lo_all_one & !opa[10] & !fa_zero9_1;
        classify_a_lo[`FP_CLASS_QNAN] = ea_lo_all_one & opa[10];      
        classify_b_lo[`FP_CLASS_SNAN] = eb_lo_all_one & !opb[12] & !fb_zero11_3;
        classify_b_lo[`FP_CLASS_QNAN] = eb_lo_all_one & opb[12];

        // potential infinity (in case NaN doesn't occur)
        res_pot_inf_lo = ea_lo_all_one & fa_zero10_1 | eb_lo_all_one & fb_zero12_3;
        prod_inf_lo = ea_lo_all_one & fa_zero10_1 | eb_lo_all_one & fb_zero12_3;
        // 0*inf
        inf_nan1_lo = ea_lo_all_one & fa_zero10_1 & eb_lo_zero | eb_lo_all_one & fb_zero12_3 & ea_lo_zero;

        classify_a_hi[`FP_CLASS_SNAN]    = ea_hi_all_one & !opa[30] & !fa_zero29_21;
        classify_a_hi[`FP_CLASS_QNAN]    = ea_hi_all_one & opa[30];      
        classify_b_hi[`FP_CLASS_SNAN]    = eb_hi_all_one & !opb[30] & !fb_zero29_21;
        classify_b_hi[`FP_CLASS_QNAN]    = eb_hi_all_one & opb[30];
        classify_c[`FP_CLASS_NEG_INF] = sc & ec_all_one & fc_zero30_8;
        classify_c[`FP_CLASS_POS_INF] = !sc & ec_all_one & fc_zero30_8;
        classify_c[`FP_CLASS_SNAN]    = ec_all_one & !opc[30] & !fc_zero29_8;
        classify_c[`FP_CLASS_QNAN]    = ec_all_one & opc[30];      

        prod_inf_hi = ea_hi_all_one & fa_zero30_21 | eb_hi_all_one & fb_zero30_21;

        // potential infinity (in case NaN doesn't occur)
        res_pot_inf_hi = ea_hi_all_one & fa_zero30_21 | eb_hi_all_one & fb_zero30_21 | ea_lo_all_one & fa_zero10_1 | eb_lo_all_one & fb_zero12_3 | ec_all_one & fc_zero30_8;
        c_inf = ec_all_one & fc_zero30_8;   
        // potential qnan (in case the result isn't snan)
        res_pot_qnan_hi = classify_a_hi[`FP_CLASS_QNAN] | classify_b_hi[`FP_CLASS_QNAN] | classify_a_lo[`FP_CLASS_QNAN] | classify_b_lo[`FP_CLASS_QNAN] | classify_c[`FP_CLASS_QNAN];
        // 0*inf and not QNaN
        inf_nan1_hi = (ea_hi_all_one & fa_zero30_21 & eb_hi_zero | eb_hi_all_one & fb_zero30_21 & ea_hi_zero | inf_nan1_lo) & ~res_pot_qnan_hi; 
        // inf-inf and not QNaN
        inf_nan2_hi = (prod_inf_hi & (prod_inf_lo & s_prod_hi_prod_lo | c_inf & s_prod_hi_c_hi) | prod_inf_lo & c_inf & s_prod_lo_c_hi) & ~res_pot_qnan_hi;   
        res_snan = classify_a_hi[`FP_CLASS_SNAN] | classify_b_hi[`FP_CLASS_SNAN] | classify_a_lo[`FP_CLASS_SNAN] | classify_b_lo[`FP_CLASS_SNAN] | classify_c[`FP_CLASS_SNAN] | inf_nan1_hi | inf_nan2_hi;                    
    end

   `VPU_DTYPE_F16 : begin    
        classify_c[`FP_CLASS_SNAN]    = ec_all_one & !opc[30] & !fc_zero29_21;
        classify_c[`FP_CLASS_QNAN]    = ec_all_one & opc[30];      
        classify_c[`FP_CLASS_NEG_INF] = sc & ec_all_one & fc_zero30_21;
        classify_c[`FP_CLASS_POS_INF] = !sc & ec_all_one & fc_zero30_21;

        res_pot_inf_hi = classify_c[`FP_CLASS_NEG_INF] | classify_c[`FP_CLASS_POS_INF];
        // potential qnan (in case the result isn't snan)
        res_pot_qnan_hi = classify_c[`FP_CLASS_QNAN];
        res_snan = classify_c[`FP_CLASS_SNAN];
    end

    `VPU_DTYPE_F11 : begin
        classify_c[`FP_CLASS_SNAN]    = ec_all_one & ~opc[30] & ~fc_zero29_25;
        classify_c[`FP_CLASS_QNAN]    = ec_all_one & opc[30];      
        classify_c[`FP_CLASS_NEG_INF] = sc & ec_all_one &  fc_zero30_25;
        classify_c[`FP_CLASS_POS_INF] = ~sc & ec_all_one & fc_zero30_25;   

        res_pot_inf_hi = classify_c[`FP_CLASS_NEG_INF] | classify_c[`FP_CLASS_POS_INF];
        // potential qnan (in case the result isn't snan)
        res_pot_qnan_hi = classify_c[`FP_CLASS_QNAN];
        res_snan = classify_c[`FP_CLASS_SNAN];
    end
    `VPU_DTYPE_F10 : begin
        classify_c[`FP_CLASS_SNAN]    = ec_all_one & ~opc[30] & ~fc_zero29_26;
        classify_c[`FP_CLASS_QNAN]    = ec_all_one & opc[30];      
        classify_c[`FP_CLASS_NEG_INF] = sc & ec_all_one & fc_zero30_26;
        classify_c[`FP_CLASS_POS_INF] = ~sc & ec_all_one & fc_zero30_26;   

        res_pot_inf_hi = classify_c[`FP_CLASS_NEG_INF] | classify_c[`FP_CLASS_POS_INF];
        // potential qnan (in case the result isn't snan)
        res_pot_qnan_hi = classify_c[`FP_CLASS_QNAN];
        res_snan = classify_c[`FP_CLASS_SNAN];     
    end
    `VPU_DTYPE_UN24 : begin
      c_zero_hi = !opc[31] & !opc[30] & fc_zero29_8;  
      classify_c[`FP_CLASS_POS_ZERO] = c_zero_hi; 
      res_unorm_one = opc[31] & fc_one30_8;       
    end     
    `VPU_DTYPE_UN16 : begin
      c_zero_hi = !opc[31] & !opc[30] & fc_zero29_16;
      classify_c[`FP_CLASS_POS_ZERO] = c_zero_hi;
      res_unorm_one = opc[31] & fc_one30_16;      
    end   
    `VPU_DTYPE_UN10 : begin
      c_zero_hi = !opc[31] & !opc[30] & fc_zero29_22;
      classify_c[`FP_CLASS_POS_ZERO] = c_zero_hi;
      res_unorm_one = opc[31] & fc_one30_22;
    end    
    `VPU_DTYPE_UN8 : begin
      c_zero_hi = !opc[31] & !opc[30] & fc_zero29_24;
      classify_c[`FP_CLASS_POS_ZERO] = c_zero_hi;
      res_unorm_one = opc[31] & fc_one30_24;
    end   
    `VPU_DTYPE_UN2 : begin
      c_zero_hi = !(opc[31] | opc[30]);
      classify_c[`FP_CLASS_POS_ZERO] = c_zero_hi;
      res_unorm_one = &opc[31:30];
    end      
    `VPU_DTYPE_SN16 : begin
      c_zero_hi = !opc[31] & !opc[30] & fc_zero29_16;
      classify_c[`FP_CLASS_POS_ZERO] = c_zero_hi;
      res_snorm_one = ~opc[31] & fc_one30_16 | opc[31] & fc_zero30_16 | opc[31] & fc_zero30_17 & opc[16];  
    end
    `VPU_DTYPE_SN8 : begin 
      c_zero_hi = !opc[31] & !opc[30] & fc_zero29_24;
      classify_c[`FP_CLASS_POS_ZERO] = c_zero_hi;
      res_snorm_one = ~opc[31] & fc_one30_24 | opc[31] & fc_zero30_24 | opc[31] & fc_zero30_25 & opc[24];      
    end      
  endcase
end

assign res_inf = !(res_qnan | res_snan) & res_pot_inf_hi | res_inf_trans;
assign res_qnan = !res_snan & res_pot_qnan_hi;

  //flags denorm mux
always_comb begin

  case (op_dtype)
    `VPU_DTYPE_F32 : begin
      flags_denorm = (ea_hi_zero & !fa_zero30_8) | (eb_hi_zero & !fb_zero30_8) | (ec_zero & !fc_zero30_8);
      if (sigs[`TXFMA_NORM_F32])
        flags_denorm = ea_hi_zero & !fa_zero30_8;
    end
    `VPU_DTYPE_INT : begin
      flags_denorm = 1'b0;

      if (sigs[`TXFMA_TRANS2])
        flags_denorm = (eb_hi_zero & !fb_zero30_8);
    end

    `VPU_DTYPE_F16_F32 : begin
      flags_denorm = (ea_hi_zero & !fa_zero30_21) | (eb_hi_zero & !fb_zero30_21) |
                        (ec_zero & !fc_zero30_8) | (ea_lo_zero & !fa_zero10_1)  | (eb_lo_zero & !fb_zero12_3);
    end
    `VPU_DTYPE_F16 : begin
      flags_denorm = ec_zero & !fc_zero30_21;
    end
    `VPU_DTYPE_F11 : begin
      flags_denorm = ec_zero & |opc[5:0];
    end
    `VPU_DTYPE_F10 : begin
      flags_denorm = ec_zero & |opc[4:0];
    end
    default : begin
      flags_denorm = 1'b0;
    end
  endcase

    //sin res is denormal for intermediate sin u-inst so final sin_p3 can process it andflash to zero      
  if ((cmd==`VPU_TRANS_SIN_TRANSFORM) || (cmd==`VPU_FCMD_FFRC_SIN)) begin
    flags_denorm = classify_c[`FP_CLASS_POS_DENORMAL] | classify_c[`FP_CLASS_NEG_DENORMAL];
  end
  if ((cmd==`VPU_TRANS_SIN_P1) || (cmd==`VPU_TRANS_SIN_P2)) begin
    flags_denorm = classify_a_hi[`FP_CLASS_POS_DENORMAL] | classify_a_hi[`FP_CLASS_NEG_DENORMAL] | classify_b_hi[`FP_CLASS_POS_DENORMAL] | classify_b_hi[`FP_CLASS_NEG_DENORMAL];
  end
  if ((cmd==`VPU_TRANS_SIN_P3)) begin
    flags_denorm = (eb_hi_zero & !fb_zero30_8);
  end  
end

  //flags
always_comb begin
  flags = '0;

  flags[`VPU_FLAG_DENORMAL] = flags_denorm;
  flags[`VPU_FLAG_INVALID]  = res_snan | rsqrt_res_nan_inv_hi | sin_res_nan_inv_hi | log_res_nan_inv_hi;
  flags[`VPU_FLAG_DIV0]     = (res_inf_trans && ((cmd==`VPU_TRANS_RCP_FMA2) | (cmd==`VPU_TRANS_RSQRT_FMA2)));
  flags[`VPU_FLAG_OVF]      = (trans_exp_fma2_exc && (cmd==`VPU_TRANS_EXP_FMA2) && classify_b_hi[`FP_CLASS_POS_INF]);
  flags[`VPU_FLAG_UNF]      = (trans_exp_fma2_exc && (cmd==`VPU_TRANS_EXP_FMA2) && (classify_b_hi[`FP_CLASS_POS_ZERO] | classify_b_hi[`FP_CLASS_NEG_ZERO]));

    //set inexact flag from frcp.ps on a value greater than 2^126
  flags[`VPU_FLAG_INEXACT]  = (res_zero && (cmd==`VPU_TRANS_RCP_FMA2));
end

assign a_is_qnan_hi = classify_a_hi[`FP_CLASS_QNAN];
assign c_is_qnan = classify_c[`FP_CLASS_QNAN];
assign a_is_snan_hi = classify_a_hi[`FP_CLASS_SNAN];
assign c_is_snan = classify_c[`FP_CLASS_SNAN];

  //flags enable
assign flags_en = (|flags);

  //special result enable
assign spec_res_en = res_inf | res_snan | res_qnan | res_pos_int_nan |
                     res_neg_int_nan | res_uint_nan | (|classify_c) |
                     res_unorm_one | res_snorm_one | res_zero | res_one;


endmodule
