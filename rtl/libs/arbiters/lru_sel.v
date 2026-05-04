// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "assert.vh"
`include "macros.vh"
`include "shire_assert.vh"

// This module implements an LRU selector (returns least recently accessed
// entry)
module lru_sel
#(
    parameter NUM_CLIENTS = 8
)
(
    // System signals
    input  logic                   clock,
    input  logic                   reset,
    // Update
    input  logic [NUM_CLIENTS-1:0] access_entry,
    // Victim
    output logic [NUM_CLIENTS-1:0] victim
);

// Store the priority bits using a triangle matrix
logic [NUM_CLIENTS-1:0][NUM_CLIENTS-1:0] prio_bits;
logic [NUM_CLIENTS-1:0][NUM_CLIENTS-1:0] prio_bits_next;

`RST_FF(clock, reset, prio_bits, prio_bits_next, {NUM_CLIENTS*NUM_CLIENTS{1'b0}})

logic [NUM_CLIENTS-1:0] win_per_client;

// Computes the winner
always_comb
begin
    // For all the clients
    for(integer i = 0; i < NUM_CLIENTS; i++)
    begin
        win_per_client[i] = 1'b1;
        // For all the prio bits on the "left" (clients with lower ids)
        // check if this client has higher priority (prio_bit set to 1)
        for(integer j = 0; j < i; j++)
        begin
            win_per_client[i] = win_per_client[i] && prio_bits[i][j];
        end
        // For all the prio bits below (clients with higher ids)
        // check if this client has higher priority (prio_bit set to 0)
        for(integer j = i + 1; j < NUM_CLIENTS; j++)
        begin
            win_per_client[i] = win_per_client[i] && ~prio_bits[j][i];
        end
        victim[i] = win_per_client[i];
    end
end

// Updates the priority bits
always_comb
begin
    prio_bits_next = prio_bits;

    // For all the clients
    for(integer i = 0; i < NUM_CLIENTS; i++)
    begin
        // If client was accessed
        if(access_entry[i])
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

`ifndef UPF
// synopsys translate_off
// Assert to guarantee one hot mux
`ASSERT_ALWAYS_MSG(ERROR_arb_lru_one_hot, clock, reset, ($onehot(victim) || |victim == 1'b0), $sformatf("LRU has more than one victim!!") )
// synopsys translate_on
`endif

endmodule

