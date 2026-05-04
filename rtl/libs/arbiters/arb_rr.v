// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"

// This module implements an high speed round robin arbiter

module arb_rr #(parameter NUM_REQS=4) (
   input  logic                clock,
   input  logic                reset,
   output logic [NUM_REQS-1:0] grants,
   input  logic                pop,
   input  logic [NUM_REQS-1:0] reqs
);

   logic [NUM_REQS-1:0] grants_held_unused_ok;

   // round robin arbiter
   arb_rr_base #(.NUM_REQS ( NUM_REQS )) arb (
      .clock       (clock),
      .reset       (reset),
      .grants      (grants),
      .grants_held (grants_held_unused_ok),
      .pop         (pop),
      .reqs        (reqs)
   );
   
endmodule
