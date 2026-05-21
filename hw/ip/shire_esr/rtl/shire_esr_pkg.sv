// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

package shire_esr_pkg;

  /* verilator lint_off UNUSEDPARAM */  // Shire ESR constants are consumed incrementally by later compute-shire tasks.

  localparam int unsigned EsrApbDataWidth = 64;
  localparam int unsigned EsrCacheBankApbAddrWidth = 10;
  localparam int unsigned EsrCacheBankApbPpAddrWidth = EsrCacheBankApbAddrWidth + 2;
  localparam int unsigned EsrShireOtherApbAddrWidth = 14;
  localparam int unsigned EsrShireOtherApbPpAddrWidth = EsrShireOtherApbAddrWidth + 2;

  localparam int unsigned NumShireIds = shirecache_pkg::NumShireIds;
  localparam int unsigned NumShireIdsBits = $clog2(NumShireIds);
  localparam int unsigned NumNeigh = shirecache_pkg::NumNeigh;
  localparam int unsigned MinPerNeigh = 8;
  localparam int unsigned ThreadsPerShire = NumNeigh * MinPerNeigh;
  localparam int unsigned TboxPerShire = NumNeigh;
  localparam int unsigned IcachePerShire = NumNeigh;
  localparam int unsigned CsrPrvLevels = 4;
  localparam int unsigned CsrPrvUser = 0;
  localparam int unsigned CsrPrvSupervisor = 1;
  localparam int unsigned CsrPrvMachine = 3;
  localparam int unsigned BarrierRegisters = 32;
  localparam int unsigned BarrierValueWidth = 8;
  localparam int unsigned UcFccRegisters = 4;
  localparam int unsigned NocIntNum = 19;
  localparam int unsigned NocDebugIntNum = 2;
  localparam int unsigned ShirePllDataBits = 16;
  localparam int unsigned ShirePllReadBits = ShirePllDataBits + 2;
  localparam int unsigned ShirePllConfBits = 256;
  localparam int unsigned ShireDllDataBits = 16;
  localparam int unsigned ShireDllReadBits = ShireDllDataBits + 2;
  localparam int unsigned ShireDllConfBits = 128;

  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScL3ShireSwizzleCtl =
      {2'd3, 10'h000};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScReqqCtl =
      {2'd3, 10'h001};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScPipeCtl =
      {2'd3, 10'h002};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScL2CacheCtl =
      {2'd3, 10'h003};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScL3CacheCtl =
      {2'd3, 10'h004};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScScpCacheCtl =
      {2'd3, 10'h005};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScIdxCopSmCtl =
      {2'd3, 10'h006};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScIdxCopSmPhysicalIndex =
      {2'd3, 10'h007};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScIdxCopSmData0 =
      {2'd3, 10'h008};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScIdxCopSmData1 =
      {2'd3, 10'h009};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScIdxCopSmEcc =
      {2'd3, 10'h00a};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScErrLogCtl =
      {2'd3, 10'h00b};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScErrLogInfo =
      {2'd3, 10'h00c};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScErrLogAddress =
      {2'd3, 10'h00d};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScSbeDbeCounts =
      {2'd3, 10'h00e};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScReqqDebugCtl =
      {2'd3, 10'h00f};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScReqqDebug0 =
      {2'd3, 10'h010};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScReqqDebug1 =
      {2'd3, 10'h011};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScReqqDebug2 =
      {2'd3, 10'h012};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScReqqDebug3 =
      {2'd3, 10'h013};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScEcoCtl =
      {2'd3, 10'h014};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScPerfmonCtlStatus =
      {2'd3, 10'h017};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScPerfmonCycCntr =
      {2'd3, 10'h018};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScPerfmonP0Cntr =
      {2'd3, 10'h019};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScPerfmonP1Cntr =
      {2'd3, 10'h01a};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScPerfmonP0Qual =
      {2'd3, 10'h01b};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScPerfmonP1Qual =
      {2'd3, 10'h01c};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScIdxCopSmCtlUser =
      {2'd0, 10'h020};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScTraceAddressEnable =
      {2'd2, 10'h3f0};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScTraceAddressValue =
      {2'd2, 10'h3f1};
  localparam logic [EsrCacheBankApbPpAddrWidth-1:0] EsrCacheBankAddrScTraceCtl =
      {2'd2, 10'h3f2};

  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrMinionFeature =
      {2'd3, 14'h0000};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShireConfig =
      {2'd3, 14'h0001};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrThread1Disable =
      {2'd3, 14'h0002};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShireCacheBuildConfig =
      {2'd3, 14'h0003};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShireCacheRevisionId =
      {2'd3, 14'h0004};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrIpiRedirectMask =
      {2'd0, 14'h0010};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrIpiRedirectFilter =
      {2'd3, 14'h0011};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrIpiTrigger =
      {2'd3, 14'h0012};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrIpiTriggerClear =
      {2'd3, 14'h0013};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrFccCredinc0 =
      {2'd0, 14'h0018};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrFccCredinc1 =
      {2'd0, 14'h0019};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrFccCredinc2 =
      {2'd0, 14'h001a};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrFccCredinc3 =
      {2'd0, 14'h001b};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrFastLocalBarrierBase =
      {2'd0, 14'h0020};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrAndOrTreeL1 =
      {2'd2, 14'h3ff0};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrMtimeLocalTarget =
      {2'd3, 14'h0043};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShirePowerCtrl =
      {2'd3, 14'h0044};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrPowerCtrlNeighNsleepin =
      {2'd3, 14'h0045};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrPowerCtrlNeighIsolation =
      {2'd3, 14'h0046};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrPowerCtrlNeighNsleepout =
      {2'd3, 14'h0047};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrThread0Disable =
      {2'd3, 14'h0048};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShireErrorLog =
      {2'd3, 14'h0049};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShirePllAutoConfig =
      {2'd3, 14'h004a};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShirePllConfigData0 =
      {2'd3, 14'h004b};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShirePllConfigData1 =
      {2'd3, 14'h004c};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShirePllConfigData2 =
      {2'd3, 14'h004d};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShirePllConfigData3 =
      {2'd3, 14'h004e};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShirePllReadData =
      {2'd3, 14'h0051};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShireCoopMode =
      {2'd1, 14'h0052};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShireCtrlClockmux =
      {2'd3, 14'h0053};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShireCacheRamCfg1 =
      {2'd3, 14'h0054};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShireCacheRamCfg2 =
      {2'd3, 14'h0055};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShireCacheRamCfg3 =
      {2'd3, 14'h0056};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShireCacheRamCfg4 =
      {2'd3, 14'h0057};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShireNocInterruptStatus =
      {2'd3, 14'h0058};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShireDllAutoConfig =
      {2'd3, 14'h0059};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShireDllConfigData0 =
      {2'd3, 14'h005a};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShireDllConfigData1 =
      {2'd3, 14'h005b};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShireDllReadData =
      {2'd3, 14'h005c};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrTboxRboxDbgRc =
      {2'd2, 14'h3ff1};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrUcConfig =
      {2'd1, 14'h005d};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrIcacheUPrefetch =
      {2'd0, 14'h005f};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrIcacheSPrefetch =
      {2'd1, 14'h0060};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrIcacheMPrefetch =
      {2'd3, 14'h0061};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrClkGateCtrl =
      {2'd3, 14'h0062};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrDebugClkGateCtrl =
      {2'd2, 14'h3ff4};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrShireChannelEcoCtl =
      {2'd3, 14'h0068};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrClkDlyCtl =
      {2'd3, 14'h0069};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrDllDlyEstCtl =
      {2'd3, 14'h006a};
  localparam logic [EsrShireOtherApbPpAddrWidth-1:0] EsrShireOtherAddrDllDlyEstSts =
      {2'd3, 14'h006b};

  typedef struct packed {
    logic anyselected;
    logic anyunavailable;
    logic allhavereset;
    logic anyhavereset;
    logic allresumeack;
    logic anyresumeack;
    logic allrunning;
    logic anyrunning;
    logic allhalted;
    logic anyhalted1;
    logic anyhalted0;
  } esr_and_or_tree_l1_t;

  typedef struct packed {
    logic [5:0]  sel_taps_feedback_shire;
    logic [5:0]  sel_taps_feedback_neigh;
    logic [23:0] sel_taps_neigh;
  } esr_clk_dly_ctl_t;

  typedef struct packed {
    logic [3:0] neighs_clock_disable;
    logic       min_frontend_clock_gate_disable;
    logic       rbox_clock_gate_disable;
    logic       min_dcache_clock_gate_disable;
    logic       min_vputrans_clock_gate_disable;
    logic       min_vputima_clock_gate_disable;
    logic       min_vpulane_clock_gate_disable;
    logic       min_intpipe_clock_gate_disable;
  } esr_clk_gate_ctrl_t;

  typedef struct packed {
    logic       dll_muxsel;
    logic [2:0] muxsel;
  } esr_clkmux_ctl_t;

  typedef struct packed {
    logic       dll_enable;
    logic [1:0] pclk_sel;
    logic       lock_reset_disable;
    logic [2:0] reg_num;
    logic [2:0] reg_first;
    logic       write;
    logic       run;
    logic       enable;
    logic       reset;
  } esr_dll_auto_config_t;

  typedef struct packed {
    logic [1:0] pclk_sel;
    logic       lock_reset_disable;
    logic [3:0] reg_num;
    logic [5:0] reg_first;
    logic       write;
    logic       run;
    logic       enable;
    logic       reset;
  } esr_pll_auto_config_t;

  typedef struct packed {
    logic       rbox_single_step;
    logic [3:0] tbox_single_step;
    logic       rbox_halted;
    logic       rbox_resume_ack;
    logic       rbox_reset_ack;
    logic [3:0] tbox_halted;
    logic [3:0] tbox_resume_ack;
  } esr_debug_rbox_tbox_rc_t;

  typedef struct packed {
    logic qos_dram;
  } esr_uc_config_t;

  typedef struct packed {
    logic [2:0] sc_mbt_bc;
    logic       sc_mbt_test_rnm;
    logic       sc_mbt_test1;
    logic [2:0] sc_mbt_wpulse;
    logic [2:0] sc_mbt_wa;
    logic [1:0] sc_mbt_ra;
    logic [3:0] sc_mbt_rm;
    logic       sc_mbt_rme;
    logic [2:0] sc_mbs_bc;
    logic       sc_mbs_test_rnm;
    logic       sc_mbs_test1;
    logic [2:0] sc_mbs_wpulse;
    logic [2:0] sc_mbs_wa;
    logic [1:0] sc_mbs_ra;
    logic [3:0] sc_mbs_rm;
    logic       sc_mbs_rme;
  } esr_shire_cache_ram_cfg1_t;

  typedef struct packed {
    logic [2:0] sc_mbd_bc;
    logic       sc_mbd_test_rnm;
    logic       sc_mbd_test1;
    logic [2:0] sc_mbd_wpulse;
    logic [2:0] sc_mbd_wa;
    logic [1:0] sc_mbd_ra;
    logic [3:0] sc_mbd_rm;
    logic       sc_mbd_rme;
  } esr_shire_cache_ram_cfg2_t;

  typedef struct packed {
    logic [2:0] sc_mbb_bc;
    logic       sc_mbb_test_rnm;
    logic       sc_mbb_test1;
    logic [2:0] sc_mbb_wpulse;
    logic [2:0] sc_mbb_wa;
    logic [1:0] sc_mbb_ra;
    logic [3:0] sc_mbb_rm;
    logic       sc_mbb_rme;
    logic [2:0] sc_mbq_bc;
    logic       sc_mbq_test_rnm;
    logic       sc_mbq_test1;
    logic [2:0] sc_mbq_wpulse;
    logic [2:0] sc_mbq_wa;
    logic [1:0] sc_mbq_ra;
    logic [3:0] sc_mbq_rm;
    logic       sc_mbq_rme;
  } esr_shire_cache_ram_cfg3_t;

  typedef struct packed {
    logic [2:0] sc_mbr_1p_bc;
    logic       sc_mbr_1p_test_rnm;
    logic       sc_mbr_1p_test1;
    logic [2:0] sc_mbr_1p_wpulse;
    logic [2:0] sc_mbr_1p_wa;
    logic [1:0] sc_mbr_1p_ra;
    logic [3:0] sc_mbr_1p_rm;
    logic       sc_mbr_1p_rme;
    logic [2:0] sc_mbr_2p_bc;
    logic       sc_mbr_2p_test_rnm;
    logic       sc_mbr_2p_test1;
    logic [2:0] sc_mbr_2p_wpulse;
    logic [2:0] sc_mbr_2p_wa;
    logic [1:0] sc_mbr_2p_ra;
    logic [3:0] sc_mbr_2p_rm;
    logic       sc_mbr_2p_rme;
    logic [2:0] sc_mbi_bc;
    logic       sc_mbi_test_rnm;
    logic       sc_mbi_test1;
    logic [2:0] sc_mbi_wpulse;
    logic [2:0] sc_mbi_wa;
    logic [1:0] sc_mbi_ra;
    logic [3:0] sc_mbi_rm;
    logic       sc_mbi_rme;
  } esr_shire_cache_ram_cfg4_t;

  typedef struct packed {
    logic [IcachePerShire-1:0]     en_mask;
    logic [minion_pkg::VaSize-1:6] start_addr;
    logic [5:0]                    num_lines;
  } esr_icache_prefetch_t;

  typedef logic [BarrierRegisters-1:0][BarrierValueWidth-1:0] uc_esr_barrier_values_t;

  typedef struct packed {
    logic [BarrierRegisters-1:0] barriers;
    logic [UcFccRegisters-1:0]   fcc;
  } uc_esr_enable_sigs_t;

  typedef struct packed {
    uc_esr_barrier_values_t barriers;
  } uc_esr_values_t;

  typedef struct packed {
    logic reset_ack;
    logic resume_ack;
    logic halt_ack;
  } bpam_rc_rbox_ack_t;

  typedef struct packed {
    logic       esr_sc_err_rsp_enable;
    logic [7:0] esr_sc_err_interrupt_enable;
  } esr_sc_err_log_ctl_t;

  typedef struct packed {
    logic       esr_sc_two_shire_aliasing_use_shire_lsb;
    logic       esr_sc_all_shire_aliasing;
    logic [3:0] esr_sc_sub_bank_sel_b2;
    logic [3:0] esr_sc_sub_bank_sel_b1;
    logic [3:0] esr_sc_sub_bank_sel_b0;
    logic [3:0] esr_sc_bank_sel_b2;
    logic [3:0] esr_sc_bank_sel_b1;
    logic [3:0] esr_sc_bank_sel_b0;
    logic [3:0] esr_sc_shire_sel_b5;
    logic [3:0] esr_sc_shire_sel_b4;
    logic [3:0] esr_sc_shire_sel_b3;
    logic [3:0] esr_sc_shire_sel_b2;
    logic [3:0] esr_sc_shire_sel_b1;
    logic [3:0] esr_sc_shire_sel_b0;
  } esr_sc_l3_shire_swizzle_ctl_t;

  typedef struct packed {
    logic [3:0]  set_base_rsvd;
    logic [11:0] set_base;
    logic [2:0]  set_size_rsvd;
    logic [12:0] set_size;
    logic [3:0]  set_mask_rsvd;
    logic [11:0] set_mask;
    logic [3:0]  tag_mask_rsvd;
    logic [11:0] tag_mask;
  } esr_sc_pipe_cache_ctl_t;

  typedef struct packed {
    logic        esr_sc_idx_cop_sm_ctl_user_en;
    logic        esr_sc_ram_deep_sleep;
    logic        esr_sc_ram_shut_down;
    logic [2:0]  esr_sc_ram_delay;
    logic        esr_sc_l2_rbuf_enable;
    logic        esr_sc_scp_rbuf_enable;
    logic        esr_sc_zero_state_enable;
    logic        esr_sc_allow_only_1_req_per_sub_bank;
    logic        esr_sc_allow_only_1_req_per_bank;
    logic [31:0] esr_sc_cbuf_entry_enable;
  } esr_sc_pipe_ctl_t;

  typedef struct packed {
    logic [7:0] esr_sc_clk_gate_disable;
    logic [3:0] esr_sc_axi_qos;
    logic       esr_sc_cbuf_enable;
    logic       esr_sc_remote_l3_enable;
    logic       esr_sc_remote_scp_enable;
    logic       esr_sc_l2_bypass;
    logic       esr_sc_l3_bypass;
    logic [5:0] esr_sc_num_l3_reqq_entries;
    logic       esr_sc_reqq_no_link_list;
    logic       esr_sc_ecc_scrub_enable;
    logic [4:0] esr_sc_l3_yield_priority;
  } esr_sc_reqq_ctl_t;

  typedef struct packed {
    logic       esr_sc_trace_filter_reqq_id_enable;
    logic [5:0] esr_sc_trace_filter_reqq_id;
    logic       esr_sc_trace_filter_port_enable;
    logic [2:0] esr_sc_trace_filter_port;
    logic       esr_sc_trace_filter_source_enable;
    logic [9:0] esr_sc_trace_filter_source;
    logic       esr_sc_trace_filter_l2_enable;
    logic       esr_sc_trace_filter_l3_enable;
    logic       esr_sc_trace_filter_fsm_enable;
    logic [11:0] esr_sc_trace_type_hot_enable;
  } esr_sc_trace_ctl_t;

  /* verilator lint_on UNUSEDPARAM */
endpackage : shire_esr_pkg
