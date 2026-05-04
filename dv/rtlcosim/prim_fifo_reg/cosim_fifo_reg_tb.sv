// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original gen_fifo_reg vs new prim_fifo_reg.

`include "soc.vh"

module cosim_fifo_reg_tb #(
  parameter int unsigned WIDTH = 8,
  parameter int unsigned DEPTH = 4
) (
  input  logic              clk_i,
  input  logic              rst_ni,

  // Shared stimulus
  input  logic              push_i,
  input  logic [WIDTH-1:0]  wdata_i,
  input  logic              pop_i,

  // New module outputs
  output logic              new_full_o,
  output logic              new_valid_o,
  output logic [WIDTH-1:0]  new_rdata_o,
  output logic [DEPTH:0]    new_level_o,

  // Original module outputs
  output logic              orig_full_o,
  output logic              orig_valid_o,
  output logic [WIDTH-1:0]  orig_rdata_o,
  output logic [DEPTH:0]    orig_level_o
);

  // ── Adapt reset ───────────────────────────────────────
  logic reset;
  assign reset = ~rst_ni;

  // ── New module ────────────────────────────────────────

  prim_fifo_reg #(
    .Width (WIDTH),
    .Depth (DEPTH)
  ) u_new (
    .clk_i,
    .rst_ni,
    .push_i,
    .wdata_i,
    .full_o   (new_full_o),
    .pop_i,
    .valid_o  (new_valid_o),
    .rdata_o  (new_rdata_o),
    .level_o  (new_level_o)
  );

  // ── Original module ───────────────────────────────────

  gen_fifo_reg #(
    .WIDTH (WIDTH),
    .DEPTH (DEPTH)
  ) u_orig (
    .clock (clk_i),
    .reset (reset),
    .push  (push_i),
    .wdata (wdata_i),
    .full  (orig_full_o),
    .pop   (pop_i),
    .valid (orig_valid_o),
    .rdata (orig_rdata_o),
    .level (orig_level_o)
  );

endmodule
