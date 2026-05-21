// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_uncacheable_fcc_tb
  import shire_uncached_pkg::*;
(
  input  logic                                      clk_i,
  input  logic                                      rst_ni,
  input  logic [UcFccRegisters-1:0]                 esr_enables_i,
  input  logic [shire_esr_pkg::EsrApbDataWidth-1:0] esr_wdata_i,
  output logic [(MinPerNeigh * NumNeigh)-1:0]       orig_fcc_flat_o,
  output logic [(MinPerNeigh * NumNeigh)-1:0]       new_fcc_flat_o,
  output logic [UcFccTargetSize-1:0]                orig_fcc_target_o,
  output logic [UcFccTargetSize-1:0]                new_fcc_target_o
);

  logic [NumNeigh-1:0][MinPerNeigh-1:0] orig_fcc;
  logic [NumNeigh-1:0][MinPerNeigh-1:0] new_fcc;

  uncacheable_fcc_orig u_orig (
    .fcc(orig_fcc),
    .fcc_target(orig_fcc_target_o),
    .esr_enables(esr_enables_i),
    .esr_wdata(esr_wdata_i)
  );

  uncacheable_fcc u_new (
    .fcc_o(new_fcc),
    .fcc_target_o(new_fcc_target_o),
    .esr_enables_i(esr_enables_i),
    .esr_wdata_i(esr_wdata_i)
  );

  assign orig_fcc_flat_o = orig_fcc;
  assign new_fcc_flat_o = new_fcc;

  /* verilator lint_off UNUSEDSIGNAL */
  logic unused_clock_reset;
  assign unused_clock_reset = clk_i ^ rst_ni;
  /* verilator lint_on UNUSEDSIGNAL */

endmodule : cosim_uncacheable_fcc_tb
