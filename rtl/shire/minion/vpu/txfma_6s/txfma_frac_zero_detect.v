// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_frac_zero_detect
#(
  parameter left_bit     = `TXFMA_A3, 
  parameter right_bit    = `TXFMA_B28
)
(
  input  [left_bit:right_bit] in_data,  
  output                      zero
);

assign zero = ~(|in_data);

endmodule

