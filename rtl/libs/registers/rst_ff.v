// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module rst_ff #(parameter width=32) 
(
  input                  clock, 
  input                  reset, 
  input [width-1:0]      D,
  output reg [width-1:0] Q
);

  always @(posedge clock)
    if (reset) Q <= '{default:0};
    else       Q <= D;
     
endmodule
