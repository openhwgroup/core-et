// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module ff #(parameter width=32) 
(
  input                  clock, 
  input [width-1:0]      D,
  output reg [width-1:0] Q
);

  always @(posedge clock)
    Q <= D;
     
endmodule
