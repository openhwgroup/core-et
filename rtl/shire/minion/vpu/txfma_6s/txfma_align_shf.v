// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_align_shf
#(
  parameter left_bit=`TXFMA_A1, 
  parameter right_bit=`TXFMA_B45, 
  parameter shft_size=`TXFMA_EXP_PS_SZ
) 
(
  input        [left_bit:right_bit] op,
  input        [shft_size-1:0]      shfamnt,
  output logic [left_bit:right_bit] op_align
);

assign op_align = op >> shfamnt;

endmodule
