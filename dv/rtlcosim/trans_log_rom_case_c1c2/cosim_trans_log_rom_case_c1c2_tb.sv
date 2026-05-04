// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_trans_log_rom_case_c1c2_tb
  import minion_pkg::*;
  import vpu_pkg::*;
(
  input  logic                                 clk_i,
  input  logic                                 rst_ni,
  input  trans_rom_index_t                     index_i,
  output logic [$bits(trans_fma_coefficients_t)-1:0] orig_data_o,
  output logic [$bits(trans_fma_coefficients_t)-1:0] new_data_o
);

  /* verilator lint_off UNUSED */  // CosimCtrl drives a dummy clock/reset even though this ROM helper is combinational.
  logic unused_ok;
  assign unused_ok = &{1'b0, clk_i, rst_ni};
  /* verilator lint_on UNUSED */

  trans_fma_coefficients_t orig_data;
  trans_fma_coefficients_t new_data;

  trans_log_rom_case_c1c2_orig u_orig (
    .index (index_i),
    .data  (orig_data)
  );

  trans_log_rom_case_c1c2 u_new (
    .index_i (index_i),
    .data_o  (new_data)
  );

  assign orig_data_o = orig_data;
  assign new_data_o = new_data;

endmodule
