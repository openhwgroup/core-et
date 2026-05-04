// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_clk_dly_buf_a
//    BUF gate for PD to introduce a 15ps delay buffer

module et_clk_dly_buf_a (
   output logic x,
   input  logic a
);
wire a1, a2;    
`ifdef ET_LIBS_USE_STD_CELLS
   generate
      if (1) begin : ET_DONT_TOUCH_et_clk_dly_buf_a
         HDBULT08_INV_CK_2   inv_gate_0 (.X(a1), .CK(a));  // delay  5ps
         HDBULT08_INV_CK_2   inv_gate_1 (.X(a2), .CK(a1)); // delay  5ps
         HDBULT08_INV_CK_2   inv_gate_2 (.X( x), .CK(a2)); // delay  5ps
      end
   endgenerate
`else

   // Transport delay
   reg    x_del;
   always @(a)
     x_del <= #0.015 a;

   assign x = x_del;
`endif

endmodule

