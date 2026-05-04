// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_txfma_4_2_compressor_tb (
  input  logic x0_i,
  input  logic x1_i,
  input  logic x2_i,
  input  logic x3_i,
  input  logic cin_i,
  output logic orig_cout_o,
  output logic orig_c_o,
  output logic orig_s_o,
  output logic new_cout_o,
  output logic new_c_o,
  output logic new_s_o
);

  txfma_4_2_compressor_orig u_orig (
    .x0   (x0_i),
    .x1   (x1_i),
    .x2   (x2_i),
    .x3   (x3_i),
    .cin  (cin_i),
    .cout (orig_cout_o),
    .c    (orig_c_o),
    .s    (orig_s_o)
  );

  txfma_4_2_compressor u_new (
    .x0_i   (x0_i),
    .x1_i   (x1_i),
    .x2_i   (x2_i),
    .x3_i   (x3_i),
    .cin_i  (cin_i),
    .cout_o (new_cout_o),
    .c_o    (new_c_o),
    .s_o    (new_s_o)
  );

endmodule : cosim_txfma_4_2_compressor_tb
