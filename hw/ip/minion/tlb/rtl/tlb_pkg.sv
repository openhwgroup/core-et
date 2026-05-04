// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// TLB-local types.
//
// These types are internal to the minion TLB implementation. Shared VM/PTW
// request and response types remain in `minion_pkg`.

package tlb_pkg;

  import minion_pkg::*;

  typedef enum logic [1:0] {
    TlbStateReady   = 2'b00,
    TlbStateRequest = 2'b01,
    TlbStateWait    = 2'b10
  } tlb_state_e;

  typedef struct packed {
    logic                         ptw_access_fault;
    logic                         expected_ex_mode;
    logic                         dirty;
    logic                         access;
    logic                         sx;
    logic                         sw;
    logic                         sr;
    logic                         u;
    logic [PaSize-1:PaUntransSize] ppn;
  } tlb_entry_data_t;

endpackage : tlb_pkg
