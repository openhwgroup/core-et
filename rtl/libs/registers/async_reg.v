// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// asyncRegister
module async_reg #(parameter width=32) (
  input CLK, reset, enableIn,
  input [width-1:0] D,
  output reg enableOut,
  output reg [width-1:0] Q);

  always @(posedge CLK or posedge reset)
    if (reset)
      Q <= 0;
    else if (enableIn)
      Q <= D;
      
  always @(posedge CLK or posedge reset)
    if (reset)
      enableOut <= 0;
    else
      enableOut <= enableIn;   
endmodule
