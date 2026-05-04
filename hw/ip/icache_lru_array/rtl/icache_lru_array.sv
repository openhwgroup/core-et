// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module icache_lru_array
  import icache_geom_pkg::*;
  import icache_pkg::*;
(
  input  logic                            clk_i,
  input  logic                            rst_ni,
  input  logic                            f0_valid_i,
  input  logic [IcacheSetAddrWidth-1:0]   f0_access_set_i,
  output logic [IcacheWays-1:0]           f1_victim_o,
  input  logic                            f1_update_en_early_i,
  input  logic                            f1_update_en_i,
  input  logic [IcacheSetAddrWidth-1:0]   f1_update_set_i,
  input  logic [IcacheWays-1:0]           f1_update_access_way_i
);

  logic [IcacheLruSetSize-1:0] f0_lru_data_rf;
  logic [IcacheLruSetSize-1:0] f0_lru_data;
  logic [IcacheLruSetSize-1:0] f0_lru_data_byp;
  logic [IcacheWays-1:0]       f0_victim;
  logic [IcacheSets-1:0]       f1_lru_set_initialized_d;
  logic [IcacheSets-1:0]       f1_lru_set_initialized_q;
  logic [IcacheLruSetSize-1:0] f1_lru_data_q;
  logic [IcacheLruSetSize-1:0] f1_new_lru_data;

  always_comb begin
    if (f1_update_en_i && (f1_update_set_i == f0_access_set_i)) begin
      f0_lru_data_byp = f1_new_lru_data;
    end else begin
      f0_lru_data_byp = f0_lru_data;
    end
  end

  assign f0_victim = icache_lru_victim(f0_lru_data_byp);

  always_comb begin
    f1_lru_set_initialized_d = f1_lru_set_initialized_q;
    f1_lru_set_initialized_d[f1_update_set_i] = 1'b1;
  end

  assign f1_new_lru_data = icache_lru_update(f1_lru_data_q, f1_update_access_way_i);

  prim_rf_1r1w_preview #(
    .Width(IcacheLruSetSize),
    .Depth(IcacheSets)
  ) u_rf_lru_state_array (
    .preview_clk_i        (clk_i),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (f1_update_en_early_i),
    .wr_en_i              (f1_update_en_i),
    .wr_addr_i            (f1_update_set_i),
    .wr_data_i            (f1_new_lru_data),
    .rd_addr_i            (f0_access_set_i),
    .rd_data_o            (f0_lru_data_rf)
  );

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f1_lru_set_initialized_q <= '0;
    end else if (f1_update_en_i) begin
      f1_lru_set_initialized_q <= f1_lru_set_initialized_d;
    end
  end

  always_ff @(posedge clk_i) begin
    if (f0_valid_i) begin
      f1_lru_data_q <= f0_lru_data_byp;
      f1_victim_o   <= f0_victim;
    end
  end

  assign f0_lru_data = f1_lru_set_initialized_q[f0_access_set_i] ? f0_lru_data_rf : '0;

endmodule
