// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_clk_muxn
//    MUXN gate for PD to introduce a 15 ps delay buffer

module et_clk_muxn(
   output logic x,
   input  logic s,
   input  logic d0,
   input  logic d1
);
`ifdef ET_LIBS_USE_STD_CELLS
   generate
      if (1) begin : ET_DONT_TOUCH_et_clk_muxn
         HDBULT08_MUXI2_Y2_2 muxn (.D0(d0),.D1(d1),.S(s),.X(x));
      end
   endgenerate
`else
   // Transport delay
   reg    d0_del, d1_del, x_del;
   always @ (d0_del, d1_del, s) begin     
     case(s)
       1'b0     : x_del = d0_del; // If s=0, output is d0
       1'b1     : x_del = d1_del; // If s=1, output is d1
       default  : x_del = 0;      // If s is anything else, out is always 0
     endcase
   end

   always @(d0) begin
     d0_del <= #0.015 d0;
   end
   always @(d1) begin
     d1_del <= #0.015 d1;
   end
   assign x = x_del;
`endif

endmodule

