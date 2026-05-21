// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module arb_prio_dly_rdy #(
  parameter int unsigned NUM_CLIENTS = 2,
  localparam int unsigned IdxW = (NUM_CLIENTS > 1) ? $clog2(NUM_CLIENTS) : 1
) (
  input  logic                    clock,
  input  logic                    reset,
  input  logic [NUM_CLIENTS-1:0]  c_valid,
  output logic [NUM_CLIENTS-1:0]  c_ready,
  output logic [IdxW-1:0]         c_winner,
  output logic                    valid,
  input  logic                    ready
);

  logic [IdxW-1:0] dly_prio_q;
  logic             busy_q;
  logic [IdxW-1:0]  prio_winner;

  always_comb begin
    prio_winner = '0;
    for (int unsigned i = 1; i < NUM_CLIENTS; i++) begin
      if (c_valid[i]) begin
        prio_winner = i[IdxW-1:0];
      end
    end
  end

  always_ff @(posedge clock) begin
    if (reset) begin
      busy_q     <= 1'b0;
      dly_prio_q <= '0;
    end else begin
      if (!ready && |c_valid && !busy_q) begin
        busy_q     <= 1'b1;
        dly_prio_q <= prio_winner;
      end else if (ready && busy_q) begin
        busy_q <= 1'b0;
      end
    end
  end

  assign c_winner = busy_q ? dly_prio_q : prio_winner;

  always_comb begin
    c_ready           = '0;
    c_ready[c_winner] = ready;
  end

  assign valid = |c_valid;

endmodule : arb_prio_dly_rdy
