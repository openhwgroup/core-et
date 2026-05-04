// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_4_2_compressor (
  input  logic x0_i,
  input  logic x1_i,
  input  logic x2_i,
  input  logic x3_i,
  input  logic cin_i,
  output logic cout_o,
  output logic c_o,
  output logic s_o
);

  logic [0:0] csa0_cout;
  logic [0:0] csa0_sum;
  logic [0:0] csa1_cout;
  logic [0:0] csa1_sum;

  txfma_csa #(
    .Bits (1)
  ) u_csa0 (
    .a_i    (x0_i),
    .b_i    (x1_i),
    .cin_i  (x2_i),
    .cout_o (csa0_cout),
    .sum_o  (csa0_sum)
  );

  txfma_csa #(
    .Bits (1)
  ) u_csa1 (
    .a_i    (csa0_sum),
    .b_i    (x3_i),
    .cin_i  (cin_i),
    .cout_o (csa1_cout),
    .sum_o  (csa1_sum)
  );

  assign cout_o = csa0_cout[0];
  assign c_o    = csa1_cout[0];
  assign s_o    = csa1_sum[0];

endmodule : txfma_4_2_compressor
