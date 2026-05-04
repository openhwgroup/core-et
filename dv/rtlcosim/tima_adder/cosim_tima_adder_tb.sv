// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_tima_adder_tb (
  input  logic [31:0] a_i,
  input  logic [31:0] b_i,
  output logic [31:0] orig_sum_o,
  output logic [31:0] new_sum_o
);

  tima_adder_orig u_orig (
    .a   (a_i),
    .b   (b_i),
    .sum (orig_sum_o)
  );

  tima_adder u_new (
    .a_i   (a_i),
    .b_i   (b_i),
    .sum_o (new_sum_o)
  );

endmodule : cosim_tima_adder_tb
