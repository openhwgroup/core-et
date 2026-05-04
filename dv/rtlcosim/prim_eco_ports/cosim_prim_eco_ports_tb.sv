// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original et_eco_ports vs new prim_eco_ports.

module cosim_prim_eco_ports_tb (
  input  logic [9:0] eco_i,
  output logic [9:0] new_eco_o,
  output logic [9:0] orig_eco_o
);

  prim_eco_ports #(
    .InputWidth (10),
    .OutputWidth(10)
  ) u_new (
    .eco_i(eco_i),
    .eco_o(new_eco_o)
  );

  et_eco_ports #(
    .INPUT_WIDTH (10),
    .OUTPUT_WIDTH(10)
  ) u_orig (
    .eco_i(eco_i),
    .eco_o(orig_eco_o)
  );

endmodule
