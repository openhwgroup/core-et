// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "assert.vh"
`include "macros.vh"
`include "shire_assert.vh"

// This module implements an LRU arbiter. 
// Two grants are calculated per cycle.  
// One or the other of the grants can be popped.
// This allows a priority grant to be maintained as well as an 
// opportunistic grant while the priority one awaits resources.

module arb_lru_2_bid
#(
    parameter NUM_CLIENTS = 8
)
(
    // System signals
    input  logic                       clock,
    input  logic                       reset,
    // bit / grant 0
    input  logic [NUM_CLIENTS-1:0]     bid0,    // valid reqs
    output logic [NUM_CLIENTS-1:0]     grant0,  // grant0 is would be winner regardless of pop0
    input  logic                       pop0,    // grant0 is taken
    // bit / grant 1
    input  logic [NUM_CLIENTS-1:0]     bid1,    // valid reqs
    output logic [NUM_CLIENTS-1:0]     grant1,  // grant1 is would be winner regardless of pop1
    input  logic                       pop1     // grant1 is taken
);


// Store the priority bits using a triangle matrix
logic[NUM_CLIENTS-1:0][NUM_CLIENTS-1:0] prio_bits;
logic[NUM_CLIENTS-1:0][NUM_CLIENTS-1:0] prio_bits_next;

`RST_EN_FF(clock, reset, ( ((|bid0) & pop0) || ((|bid1) & pop1) ) , prio_bits, prio_bits_next, '0)

logic [NUM_CLIENTS-1:0] win_per_client0;
logic [NUM_CLIENTS-1:0] win_per_client1;

// Computes the winner
always_comb
begin
    // For all the clients
    for(integer i = 0; i < NUM_CLIENTS; i++)
    begin
        win_per_client0[i] = bid0[i];
        win_per_client1[i] = bid1[i];
        // For all the prio bits on the "left" (clients with lower ids)
        // check if this client has higher priority (prio_bit set to 1)
        for(integer j = 0; j < i; j++)
        begin
            win_per_client0[i] = win_per_client0[i] && (~bid0[j] || prio_bits[i][j]);
            win_per_client1[i] = win_per_client1[i] && (~bid1[j] || prio_bits[i][j]);
        end
        // For all the prio bits below (clients with higher ids)
        // check if this client has higher priority (prio_bit set to 0)
        for(integer j = i + 1; j < NUM_CLIENTS; j++)
        begin
            win_per_client0[i] = win_per_client0[i] && (~bid0[j] || ~prio_bits[j][i]);
            win_per_client1[i] = win_per_client1[i] && (~bid1[j] || ~prio_bits[j][i]);
        end
        grant0[i] = win_per_client0[i];
        grant1[i] = win_per_client1[i];
    end
end

logic [NUM_CLIENTS-1:0] win_per_client;
assign win_per_client = (pop0) ? win_per_client0 : win_per_client1;

// Updates the priority bits
always_comb
begin
    prio_bits_next = prio_bits;

    // For all the clients
    for(integer i = 0; i < NUM_CLIENTS; i++)
    begin
        // If client was granted
        if(win_per_client[i])
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

`ASSERT_ALWAYS(ERROR_ARB_LRU_grant_one_hot_zero,          clock, reset, $onehot0(win_per_client))
`ASSERT_ALWAYS(ERROR_arb_lru_2_bid_only_supports_one_pop, clock, reset, !(pop0 & pop1))

endmodule

