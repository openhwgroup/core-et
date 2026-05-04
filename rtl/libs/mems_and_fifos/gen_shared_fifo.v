// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


// parameter ARB_TYPE = 0 for priority encoder, 1 for lru
module gen_shared_fifo  #(parameter WIDTH=20, parameter DEPTH=16, parameter WRITERS=1, parameter ARB_TYPE=0 )
   (
    // System signals
    input                          clock,
    input                          reset,
    
    // Push data
    input [WRITERS-1:0]            push_valid,
    output [WRITERS-1:0]           push_ready,
    input [WRITERS-1:0][WIDTH-1:0] push_data,
    
    // Pop data
    output                         pop_valid,
    input                          pop_ready,
    output [WIDTH-1:0]             pop_data
    );
   
   localparam LOG_WRITERS = $clog2(WRITERS);
   
   // push side 
   
   wire                            fifo_push;
   wire                            fifo_full;
   wire [WIDTH-1:0]                fifo_push_data;
   wire [LOG_WRITERS-1:0]          push_winner;
   wire                            fifo_pop;

   generate if (ARB_TYPE==0) begin: ARB_PRIO
      wire                            fifo_push_valid;
      wire                            fifo_push_ready;
      arb_prio #( .NUM_CLIENTS(WRITERS) ) arb
        (
         .c_valid(push_valid),
         .c_ready(push_ready),
         .c_winner(push_winner),
         .valid(fifo_push_valid),
         .ready(fifo_push_ready)
         );
      assign fifo_push_ready = !fifo_full;
      assign fifo_push = fifo_push_ready && fifo_push_valid;
   end // block: ARB
   else begin: ARB_LRU
      arb_lru  #( .NUM_CLIENTS(WRITERS) ) arb
        (
         .clock(clock),
         .reset(reset),
         .bid(push_valid),
         .stall(fifo_full),
         .grant(push_ready), 
         .winner(push_winner)
         );
      assign fifo_push = |push_ready;
   end // else: !if(ARB_TYPE==0)
   endgenerate
   
   gen_fifo #(.ELEM_SIZE(WIDTH), .NUM_ELEMS(DEPTH), .USE_LATCHES(0) ) fifo
     (
      .clock(clock),
      .reset(reset),
      .push(fifo_push),
      .push_early(1'b0), // Only used if implemented with latches
      .push_data(fifo_push_data),
      .full(fifo_full),
      .full_next(),
      .pop(fifo_pop),
      .pop_data(pop_data),
      .valid(pop_valid),
      .valid_next()
      );
   
   assign fifo_push_data = push_data[push_winner];
   assign fifo_pop = pop_valid && pop_ready;
   
endmodule
