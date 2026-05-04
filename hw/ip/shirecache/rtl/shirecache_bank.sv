// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire cache bank — per-bank container module.
//
// Wires together all cache bank sub-modules:
//   - shirecache_pipe           (cache pipeline)
//   - shirecache_bank_reqq      (request queue)
//   - shirecache_bank_dataq     (data queue)
//   - shirecache_bank_rspmux    (response mux)
//   - shirecache_bank_mesh      (mesh interface, x2: to_l3 and to_sys)
//   - shirecache_bank_l3_slave  (L3 slave)
//   - shirecache_bank_perfmon   (performance monitor)
//   - shirecache_bank_err_logger(error logger)
//   - shirecache_bank_l2hpf     (L2 hardware prefetcher)
//   - shirecache_bank_trace     (trace)
//   - shirecache_bist_wrapper   (BIST)
//
// This module also contains the bank-level request FIFO (depth 2) for
// incoming L2 requests and the bank-level clock gating logic.
//
// The ESR block (esr_cache_bank) is NOT instantiated here — it will be
// added once the ESR generator is ported. Instead, esr_ctl and
// esr_shire_config are accepted as inputs, and esr_status is provided
// as an output port.
//
// Replaces: shire_cache_bank (etcore-soc)
//
// Differences from original:
//   - Active-low async-assert/sync-deassert reset (rst_ni / rst_c_ni / rst_d_ni)
//   - lowRISC naming conventions (_i/_o)
//   - DFT consolidated into dft_pkg::dft_t struct
//   - RAM config via ram_cfg_pkg::ram_cfg_t
//   - Package types instead of `define macros
//   - prim_rst_sync instead of rst_repeat
//   - prim_clk_gate instead of et_clk_gate
//   - prim_fifo_reg instead of gen_fifo_reg
//   - ESR block externalized as ports (pending ESR generator)
//   - EVL_SIMULATION trans_id masking removed (always pass-through)
//   - prim_eco_ports instead of et_eco_ports

/* verilator lint_off UNUSEDSIGNAL */  // packed struct ports passed whole; sub-modules use subfields
/* verilator lint_off UNOPTFLAT */     // struct fields written by independent combinational blocks
module shirecache_bank
  import shirecache_pkg::*;
  import etlink_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;
