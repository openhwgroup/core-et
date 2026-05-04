// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vcfifo_wr_hiv_rd_pd_gv_gcd_wrapper

`include "macros.vh"
`include "shire_assert.vh"

module vcfifo_wr_hiv_rd_pd_gv_gcd_wrapper #(parameter
   NUM_ELEMS = 8,
   WR_LOV = 0,
   DOWNSTREAM_CREDITS = 0,
   // below considered localparam do not override
   PTR_W = $clog2(NUM_ELEMS),
   CNT_W = $clog2(NUM_ELEMS+1)
) (

   // read (pop) interface
   output logic                  valid,
   output logic                  empty,
   input  logic                  pop,   
   input  logic                  pop_credit,
   input  logic                  clock_pop,
   input  logic                  reset_pop_sync,

   // rd ptr to write domain to select rd_domain mux
   output logic [PTR_W-1:0]      rd_ptr,

   // cnt interface
   output logic [CNT_W-1:0]      rd_cnt_gcd,
   input  logic [CNT_W-1:0]      wr_cnt_gcd_sync,

   input  logic                  dft__reset_byp_pop,
   input  logic                  dft__reset_pop
);


   //----------------------------------------------------------------------------------
   // Wires
   //----------------------------------------------------------------------------------
   logic [PTR_W-1:0]      rd_ptr_lv;
   logic [CNT_W-1:0]      wr_cnt_gcd_sync_lv;
   logic [CNT_W-1:0]      rd_cnt_gcd_lv;
   logic                  reset_pop_sync_lv;

   //----------------------------------------------------------------------------------
   // Level shifters
   //----------------------------------------------------------------------------------
   vclevel_shft_l2h #(.WIDTH(PTR_W))     ls_l2h_rd_ptr              ( .inp_lv (rd_ptr_lv)           , .out_hv (rd_ptr)                );
   vclevel_shft_h2l #(.WIDTH(CNT_W))     ls_h2l_wr_cnt_gcd_sync     ( .inp_hv (wr_cnt_gcd_sync)     , .out_lv (wr_cnt_gcd_sync_lv)    );
   vclevel_shft_l2h #(.WIDTH(CNT_W))     ls_l2h_rd_cnt_gcd          ( .inp_lv (rd_cnt_gcd_lv)       , .out_hv (rd_cnt_gcd)            );
   vclevel_shft_h2l #(.WIDTH(1))         ls_h2l_reset_pop_sync      ( .inp_hv (reset_pop_sync)      , .out_lv (reset_pop_sync_lv)     );


   //----------------------------------------------------------------------------------
   // Low Voltage Domain
   //----------------------------------------------------------------------------------
   vcfifo_rd_pd_gcd #(
      .NUM_ELEMS (NUM_ELEMS),
      .WR_LOV (WR_LOV),
      .DOWNSTREAM_CREDITS (DOWNSTREAM_CREDITS)
   ) vcfifo_rd_pd_lv (
      .valid              ( valid              ),
      .empty              ( empty              ),
      .pop                ( pop                ),
      .pop_credit         ( pop_credit         ),
      .clock_pop          ( clock_pop          ),
      .reset_pop_sync     ( reset_pop_sync_lv  ),
      .rd_ptr             ( rd_ptr_lv          ),
      .rd_cnt_gcd         ( rd_cnt_gcd_lv      ),
      .wr_cnt_gcd_sync    ( wr_cnt_gcd_sync_lv ),
      .dft__reset_byp_pop ( dft__reset_byp_pop ),
      .dft__reset_pop     ( dft__reset_pop     )
   );

   
endmodule
