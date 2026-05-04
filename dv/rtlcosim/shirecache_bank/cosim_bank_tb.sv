// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_bank vs new shirecache_bank.
//
// Both modules are full bank containers with ~12 sub-modules each.
// Identical stimulus is driven; key outputs are compared cycle-by-cycle
// from C++.
//
// Key differences handled:
// - Original: active-high sync reset, ESR block inside
// - New: active-low async reset, ESR externalized as ports
// - Original uses DFT scan signals; new uses dft_t struct
// - Original uses esr_shire_cache_ram_cfg; new uses ram_cfg_t
// - Original masks trans_id under EVL_SIMULATION; new always passes through

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off WIDTHEXPAND */
/* verilator lint_off WIDTHTRUNC */

module cosim_bank_tb
  import shirecache_pkg::*;
  import etlink_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,

  // ── Bank request (xbar → bank) ──
  input  logic                        bank_req_valid_i,
  // xbar_req_t fields (decomposed for C++ driver)
  input  logic [PortIdSize-1:0]       bri_port_id_i,
  input  logic [DvTransIdSize-1:0]    bri_trans_id_i,
  // etlink req_t subfields:
  input  logic [4:0]                  bri_opcode_i,
  input  logic [3:0]                  bri_subopcode_i,
  input  logic [PaSize-1:0]           bri_address_i,
  input  logic [2:0]                  bri_size_i,
  input  logic [LineQwSize-1:0]       bri_qwen_i,
  input  logic [LineDataSize-1:0]     bri_data_i,
  input  logic                        bri_wdata_i,
  input  logic [etlink_pkg::IdSize-1:0] bri_id_i,
  input  logic [etlink_pkg::SourceSize-1:0] bri_source_i,

  // ── Bank response credits ──
  input  logic [Ports-1:0]            bank_rsp_ready_i,
  input  logic [Ports-1:0]            bank_rsp_2_credits_i,

  // ── Mesh master ready signals (backpressure) ──
  input  logic                        to_l3_mesh_master_bank_req_ready_i,
  input  logic                        to_l3_mesh_master_bank_rsp_valid_i,
  input  logic                        to_sys_mesh_master_bank_req_ready_i,
  input  logic                        to_sys_mesh_master_bank_rsp_valid_i,

  // ── L3 mesh slave ──
  input  logic                        l3_mesh_slave_bank_clk_en_i,
  input  logic                        l3_mesh_slave_bank_req_valid_i,
  input  logic                        l3_mesh_slave_bank_rsp_ready_i,

  // ── ESR config scalars (driven same to both) ──
  input  logic [RamDelaySize-1:0]     esr_sc_ram_delay_i,
  input  logic [SetIdSize-1:0]        esr_sc_l2_set_base_i,
  input  logic [SetAllocSize-1:0]     esr_sc_l2_set_size_i,
  input  logic [SetIdSize-1:0]        esr_sc_l2_set_mask_i,
  input  logic [SetIdSize-1:0]        esr_sc_l2_tag_mask_i,
  input  logic [SetIdSize-1:0]        esr_sc_l3_set_base_i,
  input  logic [SetAllocSize-1:0]     esr_sc_l3_set_size_i,
  input  logic [SetIdSize-1:0]        esr_sc_l3_set_mask_i,
  input  logic [SetIdSize-1:0]        esr_sc_l3_tag_mask_i,

  input  logic [BankIdSize-1:0]       my_bank_id_i,
  input  logic [ScpShireIdSize-1:0]   esr_shire_id_i,

  // ── Comparison outputs: NEW ──
  output logic                        new_bank_req_ready_o,
  output logic [Ports-1:0]            new_bank_rsp_valid_o,
  output logic [$bits(xbar_rsp_t)-1:0] new_bank_rsp_info_o,
  output logic                        new_to_l3_req_valid_o,
  output logic [$bits(mesh_master_req_t)-1:0] new_to_l3_req_info_o,
  output logic                        new_to_l3_rsp_ready_o,
  output logic                        new_to_l3_clk_en_o,
  output logic                        new_to_sys_req_valid_o,
  output logic [$bits(mesh_master_req_t)-1:0] new_to_sys_req_info_o,
  output logic                        new_to_sys_rsp_ready_o,
  output logic                        new_to_sys_clk_en_o,
  output logic                        new_l3s_req_ready_o,
  output logic                        new_l3s_rsp_valid_o,
  output logic [$bits(mesh_slave_rsp_t)-1:0] new_l3s_rsp_info_o,
  output logic                        new_err_detected_o,
  output logic                        new_err_logged_o,
  output logic                        new_trace_valid_o,
  output logic                        new_l2hpf_req_valid_o,
  output logic                        new_esr_sc_remote_l3_enable_o,
  output logic                        new_esr_sc_remote_scp_enable_o,

  // ── Comparison outputs: ORIGINAL ──
  output logic                        orig_bank_req_ready_o,
  output logic [Ports-1:0]            orig_bank_rsp_valid_o,
  output logic [$bits(xbar_rsp_t)-1:0] orig_bank_rsp_info_o,
  output logic                        orig_to_l3_req_valid_o,
  output logic [$bits(mesh_master_req_t)-1:0] orig_to_l3_req_info_o,
  output logic                        orig_to_l3_rsp_ready_o,
  output logic                        orig_to_l3_clk_en_o,
  output logic                        orig_to_sys_req_valid_o,
  output logic [$bits(mesh_master_req_t)-1:0] orig_to_sys_req_info_o,
  output logic                        orig_to_sys_rsp_ready_o,
  output logic                        orig_to_sys_clk_en_o,
  output logic                        orig_l3s_req_ready_o,
  output logic                        orig_l3s_rsp_valid_o,
  output logic [$bits(mesh_slave_rsp_t)-1:0] orig_l3s_rsp_info_o,
  output logic                        orig_err_detected_o,
  output logic                        orig_err_logged_o,
  output logic                        orig_trace_valid_o,
  output logic                        orig_l2hpf_req_valid_o,
  output logic                        orig_esr_sc_remote_l3_enable_o,
  output logic                        orig_esr_sc_remote_scp_enable_o
);

  // ════════════════════════════════════════════════════════
  // Reset
  // ════════════════════════════════════════════════════════
  wire reset = !rst_ni;

  // ════════════════════════════════════════════════════════
  // Assemble struct inputs
  // ════════════════════════════════════════════════════════

  // ── xbar_req_t for new module ──
  xbar_req_t new_bank_req_info;
  always_comb begin
    new_bank_req_info.port_id           = bri_port_id_i;
    new_bank_req_info.trans_id          = bri_trans_id_i;
    new_bank_req_info.req_info.opcode   = etlink_pkg::req_opcode_e'(bri_opcode_i);
    new_bank_req_info.req_info.subopcode = bri_subopcode_i;
    new_bank_req_info.req_info.address  = bri_address_i;
    new_bank_req_info.req_info.size     = etlink_pkg::size_e'(bri_size_i);
    new_bank_req_info.req_info.qwen     = bri_qwen_i;
    new_bank_req_info.req_info.data     = bri_data_i;
    new_bank_req_info.req_info.wdata    = bri_wdata_i;
    new_bank_req_info.req_info.id       = bri_id_i;
    new_bank_req_info.req_info.source   = bri_source_i;
  end

  sc_xbar_req_t orig_bank_req_info;
  assign orig_bank_req_info = new_bank_req_info;

  // ── Mesh master rsp inputs (tie to zero for cosim — no external fills) ──
  mesh_master_rsp_t new_mesh_rsp_zero;
  assign new_mesh_rsp_zero = '0;

  sc_mesh_master_rsp_t orig_mesh_rsp_zero;
  assign orig_mesh_rsp_zero = '0;

  // ── L3 slave request (tie to zero — no L3 requests in basic cosim) ──
  mesh_slave_req_t new_l3s_req_zero;
  assign new_l3s_req_zero = '0;

  sc_mesh_slave_req_t orig_l3s_req_zero;
  assign orig_l3s_req_zero = '0;

  // ── ESR ctl for new module ──
  bank_esr_ctl_t new_esr_ctl;
  always_comb begin
    new_esr_ctl = '0;
    new_esr_ctl.esr_sc_ram_delay      = esr_sc_ram_delay_i;
    new_esr_ctl.esr_sc_l2_set_base    = esr_sc_l2_set_base_i;
    new_esr_ctl.esr_sc_l2_set_size    = esr_sc_l2_set_size_i;
    new_esr_ctl.esr_sc_l2_set_mask    = esr_sc_l2_set_mask_i;
    new_esr_ctl.esr_sc_l2_tag_mask    = esr_sc_l2_tag_mask_i;
    new_esr_ctl.esr_sc_l3_set_base    = esr_sc_l3_set_base_i;
    new_esr_ctl.esr_sc_l3_set_size    = esr_sc_l3_set_size_i;
    new_esr_ctl.esr_sc_l3_set_mask    = esr_sc_l3_set_mask_i;
    new_esr_ctl.esr_sc_l3_tag_mask    = esr_sc_l3_tag_mask_i;
  end

  // ── ESR shire config ──
  esr_shire_config_t new_esr_shire_config;
  always_comb begin
    new_esr_shire_config = '0;
    new_esr_shire_config.shire_id = esr_shire_id_i;
  end

  esr_shire_config_t orig_esr_shire_config;
  assign orig_esr_shire_config = new_esr_shire_config;

  // ── DFT ──
  dft_t new_dft;
  assign new_dft = '0;

  ram_cfg_t new_ram_cfg;
  assign new_ram_cfg = '0;

  // ── ICACHE BIST rsp (tie to zero) ──
  icache_bist_rsp_t new_icache_bist_rsp;
  assign new_icache_bist_rsp = '0;

  sc_icache_bist_rsp_t orig_icache_bist_rsp;
  assign orig_icache_bist_rsp = '0;

  // ── APB ESR for original (idle, no transactions) ──
  APB_to_bank_ESRs_t orig_apb_esr_req;
  always_comb begin
    orig_apb_esr_req = '0;
  end

  // ── esr_shire_cache_ram_cfg for original (tie to zero) ──
  esr_shire_cache_ram_cfg_t orig_ram_cfg;
  assign orig_ram_cfg = '0;

  // ════════════════════════════════════════════════════════
  // NEW module instance
  // ════════════════════════════════════════════════════════

  xbar_rsp_t           new_bank_rsp;
  mesh_master_req_t    new_to_l3_req;
  mesh_master_req_t    new_to_sys_req;
  mesh_slave_rsp_t     new_l3s_rsp;
  icache_bist_req_t    new_icache_bist_req;
  bank_esr_status_t    new_esr_status;
  trace_packet_t       new_trace_data;
  neigh_l2hpf_req_t    new_l2hpf_req;
  l3_swizzle_ctl_t     new_swizzle_ctl;

  shirecache_bank u_new (
    .clk_i                              (clk_i),
    .clk_free_i                         (clk_i),  // same as clk for cosim
    .rst_ni                             (rst_ni),
    .rst_c_ni                           (rst_ni),
    .rst_d_ni                           (rst_ni),

    .dft_i                              (new_dft),
    .dft_sram_clk_i                     (1'b0),
    .dft_mbist_en_i                     (1'b0),
    .eco_i                              ('0),
    .eco_o                              (),
    .ram_cfg_i                          (new_ram_cfg),
    .mbi_implemented_const_i            (1'b0),

    // Bank request
    .bank_req_ready_o                   (new_bank_req_ready_o),
    .bank_req_valid_i                   (bank_req_valid_i),
    .bank_req_info_i                    (new_bank_req_info),

    // Bank response
    .bank_rsp_valid_o                   (new_bank_rsp_valid_o),
    .bank_rsp_info_o                    (new_bank_rsp),
    .bank_rsp_ready_i                   (bank_rsp_ready_i),
    .bank_rsp_2_credits_i               (bank_rsp_2_credits_i),

    // To-L3 mesh
    .to_l3_mesh_master_bank_clk_en_o    (new_to_l3_clk_en_o),
    .to_l3_mesh_master_bank_req_ready_i (to_l3_mesh_master_bank_req_ready_i),
    .to_l3_mesh_master_bank_req_valid_o (new_to_l3_req_valid_o),
    .to_l3_mesh_master_bank_req_info_o  (new_to_l3_req),
    .to_l3_mesh_master_bank_rsp_ready_o (new_to_l3_rsp_ready_o),
    .to_l3_mesh_master_bank_rsp_valid_i (to_l3_mesh_master_bank_rsp_valid_i),
    .to_l3_mesh_master_bank_rsp_info_i  (new_mesh_rsp_zero),

    // To-sys mesh
    .to_sys_mesh_master_bank_clk_en_o   (new_to_sys_clk_en_o),
    .to_sys_mesh_master_bank_req_ready_i(to_sys_mesh_master_bank_req_ready_i),
    .to_sys_mesh_master_bank_req_valid_o(new_to_sys_req_valid_o),
    .to_sys_mesh_master_bank_req_info_o (new_to_sys_req),
    .to_sys_mesh_master_bank_rsp_ready_o(new_to_sys_rsp_ready_o),
    .to_sys_mesh_master_bank_rsp_valid_i(to_sys_mesh_master_bank_rsp_valid_i),
    .to_sys_mesh_master_bank_rsp_info_i (new_mesh_rsp_zero),

    // L3 mesh slave
    .l3_mesh_slave_bank_clk_en_i        (l3_mesh_slave_bank_clk_en_i),
    .l3_mesh_slave_bank_req_ready_o     (new_l3s_req_ready_o),
    .l3_mesh_slave_bank_req_valid_i     (l3_mesh_slave_bank_req_valid_i),
    .l3_mesh_slave_bank_req_info_i      (new_l3s_req_zero),
    .l3_mesh_slave_bank_rsp_ready_i     (l3_mesh_slave_bank_rsp_ready_i),
    .l3_mesh_slave_bank_rsp_valid_o     (new_l3s_rsp_valid_o),
    .l3_mesh_slave_bank_rsp_info_o      (new_l3s_rsp),

    // BIST
    .icache_bist_req_info_o             (new_icache_bist_req),
    .icache_bist_rsp_info_i             (new_icache_bist_rsp),
    .mbist_en_i                         (1'b0),
    .mbist_sel_i                        ('0),
    .mbist_rd_en_i                      (1'b0),
    .mbist_wr_en_i                      (1'b0),
    .mbist_addr_i                       ('0),
    .mbist_wdata_i                      ('0),
    .mbist_rdata_o                      (),

    // ESR
    .esr_ctl_i                          (new_esr_ctl),
    .esr_status_o                       (new_esr_status),
    .esr_shire_config_i                 (new_esr_shire_config),

    // Errors
    .err_detected_o                     (new_err_detected_o),
    .err_logged_o                       (new_err_logged_o),

    // Constants
    .my_bank_id_i                       (my_bank_id_i),

    // Trace
    .bank_trace_data_o                  (new_trace_data),
    .bank_trace_valid_o                 (new_trace_valid_o),

    // L2 HPF
    .l2hpf_req_valid_o                  (new_l2hpf_req_valid_o),
    .l2hpf_req_info_o                   (new_l2hpf_req),

    // ESR forwarded
    .esr_sc_remote_l3_enable_o          (new_esr_sc_remote_l3_enable_o),
    .esr_sc_remote_scp_enable_o         (new_esr_sc_remote_scp_enable_o),
    .esr_sc_l3_shire_swizzle_ctl_o      (new_swizzle_ctl)
  );

  assign new_bank_rsp_info_o   = new_bank_rsp;
  assign new_to_l3_req_info_o  = new_to_l3_req;
  assign new_to_sys_req_info_o = new_to_sys_req;
  assign new_l3s_rsp_info_o    = new_l3s_rsp;

  // ════════════════════════════════════════════════════════
  // ORIGINAL module instance
  // ════════════════════════════════════════════════════════

  sc_xbar_rsp_t           orig_bank_rsp;
  sc_mesh_master_req_t    orig_to_l3_req;
  sc_mesh_master_req_t    orig_to_sys_req;
  sc_mesh_slave_rsp_t     orig_l3s_rsp;
  sc_icache_bist_req_t    orig_icache_bist_req;
  APB_from_bank_ESRs_t    orig_apb_esr_rsp;
  sc_trace_packet_t       orig_trace_data;
  sc_neigh_l2hpf_req_t    orig_l2hpf_req;
  esr_sc_l3_shire_swizzle_ctl_t orig_swizzle_ctl;

  shire_cache_bank u_orig (
    .clock                              (clk_i),
    .reset_c_in                         (reset),
    .reset_w_in                         (reset),
    .reset_d_in                         (reset),
    .mbi_implemented_const              (1'b0),

    // Bank request
    .bank_req_ready                     (orig_bank_req_ready_o),
    .bank_req_valid                     (bank_req_valid_i),
    .bank_req_info                      (orig_bank_req_info),

    // Bank response
    .bank_rsp_valid                     (orig_bank_rsp_valid_o),
    .bank_rsp_info                      (orig_bank_rsp),
    .bank_rsp_ready                     (bank_rsp_ready_i),
    .bank_rsp_2_credits                 (bank_rsp_2_credits_i),

    // To-L3 mesh
    .to_l3_mesh_master_bank_clk_en      (orig_to_l3_clk_en_o),
    .to_l3_mesh_master_bank_req_ready   (to_l3_mesh_master_bank_req_ready_i),
    .to_l3_mesh_master_bank_req_valid   (orig_to_l3_req_valid_o),
    .to_l3_mesh_master_bank_req_info    (orig_to_l3_req),
    .to_l3_mesh_master_bank_rsp_ready   (orig_to_l3_rsp_ready_o),
    .to_l3_mesh_master_bank_rsp_valid   (to_l3_mesh_master_bank_rsp_valid_i),
    .to_l3_mesh_master_bank_rsp_info    (orig_mesh_rsp_zero),

    // To-sys mesh
    .to_sys_mesh_master_bank_clk_en     (orig_to_sys_clk_en_o),
    .to_sys_mesh_master_bank_req_ready  (to_sys_mesh_master_bank_req_ready_i),
    .to_sys_mesh_master_bank_req_valid  (orig_to_sys_req_valid_o),
    .to_sys_mesh_master_bank_req_info   (orig_to_sys_req),
    .to_sys_mesh_master_bank_rsp_ready  (orig_to_sys_rsp_ready_o),
    .to_sys_mesh_master_bank_rsp_valid  (to_sys_mesh_master_bank_rsp_valid_i),
    .to_sys_mesh_master_bank_rsp_info   (orig_mesh_rsp_zero),

    // L3 mesh slave
    .l3_mesh_slave_bank_clk_en          (l3_mesh_slave_bank_clk_en_i),
    .l3_mesh_slave_bank_req_ready       (orig_l3s_req_ready_o),
    .l3_mesh_slave_bank_req_valid       (l3_mesh_slave_bank_req_valid_i),
    .l3_mesh_slave_bank_req_info        (orig_l3s_req_zero),
    .l3_mesh_slave_bank_rsp_ready       (l3_mesh_slave_bank_rsp_ready_i),
    .l3_mesh_slave_bank_rsp_valid       (orig_l3s_rsp_valid_o),
    .l3_mesh_slave_bank_rsp_info        (orig_l3s_rsp),

    // BIST
    .icache_bist_req_info               (orig_icache_bist_req),
    .icache_bist_rsp_info               (orig_icache_bist_rsp),

    // ESR (APB — idle)
    .APB_ESR_req                        (orig_apb_esr_req),
    .APB_ESR_rsp                        (orig_apb_esr_rsp),

    // Errors
    .err_detected                       (orig_err_detected_o),
    .err_logged                         (orig_err_logged_o),
    .my_bank_id                         (my_bank_id_i),

    // Trace
    .bank_trace_data                    (orig_trace_data),
    .bank_trace_valid                   (orig_trace_valid_o),

    // L2 HPF
    .sc_neigh_l2hpf_req_valid           (orig_l2hpf_req_valid_o),
    .sc_neigh_l2hpf_req_info            (orig_l2hpf_req),

    // Shire config
    .esr_shire_config                   (orig_esr_shire_config),
    .esr_shire_cache_ram_cfg            (orig_ram_cfg),

    .esr_sc_remote_l3_enable            (orig_esr_sc_remote_l3_enable_o),
    .esr_sc_remote_scp_enable           (orig_esr_sc_remote_scp_enable_o),
    .esr_sc_l3_shire_swizzle_ctl        (orig_swizzle_ctl),

    // DFT (all tied off)
    .dft__scanin_hv                     ('0),
    .dft__scanout_hv                    (),
    .dft__scan_mode_hv                  (1'b0),
    .dft__reset_byp_hv                  (1'b0),
    .dft__scan_enable_hv                (1'b0),
    .dft__test_mode_hv                  ('0),
    .dft__reset_hv                      (1'b0),
    .dft__clock_gate_en_hv              (1'b0),
    .dft__cntl_hv                       ('0),
    .dft__occ_scanin_hv                 (1'b0),
    .dft__occ_scanout_hv                (),
    .dft__occ_reset_hv                  (1'b0),
    .dft__occ_testmode_hv               (1'b0),
    .dft__scan_ate_clk_hv               (1'b0),
    .dft__occ_bypass_hv                 (1'b0),
    .dft__sram_clock                    (1'b0),
    .dft__clk_override                  (1'b0),
    .dft__mbist_en                      (1'b0),

    // ECO (tied off)
    .eco_i                              ('0),
    .eco_o                              ()
  );

  assign orig_bank_rsp_info_o   = orig_bank_rsp;
  assign orig_to_l3_req_info_o  = orig_to_l3_req;
  assign orig_to_sys_req_info_o = orig_to_sys_req;
  assign orig_l3s_rsp_info_o    = orig_l3s_rsp;

endmodule
/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on UNOPTFLAT */
/* verilator lint_on UNUSEDSIGNAL */