#(
  parameter int unsigned NumPorts         = Ports,
  parameter int unsigned NumEntries       = ReqqDepth,
  parameter int unsigned SetsPerSubBankP  = SetsPerSubBank,
  parameter bit          L2HpfImplemented = 0
) (
  input  logic                        clk_i,       // functional clock
  input  logic                        clk_free_i,  // free-running clock (perfmon, trace)
  input  logic                        rst_ni,      // warm reset (active-low)
  input  logic                        rst_c_ni,    // cold reset (active-low)
  input  logic                        rst_d_ni,    // debug reset (active-low)

  // DFT
  input  dft_t                        dft_i,
  input  logic                        dft_sram_clk_i,
  input  logic                        dft_mbist_en_i,

  // ECO ports
  input  logic [EtEcoShirecacheBankInputWidth-1:0]  eco_i,
  output logic [EtEcoShirecacheBankOutputWidth-1:0] eco_o,

  // RAM configuration
  input  ram_cfg_t                    ram_cfg_i,

  // BIST implemented constant
  input  logic                        mbi_implemented_const_i,

  // ── Requests to bank ────────────────────────────────────
  output logic                        bank_req_ready_o,
  input  logic                        bank_req_valid_i,
  input  xbar_req_t                   bank_req_info_i,

  // ── Responses to xbar ───────────────────────────────────
  output logic [NumPorts-1:0]         bank_rsp_valid_o,
  output xbar_rsp_t                   bank_rsp_info_o,
  input  logic [NumPorts-1:0]         bank_rsp_ready_i,
  input  logic [NumPorts-1:0]         bank_rsp_2_credits_i,

  // ── To-L3 mesh master interface ─────────────────────────
  output logic                        to_l3_mesh_master_bank_clk_en_o,
  input  logic                        to_l3_mesh_master_bank_req_ready_i,
  output logic                        to_l3_mesh_master_bank_req_valid_o,
  output mesh_master_req_t            to_l3_mesh_master_bank_req_info_o,

  output logic                        to_l3_mesh_master_bank_rsp_ready_o,
  input  logic                        to_l3_mesh_master_bank_rsp_valid_i,
  input  mesh_master_rsp_t            to_l3_mesh_master_bank_rsp_info_i,

  // ── To-sys mesh master interface ────────────────────────
  output logic                        to_sys_mesh_master_bank_clk_en_o,
  input  logic                        to_sys_mesh_master_bank_req_ready_i,
  output logic                        to_sys_mesh_master_bank_req_valid_o,
  output mesh_master_req_t            to_sys_mesh_master_bank_req_info_o,

  output logic                        to_sys_mesh_master_bank_rsp_ready_o,
  input  logic                        to_sys_mesh_master_bank_rsp_valid_i,
  input  mesh_master_rsp_t            to_sys_mesh_master_bank_rsp_info_i,

  // ── L3 mesh slave interface ─────────────────────────────
  input  logic                        l3_mesh_slave_bank_clk_en_i,
  output logic                        l3_mesh_slave_bank_req_ready_o,
  input  logic                        l3_mesh_slave_bank_req_valid_i,
  input  mesh_slave_req_t             l3_mesh_slave_bank_req_info_i,

  input  logic                        l3_mesh_slave_bank_rsp_ready_i,
  output logic                        l3_mesh_slave_bank_rsp_valid_o,
  output mesh_slave_rsp_t             l3_mesh_slave_bank_rsp_info_o,

  // ── BIST to icache ──────────────────────────────────────
  output icache_bist_req_t            icache_bist_req_info_o,
  input  icache_bist_rsp_t            icache_bist_rsp_info_i,

  // ── MBIST shared interface (pre-insertion) ──────────────
  input  logic                        mbist_en_i,
  input  logic [MbistSelSize-1:0]     mbist_sel_i,
  input  logic                        mbist_rd_en_i,
  input  logic                        mbist_wr_en_i,
  input  logic [MbistAddrSize-1:0]    mbist_addr_i,
  input  logic [MbistDataSize-1:0]    mbist_wdata_i,
  output logic [MbistDataSize-1:0]    mbist_rdata_o,

  // ── ESR interface (externalized — ESR block not yet available) ──
  input  bank_esr_ctl_t               esr_ctl_i,
  output bank_esr_status_t            esr_status_o,
  input  esr_shire_config_t           esr_shire_config_i,

  // ── Error outputs ───────────────────────────────────────
  output logic                        err_detected_o,
  output logic                        err_logged_o,

  // ── Constants ───────────────────────────────────────────
  input  logic [BankIdSize-1:0]       my_bank_id_i,

  // ── Trace ───────────────────────────────────────────────
  output trace_packet_t               bank_trace_data_o,
  output logic                        bank_trace_valid_o,

  // ── L2 HPF output ───────────────────────────────────────
  output logic                        l2hpf_req_valid_o,
  output neigh_l2hpf_req_t            l2hpf_req_info_o,

  // ── ESR forwarded outputs ───────────────────────────────
  output logic                        esr_sc_remote_l3_enable_o,
  output logic                        esr_sc_remote_scp_enable_o,
  output l3_swizzle_ctl_t             esr_sc_l3_shire_swizzle_ctl_o
);

  localparam int unsigned NumEntriesId = ReqqIdSize;

  // ════════════════════════════════════════════════════════════
  // Reset synchronizers
  // ════════════════════════════════════════════════════════════

  logic rst_n;    // warm reset (synchronized)
  logic rst_c_n;  // cold reset (synchronized)
  logic rst_d_n;  // debug reset (synchronized)

  prim_rst_sync u_rst_sync_w (
    .clk_i  (clk_i),
    .rst_ni (rst_ni),
    .dft_i  (dft_i),
    .rst_no (rst_n)
  );

  prim_rst_sync u_rst_sync_c (
    .clk_i  (clk_i),
    .rst_ni (rst_c_ni),
    .dft_i  (dft_i),
    .rst_no (rst_c_n)
  );

  prim_rst_sync u_rst_sync_d (
    .clk_i  (clk_i),
    .rst_ni (rst_d_ni),
    .dft_i  (dft_i),
    .rst_no (rst_d_n)
  );

  prim_eco_ports #(
    .InputWidth (EtEcoShirecacheBankInputWidth),
    .OutputWidth(EtEcoShirecacheBankOutputWidth)
  ) u_eco_ports (
    .eco_i(eco_i),
    .eco_o(eco_o)
  );

  // ════════════════════════════════════════════════════════════
  // ESR forwarding
  // ════════════════════════════════════════════════════════════

  assign esr_sc_remote_l3_enable_o  = esr_ctl_i.esr_sc_remote_l3_enable;
  assign esr_sc_remote_scp_enable_o = esr_ctl_i.esr_sc_remote_scp_enable;

  // Build L3 swizzle control from ESR fields
  l3_swizzle_ctl_t esr_sc_l3_shire_swizzle_ctl;
  always_comb begin
    esr_sc_l3_shire_swizzle_ctl.all_shire_aliasing               = esr_ctl_i.esr_sc_all_shire_aliasing;
    esr_sc_l3_shire_swizzle_ctl.two_shire_aliasing_use_shire_lsb = esr_ctl_i.esr_sc_two_shire_aliasing_use_shire_lsb;
    esr_sc_l3_shire_swizzle_ctl.sub_bank_sel_b2                  = esr_ctl_i.esr_sc_sub_bank_sel_b2;
    esr_sc_l3_shire_swizzle_ctl.sub_bank_sel_b1                  = esr_ctl_i.esr_sc_sub_bank_sel_b1;
    esr_sc_l3_shire_swizzle_ctl.sub_bank_sel_b0                  = esr_ctl_i.esr_sc_sub_bank_sel_b0;
    esr_sc_l3_shire_swizzle_ctl.bank_sel_b2                      = esr_ctl_i.esr_sc_bank_sel_b2;
    esr_sc_l3_shire_swizzle_ctl.bank_sel_b1                      = esr_ctl_i.esr_sc_bank_sel_b1;
    esr_sc_l3_shire_swizzle_ctl.bank_sel_b0                      = esr_ctl_i.esr_sc_bank_sel_b0;
    esr_sc_l3_shire_swizzle_ctl.shire_sel_b5                     = esr_ctl_i.esr_sc_shire_sel_b5;
    esr_sc_l3_shire_swizzle_ctl.shire_sel_b4                     = esr_ctl_i.esr_sc_shire_sel_b4;
    esr_sc_l3_shire_swizzle_ctl.shire_sel_b3                     = esr_ctl_i.esr_sc_shire_sel_b3;
    esr_sc_l3_shire_swizzle_ctl.shire_sel_b2                     = esr_ctl_i.esr_sc_shire_sel_b2;
    esr_sc_l3_shire_swizzle_ctl.shire_sel_b1                     = esr_ctl_i.esr_sc_shire_sel_b1;
    esr_sc_l3_shire_swizzle_ctl.shire_sel_b0                     = esr_ctl_i.esr_sc_shire_sel_b0;
  end
  assign esr_sc_l3_shire_swizzle_ctl_o = esr_sc_l3_shire_swizzle_ctl;

  // ════════════════════════════════════════════════════════════
  // Virtual shire IDs
  // ════════════════════════════════════════════════════════════

  wire [L3ShireIdSize-1:0]  my_l3_virtual_shire_id  = esr_shire_config_i.shire_id[L3ShireIdSize-1:0];
  wire [ScpShireIdSize-1:0] my_scp_virtual_shire_id = esr_shire_config_i.shire_id;

  // ════════════════════════════════════════════════════════════
  // Local wires
  // ════════════════════════════════════════════════════════════

  logic                         bank_l2_ready;
  logic                         bank_l2_valid;
  reqq_bank_req_t               bank_l2_info;

  logic                         bank_l3_ready;
  logic                         bank_l3_valid;
  reqq_bank_req_t               bank_l3_info;

  // reqq <-> dataq
  logic [NumEntriesId-1:0]      reqq_alloc_l2_id_ag;
  logic                         dataq_bank_l2_ready;
  logic [NumEntriesId-1:0]      reqq_alloc_l3_id_ag;
  logic                         dataq_bank_l3_ready;

  logic [NumEntries-1:0]        dataq_write_status_hot;
  logic                         dataq_write_status_any;
  logic [NumEntriesId-1:0]      dataq_write_status_id;
  logic [NumEntries-1:0]        dataq_read_status_hot;

  // reqq <-> err_log
  logic                         alloc_l2_valid_ag;
  reqq_alloc_t                  alloc_l2_info_ag;
  logic                         alloc_l2_valid_ad;
  reqq_alloc_t                  alloc_l2_info_ad;
  logic                         alloc_l3_valid_ag;
  reqq_alloc_t                  alloc_l3_info_ag;
  logic                         alloc_l3_valid_ad;
  reqq_alloc_t                  alloc_l3_info_ad;
  logic [NumEntries-1:0]        set_reqq_err_detected_hot;
  logic [NumEntries-1:0]        set_reqq_err_zero_data_hot;

  // err_log to trace
  reqq_entry_state_t            traced_reqq_state;

  // Interfaces with mesh and dataq
  data_rsp_modifier_t           pipe_data_rsp_modifier_info;

  // Interfaces with the pipeline
  logic                         pipe_start;
  reqq_pipe_req_t               pipe_req_info;
  logic                         pipe_active;
  pipe_busy_t                   pipe_busy;
  logic                         pipe_squash;
  logic                         pipe_tag_rsp_valid;
  tag_rsp_t                     pipe_tag_rsp_info;
  logic                         pipe_data_rsp_pre_valid;
  data_rsp_pre_t                pipe_data_rsp_pre_info;
  logic                         pipe_data_rsp_valid;
  data_rsp_t                    pipe_data_rsp_info;
  logic                         pipe_cbuf_rsp_valid;
  cbuf_rsp_t                    pipe_cbuf_rsp_info;
  rbuf_clear_req_t              pipe_rbuf_clear_req_info;
  logic                         pipe_rbuf_rd_req_ready;
  logic                         pipe_rbuf_rd_req_valid;
  rbuf_rd_req_t                 pipe_rbuf_rd_req_info;
  logic                         pipe_rbuf_rd_rsp_ready;
  logic                         pipe_rbuf_rd_rsp_ready_rspmux_l3;
  logic                         pipe_rbuf_rd_rsp_ready_rspmux_l2;
  logic                         pipe_rbuf_rd_rsp_valid;
  rbuf_rd_rsp_t                 pipe_rbuf_rd_rsp_info;
  rbuf_state_t                  pipe_rbuf_state;
  logic                         pipe_dataq_rd_req_valid;
  dataq_rd_req_t                pipe_dataq_rd_req_info;
  dataq_rd_rsp_t                pipe_dataq_rd_rsp_info;
  logic                         pipe_idx_cop_aborted;
  logic                         pipe_idx_cop_l2_busy;
  logic                         pipe_idx_cop_l3_busy;
  logic                         pipe_idx_cop_req_ready;
  logic                         pipe_idx_cop_req_valid;
  idx_cop_req_t                 pipe_idx_cop_req_info;
  logic                         pipe_idx_cop_is_active;

  // mesh bypass signals
  logic                         to_l3_mesh_bypassed_victim;
  logic                         to_sys_mesh_bypassed_victim;
  wire                          mesh_bypassed_victim = to_l3_mesh_bypassed_victim | to_sys_mesh_bypassed_victim;

  logic                         rspmux_l2_bypassed_data_rsp;
  logic                         rspmux_l3_bypassed_data_rsp;
  wire                          rspmux_bypassed_data_rsp = rspmux_l2_bypassed_data_rsp | rspmux_l3_bypassed_data_rsp;

  // Interfaces with to_l3 mesh
  logic                         reqq_to_l3_mesh_req_valid_nodata;
  logic                         reqq_to_l3_mesh_req_ready_nodata;
  logic                         reqq_to_l3_mesh_req_valid_wdata;
  logic                         reqq_to_l3_mesh_req_ready_wdata;
  reqq_mesh_req_t               reqq_to_l3_mesh_req_info;
  logic                         dataq_to_l3_mesh_req_valid;
  reqq_mesh_req_t               dataq_to_l3_mesh_req_info;
  logic                         dataq_to_l3_mesh_req_ready;
  logic                         to_l3_mesh_rsp_valid_reqq;
  wire                          to_l3_mesh_rsp_ready_reqq = 1'b1;
  logic                         to_l3_mesh_rsp_valid_dataq;
  logic                         to_l3_mesh_rsp_ready_dataq;
  logic                         to_l3_mesh_rsp_valid_rspmux_l2;
  logic                         to_l3_mesh_rsp_ready_rspmux_l2;
  logic                         to_l3_mesh_rsp_valid_l3_slave_unused;
  reqq_mesh_rsp_t               to_l3_mesh_rsp_info;

  // Interfaces with to_sys mesh
  logic                         reqq_to_sys_mesh_req_valid_nodata;
  logic                         reqq_to_sys_mesh_req_ready_nodata;
  logic                         reqq_to_sys_mesh_req_valid_wdata;
  logic                         reqq_to_sys_mesh_req_ready_wdata;
  reqq_mesh_req_t               reqq_to_sys_mesh_req_info;
  logic                         dataq_to_sys_mesh_req_valid;
  reqq_mesh_req_t               dataq_to_sys_mesh_req_info;
  logic                         dataq_to_sys_mesh_req_ready;
  logic                         to_sys_mesh_rsp_valid_reqq;
  wire                          to_sys_mesh_rsp_ready_reqq = 1'b1;
  logic                         to_sys_mesh_rsp_valid_dataq;
  logic                         to_sys_mesh_rsp_ready_dataq;
  logic                         to_sys_mesh_rsp_valid_rspmux_l2;
  logic                         to_sys_mesh_rsp_ready_rspmux_l2;
  logic                         to_sys_mesh_rsp_valid_l3_slave;
  logic                         to_sys_mesh_rsp_ready_l3_slave;
  reqq_mesh_rsp_t               to_sys_mesh_rsp_info;

  // L2 responses back to neighborhoods
  logic                         reqq_rspmux_l2_valid_nodata;
  logic                         reqq_rspmux_l2_ready_nodata;
  logic                         reqq_rspmux_l2_valid_wdata;
  logic                         reqq_rspmux_l2_ready_wdata;
  rspmux_t                      reqq_rspmux_l2_info;
  logic                         dataq_rspmux_l2_valid;
  rspmux_t                      dataq_rspmux_l2_info;
  logic                         dataq_rspmux_l2_ready;
  logic [NumEntries-1:0]        rspmux_l2_sent_valid_hot;
  rspmux_sent_t                 rspmux_l2_trace_sent;
  logic                         a_rspmux_l2_rsp_is_valid;

  // L3 responses back to L3 slave
  logic                         reqq_rspmux_l3_valid_nodata;
  logic                         reqq_rspmux_l3_ready_nodata;
  logic                         reqq_rspmux_l3_valid_wdata;
  logic                         reqq_rspmux_l3_ready_wdata;
  rspmux_t                      reqq_rspmux_l3_info;
  logic                         dataq_rspmux_l3_valid;
  rspmux_t                      dataq_rspmux_l3_info;
  logic                         dataq_rspmux_l3_ready;
  logic                         rspmux_l3_sent_valid;
  logic [NumEntriesId-1:0]      rspmux_l3_sent_id;
  logic [NumEntries-1:0]        rspmux_l3_sent_valid_hot;
  logic                         a_rspmux_l3_rsp_is_valid;

  // Performance monitor
  perfmon_req_info_t            perfmon_p0_req_info;
  perfmon_rsp_info_t            perfmon_p0_rsp_info;
  perfmon_req_info_t            perfmon_p1_req_info;
  perfmon_rsp_info_t            perfmon_p1_rsp_info;

  perfmon_rsp_info_t            reqq_perfmon_p0_rsp_info;
  perfmon_rsp_info_t            reqq_perfmon_p1_rsp_info;
  perfmon_rsp_info_t            pipe_perfmon_p1_rsp_info;

  // reqq state
  reqq_entry_state_t [NumEntries-1:0] reqq_state;
  logic                         a_reqq_entry_is_valid;

  // MBIST
  logic                         mbist_on;
  bist_req_t                    pipe_bist_req_info;
  bist_rsp_t                    pipe_bist_rsp_info;

  // ESR status assembly
  pipe_esr_status_t             pipe_esr_status;
  sbe_dbe_overflow_t            pipe_sbe_dbe_overflow;
  perfmon_cntr_overflow_t       perfmon_cntr_overflow;
  err_log_esr_status_t          err_log_esr_status;
  perfmon_esr_status_t          perfmon_esr_status;

  // Combine SBE/DBE fields from pipe/dataq into the same status register
  sbe_dbe_esr_status_t sbe_dbe_counts;
  always_comb begin
    sbe_dbe_counts.tag_state_ram_sbe_count = pipe_esr_status.pipe_sbe_dbe_esr_status.tag_state_ram_sbe_count;
    sbe_dbe_counts.tag_state_ram_dbe_count = pipe_esr_status.pipe_sbe_dbe_esr_status.tag_state_ram_dbe_count;
    sbe_dbe_counts.tag_ram_sbe_count       = pipe_esr_status.pipe_sbe_dbe_esr_status.tag_ram_sbe_count;
    sbe_dbe_counts.tag_ram_dbe_count       = pipe_esr_status.pipe_sbe_dbe_esr_status.tag_ram_dbe_count;
    sbe_dbe_counts.data_ram_sbe_count      = pipe_esr_status.pipe_sbe_dbe_esr_status.data_ram_sbe_count;
    sbe_dbe_counts.data_ram_dbe_count      = pipe_esr_status.pipe_sbe_dbe_esr_status.data_ram_dbe_count;
    // dataq and ben RAM counts: driven by dataq module (stubbed to 0 until wired)
    sbe_dbe_counts.dataq_ram_sbe_count     = '0;
    sbe_dbe_counts.dataq_ram_dbe_count     = '0;
    sbe_dbe_counts.ben_ram_sbe_count       = '0;
    sbe_dbe_counts.ben_ram_dbe_count       = '0;
  end

  assign esr_status_o.pipe_idx_cop_sm = pipe_esr_status.pipe_idx_cop_sm_esr_status;
  assign esr_status_o.err_log         = err_log_esr_status;
  /* verilator lint_off WIDTHEXPAND */
  assign esr_status_o.sbe_dbe_counts  = sbe_dbe_counts;
  /* verilator lint_on WIDTHEXPAND */
  assign esr_status_o.perfmon         = perfmon_esr_status;

  // ════════════════════════════════════════════════════════════
  // Bank clock gating
  // ════════════════════════════════════════════════════════════

  // Separate reset synchronizer for clock gate (warm reset, ungated clock)
  logic rst_w_clock_gate_n;
  prim_rst_sync u_rst_sync_w_cg (
    .clk_i  (clk_i),
    .rst_ni (rst_ni),
    .dft_i  (dft_i),
    .rst_no (rst_w_clock_gate_n)
  );

  logic clock_en_pre_d;
  assign clock_en_pre_d =
      ~rst_w_clock_gate_n
    | esr_ctl_i.esr_sc_clk_gate_disable[CgateDisBank]
    | esr_ctl_i.access_in_flight
    | mbist_on
    | bank_req_valid_i
    | bank_l2_valid
    | l3_mesh_slave_bank_clk_en_i
    | l3_mesh_slave_bank_req_valid_i
    | bank_l3_valid
    | a_reqq_entry_is_valid
    | a_rspmux_l2_rsp_is_valid
    | a_rspmux_l3_rsp_is_valid
    | pipe_active
    | pipe_idx_cop_is_active
    | pipe_idx_cop_req_valid
    ;

  logic clock_en_pre_q;
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) clock_en_pre_q <= 1'b0;
    else         clock_en_pre_q <= clock_en_pre_d;
  end

  wire clock_en = ~rst_w_clock_gate_n | clock_en_pre_q;

  logic clk_g;
  prim_clk_gate u_bank_clk_gate (
    .clk_i (clk_i),
    .en_i  (clock_en),
    .dft_i (dft_i),
    .clk_o (clk_g)
  );

  // ════════════════════════════════════════════════════════════
  // Bank request FIFO (depth 2) + L2 request mapping
  // ════════════════════════════════════════════════════════════

  logic          bank_req_fifo_full;
  logic [2:0]    bank_req_fifo_level_unused;
  xbar_req_t     bank_req_fifo_info;

  prim_fifo_reg #(
    .Width ($bits(xbar_req_t)),
    .Depth (2)
  ) u_bank_req_fifo (
    .clk_i  (clk_i),
    .rst_ni (rst_n),
    .push_i (bank_req_valid_i),
    .wdata_i(bank_req_info_i),
    .full_o (bank_req_fifo_full),
    .level_o(bank_req_fifo_level_unused),
    .pop_i  (bank_l2_ready),
    .valid_o(bank_l2_valid),
    .rdata_o(bank_req_fifo_info)
  );
  assign bank_req_ready_o = ~bank_req_fifo_full;

  wire bank_l2_address_is_scp =
    (bank_req_fifo_info.req_info.address[ScpRegionLsb +: ScpRegionIdSize] == ScpRegionId);

  wire [LineOffsetIdSize-1:0] bank_l2_address_offset =
    (bank_l2_info.opcode == ReqMsgSendData) ? {LineOffsetIdSize{1'b0}}
                                            : bank_l2_info.address[LineOffsetIdSize-1:0];

  always_comb begin
    bank_l2_info.trans_id        = bank_req_fifo_info.trans_id;
    bank_l2_info.port_id         = bank_req_fifo_info.port_id;
    bank_l2_info.id              = bank_req_fifo_info.req_info.id[ReqqTagIdSize-1:0];
    bank_l2_info.source          = bank_req_fifo_info.req_info.source[ReqqSourceSize-1:0];
    bank_l2_info.qos             = (esr_ctl_i.esr_sc_axi_qos == AxiQosMemHighPriority)
                                     ? {{(AxiAxqosSize-1){1'b0}}, 1'b1} : {AxiAxqosSize{1'b0}};
    bank_l2_info.opcode          = bank_req_fifo_info.req_info.opcode;
    bank_l2_info.subopcode       = bank_req_fifo_info.req_info.subopcode;
    bank_l2_info.address         = bank_req_fifo_info.req_info.address;
    bank_l2_info.wdata           = bank_req_fifo_info.req_info.wdata;
    bank_l2_info.data            = bank_req_fifo_info.req_info.data;
    bank_l2_info.qwen            =
      (bank_req_fifo_info.req_info.opcode == ReqAtomic)
        ? (4'b0100 | bank_req_fifo_info.req_info.qwen)
        : bank_req_fifo_info.req_info.qwen;
    bank_l2_info.size            = sc_size_t'(bank_req_fifo_info.req_info.size);
    bank_l2_info.ben             =
      (bank_l2_info.opcode == ReqAtomic)  ? HlBen :
      (bank_l2_info.opcode == ReqScpFill) ? ScpfillBen :
      gen_ben(bank_l2_info.size, bank_l2_address_offset);

    // An L2 write is partial if size is not a line
    // An SCP write is partial if size is less than qword
    bank_l2_info.write_is_partial =
      bank_l2_address_is_scp ? (bank_l2_info.size <= SizeDWord)
                             : (bank_l2_info.size != SizeLine);
  end

  // Rspmux interface with rbuf
  assign pipe_rbuf_rd_rsp_ready = pipe_rbuf_rd_rsp_ready_rspmux_l2 | pipe_rbuf_rd_rsp_ready_rspmux_l3;

  // ════════════════════════════════════════════════════════════
  // Performance monitor response merging
  // ════════════════════════════════════════════════════════════

  // Event
  assign perfmon_p0_rsp_info.ev_rsp =
    reqq_perfmon_p0_rsp_info.ev_rsp |
    pipe_perfmon_p1_rsp_info.ev_rsp;
  assign perfmon_p1_rsp_info.ev_rsp =
    reqq_perfmon_p0_rsp_info.ev_rsp |
    pipe_perfmon_p1_rsp_info.ev_rsp;

  // Resource
  assign perfmon_p0_rsp_info.rsrc_rsp =
    reqq_perfmon_p0_rsp_info.rsrc_rsp;
  assign perfmon_p1_rsp_info.rsrc_rsp =
    reqq_perfmon_p1_rsp_info.rsrc_rsp |
    pipe_perfmon_p1_rsp_info.rsrc_rsp;

  // ════════════════════════════════════════════════════════════
  // reqq
  // ════════════════════════════════════════════════════════════

  shirecache_bank_reqq #(
    .NumEntries   (NumEntries),
    .NumPorts     (NumPorts)
  ) u_reqq (
    .clk_i                              (clk_g),
    .rst_ni                             (rst_n),
    .dft_i                              (dft_i),

    // L2 request
    .bank_l2_ready_o                    (bank_l2_ready),
    .bank_l2_valid_i                    (bank_l2_valid),
    .bank_l2_info_i                     (bank_l2_info),
    .reqq_alloc_l2_id_ag_o              (reqq_alloc_l2_id_ag),
    .dataq_bank_l2_ready_i              (dataq_bank_l2_ready),

    // L3 request
    .bank_l3_ready_o                    (bank_l3_ready),
    .bank_l3_valid_i                    (bank_l3_valid),
    .bank_l3_info_i                     (bank_l3_info),
    .reqq_alloc_l3_id_ag_o              (reqq_alloc_l3_id_ag),
    .dataq_bank_l3_ready_i              (dataq_bank_l3_ready),

    // Alloc info to error logger
    .alloc_l2_valid_ag_o                (alloc_l2_valid_ag),
    .alloc_l2_info_ag_o                 (alloc_l2_info_ag),
    .alloc_l3_valid_ag_o                (alloc_l3_valid_ag),
    .alloc_l3_info_ag_o                 (alloc_l3_info_ag),
    .alloc_l2_valid_ad_o                (alloc_l2_valid_ad),
    .alloc_l2_info_ad_o                 (alloc_l2_info_ad),
    .alloc_l3_valid_ad_o                (alloc_l3_valid_ad),
    .alloc_l3_info_ad_o                 (alloc_l3_info_ad),

    // Error logger detected error
    .set_reqq_err_detected_hot_i        (set_reqq_err_detected_hot),
    .set_reqq_err_zero_data_hot_i       (set_reqq_err_zero_data_hot),

    // Idx CacheOp FSM
    .pipe_idx_cop_aborted_i             (pipe_idx_cop_aborted),
    .pipe_idx_cop_l2_busy_i             (pipe_idx_cop_l2_busy),
    .pipe_idx_cop_l3_busy_i             (pipe_idx_cop_l3_busy),
    .pipe_idx_cop_req_ready_o           (pipe_idx_cop_req_ready),
    .pipe_idx_cop_req_valid_i           (pipe_idx_cop_req_valid),
    .pipe_idx_cop_req_info_i            (pipe_idx_cop_req_info),

    // Dataq write status
    .dataq_write_status_any_i           (dataq_write_status_any),
    .dataq_write_status_hot_i           (dataq_write_status_hot),
    .dataq_write_status_id_i            (dataq_write_status_id),
    .dataq_read_status_hot_i            (dataq_read_status_hot),

    // Pipeline interfaces
    .pipe_start_o                       (pipe_start),
    .pipe_req_info_o                    (pipe_req_info),
    .pipe_busy_i                        (pipe_busy),
    .pipe_squash_i                      (pipe_squash),
    .pipe_tag_rsp_valid_i               (pipe_tag_rsp_valid),
    .pipe_tag_rsp_info_i                (pipe_tag_rsp_info),
    .pipe_data_rsp_valid_i              (pipe_data_rsp_valid),
    .pipe_data_rsp_info_i               (pipe_data_rsp_info),
    .pipe_data_rsp_pre_valid_i          (pipe_data_rsp_pre_valid),
    .pipe_data_rsp_pre_info_i           (pipe_data_rsp_pre_info),
    .pipe_cbuf_rsp_valid_i              (pipe_cbuf_rsp_valid),
    .pipe_cbuf_rsp_info_i               (pipe_cbuf_rsp_info),
    .mesh_bypassed_victim_i             (mesh_bypassed_victim),
    .rspmux_bypassed_data_rsp_i         (rspmux_bypassed_data_rsp),

    // Data response modifier
    .pipe_data_rsp_modifier_info_o      (pipe_data_rsp_modifier_info),

    // Read buffer
    .pipe_rbuf_clear_req_info_o         (pipe_rbuf_clear_req_info),
    .pipe_rbuf_rd_req_valid_o           (pipe_rbuf_rd_req_valid),
    .pipe_rbuf_rd_req_info_o            (pipe_rbuf_rd_req_info),
    .pipe_rbuf_rd_req_ready_i           (pipe_rbuf_rd_req_ready),
    .pipe_rbuf_state_i                  (pipe_rbuf_state),

    // To-L3 mesh
    .to_l3_mesh_master_bank_clk_en_o    (to_l3_mesh_master_bank_clk_en_o),
    .reqq_to_l3_mesh_req_valid_nodata_o (reqq_to_l3_mesh_req_valid_nodata),
    .reqq_to_l3_mesh_req_ready_nodata_i (reqq_to_l3_mesh_req_ready_nodata),
    .reqq_to_l3_mesh_req_valid_wdata_o  (reqq_to_l3_mesh_req_valid_wdata),
    .reqq_to_l3_mesh_req_ready_wdata_i  (reqq_to_l3_mesh_req_ready_wdata),
    .reqq_to_l3_mesh_req_info_o         (reqq_to_l3_mesh_req_info),
    .to_l3_mesh_rsp_valid_i             (to_l3_mesh_rsp_valid_reqq),
    .to_l3_mesh_rsp_info_i              (to_l3_mesh_rsp_info),

    // To-sys mesh
    .to_sys_mesh_master_bank_clk_en_o   (to_sys_mesh_master_bank_clk_en_o),
    .reqq_to_sys_mesh_req_valid_nodata_o(reqq_to_sys_mesh_req_valid_nodata),
    .reqq_to_sys_mesh_req_ready_nodata_i(reqq_to_sys_mesh_req_ready_nodata),
    .reqq_to_sys_mesh_req_valid_wdata_o (reqq_to_sys_mesh_req_valid_wdata),
    .reqq_to_sys_mesh_req_ready_wdata_i (reqq_to_sys_mesh_req_ready_wdata),
    .reqq_to_sys_mesh_req_info_o        (reqq_to_sys_mesh_req_info),
    .to_sys_mesh_rsp_valid_i            (to_sys_mesh_rsp_valid_reqq),
    .to_sys_mesh_rsp_info_i             (to_sys_mesh_rsp_info),

    // L2 rspmux
    .reqq_rspmux_l2_valid_nodata_o      (reqq_rspmux_l2_valid_nodata),
    .reqq_rspmux_l2_ready_nodata_i      (reqq_rspmux_l2_ready_nodata),
    .reqq_rspmux_l2_valid_wdata_o       (reqq_rspmux_l2_valid_wdata),
    .reqq_rspmux_l2_ready_wdata_i       (reqq_rspmux_l2_ready_wdata),
    .reqq_rspmux_l2_info_o              (reqq_rspmux_l2_info),

    // L3 rspmux
    .reqq_rspmux_l3_valid_nodata_o      (reqq_rspmux_l3_valid_nodata),
    .reqq_rspmux_l3_ready_nodata_i      (reqq_rspmux_l3_ready_nodata),
    .reqq_rspmux_l3_valid_wdata_o       (reqq_rspmux_l3_valid_wdata),
    .reqq_rspmux_l3_ready_wdata_i       (reqq_rspmux_l3_ready_wdata),
    .reqq_rspmux_l3_info_o              (reqq_rspmux_l3_info),

    // Rspmux sent feedback
    .rspmux_sent_valid_hot_i            (rspmux_l2_sent_valid_hot | rspmux_l3_sent_valid_hot),

    // reqq state
    .reqq_state_o                       (reqq_state),
    .a_reqq_entry_is_valid_o            (a_reqq_entry_is_valid),

    // mbist_on stalls inputs
    .mbist_on_i                         (mbist_on),

    // Performance monitor
    .perfmon_p0_req_info_i              (perfmon_p0_req_info),
    .perfmon_p0_rsp_info_o              (reqq_perfmon_p0_rsp_info),
    .perfmon_p1_req_info_i              (perfmon_p1_req_info),
    .perfmon_p1_rsp_info_o              (reqq_perfmon_p1_rsp_info),

    // ESRs
    .esr_shire_config_i                 (esr_shire_config_i),
    .esr_ctl_i                          (esr_ctl_i),
    .esr_sc_l3_shire_swizzle_ctl_i      (esr_sc_l3_shire_swizzle_ctl)
  );

  // ════════════════════════════════════════════════════════════
  // dataq
  // ════════════════════════════════════════════════════════════

  shirecache_bank_dataq #(
    .NumEntries (NumEntries)
  ) u_dataq (
    .clk_i                              (clk_g),
    .rst_ni                             (rst_n),

    // Pipeline write
    .pipe_data_rsp_valid_i              (pipe_data_rsp_valid),
    .pipe_data_rsp_info_i               (pipe_data_rsp_info),
    .pipe_data_rsp_modifier_info_i      (pipe_data_rsp_modifier_info),
    .mesh_bypassed_victim_i             (mesh_bypassed_victim),
    .rspmux_bypassed_data_rsp_i         (rspmux_bypassed_data_rsp),

    // Bank L2 write requests
    .dataq_bank_l2_ready_o              (dataq_bank_l2_ready),
    .bank_l2_valid_i                    (bank_l2_valid),
    .bank_l2_ready_i                    (bank_l2_ready),
    .bank_l2_info_i                     (bank_l2_info),
    .reqq_alloc_l2_id_ag_i              (reqq_alloc_l2_id_ag),

    // Mesh to_l3 write
    .to_l3_mesh_rsp_valid_i             (to_l3_mesh_rsp_valid_dataq),
    .to_l3_mesh_rsp_info_i              (to_l3_mesh_rsp_info),
    .to_l3_mesh_rsp_ready_o             (to_l3_mesh_rsp_ready_dataq),

    // Bank L3 write requests
    .dataq_bank_l3_ready_o              (dataq_bank_l3_ready),
    .bank_l3_valid_i                    (bank_l3_valid),
    .bank_l3_ready_i                    (bank_l3_ready),
    .bank_l3_info_i                     (bank_l3_info),
    .reqq_alloc_l3_id_ag_i              (reqq_alloc_l3_id_ag),

    // Mesh to_sys write
    .to_sys_mesh_rsp_valid_i            (to_sys_mesh_rsp_valid_dataq),
    .to_sys_mesh_rsp_info_i             (to_sys_mesh_rsp_info),
    .to_sys_mesh_rsp_ready_o            (to_sys_mesh_rsp_ready_dataq),

    // Pipeline read
    .pipe_dataq_rd_req_valid_i          (pipe_dataq_rd_req_valid),
    .pipe_dataq_rd_req_info_i           (pipe_dataq_rd_req_info),
    .pipe_dataq_rd_rsp_info_o           (pipe_dataq_rd_rsp_info),

    // to_l3 mesh read through dataq
    .reqq_to_l3_mesh_req_valid_wdata_i  (reqq_to_l3_mesh_req_valid_wdata),
    .reqq_to_l3_mesh_req_info_i         (reqq_to_l3_mesh_req_info),
    .reqq_to_l3_mesh_req_ready_wdata_o  (reqq_to_l3_mesh_req_ready_wdata),
    .dataq_to_l3_mesh_req_valid_o       (dataq_to_l3_mesh_req_valid),
    .dataq_to_l3_mesh_req_info_o        (dataq_to_l3_mesh_req_info),
    .dataq_to_l3_mesh_req_ready_i       (dataq_to_l3_mesh_req_ready),

    // L2 rspmux read through dataq
    .reqq_rspmux_l2_valid_wdata_i       (reqq_rspmux_l2_valid_wdata),
    .reqq_rspmux_l2_info_i              (reqq_rspmux_l2_info),
    .reqq_rspmux_l2_ready_wdata_o       (reqq_rspmux_l2_ready_wdata),
    .dataq_rspmux_l2_valid_o            (dataq_rspmux_l2_valid),
    .dataq_rspmux_l2_info_o             (dataq_rspmux_l2_info),
    .dataq_rspmux_l2_ready_i            (dataq_rspmux_l2_ready),

    // to_sys mesh read through dataq
    .reqq_to_sys_mesh_req_valid_wdata_i (reqq_to_sys_mesh_req_valid_wdata),
    .reqq_to_sys_mesh_req_info_i        (reqq_to_sys_mesh_req_info),
    .reqq_to_sys_mesh_req_ready_wdata_o (reqq_to_sys_mesh_req_ready_wdata),
    .dataq_to_sys_mesh_req_valid_o      (dataq_to_sys_mesh_req_valid),
    .dataq_to_sys_mesh_req_info_o       (dataq_to_sys_mesh_req_info),
    .dataq_to_sys_mesh_req_ready_i      (dataq_to_sys_mesh_req_ready),

    // L3 slave rspmux read through dataq
    .reqq_rspmux_l3_valid_wdata_i       (reqq_rspmux_l3_valid_wdata),
    .reqq_rspmux_l3_info_i              (reqq_rspmux_l3_info),
    .reqq_rspmux_l3_ready_wdata_o       (reqq_rspmux_l3_ready_wdata),
    .dataq_rspmux_l3_valid_o            (dataq_rspmux_l3_valid),
    .dataq_rspmux_l3_info_o             (dataq_rspmux_l3_info),
    .dataq_rspmux_l3_ready_i            (dataq_rspmux_l3_ready),

    // Write/read status to reqq
    .dataq_write_status_any_o           (dataq_write_status_any),
    .dataq_write_status_hot_o           (dataq_write_status_hot),
    .dataq_write_status_id_o            (dataq_write_status_id),
    .dataq_read_status_hot_o            (dataq_read_status_hot),

    // ESRs
    .esr_ctl_i                          (esr_ctl_i),

    .dft_i                              (dft_i)
  );

  // ════════════════════════════════════════════════════════════
  // Pipeline
  // ════════════════════════════════════════════════════════════

  shirecache_pipe #(
    .SETS_PER_SUB_BANK (SetsPerSubBankP)
  ) u_pipe (
    .clk_i                              (clk_g),
    .rst_ni                             (rst_n),
    .rst_c_ni                           (rst_c_n),

    .dft_i                              (dft_i),
    .dft_sram_clk_i                     (dft_sram_clk_i),
    .ram_cfg_i                          (ram_cfg_i),

    // Request interface from reqq
    .pipe_start_i                       (pipe_start),
    .pipe_req_info_i                    (pipe_req_info),
    .pipe_active_o                      (pipe_active),
    .pipe_busy_o                        (pipe_busy),
    .pipe_squash_o                      (pipe_squash),

    // Response interfaces to reqq
    .pipe_tag_rsp_valid_o               (pipe_tag_rsp_valid),
    .pipe_tag_rsp_info_o                (pipe_tag_rsp_info),
    .pipe_data_rsp_pre_valid_o          (pipe_data_rsp_pre_valid),
    .pipe_data_rsp_pre_info_o           (pipe_data_rsp_pre_info),
    .pipe_data_rsp_valid_o              (pipe_data_rsp_valid),
    .pipe_data_rsp_info_o               (pipe_data_rsp_info),
    .pipe_cbuf_rsp_valid_o              (pipe_cbuf_rsp_valid),
    .pipe_cbuf_rsp_info_o               (pipe_cbuf_rsp_info),

    // Read buffer
    .pipe_rbuf_clear_req_info_i         (pipe_rbuf_clear_req_info),
    .pipe_rbuf_rd_req_ready_o           (pipe_rbuf_rd_req_ready),
    .pipe_rbuf_rd_req_valid_i           (pipe_rbuf_rd_req_valid),
    .pipe_rbuf_rd_req_info_i            (pipe_rbuf_rd_req_info),
    .pipe_rbuf_rd_rsp_ready_i           (pipe_rbuf_rd_rsp_ready),
    .pipe_rbuf_rd_rsp_valid_o           (pipe_rbuf_rd_rsp_valid),
    .pipe_rbuf_rd_rsp_info_o            (pipe_rbuf_rd_rsp_info),
    .pipe_rbuf_state_o                  (pipe_rbuf_state),

    // Data interface to dataq
    .pipe_dataq_rd_req_valid_o          (pipe_dataq_rd_req_valid),
    .pipe_dataq_rd_req_info_o           (pipe_dataq_rd_req_info),
    .pipe_dataq_rd_rsp_info_i           (pipe_dataq_rd_rsp_info),

    // Idx Cop Sm
    .pipe_idx_cop_aborted_o             (pipe_idx_cop_aborted),
    .pipe_idx_cop_l2_busy_o             (pipe_idx_cop_l2_busy),
    .pipe_idx_cop_l3_busy_o             (pipe_idx_cop_l3_busy),
    .pipe_idx_cop_req_ready_i           (pipe_idx_cop_req_ready),
    .pipe_idx_cop_req_valid_o           (pipe_idx_cop_req_valid),
    .pipe_idx_cop_req_info_o            (pipe_idx_cop_req_info),
    .pipe_idx_cop_is_active_o           (pipe_idx_cop_is_active),

    // BIST
    .mbist_on_i                         (mbist_on),
    .pipe_bist_req_info_i               (pipe_bist_req_info),
    .pipe_bist_rsp_info_o               (pipe_bist_rsp_info),

    // Performance monitor
    .perfmon_req_info_i                 (perfmon_p1_req_info),
    .perfmon_rsp_info_o                 (pipe_perfmon_p1_rsp_info),

    // ESRs and constants
    .esr_ctl_i                          (esr_ctl_i),
    .pipe_esr_status_o                  (pipe_esr_status),
    .pipe_sbe_dbe_overflow_o            (pipe_sbe_dbe_overflow),
    .my_bank_id_i                       (my_bank_id_i),
    .my_l3_virtual_shire_id_i           (my_l3_virtual_shire_id),
    .my_scp_virtual_shire_id_i          (my_scp_virtual_shire_id)
  );

  // ════════════════════════════════════════════════════════════
  // To-L3 mesh
  // ════════════════════════════════════════════════════════════

  shirecache_bank_mesh #(
    .IsToL3       (1),
    .NumEntries (NumEntries)
  ) u_to_l3_mesh (
    .clk_i                              (clk_g),
    .rst_ni                             (rst_n),
    .dft_i                              (dft_i),
    .mesh_master_bank_clk_en_i          (to_l3_mesh_master_bank_clk_en_o),

    // Mesh interface
    .mesh_master_bank_req_valid_o       (to_l3_mesh_master_bank_req_valid_o),
    .mesh_master_bank_req_info_o        (to_l3_mesh_master_bank_req_info_o),
    .mesh_master_bank_req_ready_i       (to_l3_mesh_master_bank_req_ready_i),
    .mesh_master_bank_rsp_valid_i       (to_l3_mesh_master_bank_rsp_valid_i),
    .mesh_master_bank_rsp_info_i        (to_l3_mesh_master_bank_rsp_info_i),
    .mesh_master_bank_rsp_ready_o       (to_l3_mesh_master_bank_rsp_ready_o),

    .my_bank_id_i                       (my_bank_id_i),

    // Mesh requests from reqq and dataq
    .reqq_mesh_req_valid_nodata_i       (reqq_to_l3_mesh_req_valid_nodata),
    .reqq_mesh_req_info_i               (reqq_to_l3_mesh_req_info),
    .reqq_mesh_req_ready_nodata_o       (reqq_to_l3_mesh_req_ready_nodata),
    .dataq_mesh_req_valid_i             (dataq_to_l3_mesh_req_valid),
    .dataq_mesh_req_info_i              (dataq_to_l3_mesh_req_info),
    .dataq_mesh_req_ready_o             (dataq_to_l3_mesh_req_ready),

    // Victim from datapipe
    .pipe_data_rsp_valid_i              (pipe_data_rsp_valid),
    .pipe_data_rsp_info_i               (pipe_data_rsp_info),
    .pipe_data_rsp_modifier_info_i      (pipe_data_rsp_modifier_info),
    .mesh_bypassed_victim_o             (to_l3_mesh_bypassed_victim),

    // Mesh responses
    .mesh_rsp_info_o                    (to_l3_mesh_rsp_info),
    .mesh_rsp_valid_reqq_o              (to_l3_mesh_rsp_valid_reqq),
    .mesh_rsp_ready_reqq_i              (to_l3_mesh_rsp_ready_reqq),
    .mesh_rsp_valid_dataq_o             (to_l3_mesh_rsp_valid_dataq),
    .mesh_rsp_ready_dataq_i             (to_l3_mesh_rsp_ready_dataq),
    .mesh_rsp_valid_rspmux_l2_o         (to_l3_mesh_rsp_valid_rspmux_l2),
    .mesh_rsp_ready_rspmux_l2_i         (to_l3_mesh_rsp_ready_rspmux_l2),
    .mesh_rsp_valid_l3_slave_o          (to_l3_mesh_rsp_valid_l3_slave_unused),
    .mesh_rsp_ready_l3_slave_i          (1'b1),

    // reqq state
    .reqq_state_i                       (reqq_state),

    // ESRs
    .esr_ctl_i                          (esr_ctl_i)
  );

  // ════════════════════════════════════════════════════════════
  // Rspmux (L2)
  // ════════════════════════════════════════════════════════════

  shirecache_bank_rspmux #(
    .NumEntries (NumEntries),
    .NumPorts   (NumPorts)
  ) u_rspmux (
    .clk_i                              (clk_g),
    .rst_ni                             (rst_n),

    // Bank rsp output to xbar
    .bank_rsp_valid_o                   (bank_rsp_valid_o),
    .bank_rsp_info_o                    (bank_rsp_info_o),
    .bank_rsp_ready_i                   (bank_rsp_ready_i),
    .bank_rsp_2_credits_i              (bank_rsp_2_credits_i),

    // Read buffer data response
    .pipe_rbuf_rd_rsp_valid_i           (pipe_rbuf_rd_rsp_valid),
    .pipe_rbuf_rd_rsp_info_i            (pipe_rbuf_rd_rsp_info),
    .pipe_rbuf_rd_rsp_ready_o           (pipe_rbuf_rd_rsp_ready_rspmux_l2),

    // Responses from pipe data RAMs
    .pipe_data_rsp_valid_i              (pipe_data_rsp_valid),
    .pipe_data_rsp_info_i               (pipe_data_rsp_info),
    .pipe_data_rsp_modifier_info_i      (pipe_data_rsp_modifier_info),
    .pipe_data_rsp_ready_o              (rspmux_l2_bypassed_data_rsp),

    // Fill responses from mesh
    .to_l3_mesh_rsp_valid_i             (to_l3_mesh_rsp_valid_rspmux_l2),
    .to_l3_mesh_rsp_info_i              (to_l3_mesh_rsp_info),
    .to_l3_mesh_rsp_ready_o             (to_l3_mesh_rsp_ready_rspmux_l2),
    .to_sys_mesh_rsp_valid_i            (to_sys_mesh_rsp_valid_rspmux_l2),
    .to_sys_mesh_rsp_info_i             (to_sys_mesh_rsp_info),
    .to_sys_mesh_rsp_ready_o            (to_sys_mesh_rsp_ready_rspmux_l2),

    // reqq responses
    .reqq_rspmux_valid_nodata_i         (reqq_rspmux_l2_valid_nodata),
    .reqq_rspmux_info_i                 (reqq_rspmux_l2_info),
    .reqq_rspmux_ready_nodata_o         (reqq_rspmux_l2_ready_nodata),

    // dataq responses
    .dataq_rspmux_valid_i               (dataq_rspmux_l2_valid),
    .dataq_rspmux_info_i                (dataq_rspmux_l2_info),
    .dataq_rspmux_ready_o               (dataq_rspmux_l2_ready),

    // Rspmux sent feedback
    .rspmux_sent_valid_hot_o            (rspmux_l2_sent_valid_hot),
    .rspmux_trace_sent_o                (rspmux_l2_trace_sent),
    .a_rspmux_rsp_is_valid_o            (a_rspmux_l2_rsp_is_valid),

    // reqq state
    .reqq_state_i                       (reqq_state),
    .esr_ctl_i                          (esr_ctl_i)
  );

  // ════════════════════════════════════════════════════════════
  // L3 slave
  // ════════════════════════════════════════════════════════════

  shirecache_bank_l3_slave #(
    .NumEntries (NumEntries)
  ) u_l3_slave (
    .clk_i                              (clk_g),
    .rst_ni                             (rst_n),

    // Mesh interface
    .l3_mesh_slave_bank_req_valid_i     (l3_mesh_slave_bank_req_valid_i),
    .l3_mesh_slave_bank_req_info_i      (l3_mesh_slave_bank_req_info_i),
    .l3_mesh_slave_bank_req_ready_o     (l3_mesh_slave_bank_req_ready_o),
    .l3_mesh_slave_bank_rsp_valid_o     (l3_mesh_slave_bank_rsp_valid_o),
    .l3_mesh_slave_bank_rsp_info_o      (l3_mesh_slave_bank_rsp_info_o),
    .l3_mesh_slave_bank_rsp_ready_i     (l3_mesh_slave_bank_rsp_ready_i),

    // Alloc new request to reqq
    .bank_l3_valid_o                    (bank_l3_valid),
    .bank_l3_info_o                     (bank_l3_info),
    .bank_l3_ready_i                    (bank_l3_ready),

    // Rspmux — pipe data rsp
    .pipe_data_rsp_valid_i              (pipe_data_rsp_valid),
    .pipe_data_rsp_info_i               (pipe_data_rsp_info),
    .pipe_data_rsp_modifier_info_i      (pipe_data_rsp_modifier_info),
    .pipe_data_rsp_ready_o              (rspmux_l3_bypassed_data_rsp),

    // to_sys mesh rsp
    .to_sys_mesh_rsp_valid_l3_slave_i   (to_sys_mesh_rsp_valid_l3_slave),
    .to_sys_mesh_rsp_info_i             (to_sys_mesh_rsp_info),
    .to_sys_mesh_rsp_ready_l3_slave_o   (to_sys_mesh_rsp_ready_l3_slave),

    // reqq responses
    .reqq_rspmux_valid_nodata_i         (reqq_rspmux_l3_valid_nodata),
    .reqq_rspmux_info_i                 (reqq_rspmux_l3_info),
    .reqq_rspmux_ready_nodata_o         (reqq_rspmux_l3_ready_nodata),

    // dataq responses
    .dataq_rspmux_valid_i               (dataq_rspmux_l3_valid),
    .dataq_rspmux_info_i                (dataq_rspmux_l3_info),
    .dataq_rspmux_ready_o               (dataq_rspmux_l3_ready),

    // rbuf responses
    .pipe_rbuf_rd_rsp_valid_i           (pipe_rbuf_rd_rsp_valid),
    .pipe_rbuf_rd_rsp_info_i            (pipe_rbuf_rd_rsp_info),
    .pipe_rbuf_rd_rsp_ready_o           (pipe_rbuf_rd_rsp_ready_rspmux_l3),

    // rspmux sent notification
    .rspmux_sent_valid_o                (rspmux_l3_sent_valid),
    .rspmux_sent_id_o                   (rspmux_l3_sent_id),
    .rspmux_sent_valid_hot_o            (rspmux_l3_sent_valid_hot),
    .a_rspmux_rsp_is_valid_o            (a_rspmux_l3_rsp_is_valid),

    // reqq state
    .reqq_state_i                       (reqq_state),
    .esr_ctl_i                          (esr_ctl_i)
  );

  // ════════════════════════════════════════════════════════════
  // To-sys mesh
  // ════════════════════════════════════════════════════════════

  shirecache_bank_mesh #(
    .IsToL3       (0),
    .NumEntries (NumEntries)
  ) u_to_sys_mesh (
    .clk_i                              (clk_g),
    .rst_ni                             (rst_n),
    .dft_i                              (dft_i),
    .mesh_master_bank_clk_en_i          (to_sys_mesh_master_bank_clk_en_o),

    // Mesh interface
    .mesh_master_bank_req_valid_o       (to_sys_mesh_master_bank_req_valid_o),
    .mesh_master_bank_req_info_o        (to_sys_mesh_master_bank_req_info_o),
    .mesh_master_bank_req_ready_i       (to_sys_mesh_master_bank_req_ready_i),
    .mesh_master_bank_rsp_valid_i       (to_sys_mesh_master_bank_rsp_valid_i),
    .mesh_master_bank_rsp_info_i        (to_sys_mesh_master_bank_rsp_info_i),
    .mesh_master_bank_rsp_ready_o       (to_sys_mesh_master_bank_rsp_ready_o),

    .my_bank_id_i                       (my_bank_id_i),

    // Mesh requests from reqq and dataq
    .reqq_mesh_req_valid_nodata_i       (reqq_to_sys_mesh_req_valid_nodata),
    .reqq_mesh_req_info_i               (reqq_to_sys_mesh_req_info),
    .reqq_mesh_req_ready_nodata_o       (reqq_to_sys_mesh_req_ready_nodata),
    .dataq_mesh_req_valid_i             (dataq_to_sys_mesh_req_valid),
    .dataq_mesh_req_info_i              (dataq_to_sys_mesh_req_info),
    .dataq_mesh_req_ready_o             (dataq_to_sys_mesh_req_ready),

    // Victim from datapipe
    .pipe_data_rsp_valid_i              (pipe_data_rsp_valid),
    .pipe_data_rsp_info_i               (pipe_data_rsp_info),
    .pipe_data_rsp_modifier_info_i      (pipe_data_rsp_modifier_info),
    .mesh_bypassed_victim_o             (to_sys_mesh_bypassed_victim),

    // Mesh responses
    .mesh_rsp_info_o                    (to_sys_mesh_rsp_info),
    .mesh_rsp_valid_reqq_o              (to_sys_mesh_rsp_valid_reqq),
    .mesh_rsp_ready_reqq_i              (to_sys_mesh_rsp_ready_reqq),
    .mesh_rsp_valid_dataq_o             (to_sys_mesh_rsp_valid_dataq),
    .mesh_rsp_ready_dataq_i             (to_sys_mesh_rsp_ready_dataq),
    .mesh_rsp_valid_rspmux_l2_o         (to_sys_mesh_rsp_valid_rspmux_l2),
    .mesh_rsp_ready_rspmux_l2_i         (to_sys_mesh_rsp_ready_rspmux_l2),
    .mesh_rsp_valid_l3_slave_o          (to_sys_mesh_rsp_valid_l3_slave),
    .mesh_rsp_ready_l3_slave_i          (to_sys_mesh_rsp_ready_l3_slave),

    // reqq state
    .reqq_state_i                       (reqq_state),

    // ESRs
    .esr_ctl_i                          (esr_ctl_i)
  );

  // ════════════════════════════════════════════════════════════
  // Error logger
  // ════════════════════════════════════════════════════════════

  shirecache_bank_err_logger #(
    .NumPorts   (NumPorts),
    .NumEntries (NumEntries)
  ) u_err_log (
    .clk_i                              (clk_g),
    .rst_ni                             (rst_c_n),  // cold reset only

    // Output to IO-Shire
    .err_detected_o                     (err_detected_o),
    .err_logged_o                       (err_logged_o),

    // Output to reqq
    .set_reqq_err_detected_hot_o        (set_reqq_err_detected_hot),
    .set_reqq_err_zero_data_hot_o       (set_reqq_err_zero_data_hot),

    // Traced reqq state
    .traced_reqq_state_o                (traced_reqq_state),

    // Allocation decode error
    .alloc_l2_valid_ad_i                (alloc_l2_valid_ad),
    .alloc_l2_info_ad_i                 (alloc_l2_info_ad),
    .alloc_l3_valid_ad_i                (alloc_l3_valid_ad),
    .alloc_l3_info_ad_i                 (alloc_l3_info_ad),
    // Tag error
    .pipe_tag_rsp_valid_i               (pipe_tag_rsp_valid),
    .pipe_tag_rsp_info_i                (pipe_tag_rsp_info),
    // Data error
    .pipe_data_rsp_valid_i              (pipe_data_rsp_valid),
    .pipe_data_rsp_info_i               (pipe_data_rsp_info),
    // Mesh response errors
    .to_l3_mesh_rsp_valid_i             (to_l3_mesh_rsp_valid_reqq),
    .to_l3_mesh_rsp_info_i              (to_l3_mesh_rsp_info),
    .to_sys_mesh_rsp_valid_i            (to_sys_mesh_rsp_valid_reqq),
    .to_sys_mesh_rsp_info_i             (to_sys_mesh_rsp_info),
    // ECC saturation errors
    .pipe_sbe_dbe_overflow_i            (pipe_sbe_dbe_overflow),
    // Performance monitor saturation errors
    .perfmon_cntr_overflow_i            (perfmon_cntr_overflow),

    // I/O valid/ready debug
    .bank_req_valid_i                   (bank_req_ready_o),
    .bank_req_ready_i                   (bank_req_valid_i),
    .bank_rsp_valid_i                   (bank_rsp_valid_o),
    .bank_rsp_ready_i                   (bank_rsp_ready_i),
    .to_l3_mesh_master_bank_req_valid_i (to_l3_mesh_master_bank_req_valid_o),
    .to_l3_mesh_master_bank_req_ready_i (to_l3_mesh_master_bank_req_ready_i),
    .to_l3_mesh_master_bank_rsp_valid_i (to_l3_mesh_master_bank_rsp_valid_i),
    .to_l3_mesh_master_bank_rsp_ready_i (to_l3_mesh_master_bank_rsp_ready_o),
    .to_sys_mesh_master_bank_req_valid_i(to_sys_mesh_master_bank_req_valid_o),
    .to_sys_mesh_master_bank_req_ready_i(to_sys_mesh_master_bank_req_ready_i),
    .to_sys_mesh_master_bank_rsp_valid_i(to_sys_mesh_master_bank_rsp_valid_i),
    .to_sys_mesh_master_bank_rsp_ready_i(to_sys_mesh_master_bank_rsp_ready_o),
    .l3_mesh_slave_bank_req_valid_i     (l3_mesh_slave_bank_req_valid_i),
    .l3_mesh_slave_bank_req_ready_i     (l3_mesh_slave_bank_req_ready_o),
    .l3_mesh_slave_bank_rsp_valid_i     (l3_mesh_slave_bank_rsp_valid_o),
    .l3_mesh_slave_bank_rsp_ready_i     (l3_mesh_slave_bank_rsp_ready_i),

    // reqq state
    .reqq_state_i                       (reqq_state),

    // ESRs
    .err_log_esr_status_o               (err_log_esr_status),
    .esr_ctl_i                          (esr_ctl_i)
  );

  // ════════════════════════════════════════════════════════════
  // Performance monitor
  // ════════════════════════════════════════════════════════════

  shirecache_bank_perfmon u_perfmon (
    .clk_i                              (clk_g),
    .clk_free_i                         (clk_free_i),
    .rst_ni                             (rst_n),
    .dft_i                              (dft_i),

    // Performance monitor for resource and event
    .perfmon_p0_req_info_o              (perfmon_p0_req_info),
    .perfmon_p0_rsp_info_i              (perfmon_p0_rsp_info),
    .perfmon_p1_req_info_o              (perfmon_p1_req_info),
    .perfmon_p1_rsp_info_i              (perfmon_p1_rsp_info),

    // Performance counter overflow
    .perfmon_cntr_overflow_o            (perfmon_cntr_overflow),

    // ESRs
    .esr_ctl_i                          (esr_ctl_i),
    .perfmon_esr_status_o               (perfmon_esr_status)
  );

  // ════════════════════════════════════════════════════════════
  // Trace
  // ════════════════════════════════════════════════════════════

  shirecache_bank_trace #(
    .NumEntries (NumEntries)
  ) u_trace (
    .clk_free_i                         (clk_free_i),
    .rst_ni                             (rst_d_n),  // debug reset
    .dft_i                              (dft_i),

    // Output to IO-Shire
    .bank_trace_data_o                  (bank_trace_data_o),
    .bank_trace_valid_o                 (bank_trace_valid_o),

    // Allocation
    .alloc_l2_valid_ag_i                (alloc_l2_valid_ag),
    .alloc_l2_info_ag_i                 (alloc_l2_info_ag),
    .alloc_l3_valid_ag_i                (alloc_l3_valid_ag),
    .alloc_l3_info_ag_i                 (alloc_l3_info_ag),

    // Tag TC
    .pipe_tag_rsp_valid_i               (pipe_tag_rsp_valid),
    .pipe_tag_rsp_info_i                (pipe_tag_rsp_info),
    // Rbuf
    .pipe_rbuf_rd_req_ready_i           (pipe_rbuf_rd_req_ready),
    .pipe_rbuf_rd_req_valid_i           (pipe_rbuf_rd_req_valid),
    .pipe_rbuf_rd_req_info_i            (pipe_rbuf_rd_req_info),

    // Mesh requests / responses
    .to_l3_mesh_master_bank_req_valid_i (to_l3_mesh_master_bank_req_valid_o),
    .to_l3_mesh_master_bank_req_info_i  (to_l3_mesh_master_bank_req_info_o),
    .to_l3_mesh_master_bank_req_ready_i (to_l3_mesh_master_bank_req_ready_i),
    .to_l3_mesh_master_bank_rsp_valid_i (to_l3_mesh_master_bank_rsp_valid_i),
    .to_l3_mesh_master_bank_rsp_info_i  (to_l3_mesh_master_bank_rsp_info_i),
    .to_l3_mesh_master_bank_rsp_ready_i (to_l3_mesh_master_bank_rsp_ready_o),
    .to_sys_mesh_master_bank_req_valid_i(to_sys_mesh_master_bank_req_valid_o),
    .to_sys_mesh_master_bank_req_info_i (to_sys_mesh_master_bank_req_info_o),
    .to_sys_mesh_master_bank_req_ready_i(to_sys_mesh_master_bank_req_ready_i),
    .to_sys_mesh_master_bank_rsp_valid_i(to_sys_mesh_master_bank_rsp_valid_i),
    .to_sys_mesh_master_bank_rsp_info_i (to_sys_mesh_master_bank_rsp_info_i),
    .to_sys_mesh_master_bank_rsp_ready_i(to_sys_mesh_master_bank_rsp_ready_o),

    // l3 slave rsp
    .rspmux_l3_sent_valid_i             (rspmux_l3_sent_valid),
    .rspmux_l3_sent_id_i                (rspmux_l3_sent_id),

    // l2 neigh rsp
    .rspmux_l2_trace_sent_i             (rspmux_l2_trace_sent),

    // Traced reqq state
    .traced_reqq_state_i                (traced_reqq_state),

    // reqq state
    .reqq_state_i                       (reqq_state),

    // ESRs
    .esr_ctl_i                          (esr_ctl_i)
  );

  // ════════════════════════════════════════════════════════════
  // BIST wrapper
  // ════════════════════════════════════════════════════════════

  shirecache_bist_wrapper u_bist_wrapper (
    .clk_i                              (clk_g),
    .rst_ni                             (rst_n),
    .mbi_implemented_const_i            (mbi_implemented_const_i),
    .dft_mbist_en_i                     (dft_mbist_en_i),

    // MBIST shared interface
    .mbist_en_i                         (mbist_en_i),
    .mbist_sel_i                        (mbist_sel_i),
    .mbist_rd_en_i                      (mbist_rd_en_i),
    .mbist_wr_en_i                      (mbist_wr_en_i),
    .mbist_addr_i                       (mbist_addr_i),
    .mbist_wdata_i                      (mbist_wdata_i),
    .mbist_rdata_o                      (mbist_rdata_o),

    // Internal BIST req/rsp
    .mbist_on_o                         (mbist_on),
    .pipe_bist_req_info_o               (pipe_bist_req_info),
    .pipe_bist_rsp_info_i               (pipe_bist_rsp_info),
    .icache_bist_req_info_o             (icache_bist_req_info_o),
    .icache_bist_rsp_info_i             (icache_bist_rsp_info_i)
  );

  // ════════════════════════════════════════════════════════════
  // L2 HPF (Hardware Prefetcher)
  // ════════════════════════════════════════════════════════════

  shirecache_bank_l2hpf #(
    .NumEntries      (NumEntries),
    .L2HpfImplemented(L2HpfImplemented)
  ) u_l2hpf (
    .clk_i                              (clk_g),
    .rst_ni                             (rst_n),

    // Pipeline tag response
    .pipe_tag_rsp_valid_i               (pipe_tag_rsp_valid),
    .pipe_tag_rsp_info_i                (pipe_tag_rsp_info),

    // L2 HPF request
    .l2hpf_req_valid_o                  (l2hpf_req_valid_o),
    .l2hpf_req_info_o                   (l2hpf_req_info_o),

    // reqq state
    .reqq_state_i                       (reqq_state)
  );

  // ════════════════════════════════════════════════════════════
  // gen_ben function
  // ════════════════════════════════════════════════════════════

  function automatic logic [LineByteSize-1:0] gen_ben(
    input sc_size_t                   size,
    input logic [LineOffsetIdSize-1:0] address
  );
    logic [LineByteSize-1:0] size_bits;
    unique case (size)
      SizeByte:  size_bits = ByteBen;
      SizeHWord: size_bits = HwBen;
      SizeWord:  size_bits = SwBen;
      SizeDWord: size_bits = DwBen;
      SizeQWord: size_bits = QwBen;
      SizeHLine: size_bits = HlBen;
      SizeLine:  size_bits = LineBen;
      default:   size_bits = LineBen;
    endcase
    gen_ben = size_bits << address;
  endfunction

  // ════════════════════════════════════════════════════════════
  // Unused signal tie-off
  // ════════════════════════════════════════════════════════════

  logic unused_ok;
  assign unused_ok = &{1'b0,
    bank_req_fifo_level_unused,
    to_l3_mesh_rsp_valid_l3_slave_unused
  };

endmodule
/* verilator lint_on UNOPTFLAT */
/* verilator lint_on UNUSEDSIGNAL */
