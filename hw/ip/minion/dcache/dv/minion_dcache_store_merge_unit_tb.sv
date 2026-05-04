// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSED */  // clk_i/rst_ni are present for sim_ctrl.h, but the DUT is combinational
module minion_dcache_store_merge_unit_tb (
  input  logic         clk_i,
  input  logic         rst_ni,
  input  logic [4:0]   addr_i,
  input  logic [3:0]   typ_i,
  input  logic [7:0]   ps_mask_i,
  input  logic [255:0] orig_data_i,
  input  logic [255:0] store_data_i,
  input  logic         enable_lo_i,
  input  logic         enable_hi_i,
  output logic [255:0] merge_out_o
);

  minion_dcache_store_merge_unit u_dut (
    .addr_i,
    .typ_i (minion_dcache_pkg::dcache_type_e'(typ_i)),
    .ps_mask_i,
    .orig_data_i,
    .store_data_i,
    .enable_lo_i,
    .enable_hi_i,
    .merge_out_o
  );

endmodule
