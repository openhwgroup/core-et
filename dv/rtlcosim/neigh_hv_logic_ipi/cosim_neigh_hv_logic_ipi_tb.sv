// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_neigh_hv_logic_ipi_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [neigh_hv_logic_pkg::ThreadsPerN-1:0] esr_to_neigh_ipi_trigger_i,
  output logic orig_esr_to_neigh_ipi_trigger_push_o,
  output logic new_esr_to_neigh_ipi_trigger_push_o
);

  /* verilator lint_off UNUSEDSIGNAL */  // Common harness clock/reset are unused by this combinational OR-reduction leaf.
  logic unused_common_ports;
  assign unused_common_ports = clk_i ^ rst_ni;
  /* verilator lint_on UNUSEDSIGNAL */

  neigh_hv_logic_ipi u_new (
    .esr_to_neigh_ipi_trigger_i       (esr_to_neigh_ipi_trigger_i),
    .esr_to_neigh_ipi_trigger_push_o  (new_esr_to_neigh_ipi_trigger_push_o)
  );

  neigh_hv_logic_ipi_orig u_orig (
    .esr_to_neigh_ipi_trigger       (esr_to_neigh_ipi_trigger_i),
    .esr_to_neigh_ipi_trigger_push  (orig_esr_to_neigh_ipi_trigger_push_o)
  );

endmodule : cosim_neigh_hv_logic_ipi_tb
