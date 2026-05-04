// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire cache mesh slave AXI port — per-port CDC bridge and AXI channel mapping.
//
// Request path: AXI AR/AW/W channels cross from NOC to shire clock via async
// FIFOs, then AR and AW/W are arbitrated and mapped to mesh_slave_req_t.
// L3 swizzle determines target bank for routing.
// Response path: mesh_slave_rsp_t is mapped to AXI R/B channels, then crosses
// from shire clock to NOC clock via async FIFOs.
//
// Replaces: shire_cache_mesh_slave_axi_port (etcore-soc)

/* verilator lint_off UNUSEDPARAM */
/* verilator lint_off UNUSEDSIGNAL */
module shirecache_mesh_slave_axi_port
  import shirecache_pkg::*;
  import axi_pkg::*;
  import etlink_pkg::*;
#(
  parameter int unsigned NumBanks = Banks,
  parameter int unsigned PortId   = 0
) (
  input  logic                        clk_i,        // shire clock
  input  logic                        rst_ni,       // shire reset
  input  logic                        noc_clk_i,    // NOC clock
  input  logic                        noc_rst_ni,   // NOC reset
  input  dft_pkg::dft_t               dft_i,
  input  l3_swizzle_ctl_t             esr_sc_l3_shire_swizzle_ctl_i,

  // Request to bank arbiters
  input  logic                        port_req_ready_i,
  output logic [NumBanks-1:0]         port_req_valid_o,
  output mesh_slave_req_t             port_req_info_o,

  // R response from bank arbiter
  output logic                        port_r_rsp_ready_o,
  input  logic                        port_r_rsp_valid_i,
  input  mesh_slave_rsp_t             port_r_rsp_info_i,

  // B response from bank arbiter
  output logic                        port_b_rsp_ready_o,
  input  logic                        port_b_rsp_valid_i,
  input  mesh_slave_rsp_t             port_b_rsp_info_i,

  // AXI slave port
  output logic                        axi_ar_ready_o,
  input  logic                        axi_ar_valid_i,
  input  sc_slave_ar_t                axi_ar_i,

  output logic                        axi_aw_ready_o,
  input  logic                        axi_aw_valid_i,
  input  sc_slave_aw_t                axi_aw_i,

  output logic                        axi_w_ready_o,
  input  logic                        axi_w_valid_i,
  input  sc_master_w_t                axi_w_i,

  input  logic                        axi_r_ready_i,
  output logic                        axi_r_valid_o,
  output sc_slave_r_t                 axi_r_o,

  input  logic                        axi_b_ready_i,
  output logic                        axi_b_valid_o,
  output sc_slave_b_t                 axi_b_o
);

  localparam int unsigned BankIdSizeLocal = $clog2(NumBanks);
  localparam int unsigned AsyncDepth = 2 * CdcSyncStages + 2;  // 6 — matches original vcfifo depth

  // ══════════════════════════════════════════════════════════
  // Request CDC FIFOs (NOC → shire)
  // ══════════════════════════════════════════════════════════

  // AR FIFO
  logic            ar_fifo_ready, ar_fifo_valid;
  sc_slave_ar_t    ar_fifo_ar;

  prim_fifo_async_lov #(
    .Width($bits(sc_slave_ar_t)), .Depth(AsyncDepth), .SyncStages(CdcSyncStages)
  ) u_ar_fifo (
    .clk_wr_i(noc_clk_i), .rst_wr_ni(noc_rst_ni),
    .push_i(axi_ar_valid_i), .wdata_i(axi_ar_i), .ready_o(ar_fifo_ready),
    .clk_rd_i(clk_i), .rst_rd_ni(rst_ni),
    .pop_i(ar_fifo_pop), .rdata_o(ar_fifo_ar), .valid_o(ar_fifo_valid),
    .dft_lv_i (dft_i), .dft_hv_i (dft_i)
  );
  assign axi_ar_ready_o = ar_fifo_ready;

  // AW FIFO
  logic            aw_fifo_ready, aw_fifo_valid;
  sc_slave_aw_t    aw_fifo_aw;

  prim_fifo_async_lov #(
    .Width($bits(sc_slave_aw_t)), .Depth(AsyncDepth), .SyncStages(CdcSyncStages)
  ) u_aw_fifo (
    .clk_wr_i(noc_clk_i), .rst_wr_ni(noc_rst_ni),
    .push_i(axi_aw_valid_i), .wdata_i(axi_aw_i), .ready_o(aw_fifo_ready),
    .clk_rd_i(clk_i), .rst_rd_ni(rst_ni),
    .pop_i(aw_fifo_pop), .rdata_o(aw_fifo_aw), .valid_o(aw_fifo_valid),
    .dft_lv_i (dft_i), .dft_hv_i (dft_i)
  );
  assign axi_aw_ready_o = aw_fifo_ready;

  // W FIFO
  logic            w_fifo_ready, w_fifo_valid;
  sc_master_w_t    w_fifo_w;

  prim_fifo_async_lov #(
    .Width($bits(sc_master_w_t)), .Depth(AsyncDepth), .SyncStages(CdcSyncStages)
  ) u_w_fifo (
    .clk_wr_i(noc_clk_i), .rst_wr_ni(noc_rst_ni),
    .push_i(axi_w_valid_i), .wdata_i(axi_w_i), .ready_o(w_fifo_ready),
    .clk_rd_i(clk_i), .rst_rd_ni(rst_ni),
    .pop_i(w_fifo_pop), .rdata_o(w_fifo_w), .valid_o(w_fifo_valid),
    .dft_lv_i (dft_i), .dft_hv_i (dft_i)
  );
  assign axi_w_ready_o = w_fifo_ready;

  // AW+W valid only when both present
  logic aww_fifo_valid, aww_fifo_pop;
  assign aww_fifo_valid = aw_fifo_valid & w_fifo_valid;

  logic ar_fifo_pop, aw_fifo_pop, w_fifo_pop;
  assign aw_fifo_pop = aww_fifo_pop;
  assign w_fifo_pop  = aww_fifo_pop;

  // ══════════════════════════════════════════════════════════
  // Map FIFO outputs to mesh_slave_req_t
  // ══════════════════════════════════════════════════════════

  mesh_slave_req_t port_ar_req;
  always_comb begin
    port_ar_req.trans_id = '0;
    port_ar_req.id       = ar_fifo_ar.id[ScMasterIdSize-1:0];
    port_ar_req.port_id  = MaxL3SlavePortIdSize'(PortId);
    port_ar_req.source   = ar_fifo_ar.id[ScSlaveIdSize-1 -: MeshSourceBridgeIdSize];
    port_ar_req.opcode   = ReqRead;
    port_ar_req.address  = ar_fifo_ar.addr;
    port_ar_req.size     = sc_size_t'(ar_fifo_ar.size);
    port_ar_req.qos      = ar_fifo_ar.qos;
    port_ar_req.wdata    = 1'b0;
    port_ar_req.qwen     = '0;
    port_ar_req.data     = '0;
    port_ar_req.ben      = '0;
    port_ar_req.ben_en   = 1'b0;
  end

  mesh_slave_req_t port_aw_req;
  always_comb begin
    port_aw_req.trans_id = '0;
    port_aw_req.id       = aw_fifo_aw.id[ScMasterIdSize-1:0];
    port_aw_req.port_id  = MaxL3SlavePortIdSize'(PortId);
    port_aw_req.source   = aw_fifo_aw.id[ScSlaveIdSize-1 -: MeshSourceBridgeIdSize];
    port_aw_req.opcode   = req_opcode_e'(aw_fifo_aw.user[$bits(req_opcode_e)-1:0]);
    port_aw_req.address  = aw_fifo_aw.addr;
    port_aw_req.size     = sc_size_t'(aw_fifo_aw.size);
    port_aw_req.qos      = aw_fifo_aw.qos;
    port_aw_req.wdata    = 1'b1;
    port_aw_req.qwen     = {|w_fifo_w.strb[48 +: 16],
                            |w_fifo_w.strb[32 +: 16],
                            |w_fifo_w.strb[16 +: 16],
                            |w_fifo_w.strb[ 0 +: 16]};
    port_aw_req.data     = w_fifo_w.data;
    port_aw_req.ben      = w_fifo_w.strb;
    // ben_en: some but not all byte enables within a quadword
    port_aw_req.ben_en   = (|w_fifo_w.strb[48 +: 16] & ~&w_fifo_w.strb[48 +: 16]) |
                           (|w_fifo_w.strb[32 +: 16] & ~&w_fifo_w.strb[32 +: 16]) |
                           (|w_fifo_w.strb[16 +: 16] & ~&w_fifo_w.strb[16 +: 16]) |
                           (|w_fifo_w.strb[ 0 +: 16] & ~&w_fifo_w.strb[ 0 +: 16]);
  end

  // ══════════════════════════════════════════════════════════
  // AR / AW+W arbitration → request FIFO with bank routing
  // ══════════════════════════════════════════════════════════

  logic ar_fifo_grant, aww_fifo_grant;
  mesh_slave_req_t arb_req_info;
  logic arb_req_pop;

  prim_arb_rr #(.NumReqs(2), .Width($bits(mesh_slave_req_t))) u_req_arb (
    .clk_i(clk_i), .rst_ni,
    .reqs_i ({aww_fifo_valid, ar_fifo_valid}),
    .grants_o({aww_fifo_grant, ar_fifo_grant}),
    .pop_i  (arb_req_pop),
    .data_i ({port_aw_req, port_ar_req}),
    .data_o (arb_req_info)
  );

  assign aww_fifo_pop = aww_fifo_grant & arb_req_pop;
  assign ar_fifo_pop  = ar_fifo_grant  & arb_req_pop;
  logic arb_req_valid;
  assign arb_req_valid = ar_fifo_valid | aww_fifo_valid;

  // L3 swizzle for bank routing
  logic [SubBankIdSize-1:0] l3_sub_bank_id_unused;
  logic [BankIdSize-1:0]    l3_bank_id;
  logic [L3ShireIdSize-1:0] l3_shire_id_unused;

  shirecache_pipe_l3_swizzle_get u_l3_swizzle (
    .address_i     (arb_req_info.address),
    .swizzle_ctl_i (esr_sc_l3_shire_swizzle_ctl_i),
    .sub_bank_id_o (l3_sub_bank_id_unused),
    .bank_id_o     (l3_bank_id),
    .shire_id_o    (l3_shire_id_unused)
  );

  logic arb_addr_is_remote_scp;
  assign arb_addr_is_remote_scp =
    (arb_req_info.address[ScpRegionLsb +: ScpRegionIdSize] == ScpRegionId);

  logic [BankIdSizeLocal-1:0] arb_bank_id;
  assign arb_bank_id = arb_addr_is_remote_scp
    ? arb_req_info.address[BankLsb +: BankIdSizeLocal]
    : BankIdSizeLocal'(l3_bank_id);

  logic [NumBanks-1:0] arb_bank_hot;
  assign arb_bank_hot = NumBanks'(1'b1) << arb_bank_id;

  // Request FIFO (depth 2) with bank routing bits
  localparam int unsigned ReqFifoW = NumBanks + $bits(mesh_slave_req_t);
  logic [ReqFifoW-1:0] req_fifo_wdata, req_fifo_rdata;
  logic req_fifo_full, req_fifo_valid;
  logic [2:0] req_fifo_level_unused;

  assign req_fifo_wdata = {arb_bank_hot, arb_req_info};

  prim_fifo_reg #(.Width(ReqFifoW), .Depth(2)) u_req_fifo (
    .clk_i(clk_i), .rst_ni,
    .push_i(arb_req_valid & ~req_fifo_full),
    .wdata_i(req_fifo_wdata), .full_o(req_fifo_full),
    .level_o(req_fifo_level_unused),
    .pop_i(port_req_ready_i),
    .valid_o(req_fifo_valid), .rdata_o(req_fifo_rdata)
  );

  logic [NumBanks-1:0]   req_fifo_bank_hot;
  mesh_slave_req_t       req_fifo_info;
  assign {req_fifo_bank_hot, req_fifo_info} = req_fifo_rdata;

  assign arb_req_pop     = ~req_fifo_full;
  assign port_req_info_o = req_fifo_info;
  assign port_req_valid_o = {NumBanks{req_fifo_valid}} & req_fifo_bank_hot;

  // ══════════════════════════════════════════════════════════
  // Response mapping → AXI R/B
  // ══════════════════════════════════════════════════════════

  sc_slave_r_t axi_r;
  sc_slave_b_t axi_b;

  always_comb begin
    axi_r.id   = {port_r_rsp_info_i.source, port_r_rsp_info_i.id};
    axi_r.resp = port_r_rsp_info_i.err ? RespSlvErr : RespOkay;
    axi_r.last = 1'b1;
    axi_r.data = port_r_rsp_info_i.data;
    axi_b.id   = {port_b_rsp_info_i.source, port_b_rsp_info_i.id};
    axi_b.resp = port_b_rsp_info_i.err ? RespSlvErr : RespOkay;
  end

  // ══════════════════════════════════════════════════════════
  // Response CDC FIFOs (shire → NOC)
  // ══════════════════════════════════════════════════════════

  // R FIFO
  logic r_fifo_ready, r_fifo_valid;

  prim_fifo_async_hiv #(
    .Width($bits(sc_slave_r_t)), .Depth(AsyncDepth), .SyncStages(CdcSyncStages)
  ) u_r_fifo (
    .clk_wr_i(clk_i), .rst_wr_ni(rst_ni),
    .push_i(port_r_rsp_valid_i), .wdata_i(axi_r), .ready_o(r_fifo_ready),
    .clk_rd_i(noc_clk_i), .rst_rd_ni(noc_rst_ni),
    .pop_i(axi_r_ready_i), .rdata_o(axi_r_o), .valid_o(r_fifo_valid),
    .dft_hv_i (dft_i), .dft_lv_i (dft_i)
  );
  assign port_r_rsp_ready_o = r_fifo_ready;
  assign axi_r_valid_o      = r_fifo_valid;

  // B FIFO
  logic b_fifo_ready, b_fifo_valid;

  prim_fifo_async_hiv #(
    .Width($bits(sc_slave_b_t)), .Depth(AsyncDepth), .SyncStages(CdcSyncStages)
  ) u_b_fifo (
    .clk_wr_i(clk_i), .rst_wr_ni(rst_ni),
    .push_i(port_b_rsp_valid_i), .wdata_i(axi_b), .ready_o(b_fifo_ready),
    .clk_rd_i(noc_clk_i), .rst_rd_ni(noc_rst_ni),
    .pop_i(axi_b_ready_i), .rdata_o(axi_b_o), .valid_o(b_fifo_valid),
    .dft_hv_i (dft_i), .dft_lv_i (dft_i)
  );
  assign port_b_rsp_ready_o = b_fifo_ready;
  assign axi_b_valid_o      = b_fifo_valid;

  // ══════════════════════════════════════════════════════════
  // Unused signals
  // ══════════════════════════════════════════════════════════

  logic unused_ok;
  assign unused_ok = &{1'b0,
    req_fifo_level_unused,
    port_r_rsp_info_i.port_id,
    port_r_rsp_info_i.wdata,
    port_b_rsp_info_i.port_id,
    port_b_rsp_info_i.wdata,
    port_b_rsp_info_i.data,
    ar_fifo_ar.len, ar_fifo_ar.burst, ar_fifo_ar.lock,
    ar_fifo_ar.cache, ar_fifo_ar.prot, ar_fifo_ar.user,
    aw_fifo_aw.len, aw_fifo_aw.burst, aw_fifo_aw.lock,
    aw_fifo_aw.cache, aw_fifo_aw.prot,
    w_fifo_w.last, w_fifo_w.strb,
    unused_ok};

  /* verilator lint_on UNUSEDSIGNAL */
  /* verilator lint_on UNUSEDPARAM */

endmodule
