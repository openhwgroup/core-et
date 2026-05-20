// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off SYNCASYNCNET */  // Reset-manager logic intentionally uses async-assert/sync-deassert reset chains.
/* verilator lint_off UNUSEDSIGNAL */  // Integration wrapper preserves wide native seams; some status payloads are consumed by the final top only.
/* verilator lint_off UNOPTFLAT */     // Packed fanout structs are assigned fieldwise; paths are acyclic.
/* verilator lint_off VARHIDDEN */     // Parameters intentionally shadow package defaults for integration sizing.
module shire_channel_wrap
  import axi_pkg::*;
  import dft_pkg::*;
  import etlink_pkg::*;
  import icache_pkg::*;
  import minion_pkg::*;
  import neigh_ch_apb_mux_pkg::*;
  import ram_cfg_pkg::*;
  import shirecache_pkg::*;
  import shire_channel_leaves_pkg::*;
  import shire_esr_pkg::*;
  import shire_sbm_pkg::*;
  import shire_uncached_pkg::*;
#(
  parameter int unsigned NumNeigh              = shire_esr_pkg::NumNeigh,
  parameter int unsigned NumRbox               = shirecache_pkg::RboxPerShire,
  parameter int unsigned NumBanks              = shirecache_pkg::Banks,
  parameter int unsigned NumUc                 = 1,
  parameter int unsigned NumPorts              = NumNeigh + NumRbox,
  parameter int unsigned NumL3MasterPorts      = shirecache_pkg::L3MasterPorts,
  parameter int unsigned NumL3SlavePorts       = shirecache_pkg::L3SlavePorts,
  parameter int unsigned NumSysPorts           = shirecache_pkg::SysPorts,
  parameter int unsigned ThreadsPerNeigh       = shire_esr_pkg::ThreadsPerShire / NumNeigh,
  parameter int unsigned NumMinionsPerShire    = NumNeigh * shire_esr_pkg::MinPerNeigh,
  parameter int unsigned NumGlobPwrDomains     = NumNeigh,
  parameter int unsigned NumBankReqqEntries    = shirecache_pkg::ReqqDepth,
  parameter int unsigned SetsPerSubBankParam   = shirecache_pkg::SetsPerSubBank,
  parameter bit          UseNocClkGating       = 1'b1,
  parameter bit          IcacheMemsImplemented = 1'b1,
  parameter bit          L2hpfImplemented      = 1'b0,
  parameter int unsigned ChannelApbSlaves      = NumBanks + NumRbox +
                                                 (IcacheMemsImplemented ? 1 : 0) + 1,
  parameter int unsigned SbmApbSlaves          = shire_sbm_pkg::ShireApbInterfaces,
  parameter int unsigned DebugResetPulseCycles = 32
) (
  input  logic                                                clk_ctrl_i,
  input  logic                                                clk_ref_i,
  input  logic                                                clk_step_i,
  input  logic [3:0]                                          clk_pll_i,
  input  logic                                                clk_dll_i,
  input  logic                                                clk_noc_i,

  input  logic                                                rst_cold_ni,
  input  logic                                                rst_warm_ni,
  input  logic                                                rst_system_ni,
  input  logic                                                rst_system_debug_ni,
  input  logic                                                rst_noc_ni,
  output logic                                                rst_system_lv_no,
  output logic                                                rst_system_debug_lv_no,
  output logic                                                rst_noc_lv_no,

  output logic [NumNeigh-1:0]                                clk_neigh_o,
  output logic [NumNeigh-1:0]                                clk_shire_to_neigh_o,
  output logic                                                clk_shire_o,
  output logic                                                clk_shire_debug_o,

  output logic [NumNeigh-1:0]                                rst_c_shire_no,
  output logic [NumNeigh-1:0]                                rst_d_shire_no,
  output logic [NumNeigh-1:0]                                rst_w_shire_no,
  output logic [NumNeigh-1:0]                                rst_c_shire_scs_no,
  output logic [NumNeigh-1:0]                                rst_warm_to_neigh_no,
  input  logic [NumNeigh-1:0]                                rst_w_icache_ni,
  output logic                                                rst_sc_no,
  output logic                                                rst_rbox_no,

  input  dft_pkg::dft_t                                       dft_hv_i,
  input  dft_pkg::dft_t                                       dft_lv_i,
  input  logic                                                dft_sram_clk_i,
  input  logic                                                dft_mbist_en_i,

  input  esr_pkg::esr_ms_dmctrl_t                             dmctrl_i,
  output esr_pkg::esr_ms_dmctrl_t                             dmctrl_to_neigh_o [NumNeigh-1:0],
  input  shire_channel_leaves_pkg::bpam_run_control_t         bpam_run_control_i,
  output neigh_voltage_cross_pkg::bpam_run_control_neigh_t    bpam_run_control_neigh_o [NumNeigh-1:0],

  input  logic [shire_esr_pkg::NumShireIdsBits-1:0]           shire_phy_id_i,
  input  logic [shire_esr_pkg::NumShireIdsBits-1:0]           shire_id_reset_val_i,
  output logic [NumNeigh-1:0][shire_esr_pkg::NumShireIdsBits-1:0] shire_id_o,
  output logic [shire_esr_pkg::TboxPerShire-1:0][1:0]         tbox_id_o,
  output logic [shire_esr_pkg::TboxPerShire-1:0]              tbox_en_o,

  input  logic                                                plic_mtip_i,
  input  logic                                                plic_meip_i,
  input  logic                                                plic_seip_i,
  output logic [NumNeigh-1:0][shire_esr_pkg::MinPerNeigh-1:0] int_mtip_to_neigh_o,
  output logic [NumNeigh-1:0]                                plic_meip_to_neigh_o,
  output logic [NumNeigh-1:0]                                plic_seip_to_neigh_o,

  output logic [NumNeigh-1:0][shire_esr_pkg::MinPerNeigh-1:0] esr_thread0_enable_o,
  output logic [NumNeigh-1:0][shire_esr_pkg::MinPerNeigh-1:0] esr_thread1_enable_o,
  output logic [NumNeigh-1:0][ThreadsPerNeigh-1:0] esr_neigh_ipi_msip_o,
  output logic [NumNeigh-1:0][ThreadsPerNeigh-1:0] esr_neigh_ipi_redirect_trigger_o,
  output minion_pkg::esr_minion_features_t                    esr_minion_features_o [NumNeigh-1:0],
  output icache_pkg::icache_prefetch_conf_t                   esr_icache_prefetch_conf_o [NumNeigh-1:0],
  output logic [shire_esr_pkg::IcachePerShire-1:0]            esr_icache_prefetch_start_o,
  input  logic [shire_esr_pkg::IcachePerShire-1:0]            esr_icache_prefetch_done_i,
  input  logic [NumNeigh-1:0]                                esr_neigh_icache_err_detected_i,
  input  logic [NumNeigh-1:0]                                esr_neigh_icache_err_logged_i,
  output logic [NumNeigh-1:0]                                esr_shire_coop_mode_o,

  input  esr_pkg::esr_and_or_tree_l0_t                       esr_and_or_tree_l0_i [NumNeigh-1:0],
  input  shire_channel_leaves_pkg::esr_and_or_tree_l2_t       debug_and_or_tree_l2_i,
  output shire_channel_leaves_pkg::esr_and_or_tree_l2_t       debug_and_or_tree_l2_o,

  output neigh_voltage_cross_pkg::minion_chicken_bits_vc_t    esr_neigh_clk_gate_ctrl_o [NumNeigh-1:0],
  output logic [NumNeigh-1:0][minion_pkg::ShireMinionMemOverrideBits-1:0] esr_minion_mem_override_o,

  output logic [NumNeigh-1:0]                                esr_pwr_ctrl_glb_nsleepin_o,
  output logic [NumNeigh-1:0]                                esr_pwr_ctrl_glb_iso_o,
  input  logic [NumNeigh-1:0]                                esr_pwr_ctrl_glb_nsleepout_i,
  output logic [NumNeigh-1:0][shire_esr_pkg::MinPerNeigh-1:0] esr_pwr_ctrl_neigh_nsleepin_o,
  output logic [NumNeigh-1:0][shire_esr_pkg::MinPerNeigh-1:0] esr_pwr_ctrl_neigh_iso_o,
  input  logic [NumNeigh-1:0][shire_esr_pkg::MinPerNeigh-1:0] esr_pwr_ctrl_neigh_nsleepout_i,

  output esr_pkg::esr_dll_dly_est_ctl_t                      esr_dll_dly_est_ctl_o [NumNeigh-1:0],
  input  esr_pkg::esr_dll_dly_est_sts_t                      esr_dll_dly_est_sts_i [NumNeigh-1:0],
  input  logic [shirecache_pkg::BankIdSize-1:0]              status_monitor_bank_sel_i,

  output neigh_ch_apb_mux_pkg::apb_to_neigh_t                apb_esr_req_neigh_o [NumNeigh-1:0],
  input  neigh_ch_apb_mux_pkg::apb_from_neigh_t              apb_esr_rsp_neigh_i [NumNeigh-1:0],

  output logic [NumNeigh-1:0][NumBanks+NumUc-1:0]             neigh_sc_req_ready_o,
  input  logic [NumNeigh-1:0][NumBanks+NumUc-1:0]             neigh_sc_req_valid_i,
  input  etlink_pkg::req_t [NumNeigh-1:0]                     neigh_sc_req_info_i,
  input  logic [NumNeigh-1:0]                                neigh_sc_rsp_ready_i,
  output logic [NumNeigh-1:0]                                neigh_sc_rsp_valid_o,
  output etlink_pkg::rsp_t [NumNeigh-1:0]                     neigh_sc_rsp_info_o,

  output logic [NumNeigh-1:0][shire_uncached_pkg::MinPerNeigh-1:0] uc_to_neigh_fcc_o,
  output logic [NumNeigh-1:0][shire_uncached_pkg::UcFccTargetSize-1:0] uc_to_neigh_fcc_target_o,
  input  logic [NumNeigh-1:0]                                flb_neigh_l2_req_valid_i,
  input  logic [NumNeigh-1:0][shire_uncached_pkg::CsrFlBarrierSize-1:0] flb_neigh_l2_req_data_i,
  output logic [NumNeigh-1:0]                                flb_l2_neigh_resp_valid_o,
  output logic [NumNeigh-1:0]                                flb_l2_neigh_resp_data_o,

  input  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t               bpam_rc_tbox_ack_i [NumNeigh-1:0],

  output logic                                                ioshire_combined_err_int_o,
  output logic                                                ioshire_noc_err_int_o,
  input  logic [shire_channel_leaves_pkg::NocIntNum-1:0]      noc_err_int_srcs_i,
  output logic [shire_channel_leaves_pkg::NocIntNum-1:0]      noc_all_err_int_srcs_o,

  output axi_pkg::sc_master_ar_t [NumL3MasterPorts-1:0]       to_l3_axi_ar_o,
  output logic [NumL3MasterPorts-1:0]                         to_l3_axi_ar_valid_o,
  input  logic [NumL3MasterPorts-1:0]                         to_l3_axi_ar_ready_i,
  output axi_pkg::sc_master_aw_t [NumL3MasterPorts-1:0]       to_l3_axi_aw_o,
  output logic [NumL3MasterPorts-1:0]                         to_l3_axi_aw_valid_o,
  input  logic [NumL3MasterPorts-1:0]                         to_l3_axi_aw_ready_i,
  output axi_pkg::sc_master_w_t [NumL3MasterPorts-1:0]        to_l3_axi_w_o,
  output logic [NumL3MasterPorts-1:0]                         to_l3_axi_w_valid_o,
  input  logic [NumL3MasterPorts-1:0]                         to_l3_axi_w_ready_i,
  input  axi_pkg::sc_master_b_t [NumL3MasterPorts-1:0]        to_l3_axi_b_i,
  input  logic [NumL3MasterPorts-1:0]                         to_l3_axi_b_valid_i,
  output logic [NumL3MasterPorts-1:0]                         to_l3_axi_b_ready_o,
  input  axi_pkg::sc_master_r_t [NumL3MasterPorts-1:0]        to_l3_axi_r_i,
  input  logic [NumL3MasterPorts-1:0]                         to_l3_axi_r_valid_i,
  output logic [NumL3MasterPorts-1:0]                         to_l3_axi_r_ready_o,

  output axi_pkg::sc_master_ar_t [NumSysPorts-1:0]            to_sys_axi_ar_o,
  output logic [NumSysPorts-1:0]                              to_sys_axi_ar_valid_o,
  input  logic [NumSysPorts-1:0]                              to_sys_axi_ar_ready_i,
  output axi_pkg::sc_master_aw_t [NumSysPorts-1:0]            to_sys_axi_aw_o,
  output logic [NumSysPorts-1:0]                              to_sys_axi_aw_valid_o,
  input  logic [NumSysPorts-1:0]                              to_sys_axi_aw_ready_i,
  output axi_pkg::sc_master_w_t [NumSysPorts-1:0]             to_sys_axi_w_o,
  output logic [NumSysPorts-1:0]                              to_sys_axi_w_valid_o,
  input  logic [NumSysPorts-1:0]                              to_sys_axi_w_ready_i,
  input  axi_pkg::sc_master_b_t [NumSysPorts-1:0]             to_sys_axi_b_i,
  input  logic [NumSysPorts-1:0]                              to_sys_axi_b_valid_i,
  output logic [NumSysPorts-1:0]                              to_sys_axi_b_ready_o,
  input  axi_pkg::sc_master_r_t [NumSysPorts-1:0]             to_sys_axi_r_i,
  input  logic [NumSysPorts-1:0]                              to_sys_axi_r_valid_i,
  output logic [NumSysPorts-1:0]                              to_sys_axi_r_ready_o,

  input  axi_pkg::sc_slave_ar_t [NumL3SlavePorts-1:0]         l3_axi_ar_i,
  input  logic [NumL3SlavePorts-1:0]                         l3_axi_ar_valid_i,
  output logic [NumL3SlavePorts-1:0]                         l3_axi_ar_ready_o,
  input  axi_pkg::sc_slave_aw_t [NumL3SlavePorts-1:0]         l3_axi_aw_i,
  input  logic [NumL3SlavePorts-1:0]                         l3_axi_aw_valid_i,
  output logic [NumL3SlavePorts-1:0]                         l3_axi_aw_ready_o,
  input  axi_pkg::sc_master_w_t [NumL3SlavePorts-1:0]         l3_axi_w_i,
  input  logic [NumL3SlavePorts-1:0]                         l3_axi_w_valid_i,
  output logic [NumL3SlavePorts-1:0]                         l3_axi_w_ready_o,
  output axi_pkg::sc_slave_b_t [NumL3SlavePorts-1:0]          l3_axi_b_o,
  output logic [NumL3SlavePorts-1:0]                         l3_axi_b_valid_o,
  input  logic [NumL3SlavePorts-1:0]                         l3_axi_b_ready_i,
  output axi_pkg::sc_slave_r_t [NumL3SlavePorts-1:0]          l3_axi_r_o,
  output logic [NumL3SlavePorts-1:0]                         l3_axi_r_valid_o,
  input  logic [NumL3SlavePorts-1:0]                         l3_axi_r_ready_i,

  output axi_pkg::sc_master_ar_t                              uc_to_l3_axi_ar_o,
  output logic                                                uc_to_l3_axi_ar_valid_o,
  input  logic                                                uc_to_l3_axi_ar_ready_i,
  output axi_pkg::sc_master_aw_t                              uc_to_l3_axi_aw_o,
  output logic                                                uc_to_l3_axi_aw_valid_o,
  input  logic                                                uc_to_l3_axi_aw_ready_i,
  output axi_pkg::sc_master_w_t                               uc_to_l3_axi_w_o,
  output logic                                                uc_to_l3_axi_w_valid_o,
  input  logic                                                uc_to_l3_axi_w_ready_i,
  input  axi_pkg::sc_master_b_t                               uc_to_l3_axi_b_i,
  input  logic                                                uc_to_l3_axi_b_valid_i,
  output logic                                                uc_to_l3_axi_b_ready_o,
  input  axi_pkg::sc_master_r_t                               uc_to_l3_axi_r_i,
  input  logic                                                uc_to_l3_axi_r_valid_i,
  output logic                                                uc_to_l3_axi_r_ready_o,

  output axi_pkg::sc_master_ar_t                              uc_to_sys_axi_ar_o,
  output logic                                                uc_to_sys_axi_ar_valid_o,
  input  logic                                                uc_to_sys_axi_ar_ready_i,
  output axi_pkg::sc_master_aw_t                              uc_to_sys_axi_aw_o,
  output logic                                                uc_to_sys_axi_aw_valid_o,
  input  logic                                                uc_to_sys_axi_aw_ready_i,
  output axi_pkg::sc_master_w_t                               uc_to_sys_axi_w_o,
  output logic                                                uc_to_sys_axi_w_valid_o,
  input  logic                                                uc_to_sys_axi_w_ready_i,
  input  axi_pkg::sc_master_b_t                               uc_to_sys_axi_b_i,
  input  logic                                                uc_to_sys_axi_b_valid_i,
  output logic                                                uc_to_sys_axi_b_ready_o,
  input  axi_pkg::sc_master_r_t                               uc_to_sys_axi_r_i,
  input  logic                                                uc_to_sys_axi_r_valid_i,
  output logic                                                uc_to_sys_axi_r_ready_o,

  input  axi_pkg::sys_slave_ar_t                              sys_axi_ar_i,
  input  logic                                                sys_axi_ar_valid_i,
  output logic                                                sys_axi_ar_ready_o,
  input  axi_pkg::sys_slave_aw_t                              sys_axi_aw_i,
  input  logic                                                sys_axi_aw_valid_i,
  output logic                                                sys_axi_aw_ready_o,
  input  axi_pkg::sys_slave_w_t                               sys_axi_w_i,
  input  logic                                                sys_axi_w_valid_i,
  output logic                                                sys_axi_w_ready_o,
  output axi_pkg::sys_slave_b_t                               sys_axi_b_o,
  output logic                                                sys_axi_b_valid_o,
  input  logic                                                sys_axi_b_ready_i,
  output axi_pkg::sys_slave_r_t                               sys_axi_r_o,
  output logic                                                sys_axi_r_valid_o,
  input  logic                                                sys_axi_r_ready_i,
  input  logic [1:0]                                          sys_axi_aw_vcvalid_i,
  output logic [1:0]                                          sys_axi_aw_credit_o,
  input  logic [1:0]                                          sys_axi_w_vcvalid_i,

  input  logic [NumNeigh-1:0]                                icache_req_write_i,
  input  logic [NumNeigh-1:0][icache_geom_pkg::IcacheSramAddrWidth-1:0] icache_req_addr_i,
  input  logic [NumNeigh-1:0]                                icache_req_valid_i,
  output logic [NumNeigh-1:0]                                icache_req_ready_o,
  output logic [NumNeigh-1:0][icache_geom_pkg::IcacheSramDataWidth-1:0] icache_resp_dout_o,
  output logic [NumNeigh-1:0]                                icache_resp_valid_o,
  input  logic [NumNeigh-1:0]                                icache_resp_ready_i,

  output logic [NumBanks-1:0]                                sc_neigh_l2hpf_req_valid_o,
  output shirecache_pkg::neigh_l2hpf_req_t [NumBanks-1:0]    sc_neigh_l2hpf_req_info_o,
  output shirecache_pkg::trace_packet_t                      sc_trace_data_o,
  output logic                                                sc_trace_valid_o,

  input  neigh_pkg::coop_tload_slv_rdy_req_t                 coop_tload_slv_rdy_out_data_i [NumNeigh-1:0],
  input  logic [NumNeigh-1:0]                                coop_tload_slv_rdy_out_valid_i,
  output neigh_pkg::coop_tload_slv_rdy_req_t                 coop_tload_slv_rdy_in_data_o [NumNeigh-1:0][NumNeigh-2:0],
  output logic [NumNeigh-1:0][NumNeigh-2:0]                  coop_tload_slv_rdy_in_valid_o,
  input  logic [NumNeigh-1:0][neigh_pkg::ShireCoopIdSize-1:0] coop_tload_mst_done_out_coop_id_i,
  input  logic [NumNeigh-1:0][NumNeigh-2:0]                  coop_tload_mst_done_out_valid_i,
  output logic [NumNeigh-1:0][NumNeigh-2:0][neigh_pkg::ShireCoopIdSize-1:0] coop_tload_mst_done_in_coop_id_o,
  output logic [NumNeigh-1:0][NumNeigh-2:0]                  coop_tload_mst_done_in_valid_o,

  output ram_cfg_pkg::ram_cfg_t                               ram_cfg_o,
  output shire_esr_pkg::esr_clk_gate_ctrl_t                   esr_clk_gate_ctrl_o,
  output logic                                                esr_debug_clk_gate_ctrl_o,
  output shire_esr_pkg::esr_pll_auto_config_t                 esr_pll_ctrl_o,
  output logic [shire_esr_pkg::ShirePllConfBits-1:0]          esr_pll_conf_o,
  output shire_esr_pkg::esr_dll_auto_config_t                 esr_dll_ctrl_o,
  output logic [shire_esr_pkg::ShireDllConfBits-1:0]          esr_dll_conf_o,
  output shire_esr_pkg::esr_clkmux_ctl_t                      esr_shire_ctrl_clockmux_o
);

  localparam int unsigned DebugResetCountBits = $clog2(DebugResetPulseCycles);
  /* verilator lint_off WIDTHTRUNC */  // DebugResetLastValue intentionally narrows the bounded pulse-count parameter.
  localparam logic [DebugResetCountBits-1:0] DebugResetLastValue = DebugResetPulseCycles - 1;
  /* verilator lint_on WIDTHTRUNC */
  localparam int unsigned ApbBanksIdxStart = 0;
  localparam int unsigned ApbShireIdx = NumBanks;
  localparam int unsigned ApbRboxIdxStart = ApbShireIdx + 1;
  localparam int unsigned ApbIcacheIdx = ApbRboxIdxStart + NumRbox;

  dft_pkg::dft_t dft_reset_hv;
  dft_pkg::dft_t dft_reset_lv;
  assign dft_reset_hv = dft_hv_i;
  assign dft_reset_lv = dft_lv_i;

  logic [1:0] reset_warm_sync_q;
  logic [1:0] gpio_ndmreset_sync_q;
  logic [1:0] ndmreset_sync_q;
  logic [1:0] dmactive_sync_q;
  logic [1:0] reset_cold_sync_q;
  logic reset_warm_sync;
  logic gpio_ndmreset_sync;
  logic ndmreset_sync;
  logic dmactive_sync;
  logic reset_cold_sync;

  esr_pkg::esr_ms_dmctrl_t dmctrl_sync;

  logic reset_warm_async_ni;
  logic gpio_ndmreset_async_ni;
  logic ndmreset_async_ni;
  logic dmactive_async_ni;
  logic reset_cold_async_ni;

  assign reset_warm_async_ni = dft_reset_hv.scanmode ? dft_reset_hv.scan_reset_n : rst_warm_ni;
  assign gpio_ndmreset_async_ni = dft_reset_hv.scanmode ? dft_reset_hv.scan_reset_n :
                                  ~bpam_run_control_i.gpio.ndmreset;
  assign ndmreset_async_ni = dft_reset_hv.scanmode ? dft_reset_hv.scan_reset_n :
                             ~dmctrl_sync.ndmreset;
  assign dmactive_async_ni = dft_reset_hv.scanmode ? dft_reset_hv.scan_reset_n :
                             ~dmctrl_sync.dmactive;
  assign reset_cold_async_ni = dft_reset_hv.scanmode ? dft_reset_hv.scan_reset_n : rst_cold_ni;

  always_ff @(posedge clk_shire_o or negedge reset_warm_async_ni) begin
    if (!reset_warm_async_ni) reset_warm_sync_q <= '1;
    else reset_warm_sync_q <= {reset_warm_sync_q[0], 1'b0};
  end
  always_ff @(posedge clk_shire_o or negedge gpio_ndmreset_async_ni) begin
    if (!gpio_ndmreset_async_ni) gpio_ndmreset_sync_q <= '1;
    else gpio_ndmreset_sync_q <= {gpio_ndmreset_sync_q[0], 1'b0};
  end
  always_ff @(posedge clk_shire_o or negedge ndmreset_async_ni) begin
    if (!ndmreset_async_ni) ndmreset_sync_q <= '1;
    else ndmreset_sync_q <= {ndmreset_sync_q[0], 1'b0};
  end
  always_ff @(posedge clk_shire_o or negedge dmactive_async_ni) begin
    if (!dmactive_async_ni) dmactive_sync_q <= '1;
    else dmactive_sync_q <= {dmactive_sync_q[0], 1'b0};
  end
  always_ff @(posedge clk_shire_o or negedge reset_cold_async_ni) begin
    if (!reset_cold_async_ni) reset_cold_sync_q <= '1;
    else reset_cold_sync_q <= {reset_cold_sync_q[0], 1'b0};
  end

  assign reset_warm_sync = dft_reset_hv.scanmode ? ~dft_reset_hv.scan_reset_n :
                           reset_warm_sync_q[1];
  assign gpio_ndmreset_sync = dft_reset_hv.scanmode ? ~dft_reset_hv.scan_reset_n :
                              gpio_ndmreset_sync_q[1];
  assign ndmreset_sync = dft_reset_hv.scanmode ? ~dft_reset_hv.scan_reset_n :
                         ndmreset_sync_q[1];
  assign dmactive_sync = dft_reset_hv.scanmode ? ~dft_reset_hv.scan_reset_n :
                         dmactive_sync_q[1];
  assign reset_cold_sync = dft_reset_hv.scanmode ? ~dft_reset_hv.scan_reset_n :
                           reset_cold_sync_q[1];

  logic [DebugResetCountBits-1:0] dmreset_pulse_counter_q;
  logic dm_reset_pulse_q;
  logic debug_pulse_rst_ni;
  logic dm_reset_pulse;

  assign debug_pulse_rst_ni = ~(dmactive_sync | reset_cold_sync);

  always_ff @(posedge clk_shire_o or negedge debug_pulse_rst_ni) begin
    if (!debug_pulse_rst_ni) begin
      dmreset_pulse_counter_q <= '0;
    end else if (dmreset_pulse_counter_q < DebugResetLastValue) begin
      dmreset_pulse_counter_q <= dmreset_pulse_counter_q + 1'b1;
    end
  end

  always_ff @(posedge clk_shire_o or negedge debug_pulse_rst_ni) begin
    if (!debug_pulse_rst_ni) begin
      dm_reset_pulse_q <= 1'b1;
    end else if (dmreset_pulse_counter_q >= DebugResetLastValue) begin
      dm_reset_pulse_q <= 1'b0;
    end
  end

  assign dm_reset_pulse = ~dmactive_sync & dm_reset_pulse_q;

  logic rst_c_ni;
  logic rst_w_ni;
  logic rst_d_ni;
  assign rst_c_ni = dft_reset_hv.scanmode ? dft_reset_hv.scan_reset_n : ~reset_cold_sync;
  assign rst_w_ni = dft_reset_hv.scanmode ? dft_reset_hv.scan_reset_n :
                    ~(reset_cold_sync | reset_warm_sync | gpio_ndmreset_sync |
                      (ndmreset_sync & dmactive_sync));
  assign rst_d_ni = dft_reset_hv.scanmode ? dft_reset_hv.scan_reset_n :
                    ~(reset_cold_sync | dm_reset_pulse);

  logic [1:0] rst_system_sync_q;
  logic [1:0] rst_system_debug_sync_q;
  logic [1:0] rst_noc_sync_q;
  logic rst_system_sync_int_ni;
  logic rst_system_debug_sync_int_ni;
  logic rst_noc_sync_int_ni;
  logic rst_system_sync_ni;
  logic rst_system_debug_sync_ni;
  logic rst_noc_sync_ni;

  assign rst_system_sync_int_ni = dft_reset_hv.scanmode ? dft_reset_hv.scan_reset_n :
                                  rst_system_ni;
  assign rst_system_debug_sync_int_ni = dft_reset_hv.scanmode ? dft_reset_hv.scan_reset_n :
                                        rst_system_debug_ni;
  assign rst_noc_sync_int_ni = dft_reset_hv.scanmode ? dft_reset_hv.scan_reset_n :
                               rst_noc_ni;

  always_ff @(negedge clk_noc_i or negedge rst_system_sync_int_ni) begin
    if (!rst_system_sync_int_ni) rst_system_sync_q <= '0;
    else rst_system_sync_q <= {rst_system_sync_q[0], 1'b1};
  end
  always_ff @(negedge clk_noc_i or negedge rst_system_debug_sync_int_ni) begin
    if (!rst_system_debug_sync_int_ni) rst_system_debug_sync_q <= '0;
    else rst_system_debug_sync_q <= {rst_system_debug_sync_q[0], 1'b1};
  end
  always_ff @(negedge clk_noc_i or negedge rst_noc_sync_int_ni) begin
    if (!rst_noc_sync_int_ni) rst_noc_sync_q <= '0;
    else rst_noc_sync_q <= {rst_noc_sync_q[0], 1'b1};
  end

  assign rst_system_sync_ni = dft_reset_hv.scanmode ? dft_reset_hv.scan_reset_n :
                              rst_system_sync_q[1];
  assign rst_system_debug_sync_ni = dft_reset_hv.scanmode ? dft_reset_hv.scan_reset_n :
                                    rst_system_debug_sync_q[1];
  assign rst_noc_sync_ni = dft_reset_hv.scanmode ? dft_reset_hv.scan_reset_n :
                           rst_noc_sync_q[1];

  prim_rst_sync u_rst_system_lv (
    .clk_i    (clk_noc_i),
    .rst_ni   (rst_system_sync_ni),
    .dft_i    (dft_reset_lv),
    .rst_no   (rst_system_lv_no)
  );

  prim_rst_sync u_rst_system_debug_lv (
    .clk_i    (clk_noc_i),
    .rst_ni   (rst_system_debug_sync_ni),
    .dft_i    (dft_reset_lv),
    .rst_no   (rst_system_debug_lv_no)
  );

  prim_rst_sync u_rst_noc_lv (
    .clk_i    (clk_noc_i),
    .rst_ni   (rst_noc_sync_ni),
    .dft_i    (dft_reset_lv),
    .rst_no   (rst_noc_lv_no)
  );

  shire_esr_pkg::esr_clk_gate_ctrl_t channel_clk_gate_ctrl;
  logic channel_debug_clk_gate_ctrl;
  shire_esr_pkg::esr_pll_auto_config_t channel_pll_ctrl;
  logic [shire_esr_pkg::ShirePllConfBits-1:0] channel_pll_conf;
  shire_esr_pkg::esr_dll_auto_config_t channel_dll_ctrl;
  logic [shire_esr_pkg::ShireDllConfBits-1:0] channel_dll_conf;
  shire_esr_pkg::esr_clkmux_ctl_t channel_clockmux;
  shire_esr_pkg::esr_clk_dly_ctl_t channel_clk_dly_ctl_unused;
  logic channel_pll_lock_detect;
  logic channel_dll_lock_detect;
  logic channel_pll_busy;
  logic channel_pll_rrdy;
  logic [shire_esr_pkg::ShirePllDataBits-1:0] channel_pll_rdata;
  logic channel_dll_busy;
  logic channel_dll_rrdy;
  logic [shire_esr_pkg::ShireDllDataBits-1:0] channel_dll_rdata;
  logic [shire_esr_pkg::ThreadsPerShire-1:0] channel_mtime_local_target;
  logic [shire_esr_pkg::ThreadsPerShire-1:0] channel_thread0_enable;
  logic [shire_esr_pkg::ThreadsPerShire-1:0] channel_thread1_enable;
  logic [shire_esr_pkg::ThreadsPerShire-1:0] channel_ipi_msip;
  logic [shire_esr_pkg::ThreadsPerShire-1:0] channel_ipi_redirect_trigger;
  minion_pkg::esr_minion_features_t channel_minion_features [NumNeigh-1:0];
  icache_pkg::icache_prefetch_conf_t channel_icache_prefetch_conf [NumNeigh-1:0];
  logic [NumNeigh-1:0] channel_shire_coop_mode;
  logic [NumMinionsPerShire-1:0] channel_pwr_ctrl_nsleepin;
  logic [NumMinionsPerShire-1:0] channel_pwr_ctrl_iso;
  logic [NumMinionsPerShire-1:0] channel_pwr_ctrl_nsleepout;
  logic [NumGlobPwrDomains-1:0] channel_glb_nsleepin;
  logic [NumGlobPwrDomains-1:0] channel_glb_iso;
  logic [NumGlobPwrDomains-1:0] channel_glb_nsleepout;
  esr_pkg::esr_dll_dly_est_ctl_t channel_dll_dly_est_ctl;
  esr_pkg::esr_dll_dly_est_sts_t channel_dll_dly_est_sts [NumNeigh-1:0];

  axi_pkg::sys_slave_ar_t sbm_sys_axi_ar;
  axi_pkg::sys_slave_aw_t sbm_sys_axi_aw;
  axi_pkg::sys_slave_w_t sbm_sys_axi_w;
  axi_pkg::sys_slave_b_t sbm_sys_axi_b;
  axi_pkg::sys_slave_r_t sbm_sys_axi_r;
  logic sbm_enable_read;
  logic sbm_enable_write;
  logic sbm_write_credit_return;
  logic sbm_sys_axi_ar_ready;
  logic sbm_sys_axi_aw_ready;
  logic sbm_sys_axi_w_ready;
  logic sbm_sys_axi_b_valid;
  logic sbm_sys_axi_b_ready;
  logic sbm_sys_axi_r_valid;
  logic sbm_sys_axi_r_ready;

  shire_sbm_pkg::shire_apb_req_t [SbmApbSlaves-1:0] sbm_apb_req;
  shire_sbm_pkg::shire_apb_rsp_t [SbmApbSlaves-1:0] sbm_apb_rsp;
  logic [shire_sbm_pkg::AxiIdWidth-1:0] shire_virtual_id;
  assign shire_virtual_id = {{(shire_sbm_pkg::AxiIdWidth - shire_esr_pkg::NumShireIdsBits){1'b0}},
                             shire_id_o[0]};
  shire_sbm_pkg::shire_apb_req_t [ChannelApbSlaves-1:0] channel_apb_req;
  shire_sbm_pkg::shire_apb_rsp_t [ChannelApbSlaves-1:0] channel_apb_rsp;

  always_comb begin
    channel_pwr_ctrl_nsleepout = '0;
    channel_glb_nsleepout = '0;
    for (int unsigned neigh = 0; neigh < NumNeigh; neigh++) begin
      channel_glb_nsleepout[neigh] = esr_pwr_ctrl_glb_nsleepout_i[neigh];
      for (int unsigned minion = 0; minion < shire_esr_pkg::MinPerNeigh; minion++) begin
        channel_pwr_ctrl_nsleepout[neigh * shire_esr_pkg::MinPerNeigh + minion] =
            esr_pwr_ctrl_neigh_nsleepout_i[neigh][minion];
      end
    end
  end

  always_comb begin
    for (int unsigned neigh = 0; neigh < NumNeigh; neigh++) begin
      channel_dll_dly_est_sts[neigh] = esr_dll_dly_est_sts_i[neigh];
    end
  end

  always_comb begin
    for (int unsigned neigh = 0; neigh < NumNeigh; neigh++) begin
      clk_shire_to_neigh_o[neigh] = clk_shire_o;
      rst_warm_to_neigh_no[neigh] = rst_warm_ni;
      dmctrl_to_neigh_o[neigh] = dmctrl_sync;
      plic_meip_to_neigh_o[neigh] = plic_meip_i;
      plic_seip_to_neigh_o[neigh] = plic_seip_i;
      esr_minion_features_o[neigh] = channel_minion_features[neigh];
      esr_icache_prefetch_conf_o[neigh] = channel_icache_prefetch_conf[neigh];
      esr_shire_coop_mode_o[neigh] = channel_shire_coop_mode[neigh];
      esr_dll_dly_est_ctl_o[neigh] = channel_dll_dly_est_ctl;
      esr_minion_mem_override_o[neigh] = '0;
      esr_neigh_clk_gate_ctrl_o[neigh].min_frontend_clock_gate_disable =
          channel_clk_gate_ctrl.min_frontend_clock_gate_disable;
      esr_neigh_clk_gate_ctrl_o[neigh].min_dcache_clock_gate_disable =
          channel_clk_gate_ctrl.min_dcache_clock_gate_disable;
      esr_neigh_clk_gate_ctrl_o[neigh].min_vputrans_clock_gate_disable =
          channel_clk_gate_ctrl.min_vputrans_clock_gate_disable;
      esr_neigh_clk_gate_ctrl_o[neigh].min_vputima_clock_gate_disable =
          channel_clk_gate_ctrl.min_vputima_clock_gate_disable;
      esr_neigh_clk_gate_ctrl_o[neigh].min_vpulane_clock_gate_disable =
          channel_clk_gate_ctrl.min_vpulane_clock_gate_disable;
      esr_neigh_clk_gate_ctrl_o[neigh].min_intpipe_clock_gate_disable =
          channel_clk_gate_ctrl.min_intpipe_clock_gate_disable;
      for (int unsigned minion = 0; minion < shire_esr_pkg::MinPerNeigh; minion++) begin
        int unsigned flat_idx;
        flat_idx = neigh * shire_esr_pkg::MinPerNeigh + minion;
        int_mtip_to_neigh_o[neigh][minion] = plic_mtip_i & channel_mtime_local_target[flat_idx];
        esr_thread0_enable_o[neigh][minion] = channel_thread0_enable[flat_idx];
        esr_thread1_enable_o[neigh][minion] = channel_thread1_enable[flat_idx];
        esr_pwr_ctrl_neigh_nsleepin_o[neigh][minion] = channel_pwr_ctrl_nsleepin[flat_idx] |
                                                       ~rst_c_ni;
        esr_pwr_ctrl_neigh_iso_o[neigh][minion] = channel_pwr_ctrl_iso[flat_idx] & rst_c_ni;
      end
      for (int unsigned thread = 0; thread < ThreadsPerNeigh; thread++) begin
        int unsigned thread_idx;
        thread_idx = neigh * ThreadsPerNeigh + thread;
        esr_neigh_ipi_msip_o[neigh][thread] = channel_ipi_msip[thread_idx];
        esr_neigh_ipi_redirect_trigger_o[neigh][thread] = channel_ipi_redirect_trigger[thread_idx];
      end
    end
    for (int unsigned glb = 0; glb < NumNeigh; glb++) begin
      esr_pwr_ctrl_glb_nsleepin_o[glb] = channel_glb_nsleepin[glb] | ~rst_c_ni;
      esr_pwr_ctrl_glb_iso_o[glb] = channel_glb_iso[glb] & rst_c_ni;
    end
  end

  always_comb begin
    for (int unsigned idx = 0; idx < ChannelApbSlaves; idx++) begin
      channel_apb_req[idx] = '0;
    end
    for (int unsigned idx = 0; idx < SbmApbSlaves; idx++) begin
      sbm_apb_rsp[idx] = '0;
    end
    for (int unsigned bank = 0; bank < NumBanks; bank++) begin
      channel_apb_req[ApbBanksIdxStart + bank].paddr = sbm_apb_req[shire_sbm_pkg::BankRangeLo + bank].paddr[shire_sbm_pkg::ShireApbAddrWidth-1:0];
      channel_apb_req[ApbBanksIdxStart + bank].penable = sbm_apb_req[shire_sbm_pkg::BankRangeLo + bank].penable;
      channel_apb_req[ApbBanksIdxStart + bank].psel = sbm_apb_req[shire_sbm_pkg::BankRangeLo + bank].psel;
      channel_apb_req[ApbBanksIdxStart + bank].pwdata = sbm_apb_req[shire_sbm_pkg::BankRangeLo + bank].pwdata;
      channel_apb_req[ApbBanksIdxStart + bank].pwrite = sbm_apb_req[shire_sbm_pkg::BankRangeLo + bank].pwrite;
      sbm_apb_rsp[shire_sbm_pkg::BankRangeLo + bank].prdata = channel_apb_rsp[ApbBanksIdxStart + bank].prdata;
      sbm_apb_rsp[shire_sbm_pkg::BankRangeLo + bank].pready = channel_apb_rsp[ApbBanksIdxStart + bank].pready;
      sbm_apb_rsp[shire_sbm_pkg::BankRangeLo + bank].pslverr = channel_apb_rsp[ApbBanksIdxStart + bank].pslverr;
    end
    channel_apb_req[ApbShireIdx].paddr = sbm_apb_req[shire_sbm_pkg::ShireRangeLo].paddr[shire_sbm_pkg::ShireApbAddrWidth-1:0];
    channel_apb_req[ApbShireIdx].penable = sbm_apb_req[shire_sbm_pkg::ShireRangeLo].penable;
    channel_apb_req[ApbShireIdx].psel = sbm_apb_req[shire_sbm_pkg::ShireRangeLo].psel;
    channel_apb_req[ApbShireIdx].pwdata = sbm_apb_req[shire_sbm_pkg::ShireRangeLo].pwdata;
    channel_apb_req[ApbShireIdx].pwrite = sbm_apb_req[shire_sbm_pkg::ShireRangeLo].pwrite;
    sbm_apb_rsp[shire_sbm_pkg::ShireRangeLo].prdata = channel_apb_rsp[ApbShireIdx].prdata;
    sbm_apb_rsp[shire_sbm_pkg::ShireRangeLo].pready = channel_apb_rsp[ApbShireIdx].pready;
    sbm_apb_rsp[shire_sbm_pkg::ShireRangeLo].pslverr = channel_apb_rsp[ApbShireIdx].pslverr;
    for (int unsigned rbox = 0; rbox < NumRbox; rbox++) begin
      channel_apb_req[ApbRboxIdxStart + rbox].paddr = sbm_apb_req[shire_sbm_pkg::RboxRangeLo + rbox].paddr[shire_sbm_pkg::ShireApbAddrWidth-1:0];
      channel_apb_req[ApbRboxIdxStart + rbox].penable = sbm_apb_req[shire_sbm_pkg::RboxRangeLo + rbox].penable;
      channel_apb_req[ApbRboxIdxStart + rbox].psel = sbm_apb_req[shire_sbm_pkg::RboxRangeLo + rbox].psel;
      channel_apb_req[ApbRboxIdxStart + rbox].pwdata = sbm_apb_req[shire_sbm_pkg::RboxRangeLo + rbox].pwdata;
      channel_apb_req[ApbRboxIdxStart + rbox].pwrite = sbm_apb_req[shire_sbm_pkg::RboxRangeLo + rbox].pwrite;
      sbm_apb_rsp[shire_sbm_pkg::RboxRangeLo + rbox].prdata = channel_apb_rsp[ApbRboxIdxStart + rbox].prdata;
      sbm_apb_rsp[shire_sbm_pkg::RboxRangeLo + rbox].pready = channel_apb_rsp[ApbRboxIdxStart + rbox].pready;
      sbm_apb_rsp[shire_sbm_pkg::RboxRangeLo + rbox].pslverr = channel_apb_rsp[ApbRboxIdxStart + rbox].pslverr;
    end
    if (IcacheMemsImplemented) begin
      channel_apb_req[ApbIcacheIdx].paddr = sbm_apb_req[shire_sbm_pkg::IcacheApbInterfaceId].paddr[shire_sbm_pkg::ShireApbAddrWidth-1:0];
      channel_apb_req[ApbIcacheIdx].penable = sbm_apb_req[shire_sbm_pkg::IcacheApbInterfaceId].penable;
      channel_apb_req[ApbIcacheIdx].psel = sbm_apb_req[shire_sbm_pkg::IcacheApbInterfaceId].psel;
      channel_apb_req[ApbIcacheIdx].pwdata = sbm_apb_req[shire_sbm_pkg::IcacheApbInterfaceId].pwdata;
      channel_apb_req[ApbIcacheIdx].pwrite = sbm_apb_req[shire_sbm_pkg::IcacheApbInterfaceId].pwrite;
      sbm_apb_rsp[shire_sbm_pkg::IcacheApbInterfaceId].prdata = channel_apb_rsp[ApbIcacheIdx].prdata;
      sbm_apb_rsp[shire_sbm_pkg::IcacheApbInterfaceId].pready = channel_apb_rsp[ApbIcacheIdx].pready;
      sbm_apb_rsp[shire_sbm_pkg::IcacheApbInterfaceId].pslverr = channel_apb_rsp[ApbIcacheIdx].pslverr;
    end
    for (int unsigned neigh = 0; neigh < NumNeigh; neigh++) begin
      apb_esr_req_neigh_o[neigh].apb_paddr = sbm_apb_req[neigh].paddr[neigh_ch_apb_mux_pkg::NeighDmApbAddrWidth-1:0];
      apb_esr_req_neigh_o[neigh].apb_penable = sbm_apb_req[neigh].penable;
      apb_esr_req_neigh_o[neigh].apb_psel = sbm_apb_req[neigh].psel;
      apb_esr_req_neigh_o[neigh].apb_pwdata = sbm_apb_req[neigh].pwdata;
      apb_esr_req_neigh_o[neigh].apb_pwrite = sbm_apb_req[neigh].pwrite;
      sbm_apb_rsp[neigh].prdata = apb_esr_rsp_neigh_i[neigh].apb_prdata;
      sbm_apb_rsp[neigh].pready = apb_esr_rsp_neigh_i[neigh].apb_pready;
      sbm_apb_rsp[neigh].pslverr = apb_esr_rsp_neigh_i[neigh].apb_pslverr;
    end
  end

  shire_dmctrl u_dmctrl (
    .clk_i           (clk_shire_o),
    .rst_ni          (rst_c_ni),
    .dmctrl_i        (dmctrl_i),
    .dmctrl_o        (dmctrl_sync)
  );

  shire_pll_wrapper u_pll_wrapper (
    .clk_i                    (clk_ctrl_i),
    .rst_ni                   (rst_c_ni),
    .dft_i                    (dft_reset_hv),
    .clk_ref_i                (clk_ref_i),
    .clk_step_i               (clk_step_i),
    .clk_pll_i                (clk_pll_i),
    .clk_dll_i                (clk_dll_i),
    .clk_neigh_o              (clk_neigh_o),
    .clk_shire_o              (clk_shire_o),
    .clk_shire_debug_o        (clk_shire_debug_o),
    .pll_lock_i               (1'b1),
    .dll_lock_i               (1'b0),
    .pll_lock_detect_sys_o    (channel_pll_lock_detect),
    .dll_lock_detect_sys_o    (channel_dll_lock_detect),
    .clk_gate_neigh_disable_i (channel_clk_gate_ctrl.neighs_clock_disable[NumNeigh-1:0]),
    .clk_gate_debug_disable_i (channel_debug_clk_gate_ctrl),
    .pll_auto_cfg_i           (channel_pll_ctrl),
    .pll_auto_busy_o          (channel_pll_busy),
    .pll_auto_wdata_i         (channel_pll_conf),
    .pll_auto_rdata_o         (channel_pll_rdata),
    .pll_auto_rrdy_o          (channel_pll_rrdy),
    .dll_auto_cfg_i           (channel_dll_ctrl),
    .dll_auto_busy_o          (channel_dll_busy),
    .dll_auto_wdata_i         (channel_dll_conf),
    .dll_auto_rdata_o         (channel_dll_rdata),
    .dll_auto_rrdy_o          (channel_dll_rrdy),
    .clk_sel_i                (channel_clockmux)
  );

  sbm_top #(.SbmInterfaces(shire_sbm_pkg::SbmInterfaces), .ShireApbInterfaces(SbmApbSlaves)) u_sbm (
    .clk_i                 (clk_shire_o),
    .rst_ni                (rst_w_ni),
    .rst_neigh_ni          (rst_c_shire_scs_no),
    .rst_shire_cache_ni    (rst_sc_no),
    .rst_rbox_ni           (rst_rbox_no),
    .shire_virtual_id_i    (shire_virtual_id),
    .sys_axi_ar_valid_i    (sbm_enable_read),
    .sys_axi_aw_valid_i    (sbm_enable_write),
    .sbm_write_credit_return_o (sbm_write_credit_return),
    .sys_axi_ar_i          (sbm_sys_axi_ar),
    .sys_axi_ar_ready_o    (sbm_sys_axi_ar_ready),
    .sys_axi_aw_i          (sbm_sys_axi_aw),
    .sys_axi_aw_ready_o    (sbm_sys_axi_aw_ready),
    .sys_axi_w_i           (sbm_sys_axi_w),
    .sys_axi_w_ready_o     (sbm_sys_axi_w_ready),
    .sys_axi_b_o           (sbm_sys_axi_b),
    .sys_axi_b_valid_o     (sbm_sys_axi_b_valid),
    .sys_axi_b_ready_i     (sbm_sys_axi_b_ready),
    .sys_axi_r_o           (sbm_sys_axi_r),
    .sys_axi_r_valid_o     (sbm_sys_axi_r_valid),
    .sys_axi_r_ready_i     (sbm_sys_axi_r_ready),
    .apb_req_o             (sbm_apb_req),
    .apb_rsp_i             (sbm_apb_rsp)
  );

  shire_channel #(
    .NumNeigh              (NumNeigh),
    .NumRbox               (NumRbox),
    .NumBanks              (NumBanks),
    .NumUc                 (NumUc),
    .NumPorts              (NumPorts),
    .NumL3MasterPorts      (NumL3MasterPorts),
    .NumL3SlavePorts       (NumL3SlavePorts),
    .NumSysPorts           (NumSysPorts),
    .NumBankReqqEntries    (NumBankReqqEntries),
    .SetsPerSubBankParam   (SetsPerSubBankParam),
    .UseNocClkGating       (UseNocClkGating),
    .IcacheMemsImplemented (IcacheMemsImplemented),
    .L2hpfImplemented      (L2hpfImplemented),
    .ApbSlaves             (ChannelApbSlaves)
  ) u_channel (
    .clk_i                  (clk_shire_o),
    .rst_c_ni,
    .rst_w_ni,
    .rst_d_ni,
    .rst_w_icache_ni,
    .noc_clk_i              (clk_noc_i),
    .noc_rst_ni             (rst_noc_lv_no),
    .dft_hv_i               (dft_hv_i),
    .dft_lv_i               (dft_lv_i),
    .dft_sram_clk_i,
    .dft_mbist_en_i,
    .ram_cfg_o,
    .status_monitor_bank_sel_i,
    .apb_req_i              (channel_apb_req),
    .apb_rsp_o              (channel_apb_rsp),
    .shire_phy_id_i,
    .shire_id_reset_val_i,
    .sc_neigh_l2hpf_req_valid_o,
    .sc_neigh_l2hpf_req_info_o,
    .sc_trace_data_o,
    .sc_trace_valid_o,
    .neigh_sc_req_ready_o,
    .neigh_sc_req_valid_i,
    .neigh_sc_req_info_i,
    .neigh_sc_rsp_ready_i,
    .neigh_sc_rsp_valid_o,
    .neigh_sc_rsp_info_o,
    .flb_neigh_l2_req_valid_i,
    .flb_neigh_l2_req_data_i,
    .flb_l2_neigh_resp_valid_o,
    .flb_l2_neigh_resp_data_o,
    .uc_to_neigh_fcc_o,
    .uc_to_neigh_fcc_target_o,
    .bpam_run_control_i,
    .bpam_run_control_neigh_o,
    .bpam_rc_tbox_ack_i,
    .noc_err_int_srcs_i,
    .noc_all_err_int_srcs_o,
    .ioshire_log_err_int_o (ioshire_combined_err_int_o),
    .ioshire_noc_err_int_o,
    .debug_and_or_tree_l2_i,
    .debug_and_or_tree_l2_o,
    .esr_and_or_tree_l0_i,
    .coop_tload_slv_rdy_out_data_i,
    .coop_tload_slv_rdy_out_valid_i,
    .coop_tload_slv_rdy_in_data_o,
    .coop_tload_slv_rdy_in_valid_o,
    .coop_tload_mst_done_out_coop_id_i,
    .coop_tload_mst_done_out_valid_i,
    .coop_tload_mst_done_in_coop_id_o,
    .coop_tload_mst_done_in_valid_o,
    .icache_req_write_i,
    .icache_req_addr_i,
    .icache_req_valid_i,
    .icache_req_ready_o,
    .icache_resp_dout_o,
    .icache_resp_valid_o,
    .icache_resp_ready_i,
    .to_l3_axi_ar_o,
    .to_l3_axi_ar_valid_o,
    .to_l3_axi_ar_ready_i,
    .to_l3_axi_aw_o,
    .to_l3_axi_aw_valid_o,
    .to_l3_axi_aw_ready_i,
    .to_l3_axi_w_o,
    .to_l3_axi_w_valid_o,
    .to_l3_axi_w_ready_i,
    .to_l3_axi_b_i,
    .to_l3_axi_b_valid_i,
    .to_l3_axi_b_ready_o,
    .to_l3_axi_r_i,
    .to_l3_axi_r_valid_i,
    .to_l3_axi_r_ready_o,
    .to_sys_axi_ar_o,
    .to_sys_axi_ar_valid_o,
    .to_sys_axi_ar_ready_i,
    .to_sys_axi_aw_o,
    .to_sys_axi_aw_valid_o,
    .to_sys_axi_aw_ready_i,
    .to_sys_axi_w_o,
    .to_sys_axi_w_valid_o,
    .to_sys_axi_w_ready_i,
    .to_sys_axi_b_i,
    .to_sys_axi_b_valid_i,
    .to_sys_axi_b_ready_o,
    .to_sys_axi_r_i,
    .to_sys_axi_r_valid_i,
    .to_sys_axi_r_ready_o,
    .l3_axi_ar_i,
    .l3_axi_ar_valid_i,
    .l3_axi_ar_ready_o,
    .l3_axi_aw_i,
    .l3_axi_aw_valid_i,
    .l3_axi_aw_ready_o,
    .l3_axi_w_i,
    .l3_axi_w_valid_i,
    .l3_axi_w_ready_o,
    .l3_axi_b_o,
    .l3_axi_b_valid_o,
    .l3_axi_b_ready_i,
    .l3_axi_r_o,
    .l3_axi_r_valid_o,
    .l3_axi_r_ready_i,
    .uc_to_l3_axi_ar_o,
    .uc_to_l3_axi_ar_valid_o,
    .uc_to_l3_axi_ar_ready_i,
    .uc_to_l3_axi_aw_o,
    .uc_to_l3_axi_aw_valid_o,
    .uc_to_l3_axi_aw_ready_i,
    .uc_to_l3_axi_w_o,
    .uc_to_l3_axi_w_valid_o,
    .uc_to_l3_axi_w_ready_i,
    .uc_to_l3_axi_b_i,
    .uc_to_l3_axi_b_valid_i,
    .uc_to_l3_axi_b_ready_o,
    .uc_to_l3_axi_r_i,
    .uc_to_l3_axi_r_valid_i,
    .uc_to_l3_axi_r_ready_o,
    .uc_to_sys_axi_ar_o,
    .uc_to_sys_axi_ar_valid_o,
    .uc_to_sys_axi_ar_ready_i,
    .uc_to_sys_axi_aw_o,
    .uc_to_sys_axi_aw_valid_o,
    .uc_to_sys_axi_aw_ready_i,
    .uc_to_sys_axi_w_o,
    .uc_to_sys_axi_w_valid_o,
    .uc_to_sys_axi_w_ready_i,
    .uc_to_sys_axi_b_i,
    .uc_to_sys_axi_b_valid_i,
    .uc_to_sys_axi_b_ready_o,
    .uc_to_sys_axi_r_i,
    .uc_to_sys_axi_r_valid_i,
    .uc_to_sys_axi_r_ready_o,
    .sys_axi_ar_i,
    .sys_axi_ar_valid_i,
    .sys_axi_ar_ready_o,
    .sys_axi_aw_i,
    .sys_axi_aw_valid_i,
    .sys_axi_aw_ready_o,
    .sys_axi_w_i,
    .sys_axi_w_valid_i,
    .sys_axi_w_ready_o,
    .sys_axi_b_o,
    .sys_axi_b_valid_o,
    .sys_axi_b_ready_i,
    .sys_axi_r_o,
    .sys_axi_r_valid_o,
    .sys_axi_r_ready_i,
    .sys_axi_aw_vcvalid_i,
    .sys_axi_aw_credit_o,
    .sys_axi_w_vcvalid_i,
    .sbm_enable_read_o      (sbm_enable_read),
    .sbm_enable_write_o     (sbm_enable_write),
    .sbm_write_credit_return_i (sbm_write_credit_return),
    .sbm_sys_axi_ar_o       (sbm_sys_axi_ar),
    .sbm_sys_axi_ar_ready_i (sbm_sys_axi_ar_ready),
    .sbm_sys_axi_aw_o       (sbm_sys_axi_aw),
    .sbm_sys_axi_aw_ready_i (sbm_sys_axi_aw_ready),
    .sbm_sys_axi_w_o        (sbm_sys_axi_w),
    .sbm_sys_axi_w_ready_i  (sbm_sys_axi_w_ready),
    .sbm_sys_axi_b_i        (sbm_sys_axi_b),
    .sbm_sys_axi_b_valid_i  (sbm_sys_axi_b_valid),
    .sbm_sys_axi_b_ready_o  (sbm_sys_axi_b_ready),
    .sbm_sys_axi_r_i        (sbm_sys_axi_r),
    .sbm_sys_axi_r_valid_i  (sbm_sys_axi_r_valid),
    .sbm_sys_axi_r_ready_o  (sbm_sys_axi_r_ready),
    .esr_clk_gate_ctrl_o    (channel_clk_gate_ctrl),
    .esr_debug_clk_gate_ctrl_o (channel_debug_clk_gate_ctrl),
    .esr_pll_ctrl_o         (channel_pll_ctrl),
    .esr_pll_conf_o         (channel_pll_conf),
    .esr_dll_ctrl_o         (channel_dll_ctrl),
    .esr_dll_conf_o         (channel_dll_conf),
    .esr_dll_busy_i         (channel_dll_busy),
    .esr_dll_rdata_i        (channel_dll_rdata),
    .esr_dll_rrdy_i         (channel_dll_rrdy),
    .esr_dll_lock_i         (channel_dll_lock_detect),
    .esr_shire_ctrl_clockmux_o (channel_clockmux),
    .esr_pll_busy_i         (channel_pll_busy),
    .esr_pll_rdata_i        (channel_pll_rdata),
    .esr_pll_rrdy_i         (channel_pll_rrdy),
    .esr_pll_lock_i         (channel_pll_lock_detect),
    .esr_clk_dly_ctl_o      (channel_clk_dly_ctl_unused),
    .esr_dll_dly_est_ctl_o  (channel_dll_dly_est_ctl),
    .esr_dll_dly_est_sts_i  (channel_dll_dly_est_sts),
    .esr_mtime_local_target_o (channel_mtime_local_target),
    .esr_thread0_enable_o   (channel_thread0_enable),
    .esr_thread1_enable_o   (channel_thread1_enable),
    .esr_ipi_trigger_o      (channel_ipi_msip),
    .esr_ipi_redirect_trigger_o (channel_ipi_redirect_trigger),
    .esr_minion_features_o  (channel_minion_features),
    .esr_icache_prefetch_conf_o (channel_icache_prefetch_conf),
    .esr_icache_prefetch_start_o,
    .esr_icache_prefetch_done_i,
    .neigh_sc_err_detected_i (esr_neigh_icache_err_detected_i),
    .neigh_sc_err_logged_i  (esr_neigh_icache_err_logged_i),
    .esr_shire_coop_mode_o  (channel_shire_coop_mode),
    .esr_pwr_ctrl_neigh_nsleepin_o (channel_pwr_ctrl_nsleepin),
    .esr_pwr_ctrl_neigh_iso_o (channel_pwr_ctrl_iso),
    .esr_pwr_ctrl_neigh_nsleepout_i (channel_pwr_ctrl_nsleepout),
    .esr_pwr_ctrl_glb_nsleepin_o (channel_glb_nsleepin),
    .esr_pwr_ctrl_glb_iso_o (channel_glb_iso),
    .esr_pwr_ctrl_glb_nsleepout_i (channel_glb_nsleepout),
    .shire_id_o,
    .tbox_id_o,
    .tbox_en_o,
    .rst_c_shire_no,
    .rst_d_shire_no,
    .rst_w_shire_no,
    .rst_c_shire_scs_no,
    .rst_sc_no,
    .rst_rbox_no
  );

  assign esr_clk_gate_ctrl_o = channel_clk_gate_ctrl;
  assign esr_debug_clk_gate_ctrl_o = channel_debug_clk_gate_ctrl;
  assign esr_pll_ctrl_o = channel_pll_ctrl;
  assign esr_pll_conf_o = channel_pll_conf;
  assign esr_dll_ctrl_o = channel_dll_ctrl;
  assign esr_dll_conf_o = channel_dll_conf;
  assign esr_shire_ctrl_clockmux_o = channel_clockmux;

  logic unused_wrapper_inputs;
  assign unused_wrapper_inputs = ^{dft_lv_i.ram_rei, dft_lv_i.ram_wei, dft_lv_i.sram_clk_override,
                                  dft_hv_i.ram_rei, dft_hv_i.ram_wei, dft_hv_i.sram_clk_override,
                                  clk_noc_i, clk_ctrl_i, rst_system_ni, rst_system_debug_ni,
                                  dft_sram_clk_i, dft_mbist_en_i};

endmodule : shire_channel_wrap
/* verilator lint_on VARHIDDEN */
/* verilator lint_on UNOPTFLAT */
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on SYNCASYNCNET */
