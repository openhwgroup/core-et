// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vclevel_shft_l2h_2x_def_hiv

module vclevel_shft_l2h_2x_def_hiv #(parameter
   WIDTH = 1
) (
   output logic [WIDTH-1:0] out_hv,
   input  logic [WIDTH-1:0] inp_lv
);

   vclevel_shft_l2h_2x_def_hiv_gv #(.WIDTH(WIDTH)) ls_pd_gv (.out(out_hv), .inp(inp_lv));

endmodule


module vclevel_shft_l2h_2x_def_hiv_gv #(parameter
   WIDTH = 1
) (
   output logic [WIDTH-1:0] out,
   input  logic [WIDTH-1:0] inp
);

   logic [WIDTH-1:0] int_buf;
   vclevel_shft_l2h_2x_def_hiv_lv #(.WIDTH(WIDTH)) ls_pd_lv (.out(int_buf), .inp(inp));
   et_combo_buf ls [WIDTH-1:0] (.x(out), .a(int_buf));

endmodule


module vclevel_shft_l2h_2x_def_hiv_lv #(parameter
   WIDTH = 1
) (
   output logic [WIDTH-1:0] out,
   input  logic [WIDTH-1:0] inp
);

   et_combo_buf ls [WIDTH-1:0] (.x(out), .a(inp));

endmodule
