// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSEDSIGNAL */  // Container-level seams intentionally pass through wide structs.
/* verilator lint_off UNOPTFLAT */     // Packed APB/ESR structs are assigned fieldwise; paths are acyclic.
/* verilator lint_off VARHIDDEN */     // Parameters intentionally shadow package defaults for integration sizing.
module shire_channel
  import axi_pkg::*;
  import apb_pkg::*;
  import dft_pkg::*;
  import etlink_pkg::*;
  import icache_pkg::*;
  import minion_pkg::*;
  import ram_cfg_pkg::*;
  import rbox_pkg::*;
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
  parameter int unsigned NumBankReqqEntries    = shirecache_pkg::ReqqDepth,
  parameter int unsigned SetsPerSubBankParam   = shirecache_pkg::SetsPerSubBank,
  parameter bit          UseNocClkGating       = 1'b1,
  parameter bit          IcacheMemsImplemented = 1'b1,
  parameter bit          L2hpfImplemented      = 1'b0,
  parameter int unsigned ApbSlaves             = NumBanks + NumRbox +
                                                 (IcacheMemsImplemented ? 1 : 0) + 1
) (
  input  logic                                                clk_i,
  input  logic                                                rst_c_ni,
  input  logic                                                rst_w_ni,
  input  logic                                                rst_d_ni,
  input  logic [NumNeigh-1:0]                                rst_w_icache_ni,
  input  logic                                                noc_clk_i,
  input  logic                                                noc_rst_ni,

  output logic [NumNeigh-1:0]                                rst_c_shire_no,
  output logic [NumNeigh-1:0]                                rst_d_shire_no,
  output logic [NumNeigh-1:0]                                rst_w_shire_no,
  output logic [NumNeigh-1:0]                                rst_c_shire_scs_no,
  output logic                                                rst_sc_no,
  output logic                                                rst_rbox_no,

  input  dft_pkg::dft_t                                       dft_hv_i,
  input  dft_pkg::dft_t                                       dft_lv_i,
  input  logic                                                dft_sram_clk_i,
  input  logic                                                dft_mbist_en_i,

  input  shire_channel_leaves_pkg::bpam_run_control_t         bpam_run_control_i,
  output neigh_voltage_cross_pkg::bpam_run_control_neigh_t    bpam_run_control_neigh_o [NumNeigh-1:0],

  input  logic [shire_esr_pkg::NumShireIdsBits-1:0]           shire_phy_id_i,
  input  logic [shire_esr_pkg::NumShireIdsBits-1:0]           shire_id_reset_val_i,
  output logic [NumNeigh-1:0][shire_esr_pkg::NumShireIdsBits-1:0] shire_id_o,
  output logic [shire_esr_pkg::TboxPerShire-1:0][1:0]         tbox_id_o,
  output logic [shire_esr_pkg::TboxPerShire-1:0]              tbox_en_o,

  output logic [shire_esr_pkg::ThreadsPerShire-1:0]           esr_thread0_enable_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0]           esr_thread1_enable_o,
  output minion_pkg::esr_minion_features_t                    esr_minion_features_o [NumNeigh-1:0],
  output icache_pkg::icache_prefetch_conf_t                   esr_icache_prefetch_conf_o [NumNeigh-1:0],
  output logic [shire_esr_pkg::IcachePerShire-1:0]            esr_icache_prefetch_start_o,
  input  logic [shire_esr_pkg::IcachePerShire-1:0]            esr_icache_prefetch_done_i,
  input  logic [NumNeigh-1:0]                                neigh_sc_err_detected_i,
  input  logic [NumNeigh-1:0]                                neigh_sc_err_logged_i,
  output logic                                                ioshire_log_err_int_o,
  input  esr_pkg::esr_and_or_tree_l0_t                       esr_and_or_tree_l0_i [NumNeigh-1:0],
  input  shire_channel_leaves_pkg::esr_and_or_tree_l2_t       debug_and_or_tree_l2_i,
  output shire_channel_leaves_pkg::esr_and_or_tree_l2_t       debug_and_or_tree_l2_o,
  output logic [NumNeigh-1:0]                                esr_shire_coop_mode_o,
  input  logic [shire_channel_leaves_pkg::NocIntNum-1:0]      noc_err_int_srcs_i,
  output logic [shire_channel_leaves_pkg::NocIntNum-1:0]      noc_all_err_int_srcs_o,
  output logic                                                ioshire_noc_err_int_o,
  output shire_esr_pkg::esr_clk_gate_ctrl_t                   esr_clk_gate_ctrl_o,
  output logic                                                esr_debug_clk_gate_ctrl_o,

  output logic [NumNeigh-1:0][NumBanks+NumUc-1:0]             neigh_sc_req_ready_o,
  input  logic [NumNeigh-1:0][NumBanks+NumUc-1:0]             neigh_sc_req_valid_i,
  input  etlink_pkg::req_t [NumNeigh-1:0]                     neigh_sc_req_info_i,
  input  logic [NumNeigh-1:0]                                 neigh_sc_rsp_ready_i,
  output logic [NumNeigh-1:0]                                 neigh_sc_rsp_valid_o,
  output etlink_pkg::rsp_t [NumNeigh-1:0]                     neigh_sc_rsp_info_o,

  output logic [NumNeigh-1:0][shire_uncached_pkg::MinPerNeigh-1:0] uc_to_neigh_fcc_o,
  output logic [NumNeigh-1:0][shire_uncached_pkg::UcFccTargetSize-1:0] uc_to_neigh_fcc_target_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0]           esr_ipi_trigger_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0]           esr_ipi_redirect_trigger_o,

  input  logic [NumNeigh-1:0]                                flb_neigh_l2_req_valid_i,
  input  logic [NumNeigh-1:0][shire_uncached_pkg::CsrFlBarrierSize-1:0] flb_neigh_l2_req_data_i,
  output logic [NumNeigh-1:0]                                flb_l2_neigh_resp_valid_o,
  output logic [NumNeigh-1:0]                                flb_l2_neigh_resp_data_o,

  input  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t               bpam_rc_tbox_ack_i [NumNeigh-1:0],

  input  shire_sbm_pkg::shire_apb_req_t [ApbSlaves-1:0]       apb_req_i,
  output shire_sbm_pkg::shire_apb_rsp_t [ApbSlaves-1:0]       apb_rsp_o,

  output logic [shire_esr_pkg::ThreadsPerShire-1:0]           esr_mtime_local_target_o,
  output logic [NumNeigh-1:0]                                esr_pwr_ctrl_glb_nsleepin_o,
  output logic [NumNeigh-1:0]                                esr_pwr_ctrl_glb_iso_o,
  input  logic [NumNeigh-1:0]                                esr_pwr_ctrl_glb_nsleepout_i,
  output logic [NumNeigh*shire_esr_pkg::MinPerNeigh-1:0]      esr_pwr_ctrl_neigh_nsleepin_o,
  output logic [NumNeigh*shire_esr_pkg::MinPerNeigh-1:0]      esr_pwr_ctrl_neigh_iso_o,
  input  logic [NumNeigh*shire_esr_pkg::MinPerNeigh-1:0]      esr_pwr_ctrl_neigh_nsleepout_i,

  output shire_esr_pkg::esr_pll_auto_config_t                 esr_pll_ctrl_o,
  output logic [shire_esr_pkg::ShirePllConfBits-1:0]          esr_pll_conf_o,
  input  logic                                                esr_pll_busy_i,
  input  logic [shire_esr_pkg::ShirePllDataBits-1:0]          esr_pll_rdata_i,
  input  logic                                                esr_pll_rrdy_i,
  input  logic                                                esr_pll_lock_i,
  output shire_esr_pkg::esr_clk_dly_ctl_t                     esr_clk_dly_ctl_o,
  output esr_pkg::esr_dll_dly_est_ctl_t                       esr_dll_dly_est_ctl_o,
  input  esr_pkg::esr_dll_dly_est_sts_t                       esr_dll_dly_est_sts_i [NumNeigh-1:0],
  output shire_esr_pkg::esr_clkmux_ctl_t                      esr_shire_ctrl_clockmux_o,
  output shire_esr_pkg::esr_dll_auto_config_t                 esr_dll_ctrl_o,
  output logic [shire_esr_pkg::ShireDllConfBits-1:0]          esr_dll_conf_o,
  input  logic                                                esr_dll_busy_i,
  input  logic [shire_esr_pkg::ShireDllDataBits-1:0]          esr_dll_rdata_i,
  input  logic                                                esr_dll_rrdy_i,
  input  logic                                                esr_dll_lock_i,
  output ram_cfg_pkg::ram_cfg_t                               ram_cfg_o,

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

  output logic                                                sbm_enable_read_o,
  output logic                                                sbm_enable_write_o,
  input  logic                                                sbm_write_credit_return_i,
  output axi_pkg::sys_slave_ar_t                              sbm_sys_axi_ar_o,
  input  logic                                                sbm_sys_axi_ar_ready_i,
  output axi_pkg::sys_slave_aw_t                              sbm_sys_axi_aw_o,
  input  logic                                                sbm_sys_axi_aw_ready_i,
  output axi_pkg::sys_slave_w_t                               sbm_sys_axi_w_o,
  input  logic                                                sbm_sys_axi_w_ready_i,
  input  axi_pkg::sys_slave_b_t                               sbm_sys_axi_b_i,
  input  logic                                                sbm_sys_axi_b_valid_i,
  output logic                                                sbm_sys_axi_b_ready_o,
  input  axi_pkg::sys_slave_r_t                               sbm_sys_axi_r_i,
  input  logic                                                sbm_sys_axi_r_valid_i,
  output logic                                                sbm_sys_axi_r_ready_o,

  input  logic [NumNeigh-1:0]                                icache_req_write_i,
  input  logic [NumNeigh-1:0][icache_geom_pkg::IcacheSramAddrWidth-1:0] icache_req_addr_i,
  input  logic [NumNeigh-1:0]                                icache_req_valid_i,
  output logic [NumNeigh-1:0]                                icache_req_ready_o,
  output logic [NumNeigh-1:0][icache_geom_pkg::IcacheSramDataWidth-1:0] icache_resp_dout_o,
  output logic [NumNeigh-1:0]                                icache_resp_valid_o,
  input  logic [NumNeigh-1:0]                                icache_resp_ready_i,

  output logic [NumBanks-1:0]                                sc_neigh_l2hpf_req_valid_o,
  output shirecache_pkg::neigh_l2hpf_req_t [NumBanks-1:0]    sc_neigh_l2hpf_req_info_o,
  input  logic [shirecache_pkg::BankIdSize-1:0]              status_monitor_bank_sel_i,
  output shirecache_pkg::trace_packet_t                      sc_trace_data_o,
  output logic                                                sc_trace_valid_o,

  input  neigh_pkg::coop_tload_slv_rdy_req_t                 coop_tload_slv_rdy_out_data_i [NumNeigh-1:0],
  input  logic [NumNeigh-1:0]                                coop_tload_slv_rdy_out_valid_i,
  output neigh_pkg::coop_tload_slv_rdy_req_t                 coop_tload_slv_rdy_in_data_o [NumNeigh-1:0][NumNeigh-2:0],
  output logic [NumNeigh-1:0][NumNeigh-2:0]                  coop_tload_slv_rdy_in_valid_o,
  input  logic [NumNeigh-1:0][neigh_pkg::ShireCoopIdSize-1:0] coop_tload_mst_done_out_coop_id_i,
  input  logic [NumNeigh-1:0][NumNeigh-2:0]                  coop_tload_mst_done_out_valid_i,
  output logic [NumNeigh-1:0][NumNeigh-2:0][neigh_pkg::ShireCoopIdSize-1:0] coop_tload_mst_done_in_coop_id_o,
  output logic [NumNeigh-1:0][NumNeigh-2:0]                  coop_tload_mst_done_in_valid_o
);

  localparam int unsigned ApbBanksIdxStart = 0;
  localparam int unsigned ApbBanksIdxEnd = ApbBanksIdxStart + NumBanks;
  localparam int unsigned ApbShireIdx = ApbBanksIdxEnd;
  localparam int unsigned ApbRboxIdxStart = ApbShireIdx + 1;
  localparam int unsigned ApbIcacheIdx = ApbRboxIdxStart + NumRbox;
  localparam int unsigned NumOtherNeigh = NumNeigh - 1;

  typedef enum logic [2:0] {
    StApbIdle  = 3'b000,
    StApbSetup = 3'b001,
    StApbExec  = 3'b010,
    StApbReady = 3'b011,
    StApbDone  = 3'b100
  } apb_state_e;

  apb_state_e [ApbSlaves-1:0] apb_state_q;
  apb_state_e [ApbSlaves-1:0] apb_state_d;
  logic [ApbSlaves-1:0] apb_state_en;
  logic [ApbSlaves-1:0] apb_setup_en;
  logic [ApbSlaves-1:0] apb_done_en;
  shire_sbm_pkg::shire_apb_req_t [ApbSlaves-1:0] apb_req_q;
  shire_sbm_pkg::shire_apb_rsp_t [ApbSlaves-1:0] apb_rsp_q;
  shire_sbm_pkg::shire_apb_req_t [ApbSlaves-1:0] apb_req_ff;
  shire_sbm_pkg::shire_apb_rsp_t [ApbSlaves-1:0] apb_rsp_target;

  for (genvar apb_idx = 0; apb_idx < ApbSlaves; apb_idx++) begin : gen_apb_ff
    always_comb begin
      apb_state_d[apb_idx] = apb_state_q[apb_idx];
      apb_state_en[apb_idx] = 1'b0;
      unique case (apb_state_q[apb_idx])
        StApbIdle: begin
          if (apb_req_i[apb_idx].psel) begin
            apb_state_d[apb_idx] = StApbSetup;
            apb_state_en[apb_idx] = 1'b1;
          end
        end
        StApbSetup: begin
          if (apb_req_i[apb_idx].penable) begin
            apb_state_d[apb_idx] = StApbExec;
            apb_state_en[apb_idx] = 1'b1;
          end
        end
        StApbExec: begin
          if (apb_rsp_target[apb_idx].pready) begin
            apb_state_d[apb_idx] = StApbReady;
            apb_state_en[apb_idx] = 1'b1;
          end
        end
        StApbReady: begin
          apb_state_d[apb_idx] = StApbDone;
          apb_state_en[apb_idx] = 1'b1;
        end
        StApbDone: begin
          apb_state_d[apb_idx] = apb_req_i[apb_idx].psel ? StApbSetup : StApbIdle;
          apb_state_en[apb_idx] = 1'b1;
        end
        default: begin
          apb_state_d[apb_idx] = StApbIdle;
          apb_state_en[apb_idx] = 1'b1;
        end
      endcase
    end

    assign apb_setup_en[apb_idx] = ((apb_state_q[apb_idx] == StApbIdle) &&
                                    apb_req_i[apb_idx].psel) ||
                                   ((apb_state_q[apb_idx] == StApbDone) &&
                                    apb_req_i[apb_idx].psel);
    assign apb_done_en[apb_idx] = (apb_state_q[apb_idx] == StApbExec) &&
                                  apb_rsp_target[apb_idx].pready;

    always_ff @(posedge clk_i or negedge rst_c_ni) begin
      if (!rst_c_ni) begin
        apb_state_q[apb_idx] <= StApbIdle;
        apb_req_q[apb_idx] <= '0;
        apb_rsp_q[apb_idx] <= '0;
      end else begin
        if (apb_state_en[apb_idx]) begin
          apb_state_q[apb_idx] <= apb_state_d[apb_idx];
        end
        if (apb_setup_en[apb_idx]) begin
          apb_req_q[apb_idx].paddr <= apb_req_i[apb_idx].paddr;
          apb_req_q[apb_idx].pwrite <= apb_req_i[apb_idx].pwrite;
          apb_req_q[apb_idx].pwdata <= apb_req_i[apb_idx].pwdata;
        end
        if (apb_done_en[apb_idx]) begin
          apb_rsp_q[apb_idx].pslverr <= apb_rsp_target[apb_idx].pslverr;
          apb_rsp_q[apb_idx].prdata <= apb_rsp_target[apb_idx].prdata;
        end
      end
    end

    always_comb begin
      apb_req_ff[apb_idx] = apb_req_q[apb_idx];
      apb_req_ff[apb_idx].psel = (apb_state_q[apb_idx] == StApbSetup) ||
                                 (apb_state_q[apb_idx] == StApbExec);
      apb_req_ff[apb_idx].penable = (apb_state_q[apb_idx] == StApbExec);
      apb_rsp_o[apb_idx] = apb_rsp_q[apb_idx];
      apb_rsp_o[apb_idx].pready = (apb_state_q[apb_idx] == StApbReady);
    end
  end

  shirecache_pkg::esr_shire_config_t esr_shire_config;
  minion_pkg::esr_minion_features_t  esr_minion_features;
  icache_pkg::icache_prefetch_conf_t esr_icache_prefetch_conf;
  logic                              esr_shire_coop_mode;
  logic [63:0]                       esr_ipi_trigger_full;
  logic [63:0]                       esr_ipi_redirect_trigger_full;
  shire_esr_pkg::uc_esr_enable_sigs_t uc_esr_enables_esr;
  shire_esr_pkg::uc_esr_values_t      uc_esr_values_esr;
  shire_uncached_pkg::uc_esr_enable_sigs_t uc_esr_enables_uncached;
  shire_uncached_pkg::uc_esr_values_t      uc_esr_values_uncached;

  shirecache_pkg::bank_esr_ctl_t [NumBanks-1:0]    bank_esr_ctl;
  shirecache_pkg::bank_esr_status_t [NumBanks-1:0] bank_esr_status;
  shire_esr_pkg::esr_sc_l3_shire_swizzle_ctl_t [NumBanks-1:0] bank_swizzle_unused;
  logic [NumBanks-1:0][7:0]                        bank_eco_unused;
  logic [7:0]                                      shire_eco_unused;
  logic [NumBanks-1:0]                             sc_bank_err_detected;
  logic [NumBanks-1:0]                             sc_bank_err_logged;
  logic                                             esr_sc_remote_l3_enable;
  logic                                             esr_sc_remote_scp_enable;
  shire_esr_pkg::esr_uc_config_t                    esr_uc_config;

  logic [NumPorts-1:0][NumBanks+NumUc-1:0]          cache_req_ready;
  logic [NumPorts-1:0][NumBanks+NumUc-1:0]          cache_req_valid;
  etlink_pkg::req_t [NumPorts-1:0]                  cache_req_info;
  logic [NumPorts-1:0]                              cache_rsp_ready;
  logic [NumPorts-1:0]                              cache_rsp_valid;
  etlink_pkg::rsp_t [NumPorts-1:0]                  cache_rsp_info;

  logic [NumNeigh-1:0] neigh_en;
  logic                rbox_en;
  logic                cache_en;
  logic [NumNeigh-1:0] rst_c_shire_raw_ni;
  logic [NumNeigh-1:0] rst_d_shire_raw_ni;
  logic [NumNeigh-1:0] rst_w_shire_raw_ni;
  logic                rst_c_sc_raw_ni;
  logic                rst_w_sc_raw_ni;
  logic                rst_d_sc_raw_ni;
  logic                rst_c_sc_no;
  logic                rst_w_sc_no;
  logic                rst_d_sc_no;
  logic                rst_d_rbox_raw_ni;
  logic                rst_w_rbox_raw_ni;
  logic                rst_d_rbox_no;
  logic [NumNeigh-1:0] rst_w_icache_sync_no;
  logic [NumNeigh-1:0] rst_icache_no;

  logic [1:0] dft_mbist_en_sync_q;
  logic dft_mbist_en_sync;

  shirecache_pkg::xbar_req_t neigh_uc_req_info;
  shirecache_pkg::xbar_rsp_t neigh_uc_rsp_info;
  logic                      neigh_uc_req_ready;
  logic                      neigh_uc_req_valid;
  logic [NumPorts-1:0]       neigh_uc_rsp_ready;
  logic [NumPorts-1:0]       neigh_uc_rsp_valid;

  shirecache_pkg::icache_bist_req_t icache_bist_req_info;
  shirecache_pkg::icache_bist_rsp_t icache_bist_rsp_info;
  etlink_pkg::rsp_t icache_neigh_rsp_info [NumNeigh-1:0];

  rbox_pkg::rc_t bpam_run_control_rbox;
  rbox_pkg::rc_ack_t [NumRbox-1:0] rbox_rc_ack;
  shire_esr_pkg::bpam_rc_rbox_ack_t bpam_rc_rbox_ack;
  logic [shire_esr_pkg::TboxPerShire:0] debug_rc_ss_tbox_rbox;
  logic [shire_esr_pkg::TboxPerShire-1:0] tbox_single_step;
  logic rbox_single_step;

  esr_pkg::esr_and_or_tree_l0_t esr_and_or_tree_l0_q [NumNeigh-1:0];
  shire_esr_pkg::esr_and_or_tree_l1_t esr_and_or_tree_l1_local;
  logic [NumNeigh-1:0] neigh_sc_err_detected_q;
  logic ioshire_log_err_int_d;

  apb_pkg::req_t [NumRbox-1:0] rbox_apb_req;
  apb_pkg::rsp_t [NumRbox-1:0] rbox_apb_rsp;
  logic [NumBanks-1:0] bank_apb_pready;
  logic [NumBanks-1:0][shire_esr_pkg::EsrApbDataWidth-1:0] bank_apb_prdata;
  logic [NumBanks-1:0] bank_apb_pslverr;
  logic shire_apb_pready;
  logic [shire_esr_pkg::EsrApbDataWidth-1:0] shire_apb_prdata;
  logic shire_apb_pslverr;
  logic icache_apb_pready;
  logic [minion_pkg::BpamShireApbDataWidth-1:0] icache_apb_prdata;
  logic icache_apb_pslverr;

  assign neigh_en = esr_shire_config.neigh_en[NumNeigh-1:0];
  assign rbox_en = esr_shire_config.rbox_en;
  assign cache_en = esr_shire_config.cache_en;

  for (genvar rst_idx = 0; rst_idx < NumNeigh; rst_idx++) begin : gen_neigh_resets
    assign rst_c_shire_raw_ni[rst_idx] = rst_c_ni & neigh_en[rst_idx];
    assign rst_d_shire_raw_ni[rst_idx] = rst_d_ni & neigh_en[rst_idx];
    assign rst_w_shire_raw_ni[rst_idx] = rst_w_ni & neigh_en[rst_idx];

    prim_rst_sync u_rst_c_shire (
      .clk_i  (clk_i),
      .rst_ni (rst_c_shire_raw_ni[rst_idx]),
      .dft_i  (dft_hv_i),
      .rst_no (rst_c_shire_no[rst_idx])
    );

    prim_rst_sync u_rst_d_shire (
      .clk_i  (clk_i),
      .rst_ni (rst_d_shire_raw_ni[rst_idx]),
      .dft_i  (dft_hv_i),
      .rst_no (rst_d_shire_no[rst_idx])
    );

    prim_rst_sync u_rst_w_shire (
      .clk_i  (clk_i),
      .rst_ni (rst_w_shire_raw_ni[rst_idx]),
      .dft_i  (dft_hv_i),
      .rst_no (rst_w_shire_no[rst_idx])
    );

    prim_rst_sync u_rst_c_shire_scs (
      .clk_i  (clk_i),
      .rst_ni (rst_c_shire_raw_ni[rst_idx]),
      .dft_i  (dft_hv_i),
      .rst_no (rst_c_shire_scs_no[rst_idx])
    );

    prim_rst_sync u_rst_w_icache (
      .clk_i  (clk_i),
      .rst_ni (rst_w_icache_ni[rst_idx]),
      .dft_i  (dft_hv_i),
      .rst_no (rst_w_icache_sync_no[rst_idx])
    );

    assign rst_icache_no[rst_idx] = rst_w_icache_sync_no[rst_idx] & rst_w_ni;
  end

  assign rst_c_sc_raw_ni = rst_c_ni & cache_en;
  assign rst_w_sc_raw_ni = rst_w_ni & cache_en;
  assign rst_d_sc_raw_ni = rst_d_ni & cache_en;
  assign rst_sc_no = rst_c_sc_raw_ni;
  assign rst_d_rbox_raw_ni = rst_d_ni & rbox_en;
  assign rst_w_rbox_raw_ni = rst_w_ni & rbox_en;

  prim_rst_sync u_rst_c_sc (
    .clk_i  (clk_i),
    .rst_ni (rst_c_sc_raw_ni),
    .dft_i  (dft_hv_i),
    .rst_no (rst_c_sc_no)
  );

  prim_rst_sync u_rst_w_sc (
    .clk_i  (clk_i),
    .rst_ni (rst_w_sc_raw_ni),
    .dft_i  (dft_hv_i),
    .rst_no (rst_w_sc_no)
  );

  prim_rst_sync u_rst_d_sc (
    .clk_i  (clk_i),
    .rst_ni (rst_d_sc_raw_ni),
    .dft_i  (dft_hv_i),
    .rst_no (rst_d_sc_no)
  );

  prim_rst_sync u_rst_d_rbox (
    .clk_i  (clk_i),
    .rst_ni (rst_d_rbox_raw_ni),
    .dft_i  (dft_hv_i),
    .rst_no (rst_d_rbox_no)
  );

  prim_rst_sync u_rst_w_rbox (
    .clk_i  (clk_i),
    .rst_ni (rst_w_rbox_raw_ni),
    .dft_i  (dft_hv_i),
    .rst_no (rst_rbox_no)
  );

  always_ff @(posedge clk_i or negedge rst_c_sc_no) begin
    if (!rst_c_sc_no) begin
      dft_mbist_en_sync_q <= '0;
    end else begin
      dft_mbist_en_sync_q <= {dft_mbist_en_sync_q[0], dft_mbist_en_i};
    end
  end
  assign dft_mbist_en_sync = dft_mbist_en_sync_q[1];

  for (genvar bank_idx = 0; bank_idx < NumBanks; bank_idx++) begin : gen_bank_esr
    esr_cache_bank #(
      .AdWidth         (shire_esr_pkg::EsrCacheBankApbPpAddrWidth),
      .SetsPerSubBank  (SetsPerSubBankParam),
      .NumReqqEntries  (NumBankReqqEntries)
    ) u_esr_cache_bank (
      .clk_i            (clk_i),
      .rst_c_ni         (rst_c_ni),
      .rst_w_ni         (rst_w_ni),
      .rst_d_ni         (rst_d_ni),
      .test_en_i        (1'b0),
      .esr_ctl_o        (bank_esr_ctl[bank_idx]),
      .esr_status_i     (bank_esr_status[bank_idx]),
      .esr_sc_l3_shire_swizzle_ctl_o (bank_swizzle_unused[bank_idx]),
      .esr_sc_eco_ctl_o (bank_eco_unused[bank_idx]),
      .apb_paddr_i      (apb_req_ff[ApbBanksIdxStart + bank_idx].paddr[
                         shire_esr_pkg::EsrCacheBankApbPpAddrWidth-1:0]),
      .apb_pwrite_i     (apb_req_ff[ApbBanksIdxStart + bank_idx].pwrite),
      .apb_psel_i       (apb_req_ff[ApbBanksIdxStart + bank_idx].psel),
      .apb_penable_i    (apb_req_ff[ApbBanksIdxStart + bank_idx].penable),
      .apb_pwdata_i     (apb_req_ff[ApbBanksIdxStart + bank_idx].pwdata),
      .apb_pready_o     (bank_apb_pready[bank_idx]),
      .apb_prdata_o     (bank_apb_prdata[bank_idx]),
      .apb_pslverr_o    (bank_apb_pslverr[bank_idx])
    );
  end

  always_comb begin
    uc_esr_enables_uncached.barriers = uc_esr_enables_esr.barriers;
    uc_esr_enables_uncached.fcc = uc_esr_enables_esr.fcc;
    uc_esr_values_esr.barriers = uc_esr_values_uncached.barriers;
  end

  esr_shire_other #(
    .NumBanks (NumBanks),
    .NumNeigh (NumNeigh),
    .AdWidth  (shire_esr_pkg::EsrShireOtherApbPpAddrWidth)
  ) u_esr_shire_other (
    .clk_i                                (clk_i),
    .rst_c_ni                             (rst_c_ni),
    .rst_w_ni                             (rst_w_ni),
    .rst_d_ni                             (rst_d_ni),
    .test_en_i                            (1'b0),
    .dft_i                                (dft_hv_i),
    .shire_id_reset_val_i                 (shire_id_reset_val_i),
    .esr_minion_feature_o                 (esr_minion_features),
    .esr_shire_config_o                   (esr_shire_config),
    .esr_shire_cache_build_config_i       (64'h0404_0004_0404_0000),
    .esr_shire_cache_revision_id_i        ({16'h0, shire_id_reset_val_i, shire_phy_id_i, 32'h0000_0001}),
    .esr_ipi_trigger_o                    (esr_ipi_trigger_full),
    .esr_and_or_tree_l1_i                 (esr_and_or_tree_l1_local),
    .esr_mtime_local_target_o             (esr_mtime_local_target_o),
    .esr_power_ctrl_neigh_nsleepout_i     (esr_pwr_ctrl_neigh_nsleepout_i),
    .esr_shire_coop_mode_o                (esr_shire_coop_mode),
    .esr_shire_noc_interrupt_status_i     (noc_all_err_int_srcs_o),
    .esr_uc_config_o                      (esr_uc_config),
    .esr_clk_gate_ctrl_o                  (esr_clk_gate_ctrl_o),
    .esr_debug_clk_gate_ctrl_o            (esr_debug_clk_gate_ctrl_o),
    .esr_shire_channel_eco_ctl_o          (shire_eco_unused),
    .esr_clk_dly_ctl_o                    (esr_clk_dly_ctl_o),
    .esr_icache_prefetch_conf_o           (esr_icache_prefetch_conf),
    .esr_icache_prefetch_start_o          (esr_icache_prefetch_start_o),
    .esr_icache_prefetch_done_i           (esr_icache_prefetch_done_i),
    .esr_thread0_enable_o                 (esr_thread0_enable_o),
    .esr_thread1_enable_o                 (esr_thread1_enable_o),
    .neigh_sc_err_detected_i              (neigh_sc_err_detected_i),
    .neigh_sc_err_logged_i                (neigh_sc_err_logged_i),
    .sc_bank_err_detected_i               (sc_bank_err_detected),
    .sc_bank_err_logged_i                 (sc_bank_err_logged),
    .esr_ipi_redirect_trigger_o           (esr_ipi_redirect_trigger_full),
    .uc_esr_values_i                      (uc_esr_values_esr),
    .uc_esr_enables_o                     (uc_esr_enables_esr),
    .esr_power_ctrl_glb_nsleepin_o        (esr_pwr_ctrl_glb_nsleepin_o),
    .esr_power_ctrl_glb_iso_o             (esr_pwr_ctrl_glb_iso_o),
    .esr_power_ctrl_glb_nsleepout_i       (esr_pwr_ctrl_glb_nsleepout_i),
    .esr_power_ctrl_neigh_nsleepin_o      (esr_pwr_ctrl_neigh_nsleepin_o),
    .esr_power_ctrl_neigh_iso_o           (esr_pwr_ctrl_neigh_iso_o),
    .esr_pll_ctrl_o                       (esr_pll_ctrl_o),
    .esr_pll_conf_o                       (esr_pll_conf_o),
    .esr_pll_busy_i                       (esr_pll_busy_i),
    .esr_pll_rdata_i                      (esr_pll_rdata_i),
    .esr_pll_rrdy_i                       (esr_pll_rrdy_i),
    .esr_pll_lock_i                       (esr_pll_lock_i),
    .esr_dll_ctrl_o                       (esr_dll_ctrl_o),
    .esr_dll_conf_o                       (esr_dll_conf_o),
    .esr_dll_busy_i                       (esr_dll_busy_i),
    .esr_dll_rdata_i                      (esr_dll_rdata_i),
    .esr_dll_rrdy_i                       (esr_dll_rrdy_i),
    .esr_dll_lock_i                       (esr_dll_lock_i),
    .esr_shire_ctrl_clockmux_o            (esr_shire_ctrl_clockmux_o),
    .esr_dll_dly_est_ctl_o                (esr_dll_dly_est_ctl_o),
    .esr_dll_dly_est_sts_i                (esr_dll_dly_est_sts_i),
    .ram_cfg_o                            (ram_cfg_o),
    .debug_rc_tbox_ack_i                  (bpam_rc_tbox_ack_i),
    .debug_rc_rbox_ack_i                  (bpam_rc_rbox_ack),
    .debug_rc_ss_tbox_rbox_o              (debug_rc_ss_tbox_rbox),
    .apb_paddr_i                          (apb_req_ff[ApbShireIdx].paddr[
                                            shire_esr_pkg::EsrShireOtherApbPpAddrWidth-1:0]),
    .apb_pwrite_i                         (apb_req_ff[ApbShireIdx].pwrite),
    .apb_psel_i                           (apb_req_ff[ApbShireIdx].psel),
    .apb_penable_i                        (apb_req_ff[ApbShireIdx].penable),
    .apb_pwdata_i                         (apb_req_ff[ApbShireIdx].pwdata),
    .apb_pready_o                         (shire_apb_pready),
    .apb_prdata_o                         (shire_apb_prdata),
    .apb_pslverr_o                        (shire_apb_pslverr)
  );

  assign tbox_single_step = debug_rc_ss_tbox_rbox[shire_esr_pkg::TboxPerShire-1:0];
  assign rbox_single_step = debug_rc_ss_tbox_rbox[shire_esr_pkg::TboxPerShire];

  shire_bpam_run_control #(
    .NumNeigh (NumNeigh)
  ) u_bpam_run_control (
    .bpam_run_control_i       (bpam_run_control_i),
    .tbox_single_step_i       (tbox_single_step[NumNeigh-1:0]),
    .rbox_single_step_i       (rbox_single_step),
    .bpam_run_control_neigh_o (bpam_run_control_neigh_o),
    .bpam_run_control_rbox_o  (bpam_run_control_rbox)
  );

  always_comb begin
    bpam_rc_rbox_ack = '0;
    for (int unsigned rb = 0; rb < NumRbox; rb++) begin
      bpam_rc_rbox_ack.reset_ack |= rbox_rc_ack[rb].reset_ack;
      bpam_rc_rbox_ack.resume_ack |= rbox_rc_ack[rb].resume_ack;
      bpam_rc_rbox_ack.halt_ack |= rbox_rc_ack[rb].halt_ack;
    end
  end

  etlink_pkg::req_t [NumRbox-1:0] rbox_sc_req_info;
  logic [NumRbox-1:0][NumBanks+NumUc-1:0] rbox_sc_req_valid;
  logic [NumRbox-1:0] rbox_sc_rsp_ready;

  for (genvar rbox_idx = 0; rbox_idx < NumRbox; rbox_idx++) begin : gen_rbox
    always_comb begin
      rbox_apb_req[rbox_idx].paddr =
          apb_req_ff[ApbRboxIdxStart + rbox_idx].paddr[apb_pkg::DefaultAddrWidth-1:0];
      rbox_apb_req[rbox_idx].pwrite = apb_req_ff[ApbRboxIdxStart + rbox_idx].pwrite;
      rbox_apb_req[rbox_idx].psel = apb_req_ff[ApbRboxIdxStart + rbox_idx].psel;
      rbox_apb_req[rbox_idx].penable = apb_req_ff[ApbRboxIdxStart + rbox_idx].penable;
      rbox_apb_req[rbox_idx].pwdata = apb_req_ff[ApbRboxIdxStart + rbox_idx].pwdata;
    end

    rbox_top u_rbox_top (
      .clk_i                 (clk_i),
      .rst_d_ni              (rst_d_rbox_no),
      .rst_w_ni              (rst_rbox_no),
      .shire_id_i            (esr_shire_config.shire_id),
      .rbox_sc_req_o         (rbox_sc_req_info[rbox_idx]),
      .rbox_sc_req_valid_o   (rbox_sc_req_valid[rbox_idx]),
      .rbox_sc_req_ready_i   (cache_req_ready[NumNeigh + rbox_idx]),
      .sc_rbox_rsp_i         (cache_rsp_info[NumNeigh + rbox_idx]),
      .sc_rbox_rsp_valid_i   (cache_rsp_valid[NumNeigh + rbox_idx]),
      .sc_rbox_rsp_ready_o   (rbox_sc_rsp_ready[rbox_idx]),
      .rc_i                  (bpam_run_control_rbox),
      .rc_ack_o              (rbox_rc_ack[rbox_idx]),
      .apb_req_i             (rbox_apb_req[rbox_idx]),
      .apb_rsp_o             (rbox_apb_rsp[rbox_idx]),
      .ram_cfg_i             (ram_cfg_o),
      .dft_i                 (dft_hv_i)
    );
  end

  for (genvar neigh_conn = 0; neigh_conn < NumNeigh; neigh_conn++) begin : gen_neigh_cache_conn
    assign neigh_sc_req_ready_o[neigh_conn] = cache_req_ready[neigh_conn];
    assign cache_req_valid[neigh_conn] = neigh_sc_req_valid_i[neigh_conn];
    assign cache_req_info[neigh_conn] = neigh_sc_req_info_i[neigh_conn];
    assign cache_rsp_ready[neigh_conn] = neigh_sc_rsp_ready_i[neigh_conn];
    assign neigh_sc_rsp_valid_o[neigh_conn] = cache_rsp_valid[neigh_conn];
    assign neigh_sc_rsp_info_o[neigh_conn] = cache_rsp_info[neigh_conn];
  end

  for (genvar rb_conn = 0; rb_conn < NumRbox; rb_conn++) begin : gen_rbox_conn
    assign cache_req_valid[NumNeigh + rb_conn] = rbox_sc_req_valid[rb_conn];
    assign cache_req_info[NumNeigh + rb_conn] = rbox_sc_req_info[rb_conn];
    assign cache_rsp_ready[NumNeigh + rb_conn] = rbox_sc_rsp_ready[rb_conn];
  end

  shirecache_top #(
    .NumPorts              (NumPorts),
    .NumBanks              (NumBanks),
    .NumL3MasterPorts      (NumL3MasterPorts),
    .NumL3SlavePorts       (NumL3SlavePorts),
    .NumSysPorts           (NumSysPorts),
    .NumUc                 (NumUc),
    .NumBankReqqEntries    (NumBankReqqEntries),
    .SetsPerSubBankParam   (SetsPerSubBankParam),
    .UseNocClkGating       (UseNocClkGating),
    .IcacheMemsImplemented (IcacheMemsImplemented),
    .L2hpfImplemented      (L2hpfImplemented)
  ) u_shirecache_top (
    .clk_i                        (clk_i),
    .clk_free_i                   (clk_i),
    .rst_cold_ni                  (rst_c_sc_no),
    .rst_ni                       (rst_w_sc_no),
    .rst_debug_ni                 (rst_d_sc_no),
    .noc_clk_i                    (noc_clk_i),
    .noc_rst_ni                   (noc_rst_ni),
    .dft_i                        (dft_hv_i),
    .dft_sram_clk_i               (dft_sram_clk_i),
    .dft_mbist_en_i               (dft_mbist_en_sync),
    .ram_cfg_i                    (ram_cfg_o),
    .neigh_sc_req_ready_o         (cache_req_ready),
    .neigh_sc_req_valid_i         (cache_req_valid),
    .neigh_sc_req_info_i          (cache_req_info),
    .neigh_sc_rsp_ready_i         (cache_rsp_ready),
    .neigh_sc_rsp_valid_o         (cache_rsp_valid),
    .neigh_sc_rsp_info_o          (cache_rsp_info),
    .neigh_uc_req_ready_i         (neigh_uc_req_ready),
    .neigh_uc_req_valid_o         (neigh_uc_req_valid),
    .neigh_uc_req_info_o          (neigh_uc_req_info),
    .neigh_uc_rsp_ready_o         (neigh_uc_rsp_ready),
    .neigh_uc_rsp_valid_i         (neigh_uc_rsp_valid),
    .neigh_uc_rsp_info_i          (neigh_uc_rsp_info),
    .bank_esr_ctl_i               (bank_esr_ctl),
    .bank_esr_status_o            (bank_esr_status),
    .esr_sc_remote_l3_enable_o    (esr_sc_remote_l3_enable),
    .esr_sc_remote_scp_enable_o   (esr_sc_remote_scp_enable),
    .esr_shire_config_i           (esr_shire_config),
    .sc_bank_err_detected_o       (sc_bank_err_detected),
    .sc_bank_err_logged_o         (sc_bank_err_logged),
    .sc_neigh_l2hpf_req_valid_o   (sc_neigh_l2hpf_req_valid_o),
    .sc_neigh_l2hpf_req_info_o    (sc_neigh_l2hpf_req_info_o),
    .status_monitor_bank_sel_i    (status_monitor_bank_sel_i),
    .sc_trace_data_o              (sc_trace_data_o),
    .sc_trace_valid_o             (sc_trace_valid_o),
    .to_l3_axi_ar_ready_i         (to_l3_axi_ar_ready_i),
    .to_l3_axi_ar_valid_o         (to_l3_axi_ar_valid_o),
    .to_l3_axi_ar_o               (to_l3_axi_ar_o),
    .to_l3_axi_aw_ready_i         (to_l3_axi_aw_ready_i),
    .to_l3_axi_aw_valid_o         (to_l3_axi_aw_valid_o),
    .to_l3_axi_aw_o               (to_l3_axi_aw_o),
    .to_l3_axi_w_ready_i          (to_l3_axi_w_ready_i),
    .to_l3_axi_w_valid_o          (to_l3_axi_w_valid_o),
    .to_l3_axi_w_o                (to_l3_axi_w_o),
    .to_l3_axi_r_ready_o          (to_l3_axi_r_ready_o),
    .to_l3_axi_r_valid_i          (to_l3_axi_r_valid_i),
    .to_l3_axi_r_i                (to_l3_axi_r_i),
    .to_l3_axi_b_ready_o          (to_l3_axi_b_ready_o),
    .to_l3_axi_b_valid_i          (to_l3_axi_b_valid_i),
    .to_l3_axi_b_i                (to_l3_axi_b_i),
    .to_sys_axi_ar_ready_i        (to_sys_axi_ar_ready_i),
    .to_sys_axi_ar_valid_o        (to_sys_axi_ar_valid_o),
    .to_sys_axi_ar_o              (to_sys_axi_ar_o),
    .to_sys_axi_aw_ready_i        (to_sys_axi_aw_ready_i),
    .to_sys_axi_aw_valid_o        (to_sys_axi_aw_valid_o),
    .to_sys_axi_aw_o              (to_sys_axi_aw_o),
    .to_sys_axi_w_ready_i         (to_sys_axi_w_ready_i),
    .to_sys_axi_w_valid_o         (to_sys_axi_w_valid_o),
    .to_sys_axi_w_o               (to_sys_axi_w_o),
    .to_sys_axi_r_ready_o         (to_sys_axi_r_ready_o),
    .to_sys_axi_r_valid_i         (to_sys_axi_r_valid_i),
    .to_sys_axi_r_i               (to_sys_axi_r_i),
    .to_sys_axi_b_ready_o         (to_sys_axi_b_ready_o),
    .to_sys_axi_b_valid_i         (to_sys_axi_b_valid_i),
    .to_sys_axi_b_i               (to_sys_axi_b_i),
    .l3_axi_ar_ready_o            (l3_axi_ar_ready_o),
    .l3_axi_ar_valid_i            (l3_axi_ar_valid_i),
    .l3_axi_ar_i                  (l3_axi_ar_i),
    .l3_axi_aw_ready_o            (l3_axi_aw_ready_o),
    .l3_axi_aw_valid_i            (l3_axi_aw_valid_i),
    .l3_axi_aw_i                  (l3_axi_aw_i),
    .l3_axi_w_ready_o             (l3_axi_w_ready_o),
    .l3_axi_w_valid_i             (l3_axi_w_valid_i),
    .l3_axi_w_i                   (l3_axi_w_i),
    .l3_axi_r_ready_i             (l3_axi_r_ready_i),
    .l3_axi_r_valid_o             (l3_axi_r_valid_o),
    .l3_axi_r_o                   (l3_axi_r_o),
    .l3_axi_b_ready_i             (l3_axi_b_ready_i),
    .l3_axi_b_valid_o             (l3_axi_b_valid_o),
    .l3_axi_b_o                   (l3_axi_b_o),
    .icache_bist_req_info_o       (icache_bist_req_info),
    .icache_bist_rsp_info_i       (icache_bist_rsp_info)
  );

  shirecache_pkg::xbar_rsp_t uc_rsp_info;
  assign neigh_uc_rsp_info = uc_rsp_info;

  uncacheable #(
    .NumNeighP (NumNeigh),
    .NumPortsP (NumPorts)
  ) u_uncacheable (
    .clk_i                         (clk_i),
    .rst_ni                        (rst_w_ni),
    .noc_clk_i                     (noc_clk_i),
    .noc_rst_ni                    (noc_rst_ni),
    .shire_id_i                    (esr_shire_config.shire_id),
    .esr_enables_i                 (uc_esr_enables_uncached),
    .esr_wdata_i                   (apb_req_ff[ApbShireIdx].pwdata),
    .esr_values_o                  (uc_esr_values_uncached),
    .esr_uc_config_i               (esr_uc_config),
    .flb_neigh_l2_req_valid_i      (flb_neigh_l2_req_valid_i),
    .flb_neigh_l2_req_data_i       (flb_neigh_l2_req_data_i),
    .flb_l2_neigh_resp_valid_o     (flb_l2_neigh_resp_valid_o),
    .flb_l2_neigh_resp_data_o      (flb_l2_neigh_resp_data_o),
    .uc_to_neigh_fcc_o             (uc_to_neigh_fcc_o),
    .uc_to_neigh_fcc_target_o      (uc_to_neigh_fcc_target_o[0]),
    .neigh_uc_req_ready_o          (neigh_uc_req_ready),
    .neigh_uc_req_valid_i          (neigh_uc_req_valid),
    .neigh_uc_req_info_i           (neigh_uc_req_info),
    .neigh_uc_min_rsp_ready_i      (neigh_uc_rsp_ready),
    .neigh_uc_min_rsp_valid_o      (neigh_uc_rsp_valid),
    .neigh_uc_min_rsp_info_o       (uc_rsp_info),
    .sbm_enable_read_o             (sbm_enable_read_o),
    .sbm_enable_write_o            (sbm_enable_write_o),
    .sbm_write_credit_return_i     (sbm_write_credit_return_i),
    .sbm_sys_axi_ar_o              (sbm_sys_axi_ar_o),
    .sbm_sys_axi_ar_ready_i        (sbm_sys_axi_ar_ready_i),
    .sbm_sys_axi_aw_o              (sbm_sys_axi_aw_o),
    .sbm_sys_axi_aw_ready_i        (sbm_sys_axi_aw_ready_i),
    .sbm_sys_axi_w_o               (sbm_sys_axi_w_o),
    .sbm_sys_axi_w_ready_i         (sbm_sys_axi_w_ready_i),
    .sbm_sys_axi_b_i               (sbm_sys_axi_b_i),
    .sbm_sys_axi_b_valid_i         (sbm_sys_axi_b_valid_i),
    .sbm_sys_axi_b_ready_o         (sbm_sys_axi_b_ready_o),
    .sbm_sys_axi_r_i               (sbm_sys_axi_r_i),
    .sbm_sys_axi_r_valid_i         (sbm_sys_axi_r_valid_i),
    .sbm_sys_axi_r_ready_o         (sbm_sys_axi_r_ready_o),
    .l3_enabled_i                  (esr_sc_remote_l3_enable),
    .remote_scp_enabled_i          (esr_sc_remote_scp_enable),
    .sys_axi_ar_i                  (sys_axi_ar_i),
    .sys_axi_ar_valid_i            (sys_axi_ar_valid_i),
    .sys_axi_ar_ready_o            (sys_axi_ar_ready_o),
    .sys_axi_aw_i                  (sys_axi_aw_i),
    .sys_axi_aw_valid_i            (sys_axi_aw_valid_i),
    .sys_axi_aw_ready_o            (sys_axi_aw_ready_o),
    .sys_axi_w_i                   (sys_axi_w_i),
    .sys_axi_w_valid_i             (sys_axi_w_valid_i),
    .sys_axi_w_ready_o             (sys_axi_w_ready_o),
    .sys_axi_b_o                   (sys_axi_b_o),
    .sys_axi_b_valid_o             (sys_axi_b_valid_o),
    .sys_axi_b_ready_i             (sys_axi_b_ready_i),
    .sys_axi_r_o                   (sys_axi_r_o),
    .sys_axi_r_valid_o             (sys_axi_r_valid_o),
    .sys_axi_r_ready_i             (sys_axi_r_ready_i),
    .sys_axi_aw_vcvalid_i          (sys_axi_aw_vcvalid_i),
    .sys_axi_aw_credit_o           (sys_axi_aw_credit_o),
    .sys_axi_w_vcvalid_i           (sys_axi_w_vcvalid_i),
    .to_l3_ar_o                    (uc_to_l3_axi_ar_o),
    .to_l3_ar_valid_o              (uc_to_l3_axi_ar_valid_o),
    .to_l3_ar_ready_i              (uc_to_l3_axi_ar_ready_i),
    .to_l3_aw_o                    (uc_to_l3_axi_aw_o),
    .to_l3_aw_valid_o              (uc_to_l3_axi_aw_valid_o),
    .to_l3_aw_ready_i              (uc_to_l3_axi_aw_ready_i),
    .to_l3_w_o                     (uc_to_l3_axi_w_o),
    .to_l3_w_valid_o               (uc_to_l3_axi_w_valid_o),
    .to_l3_w_ready_i               (uc_to_l3_axi_w_ready_i),
    .to_l3_b_i                     (uc_to_l3_axi_b_i),
    .to_l3_b_valid_i               (uc_to_l3_axi_b_valid_i),
    .to_l3_b_ready_o               (uc_to_l3_axi_b_ready_o),
    .to_l3_r_i                     (uc_to_l3_axi_r_i),
    .to_l3_r_valid_i               (uc_to_l3_axi_r_valid_i),
    .to_l3_r_ready_o               (uc_to_l3_axi_r_ready_o),
    .to_sys_ar_o                   (uc_to_sys_axi_ar_o),
    .to_sys_ar_valid_o             (uc_to_sys_axi_ar_valid_o),
    .to_sys_ar_ready_i             (uc_to_sys_axi_ar_ready_i),
    .to_sys_aw_o                   (uc_to_sys_axi_aw_o),
    .to_sys_aw_valid_o             (uc_to_sys_axi_aw_valid_o),
    .to_sys_aw_ready_i             (uc_to_sys_axi_aw_ready_i),
    .to_sys_w_o                    (uc_to_sys_axi_w_o),
    .to_sys_w_valid_o              (uc_to_sys_axi_w_valid_o),
    .to_sys_w_ready_i              (uc_to_sys_axi_w_ready_i),
    .to_sys_b_i                    (uc_to_sys_axi_b_i),
    .to_sys_b_valid_i              (uc_to_sys_axi_b_valid_i),
    .to_sys_b_ready_o              (uc_to_sys_axi_b_ready_o),
    .to_sys_r_i                    (uc_to_sys_axi_r_i),
    .to_sys_r_valid_i              (uc_to_sys_axi_r_valid_i),
    .to_sys_r_ready_o              (uc_to_sys_axi_r_ready_o),
    .dft_hv_i                      (dft_hv_i),
    .dft_lv_i                      (dft_lv_i)
  );

  for (genvar fcc_target_idx = 1; fcc_target_idx < NumNeigh; fcc_target_idx++) begin : gen_fcc_target_rep
    assign uc_to_neigh_fcc_target_o[fcc_target_idx] = uc_to_neigh_fcc_target_o[0];
  end

  for (genvar ic_rsp_idx = 0; ic_rsp_idx < NumNeigh; ic_rsp_idx++) begin : gen_icache_rsp_info
    assign icache_neigh_rsp_info[ic_rsp_idx] = cache_rsp_info[ic_rsp_idx];
  end

  shire_ioshire_noc_ints #(
    .NocIntNum (shire_channel_leaves_pkg::NocIntNum)
  ) u_noc_ints (
    .clk_noc_i              (noc_clk_i),
    .rst_noc_ni             (noc_rst_ni),
    .noc_err_int_srcs_i     (noc_err_int_srcs_i),
    .noc_all_err_int_srcs_o (noc_all_err_int_srcs_o),
    .ioshire_noc_err_int_o  (ioshire_noc_err_int_o)
  );

  always_ff @(posedge clk_i or negedge rst_c_ni) begin
    if (!rst_c_ni) begin
      esr_and_or_tree_l0_q <= '{default: '0};
      neigh_sc_err_detected_q <= '0;
      ioshire_log_err_int_o <= 1'b0;
    end else begin
      esr_and_or_tree_l0_q <= esr_and_or_tree_l0_i;
      neigh_sc_err_detected_q <= neigh_sc_err_detected_i;
      ioshire_log_err_int_o <= ioshire_log_err_int_d;
    end
  end

  assign ioshire_log_err_int_d = (|neigh_sc_err_detected_q) | (|sc_bank_err_detected);

  shire_and_or_tree_daisychain #(
    .NumNeigh     (NumNeigh),
    .ShireIdWidth (shire_esr_pkg::NumShireIdsBits)
  ) u_and_or_tree (
    .shire_id_i                  (esr_shire_config.shire_id),
    .debug_and_or_tree_l2_i      (debug_and_or_tree_l2_i),
    .debug_and_or_tree_l2_o      (debug_and_or_tree_l2_o),
    .esr_and_or_tree_l0_i        (esr_and_or_tree_l0_q),
    .esr_and_or_tree_l1_local_o  (esr_and_or_tree_l1_local)
  );

  generate
    if (IcacheMemsImplemented) begin : gen_icache_mems
      icache_mems #(
        .NumNeigh (NumNeigh)
      ) u_icache_mems (
        .clk_i                   (clk_i),
        .rst_ni                  (rst_icache_no),
        .dft_sram_clk_i          (dft_sram_clk_i),
        .dft_i                   (dft_hv_i),
        .icache_req_write_i      (icache_req_write_i),
        .icache_req_addr_i       (icache_req_addr_i),
        .icache_req_valid_i      (icache_req_valid_i),
        .icache_req_ready_o      (icache_req_ready_o),
        .icache_resp_dout_o      (icache_resp_dout_o),
        .icache_resp_valid_o     (icache_resp_valid_o),
        .icache_resp_ready_i     (icache_resp_ready_i),
        .neigh_sc_rsp_info_i     (icache_neigh_rsp_info),
        .neigh_sc_rsp_valid_i    (cache_rsp_valid[NumNeigh-1:0]),
        .neigh_sc_rsp_ready_i    (cache_rsp_ready[NumNeigh-1:0]),
        .ram_cfg_i               (ram_cfg_o),
        .bist_req_info_i         (icache_bist_req_info),
        .bist_rsp_info_o         (icache_bist_rsp_info),
        .apb_paddr_i             (apb_req_ff[ApbIcacheIdx].paddr[
                                   icache_pkg::IcacheMemsApbAddrWidth-1:0]),
        .apb_pwrite_i            (apb_req_ff[ApbIcacheIdx].pwrite),
        .apb_psel_i              (apb_req_ff[ApbIcacheIdx].psel),
        .apb_penable_i           (apb_req_ff[ApbIcacheIdx].penable),
        .apb_pwdata_i            (apb_req_ff[ApbIcacheIdx].pwdata),
        .apb_pready_o            (icache_apb_pready),
        .apb_prdata_o            (icache_apb_prdata),
        .apb_pslverr_o           (icache_apb_pslverr)
      );
    end else begin : gen_no_icache_mems
      assign icache_req_ready_o = '0;
      assign icache_resp_dout_o = '0;
      assign icache_resp_valid_o = '0;
      assign icache_bist_rsp_info = '0;
      assign icache_apb_pready = 1'b1;
      assign icache_apb_prdata = '0;
      assign icache_apb_pslverr = 1'b1;
    end
  endgenerate

  shire_coop_tload_bus #(
    .NumNeigh      (NumNeigh),
    .NumOtherNeigh (NumOtherNeigh),
    .CoopIdWidth   (neigh_pkg::ShireCoopIdSize)
  ) u_coop_tload_bus (
    .coop_tload_slv_rdy_out_data_i        (coop_tload_slv_rdy_out_data_i),
    .coop_tload_slv_rdy_out_valid_i       (coop_tload_slv_rdy_out_valid_i),
    .coop_tload_slv_rdy_in_data_o         (coop_tload_slv_rdy_in_data_o),
    .coop_tload_slv_rdy_in_valid_o        (coop_tload_slv_rdy_in_valid_o),
    .coop_tload_mst_done_out_coop_id_i    (coop_tload_mst_done_out_coop_id_i),
    .coop_tload_mst_done_out_valid_i      (coop_tload_mst_done_out_valid_i),
    .coop_tload_mst_done_in_coop_id_o     (coop_tload_mst_done_in_coop_id_o),
    .coop_tload_mst_done_in_valid_o       (coop_tload_mst_done_in_valid_o)
  );

  always_comb begin
    for (int unsigned n = 0; n < NumNeigh; n++) begin
      shire_id_o[n] = esr_shire_config.shire_id;
      esr_minion_features_o[n] = esr_minion_features;
      esr_icache_prefetch_conf_o[n] = esr_icache_prefetch_conf;
      esr_shire_coop_mode_o[n] = esr_shire_coop_mode;
    end

    tbox_id_o[0] = esr_shire_config.tbox0_id;
    tbox_id_o[1] = esr_shire_config.tbox1_id;
    tbox_id_o[2] = esr_shire_config.tbox2_id;
    tbox_id_o[3] = esr_shire_config.tbox3_id;
    tbox_en_o = esr_shire_config.tbox_en[shire_esr_pkg::TboxPerShire-1:0];

    esr_ipi_trigger_o = esr_ipi_trigger_full[shire_esr_pkg::ThreadsPerShire-1:0];
    esr_ipi_redirect_trigger_o =
        esr_ipi_redirect_trigger_full[shire_esr_pkg::ThreadsPerShire-1:0];

    apb_rsp_target = '0;
    for (int unsigned b = 0; b < NumBanks; b++) begin
      apb_rsp_target[ApbBanksIdxStart + b].pready = bank_apb_pready[b];
      apb_rsp_target[ApbBanksIdxStart + b].prdata = bank_apb_prdata[b];
      apb_rsp_target[ApbBanksIdxStart + b].pslverr = bank_apb_pslverr[b];
    end
    apb_rsp_target[ApbShireIdx].pready = shire_apb_pready;
    apb_rsp_target[ApbShireIdx].prdata = shire_apb_prdata;
    apb_rsp_target[ApbShireIdx].pslverr = shire_apb_pslverr;
    for (int unsigned r = 0; r < NumRbox; r++) begin
      apb_rsp_target[ApbRboxIdxStart + r].pready = rbox_apb_rsp[r].pready;
      apb_rsp_target[ApbRboxIdxStart + r].prdata = rbox_apb_rsp[r].prdata;
      apb_rsp_target[ApbRboxIdxStart + r].pslverr = rbox_apb_rsp[r].pslverr;
    end
    if (IcacheMemsImplemented) begin
      apb_rsp_target[ApbIcacheIdx].pready = icache_apb_pready;
      apb_rsp_target[ApbIcacheIdx].prdata = icache_apb_prdata;
      apb_rsp_target[ApbIcacheIdx].pslverr = icache_apb_pslverr;
    end
  end

endmodule : shire_channel
/* verilator lint_on VARHIDDEN */
/* verilator lint_on UNOPTFLAT */
/* verilator lint_on UNUSEDSIGNAL */
