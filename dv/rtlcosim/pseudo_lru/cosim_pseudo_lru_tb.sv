// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_pseudo_lru_tb (
  input  logic       clk_i,
  input  logic       rst_ni,
  input  logic       access_i,
  input  logic [2:0] index_in_i,

  output logic [2:0] orig_index_out_o,
  output logic [2:0] new_index_out_o
);
  logic reset;

  assign reset = !rst_ni;

  pseudo_lru_orig #(
    .N_WAYS(8)
  ) u_orig (
    .clock    (clk_i),
    .reset    (reset),
    .access   (access_i),
    .index_in (index_in_i),
    .index_out(orig_index_out_o)
  );

  pseudo_lru #(
    .NWays(8)
  ) u_new (
    .clk_i,
    .rst_ni,
    .access_i,
    .index_in_i,
    .index_out_o(new_index_out_o)
  );
endmodule
