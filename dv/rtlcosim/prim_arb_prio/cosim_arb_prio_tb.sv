// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original arb_prio_data_held vs new prim_arb_prio.

`include "soc.vh"
`include "macros.vh"

module cosim_arb_prio_tb #(
  parameter int unsigned NUM_CLIENTS = 2,
  parameter int unsigned WIDTH       = 32
) (
  input  logic                                   clk_i,
  input  logic                                   rst_ni,

  // Shared stimulus
  input  logic [NUM_CLIENTS-1:0]                 bid_i,
  input  logic                                   stall_i,
  input  logic [NUM_CLIENTS-1:0][WIDTH-1:0]      data_i,

  // New module outputs
  output logic [NUM_CLIENTS-1:0]                 new_grant_o,
  output logic [WIDTH-1:0]                       new_data_o,

  // Original module outputs
  output logic [NUM_CLIENTS-1:0]                 orig_grant_o,
  output logic [WIDTH-1:0]                       orig_data_o
);

  // ── Adapt reset ───────────────────────────────────────
  logic reset;
  assign reset = ~rst_ni;

  // ── New module ────────────────────────────────────────

  prim_arb_prio #(
    .NumClients (NUM_CLIENTS),
    .Width      (WIDTH)
  ) u_new (
    .clk_i,
    .rst_ni,
    .bid_i,
    .stall_i,
    .grant_o  (new_grant_o),
    .data_i,
    .data_o   (new_data_o)
  );

  // ── Original module ───────────────────────────────────

  arb_prio_data_held #(
    .WIDTH       (WIDTH),
    .NUM_CLIENTS (NUM_CLIENTS)
  ) u_orig (
    .clock       (clk_i),
    .reset       (reset),
    .bid         (bid_i),
    .stall       (stall_i),
    .data_in     (data_i),
    .grant       (orig_grant_o),
    .data_winner (orig_data_o)
  );

endmodule
