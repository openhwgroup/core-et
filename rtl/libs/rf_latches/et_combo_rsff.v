// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_combo_rsff
//    combinatorial RSFF for PD dont touch; when both r and s are asserted,
//    r takes precedence.

module et_combo_rsff (
   output q,
   input  r,
   input  s
);
   
logic qn;

`ifdef ET_LIBS_USE_STD_CELLS
   generate
      if (1) begin : ET_DONT_TOUCH_et_combo_rsff
         HDBULT11_NR2_2 nor_gate_0 (.X(q), .A1(qn), .A2(r));
         HDBULT11_NR2_2 nor_gate_1 (.X(qn), .A1(s), .A2(q));
      end
   endgenerate
`else
   assign q  = !(qn || r);
   assign qn = !(s  || q);
`endif

endmodule

