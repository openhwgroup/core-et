// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_ff.v
//   ff gate for PD dont touch

`include "macros.vh"

module et_ff #(parameter
   WIDTH       = 1   // width of data
) (
   input  logic              clock,
   input  logic  [WIDTH-1:0] din,
   output logic  [WIDTH-1:0] dout
);

`ifdef ET_LIBS_USE_STD_CELLS
   et_ff_grp_cells #(.WIDTH(WIDTH)) ET_GROUP_CELLS_et_ff_grp_cells (.clock(clock), .din(din), .dout(dout));
`else
   // simple behavioral model
   `FF(clock, dout, din);
`endif  // ET_LIBS_USE_STD_CELLS

endmodule

`ifdef ET_LIBS_USE_STD_CELLS
// NOTE: GROUP_CELLS requires hard hierarchy to make it through dc elaborate
//       so going against coding styles and adding a sub-module within the same module
module et_ff_grp_cells #(parameter
   WIDTH       = 1   // width of data
) (
   input  logic              clock,
   input  logic  [WIDTH-1:0] din,
   output logic  [WIDTH-1:0] dout
);

   generate
      if (1) begin : ET_DONT_TOUCH_et_ff
         HDBULT11_FSDPQ_V2Y2_1 et_ff_flops [WIDTH-1:0] ( .Q(dout), .CK(clock), .D(din), .SI (1'b0), .SE (1'b0));
      end
   endgenerate

endmodule
`endif  // ET_LIBS_USE_STD_CELLS

