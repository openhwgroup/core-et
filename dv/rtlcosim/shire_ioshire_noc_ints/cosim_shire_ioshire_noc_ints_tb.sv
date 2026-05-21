// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_shire_ioshire_noc_ints_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [18:0] noc_err_int_srcs_i,
  output logic [18:0] orig_noc_all_err_int_srcs_o,
  output logic [18:0] new_noc_all_err_int_srcs_o,
  output logic        orig_ioshire_noc_err_int_o,
  output logic        new_ioshire_noc_err_int_o
);
  logic [20:0] orig_all;

  assign orig_noc_all_err_int_srcs_o = orig_all[18:0];

  shire_ioshire_noc_ints_orig u_orig (
    .noc_clock                (clk_i),
    .reset_noc                (!rst_ni),
    .noc_err_int_srcs_lo      (noc_err_int_srcs_i),
    .noc_dbg_err_int_srcs_lo  ('0),
    .noc_all_err_int_srcs_lo  (orig_all),
    .ioshire_noc_err_int      (orig_ioshire_noc_err_int_o)
  );

  shire_ioshire_noc_ints u_new (
    .clk_noc_i              (clk_i),
    .rst_noc_ni             (rst_ni),
    .noc_err_int_srcs_i     (noc_err_int_srcs_i),
    .noc_all_err_int_srcs_o (new_noc_all_err_int_srcs_o),
    .ioshire_noc_err_int_o  (new_ioshire_noc_err_int_o)
  );

endmodule
