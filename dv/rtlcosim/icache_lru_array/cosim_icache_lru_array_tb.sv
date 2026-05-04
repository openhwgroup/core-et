// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_icache_lru_array_tb
  import icache_geom_pkg::*;
(
  input  logic                          clk_i,
  input  logic                          rst_ni,
  input  logic                          f0_valid_i,
  input  logic [IcacheSetAddrWidth-1:0] f0_access_set_i,
  input  logic                          f1_update_en_early_i,
  input  logic                          f1_update_en_i,
  input  logic [IcacheSetAddrWidth-1:0] f1_update_set_i,
  input  logic [IcacheWays-1:0]         f1_update_access_way_i,
  output logic [IcacheWays-1:0]         new_f1_victim_o,
  output logic [IcacheWays-1:0]         orig_f1_victim_o
);

  logic reset;

  assign reset = ~rst_ni;

  icache_lru_array u_new (
    .clk_i(clk_i),
    .rst_ni(rst_ni),
    .f0_valid_i(f0_valid_i),
    .f0_access_set_i(f0_access_set_i),
    .f1_victim_o(new_f1_victim_o),
    .f1_update_en_early_i(f1_update_en_early_i),
    .f1_update_en_i(f1_update_en_i),
    .f1_update_set_i(f1_update_set_i),
    .f1_update_access_way_i(f1_update_access_way_i)
  );

  icache_lru_array_orig u_orig (
    .clock(clk_i),
    .reset(reset),
    .f0_valid(f0_valid_i),
    .f0_access_set(f0_access_set_i),
    .f1_victim(orig_f1_victim_o),
    .f1_update_en_early(f1_update_en_early_i),
    .f1_update_en(f1_update_en_i),
    .f1_update_set(f1_update_set_i),
    .f1_update_access_way(f1_update_access_way_i)
  );

endmodule
