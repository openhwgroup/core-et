// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_neigh_hv_logic_constants_tb (
  input  logic clk_i,
  input  logic rst_ni,
  output logic orig_ct_one_o,
  output logic new_ct_one_o
);

  /* verilator lint_off UNUSEDSIGNAL */  // Common harness clock/reset are unused; original leaf has no inputs.
  logic unused_common_ports;
  assign unused_common_ports = clk_i ^ rst_ni;
  /* verilator lint_on UNUSEDSIGNAL */

  neigh_hv_logic_constants u_new (
    .ct_one_o (new_ct_one_o)
  );

  neigh_hv_logic_constants_orig u_orig (
    .ct_one (orig_ct_one_o)
  );

endmodule : cosim_neigh_hv_logic_constants_tb
