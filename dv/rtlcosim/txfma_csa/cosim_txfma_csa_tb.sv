// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_txfma_csa_tb #(
  parameter int unsigned Bits = 32
) (
  input  logic [Bits-1:0] a_i,
  input  logic [Bits-1:0] b_i,
  input  logic [Bits-1:0] cin_i,
  output logic [Bits-1:0] orig_cout_o,
  output logic [Bits-1:0] orig_sum_o,
  output logic [Bits-1:0] new_cout_o,
  output logic [Bits-1:0] new_sum_o
);

  txfma_csa_orig #(
    .bits (Bits)
  ) u_orig (
    .a    (a_i),
    .b    (b_i),
    .cin  (cin_i),
    .cout (orig_cout_o),
    .sum  (orig_sum_o)
  );

  txfma_csa #(
    .Bits (Bits)
  ) u_new (
    .a_i    (a_i),
    .b_i    (b_i),
    .cin_i  (cin_i),
    .cout_o (new_cout_o),
    .sum_o  (new_sum_o)
  );

endmodule : cosim_txfma_csa_tb
