// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


// converts a fixed uint to another fixed uint with different width
// rounding and saturating if the corresponding parameters are set
module ufixed2ufixed (in,out);
   // module parameters
   parameter OUT_I_BITS=4;         // integer bits of the output
   parameter OUT_F_BITS=8;         // fractional bits of output
   parameter IN_I_BITS=OUT_I_BITS; // integer bits of the input
   parameter IN_F_BITS=OUT_F_BITS; // fractional bits of input
   parameter ROUND = 1'b1;         // round if trimming fractional bits
   parameter SAT = 1'b1;           // saturate (set to all 1's) if result doesn't fit
   
   //derived localparams
   localparam OUT_BITS = OUT_I_BITS + OUT_F_BITS;
   localparam IN_BITS = IN_I_BITS + IN_F_BITS;
   localparam MAX_I_BITS = OUT_I_BITS > IN_I_BITS? OUT_I_BITS : IN_I_BITS;
   localparam MAX_F_BITS = OUT_F_BITS > IN_F_BITS? OUT_F_BITS : IN_F_BITS;
   localparam EXPAND_SIZE = MAX_I_BITS + MAX_F_BITS;

   // module ports
   input [IN_BITS-1:0]   in;
   output [OUT_BITS-1:0] out;
   
   // output assignment
   assign out = conversion(in, ROUND, SAT);

   // helper functions
   function automatic [OUT_BITS-1:0] conversion;
      input [IN_BITS-1:0] p;
      input               round;
      input               sat;
      logic [EXPAND_SIZE + 1-1:0] e;
      begin
         e = {1'b0, expand(p)};
//         $display("e1=%x",e);
         if (round && OUT_F_BITS!=MAX_F_BITS)
           e = e + (1'b1<<(MAX_F_BITS-OUT_F_BITS-1));
//         $display("e2=%x",e);
         if (sat && e[EXPAND_SIZE+1-1:MAX_F_BITS+OUT_I_BITS]) conversion =  {(OUT_BITS) {1'b1}} ;
         else begin
            conversion[OUT_BITS-1:OUT_F_BITS] = e[MAX_F_BITS +: OUT_I_BITS];
            conversion[OUT_F_BITS-1:0] = e[MAX_F_BITS-1 -: OUT_F_BITS];
//            $display("conversion=%x",conversion);
         end
      end
   endfunction
   
   // returns a fixed point with size = <MAX_I_BITS, MAX_F_BITS>, shifted to adjust the point
   function automatic [EXPAND_SIZE-1:0] expand;
      input [IN_BITS-1:0] p;
      begin
         expand = '0;
         expand[MAX_F_BITS+:IN_I_BITS] = p [IN_BITS-1:IN_F_BITS];
         expand[MAX_F_BITS-1 -: IN_F_BITS] = p [IN_F_BITS-1:0];
      end
   endfunction
   
endmodule // ufixed2ufixed
