// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vcfifo_wr_lov_wr_pd_gv_gcd_wrapper

`include "macros.vh"
`include "shire_assert.vh"

module vcfifo_wr_lov_wr_pd_gv_gcd_wrapper #(parameter
   ELEM_SIZE = 32,
   NUM_ELEMS = 8,
   WR_LOV = 0,
   DOWNSTREAM_CREDITS = 0,
   FF_MEM_IMPL = 1,
   // below considered localparam do not override
   PTR_W = $clog2(NUM_ELEMS),
   CNT_W = $clog2(NUM_ELEMS+1)
) (

   // write (push) interface
   output logic                  ready,
   input  logic                  push,
   input  logic [ELEM_SIZE-1:0]  push_data,
   output logic                  push_credit, // only used by credit based fifo
   input  logic                  clock_push,
   input logic                   reset_push_sync,
                                 
   // cnt interface              
   output logic [CNT_W-1:0]      wr_cnt_gcd,
   input  logic [CNT_W-1:0]      rd_cnt_gcd_sync,
                                 
   // rd domain read interface   
   output logic [ELEM_SIZE-1:0]  pop_data,
   input  logic [PTR_W-1:0]      rd_ptr,
                                 
   input  logic                  dft__reset_byp_push,
   input  logic                  dft__reset_push

);


   //----------------------------------------------------------------------------------
   // Wires
   //----------------------------------------------------------------------------------
   logic [ELEM_SIZE-1:0] pop_data_lv;
   logic [PTR_W-1:0]     rd_ptr_lv; 
   logic [CNT_W-1:0]     wr_cnt_gcd_lv; 
   logic [CNT_W-1:0]     rd_cnt_gcd_sync_lv; 
   logic                 reset_push_sync_lv;

   //----------------------------------------------------------------------------------
   // Level shifters
   //----------------------------------------------------------------------------------
   vclevel_shft_l2h #(.WIDTH(ELEM_SIZE)) ls_l2h_pop_data            ( .inp_lv (pop_data_lv)         , .out_hv (pop_data)           );
   vclevel_shft_h2l #(.WIDTH(PTR_W))     ls_h2l_rd_ptr              ( .inp_hv (rd_ptr)              , .out_lv (rd_ptr_lv)          );
   vclevel_shft_l2h #(.WIDTH(CNT_W))     ls_l2h_wr_cnt_gcd          ( .inp_lv (wr_cnt_gcd_lv)       , .out_hv (wr_cnt_gcd)         );
   vclevel_shft_h2l #(.WIDTH(CNT_W))     ls_h2l_rd_cnt_gcd_sync     ( .inp_hv (rd_cnt_gcd_sync)     , .out_lv (rd_cnt_gcd_sync_lv) );
   vclevel_shft_h2l #(.WIDTH(1))         ls_h2l_reset_push_sync     ( .inp_hv (reset_push_sync)     , .out_lv (reset_push_sync_lv) );


   //----------------------------------------------------------------------------------
   // Low Voltage Domain
   //----------------------------------------------------------------------------------
   vcfifo_wr_pd_gcd #(
      .ELEM_SIZE (ELEM_SIZE),
      .NUM_ELEMS (NUM_ELEMS),
      .WR_LOV (WR_LOV),
      .DOWNSTREAM_CREDITS (DOWNSTREAM_CREDITS),
      .FF_MEM_IMPL (FF_MEM_IMPL)
   ) vcfifo_wr_pd_lv (
      .ready               ( ready               ),
      .push                ( push                ),
      .push_data           ( push_data           ),
      .push_credit         ( push_credit         ),
      .clock_push          ( clock_push          ),
      .reset_push_sync     ( reset_push_sync_lv  ),
      .wr_cnt_gcd          ( wr_cnt_gcd_lv       ),
      .rd_cnt_gcd_sync     ( rd_cnt_gcd_sync_lv  ),
      .pop_data            ( pop_data_lv         ),
      .rd_ptr              ( rd_ptr_lv           ),
      .dft__reset_byp_push ( dft__reset_byp_push ),
      .dft__reset_push     ( dft__reset_push     )
   );
   

endmodule
