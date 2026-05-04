// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_txfma_norm_shf_tb
  import vpu_pkg::*;
#(
  parameter int LeftBitIn   = TxfmaA2,
  parameter int LeftBitOut  = TxfmaA0,
  parameter int RightBitIn  = TxfmaB45,
  parameter int RightBitOut = TxfmaB31,
  parameter int ShiftSz     = TxfmaExpSextPsSz
) (
  input  logic [LeftBitIn:RightBitIn]   op_i,
  input  logic [ShiftSz-1:0]            shfamnt_i,
  output logic [LeftBitOut:RightBitOut] orig_op_norm_o,
  output logic [LeftBitOut:RightBitOut] new_op_norm_o
);

  txfma_norm_shf_orig #(
    .left_bit_in   (LeftBitIn),
    .left_bit_out  (LeftBitOut),
    .right_bit_in  (RightBitIn),
    .right_bit_out (RightBitOut),
    .shift_size    (ShiftSz)
  ) u_orig (
    .op      (op_i),
    .shfamnt (shfamnt_i),
    .op_norm (orig_op_norm_o)
  );

  txfma_norm_shf #(
    .left_bit_in   (LeftBitIn),
    .left_bit_out  (LeftBitOut),
    .right_bit_in  (RightBitIn),
    .right_bit_out (RightBitOut),
    .shift_size    (ShiftSz)
  ) u_new (
    .op_i       (op_i),
    .shfamnt_i  (shfamnt_i),
    .op_norm_o  (new_op_norm_o)
  );

endmodule : cosim_txfma_norm_shf_tb
