// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_dll_dly_est_tb #(
  parameter int unsigned Width = 8
) (
  input  logic        clk_i,
  input  logic        rst_ni,
  input  logic        reset_shire_i,
  input  logic        reset_neigh_i,
  input  logic [10:0] dll_dly_est_ctl_i,

  output logic [8:0]  orig_sts_o,
  output logic [8:0]  new_sts_o
);

  logic orig_reset_shire;
  logic orig_reset_neigh;
  logic new_rst_shire_ni;
  logic new_rst_neigh_ni;

  assign orig_reset_shire = ~rst_ni | reset_shire_i;
  assign orig_reset_neigh = ~rst_ni | reset_neigh_i;
  assign new_rst_shire_ni = rst_ni & ~reset_shire_i;
  assign new_rst_neigh_ni = rst_ni & ~reset_neigh_i;

  esr_pkg::esr_dll_dly_est_ctl_t new_ctl;
  esr_pkg::esr_dll_dly_est_sts_t new_sts;

  assign new_ctl = esr_pkg::esr_dll_dly_est_ctl_t'(dll_dly_est_ctl_i);
  assign new_sts_o = new_sts;

  dll_dly_est_orig #(
    .WIDTH(Width)
  ) u_orig (
    .clock_shire      (clk_i),
    .reset_shire      (orig_reset_shire),
    .clock_neigh      (clk_i),
    .reset_neigh      (orig_reset_neigh),
    .dll_dly_est_ctl  (dll_dly_est_ctl_i),
    .dll_dly_est_sts  (orig_sts_o)
  );

  dll_dly_est #(
    .Width(Width)
  ) u_new (
    .clk_shire_i        (clk_i),
    .rst_shire_ni       (new_rst_shire_ni),
    .clk_neigh_i        (clk_i),
    .rst_neigh_ni       (new_rst_neigh_ni),
    .dll_dly_est_ctl_i  (new_ctl),
    .dll_dly_est_sts_o  (new_sts)
  );

endmodule
