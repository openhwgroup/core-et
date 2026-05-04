// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_norm_shf 
#(
  parameter left_bit_in=`TXFMA_A2,
  parameter left_bit_out=`TXFMA_A0,
  parameter right_bit_in=`TXFMA_B45,
  parameter right_bit_out=`TXFMA_B31,
  parameter shift_size=`TXFMA_EXP_SEXT_PS_SZ
)
(
  input  [left_bit_in:right_bit_in]   op,
  input  [shift_size-1:0]             shfamnt,  
  output [left_bit_out:right_bit_out] op_norm
);

logic [left_bit_in:right_bit_in] op_norm_ext;
logic [shift_size-1:0]           shfamnt_abs;

assign shfamnt_abs = shfamnt[shift_size-1] ? -shfamnt : shfamnt;
assign op_norm_ext = shfamnt[shift_size-1] ? op >> shfamnt_abs : op << shfamnt_abs;
assign op_norm = op_norm_ext[left_bit_out:right_bit_out];

endmodule
