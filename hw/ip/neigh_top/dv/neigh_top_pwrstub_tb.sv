// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_top_pwrstub_tb
  import esr_pkg::*;
(
  input  logic                                           isolate_i,
  input  logic                                           coop_slv_valid_i,
  input  logic [neigh_pkg::NumOtherNeigh-1:0]            coop_mst_valid_i,
  input  logic [neigh_ch_apb_mux_pkg::NeighDmApbAddrWidth-1:0] apb_paddr_i,
  input  logic                                           apb_penable_i,
  input  logic                                           apb_psel_i,
  input  logic [neigh_ch_apb_mux_pkg::DataWidth-1:0]     apb_pwdata_i,
  input  logic                                           apb_pwrite_i,
  input  logic [neigh_ch_apb_mux_pkg::DataWidth-1:0]     apb_prdata_i,
  input  logic                                           apb_pready_i,
  input  logic                                           apb_pslverr_i,
  input  logic [shirecache_pkg::Banks:0]                 req_ready_i,
  input  logic [shirecache_pkg::Banks:0]                 req_valid_i,
  input  logic                                           rsp_ready_i,
  input  logic                                           rsp_valid_i,
  input  logic                                           prefetch_done_i,
  input  logic                                           icache_err_detected_i,
  input  logic                                           icache_err_logged_i,
  input  logic                                           icache_resp_valid_i,
  input  logic                                           icache_resp_ready_i,
  input  logic [9:0]                                     esr_tree_i,
  input  logic                                           flb_req_valid_i,
  input  logic                                           flb_resp_valid_i,
  input  logic                                           icache_req_write_i,
  input  logic                                           icache_req_valid_i,
  input  logic                                           icache_req_ready_i,
  input  logic                                           bpam_halt_ack_i,
  input  logic                                           bpam_resume_ack_i,
  input  logic                                           reset_w_icache_i,
  input  logic [neigh_pkg::EtEcoNeighOutputWidth-1:0]    eco_i,
  input  logic                                           pwr_glb_nsleepout_i,
  input  logic [neigh_pkg::MinPerN-1:0]                  pwr_min_nsleepout_i,

  output logic                                           coop_slv_valid_o,
  output logic [neigh_pkg::NumOtherNeigh-1:0]            coop_mst_valid_o,
  output logic [neigh_ch_apb_mux_pkg::NeighDmApbAddrWidth-1:0] apb_paddr_o,
  output logic                                           apb_penable_o,
  output logic                                           apb_psel_o,
  output logic [neigh_ch_apb_mux_pkg::DataWidth-1:0]     apb_pwdata_o,
  output logic                                           apb_pwrite_o,
  output logic [neigh_ch_apb_mux_pkg::DataWidth-1:0]     apb_prdata_o,
  output logic                                           apb_pready_o,
  output logic                                           apb_pslverr_o,
  output logic [shirecache_pkg::Banks:0]                 req_ready_o,
  output logic [shirecache_pkg::Banks:0]                 req_valid_o,
  output logic                                           rsp_ready_o,
  output logic                                           rsp_valid_o,
  output logic                                           prefetch_done_o,
  output logic                                           icache_err_detected_o,
  output logic                                           icache_err_logged_o,
  output logic                                           icache_resp_valid_o,
  output logic                                           icache_resp_ready_o,
  output logic [9:0]                                     esr_tree_o,
  output logic                                           flb_req_valid_o,
  output logic                                           flb_resp_valid_o,
  output logic                                           icache_req_write_o,
  output logic                                           icache_req_valid_o,
  output logic                                           icache_req_ready_o,
  output logic                                           bpam_halt_ack_o,
  output logic                                           bpam_resume_ack_o,
  output logic                                           reset_w_icache_o,
  output logic [neigh_pkg::EtEcoNeighOutputWidth-1:0]    eco_o,
  output logic                                           pwr_glb_nsleepout_o,
  output logic [neigh_pkg::MinPerN-1:0]                  pwr_min_nsleepout_o
);

  neigh_ch_apb_mux_pkg::apb_to_neigh_t apb_req_i;
  neigh_ch_apb_mux_pkg::apb_to_neigh_t apb_req_o;
  neigh_ch_apb_mux_pkg::apb_from_neigh_t apb_rsp_i;
  neigh_ch_apb_mux_pkg::apb_from_neigh_t apb_rsp_o;
  esr_and_or_tree_l0_t esr_tree_in;
  esr_and_or_tree_l0_t esr_tree_out;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t bpam_in;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t bpam_out;

  assign apb_req_i.apb_paddr = apb_paddr_i;
  assign apb_req_i.apb_penable = apb_penable_i;
  assign apb_req_i.apb_psel = apb_psel_i;
  assign apb_req_i.apb_pwdata = apb_pwdata_i;
  assign apb_req_i.apb_pwrite = apb_pwrite_i;
  assign apb_rsp_i.apb_prdata = apb_prdata_i;
  assign apb_rsp_i.apb_pready = apb_pready_i;
  assign apb_rsp_i.apb_pslverr = apb_pslverr_i;

  assign {esr_tree_in.anyselected,
          esr_tree_in.anyunavailable,
          esr_tree_in.allhavereset,
          esr_tree_in.anyhavereset,
          esr_tree_in.allresumeack,
          esr_tree_in.anyresumeack,
          esr_tree_in.allrunning,
          esr_tree_in.anyrunning,
          esr_tree_in.allhalted,
          esr_tree_in.anyhalted} = esr_tree_i;
  assign esr_tree_o = {esr_tree_out.anyselected,
                       esr_tree_out.anyunavailable,
                       esr_tree_out.allhavereset,
                       esr_tree_out.anyhavereset,
                       esr_tree_out.allresumeack,
                       esr_tree_out.anyresumeack,
                       esr_tree_out.allrunning,
                       esr_tree_out.anyrunning,
                       esr_tree_out.allhalted,
                       esr_tree_out.anyhalted};

  assign bpam_in.halt_ack = bpam_halt_ack_i;
  assign bpam_in.resume_ack = bpam_resume_ack_i;
  assign bpam_halt_ack_o = bpam_out.halt_ack;
  assign bpam_resume_ack_o = bpam_out.resume_ack;

  assign apb_paddr_o = apb_req_o.apb_paddr;
  assign apb_penable_o = apb_req_o.apb_penable;
  assign apb_psel_o = apb_req_o.apb_psel;
  assign apb_pwdata_o = apb_req_o.apb_pwdata;
  assign apb_pwrite_o = apb_req_o.apb_pwrite;
  assign apb_prdata_o = apb_rsp_o.apb_prdata;
  assign apb_pready_o = apb_rsp_o.apb_pready;
  assign apb_pslverr_o = apb_rsp_o.apb_pslverr;

  neigh_top_pwrstub u_dut (
    .pwr_ctrl_glb_isolate_i(isolate_i),
    .coop_tload_slv_rdy_out_valid_i(coop_slv_valid_i),
    .coop_tload_slv_rdy_out_valid_o(coop_slv_valid_o),
    .coop_tload_mst_done_out_valid_i(coop_mst_valid_i),
    .coop_tload_mst_done_out_valid_o(coop_mst_valid_o),
    .apb_esr_req_i(apb_req_i),
    .apb_esr_req_o(apb_req_o),
    .apb_esr_rsp_i(apb_rsp_i),
    .apb_esr_rsp_o(apb_rsp_o),
    .neigh_sc_req_ready_i(req_ready_i),
    .neigh_sc_req_ready_o(req_ready_o),
    .neigh_sc_req_valid_i(req_valid_i),
    .neigh_sc_req_valid_o(req_valid_o),
    .neigh_sc_rsp_ready_i(rsp_ready_i),
    .neigh_sc_rsp_valid_i(rsp_valid_i),
    .neigh_sc_rsp_ready_o(rsp_ready_o),
    .neigh_sc_rsp_valid_o(rsp_valid_o),
    .esr_icache_prefetch_done_i(prefetch_done_i),
    .esr_icache_err_detected_i(icache_err_detected_i),
    .esr_icache_err_logged_i(icache_err_logged_i),
    .esr_icache_prefetch_done_o(prefetch_done_o),
    .esr_icache_err_detected_o(icache_err_detected_o),
    .esr_icache_err_logged_o(icache_err_logged_o),
    .icache_f0_sram_resp_valid_i(icache_resp_valid_i),
    .icache_f0_sram_resp_ready_i(icache_resp_ready_i),
    .icache_f0_sram_resp_valid_o(icache_resp_valid_o),
    .icache_f0_sram_resp_ready_o(icache_resp_ready_o),
    .esr_and_or_tree_l0_i(esr_tree_in),
    .esr_and_or_tree_l0_o(esr_tree_out),
    .flb_neigh_l2_req_valid_i(flb_req_valid_i),
    .flb_l2_neigh_resp_valid_i(flb_resp_valid_i),
    .flb_neigh_l2_req_valid_o(flb_req_valid_o),
    .flb_l2_neigh_resp_valid_o(flb_resp_valid_o),
    .icache_f2_sram_req_write_i(icache_req_write_i),
    .icache_f2_sram_req_valid_i(icache_req_valid_i),
    .icache_f2_sram_req_ready_i(icache_req_ready_i),
    .icache_f2_sram_req_write_o(icache_req_write_o),
    .icache_f2_sram_req_valid_o(icache_req_valid_o),
    .icache_f2_sram_req_ready_o(icache_req_ready_o),
    .bpam_rc_tbox_ack_hi_i(bpam_in),
    .bpam_rc_tbox_ack_hi_o(bpam_out),
    .reset_w_icache_i(reset_w_icache_i),
    .reset_w_icache_o(reset_w_icache_o),
    .eco_o_i(eco_i),
    .eco_o_o(eco_o),
    .pwr_ctrl_glb_nsleepout_i(pwr_glb_nsleepout_i),
    .pwr_ctrl_min_nsleepout_i(pwr_min_nsleepout_i),
    .pwr_ctrl_glb_nsleepout_o(pwr_glb_nsleepout_o),
    .pwr_ctrl_min_nsleepout_o(pwr_min_nsleepout_o)
  );

endmodule : neigh_top_pwrstub_tb
