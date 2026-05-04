// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// Flop with Enable & Synchronous Reset
module rregsnce_sr_m (/*AUTOARG*/
   // Outputs
   q, qm,
   // Inputs
   d, en, rst_2p, clk
   ) ;
   parameter WIDTH=1;
   
   output [WIDTH-1:0] q;        // 1p signal
   output [WIDTH-1:0] qm;       // 2p signal

   input  [WIDTH-1:0] d;        // 1p signal
   input              en;       // 1p signal
   input              rst_2p;   // 2p signal
   input              clk;

   reg [WIDTH-1:0]    q;
   reg [WIDTH-1:0]    qm;

   always @(clk or d or en or q)
     if (clk)
       qm <= en ? d : q;
   
   always @(clk or qm or rst_2p)
     if (!clk)
       q <= rst_2p ? {WIDTH{1'b0}} : qm;
   
endmodule // rregsnce_sr_m

