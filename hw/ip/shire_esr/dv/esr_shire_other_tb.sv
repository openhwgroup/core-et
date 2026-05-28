// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module esr_shire_other_tb (
  input  logic        clk_i,
  input  logic        rst_ni,
  input  logic        rst_c_ni,
  input  logic        rst_w_ni,
  input  logic        rst_d_ni,
  input  logic        dft_reset_byp_i,
  input  logic        dft_reset_i,
  input  logic [shire_esr_pkg::NumShireIdsBits-1:0] shire_id_reset_val_i,

  input  logic [15:0] apb_paddr_i,
  input  logic        apb_pwrite_i,
  input  logic        apb_psel_i,
  input  logic        apb_penable_i,
  input  logic [63:0] apb_pwdata_i,
  output logic        apb_pready_o,
  output logic [63:0] apb_prdata_o,
  output logic        apb_pslverr_o,

  input  logic [63:0] build_config_i,
  input  logic [63:0] revision_id_i,
  input  logic [10:0] and_or_tree_l1_i,
  input  logic [18:0] noc_status_i,
  input  logic [7:0]  barrier7_value_i,
  input  logic [31:0] neigh_nsleepout_i,
  input  logic [3:0]  glb_nsleepout_i,
  input  logic [3:0]  neigh_err_detected_i,
  input  logic [3:0]  neigh_err_logged_i,
  input  logic [3:0]  bank_err_detected_i,
  input  logic [3:0]  bank_err_logged_i,
  input  logic        pll_busy_i,
  input  logic [15:0] pll_rdata_i,
  input  logic        pll_rrdy_i,
  input  logic        pll_lock_i,
  input  logic        dll_busy_i,
  input  logic [15:0] dll_rdata_i,
  input  logic        dll_rrdy_i,
  input  logic        dll_lock_i,
  input  logic [3:0]  dll_sts_done_i,
  input  logic [31:0] dll_sts_errn_i,
  input  logic [3:0]  tbox_halt_ack_i,
  input  logic [3:0]  tbox_resume_ack_i,
  input  logic        rbox_halt_ack_i,
  input  logic        rbox_resume_ack_i,
  input  logic        rbox_reset_ack_i,
  input  logic [3:0]  prefetch_done_i,

  output logic [63:0] minion_feature_o,
  output logic [63:0] shire_config_o,
  output logic [31:0] thread0_enable_o,
  output logic [31:0] thread1_enable_o,
  output logic [63:0] ipi_trigger_o,
  output logic [63:0] ipi_redirect_trigger_o,
  output logic [31:0] mtime_target_o,
  output logic [3:0]  power_glb_nsleepin_o,
  output logic [3:0]  power_glb_iso_o,
  output logic [31:0] power_neigh_nsleepin_o,
  output logic [31:0] power_neigh_iso_o,
  output logic [63:0] pll_ctrl_o,
  output logic [63:0] pll_conf0_o,
  output logic [63:0] dll_ctrl_o,
  output logic [63:0] dll_conf0_o,
  output logic [63:0] clockmux_o,
  output logic [63:0] uc_config_o,
  output logic [63:0] clk_gate_ctrl_o,
  output logic        debug_clk_gate_ctrl_o,
  output logic [7:0]  shire_channel_eco_ctl_o,
  output logic [63:0] clk_dly_ctl_o,
  output logic [63:0] dll_dly_est_ctl_o,
  output logic [63:0] ram_cfg_o,
  output logic [31:0] barrier_en_o,
  output logic [3:0]  fcc_en_o,
  output logic [63:0] prefetch_conf_o,
  output logic [3:0]  prefetch_start_o,
  output logic [4:0]  debug_ss_o,
  output logic        shire_coop_mode_o
);
  dft_pkg::dft_t dft;
  minion_pkg::esr_minion_features_t minion_feature;
  shirecache_pkg::esr_shire_config_t shire_config;
  shire_esr_pkg::esr_and_or_tree_l1_t and_or_tree_l1;
  shire_esr_pkg::esr_uc_config_t uc_config;
  shire_esr_pkg::uc_esr_values_t uc_values;
  shire_esr_pkg::uc_esr_enable_sigs_t uc_enables;
  shire_esr_pkg::esr_pll_auto_config_t pll_ctrl;
  shire_esr_pkg::esr_dll_auto_config_t dll_ctrl;
  shire_esr_pkg::esr_clkmux_ctl_t clockmux;
  /* verilator lint_off UNUSEDSIGNAL */  // Unit-test wrapper exposes only config word 0 of the wide PLL/DLL buses.
  logic [shire_esr_pkg::ShirePllConfBits-1:0] pll_conf;
  logic [shire_esr_pkg::ShireDllConfBits-1:0] dll_conf;
  /* verilator lint_on UNUSEDSIGNAL */
  shire_esr_pkg::esr_clk_gate_ctrl_t clk_gate_ctrl;
  shire_esr_pkg::esr_clk_dly_ctl_t clk_dly_ctl;
  ram_cfg_pkg::ram_cfg_t ram_cfg;
  icache_pkg::icache_prefetch_conf_t prefetch_conf;
  esr_pkg::esr_dll_dly_est_ctl_t dll_dly_est_ctl;
  esr_pkg::esr_dll_dly_est_sts_t dll_dly_est_sts [shire_esr_pkg::NumNeigh-1:0];
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t tbox_ack [shire_esr_pkg::NumNeigh-1:0];
  shire_esr_pkg::bpam_rc_rbox_ack_t rbox_ack;

  assign dft.scanmode = dft_reset_byp_i;
  assign dft.scan_reset_n = ~dft_reset_i;
  assign dft.sram_clk_override = 1'b0;
  assign dft.ram_rei = 1'b0;
  assign dft.ram_wei = 1'b0;

  assign and_or_tree_l1 = and_or_tree_l1_i;

  always_comb begin
    uc_values = '0;
    uc_values.barriers[7] = barrier7_value_i;
    for (int unsigned i = 0; i < shire_esr_pkg::NumNeigh; i++) begin
      dll_dly_est_sts[i].sts_done = dll_sts_done_i[i];
      dll_dly_est_sts[i].sts_errn = dll_sts_errn_i[(8*i) +: 8];
      tbox_ack[i].halt_ack = tbox_halt_ack_i[i];
      tbox_ack[i].resume_ack = tbox_resume_ack_i[i];
    end
    rbox_ack.halt_ack = rbox_halt_ack_i;
    rbox_ack.resume_ack = rbox_resume_ack_i;
    rbox_ack.reset_ack = rbox_reset_ack_i;
  end

  esr_shire_other u_dut (
    .clk_i                              (clk_i),
    .rst_c_ni                           (rst_ni & rst_c_ni),
    .rst_w_ni                           (rst_ni & rst_w_ni),
    .rst_d_ni                           (rst_ni & rst_d_ni),
    .test_en_i                          (1'b0),
    .dft_i                              (dft),
    .shire_id_reset_val_i               (shire_id_reset_val_i),
    .esr_minion_feature_o               (minion_feature),
    .esr_shire_config_o                 (shire_config),
    .esr_shire_cache_build_config_i     (build_config_i),
    .esr_shire_cache_revision_id_i      (revision_id_i),
    .esr_ipi_trigger_o                  (ipi_trigger_o),
    .esr_and_or_tree_l1_i               (and_or_tree_l1),
    .esr_mtime_local_target_o           (mtime_target_o),
    .esr_power_ctrl_neigh_nsleepout_i   (neigh_nsleepout_i),
    .esr_shire_coop_mode_o              (shire_coop_mode_o),
    .esr_shire_noc_interrupt_status_i   (noc_status_i),
    .esr_uc_config_o                    (uc_config),
    .esr_clk_gate_ctrl_o                (clk_gate_ctrl),
    .esr_debug_clk_gate_ctrl_o          (debug_clk_gate_ctrl_o),
    .esr_shire_channel_eco_ctl_o        (shire_channel_eco_ctl_o),
    .esr_clk_dly_ctl_o                  (clk_dly_ctl),
    .esr_icache_prefetch_conf_o         (prefetch_conf),
    .esr_icache_prefetch_start_o        (prefetch_start_o),
    .esr_icache_prefetch_done_i         (prefetch_done_i),
    .esr_thread0_enable_o               (thread0_enable_o),
    .esr_thread1_enable_o               (thread1_enable_o),
    .neigh_sc_err_detected_i            (neigh_err_detected_i),
    .neigh_sc_err_logged_i              (neigh_err_logged_i),
    .sc_bank_err_detected_i             (bank_err_detected_i),
    .sc_bank_err_logged_i               (bank_err_logged_i),
    .esr_ipi_redirect_trigger_o         (ipi_redirect_trigger_o),
    .uc_esr_values_i                    (uc_values),
    .uc_esr_enables_o                   (uc_enables),
    .esr_power_ctrl_glb_nsleepin_o      (power_glb_nsleepin_o),
    .esr_power_ctrl_glb_iso_o           (power_glb_iso_o),
    .esr_power_ctrl_glb_nsleepout_i     (glb_nsleepout_i),
    .esr_power_ctrl_neigh_nsleepin_o    (power_neigh_nsleepin_o),
    .esr_power_ctrl_neigh_iso_o         (power_neigh_iso_o),
    .esr_pll_ctrl_o                     (pll_ctrl),
    .esr_pll_conf_o                     (pll_conf),
    .esr_pll_busy_i                     (pll_busy_i),
    .esr_pll_rdata_i                    (pll_rdata_i),
    .esr_pll_rrdy_i                     (pll_rrdy_i),
    .esr_pll_lock_i                     (pll_lock_i),
    .esr_dll_ctrl_o                     (dll_ctrl),
    .esr_dll_conf_o                     (dll_conf),
    .esr_dll_busy_i                     (dll_busy_i),
    .esr_dll_rdata_i                    (dll_rdata_i),
    .esr_dll_rrdy_i                     (dll_rrdy_i),
    .esr_dll_lock_i                     (dll_lock_i),
    .esr_shire_ctrl_clockmux_o          (clockmux),
    .esr_dll_dly_est_ctl_o              (dll_dly_est_ctl),
    .esr_dll_dly_est_sts_i              (dll_dly_est_sts),
    .ram_cfg_o                          (ram_cfg),
    .debug_rc_tbox_ack_i                (tbox_ack),
    .debug_rc_rbox_ack_i                (rbox_ack),
    .debug_rc_ss_tbox_rbox_o            (debug_ss_o),
    .apb_paddr_i                        (apb_paddr_i),
    .apb_pwrite_i                       (apb_pwrite_i),
    .apb_psel_i                         (apb_psel_i),
    .apb_penable_i                      (apb_penable_i),
    .apb_pwdata_i                       (apb_pwdata_i),
    .apb_pready_o                       (apb_pready_o),
    .apb_prdata_o                       (apb_prdata_o),
    .apb_pslverr_o                      (apb_pslverr_o)
  );

  assign minion_feature_o = {{(64-$bits(minion_pkg::esr_minion_features_t)){1'b0}}, minion_feature};
  assign shire_config_o = {{(64-$bits(shirecache_pkg::esr_shire_config_t)){1'b0}}, shire_config};
  assign pll_ctrl_o = {{(64-$bits(shire_esr_pkg::esr_pll_auto_config_t)){1'b0}}, pll_ctrl};
  assign pll_conf0_o = pll_conf[63:0];
  assign dll_ctrl_o = {{(64-$bits(shire_esr_pkg::esr_dll_auto_config_t)){1'b0}}, dll_ctrl};
  assign dll_conf0_o = dll_conf[63:0];
  assign clockmux_o = {{(64-$bits(shire_esr_pkg::esr_clkmux_ctl_t)){1'b0}}, clockmux};
  assign uc_config_o = {{(64-$bits(shire_esr_pkg::esr_uc_config_t)){1'b0}}, uc_config};
  assign clk_gate_ctrl_o = {{(64-$bits(shire_esr_pkg::esr_clk_gate_ctrl_t)){1'b0}}, clk_gate_ctrl};
  assign clk_dly_ctl_o = {{(64-$bits(shire_esr_pkg::esr_clk_dly_ctl_t)){1'b0}}, clk_dly_ctl};
  assign dll_dly_est_ctl_o = {{(64-$bits(esr_pkg::esr_dll_dly_est_ctl_t)){1'b0}}, dll_dly_est_ctl};
  assign ram_cfg_o = {{(64-$bits(ram_cfg_pkg::ram_cfg_t)){1'b0}}, ram_cfg};
  assign barrier_en_o = uc_enables.barriers;
  assign fcc_en_o = uc_enables.fcc;
  assign prefetch_conf_o = {{(64-$bits(icache_pkg::icache_prefetch_conf_t)){1'b0}}, prefetch_conf};
endmodule
