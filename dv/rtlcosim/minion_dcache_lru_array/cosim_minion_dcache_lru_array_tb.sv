// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original dcache_lru_array vs new minion_dcache_lru_array.

`include "soc.vh"

module cosim_minion_dcache_lru_array_tb (
  input  logic        clk_i,
  input  logic        rst_ni,
  input  logic [3:0]  read_set_i,
  output logic [15:0] new_read_data_o,
  output logic [15:0] orig_read_data_o,
  input  logic        write_en_i,
  input  logic [3:0]  write_set_i,
  input  logic [15:0] write_data_i
);

  logic reset;
  assign reset = ~rst_ni;

  minion_dcache_lru_array u_new (
    .clk_i,
    .rst_ni,
    .read_set_i,
    .read_data_o (new_read_data_o),
    .write_en_i,
    .write_set_i,
    .write_data_i
  );

  dcache_lru_array u_orig (
    .clock      (clk_i),
    .reset      (reset),
    .read_set   (read_set_i),
    .read_data  (orig_read_data_o),
    .write_en   (write_en_i),
    .write_set  (write_set_i),
    .write_data (write_data_i)
  );

endmodule
