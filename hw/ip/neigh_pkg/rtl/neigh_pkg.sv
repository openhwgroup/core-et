// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

package neigh_pkg;

  import etlink_pkg::*;
  import minion_pkg::*;

  parameter int unsigned NumNeigh           = ShireCoopNeighMaskSize;
  parameter int unsigned NumOtherNeigh      = NumNeigh - 1;
  parameter int unsigned NumNeighWidth      = $clog2(NumNeigh);
  parameter int unsigned MinPerN            = ShireCoopMinMaskSize;
  parameter int unsigned IcachePerN         = 1;
  parameter int unsigned MinPerPtw          = 4;
  parameter int unsigned PtwPerN            = MinPerN / MinPerPtw;
  /* verilator lint_off UNUSEDPARAM */  // Neighborhood-shell sizing constants are consumed incrementally as the standalone shell comes in.
  parameter int unsigned NeighNumRespAgents = MinPerN + IcachePerN + PtwPerN + 1;

  /* verilator lint_off UNUSEDPARAM */  // These agent indices are shared neighborhood-shell constants.
  parameter int unsigned NeighAgentMinion   = 0;
  parameter int unsigned NeighAgentIcache   = MinPerN;
  parameter int unsigned NeighAgentPtw      = MinPerN + IcachePerN;
  parameter int unsigned NeighAgentTbox     = MinPerN + IcachePerN + PtwPerN;
  parameter int unsigned NeighFillFifoSize  = 4;
  parameter int unsigned NeighFlbElemsPerNeigh = 4;
  parameter int unsigned NeighMaxLocalConnect = 3;
  parameter int unsigned NeighMinTboxReqFifoSize = 2;
  parameter int unsigned TboxNeighReqFifoSize = 2;
  parameter int unsigned NeighScTboxRspFifoSize = 2;
  parameter int unsigned TstoreReqFifoDepth = 2;
  /* verilator lint_on UNUSEDPARAM */

  /* verilator lint_off UNUSEDPARAM */  // Cooperative TLoad constants are consumed only by the corresponding channel leaves.
  parameter int unsigned ShireCoopMissReqs        = 2;
  parameter int unsigned ShireCoopNumIds          = 32;
  parameter int unsigned ShireCoopRdyCredsWidth   = $clog2(3);
  parameter logic [ShireCoopRdyCredsWidth-1:0] ShireCoopRdyCreds = '1;
  /* verilator lint_off VARHIDDEN */  // Preserve original shared constant names while mirroring minion package values.
  parameter int unsigned ShireCoopSeqIdSize       = minion_pkg::ShireCoopSeqIdSize;
  parameter int unsigned ShireCoopIdSize          = minion_pkg::ShireCoopIdSize;
  /* verilator lint_on VARHIDDEN */
  parameter int unsigned ShireCoopMinMaskStart    = 0;
  parameter int unsigned ShireCoopSeqIdStart      = ShireCoopMinMaskSize;
  parameter int unsigned ShireCoopIdStart         = ShireCoopSeqIdStart + ShireCoopSeqIdSize;
  parameter int unsigned ShireCoopLoadIdStart     = ShireCoopSeqIdStart;
  parameter int unsigned ShireCoopLoadIdSize      = ShireCoopIdSize + ShireCoopSeqIdSize;
  parameter int unsigned ShireCoopNeighMaskStart  = ShireCoopLoadIdStart + ShireCoopLoadIdSize;
  parameter int unsigned ShireCoopDataSize        = ShireCoopNeighMaskSize +
                                                    ShireCoopLoadIdSize +
                                                    ShireCoopMinMaskSize;
  parameter int unsigned ShireCoopMstNeighAddrStart = 6;
  parameter int unsigned ShireCoopMstNeighAddrSize  = NumNeighWidth;
  parameter int unsigned ShireMaxCoop             = 8;
  parameter int unsigned ShireCoopMissIdSize      = $clog2(ShireMaxCoop);
  parameter int unsigned ShireCoopTagTableDepth   = 32;
  /* verilator lint_on UNUSEDPARAM */

  typedef struct packed {
    logic [NumNeighWidth-1:0]      master_neigh;
    logic [ShireCoopIdSize-1:0]    coop_id;
    logic [ShireCoopSeqIdSize-1:0] coop_seq_id;
    logic [MinPerN-1:0]            coop_min_mask;
  } coop_tload_slv_req_t;

  typedef struct packed {
    logic [NumNeighWidth-1:0]   dest_neigh;
    logic [ShireCoopIdSize-1:0] coop_id;
  } coop_tload_slv_rdy_req_t;

  typedef struct packed {
    logic [ShireCoopMinMaskSize-1:0]                         coop_min_mask;
    logic [ShireCoopMinMaskSize-1:0][ShireCoopMissIdSize-1:0] min_miss_id;
  } coop_tload_tag_table_data_t;

  typedef struct packed {
    logic [ShireCoopLoadIdSize-1:0] tag;
    coop_tload_tag_table_data_t     data;
  } coop_tload_tag_table_req_t;

  typedef struct packed {
    logic [MinPerN-1:0][ShireCoopMissReqs-1:0][NumOtherNeigh-1:0] mst_ext_is_rdy;
    logic [MinPerN-1:0][ShireCoopMissReqs-1:0]                    slv_has_rdy_creds;
    logic [MinPerN-1:0][ShireCoopMissReqs-1:0][NumNeighWidth-1:0] master_neigh;
    logic [MinPerN-1:0][ShireCoopMissReqs-1:0]                    coop_tload_local_rdy;
  } coop_tload_dbg_sm_t;

  typedef struct packed {
    neigh_source_e                        dest;
    logic                                 is_coop;
    logic [ShireCoopMinMaskSize-1:0]      coop_min_mask;
    logic [ShireCoopMinMaskSize-1:0][IdSize-1:0] coop_min_id;
    rsp_t                                 rsp;
  } fill_fifo_data_t;

  typedef enum logic [2:0] {
    PtwStateReady      = 3'b000,
    PtwStateReq        = 3'b001,
    PtwStateWait       = 3'b010,
    PtwStateSetDirty   = 3'b011,
    PtwStateWaitDirty  = 3'b100,
    PtwStateDone       = 3'b101
  } ptw_state_e;

  typedef struct packed {
    ptw_state_e                    state;
    logic [CsrSatpModeSz-1:0]      req_satp_mode;
    logic [1:0]                    req_prv;
    logic [VaSize-1:VaUntransSize] req_addr;
  } ptw_dbg_sm_t;

  typedef struct packed {
    logic [13:0] local_message_valid_out;
  } fln_dbg_sm_t;

  parameter int unsigned TstoreBlockDbgWidth = 22;

  typedef struct packed {
    logic [TstoreBlockDbgWidth-1:0] tstore_internal_dbg_signals;
  } tstore_block_dbg_sm_t;

  typedef struct packed {
    tstore_block_dbg_sm_t tstore_north_dbg_sm_signals;
    tstore_block_dbg_sm_t tstore_south_dbg_sm_signals;
  } tstore_dbg_sm_t;

endpackage : neigh_pkg
