// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"
`include "shire_assert.vh"

// vcfifo_rd_pd_ss - voltage change semisync fifo read power domain logic
// 
//   wrapper module for read/pop power domain logic
//   this module can exist in either the high
//   or low voltage power domains


module vcfifo_rd_pd_ss #(parameter
   ELEM_SIZE       = 32,  // WIDTH
   NUM_ELEMS       = 3,   // DEPTH
   PUSH_RELAY      = 0,   // set to 1 to use an extra relay stage on the push side
   POP_RELAY       = 0    // set to 1 to use an extra relay stage on the push side
) (
   input  logic                  clock_pop,
   input  logic                  reset_pop,
   input  logic                  reset_push,

   input  logic                  pop,
   output logic                  valid,
   output logic [ELEM_SIZE-1:0]  pop_data,

   input  logic                  push_d1,
   input  logic [ELEM_SIZE-1:0]  push_data_d1,
   output logic                  pop_d1,

   input  logic                  dft__reset_byp_pop,
   input  logic                  dft__reset_pop
);


   localparam WIDTH = ELEM_SIZE;
   localparam DEPTH = NUM_ELEMS + PUSH_RELAY + POP_RELAY;

   //----------------------------------------------------------------------------------
   // reset synchronizer
   //----------------------------------------------------------------------------------
   wire reset_sync;
   
   sync_rst  sync_rst (
      .reset_out      ( reset_sync              ), 
      .reset_in       ( reset_push || reset_pop ),
      .clock          ( clock_pop               ),
      .dft__reset_byp ( dft__reset_byp_pop      ),
      .dft__reset     ( dft__reset_pop          )
   );


   //----------------------------------------------------------------------------------
   // Pop Voltage Domain
   //----------------------------------------------------------------------------------

   wire              really_pop = pop && valid; // only indicate pop to write domain when valid data is taken
   logic             push_d2;
   logic [WIDTH-1:0] push_data_d2;
   `RST_EN_FF(clock_pop, reset_sync, pop_d1 || really_pop,   pop_d1,       really_pop,   '0)
   `RST_EN_FF(clock_pop, reset_sync, push_d2 || push_d1,     push_d2,      push_d1,      '0)
   `RST_EN_FF(clock_pop, reset_sync, push_d1,                push_data_d2, push_data_d1, '0)

   wire             push_in      = PUSH_RELAY ? push_d2      : push_d1;
   wire [WIDTH-1:0] push_data_in = PUSH_RELAY ? push_data_d2 : push_data_d1;

   logic           fifo_full_unused;
   logic [DEPTH:0] fifo_level_unused;
   gen_fifo_reg #(
      .WIDTH(WIDTH),
      .DEPTH(DEPTH)
   ) dest_fifo (
      .clock ( clock_pop                ),
      .reset ( reset_sync               ),
      .push  ( push_in                  ),
      .wdata ( push_data_in             ),
      .full  ( fifo_full_unused         ),
      .level ( fifo_level_unused        ),
      .pop   ( pop                      ),
      .valid ( valid                    ),
      .rdata ( pop_data                 )
   );
   
   // with credit based design, should never push when full
   `ASSERT_ALWAYS(ERROR_VCFIFO_RD_PS_SS_push_dest_fifo_on_full,  clock_pop, reset_sync, push_in |-> !fifo_full_unused )
   // should not get a push while still in reset
   `ASSERT_ALWAYS(ERROR_VCFIFO_RD_PS_SS_push_in_on_reset,        clock_pop, reset_push || reset_pop, push_in |-> !reset_sync )
   `ASSERT_ALWAYS(ERROR_VCFIFO_RD_PS_SS_push_dest_fifo_on_reset, clock_pop, reset_push || reset_pop, push_d1 |-> !reset_sync )

      
endmodule
