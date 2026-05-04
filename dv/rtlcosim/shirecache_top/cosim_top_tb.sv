// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache vs new shirecache_top.
//
// Both modules are the full top-level cache container:
//   - 4 banks, req/rsp crossbars, UC relay FIFO
//   - mesh master (to_l3, to_sys) with async FIFOs
//   - mesh slave (l3) with async FIFOs
//   - reset synchronizers, trace mux, bist
//
// Identical stimulus is driven; key outputs are compared cycle-by-cycle
// from C++.
//
// Key differences handled:
// - Original: active-high sync reset, internal ESR blocks, DFT scalars
// - New: active-low async reset, ESR externalized, dft_t struct
// - Original uses debug_axi_port (not present in new)
// - Original uses status_monitor_gpio[1:0] for bank select; new uses
//   status_monitor_bank_sel_i
// - Clock: noc_clk = cache_clk for simplicity (no real CDC test)

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off WIDTHEXPAND */
/* verilator lint_off WIDTHTRUNC */

module cosim_top_tb
  import shirecache_pkg::*;
  import axi_pkg::*;
  import etlink_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;
(
  input  logic                        clk_i,
  input  logic                        rst_ni,

  // ── Neighborhood request interface (per-port, decomposed) ──
  // We support driving one request at a time via port_sel
  input  logic [2:0]                  req_port_sel_i,      // which port (0..4) to drive
  input  logic [Banks:0]              req_dest_valid_i,     // dest valid mask [banks+uc]
  // etlink req_t subfields:
  input  logic [4:0]                  req_opcode_i,
  input  logic [3:0]                  req_subopcode_i,
  input  logic [PaSize-1:0]           req_address_i,
  input  logic [2:0]                  req_size_i,
  input  logic [LineQwSize-1:0]       req_qwen_i,
  input  logic [LineDataSize-1:0]     req_data_i,
  input  logic                        req_wdata_i,
  input  logic [etlink_pkg::IdSize-1:0] req_id_i,
  input  logic [etlink_pkg::SourceSize-1:0] req_source_i,

  // ── Neighborhood response ready (per-port) ──
  input  logic [Ports-1:0]            neigh_sc_rsp_ready_i,

  // ── UC request ready (backpressure from UC block) ──
  input  logic                        neigh_uc_req_ready_i,

  // ── UC response valid/info ──
  input  logic [Ports-1:0]            neigh_uc_rsp_valid_i,

  // ── AXI backpressure for mesh master (to_l3 and to_sys) ──
  input  logic [L3MasterPorts-1:0]    to_l3_axi_ar_ready_i,
  input  logic [L3MasterPorts-1:0]    to_l3_axi_aw_ready_i,
  input  logic [L3MasterPorts-1:0]    to_l3_axi_w_ready_i,
  input  logic [L3MasterPorts-1:0]    to_l3_axi_r_valid_i,
  input  logic [L3MasterPorts-1:0]    to_l3_axi_b_valid_i,

  input  logic [SysPorts-1:0]         to_sys_axi_ar_ready_i,
  input  logic [SysPorts-1:0]         to_sys_axi_aw_ready_i,
  input  logic [SysPorts-1:0]         to_sys_axi_w_ready_i,
  input  logic [SysPorts-1:0]         to_sys_axi_r_valid_i,
  input  logic [SysPorts-1:0]         to_sys_axi_b_valid_i,

  // ── L3 slave AXI (idle in basic cosim) ──
  input  logic [L3SlavePorts-1:0]     l3_axi_ar_valid_i,
  input  logic [L3SlavePorts-1:0]     l3_axi_aw_valid_i,
  input  logic [L3SlavePorts-1:0]     l3_axi_w_valid_i,
  input  logic [L3SlavePorts-1:0]     l3_axi_r_ready_i,
  input  logic [L3SlavePorts-1:0]     l3_axi_b_ready_i,

  // ── ESR config scalars ──
  input  logic [RamDelaySize-1:0]     esr_sc_ram_delay_i,
  input  logic [SetIdSize-1:0]        esr_sc_l2_set_base_i,
  input  logic [SetAllocSize-1:0]     esr_sc_l2_set_size_i,
  input  logic [SetIdSize-1:0]        esr_sc_l2_set_mask_i,
  input  logic [SetIdSize-1:0]        esr_sc_l2_tag_mask_i,
  input  logic [SetIdSize-1:0]        esr_sc_l3_set_base_i,
  input  logic [SetAllocSize-1:0]     esr_sc_l3_set_size_i,
  input  logic [SetIdSize-1:0]        esr_sc_l3_set_mask_i,
  input  logic [SetIdSize-1:0]        esr_sc_l3_tag_mask_i,
  input  logic [ScpShireIdSize-1:0]   esr_shire_id_i,

  input  logic [BankIdSize-1:0]       status_monitor_bank_sel_i,

  // ── Comparison outputs: NEW ──
  output logic [Ports-1:0]            new_neigh_sc_rsp_valid_o,
  output logic                        new_neigh_uc_req_valid_o,
  output logic [Ports-1:0]            new_neigh_uc_rsp_ready_o,
  output logic                        new_esr_sc_remote_l3_enable_o,
  output logic                        new_esr_sc_remote_scp_enable_o,
  output logic [Banks-1:0]            new_sc_bank_err_detected_o,
  output logic [Banks-1:0]            new_sc_bank_err_logged_o,
  output logic                        new_sc_trace_valid_o,
  output logic [L3MasterPorts-1:0]    new_to_l3_axi_ar_valid_o,
  output logic [L3MasterPorts-1:0]    new_to_l3_axi_aw_valid_o,
  output logic [L3MasterPorts-1:0]    new_to_l3_axi_w_valid_o,
  output logic [L3MasterPorts-1:0]    new_to_l3_axi_r_ready_o,
  output logic [L3MasterPorts-1:0]    new_to_l3_axi_b_ready_o,
  output logic [SysPorts-1:0]         new_to_sys_axi_ar_valid_o,
  output logic [SysPorts-1:0]         new_to_sys_axi_aw_valid_o,
  output logic [SysPorts-1:0]         new_to_sys_axi_w_valid_o,
  output logic [SysPorts-1:0]         new_to_sys_axi_r_ready_o,
  output logic [SysPorts-1:0]         new_to_sys_axi_b_ready_o,
  output logic [L3SlavePorts-1:0]     new_l3_axi_ar_ready_o,
  output logic [L3SlavePorts-1:0]     new_l3_axi_aw_ready_o,
  output logic [L3SlavePorts-1:0]     new_l3_axi_w_ready_o,
  output logic [L3SlavePorts-1:0]     new_l3_axi_r_valid_o,
  output logic [L3SlavePorts-1:0]     new_l3_axi_b_valid_o,

  // ── Comparison outputs: ORIGINAL ──
  output logic [Ports-1:0]            orig_neigh_sc_rsp_valid_o,
  output logic                        orig_neigh_uc_req_valid_o,
  output logic [Ports-1:0]            orig_neigh_uc_rsp_ready_o,
  output logic                        orig_esr_sc_remote_l3_enable_o,
  output logic                        orig_esr_sc_remote_scp_enable_o,
  output logic [Banks-1:0]            orig_sc_bank_err_detected_o,
  output logic [Banks-1:0]            orig_sc_bank_err_logged_o,
  output logic                        orig_sc_trace_valid_o,
  output logic [L3MasterPorts-1:0]    orig_to_l3_axi_ar_valid_o,
  output logic [L3MasterPorts-1:0]    orig_to_l3_axi_aw_valid_o,
  output logic [L3MasterPorts-1:0]    orig_to_l3_axi_w_valid_o,
  output logic [L3MasterPorts-1:0]    orig_to_l3_axi_r_ready_o,
  output logic [L3MasterPorts-1:0]    orig_to_l3_axi_b_ready_o,
  output logic [SysPorts-1:0]         orig_to_sys_axi_ar_valid_o,
  output logic [SysPorts-1:0]         orig_to_sys_axi_aw_valid_o,
  output logic [SysPorts-1:0]         orig_to_sys_axi_w_valid_o,
  output logic [SysPorts-1:0]         orig_to_sys_axi_r_ready_o,
  output logic [SysPorts-1:0]         orig_to_sys_axi_b_ready_o,
  output logic [L3SlavePorts-1:0]     orig_l3_axi_ar_ready_o,
  output logic [L3SlavePorts-1:0]     orig_l3_axi_aw_ready_o,
  output logic [L3SlavePorts-1:0]     orig_l3_axi_w_ready_o,
  output logic [L3SlavePorts-1:0]     orig_l3_axi_r_valid_o,
  output logic [L3SlavePorts-1:0]     orig_l3_axi_b_valid_o
);

  // ════════════════════════════════════════════════════════
  // Reset
  // ════════════════════════════════════════════════════════
  wire reset = !rst_ni;

  // ════════════════════════════════════════════════════════
  // Build per-port request valid/info arrays
  // ════════════════════════════════════════════════════════

  // NEW module request ports
  logic [Ports-1:0][Banks:0]             new_neigh_sc_req_valid;
  etlink_pkg::req_t [Ports-1:0]          new_neigh_sc_req_info;

  // ORIGINAL module request ports (flat)
  logic [Ports*(Banks+1)-1:0]            orig_neigh_sc_req_valid;
  et_link_req_info_t [Ports-1:0]         orig_neigh_sc_req_info;

  // Build the request from input scalars
  etlink_pkg::req_t req_info_assembled;
  always_comb begin
    req_info_assembled.opcode    = etlink_pkg::req_opcode_e'(req_opcode_i);
    req_info_assembled.subopcode = req_subopcode_i;
    req_info_assembled.address   = req_address_i;
    req_info_assembled.size      = etlink_pkg::size_e'(req_size_i);
    req_info_assembled.qwen      = req_qwen_i;
    req_info_assembled.data      = req_data_i;
    req_info_assembled.wdata     = req_wdata_i;
    req_info_assembled.id        = req_id_i;
    req_info_assembled.source    = req_source_i;
  end

  et_link_req_info_t orig_req_info_assembled;
  assign orig_req_info_assembled = req_info_assembled;

  // Route request to selected port; all others idle
  always_comb begin
    for (int p = 0; p < Ports; p++) begin
      new_neigh_sc_req_valid[p] = '0;
      new_neigh_sc_req_info[p]  = '0;
    end
    if (req_port_sel_i < Ports) begin
      new_neigh_sc_req_valid[req_port_sel_i] = req_dest_valid_i;
      new_neigh_sc_req_info[req_port_sel_i]  = req_info_assembled;
    end
  end

  // Build flat valid for original (packed as [port*(B+1) + dest])
  always_comb begin
    orig_neigh_sc_req_valid = '0;
    for (int p = 0; p < Ports; p++) begin
      orig_neigh_sc_req_info[p] = '0;
    end
    if (req_port_sel_i < Ports) begin
      for (int d = 0; d < Banks + 1; d++) begin
        orig_neigh_sc_req_valid[req_port_sel_i * (Banks + 1) + d] = req_dest_valid_i[d];
      end
      orig_neigh_sc_req_info[req_port_sel_i] = orig_req_info_assembled;
    end
  end

  // ════════════════════════════════════════════════════════
  // Build UC response info (tie to zero since we don't drive UC rsp data)
  // ════════════════════════════════════════════════════════
  xbar_rsp_t      new_uc_rsp_info;
  assign new_uc_rsp_info = '0;

  sc_xbar_rsp_t   orig_uc_rsp_info;
  assign orig_uc_rsp_info = '0;

  // ════════════════════════════════════════════════════════
  // ESR config for new module (per-bank, all identical)
  // ════════════════════════════════════════════════════════
  bank_esr_ctl_t [Banks-1:0] new_bank_esr_ctl;
  always_comb begin
    for (int b = 0; b < Banks; b++) begin
      new_bank_esr_ctl[b] = '0;
      new_bank_esr_ctl[b].esr_sc_ram_delay   = esr_sc_ram_delay_i;
      new_bank_esr_ctl[b].esr_sc_l2_set_base = esr_sc_l2_set_base_i;
      new_bank_esr_ctl[b].esr_sc_l2_set_size = esr_sc_l2_set_size_i;
      new_bank_esr_ctl[b].esr_sc_l2_set_mask = esr_sc_l2_set_mask_i;
      new_bank_esr_ctl[b].esr_sc_l2_tag_mask = esr_sc_l2_tag_mask_i;
      new_bank_esr_ctl[b].esr_sc_l3_set_base = esr_sc_l3_set_base_i;
      new_bank_esr_ctl[b].esr_sc_l3_set_size = esr_sc_l3_set_size_i;
      new_bank_esr_ctl[b].esr_sc_l3_set_mask = esr_sc_l3_set_mask_i;
      new_bank_esr_ctl[b].esr_sc_l3_tag_mask = esr_sc_l3_tag_mask_i;
    end
  end

  esr_shire_config_t esr_shire_config;
  always_comb begin
    esr_shire_config = '0;
    esr_shire_config.shire_id = esr_shire_id_i;
  end

  // ════════════════════════════════════════════════════════
  // DFT/RAM config
  // ════════════════════════════════════════════════════════
  dft_t new_dft;
  assign new_dft = '0;

  ram_cfg_t new_ram_cfg;
  assign new_ram_cfg = '0;

  // ════════════════════════════════════════════════════════
  // APB ESR for original (idle)
  // ════════════════════════════════════════════════════════
  APB_to_bank_ESRs_t [Banks-1:0] orig_apb_esr_req;
  always_comb begin
    for (int b = 0; b < Banks; b++) orig_apb_esr_req[b] = '0;
  end

  APB_from_bank_ESRs_t [Banks-1:0] orig_apb_esr_rsp;

  esr_shire_cache_ram_cfg_t orig_ram_cfg;
  assign orig_ram_cfg = '0;

  // ════════════════════════════════════════════════════════
  // AXI channel tie-offs (data channels we don't drive)
  // ════════════════════════════════════════════════════════

  // to_l3 R/B response data (tied to zero — no fills)
  sc_master_r_t [L3MasterPorts-1:0]  to_l3_axi_r_data;
  sc_master_b_t [L3MasterPorts-1:0]  to_l3_axi_b_data;
  sc_master_axi_R_channel_t [L3MasterPorts-1:0] orig_to_l3_axi_r_data;
  sc_master_axi_B_channel_t [L3MasterPorts-1:0] orig_to_l3_axi_b_data;

  always_comb begin
    for (int i = 0; i < L3MasterPorts; i++) begin
      to_l3_axi_r_data[i]      = '0;
      to_l3_axi_b_data[i]      = '0;
      orig_to_l3_axi_r_data[i] = '0;
      orig_to_l3_axi_b_data[i] = '0;
    end
  end

  // to_sys R/B response data (tied to zero)
  sc_master_r_t [SysPorts-1:0]  to_sys_axi_r_data;
  sc_master_b_t [SysPorts-1:0]  to_sys_axi_b_data;
  sc_master_axi_R_channel_t [SysPorts-1:0] orig_to_sys_axi_r_data;
  sc_master_axi_B_channel_t [SysPorts-1:0] orig_to_sys_axi_b_data;

  always_comb begin
    for (int i = 0; i < SysPorts; i++) begin
      to_sys_axi_r_data[i]      = '0;
      to_sys_axi_b_data[i]      = '0;
      orig_to_sys_axi_r_data[i] = '0;
      orig_to_sys_axi_b_data[i] = '0;
    end
  end

  // l3 slave AR/AW/W data (tied to zero — no l3 requests)
  sc_slave_ar_t [L3SlavePorts-1:0]  l3_axi_ar_data;
  sc_slave_aw_t [L3SlavePorts-1:0]  l3_axi_aw_data;
  sc_master_w_t [L3SlavePorts-1:0]  l3_axi_w_data;
  sc_slave_axi_AR_channel_t [L3SlavePorts-1:0] orig_l3_axi_ar_data;
  sc_slave_axi_AW_channel_t [L3SlavePorts-1:0] orig_l3_axi_aw_data;
  sc_slave_axi_W_channel_t  [L3SlavePorts-1:0] orig_l3_axi_w_data;

  always_comb begin
    for (int i = 0; i < L3SlavePorts; i++) begin
      l3_axi_ar_data[i]      = '0;
      l3_axi_aw_data[i]      = '0;
      l3_axi_w_data[i]       = '0;
      orig_l3_axi_ar_data[i] = '0;
      orig_l3_axi_aw_data[i] = '0;
      orig_l3_axi_w_data[i]  = '0;
    end
  end

  // ════════════════════════════════════════════════════════
  // BIST tie-offs
  // ════════════════════════════════════════════════════════
  icache_bist_rsp_t   new_icache_bist_rsp;
  assign new_icache_bist_rsp = '0;
  icache_bist_req_t   new_icache_bist_req;

  sc_icache_bist_rsp_t orig_icache_bist_rsp;
  assign orig_icache_bist_rsp = '0;
  sc_icache_bist_req_t orig_icache_bist_req;

  // ════════════════════════════════════════════════════════
  // Unused outputs from new
  // ════════════════════════════════════════════════════════
  bank_esr_status_t [Banks-1:0]   new_bank_esr_status;
  trace_packet_t                  new_sc_trace_data;
  logic [Banks-1:0]               new_l2hpf_valid;
  neigh_l2hpf_req_t [Banks-1:0]  new_l2hpf_info;
  xbar_req_t                      new_neigh_uc_req_info;

  // Unused outputs from new AXI channels
  sc_master_ar_t [L3MasterPorts-1:0] new_to_l3_axi_ar;
  sc_master_aw_t [L3MasterPorts-1:0] new_to_l3_axi_aw;
  sc_master_w_t  [L3MasterPorts-1:0] new_to_l3_axi_w;
  sc_master_ar_t [SysPorts-1:0]      new_to_sys_axi_ar;
  sc_master_aw_t [SysPorts-1:0]      new_to_sys_axi_aw;
  sc_master_w_t  [SysPorts-1:0]      new_to_sys_axi_w;
  sc_slave_r_t   [L3SlavePorts-1:0]  new_l3_axi_r;
  sc_slave_b_t   [L3SlavePorts-1:0]  new_l3_axi_b;

  // ════════════════════════════════════════════════════════
  // Unused outputs from original
  // ════════════════════════════════════════════════════════
  logic [Ports*(Banks+1)-1:0]              orig_neigh_sc_req_ready;
  et_link_rsp_info_t [Ports-1:0]           orig_neigh_sc_rsp_info;
  sc_xbar_req_t                            orig_neigh_uc_req_info;
  sc_trace_packet_t                        orig_sc_trace_data;
  logic [Banks-1:0]                        orig_l2hpf_valid;
  sc_neigh_l2hpf_req_t [Banks-1:0]        orig_l2hpf_info;
  debug_axi_port_t                         orig_debug_axi_port;

  sc_master_axi_AR_channel_t [L3MasterPorts-1:0] orig_to_l3_axi_ar;
  sc_master_axi_AW_channel_t [L3MasterPorts-1:0] orig_to_l3_axi_aw;
  sc_master_axi_W_channel_t  [L3MasterPorts-1:0] orig_to_l3_axi_w;
  sc_master_axi_AR_channel_t [SysPorts-1:0]      orig_to_sys_axi_ar;
  sc_master_axi_AW_channel_t [SysPorts-1:0]      orig_to_sys_axi_aw;
  sc_master_axi_W_channel_t  [SysPorts-1:0]      orig_to_sys_axi_w;
  sc_slave_axi_R_channel_t   [L3SlavePorts-1:0]  orig_l3_axi_r;
  sc_slave_axi_B_channel_t   [L3SlavePorts-1:0]  orig_l3_axi_b;

  // New module request ready — for comparison
  logic [Ports-1:0][Banks:0]               new_neigh_sc_req_ready;
  etlink_pkg::rsp_t [Ports-1:0]            new_neigh_sc_rsp_info;

  // ════════════════════════════════════════════════════════
  // NEW module instance
  // ════════════════════════════════════════════════════════
  shirecache_top u_new (
    .clk_i                            (clk_i),
    .rst_cold_ni                      (rst_ni),
    .rst_ni                           (rst_ni),
    .rst_debug_ni                     (rst_ni),
    .noc_clk_i                        (clk_i),   // same clock domain for cosim
    .noc_rst_ni                       (rst_ni),

    .dft_i                            (new_dft),
    .ram_cfg_i                        (new_ram_cfg),

    // Neighborhood request
    .neigh_sc_req_ready_o             (new_neigh_sc_req_ready),
    .neigh_sc_req_valid_i             (new_neigh_sc_req_valid),
    .neigh_sc_req_info_i              (new_neigh_sc_req_info),

    // Neighborhood response
    .neigh_sc_rsp_ready_i             (neigh_sc_rsp_ready_i),
    .neigh_sc_rsp_valid_o             (new_neigh_sc_rsp_valid_o),
    .neigh_sc_rsp_info_o              (new_neigh_sc_rsp_info),

    // UC
    .neigh_uc_req_ready_i             (neigh_uc_req_ready_i),
    .neigh_uc_req_valid_o             (new_neigh_uc_req_valid_o),
    .neigh_uc_req_info_o              (new_neigh_uc_req_info),
    .neigh_uc_rsp_ready_o             (new_neigh_uc_rsp_ready_o),
    .neigh_uc_rsp_valid_i             (neigh_uc_rsp_valid_i),
    .neigh_uc_rsp_info_i              (new_uc_rsp_info),

    // ESR
    .bank_esr_ctl_i                   (new_bank_esr_ctl),
    .bank_esr_status_o                (new_bank_esr_status),
    .esr_sc_remote_l3_enable_o        (new_esr_sc_remote_l3_enable_o),
    .esr_sc_remote_scp_enable_o       (new_esr_sc_remote_scp_enable_o),
    .esr_shire_config_i               (esr_shire_config),

    // Errors
    .sc_bank_err_detected_o           (new_sc_bank_err_detected_o),
    .sc_bank_err_logged_o             (new_sc_bank_err_logged_o),

    // L2HPF
    .sc_neigh_l2hpf_req_valid_o       (new_l2hpf_valid),
    .sc_neigh_l2hpf_req_info_o        (new_l2hpf_info),

    // Trace
    .status_monitor_bank_sel_i        (status_monitor_bank_sel_i),
    .sc_trace_data_o                  (new_sc_trace_data),
    .sc_trace_valid_o                 (new_sc_trace_valid_o),

    // to_l3 AXI
    .to_l3_axi_ar_ready_i             (to_l3_axi_ar_ready_i),
    .to_l3_axi_ar_valid_o             (new_to_l3_axi_ar_valid_o),
    .to_l3_axi_ar_o                   (new_to_l3_axi_ar),
    .to_l3_axi_aw_ready_i             (to_l3_axi_aw_ready_i),
    .to_l3_axi_aw_valid_o             (new_to_l3_axi_aw_valid_o),
    .to_l3_axi_aw_o                   (new_to_l3_axi_aw),
    .to_l3_axi_w_ready_i              (to_l3_axi_w_ready_i),
    .to_l3_axi_w_valid_o              (new_to_l3_axi_w_valid_o),
    .to_l3_axi_w_o                    (new_to_l3_axi_w),
    .to_l3_axi_r_ready_o              (new_to_l3_axi_r_ready_o),
    .to_l3_axi_r_valid_i              (to_l3_axi_r_valid_i),
    .to_l3_axi_r_i                    (to_l3_axi_r_data),
    .to_l3_axi_b_ready_o              (new_to_l3_axi_b_ready_o),
    .to_l3_axi_b_valid_i              (to_l3_axi_b_valid_i),
    .to_l3_axi_b_i                    (to_l3_axi_b_data),

    // to_sys AXI
    .to_sys_axi_ar_ready_i            (to_sys_axi_ar_ready_i),
    .to_sys_axi_ar_valid_o            (new_to_sys_axi_ar_valid_o),
    .to_sys_axi_ar_o                  (new_to_sys_axi_ar),
    .to_sys_axi_aw_ready_i            (to_sys_axi_aw_ready_i),
    .to_sys_axi_aw_valid_o            (new_to_sys_axi_aw_valid_o),
    .to_sys_axi_aw_o                  (new_to_sys_axi_aw),
    .to_sys_axi_w_ready_i             (to_sys_axi_w_ready_i),
    .to_sys_axi_w_valid_o             (new_to_sys_axi_w_valid_o),
    .to_sys_axi_w_o                   (new_to_sys_axi_w),
    .to_sys_axi_r_ready_o             (new_to_sys_axi_r_ready_o),
    .to_sys_axi_r_valid_i             (to_sys_axi_r_valid_i),
    .to_sys_axi_r_i                   (to_sys_axi_r_data),
    .to_sys_axi_b_ready_o             (new_to_sys_axi_b_ready_o),
    .to_sys_axi_b_valid_i             (to_sys_axi_b_valid_i),
    .to_sys_axi_b_i                   (to_sys_axi_b_data),

    // l3 slave AXI
    .l3_axi_ar_ready_o                (new_l3_axi_ar_ready_o),
    .l3_axi_ar_valid_i                (l3_axi_ar_valid_i),
    .l3_axi_ar_i                      (l3_axi_ar_data),
    .l3_axi_aw_ready_o                (new_l3_axi_aw_ready_o),
    .l3_axi_aw_valid_i                (l3_axi_aw_valid_i),
    .l3_axi_aw_i                      (l3_axi_aw_data),
    .l3_axi_w_ready_o                 (new_l3_axi_w_ready_o),
    .l3_axi_w_valid_i                 (l3_axi_w_valid_i),
    .l3_axi_w_i                       (l3_axi_w_data),
    .l3_axi_r_ready_i                 (l3_axi_r_ready_i),
    .l3_axi_r_valid_o                 (new_l3_axi_r_valid_o),
    .l3_axi_r_o                       (new_l3_axi_r),
    .l3_axi_b_ready_i                 (l3_axi_b_ready_i),
    .l3_axi_b_valid_o                 (new_l3_axi_b_valid_o),
    .l3_axi_b_o                       (new_l3_axi_b),

    // BIST
    .icache_bist_req_info_o           (new_icache_bist_req),
    .icache_bist_rsp_info_i           (new_icache_bist_rsp)
  );

  // ════════════════════════════════════════════════════════
  // ORIGINAL module instance
  // ════════════════════════════════════════════════════════

  debug_axi_port_ctl_t orig_debug_axi_port_ctl;
  assign orig_debug_axi_port_ctl = '0;

  shire_cache u_orig (
    .clock                            (clk_i),
    .reset_c_in                       (reset),
    .reset_w_in                       (reset),
    .reset_d_in                       (reset),
    .noc_clock                        (clk_i),
    .noc_reset                        (reset),

    // Neighborhood request
    .neigh_sc_req_ready               (orig_neigh_sc_req_ready),
    .neigh_sc_req_valid               (orig_neigh_sc_req_valid),
    .neigh_sc_req_info                (orig_neigh_sc_req_info),

    // Neighborhood response
    .neigh_sc_rsp_ready               (neigh_sc_rsp_ready_i),
    .neigh_sc_rsp_valid               (orig_neigh_sc_rsp_valid_o),
    .neigh_sc_rsp_info                (orig_neigh_sc_rsp_info),

    // UC
    .neigh_uc_req_ready               (neigh_uc_req_ready_i),
    .neigh_uc_req_valid               (orig_neigh_uc_req_valid_o),
    .neigh_uc_req_info                (orig_neigh_uc_req_info),
    .neigh_uc_rsp_ready               (orig_neigh_uc_rsp_ready_o),
    .neigh_uc_rsp_valid               (neigh_uc_rsp_valid_i),
    .neigh_uc_rsp_info                (orig_uc_rsp_info),

    // ESR
    .APB_ESR_req                      (orig_apb_esr_req),
    .APB_ESR_rsp                      (orig_apb_esr_rsp),
    .esr_sc_remote_l3_enable          (orig_esr_sc_remote_l3_enable_o),
    .esr_sc_remote_scp_enable         (orig_esr_sc_remote_scp_enable_o),
    .esr_shire_config                 (esr_shire_config),
    .esr_shire_cache_ram_cfg          (orig_ram_cfg),

    // Errors
    .sc_bank_err_detected             (orig_sc_bank_err_detected_o),
    .sc_bank_err_logged               (orig_sc_bank_err_logged_o),

    // L2HPF
    .sc_neigh_l2hpf_req_valid         (orig_l2hpf_valid),
    .sc_neigh_l2hpf_req_info          (orig_l2hpf_info),

    // Trace
    .status_monitor_gpio              ({{(`shire_cache_sm_gpio_width - BankIdSize){1'b0}}, status_monitor_bank_sel_i}),
    .sc_trace_data                    (orig_sc_trace_data),
    .sc_trace_valid                   (orig_sc_trace_valid_o),

    // to_l3 AXI
    .to_l3_mesh_master_axi_ARREADY    (to_l3_axi_ar_ready_i),
    .to_l3_mesh_master_axi_ARVALID    (orig_to_l3_axi_ar_valid_o),
    .to_l3_mesh_master_axi_AR         (orig_to_l3_axi_ar),
    .to_l3_mesh_master_axi_AWREADY    (to_l3_axi_aw_ready_i),
    .to_l3_mesh_master_axi_AWVALID    (orig_to_l3_axi_aw_valid_o),
    .to_l3_mesh_master_axi_AW         (orig_to_l3_axi_aw),
    .to_l3_mesh_master_axi_WREADY     (to_l3_axi_w_ready_i),
    .to_l3_mesh_master_axi_WVALID     (orig_to_l3_axi_w_valid_o),
    .to_l3_mesh_master_axi_W          (orig_to_l3_axi_w),
    .to_l3_mesh_master_axi_RREADY     (orig_to_l3_axi_r_ready_o),
    .to_l3_mesh_master_axi_RVALID     (to_l3_axi_r_valid_i),
    .to_l3_mesh_master_axi_R          (orig_to_l3_axi_r_data),
    .to_l3_mesh_master_axi_BREADY     (orig_to_l3_axi_b_ready_o),
    .to_l3_mesh_master_axi_BVALID     (to_l3_axi_b_valid_i),
    .to_l3_mesh_master_axi_B          (orig_to_l3_axi_b_data),

    // to_sys AXI
    .to_sys_mesh_master_axi_ARREADY   (to_sys_axi_ar_ready_i),
    .to_sys_mesh_master_axi_ARVALID   (orig_to_sys_axi_ar_valid_o),
    .to_sys_mesh_master_axi_AR        (orig_to_sys_axi_ar),
    .to_sys_mesh_master_axi_AWREADY   (to_sys_axi_aw_ready_i),
    .to_sys_mesh_master_axi_AWVALID   (orig_to_sys_axi_aw_valid_o),
    .to_sys_mesh_master_axi_AW        (orig_to_sys_axi_aw),
    .to_sys_mesh_master_axi_WREADY    (to_sys_axi_w_ready_i),
    .to_sys_mesh_master_axi_WVALID    (orig_to_sys_axi_w_valid_o),
    .to_sys_mesh_master_axi_W         (orig_to_sys_axi_w),
    .to_sys_mesh_master_axi_RREADY    (orig_to_sys_axi_r_ready_o),
    .to_sys_mesh_master_axi_RVALID    (to_sys_axi_r_valid_i),
    .to_sys_mesh_master_axi_R         (orig_to_sys_axi_r_data),
    .to_sys_mesh_master_axi_BREADY    (orig_to_sys_axi_b_ready_o),
    .to_sys_mesh_master_axi_BVALID    (to_sys_axi_b_valid_i),
    .to_sys_mesh_master_axi_B         (orig_to_sys_axi_b_data),

    // l3 slave AXI
    .l3_mesh_slave_axi_ARREADY        (orig_l3_axi_ar_ready_o),
    .l3_mesh_slave_axi_ARVALID        (l3_axi_ar_valid_i),
    .l3_mesh_slave_axi_AR             (orig_l3_axi_ar_data),
    .l3_mesh_slave_axi_AWREADY        (orig_l3_axi_aw_ready_o),
    .l3_mesh_slave_axi_AWVALID        (l3_axi_aw_valid_i),
    .l3_mesh_slave_axi_AW             (orig_l3_axi_aw_data),
    .l3_mesh_slave_axi_WREADY         (orig_l3_axi_w_ready_o),
    .l3_mesh_slave_axi_WVALID         (l3_axi_w_valid_i),
    .l3_mesh_slave_axi_W              (orig_l3_axi_w_data),
    .l3_mesh_slave_axi_RREADY         (l3_axi_r_ready_i),
    .l3_mesh_slave_axi_RVALID         (orig_l3_axi_r_valid_o),
    .l3_mesh_slave_axi_R              (orig_l3_axi_r),
    .l3_mesh_slave_axi_BREADY         (l3_axi_b_ready_i),
    .l3_mesh_slave_axi_BVALID         (orig_l3_axi_b_valid_o),
    .l3_mesh_slave_axi_B              (orig_l3_axi_b),

    // Debug AXI port
    .debug_axi_port_ctl               (orig_debug_axi_port_ctl),
    .shire_cache_debug_axi_port       (orig_debug_axi_port),

    // DFT
    .dft__reset_byp_hv                (1'b0),
    .dft__reset_hv                    (1'b0),
    .dft__reset_byp_lv                (1'b0),
    .dft__reset_lv                    (1'b0),
    .dft__sram_clock                  (1'b0),
    .dft__clk_override                (1'b0),
    .dft__mbist_en                    (1'b0),

    // BIST
    .icache_bist_req_info             (orig_icache_bist_req),
    .icache_bist_rsp_info             (orig_icache_bist_rsp)
  );

endmodule
/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on UNOPTFLAT */
/* verilator lint_on UNUSEDSIGNAL */
