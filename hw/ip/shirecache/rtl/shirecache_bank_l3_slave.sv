// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire cache L3 slave bank interface.
//
// Receives L3 mesh slave requests, buffers them in a depth-2 FIFO, and maps
// to bank_req format for the reqq.  Arbitrates 5 response sources (pipe data,
// to_sys mesh, reqq nodata, dataq, rbuf) via round-robin, maps to mesh slave
// response format, and sends through a depth-2 response FIFO.
//
// Replaces: shire_cache_bank_l3_slave (etcore-soc)

module shirecache_bank_l3_slave
  import shirecache_pkg::*;
  import etlink_pkg::*;
#(
  parameter int unsigned NumEntries = ReqqDepth,
  parameter int unsigned NumEntriesId = ReqqIdSize
) (
  input  logic                                clk_i,
  input  logic                                rst_ni,

  // L3 mesh slave interface
  input  logic                                l3_mesh_slave_bank_req_valid_i,
  input  mesh_slave_req_t                     l3_mesh_slave_bank_req_info_i,
  output logic                                l3_mesh_slave_bank_req_ready_o,

  output logic                                l3_mesh_slave_bank_rsp_valid_o,
  output mesh_slave_rsp_t                     l3_mesh_slave_bank_rsp_info_o,
  input  logic                                l3_mesh_slave_bank_rsp_ready_i,

  // Alloc a new request to reqq
  output logic                                bank_l3_valid_o,
  output reqq_bank_req_t                      bank_l3_info_o,
  input  logic                                bank_l3_ready_i,

  // Rspmux responses — pipe data rsp
  input  logic                                pipe_data_rsp_valid_i,
  input  data_rsp_t                           pipe_data_rsp_info_i,
  input  data_rsp_modifier_t                  pipe_data_rsp_modifier_info_i,
  output logic                                pipe_data_rsp_ready_o,

  // Rspmux responses — to_sys mesh rsp
  input  logic                                to_sys_mesh_rsp_valid_l3_slave_i,
  input  reqq_mesh_rsp_t                      to_sys_mesh_rsp_info_i,
  output logic                                to_sys_mesh_rsp_ready_l3_slave_o,

  // Rspmux responses — reqq nodata
  input  logic                                reqq_rspmux_valid_nodata_i,
  input  rspmux_t                             reqq_rspmux_info_i,
  output logic                                reqq_rspmux_ready_nodata_o,

  // Rspmux responses — dataq
  input  logic                                dataq_rspmux_valid_i,
  input  rspmux_t                             dataq_rspmux_info_i,
  output logic                                dataq_rspmux_ready_o,

  // Rspmux responses — rbuf read
  input  logic                                pipe_rbuf_rd_rsp_valid_i,
  input  rbuf_rd_rsp_t                        pipe_rbuf_rd_rsp_info_i,
  output logic                                pipe_rbuf_rd_rsp_ready_o,

  // Rspmux sent notification
  output logic                                rspmux_sent_valid_o,
  output logic [NumEntriesId-1:0]             rspmux_sent_id_o,
  output logic [NumEntries-1:0]               rspmux_sent_valid_hot_o,
  output logic                                a_rspmux_rsp_is_valid_o,

  // Reqq state
  input  reqq_entry_state_t [NumEntries-1:0]  reqq_state_i,
  input  bank_esr_ctl_t                       esr_ctl_i
);

  /* verilator lint_off UNUSEDSIGNAL */

  // ══════════════════════════════════════════════════════════
  // L3 requests — input FIFO + mapping
  // ══════════════════════════════════════════════════════════

  logic              mesh_req_fifo_valid;
  mesh_slave_req_t   mesh_req_fifo_info;
  logic              mesh_req_fifo_full;
  logic              mesh_req_fifo_pop;

  logic [2:0] mesh_req_fifo_level_unused;
  prim_fifo_reg #(
    .Width($bits(mesh_slave_req_t)),
    .Depth(2)
  ) u_mesh_req_fifo (
    .clk_i  (clk_i),
    .rst_ni (rst_ni),
    .push_i (l3_mesh_slave_bank_req_valid_i),
    .wdata_i(l3_mesh_slave_bank_req_info_i),
    .full_o (mesh_req_fifo_full),
    .level_o(mesh_req_fifo_level_unused),
    .pop_i  (mesh_req_fifo_pop),
    .valid_o(mesh_req_fifo_valid),
    .rdata_o(mesh_req_fifo_info)
  );
  assign l3_mesh_slave_bank_req_ready_o = ~mesh_req_fifo_full;

  // Map mesh slave request to bank request format
  always_comb begin
    bank_l3_valid_o               = mesh_req_fifo_valid;
    bank_l3_info_o.trans_id       = mesh_req_fifo_info.trans_id;
    bank_l3_info_o.port_id        = PortIdSize'(mesh_req_fifo_info.port_id);
    bank_l3_info_o.id             = ReqqTagIdSize'(mesh_req_fifo_info.id);
    bank_l3_info_o.source         = ReqqSourceSize'(mesh_req_fifo_info.source);
    bank_l3_info_o.qos            = {3'b0, mesh_req_fifo_info.qos == AxiQosMemHighPriority};
    bank_l3_info_o.opcode         = mesh_req_fifo_info.opcode;
    bank_l3_info_o.address        = mesh_req_fifo_info.address;
    bank_l3_info_o.wdata          = mesh_req_fifo_info.wdata;
    bank_l3_info_o.data           = mesh_req_fifo_info.data;
    bank_l3_info_o.qwen           = mesh_req_fifo_info.qwen;
    bank_l3_info_o.size           = mesh_req_fifo_info.size;
    bank_l3_info_o.subopcode      = '0;
    bank_l3_info_o.ben            = mesh_req_fifo_info.ben;
    bank_l3_info_o.write_is_partial = mesh_req_fifo_info.ben_en;
  end

  assign mesh_req_fifo_pop = bank_l3_ready_i;

  // ══════════════════════════════════════════════════════════
  // L3 responses — 5-source arbitration
  // ══════════════════════════════════════════════════════════

  localparam int unsigned NumRspSrc = 5;

  logic [NumRspSrc-1:0]           rspmux_valid;
  logic [NumRspSrc-1:0]           rspmux_ready;
  logic [NumRspSrc-1:0]           rspmux_grant;
  /* verilator lint_off UNOPTFLAT */  // struct array with independent fields assigned per-source
  mesh_slave_rsp_long_t [NumRspSrc-1:0] rspmux_info;
  /* verilator lint_on UNOPTFLAT */

  // Source 0: pipe data rsp (only normal rsp, L3 source, no ECC errors, no mesh redirect)
  assign rspmux_valid[0] = pipe_data_rsp_valid_i
    & ~(esr_ctl_i.esr_sc_err_interrupt_enable[int'(ErrLogEccSbe)] & (|pipe_data_rsp_info_i.data_ram_sbe))
    & ~(esr_ctl_i.esr_sc_err_interrupt_enable[int'(ErrLogEccDbe)] & (|pipe_data_rsp_info_i.data_ram_dbe))
    & (pipe_data_rsp_info_i.rsp_type == NormalRsp)
    & ~pipe_data_rsp_modifier_info_i.redirect_normal_rsp_to_mesh
    & (pipe_data_rsp_modifier_info_i.reqq_source == ReqqSrcL3);
  assign rspmux_info[0].reqq_id      = pipe_data_rsp_info_i.reqq_id;
  assign rspmux_info[0].rsp.trans_id = pipe_data_rsp_info_i.trans_id;
  assign rspmux_info[0].rsp.wdata    = 1'b1;
  assign pipe_data_rsp_ready_o       = rspmux_ready[0];

  // Source 1: to_sys mesh rsp
  assign rspmux_valid[1] = to_sys_mesh_rsp_valid_l3_slave_i;
  assign rspmux_info[1].reqq_id      = to_sys_mesh_rsp_info_i.reqq_id;
  assign rspmux_info[1].rsp.trans_id = to_sys_mesh_rsp_info_i.trans_id;
  assign rspmux_info[1].rsp.wdata    = 1'b1;
  assign to_sys_mesh_rsp_ready_l3_slave_o = rspmux_ready[1];

  // Source 2: reqq rspmux nodata
  assign rspmux_valid[2] = reqq_rspmux_valid_nodata_i;
  assign rspmux_info[2].reqq_id      = reqq_rspmux_info_i.reqq_id;
  assign rspmux_info[2].rsp.trans_id = reqq_rspmux_info_i.trans_id;
  assign rspmux_info[2].rsp.wdata    = reqq_rspmux_info_i.with_data;
  assign reqq_rspmux_ready_nodata_o  = rspmux_ready[2];

  // Source 3: dataq rspmux
  assign rspmux_valid[3] = dataq_rspmux_valid_i;
  assign rspmux_info[3].reqq_id      = dataq_rspmux_info_i.reqq_id;
  assign rspmux_info[3].rsp.trans_id = dataq_rspmux_info_i.trans_id;
  assign rspmux_info[3].rsp.wdata    = 1'b1;
  assign dataq_rspmux_ready_o        = rspmux_ready[3];

  // Source 4: rbuf read rsp (only if l3_source)
  assign rspmux_valid[4] = pipe_rbuf_rd_rsp_valid_i & pipe_rbuf_rd_rsp_info_i.l3_source;
  assign rspmux_info[4].reqq_id      = pipe_rbuf_rd_rsp_info_i.reqq_id;
  assign rspmux_info[4].rsp.trans_id = pipe_rbuf_rd_rsp_info_i.trans_id;
  assign rspmux_info[4].rsp.wdata    = 1'b1;
  assign pipe_rbuf_rd_rsp_ready_o    = rspmux_ready[4];

  // Data: error-zero gating per source
  assign rspmux_info[0].rsp.data =
    reqq_state_i[rspmux_info[0].reqq_id].err_zero_data ? '0 : pipe_data_rsp_info_i.data;
  assign rspmux_info[1].rsp.data =
    reqq_state_i[rspmux_info[1].reqq_id].err_zero_data ? '0 : to_sys_mesh_rsp_info_i.data;
  assign rspmux_info[2].rsp.data = '0;
  assign rspmux_info[3].rsp.data =
    reqq_state_i[rspmux_info[3].reqq_id].err_zero_data ? '0 : dataq_rspmux_info_i.data;
  assign rspmux_info[4].rsp.data =
    reqq_state_i[rspmux_info[4].reqq_id].err_zero_data ? '0 : pipe_rbuf_rd_rsp_info_i.data;

  // Common fields from reqq state
  for (genvar g = 0; g < NumRspSrc; g++) begin : gen_rsp_common
    assign rspmux_info[g].rsp.id     = MeshMasterAxiIdSize'(reqq_state_i[rspmux_info[g].reqq_id].tag_id);
    assign rspmux_info[g].rsp.port_id = reqq_state_i[rspmux_info[g].reqq_id].port_id[MaxL3SlavePortIdSize-1:0];
    assign rspmux_info[g].rsp.source = MeshSourceBridgeIdSize'(reqq_state_i[rspmux_info[g].reqq_id].source);
    assign rspmux_info[g].rsp.err    = reqq_state_i[rspmux_info[g].reqq_id].err_detected
                                       & esr_ctl_i.esr_sc_err_rsp_enable;
  end

  // Round-robin arbiter
  logic mesh_rsp_fifo_full;
  mesh_slave_rsp_long_t rspmux_winner_info;

  prim_arb_rr #(
    .NumReqs(NumRspSrc),
    .Width($bits(mesh_slave_rsp_long_t))
  ) u_rsp_arb (
    .clk_i   (clk_i),
    .rst_ni  (rst_ni),
    .reqs_i  (rspmux_valid),
    .grants_o(rspmux_grant),
    .pop_i   (~mesh_rsp_fifo_full),
    .data_i  (rspmux_info),
    .data_o  (rspmux_winner_info)
  );

  assign rspmux_ready = rspmux_grant & {NumRspSrc{~mesh_rsp_fifo_full}};

  // Sent notification
  logic [NumEntriesId-1:0] winner_id;
  assign winner_id = rspmux_winner_info.reqq_id;
  assign rspmux_sent_valid_o = (|rspmux_valid) & ~mesh_rsp_fifo_full;
  assign rspmux_sent_id_o    = winner_id;

  always_comb begin
    rspmux_sent_valid_hot_o = '0;
    rspmux_sent_valid_hot_o[winner_id] = rspmux_sent_valid_o;
  end

  // Map winner to output response
  mesh_slave_rsp_t l3_rsp_pre;
  assign l3_rsp_pre = rspmux_winner_info.rsp;

  // ──────────────────────────────────────────────────────────
  // Response FIFO (depth 2)
  // ──────────────────────────────────────────────────────────

  logic [2:0] mesh_rsp_fifo_level_unused;
  prim_fifo_reg #(
    .Width($bits(mesh_slave_rsp_t)),
    .Depth(2)
  ) u_mesh_rsp_fifo (
    .clk_i  (clk_i),
    .rst_ni (rst_ni),
    .push_i (|rspmux_valid & ~mesh_rsp_fifo_full),
    .wdata_i(l3_rsp_pre),
    .full_o (mesh_rsp_fifo_full),
    .level_o(mesh_rsp_fifo_level_unused),
    .pop_i  (l3_mesh_slave_bank_rsp_ready_i),
    .valid_o(l3_mesh_slave_bank_rsp_valid_o),
    .rdata_o(l3_mesh_slave_bank_rsp_info_o)
  );

  assign a_rspmux_rsp_is_valid_o = l3_mesh_slave_bank_rsp_valid_o;

  // ══════════════════════════════════════════════════════════
  // Unused signal aggregation
  // ══════════════════════════════════════════════════════════

  logic unused_ok;
  assign unused_ok = &{1'b0,
    mesh_req_fifo_level_unused,
    mesh_rsp_fifo_level_unused,
    pipe_data_rsp_info_i.tag_sbe,
    pipe_data_rsp_info_i.qwen,
    pipe_data_rsp_info_i.data_index,
    pipe_data_rsp_modifier_info_i.trans_id,
    pipe_data_rsp_modifier_info_i.reqq_id,
    pipe_data_rsp_modifier_info_i.dataq_id,
    pipe_data_rsp_modifier_info_i.address_space,
    pipe_data_rsp_modifier_info_i.which_mesh,
    pipe_data_rsp_modifier_info_i.disable_mesh_bypass,
    pipe_data_rsp_modifier_info_i.port_id,
    pipe_data_rsp_modifier_info_i.rsp_type,
    to_sys_mesh_rsp_info_i.dataq_id,
    to_sys_mesh_rsp_info_i.zero_data,
    to_sys_mesh_rsp_info_i.err,
    reqq_rspmux_info_i.with_data,
    reqq_rspmux_info_i.data,
    reqq_rspmux_info_i.qwen,
    reqq_rspmux_info_i.zero_data,
    dataq_rspmux_info_i.with_data,
    dataq_rspmux_info_i.qwen,
    dataq_rspmux_info_i.zero_data,
    esr_ctl_i,
    reqq_state_i,
    unused_ok};

  /* verilator lint_on UNUSEDSIGNAL */

endmodule
