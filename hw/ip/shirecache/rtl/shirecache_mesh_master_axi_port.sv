// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire cache mesh master AXI port — per-port CDC bridge and AXI channel mapping.
//
// Request path: maps mesh_master_req_t → AXI AR/AW/W channels, then crosses
// from shire clock to NOC clock via async FIFOs.
// Response path: AXI R/B channels cross from NOC to shire clock via async FIFOs,
// then R/B are arbitrated and mapped back to mesh_master_rsp_t.
//
// Replaces: shire_cache_mesh_master_axi_port (etcore-soc)

/* verilator lint_off UNUSEDPARAM */
/* verilator lint_off UNUSEDSIGNAL */
module shirecache_mesh_master_axi_port
  import shirecache_pkg::*;
  import axi_pkg::*;
  import etlink_pkg::*;
#(
  parameter bit          IsToL3     = 1,
  parameter int unsigned NumBanks = Banks,
  parameter int unsigned PortId   = 0
) (
  input  logic                        clk_i,        // shire clock (ungated, for assertions)
  input  logic                        clk_g_i,      // shire clock (gated)
  input  logic                        rst_ni,       // shire reset
  input  logic                        noc_clk_i,    // NOC clock
  input  logic                        noc_clk_g_i,  // NOC clock (gated)
  input  logic                        noc_rst_ni,   // NOC reset
  input  dft_pkg::dft_t               dft_i,

  // Port AR request (from arbiter)
  output logic                        port_ar_req_ready_o,
  input  logic                        port_ar_req_valid_i,
  input  mesh_master_req_t            port_ar_req_info_i,

  // Port AW request (from arbiter)
  output logic                        port_aw_req_ready_o,
  input  logic                        port_aw_req_valid_i,
  input  mesh_master_req_t            port_aw_req_info_i,

  // Port response (to bank rsp arbiters)
  input  logic                        port_rsp_ready_i,
  output logic [NumBanks-1:0]         port_rsp_valid_o,
  output mesh_master_rsp_t            port_rsp_info_o,

  // AXI master port
  input  logic                        axi_ar_ready_i,
  output logic                        axi_ar_valid_o,
  output sc_master_ar_t               axi_ar_o,

  input  logic                        axi_aw_ready_i,
  output logic                        axi_aw_valid_o,
  output sc_master_aw_t               axi_aw_o,

  input  logic                        axi_w_ready_i,
  output logic                        axi_w_valid_o,
  output sc_master_w_t                axi_w_o,

  output logic                        axi_r_ready_o,
  input  logic                        axi_r_valid_i,
  input  sc_master_r_t                axi_r_i,

  output logic                        axi_b_ready_o,
  input  logic                        axi_b_valid_i,
  input  sc_master_b_t                axi_b_i
);

  /* verilator lint_off UNUSEDSIGNAL */
  /* verilator lint_off UNUSEDPARAM */

  // ══════════════════════════════════════════════════════════
  // AXI channel construction from mesh request
  // ══════════════════════════════════════════════════════════

  sc_master_ar_t axi_ar;
  sc_master_aw_t axi_aw;
  sc_master_w_t  axi_w;

  // AR channel
  assign axi_ar.id    = ScMasterIdSize'({port_ar_req_info_i.source, port_ar_req_info_i.id});
  assign axi_ar.addr  = port_ar_req_info_i.address;
  assign axi_ar.len   = ScMasterAxLen;
  assign axi_ar.size  = port_ar_req_info_i.size;
  assign axi_ar.burst = ScMasterAxBurst;
  assign axi_ar.lock  = 1'b0;
  assign axi_ar.cache = ScMasterAxCache;
  assign axi_ar.prot  = ScMasterAxProt;
  assign axi_ar.qos   = port_ar_req_info_i.qos;
  assign axi_ar.user  = '0;

  // AW channel
  logic port_aw_req_atomic_opcode;
  logic [L3AtomicTransIdSize-1:0] port_aw_req_atomic_trans_id;
  assign port_aw_req_atomic_opcode   = (port_aw_req_info_i.opcode == ReqAtomicRsp);
  assign port_aw_req_atomic_trans_id = port_aw_req_info_i.data[L3AtomicTransIdLsb +: L3AtomicTransIdSize];

  assign axi_aw.id    = ScMasterIdSize'({port_aw_req_info_i.source, port_aw_req_info_i.id});
  assign axi_aw.addr  = port_aw_req_info_i.address;
  assign axi_aw.len   = ScMasterAxLen;
  assign axi_aw.size  = port_aw_req_info_i.size;
  assign axi_aw.burst = ScMasterAxBurst;
  assign axi_aw.lock  = 1'b0;
  assign axi_aw.cache = ScMasterAxCache;
  assign axi_aw.prot  = ScMasterAxProt;
  assign axi_aw.qos   = IsToL3 ? port_aw_req_info_i.qos
                              : (port_aw_req_atomic_opcode ? AxiQosAtomicRsp : port_aw_req_info_i.qos);
  assign axi_aw.user  = IsToL3 ? ScMasterAwUserSize'(port_aw_req_info_i.opcode)
                              : (port_aw_req_atomic_opcode
                                   ? ScMasterAwUserSize'(port_aw_req_atomic_trans_id)
                                   : ScMasterAwUserSize'(port_aw_req_info_i.opcode));

  // W channel
  assign axi_w.data = port_aw_req_atomic_opcode
    ? {(LineDataSize/AtomicDataSize){port_aw_req_info_i.data[0 +: AtomicDataSize]}}
    : port_aw_req_info_i.data;

  always_comb begin
    if (port_aw_req_info_i.ben_en) begin
      axi_w.strb = port_aw_req_info_i.ben;
    end else if (port_aw_req_info_i.opcode == ReqAtomicRsp) begin
      axi_w.strb = gen_strb(port_aw_req_info_i.size, port_aw_req_info_i.address);
    end else begin
      for (int qw = 0; qw < LineQwSize; qw++) begin
        axi_w.strb[qw*16 +: 16] = {16{port_aw_req_info_i.qwen[qw]}};
      end
    end
  end
  assign axi_w.last = 1'b1;

  // ══════════════════════════════════════════════════════════
  // Request CDC FIFOs (shire → NOC)
  // ══════════════════════════════════════════════════════════

  localparam int unsigned AsyncDepth   = 2 * CdcSyncStages + 2;  // 6 — matches original vcfifo depth

  // AR FIFO
  logic                          ar_fifo_ready;
  logic                          ar_fifo_valid;
  sc_master_ar_t                 ar_fifo_ar;

  // AR/AW/W: shire(HV) → NOC(LV) — write=HV → prim_fifo_async_hiv
  prim_fifo_async_hiv #(
    .Width($bits(sc_master_ar_t)), .Depth(AsyncDepth), .SyncStages(CdcSyncStages)
  ) u_ar_fifo (
    .clk_wr_i (clk_g_i),      .rst_wr_ni(rst_ni),
    .push_i   (port_ar_req_valid_i),
    .wdata_i  (axi_ar),
    .ready_o  (ar_fifo_ready),
    .clk_rd_i (noc_clk_i),    .rst_rd_ni(noc_rst_ni),
    .pop_i    (axi_ar_ready_i),
    .rdata_o  (ar_fifo_ar),
    .valid_o  (ar_fifo_valid),
    .dft_hv_i (dft_i),        .dft_lv_i (dft_i)
  );

  assign port_ar_req_ready_o = ar_fifo_ready;
  assign axi_ar_valid_o      = ar_fifo_valid;
  assign axi_ar_o            = ar_fifo_ar;

  // AW FIFO (push gated by port_aw_req_ready to keep AW/W in sync)
  logic aw_fifo_ready;
  logic aw_fifo_valid;
  sc_master_aw_t aw_fifo_aw;

  prim_fifo_async_hiv #(
    .Width($bits(sc_master_aw_t)), .Depth(AsyncDepth), .SyncStages(CdcSyncStages)
  ) u_aw_fifo (
    .clk_wr_i (clk_g_i),      .rst_wr_ni(rst_ni),
    .push_i   (port_aw_req_valid_i & port_aw_req_ready_o),
    .wdata_i  (axi_aw),
    .ready_o  (aw_fifo_ready),
    .clk_rd_i (noc_clk_i),    .rst_rd_ni(noc_rst_ni),
    .pop_i    (axi_aw_ready_i),
    .rdata_o  (aw_fifo_aw),
    .valid_o  (aw_fifo_valid),
    .dft_hv_i (dft_i),        .dft_lv_i (dft_i)
  );

  assign axi_aw_valid_o = aw_fifo_valid;
  assign axi_aw_o       = aw_fifo_aw;

  // W FIFO (push gated same as AW)
  logic w_fifo_ready;
  logic w_fifo_valid;
  sc_master_w_t w_fifo_w;

  prim_fifo_async_hiv #(
    .Width($bits(sc_master_w_t)), .Depth(AsyncDepth), .SyncStages(CdcSyncStages)
  ) u_w_fifo (
    .clk_wr_i (clk_g_i),      .rst_wr_ni(rst_ni),
    .push_i   (port_aw_req_valid_i & port_aw_req_ready_o),
    .wdata_i  (axi_w),
    .ready_o  (w_fifo_ready),
    .clk_rd_i (noc_clk_i),    .rst_rd_ni(noc_rst_ni),
    .pop_i    (axi_w_ready_i),
    .rdata_o  (w_fifo_w),
    .valid_o  (w_fifo_valid),
    .dft_hv_i (dft_i),        .dft_lv_i (dft_i)
  );

  assign port_aw_req_ready_o = aw_fifo_ready & w_fifo_ready;
  assign axi_w_valid_o       = w_fifo_valid;
  assign axi_w_o             = w_fifo_w;

  // ══════════════════════════════════════════════════════════
  // Response CDC FIFOs (NOC → shire)
  // R/B: NOC(LV) → shire(HV) — write=LV → prim_fifo_async_lov
  // ══════════════════════════════════════════════════════════

  // R FIFO
  logic            r_fifo_ready;
  logic            r_fifo_valid;
  sc_master_r_t    r_fifo_r;

  prim_fifo_async_lov #(
    .Width($bits(sc_master_r_t)), .Depth(AsyncDepth), .SyncStages(CdcSyncStages)
  ) u_r_fifo (
    .clk_wr_i (noc_clk_g_i),  .rst_wr_ni(noc_rst_ni),
    .push_i   (axi_r_valid_i),
    .wdata_i  (axi_r_i),
    .ready_o  (r_fifo_ready),
    .clk_rd_i (clk_g_i),      .rst_rd_ni(rst_ni),
    .pop_i    (r_fifo_pop),
    .rdata_o  (r_fifo_r),
    .valid_o  (r_fifo_valid),
    .dft_lv_i (dft_i),        .dft_hv_i (dft_i)
  );

  assign axi_r_ready_o = r_fifo_ready;

  // B FIFO
  logic            b_fifo_ready;
  logic            b_fifo_valid;
  sc_master_b_t    b_fifo_b;

  prim_fifo_async_lov #(
    .Width($bits(sc_master_b_t)), .Depth(AsyncDepth), .SyncStages(CdcSyncStages)
  ) u_b_fifo (
    .clk_wr_i (noc_clk_g_i),  .rst_wr_ni(noc_rst_ni),
    .push_i   (axi_b_valid_i),
    .wdata_i  (axi_b_i),
    .ready_o  (b_fifo_ready),
    .clk_rd_i (clk_g_i),      .rst_rd_ni(rst_ni),
    .pop_i    (b_fifo_pop),
    .rdata_o  (b_fifo_b),
    .valid_o  (b_fifo_valid),
    .dft_lv_i (dft_i),        .dft_hv_i (dft_i)
  );

  assign axi_b_ready_o = b_fifo_ready;

  // ══════════════════════════════════════════════════════════
  // Response mapping + R/B arbitration
  // ══════════════════════════════════════════════════════════

  // Map R FIFO output to mesh_master_rsp_t
  mesh_master_rsp_t r_fifo_pkt;
  assign r_fifo_pkt.trans_id = '0;
  assign {r_fifo_pkt.source, r_fifo_pkt.id} = r_fifo_r.id[MeshMasterSourceSize + ReqqIdSize - 1 : 0];
  assign r_fifo_pkt.port_id  = '0;
  assign r_fifo_pkt.err      = (r_fifo_r.resp != RespOkay);
  assign r_fifo_pkt.wdata    = 1'b1;   // R channel always has data
  // NOTE: B response uses R data to avoid a mux (matching original)
  assign r_fifo_pkt.data     = r_fifo_r.data;

  // Map B FIFO output to mesh_master_rsp_t
  mesh_master_rsp_t b_fifo_pkt;
  assign b_fifo_pkt.trans_id = '0;
  assign {b_fifo_pkt.source, b_fifo_pkt.id} = b_fifo_b.id[MeshMasterSourceSize + ReqqIdSize - 1 : 0];
  assign b_fifo_pkt.port_id  = '0;
  assign b_fifo_pkt.err      = (b_fifo_b.resp != RespOkay);
  assign b_fifo_pkt.wdata    = 1'b0;   // B channel never has data
  assign b_fifo_pkt.data     = r_fifo_r.data;  // Reuse R data (no mux, matching original)

  // Round-robin R/B arbitration
  logic r_fifo_grant, b_fifo_grant;
  logic r_fifo_pop, b_fifo_pop;

  prim_arb_rr #(
    .NumReqs(2),
    .Width($bits(mesh_master_rsp_t))
  ) u_rsp_arb (
    .clk_i   (clk_g_i),
    .rst_ni  (rst_ni),
    .reqs_i  ({b_fifo_valid, r_fifo_valid}),
    .grants_o({b_fifo_grant, r_fifo_grant}),
    .pop_i   (port_rsp_ready_i),
    .data_i  ({b_fifo_pkt, r_fifo_pkt}),
    .data_o  (port_rsp_info_o)
  );

  assign r_fifo_pop = r_fifo_grant & port_rsp_ready_i;
  assign b_fifo_pop = b_fifo_grant & port_rsp_ready_i;

  logic any_rsp_valid;
  assign any_rsp_valid = r_fifo_valid | b_fifo_valid;
  assign port_rsp_valid_o = {NumBanks{any_rsp_valid}} & (NumBanks'(1'b1) << port_rsp_info_o.source);

  // ══════════════════════════════════════════════════════════
  // gen_strb function — generate byte enables from size + address
  // ══════════════════════════════════════════════════════════

  function automatic logic [LineByteSize-1:0] gen_strb(
    input sc_size_t             size_in,
    input logic [PaSize-1:0]    addr_in
  );
    logic [LineByteSize-1:0] size_bits;
    unique case (size_in)
      SizeByte:  size_bits = LineByteSize'(1);
      SizeHWord: size_bits = LineByteSize'(3);
      SizeWord:  size_bits = LineByteSize'(15);
      SizeDWord: size_bits = LineByteSize'({8{1'b1}});
      SizeQWord: size_bits = LineByteSize'({16{1'b1}});
      SizeHLine: size_bits = LineByteSize'({32{1'b1}});
      SizeLine:  size_bits = {LineByteSize{1'b1}};
      default:   size_bits = {LineByteSize{1'b1}};
    endcase
    gen_strb = size_bits << addr_in[LineOffsetIdSize-1:0];
  endfunction

  // ══════════════════════════════════════════════════════════
  // Unused signals
  // ══════════════════════════════════════════════════════════

  logic unused_ok;
  assign unused_ok = &{1'b0,
    clk_i,              // only used for assertions in original
    r_fifo_r.last,      // always 1 (single beat)
    r_fifo_r.id,        // not all bits may be used
    b_fifo_b.id,        // not all bits may be used
    port_ar_req_info_i.port_id,
    port_ar_req_info_i.opcode,
    port_ar_req_info_i.wdata,
    port_ar_req_info_i.qwen,
    port_ar_req_info_i.data,
    port_ar_req_info_i.ben_en,
    port_ar_req_info_i.ben,
    port_aw_req_info_i.port_id,
    port_aw_req_info_i.wdata,
    unused_ok};

  /* verilator lint_on UNUSEDPARAM */
  /* verilator lint_on UNUSEDSIGNAL */

endmodule
