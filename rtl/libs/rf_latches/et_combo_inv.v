// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_combo_inv
//    combinatorial INV gate for PD dont touch

module et_combo_inv (
   output x,
   input  a
);
   
`ifdef ET_LIBS_USE_STD_CELLS
   generate
      if (1) begin : ET_DONT_TOUCH_et_combo_inv
         HDBULT11_INV_S_2 inv_gate (.X(x), .A(a));
      end
   endgenerate
`else
   assign x = !a;
`endif

endmodule

