// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original dcache_store_merge_unit vs new
// minion_dcache_store_merge_unit.

`include "soc.vh"

/* verilator lint_off UNUSED */  // Cosim keeps the standard clk/reset harness ports even for this combinational leaf.
module cosim_minion_dcache_store_merge_unit_tb (
  input  logic         clk_i,
  input  logic         rst_ni,
  input  logic [4:0]   addr_i,
  input  logic [3:0]   typ_i,
  input  logic [7:0]   ps_mask_i,
  input  logic [255:0] orig_data_i,
  input  logic [255:0] store_data_i,
  input  logic         enable_lo_i,
  input  logic         enable_hi_i,
  output logic [255:0] new_merge_out_o,
  output logic [255:0] orig_merge_out_o
);
/* verilator lint_on UNUSED */

  minion_dcache_store_merge_unit u_new (
    .addr_i,
    .typ_i       (minion_dcache_pkg::dcache_type_e'(typ_i)),
    .ps_mask_i,
    .orig_data_i,
    .store_data_i,
    .enable_lo_i,
    .enable_hi_i,
    .merge_out_o (new_merge_out_o)
  );

  dcache_store_merge_unit u_orig (
    .addr       (addr_i),
    .typ        (dcache_type'(typ_i)),
    .ps_mask    (ps_mask_i),
    .orig_data  (orig_data_i),
    .store_data (store_data_i),
    .enable_lo  (enable_lo_i),
    .enable_hi  (enable_hi_i),
    .merge_out  (orig_merge_out_o)
  );

endmodule
