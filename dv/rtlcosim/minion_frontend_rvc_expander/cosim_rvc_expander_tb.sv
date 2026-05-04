// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation testbench: minion_frontend_rvc_expander vs frontend_rvc_expander.

module cosim_rvc_expander_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  input  logic [31:0] stim_i,

  output logic [31:0] new_out_o,
  output logic [31:0] orig_out_o
);

  minion_frontend_rvc_expander u_new (
    .in_bits  (stim_i),
    .out_bits (new_out_o)
  );

  frontend_rvc_expander u_orig (
    .in_bits  (stim_i),
    .out_bits (orig_out_o)
  );

endmodule
