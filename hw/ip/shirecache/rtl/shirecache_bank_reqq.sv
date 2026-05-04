// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire Cache bank request queue controller.
//
// Holds all requests and victims in-flight in a bank.  Responsible for
// issuing requests to the pipeline and mesh, and responses back to
// neighborhoods.  Ordering between requests and victims with same
// address is enforced via linked lists.
//
// Instantiates 2x reqq_alloc (L2, L3) + 64x reqq_entry + arbitration
// for pipeline / mesh / rbuf / rspmux.
//
// Replaces: shire_cache_bank_reqq (etcore-soc)
//
// Differences from original:
//   - Active-low async-assert/sync-deassert reset (rst_ni)
//   - lowRISC naming conventions (_i/_o)
//   - prim_arb_rr, prim_arb_prio, prim_fifo_reg, prim_rst_sync primitives
//   - Package types instead of `define types
//   - Explicit always_ff instead of RST_FF/EN_FF/FF macros
//   - hot2bin / onehot_mux inlined
//   - DFT consolidated into dft_t

/* verilator lint_off UNUSEDSIGNAL */  // packed struct ports passed whole; reqq uses subfields only
/* verilator lint_off UNOPTFLAT */     // struct fields written by independent always_comb blocks
/* verilator lint_off UNUSEDPARAM */   // CgateDisReqq retained for documentation/future use
/* verilator lint_off VARHIDDEN */     // function param 'entries' shadows generate block label; no ambiguity
module shirecache_bank_reqq
  import shirecache_pkg::*;
  import dft_pkg::*;
