// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_adder_tb #(
  parameter int unsigned Width = 64
) (
  input  logic [Width-1:0] a_i,
  input  logic [Width-1:0] b_i,
  input  logic             cin_i,
  output logic [Width-1:0] sum_o
);

  txfma_adder #(
    .Width (Width)
  ) u_dut (
    .a_i,
    .b_i,
    .cin_i,
    .sum_o
  );

endmodule : txfma_adder_tb
