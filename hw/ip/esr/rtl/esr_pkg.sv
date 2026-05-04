// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// ESR package.
//
// Shared ET System Register types and address constants used by the
// standalone-shell ESR blocks.

package esr_pkg;
  /* verilator lint_off UNUSEDPARAM */  // Package exports additional ESR constants for modules still being ported.

  localparam int unsigned EsrApbDataWidth       = 64;
  localparam int unsigned EsrSpioApbAddrWidth   = 2;
  localparam int unsigned EsrSpioApbPpAddrWidth = EsrSpioApbAddrWidth + 2;

  typedef struct packed {
    logic [31:0] reserved2;
    logic        haltreq;
    logic        resumereq;
    logic        hartreset;
    logic        ackhavereset;
    logic [23:0] reserved1;
    logic        setresethaltreq;
    logic        clrresethaltreq;
    logic        ndmreset;
    logic        reserved0;
  } esr_spdmctrl_t;

  typedef struct packed {
    logic [56:0] reserved;
    logic        error;
    logic        exception;
    logic        busy;
    logic        havereset;
    logic        resumeack;
    logic        running;
    logic        halted;
  } esr_sphastatus_t;

  localparam logic [EsrSpioApbPpAddrWidth-1:0] EsrSpioAddrSpdmctrl   = 4'hA;
  localparam logic [EsrSpioApbPpAddrWidth-1:0] EsrSpioAddrSphastatus = 4'hB;

  /* verilator lint_on UNUSEDPARAM */
endpackage
