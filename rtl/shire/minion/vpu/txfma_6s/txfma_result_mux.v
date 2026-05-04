// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_result_mux 
(
  input                                 a_inf,
  input                                 eff_sub1,
  input         [`TXFMA_RM_SZ-1:0]      rm,
  input                                 cmp_invalid,
  input                                 norm_f32_ovf,
  input                                 norm_one_out,
  input         [`VPU_FCMD_SZ-1:0]      cmd, 
  input         [`VPU_DTYPE_SZ-1:0]     op_dtype,
  input                                 ec_zero_hi,
  input                                 ediff_lt_31_hi,
  input                                 res_zero, 
  input                                 res_one,  
  input                                 cvt_fi_pos_ovf_hi,
  input                                 cvt_fi_neg_ovf_hi,   
  input                                 cvt_fi_min_neg_hi,
  input                                 cvt_fui_pos_ovf_hi,
  input                                 frac_zero_hi,
  input                                 minmax_op,
  input                                 rmode,
  input                                 comp_op,
  input                                 rnd_ovf_hi,
  input                                 pot_ovf,
  input                                 def_ovf,
  input                                 pot_unf,
  input                                 def_unf, 
  input                                 sticky_until_l,
  input         [`VPU_FLAGS_SZ-1:0]     in_flags,
  input         [`TXFMA_SIGS_SZ-1:0]    sigs,
  input         [`TXFMA_EXP_PS_SZ-1:0]  e_res,
  input         [`TXFMA_A0:`TXFMA_B30]  rnd_frac_res,
  input                                 s_res,
  input                                 res_uint_nan,  
  input                                 res_inf,
  input                                 res_qnan,
  input                                 res_snan,
  input                                 res_pos_int_nan,
  input                                 res_neg_int_nan,
  input         [`FP_CLASS_BITS-1:0]    classify,
  input                                 res_unorm_one_in,
  input                                 res_snorm_one_in,  
  input                                 flags_en,
  input                                 spec_res_en,
  input                                 sel_gmaxu24,
  input                                 sel_gmaxu16,
  input                                 sel_gmaxu10,
  input                                 sel_gmaxu8,
  input                                 sel_gmaxu2,
  input                                 sel_gminany,
  input                                 sel_gmaxs16,
  input                                 sel_gmins16,
  input                                 sel_gmaxs8,
  input                                 sel_gmins8,
  output logic  [`FLOAT32_W-1:0]        fma_res,
  output logic  [`VPU_FLAGS_SZ-1:0]     out_flags
);

logic  [`FLOAT16_W-1:0]       fma_res_hi;
logic  [`FLOAT32_W-1:0]       norm_max_out;
logic  [`FLOAT32_W-1:0]       norm_min_out;
logic                         res_denorm_hi;
logic res_inf_out_hi;
logic ovf_hi;
logic unf_hi;
logic res_nan_hi_mux;
logic res_pos_int_nan_out_hi;
logic res_neg_int_nan_out_hi;
logic res_uint_nan_out_hi;
logic [`FP_CLASS_BITS-1:0] classify_mux;
logic res_uint_nan_mux;  
logic res_inf_mux;
logic res_qnan_mux;
logic res_snan_mux;
logic res_trans_snan;
logic res_pos_int_nan_mux;
logic res_neg_int_nan_mux;
logic res_unorm_one_in_mux;
logic res_snorm_one_in_mux;  
logic sat_to_zero; 
logic [31:0] snorm_mask;
logic trans_op;
logic res_zero_mux;
logic res_one_mux;


// saturate to zero in case of negative number can't be respresented in output format
assign sat_to_zero = s_res & ((cmd == `VPU_FCMD_CVT_F10F32) | (cmd == `VPU_FCMD_CVT_F11F32));

// masks when generating norm output
always_comb begin
  snorm_mask = 32'hffffffff;

  if (cmd == `VPU_FCMD_CVT_SN8F32)
    snorm_mask = 32'h000000ff;

  if (cmd == `VPU_FCMD_CVT_SN16F32)
    snorm_mask = 32'h0000ffff;
end


// generating min/max for norms
always_comb begin

  norm_max_out = '0;
  norm_min_out = '0;


  if (sel_gmaxu24) 
      norm_max_out = `UNORM_MAX_OUT(24);
  if (sel_gmaxu16)
      norm_max_out = `UNORM_MAX_OUT(16);
  if (sel_gmaxu10)
      norm_max_out = `UNORM_MAX_OUT(10);
  if (sel_gmaxu8)
      norm_max_out = `UNORM_MAX_OUT(8);
  if (sel_gmaxu2)
      norm_max_out = `UNORM_MAX_OUT(2);
  if (sel_gmaxs16)
      norm_max_out = `SNORM_MAX_OUT(16);
  if (sel_gmaxs8)
      norm_max_out = `SNORM_MAX_OUT(8);

  if (sel_gminany)
      norm_min_out = `UNORM_MIN_OUT;
  if (sel_gmins16)
      norm_min_out = `SNORM_MIN_OUT(16);
  if (sel_gmins8)
      norm_min_out = `SNORM_MIN_OUT(8);

end


  //gen special special
always_comb begin
  classify_mux        = '0;
  res_uint_nan_mux    = 1'b0;  
  res_inf_mux         = 1'b0;
  res_qnan_mux        = 1'b0;
  res_snan_mux        = 1'b0;
  res_pos_int_nan_mux = 1'b0;
  res_neg_int_nan_mux = 1'b0;
  res_unorm_one_in_mux   = 1'b0;
  res_snorm_one_in_mux   = 1'b0;  
  res_zero_mux        = 1'b0;
  res_one_mux         = 1'b0;

  if (spec_res_en) begin
    classify_mux         = classify;
    res_uint_nan_mux     = res_uint_nan;  
    res_inf_mux          = res_inf;
    res_qnan_mux         = res_qnan;
    res_snan_mux         = res_snan;
    res_pos_int_nan_mux  = res_pos_int_nan;
    res_neg_int_nan_mux  = res_neg_int_nan;
    res_unorm_one_in_mux = res_unorm_one_in;
    res_snorm_one_in_mux = res_snorm_one_in; 
    res_zero_mux         = res_zero; 
    res_one_mux          = res_one;
  end
end



assign res_nan_hi_mux     = res_qnan_mux | res_snan_mux;

  //trans instructions need to pass snan
assign res_trans_snan = res_snan_mux & ((cmd==`VPU_TRANS_LOG_RR) | (cmd==`VPU_TRANS_LOG_RR) | (cmd==`VPU_TRANS_LOG_FMA1) | (cmd==`VPU_TRANS_EXP_FRAC) |
                                        (cmd==`VPU_TRANS_SIN_TRANSFORM) | (cmd==`VPU_TRANS_SIN_RR) | (cmd==`VPU_TRANS_SIN_P1) | (cmd==`VPU_TRANS_SIN_P2) | 
                                        (cmd==`VPU_TRANS_EXP_RR) | (cmd==`VPU_TRANS_EXP_FMA1) | (cmd==`VPU_TRANS_RCP_FMA1) | (cmd==`VPU_TRANS_RSQRT_FMA1) |
                                        (cmd==`VPU_FCMD_FFRC_SIN));
                                           

