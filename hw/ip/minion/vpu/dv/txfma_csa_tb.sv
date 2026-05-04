// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_csa_tb #(
  parameter int unsigned Bits = 32
) (
  input  logic [Bits-1:0] a_i,
  input  logic [Bits-1:0] b_i,
  input  logic [Bits-1:0] cin_i,
  output logic [Bits-1:0] cout_o,
  output logic [Bits-1:0] sum_o
);

  txfma_csa #(
    .Bits (Bits)
  ) u_dut (
    .a_i,
    .b_i,
    .cin_i,
    .cout_o,
    .sum_o
  );

endmodule : txfma_csa_tb
