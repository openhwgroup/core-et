// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original arb_rr2 vs new prim_arb_rr2.

`include "macros.vh"

module cosim_arb_rr2_tb #(
  parameter int unsigned NUM_REQS = 64
) (
  input  logic                            clk_i,
  input  logic                            rst_ni,

  // Shared stimulus
  input  logic [NUM_REQS-1:0]             reqs_i,
  input  logic                            alloc_i,
  input  logic                            esr_wr_i,

  // New module outputs
  output logic                            new_valid_o,
  output logic                            new_valid2_o,
  output logic [NUM_REQS-1:0]             new_grants_o,
  output logic [NUM_REQS-1:0]             new_grants2_o,
  output logic [$clog2(NUM_REQS)-1:0]     new_grants_id_o,
  output logic [$clog2(NUM_REQS)-1:0]     new_grants2_id_o,

  // Original module outputs
  output logic                            orig_valid_o,
  output logic                            orig_valid2_o,
  output logic [NUM_REQS-1:0]             orig_grants_o,
  output logic [NUM_REQS-1:0]             orig_grants2_o,
  output logic [$clog2(NUM_REQS)-1:0]     orig_grants_id_o,
  output logic [$clog2(NUM_REQS)-1:0]     orig_grants2_id_o
);

  // ── Adapt reset: new uses active-low async, original uses active-high sync ──
  logic reset;
  assign reset = ~rst_ni;

  // ── New module ────────────────────────────────────────────
  prim_arb_rr2 #(
    .NumReqs (NUM_REQS)
  ) u_new (
    .clk_i,
    .rst_ni,
    .reqs_i,
    .alloc_i,
    .esr_wr_i,
    .valid_o      (new_valid_o),
    .valid2_o     (new_valid2_o),
    .grants_o     (new_grants_o),
    .grants2_o    (new_grants2_o),
    .grants_id_o  (new_grants_id_o),
    .grants2_id_o (new_grants2_id_o)
  );

  // ── Original module ───────────────────────────────────────
  arb_rr2 #(
    .NUM_REQS (NUM_REQS)
  ) u_orig (
    .clock     (clk_i),
    .reset     (reset),
    .reqs      (reqs_i),
    .alloc     (alloc_i),
    .esr_wr    (esr_wr_i),
    .valid     (orig_valid_o),
    .valid2    (orig_valid2_o),
    .grants    (orig_grants_o),
    .grants2   (orig_grants2_o),
    .grants_id (orig_grants_id_o),
    .grants2_id(orig_grants2_id_o)
  );

endmodule
