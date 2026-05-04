// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module vpu_tensorb_rf_tb
  import minion_pkg::*;
  import vpu_pkg::*;
(
  input  logic                      clk_i,
  input  logic [VpuTenbRfAddrW-1:0] rd_addr_i,
  output logic [VpuDataSSz-1:0]     rd_data_o,
  input  logic                      wr_en_i,
  input  logic                      wr_en_early_i,
  input  logic [VpuTenbRfAddrW-1:0] wr_addr_i,
  input  logic [VpuDataSSz-1:0]     wr_data_i
);

  vpu_tensorb_rf u_dut (
    .clk_i,
    .rd_addr_i,
    .rd_data_o,
    .wr_en_i,
    .wr_en_early_i,
    .wr_addr_i,
    .wr_data_i
  );

endmodule : vpu_tensorb_rf_tb
