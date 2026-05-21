// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Technology primitive: async CDC FIFO, write=low-voltage, read=high-voltage.
// ECP5 FPGA implementation.
//
// Uses the behavioral CDC structure on FPGA; the technology-specific split
// ensures ECP5 targets do not fall back implicitly to tech_generic.

module prim_fifo_async_lov
  import dft_pkg::*;
#(
  parameter int unsigned Width      = 32,
  parameter int unsigned Depth      = 8,
  parameter int unsigned SyncStages = 2,
  localparam int unsigned PtrW      = (Depth <= 1) ? 1 : $clog2(Depth),
  localparam int unsigned CntW      = $clog2(Depth + 1),
  localparam logic [PtrW-1:0] AddrOne    = {{(PtrW-1){1'b0}}, 1'b1},
  localparam logic [PtrW-1:0] LastAddr   = Depth[PtrW-1:0] - AddrOne,
  localparam logic [CntW-1:0] CountIncr  = 1,
  localparam logic [CntW-1:0] DepthCntW  = Depth[CntW-1:0]
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

  logic rst_wr_n;
  logic rst_rd_n;
  assign rst_wr_n = dft_lv_i.scanmode ? dft_lv_i.scan_reset_n : rst_wr_ni;
  assign rst_rd_n = dft_hv_i.scanmode ? dft_hv_i.scan_reset_n : rst_rd_ni;

  function automatic logic [CntW-1:0] bin2gray(input logic [CntW-1:0] bin);
    return bin ^ (bin >> 1);
  endfunction

  function automatic logic [CntW-1:0] gray2bin(input logic [CntW-1:0] gray);
    logic [CntW-1:0] bin;
    bin[CntW-1] = gray[CntW-1];
    for (int i = CntW-2; i >= 0; i--) begin
      bin[i] = bin[i+1] ^ gray[i];
    end
    return bin;
  endfunction

  logic [Width-1:0] mem [Depth];

  // The original vcfifo uses separate modulo-Depth memory addresses and
  // modulo-2^CntW Gray counters.  Keep that split for non-power-of-two depths.
  logic [PtrW-1:0] waddr_q;
  logic [CntW-1:0] wcnt_bin_q;
  logic [CntW-1:0] wcnt_gray_q;
  logic [CntW-1:0] rcnt_gray_sync;
  logic             full;

  always_ff @(posedge clk_wr_i or negedge rst_wr_n) begin
    if (!rst_wr_n) begin
      waddr_q     <= '0;
      wcnt_bin_q  <= '0;
      wcnt_gray_q <= '0;
    end else if (push_i & ~full) begin
      waddr_q     <= (waddr_q == LastAddr) ? '0 : (waddr_q + AddrOne);
      wcnt_bin_q  <= wcnt_bin_q + CountIncr;
      wcnt_gray_q <= bin2gray(wcnt_bin_q + CountIncr);
    end
  end

  always_ff @(posedge clk_wr_i) begin
    if (push_i & ~full) begin
      mem[waddr_q] <= wdata_i;
    end
  end

  logic [CntW-1:0] rcnt_bin_sync;
  logic [CntW-1:0] occupancy_wr;
  assign rcnt_bin_sync = gray2bin(rcnt_gray_sync);
  assign occupancy_wr  = wcnt_bin_q - rcnt_bin_sync;
  assign full          = (occupancy_wr == DepthCntW);
  assign ready_o       = rst_wr_n & ~full;

  logic [PtrW-1:0] raddr_q;
  logic [CntW-1:0] rcnt_bin_q;
  logic [CntW-1:0] rcnt_gray_q;
  logic [CntW-1:0] wcnt_gray_sync;
  logic             empty;

  always_ff @(posedge clk_rd_i or negedge rst_rd_n) begin
    if (!rst_rd_n) begin
      raddr_q     <= '0;
      rcnt_bin_q  <= '0;
      rcnt_gray_q <= '0;
    end else if (pop_i & ~empty) begin
      raddr_q     <= (raddr_q == LastAddr) ? '0 : (raddr_q + AddrOne);
      rcnt_bin_q  <= rcnt_bin_q + CountIncr;
      rcnt_gray_q <= bin2gray(rcnt_bin_q + CountIncr);
    end
  end

  assign rdata_o = mem[raddr_q];
  assign empty   = (rcnt_gray_q == wcnt_gray_sync);
  assign valid_o = rst_rd_n & ~empty;

  // Match the original WR_LOV phase relationship: write-count CDC stages are
  // sampled on the read clock directly; read-count CDC stages are sampled on
  // the inverted write clock and then registered once on the write clock.
  (* async_reg = "true" *) logic [CntW-1:0] wcnt_sync [SyncStages];
  always_ff @(posedge clk_rd_i or negedge rst_rd_n) begin
    if (!rst_rd_n) begin
      for (int i = 0; i < SyncStages; i++) begin
        wcnt_sync[i] <= '0;
      end
    end else begin
      wcnt_sync[0] <= wcnt_gray_q;
      for (int i = 1; i < SyncStages; i++) begin
        wcnt_sync[i] <= wcnt_sync[i-1];
      end
    end
  end
  assign wcnt_gray_sync = wcnt_sync[SyncStages-1];

  (* async_reg = "true" *) logic [CntW-1:0] rcnt_sync [SyncStages];
  logic [CntW-1:0] rcnt_gray_sync_q;
  always_ff @(negedge clk_wr_i or negedge rst_wr_n) begin
    if (!rst_wr_n) begin
      for (int i = 0; i < SyncStages; i++) begin
        rcnt_sync[i] <= '0;
      end
    end else begin
      rcnt_sync[0] <= rcnt_gray_q;
      for (int i = 1; i < SyncStages; i++) begin
        rcnt_sync[i] <= rcnt_sync[i-1];
      end
    end
  end
  always_ff @(posedge clk_wr_i or negedge rst_wr_n) begin
    if (!rst_wr_n) begin
      rcnt_gray_sync_q <= '0;
    end else begin
      rcnt_gray_sync_q <= rcnt_sync[SyncStages-1];
    end
  end
  assign rcnt_gray_sync = rcnt_gray_sync_q;

  /* verilator lint_off UNUSEDSIGNAL */
  logic unused_dft;
  assign unused_dft = &{1'b0, dft_lv_i.sram_clk_override, dft_lv_i.ram_rei, dft_lv_i.ram_wei,
                               dft_hv_i.sram_clk_override, dft_hv_i.ram_rei, dft_hv_i.ram_wei};
  /* verilator lint_on UNUSEDSIGNAL */

endmodule
