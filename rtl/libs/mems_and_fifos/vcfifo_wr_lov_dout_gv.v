// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vcfifo_wr_lov_dout_gv

module vcfifo_wr_lov_dout_gv #(parameter
   ELEM_SIZE = 32
) (

   output logic [ELEM_SIZE-1:0]  pop_data_hv,
   input  logic [ELEM_SIZE-1:0]  pop_data_gv
);

   //vclevel_shft_l2h      #(.WIDTH(    ELEM_SIZE)) ls_l2h_pop_data ( .inp_lv      (pop_data_lv), .out_hv      (pop_data_gv) );
   vcfifo_wr_lov_dout_hv #(.ELEM_SIZE(ELEM_SIZE)) dout_pd_hv      ( .pop_data_gv (pop_data_gv), .pop_data_hv (pop_data_hv) );

endmodule

module vcfifo_wr_lov_dout_hv #(parameter
   ELEM_SIZE = 32
) (
   output logic [ELEM_SIZE-1:0]  pop_data_hv,
   input  logic [ELEM_SIZE-1:0]  pop_data_gv
);

   vclevel_shft_l2h      #(.WIDTH(    ELEM_SIZE)) ls_l2h_pop_data ( .inp_lv      (pop_data_gv), .out_hv      (pop_data_hv) );

endmodule

