// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// evl_wire.vh
//
// Used to conditionally remove keyword wire from structure declarations.

`ifdef ET_SIMULATION
   `ifndef EVL_WIRE
      `define EVL_WIRE wire
   `endif // ifndef EVL_WIRE
`else // ifdef ET_SIMULATION
   `ifndef EVL_WIRE
      `define EVL_WIRE
   `endif // ifndef EVL_WIRE
`endif // !ifdef ET_SIMULATION
