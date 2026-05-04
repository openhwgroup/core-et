// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_combo_mux2
//    combinatorial data MUX2 gate for PD dont touch

module et_combo_mux2 (
   output x,
   input  d0,
   input  d1,
   input  s
);
   
`ifdef ET_LIBS_USE_STD_CELLS
   generate
      if (1) begin : ET_DONT_TOUCH_et_combo_mux2
         HDBULT11_MUX2_MM_2 mux2_gate (.X(x), .D0(d0), .D1(d1), .S(s));
      end
   endgenerate
`else
   assign x = s ? d1 : d0;
`endif

endmodule

