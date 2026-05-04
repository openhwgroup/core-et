// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module neigh_shared_ptw #(
  parameter N_REQS = 5
) (
  // System signals
  input  logic                       clock,
  input  logic                       reset,
  // Is this in the IOShire
  input  logic                       ioshire,
  // Requests
  input  minion_ptw_req [N_REQS-1:0] req_data,
  input  logic          [N_REQS-1:0] req_valid,
  output logic          [N_REQS-1:0] req_ready,
  input  logic          [N_REQS-1:0] invalidate,
  // Neighborhood ESRs
  input  esr_mprot_t                 mprot,
  input  tlb_entry_type              vmspagesize,
  // Responses
  output minion_ptw_pte              resp_data,
  output logic          [N_REQS-1:0] resp_valid,
  // Memory interface
  // L2 Request bus
  input  logic                       mem_miss_req_disable_next,
  output et_link_neigh_req_info_t    mem_miss_req_info,
  output logic                       mem_miss_req_valid,
  input  logic                       mem_miss_req_ready,
  // L2 Fill/Ack bus
  input  et_link_rsp_info_t          mem_fill_req_info,
  input  logic                       mem_fill_req_valid,
  output logic                       mem_fill_req_ready,
  // Output debug signals going to Status Monitor
  output ptw_dbg_sm_t                dbg_sm_signals
  );

  // //////////////////////////////////////////////////////////////////////////////
  // Request arbiter
  // //////////////////////////////////////////////////////////////////////////////
  minion_ptw_req req_winner;
  logic ready;
  logic [$clog2(N_REQS)-1:0] req_dest, req_dest_reg;

  arb_lru_data #(
    .WIDTH       ( $bits(minion_ptw_req) ),
    .NUM_CLIENTS ( N_REQS                )
  ) req_arb (
    // System signals
    .clock        ( clock                 ),
    .reset        ( reset                 ),
    // Bidding
    .bid          ( req_valid             ),
    .stall        ( ~ready                ),
    // Data
    .data_in      ( req_data              ),
    // Winner
    .grant        ( req_ready             ),
    .data_winner  ( req_winner            ),
    .winner       ( req_dest              )
  );

  // //////////////////////////////////////////////////////////////////////////////
  // ptw instance
  // //////////////////////////////////////////////////////////////////////////////
  logic        ptw_resp_valid;
  ptw_dbg_sm_t ptw_dbg_sm_signals;

  ptw_top ptw (
    .clock                     ( clock                     ),
    .reset                     ( reset                     ),
    // Is this in the IOShire
    .ioshire                   ( ioshire                   ),
    // Request port
    .req_data                  ( req_winner                ),
    .req_valid                 ( |req_valid                ),
    .req_ready                 ( ready                     ),
    // Neighborhood ESRs
    .mprot                     ( mprot                     ),
    .vmspagesize               ( vmspagesize               ),
    .invalidate                ( |invalidate               ),
    // Response port
    .resp_data                 ( resp_data                 ),
    .resp_valid                ( ptw_resp_valid            ),
    // Memory interface
    // L2 Request bus
    .mem_miss_req_disable_next ( mem_miss_req_disable_next ),
    .mem_miss_req_info         ( mem_miss_req_info         ),
    .mem_miss_req_valid        ( mem_miss_req_valid        ),
    .mem_miss_req_ready        ( mem_miss_req_ready        ),
    // L2 Fill/Ack bus
    .mem_fill_req_info         ( mem_fill_req_info         ),
    .mem_fill_req_valid        ( mem_fill_req_valid        ),
    .mem_fill_req_ready        ( mem_fill_req_ready        ),
    // Output debug signals going to Status Monitor
    .dbg_sm_signals            ( ptw_dbg_sm_signals        )
  );

  // Control destination requestor
  // -----------------------------
  always_ff @(posedge clock) begin
    if (|req_valid && ready)
    req_dest_reg <= req_dest;
  end

  always_comb begin
    for (integer i = 0; i < N_REQS; i++)
    resp_valid[i] = ptw_resp_valid && (req_dest_reg == i[$clog2(N_REQS)-1:0]);
  end

  // //////////////////////////////////////////////////////////////////////////////
  // Connect output debug signals
  // //////////////////////////////////////////////////////////////////////////////
  always_comb begin
    // Take signals from PTW
    dbg_sm_signals = ptw_dbg_sm_signals;

    // Connect shared_ptw signals
    dbg_sm_signals.req_satp_mode = req_winner.satp_mode;
    dbg_sm_signals.req_prv       = req_winner.prv;
    dbg_sm_signals.req_addr      = req_winner.addr;
  end

endmodule
