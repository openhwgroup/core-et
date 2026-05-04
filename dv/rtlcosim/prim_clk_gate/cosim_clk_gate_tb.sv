// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_clk_gate_tb
  import dft_pkg::*;
(
  input  logic clk_i,
  input  logic en_i,
  input  logic scanmode_i,
  output logic orig_clk_o,
  output logic new_clk_o
);

  dft_t dft;
  assign dft = '{scanmode: scanmode_i, default: '0};

  et_clk_gate u_orig (
    .enclk (orig_clk_o),
    .en    (en_i),
    .clk   (clk_i),
    .te    (scanmode_i)
  );

  prim_clk_gate u_new (
    .clk_i (clk_i),
    .en_i  (en_i),
    .dft_i (dft),
    .clk_o (new_clk_o)
  );

endmodule
