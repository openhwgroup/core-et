// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// This module implements an arbiter, giving priority to the default port and then to the highest active port
// It provides a c_ready signal to a default port even if there is no c_valid signal
// for that port and unless another port has its c_valid signal active
module arb_prio_def
#(
    parameter NUM_CLIENTS = 8,
    parameter NUM_CLIENTS_LOG = $clog2(NUM_CLIENTS),
    parameter DEF_PORT    = 0
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
      // Force output to default port to depend only on ready signal as any request from this port
      // will be always granted and c_winner will be DEF_PORT. 
      c_ready[DEF_PORT] = ready;
   end

   assign valid = |c_valid;
   
   function automatic [NUM_CLIENTS_LOG-1:0] prio_encoder;
      input [NUM_CLIENTS - 1:0] v;
      begin
        prio_encoder = DEF_PORT;
        if (v[DEF_PORT])
          prio_encoder = DEF_PORT;
        else begin
          for(int i=0;i<NUM_CLIENTS;i++) begin
            if (i != DEF_PORT)
              if (v[i])
                prio_encoder = i[NUM_CLIENTS_LOG-1:0];
          end
        end
      end
   endfunction 

endmodule

