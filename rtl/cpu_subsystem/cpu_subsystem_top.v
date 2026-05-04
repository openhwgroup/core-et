// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cpu_subsystem_top (
  // System signals
  input logic                             clock,
  input logic                             clock_ext,
  input logic                             reset_cold,
  input logic                             reset_warm,

  // Power control
  input  logic                            cpu_pd_req,
  output logic                            cpu_pd_ack,
  input  logic [`MIN_PER_N-1:0]           minion_pd_req,
  output logic [`MIN_PER_N-1:0]           minion_pd_ack,

  // Interrupt requests
  input logic [`PLIC_NEXTINT_SOURCES-1:0] plic_irq,

  // AXI MASTER INTERFACE
  // Read address channel
  output logic [`CPU_AXI_ID_WIDTH-1:0]    axi_ARID,
  output logic [`CPU_AXI_ADDR_WIDTH-1:0]  axi_ARADDR,
  output logic [`CPU_AXI_LEN_WIDTH-1:0]   axi_ARLEN,
  output logic [`CPU_AXI_SIZE_WIDTH-1:0]  axi_ARSIZE,
  output logic [`CPU_AXI_BURST_WIDTH-1:0] axi_ARBURST,
  output logic                            axi_ARLOCK,
  output logic [`CPU_AXI_CACHE_WIDTH-1:0] axi_ARCACHE,
  output logic [`CPU_AXI_PROT_WIDTH-1:0]  axi_ARPROT,
  output logic                            axi_ARVALID,
  input  logic                            axi_ARREADY,
  // Read data channel
  input  logic [`CPU_AXI_ID_WIDTH-1:0]    axi_RID,
  input  logic [`CPU_AXI_DATA_WIDTH-1:0]  axi_RDATA,
  input  logic [`CPU_AXI_RESP_WIDTH-1:0]  axi_RRESP,
  input  logic                            axi_RLAST,
  input  logic                            axi_RVALID,
  output logic                            axi_RREADY,
  // Write address channel
  output logic [`CPU_AXI_ID_WIDTH-1:0]    axi_AWID,
  output logic [`CPU_AXI_ADDR_WIDTH-1:0]  axi_AWADDR,
  output logic [`CPU_AXI_LEN_WIDTH-1:0]   axi_AWLEN,
  output logic [`CPU_AXI_SIZE_WIDTH-1:0]  axi_AWSIZE,
  output logic [`CPU_AXI_BURST_WIDTH-1:0] axi_AWBURST,
  output logic                            axi_AWLOCK,
  output logic [`CPU_AXI_CACHE_WIDTH-1:0] axi_AWCACHE,
  output logic [`CPU_AXI_PROT_WIDTH-1:0]  axi_AWPROT,
  output logic                            axi_AWVALID,
  input  logic                            axi_AWREADY,
  // Write data channel
  output logic [`CPU_AXI_DATA_WIDTH-1:0]  axi_WDATA,
  output logic [`CPU_AXI_STRB_WIDTH-1:0]  axi_WSTRB,
  output logic                            axi_WLAST,
  output logic                            axi_WVALID,
  input  logic                            axi_WREADY,
  // Write response channel
  input  logic [`CPU_AXI_ID_WIDTH-1:0]    axi_BID,
  input  logic [`CPU_AXI_RESP_WIDTH-1:0]  axi_BRESP,
  input  logic                            axi_BVALID,
  output logic                            axi_BREADY,
  // APB SLAVE INTERFACE
  input logic                             apb_psel,
  input logic                             apb_penable,
  input logic                             apb_pwrite,
  input logic [`CPU_APB_ADDR_WIDTH-1:0]   apb_paddr,
  input logic [`CPU_APB_DATA_WIDTH-1:0]   apb_pwdata,
  output logic [`CPU_APB_DATA_WIDTH-1:0]  apb_prdata,
  output logic                            apb_pready,
  output logic                            apb_pslverr
);

// System signals
logic reset_w;
logic reset_c;
logic reset_d;
logic reset_w_icache;
logic reset_w_icache_d1;
// Neighborhood - Icache L1 SRAM interface
logic icache_f2_sram_req_write;
logic [`ICACHE_SRAM_ADD_WIDTH-1:0] icache_f2_sram_req_addr;
logic icache_f2_sram_req_valid;
logic icache_f2_sram_req_ready;
logic [`ICACHE_SRAM_DATA_WIDTH-1:0] icache_f0_sram_resp_dout;
logic icache_f0_sram_resp_valid;
logic icache_f0_sram_resp_ready;
// MRAM - Neighborhood response
et_link_rsp_info_t neigh_sc_rsp_info;
logic neigh_sc_rsp_valid;
logic neigh_sc_rsp_ready;
// ESRs
minion_chicken_bits_vc_t neigh_clk_gate_ctrl;
esr_clk_gate_ctrl_t esr_clk_gate_ctrl;
logic [`MIN_PER_N-1:0] esr_thread0_enable;
logic [`MIN_PER_N-1:0] esr_thread1_enable;
logic [`ESR_MINION_FEATURE_SZ-1:0] esr_minion_features;
icache_prefetch_conf_t esr_icache_prefetch_conf;
logic esr_icache_prefetch_start;
logic esr_icache_prefetch_done;
logic esr_shire_coop_mode;
esr_ms_dmctrl_t dmctrl;
esr_sm_config_t esr_sm_config;
logic [0:0]  esr_sm_trigger;
logic [63:0] esr_sm_match;
logic [63:0] esr_sm_filter0;
logic [63:0] esr_sm_filter1;
logic [63:0] esr_sm_filter2;
logic [63:0] esr_sm_data0;
logic [63:0] esr_sm_data1;
neigh_sm_dbg_monitor_t neigh_sm_signals;

