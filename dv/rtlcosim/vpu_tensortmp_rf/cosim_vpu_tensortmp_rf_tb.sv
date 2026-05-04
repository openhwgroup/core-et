// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_vpu_tensortmp_rf_tb
  import minion_pkg::*;
  import vpu_pkg::*;
(
  input  logic                      clk_i,
  input  logic [VpuTentmpAddrW-1:0] rd_addr_i,
  input  logic                      wr_en_i,
  input  logic                      wr_en_early_i,
  input  logic [VpuTentmpAddrW-1:0] wr_addr_i,
  input  logic [VpuDataSSz-1:0]     wr_data_i,
  output logic [VpuDataSSz-1:0]     orig_rd_data_o,
  output logic [VpuDataSSz-1:0]     new_rd_data_o
);

  vpu_tensortmp_rf_orig u_orig (
    .clock       (clk_i),
    .rd_addr     (rd_addr_i),
    .rd_data     (orig_rd_data_o),
    .wr_en       (wr_en_i),
    .wr_en_early (wr_en_early_i),
    .wr_addr     (wr_addr_i),
    .wr_data     (wr_data_i)
  );

  vpu_tensortmp_rf u_new (
    .clk_i,
    .rd_addr_i,
    .rd_data_o   (new_rd_data_o),
    .wr_en_i,
    .wr_en_early_i,
    .wr_addr_i,
    .wr_data_i
  );

endmodule : cosim_vpu_tensortmp_rf_tb
