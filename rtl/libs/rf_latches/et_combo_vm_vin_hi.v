// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_combo_vm_vin_hi
//    combinatorial gate for VDD rail monitor

module et_combo_vm_vin_hi #(parameter
   WIDTH       = 1   // width of data
) (
   output [WIDTH-1:0] x
);
   
`ifdef ET_LIBS_USE_STD_CELLS
   generate
      if (1) begin : ET_DONT_TOUCH_et_combo_vm_vin_hi
         HDBULT11_BUF_S_16 buf_gate [WIDTH-1:0] (.X(x), .A(1'b1));
      end
   endgenerate
`else
   assign x = 1'b1;
`endif

endmodule

