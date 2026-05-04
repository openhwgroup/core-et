// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module prim_write_preview_en_tb (
  input  logic [7:0] d_i,
  input  logic       clk_i,
  input  logic       en_i,
  output logic [7:0] q_o
);

  prim_write_preview_en #(
    .Width (8)
  ) u_dut (
    .clk_i,
    .en_i,
    .d_i,
    .q_o
  );

endmodule
