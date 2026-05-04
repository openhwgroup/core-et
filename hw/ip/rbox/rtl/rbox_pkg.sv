// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// RBOX (Render Box) package.
//
// Defines constants, types, and utility functions for the RBOX graphics
// rasterization and depth-testing unit. Converted from the CORE-ET
// rbox_defines.vh and rbox_types.vh header files into a clean
// SystemVerilog package following lowRISC conventions.
//
// Differences from original:
//   - All `defines converted to localparam
//   - All typedefs in package scope (no `include guards)
//   - `ZX/`SX macros replaced with standard SV casts
//   - UltraSoC status monitor types retained as stub interface
//   - Debug run-control types defined here (were in debug_types.vh)
//   - Uses etlink_pkg:: for cache interface types

package rbox_pkg;

  /* verilator lint_off UNUSEDPARAM */  // Package constants defined for rbox sub-modules not yet implemented
  /* verilator lint_off UNUSEDSIGNAL */ // Packed structs passed to functions contain reserved fields

  import etlink_pkg::PaSize;

  // ════════════════════════════════════════════════════════════
  // System-wide constants (from shire_defines.vh, et_link_defines.vh)
  // ════════════════════════════════════════════════════════════

  localparam int unsigned NumNeigh       = 4;
  localparam int unsigned MinPerNeigh    = 8;
  localparam int unsigned CoreNrThreads  = 2;
  localparam int unsigned ThreadsPerS    = NumNeigh * MinPerNeigh * CoreNrThreads; // 64

  localparam int unsigned ScBanks        = 4;   // Shire cache banks

  localparam int unsigned NumShireIds    = 256;
  localparam int unsigned NumShireIdsL   = $clog2(NumShireIds); // 8
  localparam int unsigned ShireIdSize    = NumShireIdsL;

  localparam int unsigned CoreNrMsgPorts  = 4;
  localparam int unsigned CoreNrMsgPortsL = $clog2(CoreNrMsgPorts); // 2

  // ESR APB widths
  localparam int unsigned EsrRboxApbAddrWidth = 16;
  localparam int unsigned EsrApbDataWidth     = 64;

  // ESR address-space constants (from shire_defines.vh)
  localparam int unsigned AdEsrPortMsgSize    = 12;
  localparam int unsigned AdEsrMsgIdStart     = 3;
  localparam int unsigned AdEsrMsgIdSize      = 8;
  localparam int unsigned EtMsgPortIdStart    = 3;
  localparam int unsigned AdEsrPortIdStart    = AdEsrMsgIdStart + EtMsgPortIdStart; // 6
  localparam int unsigned AdEsrAgentTypeSize  = 2;
  localparam int unsigned AdEsrProtBitsSize   = 2;

  localparam logic [AdEsrPortMsgSize-1:0]  AdEsrPortMsgCommon    = 12'h800;
  localparam logic [AdEsrAgentTypeSize-1:0] AdEsrAgentTypeMinion = 2'd0;
  localparam logic [AdEsrProtBitsSize-1:0]  AdEsrProtBitsUmode   = 2'd0;
  localparam logic [ShireIdSize-1:0]        LocalShireId         = 8'hFF;

  // ════════════════════════════════════════════════════════════
  // MSAA constants
  // ════════════════════════════════════════════════════════════

  localparam int unsigned MaxMsaaSamples   = 16;
  localparam int unsigned MsaaSampleIdxSz  = $clog2(MaxMsaaSamples); // 4
  localparam int unsigned MsaaSampPosBits  = 4;

  // MSAA mode row/quad sample counts
  localparam int unsigned MsaaMode2xRowSample   = 1;
  localparam int unsigned MsaaMode2xQuadSample  = 1;
  localparam int unsigned MsaaMode4xRowSample   = 3;
  localparam int unsigned MsaaMode4xQuadSample  = 1;
  localparam int unsigned MsaaMode8xRowSample   = 6;
  localparam int unsigned MsaaMode8xQuadSample  = 8;
  localparam int unsigned MsaaMode16xRowSample  = 13;
  localparam int unsigned MsaaMode16xQuadSample = 16;

  // ════════════════════════════════════════════════════════════
  // Dimension and tile constants
  // ════════════════════════════════════════════════════════════

  localparam int unsigned MaxRenderDim  = 32768;
  localparam int unsigned MaxRenderDimL = $clog2(MaxRenderDim); // 15

  localparam int unsigned MaxMsaaDim    = MaxRenderDim * 4;     // 131072
  localparam int unsigned MaxMsaaDimL   = $clog2(MaxMsaaDim);   // 17

  localparam int unsigned MaxTileDim    = 64;
  localparam int unsigned MaxTileDimL   = $clog2(MaxTileDim);   // 6

  // ════════════════════════════════════════════════════════════
  // Edge and depth coefficient bit widths
  // ════════════════════════════════════════════════════════════

  localparam int unsigned DepthCoefBits = 32;
  localparam int unsigned DepthSampBits = 32;

  localparam int unsigned EdgeSampIntBits = 18;
  localparam int unsigned EdgeSampFrcBits = 25;
  localparam int unsigned EdgeSampBits    = EdgeSampIntBits + EdgeSampFrcBits; // 43

  localparam int unsigned EdgeCoefIntSz = 9;
  localparam int unsigned EdgeCoefFrcSz = 25;
  localparam int unsigned EdgeCoefSz    = EdgeCoefIntSz + EdgeCoefFrcSz; // 34

  localparam int unsigned StepIntSz  = 2;
  localparam int unsigned StepFrcSz  = MsaaSampPosBits; // 4
  localparam int unsigned StepSz     = StepIntSz + StepFrcSz; // 6

  // Large triangle edge coefficients
  localparam int unsigned LtEdgeCoefIntBits  = 9;
  localparam int unsigned LtEdgeCoefFrcBits  = 25;
  localparam int unsigned LtEdgeCoefBits     = LtEdgeCoefIntBits + LtEdgeCoefFrcBits; // 34
  localparam int unsigned LtEdgeSampIntBits  = 15;
  localparam int unsigned LtEdgeSampFrcBits  = 25;
  localparam int unsigned LtEdgeSampBits     = LtEdgeSampIntBits + LtEdgeSampFrcBits; // 40
  localparam int unsigned LtEdgeCoefLoBits   = 32;
  localparam int unsigned LtEdgeCoefHiBits   = LtEdgeCoefBits - LtEdgeCoefLoBits; // 2

  // 128x128 tile edge coefficients
  localparam int unsigned T128EdgeCoefIntBits = 9;
  localparam int unsigned T128EdgeCoefFrcBits = 15;
  localparam int unsigned T128EdgeCoefBits    = T128EdgeCoefIntBits + T128EdgeCoefFrcBits; // 24
  localparam int unsigned T128EdgeSampIntBits = 15;
  localparam int unsigned T128EdgeSampFrcBits = 15;
  localparam int unsigned T128EdgeSampBits    = T128EdgeSampIntBits + T128EdgeSampFrcBits; // 30

  // 64x64 tile edge coefficients
  localparam int unsigned T64EdgeCoefIntBits = 9;
  localparam int unsigned T64EdgeCoefFrcBits = 14;
  localparam int unsigned T64EdgeCoefBits    = T64EdgeCoefIntBits + T64EdgeCoefFrcBits; // 23
  localparam int unsigned T64EdgeSampIntBits = 15;
  localparam int unsigned T64EdgeSampFrcBits = 14;
  localparam int unsigned T64EdgeSampBits    = T64EdgeSampIntBits + T64EdgeSampFrcBits; // 29

  // Padding for coefficient packing in structs (32-bit aligned)
  localparam int unsigned LtEdgeCoefPadSz   = 40 - LtEdgeCoefBits - 1;    // 5
  localparam int unsigned T128EdgeCoefPadSz = 32 - T128EdgeCoefBits - 1;   // 7
  localparam int unsigned T64EdgeCoefPadSz  = 32 - T64EdgeCoefBits - 1;    // 8

  // Padding for sample packing in structs
  localparam int unsigned LtEdgeSampPadSz   = 64 - LtEdgeSampBits - 1;    // 23
  localparam int unsigned T128EdgeSampPadSz = 32 - T128EdgeSampBits - 1;   // 1
  localparam int unsigned T64EdgeSampPadSz  = 32 - T64EdgeSampBits - 1;    // 2

  // ════════════════════════════════════════════════════════════
  // Buffer sizes
  // ════════════════════════════════════════════════════════════

  localparam int unsigned InpckDataSz    = 512;

  localparam int unsigned InFifoSz       = 32;
  localparam int unsigned InFifoAddrSz   = $clog2(InFifoSz);      // 5
  localparam int unsigned InFifoPckts    = InFifoSz * 2;           // 64
  localparam int unsigned InFifoPcktSz   = $clog2(InFifoPckts);   // 6

  localparam int unsigned FifoPtrSz      = InFifoSz;
  localparam int unsigned FifoPtrAddrSz  = $clog2(FifoPtrSz);     // 5

  localparam int unsigned OutBufSz       = 128;

  localparam int unsigned OutBufBaryDataSz  = 256;
  localparam int unsigned OutBufDepthDataSz = 128;

  // ════════════════════════════════════════════════════════════
  // Depth buffer constants
  // ════════════════════════════════════════════════════════════

  localparam int unsigned DepthMantIntSz     = 1;
  localparam int unsigned DepthMantFrcSz     = 23;
  localparam int unsigned DepthMantSz        = DepthMantIntSz + DepthMantFrcSz; // 24
  localparam int unsigned DepthExpSz         = 8;
  localparam int unsigned DepthMantShiftSz   = $clog2(DepthMantSz) + 1; // 6

  localparam int unsigned DepthBufSz         = 64;
  localparam int unsigned DepthBufIdxSz      = $clog2(DepthBufSz); // 6

  localparam int unsigned DepthLineSz        = 512;
  localparam int unsigned DepthLineQw        = DepthLineSz / 64;   // 8
  localparam int unsigned DepthLineBytes     = DepthLineSz / 8;    // 64
  localparam int unsigned DepthLineQwIdxSz   = $clog2(DepthLineQw); // 3

  localparam int unsigned DepthBufTagSz      = PaSize - 1 - $clog2(DepthLineSz) - 2;
  // Original: PA_MSB - $clog2(DEPTH_LINE_SZ) - 2 → 39 - 9 - 2 = 28

  localparam int unsigned DepthLfifoSz       = 128;
  localparam int unsigned DepthQuadDataSz    = 128;

  // ════════════════════════════════════════════════════════════
  // Hart and output buffer constants
  // ════════════════════════════════════════════════════════════

  localparam int unsigned HartsPerShire   = ThreadsPerS;       // 64
  localparam int unsigned HartsL          = $clog2(HartsPerShire); // 6

  localparam int unsigned MaxHartMsgs     = 4;
  localparam int unsigned MaxHartMsgL     = $clog2(MaxHartMsgs);   // 2

  localparam int unsigned HartBufSz       = 128;
  localparam int unsigned HartBufOffSz    = $clog2(HartBufSz);    // 7

  localparam int unsigned MaxPcktMsg      = 32;
  localparam int unsigned MaxPcktMsgL     = $clog2(MaxPcktMsg);    // 5
  localparam int unsigned MaxPcktMsgLL    = $clog2(MaxPcktMsgL);   // 3 (technically ceil(log2(5))=3)

  localparam int unsigned MaxQuadPcks     = 5;
  localparam int unsigned MaxQuadPcksL    = $clog2(MaxQuadPcks);   // 3

  localparam int unsigned QuadBufSz       = 128;

  // ESR register field derived widths
  localparam int unsigned InBufOffSz      = 22;
  localparam int unsigned InBufLineSz     = InBufOffSz - 6;       // 16
  localparam int unsigned OutBufOffSz     = 21;
  localparam int unsigned OutBufLineSz    = OutBufOffSz - 5;      // 16

  // 2M page physical address
  localparam int unsigned Pg2mPaOffSz     = 21;
  localparam int unsigned Pg2mPaSz        = PaSize - Pg2mPaOffSz; // 19

  // Consume register derived width
  localparam int unsigned EsrConsumeBitsUsed = HartBufOffSz + MaxHartMsgL; // 9

  // Output buffer config derived width
  localparam int unsigned EsrOutBufCfgBitsUsed = HartBufOffSz + CoreNrMsgPortsL +
                                                  MaxHartMsgL + MaxPcktMsgLL;

  // Hart message credit counter width (includes +1 for overflow)
  localparam int unsigned HartPcktCreditSz = HartBufOffSz + 1;   // 8

  // ════════════════════════════════════════════════════════════
  // Debug RAM constants
  // ════════════════════════════════════════════════════════════

  localparam int unsigned DbgPtrSz          = 128;
  localparam int unsigned DbgPtrIdxSz       = $clog2(DbgPtrSz); // 7
  localparam int unsigned DbgMaxData        = 512;
  localparam int unsigned DbgMaxDataQw      = DbgMaxData / 64;   // 8
  localparam int unsigned DbgMaxDataQwIdxSz = $clog2(DbgMaxDataQw); // 3

  localparam int unsigned DbgDbMetaDataSz   = DepthBufTagSz + $clog2(QuadBufSz) + 3;

  // ════════════════════════════════════════════════════════════
  // UltraSoC status monitor widths (stub interface — third-party IP dropped)
  // ════════════════════════════════════════════════════════════

  localparam int unsigned SmGpioWidth      = 17;
  localparam int unsigned SmFilterWidth    = 100;
  localparam int unsigned SmMatchWidth     = 30;
  localparam int unsigned SmDataTraceWidth = 126;

  // ════════════════════════════════════════════════════════════
  // ESR reset values
  // ════════════════════════════════════════════════════════════

  localparam logic [63:0] EsrConfigReset    = 64'h0;
  localparam logic [63:0] EsrInBufPgReset   = 64'h0;
  localparam logic [63:0] EsrInBufCfgReset  = 64'h0;
  localparam logic [31:0] EsrOutBufPgReset  = 32'h0;
  localparam logic [63:0] EsrOutBufCfgReset = 64'h0;
  localparam logic [31:0] EsrStartReset     = 32'h0;
  localparam logic [31:0] EsrStatusReset    = 32'h0;
  localparam logic [31:0] EsrConsumeReset   = 32'h0;

  // ════════════════════════════════════════════════════════════
  // Enumerations
  // ════════════════════════════════════════════════════════════

  // Input packet types
  typedef enum logic [2:0] {
    InpckTriWithTile64x64   = 3'd0,
    InpckTriWithTile128x128 = 3'd1,
    InpckLargeTriangle      = 3'd2,
    InpckFullyCoveredTile   = 3'd3,
    InpckLargeTriangleTile  = 3'd4,
    InpckRboxState          = 3'd5,
    InpckFragShadingState   = 3'd6,
    InpckEndOfBuffer        = 3'd7
  } inpck_type_e;

  // Output packet types
  typedef enum logic [1:0] {
    OutpckStateInfo = 2'd0,
    OutpckQuadInfo  = 2'd1,
    OutpckEndPhase  = 2'd2
  } outpck_type_e;

  // Tile sizes
  typedef enum logic [2:0] {
    TileSize64x64 = 3'd0,
    TileSize64x32 = 3'd1,
    TileSize32x32 = 3'd2,
    TileSize16x16 = 3'd3,
    TileSize8x8   = 3'd4,
    TileSize4x4   = 3'd5
  } tile_size_e;

  // Triangle facing (culling direction)
  typedef enum logic {
    TriFacingFront = 1'b0,
    TriFacingBack  = 1'b1
  } tri_facing_e;

  // Depth-stencil formats (Vulkan VkFormat values)
  typedef enum logic [8:0] {
    DsFormatD16Unorm          = 9'd124,
    DsFormatX8D24UnormPack32  = 9'd125,
    DsFormatD32Sfloat         = 9'd126,
    DsFormatD16UnormS8Uint    = 9'd128,
    DsFormatD24UnormS8Uint    = 9'd129,
    DsFormatD32SfloatS8Uint   = 9'd130
  } ds_format_e;

  // Comparison operations (Vulkan VkCompareOp)
  typedef enum logic [2:0] {
    CompOpNever          = 3'd0,
    CompOpLess           = 3'd1,
    CompOpEqual          = 3'd2,
    CompOpLessOrEqual    = 3'd3,
    CompOpGreater        = 3'd4,
    CompOpNotEqual       = 3'd5,
    CompOpGreaterOrEqual = 3'd6,
    CompOpAlways         = 3'd7
  } comp_op_e;

  // Stencil operations (Vulkan VkStencilOp)
  typedef enum logic [2:0] {
    StencilOpKeep     = 3'd0,
    StencilOpZero     = 3'd1,
    StencilOpReplace  = 3'd2,
    StencilOpIncClamp = 3'd3,
    StencilOpDecClamp = 3'd4,
    StencilOpInvert   = 3'd5,
    StencilOpIncWrap  = 3'd6,
    StencilOpDecWrap  = 3'd7
  } stencil_op_e;

  // MSAA mode (sample count encoding)
  typedef enum logic [1:0] {
    MsaaMode2x  = 2'd0,
    MsaaMode4x  = 2'd1,
    MsaaMode8x  = 2'd2,
    MsaaMode16x = 2'd3
  } msaa_mode_e;

  // MSAA sample position (4-bit sub-pixel position)
  typedef enum logic [MsaaSampPosBits-1:0] {
    MsaaPos0000 = 4'b0000,
    MsaaPos0625 = 4'b0001,
    MsaaPos1250 = 4'b0010,
    MsaaPos1875 = 4'b0011,
    MsaaPos2500 = 4'b0100,
    MsaaPos3125 = 4'b0101,
    MsaaPos3750 = 4'b0110,
    MsaaPos4375 = 4'b0111,
    MsaaPos5000 = 4'b1000,
    MsaaPos5625 = 4'b1001,
    MsaaPos6250 = 4'b1010,
    MsaaPos6875 = 4'b1011,
    MsaaPos7500 = 4'b1100,
    MsaaPos8125 = 4'b1101,
    MsaaPos8750 = 4'b1110,
    MsaaPos9375 = 4'b1111
  } msaa_pos_e;

  // Traverse state machine (one-hot)
  typedef enum logic [3:0] {
    TraverseStart = 4'b0001,
    TraverseQuad  = 4'b0010,
    TraverseRow   = 4'b0100,
    TraverseEnd   = 4'b1000
  } traverse_state_e;

  // Traverse state index (binary encoding)
  typedef enum logic [1:0] {
    TraverseIdxStart = 2'd0,
    TraverseIdxQuad  = 2'd1,
    TraverseIdxRow   = 2'd2,
    TraverseIdxEnd   = 2'd3
  } traverse_state_idx_e;

  // RBOX status
  typedef enum logic [7:0] {
    StatusReady    = 8'h00,
    StatusWorking  = 8'h01,
    StatusFinished = 8'h02,
    StatusError    = 8'h80
  } status_e;

  // ════════════════════════════════════════════════════════════
  // Scalar type aliases
  // ════════════════════════════════════════════════════════════

  typedef logic [MsaaSampleIdxSz-1:0]   msaa_sample_idx_t;
  typedef msaa_pos_e [1:0]               msaa_sample_pos_t;
  typedef logic [MaxRenderDimL-1:2]      tile_pos_t;
  typedef logic [DepthCoefBits-1:0]      depth_coef_t;
  typedef logic [DepthSampBits-1:0]      depth_sample_t;
  typedef logic [EdgeSampBits-1:0]       edge_sample_t;
  typedef logic [EdgeCoefSz-1:0]         edge_coef_t;
  typedef logic [StepSz-1:0]            step_t;

  // ════════════════════════════════════════════════════════════
  // Structs — Edge equations
  // ════════════════════════════════════════════════════════════

  typedef struct packed {
    logic [T64EdgeCoefPadSz:0]    b_reserved;
    logic [T64EdgeCoefBits-1:0]   b_coef;       // 2's complement 9.14
    logic [T64EdgeCoefPadSz:0]    a_reserved;
    logic [T64EdgeCoefBits-1:0]   a_coef;       // 2's complement 9.14
  } edge_eq_64x64_t;

  typedef struct packed {
    logic [T128EdgeCoefPadSz:0]   b_reserved;
    logic [T128EdgeCoefBits-1:0]  b_coef;       // 2's complement 9.15
    logic [T128EdgeCoefPadSz:0]   a_reserved;
    logic [T128EdgeCoefBits-1:0]  a_coef;       // 2's complement 9.15
  } edge_eq_128x128_t;

  typedef struct packed {
    logic [LtEdgeCoefPadSz:0]     b_reserved;
    logic [LtEdgeCoefHiBits-1:0]  b_coef_hi;    // 2's complement 9.25 (hi part)
    logic [LtEdgeCoefPadSz:0]     a_reserved;
    logic [LtEdgeCoefHiBits-1:0]  a_coef_hi;
    logic [LtEdgeCoefLoBits-1:0]  b_coef_lo;
    logic [LtEdgeCoefLoBits-1:0]  a_coef_lo;
  } edge_eq_large_t;

  typedef struct packed {
    logic [31:0] b_coef;    // UNORM24 or FLOAT32
    logic [31:0] a_coef;    // UNORM24 or FLOAT32
  } depth_eq_t;

  // ════════════════════════════════════════════════════════════
  // Structs — Edge samples (packed with alignment padding)
  // ════════════════════════════════════════════════════════════

  typedef struct packed {
    logic [T64EdgeSampPadSz:0]    edge_reserved;
    logic [T64EdgeSampBits-1:0]   edge_sample;  // 2's complement 15.14
  } edge_sample_64x64_t;

  typedef struct packed {
    logic [T128EdgeSampPadSz:0]   edge_reserved;
    logic [T128EdgeSampBits-1:0]  edge_sample;  // 2's complement 15.15
  } edge_sample_128x128_t;

  typedef struct packed {
    logic [LtEdgeSampPadSz:0]     edge_reserved;
    logic [LtEdgeSampBits-1:0]    edge_sample;  // 2's complement 15.25
  } edge_sample_large_t;

  // ════════════════════════════════════════════════════════════
  // Structs — Stencil state
  // ════════════════════════════════════════════════════════════

  typedef struct packed {
    logic [7:0]  ref_value;
    logic [7:0]  write_mask;
    logic [7:0]  compare_mask;
    comp_op_e    compare_op;
    stencil_op_e depth_fail_op;
    stencil_op_e pass_op;
    stencil_op_e fail_op;
  } stencil_state_t;

  // ════════════════════════════════════════════════════════════
  // Structs — Input packets (512-bit ET-Link data payloads)
  // ════════════════════════════════════════════════════════════

  typedef struct packed {
    logic [63:0]              reserved1;
    logic [31:0]              depth_sample;
    edge_sample_64x64_t [2:0] edge_sample;
    tile_size_e               tile_size;
    tile_pos_t                tile_top;
    tile_pos_t                tile_left;
    inpck_type_e              inpck_type;
  } inpck_fully_covered_tile_t;

  typedef struct packed {
    edge_sample_large_t [2:0] edge_sample;
    logic [31:0]              depth_sample;
    tile_size_e               tile_size;
    tile_pos_t                tile_top;
    tile_pos_t                tile_left;
    inpck_type_e              inpck_type;
  } inpck_large_triangle_tile_t;

  typedef struct packed {
    logic [31:0]              depth_sample;
    edge_sample_64x64_t [2:0] edge_sample;
    logic [63:0]              triangle_data_addr;
    depth_eq_t                depth_eq;
    edge_eq_64x64_t [2:0]    edge_eq;
    logic [30:0]              reserved0;
    tile_size_e               tile_size;
    tri_facing_e              tri_facing;
    tile_pos_t                tile_top;
    tile_pos_t                tile_left;
    inpck_type_e              inpck_type;
  } inpck_tri_with_tile_64x64_t;

  typedef struct packed {
    logic [31:0]                depth_sample;
    edge_sample_128x128_t [2:0] edge_sample;
    logic [63:0]                triangle_data_addr;
    depth_eq_t                  depth_eq;
    edge_eq_128x128_t [2:0]    edge_eq;
    logic [30:0]                reserved0;
    tile_size_e                 tile_size;
    tri_facing_e                tri_facing;
    tile_pos_t                  tile_top;
    tile_pos_t                  tile_left;
    inpck_type_e                inpck_type;
  } inpck_tri_with_tile_128x128_t;

  typedef struct packed {
    logic [63:0]              triangle_data_addr;
    logic [31:0]              reserved1;
    depth_eq_t                depth_eq;
    edge_eq_large_t [2:0]    edge_eq;
    logic [59:0]              reserved0;
    tri_facing_e              tri_facing;
    inpck_type_e              inpck_type;
  } inpck_large_triangle_t;

  // Render state — packed format for bus transfer (8 QWords)
  typedef struct packed {
    // QW7
    logic [27:0]     reserved4;
    stencil_state_t  stencil_back_state;
    // QW6
    logic [27:0]     reserved3;
    stencil_state_t  stencil_front_state;
    // QW5
    logic [7:0]      reserved2;
    logic [13:0]     scissor_width;
    logic [13:0]     scissor_height;
    logic [13:0]     scissor_start_y;
    logic [13:0]     scissor_start_x;
    // QW4
    logic [31:0]     depth_max;
    logic [31:0]     depth_min;
    // QW3
    logic [31:0]     depth_bound_max;
    logic [31:0]     depth_bound_min;
    // QW2
    logic [18:0]     reserved1;
    logic [1:0]      shire_layout_height;
    logic [1:0]      shire_layout_width;
    logic [1:0]      hart_tile_height;
    logic [1:0]      hart_tile_width;
    logic            fragment_shader_per_sample;
    logic            fragment_shader_reads_coverage;
    logic            fragment_shader_reads_depth;
    logic            fragment_shader_reads_bary;
    logic            fragment_shader_disabled;
    logic            stencil_test_enable;
    logic            early_frag_tests_enable;
    comp_op_e        depth_test_compare_op;
    logic            depth_test_write_enable;
    logic            depth_test_enable;
    logic            depth_bound_enable;
    logic            depth_clamp_enable;
    logic [12:0]     ds_buffer_row_pitch;
    logic            ds_buffer_tiled;
    ds_format_e      ds_buffer_format;
    // QW1
    logic [63:0]     ds_buffer_addr;
    // QW0
    logic [31:0]     msaa_min_sample_shading;
    logic [15:0]     msaa_sample_mask;
    logic            msaa_alpha_to_one;
    logic            msaa_alpha_to_coverage;
    logic            msaa_shading_enable;
    logic [3:0]      msaa_samples;
    logic            msaa_enable;
  } state_pck_t;

  typedef struct packed {
    state_pck_t   state;
    logic [4:0]   reserved0;
    inpck_type_e  inpck_type;
  } inpck_rbox_state_t;

  typedef struct packed {
    logic [63:0]     reserved1;
    logic [63:0]     fshader_data_addr;
    logic [63:0]     fshader_function_addr;
    logic [60:0]     reserved0;
    inpck_type_e     inpck_type;
  } inpck_fsh_state_t;

  // ════════════════════════════════════════════════════════════
  // Structs — Output packets
  // ════════════════════════════════════════════════════════════

  typedef struct packed {
    logic [63:0]                                   reserved1;
    logic [63:0]                                   fshader_data_addr;
    logic [63:0]                                   fshader_function_addr;
    logic [61:0]                                   reserved0;
    outpck_type_e                                  outpck_type;
  } outpck_fsh_state_t;

  typedef struct packed {
    logic [63:0]                                   quad1_triangle_ptr;
    logic [63:0]                                   quad0_triangle_ptr;
    logic [31:0]                                   reserved6;
    logic [7:0]                                    mask;
    logic [7:$bits(msaa_sample_idx_t)]             reserved5;
    msaa_sample_idx_t                              sample_idx;
    logic [15:MaxRenderDimL]                       reserved4;
    logic [MaxRenderDimL-1:0]                      quad1_y;
    logic [15:MaxRenderDimL]                       reserved3;
    logic [MaxRenderDimL-1:0]                      quad0_y;
    logic [15:MaxRenderDimL]                       reserved2;
    logic [MaxRenderDimL-1:0]                      quad1_x;
    logic [15:MaxRenderDimL]                       reserved1;
    logic [MaxRenderDimL-1:0]                      quad0_x;
    logic [15:$bits(outpck_type_e)]                reserved0;
    outpck_type_e                                  outpck_type;
  } outpck_quad_info_t;

  typedef struct packed {
    logic [253:0]   reserved0;
    outpck_type_e   outpck_type;
  } outpck_end_phase_t;

  // ════════════════════════════════════════════════════════════
  // Structs — Internal state
  // ════════════════════════════════════════════════════════════

  // Render state — unpacked format used inside the pipeline
  typedef struct packed {
    logic            msaa_enable;
    logic [3:0]      msaa_samples;
    logic            msaa_shading_enable;
    logic            msaa_alpha_to_coverage;
    logic            msaa_alpha_to_one;
    logic [15:0]     msaa_sample_mask;
    logic [31:0]     msaa_min_sample_shading;
    logic [63:0]     ds_buffer_addr;
    ds_format_e      ds_buffer_format;
    logic            ds_buffer_tiled;
    logic [12:0]     ds_buffer_row_pitch;
    logic            depth_clamp_enable;
    logic            depth_bound_enable;
    logic            depth_test_enable;
    logic            depth_test_write_enable;
    comp_op_e        depth_test_compare_op;
    logic            early_frag_tests_enable;
    logic            stencil_test_enable;
    logic            fragment_shader_disabled;
    logic            fragment_shader_reads_bary;
    logic            fragment_shader_reads_depth;
    logic            fragment_shader_reads_coverage;
    logic            fragment_shader_per_sample;
    logic [1:0]      hart_tile_width;
    logic [1:0]      hart_tile_height;
    logic [1:0]      shire_layout_width;
    logic [1:0]      shire_layout_height;
    logic [31:0]     depth_bound_min;
    logic [31:0]     depth_bound_max;
    logic [31:0]     depth_min;
    logic [31:0]     depth_max;
    logic [13:0]     scissor_start_x;
    logic [13:0]     scissor_start_y;
    logic [13:0]     scissor_height;
    logic [13:0]     scissor_width;
    stencil_state_t  stencil_front_state;
    stencil_state_t  stencil_back_state;
  } state_t;

  // Fragment shader state
  typedef struct packed {
    logic [63:0] fshader_function_addr;
    logic [63:0] fshader_data_addr;
  } fsh_state_t;

  // Triangle state
  typedef struct packed {
    tri_facing_e        tri_facing;
    edge_coef_t [2:0]  edge_eq_a;
    edge_coef_t [2:0]  edge_eq_b;
    logic [2:0]         top_or_left_edge;
    depth_coef_t        depth_eq_a;
    depth_coef_t        depth_eq_b;
    logic [63:0]        triangle_data_addr;
  } triangle_state_t;

  // Tile state
  typedef struct packed {
    tile_size_e                  size;
    logic [MaxRenderDimL:2]      left;
    logic [MaxRenderDimL:2]      top;
    logic                        fully_covered;
    edge_sample_t [2:0]          edge_sample;
    depth_sample_t               depth_sample;
  } tile_state_t;

  // Quad data (fragment shading input)
  typedef struct packed {
    logic [MaxTileDimL-1:0]              x;
    logic [MaxTileDimL-1:0]              y;
    logic [3:0]                          mask;
    logic [3:0][31:0]                    i;
    logic [3:0][31:0]                    j;
    logic [3:0][31:0]                    depth;
    msaa_sample_idx_t                    sample_idx;
    logic [3:0][MaxMsaaSamples-1:0]      msaa_coverage;
  } quad_data_t;

  // Quad info (output metadata)
  typedef struct packed {
    logic [MaxTileDimL-1:0]  x;
    logic [MaxTileDimL-1:0]  y;
    msaa_sample_idx_t        sample_idx;
    logic [3:0]              mask;
    logic                    valid;
    logic                    test_done;
    logic [HartsL-1:0]      hart_id;
    logic                    start_pack;
  } quad_info_t;

  // Quad test info (depth testing)
  typedef struct packed {
    logic [MaxRenderDimL-1:0]    x;
    logic [MaxRenderDimL-1:0]    y;
    msaa_sample_idx_t            sample_idx;
    logic [3:0]                  mask;
    logic [3:0][31:0]            depth;
  } quad_test_t;

  // Depth buffer FIFO info
  typedef struct packed {
    logic [3:0]              mask;
    logic [DepthBufIdxSz-1:0] depth_buf_ptr;
    logic [5:0]              quad_offset;
    logic                    valid;
  } depth_lfifo_info_t;

  // Internal depth representation (FP32 decomposed)
  typedef struct packed {
    logic        infinite;
    logic        nan;
    logic        zero;
    logic        subnorm;
    logic [7:0]  e;         // Exponent (biased)
    logic        s;         // Sign
    logic [23:0] m;         // Mantissa (unsigned 1.23)
    logic [5:0]  e_delta;   // Exponent delta for renormalization
    logic [4:0]  m_shift;   // Mantissa right shift for renormalization
  } internal_depth_t;

  // ════════════════════════════════════════════════════════════
  // Structs — ESR configuration
  // ════════════════════════════════════════════════════════════

  typedef struct packed {
    logic [63:1] reserved;
    logic        enable;
  } esr_config_t;

  typedef struct packed {
    logic                      page_active1;
    logic [62:32+Pg2mPaSz]     reserved1;
    logic [PaSize-1:Pg2mPaOffSz] page1;
    logic                      page_active0;
    logic [30:Pg2mPaSz]        reserved0;
    logic [PaSize-1:Pg2mPaOffSz] page0;
  } esr_in_buf_pg_t;

  typedef struct packed {
    logic [31:InBufLineSz]     reserved1;
    logic [InBufLineSz-1:0]    line_count;
    logic [31:InBufLineSz]     reserved0;
    logic [InBufLineSz-1:0]    start_offset;
  } esr_in_buf_cfg_t;

  typedef struct packed {
    logic                      page_active;
    logic [30:Pg2mPaSz]        reserved;
    logic [PaSize-1:Pg2mPaOffSz] page;
  } esr_out_buf_pg_t;

  typedef struct packed {
    logic [31:EsrOutBufCfgBitsUsed]  reserved1;
    logic [MaxPcktMsgLL-1:0]         max_pckts_msg;
    logic [MaxHartMsgL-1:0]          hart_max_msg;
    logic [CoreNrMsgPortsL-1:0]      hart_port_id;
    logic [$clog2($clog2(HartBufSz))-1:0] hart_buf_lines;
    logic [31:OutBufLineSz]          reserved0;
    logic [OutBufLineSz-1:0]         start_offset;
  } esr_out_buf_cfg_t;

  typedef struct packed {
    logic [15:1] reserved1;
    logic        flush;
    logic [15:1] reserved0;
    logic        start;
  } esr_start_t;

  typedef struct packed {
    logic [15:HartsL]                reserved1;
    logic [HartsL-1:0]              hart_id;
    logic [15:EsrConsumeBitsUsed]    reserved0;
    logic [MaxHartMsgL-1:0]          msg_credits;
    logic [HartPcktCreditSz-1:0]     pckt_credits;
  } esr_consume_t;

  typedef struct packed {
    logic [31:$bits(status_e)]   reserved;
    status_e                     status;
  } esr_status_t;

  // ════════════════════════════════════════════════════════════
  // Structs — Status monitor control (debug observation)
  // ════════════════════════════════════════════════════════════

  typedef struct packed {
    logic                             s1_config_error;
    logic                             s1_started;
    logic                             s1_done;
    logic                             s1_sc_rbox_rsp_is_read_data;
    logic [InBufLineSz-1:0]           s1_in_pending_lines_ff;
    logic [InBufLineSz-1:0]           s1_in_process_pckts_ff;
    logic [InBufLineSz-1:0]           s1_in_buf_off;
    logic [ScBanks:0]                 s1_in_sc_req_valid_ff;
    logic [PaSize-1:0]               s1_req_pa;
    logic                             s1_in_fifo_full;
    logic                             s1_in_fifo_empty;
    logic                             s1_in_fifo_alloc;
    logic [FifoPtrAddrSz-1:0]        s1_in_fifo_alloc_ptr;
    logic [FifoPtrAddrSz-1:0]        s1_in_fifo_push_ptr;
    logic                             s1_in_fifo_push;
    logic                             s1_in_fifo_push_done;

    logic                             s2_load_rbox_state;
    logic                             s2_load_fsh_state;
    logic                             s2_load_triangle_state;
    logic                             s2_load_tile_state;
    logic                             s2_input_pck_accept;

    logic                             s3_end_of_buffer_ff;
    logic                             s3_tile_valid_ff;

    logic                             s4_out_buf_info_valid;
    logic                             s4_out_buf_packet_valid;
    quad_info_t [1:0]                 s4_out_buf_info;
    logic [HartsL-1:0]              s4_hart_id;
    logic [ScBanks:0]                 s4_out_msg_sc_req_valid_ff;
    logic [ScBanks:0]                 s4_out_pck_sc_req_valid_ff;
    logic [PaSize-1:0]               s4_out_msg_req_pa;
    logic [PaSize-1:0]               s4_out_pck_req_pa;
  } sm_ctrl_t;

  // ════════════════════════════════════════════════════════════
  // Structs — Debug run control
  // ════════════════════════════════════════════════════════════

  typedef struct packed {
    logic halt_req;
    logic resume_req;
    logic reset_req;
  } rc_gpio_t;

  typedef struct packed {
    logic halt_req;
    logic resume_req;
  } rc_event_t;

  typedef struct packed {
    rc_gpio_t  gpio;
    rc_event_t events;
    logic      single_step;
  } rc_t;

  typedef struct packed {
    logic halt_ack;
    logic resume_ack;
    logic reset_ack;
  } rc_ack_t;

  // ════════════════════════════════════════════════════════════
  // Functions
  // ════════════════════════════════════════════════════════════

  // Number of 2x2 quads in X dimension of tile
  function automatic logic [MaxTileDimL-1:0] get_quads_in_tile_x(tile_size_e tile_sz);
    unique case (tile_sz)
      TileSize64x64,
      TileSize64x32: return MaxTileDimL'(31);
      TileSize32x32: return MaxTileDimL'(15);
      TileSize16x16: return MaxTileDimL'(7);
      TileSize8x8:   return MaxTileDimL'(3);
      TileSize4x4:   return MaxTileDimL'(1);
      default:       return MaxTileDimL'(0);
    endcase
  endfunction

  // Number of 2x2 quads in Y dimension of tile
  function automatic logic [MaxTileDimL-1:0] get_quads_in_tile_y(tile_size_e tile_sz);
    unique case (tile_sz)
      TileSize64x64: return MaxTileDimL'(31);
      TileSize64x32,
      TileSize32x32: return MaxTileDimL'(15);
      TileSize16x16: return MaxTileDimL'(7);
      TileSize8x8:   return MaxTileDimL'(3);
      TileSize4x4:   return MaxTileDimL'(1);
      default:       return MaxTileDimL'(0);
    endcase
  endfunction

  // Tile position to quad offset within tile
  function automatic logic [MaxTileDimL-1:0] get_tile_quad_offset(logic [13:2] tile_pos);
    return {tile_pos[2 +: (MaxTileDimL-2)], 2'b0};
  endfunction

  // Validate MSAA mode field
  function automatic logic msaa_mode_is_valid(logic [3:0] msaa_mode);
    unique case (msaa_mode)
      4'd1, 4'd3, 4'd7, 4'd15: return 1'b1;
      default:                  return 1'b0;
    endcase
  endfunction

  // Convert MSAA sample count field to mode enum
  function automatic msaa_mode_e convert_msaa_mode(logic [3:0] msaa_mode);
    unique case (msaa_mode)
      4'd1:    return MsaaMode2x;
      4'd3:    return MsaaMode4x;
      4'd7:    return MsaaMode8x;
      4'd15:   return MsaaMode16x;
      default: return MsaaMode2x;
    endcase
  endfunction

  // Input packet size (number of 512-bit words)
  function automatic logic [1:0] inpck_sz(inpck_type_e inpck_type);
    unique case (inpck_type)
      InpckTriWithTile64x64,
      InpckTriWithTile128x128,
      InpckLargeTriangle,
      InpckRboxState:          return 2'd2;
      InpckFullyCoveredTile,
      InpckLargeTriangleTile,
      InpckFragShadingState,
      InpckEndOfBuffer:        return 2'd1;
      default:                 return 2'd1;
    endcase
  endfunction

  // Convert packed bus state to internal pipeline state
  function automatic state_t state_from_inpck(state_pck_t inpck);
    state_t s;
    s = '0;
    s.msaa_enable                    = inpck.msaa_enable;
    s.msaa_samples                   = inpck.msaa_samples;
    s.msaa_shading_enable            = inpck.msaa_shading_enable;
    s.msaa_alpha_to_coverage         = inpck.msaa_alpha_to_coverage;
    s.msaa_alpha_to_one              = inpck.msaa_alpha_to_one;
    s.msaa_sample_mask               = inpck.msaa_sample_mask;
    s.msaa_min_sample_shading        = inpck.msaa_min_sample_shading;
    s.ds_buffer_addr                 = inpck.ds_buffer_addr;
    s.ds_buffer_format               = inpck.ds_buffer_format;
    s.ds_buffer_tiled                = inpck.ds_buffer_tiled;
    s.ds_buffer_row_pitch            = inpck.ds_buffer_row_pitch;
    s.depth_clamp_enable             = inpck.depth_clamp_enable;
    s.depth_bound_enable             = inpck.depth_bound_enable;
    s.depth_test_enable              = inpck.depth_test_enable;
    s.depth_test_write_enable        = inpck.depth_test_write_enable;
    s.depth_test_compare_op          = inpck.depth_test_compare_op;
    s.early_frag_tests_enable        = inpck.early_frag_tests_enable;
    s.stencil_test_enable            = inpck.stencil_test_enable;
    s.fragment_shader_disabled       = inpck.fragment_shader_disabled;
    s.fragment_shader_reads_bary     = inpck.fragment_shader_reads_bary;
    s.fragment_shader_reads_depth    = inpck.fragment_shader_reads_depth;
    s.fragment_shader_reads_coverage = inpck.fragment_shader_reads_coverage;
    s.fragment_shader_per_sample     = inpck.fragment_shader_per_sample;
    s.hart_tile_width                = inpck.hart_tile_width;
    s.hart_tile_height               = inpck.hart_tile_height;
    s.shire_layout_width             = inpck.shire_layout_width;
    s.shire_layout_height            = inpck.shire_layout_height;
    s.depth_bound_min                = inpck.depth_bound_min;
    s.depth_bound_max                = inpck.depth_bound_max;
    s.depth_min                      = inpck.depth_min;
    s.depth_max                      = inpck.depth_max;
    s.scissor_start_x                = inpck.scissor_start_x;
    s.scissor_start_y                = inpck.scissor_start_y;
    s.scissor_height                 = inpck.scissor_height;
    s.scissor_width                  = inpck.scissor_width;
    s.stencil_front_state            = inpck.stencil_front_state;
    s.stencil_back_state             = inpck.stencil_back_state;
    return s;
  endfunction

  // Extract fragment shader state from input packet
  function automatic fsh_state_t fsh_state_from_inpck(inpck_fsh_state_t inpck);
    fsh_state_t fs;
    fs.fshader_function_addr = inpck.fshader_function_addr;
    fs.fshader_data_addr     = inpck.fshader_data_addr;
    return fs;
  endfunction

  // Check if edge is top-or-left (tie-breaking rule)
  function automatic logic is_top_or_left_edge(edge_coef_t a, edge_coef_t b);
    return (a == '0) ? b[EdgeCoefSz-1] : ~a[EdgeCoefSz-1];
  endfunction

  // ── Large triangle coefficient extraction ────────────────

  function automatic edge_coef_t lt_edge_coef(edge_eq_large_t in_edge_eq, logic coef_idx);
    logic [LtEdgeCoefBits-1:0] in_coef;
    in_coef = coef_idx ? {in_edge_eq.b_coef_hi, in_edge_eq.b_coef_lo}
                       : {in_edge_eq.a_coef_hi, in_edge_eq.a_coef_lo};
    return EdgeCoefSz'($signed(in_coef)) << (EdgeCoefFrcSz - LtEdgeCoefFrcBits);
  endfunction

  function automatic triangle_state_t lt_triangle_state(inpck_large_triangle_t inpck);
    triangle_state_t ts;
    ts.tri_facing         = inpck.tri_facing;
    ts.triangle_data_addr = inpck.triangle_data_addr;
    for (int e = 0; e < 3; e++) begin
      ts.edge_eq_a[e] = lt_edge_coef(inpck.edge_eq[e], 1'b0);
      ts.edge_eq_b[e] = lt_edge_coef(inpck.edge_eq[e], 1'b1);
      ts.top_or_left_edge[e] = is_top_or_left_edge(ts.edge_eq_a[e], ts.edge_eq_b[e]);
    end
    ts.depth_eq_a = inpck.depth_eq.a_coef;
    ts.depth_eq_b = inpck.depth_eq.b_coef;
    return ts;
  endfunction

  // ── 128x128 tile coefficient extraction ──────────────────

  function automatic edge_coef_t t128_edge_coef(edge_eq_128x128_t in_edge_eq, logic coef_idx);
    logic [T128EdgeCoefBits-1:0] in_coef;
    in_coef = coef_idx ? in_edge_eq.b_coef : in_edge_eq.a_coef;
    return EdgeCoefSz'($signed(in_coef)) << (EdgeCoefFrcSz - T128EdgeCoefFrcBits);
  endfunction

  function automatic triangle_state_t t128_triangle_state(
    inpck_tri_with_tile_128x128_t inpck
  );
    triangle_state_t ts;
    ts.tri_facing         = inpck.tri_facing;
    ts.triangle_data_addr = inpck.triangle_data_addr;
    for (int e = 0; e < 3; e++) begin
      ts.edge_eq_a[e] = t128_edge_coef(inpck.edge_eq[e], 1'b0);
      ts.edge_eq_b[e] = t128_edge_coef(inpck.edge_eq[e], 1'b1);
      ts.top_or_left_edge[e] = is_top_or_left_edge(ts.edge_eq_a[e], ts.edge_eq_b[e]);
    end
    ts.depth_eq_a = inpck.depth_eq.a_coef;
    ts.depth_eq_b = inpck.depth_eq.b_coef;
    return ts;
  endfunction

  // ── 64x64 tile coefficient extraction ────────────────────

  function automatic edge_coef_t t64_edge_coef(edge_eq_64x64_t in_edge_eq, logic coef_idx);
    logic [T64EdgeCoefBits-1:0] in_coef;
    in_coef = coef_idx ? in_edge_eq.b_coef : in_edge_eq.a_coef;
    return EdgeCoefSz'($signed(in_coef)) << (EdgeCoefFrcSz - T64EdgeCoefFrcBits);
  endfunction

  function automatic triangle_state_t t64_triangle_state(
    inpck_tri_with_tile_64x64_t inpck
  );
    triangle_state_t ts;
    ts.tri_facing         = inpck.tri_facing;
    ts.triangle_data_addr = inpck.triangle_data_addr;
    for (int e = 0; e < 3; e++) begin
      ts.edge_eq_a[e] = t64_edge_coef(inpck.edge_eq[e], 1'b0);
      ts.edge_eq_b[e] = t64_edge_coef(inpck.edge_eq[e], 1'b1);
      ts.top_or_left_edge[e] = is_top_or_left_edge(ts.edge_eq_a[e], ts.edge_eq_b[e]);
    end
    ts.depth_eq_a = inpck.depth_eq.a_coef;
    ts.depth_eq_b = inpck.depth_eq.b_coef;
    return ts;
  endfunction

  // ── Edge sample extraction (tile-format to internal) ─────

  function automatic edge_sample_t ltt_edge_sample(edge_sample_large_t in_sample);
    return EdgeSampBits'($signed(in_sample.edge_sample)) << (EdgeSampFrcBits - LtEdgeSampFrcBits);
  endfunction

  function automatic edge_sample_t t128_edge_sample(edge_sample_128x128_t in_sample);
    return EdgeSampBits'($signed(in_sample.edge_sample)) << (EdgeSampFrcBits - T128EdgeSampFrcBits);
  endfunction

  function automatic edge_sample_t t64_edge_sample(edge_sample_64x64_t in_sample);
    return EdgeSampBits'($signed(in_sample.edge_sample)) << (EdgeSampFrcBits - T64EdgeSampFrcBits);
  endfunction

  // ── Tile state extraction from input packets ─────────────
  /* verilator lint_off WIDTHEXPAND */  // tile_pos_t is 13 bits, tile_state_t.left/top is 14 bits — intentional zero-extension

  function automatic tile_state_t lt_tile_state(inpck_large_triangle_tile_t inpck);
    tile_state_t ts;
    ts.size          = inpck.tile_size;
    ts.left          = inpck.tile_left;
    ts.top           = inpck.tile_top;
    ts.fully_covered = 1'b0;
    for (int e = 0; e < 3; e++)
      ts.edge_sample[e] = ltt_edge_sample(inpck.edge_sample[e]);
    ts.depth_sample = inpck.depth_sample;
    return ts;
  endfunction

  function automatic tile_state_t t128_tile_state(inpck_tri_with_tile_128x128_t inpck);
    tile_state_t ts;
    ts.size          = inpck.tile_size;
    ts.left          = inpck.tile_left;
    ts.top           = inpck.tile_top;
    ts.fully_covered = 1'b0;
    for (int e = 0; e < 3; e++)
      ts.edge_sample[e] = t128_edge_sample(inpck.edge_sample[e]);
    ts.depth_sample = inpck.depth_sample;
    return ts;
  endfunction

  function automatic tile_state_t t64_tile_state(inpck_tri_with_tile_64x64_t inpck);
    tile_state_t ts;
    ts.size          = inpck.tile_size;
    ts.left          = inpck.tile_left;
    ts.top           = inpck.tile_top;
    ts.fully_covered = 1'b0;
    for (int e = 0; e < 3; e++)
      ts.edge_sample[e] = t64_edge_sample(inpck.edge_sample[e]);
    ts.depth_sample = inpck.depth_sample;
    return ts;
  endfunction

  function automatic tile_state_t fct_tile_state(inpck_fully_covered_tile_t inpck);
    tile_state_t ts;
    ts.size          = inpck.tile_size;
    ts.left          = inpck.tile_left;
    ts.top           = inpck.tile_top;
    ts.fully_covered = 1'b1;
    for (int e = 0; e < 3; e++)
      ts.edge_sample[e] = t64_edge_sample(inpck.edge_sample[e]);
    ts.depth_sample = inpck.depth_sample;
    return ts;
  endfunction

  /* verilator lint_on WIDTHEXPAND */

  // ── FP32 to internal depth format ────────────────────────

  function automatic internal_depth_t f32_to_depth_internal(logic [31:0] in_val);
    logic exp_special, exp_zero, mant_zero;
    internal_depth_t d;
    exp_special = (in_val[30:23] == '1);
    exp_zero    = (in_val[30:23] == '0);
    mant_zero   = (in_val[22:0]  == '0);
    d.infinite = exp_special &&  mant_zero;
    d.nan      = exp_special && !mant_zero;
    d.zero     = exp_zero    &&  mant_zero;
    d.subnorm  = exp_zero    && !mant_zero;
    d.e        = in_val[30:23];
    d.s        = in_val[31];
    d.m        = {!exp_zero, in_val[22:0]};
    d.e_delta  = '0;
    d.m_shift  = '0;
    return d;
  endfunction

  // ── Triangle coverage test ───────────────────────────────

  function automatic logic sample_inside_triangle(
    edge_sample_t [2:0] edge_sample_in,
    logic [2:0]         edge_is_top_or_left
  );
    logic is_inside;
    is_inside = 1'b1;
    for (int e = 0; e < 3; e++) begin
      is_inside = is_inside &&
                  ((~edge_sample_in[e][EdgeSampBits-1] && (edge_sample_in[e][EdgeSampBits-2:0] != '0)) ||
                   ((edge_sample_in[e] == '0) && edge_is_top_or_left[e]));
    end
    return is_inside;
  endfunction

  // ── Quad-to-hart mapping ─────────────────────────────────
  // Maps a quad's position within a tile to a hardware thread ID based on
  // the configured shire layout dimensions.

  function automatic logic [HartsL-1:0] quad_to_hart(
    logic [1:0]              hart_tile_width,
    logic [1:0]              hart_tile_height,
    logic [1:0]              shire_layout_width,
    logic [1:0]              shire_layout_height,
    logic [MaxTileDimL-1:0]  quad_x,
    logic [MaxTileDimL-1:0]  quad_y
  );
    logic [MaxTileDimL-1:0] quad_hart_x, quad_hart_y;
    quad_hart_x = quad_x >> (hart_tile_width  + 1);
    quad_hart_y = quad_y >> (hart_tile_height + 1);

    unique case ({shire_layout_height, shire_layout_width})
      {2'd0, 2'd0}: return HartsL'(0);
      {2'd0, 2'd1}: return HartsL'(quad_hart_x[0]);
      {2'd0, 2'd2}: return HartsL'(quad_hart_x[1:0]);
      {2'd0, 2'd3}: return HartsL'(quad_hart_x[2:0]);

      {2'd1, 2'd0}: return HartsL'(quad_hart_y[0]);
      {2'd1, 2'd1}: return HartsL'({quad_hart_y[0],   quad_hart_x[0]});
      {2'd1, 2'd2}: return HartsL'({quad_hart_y[0],   quad_hart_x[1:0]});
      {2'd1, 2'd3}: return HartsL'({quad_hart_y[0],   quad_hart_x[2:0]});

      {2'd2, 2'd0}: return HartsL'(quad_hart_y[1:0]);
      {2'd2, 2'd1}: return HartsL'({quad_hart_y[1:0], quad_hart_x[0]});
      {2'd2, 2'd2}: return HartsL'({quad_hart_y[1:0], quad_hart_x[1:0]});
      {2'd2, 2'd3}: return HartsL'({quad_hart_y[1:0], quad_hart_x[2:0]});

      {2'd3, 2'd0}: return HartsL'(quad_hart_y[2:0]);
      {2'd3, 2'd1}: return HartsL'({quad_hart_y[2:0], quad_hart_x[0]});
      {2'd3, 2'd2}: return HartsL'({quad_hart_y[2:0], quad_hart_x[1:0]});
      {2'd3, 2'd3}: return HartsL'({quad_hart_y[2:0], quad_hart_x[2:0]});
      default:      return '0;
    endcase
  endfunction

  // ── Hart message port physical address ───────────────────
  // Constructs the physical address for sending a message to a hart's
  // message port via the ET-Link interconnect.

  function automatic logic [PaSize-1:0] hart_msg_port_pa(
    logic [HartsL-1:0]          hart_id,
    logic [CoreNrMsgPortsL-1:0] port_id
  );
    logic [AdEsrPortMsgSize-1:0] message_id;
    message_id = AdEsrPortMsgCommon | (AdEsrPortMsgSize'(port_id) << AdEsrPortIdStart);
    return PaSize'({1'b1, AdEsrProtBitsUmode, LocalShireId,
                    AdEsrAgentTypeMinion, 8'(hart_id), message_id});
  endfunction

  // ── MSAA sample positions ────────────────────────────────
  // Returns the (x, y) sub-pixel position for a given MSAA sample index
  // and mode. Positions follow the Vulkan standard sample locations.

  function automatic msaa_sample_pos_t get_msaa_sample_pos(
    msaa_mode_e       msaa_mode,
    msaa_sample_idx_t msaa_sample
  );
    msaa_sample_pos_t pos;
    pos = '{default: MsaaPos0000};
    unique case (msaa_mode)
      MsaaMode2x:
        unique case (msaa_sample)
          'd0:     begin pos[0] = MsaaPos2500; pos[1] = MsaaPos2500; end
          'd1:     begin pos[0] = MsaaPos7500; pos[1] = MsaaPos7500; end
          default: begin pos[0] = MsaaPos0000; pos[1] = MsaaPos0000; end
        endcase
      MsaaMode4x:
        unique case (msaa_sample)
          'd0:     begin pos[0] = MsaaPos3750; pos[1] = MsaaPos1250; end
          'd1:     begin pos[0] = MsaaPos8750; pos[1] = MsaaPos3750; end
          'd2:     begin pos[0] = MsaaPos1250; pos[1] = MsaaPos6250; end
          'd3:     begin pos[0] = MsaaPos6250; pos[1] = MsaaPos8750; end
          default: begin pos[0] = MsaaPos0000; pos[1] = MsaaPos0000; end
        endcase
      MsaaMode8x:
        unique case (msaa_sample)
          'd0:     begin pos[0] = MsaaPos5625; pos[1] = MsaaPos3125; end
          'd1:     begin pos[0] = MsaaPos4375; pos[1] = MsaaPos6875; end
          'd2:     begin pos[0] = MsaaPos8125; pos[1] = MsaaPos5625; end
          'd3:     begin pos[0] = MsaaPos3125; pos[1] = MsaaPos1875; end
          'd4:     begin pos[0] = MsaaPos1875; pos[1] = MsaaPos8125; end
          'd5:     begin pos[0] = MsaaPos0625; pos[1] = MsaaPos4375; end
          'd6:     begin pos[0] = MsaaPos6875; pos[1] = MsaaPos9375; end
          'd7:     begin pos[0] = MsaaPos9375; pos[1] = MsaaPos0625; end
          default: begin pos[0] = MsaaPos0000; pos[1] = MsaaPos0000; end
        endcase
      MsaaMode16x:
        unique case (msaa_sample)
          'd0:     begin pos[0] = MsaaPos5625; pos[1] = MsaaPos5625; end
          'd1:     begin pos[0] = MsaaPos4375; pos[1] = MsaaPos3125; end
          'd2:     begin pos[0] = MsaaPos3125; pos[1] = MsaaPos6250; end
          'd3:     begin pos[0] = MsaaPos7500; pos[1] = MsaaPos4375; end
          'd4:     begin pos[0] = MsaaPos1875; pos[1] = MsaaPos3750; end
          'd5:     begin pos[0] = MsaaPos6250; pos[1] = MsaaPos8125; end
          'd6:     begin pos[0] = MsaaPos8125; pos[1] = MsaaPos6875; end
          'd7:     begin pos[0] = MsaaPos6875; pos[1] = MsaaPos1875; end
          'd8:     begin pos[0] = MsaaPos3750; pos[1] = MsaaPos8750; end
          'd9:     begin pos[0] = MsaaPos5000; pos[1] = MsaaPos0625; end
          'd10:    begin pos[0] = MsaaPos2500; pos[1] = MsaaPos1250; end
          'd11:    begin pos[0] = MsaaPos1250; pos[1] = MsaaPos7500; end
          'd12:    begin pos[0] = MsaaPos0000; pos[1] = MsaaPos5000; end
          'd13:    begin pos[0] = MsaaPos9375; pos[1] = MsaaPos2500; end
          'd14:    begin pos[0] = MsaaPos8750; pos[1] = MsaaPos9375; end
          'd15:    begin pos[0] = MsaaPos0625; pos[1] = MsaaPos0000; end
          default: begin pos[0] = MsaaPos0000; pos[1] = MsaaPos0000; end
        endcase
    endcase
    return pos;
  endfunction

  // Non-MSAA sample position (pixel center offset added in triangle setup)
  function automatic msaa_sample_pos_t get_non_msaa_sample_pos();
    msaa_sample_pos_t pos;
    pos[0] = MsaaPos0000;
    pos[1] = MsaaPos0000;
    return pos;
  endfunction

  /* verilator lint_on UNUSEDSIGNAL */
  /* verilator lint_on UNUSEDPARAM */

endpackage
