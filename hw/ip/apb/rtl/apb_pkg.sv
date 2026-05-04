// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// APB3 protocol package.
//
// Defines parameterized request/response structs for the AMBA APB3 bus.
// Used by ESR register blocks (rbox, tbox, etc.) for configuration access.

package apb_pkg;

  // Default widths matching the CORE-ET ESR APB interface.
  localparam int unsigned DefaultAddrWidth = 16;
  localparam int unsigned DefaultDataWidth = 64;

  // ── APB request (master-to-slave) ─────────────────────────
  // Parameterized via AddrWidth / DataWidth at instantiation.

  typedef struct packed {
    logic [DefaultAddrWidth-1:0] paddr;
    logic                        pwrite;
    logic                        psel;
    logic                        penable;
    logic [DefaultDataWidth-1:0] pwdata;
  } req_t;

  // ── APB response (slave-to-master) ────────────────────────

  typedef struct packed {
    logic                        pready;
    logic [DefaultDataWidth-1:0] prdata;
    logic                        pslverr;
  } rsp_t;

endpackage
