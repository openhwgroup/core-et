// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// Flop with middle latch output and negedge clock
module rregsn_m (/*AUTOARG*/
   // Outputs
   q, qm,
   // Inputs
   d, clk
   ) ;
   parameter WIDTH=1;
   
   output [WIDTH-1:0] q;
   output [WIDTH-1:0] qm;
   input  [WIDTH-1:0] d;
   input              clk;

   reg [WIDTH-1:0]    qm;
   reg [WIDTH-1:0]    q;

   always @(clk or d)
     if (clk)
       qm <= d;

   always @(clk or qm)
     if (!clk)
       q <= qm;
   
endmodule // rregs_m

