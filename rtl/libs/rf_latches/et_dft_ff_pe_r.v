// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_dft_ff_pe_r
//   non-scannable active-low async reset, posedge ff for PD dont touch

module et_dft_ff_pe_r #(parameter
   WIDTH       = 1   // width of data
) (
   input  logic              clock,
   input  logic              resetn,
   input  logic  [WIDTH-1:0] din,
   output logic  [WIDTH-1:0] dout
);


`ifdef ET_LIBS_USE_STD_CELLS
   et_dft_ff_pe_r_grp_cells #(.WIDTH(WIDTH)) ET_GROUP_CELLS_et_dft_ff_pe_r_grp_cells (.clock(clock), .resetn(resetn), .din(din), .dout(dout));
`else

   // active-low async reset, posedge ff
   always @(posedge clock or negedge resetn) begin
      if (!resetn) begin
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
module et_dft_ff_pe_r_grp_cells #(parameter
   WIDTH       = 1   // width of data
) (
   input  logic              clock,
   input  logic              resetn,
   input  logic  [WIDTH-1:0] din,
   output logic  [WIDTH-1:0] dout
);

   generate
      if (1) begin : ET_SIZE_ONLY_et_dft_ff_pe_r
         HDBULT11_FDPRBQ_V2Y2_1 et_dft_ff_pe_r_flops [WIDTH-1:0] (.Q(dout), .CK(clock), .D(din), .RD(resetn));
      end
   endgenerate

endmodule
`endif  // ET_LIBS_USE_STD_CELLS

