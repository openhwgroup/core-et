// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// This module implements an arbiter, giving priority to the highest index
module arb_prio
#(
    parameter NUM_CLIENTS = 8,
    parameter NUM_CLIENTS_LOG = $clog2(NUM_CLIENTS)
)
(

    // client side
    input  logic [NUM_CLIENTS-1:0]     c_valid,
    output logic [NUM_CLIENTS-1:0]     c_ready,
    output logic [NUM_CLIENTS_LOG-1:0] c_winner,
    
    // destination side
    output logic                       valid,
    input  logic                       ready
);
   
   assign c_winner = prio_encoder(c_valid);

   always_comb
   begin
      c_ready = {NUM_CLIENTS{1'b0}};
      c_ready[c_winner] = ready;
   end

   assign valid = |c_valid;
   
   function automatic [NUM_CLIENTS_LOG-1:0] prio_encoder;
      input [NUM_CLIENTS - 1:0] v;
      begin
         prio_encoder = { (NUM_CLIENTS_LOG)  {1'b0}};
         for(int i=1;i<NUM_CLIENTS;i++) begin
            if (v[i]) prio_encoder = i[NUM_CLIENTS_LOG-1:0];
         end
      end
   endfunction 

endmodule

