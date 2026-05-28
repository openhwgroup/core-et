// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_dll_dly_est_core_tb #(
  parameter int unsigned Width      = 8,
  parameter int unsigned SyncStages = 3,
  parameter int unsigned PolyInit   = 1
) (
  input  logic        clk_i,
  input  logic        rst_ni,
  input  logic        reset_i,
  input  logic [10:0] dll_dly_est_ctl_i,
  input  logic [7:0]  rx_dly_est_code_i,

  output logic [8:0]  orig_sts_o,
  output logic [8:0]  new_sts_o,
  output logic        orig_tx_dly_est_write_o,
  output logic        new_tx_dly_est_write_o,
  output logic [7:0]  orig_tx_dly_est_code_o,
  output logic [7:0]  new_tx_dly_est_code_o
);

  logic orig_reset;
  logic new_rst_ni;
  assign orig_reset = ~rst_ni | reset_i;
  assign new_rst_ni = rst_ni & ~reset_i;

  esr_pkg::esr_dll_dly_est_ctl_t new_ctl;
  esr_pkg::esr_dll_dly_est_sts_t new_sts;

  assign new_ctl = esr_pkg::esr_dll_dly_est_ctl_t'(dll_dly_est_ctl_i);
  assign new_sts_o = new_sts;

  dll_dly_est_core_orig #(
    .WIDTH(Width),
    .SYNC_STAGES(SyncStages),
    .POLY_INIT(PolyInit)
  ) u_orig (
    .clock            (clk_i),
    .reset            (orig_reset),
    .dll_dly_est_ctl  (dll_dly_est_ctl_i),
    .dll_dly_est_sts  (orig_sts_o),
    .tx_dly_est_write (orig_tx_dly_est_write_o),
    .tx_dly_est_code  (orig_tx_dly_est_code_o),
    .rx_dly_est_code  (rx_dly_est_code_i)
  );

  dll_dly_est_core #(
    .Width(Width),
    .SyncStages(SyncStages),
    .PolyInit(PolyInit)
  ) u_new (
    .clk_i              (clk_i),
    .rst_ni             (new_rst_ni),
    .dll_dly_est_ctl_i  (new_ctl),
    .dll_dly_est_sts_o  (new_sts),
    .tx_dly_est_write_o (new_tx_dly_est_write_o),
    .tx_dly_est_code_o  (new_tx_dly_est_code_o),
    .rx_dly_est_code_i  (rx_dly_est_code_i)
  );

endmodule
