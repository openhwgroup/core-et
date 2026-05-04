// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// vcfifo_wr_hiv_wr_pd_hv_gcd_wrapper

`include "macros.vh"
`include "shire_assert.vh"

module vcfifo_wr_hiv_wr_pd_hv_gcd_wrapper #(parameter
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
   logic [CNT_W-1:0]      wr_cnt_gcd;
   logic                  reset_push_sync;
   logic [CNT_W-1:0]      rd_cnt_gcd_sync;


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


   vcfifo_wr_pd_gcd #(
      .ELEM_SIZE (ELEM_SIZE),
      .NUM_ELEMS (NUM_ELEMS),
      .WR_LOV (WR_LOV),
      .DOWNSTREAM_CREDITS (DOWNSTREAM_CREDITS),
      .FF_MEM_IMPL (FF_MEM_IMPL)
   ) vcfifo_wr_pd (
      .ready               ( ready               ),
      .push                ( push                ),
      .push_data           ( push_data           ),
      .push_credit         ( push_credit         ),
      .clock_push          ( clock_push          ),
      .reset_push_sync     ( reset_push_sync     ),
      .wr_cnt_gcd          ( wr_cnt_gcd          ),
      .rd_cnt_gcd_sync     ( rd_cnt_gcd_sync     ),
      .pop_data            ( pop_data            ),
      .rd_ptr              ( rd_ptr              ),
      .dft__reset_byp_push ( dft__reset_byp_push ),
      .dft__reset_push     ( dft__reset_push     )
   );

endmodule
