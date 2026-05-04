// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module prim_clk_gate_tb
  import dft_pkg::*;
(
  input  logic clk_i,
  input  logic en_i,
  input  logic scanmode_i,
  output logic clk_o
);

  dft_t dft;
  assign dft = '{scanmode: scanmode_i, default: '0};

  prim_clk_gate u_dut (
    .clk_i (clk_i),
    .en_i  (en_i),
    .dft_i (dft),
    .clk_o (clk_o)
  );

endmodule
