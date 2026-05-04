// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Queue-style LRU arbiter.
//
// Tracks insertion order rather than updating priority on grants. New entries
// are pushed to the MRU position via `prio_up_i` / `prio_entry_i`, and the
// least-recently-inserted bidder wins.

module prim_queue_arb_lru #(
  parameter int unsigned NumClients    = 8,
  parameter int unsigned NumClientsLog = $clog2(NumClients)
) (
  input  logic                     clk_i,
  input  logic                     rst_ni,
  input  logic                     prio_up_i,
  input  logic [NumClientsLog-1:0] prio_entry_i,
  input  logic [NumClients-1:0]    bid_i,
  output logic [NumClientsLog-1:0] winner_o
);

  logic [NumClients-1:0][NumClients-1:0] prio_q, prio_d;
  logic [NumClients-1:0] win_per_client;

  /* verilator lint_off SYNCASYNCNET */  // rst_ni is the required async-assert/sync-deassert reset for this primitive.
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      prio_q <= '0;
    end else begin
      prio_q <= prio_d;
    end
  end
  /* verilator lint_on SYNCASYNCNET */

  always_comb begin
    winner_o = '0;
    for (int unsigned i = 0; i < NumClients; i++) begin
      win_per_client[i] = bid_i[i];
      for (int unsigned j = 0; j < i; j++) begin
        win_per_client[i] = win_per_client[i] && (~bid_i[j] || prio_q[i][j]);
      end
      for (int unsigned j = i + 1; j < NumClients; j++) begin
        win_per_client[i] = win_per_client[i] && (~bid_i[j] || ~prio_q[j][i]);
      end
      if (win_per_client[i]) begin
        winner_o = i[NumClientsLog-1:0];
      end
    end
  end

  always_comb begin
    prio_d = prio_q;
    for (int unsigned i = 0; i < NumClients; i++) begin
      if (prio_up_i && (i[NumClientsLog-1:0] == prio_entry_i)) begin
        for (int unsigned j = 0; j < i; j++) begin
          prio_d[i][j] = 1'b0;
        end
        for (int unsigned j = i + 1; j < NumClients; j++) begin
          prio_d[j][i] = 1'b1;
        end
      end
    end
  end

endmodule : prim_queue_arb_lru
