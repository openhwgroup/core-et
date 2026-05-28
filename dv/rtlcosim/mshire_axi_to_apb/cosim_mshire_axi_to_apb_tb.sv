// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_mshire_axi_to_apb_tb (
  input  logic clk_i,
  input  logic rst_ni,

  input  logic ar_valid_i,
  input  logic aw_valid_i,
  input  logic [axi_pkg::ScSlaveIdSize-1:0] ar_id_i,
  input  logic [axi_pkg::ScSlaveIdSize-1:0] aw_id_i,
  input  logic [axi_pkg::ScMasterAddrSize-1:0] ar_addr_i,
  input  logic [axi_pkg::ScMasterAddrSize-1:0] aw_addr_i,
  input  logic [63:0] wdata_i,
  input  logic b_ready_i,
  input  logic r_ready_i,
  input  logic [63:0] apb_prdata_i,
  input  logic apb_pready_i,
  input  logic apb_pslverr_i,

  output logic new_ready_o,
  output logic orig_ready_o,
  output logic new_b_valid_o,
  output logic orig_b_valid_o,
  output logic [axi_pkg::ScSlaveIdSize-1:0] new_b_id_o,
  output logic [axi_pkg::ScSlaveIdSize-1:0] orig_b_id_o,
  output logic [1:0] new_b_resp_o,
  output logic [1:0] orig_b_resp_o,
  output logic new_r_valid_o,
  output logic orig_r_valid_o,
  output logic [axi_pkg::ScSlaveIdSize-1:0] new_r_id_o,
  output logic [axi_pkg::ScSlaveIdSize-1:0] orig_r_id_o,
  output logic [1:0] new_r_resp_o,
  output logic [1:0] orig_r_resp_o,
  output logic new_r_last_o,
  output logic orig_r_last_o,
  output logic [63:0] new_r_data0_o,
  output logic [63:0] orig_r_data0_o,
  output logic [63:0] new_r_data1_o,
  output logic [63:0] orig_r_data1_o,
  output logic [63:0] new_r_data2_o,
  output logic [63:0] orig_r_data2_o,
  output logic [63:0] new_r_data3_o,
  output logic [63:0] orig_r_data3_o,
  output logic [39:0] new_apb_paddr_o,
  output logic [39:0] orig_apb_paddr_o,
  output logic new_apb_penable_o,
  output logic orig_apb_penable_o,
  output logic new_apb_psel_o,
  output logic orig_apb_psel_o,
  output logic [63:0] new_apb_pwdata_o,
  output logic [63:0] orig_apb_pwdata_o,
  output logic new_apb_pwrite_o,
  output logic orig_apb_pwrite_o
);

  logic reset;
  assign reset = ~rst_ni;

  shire_sbm_pkg::sbm_axi_a_t new_ar;
  shire_sbm_pkg::sbm_axi_a_t new_aw;
  shire_sbm_pkg::sbm_axi_w_t new_w;
  axi_pkg::sys_slave_b_t new_b;
  axi_pkg::sys_slave_r_t new_r;

  sbm_slave_axi_AR_channel_t orig_ar;
  sbm_slave_axi_AW_channel_t orig_aw;
  sbm_slave_axi_W_channel_t orig_w;
  sys_slave_axi_B_channel_t orig_b;
  sys_slave_axi_R_channel_t orig_r;

  always_comb begin
    new_ar = '{id: ar_id_i, addr: ar_addr_i};
    new_aw = '{id: aw_id_i, addr: aw_addr_i};
    new_w.data = wdata_i;

    orig_ar = '0;
    orig_ar.ID = ar_id_i;
    orig_ar.ADDR = ar_addr_i;
    orig_aw = '0;
    orig_aw.ID = aw_id_i;
    orig_aw.ADDR = aw_addr_i;
    orig_w = '0;
    orig_w.DATA = wdata_i;
  end

  mshire_axi_to_apb #(
    .ApbAddrWidth  (40),
    .ApbDataWidth  (64),
    .ShiftDataEnRd (1'b1),
    .ResponseControl (1'b0)
  ) u_new (
    .clk_i,
    .rst_ni,
    .ready_o (new_ready_o),
    .axi_ar_i (new_ar),
    .axi_ar_valid_i (ar_valid_i),
    .axi_aw_i (new_aw),
    .axi_aw_valid_i (aw_valid_i),
    .axi_w_i (new_w),
    .axi_b_o (new_b),
    .axi_b_valid_o (new_b_valid_o),
    .axi_b_ready_i (b_ready_i),
    .axi_r_o (new_r),
    .axi_r_valid_o (new_r_valid_o),
    .axi_r_ready_i (r_ready_i),
    .apb_paddr_o (new_apb_paddr_o),
    .apb_penable_o (new_apb_penable_o),
    .apb_prdata_i (apb_prdata_i),
    .apb_pready_i (apb_pready_i),
    .apb_psel_o (new_apb_psel_o),
    .apb_pslverr_i (apb_pslverr_i),
    .apb_pwdata_o (new_apb_pwdata_o),
    .apb_pwrite_o (new_apb_pwrite_o)
  );

  mshire_axi_to_apb_orig #(
    .sbm_apb_addr (40),
    .sbm_apb_data (64),
    .shift_data_en_rd (1),
    .response_control (0)
  ) u_orig (
    .clock (clk_i),
    .reset (reset),
    .ready (orig_ready_o),
    .sbm_mesh_slave_axi_AR (orig_ar),
    .sbm_mesh_slave_axi_ARVALID (ar_valid_i),
    .sbm_mesh_slave_axi_AW (orig_aw),
    .sbm_mesh_slave_axi_AWVALID (aw_valid_i),
    .sbm_mesh_slave_axi_W (orig_w),
    .sbm_mesh_slave_axi_B (orig_b),
    .sbm_mesh_slave_axi_BVALID (orig_b_valid_o),
    .sbm_mesh_slave_axi_BREADY (b_ready_i),
    .sbm_mesh_slave_axi_R (orig_r),
    .sbm_mesh_slave_axi_RVALID (orig_r_valid_o),
    .sbm_mesh_slave_axi_RREADY (r_ready_i),
    .apb_paddr (orig_apb_paddr_o),
    .apb_penable (orig_apb_penable_o),
    .apb_prdata (apb_prdata_i),
    .apb_pready (apb_pready_i),
    .apb_psel (orig_apb_psel_o),
    .apb_pslverr (apb_pslverr_i),
    .apb_pwdata (orig_apb_pwdata_o),
    .apb_pwrite (orig_apb_pwrite_o)
  );

  assign new_b_id_o = new_b.id;
  assign new_b_resp_o = new_b.resp;
  assign new_r_id_o = new_r.id;
  assign new_r_resp_o = new_r.resp;
  assign new_r_last_o = new_r.last;
  assign new_r_data0_o = new_r.data[63:0];
  assign new_r_data1_o = new_r.data[127:64];
  assign new_r_data2_o = new_r.data[191:128];
  assign new_r_data3_o = new_r.data[255:192];

  assign orig_b_id_o = orig_b.ID;
  assign orig_b_resp_o = orig_b.RESP;
  assign orig_r_id_o = orig_r.ID;
  assign orig_r_resp_o = orig_r.RESP;
  assign orig_r_last_o = orig_r.LAST;
  assign orig_r_data0_o = orig_r.DATA[63:0];
  assign orig_r_data1_o = orig_r.DATA[127:64];
  assign orig_r_data2_o = orig_r.DATA[191:128];
  assign orig_r_data3_o = orig_r.DATA[255:192];

endmodule : cosim_mshire_axi_to_apb_tb
