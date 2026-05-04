// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// Flop with Enable & Synchronous Reset
module rregsce_sr_m (
   // Outputs
   q, qm,
   // Inputs
   d, en, rst, clk
   ) ;
   parameter WIDTH=1;
   
   output [WIDTH-1:0] q;
   output [WIDTH-1:0] qm;

   input  [WIDTH-1:0] d;
   input              en;
   input              rst;
   input              clk;

   reg [WIDTH-1:0]    q;
   reg [WIDTH-1:0]    qm;
   
   always @(clk or d or en or rst or q)
     if (!clk)
       qm <= rst ? {WIDTH{1'b0}} : (en ? d : q);
     
   always @(clk or qm)
     if (clk)
       q <= qm;
   

endmodule // rregs

