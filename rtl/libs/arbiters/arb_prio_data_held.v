// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// This module implements an arbiter, giving priority to the highest index

// This module is an arb_prio_data with extra registers to hold the previous winner in order
// to hold output data constant if there isn't a current winner. 
module arb_prio_data_held
#(
    parameter WIDTH = 32,
    parameter NUM_CLIENTS = 8,
    parameter NUM_CLIENTS_LOG = $clog2(NUM_CLIENTS)
)
(
    // clock reset
    input  logic                              clock,
    input  logic                              reset,

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

wire new_winner = (|grant);
logic [NUM_CLIENTS_LOG-1:0] prev_winner;
`RST_EN_FF(clock, reset, new_winner, prev_winner, winner, '0)

assign grant = ready & bid;
logic [NUM_CLIENTS_LOG-1:0] winner_sel;

// If no new winner hold previous. 
assign winner_sel = (new_winner) ? winner : prev_winner; 
assign data_winner = data_in[winner_sel];

wire unused_ok = &{
      1'b0,
      valid
}; 

endmodule

