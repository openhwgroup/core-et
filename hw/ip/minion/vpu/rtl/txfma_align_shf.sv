// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module txfma_align_shf
  import vpu_pkg::*;
#(
  parameter int left_bit  = TxfmaA1,
  parameter int right_bit = TxfmaB45,
  parameter int shft_size = TxfmaExpPsSz
) (
  input  logic [left_bit:right_bit] op_i,
  input  logic [shft_size-1:0]      shfamnt_i,
  output logic [left_bit:right_bit] op_align_o
);

  assign op_align_o = op_i >> shfamnt_i;

endmodule : txfma_align_shf
