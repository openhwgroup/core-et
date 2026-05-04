// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// 4-2 compressor
module r42cmp (/*AUTOARG*/
   // Outputs
   sv, cv,
   // Inputs
   a, b, c, d, cin
   ) ;
   parameter WIDTH = 1;
   
   output [WIDTH-1:0] sv;
   output [WIDTH-1:0] cv;
   
   input [WIDTH-1:0]  a;
   input [WIDTH-1:0]  b;
   input [WIDTH-1:0]  c;
   input [WIDTH-1:0]  d;
   input              cin;
   
   wire [WIDTH-1:0]   cout;
   
   assign cout = (a ^ b) & c | ~(a ^ b) & a;
   
   assign sv = (a ^ b ^ c ^ d) ^ {cout[WIDTH-2:0], cin};
   assign cv = (a ^ b ^ c ^ d) & {cout[WIDTH-2:0], cin}  | ~(a ^ b ^ c ^ d) & d;
   

endmodule // r42cmp
