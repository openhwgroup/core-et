// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

package shire_sbm_pkg;

  /* verilator lint_off UNUSEDPARAM */  // constants are shared across current and downstream compute-shire blocks

  localparam int unsigned NumNeigh        = shirecache_pkg::NumNeigh;
  localparam int unsigned NumBanks        = shirecache_pkg::Banks;
  localparam int unsigned NumRbox         = shirecache_pkg::RboxPerShire;
  localparam int unsigned MinPerNeigh     = 8;
  localparam int unsigned ThreadsPerShire = NumNeigh * MinPerNeigh;

  localparam int unsigned NeighRangeLo = 0;
  localparam int unsigned NeighRangeHi = NumNeigh;
  localparam int unsigned BankRangeLo  = NeighRangeHi;
  localparam int unsigned BankRangeHi  = BankRangeLo + NumBanks;
  localparam int unsigned ShireRangeLo = BankRangeHi;
  localparam int unsigned RboxRangeLo  = ShireRangeLo + 1;

  localparam int unsigned SbmInterfaces      = NumNeigh + NumBanks + 2;
  localparam int unsigned ShireApbInterfaces = SbmInterfaces + 1;

  localparam int unsigned WriteCredits = 8;
  localparam int unsigned ReadCredits  = 8;
  localparam int unsigned MaxConcReq   = WriteCredits + ReadCredits;
  localparam int unsigned SbmIndexWidth = $clog2(MaxConcReq);

  localparam int unsigned AxiAddrWidth = axi_pkg::ScMasterAddrSize;
  localparam int unsigned AxiIdWidth   = axi_pkg::ScSlaveIdSize;
  localparam int unsigned ApbDataWidth = apb_pkg::DefaultDataWidth;

  // The original shire-top APB fanout has to carry neighborhood/debug and
  // ICache address-select bits. Keep that width at the fanout boundary; block
  // adapters may truncate to apb_pkg::DefaultAddrWidth where the target block
  // has a narrower native APB port.
  localparam int unsigned SbmApbAddrWidth      = 17;
  localparam int unsigned ShireApbAddrWidth    = 19;
  localparam int unsigned IcacheApbInterfaceId = ShireApbInterfaces - 1;

  typedef struct packed {
    logic [ShireApbAddrWidth-1:0] paddr;
    logic                         pwrite;
    logic                         psel;
    logic                         penable;
    logic [ApbDataWidth-1:0]      pwdata;
  } shire_apb_req_t;

  typedef struct packed {
    logic                         pready;
    logic [ApbDataWidth-1:0]      prdata;
    logic                         pslverr;
  } shire_apb_rsp_t;

  typedef struct packed {
    logic [AxiIdWidth-1:0]   id;
    logic [AxiAddrWidth-1:0] addr;
  } sbm_axi_a_t;

  typedef struct packed {
    logic [ApbDataWidth-1:0] data;
  } sbm_axi_w_t;

  typedef struct packed {
    logic                           valid;
    logic [3:0]                     counter;
    logic                           error;
    logic [AxiIdWidth-1:0]          axi_id;
  } bcast_ctrl_t;

  typedef struct packed {
    logic                           reserved_pos;
    logic                           ar_valid;
    sbm_axi_a_t                     ar;
    logic                           aw_valid;
    sbm_axi_a_t                     aw;
    logic [ApbDataWidth-1:0]        data;
  } req_buffer_t;

  typedef req_buffer_t [MaxConcReq-1:0] req_buffer_array_t;

  /* verilator lint_on UNUSEDPARAM */

endpackage : shire_sbm_pkg
