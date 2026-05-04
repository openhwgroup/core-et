// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"

// This module implements an high speed round robin arbiter

// It also outputs a held version of grants so that data can be held constant if 
// there are no new reqs to take.  grants == grants_held if there is something 
// valid, otherwise grants_held = previous valid grants. 

module arb_rr_base #(parameter NUM_REQS=4) (
   input  logic                clock,
   input  logic                reset,
   output logic [NUM_REQS-1:0] grants,
   output logic [NUM_REQS-1:0] grants_held,
   input  logic                pop,
   input  logic [NUM_REQS-1:0] reqs
);

   logic [NUM_REQS-1:0] mask_code;
   wire  [NUM_REQS-1:0] masked_reqs = mask_code & reqs;

   // therm_code generation
   logic [NUM_REQS-1:0] reqs_code;
   logic [NUM_REQS-1:0] masked_reqs_code;
   generate
      genvar bn;
      for (bn=0; bn<NUM_REQS; bn=bn+1) begin : reqs_code_gen_block
         assign reqs_code[bn]        = |(       reqs[0 +: (bn+1)]);
         assign masked_reqs_code[bn] = |(masked_reqs[0 +: (bn+1)]);
      end
   endgenerate

   wire any_masked_req = |masked_reqs;
   wire [NUM_REQS-1:0] muxed_reqs_code = any_masked_req ? masked_reqs_code : reqs_code;
   wire [NUM_REQS-1:0] mask_code_next;
   generate
      if (NUM_REQS == 1) begin : eq1
         assign mask_code_next = 1'b0;
      end
      else begin : gt1
         assign mask_code_next = {muxed_reqs_code[0 +: (NUM_REQS-1)], 1'b0}; // shift muxed_reqs_code left by one-bit
      end
   endgenerate
   assign grants = muxed_reqs_code ^ mask_code_next; // grant is simple xor to find one-hot location of selected reqs code

   //assign grants_held = (|grants) ? grants : (mask_code ^ {1'b1, mask_code[NUM_REQS-1:1]}); 
   //assign grants_held = (|grants) ? grants : (mask_code ^ {1'b1, mask_code[1 +: NUM_REQS-1]}); 
   assign grants_held = (|grants) ? grants : mask_code ^ ({1'b1, {NUM_REQS-1{1'b0}}} | (mask_code >> 1)); 

   // mask_code
   wire any_req = |reqs;
   wire req_taken = any_req && pop;
   `RST_EN_FF(clock, reset, req_taken, mask_code, mask_code_next, '0)

endmodule
