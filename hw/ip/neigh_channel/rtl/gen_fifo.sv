// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module gen_fifo #(
  parameter int unsigned ELEM_SIZE = 32,
  parameter int unsigned NUM_ELEMS = 4,
  parameter bit          USE_LATCHES = 1'b0
) (
  input  logic                  clock,
  input  logic                  reset,
  input  logic                  push_early,
  input  logic                  push,
  input  logic [ELEM_SIZE-1:0]  push_data,
  output logic                  full,
  output logic                  full_next,
  input  logic                  pop,
  output logic [ELEM_SIZE-1:0]  pop_data,
  output logic                  valid,
  output logic                  valid_next
);

  prim_fifo #(
    .Width(ELEM_SIZE),
    .Depth(NUM_ELEMS),
    .UseLatches(USE_LATCHES)
  ) u_fifo (
    .clk_i(clock),
    .rst_ni(!reset),
    .push_early_i(push_early),
    .push_i(push),
    .push_data_i(push_data),
    .full_o(full),
    .full_next_o(full_next),
    .pop_i(pop),
    .pop_data_o(pop_data),
    .valid_o(valid),
    .valid_next_o(valid_next)
  );

endmodule : gen_fifo
