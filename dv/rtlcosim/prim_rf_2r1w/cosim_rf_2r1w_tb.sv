// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation testbench: prim_rf_2r1w vs rf_latch_2r_1w.
//
// Parameters match intpipe_rf usage:
//   Width=64, Entries=64, Zero={2{32'b1}}, Parallel={2{32'h80000000}},
//   ParallelW=128, Level2CkGate=1.

`include "soc.vh"

module cosim_rf_2r1w_tb (
  input  logic         clk_i,
  input  logic         rst_ni,

  // Stimulus
  input  logic [5:0]   rd_addr_a_i,
  input  logic [5:0]   rd_addr_b_i,
  input  logic         wr_en_i,
  input  logic         wr_data_en_1p_i,
  input  logic [5:0]   wr_addr_i,
  input  logic [63:0]  wr_data_i,

  // Outputs — new module
  output logic [63:0]  new_rd_data_a_o,
  output logic [63:0]  new_rd_data_b_o,
  output logic [127:0] new_rd_par_o,

  // Outputs — original module
  output logic [63:0]  orig_rd_data_a_o,
  output logic [63:0]  orig_rd_data_b_o,
  output logic [127:0] orig_rd_par_o
);

  localparam int unsigned Width      = 64;
  localparam int unsigned Entries    = 64;
  localparam [Entries-1:0] Zero     = {2{32'b1}};           // entries 0 and 32
  localparam [Entries-1:0] Parallel = {2{32'h80000000}};    // entries 31 and 63
  localparam int unsigned ParallelW  = 128;
  localparam int unsigned Level2     = 1;

  // ── New module ──
  prim_rf_2r1w #(
    .Width       (Width),
    .Entries     (Entries),
    .Zero        (Zero),
    .Parallel    (Parallel),
    .ParallelW   (ParallelW),
    .Level2CkGate(Level2)
  ) u_new (
    .clk_i,
    .rd_addr_a_i,
    .rd_data_a_o (new_rd_data_a_o),
    .rd_addr_b_i,
    .rd_data_b_o (new_rd_data_b_o),
    .rd_par_o    (new_rd_par_o),
    .wr_en_i,
    .wr_data_en_1p_i,
    .wr_addr_i,
    .wr_data_i
  );

  // ── Original module ──
  rf_latch_2r_1w #(
    .WIDTH          (Width),
    .ENTRIES        (Entries),
    .ZERO           (Zero),
    .PARALLEL       (Parallel),
    .PARALLEL_W     (ParallelW),
    .LEVEL2_CLK_GATE(Level2)
  ) u_orig (
    .clk            (clk_i),
    .test_en        (1'b0),
    .rd_addr_a      (rd_addr_a_i),
    .rd_data_a      (orig_rd_data_a_o),
    .rd_addr_b      (rd_addr_b_i),
    .rd_data_b      (orig_rd_data_b_o),
    .rd_par         (orig_rd_par_o),
    .wr_en_a        (wr_en_i),
    .wr_data_a_en_1p(wr_data_en_1p_i),
    .wr_addr_a      (wr_addr_i),
    .wr_data_a      (wr_data_i)
  );

endmodule
