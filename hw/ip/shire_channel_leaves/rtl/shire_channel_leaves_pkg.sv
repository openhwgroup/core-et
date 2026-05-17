// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

package shire_channel_leaves_pkg;

  /* verilator lint_off UNUSEDPARAM */  // Package constants are consumed incrementally by downstream shire integration tasks.
  localparam int unsigned NumNeigh = shire_esr_pkg::NumNeigh;
  localparam int unsigned NumOtherNeigh = NumNeigh - 1;
  localparam int unsigned NumShireIds = shire_esr_pkg::NumShireIds;
  localparam int unsigned NumShireIdsBits = shire_esr_pkg::NumShireIdsBits;
  localparam int unsigned NocIntNum = shire_esr_pkg::NocIntNum;
  localparam int unsigned ShireCoopIdWidth = neigh_pkg::ShireCoopIdSize;

  localparam int unsigned PllAddrBits = 6;
  localparam int unsigned PllDataBits = shire_esr_pkg::ShirePllDataBits;
  localparam int unsigned PllNumDataBytes = PllDataBits / 8;
  localparam int unsigned PllClockDivideBits = 4;
  localparam int unsigned PllClockRegPtrBits = 6;
  localparam int unsigned PllClockRegNumBits = 4;
  localparam int unsigned PllConfBits = shire_esr_pkg::ShirePllConfBits;

  localparam int unsigned DllAddrBits = 3;
  localparam int unsigned DllDataBits = shire_esr_pkg::ShireDllDataBits;
  localparam int unsigned DllNumDataBytes = DllDataBits / 8;
  localparam int unsigned DllClockDivideBits = 4;
  localparam int unsigned DllClockRegPtrBits = 3;
  localparam int unsigned DllClockRegNumBits = 3;
  localparam int unsigned DllConfBits = shire_esr_pkg::ShireDllConfBits;
  /* verilator lint_on UNUSEDPARAM */

  typedef struct packed {
    logic ndmreset;
    logic halt_req;
    logic resume_req;
    logic hart_reset_req;
    logic tbox_halt_req;
    logic tbox_resume_req;
    logic rbox_halt_req;
    logic rbox_resume_req;
    logic rbox_reset_req;
  } bpam_ms_rc_gpio_t;

  typedef struct packed {
    logic halt_req;
    logic resume_req;
    logic tbox_halt_req;
    logic tbox_resume_req;
    logic rbox_halt_req;
    logic rbox_resume_req;
  } bpam_ms_rc_event_t;

  typedef struct packed {
    bpam_ms_rc_gpio_t  gpio;
    bpam_ms_rc_event_t events;
  } bpam_run_control_t;

  typedef struct packed {
    logic anyunavailable;
    logic allhavereset;
    logic anyhavereset;
    logic allresumeack;
    logic anyresumeack;
    logic allrunning;
    logic anyrunning;
    logic allhalted;
    logic anyhalted2;
    logic anyhalted1;
    logic anyhalted0;
  } esr_and_or_tree_l2_t;

  typedef logic [NocIntNum-1:0] noc_err_status_t;

endpackage : shire_channel_leaves_pkg
