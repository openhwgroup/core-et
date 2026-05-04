// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


// This module implements an arbiter, and includes selection of data based on grant

module arb_rr_data #(parameter 
   NUM_REQS = 4,
   WIDTH    = 32
) (

   input  logic                           clock,
   input  logic                           reset,
   output logic [NUM_REQS-1:0]            grants,
   input  logic                           pop,
   input  logic [NUM_REQS-1:0]            reqs,
   input  logic [NUM_REQS-1:0][WIDTH-1:0] data_in,
   output logic [WIDTH-1:0]               data_out
);

   logic [NUM_REQS-1:0] grants_held;

   // round robin arbiter
   arb_rr_base #(.NUM_REQS ( NUM_REQS )) arb (
      .clock       (clock),
      .reset       (reset),
      .grants      (grants),
      .grants_held (grants_held),
      .pop         (pop),
      .reqs        (reqs)
   );
   
   // select data_out from data_in based on grant decision
   onehot_mux #(
      .WIDTH (WIDTH),
      .NUM_OH_SEL (NUM_REQS) 
   ) mux (
      .data_out (data_out),
      .sel      (grants_held),
      .data_in  (data_in)
   );


endmodule

