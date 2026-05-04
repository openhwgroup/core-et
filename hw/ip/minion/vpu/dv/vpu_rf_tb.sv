// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module vpu_rf_tb
  import minion_pkg::*;
  import vpu_pkg::*;
#(
  parameter bit UseMmi = UseMmiVpuRfDefault
) (
  input  logic                  clk_i,
  input  logic [2:0]            rd_en_i,
  input  logic                  rd_thread_id_0_i,
  input  logic                  rd_thread_id_1_i,
  input  logic                  rd_thread_id_2_i,
  input  logic [FregAddrSize-1:0] rd_addr_0_i,
  input  logic [FregAddrSize-1:0] rd_addr_1_i,
  input  logic [FregAddrSize-1:0] rd_addr_2_i,
  input  logic [1:0]            wr_en_i,
  input  logic [1:0]            wr_en_early_i,
  input  logic                  wr_thread_id_0_i,
  input  logic                  wr_thread_id_1_i,
  input  logic [1:0]            wr_mask_i,
  input  logic [FregAddrSize-1:0] wr_addr_0_i,
  input  logic [FregAddrSize-1:0] wr_addr_1_i,
  input  logic [VpuDataSSz-1:0] wr_data_0_i,
  input  logic [VpuDataSSz-1:0] wr_data_1_i,
  output logic                  use_mmi_o,
  output logic [VpuDataSSz-1:0] rd_data_0_o,
  output logic [VpuDataSSz-1:0] rd_data_1_o,
  output logic [VpuDataSSz-1:0] rd_data_2_o
);

  logic [2:0]                   rd_thread_id;
  logic [2:0][FregAddrSize-1:0] rd_addr;
  logic [1:0]                   wr_thread_id;
  logic [1:0][FregAddrSize-1:0] wr_addr;
  logic [1:0][VpuDataSSz-1:0]   wr_data;
  logic [2:0][VpuDataSSz-1:0]   rd_data;

  assign use_mmi_o = UseMmi;

  assign rd_thread_id[0] = rd_thread_id_0_i;
  assign rd_thread_id[1] = rd_thread_id_1_i;
  assign rd_thread_id[2] = rd_thread_id_2_i;
  assign rd_addr[0] = rd_addr_0_i;
  assign rd_addr[1] = rd_addr_1_i;
  assign rd_addr[2] = rd_addr_2_i;
  assign wr_thread_id[0] = wr_thread_id_0_i;
  assign wr_thread_id[1] = wr_thread_id_1_i;
  assign wr_addr[0] = wr_addr_0_i;
  assign wr_addr[1] = wr_addr_1_i;
  assign wr_data[0] = wr_data_0_i;
  assign wr_data[1] = wr_data_1_i;
  assign rd_data_0_o = rd_data[0];
  assign rd_data_1_o = rd_data[1];
  assign rd_data_2_o = rd_data[2];

  vpu_rf #(
    .UseMmi (UseMmi)
  ) u_dut (
    .rd_en_i,
    .rd_thread_id_i (rd_thread_id),
    .rd_addr_i      (rd_addr),
    .rd_data_o      (rd_data),
    .clk_i,
    .wr_en_i,
    .wr_en_early_i,
    .wr_thread_id_i (wr_thread_id),
    .wr_mask_i,
    .wr_addr_i      (wr_addr),
    .wr_data_i      (wr_data)
  );

endmodule : vpu_rf_tb
