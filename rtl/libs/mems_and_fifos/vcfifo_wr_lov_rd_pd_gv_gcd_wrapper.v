// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vcfifo_wr_lov_rd_pd_gv_gcd_wrapper

`include "macros.vh"
`include "shire_assert.vh"

module vcfifo_wr_lov_rd_pd_gv_gcd_wrapper #(parameter
   // params for vcfifo_rd_pd_gcd
   NUM_ELEMS = 8,
   WR_LOV = 0,
   DOWNSTREAM_CREDITS = 0,
   // params for vcfifo_sync_gcd
   RD_PTR_SYNC_STAGES = 2,
   WR_PTR_SYNC_STAGES = 2,
   // below considered localparam do not override
   PTR_W = $clog2(NUM_ELEMS),
   CNT_W = $clog2(NUM_ELEMS+1)
) (

   // vcfifo_rd_pd_gcd
   // -----------------
   // read (pop) interface
   output logic                  valid,
   output logic                  empty,
   input  logic                  pop,   
   input  logic                  pop_credit,
   input  logic                  clock_pop,
   input  logic                  reset_pop,

   // rd ptr to write domain to select rd_domain mux
   output logic [PTR_W-1:0]      rd_ptr,

   // vcfifo_sync_gcd
   // -----------------
   // cnt interface
   input  logic [CNT_W-1:0]      wr_cnt_gcd,
   output logic [CNT_W-1:0]      rd_cnt_gcd_sync,
   input  logic                  clock_push,
   input  logic                  reset_push,
   output logic                  reset_push_sync,

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
   logic                 clock_push_hv;
   logic                 reset_push_hv;
   logic                 dft__reset_byp_push_hv;
   logic                 dft__reset_push_hv;

   logic [PTR_W-1:0]     rd_ptr_hv; 
   logic [CNT_W-1:0]     wr_cnt_gcd_hv; 
   logic [CNT_W-1:0]     rd_cnt_gcd_sync_hv;
   logic                 reset_push_sync_hv;


   vclevel_shft_l2h #(.WIDTH(1))         ls_l2h_clock_push          ( .inp_lv (clock_push)           , .out_hv (clock_push_hv)          );
   vclevel_shft_l2h #(.WIDTH(1))         ls_l2h_reset_push          ( .inp_lv (reset_push)           , .out_hv (reset_push_hv)          );
   vclevel_shft_l2h #(.WIDTH(1))         ls_l2h_dft__reset_byp_push ( .inp_lv (dft__reset_byp_push)  , .out_hv (dft__reset_byp_push_hv) );
   vclevel_shft_l2h #(.WIDTH(1))         ls_l2h_dft__reset_push     ( .inp_lv (dft__reset_push)      , .out_hv (dft__reset_push_hv)     );

   vclevel_shft_h2l #(.WIDTH(PTR_W))     ls_h2l_rd_ptr              ( .inp_hv (rd_ptr_hv)            , .out_lv (rd_ptr)                 );
   vclevel_shft_l2h #(.WIDTH(CNT_W))     ls_l2h_wr_cnt_gcd          ( .inp_lv (wr_cnt_gcd)           , .out_hv (wr_cnt_gcd_hv)          );
   vclevel_shft_h2l #(.WIDTH(CNT_W))     ls_h2l_rd_cnt_gcd_sync     ( .inp_hv (rd_cnt_gcd_sync_hv)   , .out_lv (rd_cnt_gcd_sync)        );
   vclevel_shft_h2l #(.WIDTH(1))         ls_h2l_reset_push_sync     ( .inp_hv (reset_push_sync_hv)   , .out_lv (reset_push_sync)        );


   //----------------------------------------------------------------------------------
   // Global to High Voltage Domain
   //----------------------------------------------------------------------------------
   vcfifo_wr_lov_rd_pd_hv_gcd_wrapper #(
      .NUM_ELEMS (NUM_ELEMS),
      .WR_LOV (WR_LOV),
      .DOWNSTREAM_CREDITS (DOWNSTREAM_CREDITS),
      .RD_PTR_SYNC_STAGES (RD_PTR_SYNC_STAGES),
      .WR_PTR_SYNC_STAGES (WR_PTR_SYNC_STAGES)
   ) vcfifo_wr_lov_rd_pd_hv (
      .valid               ( valid                  ),
      .empty               ( empty                  ),
      .pop                 ( pop                    ),
      .pop_credit          ( pop_credit             ),
      .clock_pop           ( clock_pop              ),
      .reset_pop           ( reset_pop              ),
      .rd_ptr              ( rd_ptr_hv              ),
      .wr_cnt_gcd          ( wr_cnt_gcd_hv          ),
      .rd_cnt_gcd_sync     ( rd_cnt_gcd_sync_hv     ),
      .clock_push          ( clock_push_hv          ),
      .reset_push          ( reset_push_hv          ),
      .reset_push_sync     ( reset_push_sync_hv     ),
      .dft__reset_byp_push ( dft__reset_byp_push_hv ),
      .dft__reset_push     ( dft__reset_push_hv     ),
      .dft__reset_byp_pop  ( dft__reset_byp_pop     ),
      .dft__reset_pop      ( dft__reset_pop         )
   );
   

endmodule
