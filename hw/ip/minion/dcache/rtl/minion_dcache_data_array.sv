// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// minion_dcache_data_array — four-bank 128x64 data-array wrapper.

module minion_dcache_data_array
  import ram_cfg_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                               clk_i,
  input  logic                               s1_read_valid_i,
  input  dcache_da_read_req_t                s1_read_req_i [DcacheLramNumBanks],
  output dcache_da_data_t                    s2_read_resp_o,
  input  logic                               s3_write_valid_i,
  input  logic                               s4_write_valid_i,
  input  dcache_da_write_req_t               s4_write_req_i [DcacheLramNumBanks],
  input  ram_cfg_t                           ram_cfg_i
);

  localparam int unsigned MemWords     = 128;
  localparam int unsigned MemWidth     = 64;
  localparam int unsigned UsedMemWidth = 64;
  localparam int unsigned MemAddrWidth = $clog2(MemWords);

  logic [MemAddrWidth-1:0]        s1_read_addr_block [DcacheLramNumBanks];
  logic                           s1_read_en_block [DcacheLramNumBanks];
  logic                           s1_read_en_block_l [DcacheLramNumBanks];
  logic                           s1_read_en_block_h [DcacheLramNumBanks];
  logic [MemAddrWidth-1:0]        s4_write_addr_block [DcacheLramNumBanks];
  logic                           s4_write_en_block_l [DcacheLramNumBanks];
  logic                           s4_write_en_block_h [DcacheLramNumBanks];

  logic [DcacheSetIdxWidth-1:0]   s1_read_set [DcacheLramNumBanks];
  logic [DcacheWayIdxWidth-1:0]   s1_read_way [DcacheLramNumBanks];
  logic                           s1_read_idx [DcacheLramNumBanks];

  logic [MemWidth-1:0]            s2_read_data_block [DcacheLramNumBanks];

  logic [MemWidth-1:0]            s4_write_din_block [DcacheLramNumBanks];
  logic [DcacheSetIdxWidth-1:0]   s4_write_set [DcacheLramNumBanks];
  logic [DcacheWayIdxWidth-1:0]   s4_write_way [DcacheLramNumBanks];
  logic                           s4_write_idx [DcacheLramNumBanks];

  for (genvar block = 0; block < DcacheLramNumBanks; block++) begin : gen_s1_block
    always_comb begin
      s1_read_set[block] = s1_read_req_i[block].addr[DcacheSetIdxWidth+5:6];
      s1_read_idx[block] = s1_read_req_i[block].addr[5];
      s1_read_way[block] = s1_read_req_i[block].way;

      s1_read_addr_block[block] = {s1_read_set[block], s1_read_idx[block], s1_read_way[block]};
      s1_read_en_block[block] = s1_read_valid_i
                             && (s1_read_req_i[block].valid_l || s1_read_req_i[block].valid_h);
      s1_read_en_block_l[block] = s1_read_valid_i && s1_read_req_i[block].valid_l;
      s1_read_en_block_h[block] = s1_read_valid_i && s1_read_req_i[block].valid_h;
    end
  end

  always_comb begin
    for (int i = 0; i < DcacheLramNumBanks; i++) begin
      s2_read_resp_o[i][(UsedMemWidth / 2) - 1:0] =
        s2_read_data_block[i][0 +: (UsedMemWidth / 2)];
      s2_read_resp_o[i][UsedMemWidth - 1:UsedMemWidth / 2] =
        s2_read_data_block[i][MemWidth / 2 +: (UsedMemWidth / 2)];
    end
  end

  for (genvar block = 0; block < DcacheLramNumBanks; block++) begin : gen_s4_block
    always_comb begin
      s4_write_set[block] = s4_write_req_i[block].addr[DcacheSetIdxWidth+5:6];
      s4_write_idx[block] = s4_write_req_i[block].addr[5];
      s4_write_way[block] = s4_write_req_i[block].way;

      s4_write_en_block_l[block] = s4_write_valid_i && s4_write_req_i[block].valid_l;
      s4_write_en_block_h[block] = s4_write_valid_i && s4_write_req_i[block].valid_h;
      s4_write_addr_block[block] = {s4_write_set[block], s4_write_idx[block], s4_write_way[block]};
      s4_write_din_block[block] = '0;
      s4_write_din_block[block][(UsedMemWidth / 2) - 1:0] =
        s4_write_req_i[block].data[(UsedMemWidth / 2) - 1:0];
      s4_write_din_block[block][MemWidth / 2 +: (UsedMemWidth / 2)] =
        s4_write_req_i[block].data[MemWidth / 2 +: (UsedMemWidth / 2)];
    end
  end

  for (genvar block = 0; block < DcacheLramNumBanks; block++) begin : gen_lram_block
    minion_dcache_128x64_1r1w_lram u_lram (
      .clk_i            (clk_i),
      .rd_enable_i      (s1_read_en_block[block]),
      .rd_enablelob_i   (~s1_read_en_block_l[block]),
      .rd_enablehib_i   (~s1_read_en_block_h[block]),
      .rd_addr_i        (s1_read_addr_block[block]),
      .rd_data_o        (s2_read_data_block[block]),
      .wr_enablehi_pre_i(s3_write_valid_i),
      .wr_enablehi_i    (s4_write_en_block_h[block]),
      .wr_enablelo_pre_i(s3_write_valid_i),
      .wr_enablelo_i    (s4_write_en_block_l[block]),
      .wr_addr_i        (s4_write_addr_block[block]),
      .wr_data_i        (s4_write_din_block[block]),
      .ram_cfg_i        (ram_cfg_i)
    );
  end

endmodule : minion_dcache_data_array
