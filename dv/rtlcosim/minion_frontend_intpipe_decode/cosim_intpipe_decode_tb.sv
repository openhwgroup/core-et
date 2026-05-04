// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation testbench: intpipe_decode (new) vs intpipe_decode (original).
//
// Both modules are purely combinational. The testbench exposes both
// raw decode output vectors for bit-by-bit comparison in C++.

`include "soc.vh"

module cosim_intpipe_decode_tb #(
  parameter bit EnableExtraTrans = 1'b0
) (
  input  logic        clk_i,
  input  logic        rst_ni,

  input  logic [31:0] stim_i,

  output logic [44:0] new_out_o,
  output logic [44:0] orig_out_o
);

  // ── New module (SystemVerilog, uses minion_frontend_pkg) ──
  import minion_pkg::*;
  import minion_frontend_pkg::*;

  minion_control_t new_ctrl;

  intpipe_decode #(
    .EnableExtraTrans(EnableExtraTrans)
  ) u_new (
    .inst_bits (stim_i),
    .inst_ctrl (new_ctrl)
  );

  assign new_out_o = $bits(minion_control_t)'(new_ctrl);

  // ── Original module (Verilog, uses `include "soc.vh") ──
  minion_control orig_ctrl;

  intpipe_decode_orig u_orig (
    .inst_bits (stim_i),
    .inst_ctrl (orig_ctrl)
  );

  assign orig_out_o = $bits(minion_control)'(orig_ctrl);

endmodule
