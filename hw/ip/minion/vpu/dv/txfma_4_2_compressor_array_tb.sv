// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_4_2_compressor_array_tb #(
  parameter int unsigned Width = 64
) (
  input  logic [Width-1:0] x0_i,
  input  logic [Width-1:0] x1_i,
  input  logic [Width-1:0] x2_i,
  input  logic [Width-1:0] x3_i,
  output logic [Width-1:0] c_o,
  output logic [Width-1:0] s_o
);

  txfma_4_2_compressor_array #(
    .Width (Width)
  ) u_dut (
    .x0_i,
    .x1_i,
    .x2_i,
    .x3_i,
    .c_o,
    .s_o
  );

endmodule : txfma_4_2_compressor_array_tb
