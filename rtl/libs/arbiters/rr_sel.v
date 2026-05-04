// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// This module implements selection for a round robin queue
module rr_sel
  #(
    parameter NUM_CLIENTS=8,
    parameter NUM_CLIENTS_LOG = $clog2(NUM_CLIENTS)
    )
   (
    input logic                       clock,
    input logic                       reset,
    
    input [NUM_CLIENTS-1:0]           req,
    input                             ready,
    
    output                            newReq,
    output reg [NUM_CLIENTS_LOG -1:0] selIdx
    );
   
   logic [NUM_CLIENTS-1:0]  mask;
   logic [NUM_CLIENTS-1:0]  nextMask;
   
   assign newReq = (|req) && ready;
   `RST_EN_FF(clock, reset, newReq, mask, nextMask, '0)
   always_comb begin
      selIdx = NUM_CLIENTS - 1;
      for (int i = NUM_CLIENTS - 2 ; i>=0; i-- ) 
        if (req[i]) selIdx = i;
      for (int i = NUM_CLIENTS - 1; i >=0; i-- )
        if (req[i] && mask[i]) selIdx = i;
      for (int i = 0 ; i < NUM_CLIENTS; i++)
        nextMask[i] = i > selIdx;
   end
   
endmodule
