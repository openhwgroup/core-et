// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module arb_rr #(
  parameter int unsigned NUM_REQS = 4
) (
  input  logic                 clock,
  input  logic                 reset,
  input  logic [NUM_REQS-1:0]  reqs,
  input  logic                 pop,
  output logic [NUM_REQS-1:0]  grants
);

  logic [NUM_REQS-1:0][0:0] unused_data_i;
  logic [0:0] unused_data_o;

  assign unused_data_i = '0;

  prim_arb_rr #(
    .NumReqs(NUM_REQS),
    .Width(1)
  ) u_arb (
    .clk_i(clock),
    .rst_ni(!reset),
    .reqs_i(reqs),
    .pop_i(pop),
    .grants_o(grants),
    .data_i(unused_data_i),
    .data_o(unused_data_o)
  );

endmodule : arb_rr
