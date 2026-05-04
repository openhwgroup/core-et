// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_combo_vm_vin_lo
//    combinatorial gate for VSS monitor

module et_combo_vm_vin_lo #(parameter
   WIDTH       = 1   // width of data
) (
   output [WIDTH-1:0] x
);
   
`ifdef ET_LIBS_USE_STD_CELLS
   generate
      if (1) begin : ET_DONT_TOUCH_et_combo_vm_vin_lo
         HDBULT11_BUF_S_16 buf_gate [WIDTH-1:0] (.X(x), .A(1'b0));
      end
   endgenerate
`else
   assign x = 1'b0;
`endif

endmodule

