// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Technology primitive: async CDC FIFO, write=low-voltage, read=high-voltage.
// Generic (behavioral) implementation.
// On ASIC, replaced by foundry-specific level shifters and CDC cells.
//
// Replaces: vcfifo_wr_lov_gcd (etcore-soc)

module prim_fifo_async_lov
  import dft_pkg::*;
#(
  parameter int unsigned Width      = 32,
  parameter int unsigned Depth      = 8,
  parameter int unsigned SyncStages = 2,
  localparam int unsigned PtrW     = $clog2(Depth),
  localparam int unsigned MemDepth = 2**PtrW,
  localparam int unsigned GrayW    = PtrW + 1,
  localparam logic [GrayW-1:0] PtrOne    = 1,
  localparam logic [GrayW-1:0] DepthValue = Depth[GrayW-1:0]
) (
  input  logic              clk_wr_i,
  input  logic              rst_wr_ni,
  input  logic              push_i,
  input  logic [Width-1:0]  wdata_i,
  output logic              ready_o,
  input  logic              clk_rd_i,
  input  logic              rst_rd_ni,
  input  logic              pop_i,
  output logic [Width-1:0]  rdata_o,
  output logic              valid_o,
  input  dft_t              dft_hv_i,
  input  dft_t              dft_lv_i
);

  // DFT reset bypass: write=LV, read=HV
  logic rst_wr_n, rst_rd_n;
  assign rst_wr_n = dft_lv_i.scanmode ? dft_lv_i.scan_reset_n : rst_wr_ni;
  assign rst_rd_n = dft_hv_i.scanmode ? dft_hv_i.scan_reset_n : rst_rd_ni;

  function automatic logic [GrayW-1:0] bin2gray(input logic [GrayW-1:0] bin);
    return bin ^ (bin >> 1);
  endfunction

  function automatic logic [GrayW-1:0] gray2bin(input logic [GrayW-1:0] gray);
    logic [GrayW-1:0] bin;
    bin[GrayW-1] = gray[GrayW-1];
    for (int i = GrayW-2; i >= 0; i--) bin[i] = bin[i+1] ^ gray[i];
    return bin;
  endfunction

  logic [Width-1:0] mem [MemDepth];

  // Write domain
  logic [GrayW-1:0] wptr_bin_q, wptr_gray_q, rptr_gray_sync;
  logic full;

  always_ff @(posedge clk_wr_i or negedge rst_wr_n) begin
    if (!rst_wr_n) begin
      wptr_bin_q  <= '0;
      wptr_gray_q <= '0;
    end else if (push_i & ~full) begin
      wptr_bin_q  <= wptr_bin_q + PtrOne;
      wptr_gray_q <= bin2gray(wptr_bin_q + PtrOne);
    end
  end

  always_ff @(posedge clk_wr_i) begin
    if (push_i & ~full) mem[wptr_bin_q[PtrW-1:0]] <= wdata_i;
  end

  logic [GrayW-1:0] rptr_bin_sync, occupancy_wr;
  assign rptr_bin_sync = gray2bin(rptr_gray_sync);
  assign occupancy_wr  = wptr_bin_q - rptr_bin_sync;
  assign full          = (occupancy_wr >= DepthValue);
  assign ready_o       = rst_wr_n & ~full;

  // Read domain
  logic [GrayW-1:0] rptr_bin_q, rptr_gray_q, wptr_gray_sync;
  logic empty;

  always_ff @(posedge clk_rd_i or negedge rst_rd_n) begin
    if (!rst_rd_n) begin
      rptr_bin_q  <= '0;
      rptr_gray_q <= '0;
    end else if (pop_i & ~empty) begin
      rptr_bin_q  <= rptr_bin_q + PtrOne;
      rptr_gray_q <= bin2gray(rptr_bin_q + PtrOne);
    end
  end

  assign rdata_o = mem[rptr_bin_q[PtrW-1:0]];
  assign empty   = (rptr_gray_q == wptr_gray_sync);
  assign valid_o = rst_rd_n & ~empty;

  // CDC synchronizers
  logic [GrayW-1:0] wptr_sync [SyncStages];
  always_ff @(posedge clk_rd_i or negedge rst_rd_n) begin
    if (!rst_rd_n) for (int i = 0; i < SyncStages; i++) wptr_sync[i] <= '0;
    else begin
      wptr_sync[0] <= wptr_gray_q;
      for (int i = 1; i < SyncStages; i++) wptr_sync[i] <= wptr_sync[i-1];
    end
  end
  assign wptr_gray_sync = wptr_sync[SyncStages-1];

  logic [GrayW-1:0] rptr_sync [SyncStages];
  always_ff @(posedge clk_wr_i or negedge rst_wr_n) begin
    if (!rst_wr_n) for (int i = 0; i < SyncStages; i++) rptr_sync[i] <= '0;
    else begin
      rptr_sync[0] <= rptr_gray_q;
      for (int i = 1; i < SyncStages; i++) rptr_sync[i] <= rptr_sync[i-1];
    end
  end
  assign rptr_gray_sync = rptr_sync[SyncStages-1];

  /* verilator lint_off UNUSEDSIGNAL */
  logic unused_dft;
  assign unused_dft = &{1'b0, dft_lv_i.sram_clk_override, dft_lv_i.ram_rei, dft_lv_i.ram_wei,
                               dft_hv_i.sram_clk_override, dft_hv_i.ram_rei, dft_hv_i.ram_wei};
  /* verilator lint_on UNUSEDSIGNAL */

endmodule
