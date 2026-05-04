// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire Cache package — types, enums, and constants.
//
// Replaces shire_cache_defines.vh + shire_cache_types.vh.
// Constants are derived from the CORE-ET SoC Gen1 configuration.
// All `define macros are converted to localparam.

package shirecache_pkg;
  /* verilator lint_off UNUSEDPARAM */  // package constants consumed by modules not yet instantiated
  /* verilator lint_off VARHIDDEN */  // module parameters intentionally shadow package constants for configurability

  import etlink_pkg::*;

  // ════════════════════════════════════════════════════════
  // SoC-level parameters (from shire_defines.vh)
  // ════════════════════════════════════════════════════════

  localparam int unsigned PaSize               = etlink_pkg::PaSize;  // 40

  localparam int unsigned NumNeigh              = 4;   // neighborhoods per shire
  localparam int unsigned RboxPerShire          = 1;
  localparam int unsigned NumShireIds           = 256;
  localparam int unsigned ThreadsPerShire       = 32;
  localparam int unsigned EtEcoShirecacheBankInputWidth  = 10;
  localparam int unsigned EtEcoShirecacheBankOutputWidth = 10;

  // ════════════════════════════════════════════════════════
  // Cache geometry
  // ════════════════════════════════════════════════════════

  localparam int unsigned CacheSizeMB           = 4;
  localparam int unsigned CacheSize             = CacheSizeMB * 1024 * 1024;

  localparam int unsigned LineDataSize          = 512;    // bits per cache line
  localparam int unsigned LineByteSize          = LineDataSize / 8;  // 64 bytes
  localparam int unsigned LineDwSize            = LineDataSize / 64; // 8 doublewords
  localparam int unsigned LineQwSize            = LineDataSize / 128; // 4 quadwords
  localparam int unsigned LineSwSize            = LineDataSize / 32; // 16 single words
  localparam int unsigned LineOffsetIdSize      = $clog2(LineByteSize); // 6

  localparam int unsigned Banks                 = 4;
  localparam int unsigned BankIdSize            = $clog2(Banks);   // 2
  localparam int unsigned BankLsb               = LineOffsetIdSize; // 6

  localparam int unsigned SubBanks              = 4;
  localparam int unsigned SubBankIdSize         = $clog2(SubBanks); // 2
  localparam int unsigned SubBankLsb            = LineOffsetIdSize + BankIdSize; // 8

  localparam int unsigned LinesPerSubBank       = CacheSize / LineByteSize / Banks / SubBanks;
  localparam int unsigned Ways                  = 4;
  localparam int unsigned WayIdSize             = $clog2(Ways); // 2
  localparam int unsigned SetsPerSubBank        = LinesPerSubBank / Ways;
  localparam int unsigned SetIdSize             = $clog2(SetsPerSubBank);
  localparam int unsigned SetAllocSize          = SetIdSize + 1;
  localparam int unsigned SetLsb                = LineOffsetIdSize + BankIdSize + SubBankIdSize;

  // Quad-word addressing within a line
  localparam int unsigned LineQwIdSize          = $clog2(LineQwSize); // 2

  // Tag
  localparam int unsigned TagMaxSize            = 23;
  localparam int unsigned TagIdSize             = TagMaxSize;
  localparam int unsigned TagsDataSize          = Ways * TagDataSize; // 92 — all ways concatenated

  // ECC
  localparam int unsigned TagStateEccSize       = 7;
  localparam int unsigned TagStateLruSize       = 5;
  localparam int unsigned TagStateFlagsSize     = 7; // valid, locked, zero, qwen[3:0]
  localparam int unsigned TagStateDataSize      = Ways * TagStateFlagsSize + TagStateLruSize;
  localparam int unsigned TagStateRamDataSize   = TagStateDataSize + TagStateEccSize;
  localparam int unsigned TagEccSize            = 6;
  localparam int unsigned TagDataSize           = TagMaxSize;
  localparam int unsigned TagRamDataSize        = Ways * (TagDataSize + TagEccSize);
  localparam int unsigned DwEccSize             = 8;
  localparam int unsigned LineEccSize           = LineDwSize * DwEccSize;
  localparam int unsigned DataRamAddrSize       = WayIdSize + SetIdSize;
  localparam int unsigned DataRamDataSize       = LineDataSize + LineEccSize;
  localparam int unsigned DataRamDepth          = LinesPerSubBank;

  // RAM
  localparam int unsigned RamDelaySize          = 3;
  localparam int unsigned RamDelayDefault       = 2;

  // RAM address sizes
  localparam int unsigned TagRamAddrSize        = SetIdSize;         // 9
  localparam int unsigned TagStateRamAddrSize   = SetIdSize;         // 9
  // DataRamAddrSize already defined above (= WayIdSize + SetIdSize)

  // ════════════════════════════════════════════════════════
  // L2 address decomposition
  // ════════════════════════════════════════════════════════

  localparam int unsigned TagSetSelAddrSize     = PaSize - SetLsb;            // 30
  localparam int unsigned TagOfTagSetLsb        = TagSetSelAddrSize - TagMaxSize; // 7
  localparam int unsigned TagLsb                = SetLsb + TagOfTagSetLsb;    // 17
  localparam int unsigned TagIdSizeL2           = TagMaxSize;                 // 23

  // ════════════════════════════════════════════════════════
  // L3 address decomposition (after swizzle)
  // ════════════════════════════════════════════════════════

  localparam int unsigned L3ShireLsb            = LineOffsetIdSize;                                              // 6
  localparam int unsigned L3ShireMsb            = L3ShireLsb + L3ShireIdSize - 1;                               // 10
  localparam int unsigned L3BankLsb             = LineOffsetIdSize + L3ShireIdSize;                              // 11
  localparam int unsigned L3SubBankLsb          = LineOffsetIdSize + L3ShireIdSize + BankIdSize;                 // 13
  localparam int unsigned L3SetLsb              = LineOffsetIdSize + L3ShireIdSize + BankIdSize + SubBankIdSize; // 15
  localparam int unsigned L3TagSetSelAddrSize   = PaSize - L3SetLsb;                                            // 25
  localparam int unsigned L3TagOfTagSetLsb      = L3TagSetSelAddrSize - TagMaxSize;                              // 2
  localparam int unsigned L3TagLsb              = L3SetLsb + L3TagOfTagSetLsb;                                  // 17
  localparam int unsigned L3TagIdSize           = TagMaxSize;                                                    // 23

  // ════════════════════════════════════════════════════════
  // SCP address — way comes from sub-bank bits
  // ════════════════════════════════════════════════════════

  localparam int unsigned ScpWayLsb             = SetLsb;  // 10 — SCP way is after bank+sub_bank

  // ════════════════════════════════════════════════════════
  // BIST address/data sizes
  // ════════════════════════════════════════════════════════

  localparam int unsigned MbsAddrSize           = TagStateRamAddrSize + SubBankIdSize;     // 11
  localparam int unsigned MbsDataSize           = TagStateRamDataSize;                     // 40
  localparam int unsigned MbtAddrSize           = TagRamAddrSize + SubBankIdSize;          // 11
  localparam int unsigned MbtDataSize           = TagRamDataSize;                          // 116
  localparam int unsigned MbdAddrSize           = DataRamAddrSize + SubBankIdSize + LineQwIdSize; // 15
  localparam int unsigned MbdDataSize           = DataRamDataSize / LineQwSize;            // 144

  // ════════════════════════════════════════════════════════
  // Port configuration
  // ════════════════════════════════════════════════════════

  localparam int unsigned Ports                 = NumNeigh + RboxPerShire; // 5
  localparam int unsigned PortIdSize            = $clog2(Ports);

  localparam int unsigned L3MasterPorts         = 4;
  localparam int unsigned L3SlavePorts          = 4;
  localparam int unsigned SysPorts              = 1;

  localparam int unsigned L3Shires              = 32;
  localparam int unsigned L3ShireIdSize         = $clog2(L3Shires); // 5
  localparam int unsigned ScpShires             = NumShireIds;
  localparam int unsigned ScpShireIdSize        = $clog2(ScpShires); // 8
  localparam int unsigned ScpShireLsb           = 23;
  localparam int unsigned ScpRegionIdSize       = 9;
  localparam int unsigned ScpRegionLsb          = 31;
  localparam logic [ScpRegionIdSize-1:0] ScpRegionId = 9'h001;
  localparam int unsigned MemShires             = 8;

  // ════════════════════════════════════════════════════════
  // Request queue
  // ════════════════════════════════════════════════════════

  localparam int unsigned ReqqDepth             = 64;
  localparam int unsigned ReqqIdSize            = 6;
  localparam int unsigned ReqqSourceSize        = 4; // max(ET_LINK_SOURCE_SIZE, MESH_SOURCE_BRIDGE_ID_SIZE)
  localparam int unsigned ReqqTagIdSize         = 8; // max(ET_LINK_ID_SIZE, SC_MESH_MASTER_AXI_ID_SIZE)

  // ════════════════════════════════════════════════════════
  // L3 swizzle
  // ════════════════════════════════════════════════════════

  localparam int unsigned BanksMax              = 8;
  localparam int unsigned BankIdSizeMax         = $clog2(BanksMax);   // 3
  localparam int unsigned SubBanksMax           = 8;
  localparam int unsigned SubBankIdSizeMax      = $clog2(SubBanksMax); // 3
  localparam int unsigned L3ShiresMax           = 64;
  localparam int unsigned L3ShireIdSizeMax      = $clog2(L3ShiresMax); // 6
  localparam int unsigned L3SwizzleBitSelSize   = $clog2(BankIdSizeMax + SubBankIdSizeMax + L3ShireIdSizeMax); // 4
  localparam int unsigned L3SwizzleSelSize      = 1 << L3SwizzleBitSelSize; // 16
  localparam int unsigned L3SwizzleIdSize       = L3ShireIdSize + BankIdSize + SubBankIdSize;

  // ════════════════════════════════════════════════════════
  // Buffers
  // ════════════════════════════════════════════════════════

  localparam int unsigned RbufSize              = 8;
  localparam int unsigned RbufIdSize            = $clog2(RbufSize);
  localparam int unsigned CbufSize              = 32;
  localparam int unsigned CbufIdSize            = $clog2(CbufSize);

  // ════════════════════════════════════════════════════════
  // Misc
  // ════════════════════════════════════════════════════════

  localparam int unsigned AxiAxqosSize           = 4;
  localparam int unsigned CgateDisSize          = 8;
  localparam int unsigned L3YieldSize           = 5;
  localparam int unsigned EsrSetBaseSize        = 12;
  localparam int unsigned EsrSetSizeSize        = 13;
  localparam int unsigned SbeCountSize          = 8;
  localparam int unsigned DbeCountSize          = 3;
  localparam int unsigned EsrStatusSize         = 64;
  localparam int unsigned ErrIntSize            = 8;
  localparam int unsigned TraceEnSize           = 12;
  localparam int unsigned PerfmonIncrSize       = 7;
  localparam int unsigned DvTransIdSize         = 40;

  // ════════════════════════════════════════════════════════
  // Byte enable masks — one for each ET-Link size
  // Replaces: SC_BYTE_BEN, SC_HW_BEN, SC_SW_BEN, SC_DW_BEN,
  //           SC_QW_BEN, SC_HL_BEN, SC_LINE_BEN, SC_SCPFILL_BEN
  // ════════════════════════════════════════════════════════

  localparam logic [LineByteSize-1:0] ByteBen     = LineByteSize'(64'h0000_0000_0000_0001);
  localparam logic [LineByteSize-1:0] HwBen       = LineByteSize'(64'h0000_0000_0000_0003);
  localparam logic [LineByteSize-1:0] SwBen       = LineByteSize'(64'h0000_0000_0000_000F);
  localparam logic [LineByteSize-1:0] DwBen       = LineByteSize'(64'h0000_0000_0000_00FF);
  localparam logic [LineByteSize-1:0] QwBen       = LineByteSize'(64'h0000_0000_0000_FFFF);
  localparam logic [LineByteSize-1:0] HlBen       = LineByteSize'(64'h0000_0000_FFFF_FFFF);
  localparam logic [LineByteSize-1:0] LineBen     = {LineByteSize{1'b1}};
  localparam logic [LineByteSize-1:0] ScpfillBen  = LineByteSize'(64'h0000_0000_0000_FFFF); // QWord

  // ════════════════════════════════════════════════════════
  // Enums
  // ════════════════════════════════════════════════════════

  typedef etlink_pkg::size_e sc_size_t;

  typedef enum logic {
    NormalRsp  = 1'b0,
    VictimData = 1'b1
  } pipe_data_rsp_type_e;

  typedef enum logic [2:0] {
    AddressL2  = 3'h0,
    AddressL3  = 3'h1,
    AddressScp = 3'h2,
    AddressIdx = 3'h3,
    AddressMsg = 3'h4
  } address_space_e;

  typedef enum logic [1:0] {
    ReqqSrcL2  = 2'h0,
    ReqqSrcL3  = 2'h1,
    ReqqSrcFsm = 2'h2
  } reqq_source_e;

  typedef enum logic [1:0] {
    CacheL1  = 2'h0,
    CacheL2  = 2'h1,
    CacheL3  = 2'h2,
    CacheMem = 2'h3
  } cache_level_e;

  typedef enum logic {
    ToL3  = 1'b0,
    ToSys = 1'b1
  } mesh_dest_e;

  typedef enum logic [4:0] {
    OrigRead              = 5'h00,
    OrigWrite             = 5'h01,
    OrigWriteAround       = 5'h02,
    OrigWritePartial      = 5'h03,
    OrigReadCoop          = 5'h04,
    OrigMsgSendData       = 5'h05,
    OrigAtomic            = 5'h06,
    OrigScpRead           = 5'h07,
    OrigScpWrite          = 5'h08,
    OrigScpWritePartial   = 5'h09,
    OrigScpReadCoop       = 5'h0a,
    OrigIdx               = 5'h0b,
    OrigSync              = 5'h0c,
    OrigReadForward       = 5'h0d,
    OrigWriteForward      = 5'h0e,
    OrigWriteForwardPart  = 5'h0f,
    OrigCopLock           = 5'h10,
    OrigCopUnlock         = 5'h11,
    OrigCopUnlockInv      = 5'h12,
    OrigCopFlush          = 5'h13,
    OrigCopEvict          = 5'h14,
    OrigCopFlushWData     = 5'h15,
    OrigCopEvictWData     = 5'h16,
    OrigCopPrefetch       = 5'h17,
    OrigCopScpFill        = 5'h18,
    OrigAtomic2           = 5'h19,
    OrigWriteAround2      = 5'h1a,
    OrigWritePartial2     = 5'h1b,
    OrigErr               = 5'h1c,
    OrigCopFlushNext      = 5'h1d,
    OrigCopEvictNext      = 5'h1e,
    OrigCopPrefetchNext   = 5'h1f
  } reqq_orig_opcode_e;

  typedef enum logic [5:0] {
    L2Read               = 6'h00,
    L2Write              = 6'h01,
    L2WritePartial       = 6'h02,
    L2WritePartial2nd    = 6'h03,
    L2WriteAround        = 6'h04,
    L2Lock               = 6'h05,
    L2Unlock             = 6'h06,
    L2UnlockInv          = 6'h07,
    L2Flush              = 6'h08,
    L2Evict              = 6'h09,
    L2Prefetch           = 6'h0a,
    L2Atomic             = 6'h0b,
    L2Atomic2nd          = 6'h0c,
    L2Fill               = 6'h0d,
    L2Scrub              = 6'h0e,
    L2MsgSendData        = 6'h0f,
    L3Read               = 6'h10,
    L3Write              = 6'h11,
    L3WritePartial       = 6'h12,
    L3WritePartial2nd    = 6'h13,
    L3Flush              = 6'h14,
    L3FlushWData         = 6'h15,
    L3Evict              = 6'h16,
    L3EvictWData         = 6'h17,
    L3Prefetch           = 6'h18,
    L3Atomic             = 6'h19,
    L3Atomic2nd          = 6'h1a,
    L3Fill               = 6'h1b,
    L3Scrub              = 6'h1c,
    ScpRead              = 6'h1d,
    ScpWrite             = 6'h1e,
    ScpWritePartial      = 6'h1f,
    ScpAtomic            = 6'h20,
    ScpFill              = 6'h21,
    ScpScrub             = 6'h22,
    ScpZero              = 6'h23,
    IdxAbort             = 6'h25,
    IdxRead              = 6'h26,
    IdxWrite             = 6'h27,
    IdxAllInv            = 6'h28,
    IdxL2Inv             = 6'h29,
    IdxL2Flush           = 6'h2a,
    IdxL2Evict           = 6'h2b,
    IdxL3Inv             = 6'h2c,
    IdxL3Flush           = 6'h2d,
    IdxL3Evict           = 6'h2e,
    OpSync               = 6'h2f,
    MeshRead             = 6'h30,
    MeshWrite            = 6'h31,
    MeshFlush            = 6'h32,
    MeshFlushToMem       = 6'h33,
    MeshEvict            = 6'h34,
    MeshEvictToMem       = 6'h35,
    MeshPrefetch         = 6'h36,
    MeshVictim           = 6'h37,
    MeshAtomicRsp        = 6'h38,
    ReqqWait             = 6'h3a,
    ReqqWait2            = 6'h3b,
    ReqqAllocErr         = 6'h3c,
    ReqqInitial          = 6'h3d,
    ReqqInitial2         = 6'h3e,
    ReqqDone             = 6'h3f
  } reqq_opcode_e;

  typedef enum logic [2:0] {
    DataqRdPipe     = 3'd0,
    DataqRdToL3Mesh = 3'd1,
    DataqRdRspmuxL2 = 3'd2,
    DataqRdToSysMesh= 3'd3,
    DataqRdRspmuxL3 = 3'd4
  } dataq_rd_port_e;

  typedef enum logic [2:0] {
    CBufInstall   = 3'd0,
    CBufMruUpdate = 3'd1,
    CBufClear     = 3'd2,
    CBufReplace   = 3'd3,
    CBufBypass    = 3'd4
  } cbuf_opcode_e;

  typedef enum logic [3:0] {
    AmoSwap     = 4'd0,
    AmoAdd      = 4'd1,
    AmoXor      = 4'd2,
    AmoAnd      = 4'd3,
    AmoOr       = 4'd4,
    AmoMin      = 4'd5,
    AmoMax      = 4'd6,
    AmoMinU     = 4'd7,
    AmoMaxU     = 4'd8,
    AmoMinPs    = 4'd9,
    AmoMaxPs    = 4'd10,
    AmoCompExch = 4'd11
  } amo_opcode_e;

  typedef enum logic [3:0] {
    IdxCopAllInv   = 4'd0,
    IdxCopL2Inv    = 4'd1,
    IdxCopL2Flush  = 4'd2,
    IdxCopL2Evict  = 4'd3,
    IdxCopL3Inv    = 4'd4,
    IdxCopL3Flush  = 4'd5,
    IdxCopL3Evict  = 4'd6,
    IdxCopDbgRead  = 4'd7,
    IdxCopDbgWrite = 4'd8,
    IdxCopScpZero  = 4'd9,
    IdxCopCBInv    = 4'd10
  } idx_cop_sm_opcode_e;

  typedef enum logic [4:0] {
    ErrNone                  = 5'h0,
    PipeErrL3ShireDec        = 5'h1,
    PipeErrScpShireDec       = 5'h2,
    PipeErrScpRegionDec      = 5'h3,
    PipeErrL2OpNonEn         = 5'h4,
    PipeErrL3OpNonEn         = 5'h5,
    PipeErrScpOpNonEn        = 5'h6,
    PipeErrScpSetRange       = 5'h7,
    PipeErrLock              = 5'h8,
    ReqqErrMeshResp          = 5'h10,
    ReqqErrRemoteScpDisabled = 5'h11,
    ReqqErrL2BypassAtomic    = 5'h12,
    ReqqErrL3BypassAtomic    = 5'h13,
    ReqqErrUnsupported       = 5'h14,
    ReqqErrIllegalPort       = 5'h15
  } err_rsp_e;

  typedef enum logic [2:0] {
    TraceNone    = 3'h0,
    TraceAllocL2 = 3'h1,
    TraceAllocL3 = 3'h2,
    TraceTC      = 3'h3,
    TraceRbuf    = 3'h4,
    TraceMesh    = 3'h5,
    TraceReqq    = 3'h6
  } trace_type_e;

  // ════════════════════════════════════════════════════════
  // L3 swizzle control struct (from ESR)
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    logic                           all_shire_aliasing;
    logic                           two_shire_aliasing_use_shire_lsb;
    logic [L3SwizzleBitSelSize-1:0] sub_bank_sel_b2;
    logic [L3SwizzleBitSelSize-1:0] sub_bank_sel_b1;
    logic [L3SwizzleBitSelSize-1:0] sub_bank_sel_b0;
    logic [L3SwizzleBitSelSize-1:0] bank_sel_b2;
    logic [L3SwizzleBitSelSize-1:0] bank_sel_b1;
    logic [L3SwizzleBitSelSize-1:0] bank_sel_b0;
    logic [L3SwizzleBitSelSize-1:0] shire_sel_b5;
    logic [L3SwizzleBitSelSize-1:0] shire_sel_b4;
    logic [L3SwizzleBitSelSize-1:0] shire_sel_b3;
    logic [L3SwizzleBitSelSize-1:0] shire_sel_b2;
    logic [L3SwizzleBitSelSize-1:0] shire_sel_b1;
    logic [L3SwizzleBitSelSize-1:0] shire_sel_b0;
  } l3_swizzle_ctl_t;

  // ════════════════════════════════════════════════════════
  // Data word sizes
  // ════════════════════════════════════════════════════════

  localparam int unsigned SwDataSize            = 32;   // single word
  localparam int unsigned DwDataSize            = 64;   // double word
  localparam int unsigned QwDataSize            = 128;  // quad word

  // ═══════════════════════════════════════════════════���════
  // Atomic parameters
  // ════════════════════════════════════════════════════════

  localparam int unsigned AtomicSimdSize        = 8;    // SIMD lanes
  localparam int unsigned AtomicDataSize        = AtomicSimdSize * SwDataSize;          // 256
  localparam int unsigned AtomicDataIdSize      = $clog2(LineDataSize / AtomicDataSize); // 1
  localparam int unsigned LineDwIdSize          = $clog2(LineDwSize);                   // 3
  localparam int unsigned LineSwIdSize          = $clog2(LineSwSize);                   // 4

  // Atomic address-offset bit-select ranges within LineOffsetIdSize-bit offset
  localparam int unsigned AtomicDataSelMsb      = LineOffsetIdSize - 1;                     // 5
  localparam int unsigned AtomicDataSelLsb      = LineOffsetIdSize - AtomicDataIdSize;      // 5
  localparam int unsigned AtomicDwSelMsb        = LineOffsetIdSize - 1;                     // 5
  localparam int unsigned AtomicDwSelLsb        = LineOffsetIdSize - LineDwIdSize;          // 3
  localparam int unsigned AtomicSwSelMsb        = LineOffsetIdSize - 1;                     // 5
  localparam int unsigned AtomicSwSelLsb        = LineOffsetIdSize - LineSwIdSize;          // 2
  localparam int unsigned AtomicSc64Msb         = LineOffsetIdSize - AtomicDataIdSize - 1;  // 4
  localparam int unsigned AtomicSc64Lsb         = LineOffsetIdSize - LineDwIdSize;          // 3
  localparam int unsigned AtomicSc32Msb         = LineOffsetIdSize - AtomicDataIdSize - 1;  // 4
  localparam int unsigned AtomicSc32Lsb         = LineOffsetIdSize - LineSwIdSize;          // 2

  // ════════════════════════════════════════════════════════
  // Performance monitor
  // ════════════════════════════════════════════════════════

  localparam int unsigned PerfmonEvQualSize     = 3;
  localparam int unsigned PerfmonRsrcReqSize    = 5;
  localparam int unsigned PerfmonRsrcRspSize    = PerfmonIncrSize;  // 7
  localparam int unsigned PerfmonRsrcCbuf       = 4;  // bit position in rsrc_mode

  // Perfmon event mode bit positions
  // Replaces: SC_PERFMON_EV_* (shire_cache_defines.vh)
  localparam int unsigned PerfmonEvMsg          = 1;  // ev_mode bit for MsgSendData
  localparam int unsigned PerfmonEvTag          = 2;  // ev_mode bit for tag events

  // Perfmon resource mode bit positions (two interleaved port assignments)
  // Replaces: SC_PERFMON_RSRC_* (shire_cache_defines.vh)
  // Port 0 uses: L2_REQQ(0), L3_BUSY(1), ANY_TO_L3(2), L2_TO_L3(3), L3_TO_SYS(4)
  localparam int unsigned PerfmonRsrcL2Reqq     = 0;
  localparam int unsigned PerfmonRsrcL3Busy     = 1;
  localparam int unsigned PerfmonRsrcAnyToL3    = 2;
  localparam int unsigned PerfmonRsrcL2ToL3     = 3;
  localparam int unsigned PerfmonRsrcL3ToSys    = 4;
  // Port 1 uses: L3_REQQ(0), L2_BUSY(1), ANY_TO_SYS(2), L2_TO_SYS(3)
  localparam int unsigned PerfmonRsrcL3Reqq     = 0;
  localparam int unsigned PerfmonRsrcL2Busy     = 1;
  localparam int unsigned PerfmonRsrcAnyToSys   = 2;
  localparam int unsigned PerfmonRsrcL2ToSys    = 3;

  // ════════════════════════════════════════════════════════
  // Pipeline structs
  // ════════════════════════════════════════════════════════

  // Pipe info — pipelined from sub_bank _tap/_tc stages to _dc
  typedef struct packed {
    logic [SetIdSize-1:0]       pipe_set;
    logic [SubBanks-1:0]        pipe_sub_bank;
    address_space_e             pipe_idx_address_space;
    logic                       pipe_dec_err;
    err_rsp_e                   pipe_dec_err_type;
    logic                       pipe_scp_read;
    logic                       pipe_bubble;
    logic                       idx_write;
    logic                       tag_hit;
    logic [WayIdSize-1:0]       tag_ram_hit_way;
    logic                       tag_sbe;
    logic                       victim;
    logic                       data_rd_req;
    logic                       data_wr_req;
    logic                       data_wr_zero;
    logic [LineQwSize-1:0]      data_wr_qwen;
    logic                       dataq_rd_req_en;
    logic                       dataq_rd_req_partial;
    logic [LineQwSize-1:0]      dataq_sel_qwen;
    logic                       merge_wr_data_in_rd_rsp;
    logic                       merge_rd_data_in_wr_req;
    logic                       data_rsp_req;
    logic                       data_rsp_zero;
    logic [LineQwSize-1:0]      data_rsp_qwen;
  } pipe_info_t;

  // Request queue → pipe request
  typedef struct packed {
    logic [DvTransIdSize-1:0]   trans_id;
    logic [ReqqIdSize-1:0]      reqq_id;
    reqq_opcode_e               opcode;
    logic [PaSize-1:0]          address;
    address_space_e             address_space;
    logic                       zero_data;
    logic [LineQwSize-1:0]      qwen;
    sc_size_t                   size;
    logic [SubBankIdSize-1:0]   sub_bank_id;
  } reqq_pipe_req_t;

  // Atomic operation configuration (packed in dataq operand field)
  typedef struct packed {
    logic                       dest;
    logic                       size;
    logic                       sc_vec;
    amo_opcode_e                opcode;
  } amo_conf_req_t;

  // ═════════════════════════════���══════════════════════════
  // Coalesce buffer (cbuf) structs
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    logic [DvTransIdSize-1:0]   trans_id;
    logic [ReqqIdSize-1:0]      reqq_id;
    logic                       rsp_required;
    cbuf_opcode_e               opcode;
    logic [PaSize-1:0]          install_address;
    logic [PaSize-1:0]          clear_address;
  } cbuf_req_t;

  typedef struct packed {
    logic [DvTransIdSize-1:0]   trans_id;
    logic [ReqqIdSize-1:0]      reqq_id;
    logic                       victim;
    logic [PaSize-1:0]          victim_address;
  } cbuf_rsp_t;

  typedef struct packed {
    logic [CbufSize-1:0]                cbuf_mem_valid;
    logic [CbufSize-1:0][PaSize-1:0]   cbuf_mem;
  } cbuf_state_t;

  // ════════════════════════════════════════════════════════
  // Read buffer (rbuf) structs
  // ════════════════════════════════════════════════════════

  localparam int unsigned RbufFifoSize           = 4;
  localparam int unsigned RbufPendingInstallSize  = 8;
  localparam int unsigned PerfmonEvRbuf           = 0;  // bit position in ev_mode

  typedef struct packed {
    logic [DvTransIdSize-1:0]   trans_id;
    logic [ReqqIdSize-1:0]      reqq_id;
    logic                       scp;       // 0=L2, 1=Scp
    logic [PaSize-1:0]          address;
  } rbuf_install_req_t;

  typedef struct packed {
    logic [DvTransIdSize-1:0]   trans_id;
    logic [ReqqIdSize-1:0]      reqq_id;
    logic                       scp;
    logic [PaSize-1:0]          address;
  } rbuf_evict_req_t;

  typedef struct packed {
    logic [DvTransIdSize-1:0]   trans_id;
    logic [ReqqIdSize-1:0]      reqq_id;
    logic                       scp;
    reqq_opcode_e               opcode;    // assertion only
    logic [PaSize-1:0]          address;
    logic [LineDataSize-1:0]    data;
  } rbuf_data_req_t;

  typedef struct packed {
    logic [RbufSize-1:0]                    rbuf_clear_mem_valid_hot;
    logic [RbufSize-1:0]                    rbuf_clear_mem_scp;
    logic [RbufSize-1:0][PaSize-1:0]        rbuf_clear_mem_address;
    logic [RbufSize-1:0]                    rbuf_clear_pending_install_hot;
    logic [RbufSize-1:0]                    rbuf_clear_pending_install_scp;
    logic [RbufSize-1:0][PaSize-1:0]        rbuf_clear_pending_install_address;
  } rbuf_clear_req_t;

  typedef struct packed {
    logic [DvTransIdSize-1:0]   trans_id;
    logic [ReqqIdSize-1:0]      reqq_id;
    logic                       l3_source;
    logic                       scp;       // assertion only
    logic [PaSize-1:0]          address;   // assertion only
    logic [RbufIdSize-1:0]      rbuf_id;
  } rbuf_rd_req_t;

  typedef struct packed {
    logic [DvTransIdSize-1:0]   trans_id;
    logic [ReqqIdSize-1:0]      reqq_id;
    logic                       l3_source;
    logic [LineDataSize-1:0]    data;
  } rbuf_rd_rsp_t;

  typedef struct packed {
    // mem state
    logic [RbufSize-1:0]                                rbuf_mem_valid;
    logic [RbufSize-1:0]                                rbuf_mem_scp;
    logic [RbufSize-1:0][PaSize-1:0]                    rbuf_mem;
    // eviction notice
    logic                                               rbuf_evict;
    logic                                               rbuf_evict_scp;
    logic [RbufIdSize-1:0]                              rbuf_evict_id;
    logic [PaSize-1:0]                                  rbuf_evict_address;
    // install notice
    logic                                               rbuf_install;
    logic                                               rbuf_install_scp;
    logic [RbufIdSize-1:0]                              rbuf_install_id;
    logic [PaSize-1:0]                                  rbuf_install_address;
    // pending install
    logic                                               rbuf_pending_install;
    logic                                               rbuf_pending_install_scp;
    logic [RbufIdSize-1:0]                              rbuf_pending_install_id;
    logic [PaSize-1:0]                                  rbuf_pending_install_address;
    // pending install pipeline (per stage)
    logic [RbufPendingInstallSize-1:0][RbufSize-1:0]    rbuf_pending_install_pipe_valid;
    logic [RbufPendingInstallSize-1:0]                  rbuf_pending_install_pipe_scp;
    logic [RbufPendingInstallSize-1:0][PaSize-1:0]      rbuf_pending_install_pipe_address;
  } rbuf_state_t;

  // ════════════════════════════════════════════════════════
  // Performance monitor structs
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    logic                               cgater;
    logic [PerfmonEvQualSize-1:0]       ev_mode;
    logic [PerfmonRsrcReqSize-1:0]      rsrc_mode;
  } perfmon_req_info_t;

  typedef struct packed {
    logic                               ev_rbuf_l2_hit;
    logic                               ev_rbuf_scp_hit;
    logic                               ev_msgsenddata;
    logic                               ev_tag_bubble;
    logic                               ev_tag_hit;
    logic                               ev_tag_miss;
    logic                               ev_tag_evict_dirty;
    logic                               ev_tag_evict_write_around;
    logic                               ev_tag_evict_none;
    logic [LineQwSize-1:0]              ev_tag_victim_qwen;
    reqq_opcode_e                       ev_tag_opcode;
  } perfmon_rsp_ev_info_t;

  typedef struct packed {
    logic [PerfmonRsrcRspSize-1:0]      rsrc_cntr;
  } perfmon_rsp_rsrc_info_t;

  typedef struct packed {
    perfmon_rsp_ev_info_t               ev_rsp;
    perfmon_rsp_rsrc_info_t             rsrc_rsp;
  } perfmon_rsp_info_t;

  // ════════════════════════════════════════════════════════
  // Tag state — per-way flags + shared LRU
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    logic                   valid;
    logic                   locked;
    logic                   zero;
    logic [LineQwSize-1:0]  qwen;
  } way_state_t;

  typedef struct packed {
    way_state_t [Ways-1:0]               way_state;
    logic [TagStateLruSize-1:0]          lru_state;
  } tag_state_t;

  // ════════════════════════════════════════════════════════
  // RAM request/response types
  // ════════════════════════════════════════════════════════

  // Tag state RAM
  typedef struct packed {
    logic [DvTransIdSize-1:0]          rd_trans_id;
    logic [ReqqIdSize-1:0]             rd_reqq_id;
    logic [TagStateRamAddrSize-1:0]    rd_addr;
    logic [DvTransIdSize-1:0]          wr_trans_id;
    logic [ReqqIdSize-1:0]             wr_reqq_id;
    logic [TagStateRamAddrSize-1:0]    wr_addr;
    logic [TagStateRamDataSize-1:0]    wr_data;
  } tag_state_ram_req_t;

  typedef struct packed {
    logic [DvTransIdSize-1:0]          trans_id;
    logic [ReqqIdSize-1:0]             reqq_id;
    logic [TagStateRamDataSize-1:0]    rd_data;
  } tag_state_ram_rsp_t;

  // Tag RAM
  typedef struct packed {
    logic [DvTransIdSize-1:0]          rd_trans_id;
    logic [ReqqIdSize-1:0]             rd_reqq_id;
    logic [TagRamAddrSize-1:0]         rd_addr;
    logic [DvTransIdSize-1:0]          wr_trans_id;
    logic [ReqqIdSize-1:0]             wr_reqq_id;
    logic [TagRamAddrSize-1:0]         wr_addr;
    logic [TagRamDataSize-1:0]         wr_data;
  } tag_ram_req_t;

  typedef struct packed {
    logic [DvTransIdSize-1:0]          trans_id;
    logic [ReqqIdSize-1:0]             reqq_id;
    logic [TagRamDataSize-1:0]         rd_data;
  } tag_ram_rsp_t;

  // Data RAM
  typedef struct packed {
    logic [DvTransIdSize-1:0]          rd_trans_id;
    logic [ReqqIdSize-1:0]             rd_reqq_id;
    logic [DataRamAddrSize-1:0]        rd_addr;
    logic [DvTransIdSize-1:0]          wr_trans_id;
    logic [ReqqIdSize-1:0]             wr_reqq_id;
    logic [DataRamAddrSize-1:0]        wr_addr;
    logic [DataRamDataSize-1:0]        wr_data;
    logic [LineQwSize-1:0]             wr_qwen;
    logic                              wr_xcheck_disable;
    logic [DvTransIdSize-1:0]          amo_wr_trans_id;
    logic [ReqqIdSize-1:0]             amo_wr_reqq_id;
    logic [DataRamAddrSize-1:0]        amo_wr_addr;
    logic [DataRamDataSize-1:0]        amo_wr_data;
    logic [LineQwSize-1:0]             amo_wr_qwen;
  } data_ram_req_t;

  typedef struct packed {
    logic [DvTransIdSize-1:0]          trans_id;
    logic [ReqqIdSize-1:0]             reqq_id;
    logic [DataRamDataSize-1:0]        rd_data;
  } data_ram_rsp_t;

  // ════════════════════════════════════════════════════════
  // Tag response (sub_bank → reqq)
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    logic [DvTransIdSize-1:0]   trans_id;
    logic [ReqqIdSize-1:0]      reqq_id;
    logic                       hit;
    logic [PaSize-1:0]          orig_address;
    logic                       victim;
    logic                       victim_silent;
    logic [PaSize-1:0]          victim_address;
    address_space_e             victim_address_space;
    logic                       victim_write_around;
    logic [LineQwSize-1:0]      victim_qwen;
    err_rsp_e                   err_rsp;
    logic [SetIdSize-1:0]       tag_index;
    logic                       bubble;
    reqq_opcode_e               opcode;
    logic [WayIdSize-1:0]       tag_ram_hit_way;
    logic [LineQwSize-1:0]      tag_ram_hit_qwen;
    logic                       tag_state_ram_sbe;
    logic                       tag_state_ram_dbe;
    logic [Ways-1:0]            tag_ram_sbe;
    logic [Ways-1:0]            tag_ram_dbe;
  } tag_rsp_t;

  // ════════════════════════════════════════════════════════
  // Data response types
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    logic [DvTransIdSize-1:0]   trans_id;
    logic [ReqqIdSize-1:0]      reqq_id;
    pipe_data_rsp_type_e        rsp_type;
  } data_rsp_pre_t;

  typedef struct packed {
    logic [DvTransIdSize-1:0]   trans_id;
    logic [ReqqIdSize-1:0]      reqq_id;
    pipe_data_rsp_type_e        rsp_type;
    logic                       tag_sbe;
    logic [LineDataSize-1:0]    data;
    logic [LineQwSize-1:0]      qwen;
    logic [DataRamAddrSize-1:0] data_index;
    logic [LineDwSize-1:0]      data_ram_sbe;
    logic [LineDwSize-1:0]      data_ram_dbe;
  } data_rsp_t;

  // ════════════════════════════════════════════════════════
  // Data queue read types
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    logic [DvTransIdSize-1:0]   trans_id;
    logic [ReqqIdSize-1:0]      reqq_id;
    logic                       partial;
    logic [LineQwSize-1:0]      qwen;
  } dataq_rd_req_t;

  typedef struct packed {
    logic [DvTransIdSize-1:0]   trans_id;
    logic [ReqqIdSize-1:0]      reqq_id;
    logic [LineDataSize-1:0]    data;
    logic [LineByteSize-1:0]    ben;
    logic                       err;
  } dataq_rd_rsp_t;

  // ════════════════════════════════════════════════════════
  // BIST types
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    logic                       mbist_on;
    logic                       mbs_sel;
    logic                       mbs_rd_en;
    logic                       mbs_wr_en;
    logic [MbsAddrSize-1:0]     mbs_addr;
    logic [MbsDataSize-1:0]     mbs_wdata;
    logic                       mbt_sel;
    logic                       mbt_rd_en;
    logic                       mbt_wr_en;
    logic [MbtAddrSize-1:0]     mbt_addr;
    logic [MbtDataSize-1:0]     mbt_wdata;
    logic                       mbd_sel;
    logic                       mbd_rd_en;
    logic                       mbd_wr_en;
    logic [MbdAddrSize-1:0]     mbd_addr;
    logic [MbdDataSize-1:0]     mbd_wdata;
  } bist_req_t;

  typedef struct packed {
    logic [MbsDataSize-1:0]     mbs_rdata;
    logic [MbtDataSize-1:0]     mbt_rdata;
    logic [MbdDataSize-1:0]     mbd_rdata;
  } bist_rsp_t;

  // ════════════════════════════════════════════════════════
  // SBE/DBE error status
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    logic [SbeCountSize-1:0]    tag_state_ram_sbe_count;
    logic [DbeCountSize-1:0]    tag_state_ram_dbe_count;
    logic [SbeCountSize-1:0]    tag_ram_sbe_count;
    logic [DbeCountSize-1:0]    tag_ram_dbe_count;
    logic [SbeCountSize-1:0]    data_ram_sbe_count;
    logic [DbeCountSize-1:0]    data_ram_dbe_count;
    logic [SbeCountSize-1:0]    dataq_ram_sbe_count;
    logic [DbeCountSize-1:0]    dataq_ram_dbe_count;
    logic [SbeCountSize-1:0]    ben_ram_sbe_count;
    logic [DbeCountSize-1:0]    ben_ram_dbe_count;
  } sbe_dbe_esr_status_t;

  typedef struct packed {
    logic                       tag_state_ram_sbe_overflow;
    logic                       tag_state_ram_dbe_overflow;
    logic                       tag_ram_sbe_overflow;
    logic                       tag_ram_dbe_overflow;
    logic                       data_ram_sbe_overflow;
    logic                       data_ram_dbe_overflow;
  } sbe_dbe_overflow_t;

  // ═══════════════════════════════════════════════��════════
  // Pipe-level types
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    logic [SubBanks-1:0]            sub_bank_busy;
  } pipe_busy_t;

  localparam int unsigned ScpSetLsb             = ScpWayLsb + WayIdSize; // 12 — SCP set starts after way

  // ════════════════════════════════════════════════════════
  // Index cache-op state machine types
  // ════════════════════════════════════════════════════════

  // Request from idx_cop_sm to reqq
  typedef struct packed {
    logic [DvTransIdSize-1:0]   trans_id;
    logic                       sync_flush_victims;
    reqq_opcode_e               opcode;
    logic [PaSize-1:0]          address;
  } idx_cop_req_t;

  // Debug write info from idx_cop_sm to sub_bank RAMs
  typedef struct packed {
    logic                              ecc_wr_en;
    logic                              tag_state_en;
    logic [TagStateDataSize-1:0]       tag_state;
    logic [TagStateRamDataSize-1:0]    tag_state_and_ecc;
    logic                              tags_en;
    logic [TagsDataSize-1:0]           tags;
    logic [TagRamDataSize-1:0]         tags_and_ecc;
    logic                              data_en;
    logic [LineDataSize-1:0]           data;
    logic [DataRamDataSize-1:0]        data_and_ecc;
    logic [LineQwSize-1:0]             data_qwen;
  } idx_cop_sm_dbg_write_t;

  // Debug read info from sub_bank RAMs to idx_cop_sm
  typedef struct packed {
    logic                              tag_state_and_ecc_valid;
    logic [TagStateRamDataSize-1:0]    tag_state_and_ecc;
    address_space_e                    tag_state_space;
    logic                              tags_and_ecc_valid;
    logic [TagRamDataSize-1:0]         tags_and_ecc;
    logic                              data_and_ecc_valid;
    logic [DataRamDataSize-1:0]        data_and_ecc;
  } idx_cop_sm_dbg_read_t;

  // ESR status registers from idx_cop_sm (read-back values)
  typedef struct packed {
    logic [EsrStatusSize-1:0]  ctl;
    logic [EsrStatusSize-1:0]  physical_index;
    logic [EsrStatusSize-1:0]  data0;
    logic [EsrStatusSize-1:0]  data1;
    logic [EsrStatusSize-1:0]  ecc;
  } idx_cop_sm_esr_status_t;

  // ════════════════════════════════════════════════════════
  // Bank ESR control — full config driven from ESR registers
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    // reqq esrs
    logic [CgateDisSize-1:0]            esr_sc_clk_gate_disable;
    logic [AxiAxqosSize-1:0]            esr_sc_axi_qos;
    logic                               esr_sc_cbuf_enable;
    logic                               esr_sc_remote_l3_enable;
    logic                               esr_sc_remote_scp_enable;
    logic                               esr_sc_l2_bypass;
    logic                               esr_sc_l3_bypass;
    logic [ReqqIdSize-1:0]              esr_sc_num_l3_reqq_entries;
    logic                               esr_sc_reqq_no_link_list;
    logic                               esr_sc_ecc_scrub_enable;
    logic [L3YieldSize-1:0]             esr_sc_l3_yield_priority;

    // pipe esrs
    logic                               esr_sc_idx_cop_sm_ctl_user_en;
    logic                               esr_sc_ram_deep_sleep;
    logic                               esr_sc_ram_shut_down;
    logic [RamDelaySize-1:0]            esr_sc_ram_delay;
    logic                               esr_sc_l2_rbuf_enable;
    logic                               esr_sc_scp_rbuf_enable;
    logic                               esr_sc_zero_state_enable;
    logic                               esr_sc_allow_only_1_req_per_sub_bank;
    logic                               esr_sc_allow_only_1_req_per_bank;
    logic [CbufSize-1:0]                esr_sc_cbuf_entry_enable;
    logic                               esr_sc_two_shire_aliasing_use_shire_lsb;
    logic                               esr_sc_all_shire_aliasing;
    logic [L3SwizzleBitSelSize-1:0]     esr_sc_sub_bank_sel_b2;
    logic [L3SwizzleBitSelSize-1:0]     esr_sc_sub_bank_sel_b1;
    logic [L3SwizzleBitSelSize-1:0]     esr_sc_sub_bank_sel_b0;
    logic [L3SwizzleBitSelSize-1:0]     esr_sc_bank_sel_b2;
    logic [L3SwizzleBitSelSize-1:0]     esr_sc_bank_sel_b1;
    logic [L3SwizzleBitSelSize-1:0]     esr_sc_bank_sel_b0;
    logic [L3SwizzleBitSelSize-1:0]     esr_sc_shire_sel_b5;
    logic [L3SwizzleBitSelSize-1:0]     esr_sc_shire_sel_b4;
    logic [L3SwizzleBitSelSize-1:0]     esr_sc_shire_sel_b3;
    logic [L3SwizzleBitSelSize-1:0]     esr_sc_shire_sel_b2;
    logic [L3SwizzleBitSelSize-1:0]     esr_sc_shire_sel_b1;
    logic [L3SwizzleBitSelSize-1:0]     esr_sc_shire_sel_b0;
    logic [SetIdSize-1:0]               esr_sc_l2_set_base;
    logic [SetAllocSize-1:0]            esr_sc_l2_set_size;
    logic [SetIdSize-1:0]               esr_sc_l2_set_mask;
    logic [SetIdSize-1:0]               esr_sc_l2_tag_mask;
    logic [SetIdSize-1:0]               esr_sc_l3_set_base;
    logic [SetAllocSize-1:0]            esr_sc_l3_set_size;
    logic [SetIdSize-1:0]               esr_sc_l3_set_mask;
    logic [SetIdSize-1:0]               esr_sc_l3_tag_mask;
    logic [SetIdSize-1:0]               esr_sc_scp_set_base;
    logic [SetAllocSize-1:0]            esr_sc_scp_set_size;
    logic [SetIdSize-1:0]               esr_sc_scp_set_mask;
    logic [SetIdSize-1:0]               esr_sc_scp_tag_mask;

    // err log esrs
    logic                               esr_sc_err_rsp_enable;
    logic [ErrIntSize-1:0]              esr_sc_err_interrupt_enable;

    // trace control
    logic [PaSize-1:0]                  esr_sc_trace_filter_address_enable;
    logic [PaSize-1:0]                  esr_sc_trace_filter_address_value;
    logic                               esr_sc_trace_filter_reqq_id_enable;
    logic [ReqqIdSize-1:0]              esr_sc_trace_filter_reqq_id;
    logic                               esr_sc_trace_filter_port_enable;
    logic [PortIdSize-1:0]              esr_sc_trace_filter_port;
    logic                               esr_sc_trace_filter_source_enable;
    logic [ReqqSourceSize-1:0]          esr_sc_trace_filter_source;
    logic                               esr_sc_trace_filter_l2_enable;
    logic                               esr_sc_trace_filter_l3_enable;
    logic                               esr_sc_trace_filter_fsm_enable;
    logic [TraceEnSize-1:0]             esr_sc_trace_type_hot_enable;

    // write enables
    logic                               wr_en_reqq_ctl;
    logic                               wr_en_idx_cop_sm_ctl;
    logic                               wr_en_idx_cop_sm_physical_index;
    logic                               wr_en_idx_cop_sm_data0;
    logic                               wr_en_idx_cop_sm_data1;
    logic                               wr_en_idx_cop_sm_ecc;
    logic                               wr_en_err_log_info;
    logic                               wr_en_sbe_dbe_counts;
    logic                               wr_en_reqq_debug_ctl;
    logic                               wr_en_perfmon_ctl_status;
    logic                               wr_en_perfmon_cyc_cntr;
    logic                               wr_en_perfmon_p0_cntr;
    logic                               wr_en_perfmon_p1_cntr;
    logic                               wr_en_perfmon_p0_qual;
    logic                               wr_en_perfmon_p1_qual;
    logic [DwDataSize-1:0]              wr_data;
    logic                               access_in_flight;
  } bank_esr_ctl_t;

  // ════════════════════════════════════════════════════════
  // Pipe ESR status (composite — must be after idx_cop_sm types)
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    idx_cop_sm_esr_status_t         pipe_idx_cop_sm_esr_status;
    sbe_dbe_esr_status_t            pipe_sbe_dbe_esr_status;
  } pipe_esr_status_t;

  // ════════════════════════════════════════════════════════
  // Reqq / dataq / rspmux shared types
  // ════════════════════════════════════════════════════════

  // Reqq bank request (from ET-Link/mesh to reqq)
  typedef struct packed {
    logic [DvTransIdSize-1:0]                 trans_id;
    logic [PortIdSize-1:0]                    port_id;
    logic [ReqqTagIdSize-1:0]                 id;
    logic [ReqqSourceSize-1:0]                source;
    logic [AxiAxqosSize-1:0]                  qos;
    etlink_pkg::req_opcode_e                  opcode;
    logic [etlink_pkg::SubopcodeSize-1:0]     subopcode;
    logic [PaSize-1:0]                        address;
    logic                                     wdata;
    logic [LineDataSize-1:0]                  data;
    logic [LineByteSize-1:0]                  ben;
    logic [LineQwSize-1:0]                    qwen;
    sc_size_t                                 size;
    logic                                     write_is_partial;
  } reqq_bank_req_t;

  // Reqq mesh response (fill data from L3/Sys mesh)
  typedef struct packed {
    logic [DvTransIdSize-1:0]    trans_id;
    logic [ReqqIdSize-1:0]       reqq_id;
    logic [ReqqIdSize-1:0]       dataq_id;
    logic [LineDataSize-1:0]     data;
    logic                        zero_data;
    logic                        err;
  } reqq_mesh_rsp_t;

  // Reqq mesh request (eviction/writeback to L3/Sys mesh)
  typedef struct packed {
    logic [DvTransIdSize-1:0]    trans_id;
    logic [ReqqIdSize-1:0]       reqq_id;
    logic [ReqqIdSize-1:0]       dataq_id;
    logic [LineDataSize-1:0]     data;
    logic                        with_data;
    logic                        rd_ben;
    logic [LineQwSize-1:0]       qwen;
    logic [LineByteSize-1:0]     ben;
  } reqq_mesh_req_t;

  // Response mux format
  typedef struct packed {
    logic [DvTransIdSize-1:0]    trans_id;
    logic [ReqqIdSize-1:0]       reqq_id;
    logic [LineDataSize-1:0]     data;
    logic                        with_data;
    logic                        zero_data;
    logic [LineQwSize-1:0]       qwen;
  } rspmux_t;

  // Crossbar response (bank reqq → xbar → neighborhoods)
  // Replaces: sc_xbar_rsp_t (shire_cache_types.vh)
  typedef struct packed {
    logic [PortIdSize-1:0]           port_id;
    logic [DvTransIdSize-1:0]        trans_id;
    logic                            src_is_uc;
    etlink_pkg::rsp_t                rsp_info;
  } xbar_rsp_t;

  // Crossbar request (neighborhoods → xbar → bank)
  // Replaces: sc_xbar_req_t (shire_cache_types.vh)
  typedef struct packed {
    logic [PortIdSize-1:0]           port_id;
    logic [DvTransIdSize-1:0]        trans_id;
    etlink_pkg::req_t                req_info;
  } xbar_req_t;

  // Extended rspmux type with port routing and xbar formatting
  // Replaces: sc_rspmux_long_t (shire_cache_types.vh)
  typedef struct packed {
    logic [DvTransIdSize-1:0]        trans_id;
    logic [ReqqIdSize-1:0]           reqq_id;
    logic [LineDataSize-1:0]         data;
    logic                            with_data;
    logic [Ports-1:0]                dest_hot;
    logic [PortIdSize-1:0]           port_id;
    logic                            traced;
    xbar_rsp_t                       xbar_info;
  } rspmux_long_t;

  // Rspmux sent notification (for trace)
  // Replaces: sc_rspmux_sent_t (shire_cache_types.vh)
  typedef struct packed {
    logic                            valid;
    logic [ReqqIdSize-1:0]           reqq_id;
    logic                            traced;
  } rspmux_sent_t;

  // Pipe data response modifier (reqq early lookup)
  typedef struct packed {
    logic [DvTransIdSize-1:0]    trans_id;
    logic [ReqqIdSize-1:0]       reqq_id;
    logic [ReqqIdSize-1:0]       dataq_id;
    address_space_e              address_space;
    reqq_source_e                reqq_source;
    mesh_dest_e                  which_mesh;
    logic                        disable_mesh_bypass;
    logic                        redirect_normal_rsp_to_mesh;
    logic [PortIdSize-1:0]       port_id;
    pipe_data_rsp_type_e         rsp_type;
  } data_rsp_modifier_t;

  // Dataq internal write struct
  typedef struct packed {
    logic [DvTransIdSize-1:0]    trans_id;
    logic [ReqqIdSize-1:0]       reqq_id;
    logic [ReqqIdSize-1:0]       dataq_id;
    logic [LineQwSize-1:0]       qwen;
    logic [LineDataSize-1:0]     data;
    logic                        wr_ben;
    logic [LineByteSize-1:0]     ben;
  } dataq_wr_t;

  // Dataq internal read struct
  typedef struct packed {
    logic [DvTransIdSize-1:0]    trans_id;
    logic [ReqqIdSize-1:0]       reqq_id;
    logic [ReqqIdSize-1:0]       dataq_id;
    logic [LineQwSize-1:0]       qwen;
    dataq_rd_port_e              rd_port_id;
    logic                        rd_ben;
  } dataq_rd_t;

  // Dataq data/ben wrapper types for read sink FIFOs
  typedef struct packed {
    logic [LineDataSize-1:0] data;
  } dataq_data_t;

  typedef struct packed {
    logic [LineByteSize-1:0] ben;
  } dataq_ben_t;

  // Atomic constants for dataq data packing
  localparam int unsigned AtomicConfSize       = $bits(amo_conf_req_t);
  localparam int unsigned AtomicConfLsb        = AtomicDataSize;
  localparam int unsigned AtomicAddrOffsetSize = LineOffsetIdSize;
  localparam int unsigned AtomicAddrOffsetLsb  = AtomicDataSize + AtomicConfSize;
  localparam int unsigned ByteDataSize         = 8;

  // L2 atomic data mask: low 2 QWords (256 bits) of a 512-bit line
  localparam logic [LineDataSize-1:0] L2AtomicDataMask = LineDataSize'({2*QwDataSize{1'b1}});

  // ════════════════════════════════════════════════════════
  // Request queue — constants for reqq_alloc
  // ════════════════════════════════════════════════════════

  // Clock gate disable indices
  // Replaces: SC_CLK_GATE_DISABLE_* (shire_cache_defines.vh)
  localparam int unsigned CgateDisBank        = 0;
  localparam int unsigned CgateDisReqqAlloc   = 1;
  localparam int unsigned CgateDisReqqEntries = 2;
  localparam int unsigned CgateDisMeshMaster  = 3;

  // Address range for cache-line dependency matching: PA[PA_SIZE-1 : BankLsb]
  // BankLsb = LineOffsetIdSize = 6 (64-byte lines)
  localparam int unsigned ReqqSelAddrLsb      = LineOffsetIdSize;

  // Message address field positions (from ET-Link address decode)
  localparam int unsigned MsgNeighIdLsb       = 16;
  localparam int unsigned MsgNeighIdSize      = 4;
  localparam int unsigned MsgIdLsb            = 3;
  localparam int unsigned MsgIdSize           = 8;
  localparam int unsigned MsgSourceLsb        = 12;
  localparam int unsigned MsgSourceSize       = 4;

  // Neighbor cooperative mask position in data field
  localparam int unsigned NeighCoopMaskLsb    = 16;

  // Cache level positions in data field (for CacheOps)
  localparam int unsigned StartLevelLsb       = 3;
  localparam int unsigned DestLevelLsb        = 5;

  // Unlock-invalidate flag bit in data field
  localparam int unsigned UnlockInvBit        = 3;

  // SCP shire address field (already have ScpShireLsb=23, ScpShireIdSize)
  localparam int unsigned ScpShireMsb         = ScpShireLsb + ScpShireIdSize - 1;

  // AXI QOS priority values
  localparam logic [AxiAxqosSize-1:0] AxiQosMemLowPriority  = 4'h0;
  localparam logic [AxiAxqosSize-1:0] AxiQosMemHighPriority = 4'h1;
  localparam logic [AxiAxqosSize-1:0] AxiQosAtomicRsp       = 4'h2;

  // ════════════════════════════════════════════════════════
  // Mesh master interface — sizes and types
  // ════════════════════════════════════════════════════════

  localparam int unsigned MeshMasterSourceSize  = BankIdSize;                  // 2 — $clog2(Banks)
  localparam int unsigned MaxL3SlavePorts       = 4;
  localparam int unsigned MaxL3SlavePortIdSize  = $clog2(MaxL3SlavePorts);     // 2

  // Mesh master request type (bank → mesh → L3/DDR)
  // Replaces: sc_mesh_master_req_t (shire_cache_types.vh SC_MESH_REQ_CMD_TYPE_DEF)
  typedef struct packed {
    logic [DvTransIdSize-1:0]             trans_id;
    logic [ReqqIdSize-1:0]               id;
    logic [MaxL3SlavePortIdSize-1:0]     port_id;
    logic [MeshMasterSourceSize-1:0]     source;
    etlink_pkg::req_opcode_e             opcode;
    logic [PaSize-1:0]                   address;
    sc_size_t                            size;
    logic [AxiAxqosSize-1:0]             qos;
    logic                                wdata;
    logic [LineQwSize-1:0]               qwen;
    logic [LineDataSize-1:0]             data;
    logic                                ben_en;
    logic [LineByteSize-1:0]             ben;
  } mesh_master_req_t;

  // Mesh master response type (L3/DDR → mesh → bank)
  // Replaces: sc_mesh_master_rsp_t (shire_cache_types.vh SC_MESH_RSP_TYPE_DEF)
  typedef struct packed {
    logic [DvTransIdSize-1:0]             trans_id;
    logic [ReqqIdSize-1:0]               id;
    logic [MaxL3SlavePortIdSize-1:0]     port_id;
    logic [MeshMasterSourceSize-1:0]     source;
    logic                                err;
    logic                                wdata;
    logic [LineDataSize-1:0]             data;
  } mesh_master_rsp_t;

  // Mesh slave interface — sizes and types
  // Used by L3 slave ports and mesh_slave AXI bridge
  localparam int unsigned MeshSourceBridgeIdSize = 10;
  localparam int unsigned MeshMasterAxiIdSize    = 9;
  localparam int unsigned MeshSlaveAxiIdSize     = MeshSourceBridgeIdSize + MeshMasterAxiIdSize; // 19

  // Mesh slave request type (L3 master → mesh → bank)
  // Replaces: sc_mesh_slave_req_t (SC_MESH_REQ_CMD_TYPE_DEF with AXI_ID_SIZE, BRIDGE_ID_SIZE)
  typedef struct packed {
    logic [DvTransIdSize-1:0]             trans_id;
    logic [MeshMasterAxiIdSize-1:0]      id;
    logic [MaxL3SlavePortIdSize-1:0]     port_id;
    logic [MeshSourceBridgeIdSize-1:0]   source;
    etlink_pkg::req_opcode_e             opcode;
    logic [PaSize-1:0]                   address;
    sc_size_t                            size;
    logic [AxiAxqosSize-1:0]             qos;
    logic                                wdata;
    logic [LineQwSize-1:0]               qwen;
    logic [LineDataSize-1:0]             data;
    logic                                ben_en;
    logic [LineByteSize-1:0]             ben;
  } mesh_slave_req_t;

  // Mesh slave response type (bank → mesh → L3 master)
  // Replaces: sc_mesh_slave_rsp_t
  typedef struct packed {
    logic [DvTransIdSize-1:0]             trans_id;
    logic [MeshMasterAxiIdSize-1:0]      id;
    logic [MaxL3SlavePortIdSize-1:0]     port_id;
    logic [MeshSourceBridgeIdSize-1:0]   source;
    logic                                err;
    logic                                wdata;
    logic [LineDataSize-1:0]             data;
  } mesh_slave_rsp_t;

  // Extended mesh slave response with reqq_id (for internal routing)
  // Replaces: sc_mesh_slave_rsp_long_t
  typedef struct packed {
    logic [ReqqIdSize-1:0]               reqq_id;
    mesh_slave_rsp_t                     rsp;
  } mesh_slave_rsp_long_t;

  // ════════════════════════════════════════════════════════
  // L3 atomic response address computation constants
  // ════════════════════════════════════════════════════════

  // Shire ID field within data[] for L3/SCP atomic responses
  localparam int unsigned L3AtomicShireIdSize     = 8;
  localparam int unsigned L3AtomicShireIdLsb      = AtomicAddrOffsetLsb + AtomicAddrOffsetSize; // 269
  // Address lower field (cache-line offset) within data[]
  localparam int unsigned L3AtomicAddrLowerLsb    = AtomicAddrOffsetLsb;                        // 263
  localparam int unsigned L3AtomicAddrLowerSize   = AtomicAddrOffsetSize;                       // 6
  // ESR write address for atomic response routing
  localparam int unsigned L3AtomicRspShireIdShift = 22;
  localparam logic [PaSize-1:0] L3AtomicRspAddrOffset = 40'h01803E0000;

  // Transaction ID field within data[] for L3 atomic responses (follows shire_id)
  localparam int unsigned L3AtomicTransIdSize    = 5;
  localparam int unsigned L3AtomicTransIdLsb     = L3AtomicShireIdLsb + L3AtomicShireIdSize; // 277

  // CDC synchronizer stages (technology-independent default)
  localparam int unsigned CdcSyncStages          = 2;

  // Line-select address range: PA[PaSize-1 : LineOffsetIdSize] (bits 39:6)
  // Used to extract the cache-line address (strip offset bits)
  localparam int unsigned LineSelAddrLsb          = LineOffsetIdSize;
  localparam int unsigned LineSelAddrMsb          = PaSize - 1;
  localparam int unsigned LineSelAddrSize         = PaSize - LineOffsetIdSize;

  // Shire config (from ESR — controls shire_id, neighborhood/tbox enables)
  typedef struct packed {
    logic [1:0]  tbox3_id;
    logic [1:0]  tbox2_id;
    logic [1:0]  tbox1_id;
    logic [1:0]  tbox0_id;
    logic        rbox_en;
    logic [3:0]  tbox_en;
    logic [3:0]  neigh_en;
    logic        cache_en;
    logic [ScpShireIdSize-1:0] shire_id;
  } esr_shire_config_t;

  // ════════════════════════════════════════════════════════
  // Request queue — reqq_entry types
  // ════════════════════════════════════════════════════════

  localparam int unsigned NeighCoopMaskSize = 4;  // NUM_NEIGH (neighborhoods per shire)

  // Per-entry state exported to reqq for arbitration and assertions.
  // Replaces: sc_reqq_entry_state_t (shire_cache_types.vh)
  typedef struct packed {
    logic [DvTransIdSize-1:0]          trans_id;
    logic                              valid;
    logic [ReqqIdSize-1:0]             reqq_id;
    logic [ReqqIdSize-1:0]             dataq_id;
    logic [ReqqTagIdSize-1:0]          tag_id;
    logic [ReqqSourceSize-1:0]         source;
    logic [PortIdSize-1:0]             port_id;
    logic [AxiAxqosSize-1:0]           qos;
    reqq_orig_opcode_e                 orig_opcode;
    address_space_e                    address_space;
    reqq_source_e                      reqq_source;
    cache_level_e                      cache_level_start;
    cache_level_e                      cache_level_dest;
    logic                              scp_is_remote;
    logic [NeighCoopMaskSize-1:0]      readcoop_dest;
    logic [PaSize-1:0]                 address;
    logic [LineQwSize-1:0]             qwen;
    logic [LineQwSize-1:0]             rspmux_qwen;
    sc_size_t                          size;
    logic                              zero_data;
    // State
    reqq_opcode_e                      opcode;
    // Responses
    logic                              rspmux_l2_eligible;
    logic                              rspmux_l3_eligible;
    logic                              rspmux_sent;
    // Eligibility
    logic                              pipe_req_eligible;
    logic [SubBankIdSize-1:0]          subbank;
    logic                              to_l3_mesh_req_eligible;
    logic                              to_sys_mesh_req_eligible;
    mesh_dest_e                        which_mesh;
    logic                              opcode_destined_for_mesh;
    logic                              rbuf_req_eligible;
    logic                              inflight;
    logic                              data_ready;
    logic                              wait_for_dataq;
    // Linked list tracking
    logic                              dep_valid;
    logic [ReqqIdSize-1:0]             dep_id;
    logic                              dep_tail;
    logic                              dep_meshll_tail;
    logic                              dep_non_meshll_head;
    logic                              dep_in_meshll;
    logic                              dont_depend_on_me;
    // Read buffer state tracking
    logic                              rbuf_valid;
    logic                              rbuf_pending_valid;
    logic [RbufIdSize-1:0]             rbuf_id;
    // Paired entry tracking
    logic [ReqqIdSize-1:0]             paired_id;
    // Error
    logic                              err_detected;
    logic                              err_zero_data;
    logic                              err_imprecise;
    // Trace
    logic                              traced;
  } reqq_entry_state_t;

  // Allocation info struct passed to each entry at alloc time.
  // Replaces: sc_reqq_alloc_t (shire_cache_types.vh)
  typedef struct packed {
    logic [ReqqIdSize-1:0]             reqq_id;
    logic [DvTransIdSize-1:0]          trans_id;
    logic [ReqqTagIdSize-1:0]          tag_id;
    logic [ReqqSourceSize-1:0]         source;
    logic [PortIdSize-1:0]             port_id;
    logic [AxiAxqosSize-1:0]           qos;
    address_space_e                    address_space;
    reqq_source_e                      reqq_source;
    cache_level_e                      cache_level_start;
    cache_level_e                      cache_level_dest;
    logic                              scp_is_remote;
    logic [NeighCoopMaskSize-1:0]      readcoop_dest;
    logic [PaSize-1:0]                 address;
    logic [LineQwSize-1:0]             qwen;
    sc_size_t                          size;
    reqq_orig_opcode_e                 orig_opcode;
    reqq_opcode_e                      opcode;
    logic                              zero_data;
    logic                              scp_fill_source_is_scp;
    logic                              dep_valid;
    logic                              dep_non_meshll_head;
    logic [ReqqIdSize-1:0]             dep_id;
    logic [ReqqIdSize-1:0]             paired_id;
    logic                              rbuf_valid;
    logic                              rbuf_pending_valid;
    logic [RbufIdSize-1:0]             rbuf_id;
    logic                              err_valid;
    err_rsp_e                          err_type;
    logic                              err_imprecise;
    logic                              traced;
  } reqq_alloc_t;

  // ════════════════════════════════════════════════════════
  // MBIST constants and icache BIST types
  // Replaces: SC_MBIST_* and sc_icache_bist_* (shire_cache_defines/types.vh)
  // ════════════════════════════════════════════════════════

  localparam int unsigned MbistLogicalMems  = 4;
  localparam int unsigned MbistSelSize      = $clog2(MbistLogicalMems + 1); // 3
  localparam int unsigned MbistAddrSize     = MbdAddrSize;                  // 15
  localparam int unsigned MbistDataSize     = MbdDataSize;                  // 144

  localparam int unsigned NumNeighLog       = $clog2(NumNeigh);             // 2
  localparam int unsigned MbiAddrSize       =
      icache_geom_pkg::IcacheMemAdWidth + NumNeighLog + $clog2(icache_geom_pkg::IcacheNrMems); // 13
  localparam int unsigned MbiDataSize       = icache_geom_pkg::IcacheMemWidth;   // 144

  // Per-RAM BIST delay/depth constants
  localparam int unsigned MbsRamDelay       = RamDelayDefault;
  localparam int unsigned MbtRamDelay       = RamDelayDefault;
  localparam int unsigned MbdRamDelay       = RamDelayDefault;
  localparam int unsigned MbiRamDelay       = 2;
  localparam int unsigned MbsLogicalDepth   = SetsPerSubBank * SubBanks;
  localparam int unsigned MbtLogicalDepth   = SetsPerSubBank * SubBanks;
  localparam int unsigned MbdLogicalDepth   = SetsPerSubBank * SubBanks * Ways * LineQwSize;
  localparam int unsigned MbiLogicalDepth   =
      icache_geom_pkg::IcacheMemWords * NumNeigh * icache_geom_pkg::IcacheNrMems;

  typedef struct packed {
    logic                       mbist_on;
    logic                       mbi_sel;
    logic                       mbi_rd_en;
    logic                       mbi_wr_en;
    logic [MbiAddrSize-1:0]     mbi_addr;
    logic [MbiDataSize-1:0]     mbi_wdata;
  } icache_bist_req_t;

  typedef struct packed {
    logic [MbiDataSize-1:0]     mbi_rdata;
  } icache_bist_rsp_t;

  // ════════════════════════════════════════════════════════
  // Performance monitor counter overflow + ESR status types
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    logic                       p1_cntr_overflow;
    logic                       p0_cntr_overflow;
    logic                       cyc_cntr_overflow;
  } perfmon_cntr_overflow_t;

  typedef struct packed {
    logic [EsrStatusSize-1:0]   ctl_status;
    logic [EsrStatusSize-1:0]   cyc_cntr;
    logic [EsrStatusSize-1:0]   p0_cntr;
    logic [EsrStatusSize-1:0]   p1_cntr;
    logic [EsrStatusSize-1:0]   p0_qual;
    logic [EsrStatusSize-1:0]   p1_qual;
  } perfmon_esr_status_t;

  // ════════════════════════════════════════════════════════
  // Error logger ESR status
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    logic [EsrStatusSize-1:0]   info;
    logic [EsrStatusSize-1:0]   address;
    logic [EsrStatusSize-1:0]   reqq_debug3;
    logic [EsrStatusSize-1:0]   reqq_debug2;
    logic [EsrStatusSize-1:0]   reqq_debug1;
    logic [EsrStatusSize-1:0]   reqq_debug0;
  } err_log_esr_status_t;

  // ════════════════════════════════════════════════════════
  // Bank ESR status (composite — all bank sub-module status)
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    idx_cop_sm_esr_status_t         pipe_idx_cop_sm;
    err_log_esr_status_t            err_log;
    logic [EsrStatusSize-1:0]       sbe_dbe_counts;
    perfmon_esr_status_t            perfmon;
  } bank_esr_status_t;

  // ════════════════════════════════════════════════════════
  // Error logger code and RAM enums
  // Replaces: sc_err_log_code_t, sc_err_log_ram_t (shire_cache_types.vh)
  // ════════════════════════════════════════════════════════

  // Error code — indexes into esr_sc_err_interrupt_enable[ErrIntSize-1:0].
  // Values match the SC_ERR_LOG_* defines from shire_cache_defines.vh.
  typedef enum logic [3:0] {
    ErrLogEccSbe      = 4'h0,
    ErrLogEccDbe      = 4'h1,
    ErrLogEccSat      = 4'h2,
    ErrLogDecode      = 4'h3,
    ErrLogPerfmonSat  = 4'h4
  } err_log_code_e;

  // Which RAM had the ECC error (replaces SC_ERR_LOG_RAM_* values).
  typedef enum logic [3:0] {
    ErrLogRamTagState = 4'h0,
    ErrLogRamTag      = 4'h1,
    ErrLogRamData     = 4'h2,
    ErrLogRamDataq    = 4'h3,
    ErrLogRamBen      = 4'h4
  } err_log_ram_e;

  // ════════════════════════════════════════════════════════
  // Trace types
  // Replaces: sc_trace_*_t, sc_trace_segment_t, sc_trace_packet_t
  //           (shire_cache_types.vh) and SC_TRACE_EN_* defines
  //           (shire_cache_defines.vh)
  // ════════════════════════════════════════════════════════

  // Trace enable bit positions — index into
  // bank_esr_ctl_t.esr_sc_trace_type_hot_enable[TraceEnSize-1:0].
  // Replaces SC_TRACE_EN_* defines from shire_cache_defines.vh.
  localparam int unsigned TraceEnAllocL2  = 0;
  localparam int unsigned TraceEnAllocL3  = 1;
  localparam int unsigned TraceEnTc       = 2;
  localparam int unsigned TraceEnTcReqqId = 3;
  localparam int unsigned TraceEnTcVictim = 4;
  localparam int unsigned TraceEnRbuf     = 5;
  localparam int unsigned TraceEnMesh     = 6;
  localparam int unsigned TraceEnReqq     = 7;
  localparam int unsigned TraceEnFill     = 11;

  // Trace segment size — all snippet types are padded to this width.
  // Derived from the widest snippet (trace_alloc): trace_type(3) +
  // reqq_id(ReqqIdSize) + orig_opcode(5) + port_id(PortIdSize) +
  // address(PaSize-LineOffsetIdSize) + qwen(LineQwSize).
  localparam int unsigned TraceSegmentSize = 3 + ReqqIdSize + 5
                                           + PortIdSize
                                           + (PaSize - LineOffsetIdSize)
                                           + LineQwSize;

  // Individual trace snippet types — each padded to TraceSegmentSize bits.
  // These mirror the sc_trace_*_t structs from shire_cache_types.vh.
  // Fields are listed MSB-first (packed struct convention).

  // Trace alloc snippet (replaces sc_trace_alloc_t).
  typedef struct packed {
    logic [LineQwSize-1:0]                   qwen;
    logic [PaSize-LineOffsetIdSize-1:0]      address;
    logic [PortIdSize-1:0]                   port_id;
    reqq_orig_opcode_e                       orig_opcode;
    logic [ReqqIdSize-1:0]                   reqq_id;
    trace_type_e                             trace_type;
  } trace_alloc_t;

  // Trace tag-check snippet (replaces sc_trace_tc_t).
  typedef struct packed {
    logic [TraceSegmentSize
           - (PaSize - LineOffsetIdSize)
           - LineQwSize
           - WayIdSize
           - 2                              // victim + hit
           - ReqqIdSize
           - 3                              // trace_type_e width
           - 1:0]                            padding;
    logic [PaSize-LineOffsetIdSize-1:0]      address;
    logic [LineQwSize-1:0]                   qwen;
    logic [WayIdSize-1:0]                    way;
    logic                                    victim;
    logic                                    hit;
    logic [ReqqIdSize-1:0]                   reqq_id;
    trace_type_e                             trace_type;
  } trace_tc_t;

  // Trace rbuf snippet (replaces sc_trace_rbuf_t).
  typedef struct packed {
    logic [TraceSegmentSize
           - (PaSize - LineOffsetIdSize)
           - ReqqIdSize
           - 3                              // trace_type_e width
           - 1:0]                            padding;
    logic [PaSize-LineOffsetIdSize-1:0]      address;
    logic [ReqqIdSize-1:0]                   reqq_id;
    trace_type_e                             trace_type;
  } trace_rbuf_t;

  // Trace mesh snippet (replaces sc_trace_mesh_t).
  typedef struct packed {
    logic [TraceSegmentSize
           - (6 * ReqqIdSize)
           - 6                              // 6 valid bits
           - 3                              // trace_type_e width
           - 1:0]                            padding;
    logic [ReqqIdSize-1:0]                   to_sys_rsp_reqq_id;
    logic                                    to_sys_rsp_valid;
    logic [ReqqIdSize-1:0]                   to_sys_req_reqq_id;
    logic                                    to_sys_req_valid;
    logic [ReqqIdSize-1:0]                   to_l3_rsp_reqq_id;
    logic                                    to_l3_rsp_valid;
    logic [ReqqIdSize-1:0]                   to_l3_req_reqq_id;
    logic                                    to_l3_req_valid;
    logic [ReqqIdSize-1:0]                   l3_slave_rsp_reqq_id;
    logic                                    l3_slave_rsp_valid;
    logic [ReqqIdSize-1:0]                   l2_neigh_rsp_reqq_id;
    logic                                    l2_neigh_rsp_valid;
    trace_type_e                             trace_type;
  } trace_mesh_t;

  // Trace reqq-state snippet (replaces sc_trace_reqq_t).
  typedef struct packed {
    logic [TraceSegmentSize
           - ReqqIdSize
           - 6                              // reqq_opcode_e width
           - 18                             // state bits below
           - 3                              // trace_type_e width
           - 1:0]                            padding;
    logic [ReqqIdSize-1:0]                   reqq_id;
    reqq_opcode_e                            opcode;
    logic                                    rspmux_l2_eligible;
    logic                                    rspmux_l3_eligible;
    logic                                    rspmux_sent;
    logic                                    pipe_req_eligible;
    logic                                    to_l3_mesh_req_eligible;
    logic                                    to_sys_mesh_req_eligible;
    logic                                    rbuf_req_eligible;
    logic                                    inflight;
    logic                                    data_ready;
    logic                                    wait_for_dataq;
    logic                                    dep_valid;
    logic                                    dep_tail;
    logic                                    dep_meshll_tail;
    logic                                    dep_non_meshll_head;
    logic                                    dont_depend_on_me;
    logic                                    rbuf_valid;
    logic                                    rbuf_pending_valid;
    logic                                    err_detected;
    trace_type_e                             trace_type;
  } trace_reqq_t;

  // Trace segment — union of all snippet types (all same width).
  // Replaces sc_trace_segment_t.
  typedef union packed {
    trace_alloc_t     l2_alloc;
    trace_alloc_t     l3_alloc;
    trace_tc_t        tc;
    trace_rbuf_t      rbuf;
    trace_mesh_t      mesh;
    trace_reqq_t      reqq;
  } trace_segment_t;

  // Trace packet — three segments.  Replaces sc_trace_packet_t.
  typedef struct packed {
    trace_segment_t   segment2;
    trace_segment_t   segment1;
    trace_segment_t   segment0;
  } trace_packet_t;

  // ════════════════════════════════════════════════════════
  // L2 hardware prefetcher request
  // Replaces: sc_neigh_l2hpf_req_t (shire_cache_types.vh)
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    logic [etlink_pkg::SourceSize-1:0]  source;
    logic [etlink_pkg::IdSize-1:0]      id;
    logic [PaSize-1:0]                  address;
    etlink_pkg::req_opcode_e            opcode;
    logic                               hit;
  } neigh_l2hpf_req_t;

  /* verilator lint_on VARHIDDEN */
  /* verilator lint_on UNUSEDPARAM */

endpackage
