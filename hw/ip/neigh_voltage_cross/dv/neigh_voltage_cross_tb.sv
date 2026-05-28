// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_voltage_cross_tb (
  input  logic clk_i,
  input  logic rst_ni,

  input  logic rst_c_push_ni_i,
  input  logic rst_c_pop_ni_i,
  input  logic rst_push_ni_i,
  input  logic rst_pop_ni_i,
  input  logic debug_rst_push_ni_i,
  input  logic debug_rst_pop_ni_i,
  input  logic rst_w_icache_ni_i,

  input  logic hv_scanmode_i,
  input  logic hv_scan_reset_ni_i,
  input  logic lv_scanmode_i,
  input  logic lv_scan_reset_ni_i,

  input  logic [7:0]  hi_shire_id_i,
  input  logic [1:0]  hi_neigh_id_i,
  input  logic [1:0]  hi_tbox_id_i,
  input  logic [5:0]  hi_features_i,
  input  logic [5:0]  hi_chicken_i,
  input  logic        hi_coop_mode_i,
  output logic [7:0]  hi_shire_id_sync_o,
  output logic [1:0]  hi_neigh_id_sync_o,
  output logic [1:0]  hi_tbox_id_sync_o,
  output logic [5:0]  hi_features_sync_o,
  output logic [5:0]  hi_chicken_sync_o,
  output logic        hi_coop_mode_sync_o,

  input  logic        hi_tbox_en_i,
  input  logic [7:0]  hi_t0_en_i,
  input  logic [7:0]  hi_t1_en_i,
  output logic        hi_tbox_en_sync_o,
  output logic [7:0]  hi_t0_en_sync_o,
  output logic [7:0]  hi_t1_en_sync_o,

  input  logic        hi_icache_valid_i,
  input  logic [63:0] hi_icache_data_i,
  input  logic        hi_icache_ready_i,
  output logic        hi_icache_valid_sync_o,
  output logic [63:0] hi_icache_data_sync_o,
  output logic        hi_icache_ready_sync_o,

  input  logic        hi_apb_pready_neigh_i,
  input  logic [16:0] hi_apb_paddr_i,
  input  logic        hi_apb_psel_i,
  input  logic        hi_apb_penable_i,
  input  logic        hi_apb_pwrite_i,
  input  logic [63:0] hi_apb_pwdata_i,
  output logic [16:0] hi_apb_paddr_sync_o,
  output logic        hi_apb_psel_sync_o,
  output logic        hi_apb_penable_sync_o,
  output logic        hi_apb_pwrite_sync_o,
  output logic [63:0] hi_apb_pwdata_sync_o,

  input  logic [10:0] hi_bpam_run_control_i,
  input  logic [9:0]  hi_dmctrl_i,
  output logic [10:0] hi_bpam_run_control_sync_o,
  output logic [9:0]  hi_dmctrl_sync_o,

  input  logic [15:0] hi_ipi_msip_i,
  input  logic [15:0] hi_ipi_trigger_i,
  output logic [15:0] hi_ipi_msip_sync_o,
  output logic [15:0] hi_ipi_trigger_sync_o,

  input  logic [7:0]  hi_uc_fcc_i,
  input  logic [1:0]  hi_uc_fcc_target_i,
  output logic [7:0]  hi_uc_fcc_sync_o,
  output logic [1:0]  hi_uc_fcc_target_sync_o,

  input  logic        hi_prefetch_start_i,
  input  logic [49:0] hi_prefetch_conf_i,
  output logic        hi_prefetch_start_sync_o,
  output logic [49:0] hi_prefetch_conf_sync_o,

  input  logic        lo_icache_valid_i,
  input  logic        lo_icache_write_i,
  input  logic [8:0]  lo_icache_addr_i,
  input  logic        lo_icache_ready_i,
  output logic        lo_icache_valid_sync_o,
  output logic        lo_icache_write_sync_o,
  output logic [8:0]  lo_icache_addr_sync_o,
  output logic        lo_icache_ready_sync_o,

  input  logic        lo_apb_pready_i,
  input  logic [63:0] lo_apb_prdata_i,
  input  logic        lo_apb_pslverr_i,
  output logic        lo_apb_pready_sync_o,
  output logic [63:0] lo_apb_prdata_sync_o,
  output logic        lo_apb_pslverr_sync_o,

  input  logic        lo_bpam_halt_ack_i,
  input  logic        lo_bpam_resume_ack_i,
  output logic        lo_bpam_halt_ack_sync_o,
  output logic        lo_bpam_resume_ack_sync_o,

  input  logic        lo_flb_valid_i,
  input  logic [12:0] lo_flb_data_i,
  output logic        lo_flb_valid_sync_o,
  output logic [12:0] lo_flb_data_sync_o,

  input  logic        lo_prefetch_done_i,
  output logic        lo_prefetch_done_sync_o,
  input  logic        lo_err_detected_i,
  output logic        lo_err_detected_sync_o,
  input  logic        lo_err_logged_i,
  output logic        lo_err_logged_sync_o,

  input  logic [9:0]  lo_tree_i,
  output logic [9:0]  lo_tree_sync_o,
  output logic        lo_rst_w_icache_sync_no_o
);
  import dft_pkg::*;
  import neigh_voltage_cross_pkg::*;

  dft_t dft_hv;
  dft_t dft_lv;

  assign dft_hv = '{
    scanmode: hv_scanmode_i,
    scan_reset_n: hv_scan_reset_ni_i,
    default: '0
  };

  assign dft_lv = '{
    scanmode: lv_scanmode_i,
    scan_reset_n: lv_scan_reset_ni_i,
    default: '0
  };

  logic rst_c_push_ni;
  logic rst_c_pop_ni;
  logic rst_push_ni;
  logic rst_pop_ni;
  logic debug_rst_push_ni;
  logic debug_rst_pop_ni;

  assign rst_c_push_ni = rst_ni & rst_c_push_ni_i;
  assign rst_c_pop_ni = rst_ni & rst_c_pop_ni_i;
  assign rst_push_ni = rst_ni & rst_push_ni_i;
  assign rst_pop_ni = rst_ni & rst_pop_ni_i;
  assign debug_rst_push_ni = rst_ni & debug_rst_push_ni_i;
  assign debug_rst_pop_ni = rst_ni & debug_rst_pop_ni_i;

  minion_pkg::esr_minion_features_t hi_features;
  minion_chicken_bits_vc_t hi_chicken;
  apb_to_neigh_t hi_apb_req;
  apb_to_neigh_t hi_apb_req_sync;
  esr_pkg::esr_ms_dmctrl_t hi_dmctrl;
  esr_pkg::esr_ms_dmctrl_t hi_dmctrl_sync;
  bpam_run_control_neigh_t hi_bpam_run_control;
  bpam_run_control_neigh_t hi_bpam_run_control_sync;
  icache_pkg::icache_prefetch_conf_t hi_prefetch_conf;
  icache_pkg::icache_prefetch_conf_t hi_prefetch_conf_sync;
  etlink_pkg::rsp_t hi_rsp_info;
  etlink_pkg::rsp_t hi_rsp_info_sync;
  logic hi_neigh_sc_rsp_valid_sync_unused;
  logic hi_neigh_sc_rsp_ready_sync_unused;
  logic [NeighSmGpioWidth-1:0] hi_neigh_sm_gpio_sync_unused;
  logic hi_hw_dbg_sm_monitor_enabled_sync_unused;
  logic [MinPerN-1:0] hi_int_mtip_sync_unused;
  logic hi_plic_meip_sync_unused;
  logic hi_plic_seip_sync_unused;
  logic hi_flb_l2_neigh_resp_valid_sync_unused;
  logic hi_flb_l2_neigh_resp_data_sync_unused;
  logic [icache_geom_pkg::IcacheSramDataWidth-1:0] hi_icache_data;
  logic [icache_geom_pkg::IcacheSramDataWidth-1:0] hi_icache_data_sync;
  minion_chicken_bits_vc_t hi_chicken_sync;
  minion_pkg::esr_minion_features_t hi_features_sync;

  always_comb begin
    hi_features = minion_pkg::esr_minion_features_t'(hi_features_i);
    hi_chicken = minion_chicken_bits_vc_t'(hi_chicken_i);
    hi_apb_req.apb_paddr = hi_apb_paddr_i;
    hi_apb_req.apb_penable = hi_apb_penable_i;
    hi_apb_req.apb_psel = hi_apb_psel_i;
    hi_apb_req.apb_pwdata = hi_apb_pwdata_i;
    hi_apb_req.apb_pwrite = hi_apb_pwrite_i;
    hi_dmctrl = esr_pkg::esr_ms_dmctrl_t'(hi_dmctrl_i);
    hi_bpam_run_control = bpam_run_control_neigh_t'(hi_bpam_run_control_i);
    hi_prefetch_conf = icache_pkg::icache_prefetch_conf_t'(hi_prefetch_conf_i);
    hi_rsp_info = '0;
    hi_icache_data = '0;
    hi_icache_data[63:0] = hi_icache_data_i;
  end

  assign hi_features_sync_o = hi_features_sync;
  assign hi_chicken_sync_o = hi_chicken_sync;
  assign hi_apb_paddr_sync_o = hi_apb_req_sync.apb_paddr;
  assign hi_apb_psel_sync_o = hi_apb_req_sync.apb_psel;
  assign hi_apb_penable_sync_o = hi_apb_req_sync.apb_penable;
  assign hi_apb_pwrite_sync_o = hi_apb_req_sync.apb_pwrite;
  assign hi_apb_pwdata_sync_o = hi_apb_req_sync.apb_pwdata;
  assign hi_dmctrl_sync_o = hi_dmctrl_sync;
  assign hi_bpam_run_control_sync_o = hi_bpam_run_control_sync;
  assign hi_prefetch_conf_sync_o = hi_prefetch_conf_sync;
  assign hi_icache_data_sync_o = hi_icache_data_sync[63:0];
  logic unused_hi_icache_data_hi;
  assign unused_hi_icache_data_hi = ^hi_icache_data_sync[icache_geom_pkg::IcacheSramDataWidth-1:64];

  neigh_hi_voltage_cross u_hi (
    .rst_c_push_ni                     (rst_c_push_ni),
    .rst_c_pop_ni                      (rst_c_pop_ni),
    .rst_push_ni                       (rst_push_ni),
    .rst_pop_ni                        (rst_pop_ni),
    .clk_push_i                        (clk_i),
    .clk_pop_i                         (clk_i),
    .dft_hv_i                          (dft_hv),
    .dft_lv_i                          (dft_lv),
    .shire_id_i                        (hi_shire_id_i),
    .shire_id_sync_o                   (hi_shire_id_sync_o),
    .neigh_id_i                        (hi_neigh_id_i),
    .neigh_id_sync_o                   (hi_neigh_id_sync_o),
    .tbox_id_i                         (hi_tbox_id_i),
    .tbox_id_sync_o                    (hi_tbox_id_sync_o),
    .esr_minion_features_i             (hi_features),
    .esr_minion_features_sync_o        (hi_features_sync),
    .chicken_bits_i                    (hi_chicken),
    .chicken_bits_sync_o               (hi_chicken_sync),
    .esr_shire_coop_mode_i             (hi_coop_mode_i),
    .esr_shire_coop_mode_sync_o        (hi_coop_mode_sync_o),
    .tbox_en_i                         (hi_tbox_en_i),
    .tbox_en_sync_o                    (hi_tbox_en_sync_o),
    .neigh_t0_en_i                     (hi_t0_en_i),
    .neigh_t0_en_sync_o                (hi_t0_en_sync_o),
    .neigh_t1_en_i                     (hi_t1_en_i),
    .neigh_t1_en_sync_o                (hi_t1_en_sync_o),
    .dmctrl_i                          (hi_dmctrl),
    .dmctrl_sync_o                     (hi_dmctrl_sync),
    .bpam_run_control_i                (hi_bpam_run_control),
    .bpam_run_control_sync_o           (hi_bpam_run_control_sync),
    .esr_icache_prefetch_conf_i        (hi_prefetch_conf),
    .esr_icache_prefetch_conf_sync_o   (hi_prefetch_conf_sync),
    .esr_icache_prefetch_start_i       (hi_prefetch_start_i),
    .esr_icache_prefetch_start_sync_o  (hi_prefetch_start_sync_o),
    .neigh_sc_rsp_valid_i              (1'b0),
    .neigh_sc_rsp_valid_sync_o         (hi_neigh_sc_rsp_valid_sync_unused),
    .neigh_sc_rsp_info_i               (hi_rsp_info),
    .neigh_sc_rsp_info_sync_o          (hi_rsp_info_sync),
    .neigh_sc_rsp_ready_i              (1'b0),
    .neigh_sc_rsp_ready_sync_o         (hi_neigh_sc_rsp_ready_sync_unused),
    .neigh_sm_gpio_i                   ('0),
    .neigh_sm_gpio_sync_o              (hi_neigh_sm_gpio_sync_unused),
    .hw_dbg_sm_monitor_enabled_i       (1'b0),
    .hw_dbg_sm_monitor_enabled_sync_o  (hi_hw_dbg_sm_monitor_enabled_sync_unused),
    .apb_pready_neigh_i                (hi_apb_pready_neigh_i),
    .apb_esr_req_i                     (hi_apb_req),
    .apb_esr_req_sync_o                (hi_apb_req_sync),
    .uc_to_neigh_fcc_target_i          (hi_uc_fcc_target_i),
    .uc_to_neigh_fcc_target_sync_o     (hi_uc_fcc_target_sync_o),
    .uc_to_neigh_fcc_i                 (hi_uc_fcc_i),
    .uc_to_neigh_fcc_sync_o            (hi_uc_fcc_sync_o),
    .int_mtip_i                        ('0),
    .plic_meip_i                       (1'b0),
    .plic_seip_i                       (1'b0),
    .int_mtip_sync_o                   (hi_int_mtip_sync_unused),
    .plic_meip_sync_o                  (hi_plic_meip_sync_unused),
    .plic_seip_sync_o                  (hi_plic_seip_sync_unused),
    .esr_to_neigh_ipi_msip_i           (hi_ipi_msip_i),
    .esr_to_neigh_ipi_msip_sync_o      (hi_ipi_msip_sync_o),
    .esr_to_neigh_ipi_trigger_i        (hi_ipi_trigger_i),
    .esr_to_neigh_ipi_trigger_sync_o   (hi_ipi_trigger_sync_o),
    .flb_l2_neigh_resp_valid_i         (1'b0),
    .flb_l2_neigh_resp_valid_sync_o    (hi_flb_l2_neigh_resp_valid_sync_unused),
    .flb_l2_neigh_resp_data_i          (1'b0),
    .flb_l2_neigh_resp_data_sync_o     (hi_flb_l2_neigh_resp_data_sync_unused),
    .icache_f0_sram_resp_dout_i        (hi_icache_data),
    .icache_f0_sram_resp_dout_sync_o   (hi_icache_data_sync),
    .icache_f0_sram_resp_valid_i       (hi_icache_valid_i),
    .icache_f0_sram_resp_valid_sync_o  (hi_icache_valid_sync_o),
    .icache_f0_sram_resp_ready_i       (hi_icache_ready_i),
    .icache_f0_sram_resp_ready_sync_o  (hi_icache_ready_sync_o)
  );

  neigh_hv_logic_pkg::apb_from_neigh_t lo_apb_rsp;
  neigh_hv_logic_pkg::apb_from_neigh_t lo_apb_rsp_sync;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t lo_ack;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t lo_ack_sync;
  minion_pkg::neigh_sm_dbg_monitor_t lo_sm;
  minion_pkg::neigh_sm_dbg_monitor_t lo_sm_sync;
  esr_pkg::esr_and_or_tree_l0_t lo_tree;
  esr_pkg::esr_and_or_tree_l0_t lo_tree_sync;

  always_comb begin
    lo_apb_rsp.apb_prdata = lo_apb_prdata_i;
    lo_apb_rsp.apb_pready = lo_apb_pready_i;
    lo_apb_rsp.apb_pslverr = lo_apb_pslverr_i;
    lo_ack.halt_ack = lo_bpam_halt_ack_i;
    lo_ack.resume_ack = lo_bpam_resume_ack_i;
    lo_sm = '0;
    lo_sm.data_signals[9:0] = lo_tree_i;
    lo_tree = esr_pkg::esr_and_or_tree_l0_t'(lo_tree_i);
  end

  assign lo_apb_prdata_sync_o = lo_apb_rsp_sync.apb_prdata;
  assign lo_apb_pready_sync_o = lo_apb_rsp_sync.apb_pready;
  assign lo_apb_pslverr_sync_o = lo_apb_rsp_sync.apb_pslverr;
  assign lo_bpam_halt_ack_sync_o = lo_ack_sync.halt_ack;
  assign lo_bpam_resume_ack_sync_o = lo_ack_sync.resume_ack;
  assign lo_tree_sync_o = lo_tree_sync;

  neigh_lo_voltage_cross u_lo (
    .rst_c_push_ni                    (rst_c_push_ni),
    .rst_c_pop_ni                     (rst_c_pop_ni),
    .rst_push_ni                      (rst_push_ni),
    .rst_pop_ni                       (rst_pop_ni),
    .clk_push_i                       (clk_i),
    .clk_pop_i                        (clk_i),
    .dft_hv_i                         (dft_hv),
    .dft_lv_i                         (dft_lv),
    .rst_w_icache_ni                  (rst_ni & rst_w_icache_ni_i),
    .rst_w_icache_sync_no             (lo_rst_w_icache_sync_no_o),
    .debug_rst_push_ni                (debug_rst_push_ni),
    .debug_rst_pop_ni                 (debug_rst_pop_ni),
    .esr_icache_prefetch_done_i       (lo_prefetch_done_i),
    .esr_icache_prefetch_done_sync_o  (lo_prefetch_done_sync_o),
    .esr_icache_err_detected_i        (lo_err_detected_i),
    .esr_icache_err_detected_sync_o   (lo_err_detected_sync_o),
    .esr_icache_err_logged_i          (lo_err_logged_i),
    .esr_icache_err_logged_sync_o     (lo_err_logged_sync_o),
    .bpam_rc_tbox_ack_i               (lo_ack),
    .bpam_rc_tbox_ack_sync_o          (lo_ack_sync),
    .neigh_sm_signals_i               (lo_sm),
    .neigh_sm_signals_sync_o          (lo_sm_sync),
    .apb_esr_rsp_i                    (lo_apb_rsp),
    .apb_esr_rsp_sync_o               (lo_apb_rsp_sync),
    .flb_neigh_l2_req_valid_i         (lo_flb_valid_i),
    .flb_neigh_l2_req_valid_sync_o    (lo_flb_valid_sync_o),
    .flb_neigh_l2_req_data_i          (lo_flb_data_i),
    .flb_neigh_l2_req_data_sync_o     (lo_flb_data_sync_o),
    .icache_f2_sram_req_write_i       (lo_icache_write_i),
    .icache_f2_sram_req_write_sync_o  (lo_icache_write_sync_o),
    .icache_f2_sram_req_addr_i        (lo_icache_addr_i),
    .icache_f2_sram_req_addr_sync_o   (lo_icache_addr_sync_o),
    .icache_f2_sram_req_valid_i       (lo_icache_valid_i),
    .icache_f2_sram_req_valid_sync_o  (lo_icache_valid_sync_o),
    .icache_f2_sram_req_ready_i       (lo_icache_ready_i),
    .icache_f2_sram_req_ready_sync_o  (lo_icache_ready_sync_o),
    .esr_and_or_tree_l0_i             (lo_tree),
    .esr_and_or_tree_l0_sync_o        (lo_tree_sync)
  );

  logic unused_lo_sm;
  assign unused_lo_sm = ^lo_sm_sync;
  logic unused_hi_rsp;
  assign unused_hi_rsp = ^hi_rsp_info_sync ^
                         hi_neigh_sc_rsp_valid_sync_unused ^
                         hi_neigh_sc_rsp_ready_sync_unused ^
                         ^hi_neigh_sm_gpio_sync_unused ^
                         hi_hw_dbg_sm_monitor_enabled_sync_unused ^
                         ^hi_int_mtip_sync_unused ^
                         hi_plic_meip_sync_unused ^
                         hi_plic_seip_sync_unused ^
                         hi_flb_l2_neigh_resp_valid_sync_unused ^
                         hi_flb_l2_neigh_resp_data_sync_unused ^
                         unused_hi_icache_data_hi;

endmodule : neigh_voltage_cross_tb
