// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_icache_data_array_tb
  import icache_geom_pkg::*;
  import icache_pkg::*;
(
  input  logic                          clk_i,
  input  logic                          rst_ni,
  input  logic                          f1_read_req_valid_i,
  input  logic [IcacheWayAddrWidth-1:0] f1_read_req_way_i,
  input  logic [IcacheSetAddrWidth-1:0] f1_read_req_set_i,
  input  logic                          f1_write_req_valid_i,
  input  logic [IcacheWayAddrWidth-1:0] f1_write_req_way_i,
  input  logic [IcacheSetAddrWidth-1:0] f1_write_req_set_i,
  input  logic                          f2_mem_req_ready_i,
  input  logic                          f0_mem_resp_valid_i,
  input  logic [63:0]                   f0_mem_resp_mem0_lo_i,
  input  logic [63:0]                   f0_mem_resp_mem0_hi_i,
  input  logic [15:0]                   f0_mem_resp_mem0_ecc_i,
  input  logic [63:0]                   f0_mem_resp_mem1_lo_i,
  input  logic [63:0]                   f0_mem_resp_mem1_hi_i,
  input  logic [15:0]                   f0_mem_resp_mem1_ecc_i,
  input  logic [63:0]                   f0_mem_resp_mem2_lo_i,
  input  logic [63:0]                   f0_mem_resp_mem2_hi_i,
  input  logic [15:0]                   f0_mem_resp_mem2_ecc_i,
  input  logic [63:0]                   f0_mem_resp_mem3_lo_i,
  input  logic [63:0]                   f0_mem_resp_mem3_hi_i,
  input  logic [15:0]                   f0_mem_resp_mem3_ecc_i,

  output logic                          orig_f0_read_resp_early_valid_o,
  output logic                          orig_f0_read_resp_valid_o,
  output logic [63:0]                   orig_f0_read_resp_qw0_o,
  output logic [63:0]                   orig_f0_read_resp_qw1_o,
  output logic [63:0]                   orig_f0_read_resp_qw2_o,
  output logic [63:0]                   orig_f0_read_resp_qw3_o,
  output logic [63:0]                   orig_f0_read_resp_qw4_o,
  output logic [63:0]                   orig_f0_read_resp_qw5_o,
  output logic [63:0]                   orig_f0_read_resp_qw6_o,
  output logic [63:0]                   orig_f0_read_resp_qw7_o,
  output logic [IcacheEccBlocks-1:0]    orig_f0_read_resp_sbe_per_block_o,
  output logic [IcacheEccBlocks-1:0]    orig_f0_read_resp_dbe_per_block_o,
  output logic                          orig_f2_mem_req_write_o,
  output logic [IcacheSramAddrWidth-1:0] orig_f2_mem_req_addr_o,
  output logic                          orig_f2_mem_req_valid_o,
  output logic                          orig_f0_mem_resp_ready_o,

  output logic                          new_f0_read_resp_early_valid_o,
  output logic                          new_f0_read_resp_valid_o,
  output logic [63:0]                   new_f0_read_resp_qw0_o,
  output logic [63:0]                   new_f0_read_resp_qw1_o,
  output logic [63:0]                   new_f0_read_resp_qw2_o,
  output logic [63:0]                   new_f0_read_resp_qw3_o,
  output logic [63:0]                   new_f0_read_resp_qw4_o,
  output logic [63:0]                   new_f0_read_resp_qw5_o,
  output logic [63:0]                   new_f0_read_resp_qw6_o,
  output logic [63:0]                   new_f0_read_resp_qw7_o,
  output logic [IcacheEccBlocks-1:0]    new_f0_read_resp_sbe_per_block_o,
  output logic [IcacheEccBlocks-1:0]    new_f0_read_resp_dbe_per_block_o,
  output logic                          new_f2_mem_req_write_o,
  output logic [IcacheSramAddrWidth-1:0] new_f2_mem_req_addr_o,
  output logic                          new_f2_mem_req_valid_o,
  output logic                          new_f0_mem_resp_ready_o
);

  logic reset;
  logic [IcacheNrMemsPerRow-1:0][IcacheMemWidth-1:0] new_f0_mem_resp_dout;
  logic [`ICACHE_NR_MEMS_PER_ROW-1:0][`ICACHE_MEM_WIDTH-1:0] orig_f0_mem_resp_dout;
  logic [IcacheBlockBits-1:0] new_f0_read_resp_data;
  logic [`ICACHE_BLOCK_BITS-1:0] orig_f0_read_resp_data;

  assign reset = !rst_ni;

  always_comb begin
    new_f0_mem_resp_dout = '0;
    new_f0_mem_resp_dout[0] = {f0_mem_resp_mem0_ecc_i, f0_mem_resp_mem0_hi_i, f0_mem_resp_mem0_lo_i};
    new_f0_mem_resp_dout[1] = {f0_mem_resp_mem1_ecc_i, f0_mem_resp_mem1_hi_i, f0_mem_resp_mem1_lo_i};
    new_f0_mem_resp_dout[2] = {f0_mem_resp_mem2_ecc_i, f0_mem_resp_mem2_hi_i, f0_mem_resp_mem2_lo_i};
    new_f0_mem_resp_dout[3] = {f0_mem_resp_mem3_ecc_i, f0_mem_resp_mem3_hi_i, f0_mem_resp_mem3_lo_i};

    orig_f0_mem_resp_dout = '0;
    orig_f0_mem_resp_dout[0] = {f0_mem_resp_mem0_ecc_i, f0_mem_resp_mem0_hi_i, f0_mem_resp_mem0_lo_i};
    orig_f0_mem_resp_dout[1] = {f0_mem_resp_mem1_ecc_i, f0_mem_resp_mem1_hi_i, f0_mem_resp_mem1_lo_i};
    orig_f0_mem_resp_dout[2] = {f0_mem_resp_mem2_ecc_i, f0_mem_resp_mem2_hi_i, f0_mem_resp_mem2_lo_i};
    orig_f0_mem_resp_dout[3] = {f0_mem_resp_mem3_ecc_i, f0_mem_resp_mem3_hi_i, f0_mem_resp_mem3_lo_i};
  end

  assign orig_f0_read_resp_qw0_o = orig_f0_read_resp_data[0 +: 64];
  assign orig_f0_read_resp_qw1_o = orig_f0_read_resp_data[64 +: 64];
  assign orig_f0_read_resp_qw2_o = orig_f0_read_resp_data[128 +: 64];
  assign orig_f0_read_resp_qw3_o = orig_f0_read_resp_data[192 +: 64];
  assign orig_f0_read_resp_qw4_o = orig_f0_read_resp_data[256 +: 64];
  assign orig_f0_read_resp_qw5_o = orig_f0_read_resp_data[320 +: 64];
  assign orig_f0_read_resp_qw6_o = orig_f0_read_resp_data[384 +: 64];
  assign orig_f0_read_resp_qw7_o = orig_f0_read_resp_data[448 +: 64];

  assign new_f0_read_resp_qw0_o = new_f0_read_resp_data[0 +: 64];
  assign new_f0_read_resp_qw1_o = new_f0_read_resp_data[64 +: 64];
  assign new_f0_read_resp_qw2_o = new_f0_read_resp_data[128 +: 64];
  assign new_f0_read_resp_qw3_o = new_f0_read_resp_data[192 +: 64];
  assign new_f0_read_resp_qw4_o = new_f0_read_resp_data[256 +: 64];
  assign new_f0_read_resp_qw5_o = new_f0_read_resp_data[320 +: 64];
  assign new_f0_read_resp_qw6_o = new_f0_read_resp_data[384 +: 64];
  assign new_f0_read_resp_qw7_o = new_f0_read_resp_data[448 +: 64];

  icache_data_array_orig u_orig (
    .clock                  (clk_i),
    .reset                  (reset),
    .f1_read_req_valid      (f1_read_req_valid_i),
    .f1_read_req_way        (f1_read_req_way_i),
    .f1_read_req_set        (f1_read_req_set_i),
    .f0_read_resp_early_valid(orig_f0_read_resp_early_valid_o),
    .f0_read_resp_valid     (orig_f0_read_resp_valid_o),
    .f0_read_resp_data      (orig_f0_read_resp_data),
    .f0_read_resp_sbe_per_block(orig_f0_read_resp_sbe_per_block_o),
    .f0_read_resp_dbe_per_block(orig_f0_read_resp_dbe_per_block_o),
    .f1_write_req_valid     (f1_write_req_valid_i),
    .f1_write_req_way       (f1_write_req_way_i),
    .f1_write_req_set       (f1_write_req_set_i),
    .f2_mem_req_write       (orig_f2_mem_req_write_o),
    .f2_mem_req_addr        (orig_f2_mem_req_addr_o),
    .f2_mem_req_valid       (orig_f2_mem_req_valid_o),
    .f2_mem_req_ready       (f2_mem_req_ready_i),
    .f0_mem_resp_dout       (orig_f0_mem_resp_dout),
    .f0_mem_resp_valid      (f0_mem_resp_valid_i),
    .f0_mem_resp_ready      (orig_f0_mem_resp_ready_o)
  );

  icache_data_array u_new (
    .clk_i,
    .rst_ni,
    .f1_read_req_valid_i,
    .f1_read_req_way_i,
    .f1_read_req_set_i,
    .f0_read_resp_early_valid_o(new_f0_read_resp_early_valid_o),
    .f0_read_resp_valid_o(new_f0_read_resp_valid_o),
    .f0_read_resp_data_o(new_f0_read_resp_data),
    .f0_read_resp_sbe_per_block_o(new_f0_read_resp_sbe_per_block_o),
    .f0_read_resp_dbe_per_block_o(new_f0_read_resp_dbe_per_block_o),
    .f1_write_req_valid_i,
    .f1_write_req_way_i,
    .f1_write_req_set_i,
    .f2_mem_req_write_o(new_f2_mem_req_write_o),
    .f2_mem_req_addr_o(new_f2_mem_req_addr_o),
    .f2_mem_req_valid_o(new_f2_mem_req_valid_o),
    .f2_mem_req_ready_i,
    .f0_mem_resp_dout_i(new_f0_mem_resp_dout),
    .f0_mem_resp_valid_i,
    .f0_mem_resp_ready_o(new_f0_mem_resp_ready_o)
  );

endmodule
