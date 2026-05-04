// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// This module implements an arbiter, giving priority to the highest index
// While ready from destination is low, priority is not updated if new
// valids from clients arrive
module arb_prio_dly_rdy
#(
    parameter NUM_CLIENTS = 8,
    parameter NUM_CLIENTS_LOG = $clog2(NUM_CLIENTS)
)
(
    // System signals
    input  logic                       clock,
    input  logic                       reset,

    // client side
    input  logic [NUM_CLIENTS-1:0]     c_valid,
    output logic [NUM_CLIENTS-1:0]     c_ready,
    output logic [NUM_CLIENTS_LOG-1:0] c_winner,
    
    // destination side
    output logic                       valid,
    input  logic                       ready
);

   logic [NUM_CLIENTS_LOG-1:0] dly_prio;
   logic                       busy;

   always_ff @(posedge clock)
   begin
     if (reset)
     begin
       busy     <= 1'b0;
       dly_prio <= '0;
     end
     else
     begin
       if (~ready & |c_valid & ~busy)
       begin 
         // No instant reponse when valid, store priority
         busy     <= 1'b1;
         dly_prio <= prio_encoder(c_valid); 
       end
       else if (ready & busy)
         // Cleared when response arrives
         busy <= 1'b0;
     end
   end
  
   assign c_winner = busy ? dly_prio :             // Stored priority until ready arrives
                            prio_encoder(c_valid); // Current priority

   always_comb
   begin
      c_ready           = '0;
      c_ready[c_winner] = ready;
   end

   assign valid = |c_valid;
   
   function automatic [NUM_CLIENTS_LOG-1:0] prio_encoder;
      input [NUM_CLIENTS-1:0] v;
      begin
         prio_encoder = '0;
         for (int i = 1; i < NUM_CLIENTS; i++) begin
            if (v[i]) prio_encoder = i[NUM_CLIENTS_LOG-1:0];
         end
      end
   endfunction 

endmodule

