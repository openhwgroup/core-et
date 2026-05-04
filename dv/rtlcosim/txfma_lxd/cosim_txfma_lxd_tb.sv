// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_txfma_lxd_tb #(
  parameter int unsigned InWidth = 52,
  parameter int unsigned AftA0BitsNum = 4,
  parameter int unsigned OutWidth = 7
) (
  input  logic [InWidth-1:0]  in1_i,
  input  logic [InWidth-1:0]  in2_i,
  input  logic                sub_op_i,
  output logic [OutWidth-1:0] orig_lxd_o,
  output logic [OutWidth-1:0] new_lxd_o
);

  txfma_lxd_orig #(
    .in_width        (InWidth),
    .aft_a0_bits_num (AftA0BitsNum),
    .out_width       (OutWidth)
  ) u_orig (
    .in1    (in1_i),
    .in2    (in2_i),
    .sub_op (sub_op_i),
    .lxd    (orig_lxd_o)
  );

  txfma_lxd #(
    .InWidth      (InWidth),
    .AftA0BitsNum (AftA0BitsNum),
    .OutWidth     (OutWidth)
  ) u_new (
    .in1_i,
    .in2_i,
    .sub_op_i,
    .lxd_o (new_lxd_o)
  );

endmodule : cosim_txfma_lxd_tb
