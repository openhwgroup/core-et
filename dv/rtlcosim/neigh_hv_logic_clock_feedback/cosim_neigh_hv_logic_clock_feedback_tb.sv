// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_neigh_hv_logic_clock_feedback_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic clock_shire_i,
  input  logic clock_neigh_i,
  output logic orig_dll_feedback_shire_o,
  output logic new_dll_feedback_shire_o,
  output logic orig_dll_feedback_neigh_o,
  output logic new_dll_feedback_neigh_o
);

  /* verilator lint_off UNUSEDSIGNAL */  // Common harness clock/reset are unused by this combinational feedback leaf.
  logic unused_common_ports;
  assign unused_common_ports = clk_i ^ rst_ni;
  /* verilator lint_on UNUSEDSIGNAL */

  neigh_hv_logic_clock_feedback u_new (
    .clk_shire_i           (clock_shire_i),
    .clk_neigh_i           (clock_neigh_i),
    .dll_feedback_shire_o  (new_dll_feedback_shire_o),
    .dll_feedback_neigh_o  (new_dll_feedback_neigh_o)
  );

  neigh_hv_logic_clock_feedback_orig u_orig (
    .clock_shire           (clock_shire_i),
    .clock_neigh           (clock_neigh_i),
    .dll_feedback_shire    (orig_dll_feedback_shire_o),
    .dll_feedback_neigh    (orig_dll_feedback_neigh_o)
  );

endmodule : cosim_neigh_hv_logic_clock_feedback_tb
