// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_shared_ptw #(
  parameter int unsigned NReqs = 5
) (
  input  logic                              clk_i,
  input  logic                              rst_ni,
  input  logic                              ioshire_i,
  input  minion_pkg::minion_ptw_req         req_data_i [NReqs],
  input  logic                  [NReqs-1:0] req_valid_i,
  output logic                  [NReqs-1:0] req_ready_o,
  input  logic                  [NReqs-1:0] invalidate_i,
  input  minion_dcache_pkg::esr_mprot_t     mprot_i,
  input  minion_pkg::tlb_entry_type         vmspagesize_i,
  output minion_pkg::minion_ptw_pte         resp_data_o,
  output logic                  [NReqs-1:0] resp_valid_o,
  input  logic                              mem_miss_req_disable_next_i,
  output etlink_pkg::neigh_req_t            mem_miss_req_info_o,
  output logic                              mem_miss_req_valid_o,
  input  logic                              mem_miss_req_ready_i,
  input  etlink_pkg::rsp_t                  mem_fill_req_info_i,
  input  logic                              mem_fill_req_valid_i,
  output logic                              mem_fill_req_ready_o,
  output neigh_pkg::ptw_dbg_sm_t            dbg_sm_signals_o
);

  localparam int unsigned ReqIdxWidth = (NReqs > 1) ? $clog2(NReqs) : 1;

  minion_pkg::minion_ptw_req req_winner;
  logic                      ready;
  logic [ReqIdxWidth-1:0]    req_dest;
  logic [ReqIdxWidth-1:0]    req_dest_q;
  logic                      ptw_resp_valid;
  neigh_pkg::ptw_dbg_sm_t    ptw_dbg_sm_signals;

  generate
    if (NReqs == 1) begin : gen_single_req
      assign req_winner    = req_data_i[0];
      assign req_dest      = '0;
      assign req_ready_o[0] = req_valid_i[0] && ready;
    end else begin : gen_multi_req
      /* verilator lint_off UNOPTFLAT */  // Mirrors the original arb_lru_data grant bus shape.
      logic [NReqs-1:0] arb_grant;
      /* verilator lint_on UNOPTFLAT */

      prim_arb_lru #(
        .NumClients(NReqs)
      ) u_req_arb (
        .clk_i   (clk_i),
        .rst_ni  (rst_ni),
        .bid_i   (req_valid_i),
        .stall_i (!ready),
        .grant_o (arb_grant),
        .winner_o(req_dest)
      );

      assign req_ready_o = arb_grant;
      assign req_winner  = req_data_i[req_dest];
    end
  endgenerate

  ptw_top u_ptw (
    .clk_i                      (clk_i),
    .rst_ni                     (rst_ni),
    .ioshire_i                  (ioshire_i),
    .req_data_i                 (req_winner),
    .req_valid_i                (|req_valid_i),
    .req_ready_o                (ready),
    .invalidate_i               (|invalidate_i),
    .mprot_i                    (mprot_i),
    .vmspagesize_i              (vmspagesize_i),
    .resp_data_o                (resp_data_o),
    .resp_valid_o               (ptw_resp_valid),
    .mem_miss_req_disable_next_i(mem_miss_req_disable_next_i),
    .mem_miss_req_info_o        (mem_miss_req_info_o),
    .mem_miss_req_valid_o       (mem_miss_req_valid_o),
    .mem_miss_req_ready_i       (mem_miss_req_ready_i),
    .mem_fill_req_info_i        (mem_fill_req_info_i),
    .mem_fill_req_valid_i       (mem_fill_req_valid_i),
    .mem_fill_req_ready_o       (mem_fill_req_ready_o),
    .dbg_sm_signals_o           (ptw_dbg_sm_signals)
  );

  always_ff @(posedge clk_i) begin
    if ((|req_valid_i) && ready) begin
      req_dest_q <= req_dest;
    end
  end

  always_comb begin
    for (int i = 0; i < NReqs; i++) begin
      resp_valid_o[i] = ptw_resp_valid && (req_dest_q == i[ReqIdxWidth-1:0]);
    end
  end

  always_comb begin
    dbg_sm_signals_o = ptw_dbg_sm_signals;
    dbg_sm_signals_o.req_satp_mode = req_winner.satp_mode;
    dbg_sm_signals_o.req_prv       = req_winner.prv;
    dbg_sm_signals_o.req_addr      = req_winner.addr;
  end

endmodule : neigh_shared_ptw
