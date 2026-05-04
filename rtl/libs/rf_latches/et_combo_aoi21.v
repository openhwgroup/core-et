// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_combo_and
//    combinatorial 3-input AND-OR-Inv gate for PD dont touch

module et_combo_aoi21 (
   output x,
   input  a1,
   input  a2,
   input  b
);
   
`ifdef ET_LIBS_USE_STD_CELLS
   generate
      if (1) begin : ET_DONT_TOUCH_et_combo_aoi21
         HDBULT11_AOI21_2 aoi21_gate (.X(x), .A1(a1), .A2(a2), .B(b));
      end
   endgenerate
`else
   assign x = ~((a1 && a2) || b);
`endif

endmodule

