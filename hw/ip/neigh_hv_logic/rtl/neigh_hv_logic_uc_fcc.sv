// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_hv_logic_uc_fcc #(
  parameter int unsigned MinPerN = neigh_hv_logic_pkg::MinPerN
) (
  input  logic [MinPerN-1:0] uc_to_neigh_fcc_i,
  output logic               neigh_uc_fcc_vcfifo_push_o
);

  assign neigh_uc_fcc_vcfifo_push_o = |uc_to_neigh_fcc_i;

endmodule : neigh_hv_logic_uc_fcc
