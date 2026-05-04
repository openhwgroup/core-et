// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_adder
#
(
  parameter WIDTH = 0
)
(
  input  [WIDTH-1:0]       a,
  input  [WIDTH-1:0]       b,
  input                    cin,
  output logic [WIDTH-1:0] sum
);

  logic [WIDTH:0] sum_tmp;

  always_comb begin
    sum_tmp = a + b + {{(WIDTH-1){1'b0}}, cin};
  end

  assign sum  = sum_tmp[WIDTH-1:0];

endmodule
