// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_top_pwrstub
  import esr_pkg::*;
  import neigh_pkg::*;
(
  input  logic                                 pwr_ctrl_glb_isolate_i,

  input  logic                                 coop_tload_slv_rdy_out_valid_i,
  output logic                                 coop_tload_slv_rdy_out_valid_o,
  input  logic [NumOtherNeigh-1:0]             coop_tload_mst_done_out_valid_i,
  output logic [NumOtherNeigh-1:0]             coop_tload_mst_done_out_valid_o,

  input  neigh_ch_apb_mux_pkg::apb_to_neigh_t  apb_esr_req_i,
  output neigh_ch_apb_mux_pkg::apb_to_neigh_t  apb_esr_req_o,
  input  neigh_ch_apb_mux_pkg::apb_from_neigh_t apb_esr_rsp_i,
  output neigh_ch_apb_mux_pkg::apb_from_neigh_t apb_esr_rsp_o,

  input  logic [shirecache_pkg::Banks:0]       neigh_sc_req_ready_i,
  output logic [shirecache_pkg::Banks:0]       neigh_sc_req_ready_o,
  input  logic [shirecache_pkg::Banks:0]       neigh_sc_req_valid_i,
  output logic [shirecache_pkg::Banks:0]       neigh_sc_req_valid_o,

  input  logic                                 neigh_sc_rsp_ready_i,
  input  logic                                 neigh_sc_rsp_valid_i,
  output logic                                 neigh_sc_rsp_ready_o,
  output logic                                 neigh_sc_rsp_valid_o,

  input  logic                                 esr_icache_prefetch_done_i,
  input  logic                                 esr_icache_err_detected_i,
  input  logic                                 esr_icache_err_logged_i,
  output logic                                 esr_icache_prefetch_done_o,
  output logic                                 esr_icache_err_detected_o,
  output logic                                 esr_icache_err_logged_o,

  input  logic                                 icache_f0_sram_resp_valid_i,
  input  logic                                 icache_f0_sram_resp_ready_i,
  output logic                                 icache_f0_sram_resp_valid_o,
  output logic                                 icache_f0_sram_resp_ready_o,

  input  esr_and_or_tree_l0_t                  esr_and_or_tree_l0_i,
  output esr_and_or_tree_l0_t                  esr_and_or_tree_l0_o,

  input  logic                                 flb_neigh_l2_req_valid_i,
  input  logic                                 flb_l2_neigh_resp_valid_i,
  output logic                                 flb_neigh_l2_req_valid_o,
  output logic                                 flb_l2_neigh_resp_valid_o,

  input  logic                                 icache_f2_sram_req_write_i,
  input  logic                                 icache_f2_sram_req_valid_i,
  input  logic                                 icache_f2_sram_req_ready_i,
  output logic                                 icache_f2_sram_req_write_o,
  output logic                                 icache_f2_sram_req_valid_o,
  output logic                                 icache_f2_sram_req_ready_o,

  input  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t bpam_rc_tbox_ack_hi_i,
  output neigh_hv_logic_pkg::bpam_rc_tbox_ack_t bpam_rc_tbox_ack_hi_o,

  input  logic                                 reset_w_icache_i,
  output logic                                 reset_w_icache_o,

  input  logic [EtEcoNeighOutputWidth-1:0]     eco_o_i,
  output logic [EtEcoNeighOutputWidth-1:0]     eco_o_o,

  input  logic                                 pwr_ctrl_glb_nsleepout_i,
  input  logic [MinPerN-1:0]                   pwr_ctrl_min_nsleepout_i,
  output logic                                 pwr_ctrl_glb_nsleepout_o,
  output logic [MinPerN-1:0]                   pwr_ctrl_min_nsleepout_o
);

  assign coop_tload_slv_rdy_out_valid_o  = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                           coop_tload_slv_rdy_out_valid_i;
  assign coop_tload_mst_done_out_valid_o = pwr_ctrl_glb_isolate_i ? '0 :
                                           coop_tload_mst_done_out_valid_i;

  assign apb_esr_req_o.apb_paddr   = apb_esr_req_i.apb_paddr;
  assign apb_esr_req_o.apb_penable = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                     apb_esr_req_i.apb_penable;
  assign apb_esr_req_o.apb_psel    = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                     apb_esr_req_i.apb_psel;
  assign apb_esr_req_o.apb_pwdata  = apb_esr_req_i.apb_pwdata;
  assign apb_esr_req_o.apb_pwrite  = apb_esr_req_i.apb_pwrite;

  assign apb_esr_rsp_o.apb_prdata  = apb_esr_rsp_i.apb_prdata;
  assign apb_esr_rsp_o.apb_pready  = pwr_ctrl_glb_isolate_i ? 1'b1 :
                                     apb_esr_rsp_i.apb_pready;
  assign apb_esr_rsp_o.apb_pslverr = pwr_ctrl_glb_isolate_i ? 1'b1 :
                                     apb_esr_rsp_i.apb_pslverr;

  assign neigh_sc_req_ready_o = pwr_ctrl_glb_isolate_i ? '0 : neigh_sc_req_ready_i;
  assign neigh_sc_req_valid_o = pwr_ctrl_glb_isolate_i ? '0 : neigh_sc_req_valid_i;
  assign neigh_sc_rsp_ready_o = pwr_ctrl_glb_isolate_i ? 1'b1 : neigh_sc_rsp_ready_i;
  assign neigh_sc_rsp_valid_o = pwr_ctrl_glb_isolate_i ? 1'b0 : neigh_sc_rsp_valid_i;

  assign esr_icache_prefetch_done_o = pwr_ctrl_glb_isolate_i ? 1'b1 :
                                      esr_icache_prefetch_done_i;
  assign esr_icache_err_detected_o  = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                      esr_icache_err_detected_i;
  assign esr_icache_err_logged_o    = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                      esr_icache_err_logged_i;

  assign icache_f0_sram_resp_valid_o = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                       icache_f0_sram_resp_valid_i;
  assign icache_f0_sram_resp_ready_o = pwr_ctrl_glb_isolate_i ? 1'b1 :
                                       icache_f0_sram_resp_ready_i;

  assign esr_and_or_tree_l0_o.anyselected    = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                               esr_and_or_tree_l0_i.anyselected;
  assign esr_and_or_tree_l0_o.anyunavailable = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                               esr_and_or_tree_l0_i.anyunavailable;
  assign esr_and_or_tree_l0_o.allhavereset   = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                               esr_and_or_tree_l0_i.allhavereset;
  assign esr_and_or_tree_l0_o.anyhavereset   = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                               esr_and_or_tree_l0_i.anyhavereset;
  assign esr_and_or_tree_l0_o.allresumeack   = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                               esr_and_or_tree_l0_i.allresumeack;
  assign esr_and_or_tree_l0_o.anyresumeack   = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                               esr_and_or_tree_l0_i.anyresumeack;
  assign esr_and_or_tree_l0_o.allrunning     = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                               esr_and_or_tree_l0_i.allrunning;
  assign esr_and_or_tree_l0_o.anyrunning     = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                               esr_and_or_tree_l0_i.anyrunning;
  assign esr_and_or_tree_l0_o.allhalted      = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                               esr_and_or_tree_l0_i.allhalted;
  assign esr_and_or_tree_l0_o.anyhalted      = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                               esr_and_or_tree_l0_i.anyhalted;

  assign flb_neigh_l2_req_valid_o  = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                     flb_neigh_l2_req_valid_i;
  assign flb_l2_neigh_resp_valid_o = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                     flb_l2_neigh_resp_valid_i;

  assign icache_f2_sram_req_write_o = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                      icache_f2_sram_req_write_i;
  assign icache_f2_sram_req_valid_o = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                      icache_f2_sram_req_valid_i;
  assign icache_f2_sram_req_ready_o = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                      icache_f2_sram_req_ready_i;

  assign bpam_rc_tbox_ack_hi_o.halt_ack = pwr_ctrl_glb_isolate_i ? 1'b1 :
                                          bpam_rc_tbox_ack_hi_i.halt_ack;
  assign bpam_rc_tbox_ack_hi_o.resume_ack = pwr_ctrl_glb_isolate_i ? 1'b1 :
                                            bpam_rc_tbox_ack_hi_i.resume_ack;

  assign eco_o_o = pwr_ctrl_glb_isolate_i ? '0 : eco_o_i;
  assign reset_w_icache_o = pwr_ctrl_glb_isolate_i ? 1'b0 : reset_w_icache_i;

  assign pwr_ctrl_glb_nsleepout_o = pwr_ctrl_glb_isolate_i ? 1'b0 :
                                    pwr_ctrl_glb_nsleepout_i;
  assign pwr_ctrl_min_nsleepout_o = pwr_ctrl_glb_isolate_i ? '0 :
                                    pwr_ctrl_min_nsleepout_i;

endmodule : neigh_top_pwrstub
