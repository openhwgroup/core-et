// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// fifo_gcd (Async Fifo) - based from vcfifo*gcd design
//
//    wrapper module to create generic aync fifo from vcfifo building blocks

module fifo_gcd #(parameter
   ELEM_SIZE = 32,
   NUM_ELEMS = 8,
   WR_PTR_SYNC_STAGES = 2,
   RD_PTR_SYNC_STAGES = 2,
   FF_MEM_IMPL = 1   // set to 1 to use flop based memory vs latch based memory
) (
   input  logic                  clock_push,
   input  logic                  reset_push,
   input  logic                  clock_pop,
   input  logic                  reset_pop,

   output logic                  ready,
   input  logic                  push,
   input  logic [ELEM_SIZE-1:0]  push_data,

   output logic                  valid,
   input  logic                  pop,
   output logic [ELEM_SIZE-1:0]  pop_data,

   input  logic                  dft__reset_byp_push,
   input  logic                  dft__reset_push,
   input  logic                  dft__reset_byp_pop,
   input  logic                  dft__reset_pop

);

   localparam WR_LOV             = 0;
   localparam DOWNSTREAM_CREDITS = 0;
   localparam PTR_W              = $clog2(NUM_ELEMS);
   localparam CNT_W              = $clog2(NUM_ELEMS+1);


   //----------------------------------------------------------------------------------
   // Wires
   //----------------------------------------------------------------------------------
   logic [CNT_W-1:0]     wr_cnt_gcd_hv; 
   logic [CNT_W-1:0]     wr_cnt_gcd_sync_hv; 

   logic [CNT_W-1:0]     rd_cnt_gcd_hv;
   logic [CNT_W-1:0]     rd_cnt_gcd_sync_hv;

   logic [PTR_W-1:0]     rd_ptr_hv; 
   logic [ELEM_SIZE-1:0] pop_data_hv;

   logic                 reset_push_hv;
   logic                 reset_push_sync_hv;
   logic                 reset_pop_hv;
   logic                 reset_pop_sync_hv;

   logic                 clock_push_hv;
   logic                 clock_pop_hv;

   logic                 dft__reset_byp_push_hv;
   logic                 dft__reset_push_hv;
   logic                 dft__reset_byp_pop_hv;
   logic                 dft__reset_pop_hv;


   //----------------------------------------------------------------------------------
   // Synchronization module
   //----------------------------------------------------------------------------------
   vcfifo_sync_gcd #(
      .NUM_ELEMS (NUM_ELEMS),
      .RD_PTR_SYNC_STAGES (RD_PTR_SYNC_STAGES),
      .WR_PTR_SYNC_STAGES (WR_PTR_SYNC_STAGES),
      .WR_LOV (WR_LOV),
      .FIFO_GCD (1)  // set to 1 to prevent inverting either clock into the synchronizers
   ) vcfifo_sync (
      .wr_cnt_gcd          ( wr_cnt_gcd_hv          ),
      .wr_cnt_gcd_sync     ( wr_cnt_gcd_sync_hv     ),
      .rd_cnt_gcd          ( rd_cnt_gcd_hv          ),
      .rd_cnt_gcd_sync     ( rd_cnt_gcd_sync_hv     ),
      .clock_push          ( clock_push_hv          ),
      .clock_pop           ( clock_pop_hv           ),
      .reset_push          ( reset_push_hv          ),
      .reset_push_sync     ( reset_push_sync_hv     ),
      .reset_pop           ( reset_pop_hv           ),
      .reset_pop_sync      ( reset_pop_sync_hv      ),
      .dft__reset_byp_push ( dft__reset_byp_push_hv ),
      .dft__reset_push     ( dft__reset_push_hv     ),
      .dft__reset_byp_pop  ( dft__reset_byp_pop_hv  ),
      .dft__reset_pop      ( dft__reset_pop_hv      )
   );


   //----------------------------------------------------------------------------------
   // Write Domain
   //----------------------------------------------------------------------------------
   wire push_credit_unused_ok;
   vcfifo_wr_pd_gcd #(
      .ELEM_SIZE (ELEM_SIZE),
      .NUM_ELEMS (NUM_ELEMS),
      .WR_LOV (WR_LOV), // with WR_LOV=0, no resync of reset/rd_cnt_gcd_sync will be used
      .DOWNSTREAM_CREDITS (DOWNSTREAM_CREDITS),
      .FF_MEM_IMPL (FF_MEM_IMPL)
   ) vcfifo_wr (
      .ready               ( ready                  ),
      .push                ( push                   ),
      .push_data           ( push_data              ),
      .push_credit         ( push_credit_unused_ok  ),
      .clock_push          ( clock_push             ),
      .reset_push_sync     ( reset_push_sync_hv     ),
      .wr_cnt_gcd          ( wr_cnt_gcd_hv          ),
      .rd_cnt_gcd_sync     ( rd_cnt_gcd_sync_hv     ),
      .pop_data            ( pop_data_hv            ),
      .rd_ptr              ( rd_ptr_hv              ),
      .dft__reset_byp_push ( dft__reset_byp_push_hv ),
      .dft__reset_push     ( dft__reset_push_hv     )
   );

                
   //----------------------------------------------------------------------------------
   // Read Domain
   //----------------------------------------------------------------------------------
   wire pop_credit_zero = 1'b0;
   wire empty_unused_ok;
   vcfifo_rd_pd_gcd #(
      .NUM_ELEMS (NUM_ELEMS),
      .WR_LOV (1), // with WR_LOV=1, no resync of reset/wr_cnt_gcd_sync will be use
      .DOWNSTREAM_CREDITS (DOWNSTREAM_CREDITS)
   ) vcfifo_rd (
      .valid              ( valid                 ),
      .empty              ( empty_unused_ok       ),
      .pop                ( pop                   ),
      .pop_credit         ( pop_credit_zero       ),
      .clock_pop          ( clock_pop             ),
      .reset_pop_sync     ( reset_pop_sync_hv     ),
      .rd_ptr             ( rd_ptr_hv             ),
      .rd_cnt_gcd         ( rd_cnt_gcd_hv         ),
      .wr_cnt_gcd_sync    ( wr_cnt_gcd_sync_hv    ),
      .dft__reset_byp_pop ( dft__reset_byp_pop_hv ),
      .dft__reset_pop     ( dft__reset_pop_hv     )

   );
      
   //----------------------------------------------------------------------------------
   // Input/Output assignments
   //----------------------------------------------------------------------------------

   // assign inputs
   assign clock_push_hv          = clock_push;
   assign reset_push_hv          = reset_push;
   assign dft__reset_byp_push_hv = dft__reset_byp_push;
   assign dft__reset_push_hv     = dft__reset_push;

   assign clock_pop_hv           = clock_pop;
   assign reset_pop_hv           = reset_pop;
   assign dft__reset_byp_pop_hv  = dft__reset_byp_pop;
   assign dft__reset_pop_hv      = dft__reset_pop;

   // assign outputs
   assign pop_data               = pop_data_hv;
   



endmodule
