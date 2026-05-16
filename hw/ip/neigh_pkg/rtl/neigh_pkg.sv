// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

package neigh_pkg;

  import etlink_pkg::*;
  import minion_pkg::*;

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
  /* verilator lint_on UNUSEDPARAM */

  parameter int unsigned ShireMaxCoop       = 8;
  parameter int unsigned ShireCoopMissIdSize = $clog2(ShireMaxCoop);

  typedef struct packed {
    logic [ShireCoopMinMaskSize-1:0]                         coop_min_mask;
    logic [ShireCoopMinMaskSize-1:0][ShireCoopMissIdSize-1:0] min_miss_id;
  } coop_tload_tag_table_data_t;

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

endpackage : neigh_pkg
