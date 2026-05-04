// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// This module implements an arbiter, giving priority to the highest index
module arb_prio_data
#(
    parameter WIDTH = 32,
    parameter NUM_CLIENTS = 8,
    parameter NUM_CLIENTS_LOG = $clog2(NUM_CLIENTS)
)
(
    // Bidding
    input  logic [NUM_CLIENTS-1:0]            bid,
    input  logic                              stall,
    // Data inputs
    input  logic [NUM_CLIENTS-1:0][WIDTH-1:0] data_in,
    // Result
    output logic [NUM_CLIENTS-1:0]            grant,
    output logic [WIDTH-1:0]                  data_winner
);

logic [NUM_CLIENTS_LOG-1:0] winner;
logic [NUM_CLIENTS-1:0]     ready;
logic                       valid;

// Arbiter
arb_prio
#(
    .NUM_CLIENTS ( NUM_CLIENTS )
)
arb
(
    // Bidding   
    .c_valid     ( bid         ),
    .c_ready     ( ready       ),
    .c_winner    ( winner      ),
    // destination side
    .valid       ( valid       ),
    .ready       ( !stall      )
);

assign grant = ready & bid;
assign data_winner = data_in[winner];

wire unused_ok = &{
      1'b0,
      valid
}; 

endmodule

