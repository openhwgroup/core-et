// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Minion dcache package.
//
// Minimal shared types/constants/functions needed by the first dcache tranche.
// This is a local subset of the original minion include chain. As more of the
// minion is ported, shared types can move into a common minion-level package.

package minion_dcache_pkg;
  import minion_pkg::CoreGsc32IdxVSize;
  import minion_pkg::CoreGscCntBits;
  import minion_pkg::DmemReqPsMaskSz;
  import minion_pkg::ShireCoopMinMaskSize;
  import minion_pkg::ShireCoopNeighMaskSize;
  import minion_pkg::ShireCoopIdSize;
  import minion_pkg::tensorcoop_control;
  import minion_pkg::VpuDataSz;
  import minion_pkg::minion_reg_dest_t;

  /* verilator lint_off UNUSED */  // Shared architectural constants for follow-on dcache leafs

  parameter int unsigned PaSize          = 40;
  parameter int unsigned VaSize          = 48;
  parameter int unsigned XregSize        = 64;
  parameter int unsigned VaUntransSize   = 12;
  parameter int unsigned PaUntransSize   = 12;
  parameter int unsigned DcacheLineSize  = 512;
  parameter int unsigned DcacheDataSize  = 256;
  parameter int unsigned DcacheDataBytes = DcacheDataSize / 8;
  parameter int unsigned DcachePsMaskSize = 8;
  parameter int unsigned DcacheSets       = 16;
  parameter int unsigned DcacheSetRed     = 2;
  parameter int unsigned DcacheWays       = 4;
  parameter int unsigned DcacheSetsScpSplit = 14;
  parameter int unsigned DcacheSetsForScp = 12;
  parameter int unsigned DcacheBufferSize = 8;
  parameter int unsigned DcacheLramDataSize = 64;
  parameter int unsigned DcacheLramNumBanks = 4;
  parameter int unsigned DcacheBankSelLsb = $clog2(DcacheLramDataSize / 8);
  parameter int unsigned DcacheBankSelWidth = $clog2(DcacheLramNumBanks);
  parameter int unsigned DcacheBlockSelBit = $clog2(DcacheDataSize / 8);
  parameter int unsigned DcacheBlockAddrLsb = DcacheBlockSelBit;
  parameter int unsigned DcacheBlockAddrWidth = $clog2(DcacheLineSize / DcacheDataSize);
  parameter int unsigned DcacheLruSetSize = DcacheWays * DcacheWays;
  parameter int unsigned DcacheTlbEntries  = 8;
  parameter int unsigned DcacheSetIdxWidth = $clog2(DcacheSets);
  parameter int unsigned DcacheWayIdxWidth = $clog2(DcacheWays);
  parameter int unsigned DcacheBufferIdxWidth = $clog2(DcacheBufferSize);
  parameter int unsigned DcacheLramAddrWidth = DcacheSetIdxWidth + 6;
  parameter int unsigned DcacheLramRows = DcacheSets * DcacheWays * 2;
  parameter int unsigned DcacheLramRowAddrWidth = $clog2(DcacheLramRows);
  parameter int unsigned DcacheLramRowsSplit = DcacheSetsScpSplit * DcacheWays * 2;
  parameter int unsigned PaTagExtSize     = PaSize - $clog2(DcacheSetRed) - 6;
  parameter int unsigned PaNoTagExtSize   = $clog2(DcacheSetRed) + 6;
  parameter int unsigned CoreL2BlockExtSize = 256;
  parameter int unsigned CoreL2BlockExtPasses = 512 / CoreL2BlockExtSize;
  parameter int unsigned CoreL2BlockExtAddrOffsetBits = $clog2(CoreL2BlockExtSize / 8);
  parameter int unsigned EtLinkSubopcodeSize = 7;
  parameter int unsigned EtLinkQwenSize = 4;
  parameter int unsigned EtAtomicOpcodeSize = 4;
  parameter int unsigned EtAtomicVectorSize = 1;
  parameter int unsigned EtAtomicSizeSize = 1;
  parameter int unsigned EtAtomicDestSize = 1;
  parameter int unsigned EtAtomicConfigSize =
      EtAtomicOpcodeSize + EtAtomicVectorSize + EtAtomicSizeSize + EtAtomicDestSize;
  parameter int unsigned DcacheDbgCsrTxsndSize = 2;
  parameter int unsigned DcacheDbgCsrTsSize = 4;
  parameter int unsigned DcacheDbgCsrMhSize = 4;
  parameter int unsigned DcacheDbgCsrCouSize = 4;
  parameter int unsigned DcacheDbgCsrCoul2Size = 3;
  parameter int unsigned DcacheDbgSmMatchBits = 64;
  parameter int unsigned DcacheDbgSmFilterBits = 200;
  parameter int unsigned DcacheDbgSmDataGroupBits = 128;
  parameter int unsigned DcacheDbgSmDataBits = 4 * DcacheDbgSmDataGroupBits;
  parameter int unsigned DcacheTensorCtrlDestBits = 6;
  parameter int unsigned DcacheCoL2CycleBits = 9;
  parameter int unsigned DcacheCoL2GrantBits = 2;
  parameter int unsigned DcacheReplayqSize = 8;
  parameter int unsigned DcacheReplayqAddrWidth = $clog2(DcacheReplayqSize);
  parameter int unsigned DcacheReplayqAgeMaskSize = DcacheReplayqSize * 2;
  parameter int unsigned DcacheReplayqAgeCmpWidth = $clog2(DcacheReplayqSize) + 1;
  parameter int unsigned DcacheReplayqAgeWidth = DcacheReplayqAgeCmpWidth + 1;
  parameter int unsigned DcacheReplayqAgeEraBit = DcacheReplayqAgeWidth - 1;
  parameter int unsigned DcacheReplayqAddrConflictLsb = $clog2(DcacheDataSize / 8);
  parameter int unsigned DcacheReplayqAddrConflictWidth = 10;
  parameter int unsigned DcacheMhFileSize = 2;
  parameter int unsigned DcacheVmSelWidth = 3;
  parameter logic [DcacheVmSelWidth-1:0] DcacheVmSelIp  = DcacheVmSelWidth'(0);
  parameter logic [DcacheVmSelWidth-1:0] DcacheVmSelCo  = DcacheVmSelWidth'(1);
  parameter logic [DcacheVmSelWidth-1:0] DcacheVmSelTl0 = DcacheVmSelWidth'(2);
  parameter logic [DcacheVmSelWidth-1:0] DcacheVmSelTl1 = DcacheVmSelWidth'(3);
  parameter logic [DcacheVmSelWidth-1:0] DcacheVmSelTs  = DcacheVmSelWidth'(4);
  parameter logic [DcacheVmSelWidth-1:0] DcacheVmSelL2  = DcacheVmSelWidth'(5);
  parameter int unsigned AdScpRegionStart = 31;
  parameter int unsigned AdScpRegionBits = PaSize - AdScpRegionStart;
  parameter logic [AdScpRegionBits-1:0] AdScpRegion = {{(AdScpRegionBits - 1){1'b0}}, 1'b1};

  typedef enum logic [1:0] {
    StateInvalid   = 2'b00,
    StateShared    = 2'b01,
    StateExclusive = 2'b10,
    StateModified  = 2'b11
  } dcache_line_state_e;

  typedef enum logic [1:0] {
    SnoopToExclusive = 2'b00,
    SnoopToShared    = 2'b01,
    SnoopToInvalid   = 2'b10
  } snoop_request_e;

  typedef enum logic [1:0] {
    DcacheAccessRd = 2'b00,
    DcacheAccessWi = 2'b01,
    DcacheAccessWr = 2'b11
  } dcache_access_type_e;

  typedef enum logic [3:0] {
    MhInvalid       = 4'b0000,
    MhAcquireWb     = 4'b0001,
    MhFillReq       = 4'b0010,
    MhFillResp      = 4'b0011,
    MhMetaWriteReq  = 4'b0100,
    MhMetaHazard    = 4'b0101,
    MhUcWaitIdle    = 4'b0110,
    MhUcLoadReq     = 4'b0111,
    MhUcLoadResp    = 4'b1000,
    MhUcStoreWait   = 4'b1001,
    MhUcStoreReq    = 4'b1010,
    MhUcStoreAck    = 4'b1011,
    MhFillClean     = 4'b1100
  } dcache_mh_state_e;

  typedef enum logic [2:0] {
    L2CacheOpInvalid,
    L2CacheOpTranslate,
    L2CacheOpTranslateWait,
    L2CacheOpRequest,
    L2CacheOpNextOperation,
    L2CacheOpWaitTmask
  } l2_cache_op_state_e;

  typedef enum logic [3:0] {
    CacheOpInvalid,
    CacheOpMetaRead,
    CacheOpMetaRespWait,
    CacheOpMetaResp,
    CacheOpWritebackReq,
    CacheOpWritebackResp,
    CacheOpMetaWrite,
    CacheOpMetaWriteLock,
    CacheOpWaitConflict,
    CacheOpNextOperation,
    CacheOpWaitTmask,
    CacheOpL1Prefetch,
    CacheOpNextOpOrInvalid,
    CacheOpWaitNext,
    CacheOpWaitTlb,
    CacheOpUnused
  } cache_op_state_e;

  typedef enum logic {
    CoConflictMh,
    CoConflictRq
  } cache_op_conflict_e;

  parameter logic [1:0] PrvU = 2'd0;
  parameter logic [1:0] PrvS = 2'd1;
  parameter logic [1:0] PrvM = 2'd3;

  parameter logic [1:0] MprotIoAccessModeU = 2'h0;
  parameter logic [1:0] MprotIoAccessModeS = 2'h1;
  parameter logic [1:0] MprotIoAccessModeF = 2'h2;
  parameter logic [1:0] MprotIoAccessModeM = 2'h3;

  parameter logic [1:0] MprotDramSize8G  = 2'h0;
  parameter logic [1:0] MprotDramSize16G = 2'h1;
  parameter logic [1:0] MprotDramSize24G = 2'h2;
  parameter logic [1:0] MprotDramSize32G = 2'h3;

  typedef struct packed {
    logic [1:0] prv;
    logic       mprv;
    logic [1:0] mpp;
    logic       sum;
    logic       mxr;
    logic       debug;
  } vm_status_t;

  typedef struct packed {
    logic       enable_secure_memory;
    logic [1:0] dram_size;
    logic       disable_osbox_access;
    logic       disable_pcie_access;
    logic [1:0] io_access_mode;
  } esr_mprot_t;

  typedef enum logic [4:0] {
    DcCmdXrd      = 5'b00000,
    DcCmdXwr      = 5'b00001,
    DcCmdPfr      = 5'b00010,
    DcCmdPfw      = 5'b00011,
    DcCmdXaSwap   = 5'b00100,
    DcCmdFlushAll = 5'b00101,
    DcCmdXlr      = 5'b00110,
    DcCmdXsc      = 5'b00111,
    DcCmdXaAdd    = 5'b01000,
    DcCmdXaXor    = 5'b01001,
    DcCmdXaOr     = 5'b01010,
    DcCmdXaAnd    = 5'b01011,
    DcCmdXaMin    = 5'b01100,
    DcCmdXaMax    = 5'b01101,
    DcCmdXaMinu   = 5'b01110,
    DcCmdXaMaxu   = 5'b01111,
    DcCmdFlush    = 5'b10000,
    DcCmdProduce  = 5'b10001,
    DcCmdClean    = 5'b10011,
    DcCmdXaCmpswp = 5'b11000,
    DcCmdG32      = 5'b11001,
    DcCmdSc32     = 5'b11010,
    DcCmdXwrb     = 5'b11011,
    DcCmdXrdb     = 5'b11100,
    DcCmdXaMinPs  = 5'b11101,
    DcCmdXaMaxPs  = 5'b11111
  } dcache_cmd_e;

  typedef enum logic [4:0] {
    EtLinkReqWrite       = 5'd0,
    EtLinkReqRead        = 5'd1,
    EtLinkReqReadCoop    = 5'd3,
    EtLinkReqMsgSendData = 5'd4,
    EtLinkReqAtomic      = 5'd5,
    EtLinkReqFlush       = 5'd8,
    EtLinkReqEvict       = 5'd10,
    EtLinkReqScpFill     = 5'd14,
    EtLinkReqPrefetch    = 5'd15
  } et_link_req_opcode_e;

  typedef enum logic [2:0] {
    EtLinkByte        = 3'b000,
    EtLinkHWord       = 3'b001,
    EtLinkWord        = 3'b010,
    EtLinkDWord       = 3'b011,
    EtLinkQWord       = 3'b100,
    EtLinkHLine       = 3'b101,
    EtLinkLine        = 3'b110,
    EtLinkIllegalSize = 3'b111
  } et_link_size_e;

  typedef enum logic [4:0] {
    EtLinkCoreMissIdReserved         = 5'b00000,
    EtLinkCoreMissIdTensorStoreCoop  = 5'b00001,
    EtLinkCoreMissIdMissHandler0     = 5'b00010,
    EtLinkCoreMissIdMissHandler1     = 5'b00011,
    EtLinkCoreMissIdTbox             = 5'b00100,
    EtLinkCoreMissIdReduce           = 5'b00101,
    EtLinkCoreMissIdCacheOps         = 5'b00110,
    EtLinkCoreMissIdWriteBack        = 5'b00111,
    EtLinkCoreMissIdMissHandlerUcLd0 = 5'b01000,
    EtLinkCoreMissIdMissHandlerUcLd1 = 5'b01001,
    EtLinkCoreMissIdMissHandlerUcSt0 = 5'b01010,
    EtLinkCoreMissIdMissHandlerUcSt1 = 5'b01011,
    EtLinkCoreMissIdL2Prefetch0      = 5'b01100,
    EtLinkCoreMissIdL2Prefetch1      = 5'b01101,
    EtLinkCoreMissIdL2Prefetch2      = 5'b01110,
    EtLinkCoreMissIdL2Prefetch3      = 5'b01111,
    EtLinkCoreMissIdTensorLoadExt0   = 5'b10000,
    EtLinkCoreMissIdTensorLoadExt1   = 5'b10001,
    EtLinkCoreMissIdTensorLoadExt2   = 5'b10010,
    EtLinkCoreMissIdTensorLoadExt3   = 5'b10011,
    EtLinkCoreMissIdTensorLoadExt4   = 5'b10100,
    EtLinkCoreMissIdTensorLoadExt5   = 5'b10101,
    EtLinkCoreMissIdTensorLoadExt6   = 5'b10110,
    EtLinkCoreMissIdTensorLoadExt7   = 5'b10111
  } et_link_core_miss_id_e;

  typedef enum logic [3:0] {
    EtLinkAmoSwap    = 4'b0000,
    EtLinkAmoAdd     = 4'b0001,
    EtLinkAmoXor     = 4'b0010,
    EtLinkAmoAnd     = 4'b0011,
    EtLinkAmoOr      = 4'b0100,
    EtLinkAmoMin     = 4'b0101,
    EtLinkAmoMax     = 4'b0110,
    EtLinkAmoMinU    = 4'b0111,
    EtLinkAmoMaxU    = 4'b1000,
    EtLinkAmoMinPs   = 4'b1001,
    EtLinkAmoMaxPs   = 4'b1010,
    EtLinkAmoCmpSwap = 4'b1011
  } et_link_amo_opcode_e;

  typedef enum logic [3:0] {
    DcTypeB      = 4'b0000,
    DcTypeH      = 4'b0001,
    DcTypeW      = 4'b0010,
    DcTypeD      = 4'b0011,
    DcTypeBu     = 4'b0100,
    DcTypeHu     = 4'b0101,
    DcTypeWu     = 4'b0110,
    DcTypePsBr   = 4'b1010,
    DcTypePs     = 4'b1011,
    DcTypePsGs8  = 4'b1100,
    DcTypePsGs16 = 4'b1101,
    DcTypePsGs32 = 4'b1110
  } dcache_type_e;

  typedef enum logic [1:0] {
    DcacheModeShared     = 2'b00,
    DcacheModeSplit      = 2'b01,
    DcacheModeInvalid    = 2'b10,
    DcacheModeScratchpad = 2'b11
  } dcache_mode_t;

  typedef struct packed {
    dcache_line_state_e       coh_state;
    logic [PaTagExtSize-1:0]  tag;
  } dcache_meta_data_t;

  typedef struct packed {
    logic [PaTagExtSize-1:0] tag;
    logic [DcacheSetIdxWidth-1:0] set;
    logic [DcacheWays-1:0]   way_en;
    logic                    is_tbox;
    logic                    mem_idx;
    logic [PaNoTagExtSize-1:0] addr_low;
  } dcache_wb_req_t;

  typedef struct packed {
    logic                            valid_l;
    logic                            valid_h;
    logic [DcacheWayIdxWidth-1:0]    way;
    logic [DcacheLramAddrWidth-1:0]  addr;
  } dcache_da_read_req_t;

  typedef logic [DcacheLramNumBanks-1:0][DcacheLramDataSize-1:0] dcache_da_data_t;

  typedef struct packed {
    logic                            valid_l;
    logic                            valid_h;
    logic [DcacheWayIdxWidth-1:0]    way;
    logic [DcacheLramAddrWidth-1:0]  addr;
    logic [DcacheLramDataSize-1:0]   data;
  } dcache_da_write_req_t;

  typedef struct packed {
    logic [DcacheSetIdxWidth-1:0] set_idx;
    logic [DcacheWays-1:0]        way_en;
    dcache_meta_data_t            data;
  } dcache_meta_write_req_t;

  typedef struct packed {
    logic                         valid;
    logic [DcacheWays-1:0]        way_en;
    logic [DcacheSetIdxWidth-1:0] set;
  } dcache_miss_info_t;

  typedef struct packed {
    logic [PaSize-1:0] paddr;
    dcache_type_e      typ;
    logic              amo;
    logic              write;
    logic              mem_global;
    logic              ts_tl_co;
  } dcache_pma_req_t;

  typedef struct packed {
    logic              thread_id;
    minion_pkg::cache_op_cmd_e cmd;
    logic [1:0]        start_level;
    logic [1:0]        dest_level;
    logic [VaSize-1:6] stride;
    logic              use_tmask;
    logic [3:0]        count;
    logic              is_byp;
    logic              is_l2_scp;
  } l2_cache_op_req_t;

  typedef struct packed {
    minion_pkg::cache_op_cmd_e  cmd;
    snoop_request_e             snoop_type;
    logic                       thread_id;
    logic                       use_tmask;
    logic [3:0]                 count;
    logic [1:0]                 start_level;
    logic [1:0]                 dest_level;
    logic                       prefetch;
    logic                       send_to_l2;
  } cache_op_req_t;

  typedef struct packed {
    logic access_fault;
    logic cacheable;
  } dcache_pma_resp_t;

  typedef struct packed {
    vm_status_t                    status;
    logic [VaSize-1:VaUntransSize] vpn;
    logic                          passthrough;
    logic                          store;
    logic                          msb_err;
  } dcache_tlb_req_t;

  typedef struct packed {
    logic                          fill_pending;
    logic                          miss;
    logic [PaSize-1:PaUntransSize] ppn;
    logic                          xcpt_ld;
    logic                          xcpt_st;
    logic                          access_fault;
  } dcache_tlb_resp_t;

  typedef struct packed {
    et_link_core_miss_id_e         id;
    logic                          source;
    logic                          wdata;
    et_link_req_opcode_e           opcode;
    logic [PaSize-1:0]             address;
    logic [CoreL2BlockExtSize-1:0] data;
    et_link_size_e                 size;
    logic [EtLinkQwenSize-1:0]     qwen;
    logic [EtLinkSubopcodeSize-1:0] subopcode;
  } et_link_minion_evict_req_info_t;

  typedef struct packed {
    logic [4:0]                    id;
    logic                          source;
    logic                          wdata;
    et_link_req_opcode_e           opcode;
    logic [PaSize-1:0]             address;
    logic [$bits(tensorcoop_control)-1:0] data;
    et_link_size_e                 size;
    logic [EtLinkQwenSize-1:0]     qwen;
    logic [EtLinkSubopcodeSize-1:0] subopcode;
  } et_link_minion_miss_req_info_t;

  typedef struct packed {
    logic                            vpu_rf_write;
    logic                            miss_alloc;
    logic [DcacheMhFileSize-1:0]     miss_accepted;
    logic [DcacheMhFileSize-1:0]     miss_assigned;
  } dcache_replay_conf_t;

  typedef struct packed {
    logic [PaSize-1:0]               addr;
    logic [DcacheSetIdxWidth-1:0]    set;
    dcache_cmd_e                     cmd;
    dcache_type_e                    typ;
    logic [DcacheBufferIdxWidth-1:0] buf_id;
    logic [DcacheWays-1:0]           way_en;
    logic                            tag_match;
    dcache_meta_data_t               old_meta;
    logic                            cacheable;
    logic                            thread_id;
    logic                            mem_g;
    logic                            is_amo;
    logic                            is_msg;
    logic                            is_bypass;
`ifdef DCACHE_REPORT_PC
    logic [47:0]                     pc;
`endif
  } dcache_mh_req_t;

  typedef struct packed {
    logic [VaSize:0]                 addr;
    logic                            addr_msb_err;
    logic [DcacheSetIdxWidth-1:0]    set;
    logic                            phys;
    logic                            misaligned;
    logic                            misaligned_2nd;
    minion_reg_dest_t                dest;
    dcache_cmd_e                     cmd;
    dcache_type_e                    typ;
    logic [CoreGscCntBits-1:0]       gsc_cnt;
    logic [DmemReqPsMaskSz-1:0]      ps_mask;
    logic [VpuDataSz-1:0]            data;
    logic                            replay;
    logic [DcacheReplayqAddrWidth-1:0] replay_entry;
    logic [DcacheReplayqAgeWidth-1:0] replay_age;
    logic                            cache_op;
    logic                            buf_alloc;
    logic [DcacheBufferIdxWidth-1:0] buf_id;
    logic                            cacheable;
    logic [(DcacheDataSize / 64)-1:0] chunk_read;
    logic                            uc_load_2nd;
    logic                            uc_load_ok;
    logic                            miss_fill_ok;
    logic [CoreGsc32IdxVSize-1:0]    gsc32_idx;
    logic [DcacheVmSelWidth-1:0]     vm_sel;
    logic                            mem_global;
  } dcache_req_t;

  typedef struct packed {
    logic [VaSize:0]                 addr;
    minion_reg_dest_t                dest;
    dcache_cmd_e                     cmd;
    dcache_type_e                    typ;
    logic [CoreGscCntBits-1:0]       gsc_cnt;
    logic [DmemReqPsMaskSz-1:0]      ps_mask;
    logic                            phys;
    logic                            buf_alloc;
    logic [DcacheBufferIdxWidth-1:0] buf_id;
    logic                            cache_op;
    logic                            misaligned;
    logic                            misaligned_2nd;
    logic                            cacheable;
    logic                            uc_load_2nd;
    logic [CoreGsc32IdxVSize-1:0]    gsc32_idx;
    logic [DcacheVmSelWidth-1:0]     vm_sel;
    logic                            mem_global;
  } dcache_replay_req_t;

  typedef struct packed {
    minion_reg_dest_t                dest;
    dcache_cmd_e                     cmd;
    dcache_type_e                    typ;
    logic [CoreGscCntBits-1:0]       gsc_cnt;
    logic [DmemReqPsMaskSz-1:0]      ps_mask;
    logic                            buf_alloc;
    logic [DcacheBufferIdxWidth-1:0] buf_id;
    logic                            cache_op;
    logic [CoreGsc32IdxVSize-1:0]    gsc32_idx;
    logic [DcacheVmSelWidth-1:0]     vm_sel;
    logic                            mem_global;
  } dcache_replay_req_push_t;

  typedef struct packed {
    logic [VaSize:0]                 addr;
    logic                            phys;
    logic                            misaligned;
    logic                            misaligned_2nd;
    logic                            cacheable;
    logic                            uc_load_2nd;
  } dcache_replay_req_rearm_t;

  function automatic logic dcache_type_is_byte(dcache_type_e typ);
    return (typ == DcTypeB) || (typ == DcTypeBu) || (typ == DcTypePsGs8);
  endfunction

  function automatic logic dcache_cmd_is_amo(dcache_cmd_e cmd);
    return (cmd == DcCmdXaSwap) || (cmd == DcCmdXaAdd) || (cmd == DcCmdXaXor)
        || (cmd == DcCmdXaOr) || (cmd == DcCmdXaAnd) || (cmd == DcCmdXaMin)
        || (cmd == DcCmdXaMax) || (cmd == DcCmdXaMinu) || (cmd == DcCmdXaMaxu)
        || (cmd == DcCmdXaCmpswp) || (cmd == DcCmdXaMinPs) || (cmd == DcCmdXaMaxPs);
  endfunction

  function automatic logic dcache_cmd_is_prefetch(dcache_cmd_e cmd);
    return (cmd == DcCmdPfr) || (cmd == DcCmdPfw);
  endfunction

  function automatic logic dcache_cmd_is_read(dcache_cmd_e cmd);
    return (cmd == DcCmdXrd) || (cmd == DcCmdXlr) || (cmd == DcCmdXsc)
        || dcache_cmd_is_amo(cmd) || (cmd == DcCmdG32) || (cmd == DcCmdXrdb);
  endfunction

  function automatic logic dcache_cmd_is_write(dcache_cmd_e cmd);
    return (cmd == DcCmdXwr) || (cmd == DcCmdXsc) || dcache_cmd_is_amo(cmd)
        || (cmd == DcCmdSc32) || (cmd == DcCmdXwrb);
  endfunction

  function automatic logic dcache_cmd_is_bypass(dcache_cmd_e cmd);
    return (cmd == DcCmdXwrb) || (cmd == DcCmdXrdb);
  endfunction

  function automatic logic dcache_cmd_is_write_intent(dcache_cmd_e cmd);
    return dcache_cmd_is_write(cmd) || (cmd == DcCmdPfw) || (cmd == DcCmdXlr);
  endfunction

  function automatic dcache_access_type_e dcache_cmd_to_type(dcache_cmd_e cmd);
    logic [1:0] temp;

    temp = {dcache_cmd_is_write(cmd), dcache_cmd_is_write_intent(cmd)};
    dcache_cmd_to_type = dcache_access_type_e'(temp);
  endfunction

  function automatic logic [DcacheWayIdxWidth-1:0] dcache_way_encode(logic [DcacheWays-1:0] ways);
    dcache_way_encode = '0;
    for (int unsigned i = 0; i < DcacheWays; i++) begin
      if (ways[i]) begin
        dcache_way_encode |= i[DcacheWayIdxWidth-1:0];
      end
    end
  endfunction

  function automatic logic dcache_type_is_hword(dcache_type_e typ);
    return (typ == DcTypeH) || (typ == DcTypeHu) || (typ == DcTypePsGs16);
  endfunction

  function automatic logic dcache_type_is_word(dcache_type_e typ);
    return (typ == DcTypeW) || (typ == DcTypeWu) || (typ == DcTypePsBr) || (typ == DcTypePsGs32);
  endfunction

  function automatic logic dcache_type_is_dword(dcache_type_e typ);
    return typ == DcTypeD;
  endfunction

  function automatic logic dcache_type_is_gsc(dcache_type_e typ);
    return (typ == DcTypePsGs8) || (typ == DcTypePsGs16) || (typ == DcTypePsGs32);
  endfunction

  function automatic logic [DcacheWays-1:0] dcache_way_decode(logic [DcacheWayIdxWidth-1:0] way);
    dcache_way_decode = '0;
    dcache_way_decode[way] = 1'b1;
  endfunction

  function automatic logic [DcacheSetIdxWidth-1:0] dcache_set_correct(
      logic [DcacheSetIdxWidth-1:0] set,
      dcache_mode_t dcache_mode,
      logic thread_id
  );
    if (dcache_mode == DcacheModeSplit) begin
      if (thread_id == 1'b0) begin
        dcache_set_correct = {1'b0, set[DcacheSetIdxWidth-2:0]};
      end else begin
        dcache_set_correct = {{(DcacheSetIdxWidth-1){1'b1}}, set[0]};
      end
    end else if (dcache_mode == DcacheModeScratchpad) begin
      if (thread_id == 1'b0) begin
        dcache_set_correct = {{(DcacheSetIdxWidth-2){1'b1}}, 1'b0, set[0]};
      end else begin
        dcache_set_correct = {{(DcacheSetIdxWidth-1){1'b1}}, set[0]};
      end
    end else begin
      dcache_set_correct = set;
    end
  endfunction

  function automatic logic [EtLinkQwenSize-1:0] dcache_typ2qwen(
      dcache_type_e typ,
      logic [1:0] qwaddr
  );
    if (typ == DcTypePs) begin
      dcache_typ2qwen = 4'b0011;
    end else begin
      dcache_typ2qwen = 4'b0001;
    end

    dcache_typ2qwen = dcache_typ2qwen << qwaddr;
  endfunction

  function automatic et_link_size_e dcache_uc_size(dcache_type_e typ);
    if ((typ == DcTypeB) || (typ == DcTypeBu) || (typ == DcTypePsGs8)) begin
      dcache_uc_size = EtLinkByte;
    end else if ((typ == DcTypeH) || (typ == DcTypeHu) || (typ == DcTypePsGs16)) begin
      dcache_uc_size = EtLinkHWord;
    end else if ((typ == DcTypeW) || (typ == DcTypeWu) || (typ == DcTypePsBr)
        || (typ == DcTypePsGs32)) begin
      dcache_uc_size = EtLinkWord;
    end else if (typ == DcTypeD) begin
      dcache_uc_size = EtLinkDWord;
    end else if (typ == DcTypePs) begin
      dcache_uc_size = EtLinkHLine;
    end else begin
      dcache_uc_size = EtLinkWord;
    end
  endfunction

  function automatic logic [EtAtomicConfigSize-1:0] dcache_get_et_link_amo_config(
      dcache_cmd_e cmd,
      dcache_type_e typ,
      logic mem_global
  );
    et_link_amo_opcode_e opcode;
    logic vector;
    logic size;

    if      (cmd == DcCmdXaSwap)   opcode = EtLinkAmoSwap;
    else if (cmd == DcCmdXaAdd)    opcode = EtLinkAmoAdd;
    else if (cmd == DcCmdXaXor)    opcode = EtLinkAmoXor;
    else if (cmd == DcCmdXaAnd)    opcode = EtLinkAmoAnd;
    else if (cmd == DcCmdXaOr)     opcode = EtLinkAmoOr;
    else if (cmd == DcCmdXaMin)    opcode = EtLinkAmoMin;
    else if (cmd == DcCmdXaMax)    opcode = EtLinkAmoMax;
    else if (cmd == DcCmdXaMinu)   opcode = EtLinkAmoMinU;
    else if (cmd == DcCmdXaMaxu)   opcode = EtLinkAmoMaxU;
    else if (cmd == DcCmdXaMinPs)  opcode = EtLinkAmoMinPs;
    else if (cmd == DcCmdXaMaxPs)  opcode = EtLinkAmoMaxPs;
    else if (cmd == DcCmdXaCmpswp) opcode = EtLinkAmoCmpSwap;
    else                           opcode = EtLinkAmoSwap;

    if (typ == DcTypeD) begin
      size = 1'b1;
    end else begin
      size = 1'b0;
    end

    vector = 1'b0;

    dcache_get_et_link_amo_config = {mem_global, size, vector, opcode};
  endfunction

  function automatic logic dcache_type_is_ps(dcache_type_e typ);
    return (typ == DcTypePs) || (typ == DcTypePsBr) || (typ == DcTypePsGs8)
        || (typ == DcTypePsGs16) || (typ == DcTypePsGs32);
  endfunction

  function automatic logic dcache_address_is_size_aligned(
      dcache_type_e typ,
      logic [4:0] addr
  );
    return dcache_type_is_byte(typ)
        || (dcache_type_is_hword(typ) && (addr[0] == 1'b0))
        || (dcache_type_is_word(typ) && (addr[1:0] == 2'b0))
        || (dcache_type_is_dword(typ) && (addr[2:0] == 3'b0))
        || ((typ == DcTypePs) && (addr[4:0] == 5'b0));
  endfunction

  function automatic logic dcache_access_is_page_split(
      logic [VaSize:0] addr,
      minion_pkg::tlb_entry_type vmspagesize,
      logic misaligned_2_cachelines
  );
    if (misaligned_2_cachelines) begin
      if (vmspagesize == minion_pkg::tlb_entry_type_4K) begin
        dcache_access_is_page_split = &addr[11:6];
      end else if (vmspagesize == minion_pkg::tlb_entry_type_2M) begin
        dcache_access_is_page_split = &addr[20:6];
      end else if (vmspagesize == minion_pkg::tlb_entry_type_1G) begin
        dcache_access_is_page_split = &addr[29:6];
      end else begin
        dcache_access_is_page_split = 1'b0;
      end
    end else begin
      dcache_access_is_page_split = 1'b0;
    end
  endfunction

  function automatic logic [DcacheWays-1:0] dcache_lru_victim(
      logic [DcacheLruSetSize-1:0] lru_in,
      logic [DcacheWays-1:0] way_dis,
      logic [DcacheWays-1:0] way_prio
  );
    logic [DcacheWays-1:0][DcacheWays-1:0] lru_in_tmp0;
    logic [DcacheWays-1:0][DcacheWays-1:0] lru_in_tmp1;
    logic [DcacheWays-1:0] dcache_lru_victim0;
    logic [DcacheWays-1:0] dcache_lru_victim1;

    lru_in_tmp0 = lru_in;
    lru_in_tmp1 = lru_in;
    for (int unsigned i = 0; i < DcacheWays; i++) begin
      if (way_dis[i] || way_prio[i]) begin
        for (int unsigned j = 0; j < i; j++) begin
          lru_in_tmp0[i][j] = 1'b0;
        end
        for (int unsigned j = i + 1; j < DcacheWays; j++) begin
          lru_in_tmp0[j][i] = 1'b1;
        end
      end

      if (way_dis[i]) begin
        for (int unsigned j = 0; j < i; j++) begin
          lru_in_tmp1[i][j] = 1'b0;
        end
        for (int unsigned j = i + 1; j < DcacheWays; j++) begin
          lru_in_tmp1[j][i] = 1'b1;
        end
      end
    end

    for (int unsigned i = 0; i < DcacheWays; i++) begin
      dcache_lru_victim0[i] = 1'b1;
      dcache_lru_victim1[i] = 1'b1;
      for (int unsigned j = 0; j < i; j++) begin
        dcache_lru_victim0[i] = dcache_lru_victim0[i] && lru_in_tmp0[i][j];
        dcache_lru_victim1[i] = dcache_lru_victim1[i] && lru_in_tmp1[i][j];
      end
      for (int unsigned j = i + 1; j < DcacheWays; j++) begin
        dcache_lru_victim0[i] = dcache_lru_victim0[i] && ~lru_in_tmp0[j][i];
        dcache_lru_victim1[i] = dcache_lru_victim1[i] && ~lru_in_tmp1[j][i];
      end
    end

    if (|(dcache_lru_victim0 & ~way_dis & ~way_prio)) begin
      dcache_lru_victim = dcache_lru_victim0;
    end else begin
      dcache_lru_victim = dcache_lru_victim1;
    end
  endfunction

  function automatic logic [DcacheLruSetSize-1:0] dcache_lru_update(
      logic [DcacheLruSetSize-1:0] lru_in,
      logic [DcacheWays-1:0] way_en
  );
    logic [DcacheWays-1:0][DcacheWays-1:0] lru_out;

    lru_out = lru_in;
    for (int unsigned i = 0; i < DcacheWays; i++) begin
      if (way_en[i]) begin
        for (int unsigned j = 0; j < i; j++) begin
          lru_out[i][j] = 1'b0;
        end
        for (int unsigned j = i + 1; j < DcacheWays; j++) begin
          lru_out[j][i] = 1'b1;
        end
      end
    end
    dcache_lru_update = lru_out;
  endfunction

  function automatic logic paddr_is_io_space(logic [PaSize-1:0] paddr);
    return paddr[PaSize-1:30] == 10'h000;
  endfunction

  function automatic logic paddr_is_sp_space(logic [PaSize-1:0] paddr);
    return paddr[PaSize-1:30] == 10'h001;
  endfunction

  function automatic logic paddr_is_sp_rom(logic [PaSize-1:0] paddr);
    return paddr_is_sp_space(paddr) && (paddr[29:17] == 13'h0000);
  endfunction

  function automatic logic paddr_is_sp_sram(logic [PaSize-1:0] paddr);
    return paddr_is_sp_space(paddr) && (paddr[29:20] == 10'h004);
  endfunction

  function automatic logic paddr_is_sp_sram_code(logic [PaSize-1:0] paddr);
    return paddr_is_sp_space(paddr) && (paddr[29:19] == 11'h008);
  endfunction

  function automatic logic paddr_is_sp_sram_data(logic [PaSize-1:0] paddr);
    return paddr_is_sp_space(paddr) && (paddr[29:19] == 11'h009);
  endfunction

  function automatic logic paddr_is_sp_cacheable(logic [PaSize-1:0] paddr);
    return paddr_is_sp_rom(paddr) || paddr_is_sp_sram(paddr);
  endfunction

  function automatic logic paddr_is_scratchpad(logic [PaSize-1:0] paddr);
    return paddr[PaSize-1:31] == 9'h001;
  endfunction

  function automatic logic paddr_is_esr_space(logic [PaSize-1:0] paddr);
    return paddr[PaSize-1:32] == 8'h01;
  endfunction

  function automatic logic paddr_is_reserved(logic [PaSize-1:0] paddr);
    return (paddr[PaSize-1:38] == 2'b00) && (paddr[37:33] != 5'h00);
  endfunction

  function automatic logic paddr_is_pcie_space(logic [PaSize-1:0] paddr);
    return paddr[PaSize-1:38] == 2'b01;
  endfunction

  function automatic logic paddr_is_dram(logic [PaSize-1:0] paddr);
    return paddr[PaSize-1];
  endfunction

  function automatic logic paddr_is_dram_cacheable(logic [PaSize-1:0] paddr);
    return paddr_is_dram(paddr) && (paddr[38] == 1'b0);
  endfunction

  function automatic logic paddr_is_dram_uncacheable(logic [PaSize-1:0] paddr);
    return paddr_is_dram(paddr) && (paddr[38] == 1'b1);
  endfunction

  function automatic logic paddr_is_dram_mbox(logic [PaSize-1:0] paddr);
    return paddr_is_dram(paddr) && (paddr[37:23] == 15'h0000);
  endfunction

  function automatic logic paddr_is_dram_mcode(logic [PaSize-1:0] paddr);
    return paddr_is_dram_mbox(paddr) && (paddr[22:21] == 2'h0);
  endfunction

  function automatic logic paddr_is_dram_mdata(logic [PaSize-1:0] paddr);
    return paddr_is_dram_mbox(paddr) && (paddr[22:21] != 2'h0);
  endfunction

  function automatic logic paddr_is_dram_sbox(logic [PaSize-1:0] paddr);
    return paddr_is_dram(paddr) && (paddr[37:26] == 12'h000) && (paddr[25:23] != 3'h0);
  endfunction

  function automatic logic paddr_is_dram_scode(logic [PaSize-1:0] paddr);
    return paddr_is_dram(paddr) && (paddr[37:26] == 12'h000) && (paddr[25:23] == 3'h1);
  endfunction

  function automatic logic paddr_is_dram_sdata(logic [PaSize-1:0] paddr);
    return paddr_is_dram(paddr) && (paddr[37:26] == 12'h000) && (paddr[25:24] != 2'h0);
  endfunction

  function automatic logic paddr_is_dram_osbox(logic [PaSize-1:0] paddr);
    return paddr_is_dram(paddr) && (paddr[37:32] == 6'h00) && (paddr[31:26] != 6'h00);
  endfunction

  function automatic logic paddr_is_dram_over_8g(logic [PaSize-1:0] paddr);
    return paddr_is_dram(paddr) && (paddr[37:33] != 5'h00);
  endfunction

  function automatic logic paddr_is_dram_over_16g(logic [PaSize-1:0] paddr);
    return paddr_is_dram(paddr) && (paddr[37:34] != 4'h0);
  endfunction

  function automatic logic paddr_is_dram_over_24g(logic [PaSize-1:0] paddr);
    return paddr_is_dram(paddr) && ((paddr[37:33] == 5'h03) || (paddr[37:35] != 3'h0));
  endfunction

  function automatic logic paddr_is_dram_over_32g(logic [PaSize-1:0] paddr);
    return paddr_is_dram(paddr) && (paddr[37:35] != 3'h0);
  endfunction

  function automatic logic paddr_is_dram_other(logic [PaSize-1:0] paddr, logic [1:0] dram_size);
    return paddr_is_dram(paddr) && (paddr[37:32] != 6'h00)
        && (((dram_size == MprotDramSize8G)  && !paddr_is_dram_over_8g(paddr))
         || ((dram_size == MprotDramSize16G) && !paddr_is_dram_over_16g(paddr))
         || ((dram_size == MprotDramSize24G) && !paddr_is_dram_over_24g(paddr))
         || ((dram_size == MprotDramSize32G) && !paddr_is_dram_over_32g(paddr)));
  endfunction

  function automatic logic paddr_is_dram_reserved(logic [PaSize-1:0] paddr, logic [1:0] dram_size);
    return paddr_is_dram(paddr) && (paddr[37:32] != 6'h00)
        && (((dram_size == MprotDramSize8G)  &&  paddr_is_dram_over_8g(paddr))
         || ((dram_size == MprotDramSize16G) &&  paddr_is_dram_over_16g(paddr))
         || ((dram_size == MprotDramSize24G) &&  paddr_is_dram_over_24g(paddr))
         || ((dram_size == MprotDramSize32G) &&  paddr_is_dram_over_32g(paddr)));
  endfunction

  function automatic logic dcache_replayq_entry_is_older(
      logic [DcacheReplayqAgeWidth-1:0] entry1,
      logic [DcacheReplayqAgeWidth-1:0] entry2
  );
    logic cmp;

    cmp = (entry1[DcacheReplayqAgeCmpWidth-1:0] < entry2[DcacheReplayqAgeCmpWidth-1:0]);
    if (entry1[DcacheReplayqAgeEraBit] == entry2[DcacheReplayqAgeEraBit]) begin
      return cmp;
    end
    return !cmp;
  endfunction

  function automatic logic [DcacheSetIdxWidth-1:0] get_set_from_scp_dest(
      logic [DcacheTensorCtrlDestBits-1:0] dest
  );
    logic [DcacheTensorCtrlDestBits-1:0] dest_wrap;

    if (dest < 6'd12) begin
      dest_wrap = dest;
    end else if (dest < 6'd24) begin
      dest_wrap = dest - 6'd12;
    end else if (dest < 6'd36) begin
      dest_wrap = dest - 6'd24;
    end else if (dest < 6'd48) begin
      dest_wrap = dest - 6'd36;
    end else if (dest < 6'd60) begin
      dest_wrap = dest - 6'd48;
    end else begin
      dest_wrap = dest - 6'd60;
    end

    return dest_wrap[DcacheSetIdxWidth-1:0];
  endfunction

  function automatic logic [DcacheWayIdxWidth-1:0] get_way_from_scp_dest(
      logic [DcacheTensorCtrlDestBits-1:0] dest
  );
    if (dest < 6'd12) begin
      return '0;
    end else if (dest < 6'd24) begin
      return {{(DcacheWayIdxWidth-1){1'b0}}, 1'b1};
    end else if (dest < 6'd36) begin
      return {{(DcacheWayIdxWidth-2){1'b0}}, 2'b10};
    end else if (dest < 6'd48) begin
      return {{(DcacheWayIdxWidth-2){1'b0}}, 2'b11};
    end else if (dest < 6'd60) begin
      return '0;
    end
    return {{(DcacheWayIdxWidth-1){1'b0}}, 1'b1};
  endfunction

endpackage
