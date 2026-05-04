// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"
`include "shire_assert.vh"

// vcfifo_rd_pd_ss - voltage change semisync fifo read power domain logic
// 
//   wrapper module for write/push power domain logic
//   this module can exist in either the high
//   or low voltage power domains


module vcfifo_wr_pd_ss #(parameter
   ELEM_SIZE       = 32,  // WIDTH
   NUM_ELEMS       = 3,   // DEPTH
   PUSH_RELAY      = 0,   // set to 1 to use an extra relay stage on the push side
   POP_RELAY       = 0    // set to 1 to use an extra relay stage on the pop side
) (
   input  logic                  clock_push,
   input  logic                  reset_push,
   input  logic                  reset_pop,

   input  logic                  push,
   input  logic [ELEM_SIZE-1:0]  push_data,
   output logic                  ready,

   output logic                  push_d1,
   output logic [ELEM_SIZE-1:0]  push_data_d1,
   input  logic                  pop_d1,

   input  logic                  dft__reset_byp_push,
   input  logic                  dft__reset_push
);


   localparam DEPTH = NUM_ELEMS + PUSH_RELAY + POP_RELAY;
   localparam CNT_W = $clog2(DEPTH+1);


   //----------------------------------------------------------------------------------
   // reset synchronizer
   //----------------------------------------------------------------------------------
   wire reset_sync;
   
   sync_rst sync_rst (
      .reset_out      ( reset_sync              ), 
      .reset_in       ( reset_push || reset_pop ),
      .clock          ( clock_push              ),
      .dft__reset_byp ( dft__reset_byp_push     ),
      .dft__reset     ( dft__reset_push         )

   );


   //----------------------------------------------------------------------------------
   // Push Voltage Domain
   //----------------------------------------------------------------------------------

   wire              really_push = push && ready; // only indicate push to read domain when pushing and !full
   logic             pop_d2;

   `RST_EN_FF(clock_push, reset_sync, pop_d2 || pop_d1,           pop_d2,        pop_d1,      '0)
   `RST_EN_FF(clock_push, reset_sync, push_d1 || really_push,     push_d1,       really_push, '0)
   `EN_FF(    clock_push,             really_push,                push_data_d1,  push_data)

   wire pop_in = POP_RELAY ? pop_d2 : pop_d1;

   // credits/full
   //logic             full;
   logic             full_next;
   logic [CNT_W-1:0] credits_next;
   logic [CNT_W-1:0] credits;
   always_comb begin
      // if  really_push ==  pop_in hold, i.e. if both really_push/pop hold, if neither really_push/pop hold
      // if  really_push && !pop_in increment
      // if !really_push &&  pop_in decrement
      // NOTE: timing update: credit flop is only enabled if really_push ^ pop_in = 1
      //       so simplify to only check pop_in and reverse above mux order
      //credits_next = (really_push && !pop_in) ? credits + 1'b1 : credits - 1'b1;
      credits_next = pop_in ? credits - 1'b1 : credits + 1'b1;
      full_next = (credits_next == DEPTH);
   end
   `RST_EN_FF(clock_push, reset_sync, really_push ^ pop_in, credits, credits_next, '0)

   // NOTE: some designs are attempting to push while fifo is in reset, so prevent
   //       ready from being high during reset to be safe
   // NOTE: timing update, register ready out, the below logic takes one cycle longer to
   //       squash ready upon reset_sync and goes high one cycle after reset_sync is released
   //       This counts on no data being pushed 5 cycles after the last reset is released
   //       and doesn't care if it takes a cycle longer going into reset to cause 
   //       full to be cleared.
   // NOTE: with timing updated, need to set ready upon reset_sync release otherwise it
   //       would stays stuck low.  really_push is guaranteed to be low on reset_sync_fe
   // `RST_EN_FF(clock_push, reset_sync, really_push ^ pop_in, full,    full_next,    '0)
   // assign ready = !full && !reset_sync;
   logic reset_sync_d1;
   `RST_FF(clock_push, reset_sync, reset_sync_d1, reset_sync, '1)
   wire reset_sync_fe = !reset_sync && reset_sync_d1;
   `RST_EN_FF(clock_push, reset_sync, really_push ^ (pop_in || reset_sync_fe), ready, ~full_next || reset_sync_fe, '0)
   // 

   // should not get an incoming push while still in reset
   // `ASSERT_ALWAYS(ERROR_VCFIFO_WR_PD_SS_push_vcfifo_on_full,  clock_push, reset_push || reset_pop, push |-> !reset_sync )

      
endmodule
