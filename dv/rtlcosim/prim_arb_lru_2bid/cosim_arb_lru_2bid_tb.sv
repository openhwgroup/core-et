// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original arb_lru_2_bid vs new prim_arb_lru_2bid.

`include "macros.vh"
`include "assert.vh"
`include "shire_assert.vh"

module cosim_arb_lru_2bid_tb #(
  parameter int unsigned NUM_CLIENTS = 5
) (
  input  logic                      clk_i,
  input  logic                      rst_ni,

  // Shared stimulus
  input  logic [NUM_CLIENTS-1:0]    bid0_i,
  input  logic                      pop0_i,
  input  logic [NUM_CLIENTS-1:0]    bid1_i,
  input  logic                      pop1_i,

  // New module outputs
  output logic [NUM_CLIENTS-1:0]    new_grant0_o,
  output logic [NUM_CLIENTS-1:0]    new_grant1_o,

  // Original module outputs
  output logic [NUM_CLIENTS-1:0]    orig_grant0_o,
  output logic [NUM_CLIENTS-1:0]    orig_grant1_o
);

  // ── Adapt reset: new uses active-low async, original uses active-high sync ──
  logic reset;
  assign reset = ~rst_ni;

  // ── New module ────────────────────────────────────────────
  prim_arb_lru_2bid #(
    .NumClients (NUM_CLIENTS)
  ) u_new (
    .clk_i,
    .rst_ni,
    .bid0_i,
    .grant0_o (new_grant0_o),
    .pop0_i,
    .bid1_i,
    .grant1_o (new_grant1_o),
    .pop1_i
  );

  // ── Original module ───────────────────────────────────────
  arb_lru_2_bid #(
    .NUM_CLIENTS (NUM_CLIENTS)
  ) u_orig (
    .clock    (clk_i),
    .reset    (reset),
    .bid0     (bid0_i),
    .grant0   (orig_grant0_o),
    .pop0     (pop0_i),
    .bid1     (bid1_i),
    .grant1   (orig_grant1_o),
    .pop1     (pop1_i)
  );

endmodule
