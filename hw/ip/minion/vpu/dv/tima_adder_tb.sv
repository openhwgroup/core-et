// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module tima_adder_tb #(
  parameter int unsigned Width = 32
) (
  input  logic [Width-1:0] a_i,
  input  logic [Width-1:0] b_i,
  output logic [Width-1:0] sum_o
);

  tima_adder #(
    .Width (Width)
  ) u_dut (
    .a_i,
    .b_i,
    .sum_o
  );

endmodule : tima_adder_tb
