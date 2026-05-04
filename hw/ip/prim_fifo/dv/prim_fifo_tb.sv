// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module prim_fifo_tb (
  input  logic       clk_i,
  input  logic       rst_ni,
  input  logic       push_early_i,
  input  logic       push_i,
  input  logic [7:0] push_data_i,
  input  logic       pop_i,
  output logic       ff_full_o,
  output logic       ff_full_next_o,
  output logic       ff_valid_o,
  output logic       ff_valid_next_o,
  output logic [7:0] ff_pop_data_o,
  output logic       lat_full_o,
  output logic       lat_full_next_o,
  output logic       lat_valid_o,
  output logic       lat_valid_next_o,
  output logic [7:0] lat_pop_data_o
);

  prim_fifo #(
    .Width(8),
    .Depth(4),
    .UseLatches(1'b0)
  ) u_ff_fifo (
    .clk_i,
    .rst_ni,
    .push_early_i,
    .push_i,
    .push_data_i,
    .full_o      (ff_full_o),
    .full_next_o (ff_full_next_o),
    .pop_i,
    .pop_data_o  (ff_pop_data_o),
    .valid_o     (ff_valid_o),
    .valid_next_o(ff_valid_next_o)
  );

  prim_fifo #(
    .Width(8),
    .Depth(4),
    .UseLatches(1'b1)
  ) u_latched_fifo (
    .clk_i,
    .rst_ni,
    .push_early_i,
    .push_i,
    .push_data_i,
    .full_o      (lat_full_o),
    .full_next_o (lat_full_next_o),
    .pop_i,
    .pop_data_o  (lat_pop_data_o),
    .valid_o     (lat_valid_o),
    .valid_next_o(lat_valid_next_o)
  );

endmodule
