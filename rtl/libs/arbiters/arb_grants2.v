// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"

// This module implements a high speed arbiter that returns 2 grants. 
// One of the grant is selected from reqs LSB and the other from the reqs MSB.
// The 2 grants can be unique or the same and check needs to be performed to 
// determine if the 2nd grants2 is unique.

module arb_grants2 #(parameter NUM_REQS=4) (
   output logic [NUM_REQS-1:0] grants,
   output logic [NUM_REQS-1:0] grants2,
   input  logic [NUM_REQS-1:0] reqs
);

   // therm_code generation
   logic [NUM_REQS-1:0] reqs_code;
   logic [NUM_REQS-1:0] reqs_code2;
   generate
      genvar bn;
      for (bn=0; bn<NUM_REQS; bn=bn+1) begin : reqs_code_gen_block
         assign reqs_code[bn]  = |(reqs[0 +: (bn+1)]);
         assign reqs_code2[bn] = |(reqs[(NUM_REQS-1) : bn]);
      end
   endgenerate

   wire [NUM_REQS-1:0] mask_code_next;
   wire [NUM_REQS-1:0] mask_code_next2;
   generate
      if (NUM_REQS == 1) begin : eq1
         assign mask_code_next = 1'b0;
         assign mask_code_next2 = 1'b0;
      end
      else begin : gt1
         assign mask_code_next = {reqs_code[0 +: (NUM_REQS-1)], 1'b0}; // shift reqs_code left by one-bit
         assign mask_code_next2 = {1'b0, reqs_code2[(NUM_REQS-1):1]}; // shift reqs_code right by one-bit
      end
   endgenerate
   assign grants = reqs_code ^ mask_code_next; // grant is simple xor to find one-hot location of selected reqs code
   assign grants2 = reqs_code2 ^ mask_code_next2; // grant is simple xor to find one-hot location of selected reqs code

endmodule
