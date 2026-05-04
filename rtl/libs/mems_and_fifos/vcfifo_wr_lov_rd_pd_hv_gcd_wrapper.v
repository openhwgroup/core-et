// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vcfifo_wr_lov_rd_pd_hv_gcd_wrapper

`include "macros.vh"
`include "shire_assert.vh"

module vcfifo_wr_lov_rd_pd_hv_gcd_wrapper #(parameter
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
   logic [CNT_W-1:0]      rd_cnt_gcd;
   logic                  reset_pop_sync;
   logic [CNT_W-1:0]      wr_cnt_gcd_sync;


   //----------------------------------------------------------------------------------
   // High Voltage Domain
   //----------------------------------------------------------------------------------
   vcfifo_sync_gcd #(
      .NUM_ELEMS (NUM_ELEMS),
      .RD_PTR_SYNC_STAGES (RD_PTR_SYNC_STAGES),
      .WR_PTR_SYNC_STAGES (WR_PTR_SYNC_STAGES),
      .WR_LOV (WR_LOV) 
   ) vcfifo_sync (
      .wr_cnt_gcd          ( wr_cnt_gcd          ),
      .wr_cnt_gcd_sync     ( wr_cnt_gcd_sync     ),
      .rd_cnt_gcd          ( rd_cnt_gcd          ),
      .rd_cnt_gcd_sync     ( rd_cnt_gcd_sync     ),
      .clock_push          ( clock_push          ),
      .clock_pop           ( clock_pop           ),
      .reset_push          ( reset_push          ),
      .reset_push_sync     ( reset_push_sync     ),
      .reset_pop           ( reset_pop           ),
      .reset_pop_sync      ( reset_pop_sync      ),
      .dft__reset_byp_push ( dft__reset_byp_push ),
      .dft__reset_push     ( dft__reset_push     ),
      .dft__reset_byp_pop  ( dft__reset_byp_pop  ),
      .dft__reset_pop      ( dft__reset_pop      )
   );

                
   vcfifo_rd_pd_gcd #(
      .NUM_ELEMS (NUM_ELEMS),
      .WR_LOV (WR_LOV),
      .DOWNSTREAM_CREDITS (DOWNSTREAM_CREDITS)
   ) vcfifo_rd_pd (
      .valid              ( valid                 ),
      .empty              ( empty                 ),
      .pop                ( pop                   ),
      .pop_credit         ( pop_credit            ),
      .clock_pop          ( clock_pop             ),
      .reset_pop_sync     ( reset_pop_sync        ),
      .rd_ptr             ( rd_ptr                ),
      .rd_cnt_gcd         ( rd_cnt_gcd            ),
      .wr_cnt_gcd_sync    ( wr_cnt_gcd_sync       ),
      .dft__reset_byp_pop ( dft__reset_byp_pop    ),
      .dft__reset_pop     ( dft__reset_pop        )
   );

   
endmodule
