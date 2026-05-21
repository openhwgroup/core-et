// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_uncached_arb_lru_grant_tb #(
  parameter int unsigned NumClients = 5,
  localparam int unsigned NumClientsLog = $clog2(NumClients)
) (
  input  logic                    clk_i,
  input  logic                    rst_ni,
  input  logic [NumClients-1:0]   bid_i,
  input  logic                    stall_i,
  output logic [NumClients-1:0]   orig_grant_o,
  output logic [NumClients-1:0]   new_grant_o,
  output logic [NumClientsLog-1:0] orig_winner_o,
  output logic [NumClientsLog-1:0] new_winner_o
);

  arb_lru_grant_orig #(
    .NUM_CLIENTS(NumClients),
    .NUM_CLIENTS_LOG(NumClientsLog)
  ) u_orig (
    .clock(clk_i),
    .reset(!rst_ni),
    .bid(bid_i),
    .stall(stall_i),
    .grant(orig_grant_o),
    .winner(orig_winner_o)
  );

  uncached_arb_lru_grant #(
    .NumClients(NumClients)
  ) u_new (
    .clk_i(clk_i),
    .rst_ni(rst_ni),
    .bid_i(bid_i),
    .stall_i(stall_i),
    .grant_o(new_grant_o),
    .winner_o(new_winner_o)
  );

endmodule : cosim_uncached_arb_lru_grant_tb