always_comb begin

// default is integer
  fma_res_hi = '0;
  res_inf_out_hi = 0;
  out_flags   = '0;
  res_denorm_hi  = 1'b0;

    //inexact flag clear for trans ops
  trans_op = (cmd==`VPU_TRANS_SIN_P3) | (cmd==`VPU_TRANS_EXP_FMA2) | (cmd==`VPU_FCMD_NR2_FRCPFXP) | (cmd==`VPU_TRANS_LOG_MUL) | (cmd==`VPU_TRANS_RSQRT_FMA2) | (cmd==`VPU_TRANS_RCP_FMA2) | (cmd == `VPU_FCMD_FFRC) | (cmd == `VPU_FCMD_FFRC_SIN);

  if (flags_en) begin
    out_flags = in_flags;
  end

    //inexact flag
  out_flags[`VPU_FLAG_INEXACT] = sticky_until_l & ~((op_dtype == `VPU_DTYPE_INT) & sigs[`TXFMA_MUL]);

    //convert output data  
  if(sigs[`TXFMA_CVT])
    fma_res = {s_res, e_res, rnd_frac_res[`TXFMA_B0:`TXFMA_B22]};
  else begin
      //integer trans ops
    if (cmd==`VPU_TRANS_LOG_FMA2)
      fma_res = rnd_frac_res[`TXFMA_B6:`TXFMA_B30];
    else if (cmd==`VPU_FCMD_NR1_FRCPFXP)
      fma_res = {rnd_frac_res[`TXFMA_B0:`TXFMA_B30],1'b0};
      //integer and trans fma frac
    else
      fma_res = rnd_frac_res[`TXFMA_A0:`TXFMA_B30]; 

      //EXP/SIN: pass denormal operand to next u-instructions     
    if (cmd == `VPU_TRANS_EXP_FMA1 || (cmd == `VPU_TRANS_SIN_RR)) begin
      if (out_flags[`VPU_FLAG_DENORMAL])
        fma_res = 32'b1;
    end

      //result is 1 (f32)    
    if (cmd == `VPU_TRANS_EXP_FMA2 && res_one_mux)
      fma_res = 32'h3f800000;
    
      //trans invalid flag
    if (trans_op)
      out_flags[`VPU_FLAG_INVALID] = (out_flags[`VPU_FLAG_INVALID] | res_snan_mux); 
  end


    // F32 + TENSOR.PS + CVT + GCVT

  if ((op_dtype == `VPU_DTYPE_F32) | (op_dtype == `VPU_DTYPE_F16_F32) | sigs[`TXFMA_NORM_F32] | ((op_dtype != `VPU_DTYPE_F32) & sigs[`TXFMA_F32_NORM]) | sigs[`TXFMA_TRANS2] | sigs[`TXFMA_F32_FX]) begin
    

    ////output flags////

    res_pos_int_nan_out_hi = (res_pos_int_nan_mux | cvt_fi_pos_ovf_hi) & (cmd == `VPU_FCMD_CVT_INTF32 | cmd == `VPU_FCMD_CVT_RASTF32);
    res_neg_int_nan_out_hi = (res_neg_int_nan_mux | cvt_fi_min_neg_hi) & (cmd == `VPU_FCMD_CVT_INTF32 | cmd == `VPU_FCMD_CVT_RASTF32);
    res_uint_nan_out_hi = (res_uint_nan_mux | cvt_fui_pos_ovf_hi) & (cmd == `VPU_FCMD_CVT_UINTF32);     
    ovf_hi = (def_ovf | pot_ovf & rnd_ovf_hi)  & ~(comp_op  | minmax_op | sigs[`TXFMA_F32_NORM] | (cmd == `VPU_TRANS_LOG_MUL) | sigs[`TXFMA_TRANS2] | (cmd == `VPU_TRANS_SIN_TRANSFORM) | ((cmd == `VPU_FCMD_FFRC) || (cmd == `VPU_FCMD_FFRC_SIN)));
    unf_hi = (def_unf | pot_unf & ~rnd_ovf_hi) & ~(comp_op  | minmax_op | sigs[`TXFMA_F32_NORM] | (cmd == `VPU_TRANS_LOG_MUL) | sigs[`TXFMA_TRANS2] | (cmd == `VPU_TRANS_SIN_TRANSFORM) | ((cmd == `VPU_FCMD_FFRC) || (cmd == `VPU_FCMD_FFRC_SIN)));
    res_inf_out_hi = (res_inf_mux | ovf_hi & rmode) & ~comp_op & ~(cmd == `VPU_FCMD_FFRC);

    out_flags[`VPU_FLAG_OVF]     |= ovf_hi & ~(out_flags[`VPU_FLAG_DIV0] | sigs[`TXFMA_TRANS2] | res_inf_mux | res_nan_hi_mux | sigs[`TXFMA_CVT] | sigs[`TXFMA_NORM_F32] | sigs[`TXFMA_F32_NORM] | sigs[`TXFMA_F32_FX]);
    out_flags[`VPU_FLAG_UNF]     |= unf_hi & ~(out_flags[`VPU_FLAG_DIV0] | sigs[`TXFMA_TRANS2] | res_inf_mux | res_nan_hi_mux | (frac_zero_hi & !out_flags[`VPU_FLAG_INEXACT]) | sigs[`TXFMA_CVT] | sigs[`TXFMA_NORM_F32] | sigs[`TXFMA_F32_NORM] | sigs[`TXFMA_F32_FX] | (op_dtype == `VPU_DTYPE_F16_F32));
    out_flags[`VPU_FLAG_INVALID] = (out_flags[`VPU_FLAG_INVALID] & ~(comp_op | sigs[`TXFMA_NORM_F32] | sigs[`TXFMA_F32_NORM])) | cmp_invalid;
    out_flags[`VPU_FLAG_INEXACT] = sticky_until_l & ~( out_flags[`VPU_FLAG_DIV0] | sigs[`TXFMA_TRANS2] | res_inf_mux | minmax_op | res_nan_hi_mux | 
                                   comp_op | sigs[`TXFMA_NORM_F32] | sigs[`TXFMA_F32_NORM]) | (ovf_hi & ~res_inf_mux & ~res_nan_hi_mux);

      //cvt
    if (sigs[`TXFMA_CVT]) begin
      out_flags[`VPU_FLAG_INVALID] = ((res_pos_int_nan_out_hi | cvt_fi_neg_ovf_hi | res_neg_int_nan_mux) & ((cmd == `VPU_FCMD_CVT_INTF32) | (cmd == `VPU_FCMD_CVT_RASTF32))) | ((res_uint_nan_out_hi | res_zero_mux & ~(ediff_lt_31_hi & ~rnd_frac_res[`TXFMA_INT_L])) & ~ec_zero_hi & (cmd == `VPU_FCMD_CVT_UINTF32)) | ((cmd==`VPU_FCMD_FROUND) & res_snan_mux);
      out_flags[`VPU_FLAG_INEXACT] &= !out_flags[`VPU_FLAG_INVALID];      
    end

      //exp frac: - generate invalid flag when result is denorm, zero, infinity or qnan
      //          - propagate denormal 
    if (cmd == `VPU_TRANS_EXP_FRAC) begin 
      res_denorm_hi = out_flags[`VPU_FLAG_DENORMAL];
      out_flags = '0;
      out_flags[`VPU_FLAG_INVALID] = res_zero_mux | res_inf_mux | res_qnan_mux | res_one_mux | res_denorm_hi | res_snan;
    end
      //ffrc sin: propagate denormal
    if ((cmd == `VPU_TRANS_SIN_TRANSFORM) || (cmd==`VPU_TRANS_SIN_P1) || (cmd==`VPU_TRANS_SIN_P2) || (cmd==`VPU_FCMD_FFRC_SIN)) begin 
      res_denorm_hi = out_flags[`VPU_FLAG_DENORMAL];
    end


    ////output data////

      //convert
    if (sigs[`TXFMA_CVT] && ~(cmd==`VPU_FCMD_FROUND) && ~((cmd == `VPU_FCMD_FFRC) || (cmd == `VPU_FCMD_FFRC_SIN))) begin
      fma_res = rnd_frac_res[`TXFMA_A0:`TXFMA_B30];
      if (res_neg_int_nan_out_hi)
        fma_res = `TXFMA_NEG_INT_NAN;
      if (res_pos_int_nan_out_hi)
        fma_res = `TXFMA_POS_INT_NAN;
      if (res_uint_nan_out_hi)
        fma_res = `TXFMA_POS_UINT_NAN;
      if (res_zero_mux)
        fma_res = 32'h0;
    end
      //norm to f32
    else if (sigs[`TXFMA_F32_NORM] & (classify_mux[`FP_CLASS_POS_ZERO] | res_unorm_one_in_mux | res_snorm_one_in_mux)) begin
      out_flags[`VPU_FLAG_INEXACT] = 1'b0;
      if (classify_mux[`FP_CLASS_POS_ZERO])
        fma_res = 32'h0;
      else if (res_unorm_one_in_mux)
        fma_res = {1'b0, 1'b0, `FLOAT32_E_BIAS_HEX, `FLOAT32_M'h0};
      else if (res_snorm_one_in_mux)
        fma_res = {s_res, 1'b0, `FLOAT32_E_BIAS_HEX, `FLOAT32_M'h0};
    end
      //f32 to norm
    else if (sigs[`TXFMA_NORM_F32]) begin
      if (res_nan_hi_mux)
        fma_res = norm_max_out;
      else if (res_inf_out_hi | norm_f32_ovf | norm_one_out)
        fma_res = s_res ? norm_min_out : norm_max_out;
      else
        fma_res = rnd_frac_res[`TXFMA_A0:`TXFMA_B30] & snorm_mask;
    end
      //result is zero
    else if (res_zero_mux)
      fma_res = {s_res,31'h0};
      //result is 1 (f32)
    else if (res_one_mux)
      fma_res = {s_res,31'h3f800000};
      //trans need to pass snan until last u-instruction
    else if (res_trans_snan)
      fma_res = {s_res,31'h7f800001};
      //res is nan
    else if (res_nan_hi_mux)
      fma_res = `FLOAT32_POS_CQNAN; // canonical positive qNaN
      //res is inf
    else if (res_inf_out_hi)
      fma_res = {s_res, `FLOAT32_INF}; // inf
      //trans log transform
    else if (cmd == `VPU_TRANS_LOG_RR)
      fma_res = {9'b0,rnd_frac_res[`TXFMA_B0:`TXFMA_B22]};
      //overflow, res is max finite number    
    else if (ovf_hi) begin
      fma_res = {s_res, `FLOAT32_MAX_NUM}; // maximum finite number
      out_flags[`VPU_FLAG_INEXACT] = 1;
    end
      //classify result
    else if (cmd == `VPU_FCMD_CLASS)
      fma_res = {(`FLOAT32_W-`FP_CLASS_BITS)'(0), classify_mux};
      //pass denormal result to EXP/SIN trans u-instructions
    else if (res_denorm_hi)
        fma_res = 32'h1;  
      //exp frac
    else if (cmd == `VPU_TRANS_EXP_FRAC) begin
      fma_res = rnd_frac_res[`TXFMA_A0:`TXFMA_B30];
      //sin p2
    end else if ((cmd == `VPU_TRANS_SIN_P2) && (op_dtype == `VPU_DTYPE_INT))
      fma_res = rnd_frac_res[`TXFMA_A0:`TXFMA_B22];
      //underflow
    else if (unf_hi) begin
      out_flags[`VPU_FLAG_INEXACT] = (res_zero_mux | frac_zero_hi) ? out_flags[`VPU_FLAG_INEXACT] : 1'b1;
      if (sigs[`TXFMA_FP32_ADD_SUB] & (rm == RDN) & eff_sub1 & !ec_zero_hi & frac_zero_hi)
        fma_res = {1'b1, 31'h0};
      else 
        fma_res = {s_res, 31'h0};
    end 
      //overflow    
    else if (rnd_ovf_hi) begin
      if ((e_res == 8'h0) & sigs[`TXFMA_FP32_ADD_SUB] & (rm == RDN) & eff_sub1 & !ec_zero_hi)
        fma_res = {1'b1, e_res, rnd_frac_res[`TXFMA_A0:`TXFMA_B21]};        
      else
        fma_res = {s_res, e_res, rnd_frac_res[`TXFMA_A0:`TXFMA_B21]};                
    end 
      //normal  
    else begin
      if ((e_res == 8'h0) & sigs[`TXFMA_FP32_ADD_SUB] & (rm == RDN) & eff_sub1 & !ec_zero_hi)
        fma_res = {1'b1, e_res, rnd_frac_res[`TXFMA_B0:`TXFMA_B22]};
      else
        fma_res = {s_res, e_res, rnd_frac_res[`TXFMA_B0:`TXFMA_B22]};        
    end       
  end

    // float to float
  if (sigs[`TXFMA_FX_F32]) begin
    ovf_hi = (def_ovf | pot_ovf & rnd_ovf_hi);
    unf_hi = (def_unf | pot_unf & ~rnd_ovf_hi);    
    res_inf_out_hi = (res_inf_mux | ovf_hi & rmode);

    if (res_nan_hi_mux) begin
      fma_res_hi = `FLOAT16_POS_CQNAN; // canonical positive qNaN
      if (cmd == `VPU_FCMD_CVT_F11F32)
        fma_res_hi = `FLOAT11_POS_CQNAN;
      if (cmd == `VPU_FCMD_CVT_F10F32)
        fma_res_hi = `FLOAT10_POS_CQNAN;
    end else if (sat_to_zero) begin
      fma_res_hi = '0;
    end else if (res_inf_out_hi) begin
      fma_res_hi = {s_res, `FLOAT16_INF}; // inf
      if (sigs[`TXFMA_FX_F32] && ~a_inf) 
        out_flags[`VPU_FLAG_INEXACT] = 1'b1;
      if (cmd == `VPU_FCMD_CVT_F11F32)
        fma_res_hi = `FLOAT11_INF;
      if (cmd == `VPU_FCMD_CVT_F10F32)
        fma_res_hi = `FLOAT10_INF;
    end else if (ovf_hi) begin
      fma_res_hi = {s_res, `FLOAT16_MAX_NUM}; // maximum finite number
      if (cmd == `VPU_FCMD_CVT_F11F32)
        fma_res_hi = `FLOAT11_MAX_NUM;
      if (cmd == `VPU_FCMD_CVT_F10F32)
        fma_res_hi = `FLOAT10_MAX_NUM;
      out_flags[`VPU_FLAG_INEXACT] = 1'b1;
    end else if (unf_hi) begin
      fma_res_hi = {s_res, 15'h0};
      if ((cmd == `VPU_FCMD_CVT_F11F32) | (cmd == `VPU_FCMD_CVT_F10F32))
        fma_res_hi = '0;
    end else if (rnd_ovf_hi) begin
      fma_res_hi = {s_res, {e_res[7], e_res[3:0]}, rnd_frac_res[`TXFMA_A0:`TXFMA_B8]};    
      if (cmd == `VPU_FCMD_CVT_F11F32)
        fma_res_hi = {(`FLOAT16_W-`FLOAT11_W)'(0), {e_res[7], e_res[3:0]}, rnd_frac_res[`TXFMA_A0:`TXFMA_B4]};
      if (cmd == `VPU_FCMD_CVT_F10F32)
        fma_res_hi = {(`FLOAT16_W-`FLOAT10_W)'(0), {e_res[7], e_res[3:0]}, rnd_frac_res[`TXFMA_A0:`TXFMA_B3]};
    end else begin
      fma_res_hi = {s_res, {e_res[7], e_res[3:0]}, rnd_frac_res[`TXFMA_B0:`TXFMA_B9]};
      if (cmd == `VPU_FCMD_CVT_F11F32)
        fma_res_hi = {(`FLOAT16_W-`FLOAT11_W)'(0), {e_res[7], e_res[3:0]}, rnd_frac_res[`TXFMA_B0:`TXFMA_B5]};
      if (cmd == `VPU_FCMD_CVT_F10F32)
        fma_res_hi = {(`FLOAT16_W-`FLOAT10_W)'(0), {e_res[7], e_res[3:0]}, rnd_frac_res[`TXFMA_B0:`TXFMA_B4]};       
    end

    fma_res = {16'h0, fma_res_hi};
  end

    //clear inexact flag for trans ops, assuming trans computation results are approximations    
  out_flags[`VPU_FLAG_INEXACT] &= !trans_op;  
end

endmodule
