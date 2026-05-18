// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off PINCONNECTEMPTY */  // Unit wrapper observes selected outputs and intentionally leaves unrelated outputs open.
/* verilator lint_off UNUSEDSIGNAL */  // Unit wrapper exposes/checks selected fields of wide protocol structs.

module shire_uncached_unit_tb
  import axi_pkg::*;
  import etlink_pkg::*;
  import shirecache_pkg::*;
  import shire_esr_pkg::*;
  import shire_uncached_pkg::*;
  import dft_pkg::*;
(
  input  logic        clk_i,
  input  logic        rst_ni,

  input  logic [UcFccRegisters-1:0]                 fcc_esr_enables_i,
  input  logic [shire_esr_pkg::EsrApbDataWidth-1:0] fcc_esr_wdata_i,
  output logic [(MinPerNeigh * NumNeigh)-1:0]       fcc_flat_o,
  output logic [UcFccTargetSize-1:0]                fcc_target_o,

  input  logic [BarrierRegisters-1:0]               flb_esr_enables_i,
  input  logic [shire_esr_pkg::EsrApbDataWidth-1:0] flb_esr_wdata_i,
  input  logic [NumNeigh-1:0]                       flb_req_valid_i,
  input  logic [(NumNeigh*CsrFlBarrierSize)-1:0]    flb_req_data_flat_i,
  output logic [NumNeigh-1:0]                       flb_resp_valid_o,
  output logic [NumNeigh-1:0]                       flb_resp_data_o,
  output logic [BarrierValueWidth-1:0]              flb_barrier0_o,
  output logic [BarrierValueWidth-1:0]              flb_barrier1_o,

  input  logic        ax_qos_dram_i,
  input  logic        ax_wr_valid_i,
  input  logic        ax_wr_l3_i,
  input  logic [4:0]  ax_wr_opcode_i,
  input  logic [2:0]  ax_wr_size_i,
  input  logic [39:0] ax_wr_addr_i,
  input  logic [8:0]  ax_wr_axi_id_i,
  input  logic [63:0] ax_wr_data_lo_i,
  input  logic [3:0]  ax_wr_atomic_subop_i,
  input  logic        ax_rd_valid_i,
  input  logic        ax_rd_l3_i,
  input  logic [4:0]  ax_rd_opcode_i,
  input  logic [2:0]  ax_rd_size_i,
  input  logic [39:0] ax_rd_addr_i,
  input  logic [8:0]  ax_rd_axi_id_i,
  input  logic        ax_to_sys_ar_ready_i,
  input  logic        ax_to_sys_aw_ready_i,
  input  logic        ax_to_sys_w_ready_i,
  input  logic        ax_to_l3_ar_ready_i,
  input  logic        ax_to_l3_aw_ready_i,
  input  logic        ax_to_l3_w_ready_i,
  output logic        ax_wr_ready_sys_o,
  output logic        ax_wr_ready_l3_o,
  output logic        ax_rd_ready_sys_o,
  output logic        ax_rd_ready_l3_o,
  output logic        ax_to_sys_ar_valid_o,
  output logic [8:0]  ax_to_sys_ar_id_o,
  output logic [39:0] ax_to_sys_ar_addr_o,
  output logic [2:0]  ax_to_sys_ar_size_o,
  output logic [3:0]  ax_to_sys_ar_qos_o,
  output logic        ax_to_sys_aw_valid_o,
  output logic [8:0]  ax_to_sys_aw_id_o,
  output logic [39:0] ax_to_sys_aw_addr_o,
  output logic [2:0]  ax_to_sys_aw_size_o,
  output logic [3:0]  ax_to_sys_aw_qos_o,
  output logic [4:0]  ax_to_sys_aw_user_o,
  output logic        ax_to_sys_w_valid_o,
  output logic [63:0] ax_to_sys_w_strb_o,
  output logic [63:0] ax_to_sys_w_data_lo_o,
  output logic        ax_to_sys_w_last_o,
  output logic        ax_to_l3_ar_valid_o,
  output logic [8:0]  ax_to_l3_ar_id_o,
  output logic [39:0] ax_to_l3_ar_addr_o,
  output logic [2:0]  ax_to_l3_ar_size_o,
  output logic [3:0]  ax_to_l3_ar_qos_o,
  output logic        ax_to_l3_aw_valid_o,
  output logic [8:0]  ax_to_l3_aw_id_o,
  output logic [39:0] ax_to_l3_aw_addr_o,
  output logic [2:0]  ax_to_l3_aw_size_o,
  output logic [3:0]  ax_to_l3_aw_qos_o,
  output logic [4:0]  ax_to_l3_aw_user_o,
  output logic        ax_to_l3_w_valid_o,
  output logic [63:0] ax_to_l3_w_strb_o,
  output logic [63:0] ax_to_l3_w_data_lo_o,
  output logic        ax_to_l3_w_last_o,

  input  logic [2:0]  arb_bid_i,
  input  logic        arb_stall_i,
  output logic [2:0]  arb_grant_o,
  output logic [1:0]  arb_winner_o,

  input  logic [7:0]  rsp_req_id_i,
  input  logic [3:0]  rsp_req_source_i,
  input  logic [2:0]  rsp_req_port_id_i,
  input  logic [39:0] rsp_req_trans_id_i,
  input  logic        rsp_req_resp_i,
  input  logic [4:0]  rsp_req_opcode_i,
  input  logic [2:0]  rsp_req_size_i,
  input  logic [39:0] rsp_req_addr_i,
  input  logic        rsp_reqq_valid_i,

  input  logic        slv_credit_i,
  input  logic        slv_aw_valid_i,
  input  logic        slv_w_valid_i,
  input  logic [18:0] slv_aw_id_i,
  input  logic [39:0] slv_aw_addr_i,
  input  logic [7:0]  slv_aw_len_i,
  input  logic [2:0]  slv_aw_size_i,
  input  logic [4:0]  slv_aw_user_i,
  input  logic [63:0] slv_w_data_lo_i,
  output logic        slv_new_rsp_o,
  output logic        slv_b_valid_o,
  output logic [1:0]  slv_b_resp_o,
  output logic [18:0] slv_b_id_o,
  output logic [4:0]  slv_reqq_index_o,
  output logic        slv_clear_wait_ack_o,
  output logic        slv_clear_wait_from_sys_o,
  output logic [7:0]  slv_rsp_id_o,
  output logic [3:0]  slv_rsp_dest_o,
  output logic [1:0]  slv_rsp_opcode_o,
  output logic        slv_rsp_wdata_o,
  output logic [63:0] slv_rsp_data_lo_o,
  output logic [3:0]  slv_rsp_qwen_o,
  output logic [2:0]  slv_rsp_port_id_o,
  output logic [39:0] slv_rsp_trans_id_o,

  input  logic        tol3_l3_enabled_i,
  input  logic        tol3_remote_scp_enabled_i,
  input  logic        tol3_b_valid_i,
  input  logic [8:0]  tol3_b_id_i,
  input  logic [1:0]  tol3_b_resp_i,
  input  logic        tol3_r_valid_i,
  input  logic [8:0]  tol3_r_id_i,
  input  logic [1:0]  tol3_r_resp_i,
  input  logic [63:0] tol3_r_data_lo_i,
  input  logic [63:0] tol3_r_data_hi_i,
  output logic        tol3_wr_new_rsp_o,
  output logic        tol3_rd_new_rsp_o,
  output logic [1:0]  tol3_wr_rsp_opcode_o,
  output logic [1:0]  tol3_rd_rsp_opcode_o,
  output logic [63:0] tol3_rd_rsp_data_lo_o,
  output logic        tol3_wr_clear_wait_ack_o,
  output logic        tol3_rd_clear_wait_ack_o,

  input  logic        tosys_b_valid_i,
  input  logic [8:0]  tosys_b_id_i,
  input  logic [1:0]  tosys_b_resp_i,
  input  logic        tosys_r_valid_i,
  input  logic [8:0]  tosys_r_id_i,
  input  logic [1:0]  tosys_r_resp_i,
  input  logic [63:0] tosys_r_data_lo_i,
  input  logic [63:0] tosys_r_data_hi_i,
  input  logic [4:0]  tosys_bcast_req_index_i,
  input  logic        tosys_bcast_error_reg_i,
  input  logic [5:0]  tosys_bcast_acks_i,
  output logic        tosys_wr_new_rsp_o,
  output logic        tosys_rd_new_rsp_o,
  output logic        tosys_bcast_new_ack_o,
  output logic        tosys_bcast_done_o,
  output logic        tosys_bcast_error_o,
  output logic [1:0]  tosys_wr_rsp_opcode_o,
  output logic [1:0]  tosys_rd_rsp_opcode_o,
  output logic [63:0] tosys_rd_rsp_data_lo_o,

  input  logic        nr_rsp_send_i,
  input  logic        nr_credit_i,
  input  logic        nr_from_sys_b_ready_i,
  input  logic        nr_from_sys_aw_valid_i,
  input  logic        nr_from_sys_w_valid_i,
  input  logic        nr_to_l3_b_valid_i,
  input  logic        nr_to_l3_r_valid_i,
  input  logic        nr_to_sys_b_valid_i,
  input  logic        nr_to_sys_r_valid_i,
  output logic        nr_rsp_valid_o,
  output logic [1:0]  nr_rsp_opcode_o,
  output logic [7:0]  nr_rsp_id_o,
  output logic        nr_to_l3_b_ready_o,
  output logic        nr_to_l3_r_ready_o,
  output logic        nr_to_sys_b_ready_o,
  output logic        nr_to_sys_r_ready_o,
  output logic        nr_from_sys_b_valid_o,
  output logic        nr_credit_return_o,

  input  logic        hi_logic_l3_ar_ready_i,
  input  logic        hi_logic_l3_ar_valid_i,
  input  logic        hi_logic_l3_aw_ready_i,
  input  logic        hi_logic_l3_w_ready_i,
  input  logic        hi_logic_l3_aw_valid_i,
  input  logic        hi_logic_l3_w_valid_i,
  input  logic        hi_logic_sys_ar_ready_i,
  input  logic        hi_logic_sys_ar_valid_i,
  input  logic        hi_logic_sys_b_ready_i,
  input  logic        hi_logic_sys_b_valid_i,
  input  logic [20:0] hi_logic_sys_b_bits_i,
  output logic        hi_logic_l3_ar_valid_o,
  output logic        hi_logic_l3_ar_pop_o,
  output logic        hi_logic_l3_aw_valid_o,
  output logic        hi_logic_l3_aw_pop_o,
  output logic        hi_logic_l3_w_valid_o,
  output logic        hi_logic_l3_w_pop_o,
  output logic        hi_logic_sys_ar_valid_o,
  output logic        hi_logic_sys_ar_pop_o,
  output logic        hi_logic_sys_b_valid_o,
  output logic        hi_logic_sys_b_pop_o,
  output logic [20:0] hi_logic_sys_b_bits_o,

  input  logic        lo_logic_sbm_aw_fifo_valid_i,
  input  logic        lo_logic_sbm_w_fifo_valid_i,
  input  logic        lo_logic_uc_aw_fifo_valid_i,
  input  logic        lo_logic_uc_w_fifo_valid_i,
  input  logic        lo_logic_aw_valid_i,
  input  logic        lo_logic_w_valid_i,
  input  logic [1:0]  lo_logic_aw_vcvalid_i,
  input  logic [1:0]  lo_logic_w_vcvalid_i,
  input  logic        lo_logic_sbm_aw_ready_i,
  input  logic        lo_logic_sbm_w_ready_i,
  input  logic        lo_logic_uc_aw_ready_i,
  input  logic        lo_logic_uc_w_ready_i,
  input  logic        lo_logic_sbm_aw_fifo_ready_i,
  input  logic        lo_logic_uc_aw_fifo_ready_i,
  input  logic        lo_logic_sbm_w_fifo_ready_i,
  input  logic        lo_logic_uc_w_fifo_ready_i,
  output logic        lo_logic_sbm_aw_valid_o,
  output logic        lo_logic_sbm_aw_push_o,
  output logic        lo_logic_sbm_aw_pop_o,
  output logic        lo_logic_uc_aw_valid_o,
  output logic        lo_logic_uc_aw_push_o,
  output logic        lo_logic_uc_aw_pop_o,
  output logic        lo_logic_sbm_w_valid_o,
  output logic        lo_logic_sbm_w_push_o,
  output logic        lo_logic_sbm_w_pop_o,
  output logic        lo_logic_uc_w_valid_o,
  output logic        lo_logic_uc_w_push_o,
  output logic        lo_logic_uc_w_pop_o,
  output logic        lo_logic_aw_ready_o,
  output logic        lo_logic_w_ready_o,

  input  logic        cross_dft_hv_scanmode_i,
  input  logic        cross_dft_hv_scan_reset_ni,
  input  logic        cross_dft_lv_scanmode_i,
  input  logic        cross_dft_lv_scan_reset_ni,
  input  logic        hi_cross_ar_valid_i,
  input  logic [39:0] hi_cross_ar_addr_i,
  input  logic [8:0]  hi_cross_ar_id_i,
  input  logic        hi_cross_ar_ready_lo_i,
  output logic        hi_cross_ar_ready_o,
  output logic        hi_cross_ar_valid_lo_o,
  output logic [39:0] hi_cross_ar_addr_lo_o,
  output logic [8:0]  hi_cross_ar_id_lo_o,
  input  logic        hi_cross_sys_b_valid_i,
  input  logic [18:0] hi_cross_sys_b_id_i,
  input  logic [1:0]  hi_cross_sys_b_resp_i,
  input  logic        hi_cross_sys_b_ready_lo_i,
  output logic        hi_cross_sys_b_ready_o,
  output logic        hi_cross_sys_b_valid_lo_o,
  output logic [18:0] hi_cross_sys_b_id_lo_o,
  output logic [1:0]  hi_cross_sys_b_resp_lo_o,
  input  logic [1:0]  hi_cross_credit_i,
  output logic [1:0]  hi_cross_credit_lo_o,
  input  logic        lo_cross_l3_b_valid_lo_i,
  input  logic [8:0]  lo_cross_l3_b_id_lo_i,
  input  logic [1:0]  lo_cross_l3_b_resp_lo_i,
  input  logic        lo_cross_l3_b_ready_i,
  output logic        lo_cross_l3_b_ready_lo_o,
  output logic        lo_cross_l3_b_valid_o,
  output logic [8:0]  lo_cross_l3_b_id_o,
  output logic [1:0]  lo_cross_l3_b_resp_o,
  input  logic        lo_cross_ar_valid_lo_i,
  input  logic [39:0] lo_cross_ar_addr_lo_i,
  input  logic [18:0] lo_cross_ar_id_lo_i,
  input  logic        lo_cross_ar_ready_i,
  output logic        lo_cross_ar_ready_lo_o,
  output logic        lo_cross_ar_valid_o,
  output logic [39:0] lo_cross_ar_addr_o,
  output logic [18:0] lo_cross_ar_id_o,

  input  logic [NumShireIdsBits-1:0] noc_shire_id_i,
  input  logic        noc_req_valid_i,
  input  logic        noc_req_wdata_i,
  input  logic [4:0]  noc_req_opcode_i,
  input  logic [2:0]  noc_req_size_i,
  input  logic [39:0] noc_req_addr_i,
  input  logic [63:0] noc_req_data_lo_i,
  input  logic [6:0]  noc_req_subopcode_i,
  input  logic [7:0]  noc_req_id_i,
  input  logic [3:0]  noc_req_source_i,
  input  logic [2:0]  noc_req_port_id_i,
  input  logic [39:0] noc_req_trans_id_i,
  input  logic        noc_credit_i,
  input  logic        noc_l3_enabled_i,
  input  logic        noc_remote_scp_enabled_i,
  input  logic        noc_to_l3_ar_ready_i,
  input  logic        noc_to_l3_aw_ready_i,
  input  logic        noc_to_l3_w_ready_i,
  input  logic        noc_to_sys_ar_ready_i,
  input  logic        noc_to_sys_aw_ready_i,
  input  logic        noc_to_sys_w_ready_i,
  input  logic [NumPorts-1:0] noc_rsp_ready_i,
  output logic        noc_req_ready_o,
  output logic        noc_to_l3_ar_valid_o,
  output logic        noc_to_l3_aw_valid_o,
  output logic        noc_to_sys_ar_valid_o,
  output logic        noc_to_sys_aw_valid_o,
  output logic [39:0] noc_to_sys_aw_addr_o,
  output logic [8:0]  noc_to_sys_aw_id_o,
  output logic [39:0] noc_to_l3_ar_addr_o,
  output logic [8:0]  noc_to_l3_ar_id_o,
  output logic [NumPorts-1:0] noc_rsp_valid_o,
  output logic [1:0]  noc_rsp_opcode_o,
  output logic [7:0]  noc_rsp_id_o,

  input  logic [UcFccRegisters-1:0]                 top_fcc_enables_i,
  input  logic [shire_esr_pkg::EsrApbDataWidth-1:0] top_esr_wdata_i,
  output logic [(MinPerNeigh * NumNeigh)-1:0]       top_fcc_flat_o,
  output logic [UcFccTargetSize-1:0]                top_fcc_target_o,
  output logic                                      top_neigh_ready_o
);

  logic [NumNeigh-1:0][MinPerNeigh-1:0] fcc;
  uncacheable_fcc u_fcc (
    .fcc_o(fcc),
    .fcc_target_o(fcc_target_o),
    .esr_enables_i(fcc_esr_enables_i),
    .esr_wdata_i(fcc_esr_wdata_i)
  );
  assign fcc_flat_o = fcc;

  logic [NumNeigh-1:0][CsrFlBarrierSize-1:0] flb_req_data;
  uc_esr_barrier_values_t flb_barriers;
  for (genvar i = 0; i < NumNeigh; i++) begin : gen_flb_flatten
    assign flb_req_data[i] = flb_req_data_flat_i[i*CsrFlBarrierSize +: CsrFlBarrierSize];
  end
  uncacheable_flb u_flb (
    .clk_i(clk_i),
    .rst_ni(rst_ni),
    .esr_enables_i(flb_esr_enables_i),
    .esr_wdata_i(flb_esr_wdata_i),
    .esr_barrier_values_o(flb_barriers),
    .flb_neigh_l2_req_valid_i(flb_req_valid_i),
    .flb_neigh_l2_req_data_i(flb_req_data),
    .flb_l2_neigh_resp_valid_o(flb_resp_valid_o),
    .flb_l2_neigh_resp_data_o(flb_resp_data_o)
  );
  assign flb_barrier0_o = flb_barriers[0];
  assign flb_barrier1_o = flb_barriers[1];

  esr_uc_config_t ax_config;
  uc_noc_master_req_t ax_wr_req;
  uc_noc_master_req_nodata_t ax_rd_req;
  sc_master_ar_t ax_to_sys_ar;
  sc_master_aw_t ax_to_sys_aw;
  sc_master_w_t  ax_to_sys_w;
  sc_master_ar_t ax_to_l3_ar;
  sc_master_aw_t ax_to_l3_aw;
  sc_master_w_t  ax_to_l3_w;

  always_comb begin
    ax_config = '0;
    ax_config.qos_dram = ax_qos_dram_i;

    ax_wr_req = '0;
    ax_wr_req.data[63:0] = ax_wr_data_lo_i;
    ax_wr_req.data[259:256] = ax_wr_atomic_subop_i;
    ax_wr_req.address = ax_wr_addr_i;
    ax_wr_req.size = etlink_pkg::size_e'(ax_wr_size_i);
    ax_wr_req.opcode = etlink_pkg::req_opcode_e'(ax_wr_opcode_i);
    ax_wr_req.axi_id = ax_wr_axi_id_i;
    ax_wr_req.l3_en = ax_wr_l3_i;

    ax_rd_req = '0;
    ax_rd_req.address = ax_rd_addr_i;
    ax_rd_req.size = etlink_pkg::size_e'(ax_rd_size_i);
    ax_rd_req.opcode = etlink_pkg::req_opcode_e'(ax_rd_opcode_i);
    ax_rd_req.axi_id = ax_rd_axi_id_i;
    ax_rd_req.l3_en = ax_rd_l3_i;
  end

  noc_etlink_to_axi u_axi (
    .uc_config_i(ax_config),
    .wr_req_axi_ready_tol3_o(ax_wr_ready_l3_o),
    .wr_req_axi_ready_tosys_o(ax_wr_ready_sys_o),
    .wr_req_axi_valid_i(ax_wr_valid_i),
    .wr_req_axi_info_i(ax_wr_req),
    .rd_req_axi_ready_tol3_o(ax_rd_ready_l3_o),
    .rd_req_axi_ready_tosys_o(ax_rd_ready_sys_o),
    .rd_req_axi_valid_i(ax_rd_valid_i),
    .rd_req_axi_info_i(ax_rd_req),
    .to_sys_ar_o(ax_to_sys_ar),
    .to_sys_ar_valid_o(ax_to_sys_ar_valid_o),
    .to_sys_ar_ready_i(ax_to_sys_ar_ready_i),
    .to_sys_aw_o(ax_to_sys_aw),
    .to_sys_aw_valid_o(ax_to_sys_aw_valid_o),
    .to_sys_aw_ready_i(ax_to_sys_aw_ready_i),
    .to_sys_w_o(ax_to_sys_w),
    .to_sys_w_valid_o(ax_to_sys_w_valid_o),
    .to_sys_w_ready_i(ax_to_sys_w_ready_i),
    .to_l3_ar_o(ax_to_l3_ar),
    .to_l3_ar_valid_o(ax_to_l3_ar_valid_o),
    .to_l3_ar_ready_i(ax_to_l3_ar_ready_i),
    .to_l3_aw_o(ax_to_l3_aw),
    .to_l3_aw_valid_o(ax_to_l3_aw_valid_o),
    .to_l3_aw_ready_i(ax_to_l3_aw_ready_i),
    .to_l3_w_o(ax_to_l3_w),
    .to_l3_w_valid_o(ax_to_l3_w_valid_o),
    .to_l3_w_ready_i(ax_to_l3_w_ready_i)
  );

  assign ax_to_sys_ar_id_o = ax_to_sys_ar.id;
  assign ax_to_sys_ar_addr_o = ax_to_sys_ar.addr;
  assign ax_to_sys_ar_size_o = ax_to_sys_ar.size;
  assign ax_to_sys_ar_qos_o = ax_to_sys_ar.qos;
  assign ax_to_sys_aw_id_o = ax_to_sys_aw.id;
  assign ax_to_sys_aw_addr_o = ax_to_sys_aw.addr;
  assign ax_to_sys_aw_size_o = ax_to_sys_aw.size;
  assign ax_to_sys_aw_qos_o = ax_to_sys_aw.qos;
  assign ax_to_sys_aw_user_o = ax_to_sys_aw.user;
  assign ax_to_sys_w_strb_o = ax_to_sys_w.strb;
  assign ax_to_sys_w_data_lo_o = ax_to_sys_w.data[63:0];
  assign ax_to_sys_w_last_o = ax_to_sys_w.last;
  assign ax_to_l3_ar_id_o = ax_to_l3_ar.id;
  assign ax_to_l3_ar_addr_o = ax_to_l3_ar.addr;
  assign ax_to_l3_ar_size_o = ax_to_l3_ar.size;
  assign ax_to_l3_ar_qos_o = ax_to_l3_ar.qos;
  assign ax_to_l3_aw_id_o = ax_to_l3_aw.id;
  assign ax_to_l3_aw_addr_o = ax_to_l3_aw.addr;
  assign ax_to_l3_aw_size_o = ax_to_l3_aw.size;
  assign ax_to_l3_aw_qos_o = ax_to_l3_aw.qos;
  assign ax_to_l3_aw_user_o = ax_to_l3_aw.user;
  assign ax_to_l3_w_strb_o = ax_to_l3_w.strb;
  assign ax_to_l3_w_data_lo_o = ax_to_l3_w.data[63:0];
  assign ax_to_l3_w_last_o = ax_to_l3_w.last;

  uncached_arb_lru_grant #(.NumClients(3)) u_arb (
    .clk_i(clk_i),
    .rst_ni(rst_ni),
    .bid_i(arb_bid_i),
    .stall_i(arb_stall_i),
    .grant_o(arb_grant_o),
    .winner_o(arb_winner_o)
  );

  uc_reqq_entry_t rsp_reqq_entry;
  always_comb begin
    rsp_reqq_entry = '0;
    rsp_reqq_entry.id = rsp_req_id_i;
    rsp_reqq_entry.source = rsp_req_source_i;
    rsp_reqq_entry.port_id = rsp_req_port_id_i;
    rsp_reqq_entry.trans_id = rsp_req_trans_id_i;
    rsp_reqq_entry.resp = rsp_req_resp_i;
    rsp_reqq_entry.opcode = etlink_pkg::req_opcode_e'(rsp_req_opcode_i);
    rsp_reqq_entry.size = etlink_pkg::size_e'(rsp_req_size_i);
    rsp_reqq_entry.address = rsp_req_addr_i;
  end

  sys_slave_aw_t slv_aw;
  sys_slave_w_t slv_w;
  uc_reqq_valid_t slv_clear;
  shirecache_pkg::xbar_rsp_t slv_rsp;
  sys_slave_b_t slv_b;
  always_comb begin
    slv_aw = '0;
    slv_aw.id = slv_aw_id_i;
    slv_aw.addr = slv_aw_addr_i;
    slv_aw.len = slv_aw_len_i;
    slv_aw.size = slv_aw_size_i;
    slv_aw.user = slv_aw_user_i;
    slv_w = '0;
    slv_w.data[63:0] = slv_w_data_lo_i;
    slv_w.last = 1'b1;
  end

  uncacheable_noc_slv_handler u_slv_handler (
    .reqq_entry_i(rsp_reqq_entry),
    .reqq_index_o(slv_reqq_index_o),
    .reqq_entry_valid_i(rsp_reqq_valid_i),
    .reqq_entry_clear_o(slv_clear),
    .rsp_info_o(slv_rsp),
    .new_rsp_o(slv_new_rsp_o),
    .credit_i(slv_credit_i),
    .from_sys_aw_i(slv_aw),
    .from_sys_aw_valid_i(slv_aw_valid_i),
    .from_sys_w_i(slv_w),
    .from_sys_w_valid_i(slv_w_valid_i),
    .from_sys_b_o(slv_b),
    .from_sys_b_valid_o(slv_b_valid_o)
  );
  assign slv_b_resp_o = slv_b.resp;
  assign slv_b_id_o = slv_b.id;
  assign slv_clear_wait_ack_o = slv_clear.wait_ack;
  assign slv_clear_wait_from_sys_o = slv_clear.wait_from_sys;
  assign slv_rsp_id_o = slv_rsp.rsp_info.id;
  assign slv_rsp_dest_o = slv_rsp.rsp_info.dest;
  assign slv_rsp_opcode_o = slv_rsp.rsp_info.opcode;
  assign slv_rsp_wdata_o = slv_rsp.rsp_info.wdata;
  assign slv_rsp_data_lo_o = slv_rsp.rsp_info.data[63:0];
  assign slv_rsp_qwen_o = slv_rsp.rsp_info.qwen;
  assign slv_rsp_port_id_o = slv_rsp.port_id;
  assign slv_rsp_trans_id_o = slv_rsp.trans_id;

  sc_master_b_t tol3_b;
  sc_master_r_t tol3_r;
  uc_reqq_valid_t tol3_wr_clear;
  uc_reqq_valid_t tol3_rd_clear;
  shirecache_pkg::xbar_rsp_t tol3_wr_rsp;
  shirecache_pkg::xbar_rsp_t tol3_rd_rsp;
  always_comb begin
    tol3_b = '0;
    tol3_b.id = tol3_b_id_i;
    tol3_b.resp = axi_pkg::resp_e'(tol3_b_resp_i);
    tol3_r = '0;
    tol3_r.id = tol3_r_id_i;
    tol3_r.resp = axi_pkg::resp_e'(tol3_r_resp_i);
    tol3_r.data[63:0] = tol3_r_data_lo_i;
    tol3_r.data[axi_pkg::ScMasterDataSize-1 -: 64] = tol3_r_data_hi_i;
  end
  uncacheable_noc_tol3_handler u_tol3_handler (
    .reqq_entry_i(rsp_reqq_entry),
    .wr_reqq_index_o(),
    .wr_reqq_entry_valid_i(rsp_reqq_valid_i),
    .wr_reqq_entry_clear_o(tol3_wr_clear),
    .rd_reqq_index_o(),
    .rd_reqq_entry_valid_i(rsp_reqq_valid_i),
    .rd_reqq_entry_clear_o(tol3_rd_clear),
    .wr_rsp_info_o(tol3_wr_rsp),
    .wr_new_rsp_o(tol3_wr_new_rsp_o),
    .rd_rsp_info_o(tol3_rd_rsp),
    .rd_new_rsp_o(tol3_rd_new_rsp_o),
    .l3_enabled_i(tol3_l3_enabled_i),
    .remote_scp_enabled_i(tol3_remote_scp_enabled_i),
    .to_l3_b_i(tol3_b),
    .to_l3_b_valid_i(tol3_b_valid_i),
    .to_l3_r_i(tol3_r),
    .to_l3_r_valid_i(tol3_r_valid_i)
  );
  assign tol3_wr_rsp_opcode_o = tol3_wr_rsp.rsp_info.opcode;
  assign tol3_rd_rsp_opcode_o = tol3_rd_rsp.rsp_info.opcode;
  assign tol3_rd_rsp_data_lo_o = tol3_rd_rsp.rsp_info.data[63:0];
  assign tol3_wr_clear_wait_ack_o = tol3_wr_clear.wait_ack;
  assign tol3_rd_clear_wait_ack_o = tol3_rd_clear.wait_ack;

  sc_master_b_t tosys_b;
  sc_master_r_t tosys_r;
  uc_reqq_valid_t tosys_wr_clear;
  uc_reqq_valid_t tosys_rd_clear;
  shirecache_pkg::xbar_rsp_t tosys_wr_rsp;
  shirecache_pkg::xbar_rsp_t tosys_rd_rsp;
  always_comb begin
    tosys_b = '0;
    tosys_b.id = tosys_b_id_i;
    tosys_b.resp = axi_pkg::resp_e'(tosys_b_resp_i);
    tosys_r = '0;
    tosys_r.id = tosys_r_id_i;
    tosys_r.resp = axi_pkg::resp_e'(tosys_r_resp_i);
    tosys_r.data[63:0] = tosys_r_data_lo_i;
    tosys_r.data[axi_pkg::ScMasterDataSize-1 -: 64] = tosys_r_data_hi_i;
  end
  uncacheable_noc_tosys_handler u_tosys_handler (
    .reqq_entry_i(rsp_reqq_entry),
    .wr_reqq_index_o(),
    .wr_reqq_entry_valid_i(rsp_reqq_valid_i),
    .wr_reqq_entry_clear_o(tosys_wr_clear),
    .rd_reqq_index_o(),
    .rd_reqq_entry_valid_i(rsp_reqq_valid_i),
    .rd_reqq_entry_clear_o(tosys_rd_clear),
    .broadcast_req_index_i(tosys_bcast_req_index_i),
    .broadcast_new_ack_o(tosys_bcast_new_ack_o),
    .broadcast_req_done_o(tosys_bcast_done_o),
    .broadcast_rsp_is_error_o(tosys_bcast_error_o),
    .broadcast_rsp_is_error_reg_i(tosys_bcast_error_reg_i),
    .broadcast_acks_i(tosys_bcast_acks_i),
    .wr_rsp_info_o(tosys_wr_rsp),
    .wr_new_rsp_o(tosys_wr_new_rsp_o),
    .rd_rsp_info_o(tosys_rd_rsp),
    .rd_new_rsp_o(tosys_rd_new_rsp_o),
    .to_sys_b_i(tosys_b),
    .to_sys_b_valid_i(tosys_b_valid_i),
    .to_sys_r_i(tosys_r),
    .to_sys_r_valid_i(tosys_r_valid_i)
  );
  assign tosys_wr_rsp_opcode_o = tosys_wr_rsp.rsp_info.opcode;
  assign tosys_rd_rsp_opcode_o = tosys_rd_rsp.rsp_info.opcode;
  assign tosys_rd_rsp_data_lo_o = tosys_rd_rsp.rsp_info.data[63:0];

  sys_slave_aw_t nr_aw;
  sys_slave_w_t nr_w;
  sys_slave_b_t nr_b;
  shirecache_pkg::xbar_rsp_t nr_rsp_data;
  uc_reqq_valid_t nr_clear;
  logic [4:0] nr_reqq_index_unused;
  always_comb begin
    nr_aw = slv_aw;
    nr_w = slv_w;
  end
  uncacheable_noc_rsp u_noc_rsp (
    .clk_i(clk_i),
    .rst_ni(rst_ni),
    .rsp_data_o(nr_rsp_data),
    .rsp_valid_o(nr_rsp_valid_o),
    .rsp_send_i(nr_rsp_send_i),
    .reqq_entry_i(rsp_reqq_entry),
    .reqq_index_o(nr_reqq_index_unused),
    .reqq_entry_valid_i(rsp_reqq_valid_i),
    .reqq_entry_clear_o(nr_clear),
    .broadcast_new_ack_o(),
    .broadcast_req_done_o(),
    .broadcast_rsp_is_error_o(),
    .broadcast_rsp_is_error_reg_i(1'b0),
    .broadcast_acks_i(6'd1),
    .broadcast_req_index_i(5'd0),
    .credit_i(nr_credit_i),
    .credit_return_o(nr_credit_return_o),
    .from_sys_aw_i(nr_aw),
    .from_sys_aw_valid_i(nr_from_sys_aw_valid_i),
    .from_sys_aw_ready_o(),
    .from_sys_w_i(nr_w),
    .from_sys_w_valid_i(nr_from_sys_w_valid_i),
    .from_sys_w_ready_o(),
    .from_sys_b_o(nr_b),
    .from_sys_b_valid_o(nr_from_sys_b_valid_o),
    .from_sys_b_ready_i(nr_from_sys_b_ready_i),
    .remote_scp_enabled_i(tol3_remote_scp_enabled_i),
    .l3_enabled_i(tol3_l3_enabled_i),
    .to_l3_b_i(tol3_b),
    .to_l3_b_valid_i(nr_to_l3_b_valid_i),
    .to_l3_b_ready_o(nr_to_l3_b_ready_o),
    .to_l3_r_i(tol3_r),
    .to_l3_r_valid_i(nr_to_l3_r_valid_i),
    .to_l3_r_ready_o(nr_to_l3_r_ready_o),
    .to_sys_b_i(tosys_b),
    .to_sys_b_valid_i(nr_to_sys_b_valid_i),
    .to_sys_b_ready_o(nr_to_sys_b_ready_o),
    .to_sys_r_i(tosys_r),
    .to_sys_r_valid_i(nr_to_sys_r_valid_i),
    .to_sys_r_ready_o(nr_to_sys_r_ready_o)
  );
  assign nr_rsp_opcode_o = nr_rsp_data.rsp_info.opcode;
  assign nr_rsp_id_o = nr_rsp_data.rsp_info.id;

  sys_slave_b_t hi_logic_sys_b;
  assign hi_logic_sys_b = hi_logic_sys_b_bits_i;
  uncached_hi_voltage_cross_lv_logic u_hi_logic (
    .to_axi_l3_ar_ready_lo_i(hi_logic_l3_ar_ready_i),
    .to_axi_l3_ar_vcfifo_valid_i(hi_logic_l3_ar_valid_i),
    .to_axi_l3_ar_valid_lo_o(hi_logic_l3_ar_valid_o),
    .to_axi_l3_ar_vcfifo_pop_o(hi_logic_l3_ar_pop_o),
    .to_axi_l3_aw_ready_lo_i(hi_logic_l3_aw_ready_i),
    .to_axi_l3_w_ready_lo_i(hi_logic_l3_w_ready_i),
    .to_axi_l3_aw_vcfifo_valid_i(hi_logic_l3_aw_valid_i),
    .to_axi_l3_w_vcfifo_valid_i(hi_logic_l3_w_valid_i),
    .to_axi_l3_aw_valid_lo_o(hi_logic_l3_aw_valid_o),
    .to_axi_l3_aw_vcfifo_pop_o(hi_logic_l3_aw_pop_o),
    .to_axi_l3_w_valid_lo_o(hi_logic_l3_w_valid_o),
    .to_axi_l3_w_vcfifo_pop_o(hi_logic_l3_w_pop_o),
    .to_axi_sys_ar_ready_lo_i(hi_logic_sys_ar_ready_i),
    .to_axi_sys_ar_vcfifo_valid_i(hi_logic_sys_ar_valid_i),
    .to_axi_sys_ar_valid_lo_o(hi_logic_sys_ar_valid_o),
    .to_axi_sys_ar_vcfifo_pop_o(hi_logic_sys_ar_pop_o),
    .to_axi_sys_aw_ready_lo_i(1'b1),
    .to_axi_sys_w_ready_lo_i(1'b1),
    .to_axi_sys_aw_vcfifo_valid_i(1'b0),
    .to_axi_sys_w_vcfifo_valid_i(1'b0),
    .to_axi_sys_aw_valid_lo_o(),
    .to_axi_sys_aw_vcfifo_pop_o(),
    .to_axi_sys_w_valid_lo_o(),
    .to_axi_sys_w_vcfifo_pop_o(),
    .from_axi_sys_b_ready_lo_i(hi_logic_sys_b_ready_i),
    .from_axi_sys_b_vcfifo_valid_i(hi_logic_sys_b_valid_i),
    .from_axi_sys_b_valid_lo_o(hi_logic_sys_b_valid_o),
    .from_axi_sys_b_vcfifo_pop_o(hi_logic_sys_b_pop_o),
    .from_axi_sys_b_w_i(hi_logic_sys_b),
    .from_axi_sys_b_lo_o(hi_logic_sys_b_bits_o),
    .from_axi_sys_r_ready_lo_i(1'b1),
    .from_axi_sys_r_vcfifo_valid_i(1'b0),
    .from_axi_sys_r_valid_lo_o(),
    .from_axi_sys_r_vcfifo_pop_o()
  );

  uncached_lo_voltage_cross_lv_logic u_lo_logic (
    .from_axi_sys_sbm_aw_vcfifo_valid_i(lo_logic_sbm_aw_fifo_valid_i),
    .from_axi_sys_sbm_w_vcfifo_valid_i(lo_logic_sbm_w_fifo_valid_i),
    .from_axi_sys_aw_valid_lo_i(lo_logic_aw_valid_i),
    .from_axi_sys_w_valid_lo_i(lo_logic_w_valid_i),
    .from_axi_sys_aw_vcvalid_lo_i(lo_logic_aw_vcvalid_i),
    .from_axi_sys_w_vcvalid_lo_i(lo_logic_w_vcvalid_i),
    .from_axi_sys_sbm_aw_ready_i(lo_logic_sbm_aw_ready_i),
    .from_axi_sys_sbm_w_ready_i(lo_logic_sbm_w_ready_i),
    .from_axi_sys_uc_aw_vcfifo_valid_i(lo_logic_uc_aw_fifo_valid_i),
    .from_axi_sys_uc_w_vcfifo_valid_i(lo_logic_uc_w_fifo_valid_i),
    .from_axi_sys_uc_aw_ready_i(lo_logic_uc_aw_ready_i),
    .from_axi_sys_uc_w_ready_i(lo_logic_uc_w_ready_i),
    .from_axi_sys_sbm_aw_vcfifo_ready_i(lo_logic_sbm_aw_fifo_ready_i),
    .from_axi_sys_uc_aw_vcfifo_ready_i(lo_logic_uc_aw_fifo_ready_i),
    .from_axi_sys_sbm_w_vcfifo_ready_i(lo_logic_sbm_w_fifo_ready_i),
    .from_axi_sys_uc_w_vcfifo_ready_i(lo_logic_uc_w_fifo_ready_i),
    .from_axi_sys_sbm_aw_valid_o(lo_logic_sbm_aw_valid_o),
    .from_axi_sys_sbm_aw_vcfifo_push_o(lo_logic_sbm_aw_push_o),
    .from_axi_sys_sbm_aw_vcfifo_pop_o(lo_logic_sbm_aw_pop_o),
    .from_axi_sys_uc_aw_valid_o(lo_logic_uc_aw_valid_o),
    .from_axi_sys_uc_aw_vcfifo_push_o(lo_logic_uc_aw_push_o),
    .from_axi_sys_uc_aw_vcfifo_pop_o(lo_logic_uc_aw_pop_o),
    .from_axi_sys_sbm_w_valid_o(lo_logic_sbm_w_valid_o),
    .from_axi_sys_sbm_w_vcfifo_push_o(lo_logic_sbm_w_push_o),
    .from_axi_sys_sbm_w_vcfifo_pop_o(lo_logic_sbm_w_pop_o),
    .from_axi_sys_uc_w_valid_o(lo_logic_uc_w_valid_o),
    .from_axi_sys_uc_w_vcfifo_push_o(lo_logic_uc_w_push_o),
    .from_axi_sys_uc_w_vcfifo_pop_o(lo_logic_uc_w_pop_o),
    .from_axi_sys_aw_ready_lo_o(lo_logic_aw_ready_o),
    .from_axi_sys_w_ready_lo_o(lo_logic_w_ready_o)
  );

  dft_t cross_dft_hv;
  dft_t cross_dft_lv;
  assign cross_dft_hv = '{scanmode: cross_dft_hv_scanmode_i,
                          scan_reset_n: cross_dft_hv_scan_reset_ni,
                          sram_clk_override: 1'b0,
                          ram_rei: 1'b0,
                          ram_wei: 1'b0};
  assign cross_dft_lv = '{scanmode: cross_dft_lv_scanmode_i,
                          scan_reset_n: cross_dft_lv_scan_reset_ni,
                          sram_clk_override: 1'b0,
                          ram_rei: 1'b0,
                          ram_wei: 1'b0};

  sc_master_ar_t hi_cross_ar;
  sc_master_aw_t hi_cross_aw_zero;
  sc_master_w_t hi_cross_w_zero;
  sys_slave_r_t hi_cross_sys_r_zero;
  sys_slave_b_t hi_cross_sys_b;
  sc_master_ar_t hi_cross_l3_ar_lo;
  sc_master_aw_t hi_cross_l3_aw_lo_unused;
  sc_master_w_t hi_cross_l3_w_lo_unused;
  sc_master_ar_t hi_cross_sys_ar_lo_unused;
  sc_master_aw_t hi_cross_sys_aw_lo_unused;
  sc_master_w_t hi_cross_sys_w_lo_unused;
  sys_slave_r_t hi_cross_sys_r_lo_unused;
  sys_slave_b_t hi_cross_sys_b_lo;
  always_comb begin
    hi_cross_ar = '0;
    hi_cross_ar.id = hi_cross_ar_id_i;
    hi_cross_ar.addr = hi_cross_ar_addr_i;
    hi_cross_ar.size = etlink_pkg::SizeDWord;
    hi_cross_aw_zero = '0;
    hi_cross_w_zero = '0;
    hi_cross_sys_r_zero = '0;
    hi_cross_sys_b = '0;
    hi_cross_sys_b.id = hi_cross_sys_b_id_i;
    hi_cross_sys_b.resp = axi_pkg::resp_e'(hi_cross_sys_b_resp_i);
  end
  uncached_hi_voltage_cross u_hi_cross (
    .clk_hv_i(clk_i),
    .rst_hv_ni(rst_ni),
    .clk_lv_i(clk_i),
    .rst_lv_ni(rst_ni),
    .to_axi_l3_ar_i(hi_cross_ar),
    .to_axi_l3_ar_valid_i(hi_cross_ar_valid_i),
    .to_axi_l3_ar_ready_o(hi_cross_ar_ready_o),
    .to_axi_l3_aw_i(hi_cross_aw_zero),
    .to_axi_l3_aw_valid_i(1'b0),
    .to_axi_l3_aw_ready_o(),
    .to_axi_l3_w_i(hi_cross_w_zero),
    .to_axi_l3_w_valid_i(1'b0),
    .to_axi_l3_w_ready_o(),
    .to_axi_l3_ar_lo_o(hi_cross_l3_ar_lo),
    .to_axi_l3_ar_valid_lo_o(hi_cross_ar_valid_lo_o),
    .to_axi_l3_ar_ready_lo_i(hi_cross_ar_ready_lo_i),
    .to_axi_l3_aw_lo_o(hi_cross_l3_aw_lo_unused),
    .to_axi_l3_aw_valid_lo_o(),
    .to_axi_l3_aw_ready_lo_i(1'b1),
    .to_axi_l3_w_lo_o(hi_cross_l3_w_lo_unused),
    .to_axi_l3_w_valid_lo_o(),
    .to_axi_l3_w_ready_lo_i(1'b1),
    .to_axi_sys_ar_i('0),
    .to_axi_sys_ar_valid_i(1'b0),
    .to_axi_sys_ar_ready_o(),
    .to_axi_sys_aw_i('0),
    .to_axi_sys_aw_valid_i(1'b0),
    .to_axi_sys_aw_ready_o(),
    .to_axi_sys_w_i('0),
    .to_axi_sys_w_valid_i(1'b0),
    .to_axi_sys_w_ready_o(),
    .to_axi_sys_ar_lo_o(hi_cross_sys_ar_lo_unused),
    .to_axi_sys_ar_valid_lo_o(),
    .to_axi_sys_ar_ready_lo_i(1'b1),
    .to_axi_sys_aw_lo_o(hi_cross_sys_aw_lo_unused),
    .to_axi_sys_aw_valid_lo_o(),
    .to_axi_sys_aw_ready_lo_i(1'b1),
    .to_axi_sys_w_lo_o(hi_cross_sys_w_lo_unused),
    .to_axi_sys_w_valid_lo_o(),
    .to_axi_sys_w_ready_lo_i(1'b1),
    .from_axi_sys_r_i(hi_cross_sys_r_zero),
    .from_axi_sys_r_valid_i(1'b0),
    .from_axi_sys_r_ready_o(),
    .from_axi_sys_r_lo_o(hi_cross_sys_r_lo_unused),
    .from_axi_sys_r_valid_lo_o(),
    .from_axi_sys_r_ready_lo_i(1'b1),
    .from_axi_sys_b_i(hi_cross_sys_b),
    .from_axi_sys_credit_i(hi_cross_credit_i),
    .from_axi_sys_b_valid_i(hi_cross_sys_b_valid_i),
    .from_axi_sys_b_ready_o(hi_cross_sys_b_ready_o),
    .from_axi_sys_b_lo_o(hi_cross_sys_b_lo),
    .from_axi_sys_credit_lo_o(hi_cross_credit_lo_o),
    .from_axi_sys_b_valid_lo_o(hi_cross_sys_b_valid_lo_o),
    .from_axi_sys_b_ready_lo_i(hi_cross_sys_b_ready_lo_i),
    .dft_hv_i(cross_dft_hv),
    .dft_lv_i(cross_dft_lv)
  );
  assign hi_cross_ar_addr_lo_o = hi_cross_l3_ar_lo.addr;
  assign hi_cross_ar_id_lo_o = hi_cross_l3_ar_lo.id;
  assign hi_cross_sys_b_id_lo_o = hi_cross_sys_b_lo.id;
  assign hi_cross_sys_b_resp_lo_o = hi_cross_sys_b_lo.resp;

  sc_master_b_t lo_cross_l3_b_lo;
  sc_master_r_t lo_cross_l3_r_zero;
  sc_master_b_t lo_cross_sys_b_zero;
  sc_master_r_t lo_cross_sys_r_zero;
  sys_slave_ar_t lo_cross_ar_lo;
  sys_slave_aw_t lo_cross_aw_zero;
  sys_slave_w_t lo_cross_w_zero;
  sc_master_b_t lo_cross_l3_b;
  sc_master_r_t lo_cross_l3_r_unused;
  sc_master_b_t lo_cross_sys_b_unused;
  sc_master_r_t lo_cross_sys_r_unused;
  sys_slave_ar_t lo_cross_ar;
  sys_slave_aw_t lo_cross_uc_aw_unused;
  sys_slave_aw_t lo_cross_sbm_aw_unused;
  sys_slave_w_t lo_cross_uc_w_unused;
  sys_slave_w_t lo_cross_sbm_w_unused;
  always_comb begin
    lo_cross_l3_b_lo = '0;
    lo_cross_l3_b_lo.id = lo_cross_l3_b_id_lo_i;
    lo_cross_l3_b_lo.resp = axi_pkg::resp_e'(lo_cross_l3_b_resp_lo_i);
    lo_cross_l3_r_zero = '0;
    lo_cross_sys_b_zero = '0;
    lo_cross_sys_r_zero = '0;
    lo_cross_ar_lo = '0;
    lo_cross_ar_lo.id = lo_cross_ar_id_lo_i;
    lo_cross_ar_lo.addr = lo_cross_ar_addr_lo_i;
    lo_cross_ar_lo.size = etlink_pkg::SizeWord;
    lo_cross_aw_zero = '0;
    lo_cross_w_zero = '0;
  end
  uncached_lo_voltage_cross u_lo_cross (
    .clk_lv_i(clk_i),
    .rst_lv_ni(rst_ni),
    .clk_hv_i(clk_i),
    .rst_hv_ni(rst_ni),
    .to_axi_l3_b_o(lo_cross_l3_b),
    .to_axi_l3_b_valid_o(lo_cross_l3_b_valid_o),
    .to_axi_l3_b_ready_i(lo_cross_l3_b_ready_i),
    .to_axi_l3_r_o(lo_cross_l3_r_unused),
    .to_axi_l3_r_valid_o(),
    .to_axi_l3_r_ready_i(1'b1),
    .to_axi_l3_b_lo_i(lo_cross_l3_b_lo),
    .to_axi_l3_b_valid_lo_i(lo_cross_l3_b_valid_lo_i),
    .to_axi_l3_b_ready_lo_o(lo_cross_l3_b_ready_lo_o),
    .to_axi_l3_r_lo_i(lo_cross_l3_r_zero),
    .to_axi_l3_r_valid_lo_i(1'b0),
    .to_axi_l3_r_ready_lo_o(),
    .to_axi_sys_b_o(lo_cross_sys_b_unused),
    .to_axi_sys_b_valid_o(),
    .to_axi_sys_b_ready_i(1'b1),
    .to_axi_sys_r_o(lo_cross_sys_r_unused),
    .to_axi_sys_r_valid_o(),
    .to_axi_sys_r_ready_i(1'b1),
    .to_axi_sys_b_lo_i(lo_cross_sys_b_zero),
    .to_axi_sys_b_valid_lo_i(1'b0),
    .to_axi_sys_b_ready_lo_o(),
    .to_axi_sys_r_lo_i(lo_cross_sys_r_zero),
    .to_axi_sys_r_valid_lo_i(1'b0),
    .to_axi_sys_r_ready_lo_o(),
    .from_axi_sys_ar_o(lo_cross_ar),
    .from_axi_sys_ar_valid_o(lo_cross_ar_valid_o),
    .from_axi_sys_ar_ready_i(lo_cross_ar_ready_i),
    .from_axi_sys_uc_aw_o(lo_cross_uc_aw_unused),
    .from_axi_sys_uc_aw_valid_o(),
    .from_axi_sys_uc_aw_ready_i(1'b1),
    .from_axi_sys_sbm_aw_o(lo_cross_sbm_aw_unused),
    .from_axi_sys_sbm_aw_valid_o(),
    .from_axi_sys_sbm_aw_ready_i(1'b1),
    .from_axi_sys_uc_w_o(lo_cross_uc_w_unused),
    .from_axi_sys_uc_w_valid_o(),
    .from_axi_sys_uc_w_ready_i(1'b1),
    .from_axi_sys_sbm_w_o(lo_cross_sbm_w_unused),
    .from_axi_sys_sbm_w_valid_o(),
    .from_axi_sys_sbm_w_ready_i(1'b1),
    .from_axi_sys_ar_lo_i(lo_cross_ar_lo),
    .from_axi_sys_ar_valid_lo_i(lo_cross_ar_valid_lo_i),
    .from_axi_sys_ar_ready_lo_o(lo_cross_ar_ready_lo_o),
    .from_axi_sys_aw_lo_i(lo_cross_aw_zero),
    .from_axi_sys_aw_valid_lo_i(1'b0),
    .from_axi_sys_aw_vcvalid_lo_i(2'b00),
    .from_axi_sys_aw_ready_lo_o(),
    .from_axi_sys_w_lo_i(lo_cross_w_zero),
    .from_axi_sys_w_valid_lo_i(1'b0),
    .from_axi_sys_w_vcvalid_lo_i(2'b00),
    .from_axi_sys_w_ready_lo_o(),
    .dft_lv_i(cross_dft_lv),
    .dft_hv_i(cross_dft_hv)
  );
  assign lo_cross_l3_b_id_o = lo_cross_l3_b.id;
  assign lo_cross_l3_b_resp_o = lo_cross_l3_b.resp;
  assign lo_cross_ar_addr_o = lo_cross_ar.addr;
  assign lo_cross_ar_id_o = lo_cross_ar.id;

  shirecache_pkg::xbar_req_t noc_req;
  shirecache_pkg::xbar_rsp_t noc_rsp;
  esr_uc_config_t noc_config;
  sc_master_ar_t noc_to_l3_ar;
  sc_master_aw_t noc_to_l3_aw;
  sc_master_w_t noc_to_l3_w_unused;
  sc_master_b_t noc_to_l3_b_zero;
  sc_master_r_t noc_to_l3_r_zero;
  sc_master_ar_t noc_to_sys_ar;
  sc_master_aw_t noc_to_sys_aw;
  sc_master_w_t noc_to_sys_w_unused;
  sc_master_b_t noc_to_sys_b_zero;
  sc_master_r_t noc_to_sys_r_zero;
  logic [8:0] noc_req_data_lsb;
  always_comb begin
    noc_config = '0;
    noc_config.qos_dram = ax_qos_dram_i;
    noc_req = '0;
    noc_req.port_id = noc_req_port_id_i;
    noc_req.trans_id = noc_req_trans_id_i;
    noc_req.req_info.id = noc_req_id_i;
    noc_req.req_info.source = noc_req_source_i;
    noc_req.req_info.wdata = noc_req_wdata_i;
    noc_req.req_info.opcode = etlink_pkg::req_opcode_e'(noc_req_opcode_i);
    noc_req.req_info.address = noc_req_addr_i;
    noc_req_data_lsb = {3'b000, noc_req_addr_i[5:0]} << 3;
    noc_req.req_info.data[noc_req_data_lsb +: 64] = noc_req_data_lo_i;
    noc_req.req_info.size = etlink_pkg::size_e'(noc_req_size_i);
    noc_req.req_info.qwen = 4'hf;
    noc_req.req_info.subopcode = noc_req_subopcode_i;
    noc_to_l3_b_zero = '0;
    noc_to_l3_r_zero = '0;
    noc_to_sys_b_zero = '0;
    noc_to_sys_r_zero = '0;
  end
  uncacheable_noc #(.NumPortsP(NumPorts)) u_noc (
    .clk_i(clk_i),
    .rst_ni(rst_ni),
    .shire_id_i(noc_shire_id_i),
    .uc_config_i(noc_config),
    .neigh_uc_req_ready_o(noc_req_ready_o),
    .neigh_uc_req_valid_i(noc_req_valid_i),
    .neigh_uc_req_info_i(noc_req),
    .neigh_uc_min_rsp_ready_i(noc_rsp_ready_i),
    .neigh_uc_min_rsp_valid_o(noc_rsp_valid_o),
    .neigh_uc_min_rsp_info_o(noc_rsp),
    .credit_i(noc_credit_i),
    .credit_return_o(),
    .from_sys_aw_i(slv_aw),
    .from_sys_aw_valid_i(1'b0),
    .from_sys_aw_ready_o(),
    .from_sys_w_i(slv_w),
    .from_sys_w_valid_i(1'b0),
    .from_sys_w_ready_o(),
    .from_sys_b_o(),
    .from_sys_b_valid_o(),
    .from_sys_b_ready_i(1'b1),
    .l3_enabled_i(noc_l3_enabled_i),
    .remote_scp_enabled_i(noc_remote_scp_enabled_i),
    .to_l3_ar_o(noc_to_l3_ar),
    .to_l3_ar_valid_o(noc_to_l3_ar_valid_o),
    .to_l3_ar_ready_i(noc_to_l3_ar_ready_i),
    .to_l3_aw_o(noc_to_l3_aw),
    .to_l3_aw_valid_o(noc_to_l3_aw_valid_o),
    .to_l3_aw_ready_i(noc_to_l3_aw_ready_i),
    .to_l3_w_o(noc_to_l3_w_unused),
    .to_l3_w_valid_o(),
    .to_l3_w_ready_i(noc_to_l3_w_ready_i),
    .to_l3_b_i(noc_to_l3_b_zero),
    .to_l3_b_valid_i(1'b0),
    .to_l3_b_ready_o(),
    .to_l3_r_i(noc_to_l3_r_zero),
    .to_l3_r_valid_i(1'b0),
    .to_l3_r_ready_o(),
    .to_sys_ar_o(noc_to_sys_ar),
    .to_sys_ar_valid_o(noc_to_sys_ar_valid_o),
    .to_sys_ar_ready_i(noc_to_sys_ar_ready_i),
    .to_sys_aw_o(noc_to_sys_aw),
    .to_sys_aw_valid_o(noc_to_sys_aw_valid_o),
    .to_sys_aw_ready_i(noc_to_sys_aw_ready_i),
    .to_sys_w_o(noc_to_sys_w_unused),
    .to_sys_w_valid_o(),
    .to_sys_w_ready_i(noc_to_sys_w_ready_i),
    .to_sys_b_i(noc_to_sys_b_zero),
    .to_sys_b_valid_i(1'b0),
    .to_sys_b_ready_o(),
    .to_sys_r_i(noc_to_sys_r_zero),
    .to_sys_r_valid_i(1'b0),
    .to_sys_r_ready_o()
  );
  assign noc_to_sys_aw_addr_o = noc_to_sys_aw.addr;
  assign noc_to_sys_aw_id_o = noc_to_sys_aw.id;
  assign noc_to_l3_ar_addr_o = noc_to_l3_ar.addr;
  assign noc_to_l3_ar_id_o = noc_to_l3_ar.id;
  assign noc_rsp_opcode_o = noc_rsp.rsp_info.opcode;
  assign noc_rsp_id_o = noc_rsp.rsp_info.id;

  uc_esr_enable_sigs_t top_enables;
  uc_esr_values_t top_values_unused;
  logic [NumNeigh-1:0][MinPerNeigh-1:0] top_fcc;
  dft_t dft_zero;
  assign dft_zero = '0;
  always_comb begin
    top_enables = '0;
    top_enables.fcc = top_fcc_enables_i;
  end
  uncacheable u_top (
    .clk_i(clk_i),
    .rst_ni(rst_ni),
    .noc_clk_i(clk_i),
    .noc_rst_ni(rst_ni),
    .shire_id_i(noc_shire_id_i),
    .esr_enables_i(top_enables),
    .esr_wdata_i(top_esr_wdata_i),
    .esr_values_o(top_values_unused),
    .esr_uc_config_i(noc_config),
    .flb_neigh_l2_req_valid_i('0),
    .flb_neigh_l2_req_data_i('0),
    .flb_l2_neigh_resp_valid_o(),
    .flb_l2_neigh_resp_data_o(),
    .uc_to_neigh_fcc_o(top_fcc),
    .uc_to_neigh_fcc_target_o(top_fcc_target_o),
    .neigh_uc_req_ready_o(top_neigh_ready_o),
    .neigh_uc_req_valid_i(1'b0),
    .neigh_uc_req_info_i('0),
    .neigh_uc_min_rsp_ready_i({NumPorts{1'b1}}),
    .neigh_uc_min_rsp_valid_o(),
    .neigh_uc_min_rsp_info_o(),
    .sbm_enable_read_o(),
    .sbm_enable_write_o(),
    .sbm_write_credit_return_i(1'b0),
    .sbm_sys_axi_ar_o(),
    .sbm_sys_axi_ar_ready_i(1'b1),
    .sbm_sys_axi_aw_o(),
    .sbm_sys_axi_aw_ready_i(1'b1),
    .sbm_sys_axi_w_o(),
    .sbm_sys_axi_w_ready_i(1'b1),
    .sbm_sys_axi_b_i('0),
    .sbm_sys_axi_b_valid_i(1'b0),
    .sbm_sys_axi_b_ready_o(),
    .sbm_sys_axi_r_i('0),
    .sbm_sys_axi_r_valid_i(1'b0),
    .sbm_sys_axi_r_ready_o(),
    .l3_enabled_i(1'b1),
    .remote_scp_enabled_i(1'b1),
    .sys_axi_ar_i('0),
    .sys_axi_ar_valid_i(1'b0),
    .sys_axi_ar_ready_o(),
    .sys_axi_aw_i('0),
    .sys_axi_aw_valid_i(1'b0),
    .sys_axi_aw_ready_o(),
    .sys_axi_w_i('0),
    .sys_axi_w_valid_i(1'b0),
    .sys_axi_w_ready_o(),
    .sys_axi_b_o(),
    .sys_axi_b_valid_o(),
    .sys_axi_b_ready_i(1'b1),
    .sys_axi_r_o(),
    .sys_axi_r_valid_o(),
    .sys_axi_r_ready_i(1'b1),
    .sys_axi_aw_vcvalid_i(2'b01),
    .sys_axi_aw_credit_o(),
    .sys_axi_w_vcvalid_i(2'b01),
    .to_l3_ar_o(),
    .to_l3_ar_valid_o(),
    .to_l3_ar_ready_i(1'b1),
    .to_l3_aw_o(),
    .to_l3_aw_valid_o(),
    .to_l3_aw_ready_i(1'b1),
    .to_l3_w_o(),
    .to_l3_w_valid_o(),
    .to_l3_w_ready_i(1'b1),
    .to_l3_b_i('0),
    .to_l3_b_valid_i(1'b0),
    .to_l3_b_ready_o(),
    .to_l3_r_i('0),
    .to_l3_r_valid_i(1'b0),
    .to_l3_r_ready_o(),
    .to_sys_ar_o(),
    .to_sys_ar_valid_o(),
    .to_sys_ar_ready_i(1'b1),
    .to_sys_aw_o(),
    .to_sys_aw_valid_o(),
    .to_sys_aw_ready_i(1'b1),
    .to_sys_w_o(),
    .to_sys_w_valid_o(),
    .to_sys_w_ready_i(1'b1),
    .to_sys_b_i('0),
    .to_sys_b_valid_i(1'b0),
    .to_sys_b_ready_o(),
    .to_sys_r_i('0),
    .to_sys_r_valid_i(1'b0),
    .to_sys_r_ready_o(),
    .dft_hv_i(dft_zero),
    .dft_lv_i(dft_zero)
  );
  assign top_fcc_flat_o = top_fcc;

  /* verilator lint_off UNUSEDSIGNAL */
  logic unused_unit_signals;
  assign unused_unit_signals = ^{hi_cross_l3_aw_lo_unused, hi_cross_l3_w_lo_unused,
                                 hi_cross_sys_ar_lo_unused, hi_cross_sys_aw_lo_unused,
                                 hi_cross_sys_w_lo_unused, hi_cross_sys_r_lo_unused,
                                 lo_cross_l3_r_unused, lo_cross_sys_b_unused,
                                 lo_cross_sys_r_unused, lo_cross_uc_aw_unused,
                                 lo_cross_sbm_aw_unused, lo_cross_uc_w_unused,
                                 lo_cross_sbm_w_unused, nr_clear,
                                 nr_reqq_index_unused, nr_b,
                                 noc_to_l3_aw, noc_to_l3_w_unused,
                                 noc_to_sys_ar, noc_to_sys_w_unused,
                                 top_values_unused};
  /* verilator lint_on UNUSEDSIGNAL */

endmodule : shire_uncached_unit_tb

/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on PINCONNECTEMPTY */
