// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"

// This module implements a high speed round robin arbiter
// that returns 2 grants. The grants are flopped for timing.
// The grants are reevaluated when an alloc, dealloc or reqs esr reprogramming is detected

module arb_rr2 #(parameter NUM_REQS=4) (
   input  logic                        clock,
   input  logic                        reset,
   output logic                        valid,
   output logic                        valid2,
   output logic [NUM_REQS-1:0]         grants,
   output logic [NUM_REQS-1:0]         grants2,
   output logic [$clog2(NUM_REQS)-1:0] grants_id,
   output logic [$clog2(NUM_REQS)-1:0] grants2_id,
   input  logic                        alloc,
   input  logic                        esr_wr,
   input  logic [NUM_REQS-1:0]         reqs
);

   logic [NUM_REQS-1:0] reqs_valid;
   logic [NUM_REQS-1:0] grants_pre;
   logic [NUM_REQS-1:0] grants2_pre;
   logic [NUM_REQS-1:0] grants3_pre;

   assign reqs_valid  = reqs 
     & ~(grants & {NUM_REQS{valid}}) 
     & ~(grants2 & {NUM_REQS{valid2}});

   arb_rr #(
     .NUM_REQS(NUM_REQS)
   ) arb_alloc (
      .clock         (clock),
      .reset         (reset),
      .reqs          (reqs_valid),
      .grants        (grants_pre),
      .pop           (alloc)
   );

   arb_grants2 #(
     .NUM_REQS(NUM_REQS)
   ) arb_alloc2 (
      .reqs          (reqs_valid),
      .grants        (grants2_pre),
      .grants2       (grants3_pre)
   );

   wire grants_en     = alloc | ~valid | ~valid2 | esr_wr;
   wire valid_pre     = |(grants_pre);

   logic [NUM_REQS-1:0] grants2_arb;
   wire  [NUM_REQS-1:0] grants2_diff = (grants2_pre ^ grants_pre);
   wire  [NUM_REQS-1:0] grants3_diff = (grants3_pre ^ grants_pre);
   wire grants2_valid = |(grants2_diff);
   wire grants3_valid = |(grants3_diff);
   wire valid2_pre    = grants2_valid | grants3_valid;
   assign grants2_arb = grants3_valid ? grants3_pre : grants2_pre;

   `EN_FF(clock, grants_en, grants, grants_pre)
   `EN_FF(clock, grants_en, grants2, grants2_arb)
   `RST_EN_FF(clock, reset, grants_en, valid, valid_pre, 1'b0)
   `RST_EN_FF(clock, reset, grants_en, valid2, valid2_pre, 1'b0)

   hot2bin #(
      .HOT_W(NUM_REQS)
   ) alloc_grants_bin (
      .hot(grants),
      .bin(grants_id)
   );
   hot2bin #(
      .HOT_W(NUM_REQS)
   ) alloc_grants2_bin (
      .hot(grants2),
      .bin(grants2_id)
   );

endmodule
