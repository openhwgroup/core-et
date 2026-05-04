// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Dual-bid LRU arbiter.
//
// Maintains a full LRU priority-bits triangle matrix.
// Two grants are computed per cycle from independent bid vectors.
// Only one pop per cycle (either pop0 or pop1, never both).
// The popped grant updates the LRU state.
//
// This enables a priority bid (bid0) and an opportunistic bid (bid1)
// to coexist: bid0 identifies the LRU winner among all valid FIFOs,
// while bid1 identifies the LRU winner among FIFOs whose resources
// are available. Pop is issued on the opportunistic side only.
//
// Replaces: arb_lru_2_bid (etcore-soc/rtl/libs/arbiters/arb_lru_2_bid.v)
// Differences:
//   - Active-low async-assert/sync-deassert reset (rst_ni)
//   - lowRISC naming (_i/_o, UpperCamelCase parameter)
//   - always_ff / always_comb (no macros)

module prim_arb_lru_2bid #(
  parameter int unsigned NumClients = 8
) (
  input  logic                    clk_i,
  input  logic                    rst_ni,

  // Bid / grant 0 — priority channel
  input  logic [NumClients-1:0]   bid0_i,
  output logic [NumClients-1:0]   grant0_o,
  input  logic                    pop0_i,

  // Bid / grant 1 — opportunistic channel
  input  logic [NumClients-1:0]   bid1_i,
  output logic [NumClients-1:0]   grant1_o,
  input  logic                    pop1_i
);

  // ── Priority-bits triangle matrix ─────────────────────
  logic [NumClients-1:0][NumClients-1:0] prio_q, prio_d;

  logic prio_en;
  assign prio_en = ((|bid0_i) & pop0_i) | ((|bid1_i) & pop1_i);

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      prio_q <= '0;
    end else if (prio_en) begin
      prio_q <= prio_d;
    end
  end

  // ── Winner computation ────────────────────────────────
  logic [NumClients-1:0] win_per_client0;
  logic [NumClients-1:0] win_per_client1;

  always_comb begin
    for (int unsigned i = 0; i < NumClients; i++) begin
      win_per_client0[i] = bid0_i[i];
      win_per_client1[i] = bid1_i[i];
      // Clients with lower ids: this client wins if it has higher priority
      for (int unsigned j = 0; j < i; j++) begin
        win_per_client0[i] = win_per_client0[i] & (~bid0_i[j] | prio_q[i][j]);
        win_per_client1[i] = win_per_client1[i] & (~bid1_i[j] | prio_q[i][j]);
      end
      // Clients with higher ids
      for (int unsigned j = i + 1; j < NumClients; j++) begin
        win_per_client0[i] = win_per_client0[i] & (~bid0_i[j] | ~prio_q[j][i]);
        win_per_client1[i] = win_per_client1[i] & (~bid1_i[j] | ~prio_q[j][i]);
      end
      grant0_o[i] = win_per_client0[i];
      grant1_o[i] = win_per_client1[i];
    end
  end

  // Select which winner to use for priority update
  logic [NumClients-1:0] win_per_client;
  assign win_per_client = pop0_i ? win_per_client0 : win_per_client1;

  // ── Priority update ───────────────────────────────────
  always_comb begin
    prio_d = prio_q;
    for (int unsigned i = 0; i < NumClients; i++) begin
      if (win_per_client[i]) begin
        // Winner gets lowest priority
        for (int unsigned j = 0; j < i; j++)
          prio_d[i][j] = 1'b0;
        for (int unsigned j = i + 1; j < NumClients; j++)
          prio_d[j][i] = 1'b1;
      end
    end
  end

endmodule : prim_arb_lru_2bid
