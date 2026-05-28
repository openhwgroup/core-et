// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

package neigh_ch_apb_mux_pkg;

  /* verilator lint_off UNUSEDPARAM */  // Channel APB decode constants are shared by leaf/unit/cosim wrappers.

  localparam int unsigned MinPerN              = minion_pkg::ShireCoopMinMaskSize;
  localparam int unsigned DataWidth            = minion_pkg::BpamShireApbDataWidth;
  localparam int unsigned MinDmApbAddrWidth    = minion_pkg::MinDmApbAddrWidth;
  localparam int unsigned EsrNeighApbAddrWidth = esr_pkg::EsrNeighApbAddrWidth;
  localparam int unsigned EsrNeighApbPpAddrWidth = esr_pkg::EsrNeighApbPpAddrWidth;

  // Mirrors original `TBOX_NEIGH_APB_ADDR_WIDTH` and
  // `ICACHE_NEIGH_APB_ADDR_WIDTH` from debug_defines.vh. The I-cache value is
  // intentionally kept equal to icache_pkg::IcacheNeighApbAddrWidth without
  // importing the full I-cache package into this small APB-leaf package.
  localparam int unsigned TboxNeighApbAddrWidth   = 12;
  localparam int unsigned IcacheNeighApbAddrWidth = 10;

  localparam int unsigned NeighChDmSubAddrWidth =
      (TboxNeighApbAddrWidth > EsrNeighApbPpAddrWidth)
          ? ((TboxNeighApbAddrWidth > IcacheNeighApbAddrWidth)
                 ? TboxNeighApbAddrWidth : IcacheNeighApbAddrWidth)
          : ((EsrNeighApbPpAddrWidth > IcacheNeighApbAddrWidth)
                 ? EsrNeighApbPpAddrWidth : IcacheNeighApbAddrWidth);
  localparam int unsigned NeighChDmApbAddrWidth = 1 + NeighChDmSubAddrWidth;
  localparam int unsigned NeighDmSubAddrWidth =
      (NeighChDmApbAddrWidth > MinDmApbAddrWidth) ? NeighChDmApbAddrWidth : MinDmApbAddrWidth;
  localparam int unsigned NeighDmApbAddrWidth = 1 + NeighDmSubAddrWidth;

  localparam int unsigned NeighDmApbAddrRegLsb    = 0;
  localparam int unsigned NeighDmApbAddrRegMsb    = 8;
  localparam int unsigned NeighDmApbAddrThreadLsb = 9;
  localparam int unsigned NeighDmApbAddrThreadMsb = 9;
  localparam int unsigned NeighDmApbAddrMinLsb    = 10;
  localparam int unsigned NeighDmApbAddrMinMsb    = 12;
  localparam int unsigned NeighDmApbAddrPpLsb     = NeighDmApbAddrWidth - 4;
  localparam int unsigned NeighDmApbAddrPpMsb     = NeighDmApbAddrWidth - 3;
  localparam int unsigned NeighDmApbAddrDestLsb   = NeighDmApbAddrWidth - 1;
  localparam int unsigned NeighDmApbAddrDestMsb   = NeighDmApbAddrWidth - 1;
  localparam int unsigned NeighDmApbAddrClassLsb  = NeighDmApbAddrWidth - 2;
  localparam int unsigned NeighDmApbAddrClassMsb  = NeighDmApbAddrWidth - 1;
  localparam int unsigned NeighChTboxSelectBit    = 14;

  localparam logic [1:0] NeighResourceMinionEsr = 2'b00;
  localparam logic [1:0] NeighResourceMinionDc  = 2'b10;

  /* verilator lint_on UNUSEDPARAM */

  // Mirrors original APB_ESR_TO_SLAVE_TYPE_DEF field names and packed order.
  typedef struct packed {
    logic [NeighDmApbAddrWidth-1:0] apb_paddr;
    logic                           apb_penable;
    logic                           apb_psel;
    logic [DataWidth-1:0]           apb_pwdata;
    logic                           apb_pwrite;
  } apb_to_neigh_t;

  typedef struct packed {
    logic [EsrNeighApbPpAddrWidth-1:0] apb_paddr;
    logic                              apb_penable;
    logic                              apb_psel;
    logic [DataWidth-1:0]              apb_pwdata;
    logic                              apb_pwrite;
  } apb_to_neigh_esrs_t;

  typedef struct packed {
    logic [IcacheNeighApbAddrWidth-1:0] apb_paddr;
    logic                               apb_penable;
    logic                               apb_psel;
    logic [DataWidth-1:0]               apb_pwdata;
    logic                               apb_pwrite;
  } apb_to_neigh_icache_t;

  typedef struct packed {
    logic [TboxNeighApbAddrWidth-1:0] apb_paddr;
    logic                             apb_penable;
    logic                             apb_psel;
    logic [DataWidth-1:0]             apb_pwdata;
    logic                             apb_pwrite;
  } apb_to_neigh_tbox_t;

  // Mirrors original APB_ESR_TO_MASTER_TYPE_DEF field names and packed order.
  typedef struct packed {
    logic [DataWidth-1:0] apb_prdata;
    logic                 apb_pready;
    logic                 apb_pslverr;
  } apb_from_neigh_t;

  typedef apb_from_neigh_t apb_from_neigh_channel_t;

endpackage : neigh_ch_apb_mux_pkg
