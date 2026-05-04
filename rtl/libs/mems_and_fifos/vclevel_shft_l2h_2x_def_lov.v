// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vclevel_shft_l2h_2x_def_lov

module vclevel_shft_l2h_2x_def_lov #(parameter
   WIDTH = 1
) (
   output logic [WIDTH-1:0] out_hv,
   input  logic [WIDTH-1:0] inp_lv
);

   vclevel_shft_l2h_2x_def_lov_gv #(.WIDTH(WIDTH)) ls_pd_gv (.out(out_hv), .inp(inp_lv));

endmodule


module vclevel_shft_l2h_2x_def_lov_gv #(parameter
   WIDTH = 1
) (
   output logic [WIDTH-1:0] out,
   input  logic [WIDTH-1:0] inp
);

   logic [WIDTH-1:0] int_buf;
   et_combo_buf ls [WIDTH-1:0] (.x(int_buf), .a(inp));
   vclevel_shft_l2h_2x_def_lov_hv #(.WIDTH(WIDTH)) ls_pd_hv (.out(out), .inp(int_buf));

endmodule


module vclevel_shft_l2h_2x_def_lov_hv #(parameter
   WIDTH = 1
) (
   output logic [WIDTH-1:0] out,
   input  logic [WIDTH-1:0] inp
);

   et_combo_buf ls [WIDTH-1:0] (.x(out), .a(inp));

endmodule
