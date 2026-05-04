// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module icache_data_array (
  // System signals
  input  logic                                                      clock,
  input  logic                                                      reset,
  // Read port request
  input  logic                                                      f1_read_req_valid,
  input  logic [`ICACHE_WAY_RANGE]                                  f1_read_req_way,
  input  logic [`ICACHE_SET_RANGE]                                  f1_read_req_set,
  // Read port response
  output logic                                                      f0_read_resp_valid,
  output logic [`ICACHE_BLOCK_BITS-1:0]                             f0_read_resp_data,
  output logic [`ICACHE_ECC_BLOCKS-1:0]                             f0_read_resp_sbe_per_block,
  output logic [`ICACHE_ECC_BLOCKS-1:0]                             f0_read_resp_dbe_per_block,
  // Write port request
  input  logic                                                      f1_write_req_valid,
  input  logic [`ICACHE_WAY_RANGE]                                  f1_write_req_way,
  input  logic [`ICACHE_SET_RANGE]                                  f1_write_req_set,
  // Read request to L1 SRAM blocks
  output logic                                                      f2_mem_req_write,
  output logic [`ICACHE_ROW_ADWIDTH+`ICACHE_MEM_ADWIDTH-1:0]        f2_mem_req_addr,
  output logic                                                      f2_mem_req_valid,
  input  logic                                                      f2_mem_req_ready,
  // Read response from L1 SRAM blocks
  input  logic [`ICACHE_NR_MEMS_PER_ROW-1:0][`ICACHE_MEM_WIDTH-1:0] f0_mem_resp_dout,
  input  logic                                                      f0_mem_resp_valid,
  output logic                                                      f0_mem_resp_ready
  );

  // INTERNAL DECLARATIONS
  logic                                               f1_mem_req_write;
  logic [`ICACHE_ROW_ADWIDTH+`ICACHE_MEM_ADWIDTH-1:0] f1_mem_req_addr;
  logic                                               f1_mem_req_valid; // Valid memory request in F1
  logic                                               f1_mem_req_ready;

  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // F0 Stage
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  logic [`ET_LINE_DATA_SIZE-1:0]   f0_mem_resp_data;
  logic [`ICACHE_ECC_BLOCKS*8-1:0] f0_mem_resp_ecc;
  logic [`ET_LINE_DATA_SIZE-1:0]   f1_mem_resp_data;
  logic [`ICACHE_ECC_BLOCKS*8-1:0] f1_mem_resp_ecc;
  logic [`ET_LINE_DATA_SIZE-1:0]   f1_mem_resp_data_corr; // Corrected data
  logic [`ICACHE_ECC_BLOCKS-1:0]   f1_mem_resp_sbe_per_block; // Single bit error indication per ECC block
  logic [`ICACHE_ECC_BLOCKS-1:0]   f1_mem_resp_dbe_per_block; // Double bit error indication per ECC block
  logic                            f1_mem_resp_valid;

  // //////////////////////////////////////////////////////////////////////////////
  // ECC correction for input data from L1 SRAM blocks
  // //////////////////////////////////////////////////////////////////////////////

  always_comb begin
    for (integer i = 0; i < `ICACHE_NR_MEMS_PER_ROW; i++) begin
      f0_mem_resp_data[`ICACHE_MEM_USED_BITS*i +: `ICACHE_MEM_USED_BITS] = f0_mem_resp_dout[i][0                     +: `ICACHE_MEM_USED_BITS];
      f0_mem_resp_ecc [`ICACHE_MEM_ECC_BITS *i +: `ICACHE_MEM_ECC_BITS ] = f0_mem_resp_dout[i][`ICACHE_MEM_USED_BITS +: `ICACHE_MEM_ECC_BITS ];
    end
  end

  //       CLK    RST    EN                 DOUT               DIN                DEF
  `EN_FF  (clock,        f0_mem_resp_valid, f1_mem_resp_data,  f0_mem_resp_data)
  `EN_FF  (clock,        f0_mem_resp_valid, f1_mem_resp_ecc,   f0_mem_resp_ecc)
  `RST_FF (clock, reset,                    f1_mem_resp_valid, f0_mem_resp_valid, 1'b0)

 
  for (genvar ecc_idx = 0; ecc_idx < `ICACHE_ECC_BLOCKS; ecc_idx++) begin : ECC_BLOCKS
    ecc_corr #(
     .DATA_SIZE   ( 64                                      )
    ) icache_ecc_corr (
     .data_and_ecc ( {f1_mem_resp_ecc [ecc_idx*8  +: 8],
                      f1_mem_resp_data[ecc_idx*64 +: 64]}    ),
     .data_corr    ( f1_mem_resp_data_corr[ecc_idx*64 +: 64] ),
     .sbe          ( f1_mem_resp_sbe_per_block[ecc_idx]      ),
     .dbe          ( f1_mem_resp_dbe_per_block[ecc_idx]      )
    );
  end
  
  //       CLK    RST    EN                 DOUT                        DIN                DEF
  `EN_FF  (clock,        f1_mem_resp_valid, f0_read_resp_data,          f1_mem_resp_data_corr)
  `EN_FF  (clock,        f1_mem_resp_valid, f0_read_resp_sbe_per_block, f1_mem_resp_sbe_per_block)
  `EN_FF  (clock,        f1_mem_resp_valid, f0_read_resp_dbe_per_block, f1_mem_resp_dbe_per_block)
  `RST_FF (clock, reset,                    f0_read_resp_valid,         f1_mem_resp_valid, 1'b0)

  // Always accept data responses from SRAM blocks
  assign f0_mem_resp_ready = 1'b1;

  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // F1 Stage
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  logic [`ICACHE_ROW_ADWIDTH+`ICACHE_MEM_ADWIDTH-1:0] f1_mem_req_raddr;
  logic [`ICACHE_ROW_ADWIDTH+`ICACHE_MEM_ADWIDTH-1:0] f1_mem_req_waddr;

  // //////////////////////////////////////////////////////////////////////////////
  // ADDRESS ENCODING
  // //////////////////////////////////////////////////////////////////////////////
  // - Ways of a specific set are stored in consecutive entries in memory
  // - If required bandwidth is greater than memory width, more than one memory panel per row is used
  // - If memory depth is not enough to store all the sets, more than one row of memory panels is used
  // In that case, consecutive sets are stored consecutively

  // Gets the address bits within SRAM (LSBs are used to select way, others to select set)
  assign f1_mem_req_raddr = { f1_read_req_set [0 +: `ICACHE_ROW_ADWIDTH+`ICACHE_MEM_ADWIDTH-`ICACHE_WAY_ADDR_WIDTH], f1_read_req_way };
  assign f1_mem_req_waddr = { f1_write_req_set[0 +: `ICACHE_ROW_ADWIDTH+`ICACHE_MEM_ADWIDTH-`ICACHE_WAY_ADDR_WIDTH], f1_write_req_way };

  assign f1_mem_req_write = f1_write_req_valid ? 1'b1             : 1'b0;
  assign f1_mem_req_addr  = f1_write_req_valid ? f1_mem_req_waddr : f1_mem_req_raddr;
  assign f1_mem_req_valid = f1_read_req_valid || f1_write_req_valid;

  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // F2 Stage
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////
  // //////////////////////////////////////////////////////////////////////////////

  // CLK    RST    EN                                   DOUT              DIN               DEF
  `EN_FF    (clock,        f1_mem_req_ready & f1_mem_req_valid, f2_mem_req_write, f1_mem_req_write)
  `EN_FF    (clock,        f1_mem_req_ready & f1_mem_req_valid, f2_mem_req_addr,  f1_mem_req_addr)
  `RST_EN_FF(clock, reset, f1_mem_req_ready,                    f2_mem_req_valid, f1_mem_req_valid, 1'b0)

  assign f1_mem_req_ready = f2_mem_req_ready || !f2_mem_req_valid;

endmodule
