// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"
module neigh_hv_logic_ipi(
  input  logic [`THREADS_PER_N-1:0]          esr_to_neigh_ipi_trigger,
  output logic                               esr_to_neigh_ipi_trigger_push
);

assign esr_to_neigh_ipi_trigger_push = |esr_to_neigh_ipi_trigger;

endmodule
