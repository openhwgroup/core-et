// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_txfma_align_shf_tb
  import vpu_pkg::*;
#(
  parameter int LeftBit  = TxfmaA1,
  parameter int RightBit = TxfmaB45,
  parameter int ShiftSz  = TxfmaExpPsSz
) (
  input  logic [LeftBit:RightBit] op_i,
  input  logic [ShiftSz-1:0]      shfamnt_i,
  output logic [LeftBit:RightBit] orig_op_align_o,
  output logic [LeftBit:RightBit] new_op_align_o
);

  txfma_align_shf_orig #(
    .left_bit  (LeftBit),
    .right_bit (RightBit),
    .shft_size (ShiftSz)
  ) u_orig (
    .op       (op_i),
    .shfamnt  (shfamnt_i),
    .op_align (orig_op_align_o)
  );

  txfma_align_shf #(
    .left_bit  (LeftBit),
    .right_bit (RightBit),
    .shft_size (ShiftSz)
  ) u_new (
    .op_i       (op_i),
    .shfamnt_i  (shfamnt_i),
    .op_align_o (new_op_align_o)
  );

endmodule : cosim_txfma_align_shf_tb
