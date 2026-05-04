// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "assert.vh"
`include "macros.vh"
`include "shire_assert.vh"

// This module implements an LRU arbiter
module arb_lru
#(
    parameter NUM_CLIENTS = 8,
    parameter NUM_CLIENTS_LOG = $clog2(NUM_CLIENTS)
)
(
    // System signals
    input  logic                       clock,
    input  logic                       reset,
    // Bidding
    input  logic [NUM_CLIENTS-1:0]     bid,
    input  logic                       stall,
    // Grant
    output logic [NUM_CLIENTS-1:0]     grant,
    output logic [NUM_CLIENTS_LOG-1:0] winner
);

// Store the priority bits using a triangle matrix
logic[NUM_CLIENTS-1:0][NUM_CLIENTS-1:0] prio_bits;
logic[NUM_CLIENTS-1:0][NUM_CLIENTS-1:0] prio_bits_next;

`RST_EN_FF(clock, reset, (|bid) && !stall, prio_bits, prio_bits_next, '0)

logic [NUM_CLIENTS-1:0] win_per_client;

// Computes the winner
always_comb
begin
    // For all the clients
    for(integer i = 0; i < NUM_CLIENTS; i++)
    begin
        win_per_client[i] = bid[i];
        // For all the prio bits on the "left" (clients with lower ids)
        // check if this client has higher priority (prio_bit set to 1)
        for(integer j = 0; j < i; j++)
        begin
            win_per_client[i] = win_per_client[i] && (~bid[j] || prio_bits[i][j]);
        end
        // For all the prio bits below (clients with higher ids)
        // check if this client has higher priority (prio_bit set to 0)
        for(integer j = i + 1; j < NUM_CLIENTS; j++)
        begin
            win_per_client[i] = win_per_client[i] && (~bid[j] || ~prio_bits[j][i]);
        end
        grant[i] = win_per_client[i] && ~stall;
    end
end

// Updates the priority bits
always_comb
begin
    prio_bits_next = prio_bits;
    winner = '0;

    // For all the clients
    for(integer i = 0; i < NUM_CLIENTS; i++)
    begin
        // If client is the winner
        if(win_per_client[i])
            winner = i[NUM_CLIENTS_LOG-1:0];
        // If client was granted
        if(grant[i])
        begin

            // For all the prio bits on the "left" (clients with lower ids)
            // set prio to 0 to guarantee lower prio
            for(integer j = 0; j < i; j++)
            begin
                prio_bits_next[i][j] = 1'b0;
            end
            // For all the prio bits below (clients with higher ids)
            // set prio to 1 to guarantee lower prio
            for(integer j = i + 1; j < NUM_CLIENTS; j++)
            begin
                prio_bits_next[j][i] = 1'b1;
            end
        end
    end
end

`ASSERT_ALWAYS(ERROR_ARB_LRU_grant_one_hot_zero,    clock, reset, $onehot0(grant) )

endmodule

