// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module txfma_4_2_compressor_array #(parameter width=64)
(
  input     [width-1:0] x0, x1, x2, x3,
  output    [width-1:0] c, s
);

logic [width:0] connect_carry;

assign connect_carry[0] = 1'b0;

genvar i;
generate
  for (i=0; i <= width-1; i=i+1) begin : gen_connect
    txfma_4_2_compressor unit_compressor(x0[i], x1[i], x2[i], x3[i], connect_carry[i], connect_carry[i+1], c[i], s[i]);
  end
endgenerate

endmodule
