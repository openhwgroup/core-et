// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_hv_logic_pwr_ctrl_tdr_isolate (
  input  logic dft_tdr_ctrl_i,
  input  logic pwr_ctrl_glb_isolate_i,
  output logic pwr_ctrl_glb_isolate_o
);

  assign pwr_ctrl_glb_isolate_o = dft_tdr_ctrl_i ? 1'b0 : pwr_ctrl_glb_isolate_i;

endmodule : neigh_hv_logic_pwr_ctrl_tdr_isolate
