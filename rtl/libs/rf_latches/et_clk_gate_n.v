// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_clk_gate_n
//   common negedge clock gate

module et_clk_gate_n (
   output enclk_n,
   input  en_1p,
   input  clk,
   input  te
);

`ifdef ET_LIBS_USE_STD_CELLS
   generate
      if (1) begin : ET_DONT_TOUCH_et_clk_gate_n_wrap
         HDBULT08_CKGTNLT_V5Y2_2 clk_gate_n(.SE(te), .EN(en_1p), .CK(clk), .Q(enclk_n));
      end
   endgenerate
`else

   logic en_2p;
   always @(clk or en_1p or te) begin
      if (clk) begin
         en_2p <= en_1p || te;
      end
   end

   assign enclk_n = clk || !en_2p;

`endif

endmodule

