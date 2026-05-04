// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_clk_gate
//   common posedge clock gate

module et_clk_gate (
   output enclk,
   input  en,
   input  clk,
   input  te
);
   
`ifdef ET_LIBS_USE_STD_CELLS
   generate
      if (1) begin : ET_DONT_TOUCH_et_clk_gate_wrap
         HDBULT08_CKGTPLT_V7Y2_2 clk_gate (.SE(te), .EN(en), .CK(clk), .Q(enclk));
      end
   endgenerate
`else

   logic en_1p;
   always @(clk or en or te) begin
      if (!clk) begin
         en_1p <= en || te;
      end
   end
   
   assign enclk = clk && en_1p;

`endif

endmodule

