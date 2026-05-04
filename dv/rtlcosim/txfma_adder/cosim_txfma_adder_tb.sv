// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_txfma_adder_tb #(
  parameter int unsigned Width = 64
) (
  input  logic [Width-1:0] a_i,
  input  logic [Width-1:0] b_i,
  input  logic             cin_i,
  output logic [Width-1:0] orig_sum_o,
  output logic [Width-1:0] new_sum_o
);

  txfma_adder_orig #(
    .WIDTH (Width)
  ) u_orig (
    .a   (a_i),
    .b   (b_i),
    .cin (cin_i),
    .sum (orig_sum_o)
  );

  txfma_adder #(
    .Width (Width)
  ) u_new (
    .a_i,
    .b_i,
    .cin_i,
    .sum_o (new_sum_o)
  );

endmodule : cosim_txfma_adder_tb
