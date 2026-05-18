// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_sbm_top_tb (
  input  logic clk_i,
  input  logic rst_ni,

  input  logic [shire_sbm_pkg::NumNeigh-1:0] rst_neigh_ni,
  input  logic rst_rbox_ni,
  input  logic rst_shire_cache_ni,
  input  logic [7:0] shire_virtual_id_i,
  input  logic ar_valid_i,
  input  logic aw_valid_i,
  input  logic [axi_pkg::ScSlaveIdSize-1:0] ar_id_i,
  input  logic [axi_pkg::ScSlaveIdSize-1:0] aw_id_i,
  input  logic [axi_pkg::ScMasterAddrSize-1:0] ar_addr_i,
  input  logic [axi_pkg::ScMasterAddrSize-1:0] aw_addr_i,
  input  logic [axi_pkg::AxLenSize-1:0] ar_len_i,
  input  logic [axi_pkg::AxLenSize-1:0] aw_len_i,
  input  logic [63:0] wdata_i,
  input  logic b_ready_i,
  input  logic r_ready_i,
  input  logic [shire_sbm_pkg::ShireApbInterfaces-1:0] apb_pready_i,
  input  logic [shire_sbm_pkg::ShireApbInterfaces-1:0] apb_pslverr_i,
  input  logic [63:0] apb_prdata_i,

  output logic new_ar_ready_o,
  output logic orig_ar_ready_o,
  output logic new_aw_ready_o,
  output logic orig_aw_ready_o,
  output logic new_w_ready_o,
  output logic orig_w_ready_o,
  output logic new_credit_return_write_o,
  output logic orig_credit_return_write_o,
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
  output logic [shire_sbm_pkg::ShireApbInterfaces-1:0] new_apb_psel_o,
  output logic [shire_sbm_pkg::ShireApbInterfaces-1:0] orig_apb_psel_o,
  output logic [shire_sbm_pkg::ShireApbInterfaces-1:0] new_apb_penable_o,
  output logic [shire_sbm_pkg::ShireApbInterfaces-1:0] orig_apb_penable_o,
  output logic [shire_sbm_pkg::ShireApbInterfaces-1:0] new_apb_pwrite_o,
  output logic [shire_sbm_pkg::ShireApbInterfaces-1:0] orig_apb_pwrite_o,
  output logic [18:0] new_apb_paddr_o [shire_sbm_pkg::ShireApbInterfaces],
  output logic [18:0] orig_apb_paddr_o [shire_sbm_pkg::ShireApbInterfaces],
  output logic [63:0] new_apb_pwdata_o [shire_sbm_pkg::ShireApbInterfaces],
  output logic [63:0] orig_apb_pwdata_o [shire_sbm_pkg::ShireApbInterfaces]
);

  logic reset;
  assign reset = ~rst_ni;

  axi_pkg::sys_slave_ar_t new_ar;
  axi_pkg::sys_slave_aw_t new_aw;
  axi_pkg::sys_slave_w_t new_w;
  axi_pkg::sys_slave_b_t new_b;
  axi_pkg::sys_slave_r_t new_r;
  shire_sbm_pkg::shire_apb_req_t [shire_sbm_pkg::ShireApbInterfaces-1:0] new_apb_req;
  shire_sbm_pkg::shire_apb_rsp_t [shire_sbm_pkg::ShireApbInterfaces-1:0] new_apb_rsp;

  sys_slave_axi_AR_channel_t orig_ar;
  sys_slave_axi_AW_channel_t orig_aw;
  sys_slave_axi_W_channel_t orig_w;
  sys_slave_axi_B_channel_t orig_b;
  sys_slave_axi_R_channel_t orig_r;
  APB_to_all_ESRs_t [shire_sbm_pkg::ShireApbInterfaces-1:0] orig_apb_req;
  APB_from_all_ESRs_t [shire_sbm_pkg::ShireApbInterfaces-1:0] orig_apb_rsp;

  logic orig_bpam_pready;
  logic orig_bpam_pslverr;
  logic [63:0] orig_bpam_prdata;
  logic unused_orig_bpam;
  assign unused_orig_bpam = ^{orig_bpam_pready, orig_bpam_pslverr, orig_bpam_prdata};

  always_comb begin
    new_ar = '0;
    new_ar.id = ar_id_i;
    new_ar.addr = ar_addr_i;
    new_ar.len = ar_len_i;
    new_aw = '0;
    new_aw.id = aw_id_i;
    new_aw.addr = aw_addr_i;
    new_aw.len = aw_len_i;
    new_w = '0;
    new_w.data = {4{wdata_i}};
    new_w.strb = '1;
    new_w.last = 1'b1;

    orig_ar = '0;
    orig_ar.ID = ar_id_i;
    orig_ar.ADDR = ar_addr_i;
    orig_ar.LEN = ar_len_i;
    orig_aw = '0;
    orig_aw.ID = aw_id_i;
    orig_aw.ADDR = aw_addr_i;
    orig_aw.LEN = aw_len_i;
    orig_w = '0;
    orig_w.DATA = {4{wdata_i}};
    orig_w.STRB = '1;
    orig_w.LAST = 1'b1;

    for (int i = 0; i < shire_sbm_pkg::ShireApbInterfaces; i++) begin
      new_apb_rsp[i].pready = apb_pready_i[i];
      new_apb_rsp[i].pslverr = apb_pslverr_i[i];
      new_apb_rsp[i].prdata = apb_prdata_i;
      orig_apb_rsp[i].apb_pready = apb_pready_i[i];
      orig_apb_rsp[i].apb_pslverr = apb_pslverr_i[i];
      orig_apb_rsp[i].apb_prdata = apb_prdata_i;
      new_apb_psel_o[i] = new_apb_req[i].psel;
      new_apb_penable_o[i] = new_apb_req[i].penable;
      new_apb_pwrite_o[i] = new_apb_req[i].pwrite;
      new_apb_paddr_o[i] = new_apb_req[i].paddr;
      new_apb_pwdata_o[i] = new_apb_req[i].pwdata;
      orig_apb_psel_o[i] = orig_apb_req[i].apb_psel;
      orig_apb_penable_o[i] = orig_apb_req[i].apb_penable;
      orig_apb_pwrite_o[i] = orig_apb_req[i].apb_pwrite;
      orig_apb_paddr_o[i] = orig_apb_req[i].apb_paddr;
      orig_apb_pwdata_o[i] = orig_apb_req[i].apb_pwdata;
    end
  end

  sbm_top u_new (
    .clk_i,
    .rst_ni,
    .rst_neigh_ni,
    .rst_rbox_ni,
    .rst_shire_cache_ni,
    .shire_virtual_id_i ({11'b0, shire_virtual_id_i}),
    .sys_axi_ar_valid_i (ar_valid_i),
    .sys_axi_aw_valid_i (aw_valid_i),
    .sbm_write_credit_return_o (new_credit_return_write_o),
    .sys_axi_ar_i (new_ar),
    .sys_axi_ar_ready_o (new_ar_ready_o),
    .sys_axi_aw_i (new_aw),
    .sys_axi_aw_ready_o (new_aw_ready_o),
    .sys_axi_w_i (new_w),
    .sys_axi_w_ready_o (new_w_ready_o),
    .sys_axi_b_o (new_b),
    .sys_axi_b_valid_o (new_b_valid_o),
    .sys_axi_b_ready_i (b_ready_i),
    .sys_axi_r_o (new_r),
    .sys_axi_r_valid_o (new_r_valid_o),
    .sys_axi_r_ready_i (r_ready_i),
    .apb_req_o (new_apb_req),
    .apb_rsp_i (new_apb_rsp)
  );

  sbm_top_orig #(
    .sbm_interfaces   (shire_sbm_pkg::SbmInterfaces),
    .shire_interfaces (shire_sbm_pkg::ShireApbInterfaces),
    .sbm_apb_addr     (17),
    .sbm_apb_data     (64)
  ) u_orig (
    .clock (clk_i),
    .reset_c (reset),
    .reset_w (reset),
    .reset_d (reset),
    .reset_neigh (~rst_neigh_ni),
    .reset_rbox (~rst_rbox_ni),
    .reset_sc (~rst_shire_cache_ni),
    .shire_virtual_id (shire_virtual_id_i),
    .sbm_enable_read (ar_valid_i),
    .sbm_enable_write (aw_valid_i),
    .sbm_write_credit_return (orig_credit_return_write_o),
    .msg_lock_en_esperanto (1'b0),
    .bpam_paddr ('0),
    .bpam_penable (1'b0),
    .bpam_prdata (orig_bpam_prdata),
    .bpam_pready (orig_bpam_pready),
    .bpam_pslverr (orig_bpam_pslverr),
    .bpam_psel (1'b0),
    .bpam_pwdata ('0),
    .bpam_pwrite (1'b0),
    .sbm_mesh_slave_axi_AR (orig_ar),
    .sbm_mesh_slave_axi_ARREADY (orig_ar_ready_o),
    .sbm_mesh_slave_axi_AW (orig_aw),
    .sbm_mesh_slave_axi_AWREADY (orig_aw_ready_o),
    .sbm_mesh_slave_axi_W (orig_w),
    .sbm_mesh_slave_axi_WREADY (orig_w_ready_o),
    .sbm_mesh_slave_axi_B (orig_b),
    .sbm_mesh_slave_axi_BVALID (orig_b_valid_o),
    .sbm_mesh_slave_axi_BREADY (b_ready_i),
    .sbm_mesh_slave_axi_R (orig_r),
    .sbm_mesh_slave_axi_RVALID (orig_r_valid_o),
    .sbm_mesh_slave_axi_RREADY (r_ready_i),
    .APB_ESR_req (orig_apb_req),
    .APB_ESR_rsp (orig_apb_rsp)
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

endmodule : cosim_sbm_top_tb
