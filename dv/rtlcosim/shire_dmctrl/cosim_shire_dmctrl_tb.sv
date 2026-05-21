// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_shire_dmctrl_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [9:0] dmctrl_bits_i,
  output logic [9:0] orig_dmctrl_bits_o,
  output logic [9:0] new_dmctrl_bits_o
);

  esr_ms_dmctrl_t orig_dmctrl_i;
  esr_ms_dmctrl_t orig_dmctrl_o;
  esr_pkg::esr_ms_dmctrl_t new_dmctrl_i;
  esr_pkg::esr_ms_dmctrl_t new_dmctrl_o;

  assign orig_dmctrl_i = dmctrl_bits_i;
  assign new_dmctrl_i = dmctrl_bits_i;
  assign orig_dmctrl_bits_o = orig_dmctrl_o;
  assign new_dmctrl_bits_o = new_dmctrl_o;

  shire_dmctrl_orig u_orig (
    .clock      (clk_i),
    .reset      (!rst_ni),
    .dmctrl_in  (orig_dmctrl_i),
    .dmctrl_out (orig_dmctrl_o)
  );

  shire_dmctrl u_new (
    .clk_i    (clk_i),
    .rst_ni   (rst_ni),
    .dmctrl_i (new_dmctrl_i),
    .dmctrl_o (new_dmctrl_o)
  );

endmodule
