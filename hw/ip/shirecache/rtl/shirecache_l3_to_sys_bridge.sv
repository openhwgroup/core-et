// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire cache L3-to-sys bridge — bypass bridge from L3 slave to sys master.
//
// Passes Writes, Reads, and Atomics through to the sys mesh master.
// Auto-acks all other requests (Flush, Evict, Prefetch) with no operation.
// FlushToMem and EvictToMem are converted to Write before forwarding to sys.
//
// Per-port L3 slave AXI async FIFOs (NOC→shire for AR/AW/W, shire→NOC for R/B).
// Multi-port request arbitration → ID remapping via 64-entry reqq →
// single sys master AXI async FIFOs (shire→NOC for AR/AW/W, NOC→shire for R/B).
// Response path: sys R/B → look up original L3 AXI ID from reqq → route to port.
//
// Replaces: shire_cache_l3_to_sys_bridge (etcore-soc)

/* verilator lint_off UNUSEDPARAM */
/* verilator lint_off UNUSEDSIGNAL */
module shirecache_l3_to_sys_bridge
  import shirecache_pkg::*;
  import axi_pkg::*;
  import etlink_pkg::*;
#(
  parameter int unsigned NumL3SlavePorts = L3SlavePorts
) (
  input  logic                                              clk_i,
  input  logic                                              rst_ni,
  input  logic                                              noc_clk_i,
  input  logic                                              noc_rst_ni,
  input  dft_pkg::dft_t                                     dft_i,

  // L3 slave AXI ports (per-port, NOC domain)
  input  logic [NumL3SlavePorts-1:0]                        l3_ar_valid_i,
  output logic [NumL3SlavePorts-1:0]                        l3_ar_ready_o,
  input  sc_slave_ar_t [NumL3SlavePorts-1:0]                l3_ar_i,

  input  logic [NumL3SlavePorts-1:0]                        l3_aw_valid_i,
  output logic [NumL3SlavePorts-1:0]                        l3_aw_ready_o,
  input  sc_slave_aw_t [NumL3SlavePorts-1:0]                l3_aw_i,

  input  logic [NumL3SlavePorts-1:0]                        l3_w_valid_i,
  output logic [NumL3SlavePorts-1:0]                        l3_w_ready_o,
  input  sc_master_w_t [NumL3SlavePorts-1:0]                l3_w_i,

  output logic [NumL3SlavePorts-1:0]                        l3_r_valid_o,
  input  logic [NumL3SlavePorts-1:0]                        l3_r_ready_i,
  output sc_slave_r_t [NumL3SlavePorts-1:0]                 l3_r_o,

  output logic [NumL3SlavePorts-1:0]                        l3_b_valid_o,
  input  logic [NumL3SlavePorts-1:0]                        l3_b_ready_i,
  output sc_slave_b_t [NumL3SlavePorts-1:0]                 l3_b_o,

  // Sys master AXI port (single, NOC domain)
  output logic                                              sys_ar_valid_o,
  input  logic                                              sys_ar_ready_i,
  output sc_master_ar_t                                     sys_ar_o,

  output logic                                              sys_aw_valid_o,
  input  logic                                              sys_aw_ready_i,
  output sc_master_aw_t                                     sys_aw_o,

  output logic                                              sys_w_valid_o,
  input  logic                                              sys_w_ready_i,
  output sc_master_w_t                                      sys_w_o,

  input  logic                                              sys_r_valid_i,
  output logic                                              sys_r_ready_o,
  input  sc_master_r_t                                      sys_r_i,

  input  logic                                              sys_b_valid_i,
  output logic                                              sys_b_ready_o,
  input  sc_master_b_t                                      sys_b_i
);

  // ══════════════════════════════════════════════════════════
  // Parameters
  // ══════════════════════════════════════════════════════════

  localparam int unsigned PortIdSize  = (NumL3SlavePorts == 1) ? 1 : $clog2(NumL3SlavePorts);
  localparam int unsigned ReqqSize    = 64;
  localparam int unsigned ReqqIdSize_ = $clog2(ReqqSize);  // trailing _ to avoid pkg shadow
  localparam int unsigned FifoDepth   = 2 * CdcSyncStages + 2;  // 6

  // ══════════════════════════════════════════════════════════
  // L3 slave async FIFOs (per-port)
  // ══════════════════════════════════════════════════════════

  // AR/AW/W: NOC(LV) → shire(HV) = lov
  // R/B: shire(HV) → NOC(LV) = hiv
  logic [NumL3SlavePorts-1:0]          l3_ar_fifo_ready, l3_ar_fifo_valid, l3_ar_fifo_pop;
  sc_slave_ar_t [NumL3SlavePorts-1:0]  l3_ar_fifo_rdata;
  logic [NumL3SlavePorts-1:0]          l3_aw_fifo_ready, l3_aw_fifo_valid, l3_aw_fifo_pop;
  sc_slave_aw_t [NumL3SlavePorts-1:0]  l3_aw_fifo_rdata;
  logic [NumL3SlavePorts-1:0]          l3_w_fifo_ready, l3_w_fifo_valid, l3_w_fifo_pop;
  sc_master_w_t [NumL3SlavePorts-1:0]  l3_w_fifo_rdata;
  logic [NumL3SlavePorts-1:0]          l3_r_fifo_ready, l3_r_fifo_valid;
  logic [NumL3SlavePorts-1:0]          l3_r_fifo_push;
  sc_slave_r_t [NumL3SlavePorts-1:0]   l3_r_fifo_wdata;
  logic [NumL3SlavePorts-1:0]          l3_b_fifo_ready, l3_b_fifo_valid;
  logic [NumL3SlavePorts-1:0]          l3_b_fifo_push;
  sc_slave_b_t [NumL3SlavePorts-1:0]   l3_b_fifo_wdata;

  // Decoded signals from L3 FIFO outputs
  req_opcode_e [NumL3SlavePorts-1:0]   l3_aw_opcode;
  req_opcode_e [NumL3SlavePorts-1:0]   sys_aw_opcode;
  logic [NumL3SlavePorts-1:0]          l3_ar_req;
  logic [NumL3SlavePorts-1:0]          l3_aw_req;
  logic [NumL3SlavePorts-1:0]          l3_aw_auto_ack_req;
  logic [NumL3SlavePorts-1:0]          l3_aw_auto_ack_valid;
  logic [NumL3SlavePorts-1:0]          l3_aw_auto_ack_ready;
  logic [NumL3SlavePorts-1:0]          l3_aw_auto_ack_taken;
  logic [NumL3SlavePorts-1:0]          sys_r_port_valid;
  logic [NumL3SlavePorts-1:0]          sys_b_port_valid;

  for (genvar gp = 0; gp < NumL3SlavePorts; gp++) begin : gen_l3_port

    // AR FIFO (lov: NOC→shire)
    prim_fifo_async_lov #(
      .Width($bits(sc_slave_ar_t)), .Depth(FifoDepth), .SyncStages(CdcSyncStages)
    ) u_l3_ar_fifo (
      .clk_wr_i(noc_clk_i), .rst_wr_ni(noc_rst_ni),
      .push_i(l3_ar_valid_i[gp] & l3_ar_ready_o[gp]),
      .wdata_i(l3_ar_i[gp]), .ready_o(l3_ar_fifo_ready[gp]),
      .clk_rd_i(clk_i), .rst_rd_ni(rst_ni),
      .pop_i(l3_ar_fifo_pop[gp]), .rdata_o(l3_ar_fifo_rdata[gp]), .valid_o(l3_ar_fifo_valid[gp]),
      .dft_lv_i(dft_i), .dft_hv_i(dft_i)
    );

    // AW FIFO (lov)
    prim_fifo_async_lov #(
      .Width($bits(sc_slave_aw_t)), .Depth(FifoDepth), .SyncStages(CdcSyncStages)
    ) u_l3_aw_fifo (
      .clk_wr_i(noc_clk_i), .rst_wr_ni(noc_rst_ni),
      .push_i(l3_aw_valid_i[gp] & l3_aw_ready_o[gp]),
      .wdata_i(l3_aw_i[gp]), .ready_o(l3_aw_fifo_ready[gp]),
      .clk_rd_i(clk_i), .rst_rd_ni(rst_ni),
      .pop_i(l3_aw_fifo_pop[gp]), .rdata_o(l3_aw_fifo_rdata[gp]), .valid_o(l3_aw_fifo_valid[gp]),
      .dft_lv_i(dft_i), .dft_hv_i(dft_i)
    );

    // W FIFO (lov)
    prim_fifo_async_lov #(
      .Width($bits(sc_master_w_t)), .Depth(FifoDepth), .SyncStages(CdcSyncStages)
    ) u_l3_w_fifo (
      .clk_wr_i(noc_clk_i), .rst_wr_ni(noc_rst_ni),
      .push_i(l3_w_valid_i[gp] & l3_w_ready_o[gp]),
      .wdata_i(l3_w_i[gp]), .ready_o(l3_w_fifo_ready[gp]),
      .clk_rd_i(clk_i), .rst_rd_ni(rst_ni),
      .pop_i(l3_w_fifo_pop[gp]), .rdata_o(l3_w_fifo_rdata[gp]), .valid_o(l3_w_fifo_valid[gp]),
      .dft_lv_i(dft_i), .dft_hv_i(dft_i)
    );

    // R FIFO (hiv: shire→NOC)
    prim_fifo_async_hiv #(
      .Width($bits(sc_slave_r_t)), .Depth(FifoDepth), .SyncStages(CdcSyncStages)
    ) u_l3_r_fifo (
      .clk_wr_i(clk_i), .rst_wr_ni(rst_ni),
      .push_i(l3_r_fifo_push[gp]), .wdata_i(l3_r_fifo_wdata[gp]), .ready_o(l3_r_fifo_ready[gp]),
      .clk_rd_i(noc_clk_i), .rst_rd_ni(noc_rst_ni),
      .pop_i(l3_r_valid_o[gp] & l3_r_ready_i[gp]), .rdata_o(l3_r_o[gp]), .valid_o(l3_r_valid_o[gp]),
      .dft_hv_i(dft_i), .dft_lv_i(dft_i)
    );

    // B FIFO (hiv: shire→NOC)
    prim_fifo_async_hiv #(
      .Width($bits(sc_slave_b_t)), .Depth(FifoDepth), .SyncStages(CdcSyncStages)
    ) u_l3_b_fifo (
      .clk_wr_i(clk_i), .rst_wr_ni(rst_ni),
      .push_i(l3_b_fifo_push[gp]), .wdata_i(l3_b_fifo_wdata[gp]), .ready_o(l3_b_fifo_ready[gp]),
      .clk_rd_i(noc_clk_i), .rst_rd_ni(noc_rst_ni),
      .pop_i(l3_b_valid_o[gp] & l3_b_ready_i[gp]), .rdata_o(l3_b_o[gp]), .valid_o(l3_b_valid_o[gp]),
      .dft_hv_i(dft_i), .dft_lv_i(dft_i)
    );

    // L3 slave side wiring
    assign l3_ar_ready_o[gp] = l3_ar_fifo_ready[gp];
    assign l3_aw_ready_o[gp] = l3_aw_fifo_ready[gp];
    assign l3_w_ready_o[gp]  = l3_w_fifo_ready[gp];

    // Decode AW opcode from USER field
    assign l3_aw_opcode[gp] = req_opcode_e'(l3_aw_fifo_rdata[gp].user[$bits(req_opcode_e)-1:0]);
    // FlushToMem/EvictToMem → Write for sys
    assign sys_aw_opcode[gp] = (l3_aw_opcode[gp] inside {ReqFlushToMem, ReqEvictToMem})
                                ? ReqWrite : l3_aw_opcode[gp];

    // Auto-ack: non-forwarded opcodes (Flush, Evict, Prefetch)
    assign l3_aw_auto_ack_req[gp]   = !(l3_aw_opcode[gp] inside {ReqWrite, ReqFlushToMem, ReqEvictToMem, ReqAtomic});
    assign l3_aw_auto_ack_valid[gp] = l3_aw_auto_ack_req[gp] & l3_aw_fifo_valid[gp] & l3_w_fifo_valid[gp];
    // Auto-ack priority: outstanding responses first (prevent live-lock)
    assign l3_aw_auto_ack_ready[gp] = ~sys_b_port_valid[gp] & l3_b_fifo_ready[gp];
    assign l3_aw_auto_ack_taken[gp] = l3_aw_auto_ack_valid[gp] & l3_aw_auto_ack_ready[gp];

    assign l3_ar_req[gp] = l3_ar_fifo_valid[gp];
    assign l3_aw_req[gp] = ~l3_aw_auto_ack_req[gp] & l3_aw_fifo_valid[gp] & l3_w_fifo_valid[gp];
  end

  // ══════════════════════════════════════════════════════════
  // Request arbitration (multi-port → single sys)
  // ══════════════════════════════════════════════════════════

  logic [NumL3SlavePorts-1:0] arb_ar_grant;
  logic                       arb_ar_pop;
  sc_slave_ar_t               arb_ar_data;

  logic [NumL3SlavePorts-1:0] arb_aw_grant;
  logic                       arb_aw_pop;
  sc_slave_aw_t               arb_aw_data;

  logic [NumL3SlavePorts-1:0] arb_w_grant;
  sc_master_w_t               arb_w_data;

  prim_arb_rr #(.NumReqs(NumL3SlavePorts), .Width($bits(sc_slave_ar_t))) u_arb_ar (
    .clk_i, .rst_ni,
    .reqs_i(l3_ar_req), .grants_o(arb_ar_grant), .pop_i(arb_ar_pop),
    .data_i(l3_ar_fifo_rdata), .data_o(arb_ar_data)
  );

  prim_arb_rr #(.NumReqs(NumL3SlavePorts), .Width($bits(sc_slave_aw_t))) u_arb_aw (
    .clk_i, .rst_ni,
    .reqs_i(l3_aw_req), .grants_o(arb_aw_grant), .pop_i(arb_aw_pop),
    .data_i(l3_aw_fifo_rdata), .data_o(arb_aw_data)
  );

  prim_arb_rr #(.NumReqs(NumL3SlavePorts), .Width($bits(sc_master_w_t))) u_arb_w (
    .clk_i, .rst_ni,
    .reqs_i(l3_aw_req), .grants_o(arb_w_grant), .pop_i(arb_aw_pop),
    .data_i(l3_w_fifo_rdata), .data_o(arb_w_data)
  );

  // Grant → binary port ID
  logic [PortIdSize-1:0] arb_ar_grant_id;
  logic [PortIdSize-1:0] arb_aw_grant_id;

  if (NumL3SlavePorts > 1) begin : gen_hot2bin
    prim_hot2bin #(.HotW(NumL3SlavePorts)) u_ar_h2b (.hot_i(arb_ar_grant), .bin_o(arb_ar_grant_id));
    prim_hot2bin #(.HotW(NumL3SlavePorts)) u_aw_h2b (.hot_i(arb_aw_grant), .bin_o(arb_aw_grant_id));
  end else begin : gen_single_port
    assign arb_ar_grant_id = '0;
    assign arb_aw_grant_id = '0;
  end

  // Translated AW opcode for sys side
  req_opcode_e arb_sys_aw_opcode;
  assign arb_sys_aw_opcode = (req_opcode_e'(arb_aw_data.user[$bits(req_opcode_e)-1:0])
                               inside {ReqFlushToMem, ReqEvictToMem})
                             ? ReqWrite
                             : req_opcode_e'(arb_aw_data.user[$bits(req_opcode_e)-1:0]);

  // ══════════════════════════════════════════════════════════
  // Request queue — 64 entries, stores original L3 AXI ID for response remapping
  // ══════════════════════════════════════════════════════════

  localparam int unsigned ReqqWidth = ScSlaveIdSize;  // AXI ID width

  logic [ReqqSize-1:0]                    ar_reqq_valid_q;
  logic [ReqqSize-1:0]                    aw_reqq_valid_q;
  logic [ReqqSize-1:0][ReqqWidth-1:0]     ar_reqq_entry_q;
  logic [ReqqSize-1:0][ReqqWidth-1:0]     aw_reqq_entry_q;

  logic ar_alloc, aw_alloc, r_dealloc, b_dealloc;

  logic [PortIdSize-1:0] sys_r_port_id, sys_b_port_id;
  logic [ReqqIdSize_-1:0] sys_r_reqq_id, sys_b_reqq_id;

  // Allocation arbiters — find free entry
  /* verilator lint_off PINCONNECTEMPTY */
  logic [ReqqSize-1:0] ar_reqq_alloc_hot, aw_reqq_alloc_hot;

  prim_arb_rr #(.NumReqs(ReqqSize), .Width(1)) u_ar_alloc (
    .clk_i, .rst_ni,
    .reqs_i(~ar_reqq_valid_q), .grants_o(ar_reqq_alloc_hot), .pop_i(ar_alloc),
    .data_i('0), .data_o()
  );

  prim_arb_rr #(.NumReqs(ReqqSize), .Width(1)) u_aw_alloc (
    .clk_i, .rst_ni,
    .reqs_i(~aw_reqq_valid_q), .grants_o(aw_reqq_alloc_hot), .pop_i(aw_alloc),
    .data_i('0), .data_o()
  );

  /* verilator lint_on PINCONNECTEMPTY */

  // Allocated IDs (one-hot → binary)
  logic [ReqqIdSize_-1:0] ar_reqq_alloc_id, aw_reqq_alloc_id;
  prim_hot2bin #(.HotW(ReqqSize)) u_ar_alloc_id (.hot_i(ar_reqq_alloc_hot), .bin_o(ar_reqq_alloc_id));
  prim_hot2bin #(.HotW(ReqqSize)) u_aw_alloc_id (.hot_i(aw_reqq_alloc_hot), .bin_o(aw_reqq_alloc_id));

  // Deallocation hot vectors
  logic [ReqqSize-1:0] r_dealloc_hot, b_dealloc_hot;
  assign r_dealloc_hot = ReqqSize'(1'b1) << sys_r_reqq_id;
  assign b_dealloc_hot = ReqqSize'(1'b1) << sys_b_reqq_id;

  // Valid tracking
  logic [ReqqSize-1:0] ar_reqq_valid_d, aw_reqq_valid_d;
  assign ar_reqq_valid_d = (ar_reqq_valid_q | ({ReqqSize{ar_alloc}} & ar_reqq_alloc_hot))
                         & ~({ReqqSize{r_dealloc}} & r_dealloc_hot);
  assign aw_reqq_valid_d = (aw_reqq_valid_q | ({ReqqSize{aw_alloc}} & aw_reqq_alloc_hot))
                         & ~({ReqqSize{b_dealloc}} & b_dealloc_hot);

  logic ar_reqq_full_q, aw_reqq_full_q;
  logic ar_update, aw_update;
  assign ar_update = ar_alloc | r_dealloc;
  assign aw_update = aw_alloc | b_dealloc;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      ar_reqq_valid_q <= '0;
      aw_reqq_valid_q <= '0;
      ar_reqq_full_q  <= 1'b0;
      aw_reqq_full_q  <= 1'b0;
    end else begin
      if (ar_update) begin
        ar_reqq_valid_q <= ar_reqq_valid_d;
        ar_reqq_full_q  <= &ar_reqq_valid_d;
      end
      if (aw_update) begin
        aw_reqq_valid_q <= aw_reqq_valid_d;
        aw_reqq_full_q  <= &aw_reqq_valid_d;
      end
    end
  end

  // Entry storage — save original AXI ID on allocation
  logic [ReqqWidth-1:0] ar_reqq_entry_data, aw_reqq_entry_data;

  for (genvar gi = 0; gi < ReqqSize; gi++) begin : gen_reqq
    always_ff @(posedge clk_i) begin
      if (ar_alloc & ar_reqq_alloc_hot[gi]) ar_reqq_entry_q[gi] <= ar_reqq_entry_data;
      if (aw_alloc & aw_reqq_alloc_hot[gi]) aw_reqq_entry_q[gi] <= aw_reqq_entry_data;
    end
  end

  // ══════════════════════════════════════════════════════════
  // Sys master async FIFOs (single port)
  // ══════════════════════════════════════════════════════════

  // AR/AW/W: shire(HV) → NOC(LV) = hiv
  logic            sys_ar_fifo_ready, sys_ar_fifo_valid, sys_ar_fifo_push;
  sc_master_ar_t   sys_ar_fifo_wdata, sys_ar_fifo_rdata;

  prim_fifo_async_hiv #(
    .Width($bits(sc_master_ar_t)), .Depth(FifoDepth), .SyncStages(CdcSyncStages)
  ) u_sys_ar_fifo (
    .clk_wr_i(clk_i), .rst_wr_ni(rst_ni),
    .push_i(sys_ar_fifo_push), .wdata_i(sys_ar_fifo_wdata), .ready_o(sys_ar_fifo_ready),
    .clk_rd_i(noc_clk_i), .rst_rd_ni(noc_rst_ni),
    .pop_i(sys_ar_valid_o & sys_ar_ready_i), .rdata_o(sys_ar_fifo_rdata), .valid_o(sys_ar_fifo_valid),
    .dft_hv_i(dft_i), .dft_lv_i(dft_i)
  );

  logic            sys_aw_fifo_ready, sys_aw_fifo_valid, sys_aw_fifo_push;
  sc_master_aw_t   sys_aw_fifo_wdata, sys_aw_fifo_rdata;

  prim_fifo_async_hiv #(
    .Width($bits(sc_master_aw_t)), .Depth(FifoDepth), .SyncStages(CdcSyncStages)
  ) u_sys_aw_fifo (
    .clk_wr_i(clk_i), .rst_wr_ni(rst_ni),
    .push_i(sys_aw_fifo_push), .wdata_i(sys_aw_fifo_wdata), .ready_o(sys_aw_fifo_ready),
    .clk_rd_i(noc_clk_i), .rst_rd_ni(noc_rst_ni),
    .pop_i(sys_aw_valid_o & sys_aw_ready_i), .rdata_o(sys_aw_fifo_rdata), .valid_o(sys_aw_fifo_valid),
    .dft_hv_i(dft_i), .dft_lv_i(dft_i)
  );

  logic            sys_w_fifo_ready, sys_w_fifo_valid, sys_w_fifo_push;
  sc_master_w_t    sys_w_fifo_wdata, sys_w_fifo_rdata;

  prim_fifo_async_hiv #(
    .Width($bits(sc_master_w_t)), .Depth(FifoDepth), .SyncStages(CdcSyncStages)
  ) u_sys_w_fifo (
    .clk_wr_i(clk_i), .rst_wr_ni(rst_ni),
    .push_i(sys_w_fifo_push), .wdata_i(sys_w_fifo_wdata), .ready_o(sys_w_fifo_ready),
    .clk_rd_i(noc_clk_i), .rst_rd_ni(noc_rst_ni),
    .pop_i(sys_w_valid_o & sys_w_ready_i), .rdata_o(sys_w_fifo_rdata), .valid_o(sys_w_fifo_valid),
    .dft_hv_i(dft_i), .dft_lv_i(dft_i)
  );

  // R/B: NOC(LV) → shire(HV) = lov
  logic            sys_r_fifo_ready, sys_r_fifo_valid, sys_r_fifo_pop;
  sc_master_r_t    sys_r_fifo_rdata;

  prim_fifo_async_lov #(
    .Width($bits(sc_master_r_t)), .Depth(FifoDepth), .SyncStages(CdcSyncStages)
  ) u_sys_r_fifo (
    .clk_wr_i(noc_clk_i), .rst_wr_ni(noc_rst_ni),
    .push_i(sys_r_valid_i & sys_r_ready_o), .wdata_i(sys_r_i), .ready_o(sys_r_fifo_ready),
    .clk_rd_i(clk_i), .rst_rd_ni(rst_ni),
    .pop_i(sys_r_fifo_pop), .rdata_o(sys_r_fifo_rdata), .valid_o(sys_r_fifo_valid),
    .dft_lv_i(dft_i), .dft_hv_i(dft_i)
  );

  logic            sys_b_fifo_ready, sys_b_fifo_valid, sys_b_fifo_pop;
  sc_master_b_t    sys_b_fifo_rdata;

  prim_fifo_async_lov #(
    .Width($bits(sc_master_b_t)), .Depth(FifoDepth), .SyncStages(CdcSyncStages)
  ) u_sys_b_fifo (
    .clk_wr_i(noc_clk_i), .rst_wr_ni(noc_rst_ni),
    .push_i(sys_b_valid_i & sys_b_ready_o), .wdata_i(sys_b_i), .ready_o(sys_b_fifo_ready),
    .clk_rd_i(clk_i), .rst_rd_ni(rst_ni),
    .pop_i(sys_b_fifo_pop), .rdata_o(sys_b_fifo_rdata), .valid_o(sys_b_fifo_valid),
    .dft_lv_i(dft_i), .dft_hv_i(dft_i)
  );

  // Sys master side wiring
  assign sys_ar_o       = sys_ar_fifo_rdata;
  assign sys_aw_o       = sys_aw_fifo_rdata;
  assign sys_w_o        = sys_w_fifo_rdata;
  assign sys_ar_valid_o = sys_ar_fifo_valid;
  assign sys_aw_valid_o = sys_aw_fifo_valid;
  assign sys_w_valid_o  = sys_w_fifo_valid;
  assign sys_r_ready_o  = sys_r_fifo_ready;
  assign sys_b_ready_o  = sys_b_fifo_ready;

  // ══════════════════════════════════════════════════════════
  // Control logic
  // ══════════════════════════════════════════════════════════

  // Request forwarding
  assign arb_ar_pop       = sys_ar_fifo_ready & ~ar_reqq_full_q;
  assign arb_aw_pop       = sys_aw_fifo_ready & sys_w_fifo_ready & ~aw_reqq_full_q;
  assign sys_ar_fifo_push = |l3_ar_req & arb_ar_pop;
  assign sys_aw_fifo_push = |l3_aw_req & arb_aw_pop;
  assign sys_w_fifo_push  = sys_aw_fifo_push;
  assign ar_alloc         = sys_ar_fifo_push;
  assign aw_alloc         = sys_aw_fifo_push;
  assign ar_reqq_entry_data = arb_ar_data.id;
  assign aw_reqq_entry_data = arb_aw_data.id;

  // L3 FIFO pop
  assign l3_ar_fifo_pop = {NumL3SlavePorts{sys_ar_fifo_push}} & arb_ar_grant;
  assign l3_aw_fifo_pop = ({NumL3SlavePorts{sys_aw_fifo_push}} & arb_aw_grant) | l3_aw_auto_ack_taken;
  assign l3_w_fifo_pop  = l3_aw_fifo_pop;

  // Response routing
  assign sys_r_port_id = sys_r_fifo_rdata.id[ReqqIdSize_ +: PortIdSize];
  assign sys_b_port_id = sys_b_fifo_rdata.id[ReqqIdSize_ +: PortIdSize];
  assign sys_r_reqq_id = sys_r_fifo_rdata.id[ReqqIdSize_-1:0];
  assign sys_b_reqq_id = sys_b_fifo_rdata.id[ReqqIdSize_-1:0];

  assign sys_r_port_valid = {NumL3SlavePorts{sys_r_fifo_valid}} & (NumL3SlavePorts'(1'b1) << sys_r_port_id);
  assign sys_b_port_valid = {NumL3SlavePorts{sys_b_fifo_valid}} & (NumL3SlavePorts'(1'b1) << sys_b_port_id);
  assign sys_r_fifo_pop   = |(l3_r_fifo_ready & sys_r_port_valid);
  assign sys_b_fifo_pop   = |(l3_b_fifo_ready & sys_b_port_valid);
  assign r_dealloc        = sys_r_fifo_pop;
  assign b_dealloc        = sys_b_fifo_pop;
  assign l3_r_fifo_push   = sys_r_port_valid;
  assign l3_b_fifo_push   = sys_b_port_valid | l3_aw_auto_ack_taken;

  // ID remapping: {port_id, reqq_id} for sys, original AXI ID from reqq for L3
  logic [ScMasterIdSize-1:0] to_sys_arid, to_sys_awid;
  assign to_sys_arid = ScMasterIdSize'({arb_ar_grant_id, ar_reqq_alloc_id});
  assign to_sys_awid = ScMasterIdSize'({arb_aw_grant_id, aw_reqq_alloc_id});

  logic [ScSlaveIdSize-1:0] l3_r_axi_id, l3_b_axi_id;
  assign l3_r_axi_id = ar_reqq_entry_q[sys_r_reqq_id];
  assign l3_b_axi_id = aw_reqq_entry_q[sys_b_reqq_id];

  // AR/AW/W data bus conversions (L3 → sys): replace ID, translate AW opcode
  always_comb begin
    sys_ar_fifo_wdata.id    = to_sys_arid;
    sys_ar_fifo_wdata.addr  = arb_ar_data.addr;
    sys_ar_fifo_wdata.len   = arb_ar_data.len;
    sys_ar_fifo_wdata.size  = arb_ar_data.size;
    sys_ar_fifo_wdata.burst = arb_ar_data.burst;
    sys_ar_fifo_wdata.lock  = arb_ar_data.lock;
    sys_ar_fifo_wdata.cache = arb_ar_data.cache;
    sys_ar_fifo_wdata.prot  = arb_ar_data.prot;
    sys_ar_fifo_wdata.qos   = arb_ar_data.qos;
    sys_ar_fifo_wdata.user  = arb_ar_data.user;

    sys_aw_fifo_wdata.id    = to_sys_awid;
    sys_aw_fifo_wdata.addr  = arb_aw_data.addr;
    sys_aw_fifo_wdata.len   = arb_aw_data.len;
    sys_aw_fifo_wdata.size  = arb_aw_data.size;
    sys_aw_fifo_wdata.burst = arb_aw_data.burst;
    sys_aw_fifo_wdata.lock  = arb_aw_data.lock;
    sys_aw_fifo_wdata.cache = arb_aw_data.cache;
    sys_aw_fifo_wdata.prot  = arb_aw_data.prot;
    sys_aw_fifo_wdata.qos   = arb_aw_data.qos;
    sys_aw_fifo_wdata.user  = ScMasterAwUserSize'(arb_sys_aw_opcode);

    sys_w_fifo_wdata.data   = arb_w_data.data;
    sys_w_fifo_wdata.strb   = arb_w_data.strb;
    sys_w_fifo_wdata.last   = arb_w_data.last;
  end

  // R/B response data bus conversions: replace ID with original L3 AXI ID
  for (genvar gp = 0; gp < NumL3SlavePorts; gp++) begin : gen_l3_rsp
    always_comb begin
      l3_r_fifo_wdata[gp].id   = l3_r_axi_id;
      l3_r_fifo_wdata[gp].resp = sys_r_fifo_rdata.resp;
      l3_r_fifo_wdata[gp].last = sys_r_fifo_rdata.last;
      l3_r_fifo_wdata[gp].data = sys_r_fifo_rdata.data;

      l3_b_fifo_wdata[gp].id   = l3_aw_auto_ack_taken[gp] ? l3_aw_fifo_rdata[gp].id : l3_b_axi_id;
      l3_b_fifo_wdata[gp].resp = l3_aw_auto_ack_taken[gp] ? RespOkay                 : sys_b_fifo_rdata.resp;
    end
  end

  // ══════════════════════════════════════════════════════════
  // Unused
  // ══════════════════════════════════════════════════════════

  logic unused_ok;
  assign unused_ok = &{1'b0,
    sys_r_fifo_rdata.id,
    sys_b_fifo_rdata.id,
    unused_ok};

  /* verilator lint_on UNUSEDSIGNAL */
  /* verilator lint_on UNUSEDPARAM */

endmodule
