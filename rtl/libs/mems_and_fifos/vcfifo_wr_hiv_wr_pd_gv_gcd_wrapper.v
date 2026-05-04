// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vcfifo_wr_hiv_wr_pd_gv_gcd_wrapper

`include "macros.vh"
`include "shire_assert.vh"

module vcfifo_wr_hiv_wr_pd_gv_gcd_wrapper #(parameter
   // params for vcfifo_wr_pd_gcd
   ELEM_SIZE = 32,
   NUM_ELEMS = 8,
   WR_LOV = 0,
   DOWNSTREAM_CREDITS = 0,
   FF_MEM_IMPL = 1,
   // params for vcfifo_sync_gcd
   RD_PTR_SYNC_STAGES = 2,
   WR_PTR_SYNC_STAGES = 2,
   // below considered localparam do not override
   PTR_W = $clog2(NUM_ELEMS),
   CNT_W = $clog2(NUM_ELEMS+1)
) (

   // vcfifo_wr_pd_gcd
   // -----------------
   // write (push) interface
   output logic                  ready,
   input  logic                  push,
   input  logic [ELEM_SIZE-1:0]  push_data,
   output logic                  push_credit, // only used by credit based fifo
   input  logic                  clock_push,
   input  logic                  reset_push,
                                 
   // rd domain read interface   
   output logic [ELEM_SIZE-1:0]  pop_data,
   input  logic [PTR_W-1:0]      rd_ptr,
                                 
                                 
   // vcfifo_sync_gcd            
   // -----------------          
   output logic [CNT_W-1:0]      wr_cnt_gcd_sync,
   input  logic [CNT_W-1:0]      rd_cnt_gcd,
   input  logic                  clock_pop,
   input  logic                  reset_pop,
   output logic                  reset_pop_sync,
                                 
   // common                     
   // -----------------          
   input  logic                  dft__reset_byp_push,
   input  logic                  dft__reset_push,
   input  logic                  dft__reset_byp_pop,
   input  logic                  dft__reset_pop
);

   //----------------------------------------------------------------------------------
   // Wires
   //----------------------------------------------------------------------------------
   logic                  clock_pop_hv;
   logic                  reset_pop_hv;
   logic                  dft__reset_byp_pop_hv;
   logic                  dft__reset_pop_hv;

   logic [ELEM_SIZE-1:0]  pop_data_hv;
   logic [PTR_W-1:0]      rd_ptr_hv;
   logic [CNT_W-1:0]      wr_cnt_gcd_sync_hv;
   logic [CNT_W-1:0]      rd_cnt_gcd_hv;
   logic                  reset_pop_sync_hv;

   //----------------------------------------------------------------------------------
   // Level shifters
   //----------------------------------------------------------------------------------
   vclevel_shft_l2h #(.WIDTH(1))         ls_l2h_clock_pop           ( .inp_lv (clock_pop)           , .out_hv (clock_pop_hv)          );
   vclevel_shft_l2h #(.WIDTH(1))         ls_l2h_reset_pop           ( .inp_lv (reset_pop)           , .out_hv (reset_pop_hv)          );
   vclevel_shft_l2h #(.WIDTH(1))         ls_l2h_dft__reset_byp_pop  ( .inp_lv (dft__reset_byp_pop)  , .out_hv (dft__reset_byp_pop_hv) );
   vclevel_shft_l2h #(.WIDTH(1))         ls_l2h_dft__reset_pop      ( .inp_lv (dft__reset_pop)      , .out_hv (dft__reset_pop_hv)     );

   vclevel_shft_h2l #(.WIDTH(ELEM_SIZE)) ls_h2l_pop_data            ( .inp_hv (pop_data_hv)         , .out_lv (pop_data)              );
   vclevel_shft_l2h #(.WIDTH(PTR_W))     ls_l2h_rd_ptr              ( .inp_lv (rd_ptr)              , .out_hv (rd_ptr_hv)             );
   vclevel_shft_h2l #(.WIDTH(CNT_W))     ls_h2l_wr_cnt_gcd_sync     ( .inp_hv (wr_cnt_gcd_sync_hv)  , .out_lv (wr_cnt_gcd_sync)       );
   vclevel_shft_l2h #(.WIDTH(CNT_W))     ls_l2h_rd_cnt_gcd          ( .inp_lv (rd_cnt_gcd)          , .out_hv (rd_cnt_gcd_hv)         );
   vclevel_shft_h2l #(.WIDTH(1))         ls_h2l_reset_pop_sync      ( .inp_hv (reset_pop_sync_hv)   , .out_lv (reset_pop_sync)        );


   //----------------------------------------------------------------------------------
   // Global to High Voltage Domain
   //----------------------------------------------------------------------------------
   vcfifo_wr_hiv_wr_pd_hv_gcd_wrapper #(
      .ELEM_SIZE (ELEM_SIZE),
      .NUM_ELEMS (NUM_ELEMS),
      .WR_LOV (WR_LOV),
      .DOWNSTREAM_CREDITS (DOWNSTREAM_CREDITS),
      .FF_MEM_IMPL (FF_MEM_IMPL),
      .RD_PTR_SYNC_STAGES (RD_PTR_SYNC_STAGES),
      .WR_PTR_SYNC_STAGES (WR_PTR_SYNC_STAGES),
      .PTR_W (PTR_W),
      .CNT_W (CNT_W) 
   ) vcfifo_wr_hiv_wr_pd_hv (
      .ready               ( ready                 ),
      .push                ( push                  ),
      .push_data           ( push_data             ),
      .push_credit         ( push_credit           ),
      .clock_push          ( clock_push            ),
      .reset_push          ( reset_push            ),
      .pop_data            ( pop_data_hv           ),
      .rd_ptr              ( rd_ptr_hv             ),
      .wr_cnt_gcd_sync     ( wr_cnt_gcd_sync_hv    ),
      .rd_cnt_gcd          ( rd_cnt_gcd_hv         ),
      .clock_pop           ( clock_pop_hv          ),
      .reset_pop           ( reset_pop_hv          ),
      .reset_pop_sync      ( reset_pop_sync_hv     ),
      .dft__reset_byp_push ( dft__reset_byp_push   ),
      .dft__reset_push     ( dft__reset_push       ),
      .dft__reset_byp_pop  ( dft__reset_byp_pop_hv ),
      .dft__reset_pop      ( dft__reset_pop_hv     )
   );

endmodule
