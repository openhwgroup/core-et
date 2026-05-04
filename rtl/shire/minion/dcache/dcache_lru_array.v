// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module dcache_lru_array (
  // System signals
  input  logic                         clock,
  input  logic                         reset,
  // Read port
  input  logic [`DCACHE_SET_RANGE]     read_set,
  output logic [`DCACHE_LRU_SET_RANGE] read_data,
  // Write port
  input  logic                         write_en,
  input  logic [`DCACHE_SET_RANGE]     write_set,
  input  logic [`DCACHE_LRU_SET_RANGE] write_data
);

  logic [`DCACHE_SETS-1:0][`DCACHE_LRU_SET_RANGE] lru_state, lru_state_next;

  // For all the sets in the dcache
    for (genvar i = 0; i < `DCACHE_SETS; i++) begin : lru_sets
      // Stores the LRU for the set
      `RST_EN_FF(clock, reset, write_en && (write_set == i), lru_state[i], lru_state_next[i], {`DCACHE_LRU_SET_SIZE{1'b0}})

      always_comb begin
        lru_state_next[i] = lru_state[i];
        if (write_en && (write_set == i)) begin
          lru_state_next[i] = write_data;
        end
      end

    end

  assign read_data = lru_state[read_set];

endmodule

