// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

////////////////////////////////////////////////////////////////////////////
// Each minion has its own FF to store the evict. It waits until the 512b
// of data are available before going to arbiter to get access to the L2.
// A given minion can't evict a new cacheline until the FF is emptied
////////////////////////////////////////////////////////////////////////////

module neigh_evict_ff (
  // System signals
  input  logic                                              clock,
  input  logic                                              reset,
  input  logic  [`AD_ESR_SHIRE_ID_SIZE-1:0]                 shire_id,
  input  logic  [`NUM_NEIGH_R]                              neigh_id,
  // Evict request from minion
  input  et_link_minion_evict_req_info_t                    l2_dcache_req,
  input  logic [`DCACHE_L2_EVICT_REQ_PORTS-1:0]             l2_dcache_req_valid,
  output logic [`DCACHE_L2_EVICT_REQ_PORTS-1:0]             l2_dcache_req_ready,
  // Reduce local interconnect
  input  logic [`NEIGH_MAX_LOCAL_CONNECT-1:0]               local_channel_mask,
  input  logic [`NEIGH_MAX_LOCAL_CONNECT-1:0][`MIN_PER_N_R] local_dest_id,
  input  logic                                              local_message_ready,
  output logic [`NEIGH_MAX_LOCAL_CONNECT-1:0]               local_message_valid,
  output et_link_neigh_rsp_info_t                           local_message_info,
  // Data to L2
  output et_link_neigh_req_info_t                           evict_req,
  output logic                                              evict_valid,
  input  logic                                              evict_ready,
  // Data to cooperative tstore module
  output et_link_neigh_req_info_t                           coop_evict_req,
  output logic                                              coop_evict_valid,
  input  logic                                              coop_evict_ready,

  // Output debug signals going to Status Monitor
  output logic [`DCACHE_L2_EVICT_REQ_PORTS-1:0]             dbg_sm_signals
  );

    ////////////////////////////////////////////////////////////////////////////////
    // This unit stores all the outstanding evict requests for a given minion
    // pending to be sent to the L2 and already out of the minion. There are 2
    // different outstanding evict requests per minion:
    //  - Regular evict requests
    //  - Cooperative evict requests for TStores
    //  Regular evict requests go to the L2 arbiter
    //  Cooperative TStores go to the neigh_tensor_store_buffer unit
    ////////////////////////////////////////////////////////////////////////////////

    neigh_evict_ff_unit neigh_evict_ff_regular (
      .clock                ( clock                                          ),
      .reset                ( reset                                          ),
      .shire_id             ( shire_id                                       ),
      .neigh_id             ( neigh_id                                       ),

      .l2_dcache_req        ( l2_dcache_req                                  ),
      .l2_dcache_req_valid  ( l2_dcache_req_valid[`DCACHE_L2_EVICT_IDX_COMM] ),
      .l2_dcache_req_ready  ( l2_dcache_req_ready[`DCACHE_L2_EVICT_IDX_COMM] ),

      .local_channel_mask   ( local_channel_mask                             ),
      .local_dest_id        ( local_dest_id                                  ),
      .local_message_ready  ( local_message_ready                            ),
      .local_message_valid  ( local_message_valid                            ),
      .local_message_info   ( local_message_info                             ),

      .evict_req            ( evict_req                                      ),
      .evict_valid          ( evict_valid                                    ),
      .evict_ready          ( evict_ready                                    ),

      .dbg_sm_signals       ( dbg_sm_signals[`DCACHE_L2_EVICT_IDX_COMM]      )
    );

    logic [`NEIGH_MAX_LOCAL_CONNECT-1:0] local_message_valid_unused_ok;
    et_link_neigh_rsp_info_t             local_message_info_unused_ok;
    neigh_evict_ff_unit neigh_evict_ff_tscoop (
      .clock                ( clock                                            ),
      .reset                ( reset                                            ),
      .shire_id             ( shire_id                                         ),
      .neigh_id             ( neigh_id                                         ),

      .l2_dcache_req        ( l2_dcache_req                                    ),
      .l2_dcache_req_valid  ( l2_dcache_req_valid[`DCACHE_L2_EVICT_IDX_TSTORE] ),
      .l2_dcache_req_ready  ( l2_dcache_req_ready[`DCACHE_L2_EVICT_IDX_TSTORE] ),

      .local_channel_mask   ( '0                                               ),
      .local_dest_id        ( '0                                               ),
      .local_message_ready  ( 1'b0                                             ),
      .local_message_valid  ( local_message_valid_unused_ok                    ),
      .local_message_info   ( local_message_info_unused_ok                     ),

      .evict_req            ( coop_evict_req                                   ),
      .evict_valid          ( coop_evict_valid                                 ),
      .evict_ready          ( coop_evict_ready                                 ),

      .dbg_sm_signals       ( dbg_sm_signals[`DCACHE_L2_EVICT_IDX_TSTORE]      )
    );

  endmodule

