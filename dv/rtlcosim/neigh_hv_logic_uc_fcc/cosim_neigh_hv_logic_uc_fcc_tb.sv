// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_neigh_hv_logic_uc_fcc_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [neigh_hv_logic_pkg::MinPerN-1:0] uc_to_neigh_fcc_i,
  output logic orig_neigh_uc_fcc_vcfifo_push_o,
  output logic new_neigh_uc_fcc_vcfifo_push_o
);

  /* verilator lint_off UNUSEDSIGNAL */  // Common harness clock/reset are unused by this combinational OR-reduction leaf.
  logic unused_common_ports;
  assign unused_common_ports = clk_i ^ rst_ni;
  /* verilator lint_on UNUSEDSIGNAL */

  neigh_hv_logic_uc_fcc u_new (
    .uc_to_neigh_fcc_i           (uc_to_neigh_fcc_i),
    .neigh_uc_fcc_vcfifo_push_o  (new_neigh_uc_fcc_vcfifo_push_o)
  );

  neigh_hv_logic_uc_fcc_orig u_orig (
    .uc_to_neigh_fcc           (uc_to_neigh_fcc_i),
    .neigh_uc_fcc_vcfifo_push  (orig_neigh_uc_fcc_vcfifo_push_o)
  );

endmodule : cosim_neigh_hv_logic_uc_fcc_tb
