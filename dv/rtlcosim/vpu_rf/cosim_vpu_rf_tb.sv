// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_vpu_rf_tb
  import minion_pkg::*;
  import vpu_pkg::*;
(
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
  output logic [VpuDataSSz-1:0] orig_rd_data_0_o,
  output logic [VpuDataSSz-1:0] orig_rd_data_1_o,
  output logic [VpuDataSSz-1:0] orig_rd_data_2_o,
  output logic [VpuDataSSz-1:0] new_rd_data_0_o,
  output logic [VpuDataSSz-1:0] new_rd_data_1_o,
  output logic [VpuDataSSz-1:0] new_rd_data_2_o
);

  logic [2:0]                   rd_thread_id;
  logic [2:0][FregAddrSize-1:0] rd_addr;
  logic [1:0]                   wr_thread_id;
  logic [1:0][FregAddrSize-1:0] wr_addr;
  logic [1:0][VpuDataSSz-1:0]   wr_data;
  logic [2:0][VpuDataSSz-1:0]   orig_rd_data;
  logic [2:0][VpuDataSSz-1:0]   new_rd_data;

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

  assign orig_rd_data_0_o = orig_rd_data[0];
  assign orig_rd_data_1_o = orig_rd_data[1];
  assign orig_rd_data_2_o = orig_rd_data[2];
  assign new_rd_data_0_o = new_rd_data[0];
  assign new_rd_data_1_o = new_rd_data[1];
  assign new_rd_data_2_o = new_rd_data[2];

  vpu_rf_orig #(
    .USE_MMI (0)
  ) u_orig (
    .clock       (clk_i),
    .rd_en       (rd_en_i),
    .rd_thrid    (rd_thread_id),
    .rd_addr     (rd_addr),
    .rd_data     (orig_rd_data),
    .wr_en       (wr_en_i),
    .wr_en_early (wr_en_early_i),
    .wr_thrid    (wr_thread_id),
    .wr_mask     (wr_mask_i),
    .wr_addr     (wr_addr),
    .wr_data     (wr_data)
  );

  vpu_rf #(
    .UseMmi (1'b0)
  ) u_new (
    .rd_en_i        (rd_en_i),
    .rd_thread_id_i (rd_thread_id),
    .rd_addr_i      (rd_addr),
    .rd_data_o      (new_rd_data),
    .clk_i          (clk_i),
    .wr_en_i        (wr_en_i),
    .wr_en_early_i  (wr_en_early_i),
    .wr_thread_id_i (wr_thread_id),
    .wr_mask_i      (wr_mask_i),
    .wr_addr_i      (wr_addr),
    .wr_data_i      (wr_data)
  );

endmodule : cosim_vpu_rf_tb
