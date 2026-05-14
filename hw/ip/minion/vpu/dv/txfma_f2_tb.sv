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

module txfma_f2_tb
(
  input logic ctrl_f1_clk,
  input logic ctrl_f2_clk,
  input logic [TXFMA_A3:TXFMA_B27] fa_f2a_h,
  input logic [TXFMA_A3:TXFMA_B27] fc_f2a_h,
  input logic [VPU_DATA_S_SZ-1:0] booth_fCin_f2a_h,
  input logic [VPU_DATA_S_SZ-1:0] booth_fSxt_f2a_h,
  input logic [32:0] booth_fpp17_f2a_h,
  input logic [TXFMA_A3:0] wallace_fC21_f2a_h,
  input logic [TXFMA_A3:0] wallace_fC22_f2a_h,
  input logic [TXFMA_A3:0] wallace_fS21_f2a_h,
  input logic [TXFMA_A3:0] wallace_fS22_f2a_h,
  input logic paddin1_sels_f1a_h,
  input logic paddin2_selc_f1a_h,
  input logic paddin2_selfcneg_f1a_h,
  input logic padd_cin_f1a_h,
  input logic sel_famask_f1a_h,
  output logic [TXFMA_A3:TXFMA_B27] fa_f3a_h,
  output logic [TXFMA_A3:TXFMA_B27] fc_f3a_h,
  output logic [TXFMA_A3:TXFMA_B59] prodsum_f3a_h
);

  txfma_f2 u_dut (.*);

endmodule : txfma_f2_tb