logic hw_dbg_sm_monitor_enabled;
logic [`neigh_sm_gpio_width - 1:0] neigh_sm_gpio;

// ET-LINK INTERFACE
// ET-Link Request bus
et_link_req_info_t etl_req_info;
logic etl_req_valid;
logic etl_req_ready;
logic [(`SC_BANKS+1)-1:0] neigh_sc_req_ready;
logic [(`SC_BANKS+1)-1:0] neigh_sc_req_valid;
// ET-Link Response bus
et_link_rsp_info_t etl_rsp_info;
logic etl_rsp_valid;
logic etl_rsp_ready;
// APB
APB_to_cpu_t cpu_apb_req;
APB_from_cpu_t cpu_apb_rsp;
APB_to_cpu_icache_t l1_icache_apb_req;
APB_from_cpu_icache_t l1_icache_apb_rsp;
APB_to_neigh_t neigh_apb_req;
APB_from_neigh_t neigh_apb_rsp;
APB_to_cpu_reg_t cpu_reg_apb_req;
APB_from_cpu_reg_t cpu_reg_apb_rsp;
APB_to_plic_t plic_apb_req;
APB_from_plic_t plic_apb_rsp;
// Interrupts
logic [`THREADS_PER_N-1:0] int_mtip;
logic [`THREADS_PER_N-1:0] plic_meip;
logic [`THREADS_PER_N-1:0] plic_seip;
// IPI
logic [`THREADS_PER_N-1:0]      ipi_msip;
logic [`THREADS_PER_N-1:0]      esr_ipi_trigger;
logic [`THREADS_PER_N-1:0]      esr_ipi_redirect_trigger;
logic [`MIN_PER_N-1:0]          fcc;
logic [`UC_FCC_TARGET_SIZE-1:0] fcc_target;
// clint timer
cpu_clint_timer_regs_we_t esr_timer_regs_we;
cpu_clint_timer_regs_t esr_timer_regs_write;
cpu_clint_timer_regs_t esr_timer_regs_read;
// Fast Local Barrier and FCC
uc_esr_values_t esr_values;
uc_esr_enable_sigs esr_enables;
logic flb_neigh_l2_req_valid;
logic [`CSR_FL_BARRIER_RANGE] flb_neigh_l2_req_data;
logic flb_l2_neigh_resp_valid;
logic flb_l2_neigh_resp_data;
// Power down control
logic cpu_pd_ack_ff;
logic minion_pd_ack_ff;
logic apb_is_idle;

sys_gasket_reset i_sys_gasket_reset(
  .clock           (clock),
  .reset_warm_in   (reset_warm),
  .gpio_ndmreset_in('0), // Not used
  .ndmreset_in     (dmctrl.ndmreset),
  .dmactive_in     (dmctrl.dmactive),
  .reset_cold_in   (reset_cold),
  .dft__reset_byp  ('0),
  .dft__reset      ('0),
  .reset_w         (reset_w),
  .reset_c         (reset_c),
  .reset_d         (reset_d)
);

assign neigh_sc_req_ready = {(`SC_BANKS+1){etl_req_ready}};
assign etl_req_valid = |neigh_sc_req_valid;

cpu_etl2axi_top i_etl2axi(
  // System signals
  .clock        (clock),
  .reset        (reset_w),
  // ET-LINK INTERFACE
  // ET-Link Request
  .etl_req_info (etl_req_info),
  .etl_req_valid(etl_req_valid),
  .etl_req_ready(etl_req_ready),
  // ET-Link Response
  .etl_rsp_info (etl_rsp_info),
  .etl_rsp_valid(etl_rsp_valid),
  .etl_rsp_ready(etl_rsp_ready),
  // AXI MASTER INTERFACE
  // Read address channel
  .axi_ARID     (axi_ARID),
  .axi_ARADDR   (axi_ARADDR),
  .axi_ARLEN    (axi_ARLEN),
  .axi_ARSIZE   (axi_ARSIZE),
  .axi_ARBURST  (axi_ARBURST),
  .axi_ARLOCK   (axi_ARLOCK),
  .axi_ARCACHE  (axi_ARCACHE),
  .axi_ARPROT   (axi_ARPROT),
  .axi_ARVALID  (axi_ARVALID),
  .axi_ARREADY  (axi_ARREADY),
  // Read data channel
  .axi_RID      (axi_RID),
  .axi_RDATA    (axi_RDATA),
  .axi_RRESP    (axi_RRESP),
  .axi_RLAST    (axi_RLAST),
  .axi_RVALID   (axi_RVALID),
  .axi_RREADY   (axi_RREADY),
  // Write address channel
  .axi_AWID     (axi_AWID),
  .axi_AWADDR   (axi_AWADDR),
  .axi_AWLEN    (axi_AWLEN),
  .axi_AWSIZE   (axi_AWSIZE),
  .axi_AWBURST  (axi_AWBURST),
  .axi_AWLOCK   (axi_AWLOCK),
  .axi_AWCACHE  (axi_AWCACHE),
  .axi_AWPROT   (axi_AWPROT),
  .axi_AWVALID  (axi_AWVALID),
  .axi_AWREADY  (axi_AWREADY),
  // Write data channel
  .axi_WDATA    (axi_WDATA),
  .axi_WSTRB    (axi_WSTRB),
  .axi_WLAST    (axi_WLAST),
  .axi_WVALID   (axi_WVALID),
  .axi_WREADY   (axi_WREADY),
  // Write response channel
  .axi_BID      (axi_BID),
  .axi_BRESP    (axi_BRESP),
  .axi_BVALID   (axi_BVALID),
  .axi_BREADY   (axi_BREADY)
);

assign cpu_apb_req = '{apb_psel    : apb_psel,
                       apb_penable : apb_penable,
                       apb_paddr   : apb_paddr,
                       apb_pwrite  : apb_pwrite,
                       apb_pwdata  : apb_pwdata};

assign apb_pready  = cpu_apb_rsp.apb_pready;
assign apb_prdata  = cpu_apb_rsp.apb_prdata;
assign apb_pslverr = cpu_apb_rsp.apb_pslverr;

cpu_apb_mux i_apb_mux (
  // System signals
  .clock            (clock),
  .reset            (reset_c),
  // APB connection with the CPU subsystem
  .cpu_apb_req      (cpu_apb_req),
  .cpu_apb_rsp      (cpu_apb_rsp),
  // APB connection with L1 ICache Data RAM
  .l1_icache_apb_req(l1_icache_apb_req),
  .l1_icache_apb_rsp(l1_icache_apb_rsp),
  // APB connection with the Neighborhood
  .neigh_apb_req    (neigh_apb_req),
  .neigh_apb_rsp    (neigh_apb_rsp),
  // APB connection with ESR
  .cpu_reg_apb_req  (cpu_reg_apb_req),
  .cpu_reg_apb_rsp  (cpu_reg_apb_rsp),
  // APB connection with PLIC
  .plic_apb_req     (plic_apb_req),
  .plic_apb_rsp     (plic_apb_rsp)
);

cpu_plic cpu_plic (
  // System signals
  .clock                 (clock),
  .reset                 (reset_w),
  // APB connection with the CPU subsystem
  .plic_apb_req          (plic_apb_req),
  .plic_apb_rsp          (plic_apb_rsp),
  // Interrupts input
  .interrupt_req_from_src(plic_irq),
  .plic_meip             (plic_meip),
  .plic_seip             (plic_seip)
);

rst_repeat rst_repeat_reset_w_icache_d1 (
   .reset_out     (reset_w_icache_d1),
   .reset_in      (reset_w_icache),
   .clock         (clock),
   .dft__reset_byp('0),
   .dft__reset    ('0)
);

sc_icache_bist_rsp_t i_icache_data_ram_bist_rsp_info_unused_ok;
icache_data_ram_wrap i_icache_data_ram (
  // System signals
  .clock                  (clock),
  .reset                  (reset_w_icache_d1),
  .dft__sram_clock        ('0),
  .dft__clk_override      ('0),
  // Neighborhood - Icache L1 SRAM interface
  .icache_req_write       (icache_f2_sram_req_write),
  .icache_req_addr        (icache_f2_sram_req_addr),
  .icache_req_valid       (icache_f2_sram_req_valid),
  .icache_req_ready       (icache_f2_sram_req_ready),
  .icache_resp_dout       (icache_f0_sram_resp_dout),
  .icache_resp_valid      (icache_f0_sram_resp_valid),
  .icache_resp_ready      (icache_f0_sram_resp_ready),
  // L2 - Neighborhood response
  .neigh_sc_rsp_info      (etl_rsp_info),
  .neigh_sc_rsp_valid     (etl_rsp_valid),
  .neigh_sc_rsp_ready     (etl_rsp_ready),
  // ESRs
  .esr_shire_cache_ram_cfg('0),
  // BIST not currently supported; reserved for future use
  .bist_req_info          ('0),
  .bist_rsp_info          (i_icache_data_ram_bist_rsp_info_unused_ok),
  // APB signals for debug
  .apb_paddr              (l1_icache_apb_req.apb_paddr),
  .apb_pwrite             (l1_icache_apb_req.apb_pwrite),
  .apb_psel               (l1_icache_apb_req.apb_psel),
  .apb_penable            (l1_icache_apb_req.apb_penable),
  .apb_pwdata             (l1_icache_apb_req.apb_pwdata),
  .apb_pready             (l1_icache_apb_rsp.apb_pready),
  .apb_prdata             (l1_icache_apb_rsp.apb_prdata),
  .apb_pslverr            (l1_icache_apb_rsp.apb_pslverr)
);

logic [`SHIRE_DFT_SCANOUT_NEIGH_WIDTH-1:0] i_neigh_dft__scanout_hv_unused_ok;
logic i_neigh_dft__occ_scanout_hv_unused_ok;
logic [`ET_ECO_NEIGH_OUTPUT_WIDTH-1:0] i_neigh_eco_o_unused_ok;
logic i_neigh_pwr_ctrl_glb_nsleepout_unused_ok;
logic [`MIN_PER_N-1:0] i_neigh_pwr_ctrl_min_nsleepout_unused_ok;
bpam_rc_tbox_ack_t i_neigh_bpam_rc_tbox_ack_hi_unused_ok;
logic i_neigh_esr_icache_err_detected_unused_ok;
logic i_neigh_esr_icache_err_logged_unused_ok;
esr_and_or_tree_L0_t i_neigh_esr_and_or_tree_L0_unused_ok;
logic i_neigh_voltage_monitor_vdd_unused_ok;
logic i_neigh_voltage_monitor_vss_unused_ok;
coop_tload_slv_rdy_req_t i_neigh_coop_tload_slv_rdy_out_data_unused_ok;
logic i_neigh_coop_tload_slv_rdy_out_valid_unused_ok;
logic [`SHIRE_COOP_ID_RANGE] i_neigh_coop_tload_mst_done_out_coop_id_unused_ok;
logic [(`NUM_NEIGH-1)-1:0] i_neigh_coop_tload_mst_done_out_valid_unused_ok;
neigh_top i_neigh (
  // System signals
  .clock                          (clock),
  .reset_cold                     (reset_cold),
  .reset_warm                     (reset_warm),
  .reset_w_icache                 (reset_w_icache),
  // DFT signals
  .dft__scanin_hv                 ('0),
  .dft__scanout_hv                (i_neigh_dft__scanout_hv_unused_ok),
  .dft__scan_mode_hv              ('0),
  .dft__reset_byp_hv              ('0),
  .dft__scan_enable_hv            ('0),
  .dft__test_mode_hv              ('0),
  .dft__reset_hv                  ('0),
  .dft__clock_gate_en_hv          ('0),
  .dft__cntl_hv                   ('0),
  .dft__occ_scanin_hv             ('0),
  .dft__occ_scanout_hv            (i_neigh_dft__occ_scanout_hv_unused_ok),
  .dft__occ_reset_hv              ('0),
  .dft__occ_testmode_hv           ('0),
  .dft__scan_ate_clk_hv           ('0),
  .dft__occ_bypass_hv             ('0),
  .dft__use_reset_cntl_hv         ('0),
  .dft__reset_cntl_hv             ('0),
  // ECO ports
  .eco_i                          ('0),
  .eco_o                          (i_neigh_eco_o_unused_ok),
  // Power Ctrl - power control handled outside cpu subsystem and this ifce is not used
  .pwr_ctrl_glb_nsleepin          ('1),
  .pwr_ctrl_glb_nsleepout         (i_neigh_pwr_ctrl_glb_nsleepout_unused_ok),
  .pwr_ctrl_glb_isolate           ('0),
  .pwr_ctrl_min_nsleepin          ('1),
  .pwr_ctrl_min_nsleepout         (i_neigh_pwr_ctrl_min_nsleepout_unused_ok),
  .pwr_ctrl_min_isolate           ('0),
  // Clock gate Ctrl override
  .esr_clk_gate_ctrl              (neigh_clk_gate_ctrl),
  // Memory control override
  .esr_minion_mem_override        ('0),
  // Debug
  .bpam_rc_tbox_ack_hi            (i_neigh_bpam_rc_tbox_ack_hi_unused_ok),
  // ESRs
  .shire_id                       ('0),
  .neigh_id                       ('0),
  .shire_tbox_id                  ('0),
  .shire_tbox_en                  ('0),
  .esr_thread0_enable             (esr_thread0_enable),
  .esr_thread1_enable             (esr_thread1_enable),
  .esr_minion_features            (esr_minion_features),
  .esr_icache_prefetch_conf       (esr_icache_prefetch_conf),
  .esr_icache_prefetch_start      (esr_icache_prefetch_start),
  .esr_icache_prefetch_done       (esr_icache_prefetch_done),
  .esr_icache_err_detected        (i_neigh_esr_icache_err_detected_unused_ok),
  .esr_icache_err_logged          (i_neigh_esr_icache_err_logged_unused_ok),
  .dmctrl                         (dmctrl),
  .esr_and_or_tree_L0             (i_neigh_esr_and_or_tree_L0_unused_ok),
  .esr_shire_coop_mode            (esr_shire_coop_mode),
  // L2 Request bus
  .neigh_sc_req_ready             (neigh_sc_req_ready),
  .neigh_sc_req_valid             (neigh_sc_req_valid),
  .neigh_sc_req_info              (etl_req_info),
  // L2 Response bus
  .neigh_sc_rsp_ready             (etl_rsp_ready),
  .neigh_sc_rsp_valid             (etl_rsp_valid),
  .neigh_sc_rsp_info              (etl_rsp_info),
  // UltraSoc Status Monitor connection
  .hw_dbg_sm_monitor_enabled      (hw_dbg_sm_monitor_enabled),
  .neigh_sm_gpio                  (neigh_sm_gpio),
  .neigh_sm_signals               (neigh_sm_signals),
  //APB connection
  .APB_ESR_req                    (neigh_apb_req),
  .APB_ESR_rsp                    (neigh_apb_rsp),
  // Interrupts
  .int_mtip                       (int_mtip),
  .int_meip                       (plic_meip),
  .int_seip                       (plic_seip),
  // IPI
  .ipi_msip                       (ipi_msip),
  .ipi_redirect_trigger           (esr_ipi_redirect_trigger),
  .uc_to_neigh_fcc                (fcc),
  .uc_to_neigh_fcc_target         (fcc_target),
  // Fast Local Barrier request interface between neigh and L2
  .flb_neigh_l2_req_valid         (flb_neigh_l2_req_valid),
  .flb_neigh_l2_req_data          (flb_neigh_l2_req_data),
  // Fast Local Barrier response interface between neigh and
  .flb_l2_neigh_resp_valid        (flb_l2_neigh_resp_valid),
  .flb_l2_neigh_resp_data         (flb_l2_neigh_resp_data),
  // Neigh - Icache L1 SRAM interface
  .icache_f2_sram_req_write       (icache_f2_sram_req_write),
  .icache_f2_sram_req_addr        (icache_f2_sram_req_addr),
  .icache_f2_sram_req_valid       (icache_f2_sram_req_valid),
  .icache_f2_sram_req_ready       (icache_f2_sram_req_ready),
  .icache_f0_sram_resp_dout       (icache_f0_sram_resp_dout),
  .icache_f0_sram_resp_valid      (icache_f0_sram_resp_valid),
  .icache_f0_sram_resp_ready      (icache_f0_sram_resp_ready),
  // Voltage sensors ports
  .voltage_monitor_vdd            (i_neigh_voltage_monitor_vdd_unused_ok),
  .voltage_monitor_vss            (i_neigh_voltage_monitor_vss_unused_ok),
  // North interface to other Neigh
  // -------------------
  // Cooperative TLoad slave ready output bus
  .coop_tload_slv_rdy_out_data    (i_neigh_coop_tload_slv_rdy_out_data_unused_ok),
  .coop_tload_slv_rdy_out_valid   (i_neigh_coop_tload_slv_rdy_out_valid_unused_ok),
  // Cooperative TLoad slave ready input buses
  .coop_tload_slv_rdy_in_data     ('0),
  .coop_tload_slv_rdy_in_valid    ('0),
  // Cooperative TLoad master done output bus
  .coop_tload_mst_done_out_coop_id(i_neigh_coop_tload_mst_done_out_coop_id_unused_ok),
  .coop_tload_mst_done_out_valid  (i_neigh_coop_tload_mst_done_out_valid_unused_ok),
  // Cooperative TLoad master done input buses
  .coop_tload_mst_done_in_coop_id ('0),
  .coop_tload_mst_done_in_valid   ('0)
);

assign hw_dbg_sm_monitor_enabled = esr_sm_config.sm_enable;
assign neigh_sm_gpio = {esr_sm_config.sm_target_sel,esr_sm_config.sm_data_sel};

uncacheable_flb #(
  .NUM_NEIGH (1)
) i_flb (
  .clock                  (clock),
  .reset                  (reset_w),
  .esr_enables            (esr_enables.barriers),
  .esr_wdata              (apb_pwdata),
  .esr_barrier_values     (esr_values.barriers),
  // Fast Local Barrier
  .flb_neigh_l2_req_valid (flb_neigh_l2_req_valid),
  .flb_neigh_l2_req_data  (flb_neigh_l2_req_data),
  .flb_l2_neigh_resp_valid(flb_l2_neigh_resp_valid),
  .flb_l2_neigh_resp_data (flb_l2_neigh_resp_data)
);

uncacheable_fcc #(
  .NUM_NEIGH (1)
) i_fcc (
  // Operation request bus from minions
  .fcc_target (fcc_target),
  .fcc        (fcc),
  .esr_enables(esr_enables.fcc),
  .esr_wdata  (apb_pwdata)
);

esr_shire_other #(
  .AD_WIDTH (`ESR_SHIRE_OTHER_APB_AD_PP_WIDTH)
) i_esr (
   // System signals
   .clock                      (clock),
   .reset_c                    (reset_c),
   .reset_w                    (reset_w),
   .reset_d                    (reset_d),
   .test_en                    (1'b0), // DFT_SCAN_INSERT
   .dft__reset_byp             (1'b0),
   .dft__reset                 (1'b0),
   .esr_minion_feature_op      (esr_minion_features),
   .esr_icache_prefetch_conf   (esr_icache_prefetch_conf),
   .esr_icache_prefetch_start  (esr_icache_prefetch_start),
   .esr_icache_prefetch_done   (esr_icache_prefetch_done),
   .esr_thread0_enable         (esr_thread0_enable),
   .esr_thread1_enable         (esr_thread1_enable),
   .uc_esr_enables             (esr_enables),
   .uc_esr_values              (esr_values),
   .esr_clk_gate_ctrl_op       (esr_clk_gate_ctrl),
   .esr_shire_coop_mode_op     (esr_shire_coop_mode),
   .esr_ipi_trigger_op         (esr_ipi_trigger),
   .esr_ipi_redirect_trigger_op(esr_ipi_redirect_trigger),
   .esr_timer_regs_read_ip     (esr_timer_regs_read),
   .esr_timer_regs_write_op    (esr_timer_regs_write),
   .esr_timer_regs_we_op       (esr_timer_regs_we),
   .esr_sm_config_op           (esr_sm_config),
   .esr_sm_match               (esr_sm_match),
   .esr_sm_filter0             (esr_sm_filter0),
   .esr_sm_filter1             (esr_sm_filter1),
   .esr_sm_filter2             (esr_sm_filter2),
   .esr_sm_data0               (esr_sm_data0),
   .esr_sm_data1               (esr_sm_data1),
   .esr_dmctrl_op              (dmctrl),
   .apb_paddr                  (cpu_reg_apb_req.apb_paddr[`ESR_SHIRE_OTHER_APB_AD_PP_WIDTH-1:0]),
   .apb_pwrite                 (cpu_reg_apb_req.apb_pwrite),
   .apb_psel                   (cpu_reg_apb_req.apb_psel),
   .apb_penable                (cpu_reg_apb_req.apb_penable),
   .apb_pwdata                 (cpu_reg_apb_req.apb_pwdata),
   .apb_pready                 (cpu_reg_apb_rsp.apb_pready),
   .apb_prdata                 (cpu_reg_apb_rsp.apb_prdata),
   .apb_pslverr                (cpu_reg_apb_rsp.apb_pslverr)
);

assign neigh_clk_gate_ctrl = '{ min_frontend_clock_gate_disable: esr_clk_gate_ctrl.min_frontend_clock_gate_disable,
                                min_dcache_clock_gate_disable:   esr_clk_gate_ctrl.min_dcache_clock_gate_disable,
                                min_vputrans_clock_gate_disable: esr_clk_gate_ctrl.min_vputrans_clock_gate_disable,
                                min_vputima_clock_gate_disable:  esr_clk_gate_ctrl.min_vputima_clock_gate_disable,
                                min_vpulane_clock_gate_disable:  esr_clk_gate_ctrl.min_vpulane_clock_gate_disable,
                                min_intpipe_clock_gate_disable:  esr_clk_gate_ctrl.min_intpipe_clock_gate_disable};

assign esr_sm_match        = neigh_sm_signals.match_signals;
assign esr_sm_filter0      = neigh_sm_signals.filter_signals[63:0];
assign esr_sm_filter1      = neigh_sm_signals.filter_signals[127:64];
assign esr_sm_filter2      = neigh_sm_signals.filter_signals[191:128];
assign esr_sm_data0        = neigh_sm_signals.data_signals[63:0];
assign esr_sm_data1        = neigh_sm_signals.data_signals[127:64];

cpu_clint i_cpu_clint (
  .clock_ext       (clock_ext),
  .clock           (clock),
  .reset           (reset_w),
  .timer_regs_we   (esr_timer_regs_we),
  .timer_regs_write(esr_timer_regs_write),
  .timer_regs_read (esr_timer_regs_read),
  .mtip            (int_mtip),
  .esr_ipi_trigger (esr_ipi_trigger),
  .msip            (ipi_msip)
);

// =========================
// CPU Power Down Control
// =========================
// Power down request is acknowledged when no APB transaction is ongoing by checking PSEL signal.
// When the power is off, the APB bus will respond with slave error.
// UPF isolation clamps APB outputs (pready=1, pslverr=1) when powered off.
// AXI transactions are not considered since CPU subsystem is a master and responses will be aborsorbed.
// When the power is off, UPF isolation will clamp AXI ready signals (axi_rready=1, axi_bready=1).

assign apb_is_idle      = ~cpu_apb_req.apb_psel & ~cpu_apb_req.apb_penable;
assign cpu_pd_ack_ff    = cpu_pd_req & apb_is_idle;
assign minion_pd_ack_ff = minion_pd_req & {`MIN_PER_N{apb_is_idle}};

//      CLK    RST      DOUT           DIN               DEF
`RST_FF(clock, reset_w, cpu_pd_ack,    cpu_pd_ack_ff,    '0)
`RST_FF(clock, reset_w, minion_pd_ack, minion_pd_ack_ff, '0)
 
`ifdef ET_SIMULATION
   logic [`MIN_PER_N-1:0] monitor_minion_mask;
   always_comb begin
      //foreach(monitor_minion_mask[i])
      //  monitor_minion_mask[i] = i%2 == 0 ? esr_thread0_enable[i/2] : esr_thread1_enable[i/2];
      monitor_minion_mask = '0; // Mask indicates which minions were disabled but we don't have a way to disable right now
   end
   
   et_cpu_ss_monitor 
   #( .NUM_MINIONS(`MIN_PER_N) )
   cpu_subsystem_monitor
   (
    .reset ( reset_cold ),
    .clock ( clock ),
    .minion_mask (monitor_minion_mask)
   );
                     
`endif

endmodule
