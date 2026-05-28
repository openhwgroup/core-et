// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_neigh_hv_logic_icache_err_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic ff_datain_i,
  output logic orig_ff_dataout_o,
  output logic new_ff_dataout_o
);

  neigh_hv_logic_icache_err u_new (
    .rst_pop_ni    (rst_ni),
    .clk_pop_i     (clk_i),
    .ff_datain_i   (ff_datain_i),
    .ff_dataout_o  (new_ff_dataout_o)
  );

  neigh_hv_logic_icache_err_orig u_orig (
    .reset_pop   (!rst_ni),
    .clock_pop   (clk_i),
    .ff_datain   (ff_datain_i),
    .ff_dataout  (orig_ff_dataout_o)
  );

endmodule : cosim_neigh_hv_logic_icache_err_tb
