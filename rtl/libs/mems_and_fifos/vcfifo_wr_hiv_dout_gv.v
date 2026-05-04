// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vcfifo_wr_hiv_dout_gv

module vcfifo_wr_hiv_dout_gv #(parameter
   ELEM_SIZE = 32
) (

   output logic [ELEM_SIZE-1:0]  pop_data_lv,
   input  logic [ELEM_SIZE-1:0]  pop_data_gv
);

   // vclevel_shft_h2l      #(.WIDTH(    ELEM_SIZE)) ls_h2l_pop_data ( .inp_hv      (pop_data_hv), .out_lv      (pop_data_gv) );
   vcfifo_wr_hiv_dout_lv #(.ELEM_SIZE(ELEM_SIZE)) dout_pd_lv      ( .pop_data_gv (pop_data_gv), .pop_data_lv (pop_data_lv) );

endmodule

module vcfifo_wr_hiv_dout_lv #(parameter
   ELEM_SIZE = 32
) (
   output logic [ELEM_SIZE-1:0]  pop_data_lv,
   input  logic [ELEM_SIZE-1:0]  pop_data_gv
);

   vclevel_shft_h2l      #(.WIDTH(    ELEM_SIZE)) ls_h2l_pop_data ( .inp_hv      (pop_data_gv), .out_lv      (pop_data_lv) );

endmodule

