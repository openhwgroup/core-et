// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_ff_async_clr.v
//   ff gate with async reset for PD dont touch
//   RESET_VAL = 1'b0

module et_ff_async_reset #(parameter
   WIDTH       = 1   // width of data
) (
   input  logic              clock,
   input  logic              reset,
   input  logic  [WIDTH-1:0] din,
   output logic  [WIDTH-1:0] dout
);

`ifdef ET_LIBS_USE_STD_CELLS
   et_ff_async_reset_grp_cells #(.WIDTH(WIDTH)) ET_GROUP_CELLS_et_ff_async_rst_grp_cells (.clock(clock), .reset(reset), .din(din), .dout(dout));
`else
   // simple behavioral model
   // active-low async reset, posedge ff
   always @(posedge clock or negedge reset) begin
      if (!reset) begin
         dout <= {WIDTH {1'b0}};
      end
      else begin
         dout <= din;
      end
   end
`endif  // ET_LIBS_USE_STD_CELLS

endmodule

`ifdef ET_LIBS_USE_STD_CELLS
// NOTE: GROUP_CELLS requires hard hierarchy to make it through dc elaborate
//       so going against coding styles and adding a sub-module within the same module
module et_ff_async_reset_grp_cells #(parameter
   WIDTH       = 1   // width of data
) (
   input  logic              clock,
   input  logic              reset,
   input  logic  [WIDTH-1:0] din,
   output logic  [WIDTH-1:0] dout
);

   generate
      if (1) begin : ET_DONT_TOUCH_et_ff
         HDBULT08_FSDPRBQ_CBV2Y2_1 et_ff_flops [WIDTH-1:0] ( .Q(dout), .CK(clock), .RD(reset), .D(din), .SI (1'b0), .SE (1'b0));
      end
   endgenerate

endmodule
`endif  // ET_LIBS_USE_STD_CELLS

