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

module txfma_f3_tb
(
  input logic ctrl_f2_clk,
  input logic ctrl_f3_clk,
  input logic [TXFMA_A3:TXFMA_B27] fa_f3a_h,
  input logic [TXFMA_A3:TXFMA_B27] fc_f3a_h,
  input logic [TXFMA_A3:0] prodsum_f3a_h,
  input logic [6:0] exp_res_f2a_h,
  input logic [5:0] ediff_abs_diff_hi_f2a_h,
  input logic [4:0] align2_rshamt_c3_f2a_h,
  input logic [1:0] op3p3spc_sel_f2a_h,
  input logic [2:0] op3p2c_sel_f2a_h,
  input logic [1:0] imxc_sel_f3a_h,
  input logic [1:0] ugfxrplc_sel_f2a_h,
  input logic [1:0] sgfxrplc_sel_f2a_h,
  input logic [1:0] icgfxc_sel_f2a_h,
  input logic [1:0] al1znp2c_sel_f2a_h,
  input logic [1:0] al2c_sel_f2a_h,
  input logic [1:0] al1p2c_sel_f2a_h,
  input logic al1p1c_sel_f2a_h,
  input logic op3p1c_sel_r_f2a_h,
  input logic op3p1c_sel_p_f2a_h,
  input logic op3g_sel_f2a_h,
  input logic op2_sxtto_g_f2a_h,
  input logic rm_rne_f2a_h,
  input logic rm_rmm_f2a_h,
  input logic frnd_rupnsc_f2a_h,
  input logic frnd_rdnsc_f2a_h,
  input logic frnd_t1_f2a_h,
  input logic frnd_t2_f2a_h,
  input logic al1c_zap_f2a_h,
  input logic op1c_log_mul_clr_f2a_h,
  input logic negop1c_int_logmul_clr_f2a_h,
  input logic psc_op1_clr_f2a_h,
  input logic align2_zap_f2a_h,
  input logic align2c_neg_f2a_h,
  input logic al2c3_neg_cin_ps_f3a_h,
  output logic fround_rbit_f3a_h,
  output logic prodsum_a2_f3a_h,
  output logic op2_sxtto_g_f3a_h,
  output logic [TXFMA_A3:TXFMA_B47] comb_s_f4a_h,
  output logic [TXFMA_A3:TXFMA_B47] comb_c_f4a_h,
  output logic [TXFMA_A3:TXFMA_B27] int_minmax_res_f4a_h
);

  txfma_f3 u_dut (.*);

endmodule : txfma_f3_tb
