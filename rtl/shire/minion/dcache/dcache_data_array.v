// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module dcache_data_array (
  // System signals
  input  logic                                         clock,
  // Read port request
  input  logic                                         s1_read_valid,
  input  dcache_da_read_req [`DCACHE_LRAM_BANK_RANGE]  s1_read_req,
  output dcache_da_data                                s2_read_resp,
  // Write port request
  input  logic                                         s4_write_valid,
  input  dcache_da_write_req [`DCACHE_LRAM_BANK_RANGE] s4_write_req
);

  localparam memWords     = 128;
  localparam memWidth     = 64;
  localparam usedMemWidth = 64; //some bits will be unused if data width not multiple of mem data width
  localparam memAdWidth   = $clog2(memWords);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// S1 stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  logic [`DCACHE_LRAM_BANK_RANGE][memAdWidth-1:0] s1_read_addr_block;  // Per block read address
  logic [`DCACHE_LRAM_BANK_RANGE]                 s1_read_en_block;    // Per block read enable
  logic [`DCACHE_LRAM_BANK_RANGE]                 s1_read_en_block_l;  // Per block read enable of low half
  logic [`DCACHE_LRAM_BANK_RANGE]                 s1_read_en_block_h;  // Per block read enable of high half
  logic [`DCACHE_LRAM_BANK_RANGE][memAdWidth-1:0] s4_write_addr_block; // Per block write address
  logic [`DCACHE_LRAM_BANK_RANGE]                 s4_write_en_block_l; // Per block write enable of low half
  logic [`DCACHE_LRAM_BANK_RANGE]                 s4_write_en_block_h; // Per block write enable of high half

  logic [`DCACHE_LRAM_BANK_RANGE][`DCACHE_SET_RANGE] s1_read_set;      // Set being read
  logic [`DCACHE_LRAM_BANK_RANGE][`DCACHE_WAY_RANGE] s1_read_way;      // Way being read
  logic [`DCACHE_LRAM_BANK_RANGE]                    s1_read_idx;      // Idx of the half line


  for (genvar block = 0; block < `DCACHE_LRAM_NUM_BANKS; block++) begin : S1_BLOCKS
      // Read
    always_comb begin
      s1_read_set[block] = s1_read_req[block].addr[`DCACHE_SET_ADDR_RANGE];
      s1_read_idx[block] = s1_read_req[block].addr[5]; // Half line is 32 bytes
      s1_read_way[block] = s1_read_req[block].way;  
      s1_read_addr_block[block] = {s1_read_set[block],s1_read_idx[block],s1_read_way[block]}; // Way is in the LSB of the address
      s1_read_en_block[block]   = s1_read_valid && (s1_read_req[block].valid_l || s1_read_req[block].valid_h);
      s1_read_en_block_l[block] = s1_read_valid && s1_read_req[block].valid_l;
      s1_read_en_block_h[block] = s1_read_valid && s1_read_req[block].valid_h;
    end
  end


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// S2 stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  logic [`DCACHE_LRAM_BANK_RANGE][memWidth-1:0]      s2_read_data_block; // Block data output

  // Muxes the data output
  always_comb begin
    for (integer i = 0; i < `DCACHE_LRAM_NUM_BANKS; i++) begin
      // memWidth and usedMemWidth may be different
      s2_read_resp[i][usedMemWidth/2-1:0]            = s2_read_data_block[i][0+:usedMemWidth/2];
      s2_read_resp[i][usedMemWidth-1:usedMemWidth/2] = s2_read_data_block[i][memWidth/2+:usedMemWidth/2];
    end
  end

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// S4 stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  logic [`DCACHE_LRAM_BANK_RANGE][memWidth-1:0] s4_write_din_block; // Per block write data

  logic [`DCACHE_LRAM_BANK_RANGE][`DCACHE_SET_RANGE] s4_write_set;      // Set being written
  logic [`DCACHE_LRAM_BANK_RANGE][`DCACHE_WAY_RANGE] s4_write_way;      // Way being written
  logic [`DCACHE_LRAM_BANK_RANGE]                    s4_write_idx;      // Idx of the half line


  
  for (genvar block = 0; block < `DCACHE_LRAM_NUM_BANKS; block++) begin : S4_BLOCKS
    // Write
    always_comb begin
      s4_write_set[block] = s4_write_req[block].addr[`DCACHE_SET_ADDR_RANGE];
      s4_write_idx[block] = s4_write_req[block].addr[5]; // Half line is 32 bytes
      s4_write_way[block] = s4_write_req[block].way;
      s4_write_en_block_l[block]  = s4_write_valid && s4_write_req[block].valid_l;
      s4_write_en_block_h[block]  = s4_write_valid && s4_write_req[block].valid_h;
      s4_write_addr_block[block]  = { s4_write_set[block], s4_write_idx[block], s4_write_way[block]};
      // memWidth and usedMemWith may be eventually different
      s4_write_din_block[block]  = '0;
      s4_write_din_block[block][(usedMemWidth/2-1):0]         = s4_write_req[block].data[(usedMemWidth/2-1):0];
      s4_write_din_block[block][memWidth/2+:usedMemWidth/2]   = s4_write_req[block].data[memWidth/2+:usedMemWidth/2];

      end
    end


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// LRAM instance
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
 
  for (genvar block = 0; block < `DCACHE_LRAM_NUM_BANKS; block++) begin : RAM_BLOCKS  
    dcache_data_ram_wrap dcache_data_ram_wrap_inst (
      // System signals
      .clk             ( clock                      ),
      // Read port
      .rd_enable       ( s1_read_en_block[block]    ),
      .rd_enablelob    ( ~s1_read_en_block_l[block] ),
      .rd_enablehib    ( ~s1_read_en_block_h[block] ),
      .rd_addr         ( s1_read_addr_block[block]  ),
      .rd_data         ( s2_read_data_block[block]  ),
      // Write port
      .wr_enablehi     ( s4_write_en_block_h[block] ),
      .wr_enablelo     ( s4_write_en_block_l[block] ),
      .wr_addr         ( s4_write_addr_block[block] ),
      .wr_data         ( s4_write_din_block[block]  )
    );

    end


endmodule // dcache_data_array

