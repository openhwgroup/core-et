// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module esr_shire_other #(
  parameter int unsigned NumBanks = shirecache_pkg::Banks,
  parameter int unsigned NumNeigh = shire_esr_pkg::NumNeigh,
  parameter int unsigned AdWidth = shire_esr_pkg::EsrShireOtherApbPpAddrWidth
) (
  input  logic                                          clk_i,
  input  logic                                          rst_c_ni,
  input  logic                                          rst_w_ni,
  input  logic                                          rst_d_ni,
  /* verilator lint_off UNUSEDSIGNAL */  // Preserved original test-enable port; the named write-preview primitive has no test-enable input.
  input  logic                                          test_en_i,
  /* verilator lint_on UNUSEDSIGNAL */
  input  dft_pkg::dft_t                                 dft_i,

  input  logic [shire_esr_pkg::NumShireIdsBits-1:0]     shire_id_reset_val_i,

  output minion_pkg::esr_minion_features_t              esr_minion_feature_o,
  output shirecache_pkg::esr_shire_config_t             esr_shire_config_o,
  input  logic [63:0]                                   esr_shire_cache_build_config_i,
  input  logic [63:0]                                   esr_shire_cache_revision_id_i,
  output logic [63:0]                                   esr_ipi_trigger_o,
  input  shire_esr_pkg::esr_and_or_tree_l1_t            esr_and_or_tree_l1_i,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0]     esr_mtime_local_target_o,
  input  logic [NumNeigh*shire_esr_pkg::MinPerNeigh-1:0] esr_power_ctrl_neigh_nsleepout_i,
  output logic                                          esr_shire_coop_mode_o,
  input  logic [shire_esr_pkg::NocIntNum-1:0]           esr_shire_noc_interrupt_status_i,
  output shire_esr_pkg::esr_uc_config_t                 esr_uc_config_o,
  output shire_esr_pkg::esr_clk_gate_ctrl_t             esr_clk_gate_ctrl_o,
  output logic                                          esr_debug_clk_gate_ctrl_o,
  output logic [7:0]                                    esr_shire_channel_eco_ctl_o,
  output shire_esr_pkg::esr_clk_dly_ctl_t               esr_clk_dly_ctl_o,

  output icache_pkg::icache_prefetch_conf_t             esr_icache_prefetch_conf_o,
  output logic [shire_esr_pkg::IcachePerShire-1:0]      esr_icache_prefetch_start_o,
  input  logic [shire_esr_pkg::IcachePerShire-1:0]      esr_icache_prefetch_done_i,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0]     esr_thread0_enable_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0]     esr_thread1_enable_o,
  input  logic [NumNeigh-1:0]                           neigh_sc_err_detected_i,
  input  logic [NumNeigh-1:0]                           neigh_sc_err_logged_i,
  input  logic [NumBanks-1:0]                           sc_bank_err_detected_i,
  input  logic [NumBanks-1:0]                           sc_bank_err_logged_i,

  output logic [63:0]                                   esr_ipi_redirect_trigger_o,

  input  shire_esr_pkg::uc_esr_values_t                 uc_esr_values_i,
  output shire_esr_pkg::uc_esr_enable_sigs_t            uc_esr_enables_o,

  output logic [NumNeigh-1:0]                           esr_power_ctrl_glb_nsleepin_o,
  output logic [NumNeigh-1:0]                           esr_power_ctrl_glb_iso_o,
  input  logic [NumNeigh-1:0]                           esr_power_ctrl_glb_nsleepout_i,
  output logic [NumNeigh*shire_esr_pkg::MinPerNeigh-1:0] esr_power_ctrl_neigh_nsleepin_o,
  output logic [NumNeigh*shire_esr_pkg::MinPerNeigh-1:0] esr_power_ctrl_neigh_iso_o,

  output shire_esr_pkg::esr_pll_auto_config_t           esr_pll_ctrl_o,
  output logic [shire_esr_pkg::ShirePllConfBits-1:0]    esr_pll_conf_o,
  input  logic                                          esr_pll_busy_i,
  input  logic [shire_esr_pkg::ShirePllDataBits-1:0]    esr_pll_rdata_i,
  input  logic                                          esr_pll_rrdy_i,
  input  logic                                          esr_pll_lock_i,

  output shire_esr_pkg::esr_dll_auto_config_t           esr_dll_ctrl_o,
  output logic [shire_esr_pkg::ShireDllConfBits-1:0]    esr_dll_conf_o,
  input  logic                                          esr_dll_busy_i,
  input  logic [shire_esr_pkg::ShireDllDataBits-1:0]    esr_dll_rdata_i,
  input  logic                                          esr_dll_rrdy_i,
  input  logic                                          esr_dll_lock_i,

  output shire_esr_pkg::esr_clkmux_ctl_t                esr_shire_ctrl_clockmux_o,

  output esr_pkg::esr_dll_dly_est_ctl_t                 esr_dll_dly_est_ctl_o,
  input  esr_pkg::esr_dll_dly_est_sts_t                 esr_dll_dly_est_sts_i [NumNeigh-1:0],

  output ram_cfg_pkg::ram_cfg_t                         ram_cfg_o,

  input  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t         debug_rc_tbox_ack_i [NumNeigh-1:0],
  input  shire_esr_pkg::bpam_rc_rbox_ack_t              debug_rc_rbox_ack_i,
  output logic [shire_esr_pkg::TboxPerShire:0]          debug_rc_ss_tbox_rbox_o,

  input  logic [AdWidth-1:0]                            apb_paddr_i,
  input  logic                                          apb_pwrite_i,
  input  logic                                          apb_psel_i,
  input  logic                                          apb_penable_i,
  input  logic [shire_esr_pkg::EsrApbDataWidth-1:0]     apb_pwdata_i,
  output logic                                          apb_pready_o,
  output logic [shire_esr_pkg::EsrApbDataWidth-1:0]     apb_prdata_o,
  output logic                                          apb_pslverr_o
);
  import shire_esr_pkg::*;

  localparam int unsigned MinionFeatureWidth = $bits(minion_pkg::esr_minion_features_t);
  localparam int unsigned ShireConfigWidth = $bits(shirecache_pkg::esr_shire_config_t);
  localparam int unsigned IcachePrefetchWidth = $bits(esr_icache_prefetch_t);
  localparam int unsigned MinionFeatureDisableMultithreadingBit = MinionFeatureWidth - 2;
  localparam logic [MinionFeatureWidth-1:0] MinionFeatureResetVal =
      {{(MinionFeatureWidth-1){1'b0}}, 1'b1};
  localparam logic [63:0] IpiRedirectFilterResetVal = 64'hffff_ffff_ffff_ffff;
  localparam logic [3*NumNeigh-1:0] ShirePowerCtrlResetVal = {{(3*NumNeigh-4){1'b0}}, 4'hf};

  localparam logic [AdWidth-1:0] AddrMinionFeature = EsrShireOtherAddrMinionFeature[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShireConfig = EsrShireOtherAddrShireConfig[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrThread1Disable = EsrShireOtherAddrThread1Disable[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShireCacheBuildConfig = EsrShireOtherAddrShireCacheBuildConfig[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShireCacheRevisionId = EsrShireOtherAddrShireCacheRevisionId[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrIpiRedirectMask = EsrShireOtherAddrIpiRedirectMask[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrIpiRedirectFilter = EsrShireOtherAddrIpiRedirectFilter[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrIpiTrigger = EsrShireOtherAddrIpiTrigger[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrIpiTriggerClear = EsrShireOtherAddrIpiTriggerClear[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrFccCredinc0 = EsrShireOtherAddrFccCredinc0[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrFccCredinc1 = EsrShireOtherAddrFccCredinc1[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrFccCredinc2 = EsrShireOtherAddrFccCredinc2[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrFccCredinc3 = EsrShireOtherAddrFccCredinc3[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrFastLocalBarrierBase = EsrShireOtherAddrFastLocalBarrierBase[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrAndOrTreeL1 = EsrShireOtherAddrAndOrTreeL1[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrMtimeLocalTarget = EsrShireOtherAddrMtimeLocalTarget[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShirePowerCtrl = EsrShireOtherAddrShirePowerCtrl[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrPowerCtrlNeighNsleepin = EsrShireOtherAddrPowerCtrlNeighNsleepin[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrPowerCtrlNeighIsolation = EsrShireOtherAddrPowerCtrlNeighIsolation[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrPowerCtrlNeighNsleepout = EsrShireOtherAddrPowerCtrlNeighNsleepout[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrThread0Disable = EsrShireOtherAddrThread0Disable[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShireErrorLog = EsrShireOtherAddrShireErrorLog[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShirePllAutoConfig = EsrShireOtherAddrShirePllAutoConfig[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShirePllConfigData0 = EsrShireOtherAddrShirePllConfigData0[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShirePllConfigData1 = EsrShireOtherAddrShirePllConfigData1[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShirePllConfigData2 = EsrShireOtherAddrShirePllConfigData2[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShirePllConfigData3 = EsrShireOtherAddrShirePllConfigData3[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShirePllReadData = EsrShireOtherAddrShirePllReadData[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShireCoopMode = EsrShireOtherAddrShireCoopMode[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShireCtrlClockmux = EsrShireOtherAddrShireCtrlClockmux[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShireCacheRamCfg1 = EsrShireOtherAddrShireCacheRamCfg1[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShireCacheRamCfg2 = EsrShireOtherAddrShireCacheRamCfg2[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShireCacheRamCfg3 = EsrShireOtherAddrShireCacheRamCfg3[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShireCacheRamCfg4 = EsrShireOtherAddrShireCacheRamCfg4[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShireNocInterruptStatus = EsrShireOtherAddrShireNocInterruptStatus[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShireDllAutoConfig = EsrShireOtherAddrShireDllAutoConfig[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShireDllConfigData0 = EsrShireOtherAddrShireDllConfigData0[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShireDllConfigData1 = EsrShireOtherAddrShireDllConfigData1[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShireDllReadData = EsrShireOtherAddrShireDllReadData[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrTboxRboxDbgRc = EsrShireOtherAddrTboxRboxDbgRc[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrUcConfig = EsrShireOtherAddrUcConfig[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrIcacheUPrefetch = EsrShireOtherAddrIcacheUPrefetch[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrIcacheSPrefetch = EsrShireOtherAddrIcacheSPrefetch[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrIcacheMPrefetch = EsrShireOtherAddrIcacheMPrefetch[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrClkGateCtrl = EsrShireOtherAddrClkGateCtrl[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrDebugClkGateCtrl = EsrShireOtherAddrDebugClkGateCtrl[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrShireChannelEcoCtl = EsrShireOtherAddrShireChannelEcoCtl[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrClkDlyCtl = EsrShireOtherAddrClkDlyCtl[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrDllDlyEstCtl = EsrShireOtherAddrDllDlyEstCtl[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrDllDlyEstSts = EsrShireOtherAddrDllDlyEstSts[AdWidth-1:0];

  logic s0_en;
  logic s0_addr_valid;
  logic [EsrApbDataWidth-1:0] s0_rdata;
  logic s1_en;
  logic s1_addr_valid_q;
  logic [EsrApbDataWidth-1:0] s1_rdata_q;
  logic s0_wen_p2;
  logic [EsrApbDataWidth-1:0] apb_pwdata_1p;

  logic reset_c_reg;
  logic reset_w_reg;
  logic rst_c_sync_n;
  logic rst_w_sync_n;

  logic [MinionFeatureWidth-1:0] reg_minion_feature_q;
  logic [MinionFeatureWidth-1:0] reg_minion_feature_d;
  logic [ThreadsPerShire-1:0] reg_thread1_disable_q;
  logic [ThreadsPerShire-1:0] reg_thread1_disable_d;
  logic [63:0] reg_ipi_redirect_filter_q;
  logic [63:0] reg_ipi_redirect_filter_d;
  logic [ThreadsPerShire-1:0] reg_thread0_disable_q;
  logic [ThreadsPerShire-1:0] reg_thread0_disable_d;
  logic [IcachePrefetchWidth-1:0] reg_icache_uprefetch_q, reg_icache_uprefetch_d;
  logic [IcachePrefetchWidth-1:0] reg_icache_sprefetch_q, reg_icache_sprefetch_d;
  logic [IcachePrefetchWidth-1:0] reg_icache_mprefetch_q, reg_icache_mprefetch_d;

  shirecache_pkg::esr_shire_config_t reg_shire_config_q, reg_shire_config_d;
  logic [63:0] reg_ipi_trigger_q, reg_ipi_trigger_d;
  logic [ThreadsPerShire-1:0] reg_mtime_local_target_q, reg_mtime_local_target_d;
  logic [3*NumNeigh-1:0] reg_shire_power_ctrl_q, reg_shire_power_ctrl_d;
  logic [NumNeigh*MinPerNeigh-1:0] reg_power_ctrl_neigh_nsleepin_q, reg_power_ctrl_neigh_nsleepin_d;
  logic [NumNeigh*MinPerNeigh-1:0] reg_power_ctrl_neigh_isolation_q, reg_power_ctrl_neigh_isolation_d;
  esr_pll_auto_config_t reg_shire_pll_auto_config_q, reg_shire_pll_auto_config_d;
  logic [63:0] reg_shire_pll_config_data_0_q, reg_shire_pll_config_data_0_d;
  logic [63:0] reg_shire_pll_config_data_1_q, reg_shire_pll_config_data_1_d;
  logic [63:0] reg_shire_pll_config_data_2_q, reg_shire_pll_config_data_2_d;
  logic [63:0] reg_shire_pll_config_data_3_q, reg_shire_pll_config_data_3_d;
  logic reg_shire_coop_mode_q, reg_shire_coop_mode_d;
  esr_clkmux_ctl_t reg_shire_ctrl_clockmux_q, reg_shire_ctrl_clockmux_d;
  esr_shire_cache_ram_cfg1_t reg_shire_cache_ram_cfg1_q, reg_shire_cache_ram_cfg1_d;
  esr_shire_cache_ram_cfg2_t reg_shire_cache_ram_cfg2_q, reg_shire_cache_ram_cfg2_d;
  esr_shire_cache_ram_cfg3_t reg_shire_cache_ram_cfg3_q, reg_shire_cache_ram_cfg3_d;
  esr_shire_cache_ram_cfg4_t reg_shire_cache_ram_cfg4_q, reg_shire_cache_ram_cfg4_d;
  esr_dll_auto_config_t reg_shire_dll_auto_config_q, reg_shire_dll_auto_config_d;
  logic [63:0] reg_shire_dll_config_data_0_q, reg_shire_dll_config_data_0_d;
  logic [63:0] reg_shire_dll_config_data_1_q, reg_shire_dll_config_data_1_d;
  esr_debug_rbox_tbox_rc_t reg_tbox_rbox_dbg_rc_q, reg_tbox_rbox_dbg_rc_d;
  esr_uc_config_t reg_uc_config_q, reg_uc_config_d;
  esr_clk_gate_ctrl_t reg_clk_gate_ctrl_q, reg_clk_gate_ctrl_d;
  logic reg_debug_clk_gate_ctrl_q, reg_debug_clk_gate_ctrl_d;
  logic [7:0] reg_shire_channel_eco_ctl_q, reg_shire_channel_eco_ctl_d;
  esr_clk_dly_ctl_t reg_clk_dly_ctl_q, reg_clk_dly_ctl_d;
  logic [10:0] dll_dly_est_ctl_q, dll_dly_est_ctl_d;
  logic [ShirePllReadBits-1:0] reg_shire_pll_read_data_q, reg_shire_pll_read_data_d;
  logic [ShireDllReadBits-1:0] reg_shire_dll_read_data_q, reg_shire_dll_read_data_d;
  logic [8*NumNeigh+4-1:0] dll_dly_est_sts_q, dll_dly_est_sts_d;

  logic features_written_q;

  logic reg_minion_feature_en_q, reg_minion_feature_en_d;
  logic reg_shire_config_en_q, reg_shire_config_en_d;
  logic reg_thread1_disable_en_q, reg_thread1_disable_en_d;
  logic reg_ipi_redirect_mask_en_q, reg_ipi_redirect_mask_en_d;
  logic reg_ipi_redirect_filter_en_q, reg_ipi_redirect_filter_en_d;
  logic reg_ipi_trigger_en_q, reg_ipi_trigger_en_d;
  logic reg_ipi_trigger_clear_en_q, reg_ipi_trigger_clear_en_d;
  logic [UcFccRegisters-1:0] reg_fcc_credinc_en_q, reg_fcc_credinc_en_d;
  logic [BarrierRegisters-1:0] reg_fast_local_barrier_en_q, reg_fast_local_barrier_en_d;
  logic reg_mtime_local_target_en_q, reg_mtime_local_target_en_d;
  logic reg_shire_power_ctrl_en_q, reg_shire_power_ctrl_en_d;
  logic reg_power_ctrl_neigh_nsleepin_en_q, reg_power_ctrl_neigh_nsleepin_en_d;
  logic reg_power_ctrl_neigh_isolation_en_q, reg_power_ctrl_neigh_isolation_en_d;
  logic reg_thread0_disable_en_q, reg_thread0_disable_en_d;
  logic reg_shire_pll_auto_config_en_q, reg_shire_pll_auto_config_en_d;
  logic reg_shire_pll_config_data_0_en_q, reg_shire_pll_config_data_0_en_d;
  logic reg_shire_pll_config_data_1_en_q, reg_shire_pll_config_data_1_en_d;
  logic reg_shire_pll_config_data_2_en_q, reg_shire_pll_config_data_2_en_d;
  logic reg_shire_pll_config_data_3_en_q, reg_shire_pll_config_data_3_en_d;
  logic reg_shire_coop_mode_en_q, reg_shire_coop_mode_en_d;
  logic reg_shire_ctrl_clockmux_en_q, reg_shire_ctrl_clockmux_en_d;
  logic reg_shire_cache_ram_cfg1_en_q, reg_shire_cache_ram_cfg1_en_d;
  logic reg_shire_cache_ram_cfg2_en_q, reg_shire_cache_ram_cfg2_en_d;
  logic reg_shire_cache_ram_cfg3_en_q, reg_shire_cache_ram_cfg3_en_d;
  logic reg_shire_cache_ram_cfg4_en_q, reg_shire_cache_ram_cfg4_en_d;
  logic reg_shire_dll_auto_config_en_q, reg_shire_dll_auto_config_en_d;
  logic reg_shire_dll_config_data_0_en_q, reg_shire_dll_config_data_0_en_d;
  logic reg_shire_dll_config_data_1_en_q, reg_shire_dll_config_data_1_en_d;
  logic reg_tbox_rbox_dbg_rc_en_q, reg_tbox_rbox_dbg_rc_en_d;
  logic reg_uc_config_en_q, reg_uc_config_en_d;
  logic reg_icache_uprefetch_en_q, reg_icache_uprefetch_en_d;
  logic reg_icache_sprefetch_en_q, reg_icache_sprefetch_en_d;
  logic reg_icache_mprefetch_en_q, reg_icache_mprefetch_en_d;
  logic reg_clk_gate_ctrl_en_q, reg_clk_gate_ctrl_en_d;
  logic reg_debug_clk_gate_ctrl_en_q, reg_debug_clk_gate_ctrl_en_d;
  logic reg_shire_channel_eco_ctl_en_q, reg_shire_channel_eco_ctl_en_d;
  logic reg_clk_dly_ctl_en_q, reg_clk_dly_ctl_en_d;
  logic reg_dll_dly_est_ctl_en_q, reg_dll_dly_est_ctl_en_d;

  logic [CsrPrvLevels-1:0] icache_prv_prefetch_pending_q, icache_prv_prefetch_pending_d;
  logic [CsrPrvLevels-1:0] icache_prv_prefetch_serve;
  icache_pkg::icache_prefetch_conf_t esr_icache_prefetch_conf_q, esr_icache_prefetch_conf_d;
  logic [IcachePerShire-1:0] esr_icache_prefetch_start_q, esr_icache_prefetch_start_d;
  logic [IcachePerShire-1:0] esr_icache_prefetch_done_q;
  logic [IcachePerShire-1:0] icache_prefetch_busy_q, icache_prefetch_busy_d;
  logic [IcachePerShire-1:0] icache_prefetch_idle_q, icache_prefetch_idle_d;
  logic icache_prefetch_all_idle;

  esr_icache_prefetch_t reg_icache_uprefetch;
  esr_icache_prefetch_t reg_icache_sprefetch;
  esr_icache_prefetch_t reg_icache_mprefetch;

  assign reg_icache_uprefetch = reg_icache_uprefetch_q;
  assign reg_icache_sprefetch = reg_icache_sprefetch_q;
  assign reg_icache_mprefetch = reg_icache_mprefetch_q;

  function automatic shirecache_pkg::esr_shire_config_t shire_config_from_bits(
    input logic [ShireConfigWidth-1:0] bits
  );
    shirecache_pkg::esr_shire_config_t ret;
    begin
      ret.tbox3_id = bits[25:24];
      ret.tbox2_id = bits[23:22];
      ret.tbox1_id = bits[21:20];
      ret.tbox0_id = bits[19:18];
      ret.rbox_en = bits[17];
      ret.tbox_en = bits[16:13];
      ret.neigh_en = bits[12:9];
      ret.cache_en = bits[8];
      ret.shire_id = bits[7:0];
      return ret;
    end
  endfunction

  function automatic shirecache_pkg::esr_shire_config_t shire_config_reset(
    input logic [NumShireIdsBits-1:0] shire_id
  );
    logic [ShireConfigWidth-1:0] raw;
    begin
      raw = 26'h392a000 | {{(ShireConfigWidth-NumShireIdsBits){1'b0}}, shire_id};
      return shire_config_from_bits(raw);
    end
  endfunction

  function automatic esr_pll_auto_config_t pll_auto_from_bits(input logic [$bits(esr_pll_auto_config_t)-1:0] bits);
    esr_pll_auto_config_t ret;
    begin
      ret = bits;
      return ret;
    end
  endfunction

  function automatic esr_dll_auto_config_t dll_auto_from_bits(input logic [$bits(esr_dll_auto_config_t)-1:0] bits);
    esr_dll_auto_config_t ret;
    begin
      ret = bits;
      return ret;
    end
  endfunction

  function automatic esr_shire_cache_ram_cfg1_t ram_cfg1_reset();
    esr_shire_cache_ram_cfg1_t ret;
    begin
      ret = '0;
      ret.sc_mbt_wa = 3'b111;
      ret.sc_mbt_ra = 2'b01;
      ret.sc_mbs_wa = 3'b110;
      ret.sc_mbs_ra = 2'b10;
      return ret;
    end
  endfunction

  function automatic esr_shire_cache_ram_cfg2_t ram_cfg2_reset();
    esr_shire_cache_ram_cfg2_t ret;
    begin
      ret = '0;
      ret.sc_mbd_wa = 3'b111;
      ret.sc_mbd_ra = 2'b01;
      return ret;
    end
  endfunction

  function automatic esr_shire_cache_ram_cfg3_t ram_cfg3_reset();
    esr_shire_cache_ram_cfg3_t ret;
    begin
      ret = '0;
      ret.sc_mbb_wa = 3'b110;
      ret.sc_mbb_ra = 2'b01;
      ret.sc_mbb_rm = 4'b0001;
      ret.sc_mbb_rme = 1'b1;
      ret.sc_mbq_wa = 3'b110;
      ret.sc_mbq_ra = 2'b01;
      ret.sc_mbq_rm = 4'b0001;
      ret.sc_mbq_rme = 1'b1;
      return ret;
    end
  endfunction

  function automatic esr_shire_cache_ram_cfg4_t ram_cfg4_reset();
    esr_shire_cache_ram_cfg4_t ret;
    begin
      ret = '0;
      ret.sc_mbr_1p_wa = 3'b110;
      ret.sc_mbr_1p_ra = 2'b10;
      ret.sc_mbr_2p_wa = 3'b110;
      ret.sc_mbr_2p_ra = 2'b01;
      ret.sc_mbr_2p_rm = 4'b0001;
      ret.sc_mbr_2p_rme = 1'b1;
      ret.sc_mbi_wa = 3'b111;
      ret.sc_mbi_ra = 2'b01;
      return ret;
    end
  endfunction

  prim_rst_sync u_rst_c_sync (
    .clk_i  (clk_i),
    .rst_ni (rst_c_ni),
    .dft_i  (dft_i),
    .rst_no (rst_c_sync_n)
  );

  prim_rst_sync u_rst_w_sync (
    .clk_i  (clk_i),
    .rst_ni (rst_w_ni),
    .dft_i  (dft_i),
    .rst_no (rst_w_sync_n)
  );

  assign reset_c_reg = !rst_c_sync_n;
  assign reset_w_reg = !rst_w_sync_n;

  prim_write_preview_en #(
    .Width (1)
  ) u_s0_wen_p2 (
    .clk_i (clk_i),
    .en_i  (1'b1),
    .d_i   (s0_en && apb_pwrite_i),
    .q_o   (s0_wen_p2)
  );

  prim_write_preview_en #(
    .Width (EsrApbDataWidth)
  ) u_apb_pwdata_1p (
    .clk_i (clk_i),
    .en_i  (s0_wen_p2),
    .d_i   (apb_pwdata_i),
    .q_o   (apb_pwdata_1p)
  );

  assign reg_minion_feature_d = apb_pwdata_1p[MinionFeatureWidth-1:0];
  assign reg_thread1_disable_d = apb_pwdata_1p[ThreadsPerShire-1:0];
  assign reg_ipi_redirect_filter_d = apb_pwdata_1p;
  assign reg_thread0_disable_d = apb_pwdata_1p[ThreadsPerShire-1:0];
  assign reg_icache_uprefetch_d = apb_pwdata_1p[IcachePrefetchWidth-1:0];
  assign reg_icache_sprefetch_d = apb_pwdata_1p[IcachePrefetchWidth-1:0];
  assign reg_icache_mprefetch_d = apb_pwdata_1p[IcachePrefetchWidth-1:0];

  prim_write_commit_rst_en #(
    .Width      (MinionFeatureWidth),
    .ResetValue (MinionFeatureResetVal)
  ) u_reg_minion_feature (
    .clk_i (clk_i),
    .rst_i (reset_c_reg),
    .en_i  (reg_minion_feature_en_q),
    .d_i   (reg_minion_feature_d),
    .q_o   (reg_minion_feature_q)
  );

  prim_write_commit_rst_en #(
    .Width      (ThreadsPerShire),
    .ResetValue ({ThreadsPerShire{1'b1}})
  ) u_reg_thread1_disable (
    .clk_i (clk_i),
    .rst_i (reset_c_reg),
    .en_i  (reg_thread1_disable_en_q),
    .d_i   (reg_thread1_disable_d),
    .q_o   (reg_thread1_disable_q)
  );

  prim_write_commit_rst_en #(
    .Width      (64),
    .ResetValue (IpiRedirectFilterResetVal)
  ) u_reg_ipi_redirect_filter (
    .clk_i (clk_i),
    .rst_i (reset_w_reg),
    .en_i  (reg_ipi_redirect_filter_en_q),
    .d_i   (reg_ipi_redirect_filter_d),
    .q_o   (reg_ipi_redirect_filter_q)
  );

  prim_write_commit_rst_en #(
    .Width      (ThreadsPerShire),
    .ResetValue ({ThreadsPerShire{1'b1}})
  ) u_reg_thread0_disable (
    .clk_i (clk_i),
    .rst_i (reset_c_reg),
    .en_i  (reg_thread0_disable_en_q),
    .d_i   (reg_thread0_disable_d),
    .q_o   (reg_thread0_disable_q)
  );

  prim_write_commit_rst_en #(
    .Width      (IcachePrefetchWidth),
    .ResetValue ({IcachePrefetchWidth{1'b0}})
  ) u_reg_icache_uprefetch (
    .clk_i (clk_i),
    .rst_i (reset_w_reg),
    .en_i  (reg_icache_uprefetch_en_q),
    .d_i   (reg_icache_uprefetch_d),
    .q_o   (reg_icache_uprefetch_q)
  );

  prim_write_commit_rst_en #(
    .Width      (IcachePrefetchWidth),
    .ResetValue ({IcachePrefetchWidth{1'b0}})
  ) u_reg_icache_sprefetch (
    .clk_i (clk_i),
    .rst_i (reset_w_reg),
    .en_i  (reg_icache_sprefetch_en_q),
    .d_i   (reg_icache_sprefetch_d),
    .q_o   (reg_icache_sprefetch_q)
  );

  prim_write_commit_rst_en #(
    .Width      (IcachePrefetchWidth),
    .ResetValue ({IcachePrefetchWidth{1'b0}})
  ) u_reg_icache_mprefetch (
    .clk_i (clk_i),
    .rst_i (reset_w_reg),
    .en_i  (reg_icache_mprefetch_en_q),
    .d_i   (reg_icache_mprefetch_d),
    .q_o   (reg_icache_mprefetch_q)
  );

  always_comb begin
    s0_en = apb_psel_i && !apb_penable_i;
    s0_addr_valid = 1'b1;
    s0_rdata = '0;

    reg_minion_feature_en_d = 1'b0;
    reg_shire_config_en_d = 1'b0;
    reg_thread1_disable_en_d = 1'b0;
    reg_ipi_redirect_mask_en_d = 1'b0;
    reg_ipi_redirect_filter_en_d = 1'b0;
    reg_ipi_trigger_en_d = 1'b0;
    reg_ipi_trigger_clear_en_d = 1'b0;
    reg_fcc_credinc_en_d = '0;
    reg_fast_local_barrier_en_d = '0;
    reg_mtime_local_target_en_d = 1'b0;
    reg_shire_power_ctrl_en_d = 1'b0;
    reg_power_ctrl_neigh_nsleepin_en_d = 1'b0;
    reg_power_ctrl_neigh_isolation_en_d = 1'b0;
    reg_thread0_disable_en_d = 1'b0;
    reg_shire_pll_auto_config_en_d = 1'b0;
    reg_shire_pll_config_data_0_en_d = 1'b0;
    reg_shire_pll_config_data_1_en_d = 1'b0;
    reg_shire_pll_config_data_2_en_d = 1'b0;
    reg_shire_pll_config_data_3_en_d = 1'b0;
    reg_shire_coop_mode_en_d = 1'b0;
    reg_shire_ctrl_clockmux_en_d = 1'b0;
    reg_shire_cache_ram_cfg1_en_d = 1'b0;
    reg_shire_cache_ram_cfg2_en_d = 1'b0;
    reg_shire_cache_ram_cfg3_en_d = 1'b0;
    reg_shire_cache_ram_cfg4_en_d = 1'b0;
    reg_shire_dll_auto_config_en_d = 1'b0;
    reg_shire_dll_config_data_0_en_d = 1'b0;
    reg_shire_dll_config_data_1_en_d = 1'b0;
    reg_tbox_rbox_dbg_rc_en_d = 1'b0;
    reg_uc_config_en_d = 1'b0;
    reg_icache_uprefetch_en_d = 1'b0;
    reg_icache_sprefetch_en_d = 1'b0;
    reg_icache_mprefetch_en_d = 1'b0;
    reg_clk_gate_ctrl_en_d = 1'b0;
    reg_debug_clk_gate_ctrl_en_d = 1'b0;
    reg_shire_channel_eco_ctl_en_d = 1'b0;
    reg_clk_dly_ctl_en_d = 1'b0;
    reg_dll_dly_est_ctl_en_d = 1'b0;

    if (s0_en) begin
      unique case (apb_paddr_i)
        AddrMinionFeature: begin
          reg_minion_feature_en_d = apb_pwrite_i && !features_written_q;
          s0_rdata[MinionFeatureWidth-1:0] = reg_minion_feature_q;
        end
        AddrShireConfig: begin
          reg_shire_config_en_d = apb_pwrite_i;
          s0_rdata[ShireConfigWidth-1:0] = reg_shire_config_q;
        end
        AddrThread1Disable: begin
          reg_thread1_disable_en_d = apb_pwrite_i;
          s0_rdata[ThreadsPerShire-1:0] = reg_thread1_disable_q;
        end
        AddrShireCacheBuildConfig: s0_rdata = esr_shire_cache_build_config_i;
        AddrShireCacheRevisionId: s0_rdata = esr_shire_cache_revision_id_i;
        AddrIpiRedirectMask: begin
          reg_ipi_redirect_mask_en_d = apb_pwrite_i;
          s0_rdata = '0;
        end
        AddrIpiRedirectFilter: begin
          reg_ipi_redirect_filter_en_d = apb_pwrite_i;
          s0_rdata = reg_ipi_redirect_filter_q;
        end
        AddrIpiTrigger: begin
          reg_ipi_trigger_en_d = apb_pwrite_i;
          s0_rdata = reg_ipi_trigger_q;
        end
        AddrIpiTriggerClear: begin
          reg_ipi_trigger_clear_en_d = apb_pwrite_i;
          s0_rdata = reg_ipi_trigger_q;
        end
        AddrFccCredinc0: begin
          reg_fcc_credinc_en_d[0] = apb_pwrite_i;
          s0_rdata = '0;
        end
        AddrFccCredinc1: begin
          reg_fcc_credinc_en_d[1] = apb_pwrite_i;
          s0_rdata = '0;
        end
        AddrFccCredinc2: begin
          reg_fcc_credinc_en_d[2] = apb_pwrite_i;
          s0_rdata = '0;
        end
        AddrFccCredinc3: begin
          reg_fcc_credinc_en_d[3] = apb_pwrite_i;
          s0_rdata = '0;
        end
        AddrAndOrTreeL1: s0_rdata[$bits(esr_and_or_tree_l1_t)-1:0] = esr_and_or_tree_l1_i;
        AddrMtimeLocalTarget: begin
          reg_mtime_local_target_en_d = apb_pwrite_i;
          s0_rdata[ThreadsPerShire-1:0] = reg_mtime_local_target_q;
        end
        AddrShirePowerCtrl: begin
          reg_shire_power_ctrl_en_d = apb_pwrite_i;
          s0_rdata[3*NumNeigh-1:0] = reg_shire_power_ctrl_q;
        end
        AddrPowerCtrlNeighNsleepin: begin
          reg_power_ctrl_neigh_nsleepin_en_d = apb_pwrite_i;
          s0_rdata[NumNeigh*MinPerNeigh-1:0] = reg_power_ctrl_neigh_nsleepin_q;
        end
        AddrPowerCtrlNeighIsolation: begin
          reg_power_ctrl_neigh_isolation_en_d = apb_pwrite_i;
          s0_rdata[NumNeigh*MinPerNeigh-1:0] = reg_power_ctrl_neigh_isolation_q;
        end
        AddrPowerCtrlNeighNsleepout: begin
          s0_rdata[NumNeigh*MinPerNeigh-1:0] = esr_power_ctrl_neigh_nsleepout_i;
        end
        AddrThread0Disable: begin
          reg_thread0_disable_en_d = apb_pwrite_i;
          s0_rdata[ThreadsPerShire-1:0] = reg_thread0_disable_q;
        end
        AddrShireErrorLog: begin
          s0_rdata[2*NumNeigh+2*NumBanks-1:0] = {
              neigh_sc_err_logged_i, sc_bank_err_logged_i,
              neigh_sc_err_detected_i, sc_bank_err_detected_i};
        end
        AddrShirePllAutoConfig: begin
          reg_shire_pll_auto_config_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_pll_auto_config_t)-1:0] = reg_shire_pll_auto_config_q;
        end
        AddrShirePllConfigData0: begin
          reg_shire_pll_config_data_0_en_d = apb_pwrite_i;
          s0_rdata = reg_shire_pll_config_data_0_q;
        end
        AddrShirePllConfigData1: begin
          reg_shire_pll_config_data_1_en_d = apb_pwrite_i;
          s0_rdata = reg_shire_pll_config_data_1_q;
        end
        AddrShirePllConfigData2: begin
          reg_shire_pll_config_data_2_en_d = apb_pwrite_i;
          s0_rdata = reg_shire_pll_config_data_2_q;
        end
        AddrShirePllConfigData3: begin
          reg_shire_pll_config_data_3_en_d = apb_pwrite_i;
          s0_rdata = reg_shire_pll_config_data_3_q;
        end
        AddrShirePllReadData: s0_rdata[ShirePllReadBits-1:0] = reg_shire_pll_read_data_q;
        AddrShireCoopMode: begin
          reg_shire_coop_mode_en_d = apb_pwrite_i;
          s0_rdata[0] = reg_shire_coop_mode_q;
        end
        AddrShireCtrlClockmux: begin
          reg_shire_ctrl_clockmux_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_clkmux_ctl_t)-1:0] = reg_shire_ctrl_clockmux_q;
        end
        AddrShireCacheRamCfg1: begin
          reg_shire_cache_ram_cfg1_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_shire_cache_ram_cfg1_t)-1:0] = reg_shire_cache_ram_cfg1_q;
        end
        AddrShireCacheRamCfg2: begin
          reg_shire_cache_ram_cfg2_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_shire_cache_ram_cfg2_t)-1:0] = reg_shire_cache_ram_cfg2_q;
        end
        AddrShireCacheRamCfg3: begin
          reg_shire_cache_ram_cfg3_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_shire_cache_ram_cfg3_t)-1:0] = reg_shire_cache_ram_cfg3_q;
        end
        AddrShireCacheRamCfg4: begin
          reg_shire_cache_ram_cfg4_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_shire_cache_ram_cfg4_t)-1:0] = reg_shire_cache_ram_cfg4_q;
        end
        AddrShireNocInterruptStatus: s0_rdata[NocIntNum-1:0] = esr_shire_noc_interrupt_status_i;
        AddrShireDllAutoConfig: begin
          reg_shire_dll_auto_config_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_dll_auto_config_t)-1:0] = reg_shire_dll_auto_config_q;
        end
        AddrShireDllConfigData0: begin
          reg_shire_dll_config_data_0_en_d = apb_pwrite_i;
          s0_rdata = reg_shire_dll_config_data_0_q;
        end
        AddrShireDllConfigData1: begin
          reg_shire_dll_config_data_1_en_d = apb_pwrite_i;
          s0_rdata = reg_shire_dll_config_data_1_q;
        end
        AddrShireDllReadData: s0_rdata[ShireDllReadBits-1:0] = reg_shire_dll_read_data_q;
        AddrTboxRboxDbgRc: begin
          reg_tbox_rbox_dbg_rc_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_debug_rbox_tbox_rc_t)-1:0] = reg_tbox_rbox_dbg_rc_q;
        end
        AddrUcConfig: begin
          reg_uc_config_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_uc_config_t)-1:0] = reg_uc_config_q;
        end
        AddrIcacheUPrefetch: begin
          reg_icache_uprefetch_en_d = apb_pwrite_i;
          s0_rdata[0] = icache_prefetch_all_idle;
        end
        AddrIcacheSPrefetch: begin
          reg_icache_sprefetch_en_d = apb_pwrite_i;
          s0_rdata[0] = icache_prefetch_all_idle;
        end
        AddrIcacheMPrefetch: begin
          reg_icache_mprefetch_en_d = apb_pwrite_i;
          s0_rdata[0] = icache_prefetch_all_idle;
        end
        AddrClkGateCtrl: begin
          reg_clk_gate_ctrl_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_clk_gate_ctrl_t)-1:0] = reg_clk_gate_ctrl_q;
        end
        AddrDebugClkGateCtrl: begin
          reg_debug_clk_gate_ctrl_en_d = apb_pwrite_i;
          s0_rdata[0] = reg_debug_clk_gate_ctrl_q;
        end
        AddrShireChannelEcoCtl: begin
          reg_shire_channel_eco_ctl_en_d = apb_pwrite_i;
          s0_rdata[7:0] = reg_shire_channel_eco_ctl_q;
        end
        AddrClkDlyCtl: begin
          reg_clk_dly_ctl_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_clk_dly_ctl_t)-1:0] = reg_clk_dly_ctl_q;
        end
        AddrDllDlyEstCtl: begin
          reg_dll_dly_est_ctl_en_d = apb_pwrite_i;
          s0_rdata[10:0] = dll_dly_est_ctl_q;
        end
        AddrDllDlyEstSts: begin
          s0_rdata[8*NumNeigh+4-1:0] = dll_dly_est_sts_q;
        end
        default: begin
          if (apb_paddr_i[AdWidth-1:5] == AddrFastLocalBarrierBase[AdWidth-1:5]) begin
            reg_fast_local_barrier_en_d[apb_paddr_i[4:0]] = apb_pwrite_i;
            s0_rdata[BarrierValueWidth-1:0] = uc_esr_values_i.barriers[apb_paddr_i[4:0]];
          end else begin
            s0_addr_valid = 1'b0;
          end
        end
      endcase
    end
  end

  assign s1_en = apb_psel_i && apb_penable_i;

  always_ff @(posedge clk_i) begin
    if (s0_en && !apb_pwrite_i) begin
      s1_rdata_q <= s0_rdata;
    end
  end

  always_comb begin
    reg_shire_config_d = reg_shire_config_q;
    reg_ipi_trigger_d = reg_ipi_trigger_q;
    reg_mtime_local_target_d = reg_mtime_local_target_q;
    reg_shire_power_ctrl_d = reg_shire_power_ctrl_q;
    reg_power_ctrl_neigh_nsleepin_d = reg_power_ctrl_neigh_nsleepin_q;
    reg_power_ctrl_neigh_isolation_d = reg_power_ctrl_neigh_isolation_q;
    reg_shire_pll_auto_config_d = reg_shire_pll_auto_config_q;
    reg_shire_pll_config_data_0_d = reg_shire_pll_config_data_0_q;
    reg_shire_pll_config_data_1_d = reg_shire_pll_config_data_1_q;
    reg_shire_pll_config_data_2_d = reg_shire_pll_config_data_2_q;
    reg_shire_pll_config_data_3_d = reg_shire_pll_config_data_3_q;
    reg_shire_coop_mode_d = reg_shire_coop_mode_q;
    reg_shire_ctrl_clockmux_d = reg_shire_ctrl_clockmux_q;
    reg_shire_cache_ram_cfg1_d = reg_shire_cache_ram_cfg1_q;
    reg_shire_cache_ram_cfg2_d = reg_shire_cache_ram_cfg2_q;
    reg_shire_cache_ram_cfg3_d = reg_shire_cache_ram_cfg3_q;
    reg_shire_cache_ram_cfg4_d = reg_shire_cache_ram_cfg4_q;
    reg_shire_dll_auto_config_d = reg_shire_dll_auto_config_q;
    reg_shire_dll_config_data_0_d = reg_shire_dll_config_data_0_q;
    reg_shire_dll_config_data_1_d = reg_shire_dll_config_data_1_q;
    reg_tbox_rbox_dbg_rc_d = reg_tbox_rbox_dbg_rc_q;
    reg_uc_config_d = reg_uc_config_q;
    reg_clk_gate_ctrl_d = reg_clk_gate_ctrl_q;
    reg_debug_clk_gate_ctrl_d = reg_debug_clk_gate_ctrl_q;
    reg_shire_channel_eco_ctl_d = reg_shire_channel_eco_ctl_q;
    reg_clk_dly_ctl_d = reg_clk_dly_ctl_q;
    dll_dly_est_ctl_d = dll_dly_est_ctl_q;
    dll_dly_est_ctl_d[9] = 1'b0;
    dll_dly_est_ctl_d[8] = 1'b0;

    if (reg_shire_config_en_q) begin
      reg_shire_config_d = shire_config_from_bits(apb_pwdata_i[ShireConfigWidth-1:0]);
    end
    if (reg_ipi_trigger_en_q) begin
      reg_ipi_trigger_d = reg_ipi_trigger_q | apb_pwdata_i;
    end
    if (reg_ipi_trigger_clear_en_q) begin
      reg_ipi_trigger_d = reg_ipi_trigger_q & ~apb_pwdata_i;
    end
    if (reg_mtime_local_target_en_q) begin
      reg_mtime_local_target_d = apb_pwdata_i[ThreadsPerShire-1:0];
    end
    if (reg_shire_power_ctrl_en_q) begin
      reg_shire_power_ctrl_d = apb_pwdata_i[3*NumNeigh-1:0];
    end
    reg_shire_power_ctrl_d[2*NumNeigh +: NumNeigh] = esr_power_ctrl_glb_nsleepout_i;
    if (reg_power_ctrl_neigh_nsleepin_en_q) begin
      reg_power_ctrl_neigh_nsleepin_d = apb_pwdata_i[NumNeigh*MinPerNeigh-1:0];
    end
    if (reg_power_ctrl_neigh_isolation_en_q) begin
      reg_power_ctrl_neigh_isolation_d = apb_pwdata_i[NumNeigh*MinPerNeigh-1:0];
    end
    if (reg_shire_pll_auto_config_en_q) begin
      reg_shire_pll_auto_config_d = pll_auto_from_bits(apb_pwdata_i[$bits(esr_pll_auto_config_t)-1:0]);
    end
    if (reg_shire_pll_config_data_0_en_q) begin
      reg_shire_pll_config_data_0_d = apb_pwdata_i;
    end
    if (reg_shire_pll_config_data_1_en_q) begin
      reg_shire_pll_config_data_1_d = apb_pwdata_i;
    end
    if (reg_shire_pll_config_data_2_en_q) begin
      reg_shire_pll_config_data_2_d = apb_pwdata_i;
    end
    if (reg_shire_pll_config_data_3_en_q) begin
      reg_shire_pll_config_data_3_d = apb_pwdata_i;
    end
    if (reg_shire_coop_mode_en_q) begin
      reg_shire_coop_mode_d = apb_pwdata_i[0];
    end
    if (reg_shire_ctrl_clockmux_en_q) begin
      reg_shire_ctrl_clockmux_d = apb_pwdata_i[$bits(esr_clkmux_ctl_t)-1:0];
    end
    if (reg_shire_cache_ram_cfg1_en_q) begin
      reg_shire_cache_ram_cfg1_d = apb_pwdata_i[$bits(esr_shire_cache_ram_cfg1_t)-1:0];
    end
    if (reg_shire_cache_ram_cfg2_en_q) begin
      reg_shire_cache_ram_cfg2_d = apb_pwdata_i[$bits(esr_shire_cache_ram_cfg2_t)-1:0];
    end
    if (reg_shire_cache_ram_cfg3_en_q) begin
      reg_shire_cache_ram_cfg3_d = apb_pwdata_i[$bits(esr_shire_cache_ram_cfg3_t)-1:0];
    end
    if (reg_shire_cache_ram_cfg4_en_q) begin
      reg_shire_cache_ram_cfg4_d = apb_pwdata_i[$bits(esr_shire_cache_ram_cfg4_t)-1:0];
    end
    if (reg_shire_dll_auto_config_en_q) begin
      reg_shire_dll_auto_config_d = dll_auto_from_bits(apb_pwdata_i[$bits(esr_dll_auto_config_t)-1:0]);
    end
    if (reg_shire_dll_config_data_0_en_q) begin
      reg_shire_dll_config_data_0_d = apb_pwdata_i;
    end
    if (reg_shire_dll_config_data_1_en_q) begin
      reg_shire_dll_config_data_1_d = apb_pwdata_i;
    end
    if (reg_tbox_rbox_dbg_rc_en_q) begin
      reg_tbox_rbox_dbg_rc_d = apb_pwdata_i[$bits(esr_debug_rbox_tbox_rc_t)-1:0];
    end
    reg_tbox_rbox_dbg_rc_d.rbox_halted = debug_rc_rbox_ack_i.halt_ack;
    reg_tbox_rbox_dbg_rc_d.rbox_resume_ack = debug_rc_rbox_ack_i.resume_ack;
    reg_tbox_rbox_dbg_rc_d.rbox_reset_ack = debug_rc_rbox_ack_i.reset_ack;
    for (int unsigned i = 0; i < TboxPerShire; i++) begin
      if (i < NumNeigh) begin
        reg_tbox_rbox_dbg_rc_d.tbox_halted[i] = debug_rc_tbox_ack_i[i].halt_ack;
        reg_tbox_rbox_dbg_rc_d.tbox_resume_ack[i] = debug_rc_tbox_ack_i[i].resume_ack;
      end else begin
        reg_tbox_rbox_dbg_rc_d.tbox_halted[i] = 1'b0;
        reg_tbox_rbox_dbg_rc_d.tbox_resume_ack[i] = 1'b0;
      end
    end
    if (reg_uc_config_en_q) begin
      reg_uc_config_d = apb_pwdata_i[$bits(esr_uc_config_t)-1:0];
    end
    if (reg_clk_gate_ctrl_en_q) begin
      reg_clk_gate_ctrl_d = apb_pwdata_i[$bits(esr_clk_gate_ctrl_t)-1:0];
    end
    if (reg_debug_clk_gate_ctrl_en_q) begin
      reg_debug_clk_gate_ctrl_d = apb_pwdata_i[0];
    end
    if (reg_shire_channel_eco_ctl_en_q) begin
      reg_shire_channel_eco_ctl_d = apb_pwdata_i[7:0];
    end
    if (reg_clk_dly_ctl_en_q) begin
      reg_clk_dly_ctl_d = apb_pwdata_i[$bits(esr_clk_dly_ctl_t)-1:0];
    end
    if (reg_dll_dly_est_ctl_en_q) begin
      dll_dly_est_ctl_d = apb_pwdata_i[10:0];
    end
  end

  always_comb begin
    reg_shire_pll_read_data_d = reg_shire_pll_read_data_q;
    if (esr_pll_rrdy_i) begin
      reg_shire_pll_read_data_d[ShirePllDataBits-1:0] = esr_pll_rdata_i;
    end
    reg_shire_pll_read_data_d[ShirePllDataBits] = esr_pll_busy_i;
    reg_shire_pll_read_data_d[ShirePllDataBits+1] = esr_pll_lock_i;

    reg_shire_dll_read_data_d = reg_shire_dll_read_data_q;
    if (esr_dll_rrdy_i) begin
      reg_shire_dll_read_data_d[ShireDllDataBits-1:0] = esr_dll_rdata_i;
    end
    reg_shire_dll_read_data_d[ShireDllDataBits] = esr_dll_busy_i;
    reg_shire_dll_read_data_d[ShireDllDataBits+1] = esr_dll_lock_i;

    dll_dly_est_sts_d = '0;
    for (int unsigned i = 0; i < NumNeigh; i++) begin
      dll_dly_est_sts_d[i] = esr_dll_dly_est_sts_i[i].sts_done;
      dll_dly_est_sts_d[(8*i + 4) +: 8] = esr_dll_dly_est_sts_i[i].sts_errn;
    end
  end

  always_comb begin
    icache_prv_prefetch_pending_d = icache_prv_prefetch_pending_q;
    icache_prv_prefetch_serve = '0;
    esr_icache_prefetch_conf_d = esr_icache_prefetch_conf_q;
    esr_icache_prefetch_start_d = '0;
    icache_prefetch_busy_d = icache_prefetch_busy_q;
    icache_prefetch_idle_d = icache_prefetch_idle_q;

    if (reg_icache_uprefetch_en_q && reg_shire_coop_mode_q) begin
      icache_prv_prefetch_pending_d[CsrPrvUser] = 1'b1;
    end
    if (reg_icache_sprefetch_en_q && reg_shire_coop_mode_q) begin
      icache_prv_prefetch_pending_d[CsrPrvSupervisor] = 1'b1;
    end
    if (reg_icache_mprefetch_en_q && reg_shire_coop_mode_q) begin
      icache_prv_prefetch_pending_d[CsrPrvMachine] = 1'b1;
    end

    icache_prv_prefetch_serve[CsrPrvMachine] = icache_prv_prefetch_pending_q[CsrPrvMachine];
    icache_prv_prefetch_serve[2] = icache_prv_prefetch_pending_q[2] &&
                                   !icache_prv_prefetch_pending_q[CsrPrvMachine] &&
                                   (icache_prefetch_all_idle || (esr_icache_prefetch_conf_q.prv <= 2'd2));
    icache_prv_prefetch_serve[CsrPrvSupervisor] =
        icache_prv_prefetch_pending_q[CsrPrvSupervisor] &&
        !(|icache_prv_prefetch_pending_q[CsrPrvMachine:2]) &&
        (icache_prefetch_all_idle || (esr_icache_prefetch_conf_q.prv <= 2'd1));
    icache_prv_prefetch_serve[CsrPrvUser] =
        icache_prv_prefetch_pending_q[CsrPrvUser] &&
        !(|icache_prv_prefetch_pending_q[CsrPrvMachine:1]) &&
        (icache_prefetch_all_idle || (esr_icache_prefetch_conf_q.prv <= 2'd0));

    if (icache_prv_prefetch_serve[CsrPrvMachine]) begin
      esr_icache_prefetch_conf_d.prv = 2'd3;
      esr_icache_prefetch_conf_d.start_addr = reg_icache_mprefetch.start_addr;
      esr_icache_prefetch_conf_d.num_lines = reg_icache_mprefetch.num_lines;
      esr_icache_prefetch_start_d = reg_icache_mprefetch.en_mask;
    end else if (icache_prv_prefetch_serve[CsrPrvSupervisor]) begin
      esr_icache_prefetch_conf_d.prv = 2'd1;
      esr_icache_prefetch_conf_d.start_addr = reg_icache_sprefetch.start_addr;
      esr_icache_prefetch_conf_d.num_lines = reg_icache_sprefetch.num_lines;
      esr_icache_prefetch_start_d = reg_icache_sprefetch.en_mask;
    end else if (icache_prv_prefetch_serve[CsrPrvUser]) begin
      esr_icache_prefetch_conf_d.prv = 2'd0;
      esr_icache_prefetch_conf_d.start_addr = reg_icache_uprefetch.start_addr;
      esr_icache_prefetch_conf_d.num_lines = reg_icache_uprefetch.num_lines;
      esr_icache_prefetch_start_d = reg_icache_uprefetch.en_mask;
    end

    icache_prv_prefetch_pending_d &= ~icache_prv_prefetch_serve;
    for (int unsigned i = 0; i < IcachePerShire; i++) begin
      if (esr_icache_prefetch_start_d[i]) begin
        icache_prefetch_busy_d[i] = 1'b0;
      end else if (esr_icache_prefetch_done_q[i] && !esr_icache_prefetch_done_i[i]) begin
        icache_prefetch_busy_d[i] = 1'b1;
      end else if (!esr_icache_prefetch_done_q[i] && esr_icache_prefetch_done_i[i]) begin
        icache_prefetch_busy_d[i] = 1'b0;
      end
    end
    icache_prefetch_idle_d = (icache_prefetch_idle_q |
                              (icache_prefetch_busy_q & ~icache_prefetch_busy_d)) &
                             ~esr_icache_prefetch_start_d;
  end

  assign icache_prefetch_all_idle = &icache_prefetch_idle_q;

  always_ff @(posedge clk_i or negedge rst_c_ni) begin
    if (!rst_c_ni) begin
      features_written_q <= 1'b0;
      s1_addr_valid_q <= 1'b0;
      reg_minion_feature_en_q <= 1'b0;
      reg_shire_config_en_q <= 1'b0;
      reg_thread1_disable_en_q <= 1'b0;
      reg_ipi_redirect_mask_en_q <= 1'b0;
      reg_ipi_redirect_filter_en_q <= 1'b0;
      reg_ipi_trigger_en_q <= 1'b0;
      reg_ipi_trigger_clear_en_q <= 1'b0;
      reg_fcc_credinc_en_q <= '0;
      reg_fast_local_barrier_en_q <= '0;
      reg_mtime_local_target_en_q <= 1'b0;
      reg_shire_power_ctrl_en_q <= 1'b0;
      reg_power_ctrl_neigh_nsleepin_en_q <= 1'b0;
      reg_power_ctrl_neigh_isolation_en_q <= 1'b0;
      reg_thread0_disable_en_q <= 1'b0;
      reg_shire_pll_auto_config_en_q <= 1'b0;
      reg_shire_pll_config_data_0_en_q <= 1'b0;
      reg_shire_pll_config_data_1_en_q <= 1'b0;
      reg_shire_pll_config_data_2_en_q <= 1'b0;
      reg_shire_pll_config_data_3_en_q <= 1'b0;
      reg_shire_coop_mode_en_q <= 1'b0;
      reg_shire_ctrl_clockmux_en_q <= 1'b0;
      reg_shire_cache_ram_cfg1_en_q <= 1'b0;
      reg_shire_cache_ram_cfg2_en_q <= 1'b0;
      reg_shire_cache_ram_cfg3_en_q <= 1'b0;
      reg_shire_cache_ram_cfg4_en_q <= 1'b0;
      reg_shire_dll_auto_config_en_q <= 1'b0;
      reg_shire_dll_config_data_0_en_q <= 1'b0;
      reg_shire_dll_config_data_1_en_q <= 1'b0;
      reg_tbox_rbox_dbg_rc_en_q <= 1'b0;
      reg_uc_config_en_q <= 1'b0;
      reg_icache_uprefetch_en_q <= 1'b0;
      reg_icache_sprefetch_en_q <= 1'b0;
      reg_icache_mprefetch_en_q <= 1'b0;
      reg_clk_gate_ctrl_en_q <= 1'b0;
      reg_debug_clk_gate_ctrl_en_q <= 1'b0;
      reg_shire_channel_eco_ctl_en_q <= 1'b0;
      reg_clk_dly_ctl_en_q <= 1'b0;
      reg_dll_dly_est_ctl_en_q <= 1'b0;
    end else begin
      features_written_q <= features_written_q || reg_minion_feature_en_q;
      if (s0_en) begin
        s1_addr_valid_q <= s0_addr_valid;
      end
      if (s0_en || s1_en) begin
        reg_minion_feature_en_q <= reg_minion_feature_en_d;
        reg_shire_config_en_q <= reg_shire_config_en_d;
        reg_thread1_disable_en_q <= reg_thread1_disable_en_d;
        reg_ipi_redirect_mask_en_q <= reg_ipi_redirect_mask_en_d;
        reg_ipi_redirect_filter_en_q <= reg_ipi_redirect_filter_en_d;
        reg_ipi_trigger_en_q <= reg_ipi_trigger_en_d;
        reg_ipi_trigger_clear_en_q <= reg_ipi_trigger_clear_en_d;
        reg_fcc_credinc_en_q <= reg_fcc_credinc_en_d;
        reg_fast_local_barrier_en_q <= reg_fast_local_barrier_en_d;
        reg_mtime_local_target_en_q <= reg_mtime_local_target_en_d;
        reg_shire_power_ctrl_en_q <= reg_shire_power_ctrl_en_d;
        reg_power_ctrl_neigh_nsleepin_en_q <= reg_power_ctrl_neigh_nsleepin_en_d;
        reg_power_ctrl_neigh_isolation_en_q <= reg_power_ctrl_neigh_isolation_en_d;
        reg_thread0_disable_en_q <= reg_thread0_disable_en_d;
        reg_shire_pll_auto_config_en_q <= reg_shire_pll_auto_config_en_d;
        reg_shire_pll_config_data_0_en_q <= reg_shire_pll_config_data_0_en_d;
        reg_shire_pll_config_data_1_en_q <= reg_shire_pll_config_data_1_en_d;
        reg_shire_pll_config_data_2_en_q <= reg_shire_pll_config_data_2_en_d;
        reg_shire_pll_config_data_3_en_q <= reg_shire_pll_config_data_3_en_d;
        reg_shire_coop_mode_en_q <= reg_shire_coop_mode_en_d;
        reg_shire_ctrl_clockmux_en_q <= reg_shire_ctrl_clockmux_en_d;
        reg_shire_cache_ram_cfg1_en_q <= reg_shire_cache_ram_cfg1_en_d;
        reg_shire_cache_ram_cfg2_en_q <= reg_shire_cache_ram_cfg2_en_d;
        reg_shire_cache_ram_cfg3_en_q <= reg_shire_cache_ram_cfg3_en_d;
        reg_shire_cache_ram_cfg4_en_q <= reg_shire_cache_ram_cfg4_en_d;
        reg_shire_dll_auto_config_en_q <= reg_shire_dll_auto_config_en_d;
        reg_shire_dll_config_data_0_en_q <= reg_shire_dll_config_data_0_en_d;
        reg_shire_dll_config_data_1_en_q <= reg_shire_dll_config_data_1_en_d;
        reg_tbox_rbox_dbg_rc_en_q <= reg_tbox_rbox_dbg_rc_en_d;
        reg_uc_config_en_q <= reg_uc_config_en_d;
        reg_icache_uprefetch_en_q <= reg_icache_uprefetch_en_d;
        reg_icache_sprefetch_en_q <= reg_icache_sprefetch_en_d;
        reg_icache_mprefetch_en_q <= reg_icache_mprefetch_en_d;
        reg_clk_gate_ctrl_en_q <= reg_clk_gate_ctrl_en_d;
        reg_debug_clk_gate_ctrl_en_q <= reg_debug_clk_gate_ctrl_en_d;
        reg_shire_channel_eco_ctl_en_q <= reg_shire_channel_eco_ctl_en_d;
        reg_clk_dly_ctl_en_q <= reg_clk_dly_ctl_en_d;
        reg_dll_dly_est_ctl_en_q <= reg_dll_dly_est_ctl_en_d;
      end
    end
  end

  always_ff @(posedge clk_i or negedge rst_c_ni) begin
    if (!rst_c_ni) begin
      reg_shire_config_q <= shire_config_reset(shire_id_reset_val_i);
      reg_mtime_local_target_q <= '0;
      reg_shire_power_ctrl_q <= ShirePowerCtrlResetVal;
      reg_power_ctrl_neigh_nsleepin_q <= '1;
      reg_power_ctrl_neigh_isolation_q <= '0;
      reg_shire_pll_auto_config_q <= '0;
      reg_shire_ctrl_clockmux_q <= '0;
      reg_shire_cache_ram_cfg1_q <= ram_cfg1_reset();
      reg_shire_cache_ram_cfg2_q <= ram_cfg2_reset();
      reg_shire_cache_ram_cfg3_q <= ram_cfg3_reset();
      reg_shire_cache_ram_cfg4_q <= ram_cfg4_reset();
      reg_shire_dll_auto_config_q <= '0;
      reg_uc_config_q <= '0;
      reg_clk_gate_ctrl_q <= '0;
      reg_shire_channel_eco_ctl_q <= '0;
      reg_clk_dly_ctl_q <= '0;
      dll_dly_est_ctl_q <= '0;
      reg_shire_pll_read_data_q <= '0;
      reg_shire_dll_read_data_q <= '0;
      dll_dly_est_sts_q <= '0;
    end else begin
      reg_shire_config_q <= reg_shire_config_d;
      reg_mtime_local_target_q <= reg_mtime_local_target_d;
      reg_shire_power_ctrl_q <= reg_shire_power_ctrl_d;
      reg_power_ctrl_neigh_nsleepin_q <= reg_power_ctrl_neigh_nsleepin_d;
      reg_power_ctrl_neigh_isolation_q <= reg_power_ctrl_neigh_isolation_d;
      reg_shire_pll_auto_config_q <= reg_shire_pll_auto_config_d;
      reg_shire_ctrl_clockmux_q <= reg_shire_ctrl_clockmux_d;
      reg_shire_cache_ram_cfg1_q <= reg_shire_cache_ram_cfg1_d;
      reg_shire_cache_ram_cfg2_q <= reg_shire_cache_ram_cfg2_d;
      reg_shire_cache_ram_cfg3_q <= reg_shire_cache_ram_cfg3_d;
      reg_shire_cache_ram_cfg4_q <= reg_shire_cache_ram_cfg4_d;
      reg_shire_dll_auto_config_q <= reg_shire_dll_auto_config_d;
      reg_uc_config_q <= reg_uc_config_d;
      reg_clk_gate_ctrl_q <= reg_clk_gate_ctrl_d;
      reg_shire_channel_eco_ctl_q <= reg_shire_channel_eco_ctl_d;
      reg_clk_dly_ctl_q <= reg_clk_dly_ctl_d;
      dll_dly_est_ctl_q <= dll_dly_est_ctl_d;
      reg_shire_pll_read_data_q <= reg_shire_pll_read_data_d;
      reg_shire_dll_read_data_q <= reg_shire_dll_read_data_d;
      dll_dly_est_sts_q <= dll_dly_est_sts_d;
    end
  end

  always_ff @(posedge clk_i or negedge rst_w_ni) begin
    if (!rst_w_ni) begin
      reg_ipi_trigger_q <= '0;
      reg_shire_coop_mode_q <= 1'b0;
      esr_icache_prefetch_start_q <= '0;
      esr_icache_prefetch_done_q <= '1;
      icache_prv_prefetch_pending_q <= '0;
      icache_prefetch_busy_q <= '0;
      icache_prefetch_idle_q <= '1;
    end else begin
      reg_ipi_trigger_q <= reg_ipi_trigger_d;
      reg_shire_coop_mode_q <= reg_shire_coop_mode_d;
      esr_icache_prefetch_start_q <= esr_icache_prefetch_start_d;
      esr_icache_prefetch_done_q <= esr_icache_prefetch_done_i;
      icache_prv_prefetch_pending_q <= icache_prv_prefetch_pending_d;
      icache_prefetch_busy_q <= icache_prefetch_busy_d;
      icache_prefetch_idle_q <= icache_prefetch_idle_d;
    end
  end

  always_ff @(posedge clk_i) begin
    if (|icache_prv_prefetch_serve) begin
      esr_icache_prefetch_conf_q <= esr_icache_prefetch_conf_d;
    end
    reg_shire_pll_config_data_0_q <= reg_shire_pll_config_data_0_d;
    reg_shire_pll_config_data_1_q <= reg_shire_pll_config_data_1_d;
    reg_shire_pll_config_data_2_q <= reg_shire_pll_config_data_2_d;
    reg_shire_pll_config_data_3_q <= reg_shire_pll_config_data_3_d;
    reg_shire_dll_config_data_0_q <= reg_shire_dll_config_data_0_d;
    reg_shire_dll_config_data_1_q <= reg_shire_dll_config_data_1_d;
  end

  always_ff @(posedge clk_i or negedge rst_d_ni) begin
    if (!rst_d_ni) begin
      reg_tbox_rbox_dbg_rc_q <= '0;
      reg_debug_clk_gate_ctrl_q <= 1'b0;
    end else begin
      reg_tbox_rbox_dbg_rc_q <= reg_tbox_rbox_dbg_rc_d;
      reg_debug_clk_gate_ctrl_q <= reg_debug_clk_gate_ctrl_d;
    end
  end

  assign esr_minion_feature_o = reg_minion_feature_q;
  assign esr_shire_config_o = reg_shire_config_q;
  assign esr_thread0_enable_o = ~reg_thread0_disable_q;
  assign esr_thread1_enable_o = reg_minion_feature_q[MinionFeatureDisableMultithreadingBit] ?
                                '0 : ~reg_thread1_disable_q;
  assign esr_ipi_trigger_o = reg_ipi_trigger_q;
  assign esr_mtime_local_target_o = reg_mtime_local_target_q;
  assign esr_ipi_redirect_trigger_o = reg_ipi_redirect_mask_en_q ?
                                      (reg_ipi_redirect_filter_q & apb_pwdata_i) : '0;
  assign esr_power_ctrl_glb_nsleepin_o = reg_shire_power_ctrl_q[0 +: NumNeigh];
  assign esr_power_ctrl_glb_iso_o = reg_shire_power_ctrl_q[NumNeigh +: NumNeigh];
  assign esr_power_ctrl_neigh_nsleepin_o = reg_power_ctrl_neigh_nsleepin_q;
  assign esr_power_ctrl_neigh_iso_o = reg_power_ctrl_neigh_isolation_q;
  assign esr_pll_ctrl_o = reg_shire_pll_auto_config_q;
  assign esr_pll_conf_o = {
      reg_shire_pll_config_data_3_q,
      reg_shire_pll_config_data_2_q,
      reg_shire_pll_config_data_1_q,
      reg_shire_pll_config_data_0_q};
  assign esr_dll_ctrl_o = reg_shire_dll_auto_config_q;
  assign esr_dll_conf_o = {reg_shire_dll_config_data_1_q, reg_shire_dll_config_data_0_q};
  assign esr_shire_coop_mode_o = reg_shire_coop_mode_q;
  assign esr_shire_ctrl_clockmux_o = reg_shire_ctrl_clockmux_q;
  assign esr_uc_config_o = reg_uc_config_q;
  assign esr_clk_gate_ctrl_o = reg_clk_gate_ctrl_q;
  assign esr_debug_clk_gate_ctrl_o = reg_debug_clk_gate_ctrl_q;
  assign esr_shire_channel_eco_ctl_o = reg_shire_channel_eco_ctl_q;
  assign esr_clk_dly_ctl_o = reg_clk_dly_ctl_q;
  assign esr_dll_dly_est_ctl_o.ctl_enable = dll_dly_est_ctl_q[10];
  assign esr_dll_dly_est_ctl_o.ctl_init = dll_dly_est_ctl_q[9];
  assign esr_dll_dly_est_ctl_o.ctl_start = dll_dly_est_ctl_q[8];
  assign esr_dll_dly_est_ctl_o.ctl_txn = dll_dly_est_ctl_q[7:0];

  assign ram_cfg_o.test_en = reg_shire_cache_ram_cfg2_q.sc_mbd_test1 |
                             reg_shire_cache_ram_cfg2_q.sc_mbd_test_rnm;
  assign ram_cfg_o.rm = reg_shire_cache_ram_cfg2_q.sc_mbd_rm;
  assign ram_cfg_o.rme = reg_shire_cache_ram_cfg2_q.sc_mbd_rme;
  assign ram_cfg_o.ra = reg_shire_cache_ram_cfg2_q.sc_mbd_ra;
  assign ram_cfg_o.wa = reg_shire_cache_ram_cfg2_q.sc_mbd_wa[1:0];
  assign ram_cfg_o.wpulse = reg_shire_cache_ram_cfg2_q.sc_mbd_wpulse;
  assign ram_cfg_o.deep_sleep = 1'b0;
  assign ram_cfg_o.shut_down = 1'b0;

  assign uc_esr_enables_o.barriers = reg_fast_local_barrier_en_q;
  assign uc_esr_enables_o.fcc = reg_fcc_credinc_en_q;

  assign esr_icache_prefetch_conf_o = esr_icache_prefetch_conf_q;
  assign esr_icache_prefetch_start_o = esr_icache_prefetch_start_q;
  assign debug_rc_ss_tbox_rbox_o = {
      reg_tbox_rbox_dbg_rc_q.rbox_single_step,
      reg_tbox_rbox_dbg_rc_q.tbox_single_step[TboxPerShire-1:0]};

  assign apb_prdata_o = s1_rdata_q;
  assign apb_pslverr_o = !s1_addr_valid_q;
  assign apb_pready_o = 1'b1;

  logic unused_inputs;
  assign unused_inputs = &{
    1'b0,
    test_en_i,
    dft_i.scanmode,
    dft_i.scan_reset_n,
    dft_i.sram_clk_override,
    dft_i.ram_rei,
    dft_i.ram_wei,
    esr_icache_prefetch_done_q,
    &reg_ipi_redirect_mask_en_q,
    &reg_shire_cache_ram_cfg1_q,
    &reg_shire_cache_ram_cfg3_q,
    &reg_shire_cache_ram_cfg4_q,
    &reg_power_ctrl_neigh_nsleepin_q,
    &reg_power_ctrl_neigh_isolation_q
  };
endmodule
