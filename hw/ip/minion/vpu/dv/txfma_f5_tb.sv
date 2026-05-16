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

module txfma_f5_tb
(
  input logic ctrl_f4_clk,
  input logic ctrl_f5_clk,
  input logic [TXFMA_A3:TXFMA_B47] comb_add_f5a_h,
  input logic [TXFMA_A3:TXFMA_B27] int_minmax_res_f5a_h,
  input logic [TXFMA_NORM_SHF_AMNT_SZ-1:0] enc_nsamt_f4a_h,
  input logic neg_norm_f5a_h,
  input logic en_logrrsel1b_f5a_l,
  input logic mmx_sel_immres_f5a_h,
  input logic nshcen_sel1b_f5a_h,
  input logic rc_ps_f5a_h,
  input logic rc_ph_f5a_h,
  input logic rc_int32_f5a_h,
  input logic int32_stky2r_f5a_h,
  input logic stky2g_f5a_h,
  input logic rc_3rmxr_f5a_h,
  input logic rc_rl01_f5a_h,
  input logic rc_rmm_f5a_h,
  input logic rc_rup_f5a_h,
  input logic rc_rne_f5a_h,
  input logic rc_rdn_f5a_h,
  input logic rc_res_sign_f5a_l,
  input logic rc_transrnd_f5a_h,
  input logic rc_transrnd_log_fma2_f5a_h,
  output logic ph_allone2r_f5a_h,
  output logic ph_allone2l_f5a_h,
  output logic ps_allone2r_f5a_h,
  output logic ps_allone2l_f5a_h,
  output logic nshc_f5a_h,
  output logic ph_stky2l_f5a_h,
  output logic ps_stky2l_f5a_h,
  output logic int32_stky2l_f5a_h,
  output logic comb_add_a3_f5a_h,
  output logic frac_zero_f6a_h,
  output logic rnd_l_ph_f6a_h,
  output logic rnd_r_ph_f6a_h,
  output logic rnd_l_ps_f6a_h,
  output logic rnd_r_ps_f6a_h,
  output logic rnd_l_int_f6a_h,
  output logic rnd_r_int_f6a_h,
  output logic [TXFMA_FTZ_SZ-1:0] ftz_f6a_l,
  output logic [TXFMA_A0:TXFMA_B31] mergd_frac_res_hi_f6a_h
);

  txfma_f5 u_dut (.*);

endmodule : txfma_f5_tb