#(
  parameter int unsigned NumEntries   = ReqqDepth,
  parameter int unsigned NumPorts     = Ports,
  parameter int unsigned NumEntriesId = ReqqIdSize
) (
  input  logic                        clk_i,
  input  logic                        rst_ni,
  input  dft_t                        dft_i,

  // L2 Requests to bank
  output logic                        bank_l2_ready_o,
  input  logic                        bank_l2_valid_i,
  input  reqq_bank_req_t              bank_l2_info_i,
  output logic [NumEntriesId-1:0]     reqq_alloc_l2_id_ag_o,
  input  logic                        dataq_bank_l2_ready_i,  // dataq has room for incoming write

  // L3 Requests to bank
  output logic                        bank_l3_ready_o,
  input  logic                        bank_l3_valid_i,
  input  reqq_bank_req_t              bank_l3_info_i,
  output logic [NumEntriesId-1:0]     reqq_alloc_l3_id_ag_o,
  input  logic                        dataq_bank_l3_ready_i,  // dataq has room for incoming write

  // Alloc info outputs to error logger
  output logic                        alloc_l2_valid_ag_o,
  output reqq_alloc_t                 alloc_l2_info_ag_o,
  output logic                        alloc_l3_valid_ag_o,
  output reqq_alloc_t                 alloc_l3_info_ag_o,
  output logic                        alloc_l2_valid_ad_o,
  output reqq_alloc_t                 alloc_l2_info_ad_o,
  output logic                        alloc_l3_valid_ad_o,
  output reqq_alloc_t                 alloc_l3_info_ad_o,

  // Error logger detected an error in a reqq entry
  input  logic [NumEntries-1:0]       set_reqq_err_detected_hot_i,
  input  logic [NumEntries-1:0]       set_reqq_err_zero_data_hot_i,

  // Idx Requests from CacheOp FSM
  input  logic                        pipe_idx_cop_aborted_i,
  input  logic                        pipe_idx_cop_l2_busy_i,
  input  logic                        pipe_idx_cop_l3_busy_i,
  output logic                        pipe_idx_cop_req_ready_o,
  input  logic                        pipe_idx_cop_req_valid_i,
  input  idx_cop_req_t                pipe_idx_cop_req_info_i,

  // Dataq write in ram feedback
  input  logic                        dataq_write_status_any_i,
  input  logic [NumEntries-1:0]       dataq_write_status_hot_i,
  input  logic [NumEntriesId-1:0]     dataq_write_status_id_i,
  input  logic [NumEntries-1:0]       dataq_read_status_hot_i,

  // Interfaces with the pipeline
  output logic                        pipe_start_o,
  output reqq_pipe_req_t              pipe_req_info_o,
  input  pipe_busy_t                  pipe_busy_i,
  input  logic                        pipe_squash_i,
  input  logic                        pipe_tag_rsp_valid_i,
  input  tag_rsp_t                    pipe_tag_rsp_info_i,
  input  logic                        pipe_data_rsp_valid_i,
  input  data_rsp_t                   pipe_data_rsp_info_i,
  input  logic                        pipe_data_rsp_pre_valid_i,  // 1 cp before pipe_data_rsp_valid
  input  data_rsp_pre_t               pipe_data_rsp_pre_info_i,
  input  logic                        pipe_cbuf_rsp_valid_i,
  input  cbuf_rsp_t                   pipe_cbuf_rsp_info_i,
  input  logic                        mesh_bypassed_victim_i,       // victim already sent to mesh
  input  logic                        rspmux_bypassed_data_rsp_i,   // response already sent to rspmux

  // Interfaces with mesh and dataq
  output data_rsp_modifier_t          pipe_data_rsp_modifier_info_o, // fixup reqq id / prevent extraneous victim

  // Interfaces with the read buffer
  output rbuf_clear_req_t             pipe_rbuf_clear_req_info_o,
  output logic                        pipe_rbuf_rd_req_valid_o,
  output rbuf_rd_req_t                pipe_rbuf_rd_req_info_o,
  input  logic                        pipe_rbuf_rd_req_ready_i,
  input  rbuf_state_t                 pipe_rbuf_state_i,

  // Interfaces with to_l3 mesh
  output logic                        to_l3_mesh_master_bank_clk_en_o,
  output logic                        reqq_to_l3_mesh_req_valid_nodata_o,
  input  logic                        reqq_to_l3_mesh_req_ready_nodata_i,
  output logic                        reqq_to_l3_mesh_req_valid_wdata_o,
  input  logic                        reqq_to_l3_mesh_req_ready_wdata_i,
  output reqq_mesh_req_t              reqq_to_l3_mesh_req_info_o,
  input  logic                        to_l3_mesh_rsp_valid_i,
  input  reqq_mesh_rsp_t              to_l3_mesh_rsp_info_i,

  // Interfaces with to_sys mesh
  output logic                        to_sys_mesh_master_bank_clk_en_o,
  output logic                        reqq_to_sys_mesh_req_valid_nodata_o,
  input  logic                        reqq_to_sys_mesh_req_ready_nodata_i,
  output logic                        reqq_to_sys_mesh_req_valid_wdata_o,
  input  logic                        reqq_to_sys_mesh_req_ready_wdata_i,
  output reqq_mesh_req_t              reqq_to_sys_mesh_req_info_o,
  input  logic                        to_sys_mesh_rsp_valid_i,
  input  reqq_mesh_rsp_t              to_sys_mesh_rsp_info_i,

  // Responses back to neighborhoods
  output logic                        reqq_rspmux_l2_valid_nodata_o,
  input  logic                        reqq_rspmux_l2_ready_nodata_i,
  output logic                        reqq_rspmux_l2_valid_wdata_o,
  input  logic                        reqq_rspmux_l2_ready_wdata_i,
  output rspmux_t                     reqq_rspmux_l2_info_o,

  // Responses back to L3 slave
  output logic                        reqq_rspmux_l3_valid_nodata_o,
  input  logic                        reqq_rspmux_l3_ready_nodata_i,
  output logic                        reqq_rspmux_l3_valid_wdata_o,
  input  logic                        reqq_rspmux_l3_ready_wdata_i,
  output rspmux_t                     reqq_rspmux_l3_info_o,

  // Rspmux feedback of which response is getting sent back
  input  logic [NumEntries-1:0]       rspmux_sent_valid_hot_i,

  // reqq state
  output reqq_entry_state_t [NumEntries-1:0] reqq_state_o,
  output logic                        a_reqq_entry_is_valid_o,

  // mbist_on stalls l2 and l3 inputs
  input  logic                        mbist_on_i,

  // Performance monitor for resource and event
  input  perfmon_req_info_t           perfmon_p0_req_info_i,
  output perfmon_rsp_info_t           perfmon_p0_rsp_info_o,
  input  perfmon_req_info_t           perfmon_p1_req_info_i,
  output perfmon_rsp_info_t           perfmon_p1_rsp_info_o,

  // ESRs
  input  esr_shire_config_t           esr_shire_config_i,
  input  bank_esr_ctl_t               esr_ctl_i,
  input  l3_swizzle_ctl_t             esr_sc_l3_shire_swizzle_ctl_i
);

  // ══════════════════════════════════════════════════════════
  // Reset synchronizer
  // ══════════════════════════════════════════════════════════

  /* verilator lint_off SYNCASYNCNET */  // rst_n used as both async reset and sync clock-enable (matches original design intent)
  logic rst_n;
  /* verilator lint_on SYNCASYNCNET */
  prim_rst_sync u_rst_sync (
    .clk_i  (clk_i),
    .rst_ni (rst_ni),
    .dft_i  (dft_i),
    .rst_no (rst_n)
  );

  // ══════════════════════════════════════════════════════════
  // Local parameters
  // ══════════════════════════════════════════════════════════

  localparam int unsigned CgateDisReqq = 0; // SC_CLK_GATE_DISABLE_REQQ value

  // ══════════════════════════════════════════════════════════
  // ESR programmable number of L3 versus L2 entries
  // ══════════════════════════════════════════════════════════
  // Note if NumEntries > 64, the reqq_l3_entries has to be refactored.

  logic [NumEntries-1:0] reqq_l3_entries;
  assign reqq_l3_entries =
    NumEntries'(({{64{1'b1}}, {NumEntries{1'b0}}}) >> esr_ctl_i.esr_sc_num_l3_reqq_entries) &
    ({{NumEntries-2{1'b1}}, {2{1'b0}}});
  logic [NumEntries-1:0] reqq_l2_entries;
  assign reqq_l2_entries = ~reqq_l3_entries;

  logic [NumEntries-1:0] reqq_entry_valid;
  logic [NumEntries-1:0] clear_dep_hot;  // entries that no longer need to pause dependencies behind them

  // For Alloc
  logic [NumEntries-1:0] alloc_upgrade_to_meshll_hot;
  logic [NumEntries-1:0] upgrade_to_meshll_hot;
  logic [NumEntries-1:0] entry_is_non_meshll;

  // For DV
  logic [NumEntries-1:0] reqq_entries_alloc_hot;

  // For Dealloc
  logic [NumEntries-1:0] reqq_entries_dealloc_hot;

  logic coalesce_victim_match_alloc_ag;   // coalescing victim matched this alloc
  logic alloc_match_tag_victim_ag;        // tag victim matched this alloc

  // ══════════════════════════════════════════════════════════
  // Allocate L2 / L3 logic
  // ══════════════════════════════════════════════════════════

  logic [NumEntries-1:0] alloc_l2_1hot_ag;
  logic [NumEntries-1:0] alloc_l2_1hot_ad;

  logic [NumEntries-1:0] alloc_l3_1hot_ag;
  logic [NumEntries-1:0] alloc_l3_1hot_ad;

  logic            [1:0]                      alloc_l2_l3_match_tag_victim_ag;
  logic            [1:0]                      bank_ready;
  logic            [1:0] [NumEntriesId-1:0]   reqq_alloc_id_ag;
  logic            [1:0]                      alloc_valid_ag;
  logic            [1:0] [NumEntries-1:0]     alloc_1hot_ag;
  reqq_alloc_t     [1:0]                      alloc_info_ag;
  logic            [1:0]                      alloc_valid_ad;
  logic            [1:0] [NumEntries-1:0]     alloc_1hot_ad;
  reqq_alloc_t     [1:0]                      alloc_info_ad;
  logic            [1:0]                      pipe_idx_cop_req_ready_pre;
  logic            [1:0]                      reqq_full;

  assign alloc_match_tag_victim_ag  = |(alloc_l2_l3_match_tag_victim_ag);

  assign bank_l2_ready_o            = bank_ready[0];
  assign reqq_alloc_l2_id_ag_o      = reqq_alloc_id_ag[0];

  assign alloc_l2_valid_ag_o        = alloc_valid_ag[0];
  assign alloc_l2_1hot_ag           = alloc_1hot_ag[0];
  assign alloc_l2_info_ag_o         = alloc_info_ag[0];

  assign alloc_l2_valid_ad_o        = alloc_valid_ad[0];
  assign alloc_l2_1hot_ad           = alloc_1hot_ad[0];
  assign alloc_l2_info_ad_o         = alloc_info_ad[0];

  assign bank_l3_ready_o            = bank_ready[1];
  assign reqq_alloc_l3_id_ag_o      = reqq_alloc_id_ag[1];

  assign alloc_l3_valid_ag_o        = alloc_valid_ag[1];
  assign alloc_l3_1hot_ag           = alloc_1hot_ag[1];
  assign alloc_l3_info_ag_o         = alloc_info_ag[1];

  assign alloc_l3_valid_ad_o        = alloc_valid_ad[1];
  assign alloc_l3_1hot_ad           = alloc_1hot_ad[1];
  assign alloc_l3_info_ad_o         = alloc_info_ad[1];

  assign pipe_idx_cop_req_ready_o   = |(pipe_idx_cop_req_ready_pre);
  logic reqq_l2_full;
  logic reqq_l3_full;
  assign reqq_l2_full               = reqq_full[0];
  assign reqq_l3_full               = reqq_full[1];

  // Paired allocation for WriteAround2 and Atomic2
  logic [NumEntries-1:0] alloc_l2_1hot_paired_ag;
  logic [NumEntries-1:0] alloc_l3_1hot_paired_ag;
  logic            [1:0] [NumEntries-1:0]    alloc_1hot_paired_ag;
  reqq_alloc_t     [1:0]                     alloc_info_paired_ag;
  reqq_alloc_t     alloc_l2_info_paired_ag;
  reqq_alloc_t     alloc_l3_info_paired_ag;
  assign alloc_l2_1hot_paired_ag    = alloc_1hot_paired_ag[0];
  assign alloc_l3_1hot_paired_ag    = alloc_1hot_paired_ag[1];
  assign alloc_l2_info_paired_ag    = alloc_info_paired_ag[0];
  assign alloc_l3_info_paired_ag    = alloc_info_paired_ag[1];

  // esr_sc_num_l3_reqq_entries has a 4 cyc MCP path
  logic esr_wr_reqq_entries_d1;
  logic esr_wr_reqq_entries_d2;
  logic esr_wr_reqq_entries_d3;
  logic esr_wr_reqq_entries_d4;

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n) begin
      esr_wr_reqq_entries_d1 <= '0;
      esr_wr_reqq_entries_d2 <= '0;
      esr_wr_reqq_entries_d3 <= '0;
      esr_wr_reqq_entries_d4 <= '0;
    end else if (esr_ctl_i.access_in_flight) begin
      esr_wr_reqq_entries_d1 <= esr_ctl_i.wr_en_reqq_ctl;
      esr_wr_reqq_entries_d2 <= esr_wr_reqq_entries_d1;
      esr_wr_reqq_entries_d3 <= esr_wr_reqq_entries_d2;
      esr_wr_reqq_entries_d4 <= esr_wr_reqq_entries_d3;
    end
  end

  // ══════════════════════════════════════════════════════════
  // Alloc instances (L2 = gen_it 0, L3 = gen_it 1)
  // ══════════════════════════════════════════════════════════

  for (genvar gen_it = 0; gen_it < 2; gen_it++) begin : allocs
    shirecache_bank_reqq_alloc #(
      .IsL2       (gen_it == 0),
      .NumPorts   (NumPorts),
      .NumEntries (NumEntries)
    ) alloc (
      .clk_i                         (clk_i),
      .rst_ni                        (rst_n),
      .dft_i                         (dft_i),
      .reqq_allowed_entries_i        ((gen_it == 0) ? reqq_l2_entries : reqq_l3_entries),
      .reqq_full_o                   (reqq_full[gen_it]),

      // L2/L3 Requests to bank
      .bank_ready_o                  (bank_ready[gen_it]),
      .bank_valid_i                  ((gen_it == 0) ? bank_l2_valid_i : bank_l3_valid_i),
      .bank_info_i                   ((gen_it == 0) ? bank_l2_info_i  : bank_l3_info_i),
      .reqq_alloc_id_ag_o            (reqq_alloc_id_ag[gen_it]),
      .dataq_bank_ready_i            ((gen_it == 0) ? dataq_bank_l2_ready_i : dataq_bank_l3_ready_i),

      // Idx Requests from CacheOp FSM
      .pipe_idx_cop_aborted_i        (pipe_idx_cop_aborted_i),
      .pipe_idx_cop_l2_busy_i        (pipe_idx_cop_l2_busy_i),
      .pipe_idx_cop_l3_busy_i        (pipe_idx_cop_l3_busy_i),
      .pipe_idx_cop_req_ready_o      (pipe_idx_cop_req_ready_pre[gen_it]),
      .pipe_idx_cop_req_valid_i      (pipe_idx_cop_req_valid_i),
      .pipe_idx_cop_req_info_i       (pipe_idx_cop_req_info_i),

      // Interfaces with the pipeline
      .pipe_tag_rsp_valid_i          (pipe_tag_rsp_valid_i),
      .pipe_tag_rsp_info_i           (pipe_tag_rsp_info_i),

      // Interfaces with the read buffer
      .pipe_rbuf_clear_req_info_i    (pipe_rbuf_clear_req_info_o),
      .pipe_rbuf_state_i             (pipe_rbuf_state_i),

      .alloc_match_tag_victim_ag_o   (alloc_l2_l3_match_tag_victim_ag[gen_it]),

      // reqq state
      .reqq_entry_valid_i            (reqq_entry_valid),
      .upgrade_to_meshll_hot_i       (alloc_upgrade_to_meshll_hot),
      .entry_is_non_meshll_i         (entry_is_non_meshll),
      .clear_dep_hot_i               (clear_dep_hot),
      .reqq_state_i                  (reqq_state_o),

      // Alloc outputs
      .alloc_valid_ag_o              (alloc_valid_ag[gen_it]),
      .alloc_1hot_ag_o               (alloc_1hot_ag[gen_it]),
      .alloc_info_ag_o               (alloc_info_ag[gen_it]),
      .alloc_valid_ad_o              (alloc_valid_ad[gen_it]),
      .alloc_1hot_ad_o               (alloc_1hot_ad[gen_it]),
      .alloc_info_ad_o               (alloc_info_ad[gen_it]),

      // Paired allocation for WriteAround2 and Atomic2
      .alloc_1hot_paired_ag_o        (alloc_1hot_paired_ag[gen_it]),
      .alloc_info_paired_ag_o        (alloc_info_paired_ag[gen_it]),

      // mbist_on stalls l2 and l3 inputs
      .mbist_on_i                    (mbist_on_i),

      // ESRs
      .esr_shire_config_i            (esr_shire_config_i),
      .esr_wr_reqq_entries_i         (esr_wr_reqq_entries_d4),
      .esr_ctl_i                     (esr_ctl_i)
    );
  end

  assign a_reqq_entry_is_valid_o = |reqq_entry_valid;

  // ══════════════════════════════════════════════════════════
  // Clear rbuf entries
  // ══════════════════════════════════════════════════════════
  // If something is in the rbuf and eligible and it is anything but a read,
  // then clear the rbuf entry.

  logic [NumEntries-1:0] reqq_entry_changes_cache;
  logic [NumEntries-1:0] reqq_rbuf_clear;
  logic [NumEntries-1:0] reqq_rbuf_clear_pending_install;

  always_comb begin
    reqq_rbuf_clear                                          = '0;
    reqq_rbuf_clear_pending_install                          = '0;
    pipe_rbuf_clear_req_info_o.rbuf_clear_mem_valid_hot      = '0;
    pipe_rbuf_clear_req_info_o.rbuf_clear_mem_scp            = '0;
    pipe_rbuf_clear_req_info_o.rbuf_clear_mem_address        = '0;
    pipe_rbuf_clear_req_info_o.rbuf_clear_pending_install_hot     = '0;
    pipe_rbuf_clear_req_info_o.rbuf_clear_pending_install_scp     = '0;
    pipe_rbuf_clear_req_info_o.rbuf_clear_pending_install_address = '0;
    for (integer ii = 0; ii < NumEntries; ii++) begin
      reqq_entry_changes_cache[ii] = ~(reqq_state_o[ii].opcode inside {
        L2Read, ScpRead, L2Unlock, L2Prefetch, L2Flush});
      // Eligible and in rbuf
      if (reqq_state_o[ii].pipe_req_eligible &
          reqq_state_o[ii].rbuf_valid &
          reqq_entry_changes_cache[ii]) begin
        reqq_rbuf_clear[ii] |= 1'b1;
        pipe_rbuf_clear_req_info_o.rbuf_clear_mem_valid_hot[reqq_state_o[ii].rbuf_id]  |= 1'b1;
        pipe_rbuf_clear_req_info_o.rbuf_clear_mem_scp[reqq_state_o[ii].rbuf_id]        |= (reqq_state_o[ii].address_space == AddressScp);
        pipe_rbuf_clear_req_info_o.rbuf_clear_mem_address[reqq_state_o[ii].rbuf_id]    |= reqq_state_o[ii].address;
      end
      // Eligible and pending install
      if (reqq_state_o[ii].pipe_req_eligible &
          reqq_state_o[ii].rbuf_pending_valid &
          reqq_entry_changes_cache[ii]) begin
        reqq_rbuf_clear_pending_install[ii] |= 1'b1;
        pipe_rbuf_clear_req_info_o.rbuf_clear_pending_install_hot[reqq_state_o[ii].rbuf_id]     |= 1'b1;
        pipe_rbuf_clear_req_info_o.rbuf_clear_pending_install_scp[reqq_state_o[ii].rbuf_id]     |= (reqq_state_o[ii].address_space == AddressScp);
        pipe_rbuf_clear_req_info_o.rbuf_clear_pending_install_address[reqq_state_o[ii].rbuf_id] |= reqq_state_o[ii].address;
      end
    end
  end

  // ══════════════════════════════════════════════════════════
  // Convert pipe responses to hot vectors
  // ══════════════════════════════════════════════════════════

  logic [NumEntries-1:0] pipe_tag_rsp_valid_1hot;
  logic [NumEntries-1:0] pipe_data_rsp_valid_1hot;

  always_comb begin
    pipe_tag_rsp_valid_1hot = '0;
    pipe_tag_rsp_valid_1hot[pipe_tag_rsp_info_i.reqq_id] = pipe_tag_rsp_valid_i;

    pipe_data_rsp_valid_1hot = '0;
    pipe_data_rsp_valid_1hot[pipe_data_rsp_info_i.reqq_id] = pipe_data_rsp_valid_i;
  end

  // ══════════════════════════════════════════════════════════
  // Pipeline early 1cp data response info for reqq
  // ══════════════════════════════════════════════════════════
  //  - Find the paired reqq_id for atomic hit to write partial.
  //  - Disable extraneous victim from being issued to the mesh.

  data_rsp_modifier_t pipe_data_rsp_modifier_info_next;

  always_comb begin
    pipe_data_rsp_modifier_info_next                           = data_rsp_modifier_t'('0);
    pipe_data_rsp_modifier_info_next.trans_id                  = pipe_data_rsp_pre_info_i.trans_id;
    pipe_data_rsp_modifier_info_next.reqq_id                   = pipe_data_rsp_pre_info_i.reqq_id;
    pipe_data_rsp_modifier_info_next.address_space             = reqq_state_o[pipe_data_rsp_pre_info_i.reqq_id].address_space;
    pipe_data_rsp_modifier_info_next.dataq_id                  = reqq_state_o[pipe_data_rsp_pre_info_i.reqq_id].dataq_id;
    pipe_data_rsp_modifier_info_next.which_mesh                = reqq_state_o[pipe_data_rsp_pre_info_i.reqq_id].which_mesh;
    pipe_data_rsp_modifier_info_next.reqq_source               = reqq_state_o[pipe_data_rsp_pre_info_i.reqq_id].reqq_source;
    pipe_data_rsp_modifier_info_next.port_id                   = reqq_state_o[pipe_data_rsp_pre_info_i.reqq_id].port_id;

    pipe_data_rsp_modifier_info_next.disable_mesh_bypass       = reqq_state_o[pipe_data_rsp_pre_info_i.reqq_id].valid &
      reqq_state_o[pipe_data_rsp_pre_info_i.reqq_id].dep_valid;
    pipe_data_rsp_modifier_info_next.redirect_normal_rsp_to_mesh =
      (reqq_state_o[pipe_data_rsp_pre_info_i.reqq_id].orig_opcode == OrigAtomic) &
      (reqq_state_o[pipe_data_rsp_pre_info_i.reqq_id].address_space inside {AddressL3, AddressScp});
    pipe_data_rsp_modifier_info_next.rsp_type                  = pipe_data_rsp_pre_info_i.rsp_type;
  end

  always_ff @(posedge clk_i) begin
    if (pipe_data_rsp_pre_valid_i) begin
      pipe_data_rsp_modifier_info_o <= pipe_data_rsp_modifier_info_next;
    end
  end

  // ══════════════════════════════════════════════════════════
  // Convert mesh response to hot vector
  // ══════════════════════════════════════════════════════════

  logic [NumEntries-1:0] to_l3_mesh_rsp_valid_1hot;
  logic                  to_l3_mesh_rsp_valid_any;

  always_comb begin
    to_l3_mesh_rsp_valid_1hot = '0;
    to_l3_mesh_rsp_valid_1hot[to_l3_mesh_rsp_info_i.reqq_id] = to_l3_mesh_rsp_valid_i;
    to_l3_mesh_rsp_valid_any  = |(to_l3_mesh_rsp_valid_i);
  end

  logic [NumEntries-1:0] to_sys_mesh_rsp_valid_1hot;
  logic                  to_sys_mesh_rsp_valid_any;

  always_comb begin
    to_sys_mesh_rsp_valid_1hot = '0;
    to_sys_mesh_rsp_valid_1hot[to_sys_mesh_rsp_info_i.reqq_id] = to_sys_mesh_rsp_valid_i;
    to_sys_mesh_rsp_valid_any  = |(to_sys_mesh_rsp_valid_i);
  end

  // ══════════════════════════════════════════════════════════
  // Arbitrate to pipeline
  // ══════════════════════════════════════════════════════════
  // Select an eligible entry to send to the pipeline.
  // Round robin for each sub-bank, then arbitrate for a single output
  // based upon sub-bank busy.

  logic [NumEntries-1:0] rbuf_req_picked_1hot;
  logic [NumEntries-1:0] pipe_req_eligible_hot;
  logic [NumEntries-1:0] pipe_req_picked_1hot_r0;
  reqq_pipe_req_t [NumEntries-1:0] pipe_req_info_all;

  // Round robin entries per subbank
  logic              [1:0] [SubBanks-1:0] [NumEntries-1:0] pipe_req_eligible_subbank_hot;
  logic              [1:0] [SubBanks-1:0] [NumEntries-1:0] pipe_req_grant_entry_per_subbank_1hot;

  reqq_pipe_req_t    [1:0] [SubBanks-1:0]                  pipe_req_info_per_subbank;
  logic              [1:0] [SubBanks-1:0]                  pipe_req_valid_per_subbank;
  logic              [1:0] [SubBanks-1:0]                  pipe_req_pop_per_subbank;

  reqq_pipe_req_t    [1:0]                                 pipe_req_info_per_lx;
  logic              [1:0]                                 pipe_req_valid_per_lx;
  logic              [1:0]                                 pipe_req_pop_per_lx;
  logic                                                    handling_a_priority_op;

  localparam int unsigned ArbL3 = 1;
  localparam int unsigned ArbL2 = 0;

  for (genvar lx = 0; lx < 2; lx++) begin : l2_l3
    for (genvar gen_it = 0; gen_it < SubBanks; gen_it++) begin : g_subbank
      // Round robin over all reqq entries to select a winner for each subbank.
      logic [NumEntries-1:0] reqq_allowed_entries;
      assign reqq_allowed_entries = (lx == ArbL2) ? reqq_l2_entries : reqq_l3_entries;

      always_comb begin
        for (integer ii = 0; ii < NumEntries; ii++) begin
          // eligible for this subbank if eligible and subbank address bits = this subbank
          pipe_req_eligible_subbank_hot[lx][gen_it][ii] =
            ~rbuf_req_picked_1hot[ii] & pipe_req_eligible_hot[ii] & reqq_allowed_entries[ii] &
            (reqq_state_o[ii].subbank == SubBankIdSize'(gen_it));
        end
      end

      prim_arb_rr #(
        .NumReqs  (NumEntries),
        .Width    ($bits(reqq_pipe_req_t))
      ) pipe_rr_subbanks (
        .clk_i    (clk_i),
        .rst_ni   (rst_n),
        .reqs_i   (pipe_req_eligible_subbank_hot[lx][gen_it]),
        .grants_o (pipe_req_grant_entry_per_subbank_1hot[lx][gen_it]),
        .pop_i    (pipe_req_pop_per_subbank[lx][gen_it] & ~handling_a_priority_op),
        .data_i   (pipe_req_info_all),
        .data_o   (pipe_req_info_per_subbank[lx][gen_it])
      );
      assign pipe_req_valid_per_subbank[lx][gen_it] = |(pipe_req_eligible_subbank_hot[lx][gen_it]);
    end

    // Round robin which subbank winner to choose taking into account busy
    logic [SubBanks-1:0] pipe_req_grant_subbank_1hot;
    prim_arb_rr #(
      .NumReqs  (SubBanks),
      .Width    ($bits(reqq_pipe_req_t))
    ) arb_rr (
      .clk_i    (clk_i),
      .rst_ni   (rst_n),
      .reqs_i   (pipe_req_valid_per_subbank[lx] & ~pipe_busy_i.sub_bank_busy),
      .grants_o (pipe_req_grant_subbank_1hot),
      .pop_i    (pipe_req_pop_per_lx[lx] & ~handling_a_priority_op),
      .data_i   (pipe_req_info_per_subbank[lx]),
      .data_o   (pipe_req_info_per_lx[lx])
    );
    assign pipe_req_pop_per_subbank[lx] = pipe_req_grant_subbank_1hot & {SubBanks{pipe_req_pop_per_lx[lx]}};
    assign pipe_req_valid_per_lx[lx]    = |(pipe_req_valid_per_subbank[lx] & ~pipe_busy_i.sub_bank_busy);
  end

  // L3 yield priority
  logic [L3YieldSize-1:0] l3_yield_priority;
  logic l3_yield_priority_en;
  assign l3_yield_priority_en = (esr_ctl_i.esr_sc_l3_yield_priority != L3YieldSize'(0));

  // l3 yield when we reach esr limit and there is l2 request available to go to pipe
  logic l3_yield_priority_valid;
  assign l3_yield_priority_valid =
    (esr_ctl_i.esr_sc_l3_yield_priority == l3_yield_priority) &
    pipe_req_valid_per_lx[ArbL2];

  // Clear l3 yield whenever l2 request wins arbitration
  logic l3_yield_priority_clear;
  assign l3_yield_priority_clear = pipe_req_pop_per_lx[ArbL2];

  // Update l3 yield if enabled and either l2 wins or l3 wins and esr limit is not reached
  logic l3_yield_priority_update;
  assign l3_yield_priority_update =
    l3_yield_priority_en &
    (pipe_req_pop_per_lx[ArbL2] |
     ((l3_yield_priority < esr_ctl_i.esr_sc_l3_yield_priority) & pipe_req_pop_per_lx[ArbL3]));

  logic [L3YieldSize-1:0] l3_yield_priority_next;
  assign l3_yield_priority_next =
    l3_yield_priority_clear ? L3YieldSize'(0)
                            : (l3_yield_priority + 1'b1);

  // l3 yield to l2 request when esr condition is met
  logic [1:0] l3_yield_priority_req;
  assign l3_yield_priority_req =
    l3_yield_priority_en ? {~l3_yield_priority_valid, 1'b1}
                         : 2'h3;

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n)
      l3_yield_priority <= '0;
    else if (l3_yield_priority_update)
      l3_yield_priority <= l3_yield_priority_next;
  end

  // Priority select L3 over L2.
  reqq_pipe_req_t pipe_req_info_unused;
  prim_arb_prio #(
    .NumClients (2),
    .Width      ($bits(reqq_pipe_req_t))
  ) pipe_arb (
    .clk_i    (clk_i),
    .rst_ni   (rst_n),
    .bid_i    (pipe_req_valid_per_lx & l3_yield_priority_req),
    .stall_i  (1'b0),
    .grant_o  (pipe_req_pop_per_lx),
    .data_i   (pipe_req_info_per_lx),
    .data_o   (pipe_req_info_unused)
  );
  assign pipe_start_o = |(pipe_req_valid_per_lx & l3_yield_priority_req);

  // Signal reqq inflight with the winning entry from the subbank that was picked
  always_comb begin
    pipe_req_picked_1hot_r0 = '0;
    for (integer ln = 0; ln < 2; ln++) begin
      for (integer ii = 0; ii < SubBanks; ii++) begin
        pipe_req_picked_1hot_r0 |=
          ({NumEntries{pipe_req_pop_per_subbank[ln][ii]}} & pipe_req_grant_entry_per_subbank_1hot[ln][ii]);
      end
    end
  end

  // Inline onehot_mux: select data_out from data_in based on grant decision
  always_comb begin
    pipe_req_info_o = reqq_pipe_req_t'('0);
    for (integer ii = 0; ii < NumEntries; ii++) begin
      if (pipe_req_picked_1hot_r0[ii]) begin
        pipe_req_info_o = pipe_req_info_all[ii];
      end
    end
  end

  // Register which entry is picked
  logic pipe_start_r1;
  logic [NumEntries-1:0] pipe_req_picked_1hot_r1;
  logic [ReqqIdSize-1:0] pipe_req_reqq_id_r1;

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n) begin
      pipe_req_picked_1hot_r1 <= '0;
      pipe_start_r1           <= '0;
    end else if (pipe_start_o | pipe_start_r1) begin
      pipe_req_picked_1hot_r1 <= pipe_req_picked_1hot_r0;
      pipe_start_r1           <= pipe_start_o;
    end
  end

  always_ff @(posedge clk_i) begin
    if (pipe_start_o)
      pipe_req_reqq_id_r1 <= pipe_req_info_o.reqq_id;
  end

  logic pipe_start_squashed_r1;
  assign pipe_start_squashed_r1 = ~pipe_squash_i & pipe_start_r1;

  // Squash the picked entry if the pipe can't take it.
  logic [NumEntries-1:0] pipe_req_picked_1hot_squashed_r1;
  assign pipe_req_picked_1hot_squashed_r1 = {NumEntries{~pipe_squash_i}} & pipe_req_picked_1hot_r1;

  // ══════════════════════════════════════════════════════════
  // Arbitrate to read buffer
  // ══════════════════════════════════════════════════════════

  logic [NumEntries-1:0] rbuf_req_eligible_hot;
  logic [NumEntries-1:0] rbuf_req_picked_1hot_pre;
  logic [NumEntries-1:0] rbuf_req_grant_1hot;
  rbuf_rd_req_t [NumEntries-1:0] rbuf_req_info_all;

  // Arbitrate across eligible entries
  prim_arb_rr #(
    .NumReqs  (NumEntries),
    .Width    ($bits(rbuf_rd_req_t))
  ) rbuf_rr (
    .clk_i    (clk_i),
    .rst_ni   (rst_n),
    .reqs_i   (rbuf_req_eligible_hot & ~rbuf_req_picked_1hot),
    .grants_o (rbuf_req_grant_1hot),
    .pop_i    (pipe_rbuf_rd_req_ready_i),
    .data_i   (rbuf_req_info_all),
    .data_o   (pipe_rbuf_rd_req_info_o)
  );

  logic pipe_rbuf_rd_req_clear;
  assign pipe_rbuf_rd_req_valid_o = |(rbuf_req_eligible_hot & ~rbuf_req_picked_1hot);
  assign rbuf_req_picked_1hot_pre = rbuf_req_grant_1hot & {NumEntries{pipe_rbuf_rd_req_ready_i}};

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n) begin
      rbuf_req_picked_1hot    <= '0;
      pipe_rbuf_rd_req_clear  <= '0;
    end else if (pipe_rbuf_rd_req_valid_o | pipe_rbuf_rd_req_clear) begin
      rbuf_req_picked_1hot    <= rbuf_req_picked_1hot_pre;
      pipe_rbuf_rd_req_clear  <= pipe_rbuf_rd_req_valid_o;
    end
  end

  // ══════════════════════════════════════════════════════════
  // Arbitrate to_l3 mesh
  // ══════════════════════════════════════════════════════════

  logic [NumEntries-1:0]             to_l3_mesh_clk_en_all;
  logic [NumEntries-1:0]             to_l3_mesh_req_eligible_hot;
  logic [NumEntries-1:0]             to_l3_mesh_req_picked_1hot;
  logic [NumEntries-1:0]             to_l3_mesh_req_picked_1hot_pre;
  logic [NumEntries-1:0]             to_l3_mesh_req_grant_1hot;
  reqq_mesh_req_t [NumEntries-1:0]   to_l3_mesh_req_info_all;
  reqq_mesh_req_t                    to_l3_mesh_req_info_pre;
  reqq_mesh_req_t                    to_l3_mesh_req_info;
  logic                              to_l3_mesh_req_ready;
  logic                              to_l3_mesh_req_fifo_full;
  logic                              to_l3_mesh_req_valid;
  logic                              to_l3_mesh_req_valid_pre;

  // Arbitrate across eligible entries
  prim_arb_rr #(
    .NumReqs  (NumEntries),
    .Width    ($bits(reqq_mesh_req_t))
  ) to_l3_rr (
    .clk_i    (clk_i),
    .rst_ni   (rst_n),
    .reqs_i   (to_l3_mesh_req_eligible_hot & ~to_l3_mesh_req_picked_1hot),
    .grants_o (to_l3_mesh_req_grant_1hot),
    .pop_i    (~to_l3_mesh_req_fifo_full),
    .data_i   (to_l3_mesh_req_info_all),
    .data_o   (to_l3_mesh_req_info_pre)
  );

  logic [2:0] to_l3_mesh_req_fifo_level_unused;
  prim_fifo_reg #(
    .Width ($bits(reqq_mesh_req_t)),
    .Depth (2)
  ) to_l3_mesh_fifo (
    .clk_i   (clk_i),
    .rst_ni  (rst_n),
    .push_i  (to_l3_mesh_req_valid_pre),
    .wdata_i (to_l3_mesh_req_info_pre),
    .full_o  (to_l3_mesh_req_fifo_full),
    .level_o (to_l3_mesh_req_fifo_level_unused),
    .pop_i   (to_l3_mesh_req_ready),
    .valid_o (to_l3_mesh_req_valid),
    .rdata_o (to_l3_mesh_req_info)
  );

  // Route the request to either the mesh if no data or dataq if data needs to be read.
  assign to_l3_mesh_req_valid_pre           = |(to_l3_mesh_req_eligible_hot & ~to_l3_mesh_req_picked_1hot);
  assign reqq_to_l3_mesh_req_valid_nodata_o = to_l3_mesh_req_valid & ~to_l3_mesh_req_info.with_data;
  assign reqq_to_l3_mesh_req_valid_wdata_o  = to_l3_mesh_req_valid &  to_l3_mesh_req_info.with_data;

  always_comb begin
    reqq_to_l3_mesh_req_info_o      = to_l3_mesh_req_info;
    reqq_to_l3_mesh_req_info_o.qwen = reqq_state_o[to_l3_mesh_req_info.dataq_id].qwen;
  end

  assign to_l3_mesh_req_ready = (to_l3_mesh_req_info.with_data)
    ? reqq_to_l3_mesh_req_ready_wdata_i : reqq_to_l3_mesh_req_ready_nodata_i;

  assign to_l3_mesh_req_picked_1hot_pre = to_l3_mesh_req_grant_1hot & {NumEntries{~to_l3_mesh_req_fifo_full}};

  logic to_l3_mesh_req_picked_valid_d0;
  logic to_l3_mesh_req_picked_valid_d1;
  logic to_l3_mesh_req_picked_valid;
  assign to_l3_mesh_req_picked_valid_d0 = (|to_l3_mesh_req_eligible_hot) & ~to_l3_mesh_req_fifo_full;
  assign to_l3_mesh_req_picked_valid_d1 = (|to_l3_mesh_req_picked_1hot);
  assign to_l3_mesh_req_picked_valid    = to_l3_mesh_req_picked_valid_d0 | to_l3_mesh_req_picked_valid_d1;

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n)
      to_l3_mesh_req_picked_1hot <= '0;
    else if (to_l3_mesh_req_picked_valid)
      to_l3_mesh_req_picked_1hot <= to_l3_mesh_req_picked_1hot_pre;
  end

  /* verilator lint_off SYNCASYNCNET */  // rst_n used as sync clock-enable for mesh master
  always_ff @(posedge clk_i) begin
    to_l3_mesh_master_bank_clk_en_o <=
      |to_l3_mesh_clk_en_all | ~rst_n | esr_ctl_i.esr_sc_clk_gate_disable[CgateDisMeshMaster];
  end
  /* verilator lint_on SYNCASYNCNET */

  // ══════════════════════════════════════════════════════════
  // Arbitrate to_sys mesh
  // ══════════════════════════════════════════════════════════

  logic [NumEntries-1:0]             to_sys_mesh_clk_en_all;
  logic [NumEntries-1:0]             to_sys_mesh_req_eligible_hot;
  logic [NumEntries-1:0]             to_sys_mesh_req_picked_1hot;
  logic [NumEntries-1:0]             to_sys_mesh_req_picked_1hot_pre;
  logic [NumEntries-1:0]             to_sys_mesh_req_grant_1hot;
  reqq_mesh_req_t [NumEntries-1:0]   to_sys_mesh_req_info_all;
  reqq_mesh_req_t                    to_sys_mesh_req_info_pre;
  reqq_mesh_req_t                    to_sys_mesh_req_info;
  logic                              to_sys_mesh_req_ready;
  logic                              to_sys_mesh_req_fifo_full;
  logic                              to_sys_mesh_req_valid;
  logic                              to_sys_mesh_req_valid_pre;

  // Arbitrate across eligible entries
  prim_arb_rr #(
    .NumReqs  (NumEntries),
    .Width    ($bits(reqq_mesh_req_t))
  ) to_sys_rr (
    .clk_i    (clk_i),
    .rst_ni   (rst_n),
    .reqs_i   (to_sys_mesh_req_eligible_hot & ~to_sys_mesh_req_picked_1hot),
    .grants_o (to_sys_mesh_req_grant_1hot),
    .pop_i    (~to_sys_mesh_req_fifo_full),
    .data_i   (to_sys_mesh_req_info_all),
    .data_o   (to_sys_mesh_req_info_pre)
  );

  logic [2:0] to_sys_mesh_req_fifo_level_unused;
  prim_fifo_reg #(
    .Width ($bits(reqq_mesh_req_t)),
    .Depth (2)
  ) to_sys_mesh_fifo (
    .clk_i   (clk_i),
    .rst_ni  (rst_n),
    .push_i  (to_sys_mesh_req_valid_pre),
    .wdata_i (to_sys_mesh_req_info_pre),
    .full_o  (to_sys_mesh_req_fifo_full),
    .level_o (to_sys_mesh_req_fifo_level_unused),
    .pop_i   (to_sys_mesh_req_ready),
    .valid_o (to_sys_mesh_req_valid),
    .rdata_o (to_sys_mesh_req_info)
  );

  // Route the request to either the mesh if no data or dataq if data needs to be read.
  assign to_sys_mesh_req_valid_pre           = |(to_sys_mesh_req_eligible_hot & ~to_sys_mesh_req_picked_1hot);
  assign reqq_to_sys_mesh_req_valid_nodata_o = to_sys_mesh_req_valid & ~to_sys_mesh_req_info.with_data;
  assign reqq_to_sys_mesh_req_valid_wdata_o  = to_sys_mesh_req_valid &  to_sys_mesh_req_info.with_data;

  always_comb begin
    reqq_to_sys_mesh_req_info_o      = to_sys_mesh_req_info;
    reqq_to_sys_mesh_req_info_o.qwen = reqq_state_o[to_sys_mesh_req_info.dataq_id].qwen;
  end

  assign to_sys_mesh_req_ready = (to_sys_mesh_req_info.with_data)
    ? reqq_to_sys_mesh_req_ready_wdata_i : reqq_to_sys_mesh_req_ready_nodata_i;

  // Signal picked to the entry to set inflight.
  assign to_sys_mesh_req_picked_1hot_pre = to_sys_mesh_req_grant_1hot & {NumEntries{~to_sys_mesh_req_fifo_full}};

  logic to_sys_mesh_req_picked_valid_d0;
  logic to_sys_mesh_req_picked_valid_d1;
  logic to_sys_mesh_req_picked_valid;
  assign to_sys_mesh_req_picked_valid_d0 = (|to_sys_mesh_req_eligible_hot) & ~to_sys_mesh_req_fifo_full;
  assign to_sys_mesh_req_picked_valid_d1 = (|to_sys_mesh_req_picked_1hot);
  assign to_sys_mesh_req_picked_valid    = to_sys_mesh_req_picked_valid_d0 | to_sys_mesh_req_picked_valid_d1;

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n)
      to_sys_mesh_req_picked_1hot <= '0;
    else if (to_sys_mesh_req_picked_valid)
      to_sys_mesh_req_picked_1hot <= to_sys_mesh_req_picked_1hot_pre;
  end

  /* verilator lint_off SYNCASYNCNET */  // rst_n used as sync clock-enable for sys mesh master
  always_ff @(posedge clk_i) begin
    to_sys_mesh_master_bank_clk_en_o <=
      |to_sys_mesh_clk_en_all | ~rst_n | esr_ctl_i.esr_sc_clk_gate_disable[CgateDisMeshMaster];
  end
  /* verilator lint_on SYNCASYNCNET */

  // ══════════════════════════════════════════════════════════
  // Arbitrate to L2 rsp back to neighborhoods
  // ══════════════════════════════════════════════════════════

  logic [NumEntries-1:0]             rspmux_l2_eligible_hot;
  logic [NumEntries-1:0]             rspmux_l2_picked_1hot;
  logic [NumEntries-1:0]             rspmux_l2_grant_1hot;
  rspmux_t [NumEntries-1:0]          rspmux_l2_info_all;
  rspmux_t                           rspmux_l2_info_pre;
  logic                              reqq_rspmux_l2_ready;
  logic                              reqq_rspmux_l2_fifo_full;
  logic                              reqq_rspmux_l2_valid;
  logic                              reqq_rspmux_l2_valid_pre;

  // Arbitrate across eligible entries
  prim_arb_rr #(
    .NumReqs  (NumEntries),
    .Width    ($bits(rspmux_t))
  ) rspmux_l2_rr (
    .clk_i    (clk_i),
    .rst_ni   (rst_n),
    .reqs_i   (rspmux_l2_eligible_hot & ~rspmux_l2_picked_1hot),
    .grants_o (rspmux_l2_grant_1hot),
    .pop_i    (~reqq_rspmux_l2_fifo_full),
    .data_i   (rspmux_l2_info_all),
    .data_o   (rspmux_l2_info_pre)
  );

  logic [2:0] reqq_rspmux_l2_fifo_level_unused;
  prim_fifo_reg #(
    .Width ($bits(rspmux_t)),
    .Depth (2)
  ) reqq_rspmux_l2_fifo (
    .clk_i   (clk_i),
    .rst_ni  (rst_n),
    .push_i  (reqq_rspmux_l2_valid_pre),
    .wdata_i (rspmux_l2_info_pre),
    .full_o  (reqq_rspmux_l2_fifo_full),
    .level_o (reqq_rspmux_l2_fifo_level_unused),
    .pop_i   (reqq_rspmux_l2_ready),
    .valid_o (reqq_rspmux_l2_valid),
    .rdata_o (reqq_rspmux_l2_info_o)
  );

  // Route the request to either the rspmux if no data or dataq if data needs to be read.
  assign reqq_rspmux_l2_valid_pre       = |(rspmux_l2_eligible_hot & ~rspmux_l2_picked_1hot);
  logic  reqq_rspmux_l2_to_dataq;
  assign reqq_rspmux_l2_to_dataq        = reqq_rspmux_l2_info_o.with_data & ~reqq_rspmux_l2_info_o.zero_data;
  assign reqq_rspmux_l2_valid_nodata_o  = reqq_rspmux_l2_valid & ~reqq_rspmux_l2_to_dataq;
  assign reqq_rspmux_l2_valid_wdata_o   = reqq_rspmux_l2_valid &  reqq_rspmux_l2_to_dataq;

  assign reqq_rspmux_l2_ready = (reqq_rspmux_l2_to_dataq)
    ? reqq_rspmux_l2_ready_wdata_i : reqq_rspmux_l2_ready_nodata_i;

  // Signal picked to the entry to clr eligible
  logic rspmux_l2_req_clear;
  logic [NumEntries-1:0] rspmux_l2_picked_1hot_pre;
  assign rspmux_l2_picked_1hot_pre = rspmux_l2_grant_1hot & {NumEntries{~reqq_rspmux_l2_fifo_full}};

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n) begin
      rspmux_l2_picked_1hot  <= '0;
      rspmux_l2_req_clear    <= '0;
    end else if (reqq_rspmux_l2_valid_pre | rspmux_l2_req_clear) begin
      rspmux_l2_picked_1hot  <= rspmux_l2_picked_1hot_pre;
      rspmux_l2_req_clear    <= reqq_rspmux_l2_valid_pre;
    end
  end

  // ══════════════════════════════════════════════════════════
  // Arbitrate to L3 rsp back to L3 slave
  // ══════════════════════════════════════════════════════════

  logic [NumEntries-1:0]             rspmux_l3_eligible_hot;
  logic [NumEntries-1:0]             rspmux_l3_picked_1hot;
  logic [NumEntries-1:0]             rspmux_l3_grant_1hot;
  rspmux_t [NumEntries-1:0]          rspmux_l3_info_all;
  rspmux_t                           rspmux_l3_info_pre;
  logic                              reqq_rspmux_l3_ready;
  logic                              reqq_rspmux_l3_fifo_full;
  logic                              reqq_rspmux_l3_valid;
  logic                              reqq_rspmux_l3_valid_pre;

  // Arbitrate across eligible entries
  prim_arb_rr #(
    .NumReqs  (NumEntries),
    .Width    ($bits(rspmux_t))
  ) rspmux_l3_rr (
    .clk_i    (clk_i),
    .rst_ni   (rst_n),
    .reqs_i   (rspmux_l3_eligible_hot & ~rspmux_l3_picked_1hot),
    .grants_o (rspmux_l3_grant_1hot),
    .pop_i    (~reqq_rspmux_l3_fifo_full),
    .data_i   (rspmux_l3_info_all),
    .data_o   (rspmux_l3_info_pre)
  );

  logic [2:0] reqq_rspmux_l3_fifo_level_unused;
  prim_fifo_reg #(
    .Width ($bits(rspmux_t)),
    .Depth (2)
  ) reqq_rspmux_l3_fifo (
    .clk_i   (clk_i),
    .rst_ni  (rst_n),
    .push_i  (reqq_rspmux_l3_valid_pre),
    .wdata_i (rspmux_l3_info_pre),
    .full_o  (reqq_rspmux_l3_fifo_full),
    .level_o (reqq_rspmux_l3_fifo_level_unused),
    .pop_i   (reqq_rspmux_l3_ready),
    .valid_o (reqq_rspmux_l3_valid),
    .rdata_o (reqq_rspmux_l3_info_o)
  );

  // Route the request to either the rspmux if no data or dataq if data needs to be read.
  assign reqq_rspmux_l3_valid_pre       = |(rspmux_l3_eligible_hot & ~rspmux_l3_picked_1hot);
  logic  reqq_rspmux_l3_to_dataq;
  assign reqq_rspmux_l3_to_dataq        = reqq_rspmux_l3_info_o.with_data & ~reqq_rspmux_l3_info_o.zero_data;
  assign reqq_rspmux_l3_valid_nodata_o  = reqq_rspmux_l3_valid & ~reqq_rspmux_l3_to_dataq;
  assign reqq_rspmux_l3_valid_wdata_o   = reqq_rspmux_l3_valid &  reqq_rspmux_l3_to_dataq;

  assign reqq_rspmux_l3_ready = (reqq_rspmux_l3_to_dataq)
    ? reqq_rspmux_l3_ready_wdata_i : reqq_rspmux_l3_ready_nodata_i;

  // Signal picked to the entry to clr eligible
  logic rspmux_l3_req_clear;
  logic [NumEntries-1:0] rspmux_l3_picked_1hot_pre;
  assign rspmux_l3_picked_1hot_pre = rspmux_l3_grant_1hot & {NumEntries{~reqq_rspmux_l3_fifo_full}};

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n) begin
      rspmux_l3_picked_1hot  <= '0;
      rspmux_l3_req_clear    <= '0;
    end else if (reqq_rspmux_l3_valid_pre | rspmux_l3_req_clear) begin
      rspmux_l3_picked_1hot  <= rspmux_l3_picked_1hot_pre;
      rspmux_l3_req_clear    <= reqq_rspmux_l3_valid_pre;
    end
  end

  // ══════════════════════════════════════════════════════════
  // Pipe priority changes
  // ══════════════════════════════════════════════════════════
  // Atomic_2nd_pass follows a preceding atomic fill and needs priority to the pipeline
  // to ensure no livelock.
  // WritePartial_2nd_pass follows a preceding writePartial fill and needs priority.

  logic [NumEntries-1:0] reqq_2nd_pass_hot;
  logic                  reqq_2nd_pass_need_priority;

  always_comb begin
    for (int ii = 0; ii < NumEntries; ii++) begin
      reqq_2nd_pass_hot[ii] = reqq_entry_valid[ii] &
        // Currently being picked and will cause a 2nd_pass to become eligible
        ((pipe_req_picked_1hot_squashed_r1[ii] &
          (reqq_state_o[ii].orig_opcode inside {OrigAtomic2, OrigWritePartial2}) &
          (reqq_state_o[ii].opcode inside {L2Fill, L3Fill})) |
        // 2nd_pass is eligible
        (reqq_state_o[ii].opcode inside {
          L2Atomic2nd,
          L3Atomic2nd,
          L2WritePartial2nd,
          L3WritePartial2nd}));
    end
    reqq_2nd_pass_need_priority = |reqq_2nd_pass_hot;
  end

  // Idx Reads and Write need priority to the pipeline
  // to ensure they finish before the next Debug issued Read or Write.
  logic [NumEntries-1:0] reqq_idx_cop_hot;
  logic                  reqq_idx_cop_need_priority;
  always_comb begin
    for (int ii = 0; ii < NumEntries; ii++) begin
      reqq_idx_cop_hot[ii] = reqq_entry_valid[ii] &
                              (reqq_state_o[ii].opcode inside {IdxRead, IdxWrite});
    end
    // Atomic 2nd_pass takes priority over idx read/write.
    reqq_idx_cop_need_priority = (|reqq_idx_cop_hot) & ~reqq_2nd_pass_need_priority;
  end

  // If we've giving a reqq entry priority and squelching all other entries, don't update the rr pointer.
  assign handling_a_priority_op = reqq_2nd_pass_need_priority | reqq_idx_cop_need_priority;

  // ══════════════════════════════════════════════════════════
  // Insert victims into linked lists
  // ══════════════════════════════════════════════════════════
  // If a new victim comes in, check to see it if matches any victims ahead
  // of it and put the new victim at the tail.
  // Note that victims are capacity victims or self-evict (read hit partial).

  logic tag_victim;
  assign tag_victim = pipe_tag_rsp_valid_i & pipe_tag_rsp_info_i.victim;

  logic [NumEntries-1:0] tag_victim_address_match;
  logic [NumEntries-1:0] tag_self_victim_hot;
  logic [NumEntries-1:0] tag_victim_match_meshll_tail;
  logic [NumEntries-1:0] tag_victim_match_non_meshll;

  always_comb begin
    for (int ii = 0; ii < NumEntries; ii++) begin
      // All entries that match the victim, not including the self evict entry.
      tag_victim_address_match[ii] = reqq_entry_valid[ii] &
        (reqq_state_o[ii].address_space == pipe_tag_rsp_info_i.victim_address_space) &
        tag_victim & ~pipe_tag_rsp_valid_1hot[ii] &
        masked_address_compare(pipe_tag_rsp_info_i.victim_address,
                               reqq_state_o[ii].address,
                               pipe_tag_rsp_info_i.victim_address_space);

      // The self evict entry.
      tag_self_victim_hot[ii] = tag_victim &
        pipe_tag_rsp_info_i.hit & pipe_tag_rsp_valid_1hot[ii];

      // matching address against an entry already in meshll
      tag_victim_match_meshll_tail[ii] = tag_victim_address_match[ii] &
        (~reqq_state_o[ii].dont_depend_on_me & ~clear_dep_hot[ii]) &
          reqq_state_o[ii].dep_meshll_tail;

      // matching address against an entry not in meshll
      tag_victim_match_non_meshll[ii] = tag_victim_address_match[ii] &
        (~reqq_state_o[ii].dont_depend_on_me & ~clear_dep_hot[ii]) &
         ~reqq_state_o[ii].dep_in_meshll;
    end
  end

  // Inline hot2bin for tag_victim_dep_id
  logic [NumEntriesId-1:0] tag_victim_dep_id;
  always_comb begin
    tag_victim_dep_id = '0;
    for (int ii = 0; ii < NumEntries; ii++) begin
      if (tag_victim_match_meshll_tail[ii])
        tag_victim_dep_id = NumEntriesId'(ii);
    end
  end

  // New victim is dependent upon another if there was a match ahead of it.
  logic tag_victim_dep_valid;
  assign tag_victim_dep_valid = |tag_victim_match_meshll_tail;

  // Victim is tail of total linked list if no matches behind it and not alloc match.
  logic tag_victim_dep_tail;
  assign tag_victim_dep_tail = ~|{tag_victim_match_non_meshll, alloc_match_tag_victim_ag};

  // Tell existing victims that new victim is behind them.
  logic [NumEntries-1:0] tag_victim_insert_behind_me;
  assign tag_victim_insert_behind_me = tag_victim_match_meshll_tail;

  // Tell non_victim_head that new victim is ahead of them.
  logic [NumEntries-1:0] tag_victim_insert_ahead_of_me;
  always_comb begin
    for (int ii = 0; ii < NumEntries; ii++) begin
      // Insert in front of me if I'm the head, or I'm becoming the head.
      // There is no insert_ahead_of_me if it is a self victim.
      tag_victim_insert_ahead_of_me[ii] = tag_victim_match_non_meshll[ii] &
        (reqq_state_o[ii].dep_non_meshll_head | clear_dep_hot[reqq_state_o[ii].dep_id]);
    end
  end

  // self_victim_still_dependable for assertion context (used in unused_ok)
  logic self_victim_still_dependable;
  assign self_victim_still_dependable =
    ((pipe_tag_rsp_info_i.hit & pipe_tag_rsp_info_i.victim) |
     ((reqq_state_o[pipe_tag_rsp_info_i.reqq_id].orig_opcode == OrigWriteAround) &
      (reqq_state_o[pipe_tag_rsp_info_i.reqq_id].qwen == {LineQwSize{1'b1}}) &
      (pipe_tag_rsp_info_i.victim))) &
     reqq_state_o[pipe_tag_rsp_info_i.reqq_id].dep_non_meshll_head &
    ~reqq_state_o[pipe_tag_rsp_info_i.reqq_id].dont_depend_on_me;

  // ══════════════════════════════════════════════════════════
  // Upgrade head of linked list destined to mesh to meshll
  // ══════════════════════════════════════════════════════════

  logic [NumEntries-1:0] upgrade_ahead_of_me_to_meshll;

  // If entry is the head of the linked list, headed to mesh then upgrade me to meshll.
  // If self-victim, then it is also being upgraded to meshll.
  always_comb begin
    for (int ii = 0; ii < NumEntries; ii++) begin
      upgrade_to_meshll_hot[ii] =
         reqq_state_o[ii].valid &
        ~reqq_state_o[ii].dep_valid &
        ~reqq_state_o[ii].dep_in_meshll &
         reqq_state_o[ii].opcode_destined_for_mesh;
    end
  end

  always_comb begin
    for (int ii = 0; ii < NumEntries; ii++) begin
      alloc_upgrade_to_meshll_hot[ii] =
        tag_self_victim_hot[ii] |
        upgrade_to_meshll_hot[ii];

      upgrade_ahead_of_me_to_meshll[ii] =
        reqq_state_o[ii].valid &
        reqq_state_o[ii].dep_valid &
        (upgrade_to_meshll_hot[reqq_state_o[ii].dep_id] |
         tag_self_victim_hot[reqq_state_o[ii].dep_id]);
    end
  end

  // There is a simultaneous tag_victim and upgrade if the victim is not a self_victim
  // and the upgrade matches the victim address.
  logic tag_victim_upgrade_behind_me;
  assign tag_victim_upgrade_behind_me =
    |(upgrade_to_meshll_hot & tag_victim_address_match);

  // ══════════════════════════════════════════════════════════
  // Append coalescing buffer evictions to linked lists
  // ══════════════════════════════════════════════════════════

  logic coalesce_victim;
  assign coalesce_victim = pipe_cbuf_rsp_valid_i & pipe_cbuf_rsp_info_i.victim;

  cbuf_rsp_t pipe_cbuf_rsp_info_fixup;
  logic coalesce_victim_matches_tag_victim;
  assign coalesce_victim_matches_tag_victim =
    pipe_tag_rsp_valid_i & pipe_tag_rsp_info_i.victim &
    pipe_cbuf_rsp_valid_i & pipe_cbuf_rsp_info_i.victim &
    (pipe_tag_rsp_info_i.victim_address_space == AddressL2) &
    masked_address_compare(pipe_cbuf_rsp_info_i.victim_address,
                           pipe_tag_rsp_info_i.victim_address,
                           pipe_tag_rsp_info_i.victim_address_space);

  always_comb begin
    pipe_cbuf_rsp_info_fixup = pipe_cbuf_rsp_info_i;
    if (coalesce_victim_matches_tag_victim) begin
      pipe_cbuf_rsp_info_fixup.victim = 1'b0;
    end
  end

  // Which entry is becoming a coalescing victim
  logic coalesce_victim_fixup;
  assign coalesce_victim_fixup = pipe_cbuf_rsp_valid_i & pipe_cbuf_rsp_info_fixup.victim;

  logic [NumEntries-1:0] coalesce_victim_hot;
  always_comb begin
    coalesce_victim_hot = '0;
    coalesce_victim_hot[reqq_state_o[pipe_cbuf_rsp_info_fixup.reqq_id].paired_id] |= coalesce_victim_fixup;
  end

  // Entries that match the cbuf
  logic [NumEntries-1:0] cbuf_victim_address_match_hot;
  logic [NumEntries-1:0] cbuf_victim_address_match_tail_1hot;
  always_comb begin
    for (int ii = 0; ii < NumEntries; ii++) begin
      // All entries that match the victim
      cbuf_victim_address_match_hot[ii] = reqq_entry_valid[ii] & reqq_l2_entries[ii] &
        (reqq_state_o[ii].address_space == AddressL2) &
        (~reqq_state_o[ii].dont_depend_on_me & ~clear_dep_hot[ii]) &
        pipe_cbuf_rsp_valid_i &
        pipe_cbuf_rsp_info_fixup.victim &
        masked_address_compare(pipe_cbuf_rsp_info_i.victim_address,
                               reqq_state_o[ii].address, AddressL2);

      cbuf_victim_address_match_tail_1hot[ii] = cbuf_victim_address_match_hot[ii] & reqq_state_o[ii].dep_tail;
    end
  end

  // Inline hot2bin for cbuf_address_match_tail_id
  logic [NumEntriesId-1:0] cbuf_address_match_tail_id;
  always_comb begin
    cbuf_address_match_tail_id = '0;
    for (int ii = 0; ii < NumEntries; ii++) begin
      if (cbuf_victim_address_match_tail_1hot[ii])
        cbuf_address_match_tail_id = NumEntriesId'(ii);
    end
  end

  // Check if also match ag
  // If coalesce matches alloc, it goes behind alloc
  assign coalesce_victim_match_alloc_ag =
    (alloc_l2_info_ag_o.address_space == AddressL2) &
    alloc_l2_valid_ag_o &
    ~alloc_l2_info_ag_o.err_valid &
    ~(alloc_l2_info_ag_o.orig_opcode inside {OrigAtomic2, OrigWriteAround2, OrigWritePartial2}) &
    pipe_cbuf_rsp_valid_i & pipe_cbuf_rsp_info_fixup.victim &
    masked_address_compare(alloc_l2_info_ag_o.address,
                           pipe_cbuf_rsp_info_i.victim_address, AddressL2);

  logic coalesce_victim_dep_valid;
  assign coalesce_victim_dep_valid = |{cbuf_victim_address_match_hot, coalesce_victim_match_alloc_ag};

  logic [NumEntriesId-1:0] coalesce_victim_dep_id;
  assign coalesce_victim_dep_id = (coalesce_victim_match_alloc_ag)
    ? alloc_l2_info_ag_o.reqq_id : cbuf_address_match_tail_id;

  always_comb begin
    for (int ii = 0; ii < NumEntries; ii++) begin
      entry_is_non_meshll[ii] = ~reqq_state_o[ii].dep_in_meshll;
    end
  end

  // non_meshll_head if attaching to mesh_ll and there isn't a simultaneous alloc ahead of us.
  logic coalesce_victim_match_non_meshll;
  assign coalesce_victim_match_non_meshll =
    |(cbuf_victim_address_match_hot & (entry_is_non_meshll & ~upgrade_to_meshll_hot));

  logic coalesce_victim_dep_non_meshll_head;
  assign coalesce_victim_dep_non_meshll_head =
    ~coalesce_victim_match_non_meshll & ~coalesce_victim_match_alloc_ag;

  // Tell existing tail that new victim is behind them.
  logic [NumEntries-1:0] coalesce_victim_insert_behind_me;
  assign coalesce_victim_insert_behind_me = cbuf_victim_address_match_tail_1hot;

  // ══════════════════════════════════════════════════════════
  // Instantiate all of the entries
  // ══════════════════════════════════════════════════════════

  logic           [NumEntries-1:0] alloc_me_1hot_ag;
  reqq_alloc_t    [NumEntries-1:0] alloc_me_info_ag;

  for (genvar gen_it = 0; gen_it < NumEntries; gen_it++) begin : entries

    always_comb begin
      alloc_me_1hot_ag[gen_it] = reqq_l2_entries[gen_it]
        ? (alloc_l2_1hot_paired_ag[gen_it] | alloc_l2_1hot_ag[gen_it])
        : (alloc_l3_1hot_paired_ag[gen_it] | alloc_l3_1hot_ag[gen_it]);

      alloc_me_info_ag[gen_it] = reqq_l2_entries[gen_it]
        ? (alloc_l2_1hot_ag[gen_it] ? alloc_l2_info_ag_o : alloc_l2_info_paired_ag)
        : (alloc_l3_1hot_ag[gen_it] ? alloc_l3_info_ag_o : alloc_l3_info_paired_ag);
    end

    shirecache_bank_reqq_entry #(
      .NumEntries (NumEntries)
    ) entry (
      .clk_i                               (clk_i),
      .rst_ni                              (rst_n),
      .my_reqq_id_i                        (NumEntriesId'(gen_it)),

      .alloc_me_ag_i                       (alloc_me_1hot_ag[gen_it]),
      .alloc_me_ad_i                       ((reqq_l2_entries[gen_it]) ? alloc_l2_1hot_ad[gen_it] : alloc_l3_1hot_ad[gen_it]),
      .alloc_info_ag_i                     (alloc_me_info_ag[gen_it]),
      .alloc_any_ag_i                      ((reqq_l2_entries[gen_it]) ? alloc_l2_valid_ag_o : alloc_l3_valid_ag_o),
      .alloc_any_info_ag_i                 ((reqq_l2_entries[gen_it]) ? alloc_l2_info_ag_o  : alloc_l3_info_ag_o),

      // The coalescing victim matched this alloc
      .coalesce_victim_match_alloc_ag_i    ((reqq_l2_entries[gen_it]) ? coalesce_victim_match_alloc_ag : 1'b0),

      .clear_dep_behind_me_o               (clear_dep_hot[gen_it]),
      .clear_dep_hot_i                     (clear_dep_hot),
      .clear_dep_rbuf_1hot_pre_i           (rbuf_req_picked_1hot_pre),
      .tag_victim_dep_valid_i              (tag_victim_dep_valid),
      .tag_victim_dep_id_i                 (tag_victim_dep_id),
      .tag_victim_dep_tail_i               (tag_victim_dep_tail),
      .tag_victim_insert_behind_me_i       (tag_victim_insert_behind_me[gen_it]),
      .tag_victim_insert_ahead_of_me_i     (tag_victim_insert_ahead_of_me[gen_it]),
      .tag_victim_address_match_i          (tag_victim_address_match[gen_it]),
      .tag_victim_upgrade_behind_me_i      (tag_victim_upgrade_behind_me),
      .upgrade_me_to_meshll_i              (upgrade_to_meshll_hot[gen_it]),
      .upgrade_ahead_of_me_to_meshll_i     (upgrade_ahead_of_me_to_meshll[gen_it]),

      .coalesce_victim_me_i                (coalesce_victim_hot[gen_it]),
      .coalesce_victim_dep_valid_i         (coalesce_victim_dep_valid),
      .coalesce_victim_dep_id_i            (coalesce_victim_dep_id),
      .coalesce_victim_dep_non_meshll_head_i (coalesce_victim_dep_non_meshll_head),
      .coalesce_victim_insert_behind_me_i  (coalesce_victim_insert_behind_me[gen_it]),

      .dataq_write_status_any_i            (dataq_write_status_any_i),
      .dataq_write_status_me_i             (dataq_write_status_hot_i[gen_it]),
      .dataq_write_status_id_i             (dataq_write_status_id_i),
      .dataq_read_status_me_i              (dataq_read_status_hot_i[gen_it]),

      // Interfaces with the pipeline
      .pipe_req_eligible_o                 (pipe_req_eligible_hot[gen_it]),
      .pipe_req_info_me_o                  (pipe_req_info_all[gen_it]),
      .pipe_req_picked_me_i                (pipe_req_picked_1hot_squashed_r1[gen_it]),
      .pipe_req_reqq_id_any_i              (pipe_req_reqq_id_r1),
      .pipe_req_picked_any_i               (pipe_start_squashed_r1),

      .pipe_tag_rsp_valid_any_i            (pipe_tag_rsp_valid_i),
      .pipe_tag_rsp_valid_me_i             (pipe_tag_rsp_valid_1hot[gen_it]),
      .pipe_tag_rsp_info_i                 (pipe_tag_rsp_info_i),
      .pipe_data_rsp_valid_me_i            (pipe_data_rsp_valid_1hot[gen_it]),
      .pipe_data_rsp_info_i                (pipe_data_rsp_info_i),
      .pipe_cbuf_rsp_valid_any_i           (pipe_cbuf_rsp_valid_i),
      .pipe_cbuf_rsp_info_i                (pipe_cbuf_rsp_info_fixup),
      .mesh_bypassed_victim_i              (mesh_bypassed_victim_i),
      .rspmux_bypassed_data_rsp_i          (rspmux_bypassed_data_rsp_i),

      // Interfaces with the rbuf
      .rbuf_req_eligible_o                 (rbuf_req_eligible_hot[gen_it]),
      .rbuf_req_info_me_o                  (rbuf_req_info_all[gen_it]),
      .rbuf_req_picked_me_i                (rbuf_req_picked_1hot[gen_it]),
      .pipe_rbuf_state_i                   (pipe_rbuf_state_i),
      .pipe_rbuf_clear_req_info_i          (pipe_rbuf_clear_req_info_o),

      // Interfaces with mesh
      .to_l3_mesh_clk_en_o                (to_l3_mesh_clk_en_all[gen_it]),
      .to_l3_mesh_req_eligible_o          (to_l3_mesh_req_eligible_hot[gen_it]),
      .to_l3_mesh_req_info_o              (to_l3_mesh_req_info_all[gen_it]),
      .to_l3_mesh_req_picked_i            (to_l3_mesh_req_picked_1hot[gen_it]),
      .to_l3_mesh_rsp_valid_me_i          (to_l3_mesh_rsp_valid_1hot[gen_it]),
      .to_l3_mesh_rsp_valid_any_i         (to_l3_mesh_rsp_valid_any),
      .to_l3_mesh_rsp_info_i              (to_l3_mesh_rsp_info_i),

      .to_sys_mesh_clk_en_o               (to_sys_mesh_clk_en_all[gen_it]),
      .to_sys_mesh_req_eligible_o         (to_sys_mesh_req_eligible_hot[gen_it]),
      .to_sys_mesh_req_info_o             (to_sys_mesh_req_info_all[gen_it]),
      .to_sys_mesh_req_picked_i           (to_sys_mesh_req_picked_1hot[gen_it]),
      .to_sys_mesh_rsp_valid_me_i         (to_sys_mesh_rsp_valid_1hot[gen_it]),
      .to_sys_mesh_rsp_valid_any_i        (to_sys_mesh_rsp_valid_any),
      .to_sys_mesh_rsp_info_i             (to_sys_mesh_rsp_info_i),

      // L2 Responses back to neighborhoods
      .rspmux_l2_eligible_o               (rspmux_l2_eligible_hot[gen_it]),
      .rspmux_l2_info_o                   (rspmux_l2_info_all[gen_it]),
      .rspmux_l2_picked_i                 (rspmux_l2_picked_1hot[gen_it]),

      // L3 Responses back to L3 slave
      .rspmux_l3_eligible_o               (rspmux_l3_eligible_hot[gen_it]),
      .rspmux_l3_info_o                   (rspmux_l3_info_all[gen_it]),
      .rspmux_l3_picked_i                 (rspmux_l3_picked_1hot[gen_it]),

      .rspmux_sent_valid_i                (rspmux_sent_valid_hot_i[gen_it]),

      // Other Ops need priority
      .reqq_2nd_pass_need_priority_i      (reqq_2nd_pass_need_priority),
      .reqq_idx_cop_need_priority_i       (reqq_idx_cop_need_priority),

      // Abort idx cop
      .pipe_idx_cop_aborted_i             (pipe_idx_cop_aborted_i),

      .reqq_entry_valid_o                 (reqq_entry_valid[gen_it]),
      .reqq_entry_state_o                 (reqq_state_o[gen_it]),

      // For DV
      .reqq_entry_alloc_o                 (reqq_entries_alloc_hot[gen_it]),

      // For Dealloc
      .reqq_entry_dealloc_o               (reqq_entries_dealloc_hot[gen_it]),

      // Set error if it needs to be returned with the response
      .set_reqq_err_detected_me_i         (set_reqq_err_detected_hot_i[gen_it]),
      .set_reqq_err_zero_data_me_i        (set_reqq_err_zero_data_hot_i[gen_it]),

      // ESRs
      .esr_ctl_i                          (esr_ctl_i),
      .esr_sc_l3_shire_swizzle_ctl_i      (esr_sc_l3_shire_swizzle_ctl_i),

      .dft_i                              (dft_i)
    );
  end

  // ══════════════════════════════════════════════════════════
  // Performance monitor
  // ══════════════════════════════════════════════════════════
  // Resource measurement skewed by 2 cp wrt to rsrc_mode from perfmon.
  // Event measurement is not skewed.

  perfmon_req_info_t perfmon_p0_req_info_d1;
  perfmon_req_info_t perfmon_p1_req_info_d1;

  logic perfmon_rsp_msg_next;
  assign perfmon_rsp_msg_next =
    perfmon_p0_req_info_i.ev_mode[PerfmonEvMsg] &
    bank_l2_ready_o & bank_l2_valid_i &
    (bank_l2_info_i.opcode == etlink_pkg::ReqMsgSendData);

  logic [NumEntries-1:0] any_reqq_to_mesh;
  logic [NumEntries-1:0] any_reqq_to_l3_port;
  logic [NumEntries-1:0] l2_reqq_to_l3_port;
  logic [NumEntries-1:0] any_reqq_to_sys_port;
  logic [NumEntries-1:0] l2_reqq_to_sys_port;
  logic [NumEntries-1:0] l3_reqq_to_sys_port;

  always_comb begin
    any_reqq_to_mesh     = '0;
    any_reqq_to_l3_port  = '0;
    l2_reqq_to_l3_port   = '0;
    any_reqq_to_sys_port = '0;
    l2_reqq_to_sys_port  = '0;
    l3_reqq_to_sys_port  = '0;

    for (int ii = 0; ii < NumEntries; ii++) begin
      any_reqq_to_mesh[ii] = reqq_entry_valid[ii] &
        reqq_state_o[ii].opcode_destined_for_mesh;

      any_reqq_to_l3_port[ii] = any_reqq_to_mesh[ii] &
        (reqq_state_o[ii].which_mesh == ToL3);
      l2_reqq_to_l3_port[ii] = any_reqq_to_l3_port[ii] &
        ~(reqq_state_o[ii].reqq_source == ReqqSrcL3);

      any_reqq_to_sys_port[ii] = any_reqq_to_mesh[ii] &
        (reqq_state_o[ii].which_mesh == ToSys);
      l2_reqq_to_sys_port[ii] = any_reqq_to_sys_port[ii] &
        ~(reqq_state_o[ii].reqq_source == ReqqSrcL3);
      l3_reqq_to_sys_port[ii] = any_reqq_to_sys_port[ii] &
        (reqq_state_o[ii].reqq_source == ReqqSrcL3);
    end
  end

  logic [NumEntries-1:0] perfmon_reqq_l2_entries_d1;
  assign perfmon_reqq_l2_entries_d1 =
    perfmon_p0_req_info_d1.rsrc_mode[PerfmonRsrcL2Reqq] ? (reqq_l2_entries & reqq_entry_valid) : '0;

  logic [NumEntries-1:0] perfmon_reqq_l2_busy_d1;
  assign perfmon_reqq_l2_busy_d1 =
    /* verilator lint_off WIDTHEXPAND */  // 1-bit AND intentionally widened to fill vector
    perfmon_p1_req_info_d1.rsrc_mode[PerfmonRsrcL2Busy] ? NumEntries'(reqq_l2_full & bank_l2_valid_i) : '0;
    /* verilator lint_on WIDTHEXPAND */

  logic [NumEntries-1:0] perfmon_reqq_l3_entries_d1;
  assign perfmon_reqq_l3_entries_d1 =
    perfmon_p1_req_info_d1.rsrc_mode[PerfmonRsrcL3Reqq] ? (reqq_l3_entries & reqq_entry_valid) : '0;

  logic [NumEntries-1:0] perfmon_reqq_l3_busy_d1;
  assign perfmon_reqq_l3_busy_d1 =
    /* verilator lint_off WIDTHEXPAND */  // 1-bit AND intentionally widened to fill vector
    perfmon_p0_req_info_d1.rsrc_mode[PerfmonRsrcL3Busy] ? NumEntries'(reqq_l3_full & bank_l3_valid_i) : '0;
    /* verilator lint_on WIDTHEXPAND */

  logic [NumEntries-1:0] perfmon_reqq_any_reqq_to_l3_request_d1;
  assign perfmon_reqq_any_reqq_to_l3_request_d1 =
    perfmon_p0_req_info_d1.rsrc_mode[PerfmonRsrcAnyToL3] ? any_reqq_to_l3_port : '0;

  logic [NumEntries-1:0] perfmon_reqq_l2_reqq_to_l3_request_d1;
  assign perfmon_reqq_l2_reqq_to_l3_request_d1 =
    perfmon_p0_req_info_d1.rsrc_mode[PerfmonRsrcL2ToL3] ? l2_reqq_to_l3_port : '0;

  logic [NumEntries-1:0] perfmon_reqq_any_reqq_to_sys_request_d1;
  assign perfmon_reqq_any_reqq_to_sys_request_d1 =
    perfmon_p1_req_info_d1.rsrc_mode[PerfmonRsrcAnyToSys] ? any_reqq_to_sys_port : '0;

  logic [NumEntries-1:0] perfmon_reqq_l2_reqq_to_sys_request_d1;
  assign perfmon_reqq_l2_reqq_to_sys_request_d1 =
    perfmon_p1_req_info_d1.rsrc_mode[PerfmonRsrcL2ToSys] ? l2_reqq_to_sys_port : '0;

  logic [NumEntries-1:0] perfmon_reqq_l3_reqq_to_sys_request_d1;
  assign perfmon_reqq_l3_reqq_to_sys_request_d1 =
    perfmon_p0_req_info_d1.rsrc_mode[PerfmonRsrcL3ToSys] ? l3_reqq_to_sys_port : '0;

  logic perfmon_rsp_msg;
  logic [NumEntries-1:0] perfmon_rsp_p0_reqq_d1;
  logic [NumEntries-1:0] perfmon_rsp_p0_reqq_d2;
  logic [NumEntries-1:0] perfmon_rsp_p1_reqq_d1;
  logic [NumEntries-1:0] perfmon_rsp_p1_reqq_d2;
  logic [PerfmonRsrcRspSize-1:0] perfmon_rsp_p0_cntr_d2;
  logic [PerfmonRsrcRspSize-1:0] perfmon_rsp_p0_cntr_d3;
  logic [PerfmonRsrcRspSize-1:0] perfmon_rsp_p1_cntr_d2;
  logic [PerfmonRsrcRspSize-1:0] perfmon_rsp_p1_cntr_d3;

  always_comb begin
    perfmon_p0_rsp_info_o                       = '0;
    perfmon_p1_rsp_info_o                       = '0;

    perfmon_p0_rsp_info_o.ev_rsp.ev_msgsenddata = perfmon_rsp_msg;

    perfmon_p0_rsp_info_o.rsrc_rsp.rsrc_cntr    = perfmon_rsp_p0_cntr_d3;
    perfmon_p1_rsp_info_o.rsrc_rsp.rsrc_cntr    = perfmon_rsp_p1_cntr_d3;

    perfmon_rsp_p0_reqq_d1 =
      perfmon_reqq_l2_entries_d1 |
      perfmon_reqq_l3_busy_d1 |
      perfmon_reqq_any_reqq_to_l3_request_d1 |
      perfmon_reqq_l2_reqq_to_l3_request_d1 |
      perfmon_reqq_l3_reqq_to_sys_request_d1;

    perfmon_rsp_p1_reqq_d1 =
      perfmon_reqq_l3_entries_d1 |
      perfmon_reqq_l2_busy_d1 |
      perfmon_reqq_any_reqq_to_sys_request_d1 |
      perfmon_reqq_l2_reqq_to_sys_request_d1;

    perfmon_rsp_p0_cntr_d2 = count_valid_entries(NumEntries, perfmon_rsp_p0_reqq_d2);
    perfmon_rsp_p1_cntr_d2 = count_valid_entries(NumEntries, perfmon_rsp_p1_reqq_d2);
  end

  logic perfmon_cgater;
  assign perfmon_cgater = perfmon_p0_req_info_i.cgater;

  always_ff @(posedge clk_i or negedge rst_n) begin
    if (!rst_n) begin
      perfmon_rsp_msg           <= '0;
      perfmon_p0_req_info_d1    <= '0;
      perfmon_p1_req_info_d1    <= '0;
      perfmon_rsp_p0_reqq_d2    <= '0;
      perfmon_rsp_p1_reqq_d2    <= '0;
      perfmon_rsp_p0_cntr_d3    <= '0;
      perfmon_rsp_p1_cntr_d3    <= '0;
    end else if (perfmon_cgater) begin
      perfmon_rsp_msg           <= perfmon_rsp_msg_next;
      perfmon_p0_req_info_d1    <= perfmon_p0_req_info_i;
      perfmon_p1_req_info_d1    <= perfmon_p1_req_info_i;
      perfmon_rsp_p0_reqq_d2    <= perfmon_rsp_p0_reqq_d1;
      perfmon_rsp_p1_reqq_d2    <= perfmon_rsp_p1_reqq_d1;
      perfmon_rsp_p0_cntr_d3    <= perfmon_rsp_p0_cntr_d2;
      perfmon_rsp_p1_cntr_d3    <= perfmon_rsp_p1_cntr_d2;
    end
  end

  // ══════════════════════════════════════════════════════════
  // Functions
  // ══════════════════════════════════════════════════════════

  // Return total number of valid entries
  function automatic logic [PerfmonRsrcRspSize-1:0] count_valid_entries;
    input integer num_entries;
    input logic [NumEntries-1:0] entries;
    begin
      count_valid_entries = '0;
      for (integer ii = 0; ii < num_entries; ii++) begin
        count_valid_entries = count_valid_entries + PerfmonRsrcRspSize'(entries[ii]);
      end
    end
  endfunction

  // Compare the cache line address bits.  Mask off sub cache line bits.
  function automatic bit masked_address_compare;
    input logic [PaSize-1:0] address_a;
    input logic [PaSize-1:0] address_b;
    input address_space_e address_space;
    begin
      masked_address_compare = '0;
      if (address_space == AddressL2) begin
        masked_address_compare |= (address_a[(PaSize-1) : ReqqSelAddrLsb] == address_b[(PaSize-1) : ReqqSelAddrLsb]);
      end
      if (address_space == AddressL3) begin
        masked_address_compare |= (address_a[(PaSize-1) : ReqqSelAddrLsb] == address_b[(PaSize-1) : ReqqSelAddrLsb]);
      end
      if (address_space == AddressScp) begin
        masked_address_compare |= (address_a[(PaSize-1) : ReqqSelAddrLsb] == address_b[(PaSize-1) : ReqqSelAddrLsb]);
      end
      if (address_space == AddressMsg) begin
        masked_address_compare |= (address_a[MsgNeighIdLsb +: MsgNeighIdSize] == address_b[MsgNeighIdLsb +: MsgNeighIdSize]);
      end
    end
  endfunction

  // ══════════════════════════════════════════════════════════
  // LINT
  // ══════════════════════════════════════════════════════════

  logic unused_ok;
  assign unused_ok = &{
    1'b0,
    reqq_entries_dealloc_hot,
    reqq_entries_alloc_hot,
    self_victim_still_dependable,
    reqq_rspmux_l2_fifo_level_unused,
    reqq_rspmux_l3_fifo_level_unused,
    to_l3_mesh_req_fifo_level_unused,
    to_sys_mesh_req_fifo_level_unused,
    reqq_state_o,
    pipe_req_info_unused,
    perfmon_p0_req_info_i,
    perfmon_p1_req_info_i,
    perfmon_p0_req_info_d1,
    perfmon_p1_req_info_d1,
    reqq_rbuf_clear,
    reqq_rbuf_clear_pending_install,
    coalesce_victim,
    coalesce_victim_matches_tag_victim
  };

  // ══════════════════════════════════════════════════════════
  // Assertions (Verilator-compatible)
  // ══════════════════════════════════════════════════════════
  // synthesis translate_off
`ifdef VERILATOR
  /* verilator lint_off SYNCASYNCNET */  // rst_ni/rst_n used in assertion guards
  always_ff @(posedge clk_i) begin
    // One-hot picks
    // Match the original assertion intent more closely: do not inspect xrand
    // startup state before the raw top-level reset has been asserted.
    if (rst_ni && rst_n && !$onehot0(pipe_req_picked_1hot_r1))
      $error("ERROR_pipe_req_picked_1hot");
    if (rst_ni && rst_n && !$onehot0(rbuf_req_picked_1hot))
      $error("ERROR_rbuf_req_picked_1hot");
    if (rst_ni && rst_n && !$onehot0(rspmux_l2_picked_1hot))
      $error("ERROR_rspmux_l2_req_picked_1hot");
    if (rst_ni && rst_n && !$onehot0(rspmux_l3_picked_1hot))
      $error("ERROR_rspmux_l3_req_picked_1hot");
  end
  /* verilator lint_on SYNCASYNCNET */
`endif
  // synthesis translate_on

endmodule
