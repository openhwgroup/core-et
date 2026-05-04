// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_combo_tie0
//    combinatorial TIE0 gate for PD dont touch

module et_combo_tie0 (
   output x
);
   
`ifdef ET_LIBS_USE_STD_CELLS
   generate
      if (1) begin : ET_DONT_TOUCH_et_combo_tie0
         HDBULT11_TIE0_1 tie0_gate (.X(x));
      end
   endgenerate
`else
   assign x = 1'b0;
`endif

endmodule

