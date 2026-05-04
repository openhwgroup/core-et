// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_combo_buf
//    combinatorial BUF gate for PD dont touch

module et_combo_buf #(parameter
   WIDTH       = 1   // width of data
) (
   output [WIDTH-1:0] x,
   input  [WIDTH-1:0] a
);
   
`ifdef ET_LIBS_USE_STD_CELLS
   generate
      if (1) begin : ET_DONT_TOUCH_et_combo_buf
         HDBULT11_BUF_S_2 buf_gate [WIDTH-1:0] (.X(x), .A(a));
      end
   endgenerate
`else
   assign x = a;
`endif

endmodule

