// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original arb_rr_data vs new prim_arb_rr.

`include "macros.vh"

module cosim_arb_rr_tb #(
  parameter int unsigned NUM_REQS = 4,
  parameter int unsigned WIDTH    = 32
) (
  input  logic                            clk_i,
  input  logic                            rst_ni,

  // Shared stimulus
  input  logic [NUM_REQS-1:0]             reqs_i,
  input  logic                            pop_i,
  input  logic [NUM_REQS-1:0][WIDTH-1:0]  data_i,

  // New module outputs
  output logic [NUM_REQS-1:0]             new_grants_o,
  output logic [WIDTH-1:0]                new_data_o,

  // Original module outputs
  output logic [NUM_REQS-1:0]             orig_grants_o,
  output logic [WIDTH-1:0]                orig_data_o
);

  // ── Adapt reset: new uses active-low async, original uses active-high sync ──
  logic reset;
  assign reset = ~rst_ni;

  // ── New module ────────────────────────────────────────────
  prim_arb_rr #(
    .NumReqs (NUM_REQS),
    .Width   (WIDTH)
  ) u_new (
    .clk_i,
    .rst_ni,
    .reqs_i,
    .pop_i,
    .grants_o (new_grants_o),
    .data_i,
    .data_o   (new_data_o)
  );

  // ── Original module ───────────────────────────────────────
  arb_rr_data #(
    .NUM_REQS (NUM_REQS),
    .WIDTH    (WIDTH)
  ) u_orig (
    .clock    (clk_i),
    .reset    (reset),
    .reqs     (reqs_i),
    .pop      (pop_i),
    .grants   (orig_grants_o),
    .data_in  (data_i),
    .data_out (orig_data_o)
  );

endmodule
