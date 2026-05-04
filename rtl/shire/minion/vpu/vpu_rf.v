// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module vpu_rf (
  // System signal
  input  logic                           clock,
  // Read ports
  input  logic [2:0]                     rd_en,
  input  logic [2:0]                     rd_thrid,
  input  logic [2:0][`FREG_ADDR_RANGE]   rd_addr,
  output logic [2:0][`VPU_DATA_S_SZ-1:0] rd_data,
  // Write ports
  input  logic [1:0]                     wr_en,
  input  logic [1:0]                     wr_thrid,
  input  logic [1:0]                     wr_mask,
  input  logic [1:0][`FREG_ADDR_RANGE]   wr_addr,
  input  logic [1:0][`VPU_DATA_S_SZ-1:0] wr_data
);

  wire wr_en_masked_0 = wr_en[0] & wr_mask[0] ;
  wire wr_en_masked_1 = wr_en[1] & wr_mask[1] ;
////////////////////////////////////////////////////////////////////////////////
// Bypass logic on read write collosions
////////////////////////////////////////////////////////////////////////////////
  logic [2:0][`VPU_DATA_S_SZ-1:0] ram_rd_data;
  logic [2:0]                     bypass_sel;
  logic [2:0][`VPU_DATA_S_SZ-1:0] bypass_data;
  logic [2:0]                     match0, match1;

  logic [2:0]                     bypass_sel_next;
  logic [2:0][`VPU_DATA_S_SZ-1:0] bypass_data_next;

  always_comb begin
    for (int i=0; i<3; i++) begin
      match0[i] = wr_en_masked_0 && rd_en[i] && ({wr_thrid[0], wr_addr[0]} == {rd_thrid[i], rd_addr[i]});
      match1[i] = wr_en_masked_1 && rd_en[i] && ({wr_thrid[1], wr_addr[1]} == {rd_thrid[i], rd_addr[i]});
      bypass_sel_next[i]  = match1[i] | match0[i];
      bypass_data_next[i] = match1[i] ? wr_data[1] : wr_data[0];
    end
  end

  for (genvar i=0; i<3; i++) begin
    `FF( clock, bypass_sel[i],  bypass_sel_next[i] ) //flops to match ram read latency
    `FF( clock, bypass_data[i], bypass_data_next[i] )
  end


  assign rd_data[0] = bypass_sel[0] ? bypass_data[0] : ram_rd_data[0];
  assign rd_data[1] = bypass_sel[1] ? bypass_data[1] : ram_rd_data[1];
  assign rd_data[2] = bypass_sel[2] ? bypass_data[2] : ram_rd_data[2];
////////////////////////////////////////////////////////////////////////////////
// Data storage
////////////////////////////////////////////////////////////////////////////////
  vpu_64x32_3r2w_vpurf vpu_64x32_3r2w_vpurf (
    // System signals
    .clk        ( clock                       ),
    // Read ports
    .rd_enable0 ( rd_en[0]                    ),
    .rd_addr0   ( { rd_thrid[0], rd_addr[0] } ),
    .rd_data0   ( ram_rd_data[0]              ),
    .rd_enable1 ( rd_en[1]                    ),
    .rd_addr1   ( { rd_thrid[1], rd_addr[1] } ),
    .rd_data1   ( ram_rd_data[1]              ),
    .rd_enable2 ( rd_en[2]                    ),
    .rd_addr2   ( { rd_thrid[2], rd_addr[2] } ),
    .rd_data2   ( ram_rd_data[2]              ),
    // Write ports
    .wr_enable0 ( wr_en_masked_0              ),
    .wr_addr0   ( { wr_thrid[0], wr_addr[0] } ),
    .wr_data0   ( wr_data[0]                  ),
    .wr_enable1 ( wr_en_masked_1              ),
    .wr_addr1   ( { wr_thrid[1], wr_addr[1] } ),
    .wr_data1   ( wr_data[1]                  )
  );
endmodule
