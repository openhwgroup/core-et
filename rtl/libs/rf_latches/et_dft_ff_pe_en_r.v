// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_dft_ff_pe_en_r
//   non-scannable active-low async reset, posedge ff, with enable for PD dont touch

module et_dft_ff_pe_en_r #(parameter
   WIDTH       = 1   // width of data
) (
   input  logic              clock,
   input  logic              resetn,
   input  logic              enable,
   input  logic  [WIDTH-1:0] din,
   output logic  [WIDTH-1:0] dout
);


`ifdef ET_LIBS_USE_STD_CELLS
   et_dft_ff_pe_en_r_grp_cells #(.WIDTH(WIDTH)) ET_GROUP_CELLS_et_dft_ff_pe_en_r_grp_cells (.clock(clock), .resetn(resetn), .enable(enable), .din(din), .dout(dout));
`else

   // active-low async reset, posedge ff, with enable
   always @(posedge clock or negedge resetn) begin
      if (!resetn) begin
         dout <= {WIDTH {1'b0}};
      end
      else begin
         if (enable) begin
            dout <= din;
         end
      end
   end

`endif  // ET_LIBS_USE_STD_CELLS

endmodule


`ifdef ET_LIBS_USE_STD_CELLS
// NOTE: GROUP_CELLS requires hard hierarchy to make it through dc elaborate
//       so going against coding styles and adding a sub-module within the same module
module et_dft_ff_pe_en_r_grp_cells #(parameter
   WIDTH       = 1   // width of data
) (
   input  logic              clock,
   input  logic              resetn,
   input  logic              enable,
   input  logic  [WIDTH-1:0] din,
   output logic  [WIDTH-1:0] dout
);

   generate
      if (1) begin : ET_SIZE_ONLY_et_dft_ff_pe_en_r
         // NOTE: EN on this cell is active low
         logic enable_n;
         et_combo_inv u_enable_n (.x(enable_n), .a(enable));
         HDBULT11_FSDPHRBQ_V2Y2_1 et_dft_ff_pe_en_r_flops [WIDTH-1:0] (.Q(dout), .CK(clock), .D(din), .EN(enable_n), .RD(resetn), .SI(1'b0), .SE(1'b0));
      end
   endgenerate

endmodule
`endif  // ET_LIBS_USE_STD_CELLS

