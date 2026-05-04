// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Priority arbiter with data selection and output hold.
//
// Grants the highest-indexed bidder. When stall_i is asserted, no grant
// is issued. When no bidder wins, data_o holds the previous winner's data.
//
// Replaces: arb_prio_data_held + arb_prio (etcore-soc)

module prim_arb_prio #(
  parameter int unsigned NumClients = 2,
  parameter int unsigned Width      = 32,
  localparam int unsigned IdxW      = $clog2(NumClients)
) (
  input  logic                                clk_i,
  input  logic                                rst_ni,

  input  logic [NumClients-1:0]               bid_i,
  input  logic                                stall_i,
  output logic [NumClients-1:0]               grant_o,
  input  logic [NumClients-1:0][Width-1:0]    data_i,
  output logic [Width-1:0]                    data_o
);

  // Priority encoder — highest index wins
  logic [IdxW-1:0]        winner;
  logic                   valid;

  always_comb begin
    winner = '0;
    for (int i = 1; i < NumClients; i++) begin
      if (bid_i[i]) winner = IdxW'(i);
    end
  end

  assign valid = |bid_i;

  // Only the highest-priority winner gets a grant (matching original arb_prio
  // which sets c_ready[c_winner] = ready, all others = 0).
  always_comb begin
    grant_o = '0;
    if (!stall_i) grant_o[winner] = bid_i[winner];
  end

  // Hold previous winner index when no new grant
  logic new_winner;
  assign new_winner = |grant_o;

  logic [IdxW-1:0] prev_winner_q;
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)            prev_winner_q <= '0;
    else if (new_winner)    prev_winner_q <= winner;
  end

  logic [IdxW-1:0] winner_sel;
  assign winner_sel = new_winner ? winner : prev_winner_q;
  assign data_o = data_i[winner_sel];

  logic unused_ok;
  assign unused_ok = &{1'b0, valid};

endmodule : prim_arb_prio
