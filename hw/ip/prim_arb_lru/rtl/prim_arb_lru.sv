// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// LRU arbiter primitive.
//
// Maintains a priority-bits triangle matrix to track LRU ordering.
// When bid[i] is asserted, client i is requesting. The arbiter
// grants the least-recently-used bidder. After a grant, the winner
// is moved to MRU position.
//
// Replaces: arb_lru (etcore-soc/rtl/libs/arbiters/arb_lru.v)
// Differences:
//   - Active-low async-assert/sync-deassert reset (rst_ni)
//   - lowRISC naming (_i/_o, _d/_q)
//   - always_ff / always_comb (no macros)

module prim_arb_lru #(
  parameter int unsigned NumClients    = 8,
  parameter int unsigned NumClientsLog = $clog2(NumClients)
) (
  input  logic                       clk_i,
  input  logic                       rst_ni,
  input  logic [NumClients-1:0]      bid_i,
  input  logic                       stall_i,
  output logic [NumClients-1:0]      grant_o,
  output logic [NumClientsLog-1:0]   winner_o
);

  // ── Priority-bits triangle matrix ─────────────────────
  logic [NumClients-1:0][NumClients-1:0] prio_q, prio_d;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      prio_q <= '0;
    end else if ((|bid_i) && !stall_i) begin
      prio_q <= prio_d;
    end
  end

  // ── Winner computation ────────────────────────────────
  logic [NumClients-1:0] win_per_client;

  always_comb begin
    for (int unsigned i = 0; i < NumClients; i++) begin
      win_per_client[i] = bid_i[i];
      // Clients with lower ids: this client wins if it has higher priority
      for (int unsigned j = 0; j < i; j++) begin
        win_per_client[i] = win_per_client[i] && (~bid_i[j] || prio_q[i][j]);
      end
      // Clients with higher ids
      for (int unsigned j = i + 1; j < NumClients; j++) begin
        win_per_client[i] = win_per_client[i] && (~bid_i[j] || ~prio_q[j][i]);
      end
      grant_o[i] = win_per_client[i] && ~stall_i;
    end
  end

  // ── Priority update ───────────────────────────────────
  always_comb begin
    prio_d   = prio_q;
    winner_o = '0;
    for (int unsigned i = 0; i < NumClients; i++) begin
      if (win_per_client[i])
        winner_o = NumClientsLog'(i);
      if (grant_o[i]) begin
        // Winner gets lowest priority
        for (int unsigned j = 0; j < i; j++)
          prio_d[i][j] = 1'b0;
        for (int unsigned j = i + 1; j < NumClients; j++)
          prio_d[j][i] = 1'b1;
      end
    end
  end

endmodule : prim_arb_lru
