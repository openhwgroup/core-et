// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module uncached_arb_lru_grant #(
  parameter int unsigned NumClients = 2,
  localparam int unsigned NumClientsLog = $clog2(NumClients)
) (
  input  logic                     clk_i,
  input  logic                     rst_ni,
  input  logic [NumClients-1:0]    bid_i,
  input  logic                     stall_i,
  output logic [NumClients-1:0]    grant_o,
  output logic [NumClientsLog-1:0] winner_o
);

  logic [NumClients-1:0][NumClients-1:0] prio_q;
  logic [NumClients-1:0][NumClients-1:0] prio_d;
  logic [NumClients-1:0] win_per_client;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      prio_q <= '0;
    end else if ((|bid_i) && !stall_i) begin
      prio_q <= prio_d;
    end
  end

  always_comb begin
    for (int unsigned i = 0; i < NumClients; i++) begin
      win_per_client[i] = bid_i[i];
      for (int unsigned j = 0; j < i; j++) begin
        win_per_client[i] = win_per_client[i] && (!bid_i[j] || prio_q[i][j]);
      end
      for (int unsigned j = i + 1; j < NumClients; j++) begin
        win_per_client[i] = win_per_client[i] && (!bid_i[j] || !prio_q[j][i]);
      end
      grant_o[i] = win_per_client[i];
    end
  end

  always_comb begin
    prio_d = prio_q;
    winner_o = '0;
    for (int unsigned i = 0; i < NumClients; i++) begin
      if (win_per_client[i]) begin
        winner_o = i[NumClientsLog-1:0];
      end
      if (grant_o[i]) begin
        for (int unsigned j = 0; j < i; j++) begin
          prio_d[i][j] = 1'b0;
        end
        for (int unsigned j = i + 1; j < NumClients; j++) begin
          prio_d[j][i] = 1'b1;
        end
      end
    end
  end

endmodule : uncached_arb_lru_grant
