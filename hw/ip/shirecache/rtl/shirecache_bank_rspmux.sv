// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire cache bank response mux.
//
// Aggregates 6 response sources, converts them to rspmux_long_t format
// (with port routing, ET-Link response formatting), arbitrates per-port
// with round-robin into per-port FIFOs (depth 2), then selects the
// winning response via LRU priority/opportunistic arbitration with
// credit-based flow control for broadcast responses.
//
// Replaces:
//   shire_cache_bank_rspmux.v      (314 lines)
//   shire_cache_bank_rspmux_xbar_arb.v (121 lines)

module shirecache_bank_rspmux
  import shirecache_pkg::*;
  import etlink_pkg::*;
#(
  parameter int unsigned NumEntries   = ReqqDepth,
  parameter int unsigned NumPorts     = Ports
) (
  input  logic                                  clk_i,
  input  logic                                  rst_ni,

  // ── Bank rsp output to xbar ──────────────────────────────
  output logic [NumPorts-1:0]                   bank_rsp_valid_o,
  output xbar_rsp_t                             bank_rsp_info_o,
  input  logic [NumPorts-1:0]                   bank_rsp_ready_i,
  input  logic [NumPorts-1:0]                   bank_rsp_2_credits_i,

  // ── Responses from pipe rbuf ─────────────────────────────
  input  logic                                  pipe_rbuf_rd_rsp_valid_i,
  input  rbuf_rd_rsp_t                          pipe_rbuf_rd_rsp_info_i,
  output logic                                  pipe_rbuf_rd_rsp_ready_o,

  // ── Responses from pipe data RAMs ────────────────────────
  input  logic                                  pipe_data_rsp_valid_i,
  input  data_rsp_t                             pipe_data_rsp_info_i,
  input  data_rsp_modifier_t                    pipe_data_rsp_modifier_info_i,
  output logic                                  pipe_data_rsp_ready_o,

  // ── Fill responses from mesh ─────────────────────────────
  input  logic                                  to_l3_mesh_rsp_valid_i,
  input  reqq_mesh_rsp_t                        to_l3_mesh_rsp_info_i,
  output logic                                  to_l3_mesh_rsp_ready_o,
  input  logic                                  to_sys_mesh_rsp_valid_i,
  input  reqq_mesh_rsp_t                        to_sys_mesh_rsp_info_i,
  output logic                                  to_sys_mesh_rsp_ready_o,

  // ── Reqq responses (no data) ─────────────────────────────
  input  logic                                  reqq_rspmux_valid_nodata_i,
  input  rspmux_t                               reqq_rspmux_info_i,
  output logic                                  reqq_rspmux_ready_nodata_o,

  // ── Dataq responses (with data) ──────────────────────────
  input  logic                                  dataq_rspmux_valid_i,
  input  rspmux_t                               dataq_rspmux_info_i,
  output logic                                  dataq_rspmux_ready_o,

  // ── Rspmux sent notification ─────────────────────────────
  output logic [NumEntries-1:0]                 rspmux_sent_valid_hot_o,
  output rspmux_sent_t                          rspmux_trace_sent_o,
  output logic                                  a_rspmux_rsp_is_valid_o,

  // ── Reqq state ───────────────────────────────────────────
  input  reqq_entry_state_t [NumEntries-1:0]    reqq_state_i,
  input  bank_esr_ctl_t                         esr_ctl_i
);

  localparam int unsigned NumRspSrc = 6;

  // ════════════════════════════════════════════════════════════
  // Per-source base fields (trans_id, reqq_id, data, port_id, with_data)
  // ════════════════════════════════════════════════════════════

  logic [DvTransIdSize-1:0]  src_trans_id [NumRspSrc];
  logic [ReqqIdSize-1:0]     src_reqq_id  [NumRspSrc];
  logic [LineDataSize-1:0]   src_data     [NumRspSrc];
  logic [PortIdSize-1:0]     src_port_id  [NumRspSrc];
  logic                      src_with_data[NumRspSrc];

  // Source 0: pipe rbuf read response
  assign src_trans_id[0]  = pipe_rbuf_rd_rsp_info_i.trans_id;
  assign src_reqq_id[0]   = pipe_rbuf_rd_rsp_info_i.reqq_id;
  assign src_data[0]      = pipe_rbuf_rd_rsp_info_i.data;
  assign src_port_id[0]   = reqq_state_i[pipe_rbuf_rd_rsp_info_i.reqq_id].port_id;
  assign src_with_data[0] = 1'b1;

  // Source 1: pipe data RAM response
  assign src_trans_id[1]  = pipe_data_rsp_info_i.trans_id;
  assign src_reqq_id[1]   = pipe_data_rsp_info_i.reqq_id;
  assign src_data[1]      = pipe_data_rsp_info_i.data;
  assign src_port_id[1]   = pipe_data_rsp_modifier_info_i.port_id;
  assign src_with_data[1] = 1'b1;

  // Source 2: L3 mesh response
  assign src_trans_id[2]  = to_l3_mesh_rsp_info_i.trans_id;
  assign src_reqq_id[2]   = to_l3_mesh_rsp_info_i.reqq_id;
  assign src_data[2]      = to_l3_mesh_rsp_info_i.data;
  assign src_port_id[2]   = reqq_state_i[to_l3_mesh_rsp_info_i.reqq_id].port_id;
  assign src_with_data[2] = 1'b1;

  // Source 3: Sys mesh response
  assign src_trans_id[3]  = to_sys_mesh_rsp_info_i.trans_id;
  assign src_reqq_id[3]   = to_sys_mesh_rsp_info_i.reqq_id;
  assign src_data[3]      = to_sys_mesh_rsp_info_i.data;
  assign src_port_id[3]   = reqq_state_i[to_sys_mesh_rsp_info_i.reqq_id].port_id;
  assign src_with_data[3] = 1'b1;

  // Source 4: reqq no-data response
  assign src_trans_id[4]  = reqq_rspmux_info_i.trans_id;
  assign src_reqq_id[4]   = reqq_rspmux_info_i.reqq_id;
  assign src_data[4]      = '0;
  assign src_port_id[4]   = reqq_state_i[reqq_rspmux_info_i.reqq_id].port_id;
  assign src_with_data[4] = reqq_rspmux_info_i.with_data;

  // Source 5: dataq response
  assign src_trans_id[5]  = dataq_rspmux_info_i.trans_id;
  assign src_reqq_id[5]   = dataq_rspmux_info_i.reqq_id;
  assign src_data[5]      = dataq_rspmux_info_i.data;
  assign src_port_id[5]   = reqq_state_i[dataq_rspmux_info_i.reqq_id].port_id;
  assign src_with_data[5] = 1'b1;

  // ════════════════════════════════════════════════════════════
  // Per-source valid signals
  // ════════════════════════════════════════════════════════════

  logic [NumRspSrc-1:0] rspmux_valid;
  logic [NumRspSrc-1:0] rspmux_ready;

  // Responses from rbuf are only valid if they are from l2.
  assign rspmux_valid[0] = pipe_rbuf_rd_rsp_valid_i &
                           ~pipe_rbuf_rd_rsp_info_i.l3_source;
  assign pipe_rbuf_rd_rsp_ready_o = rspmux_ready[0];

  // Responses from pipe are only eligible if they are hit data (not victim) from neigh.
  assign rspmux_valid[1] = pipe_data_rsp_valid_i &
    (~(esr_ctl_i.esr_sc_err_interrupt_enable[int'(ErrLogEccSbe)] &
       (|pipe_data_rsp_info_i.data_ram_sbe))) &
    (~(esr_ctl_i.esr_sc_err_interrupt_enable[int'(ErrLogEccDbe)] &
       (|pipe_data_rsp_info_i.data_ram_dbe))) &
    (pipe_data_rsp_modifier_info_i.rsp_type == NormalRsp) &
    (pipe_data_rsp_modifier_info_i.reqq_source == ReqqSrcL2);
  assign pipe_data_rsp_ready_o = rspmux_ready[1];

  assign rspmux_valid[2]        = to_l3_mesh_rsp_valid_i;
  assign to_l3_mesh_rsp_ready_o = rspmux_ready[2];

  assign rspmux_valid[3]        = to_sys_mesh_rsp_valid_i;
  assign to_sys_mesh_rsp_ready_o = rspmux_ready[3];

  assign rspmux_valid[4]            = reqq_rspmux_valid_nodata_i;
  assign reqq_rspmux_ready_nodata_o = rspmux_ready[4];

  assign rspmux_valid[5]        = dataq_rspmux_valid_i;
  assign dataq_rspmux_ready_o   = rspmux_ready[5];

  // ════════════════════════════════════════════════════════════
  // Build full rspmux_long_t per source (base fields + reqq state lookup)
  // ════════════════════════════════════════════════════════════

  rspmux_long_t [NumRspSrc-1:0] rspmux_info;

  for (genvar gs = 0; gs < NumRspSrc; gs++) begin : g_fill
    always_comb begin
      // Base fields
      rspmux_info[gs].trans_id  = src_trans_id[gs];
      rspmux_info[gs].reqq_id   = src_reqq_id[gs];
      rspmux_info[gs].data      = src_data[gs];
      rspmux_info[gs].port_id   = src_port_id[gs];
      rspmux_info[gs].with_data = src_with_data[gs];

      // Reqq state lookup
      rspmux_info[gs].traced = reqq_state_i[src_reqq_id[gs]].traced;
      rspmux_info[gs].dest_hot =
        (reqq_state_i[src_reqq_id[gs]].orig_opcode inside {OrigReadCoop, OrigScpReadCoop})
          ? NumPorts'(reqq_state_i[src_reqq_id[gs]].readcoop_dest)
          : NumPorts'(1'b1) << reqq_state_i[src_reqq_id[gs]].port_id;

      // ET-Link response formatting
      rspmux_info[gs].xbar_info.trans_id  = src_trans_id[gs];
      rspmux_info[gs].xbar_info.port_id   = src_port_id[gs];
      rspmux_info[gs].xbar_info.src_is_uc = 1'b0;

      rspmux_info[gs].xbar_info.rsp_info.id =
        reqq_state_i[src_reqq_id[gs]].tag_id[etlink_pkg::IdSize-1:0];
      rspmux_info[gs].xbar_info.rsp_info.dest =
        reqq_state_i[src_reqq_id[gs]].source[etlink_pkg::SourceSize-1:0];

      rspmux_info[gs].xbar_info.rsp_info.wdata =
        (reqq_state_i[src_reqq_id[gs]].err_detected & esr_ctl_i.esr_sc_err_rsp_enable) ? 1'b0 :
        (reqq_state_i[src_reqq_id[gs]].orig_opcode == OrigMsgSendData)                 ? 1'b1 :
        src_with_data[gs];

      rspmux_info[gs].xbar_info.rsp_info.opcode =
        (reqq_state_i[src_reqq_id[gs]].err_detected & esr_ctl_i.esr_sc_err_rsp_enable)
          ? etlink_pkg::RspErr :
        (reqq_state_i[src_reqq_id[gs]].orig_opcode == OrigMsgSendData)
          ? etlink_pkg::RspMsgRcvData :
        src_with_data[gs]
          ? etlink_pkg::RspAckData :
            etlink_pkg::RspAck;

      rspmux_info[gs].xbar_info.rsp_info.data =
        reqq_state_i[src_reqq_id[gs]].err_zero_data ? '0 : src_data[gs];

      rspmux_info[gs].xbar_info.rsp_info.size =
        sc_size_t'(reqq_state_i[src_reqq_id[gs]].size);

      rspmux_info[gs].xbar_info.rsp_info.qwen =
        src_with_data[gs] ? reqq_state_i[src_reqq_id[gs]].rspmux_qwen : '0;
    end
  end

  // ════════════════════════════════════════════════════════════
  // Per-port round-robin arbitration into per-port FIFOs
  // ════════════════════════════════════════════════════════════

  logic [NumPorts-1:0][NumRspSrc-1:0]  rspmux_grant_per_port;
  logic [NumPorts-1:0][NumRspSrc-1:0]  rspmux_valid_per_port;
  logic [NumPorts-1:0]                 rspmux_fifo_full;
  logic [NumPorts-1:0]                 rspmux_fifo_valid;
  logic [NumPorts-1:0]                 rspmux_fifo_pop;
  rspmux_long_t [NumPorts-1:0]         rspmux_fifo_data;
  logic [NumPorts-1:0][NumPorts-1:0]   rspmux_fifo_dest_hot;

  for (genvar gp = 0; gp < NumPorts; gp++) begin : g_rsp
    for (genvar gs = 0; gs < NumRspSrc; gs++) begin : g_src
      assign rspmux_valid_per_port[gp][gs] =
        rspmux_valid[gs] & (rspmux_info[gs].port_id == PortIdSize'(gp));
    end

    rspmux_long_t rspmux_info_winner;

    prim_arb_rr #(
      .NumReqs (NumRspSrc),
      .Width   ($bits(rspmux_long_t))
    ) u_port_rsp_arb (
      .clk_i,
      .rst_ni,
      .reqs_i   (rspmux_valid_per_port[gp]),
      .grants_o (rspmux_grant_per_port[gp]),
      .pop_i    (~rspmux_fifo_full[gp]),
      .data_i   (rspmux_info),
      .data_o   (rspmux_info_winner)
    );

    /* verilator lint_off UNUSEDSIGNAL */  // FIFO level not used in this module
    logic [2:0] rspmux_fifo_level_unused;
    /* verilator lint_on UNUSEDSIGNAL */

    prim_fifo_reg #(
      .Width ($bits(rspmux_long_t)),
      .Depth (2)
    ) u_rspmux_fifo (
      .clk_i,
      .rst_ni,
      .push_i  (|rspmux_valid_per_port[gp]),
      .wdata_i (rspmux_info_winner),
      .full_o  (rspmux_fifo_full[gp]),
      .level_o (rspmux_fifo_level_unused),
      .pop_i   (rspmux_fifo_pop[gp]),
      .valid_o (rspmux_fifo_valid[gp]),
      .rdata_o (rspmux_fifo_data[gp])
    );
  end

  // OR all per-port grants to form ready signals
  always_comb begin
    rspmux_ready = '0;
    for (int unsigned port = 0; port < NumPorts; port++) begin
      rspmux_ready |= (rspmux_grant_per_port[port] & rspmux_valid_per_port[port] &
                        ~{NumRspSrc{rspmux_fifo_full[port]}});
    end
  end

  // Tell reqq that the rsp is sent (multiple rsp can be sent per cycle)
  always_comb begin
    rspmux_sent_valid_hot_o = '0;
    for (int unsigned src = 0; src < NumRspSrc; src++) begin
      rspmux_sent_valid_hot_o[rspmux_info[src].reqq_id] |= rspmux_ready[src];
    end
  end

  // Pull out the dest_hot bits
  always_comb begin
    rspmux_fifo_dest_hot = '0;
    for (int unsigned port = 0; port < NumPorts; port++) begin
      rspmux_fifo_dest_hot[port] |= rspmux_fifo_data[port].dest_hot;
    end
  end

  // ════════════════════════════════════════════════════════════
  // Xbar arbiter (inlined from shire_cache_bank_rspmux_xbar_arb)
  //
  // LRU priority/opportunistic arbiter with credit-based flow
  // control for broadcast responses.
  //
  // The priority_grant (oldest outstanding response) reserves xbar
  // FIFO space. Opportunistic responses are sent when all needed
  // resources are ready after taking into account the reserved space.
  // ════════════════════════════════════════════════════════════

  logic [NumPorts-1:0] priority_grant;
  logic [NumPorts-1:0] opportunistic_grant;
  logic [NumPorts-1:0] reserved_resources;
  logic [NumPorts-1:0] all_resources_are_ready;

  prim_arb_lru_2bid #(
    .NumClients (NumPorts)
  ) u_arb_lru (
    .clk_i,
    .rst_ni,
    .bid0_i   (rspmux_fifo_valid),
    .pop0_i   (1'b0),  // priority grant never pops directly; opportunistic handles it
    .grant0_o (priority_grant),
    .bid1_i   (rspmux_fifo_valid & all_resources_are_ready),
    .pop1_i   (|(rspmux_fifo_valid & all_resources_are_ready)),
    .grant1_o (opportunistic_grant)
  );

  // ── Select reserved_resources via one-hot mux ─────────────
  always_comb begin
    reserved_resources = '0;
    for (int unsigned ii = 0; ii < NumPorts; ii++) begin
      reserved_resources |= {NumPorts{priority_grant[ii]}} & rspmux_fifo_dest_hot[ii];
    end
  end

  // ── Calculate all_resources_are_ready per FIFO ────────────
  always_comb begin
    for (int unsigned ii = 0; ii < NumPorts; ii++) begin
      all_resources_are_ready[ii] = 1'b1;
      for (int unsigned jj = 0; jj < NumPorts; jj++) begin
        all_resources_are_ready[ii] &=
          (bank_rsp_2_credits_i[jj]                              |  // 2 spots available
           ~rspmux_fifo_dest_hot[ii][jj]                         |  // don't need this resource
           (bank_rsp_ready_i[jj] & priority_grant[ii])           |  // resource available and I'm priority
           (bank_rsp_ready_i[jj] & ~reserved_resources[jj]));       // resource available and not reserved
      end
    end
  end

  // ── bank_rsp_valid: dest_hot of the opportunistic grant ───
  always_comb begin
    bank_rsp_valid_o = '0;
    for (int unsigned ii = 0; ii < NumPorts; ii++) begin
      bank_rsp_valid_o |= {NumPorts{opportunistic_grant[ii] & all_resources_are_ready[ii]}} &
                          rspmux_fifo_dest_hot[ii];
    end
  end

  // ── bank_rsp_data: data of the opportunistic grant ────────
  rspmux_long_t bank_rsp_info_full;

  always_comb begin
    bank_rsp_info_full = '0;
    for (int unsigned ii = 0; ii < NumPorts; ii++) begin
      /* verilator lint_off WIDTHEXPAND */  // one-hot mux: OR-reduction across all ports
      bank_rsp_info_full |= rspmux_long_t'({$bits(rspmux_long_t){opportunistic_grant[ii]}} &
                             $bits(rspmux_long_t)'(rspmux_fifo_data[ii]));
      /* verilator lint_on WIDTHEXPAND */
    end
  end

  assign bank_rsp_info_o = bank_rsp_info_full.xbar_info;

  assign rspmux_fifo_pop = opportunistic_grant & rspmux_fifo_valid & all_resources_are_ready;

  // ════════════════════════════════════════════════════════════
  // Rspmux sent trace notification
  // ════════════════════════════════════════════════════════════

  assign rspmux_trace_sent_o.valid   = |(bank_rsp_valid_o & bank_rsp_ready_i);
  assign rspmux_trace_sent_o.reqq_id = bank_rsp_info_full.reqq_id;
  assign rspmux_trace_sent_o.traced  = bank_rsp_info_full.traced;

  assign a_rspmux_rsp_is_valid_o     = |rspmux_fifo_valid;

  // ════════════════════════════════════════════════════════════
  // Unused signals
  // ════════════════════════════════════════════════════════════

  /* verilator lint_off UNUSEDSIGNAL */  // fields consumed only in the original for coverage/assertions
  logic unused_ok;
  assign unused_ok = &{1'b0,
    bank_rsp_info_full,
    to_l3_mesh_rsp_info_i.dataq_id,
    to_l3_mesh_rsp_info_i.err,
    to_l3_mesh_rsp_info_i.zero_data,
    to_sys_mesh_rsp_info_i.dataq_id,
    to_sys_mesh_rsp_info_i.err,
    to_sys_mesh_rsp_info_i.zero_data,
    reqq_rspmux_info_i.with_data,
    reqq_rspmux_info_i.data,
    reqq_rspmux_info_i.qwen,
    reqq_rspmux_info_i.zero_data,
    dataq_rspmux_info_i.with_data,
    dataq_rspmux_info_i.qwen,
    dataq_rspmux_info_i.zero_data,
    pipe_data_rsp_modifier_info_i,
    pipe_data_rsp_info_i,
    esr_ctl_i,
    reqq_state_i
  };
  /* verilator lint_on UNUSEDSIGNAL */

endmodule : shirecache_bank_rspmux
