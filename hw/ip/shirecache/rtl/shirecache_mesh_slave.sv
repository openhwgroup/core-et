// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire cache mesh slave — routes AXI slave port requests to banks.
//
// Mirror of mesh_master: per-bank response FIFOs with port routing,
// per-port R/B response arbitration, per-port AXI port instances,
// per-bank request arbitration from ports + request FIFOs.
//
// Debug AXI port: deferred to monitoring modules (#21-25).
//
// Replaces: shire_cache_mesh_slave (etcore-soc)

/* verilator lint_off UNUSEDPARAM */
/* verilator lint_off UNUSEDSIGNAL */
module shirecache_mesh_slave
  import shirecache_pkg::*;
  import axi_pkg::*;
  import etlink_pkg::*;
#(
  parameter int unsigned NumBanks = Banks,
  parameter int unsigned NumPorts = 1
) (
  input  logic                                  clk_i,
  input  logic                                  rst_ni,
  input  logic                                  noc_clk_i,
  input  logic                                  noc_rst_ni,
  input  dft_pkg::dft_t                         dft_i,
  input  l3_swizzle_ctl_t                       esr_sc_l3_shire_swizzle_ctl_i,

  // Per-bank clock enable out (indicates incoming request)
  output logic [NumBanks-1:0]                   mesh_slave_bank_clk_en_o,

  // Per-bank mesh request
  input  logic [NumBanks-1:0]                   mesh_slave_bank_req_ready_i,
  output logic [NumBanks-1:0]                   mesh_slave_bank_req_valid_o,
  output mesh_slave_req_t [NumBanks-1:0]        mesh_slave_bank_req_info_o,

  // Per-bank mesh response
  output logic [NumBanks-1:0]                   mesh_slave_bank_rsp_ready_o,
  input  logic [NumBanks-1:0]                   mesh_slave_bank_rsp_valid_i,
  input  mesh_slave_rsp_t [NumBanks-1:0]        mesh_slave_bank_rsp_info_i,

  // AXI slave ports (per-port)
  output logic [NumPorts-1:0]                   axi_ar_ready_o,
  input  logic [NumPorts-1:0]                   axi_ar_valid_i,
  input  sc_slave_ar_t [NumPorts-1:0]           axi_ar_i,

  output logic [NumPorts-1:0]                   axi_aw_ready_o,
  input  logic [NumPorts-1:0]                   axi_aw_valid_i,
  input  sc_slave_aw_t [NumPorts-1:0]           axi_aw_i,

  output logic [NumPorts-1:0]                   axi_w_ready_o,
  input  logic [NumPorts-1:0]                   axi_w_valid_i,
  input  sc_master_w_t [NumPorts-1:0]           axi_w_i,

  input  logic [NumPorts-1:0]                   axi_r_ready_i,
  output logic [NumPorts-1:0]                   axi_r_valid_o,
  output sc_slave_r_t [NumPorts-1:0]            axi_r_o,

  input  logic [NumPorts-1:0]                   axi_b_ready_i,
  output logic [NumPorts-1:0]                   axi_b_valid_o,
  output sc_slave_b_t [NumPorts-1:0]            axi_b_o
);

  localparam int unsigned PortIdSize  = (NumPorts == 1) ? 1 : $clog2(NumPorts);
  localparam int unsigned RspInfoSize = $bits(mesh_slave_rsp_t);

  // ══════════════════════════════════════════════════════════
  // Per-bank response FIFOs with port routing
  // ══════════════════════════════════════════════════════════

  logic [NumBanks-1:0]                            bank_rsp_fifo_valid;
  logic [NumBanks-1:0]                            bank_rsp_fifo_r_rsp;
  logic [NumBanks-1:0][NumPorts-1:0]              bank_rsp_fifo_port_hot;
  logic [NumBanks-1:0][RspInfoSize-1:0]           bank_rsp_fifo_info;
  logic [NumBanks-1:0]                            bank_rsp_fifo_pop;

  logic [NumBanks-1:0][NumPorts-1:0]              bank_rsp_fifo_r_rsp_valid;
  logic [NumBanks-1:0][NumPorts-1:0]              bank_rsp_fifo_b_rsp_valid;

  for (genvar gb = 0; gb < NumBanks; gb++) begin : gen_bank_rsp

    // Classify: wdata=1 → R response, wdata=0 → B response
    logic r_rsp;
    assign r_rsp = mesh_slave_bank_rsp_info_i[gb].wdata;

    // Port routing from port_id field
    logic [PortIdSize-1:0] rsp_port_id;
    logic [NumPorts-1:0]   rsp_port_hot;
    assign rsp_port_id = mesh_slave_bank_rsp_info_i[gb].port_id[PortIdSize-1:0];
    assign rsp_port_hot = NumPorts'(1'b1) << rsp_port_id;

    logic [NumPorts-1:0] port_hot_sel;
    assign port_hot_sel = (NumPorts == 1) ? NumPorts'(1'b1) : rsp_port_hot;

    // FIFO stores {r_rsp, port_hot, rsp_info}
    localparam int unsigned BankRspFifoW = 1 + NumPorts + RspInfoSize;
    logic [BankRspFifoW-1:0] fifo_wdata, fifo_rdata;
    logic fifo_full, fifo_valid;
    logic [2:0] fifo_level_unused;

    assign fifo_wdata = {r_rsp, port_hot_sel, mesh_slave_bank_rsp_info_i[gb]};

    prim_fifo_reg #(.Width(BankRspFifoW), .Depth(2)) u_rsp_fifo (
      .clk_i(clk_i), .rst_ni,
      .push_i(mesh_slave_bank_rsp_valid_i[gb]),
      .wdata_i(fifo_wdata), .full_o(fifo_full),
      .level_o(fifo_level_unused),
      .pop_i(bank_rsp_fifo_pop[gb]),
      .valid_o(fifo_valid), .rdata_o(fifo_rdata)
    );

    assign {bank_rsp_fifo_r_rsp[gb], bank_rsp_fifo_port_hot[gb], bank_rsp_fifo_info[gb]} = fifo_rdata;
    assign bank_rsp_fifo_valid[gb] = fifo_valid;
    assign mesh_slave_bank_rsp_ready_o[gb] = ~fifo_full;

    assign bank_rsp_fifo_r_rsp_valid[gb] =
      {NumPorts{bank_rsp_fifo_r_rsp[gb] & fifo_valid}} & bank_rsp_fifo_port_hot[gb];
    assign bank_rsp_fifo_b_rsp_valid[gb] =
      {NumPorts{~bank_rsp_fifo_r_rsp[gb] & fifo_valid}} & bank_rsp_fifo_port_hot[gb];
  end

  // ══════════════════════════════════════════════════════════
  // Per-port response arbitration (bank → port)
  // ══════════════════════════════════════════════════════════

  // Reorder [bank][port] → [port][bank]
  logic [NumPorts-1:0][NumBanks-1:0] port_r_rsp_valid;
  logic [NumPorts-1:0][NumBanks-1:0] port_b_rsp_valid;
  always_comb begin
    for (int b = 0; b < NumBanks; b++) begin
      for (int p = 0; p < NumPorts; p++) begin
        port_r_rsp_valid[p][b] = bank_rsp_fifo_r_rsp_valid[b][p];
        port_b_rsp_valid[p][b] = bank_rsp_fifo_b_rsp_valid[b][p];
      end
    end
  end

  logic [NumPorts-1:0][NumBanks-1:0]     port_r_rsp_grant;
  logic [NumPorts-1:0]                   port_r_rsp_ready;
  logic [NumPorts-1:0][RspInfoSize-1:0]  port_r_rsp_info;

  logic [NumPorts-1:0][NumBanks-1:0]     port_b_rsp_grant;
  logic [NumPorts-1:0]                   port_b_rsp_ready;
  logic [NumPorts-1:0][RspInfoSize-1:0]  port_b_rsp_info;

  for (genvar gp = 0; gp < NumPorts; gp++) begin : gen_rsp_arb
    prim_arb_rr #(.NumReqs(NumBanks), .Width(RspInfoSize)) u_r_arb (
      .clk_i(clk_i), .rst_ni,
      .reqs_i(port_r_rsp_valid[gp]), .grants_o(port_r_rsp_grant[gp]),
      .pop_i(port_r_rsp_ready[gp]),
      .data_i(bank_rsp_fifo_info), .data_o(port_r_rsp_info[gp])
    );
    prim_arb_rr #(.NumReqs(NumBanks), .Width(RspInfoSize)) u_b_arb (
      .clk_i(clk_i), .rst_ni,
      .reqs_i(port_b_rsp_valid[gp]), .grants_o(port_b_rsp_grant[gp]),
      .pop_i(port_b_rsp_ready[gp]),
      .data_i(bank_rsp_fifo_info), .data_o(port_b_rsp_info[gp])
    );
  end

  // ══════════════════════════════════════════════════════════
  // Per-port AXI port instances
  // ══════════════════════════════════════════════════════════

  logic [NumPorts-1:0]                   port_req_ready;
  mesh_slave_req_t [NumPorts-1:0]        port_req_info;
  logic [NumPorts-1:0][NumBanks-1:0]     port_req_valid;

  for (genvar gp = 0; gp < NumPorts; gp++) begin : gen_axi_port
    shirecache_mesh_slave_axi_port #(
      .NumBanks(NumBanks), .PortId(gp)
    ) u_axi_port (
      .clk_i(clk_i), .rst_ni,
      .noc_clk_i, .noc_rst_ni,
      .dft_i,
      .esr_sc_l3_shire_swizzle_ctl_i,
      .port_req_ready_i   (port_req_ready[gp]),
      .port_req_valid_o   (port_req_valid[gp]),
      .port_req_info_o    (port_req_info[gp]),
      .port_r_rsp_ready_o (port_r_rsp_ready[gp]),
      .port_r_rsp_valid_i (|port_r_rsp_valid[gp]),
      .port_r_rsp_info_i  (mesh_slave_rsp_t'(port_r_rsp_info[gp])),
      .port_b_rsp_ready_o (port_b_rsp_ready[gp]),
      .port_b_rsp_valid_i (|port_b_rsp_valid[gp]),
      .port_b_rsp_info_i  (mesh_slave_rsp_t'(port_b_rsp_info[gp])),
      .axi_ar_ready_o(axi_ar_ready_o[gp]),
      .axi_ar_valid_i(axi_ar_valid_i[gp]),
      .axi_ar_i      (axi_ar_i[gp]),
      .axi_aw_ready_o(axi_aw_ready_o[gp]),
      .axi_aw_valid_i(axi_aw_valid_i[gp]),
      .axi_aw_i      (axi_aw_i[gp]),
      .axi_w_ready_o (axi_w_ready_o[gp]),
      .axi_w_valid_i (axi_w_valid_i[gp]),
      .axi_w_i       (axi_w_i[gp]),
      .axi_r_ready_i (axi_r_ready_i[gp]),
      .axi_r_valid_o (axi_r_valid_o[gp]),
      .axi_r_o       (axi_r_o[gp]),
      .axi_b_ready_i (axi_b_ready_i[gp]),
      .axi_b_valid_o (axi_b_valid_o[gp]),
      .axi_b_o       (axi_b_o[gp])
    );
  end

  // ══════════════════════════════════════════════════════════
  // Per-bank request arbitration (port → bank) + request FIFOs
  // ══════════════════════════════════════════════════════════

  // Reorder [port][bank] → [bank][port]
  logic [NumBanks-1:0][NumPorts-1:0] bank_port_req_valid;
  always_comb begin
    for (int b = 0; b < NumBanks; b++)
      for (int p = 0; p < NumPorts; p++)
        bank_port_req_valid[b][p] = port_req_valid[p][b];
  end

  localparam int unsigned ReqInfoSize = $bits(mesh_slave_req_t);

  logic [NumBanks-1:0][NumPorts-1:0]     bank_req_grant;
  logic [NumBanks-1:0]                   bank_req_ready;
  logic [NumBanks-1:0][ReqInfoSize-1:0]  bank_req_info;

  for (genvar gb = 0; gb < NumBanks; gb++) begin : gen_req_arb
    prim_arb_rr #(.NumReqs(NumPorts), .Width(ReqInfoSize)) u_req_arb (
      .clk_i(clk_i), .rst_ni,
      .reqs_i(bank_port_req_valid[gb]), .grants_o(bank_req_grant[gb]),
      .pop_i(bank_req_ready[gb]),
      .data_i(port_req_info), .data_o(bank_req_info[gb])
    );
  end

  // Request FIFOs (depth 2)
  logic [NumBanks-1:0] bank_req_fifo_push;
  for (genvar gb = 0; gb < NumBanks; gb++) begin : gen_req_fifo
    logic [2:0] req_fifo_level_unused;
    logic       req_fifo_full;

    assign bank_req_fifo_push[gb] = |bank_port_req_valid[gb];

    prim_fifo_reg #(.Width(ReqInfoSize), .Depth(2)) u_req_fifo (
      .clk_i(clk_i), .rst_ni,
      .push_i(bank_req_fifo_push[gb]),
      .wdata_i(bank_req_info[gb]),
      .full_o(req_fifo_full),
      .level_o(req_fifo_level_unused),
      .pop_i(mesh_slave_bank_req_ready_i[gb]),
      .valid_o(mesh_slave_bank_req_valid_o[gb]),
      .rdata_o(mesh_slave_bank_req_info_o[gb])
    );

    assign bank_req_ready[gb] = ~req_fifo_full;
  end

  // Clock enable: signal to banks when a request is being pushed
  assign mesh_slave_bank_clk_en_o = bank_req_fifo_push;

  // ══════════════════════════════════════════════════════════
  // Pop logic
  // ══════════════════════════════════════════════════════════

  // Bank rsp FIFO pop: when winning arbiter + port ready
  always_comb begin
    for (int b = 0; b < NumBanks; b++) begin
      bank_rsp_fifo_pop[b] = 1'b0;
      for (int p = 0; p < NumPorts; p++)
        bank_rsp_fifo_pop[b] |=
          (port_r_rsp_ready[p] & port_r_rsp_grant[p][b]) |
          (port_b_rsp_ready[p] & port_b_rsp_grant[p][b]);
    end
  end

  // Port req ready: when target bank's req FIFO has room
  always_comb begin
    for (int p = 0; p < NumPorts; p++) begin
      port_req_ready[p] = 1'b0;
      for (int b = 0; b < NumBanks; b++)
        port_req_ready[p] |= bank_req_ready[b] & bank_req_grant[b][p];
    end
  end

  /* verilator lint_on UNUSEDSIGNAL */
  /* verilator lint_on UNUSEDPARAM */

endmodule
