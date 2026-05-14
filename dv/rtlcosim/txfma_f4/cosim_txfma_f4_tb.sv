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

module cosim_txfma_f4_tb
(
  input logic ctrl_f3_clk,
  input logic ctrl_f4_clk,
  input logic [TXFMA_A3:TXFMA_B47] comb_s_f4a_h,
  input logic [TXFMA_A3:TXFMA_B47] comb_c_f4a_h,
  input logic [VPU_DATA_S_SZ-1:0] int_minmax_res_f4a_h,
  input logic neg_norm_a3_f4a_h,
  input logic neg_norm_na3_f4a_h,
  input logic clearphps_en_f4a_h,
  input logic clear16frnd_nshin_f4a_l,
  input logic ph_ps_clear_f4a_l,
  input logic al1c3_neg_cin_f3a_h,
  input logic fma_subopc3_f3a_h,
  input logic op2_sxtto_g_f3a_h,
  output logic orig_neg_norm_f5a_h,
  output logic new_neg_norm_f5a_h,
  output logic [VPU_DATA_S_SZ-1:0] orig_int_minmax_res_f5a_h,
  output logic [VPU_DATA_S_SZ-1:0] new_int_minmax_res_f5a_h,
  output logic [TXFMA_A3:TXFMA_B47] orig_comb_add_f5a_h,
  output logic [TXFMA_A3:TXFMA_B47] new_comb_add_f5a_h,
  output logic [TXFMA_LXD_PS_SZ-1:0] orig_elxd_f4a_h,
  output logic [TXFMA_LXD_PS_SZ-1:0] new_elxd_f4a_h,
  output logic [TXFMA_WSPS_ETRZ_SZ-1:0] orig_etrz50_f4a_h,
  output logic [TXFMA_WSPS_ETRZ_SZ-1:0] new_etrz50_f4a_h
);

  txfma_f4_orig u_orig (
    .ctrl_f3_clk (ctrl_f3_clk),
    .ctrl_f4_clk (ctrl_f4_clk),
    .comb_s_f4a_h (comb_s_f4a_h),
    .comb_c_f4a_h (comb_c_f4a_h),
    .int_minmax_res_f4a_h (int_minmax_res_f4a_h),
    .neg_norm_a3_f4a_h (neg_norm_a3_f4a_h),
    .neg_norm_na3_f4a_h (neg_norm_na3_f4a_h),
    .clearphps_en_f4a_h (clearphps_en_f4a_h),
    .clear16frnd_nshin_f4a_l (clear16frnd_nshin_f4a_l),
    .ph_ps_clear_f4a_l (ph_ps_clear_f4a_l),
    .al1c3_neg_cin_f3a_h (al1c3_neg_cin_f3a_h),
    .fma_subopc3_f3a_h (fma_subopc3_f3a_h),
    .op2_sxtto_g_f3a_h (op2_sxtto_g_f3a_h),
    .neg_norm_f5a_h (orig_neg_norm_f5a_h),
    .int_minmax_res_f5a_h (orig_int_minmax_res_f5a_h),
    .comb_add_f5a_h (orig_comb_add_f5a_h),
    .elxd_f4a_h (orig_elxd_f4a_h),
    .etrz50_f4a_h (orig_etrz50_f4a_h)
  );

  txfma_f4 u_new (
    .ctrl_f3_clk (ctrl_f3_clk),
    .ctrl_f4_clk (ctrl_f4_clk),
    .comb_s_f4a_h (comb_s_f4a_h),
    .comb_c_f4a_h (comb_c_f4a_h),
    .int_minmax_res_f4a_h (int_minmax_res_f4a_h),
    .neg_norm_a3_f4a_h (neg_norm_a3_f4a_h),
    .neg_norm_na3_f4a_h (neg_norm_na3_f4a_h),
    .clearphps_en_f4a_h (clearphps_en_f4a_h),
    .clear16frnd_nshin_f4a_l (clear16frnd_nshin_f4a_l),
    .ph_ps_clear_f4a_l (ph_ps_clear_f4a_l),
    .al1c3_neg_cin_f3a_h (al1c3_neg_cin_f3a_h),
    .fma_subopc3_f3a_h (fma_subopc3_f3a_h),
    .op2_sxtto_g_f3a_h (op2_sxtto_g_f3a_h),
    .neg_norm_f5a_h (new_neg_norm_f5a_h),
    .int_minmax_res_f5a_h (new_int_minmax_res_f5a_h),
    .comb_add_f5a_h (new_comb_add_f5a_h),
    .elxd_f4a_h (new_elxd_f4a_h),
    .etrz50_f4a_h (new_etrz50_f4a_h)
  );

endmodule : cosim_txfma_f4_tb
