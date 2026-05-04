// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_dft_d_buf.v
//
// This module is used as an anchor to insert the OCC block.
// It the block will be placed between a buf and a mux
// The mux is controlled by the dft_hub with a select and an
// alternate clock.

module et_dft_occ_insert (
   output logic clock_out,
   input  logic clock_in,
   input  logic select,
   input  logic dft_clock_in
);

   et_dft_occ_insert_grp_cells ET_GROUP_CELLS_et_dft_occ_insert_grp_cells (
      .clock_out    ( clock_out    ),
      .clock_in     ( clock_in     ),
      .select       ( select       ),
      .dft_clock_in ( dft_clock_in )
   );

endmodule


// NOTE: GROUP_CELLS requires hard hierarchy to make it through dc elaborate
//       so going against coding styles and adding a sub-module within the same module
module et_dft_occ_insert_grp_cells (
   output logic clock_out,
   input  logic clock_in,
   input  logic select,
   input  logic dft_clock_in
);

   logic clock_in_buf;
   et_combo_buf  et_combo_buf_in   (.x(clock_in_buf), .a(clock_in));
   et_clk_mux2   et_clk_mux2_out   (.x(clock_out), .d0(clock_in_buf), .d1(dft_clock_in), .s(select));

endmodule

