// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module tima_adder #(
  parameter int unsigned Width = 32
) (
  input  logic [Width-1:0] a_i,
  input  logic [Width-1:0] b_i,
  output logic [Width-1:0] sum_o
);

  logic [Width-1:0] sum_tmp;

  always_comb begin
    sum_tmp = a_i + b_i;
  end

  assign sum_o = sum_tmp[Width-1:0];

endmodule : tima_adder
