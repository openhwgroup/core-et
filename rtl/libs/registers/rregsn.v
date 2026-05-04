// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// Flop with negedge clock
module rregsn (/*AUTOARG*/
   // Outputs
   q,
   // Inputs
   d, clk
   ) ;
   parameter WIDTH=1;
   
   output [WIDTH-1:0] q;
   input  [WIDTH-1:0] d;
   input              clk;

   reg [WIDTH-1:0]    q;

   always @(negedge clk)
     q <= d;
   
endmodule // rregsn

