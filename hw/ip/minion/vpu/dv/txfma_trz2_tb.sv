// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_trz2_tb #(
  parameter int unsigned InWidth  = 52,
  parameter int unsigned OutWidth = 6
) (
  input  logic [InWidth-1:0]  in1_i,
  input  logic [InWidth-1:0]  in2_i,
  input  logic                add_cin_i,
  output logic [OutWidth-1:0] trz_o
);

  txfma_trz2 #(
    .InWidth  (InWidth),
    .OutWidth (OutWidth)
  ) u_dut (
    .in1_i,
    .in2_i,
    .add_cin_i,
    .trz_o
  );

endmodule : txfma_trz2_tb
