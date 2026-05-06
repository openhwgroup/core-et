// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_txfma_4_2_compressor_array_tb #(
  parameter int unsigned Width = 64
) (
  input  logic [Width-1:0] x0_i,
  input  logic [Width-1:0] x1_i,
  input  logic [Width-1:0] x2_i,
  input  logic [Width-1:0] x3_i,
  output logic [Width-1:0] orig_c_o,
  output logic [Width-1:0] orig_s_o,
  output logic [Width-1:0] new_c_o,
  output logic [Width-1:0] new_s_o
);

  // mk/rtlcosim.mk renames complete original module identifiers with `_orig`.
  txfma_4_2_compressor_array_orig #(
    .width (Width)
  ) u_orig (
    .x0 (x0_i),
    .x1 (x1_i),
    .x2 (x2_i),
    .x3 (x3_i),
    .c  (orig_c_o),
    .s  (orig_s_o)
  );

  txfma_4_2_compressor_array #(
    .Width (Width)
  ) u_new (
    .x0_i (x0_i),
    .x1_i (x1_i),
    .x2_i (x2_i),
    .x3_i (x3_i),
    .c_o  (new_c_o),
    .s_o  (new_s_o)
  );

endmodule : cosim_txfma_4_2_compressor_array_tb
