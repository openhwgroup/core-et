// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_fifo_tb (
  input  logic       clk_i,
  input  logic       rst_ni,
  input  logic       push_early_i,
  input  logic       push_i,
  input  logic [7:0] push_data_i,
  input  logic       pop_i,

  output logic       orig_ff_full_o,
  output logic       orig_ff_full_next_o,
  output logic       orig_ff_valid_o,
  output logic       orig_ff_valid_next_o,
  output logic [7:0] orig_ff_pop_data_o,
  output logic       new_ff_full_o,
  output logic       new_ff_full_next_o,
  output logic       new_ff_valid_o,
  output logic       new_ff_valid_next_o,
  output logic [7:0] new_ff_pop_data_o,

  output logic       orig_lat_full_o,
  output logic       orig_lat_full_next_o,
  output logic       orig_lat_valid_o,
  output logic       orig_lat_valid_next_o,
  output logic [7:0] orig_lat_pop_data_o,
  output logic       new_lat_full_o,
  output logic       new_lat_full_next_o,
  output logic       new_lat_valid_o,
  output logic       new_lat_valid_next_o,
  output logic [7:0] new_lat_pop_data_o
);

  logic reset;
  assign reset = ~rst_ni;

  gen_fifo #(
    .ELEM_SIZE(8),
    .NUM_ELEMS(4),
    .USE_LATCHES(0)
  ) u_orig_ff (
    .clock     (clk_i),
    .reset     (reset),
    .push_early(push_early_i),
    .push      (push_i),
    .push_data (push_data_i),
    .full      (orig_ff_full_o),
    .full_next (orig_ff_full_next_o),
    .pop       (pop_i),
    .pop_data  (orig_ff_pop_data_o),
    .valid     (orig_ff_valid_o),
    .valid_next(orig_ff_valid_next_o)
  );

  prim_fifo #(
    .Width(8),
    .Depth(4),
    .UseLatches(1'b0)
  ) u_new_ff (
    .clk_i,
    .rst_ni,
    .push_early_i,
    .push_i,
    .push_data_i,
    .full_o      (new_ff_full_o),
    .full_next_o (new_ff_full_next_o),
    .pop_i,
    .pop_data_o  (new_ff_pop_data_o),
    .valid_o     (new_ff_valid_o),
    .valid_next_o(new_ff_valid_next_o)
  );

  gen_fifo #(
    .ELEM_SIZE(8),
    .NUM_ELEMS(4),
    .USE_LATCHES(1)
  ) u_orig_lat (
    .clock     (clk_i),
    .reset     (reset),
    .push_early(push_early_i),
    .push      (push_i),
    .push_data (push_data_i),
    .full      (orig_lat_full_o),
    .full_next (orig_lat_full_next_o),
    .pop       (pop_i),
    .pop_data  (orig_lat_pop_data_o),
    .valid     (orig_lat_valid_o),
    .valid_next(orig_lat_valid_next_o)
  );

  prim_fifo #(
    .Width(8),
    .Depth(4),
    .UseLatches(1'b1)
  ) u_new_lat (
    .clk_i,
    .rst_ni,
    .push_early_i,
    .push_i,
    .push_data_i,
    .full_o      (new_lat_full_o),
    .full_next_o (new_lat_full_next_o),
    .pop_i,
    .pop_data_o  (new_lat_pop_data_o),
    .valid_o     (new_lat_valid_o),
    .valid_next_o(new_lat_valid_next_o)
  );

endmodule
