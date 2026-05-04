// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_dft_d_buf.v
// Note: boot-legged from et_via_prog_buf.v  (cp -p et_via_prog_buf.v et_dft_d_buf.v)
// et_via_prog_buf.v
//
// This module implements a back-to-back pair of buffers, which will be inserted
// in-line on signals that may need to be intercepted via DC (or an ECO in the 
// future).  This structure is used primarily as a placeholder for down-stream 
// tools and flows.

module et_dft_d_buf #(parameter
   WIDTH       = 1   // width of data
) (
   output [WIDTH-1:0] x,
   input  [WIDTH-1:0] a
);

   et_dft_d_buf_grp_cells #(.WIDTH(WIDTH)) ET_GROUP_CELLS_et_dft_d_buf_grp_cells (.x(x), .a(a));

endmodule


// NOTE: GROUP_CELLS requires hard hierarchy to make it through dc elaborate
//       so going against coding styles and adding a sub-module within the same module
module et_dft_d_buf_grp_cells #(parameter
   WIDTH       = 1   // width of data
) (
   output [WIDTH-1:0] x,
   input  [WIDTH-1:0] a
);

   wire [WIDTH-1:0] b;
   et_combo_buf #(.WIDTH(WIDTH)) et_combo_buf_in  (.x(b), .a(a));
   et_combo_buf #(.WIDTH(WIDTH)) et_combo_buf_out (.x(x), .a(b));

endmodule

