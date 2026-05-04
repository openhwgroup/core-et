// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// AXI4 protocol types.
//
// Defines parameterized AXI4 channel structs (AR, AW, W, R, B) and
// shire-cache-specific instantiations used by the mesh interfaces.
//
// Replaces: axi_defines.vh + axi_types.vh (etcore-soc)

package axi_pkg;

  /* verilator lint_off UNUSEDPARAM */  // constants consumed by modules not yet instantiated

  // ── AXI4 protocol field widths (spec-mandated) ──────────
  localparam int unsigned AxLenSize   = 8;
  localparam int unsigned AxSizeSize  = 3;
  localparam int unsigned AxBurstSize = 2;
  localparam int unsigned AxCacheSize = 4;
  localparam int unsigned AxProtSize  = 3;
  localparam int unsigned AxQosSize   = 4;
  localparam int unsigned RespSize    = 2;

  // ── AXI4 response codes ─────────────────────────────────
  typedef enum logic [RespSize-1:0] {
    RespOkay   = 2'b00,
    RespExOkay = 2'b01,
    RespSlvErr = 2'b10,
    RespDecErr = 2'b11
  } resp_e;

  // ── Shire cache mesh master AXI parameters ──────────────
  // Design-specific widths for the mesh master (to_l3, to_sys) ports.
  localparam int unsigned ScMasterIdSize     = 9;   // 2-3 bank + 6 reqq ID
  localparam int unsigned ScMasterAddrSize   = 40;
  localparam int unsigned ScMasterDataSize   = 512;
  localparam int unsigned ScMasterStrbSize   = ScMasterDataSize / 8;  // 64
  localparam int unsigned ScMasterArUserSize = 1;
  localparam int unsigned ScMasterAwUserSize = 5;

  // Mesh slave adds bridge source bits to master ID
  localparam int unsigned MeshSourceBridgeIdSize = 10;
  localparam int unsigned ScSlaveIdSize = MeshSourceBridgeIdSize + ScMasterIdSize;  // 19

  // ── AXI4 constant values for mesh master ────────────────
  localparam logic [AxLenSize-1:0]   ScMasterAxLen   = 8'b0;        // single beat
  localparam logic [AxBurstSize-1:0] ScMasterAxBurst = 2'b01;       // INCR
  localparam logic [AxCacheSize-1:0] ScMasterAxCache = 4'b0000;     // non-cacheable
  localparam logic [AxProtSize-1:0]  ScMasterAxProt  = 3'b010;      // data, non-secure, unprivileged
  localparam logic [AxQosSize-1:0]   ScMasterAxQos   = 4'b0000;     // default QOS

  // ── AXI4 channel structs ────────────────────────────────

  // AR / AW channel (address)
  typedef struct packed {
    logic [ScMasterIdSize-1:0]    id;
    logic [ScMasterAddrSize-1:0]  addr;
    logic [AxLenSize-1:0]         len;
    logic [AxSizeSize-1:0]        size;
    logic [AxBurstSize-1:0]       burst;
    logic                         lock;
    logic [AxCacheSize-1:0]       cache;
    logic [AxProtSize-1:0]        prot;
    logic [AxQosSize-1:0]         qos;
    logic [ScMasterArUserSize-1:0] user;
  } sc_master_ar_t;

  typedef struct packed {
    logic [ScMasterIdSize-1:0]    id;
    logic [ScMasterAddrSize-1:0]  addr;
    logic [AxLenSize-1:0]         len;
    logic [AxSizeSize-1:0]        size;
    logic [AxBurstSize-1:0]       burst;
    logic                         lock;
    logic [AxCacheSize-1:0]       cache;
    logic [AxProtSize-1:0]        prot;
    logic [AxQosSize-1:0]         qos;
    logic [ScMasterAwUserSize-1:0] user;
  } sc_master_aw_t;

  // Slave AR/AW (wider ID)
  typedef struct packed {
    logic [ScSlaveIdSize-1:0]     id;
    logic [ScMasterAddrSize-1:0]  addr;
    logic [AxLenSize-1:0]         len;
    logic [AxSizeSize-1:0]        size;
    logic [AxBurstSize-1:0]       burst;
    logic                         lock;
    logic [AxCacheSize-1:0]       cache;
    logic [AxProtSize-1:0]        prot;
    logic [AxQosSize-1:0]         qos;
    logic [ScMasterArUserSize-1:0] user;
  } sc_slave_ar_t;

  typedef struct packed {
    logic [ScSlaveIdSize-1:0]     id;
    logic [ScMasterAddrSize-1:0]  addr;
    logic [AxLenSize-1:0]         len;
    logic [AxSizeSize-1:0]        size;
    logic [AxBurstSize-1:0]       burst;
    logic                         lock;
    logic [AxCacheSize-1:0]       cache;
    logic [AxProtSize-1:0]        prot;
    logic [AxQosSize-1:0]         qos;
    logic [ScMasterAwUserSize-1:0] user;
  } sc_slave_aw_t;

  // W channel (write data)
  typedef struct packed {
    logic [ScMasterDataSize-1:0]  data;
    logic [ScMasterStrbSize-1:0]  strb;
    logic                         last;
  } sc_master_w_t;

  // R channel (read response)
  typedef struct packed {
    logic [ScMasterIdSize-1:0]    id;
    resp_e                        resp;
    logic                         last;
    logic [ScMasterDataSize-1:0]  data;
  } sc_master_r_t;

  typedef struct packed {
    logic [ScSlaveIdSize-1:0]     id;
    resp_e                        resp;
    logic                         last;
    logic [ScMasterDataSize-1:0]  data;
  } sc_slave_r_t;

  // B channel (write response)
  typedef struct packed {
    logic [ScMasterIdSize-1:0]    id;
    resp_e                        resp;
  } sc_master_b_t;

  typedef struct packed {
    logic [ScSlaveIdSize-1:0]     id;
    resp_e                        resp;
  } sc_slave_b_t;

  // ── Composite port structs ──────────────────────────────
  // All 5 channels + valid/ready in one struct (for concise port lists).

  typedef struct packed {
    logic            ar_ready;
    logic            ar_valid;
    sc_master_ar_t   ar;
    logic            aw_ready;
    logic            aw_valid;
    sc_master_aw_t   aw;
    logic            w_ready;
    logic            w_valid;
    sc_master_w_t    w;
    logic            r_ready;
    logic            r_valid;
    sc_master_r_t    r;
    logic            b_ready;
    logic            b_valid;
    sc_master_b_t    b;
  } sc_master_port_t;

  typedef struct packed {
    logic            ar_ready;
    logic            ar_valid;
    sc_slave_ar_t    ar;
    logic            aw_ready;
    logic            aw_valid;
    sc_slave_aw_t    aw;
    logic            w_ready;
    logic            w_valid;
    sc_master_w_t    w;  // same data width for slave
    logic            r_ready;
    logic            r_valid;
    sc_slave_r_t     r;
    logic            b_ready;
    logic            b_valid;
    sc_slave_b_t     b;
  } sc_slave_port_t;

endpackage
