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

module txfma_f1_tb
(
  input logic ctrl_f0_clk,
  input logic multype_f0_clk,
  input logic ctrl_f1_clk,
  input logic multype_f1_clk,
  input logic [VPU_DATA_S_SZ-1:0] fa_f0a_h,
  input logic [VPU_DATA_S_SZ-1:0] fb_f0a_h,
  input logic [VPU_DATA_S_SZ-1:0] fc_f0a_h,
  input logic dtps_f0a_h,
  input logic mul_sign_f0a_h,
  input logic mul_hp_mode_f0a_h,
  output logic fc_b0_f2a_h,
  output logic [TXFMA_A3:TXFMA_B27] fc_f2a_h,
  output logic [TXFMA_A3:TXFMA_B27] fa_f2a_h,
  output logic [TXFMA_A3:TXFMA_B59] wallace_fC21_f2a_h,
  output logic [TXFMA_A3:TXFMA_B59] wallace_fC22_f2a_h,
  output logic [TXFMA_A3:TXFMA_B59] wallace_fS21_f2a_h,
  output logic [TXFMA_A3:TXFMA_B59] wallace_fS22_f2a_h,
  output logic [VPU_DATA_S_SZ-1:0] booth_fCin_f2a_h,
  output logic [VPU_DATA_S_SZ-1:0] booth_fSxt_f2a_h,
  output logic [32:0] booth_fpp17_f2a_h,
  output logic [3:0] etrz_fa_hi_ph_f1a_h,
  output logic [4:0] etrz_fa_lo_ps_f1a_h,
  output logic [3:0] etrz_fb_hi_ph_f1a_h,
  output logic [4:0] etrz_fb_lo_ps_f1a_h,
  output logic [4:0] etrz_fc_f1a_h
);

  txfma_f1 u_dut (.*);

endmodule : txfma_f1_tb
