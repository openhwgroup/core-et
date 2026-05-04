// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_c1 
(
  input  [`TXFMA_SIGS_SZ-1:0]         sigs_f1a_h,
  input  [`VPU_FCMD_SZ-1:0]           cmd_f1a_h,  
  input  [`VPU_DTYPE_SZ-1:0]          op_dtype_f1a_h,
  input  [`TXFMA_EXP_PS_SZ-1:0]       ea_hi_f1a_h,
  input                               fa_hi_zero_f1a_h,
  input                               fb_hi_zero_f1a_h,
  input                               sa_hi_f1a_h,
  input                               sb_hi_f1a_h,
  input                               sa_lo_f1a_h,
  input                               sb_lo_f1a_h,
  input                               ea_hi_zero_f1a_h,
  input                               eb_hi_zero_f1a_h, 
  input                               ea_lo_zero_f1a_h,
  input                               eb_lo_zero_f1a_h,
  input                               trans_taylor,   
  output                              mul_sign_f1a_h,  
  output                              prod_sign_hi_f1a_h,
  output                              prod_sign_lo_f1a_h,
  output logic                        eprod_hi_zero_f1a_h,
  output                              eprod_lo_zero_f1a_h,
  output logic                        norm_f32_ovf_f1a_h,
  output logic                        norm_one_out_f1a_h,
  output logic                        fa_en_f1a_h
);

logic trans_mul_sign_f1a_h;




assign trans_mul_sign_f1a_h = sigs_f1a_h[`TXFMA_TRANS2] & (cmd_f1a_h!=`VPU_TRANS_EXP_FMA2) | (cmd_f1a_h==`VPU_TRANS_LOG_FMA2) | ((cmd_f1a_h==`VPU_TRANS_SIN_P2) & !trans_taylor);


  // multiplication sign operation
assign mul_sign_f1a_h = ((cmd_f1a_h==`VPU_FCMD_MULL || cmd_f1a_h==`VPU_FCMD_MULH || trans_mul_sign_f1a_h || cmd_f1a_h==`VPU_FCMD_NR2_FRCPFXP) && op_dtype_f1a_h==`VPU_DTYPE_INT);

  // product sign
assign prod_sign_hi_f1a_h = (sa_hi_f1a_h ^ (sb_hi_f1a_h & sigs_f1a_h[`TXFMA_REN2])) | (cmd_f1a_h==`VPU_TRANS_EXP_FRAC);
assign prod_sign_lo_f1a_h = sa_lo_f1a_h ^ sb_lo_f1a_h;

  // product zero detection
always_comb begin  
  eprod_hi_zero_f1a_h = (ea_hi_zero_f1a_h | eb_hi_zero_f1a_h);

  if (cmd_f1a_h == `VPU_TRANS_LOG_MUL) 
    eprod_hi_zero_f1a_h = (fa_hi_zero_f1a_h | fb_hi_zero_f1a_h);
end

assign eprod_lo_zero_f1a_h = ea_lo_zero_f1a_h | eb_lo_zero_f1a_h;

assign norm_f32_ovf_f1a_h = ~fa_hi_zero_f1a_h & (ea_hi_f1a_h == `FLOAT32_E_BIAS) | (ea_hi_f1a_h > `FLOAT32_E_BIAS);
assign norm_one_out_f1a_h = fa_hi_zero_f1a_h & (ea_hi_f1a_h == `FLOAT32_E_BIAS);


assign fa_en_f1a_h = (op_dtype_f1a_h!=`VPU_DTYPE_F16_F32) | 
                     !((op_dtype_f1a_h==`VPU_DTYPE_F16_F32) & (sigs_f1a_h[`TXFMA_FMA] | sigs_f1a_h[`TXFMA_MUL]));

endmodule 
