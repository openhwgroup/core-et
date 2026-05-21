// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_neigh_hi_voltage_cross_tb
  import dft_pkg::*;
  import neigh_voltage_cross_pkg::*;
#(
  parameter int unsigned FifoDepth = DefaultFifoDepth,
  parameter int unsigned HiOutWidth =
      NvcNumShireIdsBits + NumNeighBits + TboxPerShireBits +
      $bits(minion_pkg::esr_minion_features_t) + $bits(minion_chicken_bits_vc_t) +
      1 + 1 + MinPerN + MinPerN + $bits(esr_pkg::esr_ms_dmctrl_t) +
      $bits(bpam_run_control_neigh_t) + $bits(icache_pkg::icache_prefetch_conf_t) +
      1 + 1 + $bits(etlink_pkg::rsp_t) + 1 + NeighSmGpioWidth + 1 +
      $bits(apb_to_neigh_t) + UcFccTargetSize + MinPerN + MinPerN + 1 + 1 +
      ThreadsPerN + ThreadsPerN + 1 + 1 + icache_geom_pkg::IcacheSramDataWidth + 1 + 1
) (
  input  logic clk_i,
  input  logic rst_ni,

  input  logic reset_c_push_i,
  input  logic reset_c_pop_i,
  input  logic reset_push_i,
  input  logic reset_pop_i,
  input  logic dft_reset_byp_hv_i,
  input  logic dft_reset_hv_i,
  input  logic dft_reset_byp_lv_i,
  input  logic dft_reset_lv_i,

  input  logic [NvcNumShireIdsBits-1:0] shire_id_i,
  input  logic [NumNeighBits-1:0]        neigh_id_i,
  input  logic [TboxPerShireBits-1:0]    tbox_id_i,
  input  logic [$bits(minion_pkg::esr_minion_features_t)-1:0] esr_minion_features_i,
  input  logic [$bits(minion_chicken_bits_vc_t)-1:0] chicken_bits_i,
  input  logic                           esr_shire_coop_mode_i,
  input  logic                           tbox_en_i,
  input  logic [MinPerN-1:0]             neigh_t0_en_i,
  input  logic [MinPerN-1:0]             neigh_t1_en_i,
  input  logic [$bits(esr_pkg::esr_ms_dmctrl_t)-1:0] dmctrl_i,
  input  logic [$bits(bpam_run_control_neigh_t)-1:0] bpam_run_control_i,
  input  logic [$bits(icache_pkg::icache_prefetch_conf_t)-1:0] esr_icache_prefetch_conf_i,
  input  logic                           esr_icache_prefetch_start_i,
  input  logic                           neigh_sc_rsp_valid_i,
  input  logic [$bits(etlink_pkg::rsp_t)-1:0] neigh_sc_rsp_info_i,
  input  logic                           neigh_sc_rsp_ready_i,
  input  logic [NeighSmGpioWidth-1:0]    neigh_sm_gpio_i,
  input  logic                           hw_dbg_sm_monitor_enabled_i,
  input  logic                           apb_pready_neigh_i,
  input  logic [$bits(apb_to_neigh_t)-1:0] apb_esr_req_i,
  input  logic [UcFccTargetSize-1:0]     uc_to_neigh_fcc_target_i,
  input  logic [MinPerN-1:0]             uc_to_neigh_fcc_i,
  input  logic [MinPerN-1:0]             int_mtip_i,
  input  logic                           plic_meip_i,
  input  logic                           plic_seip_i,
  input  logic [ThreadsPerN-1:0]         esr_to_neigh_ipi_msip_i,
  input  logic [ThreadsPerN-1:0]         esr_to_neigh_ipi_trigger_i,
  input  logic                           flb_l2_neigh_resp_valid_i,
  input  logic                           flb_l2_neigh_resp_data_i,
  input  logic [icache_geom_pkg::IcacheSramDataWidth-1:0] icache_f0_sram_resp_dout_i,
  input  logic                           icache_f0_sram_resp_valid_i,
  input  logic                           icache_f0_sram_resp_ready_i,

  output logic [HiOutWidth-1:0] orig_out_o,
  output logic [HiOutWidth-1:0] new_out_o
);

  dft_t dft_hv;
  dft_t dft_lv;
  assign dft_hv = '{scanmode: dft_reset_byp_hv_i, scan_reset_n: ~dft_reset_hv_i, default: '0};
  assign dft_lv = '{scanmode: dft_reset_byp_lv_i, scan_reset_n: ~dft_reset_lv_i, default: '0};

  logic rst_c_push_ni;
  logic rst_c_pop_ni;
  logic rst_push_ni;
  logic rst_pop_ni;
  assign rst_c_push_ni = rst_ni & ~reset_c_push_i;
  assign rst_c_pop_ni  = rst_ni & ~reset_c_pop_i;
  assign rst_push_ni   = rst_ni & ~reset_push_i;
  assign rst_pop_ni    = rst_ni & ~reset_pop_i;

  minion_pkg::esr_minion_features_t new_esr_minion_features;
  minion_chicken_bits_vc_t          new_chicken_bits;
  esr_pkg::esr_ms_dmctrl_t          new_dmctrl;
  bpam_run_control_neigh_t          new_bpam_run_control;
  icache_pkg::icache_prefetch_conf_t new_prefetch_conf;
  etlink_pkg::rsp_t                 new_neigh_sc_rsp_info;
  apb_to_neigh_t                    new_apb_esr_req;

  assign new_esr_minion_features = minion_pkg::esr_minion_features_t'(esr_minion_features_i);
  assign new_chicken_bits = minion_chicken_bits_vc_t'(chicken_bits_i);
  assign new_dmctrl = esr_pkg::esr_ms_dmctrl_t'(dmctrl_i);
  assign new_bpam_run_control = bpam_run_control_neigh_t'(bpam_run_control_i);
  assign new_prefetch_conf = icache_pkg::icache_prefetch_conf_t'(esr_icache_prefetch_conf_i);
  assign new_neigh_sc_rsp_info = etlink_pkg::rsp_t'(neigh_sc_rsp_info_i);
  assign new_apb_esr_req = apb_to_neigh_t'(apb_esr_req_i);

  logic [NvcNumShireIdsBits-1:0] orig_shire_id_sync;
  logic [NvcNumShireIdsBits-1:0] new_shire_id_sync;
  logic [NumNeighBits-1:0] orig_neigh_id_sync;
  logic [NumNeighBits-1:0] new_neigh_id_sync;
  logic [TboxPerShireBits-1:0] orig_tbox_id_sync;
  logic [TboxPerShireBits-1:0] new_tbox_id_sync;
  logic [$bits(minion_pkg::esr_minion_features_t)-1:0] orig_esr_minion_features_sync;
  minion_pkg::esr_minion_features_t new_esr_minion_features_sync;
  logic [$bits(minion_chicken_bits_vc_t)-1:0] orig_chicken_bits_sync;
  minion_chicken_bits_vc_t new_chicken_bits_sync;
  logic orig_esr_shire_coop_mode_sync;
  logic new_esr_shire_coop_mode_sync;
  logic orig_tbox_en_sync;
  logic new_tbox_en_sync;
  logic [MinPerN-1:0] orig_neigh_t0_en_sync;
  logic [MinPerN-1:0] new_neigh_t0_en_sync;
  logic [MinPerN-1:0] orig_neigh_t1_en_sync;
  logic [MinPerN-1:0] new_neigh_t1_en_sync;
  logic [$bits(esr_pkg::esr_ms_dmctrl_t)-1:0] orig_dmctrl_sync;
  esr_pkg::esr_ms_dmctrl_t new_dmctrl_sync;
  logic [$bits(bpam_run_control_neigh_t)-1:0] orig_bpam_run_control_sync;
  bpam_run_control_neigh_t new_bpam_run_control_sync;
  logic [$bits(icache_pkg::icache_prefetch_conf_t)-1:0] orig_prefetch_conf_sync;
  icache_pkg::icache_prefetch_conf_t new_prefetch_conf_sync;
  logic orig_prefetch_start_sync;
  logic new_prefetch_start_sync;
  logic orig_neigh_sc_rsp_valid_sync;
  logic new_neigh_sc_rsp_valid_sync;
  logic [$bits(etlink_pkg::rsp_t)-1:0] orig_neigh_sc_rsp_info_sync;
  etlink_pkg::rsp_t new_neigh_sc_rsp_info_sync;
  logic orig_neigh_sc_rsp_ready_sync;
  logic new_neigh_sc_rsp_ready_sync;
  logic [NeighSmGpioWidth-1:0] orig_neigh_sm_gpio_sync;
  logic [NeighSmGpioWidth-1:0] new_neigh_sm_gpio_sync;
  logic orig_hw_dbg_sm_monitor_enabled_sync;
  logic new_hw_dbg_sm_monitor_enabled_sync;
  logic [$bits(apb_to_neigh_t)-1:0] orig_apb_esr_req_sync;
  apb_to_neigh_t new_apb_esr_req_sync;
  logic [UcFccTargetSize-1:0] orig_uc_to_neigh_fcc_target_sync;
  logic [UcFccTargetSize-1:0] new_uc_to_neigh_fcc_target_sync;
  logic [MinPerN-1:0] orig_uc_to_neigh_fcc_sync;
  logic [MinPerN-1:0] new_uc_to_neigh_fcc_sync;
  logic [MinPerN-1:0] orig_int_mtip_sync;
  logic [MinPerN-1:0] new_int_mtip_sync;
  logic orig_plic_meip_sync;
  logic new_plic_meip_sync;
  logic orig_plic_seip_sync;
  logic new_plic_seip_sync;
  logic [ThreadsPerN-1:0] orig_ipi_msip_sync;
  logic [ThreadsPerN-1:0] new_ipi_msip_sync;
  logic [ThreadsPerN-1:0] orig_ipi_trigger_sync;
  logic [ThreadsPerN-1:0] new_ipi_trigger_sync;
  logic orig_flb_resp_valid_sync;
  logic new_flb_resp_valid_sync;
  logic orig_flb_resp_data_sync;
  logic new_flb_resp_data_sync;
  logic [icache_geom_pkg::IcacheSramDataWidth-1:0] orig_icache_dout_sync;
  logic [icache_geom_pkg::IcacheSramDataWidth-1:0] new_icache_dout_sync;
  logic orig_icache_valid_sync;
  logic new_icache_valid_sync;
  logic orig_icache_ready_sync;
  logic new_icache_ready_sync;

  neigh_hi_voltage_cross #(
    .FifoDepth(FifoDepth)
  ) u_new (
    .rst_c_push_ni                    (rst_c_push_ni),
    .rst_c_pop_ni                     (rst_c_pop_ni),
    .rst_push_ni                      (rst_push_ni),
    .rst_pop_ni                       (rst_pop_ni),
    .clk_push_i                       (clk_i),
    .clk_pop_i                        (clk_i),
    .dft_hv_i                         (dft_hv),
    .dft_lv_i                         (dft_lv),
    .shire_id_i                       (shire_id_i),
    .shire_id_sync_o                  (new_shire_id_sync),
    .neigh_id_i                       (neigh_id_i),
    .neigh_id_sync_o                  (new_neigh_id_sync),
    .tbox_id_i                        (tbox_id_i),
    .tbox_id_sync_o                   (new_tbox_id_sync),
    .esr_minion_features_i            (new_esr_minion_features),
    .esr_minion_features_sync_o       (new_esr_minion_features_sync),
    .chicken_bits_i                   (new_chicken_bits),
    .chicken_bits_sync_o              (new_chicken_bits_sync),
    .esr_shire_coop_mode_i            (esr_shire_coop_mode_i),
    .esr_shire_coop_mode_sync_o       (new_esr_shire_coop_mode_sync),
    .tbox_en_i                        (tbox_en_i),
    .tbox_en_sync_o                   (new_tbox_en_sync),
    .neigh_t0_en_i                    (neigh_t0_en_i),
    .neigh_t0_en_sync_o               (new_neigh_t0_en_sync),
    .neigh_t1_en_i                    (neigh_t1_en_i),
    .neigh_t1_en_sync_o               (new_neigh_t1_en_sync),
    .dmctrl_i                         (new_dmctrl),
    .dmctrl_sync_o                    (new_dmctrl_sync),
    .bpam_run_control_i               (new_bpam_run_control),
    .bpam_run_control_sync_o          (new_bpam_run_control_sync),
    .esr_icache_prefetch_conf_i       (new_prefetch_conf),
    .esr_icache_prefetch_conf_sync_o  (new_prefetch_conf_sync),
    .esr_icache_prefetch_start_i      (esr_icache_prefetch_start_i),
    .esr_icache_prefetch_start_sync_o (new_prefetch_start_sync),
    .neigh_sc_rsp_valid_i             (neigh_sc_rsp_valid_i),
    .neigh_sc_rsp_valid_sync_o        (new_neigh_sc_rsp_valid_sync),
    .neigh_sc_rsp_info_i              (new_neigh_sc_rsp_info),
    .neigh_sc_rsp_info_sync_o         (new_neigh_sc_rsp_info_sync),
    .neigh_sc_rsp_ready_i             (neigh_sc_rsp_ready_i),
    .neigh_sc_rsp_ready_sync_o        (new_neigh_sc_rsp_ready_sync),
    .neigh_sm_gpio_i                  (neigh_sm_gpio_i),
    .neigh_sm_gpio_sync_o             (new_neigh_sm_gpio_sync),
    .hw_dbg_sm_monitor_enabled_i      (hw_dbg_sm_monitor_enabled_i),
    .hw_dbg_sm_monitor_enabled_sync_o (new_hw_dbg_sm_monitor_enabled_sync),
    .apb_pready_neigh_i               (apb_pready_neigh_i),
    .apb_esr_req_i                    (new_apb_esr_req),
    .apb_esr_req_sync_o               (new_apb_esr_req_sync),
    .uc_to_neigh_fcc_target_i         (uc_to_neigh_fcc_target_i),
    .uc_to_neigh_fcc_target_sync_o    (new_uc_to_neigh_fcc_target_sync),
    .uc_to_neigh_fcc_i                (uc_to_neigh_fcc_i),
    .uc_to_neigh_fcc_sync_o           (new_uc_to_neigh_fcc_sync),
    .int_mtip_i                       (int_mtip_i),
    .plic_meip_i                      (plic_meip_i),
    .plic_seip_i                      (plic_seip_i),
    .int_mtip_sync_o                  (new_int_mtip_sync),
    .plic_meip_sync_o                 (new_plic_meip_sync),
    .plic_seip_sync_o                 (new_plic_seip_sync),
    .esr_to_neigh_ipi_msip_i          (esr_to_neigh_ipi_msip_i),
    .esr_to_neigh_ipi_msip_sync_o     (new_ipi_msip_sync),
    .esr_to_neigh_ipi_trigger_i       (esr_to_neigh_ipi_trigger_i),
    .esr_to_neigh_ipi_trigger_sync_o  (new_ipi_trigger_sync),
    .flb_l2_neigh_resp_valid_i        (flb_l2_neigh_resp_valid_i),
    .flb_l2_neigh_resp_valid_sync_o   (new_flb_resp_valid_sync),
    .flb_l2_neigh_resp_data_i         (flb_l2_neigh_resp_data_i),
    .flb_l2_neigh_resp_data_sync_o    (new_flb_resp_data_sync),
    .icache_f0_sram_resp_dout_i       (icache_f0_sram_resp_dout_i),
    .icache_f0_sram_resp_dout_sync_o  (new_icache_dout_sync),
    .icache_f0_sram_resp_valid_i      (icache_f0_sram_resp_valid_i),
    .icache_f0_sram_resp_valid_sync_o (new_icache_valid_sync),
    .icache_f0_sram_resp_ready_i      (icache_f0_sram_resp_ready_i),
    .icache_f0_sram_resp_ready_sync_o (new_icache_ready_sync)
  );

  neigh_hi_voltage_cross_orig #(
    .NUM_ELEMS(FifoDepth)
  ) u_orig (
    .reset_c_push                    (!rst_c_push_ni),
    .reset_c_pop                     (!rst_c_pop_ni),
    .reset_push                      (!rst_push_ni),
    .reset_pop                       (!rst_pop_ni),
    .clock_push                      (clk_i),
    .clock_pop                       (clk_i),
    .dft__reset_byp_hv               (dft_reset_byp_hv_i),
    .dft__reset_hv                   (dft_reset_hv_i),
    .dft__reset_byp_lv               (dft_reset_byp_lv_i),
    .dft__reset_lv                   (dft_reset_lv_i),
    .shire_id                        (shire_id_i),
    .shire_id_sync                   (orig_shire_id_sync),
    .neigh_id                        (neigh_id_i),
    .neigh_id_sync                   (orig_neigh_id_sync),
    .tbox_id                         (tbox_id_i),
    .tbox_id_sync                    (orig_tbox_id_sync),
    .esr_minion_features             (esr_minion_features_i),
    .esr_minion_features_sync        (orig_esr_minion_features_sync),
    .chicken_bits                    (chicken_bits_i),
    .chicken_bits_sync               (orig_chicken_bits_sync),
    .esr_shire_coop_mode             (esr_shire_coop_mode_i),
    .esr_shire_coop_mode_sync        (orig_esr_shire_coop_mode_sync),
    .tbox_en                         (tbox_en_i),
    .tbox_en_sync                    (orig_tbox_en_sync),
    .neigh_t0_en                     (neigh_t0_en_i),
    .neigh_t0_en_sync                (orig_neigh_t0_en_sync),
    .neigh_t1_en                     (neigh_t1_en_i),
    .neigh_t1_en_sync                (orig_neigh_t1_en_sync),
    .dmctrl                          (dmctrl_i),
    .dmctrl_sync                     (orig_dmctrl_sync),
    .bpam_run_control                (bpam_run_control_i),
    .bpam_run_control_sync           (orig_bpam_run_control_sync),
    .esr_icache_prefetch_conf        (esr_icache_prefetch_conf_i),
    .esr_icache_prefetch_conf_sync   (orig_prefetch_conf_sync),
    .esr_icache_prefetch_start       (esr_icache_prefetch_start_i),
    .esr_icache_prefetch_start_sync  (orig_prefetch_start_sync),
    .neigh_sc_rsp_valid              (neigh_sc_rsp_valid_i),
    .neigh_sc_rsp_valid_sync         (orig_neigh_sc_rsp_valid_sync),
    .neigh_sc_rsp_info               (neigh_sc_rsp_info_i),
    .neigh_sc_rsp_info_sync          (orig_neigh_sc_rsp_info_sync),
    .neigh_sc_rsp_ready              (neigh_sc_rsp_ready_i),
    .neigh_sc_rsp_ready_sync         (orig_neigh_sc_rsp_ready_sync),
    .neigh_sm_gpio                   (neigh_sm_gpio_i),
    .neigh_sm_gpio_sync              (orig_neigh_sm_gpio_sync),
    .hw_dbg_sm_monitor_enabled       (hw_dbg_sm_monitor_enabled_i),
    .hw_dbg_sm_monitor_enabled_sync  (orig_hw_dbg_sm_monitor_enabled_sync),
    .apb_pready_neigh                (apb_pready_neigh_i),
    .apb_esr_req                     (apb_esr_req_i),
    .apb_esr_req_sync                (orig_apb_esr_req_sync),
    .uc_to_neigh_fcc_target          (uc_to_neigh_fcc_target_i),
    .uc_to_neigh_fcc_target_sync     (orig_uc_to_neigh_fcc_target_sync),
    .uc_to_neigh_fcc                 (uc_to_neigh_fcc_i),
    .uc_to_neigh_fcc_sync            (orig_uc_to_neigh_fcc_sync),
    .int_mtip                        (int_mtip_i),
    .plic_meip                       (plic_meip_i),
    .plic_seip                       (plic_seip_i),
    .int_mtip_sync                   (orig_int_mtip_sync),
    .plic_meip_sync                  (orig_plic_meip_sync),
    .plic_seip_sync                  (orig_plic_seip_sync),
    .esr_to_neigh_ipi_msip           (esr_to_neigh_ipi_msip_i),
    .esr_to_neigh_ipi_msip_sync      (orig_ipi_msip_sync),
    .esr_to_neigh_ipi_trigger        (esr_to_neigh_ipi_trigger_i),
    .esr_to_neigh_ipi_trigger_sync   (orig_ipi_trigger_sync),
    .flb_l2_neigh_resp_valid         (flb_l2_neigh_resp_valid_i),
    .flb_l2_neigh_resp_valid_sync    (orig_flb_resp_valid_sync),
    .flb_l2_neigh_resp_data          (flb_l2_neigh_resp_data_i),
    .flb_l2_neigh_resp_data_sync     (orig_flb_resp_data_sync),
    .icache_f0_sram_resp_dout        (icache_f0_sram_resp_dout_i),
    .icache_f0_sram_resp_dout_sync   (orig_icache_dout_sync),
    .icache_f0_sram_resp_valid       (icache_f0_sram_resp_valid_i),
    .icache_f0_sram_resp_valid_sync  (orig_icache_valid_sync),
    .icache_f0_sram_resp_ready       (icache_f0_sram_resp_ready_i),
    .icache_f0_sram_resp_ready_sync  (orig_icache_ready_sync)
  );

  assign orig_out_o = {orig_shire_id_sync,
                       orig_neigh_id_sync,
                       orig_tbox_id_sync,
                       orig_esr_minion_features_sync,
                       orig_chicken_bits_sync,
                       orig_esr_shire_coop_mode_sync,
                       orig_tbox_en_sync,
                       orig_neigh_t0_en_sync,
                       orig_neigh_t1_en_sync,
                       orig_dmctrl_sync,
                       orig_bpam_run_control_sync,
                       orig_prefetch_conf_sync,
                       orig_prefetch_start_sync,
                       orig_neigh_sc_rsp_valid_sync,
                       orig_neigh_sc_rsp_info_sync,
                       orig_neigh_sc_rsp_ready_sync,
                       orig_neigh_sm_gpio_sync,
                       orig_hw_dbg_sm_monitor_enabled_sync,
                       orig_apb_esr_req_sync,
                       orig_uc_to_neigh_fcc_target_sync,
                       orig_uc_to_neigh_fcc_sync,
                       orig_int_mtip_sync,
                       orig_plic_meip_sync,
                       orig_plic_seip_sync,
                       orig_ipi_msip_sync,
                       orig_ipi_trigger_sync,
                       orig_flb_resp_valid_sync,
                       orig_flb_resp_data_sync,
                       orig_icache_dout_sync,
                       orig_icache_valid_sync,
                       orig_icache_ready_sync};

  assign new_out_o = {new_shire_id_sync,
                      new_neigh_id_sync,
                      new_tbox_id_sync,
                      new_esr_minion_features_sync,
                      new_chicken_bits_sync,
                      new_esr_shire_coop_mode_sync,
                      new_tbox_en_sync,
                      new_neigh_t0_en_sync,
                      new_neigh_t1_en_sync,
                      new_dmctrl_sync,
                      new_bpam_run_control_sync,
                      new_prefetch_conf_sync,
                      new_prefetch_start_sync,
                      new_neigh_sc_rsp_valid_sync,
                      new_neigh_sc_rsp_info_sync,
                      new_neigh_sc_rsp_ready_sync,
                      new_neigh_sm_gpio_sync,
                      new_hw_dbg_sm_monitor_enabled_sync,
                      new_apb_esr_req_sync,
                      new_uc_to_neigh_fcc_target_sync,
                      new_uc_to_neigh_fcc_sync,
                      new_int_mtip_sync,
                      new_plic_meip_sync,
                      new_plic_seip_sync,
                      new_ipi_msip_sync,
                      new_ipi_trigger_sync,
                      new_flb_resp_valid_sync,
                      new_flb_resp_data_sync,
                      new_icache_dout_sync,
                      new_icache_valid_sync,
                      new_icache_ready_sync};

endmodule : cosim_neigh_hi_voltage_cross_tb
