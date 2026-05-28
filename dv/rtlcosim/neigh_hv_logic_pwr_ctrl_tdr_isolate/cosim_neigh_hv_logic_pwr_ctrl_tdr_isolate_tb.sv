// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_neigh_hv_logic_pwr_ctrl_tdr_isolate_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic dft_tdr_ctrl_i,
  input  logic pwr_ctrl_glb_isolate_i,
  output logic orig_pwr_ctrl_glb_isolate_o,
  output logic new_pwr_ctrl_glb_isolate_o
);

  /* verilator lint_off UNUSEDSIGNAL */  // Common harness clock/reset are unused by this combinational DFT mask leaf.
  logic unused_common_ports;
  assign unused_common_ports = clk_i ^ rst_ni;
  /* verilator lint_on UNUSEDSIGNAL */

  neigh_hv_logic_pwr_ctrl_tdr_isolate u_new (
    .dft_tdr_ctrl_i          (dft_tdr_ctrl_i),
    .pwr_ctrl_glb_isolate_i  (pwr_ctrl_glb_isolate_i),
    .pwr_ctrl_glb_isolate_o  (new_pwr_ctrl_glb_isolate_o)
  );

  neigh_hv_logic_pwr_ctrl_tdr_isolate_orig u_orig (
    .dft_tdr_ctrl_in          (dft_tdr_ctrl_i),
    .pwr_ctrl_glb_isolate_in  (pwr_ctrl_glb_isolate_i),
    .pwr_ctrl_glb_isolate_out (orig_pwr_ctrl_glb_isolate_o)
  );

endmodule : cosim_neigh_hv_logic_pwr_ctrl_tdr_isolate_tb
