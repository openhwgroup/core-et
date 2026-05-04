// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "assert.vh"
`include "macros.vh"

// This module implements an LRU arbiter
module arb_lru_data
#(
    parameter WIDTH = 32,
    parameter NUM_CLIENTS = 8,
    parameter NUM_CLIENTS_LOG = $clog2(NUM_CLIENTS)
)
(
    // System signals
    input logic                              clock,
    input logic                              reset,
    // Bidding
    input logic [NUM_CLIENTS-1:0]            bid,
    input logic                              stall,
    // Data inputs
    input logic [NUM_CLIENTS-1:0][WIDTH-1:0] data_in,
    // Grant
    output logic [NUM_CLIENTS-1:0]           grant,
    output logic [WIDTH-1:0]                 data_winner,
    output logic [NUM_CLIENTS_LOG-1:0]       winner
);

// Arbiter with the LRU
arb_lru
#(
    .NUM_CLIENTS ( NUM_CLIENTS ) 
)
arb
(
    // System signals
    .clock  ( clock  ),
    .reset  ( reset  ),
    // Bidding
    .bid    ( bid    ),
    .stall  ( stall  ),
    // Grant
    .grant  ( grant  ),
    .winner ( winner )
);

// Selects the output data based on the winner
assign data_winner = data_in[winner];

endmodule

