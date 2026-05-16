// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_hv_logic_clock_feedback (
  input  logic clk_shire_i,
  input  logic clk_neigh_i,
  output logic dll_feedback_shire_o,
  output logic dll_feedback_neigh_o
);

  assign dll_feedback_shire_o = ~clk_shire_i;
  assign dll_feedback_neigh_o = clk_neigh_i;

endmodule : neigh_hv_logic_clock_feedback
