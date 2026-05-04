// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_txfma_trz2_tb #(
  parameter int unsigned InWidth  = 52,
  parameter int unsigned OutWidth = 6
) (
  input  logic [InWidth-1:0]  in1_i,
  input  logic [InWidth-1:0]  in2_i,
  input  logic                add_cin_i,
  output logic [OutWidth-1:0] orig_trz_o,
  output logic [OutWidth-1:0] new_trz_o
);

  txfma_trz2_orig #(
    .in_width  (InWidth),
    .out_width (OutWidth)
  ) u_orig (
    .in1     (in1_i),
    .in2     (in2_i),
    .add_cin (add_cin_i),
    .trz     (orig_trz_o)
  );

  txfma_trz2 #(
    .InWidth  (InWidth),
    .OutWidth (OutWidth)
  ) u_new (
    .in1_i,
    .in2_i,
    .add_cin_i,
    .trz_o (new_trz_o)
  );

endmodule : cosim_txfma_trz2_tb
