// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_csa #(
  parameter int unsigned Bits = 4
) (
  input  logic [Bits-1:0] a_i,
  input  logic [Bits-1:0] b_i,
  input  logic [Bits-1:0] cin_i,
  output logic [Bits-1:0] cout_o,
  output logic [Bits-1:0] sum_o
);

  assign sum_o  = a_i ^ b_i ^ cin_i;
  assign cout_o = (a_i & b_i) | (a_i & cin_i) | (b_i & cin_i);

endmodule : txfma_csa
