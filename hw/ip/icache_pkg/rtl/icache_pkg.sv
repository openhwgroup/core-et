// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSEDPARAM */  // Standalone-shell icache constants/types are adopted incrementally.
package icache_pkg;

  import icache_geom_pkg::*;
  import etlink_pkg::*;
  import minion_pkg::*;
  import minion_frontend_pkg::*;

  // ════════════════════════════════════════════════════════
  // ICache constants
  // ════════════════════════════════════════════════════════

  localparam int unsigned IcacheChunkAddrSize  = $clog2(IcacheBlockBits / FeFetchReadSize);
  localparam int unsigned IcacheNrReqs         = 2;
  localparam int unsigned IcacheTlbEntries     = 16;

  localparam int unsigned IcacheDataAddrSize   = IcacheSetAddrWidth + IcacheBlockAddrSize;
  localparam int unsigned IcachePaTagSize      = PaSize - IcacheDataAddrSize;
  localparam int unsigned IcacheMicroTagSize   = PaSize - IcacheBlockAddrSize;

  localparam int unsigned IcachePrefetchMaxLines = 64;
  localparam int unsigned IcachePrefetchLinesSize = $clog2(IcachePrefetchMaxLines);

  localparam int unsigned IcacheEccBlocks      = DataSize / 64;
  localparam int unsigned IcacheEccBlocksPerMem = IcacheEccBlocks / IcacheNrMemsPerRow;

  localparam int unsigned IcacheDataApbAddrWidth         = 13;
  localparam int unsigned IcacheNeighApbAddrWidth        = 10;
  localparam int unsigned IcacheMemsApbNeighWidth        = 3;
  localparam int unsigned IcacheMemsApbAddrWidth         =
      IcacheMemsApbNeighWidth + 3 + IcacheDataApbAddrWidth;
  localparam int unsigned IcacheDbgAddrRegionStart       = 9;
  localparam int unsigned IcacheDbgAddrRegionSize        =
      IcacheNeighApbAddrWidth - IcacheDbgAddrRegionStart;
  localparam logic [IcacheDbgAddrRegionSize-1:0] IcacheDbgTagAddrRegion = '0;
  localparam logic [IcacheDbgAddrRegionSize-1:0] IcacheDbgUcacheAddrRegion =
      {{(IcacheDbgAddrRegionSize - 1){1'b0}}, 1'b1};
  localparam int unsigned IcacheDbgTagAddrWidth          = $clog2(IcacheSets * IcacheWays / 2);
  localparam int unsigned IcacheDbgUcacheAddrStart       =
      IcacheDbgAddrRegionStart - IcacheDbgAddrRegionSize;
  localparam int unsigned IcacheDbgUcacheAddrSize        = 1;
  localparam int unsigned IcacheDbgUcacheBlockAddrStart  =
      IcacheDbgUcacheAddrStart - IcacheDbgUcacheAddrSize;
  localparam int unsigned IcacheDbgUcacheBlockAddrSize   = 1;
  localparam int unsigned IcacheDbgUcacheSubblockAddrStart =
      IcacheDbgUcacheBlockAddrStart - IcacheDbgUcacheBlockAddrSize - IcacheMicroCacheAdWidth;
  localparam int unsigned IcacheDbgUcacheSubblockAddrSize = 1;
  localparam logic [IcacheDbgUcacheBlockAddrSize-1:0] IcacheDbgUdataAddrRegion = '0;
  localparam logic [IcacheDbgUcacheBlockAddrSize-1:0] IcacheDbgUtagBlockAddrRegion =
      {{(IcacheDbgUcacheBlockAddrSize - 1){1'b0}}, 1'b1};
  localparam logic [IcacheDbgUcacheSubblockAddrSize-1:0] IcacheDbgUtagSubblockAddrRegion = '0;
  localparam logic [IcacheDbgUcacheBlockAddrSize-1:0] IcacheDbgErrBlockAddrRegion =
      {{(IcacheDbgUcacheBlockAddrSize - 1){1'b0}}, 1'b1};
  localparam logic [IcacheDbgUcacheSubblockAddrSize-1:0] IcacheDbgErrSubblockAddrRegion =
      {{(IcacheDbgUcacheSubblockAddrSize - 1){1'b0}}, 1'b1};
  localparam int unsigned IcacheDbgUtagAddrWidth         = IcacheMicroCacheAdWidth;
  localparam int unsigned IcacheDbgUdataAddrWidth        =
      $clog2(IcacheMicroCacheSize * IcacheBlockBits / BpamShireApbDataWidth);
  localparam int unsigned IcacheDbgUdataMemChunkAddrWidth =
      $clog2(IcacheBlockBits / BpamShireApbDataWidth);
  localparam int unsigned IcacheDbgUcacheAddrWidth       = IcacheDbgUdataAddrWidth + 1;
  localparam int unsigned IcacheDbgDataMemChunkAddrWidth =
      $clog2(IcacheMemUsedBits / 64);
  localparam int unsigned IcacheDbgDataBlockChunkAddrWidth =
      $clog2(IcacheBlockBits / 64) + 1;

  // ════════════════════════════════════════════════════════
  // ICache enums
  // ════════════════════════════════════════════════════════

  typedef enum logic [1:0] {
    IcachePrefetchStateIdle      = 2'b00,
    IcachePrefetchStateRequest   = 2'b01,
    IcachePrefetchStateCheckHit  = 2'b10,
    IcachePrefetchStateFillWait  = 2'b11
  } icache_prefetch_state_e;

  typedef enum logic [1:0] {
    IcacheMissStateReady     = 2'b00,
    IcacheMissStateRequest   = 2'b01,
    IcacheMissStateFillWait  = 2'b10
  } icache_miss_state_e;

  typedef enum logic [3:0] {
    IcacheErrCodeSbe       = 4'd0,
    IcacheErrCodeDbe       = 4'd1,
    IcacheErrCodeEccCntOv  = 4'd2
  } icache_err_code_e;

  typedef enum logic [2:0] {
    IcacheDataDbgStateIdle           = 3'b000,
    IcacheDataDbgStateWaitReadReady  = 3'b001,
    IcacheDataDbgStateRead           = 3'b010,
    IcacheDataDbgStateWaitWriteReady = 3'b011,
    IcacheDataDbgStateWrite          = 3'b100,
    IcacheDataDbgStateDone           = 3'b101
  } icache_data_dbg_state_e;

  // ════════════════════════════════════════════════════════
  // Frontend-facing aliases
  // ════════════════════════════════════════════════════════

  typedef fe_icache_req_t   frontend_icache_req_t;
  typedef icache_fe_resp_t  icache_frontend_resp_t;
  typedef fe_operation_t    frontend_operation_t;
  typedef fe_core_resp_t    frontend_core_resp_t;
  typedef fe_thread_data_t  frontend_thread_data_t;

  // ════════════════════════════════════════════════════════
  // ICache-local structs
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    logic                         valid;
    logic [IcacheSetAddrWidth-1:0] idx;
    logic [IcacheWayAddrWidth-1:0] way;
    logic [IcachePaTagSize-1:0]    data;
  } icache_tag_write_req_t;

  typedef struct packed {
    vm_status_t                  status;
    logic [VaTransSize-1:0]      vpn;
    logic                        passthrough;
    logic                        msb_err;
  } icache_tlb_req_t;

  typedef struct packed {
    logic                        fill_pending;
    logic                        miss;
    logic [PaTransSize-1:0]      ppn;
    logic                        xcpt_if;
    logic                        access_fault;
  } icache_tlb_resp_t;

  typedef struct packed {
    logic                        access_fault;
    logic                        cacheable;
  } icache_pma_resp_t;

  typedef struct packed {
    logic [1:0]                  prv;
    logic [VaSize-1:6]           start_addr;
    logic [5:0]                  num_lines;
  } icache_prefetch_conf_t;

  typedef struct packed {
    logic                        err_rsp_enable;
    logic [2:0]                  err_interrupt_enable;
  } esr_icache_err_log_ctl_t;

  typedef struct packed {
    logic [IcacheWayAddrWidth-1:0] way;
    logic [IcacheSetAddrWidth-1:0] set;
    logic [IcacheEccBlocks-1:0]    dbe_per_block;
    logic [IcacheEccBlocks-1:0]    sbe_per_block;
    logic [PaSize-1:IcacheBlockAddrSize] address;
  } icache_err_log_info_t;

  typedef struct packed {
    logic                         dbe_ov;
    logic [51:48]                 reserved_51_48;
    logic [IcacheEccBlocks-1:0]   err_bits;
    logic [39:17]                 reserved_39_17;
    logic [IcacheWayAddrWidth-1:0] way;
    logic [IcacheSetAddrWidth-1:0] set;
    icache_err_code_e             err_code;
    logic                         imprecise;
    logic                         enabled;
    logic                         multi;
    logic                         valid;
  } esr_icache_err_log_info_t;

  typedef struct packed {
    icache_prefetch_state_e                             ucache_f0_prefetch_state;
    icache_miss_state_e [IcacheNrReqs-1:0]             ucache_f0_miss_state;
    logic [IcacheNrReqs-1:0]                           ucache_f2_valid;
    logic [IcacheNrReqs-1:0][IcacheMicroCacheSize-1:0] ucache_f2_hit_array;
    logic [IcacheNrReqs-1:0][PaSize-1:6]               ucache_f2_paddr;
    logic [IcacheWays-1:0]                             f1_tag_hit;
    icache_miss_state_e                                f0_miss_state;
    logic                                              f0_l1_miss_req_valid;
    logic [PaSize-1:6]                                 f0_l1_miss_req_addr;
    logic [IcacheNrReqs-1:0]                           f1_req_valid;
    logic [IcacheNrReqs-1:0][VaSize-1:6]               f1_req_addr;
  } icache_dbg_sm_t;

  // ════════════════════════════════════════════════════════
  // ICache helper functions
  // ════════════════════════════════════════════════════════

  function automatic logic [IcacheWays-1:0] icache_lru_victim(
      input logic [IcacheLruSetSize-1:0] lru_in
  );
    logic [IcacheWays-1:0][IcacheWays-1:0] lru_in_tmp;

    lru_in_tmp = lru_in;

    for (int i = 0; i < IcacheWays; i++) begin
      icache_lru_victim[i] = 1'b1;
      for (int j = 0; j < i; j++) begin
        icache_lru_victim[i] &= lru_in_tmp[i][j];
      end
      for (int j = i + 1; j < IcacheWays; j++) begin
        icache_lru_victim[i] &= ~lru_in_tmp[j][i];
      end
    end
  endfunction

  function automatic logic [IcacheLruSetSize-1:0] icache_lru_update(
      input logic [IcacheLruSetSize-1:0] lru_in,
      input logic [IcacheWays-1:0]       way_en
  );
    logic [IcacheWays-1:0][IcacheWays-1:0] lru_out;

    lru_out = lru_in;
    for (int i = 0; i < IcacheWays; i++) begin
      if (way_en[i]) begin
        for (int j = 0; j < i; j++) begin
          lru_out[i][j] = 1'b0;
        end
        for (int j = i + 1; j < IcacheWays; j++) begin
          lru_out[j][i] = 1'b1;
        end
      end
    end
    return lru_out;
  endfunction

endpackage : icache_pkg
/* verilator lint_on UNUSEDPARAM */
