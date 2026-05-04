// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_combo_and
//    combinatorial AND gate for PD dont touch

module et_combo_and (
   output x,
   input  a1,
   input  a2
);
   
`ifdef ET_LIBS_USE_STD_CELLS
   generate
      if (1) begin : ET_DONT_TOUCH_et_combo_and
         HDBULT11_AN2_MM_2 and_gate (.X(x), .A1(a1), .A2(a2));
      end
   endgenerate
`else
   assign x = a1 && a2;
`endif

endmodule

