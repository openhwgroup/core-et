// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_evict_ff
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic [NumShireIdsBits-1:0]                  shire_id_i,
  input  logic [1:0]                                  neigh_id_i,
  input  logic                                        clk_i,
  input  logic                                        rst_ni,
  input  et_link_minion_evict_req_info_t              l2_dcache_req_i,
  input  logic [DcacheL2EvictReqPorts-1:0]            l2_dcache_req_valid_i,
  output logic [DcacheL2EvictReqPorts-1:0]            l2_dcache_req_ready_o,
  input  logic [2:0]                                  local_channel_mask_i,
  input  logic [2:0][2:0]                             local_dest_id_i,
  input  logic                                        local_message_ready_i,
  output logic [2:0]                                  local_message_valid_o,
  output etlink_pkg::neigh_rsp_t                      local_message_info_o,
  output etlink_pkg::neigh_req_t                      evict_req_o,
  output logic                                        evict_valid_o,
  input  logic                                        evict_ready_i,
  output etlink_pkg::neigh_req_t                      coop_evict_req_o,
  output logic                                        coop_evict_valid_o,
  input  logic                                        coop_evict_ready_i,
  output logic [DcacheL2EvictReqPorts-1:0]            dbg_sm_signals_o
);

  localparam int unsigned DcacheL2EvictIdxTstore = 0;
  localparam int unsigned DcacheL2EvictIdxComm   = 1;

  neigh_evict_ff_unit u_neigh_evict_ff_regular (
    .clk_i,
    .rst_ni,
    .shire_id_i,
    .neigh_id_i,
    .l2_dcache_req_i,
    .l2_dcache_req_valid_i(l2_dcache_req_valid_i[DcacheL2EvictIdxComm]),
    .l2_dcache_req_ready_o(l2_dcache_req_ready_o[DcacheL2EvictIdxComm]),
    .local_channel_mask_i,
    .local_dest_id_i,
    .local_message_ready_i,
    .local_message_valid_o,
    .local_message_info_o,
    .evict_req_o,
    .evict_valid_o,
    .evict_ready_i,
    .dbg_sm_signals_o(dbg_sm_signals_o[DcacheL2EvictIdxComm])
  );

  /* verilator lint_off PINCONNECTEMPTY */  // Cooperative tensor-store evicts intentionally disable local-message outputs.
  neigh_evict_ff_unit u_neigh_evict_ff_tscoop (
    .clk_i,
    .rst_ni,
    .shire_id_i,
    .neigh_id_i,
    .l2_dcache_req_i,
    .l2_dcache_req_valid_i(l2_dcache_req_valid_i[DcacheL2EvictIdxTstore]),
    .l2_dcache_req_ready_o(l2_dcache_req_ready_o[DcacheL2EvictIdxTstore]),
    .local_channel_mask_i('0),
    .local_dest_id_i('0),
    .local_message_ready_i(1'b0),
    .local_message_valid_o(),
    .local_message_info_o(),
    .evict_req_o(coop_evict_req_o),
    .evict_valid_o(coop_evict_valid_o),
    .evict_ready_i(coop_evict_ready_i),
    .dbg_sm_signals_o(dbg_sm_signals_o[DcacheL2EvictIdxTstore])
  );
  /* verilator lint_on PINCONNECTEMPTY */

endmodule
