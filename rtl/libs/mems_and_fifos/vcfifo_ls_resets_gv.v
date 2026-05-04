// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vcfifo_ls_resets_gv

module vcfifo_ls_resets_gv (
   output clock_gv,
   output reset_gv,
   output dft__reset_byp_gv,
   output dft__reset_gv,

   input  clock_lv,
   input  reset_lv,
   input  dft__reset_byp_lv,
   input  dft__reset_lv
);

   vcfifo_ls_resets_lv resets_pd_lv (
      .clock_gv          ( clock_gv          ),
      .reset_gv          ( reset_gv          ),
      .dft__reset_byp_gv ( dft__reset_byp_gv ),
      .dft__reset_gv     ( dft__reset_gv     ),
      .clock_lv          ( clock_lv          ),
      .reset_lv          ( reset_lv          ),
      .dft__reset_byp_lv ( dft__reset_byp_lv ),
      .dft__reset_lv     ( dft__reset_lv     )
   );

endmodule

module vcfifo_ls_resets_lv (
   output clock_gv,
   output reset_gv,
   output dft__reset_byp_gv,
   output dft__reset_gv,

   input  clock_lv,
   input  reset_lv,
   input  dft__reset_byp_lv,
   input  dft__reset_lv
);

   vclevel_shft_l2h      #(.WIDTH( 1)) ls_l2h_clock          ( .inp_lv (clock_lv)         , .out_hv (clock_gv)          );
   vclevel_shft_l2h      #(.WIDTH( 1)) ls_l2h_reset          ( .inp_lv (reset_lv)         , .out_hv (reset_gv)          );
   vclevel_shft_l2h      #(.WIDTH( 1)) ls_l2h_dft__reset_byp ( .inp_lv (dft__reset_byp_lv), .out_hv (dft__reset_byp_gv) );
   vclevel_shft_l2h      #(.WIDTH( 1)) ls_l2h_dft__reset     ( .inp_lv (dft__reset_lv)    , .out_hv (dft__reset_gv)     );

endmodule

