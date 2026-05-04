// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_combo_nor2b
//    combinatorial NOR2b gate for PD dont touch
//    NOTE: inverted input is a
//      x = !(!a || b)

module et_combo_nor2b (
   output x,
   input  a,
   input  b
);
   
`ifdef ET_LIBS_USE_STD_CELLS
   generate
      if (1) begin : ET_DONT_TOUCH_et_combo_nor2b
         HDBULT11_NR2B_2 nor2b_gate (.X(x), .A(a), .B(b));
      end
   endgenerate
`else
   assign x = !(!a || b);
`endif

endmodule

