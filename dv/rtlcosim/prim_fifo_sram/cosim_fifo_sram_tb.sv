// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original rbox_fifo vs new prim_fifo_sram.
// Both modules receive identical stimulus; outputs are exposed for
// cycle-by-cycle comparison in C++.

`include "soc.vh"

module cosim_fifo_sram_tb #(
  parameter int unsigned WIDTH = 64,
  parameter int unsigned DEPTH = 64,
  parameter int unsigned PORTS = 2
) (
  input  logic              clk_i,
  input  logic              rst_ni,

  // Shared stimulus
  input  logic              wr_en_i,
  input  logic [WIDTH-1:0]  wr_data_i,
  input  logic              rd_en_i,

  // New module outputs
  output logic              new_wr_done_o,
  output logic              new_rd_valid_o,
  output logic [WIDTH-1:0]  new_rd_data_o,

  // Original module outputs
  output logic              orig_wr_done_o,
  output logic              orig_rd_valid_o,
  output logic [WIDTH-1:0]  orig_rd_data_o
);

  // ── Adapt reset polarity ──────────────────────────────
  // New: active-low (rst_ni). Original: active-high (reset).
  logic reset;
  assign reset = ~rst_ni;

  // ── Tie off ASIC-only ports ───────────────────────────
  esr_shire_cache_ram_cfg_t ram_cfg;
  assign ram_cfg = '0;

  // ── New module ────────────────────────────────────────

  prim_fifo_sram #(
    .Width (WIDTH),
    .Depth (DEPTH),
    .Ports (PORTS)
  ) u_new (
    .clk_i,
    .rst_ni,
    .wr_en_i,
    .wr_data_i,
    .wr_done_o  (new_wr_done_o),
    .rd_en_i,
    .rd_valid_o (new_rd_valid_o),
    .rd_data_o  (new_rd_data_o),
    .ram_cfg_i  (ram_cfg_pkg::ram_cfg_t'('0)),
    .dft_i      (dft_pkg::dft_t'('0))
  );

  // ── Original module ───────────────────────────────────

  rbox_fifo #(
    .WIDTH (WIDTH),
    .DEPTH (DEPTH),
    .PORTS (PORTS)
  ) u_orig (
    .clock                  (clk_i),
    .reset                  (reset),
    .esr_shire_cache_ram_cfg(ram_cfg),
    .wr_data                (wr_data_i),
    .wr_en                  (wr_en_i),
    .wr_done                (orig_wr_done_o),
    .rd_en                  (rd_en_i),
    .rd_valid               (orig_rd_valid_o),
    .rd_data                (orig_rd_data_o)
  );

endmodule
