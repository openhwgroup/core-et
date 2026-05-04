// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vclevel_shft_l2h

module vclevel_shft_l2h #(parameter
   WIDTH = 1
) (
   output logic [WIDTH-1:0] out_hv,
   input  logic [WIDTH-1:0] inp_lv
);

   et_combo_buf ls [WIDTH-1:0] (.x(out_hv), .a(inp_lv));

endmodule