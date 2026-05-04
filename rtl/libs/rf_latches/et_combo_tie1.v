// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_combo_tie1
//    combinatorial TIE1 gate for PD dont touch

module et_combo_tie1 (
   output x
);
   
`ifdef ET_LIBS_USE_STD_CELLS
   generate
      if (1) begin : ET_DONT_TOUCH_et_combo_tie1
         HDBULT11_TIE1_1 tie1_gate (.X(x));
      end
   endgenerate
`else
   assign x = 1'b1;
`endif

endmodule

