// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_clear
(
  input  [`TXFMA_SIGS_SZ-1:0]           sigs,
  input  [`VPU_DTYPE_SZ-1:0]            op_dtype,
  input  [`VPU_DATA_S_SZ-1:0]           fa,
  input  [`VPU_DATA_S_SZ-1:0]           fb,
  input  [`VPU_DATA_S_SZ-1:0]           fc,  
  input                                 sa_hi,
  input                                 sb_hi,
  input                                 sc,
  input                                 sa_lo,
  input                                 sb_lo,
  input  [`TXFMA_EXP_PS_SZ-1:0]         ea_hi,
  input  [`TXFMA_EXP_PS_SZ-1:0]         eb_hi,
  input  [`TXFMA_EXP_PS_SZ-1:0]         ec,       
  input  [`TXFMA_EXP_PH_SZ-1:0]         ea_lo,         
  input  [`TXFMA_EXP_PH_SZ-1:0]         eb_lo,         
  input                                 ea_hi_zero,
  input                                 eb_hi_zero,
  input                                 ec_zero,
  input                                 ea_lo_zero,
  input                                 eb_lo_zero,
  input                                 ea_hi_all_one,
  input                                 eb_hi_all_one, 
  input                                 ec_all_one,
  input                                 ea_lo_all_one,
  input                                 eb_lo_all_one,
  output logic                          sa_hi_clr,
  output logic                          sb_hi_clr,
  output logic                          sc_clr,
  output logic   [`TXFMA_EXP_PS_SZ-1:0] ea_hi_clr,
  output logic   [`TXFMA_EXP_PS_SZ-1:0] eb_hi_clr,
  output logic   [`TXFMA_EXP_PS_SZ-1:0] ec_clr,
  output logic   [`VPU_DATA_S_SZ-1:0]   fa_clr,
  output logic   [`VPU_DATA_S_SZ-1:0]   fb_clr,
  output logic   [`VPU_DATA_S_SZ-1:0]   fc_clr,
  output logic                          ea_hi_zero_clr,
  output logic                          eb_hi_zero_clr,
  output logic                          ec_zero_clr,
  output logic                          ea_hi_all_one_clr,
  output logic                          eb_hi_all_one_clr,
  output logic                          ec_all_one_clr,
  output logic                          sa_lo_clr,
  output logic                          sb_lo_clr,
  output logic   [`TXFMA_EXP_PH_SZ-1:0] ea_lo_clr,
  output logic   [`TXFMA_EXP_PH_SZ-1:0] eb_lo_clr,
  output logic                          ea_lo_zero_clr,
  output logic                          eb_lo_zero_clr,
  output logic                          ea_lo_all_one_clr,
  output logic                          eb_lo_all_one_clr 
);

logic clear_a_b;

  //clear a and b in case of operands A or B not used
