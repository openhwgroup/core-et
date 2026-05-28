// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_c1_tb import vpu_defs_pkg::*;
(
  input logic ctrl_valid_f1a_h,
  input logic [TXFMA_SIGS_SZ-1:0] sigs_f1a_h,
  input logic [VPU_FCMD_SZ-1:0] cmd_in_f1a_h,
  input logic [VPU_DTYPE_SZ-1:0] op_dtype_f1a_h,
  input logic [TXFMA_EXP_PS_SZ-1:0] ea_hi_f1a_h,
  input logic fa_hi_zero_f1a_h,
  input logic sa_hi_f1a_h,
  input logic sb_hi_f1a_h,
  input logic sc_f1a_h,
  input logic sa_lo_f1a_h,
  input logic sb_lo_f1a_h,
  input logic ea_hi_zero_f1a_h,
  input logic eb_hi_zero_f1a_h,
  input logic ea_lo_zero_f1a_h,
  input logic eb_lo_zero_f1a_h,
  input logic flog_mul_prod_zero_f1a_h,
  output logic minmax_op_f1a_h,
  output logic prod_sign_hi_f1a_h,
  output logic prod_sign_lo_f1a_h,
  output logic eprod_hi_zero_f1a_h,
  output logic eprod_lo_zero_f1a_h,
  output logic norm_f32_ovf_f1a_h,
  output logic norm_one_out_f1a_h,
  output logic paddin1_sels_f1a_h,
  output logic paddin2_selc_f1a_h,
  output logic paddin2_selfcneg_f1a_h,
  output logic padd_cin_f1a_h,
  output logic sel_famask_f1a_h,
  output logic sel_fcmask_f1a_h,
  output logic fa_en_f1a_h,
  output logic [VPU_FCMD_SZ-1:0] cmd_f1a_h,
  output logic use_mul_f1a_h,
  output logic [63:0] out_chunk0_o
);

  txfma_c1 u_dut (
    .ctrl_valid_f1a_h (ctrl_valid_f1a_h),
    .sigs_f1a_h (sigs_f1a_h),
    .cmd_in_f1a_h (cmd_in_f1a_h),
    .op_dtype_f1a_h (op_dtype_f1a_h),
    .ea_hi_f1a_h (ea_hi_f1a_h),
    .fa_hi_zero_f1a_h (fa_hi_zero_f1a_h),
    .sa_hi_f1a_h (sa_hi_f1a_h),
    .sb_hi_f1a_h (sb_hi_f1a_h),
    .sc_f1a_h (sc_f1a_h),
    .sa_lo_f1a_h (sa_lo_f1a_h),
    .sb_lo_f1a_h (sb_lo_f1a_h),
    .ea_hi_zero_f1a_h (ea_hi_zero_f1a_h),
    .eb_hi_zero_f1a_h (eb_hi_zero_f1a_h),
    .ea_lo_zero_f1a_h (ea_lo_zero_f1a_h),
    .eb_lo_zero_f1a_h (eb_lo_zero_f1a_h),
    .flog_mul_prod_zero_f1a_h (flog_mul_prod_zero_f1a_h),
    .minmax_op_f1a_h (minmax_op_f1a_h),
    .prod_sign_hi_f1a_h (prod_sign_hi_f1a_h),
    .prod_sign_lo_f1a_h (prod_sign_lo_f1a_h),
    .eprod_hi_zero_f1a_h (eprod_hi_zero_f1a_h),
    .eprod_lo_zero_f1a_h (eprod_lo_zero_f1a_h),
    .norm_f32_ovf_f1a_h (norm_f32_ovf_f1a_h),
    .norm_one_out_f1a_h (norm_one_out_f1a_h),
    .paddin1_sels_f1a_h (paddin1_sels_f1a_h),
    .paddin2_selc_f1a_h (paddin2_selc_f1a_h),
    .paddin2_selfcneg_f1a_h (paddin2_selfcneg_f1a_h),
    .padd_cin_f1a_h (padd_cin_f1a_h),
    .sel_famask_f1a_h (sel_famask_f1a_h),
    .sel_fcmask_f1a_h (sel_fcmask_f1a_h),
    .fa_en_f1a_h (fa_en_f1a_h),
    .cmd_f1a_h (cmd_f1a_h),
    .use_mul_f1a_h (use_mul_f1a_h)
  );

  logic [21:0] all_outputs;
  assign all_outputs = {
    minmax_op_f1a_h,
    prod_sign_hi_f1a_h,
    prod_sign_lo_f1a_h,
    eprod_hi_zero_f1a_h,
    eprod_lo_zero_f1a_h,
    norm_f32_ovf_f1a_h,
    norm_one_out_f1a_h,
    paddin1_sels_f1a_h,
    paddin2_selc_f1a_h,
    paddin2_selfcneg_f1a_h,
    padd_cin_f1a_h,
    sel_famask_f1a_h,
    sel_fcmask_f1a_h,
    fa_en_f1a_h,
    cmd_f1a_h,
    use_mul_f1a_h
  };

  assign out_chunk0_o = {{ 42{1'b0} }, all_outputs[21:0]};

endmodule : txfma_c1_tb
