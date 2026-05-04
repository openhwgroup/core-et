// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "macros.vh"

// This module implements an high speed round robin arbiter, used as a slice
//
// Top half is duplicate of arb_rr, implementing a NUM_REQS:1 round-robin fair arbitration.  It is used just to maintain a local copy of the mask_code fairness flops
// Bottom half is a reduced sized slice, which uses the current (entire) mask_code value, and only the local reqs for this slice, to determine the current local winner.
//    This allows a local critical path of local (slice) reqs through arbitration, out to data muxing.
//    There is independent timing path from all reqs to mask_code flops.  All reqs may come from far away.
//
// Slicing selection is determined by the REQS_MASK parameter to indicate which reqs are local and should be considered for local data muxing control
   

module arb_rr_slice #(parameter NUM_REQS=4,
                parameter ARB_SLICE_MODE=0,   // Enable Arbiter slice mode:  This arbitrates fairly over NUM_REQS reqs, but only drives a slice of data muxes with grants
                parameter REQS_MASK=0) (      // 0=reqs bit is considered for grants.  1=reqs bit is not considered for grant
   input  logic                clock,
   input  logic                reset,
   output logic [NUM_REQS-1:0] grants,
   output logic [NUM_REQS-1:0] masked_grants,
   input  logic                pop,
   input  logic [NUM_REQS-1:0] reqs
);

   logic [NUM_REQS-1:0] full_grants;
   logic [NUM_REQS-1:0] slice_grants;
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
   assign full_grants = muxed_reqs_code ^ mask_code_next; // grant is simple xor to find one-hot location of selected reqs code

   // mask_code
   wire any_req = |reqs;
   wire req_taken = any_req && pop;
   `RST_EN_FF(clock, reset, req_taken, mask_code, mask_code_next, '0)

   assign grants        = full_grants;    // May be used for popping source
   assign masked_grants = slice_grants;   // May be used for local slice mux control

   generate
      if (ARB_SLICE_MODE) begin : slice_mode

         // Code is identical copy/paste of above arbiter, except mask reqs by fixed constant pattern to ignore timing of remote reqs
         // (And generate-endgenerate pairs are removed because already in a generate block.  generate blocks are redundant!)
         wire  [NUM_REQS-1:0] sl_slice_reqs  = reqs & ~REQS_MASK;        // Mask reqs for this slice - will determine grants only (not true mask_code next state)
         wire  [NUM_REQS-1:0] sl_masked_reqs = mask_code & sl_slice_reqs;   // Use masked reqs.  (Note confusing nomenclature copied here)

         // therm_code generation
         logic [NUM_REQS-1:0] sl_reqs_code;
         logic [NUM_REQS-1:0] sl_masked_reqs_code;
         genvar               sl_bn;
         for (sl_bn=0; sl_bn<NUM_REQS; sl_bn=sl_bn+1) begin : reqs_code_gen
            assign sl_reqs_code[sl_bn]        = |( sl_slice_reqs[0 +: (sl_bn+1)]);
            assign sl_masked_reqs_code[sl_bn] = |(sl_masked_reqs[0 +: (sl_bn+1)]);
         end
         
         wire sl_any_masked_req = |sl_masked_reqs;
         wire [NUM_REQS-1:0] sl_muxed_reqs_code = sl_any_masked_req ? sl_masked_reqs_code : sl_reqs_code;
         wire [NUM_REQS-1:0] sl_mask_code_next;
         if (NUM_REQS == 1) begin : sl_eq1
            assign sl_mask_code_next = 1'b0;
         end
         else begin : sl_gt1
            assign sl_mask_code_next = {sl_muxed_reqs_code[0 +: (NUM_REQS-1)], 1'b0}; // shift muxed_reqs_code left by one-bit
         end
         assign slice_grants = sl_muxed_reqs_code ^ sl_mask_code_next; // grant is simple xor to find one-hot location of selected reqs code

      end
      else begin : full_mode
         assign slice_grants = full_grants;
      end
   endgenerate
   

endmodule
