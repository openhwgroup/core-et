// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module dll_dly_est_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  input  logic [10:0] core_ctl_i,
  input  logic [7:0]  core_rx_code_i,
  output logic [8:0]  core_sts_o,
  output logic        core_tx_write_o,
  output logic [7:0]  core_tx_code_o,

  input  logic [10:0] wrap_ctl_i,
  input  logic        wrap_rst_neigh_ni,
  output logic [8:0]  wrap_sts_o
);

  esr_pkg::esr_dll_dly_est_ctl_t core_ctl;
  esr_pkg::esr_dll_dly_est_sts_t core_sts;
  esr_pkg::esr_dll_dly_est_ctl_t wrap_ctl;
  esr_pkg::esr_dll_dly_est_sts_t wrap_sts;

  assign core_ctl = esr_pkg::esr_dll_dly_est_ctl_t'(core_ctl_i);
  assign core_sts_o = core_sts;
  assign wrap_ctl = esr_pkg::esr_dll_dly_est_ctl_t'(wrap_ctl_i);
  assign wrap_sts_o = wrap_sts;

  dll_dly_est_core u_core (
    .clk_i              (clk_i),
    .rst_ni             (rst_ni),
    .dll_dly_est_ctl_i  (core_ctl),
    .dll_dly_est_sts_o  (core_sts),
    .tx_dly_est_write_o (core_tx_write_o),
    .tx_dly_est_code_o  (core_tx_code_o),
    .rx_dly_est_code_i  (core_rx_code_i)
  );

  dll_dly_est u_wrap (
    .clk_shire_i        (clk_i),
    .rst_shire_ni       (rst_ni),
    .clk_neigh_i        (clk_i),
    .rst_neigh_ni       (rst_ni & wrap_rst_neigh_ni),
    .dll_dly_est_ctl_i  (wrap_ctl),
    .dll_dly_est_sts_o  (wrap_sts)
  );

endmodule
