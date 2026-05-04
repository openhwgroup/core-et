// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vclevel_shft_h2l

module vclevel_shft_h2l #(parameter
   WIDTH = 1
) (
   output logic [WIDTH-1:0] out_lv,
   input  logic [WIDTH-1:0] inp_hv
);

   et_combo_buf ls [WIDTH-1:0] (.x(out_lv), .a(inp_hv));

   
endmodule
