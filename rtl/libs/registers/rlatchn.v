// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// Transparent Low D-Latch
module rlatchn (/*AUTOARG*/
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
   /* verilator lint_off COMBDLY */
   /* verilator lint_off LATCH */
   always @(clk or d)
     if (!clk)
       q <= d;
   
   /* verilator lint_on COMBDLY */
   /* verilator lint_on LATCH */

endmodule // rlatch_n

