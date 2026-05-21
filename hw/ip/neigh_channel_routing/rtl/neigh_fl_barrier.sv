// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_fl_barrier #(
  parameter int unsigned NrMinions = neigh_pkg::MinPerN,
  parameter int unsigned FlbWidth  = minion_pkg::CsrFlBarrierSize,
  parameter int unsigned FifoDepth = neigh_pkg::NeighFlbElemsPerNeigh,
  localparam int unsigned MinionIdxWidth = (NrMinions > 1) ? $clog2(NrMinions) : 1
) (
  input  logic                                      clk_i,
  input  logic                                      rst_ni,

  input  logic [NrMinions-1:0]                      flb_core_neigh_req_valid_i,
  input  logic [NrMinions-1:0][FlbWidth-1:0]        flb_core_neigh_req_data_i,

  output logic [NrMinions-1:0]                      flb_neigh_core_resp_valid_o,
  output logic                                      flb_neigh_core_resp_data_o,

  output logic                                      flb_neigh_l2_req_valid_o,
  output logic [FlbWidth-1:0]                       flb_neigh_l2_req_data_o,

  input  logic                                      flb_l2_neigh_resp_valid_i,
  input  logic                                      flb_l2_neigh_resp_data_i
);

  logic [NrMinions-1:0][FlbWidth-1:0] data_q;
  logic [NrMinions-1:0]               pending_q;
  logic [NrMinions-1:0]               pending_d;
  logic [MinionIdxWidth-1:0]          arb_winner;
  logic [NrMinions-1:0]               arb_grant_unused;
  logic                               req_fifo_full;
  logic                               req_fifo_full_next_unused;
  logic                               req_fifo_valid_unused;
  logic                               req_fifo_valid_next_unused;
  logic                               flb_neigh_l2_req_valid_d;
  logic [MinionIdxWidth-1:0]          req_fifo_pop_data;
  logic [NrMinions-1:0]               flb_neigh_core_resp_valid_d;

  always_comb begin
    pending_d = pending_q;

    if (!req_fifo_full) begin
      pending_d[arb_winner] = 1'b0;
    end

    for (int unsigned i = 0; i < NrMinions; i++) begin
      if (flb_core_neigh_req_valid_i[i]) begin
        pending_d[i] = 1'b1;
      end
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      pending_q <= '0;
    end else begin
      pending_q <= pending_d;
    end
  end

  // Original EN_FF storage has no reset; preserve the unreset data registers.
  always_ff @(posedge clk_i) begin
    for (int unsigned i = 0; i < NrMinions; i++) begin
      if (flb_core_neigh_req_valid_i[i]) begin
        data_q[i] <= flb_core_neigh_req_data_i[i];
      end
    end
  end

  prim_arb_lru #(
    .NumClients(NrMinions),
    .NumClientsLog(MinionIdxWidth)
  ) u_arb_to_l2 (
    .clk_i    (clk_i),
    .rst_ni   (rst_ni),
    .bid_i    (pending_q),
    .stall_i  (req_fifo_full),
    .grant_o  (arb_grant_unused),
    .winner_o (arb_winner)
  );

  assign flb_neigh_l2_req_valid_d = (|pending_q) && !req_fifo_full;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      flb_neigh_l2_req_valid_o <= 1'b0;
    end else begin
      flb_neigh_l2_req_valid_o <= flb_neigh_l2_req_valid_d;
    end
  end

  // Original EN_FF storage has no reset; preserve the unreset L2 request data register.
  always_ff @(posedge clk_i) begin
    if (flb_neigh_l2_req_valid_d) begin
      flb_neigh_l2_req_data_o <= data_q[arb_winner];
    end
  end

  prim_fifo #(
    .Width     (MinionIdxWidth),
    .Depth     (FifoDepth),
    .UseLatches(1'b0)
  ) u_req_fifo (
    .clk_i         (clk_i),
    .rst_ni        (rst_ni),
    .push_early_i  (1'b0),
    .push_i        (flb_neigh_l2_req_valid_d),
    .push_data_i   (arb_winner),
    .full_o        (req_fifo_full),
    .full_next_o   (req_fifo_full_next_unused),
    .pop_i         (flb_l2_neigh_resp_valid_i),
    .pop_data_o    (req_fifo_pop_data),
    .valid_o       (req_fifo_valid_unused),
    .valid_next_o  (req_fifo_valid_next_unused)
  );

  always_comb begin
    flb_neigh_core_resp_valid_d = '0;
    if (flb_l2_neigh_resp_valid_i) begin
      flb_neigh_core_resp_valid_d[req_fifo_pop_data] = 1'b1;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      flb_neigh_core_resp_valid_o <= '0;
    end else begin
      flb_neigh_core_resp_valid_o <= flb_neigh_core_resp_valid_d;
    end
  end

  // Original EN_FF storage has no reset; preserve the unreset response-data register.
  always_ff @(posedge clk_i) begin
    if (flb_l2_neigh_resp_valid_i) begin
      flb_neigh_core_resp_data_o <= flb_l2_neigh_resp_data_i;
    end
  end

  logic unused_ok;
  assign unused_ok = ^{1'b0, arb_grant_unused, req_fifo_full_next_unused,
                       req_fifo_valid_unused, req_fifo_valid_next_unused};

endmodule : neigh_fl_barrier
