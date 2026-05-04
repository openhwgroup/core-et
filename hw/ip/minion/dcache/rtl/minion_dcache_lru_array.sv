// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// minion_dcache_lru_array — DCache LRU state array.

module minion_dcache_lru_array
  import minion_dcache_pkg::*;
(
  input  logic                         clk_i,
  input  logic                         rst_ni,
  input  logic [DcacheSetIdxWidth-1:0] read_set_i,
  output logic [DcacheLruSetSize-1:0]  read_data_o,
  input  logic                         write_en_i,
  input  logic [DcacheSetIdxWidth-1:0] write_set_i,
  input  logic [DcacheLruSetSize-1:0]  write_data_i
);

  logic [DcacheSets-1:0][DcacheLruSetSize-1:0] lru_state_d;
  logic [DcacheSets-1:0][DcacheLruSetSize-1:0] lru_state_q;

  for (genvar i = 0; i < DcacheSets; i++) begin : gen_lru_sets
    always_comb begin
      lru_state_d[i] = lru_state_q[i];
      if (write_en_i && (write_set_i == i)) begin
        lru_state_d[i] = write_data_i;
      end
    end

    always_ff @(posedge clk_i) begin
      if (!rst_ni) begin
        lru_state_q[i] <= '0;
      end else begin
        lru_state_q[i] <= lru_state_d[i];
      end
    end
  end

  assign read_data_o = lru_state_q[read_set_i];

endmodule
