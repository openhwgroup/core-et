// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

package neigh_hv_logic_pkg;

  import etlink_pkg::*;
  import minion_pkg::*;
  import shirecache_pkg::*;

  /* verilator lint_off UNUSEDPARAM */  // Shared neighborhood support constants are consumed incrementally by the top-level shell.
  localparam int unsigned MinPerN      = minion_pkg::ShireCoopMinMaskSize;
  localparam int unsigned ThreadsPerN  = MinPerN * minion_pkg::NrThreadsDefault;
  localparam int unsigned ScBanks      = shirecache_pkg::Banks;
  localparam int unsigned UcDataSize   = etlink_pkg::NeighDataSize;
  localparam int unsigned BpamRcSize   = 4;
  /* verilator lint_on UNUSEDPARAM */

  // Mirrors original APB_from_neigh_t field order and names from shire_types.vh.
  typedef struct packed {
    logic [minion_pkg::BpamShireApbDataWidth-1:0] apb_prdata;
    logic                                         apb_pready;
    logic                                         apb_pslverr;
  } apb_from_neigh_t;

  // Mirrors original bpam_rc_tbox_ack_t from debug_types.vh.
  typedef struct packed {
    logic halt_ack;
    logic resume_ack;
  } bpam_rc_tbox_ack_t;

endpackage : neigh_hv_logic_pkg
