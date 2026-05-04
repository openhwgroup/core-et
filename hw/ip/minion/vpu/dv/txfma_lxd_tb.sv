// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_lxd_tb #(
  parameter int unsigned InWidth      = 52,
  parameter int unsigned AftA0BitsNum = 4,
  parameter int unsigned OutWidth     = 7
) (
  input  logic [InWidth-1:0]  in1_i,
  input  logic [InWidth-1:0]  in2_i,
  input  logic                sub_op_i,
  output logic [OutWidth-1:0] lxd_o
);

  txfma_lxd #(
    .InWidth      (InWidth),
    .AftA0BitsNum (AftA0BitsNum),
    .OutWidth     (OutWidth)
  ) u_dut (
    .in1_i,
    .in2_i,
    .sub_op_i,
    .lxd_o
  );

endmodule : txfma_lxd_tb
