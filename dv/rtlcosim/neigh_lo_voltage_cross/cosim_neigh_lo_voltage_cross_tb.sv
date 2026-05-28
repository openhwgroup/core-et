// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_neigh_lo_voltage_cross_tb
  import dft_pkg::*;
  import neigh_voltage_cross_pkg::*;
#(
  parameter int unsigned FifoDepth = DefaultFifoDepth,
  parameter int unsigned LoOutWidth =
      1 + 1 + 1 + $bits(neigh_hv_logic_pkg::bpam_rc_tbox_ack_t) +
      $bits(minion_pkg::neigh_sm_dbg_monitor_t) +
      $bits(neigh_hv_logic_pkg::apb_from_neigh_t) + 1 + NvcCsrFlBarrierSize +
      1 + icache_geom_pkg::IcacheSramAddrWidth + 1 + 1 +
      $bits(esr_pkg::esr_and_or_tree_l0_t) + 1
) (
  input  logic clk_i,
  input  logic rst_ni,

  input  logic reset_c_push_i,
  input  logic reset_c_pop_i,
  input  logic reset_push_i,
  input  logic reset_pop_i,
  input  logic debug_reset_push_i,
  input  logic debug_reset_pop_i,
  input  logic reset_w_icache_i,
  input  logic dft_reset_byp_hv_i,
  input  logic dft_reset_hv_i,
  input  logic dft_reset_byp_lv_i,
  input  logic dft_reset_lv_i,

  input  logic esr_icache_prefetch_done_i,
  input  logic esr_icache_err_detected_i,
  input  logic esr_icache_err_logged_i,
  input  logic [$bits(neigh_hv_logic_pkg::bpam_rc_tbox_ack_t)-1:0] bpam_rc_tbox_ack_i,
  input  logic [$bits(minion_pkg::neigh_sm_dbg_monitor_t)-1:0] neigh_sm_signals_i,
  input  logic [$bits(neigh_hv_logic_pkg::apb_from_neigh_t)-1:0] apb_esr_rsp_i,
  input  logic flb_neigh_l2_req_valid_i,
  input  logic [NvcCsrFlBarrierSize-1:0] flb_neigh_l2_req_data_i,
  input  logic icache_f2_sram_req_write_i,
  input  logic [icache_geom_pkg::IcacheSramAddrWidth-1:0] icache_f2_sram_req_addr_i,
  input  logic icache_f2_sram_req_valid_i,
  input  logic icache_f2_sram_req_ready_i,
  input  logic [$bits(esr_pkg::esr_and_or_tree_l0_t)-1:0] esr_and_or_tree_l0_i,

  output logic [LoOutWidth-1:0] orig_out_o,
  output logic [LoOutWidth-1:0] new_out_o
);
  dft_t dft_hv;
  dft_t dft_lv;
  assign dft_hv = '{scanmode: dft_reset_byp_hv_i, scan_reset_n: ~dft_reset_hv_i, default: '0};
  assign dft_lv = '{scanmode: dft_reset_byp_lv_i, scan_reset_n: ~dft_reset_lv_i, default: '0};

  logic rst_c_push_ni;
  logic rst_c_pop_ni;
  logic rst_push_ni;
  logic rst_pop_ni;
  logic debug_rst_push_ni;
  logic debug_rst_pop_ni;
  logic rst_w_icache_ni;
  assign rst_c_push_ni    = rst_ni & ~reset_c_push_i;
  assign rst_c_pop_ni     = rst_ni & ~reset_c_pop_i;
  assign rst_push_ni      = rst_ni & ~reset_push_i;
  assign rst_pop_ni       = rst_ni & ~reset_pop_i;
  assign debug_rst_push_ni = rst_ni & ~debug_reset_push_i;
  assign debug_rst_pop_ni  = rst_ni & ~debug_reset_pop_i;
  assign rst_w_icache_ni   = rst_ni & ~reset_w_icache_i;

  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t new_bpam_ack;
  minion_pkg::neigh_sm_dbg_monitor_t new_neigh_sm_signals;
  neigh_hv_logic_pkg::apb_from_neigh_t new_apb_esr_rsp;
  esr_pkg::esr_and_or_tree_l0_t new_and_or_tree_l0;
  assign new_bpam_ack = neigh_hv_logic_pkg::bpam_rc_tbox_ack_t'(bpam_rc_tbox_ack_i);
  assign new_neigh_sm_signals = minion_pkg::neigh_sm_dbg_monitor_t'(neigh_sm_signals_i);
  assign new_apb_esr_rsp = neigh_hv_logic_pkg::apb_from_neigh_t'(apb_esr_rsp_i);
  assign new_and_or_tree_l0 = esr_pkg::esr_and_or_tree_l0_t'(esr_and_or_tree_l0_i);

  logic orig_prefetch_done_sync;
  logic new_prefetch_done_sync;
  logic orig_err_detected_sync;
  logic new_err_detected_sync;
  logic orig_err_logged_sync;
  logic new_err_logged_sync;
  logic [$bits(neigh_hv_logic_pkg::bpam_rc_tbox_ack_t)-1:0] orig_bpam_ack_sync;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t new_bpam_ack_sync;
  logic [$bits(minion_pkg::neigh_sm_dbg_monitor_t)-1:0] orig_neigh_sm_signals_sync;
  minion_pkg::neigh_sm_dbg_monitor_t new_neigh_sm_signals_sync;
  logic [$bits(neigh_hv_logic_pkg::apb_from_neigh_t)-1:0] orig_apb_esr_rsp_sync;
  neigh_hv_logic_pkg::apb_from_neigh_t new_apb_esr_rsp_sync;
  logic orig_flb_req_valid_sync;
  logic new_flb_req_valid_sync;
  logic [NvcCsrFlBarrierSize-1:0] orig_flb_req_data_sync;
  logic [NvcCsrFlBarrierSize-1:0] new_flb_req_data_sync;
  logic orig_icache_write_sync;
  logic new_icache_write_sync;
  logic [icache_geom_pkg::IcacheSramAddrWidth-1:0] orig_icache_addr_sync;
  logic [icache_geom_pkg::IcacheSramAddrWidth-1:0] new_icache_addr_sync;
  logic orig_icache_valid_sync;
  logic new_icache_valid_sync;
  logic orig_icache_ready_sync;
  logic new_icache_ready_sync;
  logic [$bits(esr_pkg::esr_and_or_tree_l0_t)-1:0] orig_and_or_tree_l0_sync;
  esr_pkg::esr_and_or_tree_l0_t new_and_or_tree_l0_sync;
  logic orig_reset_w_icache_sync;
  logic new_rst_w_icache_sync_n;

  neigh_lo_voltage_cross #(
    .FifoDepth(FifoDepth),
    .CrossClockVoltage(1'b1),
    .StubMinions('0)
  ) u_new (
    .rst_c_push_ni                   (rst_c_push_ni),
    .rst_c_pop_ni                    (rst_c_pop_ni),
    .rst_push_ni                     (rst_push_ni),
    .rst_pop_ni                      (rst_pop_ni),
    .clk_push_i                      (clk_i),
    .clk_pop_i                       (clk_i),
    .dft_hv_i                        (dft_hv),
    .dft_lv_i                        (dft_lv),
    .rst_w_icache_ni                 (rst_w_icache_ni),
    .rst_w_icache_sync_no            (new_rst_w_icache_sync_n),
    .debug_rst_push_ni               (debug_rst_push_ni),
    .debug_rst_pop_ni                (debug_rst_pop_ni),
    .esr_icache_prefetch_done_i      (esr_icache_prefetch_done_i),
    .esr_icache_prefetch_done_sync_o (new_prefetch_done_sync),
    .esr_icache_err_detected_i       (esr_icache_err_detected_i),
    .esr_icache_err_detected_sync_o  (new_err_detected_sync),
    .esr_icache_err_logged_i         (esr_icache_err_logged_i),
    .esr_icache_err_logged_sync_o    (new_err_logged_sync),
    .bpam_rc_tbox_ack_i              (new_bpam_ack),
    .bpam_rc_tbox_ack_sync_o         (new_bpam_ack_sync),
    .neigh_sm_signals_i              (new_neigh_sm_signals),
    .neigh_sm_signals_sync_o         (new_neigh_sm_signals_sync),
    .apb_esr_rsp_i                   (new_apb_esr_rsp),
    .apb_esr_rsp_sync_o              (new_apb_esr_rsp_sync),
    .flb_neigh_l2_req_valid_i        (flb_neigh_l2_req_valid_i),
    .flb_neigh_l2_req_valid_sync_o   (new_flb_req_valid_sync),
    .flb_neigh_l2_req_data_i         (flb_neigh_l2_req_data_i),
    .flb_neigh_l2_req_data_sync_o    (new_flb_req_data_sync),
    .icache_f2_sram_req_write_i      (icache_f2_sram_req_write_i),
    .icache_f2_sram_req_write_sync_o (new_icache_write_sync),
    .icache_f2_sram_req_addr_i       (icache_f2_sram_req_addr_i),
    .icache_f2_sram_req_addr_sync_o  (new_icache_addr_sync),
    .icache_f2_sram_req_valid_i      (icache_f2_sram_req_valid_i),
    .icache_f2_sram_req_valid_sync_o (new_icache_valid_sync),
    .icache_f2_sram_req_ready_i      (icache_f2_sram_req_ready_i),
    .icache_f2_sram_req_ready_sync_o (new_icache_ready_sync),
    .esr_and_or_tree_l0_i            (new_and_or_tree_l0),
    .esr_and_or_tree_l0_sync_o       (new_and_or_tree_l0_sync)
  );

  neigh_lo_voltage_cross_orig #(
    .NUM_ELEMS(FifoDepth)
  ) u_orig (
    .reset_c_push                   (!rst_c_push_ni),
    .reset_c_pop                    (!rst_c_pop_ni),
    .reset_push                     (!rst_push_ni),
    .reset_pop                      (!rst_pop_ni),
    .clock_push                     (clk_i),
    .clock_pop                      (clk_i),
    .dft__reset_byp_hv              (dft_reset_byp_hv_i),
    .dft__reset_hv                  (dft_reset_hv_i),
    .dft__reset_byp_lv              (dft_reset_byp_lv_i),
    .dft__reset_lv                  (dft_reset_lv_i),
    .reset_w_icache                 (!rst_w_icache_ni),
    .reset_w_icache_sync            (orig_reset_w_icache_sync),
    .debug_reset_push               (!debug_rst_push_ni),
    .debug_reset_pop                (!debug_rst_pop_ni),
    .esr_icache_prefetch_done       (esr_icache_prefetch_done_i),
    .esr_icache_prefetch_done_sync  (orig_prefetch_done_sync),
    .esr_icache_err_detected        (esr_icache_err_detected_i),
    .esr_icache_err_detected_sync   (orig_err_detected_sync),
    .esr_icache_err_logged          (esr_icache_err_logged_i),
    .esr_icache_err_logged_sync     (orig_err_logged_sync),
    .bpam_rc_tbox_ack               (bpam_rc_tbox_ack_i),
    .bpam_rc_tbox_ack_sync          (orig_bpam_ack_sync),
    .neigh_sm_signals               (neigh_sm_signals_i),
    .neigh_sm_signals_sync          (orig_neigh_sm_signals_sync),
    .apb_esr_rsp                    (apb_esr_rsp_i),
    .apb_esr_rsp_sync               (orig_apb_esr_rsp_sync),
    .flb_neigh_l2_req_valid         (flb_neigh_l2_req_valid_i),
    .flb_neigh_l2_req_valid_sync    (orig_flb_req_valid_sync),
    .flb_neigh_l2_req_data          (flb_neigh_l2_req_data_i),
    .flb_neigh_l2_req_data_sync     (orig_flb_req_data_sync),
    .icache_f2_sram_req_write       (icache_f2_sram_req_write_i),
    .icache_f2_sram_req_write_sync  (orig_icache_write_sync),
    .icache_f2_sram_req_addr        (icache_f2_sram_req_addr_i),
    .icache_f2_sram_req_addr_sync   (orig_icache_addr_sync),
    .icache_f2_sram_req_valid       (icache_f2_sram_req_valid_i),
    .icache_f2_sram_req_valid_sync  (orig_icache_valid_sync),
    .icache_f2_sram_req_ready       (icache_f2_sram_req_ready_i),
    .icache_f2_sram_req_ready_sync  (orig_icache_ready_sync),
    .esr_and_or_tree_L0             (esr_and_or_tree_l0_i),
    .esr_and_or_tree_L0_sync        (orig_and_or_tree_l0_sync)
  );

  assign orig_out_o = {orig_prefetch_done_sync,
                       orig_err_detected_sync,
                       orig_err_logged_sync,
                       orig_bpam_ack_sync,
                       orig_neigh_sm_signals_sync,
                       orig_apb_esr_rsp_sync,
                       orig_flb_req_valid_sync,
                       orig_flb_req_data_sync,
                       orig_icache_write_sync,
                       orig_icache_addr_sync,
                       orig_icache_valid_sync,
                       orig_icache_ready_sync,
                       orig_and_or_tree_l0_sync,
                       ~orig_reset_w_icache_sync};

  assign new_out_o = {new_prefetch_done_sync,
                      new_err_detected_sync,
                      new_err_logged_sync,
                      new_bpam_ack_sync,
                      new_neigh_sm_signals_sync,
                      new_apb_esr_rsp_sync,
                      new_flb_req_valid_sync,
                      new_flb_req_data_sync,
                      new_icache_write_sync,
                      new_icache_addr_sync,
                      new_icache_valid_sync,
                      new_icache_ready_sync,
                      new_and_or_tree_l0_sync,
                      new_rst_w_icache_sync_n};

endmodule : cosim_neigh_lo_voltage_cross_tb
