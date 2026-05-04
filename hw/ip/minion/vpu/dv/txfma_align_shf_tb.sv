// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_align_shf_tb
  import vpu_pkg::*;
#(
  parameter int LeftBit  = TxfmaA1,
  parameter int RightBit = TxfmaB45,
  parameter int ShiftSz  = TxfmaExpPsSz
) (
  input  logic [LeftBit:RightBit] op_i,
  input  logic [ShiftSz-1:0]      shfamnt_i,
  output logic [LeftBit:RightBit] op_align_o
);

  txfma_align_shf #(
    .left_bit  (LeftBit),
    .right_bit (RightBit),
    .shft_size (ShiftSz)
  ) u_dut (
    .op_i,
    .shfamnt_i,
    .op_align_o
  );

endmodule : txfma_align_shf_tb
