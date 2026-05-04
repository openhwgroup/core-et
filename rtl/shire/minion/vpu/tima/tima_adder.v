// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module tima_adder #(
  parameter WIDTH = 32
) (
  input  [WIDTH-1:0]       a,
  input  [WIDTH-1:0]       b,
  output logic [WIDTH-1:0] sum
);

  logic [WIDTH-1:0] sum_tmp;

  always_comb begin
    sum_tmp = a + b;
  end

  assign sum  = sum_tmp[WIDTH-1:0];

endmodule
