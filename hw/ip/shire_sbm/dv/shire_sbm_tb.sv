// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module shire_sbm_tb (
  input  logic clk_i,
  input  logic rst_ni,

  input  logic [shire_sbm_pkg::NumNeigh-1:0] rst_neigh_ni,
  input  logic rst_rbox_ni,
  input  logic rst_shire_cache_ni,
  input  logic [7:0] shire_virtual_id_i,

  input  logic ar_valid_i,
  input  logic [axi_pkg::ScSlaveIdSize-1:0] ar_id_i,
  input  logic [axi_pkg::ScMasterAddrSize-1:0] ar_addr_i,
  input  logic [axi_pkg::AxLenSize-1:0] ar_len_i,
  output logic ar_ready_o,

  input  logic aw_valid_i,
  input  logic [axi_pkg::ScSlaveIdSize-1:0] aw_id_i,
  input  logic [axi_pkg::ScMasterAddrSize-1:0] aw_addr_i,
  input  logic [axi_pkg::AxLenSize-1:0] aw_len_i,
  input  logic [63:0] wdata_i,
  output logic aw_ready_o,
  output logic w_ready_o,

  output logic b_valid_o,
  output logic [axi_pkg::ScSlaveIdSize-1:0] b_id_o,
  output logic [1:0] b_resp_o,
  input  logic b_ready_i,

  output logic r_valid_o,
  output logic [axi_pkg::ScSlaveIdSize-1:0] r_id_o,
  output logic [1:0] r_resp_o,
  output logic r_last_o,
  output logic [63:0] r_data_lo_o,
  input  logic r_ready_i,

  output logic credit_return_write_o,

  output logic [shire_sbm_pkg::ShireApbInterfaces-1:0] apb_psel_o,
  output logic [shire_sbm_pkg::ShireApbInterfaces-1:0] apb_penable_o,
  output logic [shire_sbm_pkg::ShireApbInterfaces-1:0] apb_pwrite_o,
  input  logic [3:0] obs_index_i,
  output logic [shire_sbm_pkg::ShireApbAddrWidth-1:0] obs_paddr_o,
  output logic [63:0] obs_pwdata_o,
  input  logic [shire_sbm_pkg::ShireApbInterfaces-1:0] apb_pready_i,
  input  logic [shire_sbm_pkg::ShireApbInterfaces-1:0] apb_pslverr_i,
  input  logic [63:0] apb_prdata_i
);

  axi_pkg::sys_slave_ar_t sys_ar;
  axi_pkg::sys_slave_aw_t sys_aw;
  axi_pkg::sys_slave_w_t  sys_w;
  axi_pkg::sys_slave_b_t  sys_b;
  axi_pkg::sys_slave_r_t  sys_r;

  shire_sbm_pkg::shire_apb_req_t [shire_sbm_pkg::ShireApbInterfaces-1:0] apb_req;
  shire_sbm_pkg::shire_apb_rsp_t [shire_sbm_pkg::ShireApbInterfaces-1:0] apb_rsp;

  always_comb begin
    sys_ar = '0;
    sys_ar.id = ar_id_i;
    sys_ar.addr = ar_addr_i;
    sys_ar.len = ar_len_i;

    sys_aw = '0;
    sys_aw.id = aw_id_i;
    sys_aw.addr = aw_addr_i;
    sys_aw.len = aw_len_i;

    sys_w = '0;
    sys_w.data = {4{wdata_i}};
    sys_w.strb = '1;
    sys_w.last = 1'b1;

    for (int i = 0; i < shire_sbm_pkg::ShireApbInterfaces; i++) begin
      apb_psel_o[i] = apb_req[i].psel;
      apb_penable_o[i] = apb_req[i].penable;
      apb_pwrite_o[i] = apb_req[i].pwrite;
      apb_rsp[i].pready = apb_pready_i[i];
      apb_rsp[i].pslverr = apb_pslverr_i[i];
      apb_rsp[i].prdata = apb_prdata_i;
    end
  end

  logic [3:0] obs_idx;
  assign obs_idx = (obs_index_i < 4'd11) ? obs_index_i : 4'd0;
  assign obs_paddr_o = apb_req[obs_idx].paddr;
  assign obs_pwdata_o = apb_req[obs_idx].pwdata;

  sbm_top u_dut (
    .clk_i,
    .rst_ni,
    .rst_neigh_ni,
    .rst_rbox_ni,
    .rst_shire_cache_ni,
    .shire_virtual_id_i ({11'b0, shire_virtual_id_i}),
    .sys_axi_ar_valid_i (ar_valid_i),
    .sys_axi_aw_valid_i (aw_valid_i),
    .sbm_write_credit_return_o (credit_return_write_o),
    .sys_axi_ar_i (sys_ar),
    .sys_axi_ar_ready_o (ar_ready_o),
    .sys_axi_aw_i (sys_aw),
    .sys_axi_aw_ready_o (aw_ready_o),
    .sys_axi_w_i (sys_w),
    .sys_axi_w_ready_o (w_ready_o),
    .sys_axi_b_o (sys_b),
    .sys_axi_b_valid_o (b_valid_o),
    .sys_axi_b_ready_i (b_ready_i),
    .sys_axi_r_o (sys_r),
    .sys_axi_r_valid_o (r_valid_o),
    .sys_axi_r_ready_i (r_ready_i),
    .apb_req_o (apb_req),
    .apb_rsp_i (apb_rsp)
  );

  assign b_id_o = sys_b.id;
  assign b_resp_o = sys_b.resp;
  assign r_id_o = sys_r.id;
  assign r_resp_o = sys_r.resp;
  assign r_last_o = sys_r.last;
  assign r_data_lo_o = sys_r.data[63:0];

endmodule : shire_sbm_tb
