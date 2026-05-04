// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_via_prog_buf.v
//
// This module implements a via programmable buffer where the input
// and output can be disconnected and the output can be tied to 1/0
// with a metal fix

module et_via_prog_buf
(
    output  x,
    input   a
);

   et_via_prog_buf_grp_cells ET_GROUP_CELLS_et_via_prog_buf_grp_cells(.x(x), .a(a));

endmodule

// NOTE: GROUP_CELLS requires hard hierarchy to make it through dc elaborate
//       so going against coding styles and adding a sub-module within the same module
module et_via_prog_buf_grp_cells
(
    output  x,
    input   a
);

   wire        b;
   et_combo_buf et_combo_buf_in (.x(b), .a(a));
   et_combo_buf et_combo_buf_out(.x(x), .a(b));

endmodule

