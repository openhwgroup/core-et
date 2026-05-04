// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// 3-2 compressor
module r32cmp (/*AUTOARG*/
   // Outputs
   sv, cv,
   // Inputs
   a, b, c
   ) ;
   parameter WIDTH = 1;
   
   output [WIDTH-1:0] sv;
   output [WIDTH-1:0] cv;
   
   input [WIDTH-1:0]  a;
   input [WIDTH-1:0]  b;
   input [WIDTH-1:0]  c;
   
   wire [WIDTH-1:0]   sv;
   wire [WIDTH-1:0]   cv;

   assign sv = a ^ b ^ c;
   assign cv = a & b | a & c | b & c;

endmodule // r32cmp
