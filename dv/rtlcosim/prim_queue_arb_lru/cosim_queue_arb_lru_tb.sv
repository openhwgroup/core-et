// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_queue_arb_lru_tb #(
  parameter int unsigned NUM_CLIENTS = 4
) (
  input  logic                            clk_i,
  input  logic                            rst_ni,
  input  logic                            prio_up_i,
  input  logic [$clog2(NUM_CLIENTS)-1:0]  prio_entry_i,
  input  logic [NUM_CLIENTS-1:0]          bid_i,
  output logic [$clog2(NUM_CLIENTS)-1:0]  new_winner_o,
  output logic [$clog2(NUM_CLIENTS)-1:0]  orig_winner_o
);

  logic reset;
  assign reset = ~rst_ni;

  prim_queue_arb_lru #(
    .NumClients (NUM_CLIENTS)
  ) u_new (
    .clk_i,
    .rst_ni,
    .prio_up_i,
    .prio_entry_i,
    .bid_i,
    .winner_o (new_winner_o)
  );

  queue_arb_lru #(
    .NUM_CLIENTS (NUM_CLIENTS)
  ) u_orig (
    .clock      (clk_i),
    .reset      (reset),
    .prio_up    (prio_up_i),
    .prio_entry (prio_entry_i),
    .bid        (bid_i),
    .winner     (orig_winner_o)
  );

endmodule
