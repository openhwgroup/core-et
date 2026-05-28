// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Technology primitive: async CDC FIFO, write=high-voltage, read=low-voltage.
// Generic (behavioral) implementation.
// On ASIC, replaced by foundry-specific level shifters and CDC cells.
//
// Replaces: vcfifo_wr_hiv_gcd (etcore-soc)

module prim_fifo_async_hiv
  import dft_pkg::*;
#(
  parameter int unsigned Width      = 32,
  parameter int unsigned Depth      = 8,
  parameter int unsigned SyncStages = 2,
  localparam int unsigned PtrW     = (Depth <= 1) ? 1 : $clog2(Depth),
  localparam int unsigned CntW     = $clog2(Depth + 1),
  localparam logic [PtrW-1:0] AddrOne    = {{(PtrW-1){1'b0}}, 1'b1},
  localparam logic [PtrW-1:0] LastAddr   = Depth[PtrW-1:0] - AddrOne,
  localparam logic [CntW-1:0] CountOne   = 1,
  localparam logic [CntW-1:0] DepthValue = Depth[CntW-1:0]
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

  // DFT reset bypass: write=HV, read=LV
  logic rst_wr_n, rst_rd_n;
  assign rst_wr_n = dft_hv_i.scanmode ? dft_hv_i.scan_reset_n : rst_wr_ni;
  assign rst_rd_n = dft_lv_i.scanmode ? dft_lv_i.scan_reset_n : rst_rd_ni;

  function automatic logic [CntW-1:0] bin2gray(input logic [CntW-1:0] bin);
    return bin ^ (bin >> 1);
  endfunction

  function automatic logic [CntW-1:0] gray2bin(input logic [CntW-1:0] gray);
    logic [CntW-1:0] bin;
    bin[CntW-1] = gray[CntW-1];
    for (int i = CntW-2; i >= 0; i--) bin[i] = bin[i+1] ^ gray[i];
    return bin;
  endfunction

  logic [Width-1:0] mem [Depth];

  // Write domain.  The original vcfifo uses separate modulo-Depth memory
  // addresses and modulo-2^CntW Gray counters; preserve that behavior for
  // non-power-of-two FIFO depths such as the uncached AXI crossings.
  logic [PtrW-1:0] waddr_q;
  logic [CntW-1:0] wcnt_bin_q, wcnt_gray_q, rcnt_gray_sync;
  logic full;

  always_ff @(posedge clk_wr_i or negedge rst_wr_n) begin
    if (!rst_wr_n) begin
      waddr_q     <= '0;
      wcnt_bin_q  <= '0;
      wcnt_gray_q <= '0;
    end else if (push_i & ~full) begin
      waddr_q     <= (waddr_q == LastAddr) ? '0 : (waddr_q + AddrOne);
      wcnt_bin_q  <= wcnt_bin_q + CountOne;
      wcnt_gray_q <= bin2gray(wcnt_bin_q + CountOne);
    end
  end

  always_ff @(posedge clk_wr_i) begin
    if (push_i & ~full) mem[waddr_q] <= wdata_i;
  end

  logic [CntW-1:0] rcnt_bin_sync, occupancy_wr;
  assign rcnt_bin_sync = gray2bin(rcnt_gray_sync);
  assign occupancy_wr  = wcnt_bin_q - rcnt_bin_sync;
  assign full          = (occupancy_wr == DepthValue);
  assign ready_o       = rst_wr_n & ~full;

  // Read domain
  logic [PtrW-1:0] raddr_q;
  logic [CntW-1:0] rcnt_bin_q, rcnt_gray_q, wcnt_gray_sync;
  logic empty;

  always_ff @(posedge clk_rd_i or negedge rst_rd_n) begin
    if (!rst_rd_n) begin
      raddr_q     <= '0;
      rcnt_bin_q  <= '0;
      rcnt_gray_q <= '0;
    end else if (pop_i & ~empty) begin
      raddr_q     <= (raddr_q == LastAddr) ? '0 : (raddr_q + AddrOne);
      rcnt_bin_q  <= rcnt_bin_q + CountOne;
      rcnt_gray_q <= bin2gray(rcnt_bin_q + CountOne);
    end
  end

  assign rdata_o = mem[raddr_q];
  assign empty   = (rcnt_gray_q == wcnt_gray_sync);
  assign valid_o = rst_rd_n & ~empty;

  // CDC synchronizers.  The original WR_HIV vcfifo samples write-count CDC
  // stages on the inverted read clock, then registers the synchronized count
  // once on the read clock in vcfifo_rd_pd_gcd.  The read-count CDC path into
  // the write domain is sampled on the write clock directly.
  logic [CntW-1:0] wcnt_sync [SyncStages];
  logic [CntW-1:0] wcnt_gray_sync_q;
  always_ff @(negedge clk_rd_i or negedge rst_rd_n) begin
    if (!rst_rd_n) for (int i = 0; i < SyncStages; i++) wcnt_sync[i] <= '0;
    else begin
      wcnt_sync[0] <= wcnt_gray_q;
      for (int i = 1; i < SyncStages; i++) wcnt_sync[i] <= wcnt_sync[i-1];
    end
  end
  always_ff @(posedge clk_rd_i or negedge rst_rd_n) begin
    if (!rst_rd_n) wcnt_gray_sync_q <= '0;
    else           wcnt_gray_sync_q <= wcnt_sync[SyncStages-1];
  end
  assign wcnt_gray_sync = wcnt_gray_sync_q;

  logic [CntW-1:0] rcnt_sync [SyncStages];
  always_ff @(posedge clk_wr_i or negedge rst_wr_n) begin
    if (!rst_wr_n) for (int i = 0; i < SyncStages; i++) rcnt_sync[i] <= '0;
    else begin
      rcnt_sync[0] <= rcnt_gray_q;
      for (int i = 1; i < SyncStages; i++) rcnt_sync[i] <= rcnt_sync[i-1];
    end
  end
  assign rcnt_gray_sync = rcnt_sync[SyncStages-1];

  /* verilator lint_off UNUSEDSIGNAL */
  logic unused_dft;
  assign unused_dft = &{1'b0, dft_hv_i.sram_clk_override, dft_hv_i.ram_rei, dft_hv_i.ram_wei,
                               dft_lv_i.sram_clk_override, dft_lv_i.ram_rei, dft_lv_i.ram_wei};
  /* verilator lint_on UNUSEDSIGNAL */

endmodule
