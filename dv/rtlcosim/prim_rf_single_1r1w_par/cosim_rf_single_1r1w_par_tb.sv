// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_rf_single_1r1w_par_tb #(
  parameter int unsigned WIDTH = 32
) (
  input  logic             clk_i,
  output logic [WIDTH-1:0] new_rd_data_o,
  output logic [WIDTH-1:0] orig_rd_data_o,
  input  logic [WIDTH-1:0] wr_data_i,
  input  logic             wr_data_en_1p_i,
  input  logic             wr_en_i
);

  prim_rf_single_1r1w_par #(
    .Width (WIDTH)
  ) u_new (
    .clk_i,
    .rd_data_o (new_rd_data_o),
    .wr_data_i,
    .wr_data_en_1p_i,
    .wr_en_i
  );

  rf_latch_single_1r_1w_par #(
    .WIDTH (WIDTH)
  ) u_orig (
    .clk             (clk_i),
    .rd_data_a       (orig_rd_data_o),
    .wr_data_a       (wr_data_i),
    .wr_data_a_en_1p (wr_data_en_1p_i),
    .wr_en_a         (wr_en_i),
    .test_en         (1'b0)
  );

endmodule
