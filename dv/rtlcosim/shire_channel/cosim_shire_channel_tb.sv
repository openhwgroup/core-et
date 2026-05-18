// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */  // Full container cosim ties off many third-party-omitted sidebands.
/* verilator lint_off WIDTHEXPAND */    // Original macro widths are adapted to native package widths.
/* verilator lint_off WIDTHTRUNC */     // Original macro widths are adapted to native package widths.
/* verilator lint_off PINCONNECTEMPTY */  // Removed third-party/TDR/SMS outputs are intentionally unobserved.
module cosim_shire_channel_tb
  import axi_pkg::*;
  import dft_pkg::*;
  import etlink_pkg::*;
  import icache_pkg::*;
  import ram_cfg_pkg::*;
  import shire_channel_leaves_pkg::*;
  import shire_esr_pkg::*;
  import shire_sbm_pkg::*;
  import shire_uncached_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,

  input  logic rst_c_ext_ni,
  input  logic rst_w_ext_ni,
  input  logic rst_d_ext_ni,
  input  logic rst_noc_ext_ni,
  input  logic [shire_esr_pkg::NumNeigh-1:0] rst_w_icache_ext_ni,

  input  logic dft_scanmode_i,
  input  logic dft_scan_reset_ni,
  input  logic dft_sram_clk_i,
  input  logic dft_sram_clk_override_i,
  input  logic dft_ram_rei_i,
  input  logic dft_ram_wei_i,
  input  logic dft_mbist_en_i,

  input  logic [shire_esr_pkg::NumShireIdsBits-1:0] shire_id_reset_val_i,
  input  logic [shire_esr_pkg::NumShireIdsBits-1:0] shire_phy_id_i,

  input  logic [2:0] apb_sel_i,
  input  logic [shire_sbm_pkg::ShireApbAddrWidth-1:0] apb_paddr_i,
  input  logic apb_pwrite_i,
  input  logic apb_psel_i,
  input  logic apb_penable_i,
  input  logic [shire_sbm_pkg::ApbDataWidth-1:0] apb_pwdata_i,

  input  logic [shire_channel_leaves_pkg::NocIntNum-1:0] noc_err_int_srcs_i,
  input  logic [shire_esr_pkg::NumNeigh-1:0] neigh_sc_err_detected_i,
  input  logic [shire_esr_pkg::NumNeigh-1:0] neigh_sc_err_logged_i,
  input  logic [shire_esr_pkg::NumNeigh-1:0] flb_neigh_l2_req_valid_i,
  input  logic [shire_esr_pkg::NumNeigh-1:0][shire_uncached_pkg::CsrFlBarrierSize-1:0] flb_neigh_l2_req_data_i,

  input  logic neigh0_req_valid_i,
  input  logic [shirecache_pkg::Banks:0] neigh0_req_dest_i,
  input  logic [etlink_pkg::PaSize-1:0] neigh0_req_addr_i,
  input  logic [4:0] neigh0_req_opcode_i,
  input  logic [2:0] neigh0_req_size_i,
  input  logic [etlink_pkg::QwenSize-1:0] neigh0_req_qwen_i,

  input  logic [shire_esr_pkg::NumNeigh-1:0] icache_req_valid_i,
  input  logic [shire_esr_pkg::NumNeigh-1:0] icache_req_write_i,
  input  logic [shire_esr_pkg::NumNeigh-1:0][icache_geom_pkg::IcacheSramAddrWidth-1:0] icache_req_addr_i,
  input  logic [shire_esr_pkg::NumNeigh-1:0] icache_resp_ready_i,

  input  logic [shire_esr_pkg::NumNeigh-1:0] coop_slv_valid_i,
  input  logic [shire_esr_pkg::NumNeigh-1:0][neigh_pkg::ShireCoopIdSize-1:0] coop_done_id_i,
  input  logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumNeigh-2:0] coop_done_valid_i,

  output logic [shire_esr_pkg::NumNeigh-1:0] orig_rst_c_shire_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_rst_c_shire_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_rst_w_shire_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_rst_w_shire_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_rst_d_shire_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_rst_d_shire_no_o,
  output logic orig_rst_sc_no_o,
  output logic new_rst_sc_no_o,
  output logic orig_rst_rbox_no_o,
  output logic new_rst_rbox_no_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumShireIdsBits-1:0] orig_shire_id_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumShireIdsBits-1:0] new_shire_id_o,
  output logic [shire_esr_pkg::TboxPerShire-1:0] orig_tbox_en_o,
  output logic [shire_esr_pkg::TboxPerShire-1:0] new_tbox_en_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] orig_esr_thread0_enable_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] new_esr_thread0_enable_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] orig_esr_thread1_enable_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] new_esr_thread1_enable_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] orig_esr_ipi_trigger_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] new_esr_ipi_trigger_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] orig_esr_ipi_redirect_trigger_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] new_esr_ipi_redirect_trigger_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] orig_esr_mtime_local_target_o,
  output logic [shire_esr_pkg::ThreadsPerShire-1:0] new_esr_mtime_local_target_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_esr_shire_coop_mode_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_esr_shire_coop_mode_o,
  output logic orig_ioshire_log_err_int_o,
  output logic new_ioshire_log_err_int_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_uncached_pkg::MinPerNeigh-1:0] orig_uc_to_neigh_fcc_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_uncached_pkg::MinPerNeigh-1:0] new_uc_to_neigh_fcc_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_uncached_pkg::UcFccTargetSize-1:0] orig_uc_to_neigh_fcc_target_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_uncached_pkg::UcFccTargetSize-1:0] new_uc_to_neigh_fcc_target_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_flb_l2_neigh_resp_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_flb_l2_neigh_resp_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] orig_flb_l2_neigh_resp_data_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_flb_l2_neigh_resp_data_o,
  output logic orig_apb_pready_o,
  output logic new_apb_pready_o,
  output logic [shire_sbm_pkg::ApbDataWidth-1:0] orig_apb_prdata_o,
  output logic [shire_sbm_pkg::ApbDataWidth-1:0] new_apb_prdata_o,
  output logic orig_apb_pslverr_o,
  output logic new_apb_pslverr_o,
  output logic orig_sbm_enable_read_o,
  output logic new_sbm_enable_read_o,
  output logic orig_sbm_enable_write_o,
  output logic new_sbm_enable_write_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_icache_req_ready_o,
  output logic [shire_esr_pkg::NumNeigh-1:0] new_icache_resp_valid_o,
  output logic [shire_channel_leaves_pkg::NocIntNum-1:0] new_noc_all_err_int_srcs_o,
  output logic new_ioshire_noc_err_int_o,
  output logic [$bits(ram_cfg_pkg::ram_cfg_t)-1:0] new_ram_cfg_flat_o,
  output logic [$bits(shire_esr_pkg::esr_clk_gate_ctrl_t)-1:0] new_clk_gate_ctrl_flat_o,
  output logic new_debug_clk_gate_ctrl_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumNeigh-2:0] new_coop_slv_valid_o,
  output logic [shire_esr_pkg::NumNeigh-1:0][shire_esr_pkg::NumNeigh-2:0] new_coop_done_valid_o
);

  localparam int unsigned NUM_NEIGH = shire_esr_pkg::NumNeigh;
  localparam int unsigned NUM_RBOX = shirecache_pkg::RboxPerShire;
  localparam int unsigned NUM_BANKS = shirecache_pkg::Banks;
  localparam int unsigned NUM_UC = 1;
  localparam int unsigned NUM_L3_MASTER_PORTS = shirecache_pkg::L3MasterPorts;
  localparam int unsigned NUM_L3_SLAVE_PORTS = shirecache_pkg::L3SlavePorts;
  localparam int unsigned APB_SLAVES = NUM_BANKS + NUM_RBOX + 1;

  // Original CORE-ET port signals. The original ICache and cache hierarchy are
  // disabled in this container cosim (their translated blocks have standalone
  // cosims); retained ESR/reset/uncached/run-control glue remains live.
   logic                                                 clock;
   logic                                                 reset_c;
   logic                                                 reset_w;
   logic                                                 reset_d;
   logic [NUM_NEIGH-1:0]                                 reset_w_icache;
   logic                                                 noc_clock;
   logic                                                 noc_reset;
  logic [NUM_NEIGH-1:0]                                 reset_c_shire;
  logic [NUM_NEIGH-1:0]                                 reset_d_shire;
  logic [NUM_NEIGH-1:0]                                 reset_w_shire;
  logic [NUM_NEIGH-1:0]                                 reset_c_shire_scs;
  logic                                                 reset_sc;
  logic                                                 reset_rbox;
   debug_axi_port_ctl_t                                  debug_axi_port_ctl;
  debug_axi_port_t                                      shire_channel_debug_axi_port;
   esr_and_or_tree_L2_t                                  debug_and_or_tree_L2_in;
  esr_and_or_tree_L2_t                                  debug_and_or_tree_L2_out;
   bpam_run_control_t                                    bpam_run_control;
  bpam_run_control_neigh_t [NUM_NEIGH-1:0]              bpam_run_control_neigh_op;
   logic [`NUM_SHIRES_R]                                 shire_phy_id;
   logic [`NUM_SHIRE_IDS_R]                              shire_id_reset_val;
  logic [NUM_NEIGH-1:0][`NUM_SHIRE_IDS_R]               shire_id;
  logic [`TBOX_PER_SHIRE-1:0][`TBOX_PER_SHIRE_R]        tbox_id;
  logic [`TBOX_PER_SHIRE-1:0]                           tbox_en;
  logic [`MIN_PER_S-1:0]                                esr_thread0_enable;
  logic [`MIN_PER_S-1:0]                                esr_thread1_enable;
  logic [NUM_NEIGH-1:0][`ESR_MINION_FEATURE_SZ-1:0]     esr_minion_features;
  icache_prefetch_conf_t  [NUM_NEIGH-1:0]               esr_icache_prefetch_conf;
  logic [`ICACHE_PER_SHIRE-1:0]                         esr_icache_prefetch_start;
   logic [`ICACHE_PER_SHIRE-1:0]                         esr_icache_prefetch_done;
   logic [NUM_NEIGH-1:0]                                 neigh_sc_err_detected;
   logic [NUM_NEIGH-1:0]                                 neigh_sc_err_logged;
  logic                                                 ioshire_log_err_int;
   esr_and_or_tree_L0_t  [NUM_NEIGH-1:0]                 esr_and_or_tree_L0;
  logic [NUM_NEIGH-1:0]                                 esr_shire_coop_mode;
   logic [`NOC_INT_NUM+`NOC_DBG_INT_NUM-1:0]             esr_noc_interrupt_status_ip;
  esr_clk_gate_ctrl_t                                   esr_clk_gate_ctrl_op;
  logic                                                 esr_debug_clk_gate_ctrl_op;
  logic [NUM_NEIGH-1:0][(NUM_BANKS+NUM_UC)-1:0]         neigh_sc_req_ready;
   logic [NUM_NEIGH-1:0][(NUM_BANKS+NUM_UC)-1:0]         neigh_sc_req_valid;
   et_link_req_info_t  [NUM_NEIGH-1:0]                   neigh_sc_req_info;
   logic [NUM_NEIGH-1:0]                                 neigh_sc_rsp_ready;
  logic [NUM_NEIGH-1:0]                                 neigh_sc_rsp_valid;
  et_link_rsp_info_t  [NUM_NEIGH-1:0]                   neigh_sc_rsp_info;
  logic [NUM_NEIGH-1:0][`MIN_PER_N-1:0]                 uc_to_neigh_fcc;
  logic [NUM_NEIGH-1:0][`UC_FCC_TARGET_SIZE-1:0]        uc_to_neigh_fcc_target;
  logic [`THREADS_PER_S-1:0]                            esr_ipi_trigger;
  logic [`THREADS_PER_S-1:0]                            esr_ipi_redirect_trigger;
   logic [NUM_NEIGH-1:0]                                 flb_neigh_l2_req_valid;
   logic [NUM_NEIGH-1:0][`CSR_FL_BARRIER_RANGE]          flb_neigh_l2_req_data;
  logic [NUM_NEIGH-1:0]                                 flb_l2_neigh_resp_valid;
  logic [NUM_NEIGH-1:0]                                 flb_l2_neigh_resp_data;
   logic [`shire_cache_sm_gpio_width-1:0]                status_monitor_gpio;
  Neigh_ET_Link_us_monitor_t  [NUM_NEIGH+NUM_RBOX-1:0]  neigh_et_link_monitor;
   logic                                                 etlink_rbox_sm_enabled;
  rbox_dbg_us_monitor_t  [NUM_RBOX-1:0]                 rbox_dbg_sig;
   logic [`RBOX_SM_GPIO_WIDTH-1:0]                       rbox_sm_gpio;
   bpam_rc_tbox_ack_t  [NUM_NEIGH-1:0]                   bpam_rc_tbox_ack;
  sc_trace_packet_t                                     sc_trace_data;
  logic                                                 sc_trace_valid;
   APB_to_all_ESRs_t   [APB_SLAVES-1:0]                  APB_ESR_req;
  APB_from_all_ESRs_t [APB_SLAVES-1:0]                  APB_ESR_rsp;
  logic [`MIN_PER_S-1:0]                                esr_mtime_local_target_op;
  logic [NUM_NEIGH-1:0]                                 esr_pwr_ctrl_glb_nsleepin_op;
  logic [NUM_NEIGH-1:0]                                 esr_pwr_ctrl_glb_iso_op;
   logic [NUM_NEIGH-1:0]                                 esr_pwr_ctrl_glb_nsleepout_ip;
  logic [NUM_NEIGH*`MIN_PER_N-1:0]                      esr_pwr_ctrl_neigh_nsleepin_op;
  logic [NUM_NEIGH*`MIN_PER_N-1:0]                      esr_pwr_ctrl_neigh_iso_op;
   logic [NUM_NEIGH*`MIN_PER_N-1:0]                      esr_pwr_ctrl_neigh_nsleepout_ip;
  logic [`SHIRE_PLL_ESR_CTRL_BITS-1:0]                  esr_pll_ctrl_op;
  logic [`SHIRE_PLL_ESR_CONF_BITS-1:0]                  esp_pll_conf_op;
   logic                                                 esr_pll_busy_ip;
   logic [`SHIRE_PLL_DATA_BITS-1:0]                      esr_pll_rdata_ip;
   logic                                                 esr_pll_rrdy_ip;
   logic                                                 esr_pll_lock_ip;
  esr_clk_dly_ctl_t                                     esr_clk_dly_ctl_op;
  esr_dll_dly_est_ctl_t                                 esr_dll_dly_est_ctl_op;
  esr_dll_dly_est_sts_t                                  esr_dll_dly_est_sts_0_ip;
  esr_dll_dly_est_sts_t                                  esr_dll_dly_est_sts_1_ip;
  esr_dll_dly_est_sts_t                                  esr_dll_dly_est_sts_2_ip;
  esr_dll_dly_est_sts_t                                  esr_dll_dly_est_sts_3_ip;
  esr_clkmux_ctl_t                                      esr_shire_ctrl_clockmux_op;
  logic [`SHIRE_DLL_ESR_CTRL_BITS-1:0]                  esr_dll_ctrl_op;
  logic [`SHIRE_DLL_ESR_CONF_BITS-1:0]                  esp_dll_conf_op;
   logic                                                 esr_dll_busy_ip;
   logic [`SHIRE_DLL_DATA_BITS-1:0]                      esr_dll_rdata_ip;
   logic                                                 esr_dll_rrdy_ip;
   logic                                                 esr_dll_lock_ip;
  logic [`SHIRE_MINION_MEM_OVERRIDE_BITS-1:0]           esr_minion_mem_override;
  sc_master_axi_AR_channel_t [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_AR;
  logic                      [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_ARVALID;
   logic                      [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_ARREADY;
  sc_master_axi_AW_channel_t [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_AW;
  logic                      [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_AWVALID;
   logic                      [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_AWREADY;
  sc_master_axi_W_channel_t  [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_W;
  logic                      [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_WVALID;
   logic                      [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_WREADY;
   sc_master_axi_B_channel_t  [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_B;
   logic                      [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_BVALID;
  logic                      [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_BREADY;
   sc_master_axi_R_channel_t  [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_R;
   logic                      [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_RVALID;
  logic                      [NUM_L3_MASTER_PORTS-1:0]  to_l3_mesh_master_axi_RREADY;
  sc_master_axi_AR_channel_t                            to_sys_mesh_master_axi_AR;
  logic                                                 to_sys_mesh_master_axi_ARVALID;
   logic                                                 to_sys_mesh_master_axi_ARREADY;
  sc_master_axi_AW_channel_t                            to_sys_mesh_master_axi_AW;
  logic                                                 to_sys_mesh_master_axi_AWVALID;
   logic                                                 to_sys_mesh_master_axi_AWREADY;
  sc_master_axi_W_channel_t                             to_sys_mesh_master_axi_W;
  logic                                                 to_sys_mesh_master_axi_WVALID;
   logic                                                 to_sys_mesh_master_axi_WREADY;
   sc_master_axi_B_channel_t                             to_sys_mesh_master_axi_B;
   logic                                                 to_sys_mesh_master_axi_BVALID;
  logic                                                 to_sys_mesh_master_axi_BREADY;
   sc_master_axi_R_channel_t                             to_sys_mesh_master_axi_R;
   logic                                                 to_sys_mesh_master_axi_RVALID;
  logic                                                 to_sys_mesh_master_axi_RREADY;
   sc_slave_axi_AR_channel_t [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_AR;
   logic                     [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_ARVALID;
  logic                     [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_ARREADY;
   sc_slave_axi_AW_channel_t [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_AW;
   logic                     [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_AWVALID;
  logic                     [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_AWREADY;
   sc_slave_axi_W_channel_t  [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_W;
   logic                     [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_WVALID;
  logic                     [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_WREADY;
  sc_slave_axi_B_channel_t  [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_B;
  logic                     [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_BVALID;
   logic                     [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_BREADY;
  sc_slave_axi_R_channel_t  [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_R;
  logic                     [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_RVALID;
   logic                     [NUM_L3_SLAVE_PORTS-1:0]    l3_mesh_slave_axi_RREADY;
  sc_master_axi_AR_channel_t                            uc_to_l3_mesh_master_axi_AR;
  logic                                                 uc_to_l3_mesh_master_axi_ARVALID;
   logic                                                 uc_to_l3_mesh_master_axi_ARREADY;
  sc_master_axi_AW_channel_t                            uc_to_l3_mesh_master_axi_AW;
  logic                                                 uc_to_l3_mesh_master_axi_AWVALID;
   logic                                                 uc_to_l3_mesh_master_axi_AWREADY;
  sc_master_axi_W_channel_t                             uc_to_l3_mesh_master_axi_W;
  logic                                                 uc_to_l3_mesh_master_axi_WVALID;
   logic                                                 uc_to_l3_mesh_master_axi_WREADY;
   sc_master_axi_B_channel_t                             uc_to_l3_mesh_master_axi_B;
   logic                                                 uc_to_l3_mesh_master_axi_BVALID;
  logic                                                 uc_to_l3_mesh_master_axi_BREADY;
   sc_master_axi_R_channel_t                             uc_to_l3_mesh_master_axi_R;
   logic                                                 uc_to_l3_mesh_master_axi_RVALID;
  logic                                                 uc_to_l3_mesh_master_axi_RREADY;
  sc_master_axi_AR_channel_t                            uc_to_sys_mesh_master_axi_AR;
  logic                                                 uc_to_sys_mesh_master_axi_ARVALID;
   logic                                                 uc_to_sys_mesh_master_axi_ARREADY;
  sc_master_axi_AW_channel_t                            uc_to_sys_mesh_master_axi_AW;
  logic                                                 uc_to_sys_mesh_master_axi_AWVALID;
   logic                                                 uc_to_sys_mesh_master_axi_AWREADY;
  sc_master_axi_W_channel_t                             uc_to_sys_mesh_master_axi_W;
  logic                                                 uc_to_sys_mesh_master_axi_WVALID;
   logic                                                 uc_to_sys_mesh_master_axi_WREADY;
   sc_master_axi_B_channel_t                             uc_to_sys_mesh_master_axi_B;
   logic                                                 uc_to_sys_mesh_master_axi_BVALID;
  logic                                                 uc_to_sys_mesh_master_axi_BREADY;
   sc_master_axi_R_channel_t                             uc_to_sys_mesh_master_axi_R;
   logic                                                 uc_to_sys_mesh_master_axi_RVALID;
  logic                                                 uc_to_sys_mesh_master_axi_RREADY;
   sys_slave_axi_AR_channel_t                            sys_mesh_slave_axi_AR;
   logic                                                 sys_mesh_slave_axi_ARVALID;
  logic                                                 sys_mesh_slave_axi_ARREADY;
   sys_slave_axi_AW_channel_t                            sys_mesh_slave_axi_AW;
   logic                                                 sys_mesh_slave_axi_AWVALID;
  logic                                                 sys_mesh_slave_axi_AWREADY;
   sys_slave_axi_W_channel_t                             sys_mesh_slave_axi_W;
   logic                                                 sys_mesh_slave_axi_WVALID;
  logic                                                 sys_mesh_slave_axi_WREADY;
  sys_slave_axi_B_channel_t                             sys_mesh_slave_axi_B;
  logic                                                 sys_mesh_slave_axi_BVALID;
   logic                                                 sys_mesh_slave_axi_BREADY;
  sys_slave_axi_R_channel_t                             sys_mesh_slave_axi_R;
  logic                                                 sys_mesh_slave_axi_RVALID;
   logic                                                 sys_mesh_slave_axi_RREADY;
   logic [1:0]                                           sys_mesh_slave_axi_AWvcvalid;
  logic [1:0]                                           sys_mesh_slave_axi_AWcredit;
   logic [1:0]                                           sys_mesh_slave_axi_Wvcvalid;
  logic                                                 sbm_enable_read;
  logic                                                 sbm_enable_write;
   logic                                                 sbm_write_credit_return;
  sys_slave_axi_AR_channel_t                            sbm_sys_mesh_slave_axi_AR;
   logic                                                 sbm_sys_mesh_slave_axi_ARREADY;
  sys_slave_axi_AW_channel_t                            sbm_sys_mesh_slave_axi_AW;
   logic                                                 sbm_sys_mesh_slave_axi_AWREADY;
  sys_slave_axi_W_channel_t                             sbm_sys_mesh_slave_axi_W;
   logic                                                 sbm_sys_mesh_slave_axi_WREADY;
   sys_slave_axi_B_channel_t                             sbm_sys_mesh_slave_axi_B;
   logic                                                 sbm_sys_mesh_slave_axi_BVALID;
  logic                                                 sbm_sys_mesh_slave_axi_BREADY;
   sys_slave_axi_R_channel_t                             sbm_sys_mesh_slave_axi_R;
   logic                                                 sbm_sys_mesh_slave_axi_RVALID;
  logic                                                 sbm_sys_mesh_slave_axi_RREADY;
   logic [NUM_NEIGH-1:0]                                 icache_f2_sram_req_write;
   logic [NUM_NEIGH-1:0][`ICACHE_SRAM_ADD_WIDTH-1:0]     icache_f2_sram_req_addr;
   logic [NUM_NEIGH-1:0]                                 icache_f2_sram_req_valid;
  logic [NUM_NEIGH-1:0]                                 icache_f2_sram_req_ready;
  logic [NUM_NEIGH-1:0][`ICACHE_SRAM_DATA_WIDTH-1:0]    icache_f0_sram_resp_dout;
  logic [NUM_NEIGH-1:0]                                 icache_f0_sram_resp_valid;
   logic [NUM_NEIGH-1:0]                                 icache_f0_sram_resp_ready;
   logic                                                 dft__reset_byp_hv;
   logic                                                 dft__reset_hv;
   logic                                                 dft__reset_byp_lv;
   logic                                                 dft__reset_lv;
   logic                                                 dft__sram_clock;
   logic                                                 dft__clk_override;
   logic                                                 dft__mbist_en;
   logic                                                 dft__bist_insertion;
   et_tdr_cntl_t                                         dft__tdr_cntl;
   logic                                                 dft__tdr_tck;
   logic                                                 dft__tdr_tdi_sc;
  logic                                                 dft__tdr_tdo_sc;
  logic                                                 use_shire_tdr_clk_cntl;
  logic                                                 use_shire_tdr_reset_cntl;
  shire_tdr_clk_cntl_t                                  shire_tdr_clk_cntl;
  shire_tdr_reset_cntl_t                                shire_tdr_reset_cntl;
  logic                [NUM_BANKS-1:0]                  sc_neigh_l2hpf_req_valid;
  sc_neigh_l2hpf_req_t [NUM_BANKS-1:0]                  sc_neigh_l2hpf_req_info;
   logic                                                 vl_sms_shire_channel_wrck;
   logic                                                 vl_sms_shire_channel_wrstn;
   logic                                                 vl_sms_shire_channel_rst;
   logic                                                 vl_sms_shire_channel_wsi;
   logic                                                 vl_sms_shire_channel_selectwir;
   logic                                                 vl_sms_shire_channel_capturewr;
   logic                                                 vl_sms_shire_channel_shiftwr;
   logic                                                 vl_sms_shire_channel_updatewr;
  logic                                                 vl_sms_shire_channel_wso;
   logic                                                 dm0_shire_channel;
   logic                                                 dm1_shire_channel;
   logic                                                 dm2_shire_channel;

  logic [2:0] apb_sel_q;
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) apb_sel_q <= '0;
    else if (apb_psel_i && !apb_penable_i) apb_sel_q <= apb_sel_i;
  end

  always_comb begin
    clock = clk_i;
    reset_c = !rst_ni | !rst_c_ext_ni;
    reset_w = !rst_ni | !rst_w_ext_ni;
    reset_d = !rst_ni | !rst_d_ext_ni;
    reset_w_icache = ~rst_w_icache_ext_ni;
    noc_clock = clk_i;
    noc_reset = !rst_ni | !rst_noc_ext_ni;

    debug_axi_port_ctl = '0;
    debug_and_or_tree_L2_in = '0;
    bpam_run_control = '0;
    bpam_run_control.gpio.ndmreset = dft_scanmode_i;
    bpam_run_control.gpio.halt_req = dft_ram_rei_i;
    bpam_run_control.gpio.resume_req = dft_ram_wei_i;
    bpam_run_control.events.halt_req = dft_sram_clk_override_i;
    bpam_run_control.events.resume_req = dft_mbist_en_i;

    shire_phy_id = shire_phy_id_i;
    shire_id_reset_val = shire_id_reset_val_i;
    esr_icache_prefetch_done = '0;
    neigh_sc_err_detected = neigh_sc_err_detected_i;
    neigh_sc_err_logged = neigh_sc_err_logged_i;
    esr_and_or_tree_L0 = '{default: '0};
    esr_noc_interrupt_status_ip = {{`NOC_DBG_INT_NUM{1'b0}}, noc_err_int_srcs_i};

    neigh_sc_req_valid = '0;
    neigh_sc_req_valid[0] = neigh0_req_valid_i ? neigh0_req_dest_i : '0;
    neigh_sc_req_info = '{default: '0};
    neigh_sc_req_info[0].opcode = et_link_req_opcode_t'(neigh0_req_opcode_i);
    neigh_sc_req_info[0].size = et_link_size_t'(neigh0_req_size_i);
    neigh_sc_req_info[0].address = neigh0_req_addr_i;
    neigh_sc_req_info[0].qwen = neigh0_req_qwen_i;
    neigh_sc_rsp_ready = '1;

    flb_neigh_l2_req_valid = flb_neigh_l2_req_valid_i;
    flb_neigh_l2_req_data = flb_neigh_l2_req_data_i;
    status_monitor_gpio = '0;
    etlink_rbox_sm_enabled = 1'b0;
    rbox_sm_gpio = '0;
    bpam_rc_tbox_ack = '{default: '0};

    APB_ESR_req = '{default: '0};
    if (apb_sel_i < APB_SLAVES[2:0]) begin
      APB_ESR_req[apb_sel_i].apb_paddr = apb_paddr_i[`APB_AD_SHIRE_TOP_WIDTH-1:0];
      APB_ESR_req[apb_sel_i].apb_pwrite = apb_pwrite_i;
      APB_ESR_req[apb_sel_i].apb_psel = apb_psel_i;
      APB_ESR_req[apb_sel_i].apb_penable = apb_penable_i;
      APB_ESR_req[apb_sel_i].apb_pwdata = apb_pwdata_i;
    end

    esr_pwr_ctrl_glb_nsleepout_ip = '0;
    esr_pwr_ctrl_neigh_nsleepout_ip = '0;
    esr_pll_busy_ip = 1'b0;
    esr_pll_rdata_ip = '0;
    esr_pll_rrdy_ip = 1'b0;
    esr_pll_lock_ip = 1'b0;
    esr_dll_dly_est_sts_0_ip = '0;
    esr_dll_dly_est_sts_1_ip = '0;
    esr_dll_dly_est_sts_2_ip = '0;
    esr_dll_dly_est_sts_3_ip = '0;
    esr_dll_busy_ip = 1'b0;
    esr_dll_rdata_ip = '0;
    esr_dll_rrdy_ip = 1'b0;
    esr_dll_lock_ip = 1'b0;

    to_l3_mesh_master_axi_ARREADY = '1;
    to_l3_mesh_master_axi_AWREADY = '1;
    to_l3_mesh_master_axi_WREADY = '1;
    to_l3_mesh_master_axi_B = '{default: '0};
    to_l3_mesh_master_axi_BVALID = '0;
    to_l3_mesh_master_axi_R = '{default: '0};
    to_l3_mesh_master_axi_RVALID = '0;
    to_sys_mesh_master_axi_ARREADY = 1'b1;
    to_sys_mesh_master_axi_AWREADY = 1'b1;
    to_sys_mesh_master_axi_WREADY = 1'b1;
    to_sys_mesh_master_axi_B = '0;
    to_sys_mesh_master_axi_BVALID = 1'b0;
    to_sys_mesh_master_axi_R = '0;
    to_sys_mesh_master_axi_RVALID = 1'b0;
    l3_mesh_slave_axi_AR = '{default: '0};
    l3_mesh_slave_axi_ARVALID = '0;
    l3_mesh_slave_axi_AW = '{default: '0};
    l3_mesh_slave_axi_AWVALID = '0;
    l3_mesh_slave_axi_W = '{default: '0};
    l3_mesh_slave_axi_WVALID = '0;
    l3_mesh_slave_axi_BREADY = '1;
    l3_mesh_slave_axi_RREADY = '1;
    uc_to_l3_mesh_master_axi_ARREADY = 1'b1;
    uc_to_l3_mesh_master_axi_AWREADY = 1'b1;
    uc_to_l3_mesh_master_axi_WREADY = 1'b1;
    uc_to_l3_mesh_master_axi_B = '0;
    uc_to_l3_mesh_master_axi_BVALID = 1'b0;
    uc_to_l3_mesh_master_axi_R = '0;
    uc_to_l3_mesh_master_axi_RVALID = 1'b0;
    uc_to_sys_mesh_master_axi_ARREADY = 1'b1;
    uc_to_sys_mesh_master_axi_AWREADY = 1'b1;
    uc_to_sys_mesh_master_axi_WREADY = 1'b1;
    uc_to_sys_mesh_master_axi_B = '0;
    uc_to_sys_mesh_master_axi_BVALID = 1'b0;
    uc_to_sys_mesh_master_axi_R = '0;
    uc_to_sys_mesh_master_axi_RVALID = 1'b0;
    sys_mesh_slave_axi_AR = '0;
    sys_mesh_slave_axi_ARVALID = 1'b0;
    sys_mesh_slave_axi_AW = '0;
    sys_mesh_slave_axi_AWVALID = 1'b0;
    sys_mesh_slave_axi_W = '0;
    sys_mesh_slave_axi_WVALID = 1'b0;
    sys_mesh_slave_axi_BREADY = 1'b1;
    sys_mesh_slave_axi_RREADY = 1'b1;
    sys_mesh_slave_axi_AWvcvalid = '0;
    sys_mesh_slave_axi_Wvcvalid = '0;
    sbm_write_credit_return = 1'b0;
    sbm_sys_mesh_slave_axi_ARREADY = 1'b1;
    sbm_sys_mesh_slave_axi_AWREADY = 1'b1;
    sbm_sys_mesh_slave_axi_WREADY = 1'b1;
    sbm_sys_mesh_slave_axi_B = '0;
    sbm_sys_mesh_slave_axi_BVALID = 1'b0;
    sbm_sys_mesh_slave_axi_R = '0;
    sbm_sys_mesh_slave_axi_RVALID = 1'b0;

    icache_f2_sram_req_write = icache_req_write_i;
    icache_f2_sram_req_addr = icache_req_addr_i;
    icache_f2_sram_req_valid = icache_req_valid_i;
    icache_f0_sram_resp_ready = icache_resp_ready_i;
    dft__reset_byp_hv = dft_scanmode_i;
    dft__reset_hv = !dft_scan_reset_ni;
    dft__reset_byp_lv = dft_scanmode_i;
    dft__reset_lv = !dft_scan_reset_ni;
    dft__sram_clock = dft_sram_clk_i;
    dft__clk_override = dft_sram_clk_override_i;
    dft__mbist_en = dft_mbist_en_i;
    dft__bist_insertion = 1'b0;
    dft__tdr_cntl = '0;
    dft__tdr_tck = clk_i;
    dft__tdr_tdi_sc = 1'b0;
    vl_sms_shire_channel_wrck = 1'b0;
    vl_sms_shire_channel_wrstn = 1'b1;
    vl_sms_shire_channel_rst = 1'b0;
    vl_sms_shire_channel_wsi = 1'b0;
    vl_sms_shire_channel_selectwir = 1'b0;
    vl_sms_shire_channel_capturewr = 1'b0;
    vl_sms_shire_channel_shiftwr = 1'b0;
    vl_sms_shire_channel_updatewr = 1'b0;
    dm0_shire_channel = 1'b0;
    dm1_shire_channel = 1'b0;
    dm2_shire_channel = 1'b0;
  end

  shire_channel_orig #(
    .DV_SC_STUB              (1'b1),
    .NUM_RBOX                (NUM_RBOX),
    .NUM_BANKS               (NUM_BANKS),
    .NUM_L3_MASTER_PORTS     (NUM_L3_MASTER_PORTS),
    .NUM_L3_SLAVE_PORTS      (NUM_L3_SLAVE_PORTS),
    .ICACHE_MEMS_IMPLEMENTED (0),
    .L2HPF_IMPLEMENTED       (0),
    .APB_SLAVES              (APB_SLAVES)
  ) u_orig (.*);

  shire_channel_tb u_new (
    .clk_i,
    .rst_ni,
    .rst_c_ext_ni,
    .rst_w_ext_ni,
    .rst_d_ext_ni,
    .rst_noc_ext_ni,
    .rst_w_icache_ext_ni,
    .dft_scanmode_i,
    .dft_scan_reset_ni,
    .dft_sram_clk_i,
    .dft_sram_clk_override_i,
    .dft_ram_rei_i,
    .dft_ram_wei_i,
    .dft_mbist_en_i,
    .shire_id_reset_val_i,
    .shire_phy_id_i,
    .apb_sel_i,
    .apb_paddr_i,
    .apb_pwrite_i,
    .apb_psel_i,
    .apb_penable_i,
    .apb_pwdata_i,
    .apb_pready_o(new_apb_pready_o),
    .apb_prdata_o(new_apb_prdata_o),
    .apb_pslverr_o(new_apb_pslverr_o),
    .noc_err_int_srcs_i,
    .neigh_sc_err_detected_i,
    .neigh_sc_err_logged_i,
    .flb_neigh_l2_req_valid_i,
    .flb_neigh_l2_req_data_i,
    .neigh0_req_valid_i,
    .neigh0_req_dest_i,
    .neigh0_req_addr_i,
    .neigh0_req_opcode_i,
    .neigh0_req_size_i,
    .neigh0_req_qwen_i,
    .icache_req_valid_i,
    .icache_req_write_i,
    .icache_req_addr_i,
    .icache_resp_ready_i,
    .coop_slv_valid_i,
    .coop_done_id_i,
    .coop_done_valid_i,
    .rst_c_shire_no_o(new_rst_c_shire_no_o),
    .rst_w_shire_no_o(new_rst_w_shire_no_o),
    .rst_d_shire_no_o(new_rst_d_shire_no_o),
    .rst_sc_no_o(new_rst_sc_no_o),
    .rst_rbox_no_o(new_rst_rbox_no_o),
    .shire_id_o(new_shire_id_o),
    .tbox_en_o(new_tbox_en_o),
    .esr_thread0_enable_o(new_esr_thread0_enable_o),
    .esr_thread1_enable_o(new_esr_thread1_enable_o),
    .esr_ipi_trigger_o(new_esr_ipi_trigger_o),
    .esr_ipi_redirect_trigger_o(new_esr_ipi_redirect_trigger_o),
    .esr_mtime_local_target_o(new_esr_mtime_local_target_o),
    .esr_shire_coop_mode_o(new_esr_shire_coop_mode_o),
    .ioshire_log_err_int_o(new_ioshire_log_err_int_o),
    .ioshire_noc_err_int_o(new_ioshire_noc_err_int_o),
    .noc_all_err_int_srcs_o(new_noc_all_err_int_srcs_o),
    .uc_to_neigh_fcc_o(new_uc_to_neigh_fcc_o),
    .uc_to_neigh_fcc_target_o(new_uc_to_neigh_fcc_target_o),
    .flb_l2_neigh_resp_valid_o(new_flb_l2_neigh_resp_valid_o),
    .flb_l2_neigh_resp_data_o(new_flb_l2_neigh_resp_data_o),
    .neigh_sc_req_ready_o(),
    .neigh_sc_rsp_valid_o(),
    .icache_req_ready_o(new_icache_req_ready_o),
    .icache_resp_valid_o(new_icache_resp_valid_o),
    .ram_cfg_flat_o(new_ram_cfg_flat_o),
    .clk_gate_ctrl_flat_o(new_clk_gate_ctrl_flat_o),
    .debug_clk_gate_ctrl_o(new_debug_clk_gate_ctrl_o),
    .coop_slv_valid_o(new_coop_slv_valid_o),
    .coop_done_valid_o(new_coop_done_valid_o),
    .sbm_enable_read_o(new_sbm_enable_read_o),
    .sbm_enable_write_o(new_sbm_enable_write_o)
  );

  assign orig_rst_c_shire_no_o = ~reset_c_shire;
  assign orig_rst_w_shire_no_o = ~reset_w_shire;
  assign orig_rst_d_shire_no_o = ~reset_d_shire;
  assign orig_rst_sc_no_o = ~reset_sc;
  assign orig_rst_rbox_no_o = ~reset_rbox;
  assign orig_shire_id_o = shire_id;
  assign orig_tbox_en_o = tbox_en;
  assign orig_esr_thread0_enable_o = esr_thread0_enable;
  assign orig_esr_thread1_enable_o = esr_thread1_enable;
  assign orig_esr_ipi_trigger_o = esr_ipi_trigger;
  assign orig_esr_ipi_redirect_trigger_o = esr_ipi_redirect_trigger;
  assign orig_esr_mtime_local_target_o = esr_mtime_local_target_op;
  assign orig_esr_shire_coop_mode_o = esr_shire_coop_mode;
  assign orig_ioshire_log_err_int_o = ioshire_log_err_int;
  assign orig_uc_to_neigh_fcc_o = uc_to_neigh_fcc;
  assign orig_uc_to_neigh_fcc_target_o = uc_to_neigh_fcc_target;
  assign orig_flb_l2_neigh_resp_valid_o = flb_l2_neigh_resp_valid;
  assign orig_flb_l2_neigh_resp_data_o = flb_l2_neigh_resp_data;
  assign orig_apb_pready_o = APB_ESR_rsp[apb_sel_q].apb_pready;
  assign orig_apb_prdata_o = APB_ESR_rsp[apb_sel_q].apb_prdata;
  assign orig_apb_pslverr_o = APB_ESR_rsp[apb_sel_q].apb_pslverr;
  assign orig_sbm_enable_read_o = sbm_enable_read;
  assign orig_sbm_enable_write_o = sbm_enable_write;

endmodule
/* verilator lint_on PINCONNECTEMPTY */
/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on UNUSEDSIGNAL */
