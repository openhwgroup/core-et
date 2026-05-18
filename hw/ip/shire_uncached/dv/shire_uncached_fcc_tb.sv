// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module shire_uncached_fcc_tb
  import shire_uncached_pkg::*;
(
  input  logic                                           clk_i,
  input  logic                                           rst_ni,
  input  logic [UcFccRegisters-1:0]                      esr_enables_i,
  input  logic [shire_esr_pkg::EsrApbDataWidth-1:0]      esr_wdata_i,
  output logic [(MinPerNeigh * NumNeigh)-1:0]            fcc_flat_o,
  output logic [UcFccTargetSize-1:0]                     fcc_target_o
);

  logic [NumNeigh-1:0][MinPerNeigh-1:0] fcc;

  uncacheable_fcc u_dut (
    .fcc_o(fcc),
    .fcc_target_o(fcc_target_o),
    .esr_enables_i(esr_enables_i),
    .esr_wdata_i(esr_wdata_i)
  );

  assign fcc_flat_o = fcc;

  /* verilator lint_off UNUSEDSIGNAL */
  logic unused_clock_reset;
  assign unused_clock_reset = clk_i ^ rst_ni;
  /* verilator lint_on UNUSEDSIGNAL */

endmodule : shire_uncached_fcc_tb
