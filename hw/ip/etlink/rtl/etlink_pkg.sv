// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// ET-Link bus protocol package.
//
// Defines the request/response types for the ET-Link interconnect used
// between IP blocks and the Shire Cache. This is a protocol definition
// only — there is no ET-Link IP module.
//
// The structs are parameterized via the etlink_req_t / etlink_rsp_t
// typedefs at the bottom. RBOX and other blocks use the full-width
// (512-bit data) variants.

package etlink_pkg;

  // ── System constants ────────────────────────────────────
  // These define the SoC-wide address and data widths.

  /* verilator lint_off VARHIDDEN */  // ET-Link/minion_pkg both publish the SoC PA width.
  localparam int unsigned PaSize       = 40;   // Physical address width
  /* verilator lint_on VARHIDDEN */
  /* verilator lint_off UNUSEDPARAM */  // PaMsb used by downstream modules not yet instantiated
  localparam int unsigned PaMsb        = PaSize - 1;
  /* verilator lint_on UNUSEDPARAM */

  localparam int unsigned IdSize       = 8;    // Request/response ID width
  localparam int unsigned SourceSize   = 4;    // Source/dest field width
  localparam int unsigned DataSize      = 512; // Cache line data width
  localparam int unsigned NeighDataSize = 256; // Neighborhood message data width
  localparam int unsigned QwenSize     = 4;    // QWord enable width
  localparam int unsigned SubopcodeSize = 7;   // Subopcode field width

  // ── Request opcodes ─────────────────────────────────────

  typedef enum logic [4:0] {
    ReqWrite        = 5'd0,
    ReqRead         = 5'd1,
    ReqWriteAround  = 5'd2,
    ReqReadCoop     = 5'd3,
    ReqMsgSendData  = 5'd4,
    ReqAtomic       = 5'd5,
    ReqScpRead      = 5'd6,
    ReqScpWrite     = 5'd7,
    ReqFlush        = 5'd8,
    ReqFlushToMem   = 5'd9,
    ReqEvict        = 5'd10,
    ReqEvictToMem   = 5'd11,
    ReqLock         = 5'd12,
    ReqUnlock       = 5'd13,
    ReqScpFill      = 5'd14,
    ReqPrefetch     = 5'd15,
    ReqAtomicRsp    = 5'd16
  } req_opcode_e;

  // ── Response opcodes ────────────────────────────────────

  typedef enum logic [1:0] {
    RspAck        = 2'd0,
    RspAckData    = 2'd1,
    RspMsgRcvData = 2'd2,
    RspErr        = 2'd3
  } rsp_opcode_e;

  // ── Transaction size ────────────────────────────────────

  typedef enum logic [2:0] {
    SizeByte  = 3'b000,  // 8b
    SizeHWord = 3'b001,  // 16b
    SizeWord  = 3'b010,  // 32b
    SizeDWord = 3'b011,  // 64b
    SizeQWord = 3'b100,  // 128b
    SizeHLine = 3'b101,  // 256b
    SizeLine  = 3'b110   // 512b
  } size_e;

  // ── Neighbor source IDs ─────────────────────────────────

  typedef enum logic [3:0] {
    SrcMinion0 = 4'd0,
    SrcMinion1 = 4'd1,
    SrcMinion2 = 4'd2,
    SrcMinion3 = 4'd3,
    SrcMinion4 = 4'd4,
    SrcMinion5 = 4'd5,
    SrcMinion6 = 4'd6,
    SrcMinion7 = 4'd7,
    SrcIcache  = 4'd8,
    SrcPtw0    = 4'd9,
    SrcPtw1    = 4'd10,
    SrcTbox    = 4'd11,
    SrcCoop    = 4'd12
  } neigh_source_e;

  // ── Request struct ──────────────────────────────────────
  // Full-width request (512-bit data), used by RBOX and most blocks.

  typedef struct packed {
    logic [IdSize-1:0]          id;
    logic [SourceSize-1:0]      source;
    logic                       wdata;      // Request carries write data
    req_opcode_e                opcode;
    logic [PaSize-1:0]          address;
    logic [DataSize-1:0]        data;
    size_e                      size;
    logic [QwenSize-1:0]        qwen;       // QWord enable
    logic [SubopcodeSize-1:0]   subopcode;
  } req_t;

  // ── Neighborhood request struct ─────────────────────────
  // 256-bit data variant used by the standalone neighborhood shell.

  typedef struct packed {
    logic [IdSize-1:0]          id;
    logic [SourceSize-1:0]      source;
    logic                       wdata;
    req_opcode_e                opcode;
    logic [PaSize-1:0]          address;
    logic [NeighDataSize-1:0]   data;
    size_e                      size;
    logic [QwenSize-1:0]        qwen;
    logic [SubopcodeSize-1:0]   subopcode;
  } neigh_req_t;

  // ── Response struct ─────────────────────────────────────
  // Full-width response (512-bit data).

  typedef struct packed {
    logic [IdSize-1:0]          id;
    logic [SourceSize-1:0]      dest;
    logic                       wdata;      // Response carries read data
    rsp_opcode_e                opcode;
    logic [DataSize-1:0]        data;
    size_e                      size;
    logic [QwenSize-1:0]        qwen;
  } rsp_t;

  // ── Neighborhood response struct ────────────────────────
  // 256-bit data variant used by the standalone neighborhood shell.

  typedef struct packed {
    logic [IdSize-1:0]          id;
    logic [SourceSize-1:0]      dest;
    logic                       wdata;
    rsp_opcode_e                opcode;
    logic [NeighDataSize-1:0]   data;
    size_e                      size;
    logic [QwenSize-1:0]        qwen;
  } neigh_rsp_t;

  // ── No-data request struct ──────────────────────────────
  // For interfaces that don't carry data (address-only requests).

  typedef struct packed {
    logic [IdSize-1:0]          id;
    logic [SourceSize-1:0]      source;
    logic                       wdata;
    req_opcode_e                opcode;
    logic [PaSize-1:0]          address;
    size_e                      size;
    logic [QwenSize-1:0]        qwen;
    logic [SubopcodeSize-1:0]   subopcode;
  } req_nodata_t;

  // ── No-data response struct ─────────────────────────────
  // For interfaces that don't return data (ack-only responses).

  typedef struct packed {
    logic [IdSize-1:0]          id;
    logic [SourceSize-1:0]      dest;
    logic                       wdata;
    rsp_opcode_e                opcode;
    size_e                      size;
    logic [QwenSize-1:0]        qwen;
  } rsp_nodata_t;

endpackage
