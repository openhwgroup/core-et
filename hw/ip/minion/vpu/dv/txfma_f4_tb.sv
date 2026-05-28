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

module txfma_f4_tb
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
  output logic neg_norm_f5a_h,
  output logic [VPU_DATA_S_SZ-1:0] int_minmax_res_f5a_h,
  output logic [TXFMA_A3:TXFMA_B47] comb_add_f5a_h,
  output logic [TXFMA_LXD_PS_SZ-1:0] elxd_f4a_h,
  output logic [TXFMA_WSPS_ETRZ_SZ-1:0] etrz50_f4a_h
);

  txfma_f4 u_dut (.*);

endmodule : txfma_f4_tb
