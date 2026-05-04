// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_bist_mbx vs new shirecache_bist_mbx.
//
// Both modules receive identical stimulus. Key outputs are compared
// cycle-by-cycle via scalar ports driven/sampled by C++.

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off UNUSEDPARAM */
/* verilator lint_off VARHIDDEN */
module cosim_bist_mbx_tb #(
  parameter int unsigned DataSize = 16,
  parameter int unsigned AddrSize = 4,
  parameter int unsigned RamDelay = 2
) (
  input  logic                    clk_i,
  input  logic                    rst_ni,

  // MBIST inputs (shared stimulus)
  input  logic                    mbx_impl_i,
  input  logic                    mbist_sel_i,
  input  logic                    mbist_rd_en_i,
  input  logic                    mbist_wr_en_i,
  input  logic [AddrSize-1:0]     mbist_addr_i,
  input  logic [DataSize-1:0]     mbist_wdata_i,

  // mbx_rdata from design memory (shared input)
  input  logic [DataSize-1:0]     mbx_rdata_i,

  // ── Original outputs ──────────────────────────────────────────
  output logic [DataSize-1:0]     orig_mbist_rdata_o,
  output logic                    orig_mbx_sel_o,
  output logic                    orig_mbx_rd_en_o,
  output logic                    orig_mbx_wr_en_o,
  output logic [AddrSize-1:0]     orig_mbx_addr_o,
  output logic [DataSize-1:0]     orig_mbx_wdata_o,

  // ── New outputs ───────────────────────────────────────────────
  output logic [DataSize-1:0]     new_mbist_rdata_o,
  output logic                    new_mbx_sel_o,
  output logic                    new_mbx_rd_en_o,
  output logic                    new_mbx_wr_en_o,
  output logic [AddrSize-1:0]     new_mbx_addr_o,
  output logic [DataSize-1:0]     new_mbx_wdata_o
);

  // ── Original module (active-HIGH reset) ───────────────────────
  shire_cache_bist_mbx #(
    .BIST_SEQ_START_DELAY (1),
    .NUM_ADDR_TO_TEST     (10),
    .DATA_SIZE            (DataSize),
    .ADDR_SIZE            (AddrSize),
    .RAM_DELAY            (RamDelay),
    .READ_DELAY           (3),
    .LOGICAL_RAM_DEPTH    (1 << AddrSize)
  ) u_orig (
    .clock       (clk_i),
    .reset       (~rst_ni),
    .mbx_impl    (mbx_impl_i),

    .mbist_sel   (mbist_sel_i),
    .mbist_rd_en (mbist_rd_en_i),
    .mbist_wr_en (mbist_wr_en_i),
    .mbist_addr  (mbist_addr_i),
    .mbist_wdata (mbist_wdata_i),
    .mbist_rdata (orig_mbist_rdata_o),

    .mbx_sel     (orig_mbx_sel_o),
    .mbx_rd_en   (orig_mbx_rd_en_o),
    .mbx_wr_en   (orig_mbx_wr_en_o),
    .mbx_addr    (orig_mbx_addr_o),
    .mbx_wdata   (orig_mbx_wdata_o),
    .mbx_rdata   (mbx_rdata_i)
  );

  // ── New module (active-LOW reset) ─────────────────────────────
  shirecache_bist_mbx #(
    .DataSize        (DataSize),
    .AddrSize        (AddrSize),
    .RamDelay        (RamDelay),
    .LogicalRamDepth (1 << AddrSize)
  ) u_new (
    .clk_i        (clk_i),
    .rst_ni       (rst_ni),
    .mbx_impl_i   (mbx_impl_i),

    .mbist_sel_i   (mbist_sel_i),
    .mbist_rd_en_i (mbist_rd_en_i),
    .mbist_wr_en_i (mbist_wr_en_i),
    .mbist_addr_i  (mbist_addr_i),
    .mbist_wdata_i (mbist_wdata_i),
    .mbist_rdata_o (new_mbist_rdata_o),

    .mbx_sel_o     (new_mbx_sel_o),
    .mbx_rd_en_o   (new_mbx_rd_en_o),
    .mbx_wr_en_o   (new_mbx_wr_en_o),
    .mbx_addr_o    (new_mbx_addr_o),
    .mbx_wdata_o   (new_mbx_wdata_o),
    .mbx_rdata_i   (mbx_rdata_i)
  );

  /* verilator lint_on UNUSEDPARAM */
  /* verilator lint_on VARHIDDEN */
  /* verilator lint_on UNOPTFLAT */
  /* verilator lint_on UNUSEDSIGNAL */

endmodule
