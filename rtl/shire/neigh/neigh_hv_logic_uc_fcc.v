// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module neigh_hv_logic_uc_fcc (
  input logic [`MIN_PER_N-1:0]  uc_to_neigh_fcc, 
  output logic                  neigh_uc_fcc_vcfifo_push
);
assign neigh_uc_fcc_vcfifo_push = |uc_to_neigh_fcc;

endmodule
