// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_hv_logic_ipi #(
  parameter int unsigned ThreadsPerN = neigh_hv_logic_pkg::ThreadsPerN
) (
  input  logic [ThreadsPerN-1:0] esr_to_neigh_ipi_trigger_i,
  output logic                   esr_to_neigh_ipi_trigger_push_o
);

  assign esr_to_neigh_ipi_trigger_push_o = |esr_to_neigh_ipi_trigger_i;

endmodule : neigh_hv_logic_ipi
