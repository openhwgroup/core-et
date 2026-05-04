// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module icache_lru_array_tb
  import icache_geom_pkg::*;
(
  input  logic                          clk_i,
  input  logic                          rst_ni,
  input  logic                          f0_valid_i,
  input  logic [IcacheSetAddrWidth-1:0] f0_access_set_i,
  output logic [IcacheWays-1:0]         f1_victim_o,
  input  logic                          f1_update_en_early_i,
  input  logic                          f1_update_en_i,
  input  logic [IcacheSetAddrWidth-1:0] f1_update_set_i,
  input  logic [IcacheWays-1:0]         f1_update_access_way_i
);

  icache_lru_array u_dut (
    .clk_i,
    .rst_ni,
    .f0_valid_i,
    .f0_access_set_i,
    .f1_victim_o,
    .f1_update_en_early_i,
    .f1_update_en_i,
    .f1_update_set_i,
    .f1_update_access_way_i
  );

endmodule
