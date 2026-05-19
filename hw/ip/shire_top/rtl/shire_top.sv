// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off PINCONNECTEMPTY */  // Native top intentionally ties off/open-connects omitted scan/TBOX/third-party child pins.
/* verilator lint_off UNUSEDSIGNAL */     // Integration top keeps retained internal observation seams used by DV/cosim.
/* verilator lint_off VARHIDDEN */        // Parameters intentionally mirror package topology constants.
module shire_top
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
  parameter bit          EnableGfx             = 1'b0,
  parameter logic [NumNeigh-1:0][neigh_pkg::MinPerN-1:0] DisableMinions = '0,
  parameter logic [NumNeigh-1:0][neigh_pkg::MinPerN-1:0] StubMinions    = '0,
  parameter int unsigned ChannelApbSlaves      = NumBanks + NumRbox +
                                                 (IcacheMemsImplemented ? 1 : 0) + 1,
  parameter int unsigned SbmApbSlaves          = shire_sbm_pkg::ShireApbInterfaces,
  parameter int unsigned DebugResetPulseCycles = 32
) (
  input  logic                                                clk_shire_i,
  input  logic                                                clk_debug_i,
  input  logic [NumNeigh-1:0]                                clk_neigh_i,
  input  logic                                                clk_noc_i,

  input  logic                                                rst_cold_ni,
  input  logic                                                rst_warm_ni,
  input  logic                                                rst_system_ni,
  input  logic                                                rst_system_debug_ni,
  output logic                                                rst_system_lv_no,
  output logic                                                rst_system_debug_lv_no,

  input  dft_pkg::dft_t                                       dft_hv_i,
  input  dft_pkg::dft_t                                       dft_lv_i,
  input  logic                                                dft_sram_clk_i,
  input  logic                                                dft_mbist_en_i,

  input  esr_pkg::esr_ms_dmctrl_t                             dmctrl_i,
  input  shire_channel_leaves_pkg::bpam_run_control_t         bpam_run_control_i,

  input  logic [shire_esr_pkg::NumShireIdsBits-1:0]           shire_phy_id_i,
  input  logic [shire_esr_pkg::NumShireIdsBits-1:0]           shire_id_reset_val_i,

  input  logic                                                plic_mtip_i,
  input  logic                                                plic_meip_i,
  input  logic                                                plic_seip_i,

  input  shire_channel_leaves_pkg::esr_and_or_tree_l2_t       debug_and_or_tree_l2_i,
  output shire_channel_leaves_pkg::esr_and_or_tree_l2_t       debug_and_or_tree_l2_o,

  output logic                                                ioshire_combined_err_int_o,
  output logic                                                ioshire_noc_err_int_o,
  input  logic [shire_channel_leaves_pkg::NocIntNum-1:0]      noc_err_int_srcs_i,
  output logic [shire_channel_leaves_pkg::NocIntNum-1:0]      noc_all_err_int_srcs_o,
  input  logic [shirecache_pkg::BankIdSize-1:0]               status_monitor_bank_sel_i,

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

  output logic [NumBanks-1:0]                                sc_neigh_l2hpf_req_valid_o,
  output shirecache_pkg::neigh_l2hpf_req_t [NumBanks-1:0]    sc_neigh_l2hpf_req_info_o,
  output shirecache_pkg::trace_packet_t                      sc_trace_data_o,
  output logic                                                sc_trace_valid_o,

  output ram_cfg_pkg::ram_cfg_t                               ram_cfg_o,
  output shire_esr_pkg::esr_clk_gate_ctrl_t                   esr_clk_gate_ctrl_o,
  output logic                                                esr_debug_clk_gate_ctrl_o,
  output shire_esr_pkg::esr_pll_auto_config_t                 esr_pll_ctrl_o,
  output logic [shire_esr_pkg::ShirePllConfBits-1:0]          esr_pll_conf_o,
  output shire_esr_pkg::esr_dll_auto_config_t                 esr_dll_ctrl_o,
  output logic [shire_esr_pkg::ShireDllConfBits-1:0]          esr_dll_conf_o,
  output shire_esr_pkg::esr_clkmux_ctl_t                      esr_shire_ctrl_clockmux_o
);

  logic [NumNeigh-1:0] clk_neigh;
  logic [NumNeigh-1:0] clk_shire_to_neigh;
  logic clk_shire;
  logic clk_shire_debug;
  logic [NumNeigh-1:0] wrap_clk_neigh;
  logic [NumNeigh-1:0] wrap_clk_shire_to_neigh;
  logic wrap_clk_shire;
  logic wrap_clk_shire_debug;

  // Per the native compute-shire contract, the public top receives already
  // generated functional clocks.  The translated wrapper's clock-control leaf
  // remains instantiated for ESR/status behavior, but neighborhood clocks at
  // this boundary come directly from `clk_neigh_i` and `clk_shire_i`.
  assign clk_neigh = clk_neigh_i;
  assign clk_shire_to_neigh = {NumNeigh{clk_shire_i}};
  assign clk_shire = clk_shire_i;
  assign clk_shire_debug = clk_debug_i;

  logic [NumNeigh-1:0] rst_c_shire_n;
  logic [NumNeigh-1:0] rst_d_shire_n;
  logic [NumNeigh-1:0] rst_w_shire_n;
  logic [NumNeigh-1:0] rst_c_shire_scs_n;
  logic [NumNeigh-1:0] rst_warm_to_neigh_n;
  logic [NumNeigh-1:0] rst_w_icache_n;
  logic rst_sc_n;
  logic rst_rbox_n;

  esr_pkg::esr_ms_dmctrl_t dmctrl_to_neigh [NumNeigh-1:0];
  neigh_voltage_cross_pkg::bpam_run_control_neigh_t bpam_run_control_neigh [NumNeigh-1:0];
  logic [NumNeigh-1:0][shire_esr_pkg::NumShireIdsBits-1:0] shire_id;
  logic [NumNeigh-1:0][neigh_pkg::NumNeighWidth-1:0] neigh_id;
  logic [shire_esr_pkg::TboxPerShire-1:0][1:0] tbox_id;
  logic [shire_esr_pkg::TboxPerShire-1:0] tbox_en;

  logic [NumNeigh-1:0][shire_esr_pkg::MinPerNeigh-1:0] int_mtip_to_neigh;
  logic [NumNeigh-1:0] plic_meip_to_neigh;
  logic [NumNeigh-1:0] plic_seip_to_neigh;
  logic [NumNeigh-1:0][shire_esr_pkg::MinPerNeigh-1:0] esr_thread0_enable;
  logic [NumNeigh-1:0][shire_esr_pkg::MinPerNeigh-1:0] esr_thread1_enable;
  logic [NumNeigh-1:0][ThreadsPerNeigh-1:0] esr_neigh_ipi_msip;
  logic [NumNeigh-1:0][ThreadsPerNeigh-1:0] esr_neigh_ipi_redirect_trigger;
  minion_pkg::esr_minion_features_t esr_minion_features [NumNeigh-1:0];
  icache_pkg::icache_prefetch_conf_t esr_icache_prefetch_conf [NumNeigh-1:0];
  logic [shire_esr_pkg::IcachePerShire-1:0] esr_icache_prefetch_start;
  logic [shire_esr_pkg::IcachePerShire-1:0] esr_icache_prefetch_done;
  logic [NumNeigh-1:0] esr_neigh_icache_err_detected;
  logic [NumNeigh-1:0] esr_neigh_icache_err_logged;
  logic [NumNeigh-1:0] esr_shire_coop_mode;

  esr_pkg::esr_and_or_tree_l0_t esr_and_or_tree_l0 [NumNeigh-1:0];
  neigh_voltage_cross_pkg::minion_chicken_bits_vc_t esr_neigh_clk_gate_ctrl [NumNeigh-1:0];
  logic [NumNeigh-1:0][minion_pkg::ShireMinionMemOverrideBits-1:0] esr_minion_mem_override;
  logic [NumNeigh-1:0] esr_pwr_ctrl_glb_nsleepin;
  logic [NumNeigh-1:0] esr_pwr_ctrl_glb_iso;
  logic [NumNeigh-1:0] esr_pwr_ctrl_glb_nsleepout;
  logic [NumNeigh-1:0][shire_esr_pkg::MinPerNeigh-1:0] esr_pwr_ctrl_neigh_nsleepin;
  logic [NumNeigh-1:0][shire_esr_pkg::MinPerNeigh-1:0] esr_pwr_ctrl_neigh_iso;
  logic [NumNeigh-1:0][shire_esr_pkg::MinPerNeigh-1:0] esr_pwr_ctrl_neigh_nsleepout;
  esr_pkg::esr_dll_dly_est_ctl_t esr_dll_dly_est_ctl [NumNeigh-1:0];
  esr_pkg::esr_dll_dly_est_sts_t esr_dll_dly_est_sts [NumNeigh-1:0];

  neigh_ch_apb_mux_pkg::apb_to_neigh_t apb_esr_req_neigh [NumNeigh-1:0];
  neigh_ch_apb_mux_pkg::apb_from_neigh_t apb_esr_rsp_neigh [NumNeigh-1:0];

  logic [NumNeigh-1:0][NumBanks+NumUc-1:0] neigh_sc_req_ready;
  logic [NumNeigh-1:0][NumBanks+NumUc-1:0] neigh_sc_req_valid;
  etlink_pkg::req_t [NumNeigh-1:0] neigh_sc_req_info;
  logic [NumNeigh-1:0] neigh_sc_rsp_ready;
  logic [NumNeigh-1:0] neigh_sc_rsp_valid;
  etlink_pkg::rsp_t [NumNeigh-1:0] neigh_sc_rsp_info;

  logic [NumNeigh-1:0][shire_uncached_pkg::MinPerNeigh-1:0] uc_to_neigh_fcc;
  logic [NumNeigh-1:0][shire_uncached_pkg::UcFccTargetSize-1:0] uc_to_neigh_fcc_target;
  logic [NumNeigh-1:0] flb_neigh_l2_req_valid;
  logic [NumNeigh-1:0][shire_uncached_pkg::CsrFlBarrierSize-1:0] flb_neigh_l2_req_data;
  logic [NumNeigh-1:0] flb_l2_neigh_resp_valid;
  logic [NumNeigh-1:0] flb_l2_neigh_resp_data;

  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t bpam_rc_tbox_ack [NumNeigh-1:0];
  logic [NumNeigh-1:0] icache_req_write;
  logic [NumNeigh-1:0][icache_geom_pkg::IcacheSramAddrWidth-1:0] icache_req_addr;
  logic [NumNeigh-1:0] icache_req_valid;
  logic [NumNeigh-1:0] icache_req_ready;
  logic [NumNeigh-1:0][icache_geom_pkg::IcacheSramDataWidth-1:0] icache_resp_dout;
  logic [NumNeigh-1:0] icache_resp_valid;
  logic [NumNeigh-1:0] icache_resp_ready;

  neigh_pkg::coop_tload_slv_rdy_req_t coop_tload_slv_rdy_out_data [NumNeigh-1:0];
  logic [NumNeigh-1:0] coop_tload_slv_rdy_out_valid;
  neigh_pkg::coop_tload_slv_rdy_req_t coop_tload_slv_rdy_in_data [NumNeigh-1:0][NumNeigh-2:0];
  logic [NumNeigh-1:0][NumNeigh-2:0] coop_tload_slv_rdy_in_valid;
  logic [NumNeigh-1:0][neigh_pkg::ShireCoopIdSize-1:0] coop_tload_mst_done_out_coop_id;
  logic [NumNeigh-1:0][NumNeigh-2:0] coop_tload_mst_done_out_valid;
  logic [NumNeigh-1:0][NumNeigh-2:0][neigh_pkg::ShireCoopIdSize-1:0] coop_tload_mst_done_in_coop_id;
  logic [NumNeigh-1:0][NumNeigh-2:0] coop_tload_mst_done_in_valid;

  always_comb begin
    for (int unsigned neigh = 0; neigh < NumNeigh; neigh++) begin
      neigh_id[neigh] = neigh[neigh_pkg::NumNeighWidth-1:0];
    end
  end

  shire_channel_wrap #(
    .NumNeigh             (NumNeigh),
    .NumRbox              (NumRbox),
    .NumBanks             (NumBanks),
    .NumUc                (NumUc),
    .NumPorts             (NumPorts),
    .NumL3MasterPorts     (NumL3MasterPorts),
    .NumL3SlavePorts      (NumL3SlavePorts),
    .NumSysPorts          (NumSysPorts),
    .ThreadsPerNeigh      (ThreadsPerNeigh),
    .NumMinionsPerShire   (NumMinionsPerShire),
    .NumGlobPwrDomains    (NumGlobPwrDomains),
    .NumBankReqqEntries   (NumBankReqqEntries),
    .SetsPerSubBankParam  (SetsPerSubBankParam),
    .UseNocClkGating      (UseNocClkGating),
    .IcacheMemsImplemented(IcacheMemsImplemented),
    .L2hpfImplemented     (L2hpfImplemented),
    .ChannelApbSlaves     (ChannelApbSlaves),
    .SbmApbSlaves         (SbmApbSlaves),
    .DebugResetPulseCycles(DebugResetPulseCycles)
  ) u_channel_wrap (
    .clk_ctrl_i                 (clk_shire_i),
    .clk_ref_i                  (clk_shire_i),
    .clk_step_i                 (clk_shire_i),
    .clk_pll_i                  ({4{clk_shire_i}}),
    .clk_dll_i                  (clk_shire_i),
    .clk_noc_i,
    .rst_cold_ni,
    .rst_warm_ni,
    .rst_system_ni,
    .rst_system_debug_ni,
    .rst_system_lv_no,
    .rst_system_debug_lv_no,
    .clk_neigh_o                 (wrap_clk_neigh),
    .clk_shire_to_neigh_o        (wrap_clk_shire_to_neigh),
    .clk_shire_o                 (wrap_clk_shire),
    .clk_shire_debug_o           (wrap_clk_shire_debug),
    .rst_c_shire_no              (rst_c_shire_n),
    .rst_d_shire_no              (rst_d_shire_n),
    .rst_w_shire_no              (rst_w_shire_n),
    .rst_c_shire_scs_no          (rst_c_shire_scs_n),
    .rst_warm_to_neigh_no        (rst_warm_to_neigh_n),
    .rst_w_icache_ni             (rst_w_icache_n),
    .rst_sc_no                   (rst_sc_n),
    .rst_rbox_no                 (rst_rbox_n),
    .dft_hv_i,
    .dft_lv_i,
    .dft_sram_clk_i,
    .dft_mbist_en_i,
    .dmctrl_i,
    .dmctrl_to_neigh_o           (dmctrl_to_neigh),
    .bpam_run_control_i,
    .bpam_run_control_neigh_o    (bpam_run_control_neigh),
    .shire_phy_id_i,
    .shire_id_reset_val_i,
    .shire_id_o                  (shire_id),
    .tbox_id_o                   (tbox_id),
    .tbox_en_o                   (tbox_en),
    .plic_mtip_i,
    .plic_meip_i,
    .plic_seip_i,
    .int_mtip_to_neigh_o         (int_mtip_to_neigh),
    .plic_meip_to_neigh_o        (plic_meip_to_neigh),
    .plic_seip_to_neigh_o        (plic_seip_to_neigh),
    .esr_thread0_enable_o        (esr_thread0_enable),
    .esr_thread1_enable_o        (esr_thread1_enable),
    .esr_neigh_ipi_msip_o        (esr_neigh_ipi_msip),
    .esr_neigh_ipi_redirect_trigger_o(esr_neigh_ipi_redirect_trigger),
    .esr_minion_features_o       (esr_minion_features),
    .esr_icache_prefetch_conf_o  (esr_icache_prefetch_conf),
    .esr_icache_prefetch_start_o (esr_icache_prefetch_start),
    .esr_icache_prefetch_done_i  (esr_icache_prefetch_done),
    .esr_neigh_icache_err_detected_i(esr_neigh_icache_err_detected),
    .esr_neigh_icache_err_logged_i(esr_neigh_icache_err_logged),
    .esr_shire_coop_mode_o       (esr_shire_coop_mode),
    .esr_and_or_tree_l0_i        (esr_and_or_tree_l0),
    .debug_and_or_tree_l2_i,
    .debug_and_or_tree_l2_o,
    .esr_neigh_clk_gate_ctrl_o   (esr_neigh_clk_gate_ctrl),
    .esr_minion_mem_override_o   (esr_minion_mem_override),
    .esr_pwr_ctrl_glb_nsleepin_o (esr_pwr_ctrl_glb_nsleepin),
    .esr_pwr_ctrl_glb_iso_o      (esr_pwr_ctrl_glb_iso),
    .esr_pwr_ctrl_glb_nsleepout_i(esr_pwr_ctrl_glb_nsleepout),
    .esr_pwr_ctrl_neigh_nsleepin_o(esr_pwr_ctrl_neigh_nsleepin),
    .esr_pwr_ctrl_neigh_iso_o    (esr_pwr_ctrl_neigh_iso),
    .esr_pwr_ctrl_neigh_nsleepout_i(esr_pwr_ctrl_neigh_nsleepout),
    .esr_dll_dly_est_ctl_o       (esr_dll_dly_est_ctl),
    .esr_dll_dly_est_sts_i       (esr_dll_dly_est_sts),
    .status_monitor_bank_sel_i,
    .apb_esr_req_neigh_o         (apb_esr_req_neigh),
    .apb_esr_rsp_neigh_i         (apb_esr_rsp_neigh),
    .neigh_sc_req_ready_o        (neigh_sc_req_ready),
    .neigh_sc_req_valid_i        (neigh_sc_req_valid),
    .neigh_sc_req_info_i         (neigh_sc_req_info),
    .neigh_sc_rsp_ready_i        (neigh_sc_rsp_ready),
    .neigh_sc_rsp_valid_o        (neigh_sc_rsp_valid),
    .neigh_sc_rsp_info_o         (neigh_sc_rsp_info),
    .uc_to_neigh_fcc_o           (uc_to_neigh_fcc),
    .uc_to_neigh_fcc_target_o    (uc_to_neigh_fcc_target),
    .flb_neigh_l2_req_valid_i    (flb_neigh_l2_req_valid),
    .flb_neigh_l2_req_data_i     (flb_neigh_l2_req_data),
    .flb_l2_neigh_resp_valid_o   (flb_l2_neigh_resp_valid),
    .flb_l2_neigh_resp_data_o    (flb_l2_neigh_resp_data),
    .bpam_rc_tbox_ack_i          (bpam_rc_tbox_ack),
    .ioshire_combined_err_int_o,
    .ioshire_noc_err_int_o,
    .noc_err_int_srcs_i,
    .noc_all_err_int_srcs_o,
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
    .icache_req_write_i          (icache_req_write),
    .icache_req_addr_i           (icache_req_addr),
    .icache_req_valid_i          (icache_req_valid),
    .icache_req_ready_o          (icache_req_ready),
    .icache_resp_dout_o          (icache_resp_dout),
    .icache_resp_valid_o         (icache_resp_valid),
    .icache_resp_ready_i         (icache_resp_ready),
    .sc_neigh_l2hpf_req_valid_o,
    .sc_neigh_l2hpf_req_info_o,
    .sc_trace_data_o,
    .sc_trace_valid_o,
    .coop_tload_slv_rdy_out_data_i(coop_tload_slv_rdy_out_data),
    .coop_tload_slv_rdy_out_valid_i(coop_tload_slv_rdy_out_valid),
    .coop_tload_slv_rdy_in_data_o(coop_tload_slv_rdy_in_data),
    .coop_tload_slv_rdy_in_valid_o(coop_tload_slv_rdy_in_valid),
    .coop_tload_mst_done_out_coop_id_i(coop_tload_mst_done_out_coop_id),
    .coop_tload_mst_done_out_valid_i(coop_tload_mst_done_out_valid),
    .coop_tload_mst_done_in_coop_id_o(coop_tload_mst_done_in_coop_id),
    .coop_tload_mst_done_in_valid_o(coop_tload_mst_done_in_valid),
    .ram_cfg_o,
    .esr_clk_gate_ctrl_o,
    .esr_debug_clk_gate_ctrl_o,
    .esr_pll_ctrl_o,
    .esr_pll_conf_o,
    .esr_dll_ctrl_o,
    .esr_dll_conf_o,
    .esr_shire_ctrl_clockmux_o
  );

  for (genvar gen_neigh = 0; gen_neigh < NumNeigh; gen_neigh++) begin : gen_neigh_top
    neigh_pkg::coop_tload_slv_rdy_req_t [NumNeigh-2:0] coop_tload_slv_rdy_in_data_packed;
    for (genvar coop_src = 0; coop_src < NumNeigh - 1; coop_src++) begin : gen_coop_pack
      assign coop_tload_slv_rdy_in_data_packed[coop_src] =
          coop_tload_slv_rdy_in_data[gen_neigh][coop_src];
    end

    logic [neigh_hv_logic_pkg::ThreadsPerN-1:0] esr_neigh_ipi_msip_ext;
    logic [neigh_hv_logic_pkg::ThreadsPerN-1:0] esr_neigh_ipi_redirect_trigger_ext;
    always_comb begin
      esr_neigh_ipi_msip_ext = '0;
      esr_neigh_ipi_redirect_trigger_ext = '0;
      esr_neigh_ipi_msip_ext[ThreadsPerNeigh-1:0] = esr_neigh_ipi_msip[gen_neigh];
      esr_neigh_ipi_redirect_trigger_ext[ThreadsPerNeigh-1:0] =
          esr_neigh_ipi_redirect_trigger[gen_neigh];
    end

    neigh_top #(
      .ShireId        (0),
      .NeighId        (gen_neigh),
      .EnableGfx      (EnableGfx),
      .DisableMinions (DisableMinions[gen_neigh]),
      .StubMinions    (StubMinions[gen_neigh])
    ) u_neigh_top (
      .clk_i                         (clk_neigh[gen_neigh]),
      .clk_shire_i                   (clk_shire_to_neigh[gen_neigh]),
      .rst_c_shire_ni                (rst_c_shire_n[gen_neigh]),
      .rst_d_shire_ni                (rst_d_shire_n[gen_neigh]),
      .rst_w_shire_ni                (rst_w_shire_n[gen_neigh]),
      .rst_warm_ni                   (rst_warm_to_neigh_n[gen_neigh]),
      .rst_w_icache_no               (rst_w_icache_n[gen_neigh]),
      .dft_scanin_hv_i               ('0),
      .dft_scanout_hv_o              (),
      .dft_hv_i                      (dft_hv_i),
      .dft_scan_enable_hv_i          (1'b0),
      .dft_test_mode_hv_i            ('0),
      .dft_clock_gate_en_hv_i        (1'b0),
      .dft_cntl_hv_i                 ('0),
      .dft_occ_scanin_hv_i           (1'b0),
      .dft_occ_scanout_hv_o          (),
      .dft_occ_reset_hv_i            (1'b0),
      .dft_occ_testmode_hv_i         (1'b0),
      .dft_scan_ate_clk_hv_i         (1'b0),
      .dft_occ_bypass_hv_i           (1'b0),
      .dft_use_reset_cntl_hv_i       (1'b0),
      .dft_reset_cntl_hv_i           ('0),
      .dft_scanin_tbox_hv_i          ('0),
      .dft_scanout_tbox_hv_o         (),
      .dft_occ_scanin_tbox_hv_i      (1'b0),
      .dft_occ_scanout_tbox_hv_o     (),
      .eco_i                         ('0),
      .eco_o                         (),
      .pwr_ctrl_glb_nsleepin_i       (esr_pwr_ctrl_glb_nsleepin[gen_neigh]),
      .pwr_ctrl_glb_nsleepout_o      (esr_pwr_ctrl_glb_nsleepout[gen_neigh]),
      .pwr_ctrl_glb_isolate_i        (esr_pwr_ctrl_glb_iso[gen_neigh]),
      .pwr_ctrl_min_nsleepin_i       (esr_pwr_ctrl_neigh_nsleepin[gen_neigh]),
      .pwr_ctrl_min_nsleepout_o      (esr_pwr_ctrl_neigh_nsleepout[gen_neigh]),
      .pwr_ctrl_min_isolate_i        (esr_pwr_ctrl_neigh_iso[gen_neigh]),
      .esr_clk_gate_ctrl_i           (esr_neigh_clk_gate_ctrl[gen_neigh]),
      .esr_minion_mem_override_i     (esr_minion_mem_override[gen_neigh]),
      .bpam_rc_tbox_ack_hi_o         (bpam_rc_tbox_ack[gen_neigh]),
      .bpam_run_control_i            (bpam_run_control_neigh[gen_neigh]),
      .shire_id_i                    (shire_id[gen_neigh]),
      .neigh_id_i                    (neigh_id[gen_neigh]),
      .shire_tbox_id_i               (tbox_id[gen_neigh]),
      .shire_tbox_en_i               (tbox_en[gen_neigh]),
      .esr_thread0_enable_i          (esr_thread0_enable[gen_neigh]),
      .esr_thread1_enable_i          (esr_thread1_enable[gen_neigh]),
      .esr_minion_features_i         (esr_minion_features[gen_neigh]),
      .esr_icache_prefetch_conf_i    (esr_icache_prefetch_conf[gen_neigh]),
      .esr_icache_prefetch_start_i   (esr_icache_prefetch_start[gen_neigh]),
      .esr_icache_prefetch_done_o    (esr_icache_prefetch_done[gen_neigh]),
      .esr_icache_err_detected_o     (esr_neigh_icache_err_detected[gen_neigh]),
      .esr_icache_err_logged_o       (esr_neigh_icache_err_logged[gen_neigh]),
      .dmctrl_i                      (dmctrl_to_neigh[gen_neigh]),
      .esr_and_or_tree_l0_o          (esr_and_or_tree_l0[gen_neigh]),
      .esr_shire_coop_mode_i         (esr_shire_coop_mode[gen_neigh]),
      .dll_feedback_shire_o          (),
      .dll_feedback_neigh_o          (),
      .esr_dll_dly_est_ctl_i         (esr_dll_dly_est_ctl[gen_neigh]),
      .esr_dll_dly_est_sts_o         (esr_dll_dly_est_sts[gen_neigh]),
      .neigh_sc_req_ready_i          (neigh_sc_req_ready[gen_neigh]),
      .neigh_sc_req_valid_o          (neigh_sc_req_valid[gen_neigh]),
      .neigh_sc_req_info_o           (neigh_sc_req_info[gen_neigh]),
      .neigh_sc_rsp_ready_o          (neigh_sc_rsp_ready[gen_neigh]),
      .neigh_sc_rsp_valid_i          (neigh_sc_rsp_valid[gen_neigh]),
      .neigh_sc_rsp_info_i           (neigh_sc_rsp_info[gen_neigh]),
      .hw_dbg_sm_monitor_enabled_i   (1'b0),
      .neigh_sm_gpio_i               ('0),
      .neigh_sm_signals_o            (),
      .apb_esr_req_i                 (apb_esr_req_neigh[gen_neigh]),
      .apb_esr_rsp_o                 (apb_esr_rsp_neigh[gen_neigh]),
      .int_mtip_i                    (int_mtip_to_neigh[gen_neigh]),
      .int_meip_i                    (plic_meip_to_neigh[gen_neigh]),
      .int_seip_i                    (plic_seip_to_neigh[gen_neigh]),
      .ipi_msip_i                    (esr_neigh_ipi_msip_ext),
      .ipi_redirect_trigger_i        (esr_neigh_ipi_redirect_trigger_ext),
      .uc_to_neigh_fcc_i             (uc_to_neigh_fcc[gen_neigh]),
      .uc_to_neigh_fcc_target_i      (uc_to_neigh_fcc_target[gen_neigh]),
      .flb_neigh_l2_req_valid_o      (flb_neigh_l2_req_valid[gen_neigh]),
      .flb_neigh_l2_req_data_o       (flb_neigh_l2_req_data[gen_neigh]),
      .flb_l2_neigh_resp_valid_i     (flb_l2_neigh_resp_valid[gen_neigh]),
      .flb_l2_neigh_resp_data_i      (flb_l2_neigh_resp_data[gen_neigh]),
      .icache_f2_sram_req_write_o    (icache_req_write[gen_neigh]),
      .icache_f2_sram_req_addr_o     (icache_req_addr[gen_neigh]),
      .icache_f2_sram_req_valid_o    (icache_req_valid[gen_neigh]),
      .icache_f2_sram_req_ready_i    (icache_req_ready[gen_neigh]),
      .icache_f0_sram_resp_dout_i    (icache_resp_dout[gen_neigh]),
      .icache_f0_sram_resp_valid_i   (icache_resp_valid[gen_neigh]),
      .icache_f0_sram_resp_ready_o   (icache_resp_ready[gen_neigh]),
      .voltage_monitor_vdd_o         (),
      .voltage_monitor_vss_o         (),
      .clk_tbox_o                    (),
      .tbox_rst_c_no                 (),
      .tbox_rst_d_no                 (),
      .tbox_rst_w_no                 (),
      .dft_tbox_lv_o                 (),
      .dft_scanin_to_tbox_lv_o       (),
      .dft_scanout_to_tbox_lv_i      ('0),
      .dft_occ_scanin_to_tbox_lv_o   (),
      .dft_occ_scanout_to_tbox_lv_i  (1'b0),
      .dft_scan_enable_to_tbox_lv_o  (),
      .dft_test_mode_to_tbox_lv_o    (),
      .dft_clock_gate_en_to_tbox_lv_o(),
      .dft_cntl_to_tbox_lv_o         (),
      .dft_occ_reset_to_tbox_lv_o    (),
      .dft_occ_testmode_to_tbox_lv_o (),
      .dft_scan_ate_clk_to_tbox_lv_o (),
      .dft_occ_bypass_to_tbox_lv_o   (),
      .tbox_id_lo_o                  (),
      .tbox_control_esr_o            (),
      .tbox_status_esr_i             ('0),
      .tbox_image_table_ptr_esr_o    (),
      .tbox_neigh_req_ready_o        (),
      .tbox_neigh_req_valid_i        (1'b0),
      .tbox_neigh_req_info_i         ('0),
      .tbox_neigh_rsp_ready_i        (1'b0),
      .tbox_neigh_rsp_valid_o        (),
      .tbox_neigh_rsp_info_o         (),
      .tbox_dbg_signals_en_o         (),
      .tbox_dbg_signals_i            ('0),
      .tbox_dbg_signals_mux_o        (),
      .tbox_apb_req_o                (),
      .tbox_apb_rsp_i                ('0),
      .tbox_bpam_run_control_o       (),
      .tbox_bpam_run_control_ack_i   ('0),
      .coop_tload_slv_rdy_out_data_o (coop_tload_slv_rdy_out_data[gen_neigh]),
      .coop_tload_slv_rdy_out_valid_o(coop_tload_slv_rdy_out_valid[gen_neigh]),
      .coop_tload_slv_rdy_in_data_i  (coop_tload_slv_rdy_in_data_packed),
      .coop_tload_slv_rdy_in_valid_i (coop_tload_slv_rdy_in_valid[gen_neigh]),
      .coop_tload_mst_done_out_coop_id_o(coop_tload_mst_done_out_coop_id[gen_neigh]),
      .coop_tload_mst_done_out_valid_o(coop_tload_mst_done_out_valid[gen_neigh]),
      .coop_tload_mst_done_in_coop_id_i(coop_tload_mst_done_in_coop_id[gen_neigh]),
      .coop_tload_mst_done_in_valid_i(coop_tload_mst_done_in_valid[gen_neigh])
    );
  end

endmodule : shire_top
/* verilator lint_on VARHIDDEN */
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on PINCONNECTEMPTY */
