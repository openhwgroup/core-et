// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"
module neigh_hv_logic_pwr_ctrl_tdr_isolate (
  input  logic  dft_tdr_ctrl_in,
  input  logic  pwr_ctrl_glb_isolate_in,
  output logic  pwr_ctrl_glb_isolate_out
);
assign pwr_ctrl_glb_isolate_out = dft_tdr_ctrl_in ? 1'b0 : pwr_ctrl_glb_isolate_in;

endmodule
