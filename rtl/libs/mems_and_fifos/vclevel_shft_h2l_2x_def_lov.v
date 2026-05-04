// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vclevel_shft_h2l_2x_def_lov

module vclevel_shft_h2l_2x_def_lov #(parameter
   WIDTH = 1
) (
   output logic [WIDTH-1:0] out_lv,
   input  logic [WIDTH-1:0] inp_hv
);

   vclevel_shft_h2l_2x_def_lov_gv #(.WIDTH(WIDTH)) ls_pd_gv (.out(out_lv), .inp(inp_hv));

endmodule


module vclevel_shft_h2l_2x_def_lov_gv #(parameter
   WIDTH = 1
) (
   output logic [WIDTH-1:0] out,
   input  logic [WIDTH-1:0] inp
);

   logic [WIDTH-1:0] int_buf;
   vclevel_shft_h2l_2x_def_lov_hv #(.WIDTH(WIDTH)) ls_pd_hv (.out(int_buf), .inp(inp));
   et_combo_buf ls [WIDTH-1:0] (.x(out), .a(int_buf));

endmodule


module vclevel_shft_h2l_2x_def_lov_hv #(parameter
   WIDTH = 1
) (
   output logic [WIDTH-1:0] out,
   input  logic [WIDTH-1:0] inp
);

   et_combo_buf ls [WIDTH-1:0] (.x(out), .a(inp));

endmodule
