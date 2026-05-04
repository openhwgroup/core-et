// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module icache_data_array (
  input  logic                                                    clk_i,
  input  logic                                                    rst_ni,

  input  logic                                                    f1_read_req_valid_i,
  input  logic [icache_geom_pkg::IcacheWayAddrWidth-1:0]         f1_read_req_way_i,
  input  logic [icache_geom_pkg::IcacheSetAddrWidth-1:0]         f1_read_req_set_i,

  output logic                                                    f0_read_resp_early_valid_o,
  output logic                                                    f0_read_resp_valid_o,
  output logic [icache_geom_pkg::IcacheBlockBits-1:0]            f0_read_resp_data_o,
  output logic [icache_pkg::IcacheEccBlocks-1:0]                 f0_read_resp_sbe_per_block_o,
  output logic [icache_pkg::IcacheEccBlocks-1:0]                 f0_read_resp_dbe_per_block_o,

  input  logic                                                    f1_write_req_valid_i,
  input  logic [icache_geom_pkg::IcacheWayAddrWidth-1:0]         f1_write_req_way_i,
  input  logic [icache_geom_pkg::IcacheSetAddrWidth-1:0]         f1_write_req_set_i,

  output logic                                                    f2_mem_req_write_o,
  output logic [icache_geom_pkg::IcacheSramAddrWidth-1:0]        f2_mem_req_addr_o,
  output logic                                                    f2_mem_req_valid_o,
  input  logic                                                    f2_mem_req_ready_i,

  input  logic [icache_geom_pkg::IcacheNrMemsPerRow-1:0]
               [icache_geom_pkg::IcacheMemWidth-1:0]             f0_mem_resp_dout_i,
  input  logic                                                    f0_mem_resp_valid_i,
  output logic                                                    f0_mem_resp_ready_o
);

  import icache_geom_pkg::*;
  import icache_pkg::*;

  localparam int unsigned SetSliceWidth  = IcacheSramAddrWidth - IcacheWayAddrWidth;
  localparam int unsigned EccBlockBits   = IcacheMemEccBits / 2;
  localparam int unsigned EccVectorWidth = IcacheEccBlocks * EccBlockBits;

  logic                             f1_mem_req_write;
  logic [IcacheSramAddrWidth-1:0]   f1_mem_req_addr;
  logic                             f1_mem_req_valid;
  logic                             f1_mem_req_ready;

  logic [IcacheBlockBits-1:0]       f0_mem_resp_data;
  logic [EccVectorWidth-1:0]        f0_mem_resp_ecc;
  logic [IcacheBlockBits-1:0]       f1_mem_resp_data_q;
  logic [EccVectorWidth-1:0]        f1_mem_resp_ecc_q;
  logic [IcacheBlockBits-1:0]       f1_mem_resp_data_corr;
  logic [IcacheEccBlocks-1:0]       f1_mem_resp_sbe_per_block;
  logic [IcacheEccBlocks-1:0]       f1_mem_resp_dbe_per_block;
  logic                             f1_mem_resp_valid_q;

  logic [IcacheSramAddrWidth-1:0]   f1_mem_req_raddr;
  logic [IcacheSramAddrWidth-1:0]   f1_mem_req_waddr;

  always_comb begin
    for (int i = 0; i < IcacheNrMemsPerRow; i++) begin
      f0_mem_resp_data[IcacheMemUsedBits*i +: IcacheMemUsedBits] =
          f0_mem_resp_dout_i[i][0 +: IcacheMemUsedBits];
      f0_mem_resp_ecc[IcacheMemEccBits*i +: IcacheMemEccBits] =
          f0_mem_resp_dout_i[i][IcacheMemUsedBits +: IcacheMemEccBits];
    end
  end

  always_ff @(posedge clk_i) begin
    if (f0_mem_resp_valid_i) begin
      f1_mem_resp_data_q <= f0_mem_resp_data;
      f1_mem_resp_ecc_q  <= f0_mem_resp_ecc;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f1_mem_resp_valid_q      <= 1'b0;
      f0_read_resp_valid_o     <= 1'b0;
      f2_mem_req_valid_o       <= 1'b0;
    end else begin
      f1_mem_resp_valid_q  <= f0_mem_resp_valid_i;
      f0_read_resp_valid_o <= f1_mem_resp_valid_q;

      if (f1_mem_req_ready) begin
        f2_mem_req_valid_o <= f1_mem_req_valid;
      end
    end
  end

  generate
    for (genvar ecc_idx = 0; ecc_idx < IcacheEccBlocks; ecc_idx++) begin : gen_ecc_blocks
      prim_ecc_dec #(
        .DataWidth(64)
      ) u_icache_ecc_corr (
        .data_and_ecc_i({f1_mem_resp_ecc_q[ecc_idx*EccBlockBits +: EccBlockBits],
                         f1_mem_resp_data_q[ecc_idx*64 +: 64]}),
        .data_o        (f1_mem_resp_data_corr[ecc_idx*64 +: 64]),
        .sbe_o         (f1_mem_resp_sbe_per_block[ecc_idx]),
        .dbe_o         (f1_mem_resp_dbe_per_block[ecc_idx])
      );
    end
  endgenerate

  always_ff @(posedge clk_i) begin
    if (f1_mem_resp_valid_q) begin
      f0_read_resp_data_o          <= f1_mem_resp_data_corr;
      f0_read_resp_sbe_per_block_o <= f1_mem_resp_sbe_per_block;
      f0_read_resp_dbe_per_block_o <= f1_mem_resp_dbe_per_block;
    end
  end

  assign f0_read_resp_early_valid_o = f1_mem_resp_valid_q;
  assign f0_mem_resp_ready_o        = 1'b1;

  assign f1_mem_req_raddr = {
    f1_read_req_set_i[0 +: SetSliceWidth],
    f1_read_req_way_i
  };
  assign f1_mem_req_waddr = {
    f1_write_req_set_i[0 +: SetSliceWidth],
    f1_write_req_way_i
  };

  assign f1_mem_req_write = f1_write_req_valid_i;
  assign f1_mem_req_addr  = f1_write_req_valid_i ? f1_mem_req_waddr : f1_mem_req_raddr;
  assign f1_mem_req_valid = f1_read_req_valid_i || f1_write_req_valid_i;

  always_ff @(posedge clk_i) begin
    if (f1_mem_req_ready && f1_mem_req_valid) begin
      f2_mem_req_write_o <= f1_mem_req_write;
      f2_mem_req_addr_o  <= f1_mem_req_addr;
    end
  end

  assign f1_mem_req_ready = f2_mem_req_ready_i || !f2_mem_req_valid_o;

endmodule : icache_data_array
