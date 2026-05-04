// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Minion frontend package.
//
// Frontend-specific types (fetch buffers, ICache interface, thread data).
// Imports minion_pkg for shared constants and types.

/* verilator lint_off UNUSEDPARAM */
package minion_frontend_pkg;

  import minion_pkg::*;

  // ════════════════════════════════════════════════════════
  // Fetch buffer constants
  // ════════════════════════════════════════════════════════

  parameter int unsigned FeFetchBuffers     = 2;
  parameter int unsigned FeFetchBufferIdxW  = $clog2(FeFetchBuffers);
  parameter int unsigned FeFetchReadSize    = 256;  // bits per fetch block
  parameter int unsigned FeFetchValidInsts  = 16;   // 256 / 16-bit halfwords
  parameter int unsigned FeFetchPtrSize     = 5;    // $clog2(16 * 2)
  parameter int unsigned FeBufferPosSz      = 4;    // FeFetchPtrSize - 1
  parameter int unsigned FeFetchReadLsb     = 5;    // byte offset bits in fetch address

  // ════════════════════════════════════════════════════════
  // Frontend ↔ ICache interface
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    logic [0:0]          thread_id;
    vm_status_t          vm_status;
    logic [VaSizeExt-1:0] addr;
  } fe_icache_req_t;

  typedef struct packed {
    logic [FeFetchReadSize-1:0] data;
    logic                       page_fault;
    logic                       access_fault;
    logic                       cacheable;
    logic                       bus_err;
    logic                       ecc_err;
  } icache_fe_resp_t;

  // ════════════════════════════════════════════════════════
  // Frontend internal: instruction with metadata
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    logic [PcSizeExt-1:0] pc;
    logic [InstSize-1:0]  inst_bits;
    logic                 page_fault0;
    logic                 access_fault0;
    logic                 page_fault1;
    logic                 access_fault1;
    logic                 replay;
    logic                 bus_err;
    logic                 ecc_err;
  } fe_operation_t;

  // ════════════════════════════════════════════════════════
  // Frontend → Core: issued instruction
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    logic [PcSizeExt-1:0] pc;
    logic [InstSize-1:0]  inst_bits;
    logic                 pf0;
    logic                 pf1;
    logic                 af0;
    logic                 af1;
    logic                 replay;
    logic                 rvc;
    logic                 bus_error;
    logic                 ecc_error;
  } fe_core_resp_t;

  // ════════════════════════════════════════════════════════
  // Core → Frontend: redirect request (mispredict / exception)
  // ════════════════════════════════════════════════════════

  typedef struct packed {
    logic halt;
    logic resume;
  } fe_req_debug_t;

  typedef struct packed {
    logic [PcSizeExt-1:0] pc;
    logic                 speculative;
    fe_req_debug_t        debug_info;
  } fe_req_t;

  // (Shared types moved to minion_pkg)

  // Frontend internal: per-thread FIFO data (decoded instruction)
  // ════════════════════════════════════════════════════════

  // What the 2-entry instruction FIFO stores per thread.
  // Matches original's frontend_thread_data (frontend_types.vh).
  typedef struct packed {
    fe_core_resp_t    core_resp;
    vpu_ctrl_sigs_t   vpu_ctrl_sigs;
    minion_control_t  intpipe_ctrl;
  } fe_thread_data_t;

  typedef struct packed {
    logic                               core_req_valid;
    logic [PcSizeExt-1:0]               core_req_pc;
    logic [VaSizeExt-1:0]               f0_pc;
    logic [VaSizeExt-1:0]               f0_pc_next;
    logic                               f0_pc_update;
    logic                               clock_enable;
    logic [3:0]                         gated_clk_ticks;
    logic [FeFetchBuffers-1:0]          buffer_empty;
    logic [VaSizeExt-1:0]               buffer_pc0;
    logic [VaSizeExt-1:0]               buffer_pc1;
    logic [FeFetchBufferIdxW-1:0]       req_buffer;
    logic [FeFetchBufferIdxW-1:0]       f6_req_buffer;
    logic [FeFetchBufferIdxW-1:0]       f6_buffer_id;
    logic [FeFetchBufferIdxW-1:0]       f7_buffer_id;
    logic [FeFetchPtrSize-1:0]          f6_buffer_ptr;
    logic [FeFetchPtrSize-1:0]          f6_buffer_ptr_next;
    logic [FeFetchPtrSize-1:0]          f7_buffer_ptr;
    logic                               icache_inflight;
    logic                               miss_pending;
    logic                               f1_req_valid;
    logic                               f5_req_valid;
    logic                               f6_req_valid;
    logic                               f6_buffer_wr;
    logic                               f6_valid;
    logic                               f6_stall;
    logic                               f7_valid;
    logic                               f7_stall;
  } fe_thread_debug_t;

endpackage
/* verilator lint_on UNUSEDPARAM */
