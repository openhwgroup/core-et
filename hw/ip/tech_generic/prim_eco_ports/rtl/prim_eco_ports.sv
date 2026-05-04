// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// prim_eco_ports -- generic/default ECO spare-port wrapper.
//
// The generic implementation matches the non-std-cell original et_eco_ports
// fallback: output ECO pins are tied low and input ECO pins are preserved in
// the cone so they remain available for later technology-specific buffering.

module prim_eco_ports #(
  parameter int unsigned InputWidth = 4,
  parameter int unsigned OutputWidth = 4
) (
  output logic [OutputWidth-1:0] eco_o,
  input  logic [InputWidth-1:0]  eco_i
);

  logic unused_ok;

  assign eco_o = '0;
  assign unused_ok = &{1'b0, eco_i};

endmodule
