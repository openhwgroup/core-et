// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_4_2_compressor_tb (
  input  logic x0_i,
  input  logic x1_i,
  input  logic x2_i,
  input  logic x3_i,
  input  logic cin_i,
  output logic cout_o,
  output logic c_o,
  output logic s_o
);

  txfma_4_2_compressor u_dut (
    .x0_i,
    .x1_i,
    .x2_i,
    .x3_i,
    .cin_i,
    .cout_o,
    .c_o,
    .s_o
  );

endmodule : txfma_4_2_compressor_tb
