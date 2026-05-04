// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// minion_dcache_lru_array — Verilator wrapper.

module minion_dcache_lru_array_tb (
  input  logic        clk_i,
  input  logic        rst_ni,
  input  logic [3:0]  read_set_i,
  output logic [15:0] read_data_o,
  input  logic        write_en_i,
  input  logic [3:0]  write_set_i,
  input  logic [15:0] write_data_i
);

  minion_dcache_lru_array u_dut (
    .clk_i,
    .rst_ni,
    .read_set_i,
    .read_data_o,
    .write_en_i,
    .write_set_i,
    .write_data_i
  );

endmodule
