// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_esr_shire_other_tb (
  input  logic        clk_i,
  input  logic        rst_c_ni,
  input  logic        rst_w_ni,
  input  logic        rst_d_ni,
  input  logic        test_en_i,
  input  logic        dft_reset_byp_i,
  input  logic        dft_reset_i,
  input  logic [shire_esr_pkg::NumShireIdsBits-1:0] shire_id_reset_val_i,

  input  logic [15:0] apb_paddr_i,
  input  logic        apb_pwrite_i,
  input  logic        apb_psel_i,
  input  logic        apb_penable_i,
  input  logic [63:0] apb_pwdata_i,

  input  logic [63:0] status_seed_i,
  input  logic [3:0]  prefetch_done_i,
  input  logic [3:0]  dll_sts_done_i,
  input  logic [31:0] dll_sts_errn_i,
  input  logic [3:0]  tbox_halt_ack_i,
  input  logic [3:0]  tbox_resume_ack_i,
  input  logic        rbox_halt_ack_i,
  input  logic        rbox_resume_ack_i,
  input  logic        rbox_reset_ack_i,

  output logic        orig_apb_pready_o,
  output logic [63:0] orig_apb_prdata_o,
  output logic        orig_apb_pslverr_o,
  output logic        new_apb_pready_o,
  output logic [63:0] new_apb_prdata_o,
  output logic        new_apb_pslverr_o,

  output logic [63:0] orig_minion_feature_o,
  output logic [63:0] new_minion_feature_o,
  output logic [63:0] orig_shire_config_o,
  output logic [63:0] new_shire_config_o,
  output logic [63:0] orig_ipi_trigger_o,
  output logic [63:0] new_ipi_trigger_o,
  output logic [31:0] orig_thread0_enable_o,
  output logic [31:0] new_thread0_enable_o,
  output logic [31:0] orig_thread1_enable_o,
  output logic [31:0] new_thread1_enable_o,
  output logic [63:0] orig_ipi_redirect_trigger_o,
  output logic [63:0] new_ipi_redirect_trigger_o,
  output logic [31:0] orig_mtime_target_o,
  output logic [31:0] new_mtime_target_o,
  output logic [3:0]  orig_power_glb_nsleepin_o,
  output logic [3:0]  new_power_glb_nsleepin_o,
  output logic [3:0]  orig_power_glb_iso_o,
  output logic [3:0]  new_power_glb_iso_o,
  output logic [31:0] orig_power_neigh_nsleepin_o,
  output logic [31:0] new_power_neigh_nsleepin_o,
  output logic [31:0] orig_power_neigh_iso_o,
  output logic [31:0] new_power_neigh_iso_o,
  output logic        orig_shire_coop_mode_o,
  output logic        new_shire_coop_mode_o,
  output logic [63:0] orig_uc_config_o,
  output logic [63:0] new_uc_config_o,
  output logic [63:0] orig_clk_gate_ctrl_o,
  output logic [63:0] new_clk_gate_ctrl_o,
  output logic        orig_debug_clk_gate_ctrl_o,
  output logic        new_debug_clk_gate_ctrl_o,
  output logic [7:0]  orig_eco_o,
  output logic [7:0]  new_eco_o,
  output logic [63:0] orig_clk_dly_ctl_o,
  output logic [63:0] new_clk_dly_ctl_o,
  output logic [63:0] orig_pll_ctrl_o,
  output logic [63:0] new_pll_ctrl_o,
  output logic [63:0] orig_pll_conf0_o,
  output logic [63:0] new_pll_conf0_o,
  output logic [63:0] orig_pll_conf1_o,
  output logic [63:0] new_pll_conf1_o,
  output logic [63:0] orig_pll_conf2_o,
  output logic [63:0] new_pll_conf2_o,
  output logic [63:0] orig_pll_conf3_o,
  output logic [63:0] new_pll_conf3_o,
  output logic [63:0] orig_dll_ctrl_o,
  output logic [63:0] new_dll_ctrl_o,
  output logic [63:0] orig_dll_conf0_o,
  output logic [63:0] new_dll_conf0_o,
  output logic [63:0] orig_dll_conf1_o,
  output logic [63:0] new_dll_conf1_o,
  output logic [63:0] orig_clockmux_o,
  output logic [63:0] new_clockmux_o,
  output logic [63:0] orig_dll_dly_est_ctl_o,
  output logic [63:0] new_dll_dly_est_ctl_o,
  output logic [63:0] orig_ram_cfg_o,
  output logic [63:0] new_ram_cfg_o,
  output logic [31:0] orig_barrier_en_o,
  output logic [31:0] new_barrier_en_o,
  output logic [3:0]  orig_fcc_en_o,
  output logic [3:0]  new_fcc_en_o,
  output logic [63:0] orig_prefetch_conf_o,
  output logic [63:0] new_prefetch_conf_o,
  output logic [3:0]  orig_prefetch_start_o,
  output logic [3:0]  new_prefetch_start_o,
  output logic [4:0]  orig_debug_ss_o,
  output logic [4:0]  new_debug_ss_o
);
  localparam int unsigned NumNeigh = shire_esr_pkg::NumNeigh;
  localparam int unsigned NumBanks = shirecache_pkg::Banks;

  dft_pkg::dft_t new_dft;

  esr_minion_features_t orig_minion_feature;
  esr_shire_config_t orig_shire_config;
  esr_and_or_tree_L1_t orig_and_or_tree_l1;
  esr_uc_config_t orig_uc_config;
  esr_clk_gate_ctrl_t orig_clk_gate_ctrl;
  esr_clk_dly_ctl_t orig_clk_dly_ctl;
  icache_prefetch_conf_t orig_prefetch_conf;
  uc_esr_values_t orig_uc_values;
  uc_esr_enable_sigs orig_uc_enables;
  esr_clkmux_ctl_t orig_clockmux;
  esr_dll_dly_est_ctl_t orig_dll_dly_est_ctl;
  esr_dll_dly_est_sts_t orig_dll_dly_est_sts [NumNeigh-1:0];
  esr_shire_cache_ram_cfg_t orig_ram_cfg_full;
  bpam_rc_tbox_ack_t [NumNeigh-1:0] orig_tbox_ack;
  bpam_rc_rbox_ack_t orig_rbox_ack;

  minion_pkg::esr_minion_features_t new_minion_feature;
  shirecache_pkg::esr_shire_config_t new_shire_config;
  shire_esr_pkg::esr_and_or_tree_l1_t new_and_or_tree_l1;
  shire_esr_pkg::esr_uc_config_t new_uc_config;
  shire_esr_pkg::uc_esr_values_t new_uc_values;
  shire_esr_pkg::uc_esr_enable_sigs_t new_uc_enables;
  shire_esr_pkg::esr_pll_auto_config_t new_pll_ctrl;
  shire_esr_pkg::esr_dll_auto_config_t new_dll_ctrl;
  shire_esr_pkg::esr_clkmux_ctl_t new_clockmux;
  shire_esr_pkg::esr_clk_gate_ctrl_t new_clk_gate_ctrl;
  shire_esr_pkg::esr_clk_dly_ctl_t new_clk_dly_ctl;
  ram_cfg_pkg::ram_cfg_t new_ram_cfg;
  icache_pkg::icache_prefetch_conf_t new_prefetch_conf;
  esr_pkg::esr_dll_dly_est_ctl_t new_dll_dly_est_ctl;
  esr_pkg::esr_dll_dly_est_sts_t new_dll_dly_est_sts [NumNeigh-1:0];
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t new_tbox_ack [NumNeigh-1:0];
  shire_esr_pkg::bpam_rc_rbox_ack_t new_rbox_ack;

  logic [$bits(esr_pll_auto_config_t)-1:0] orig_pll_ctrl;
  logic [$bits(esr_dll_auto_config_t)-1:0] orig_dll_ctrl;
  logic [shire_esr_pkg::ShirePllConfBits-1:0] orig_pll_conf;
  logic [shire_esr_pkg::ShireDllConfBits-1:0] orig_dll_conf;
  logic [shire_esr_pkg::ShirePllConfBits-1:0] new_pll_conf;
  logic [shire_esr_pkg::ShireDllConfBits-1:0] new_dll_conf;
  ram_cfg_pkg::ram_cfg_t orig_ram_cfg_native;

  assign new_dft.scanmode = dft_reset_byp_i;
  assign new_dft.scan_reset_n = ~dft_reset_i;
  assign new_dft.sram_clk_override = 1'b0;
  assign new_dft.ram_rei = 1'b0;
  assign new_dft.ram_wei = 1'b0;

  always_comb begin
    orig_and_or_tree_l1 = status_seed_i[$bits(esr_and_or_tree_L1_t)-1:0];
    new_and_or_tree_l1 = status_seed_i[$bits(shire_esr_pkg::esr_and_or_tree_l1_t)-1:0];
    orig_uc_values = '0;
    new_uc_values = '0;
    for (int unsigned i = 0; i < shire_esr_pkg::BarrierRegisters; i++) begin
      orig_uc_values.barriers[i] = status_seed_i[7:0] + i[7:0];
      new_uc_values.barriers[i] = status_seed_i[7:0] + i[7:0];
    end
    for (int unsigned i = 0; i < NumNeigh; i++) begin
      orig_dll_dly_est_sts[i].sts_done = dll_sts_done_i[i];
      orig_dll_dly_est_sts[i].sts_errn = dll_sts_errn_i[(8*i) +: 8];
      new_dll_dly_est_sts[i].sts_done = dll_sts_done_i[i];
      new_dll_dly_est_sts[i].sts_errn = dll_sts_errn_i[(8*i) +: 8];
      orig_tbox_ack[i].halt_ack = tbox_halt_ack_i[i];
      orig_tbox_ack[i].resume_ack = tbox_resume_ack_i[i];
      new_tbox_ack[i].halt_ack = tbox_halt_ack_i[i];
      new_tbox_ack[i].resume_ack = tbox_resume_ack_i[i];
    end
    orig_rbox_ack.halt_ack = rbox_halt_ack_i;
    orig_rbox_ack.resume_ack = rbox_resume_ack_i;
    orig_rbox_ack.reset_ack = rbox_reset_ack_i;
    new_rbox_ack.halt_ack = rbox_halt_ack_i;
    new_rbox_ack.resume_ack = rbox_resume_ack_i;
    new_rbox_ack.reset_ack = rbox_reset_ack_i;

    orig_ram_cfg_native = '0;
    orig_ram_cfg_native.test_en = orig_ram_cfg_full.cfg2.sc_mbd_test1 |
                                  orig_ram_cfg_full.cfg2.sc_mbd_test_rnm;
    orig_ram_cfg_native.rm = orig_ram_cfg_full.cfg2.sc_mbd_rm;
    orig_ram_cfg_native.rme = orig_ram_cfg_full.cfg2.sc_mbd_rme;
    orig_ram_cfg_native.ra = orig_ram_cfg_full.cfg2.sc_mbd_ra;
    orig_ram_cfg_native.wa = orig_ram_cfg_full.cfg2.sc_mbd_wa[1:0];
    orig_ram_cfg_native.wpulse = orig_ram_cfg_full.cfg2.sc_mbd_wpulse;
  end

  esr_shire_other_orig #(
    .NUM_BANKS          (NumBanks),
    .NUM_NEIGH          (NumNeigh),
    .AD_WIDTH           (16),
    .SETS_PER_SUB_BANK  (shirecache_pkg::SetsPerSubBank),
    .NUM_REQQ_ENTRIES   (shirecache_pkg::ReqqDepth)
  ) u_orig (
    .clock                              (clk_i),
    .reset_c                            (!rst_c_ni),
    .reset_w                            (!rst_w_ni),
    .reset_d                            (!rst_d_ni),
    .test_en                            (test_en_i),
    .esr_minion_feature_op              (orig_minion_feature),
    .esr_shire_config_op                (orig_shire_config),
    .esr_shire_cache_build_config_ip    (status_seed_i ^ 64'h1234_5678_9abc_def0),
    .esr_shire_cache_revision_id_ip     (status_seed_i ^ 64'h0fed_cba9_8765_4321),
    .esr_ipi_trigger_op                 (orig_ipi_trigger_o),
    .esr_and_or_treeL1_ip               (orig_and_or_tree_l1),
    .esr_mtime_local_target_op          (orig_mtime_target_o),
    .esr_power_ctrl_neigh_nsleepout_ip  (status_seed_i[31:0]),
    .esr_shire_coop_mode_op             (orig_shire_coop_mode_o),
    .esr_shire_noc_interrupt_status_ip  ({2'b0, status_seed_i[shire_esr_pkg::NocIntNum-1:0]}),
    .esr_uc_config_op                   (orig_uc_config),
    .esr_clk_gate_ctrl_op               (orig_clk_gate_ctrl),
    .esr_debug_clk_gate_ctrl_op         (orig_debug_clk_gate_ctrl_o),
    .esr_shire_channel_eco_ctl_op       (orig_eco_o),
    .esr_clk_dly_ctl_op                 (orig_clk_dly_ctl),
    .shire_id_reset_val                 (shire_id_reset_val_i),
    .esr_icache_prefetch_conf           (orig_prefetch_conf),
    .esr_icache_prefetch_start          (orig_prefetch_start_o),
    .esr_icache_prefetch_done           (prefetch_done_i),
    .esr_thread0_enable                 (orig_thread0_enable_o),
    .esr_thread1_enable                 (orig_thread1_enable_o),
    .neigh_sc_err_detected_ip           (status_seed_i[3:0]),
    .neigh_sc_err_logged_ip             (status_seed_i[7:4]),
    .sc_bank_err_detected_ip            (status_seed_i[11:8]),
    .sc_bank_err_logged_ip              (status_seed_i[15:12]),
    .esr_ipi_redirect_trigger_op        (orig_ipi_redirect_trigger_o),
    .uc_esr_values                      (orig_uc_values),
    .uc_esr_enables                     (orig_uc_enables),
    .esr_power_ctrl_glb_nsleepin_op     (orig_power_glb_nsleepin_o),
    .esr_power_ctrl_glb_iso_op          (orig_power_glb_iso_o),
    .esr_power_ctrl_glb_nsleepout_ip    (status_seed_i[19:16]),
    .esr_power_ctrl_neigh_nsleepin_op   (orig_power_neigh_nsleepin_o),
    .esr_power_ctrl_neigh_iso_op        (orig_power_neigh_iso_o),
    .esr_pll_ctrl_op                    (orig_pll_ctrl),
    .esp_pll_conf_op                    (orig_pll_conf),
    .esr_pll_busy_ip                    (status_seed_i[20]),
    .esr_pll_rdata_ip                   (status_seed_i[35:20]),
    .esr_pll_rrdy_ip                    (status_seed_i[36]),
    .esr_pll_lock_ip                    (status_seed_i[37]),
    .esr_dll_ctrl_op                    (orig_dll_ctrl),
    .esp_dll_conf_op                    (orig_dll_conf),
    .esr_dll_busy_ip                    (status_seed_i[38]),
    .esr_dll_rdata_ip                   (status_seed_i[54:39]),
    .esr_dll_rrdy_ip                    (status_seed_i[55]),
    .esr_dll_lock_ip                    (status_seed_i[56]),
    .esr_shire_ctrl_clockmux_op         (orig_clockmux),
    .esr_dll_dly_est_ctl_op             (orig_dll_dly_est_ctl),
    .esr_dll_dly_est_sts_0_ip           (orig_dll_dly_est_sts[0]),
    .esr_dll_dly_est_sts_1_ip           (orig_dll_dly_est_sts[1]),
    .esr_dll_dly_est_sts_2_ip           (orig_dll_dly_est_sts[2]),
    .esr_dll_dly_est_sts_3_ip           (orig_dll_dly_est_sts[3]),
    .esr_shire_cache_ram_cfg            (orig_ram_cfg_full),
    .debug_rc_tbox_ack                  (orig_tbox_ack),
    .debug_rc_rbox_ack                  (orig_rbox_ack),
    .debug_rc_ss_tbox_rbox              (orig_debug_ss_o),
    .dft__reset_byp                     (dft_reset_byp_i),
    .dft__reset                         (dft_reset_i),
    .apb_paddr                          (apb_paddr_i),
    .apb_pwrite                         (apb_pwrite_i),
    .apb_psel                           (apb_psel_i),
    .apb_penable                        (apb_penable_i),
    .apb_pwdata                         (apb_pwdata_i),
    .apb_pready                         (orig_apb_pready_o),
    .apb_prdata                         (orig_apb_prdata_o),
    .apb_pslverr                        (orig_apb_pslverr_o)
  );

  esr_shire_other u_new (
    .clk_i                              (clk_i),
    .rst_c_ni                           (rst_c_ni),
    .rst_w_ni                           (rst_w_ni),
    .rst_d_ni                           (rst_d_ni),
    .test_en_i                          (test_en_i),
    .dft_i                              (new_dft),
    .shire_id_reset_val_i               (shire_id_reset_val_i),
    .esr_minion_feature_o               (new_minion_feature),
    .esr_shire_config_o                 (new_shire_config),
    .esr_shire_cache_build_config_i     (status_seed_i ^ 64'h1234_5678_9abc_def0),
    .esr_shire_cache_revision_id_i      (status_seed_i ^ 64'h0fed_cba9_8765_4321),
    .esr_ipi_trigger_o                  (new_ipi_trigger_o),
    .esr_and_or_tree_l1_i               (new_and_or_tree_l1),
    .esr_mtime_local_target_o           (new_mtime_target_o),
    .esr_power_ctrl_neigh_nsleepout_i   (status_seed_i[31:0]),
    .esr_shire_coop_mode_o              (new_shire_coop_mode_o),
    .esr_shire_noc_interrupt_status_i   (status_seed_i[shire_esr_pkg::NocIntNum-1:0]),
    .esr_uc_config_o                    (new_uc_config),
    .esr_clk_gate_ctrl_o                (new_clk_gate_ctrl),
    .esr_debug_clk_gate_ctrl_o          (new_debug_clk_gate_ctrl_o),
    .esr_shire_channel_eco_ctl_o        (new_eco_o),
    .esr_clk_dly_ctl_o                  (new_clk_dly_ctl),
    .esr_icache_prefetch_conf_o         (new_prefetch_conf),
    .esr_icache_prefetch_start_o        (new_prefetch_start_o),
    .esr_icache_prefetch_done_i         (prefetch_done_i),
    .esr_thread0_enable_o               (new_thread0_enable_o),
    .esr_thread1_enable_o               (new_thread1_enable_o),
    .neigh_sc_err_detected_i            (status_seed_i[3:0]),
    .neigh_sc_err_logged_i              (status_seed_i[7:4]),
    .sc_bank_err_detected_i             (status_seed_i[11:8]),
    .sc_bank_err_logged_i               (status_seed_i[15:12]),
    .esr_ipi_redirect_trigger_o         (new_ipi_redirect_trigger_o),
    .uc_esr_values_i                    (new_uc_values),
    .uc_esr_enables_o                   (new_uc_enables),
    .esr_power_ctrl_glb_nsleepin_o      (new_power_glb_nsleepin_o),
    .esr_power_ctrl_glb_iso_o           (new_power_glb_iso_o),
    .esr_power_ctrl_glb_nsleepout_i     (status_seed_i[19:16]),
    .esr_power_ctrl_neigh_nsleepin_o    (new_power_neigh_nsleepin_o),
    .esr_power_ctrl_neigh_iso_o         (new_power_neigh_iso_o),
    .esr_pll_ctrl_o                     (new_pll_ctrl),
    .esr_pll_conf_o                     (new_pll_conf),
    .esr_pll_busy_i                     (status_seed_i[20]),
    .esr_pll_rdata_i                    (status_seed_i[35:20]),
    .esr_pll_rrdy_i                     (status_seed_i[36]),
    .esr_pll_lock_i                     (status_seed_i[37]),
    .esr_dll_ctrl_o                     (new_dll_ctrl),
    .esr_dll_conf_o                     (new_dll_conf),
    .esr_dll_busy_i                     (status_seed_i[38]),
    .esr_dll_rdata_i                    (status_seed_i[54:39]),
    .esr_dll_rrdy_i                     (status_seed_i[55]),
    .esr_dll_lock_i                     (status_seed_i[56]),
    .esr_shire_ctrl_clockmux_o          (new_clockmux),
    .esr_dll_dly_est_ctl_o              (new_dll_dly_est_ctl),
    .esr_dll_dly_est_sts_i              (new_dll_dly_est_sts),
    .ram_cfg_o                          (new_ram_cfg),
    .debug_rc_tbox_ack_i                (new_tbox_ack),
    .debug_rc_rbox_ack_i                (new_rbox_ack),
    .debug_rc_ss_tbox_rbox_o            (new_debug_ss_o),
    .apb_paddr_i                        (apb_paddr_i),
    .apb_pwrite_i                       (apb_pwrite_i),
    .apb_psel_i                         (apb_psel_i),
    .apb_penable_i                      (apb_penable_i),
    .apb_pwdata_i                       (apb_pwdata_i),
    .apb_pready_o                       (new_apb_pready_o),
    .apb_prdata_o                       (new_apb_prdata_o),
    .apb_pslverr_o                      (new_apb_pslverr_o)
  );

  assign orig_minion_feature_o = {{(64-$bits(esr_minion_features_t)){1'b0}}, orig_minion_feature};
  assign new_minion_feature_o = {{(64-$bits(minion_pkg::esr_minion_features_t)){1'b0}}, new_minion_feature};
  assign orig_shire_config_o = {{(64-$bits(esr_shire_config_t)){1'b0}}, orig_shire_config};
  assign new_shire_config_o = {{(64-$bits(shirecache_pkg::esr_shire_config_t)){1'b0}}, new_shire_config};
  assign orig_uc_config_o = {{(64-$bits(esr_uc_config_t)){1'b0}}, orig_uc_config};
  assign new_uc_config_o = {{(64-$bits(shire_esr_pkg::esr_uc_config_t)){1'b0}}, new_uc_config};
  assign orig_clk_gate_ctrl_o = {{(64-$bits(esr_clk_gate_ctrl_t)){1'b0}}, orig_clk_gate_ctrl};
  assign new_clk_gate_ctrl_o = {{(64-$bits(shire_esr_pkg::esr_clk_gate_ctrl_t)){1'b0}}, new_clk_gate_ctrl};
  assign orig_clk_dly_ctl_o = {{(64-$bits(esr_clk_dly_ctl_t)){1'b0}}, orig_clk_dly_ctl};
  assign new_clk_dly_ctl_o = {{(64-$bits(shire_esr_pkg::esr_clk_dly_ctl_t)){1'b0}}, new_clk_dly_ctl};
  assign orig_pll_ctrl_o = {{(64-$bits(esr_pll_auto_config_t)){1'b0}}, orig_pll_ctrl};
  assign new_pll_ctrl_o = {{(64-$bits(shire_esr_pkg::esr_pll_auto_config_t)){1'b0}}, new_pll_ctrl};
  assign orig_pll_conf0_o = orig_pll_conf[0 +: 64];
  assign orig_pll_conf1_o = orig_pll_conf[64 +: 64];
  assign orig_pll_conf2_o = orig_pll_conf[128 +: 64];
  assign orig_pll_conf3_o = orig_pll_conf[192 +: 64];
  assign new_pll_conf0_o = new_pll_conf[0 +: 64];
  assign new_pll_conf1_o = new_pll_conf[64 +: 64];
  assign new_pll_conf2_o = new_pll_conf[128 +: 64];
  assign new_pll_conf3_o = new_pll_conf[192 +: 64];
  assign orig_dll_ctrl_o = {{(64-$bits(esr_dll_auto_config_t)){1'b0}}, orig_dll_ctrl};
  assign new_dll_ctrl_o = {{(64-$bits(shire_esr_pkg::esr_dll_auto_config_t)){1'b0}}, new_dll_ctrl};
  assign orig_dll_conf0_o = orig_dll_conf[0 +: 64];
  assign orig_dll_conf1_o = orig_dll_conf[64 +: 64];
  assign new_dll_conf0_o = new_dll_conf[0 +: 64];
  assign new_dll_conf1_o = new_dll_conf[64 +: 64];
  assign orig_clockmux_o = {{(64-$bits(esr_clkmux_ctl_t)){1'b0}}, orig_clockmux};
  assign new_clockmux_o = {{(64-$bits(shire_esr_pkg::esr_clkmux_ctl_t)){1'b0}}, new_clockmux};
  assign orig_dll_dly_est_ctl_o = {{(64-$bits(esr_dll_dly_est_ctl_t)){1'b0}}, orig_dll_dly_est_ctl};
  assign new_dll_dly_est_ctl_o = {{(64-$bits(esr_pkg::esr_dll_dly_est_ctl_t)){1'b0}}, new_dll_dly_est_ctl};
  assign orig_ram_cfg_o = {{(64-$bits(ram_cfg_pkg::ram_cfg_t)){1'b0}}, orig_ram_cfg_native};
  assign new_ram_cfg_o = {{(64-$bits(ram_cfg_pkg::ram_cfg_t)){1'b0}}, new_ram_cfg};
  assign orig_barrier_en_o = orig_uc_enables.barriers;
  assign new_barrier_en_o = new_uc_enables.barriers;
  assign orig_fcc_en_o = orig_uc_enables.fcc;
  assign new_fcc_en_o = new_uc_enables.fcc;
  assign orig_prefetch_conf_o = {{(64-$bits(icache_prefetch_conf_t)){1'b0}}, orig_prefetch_conf};
  assign new_prefetch_conf_o = {{(64-$bits(icache_pkg::icache_prefetch_conf_t)){1'b0}}, new_prefetch_conf};
endmodule
