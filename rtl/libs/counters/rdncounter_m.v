// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// rdncounter with latch outputs
module rdncounter_m(/*AUTOARG*/
   // Outputs
   q_2p, q_1p,
   // Inputs
   init_2p, set_2p, en_2p, rst_2p, clk
   );

   parameter WIDTH = 2;

   output  [WIDTH-1:0] q_2p;
   output  [WIDTH-1:0] q_1p;

   input [WIDTH-1:0]   init_2p; // initial value
   input               set_2p;  // set initial value
   input               en_2p;   // enable counter
   input               rst_2p;  // reset
   input               clk;     // Phase 1 clock

   reg [WIDTH-1:0]     q_1p;
   reg [WIDTH-1:0]     q_2p;
   
   // Ph2
   always @(clk or init_2p or en_2p or set_2p or rst_2p or q_2p)
     if (!clk)
       q_1p <= rst_2p ? {WIDTH{1'b0}} :
               set_2p ? init_2p :
               en_2p  ? q_2p - {{WIDTH-1{1'b0}}, 1'b1} :
                        q_2p;
   // Ph1
   always @(clk or q_1p)
     if (clk)
       q_2p <= q_1p;

endmodule // rdncounter_m