assign clear_a_b = sigs[`TXFMA_REN1] | sigs[`TXFMA_REN2] | !sigs[`TXFMA_REN3];

// first operand not used, we assume neutral value:'1' 0x3f800000: sign=0, exp=7f, frac=800000. In case we use only operand C, this operand is 0x00000000
assign sa_hi_clr = sa_hi & sigs[`TXFMA_REN1];
assign sa_lo_clr = sa_lo & sigs[`TXFMA_REN1] & op_dtype==`VPU_DTYPE_F16_F32;
assign ea_hi_zero_clr = ea_hi_zero & sigs[`TXFMA_REN1];
assign ea_lo_zero_clr = ea_lo_zero & sigs[`TXFMA_REN1] & op_dtype==`VPU_DTYPE_F16_F32;
assign ea_hi_all_one_clr = ea_hi_all_one & sigs[`TXFMA_REN1];
assign ea_lo_all_one_clr = ea_lo_all_one & sigs[`TXFMA_REN1] & op_dtype==`VPU_DTYPE_F16_F32;
assign ea_hi_clr[`TXFMA_EXP_PS_SZ-1] = ea_hi[`TXFMA_EXP_PS_SZ-1] & sigs[`TXFMA_REN1];
assign ea_lo_clr[`TXFMA_EXP_PH_SZ-1] = ea_lo[`TXFMA_EXP_PH_SZ-1] & sigs[`TXFMA_REN1] & op_dtype==`VPU_DTYPE_F16_F32;
assign ea_hi_clr[`TXFMA_EXP_PS_SZ-2:0] = (ea_hi[`TXFMA_EXP_PS_SZ-2:0] | {(`TXFMA_EXP_PS_SZ-1){!(sigs[`TXFMA_REN1])}}) & {(`TXFMA_EXP_PS_SZ-1){clear_a_b}};
assign ea_lo_clr[`TXFMA_EXP_PH_SZ-2:0] = (ea_lo[`TXFMA_EXP_PH_SZ-2:0] | {(`TXFMA_EXP_PH_SZ-1){!(sigs[`TXFMA_REN1] & op_dtype==`VPU_DTYPE_F16_F32)}}) & {(`TXFMA_EXP_PH_SZ-1){clear_a_b}};
assign fa_clr[`VPU_DATA_S_SZ-1] =   (fa[`VPU_DATA_S_SZ-1] | !sigs[`TXFMA_REN1]) & clear_a_b;
assign fa_clr[`VPU_DATA_S_SZ-2:0] = fa[`VPU_DATA_S_SZ-2:0] & {(`FLOAT32_W-1){sigs[`TXFMA_REN1]}};

// second operand not used, we assume neutral value:'1' 0x3f800000: sign=0, exp=7f, frac=800000. In case we use only operand C, this operand is 0x00000000
assign sb_hi_clr = sb_hi & sigs[`TXFMA_REN2];
assign sb_lo_clr = sb_lo & sigs[`TXFMA_REN2] & op_dtype==`VPU_DTYPE_F16_F32;
assign eb_hi_zero_clr = eb_hi_zero & sigs[`TXFMA_REN2] & ~sigs[`TXFMA_NORM_F32];
assign eb_lo_zero_clr = eb_lo_zero & sigs[`TXFMA_REN2] & op_dtype==`VPU_DTYPE_F16_F32;
assign eb_hi_all_one_clr = eb_hi_all_one & sigs[`TXFMA_REN2] & ~sigs[`TXFMA_NORM_F32];
assign eb_lo_all_one_clr = eb_lo_all_one & sigs[`TXFMA_REN2] & op_dtype==`VPU_DTYPE_F16_F32;
assign eb_hi_clr[`TXFMA_EXP_PS_SZ-1] = eb_hi[`TXFMA_EXP_PS_SZ-1] & sigs[`TXFMA_REN2];
assign eb_lo_clr[`TXFMA_EXP_PH_SZ-1] = eb_lo[`TXFMA_EXP_PH_SZ-1] & sigs[`TXFMA_REN2] & op_dtype==`VPU_DTYPE_F16_F32;
assign eb_hi_clr[`TXFMA_EXP_PS_SZ-2:0] = (eb_hi[`TXFMA_EXP_PS_SZ-2:0] | {(`TXFMA_EXP_PS_SZ-1){!sigs[`TXFMA_REN2]}}) & {(`TXFMA_EXP_PS_SZ-1){clear_a_b}};
assign eb_lo_clr[`TXFMA_EXP_PH_SZ-2:0] = (eb_lo[`TXFMA_EXP_PH_SZ-2:0] | {(`TXFMA_EXP_PH_SZ-1){!(sigs[`TXFMA_REN2] & op_dtype==`VPU_DTYPE_F16_F32)}}) & {(`TXFMA_EXP_PH_SZ-1){clear_a_b}};
assign fb_clr[`VPU_DATA_S_SZ-1] =   (fb[`VPU_DATA_S_SZ-1] | !sigs[`TXFMA_REN2]) & clear_a_b;
assign fb_clr[`VPU_DATA_S_SZ-2:0] = fb[`VPU_DATA_S_SZ-2:0] & {(`FLOAT32_W-1){sigs[`TXFMA_REN2]}};

// third operand not used, we assume neutral value:'0' 0x00000000: sign=0, exp=0, frac=000000
assign sc_clr = sc & sigs[`TXFMA_REN3];
assign ec_zero_clr = ec_zero & sigs[`TXFMA_REN3];
assign ec_all_one_clr = ec_all_one & sigs[`TXFMA_REN3];
assign ec_clr[`TXFMA_EXP_PS_SZ-1:0] = ec[`TXFMA_EXP_PS_SZ-1:0] & {`TXFMA_EXP_PS_SZ{sigs[`TXFMA_REN3]}};
assign fc_clr[`VPU_DATA_S_SZ-1:0] = fc[`VPU_DATA_S_SZ-1:0] & {`FLOAT32_W{sigs[`TXFMA_REN3]}};

endmodule

