// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vclevel_shft_h2l_2x_def_hiv

module vclevel_shft_h2l_2x_def_hiv #(parameter
   WIDTH = 1
) (
   output logic [WIDTH-1:0] out_lv,
   input  logic [WIDTH-1:0] inp_hv
);

   vclevel_shft_h2l_2x_def_hiv_gv #(.WIDTH(WIDTH)) ls_pd_gv (.out(out_lv), .inp(inp_hv));

endmodule


module vclevel_shft_h2l_2x_def_hiv_gv #(parameter
   WIDTH = 1
) (
   output logic [WIDTH-1:0] out,
   input  logic [WIDTH-1:0] inp
);

   logic [WIDTH-1:0] int_buf;
   et_combo_buf ls [WIDTH-1:0] (.x(int_buf), .a(inp));
   vclevel_shft_h2l_2x_def_hiv_lv #(.WIDTH(WIDTH)) ls_pd_lv (.out(out), .inp(int_buf));

endmodule


module vclevel_shft_h2l_2x_def_hiv_lv #(parameter
   WIDTH = 1
) (
   output logic [WIDTH-1:0] out,
   input  logic [WIDTH-1:0] inp
);

   et_combo_buf ls [WIDTH-1:0] (.x(out), .a(inp));

endmodule
