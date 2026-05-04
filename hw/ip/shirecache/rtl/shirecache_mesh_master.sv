// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire cache mesh master — routes bank requests to AXI master ports.
//
// Per-bank request FIFOs (depth 2) with L3 swizzle for port routing.
// Per-port AR/AW arbitration selects among banks.
// Per-port AXI port instances handle CDC and AXI channel mapping.
// Per-bank response arbitration selects among ports + response FIFOs.
//
// Clock gating: a registered clk_gate_en is synced to NOC domain via 2-FF
// synchronizer, then both shire and NOC clocks are gated.
//
// DFT improvement: original hard-codes et_clk_gate .te(1'b0); we route dft_i
// so scanmode forces both gated clocks on for full scan coverage.
//
// Debug AXI port: deferred to monitoring modules (#21-25).
//
// Replaces: shire_cache_mesh_master (etcore-soc)

/* verilator lint_off UNUSEDPARAM */
/* verilator lint_off UNUSEDSIGNAL */
module shirecache_mesh_master
  import shirecache_pkg::*;
  import axi_pkg::*;
  import etlink_pkg::*;
#(
  parameter bit          IsToL3             = 1,
  parameter int unsigned NumBanks         = Banks,
  parameter int unsigned NumPorts         = 1,
  parameter bit          UseNocClkGating  = 1
) (
  input  logic                                  clk_i,
  input  logic                                  rst_ni,
  input  logic                                  noc_clk_i,
  input  logic                                  noc_rst_ni,
  input  dft_pkg::dft_t                         dft_i,
  input  l3_swizzle_ctl_t                       esr_sc_l3_shire_swizzle_ctl_i,

  // Per-bank clock enable (from bank mesh interfaces)
  input  logic [NumBanks-1:0]                   mesh_master_bank_clk_en_i,

  // Per-bank mesh request
  output logic [NumBanks-1:0]                   mesh_master_bank_req_ready_o,
  input  logic [NumBanks-1:0]                   mesh_master_bank_req_valid_i,
  input  mesh_master_req_t [NumBanks-1:0]       mesh_master_bank_req_info_i,

  // Per-bank mesh response
  input  logic [NumBanks-1:0]                   mesh_master_bank_rsp_ready_i,
  output logic [NumBanks-1:0]                   mesh_master_bank_rsp_valid_o,
  output mesh_master_rsp_t [NumBanks-1:0]       mesh_master_bank_rsp_info_o,

  // AXI master ports (per-port)
  input  logic [NumPorts-1:0]                   axi_ar_ready_i,
  output logic [NumPorts-1:0]                   axi_ar_valid_o,
  output sc_master_ar_t [NumPorts-1:0]          axi_ar_o,

  input  logic [NumPorts-1:0]                   axi_aw_ready_i,
  output logic [NumPorts-1:0]                   axi_aw_valid_o,
  output sc_master_aw_t [NumPorts-1:0]          axi_aw_o,

  input  logic [NumPorts-1:0]                   axi_w_ready_i,
  output logic [NumPorts-1:0]                   axi_w_valid_o,
  output sc_master_w_t [NumPorts-1:0]           axi_w_o,

  output logic [NumPorts-1:0]                   axi_r_ready_o,
  input  logic [NumPorts-1:0]                   axi_r_valid_i,
  input  sc_master_r_t [NumPorts-1:0]           axi_r_i,

  output logic [NumPorts-1:0]                   axi_b_ready_o,
  input  logic [NumPorts-1:0]                   axi_b_valid_i,
  input  sc_master_b_t [NumPorts-1:0]           axi_b_i
);

  localparam int unsigned PortIdSize  = (NumPorts == 1) ? 1 : $clog2(NumPorts);
  localparam int unsigned ReqInfoSize = $bits(mesh_master_req_t);

  // ══════════════════════════════════════════════════════════
  // Clock gating
  // DFT fix: original hard-codes et_clk_gate .te(1'b0); we route dft_i
  // so scanmode forces both gated clocks on for full scan coverage.
  // ══════════════════════════════════════════════════════════

  // Register the enable (1 cycle delay for clean gate)
  /* verilator lint_off SYNCASYNCNET */  // The gate-enable path intentionally feeds prim_clk_gate and a synchronized NOC-domain copy, which Verilator reports as mixed sync/async use.
  logic clk_gate_en_d, clk_gate_en_q;
  assign clk_gate_en_d = |mesh_master_bank_clk_en_i | ~rst_ni;
  always_ff @(posedge clk_i) begin
    clk_gate_en_q <= clk_gate_en_d;
  end

  // CDC sync to NOC domain (2-FF synchronizer, replaces vclevel_shft + et_cdc_sync)
  logic noc_clk_gate_en_sync;
  logic [1:0] noc_clk_gate_sync_q;
  always_ff @(posedge noc_clk_i or negedge noc_rst_ni) begin
    if (!noc_rst_ni) noc_clk_gate_sync_q <= 2'b0;
    else             noc_clk_gate_sync_q <= {noc_clk_gate_sync_q[0], clk_gate_en_q};
  end
  /* verilator lint_on SYNCASYNCNET */
  assign noc_clk_gate_en_sync = UseNocClkGating ? noc_clk_gate_sync_q[1] : 1'b1;

  // Gated clocks
  logic clk_g, noc_clk_g;

  prim_clk_gate u_clk_gate (
    .clk_i (clk_i), .en_i(clk_gate_en_q), .dft_i(dft_i), .clk_o(clk_g)
  );
  prim_clk_gate u_noc_clk_gate (
    .clk_i (noc_clk_i), .en_i(noc_clk_gate_en_sync), .dft_i(dft_i), .clk_o(noc_clk_g)
  );

  // ══════════════════════════════════════════════════════════
  // Per-bank request FIFOs with L3 swizzle port routing
  // ══════════════════════════════════════════════════════════

  logic [NumBanks-1:0]                            bank_req_fifo_valid;
  logic [NumBanks-1:0]                            bank_req_fifo_opcode_read;
  logic [NumBanks-1:0][NumPorts-1:0]              bank_req_fifo_port_hot;
  logic [NumBanks-1:0][ReqInfoSize-1:0]           bank_req_fifo_info;
  logic [NumBanks-1:0]                            bank_req_fifo_pop;

  logic [NumBanks-1:0][NumPorts-1:0]              bank_req_fifo_ar_req_valid;
  logic [NumBanks-1:0][NumPorts-1:0]              bank_req_fifo_aw_req_valid;

  for (genvar gb = 0; gb < NumBanks; gb++) begin : gen_bank_req

    // L3 swizzle to determine port routing
    logic [SubBankIdSize-1:0] l3_sub_bank_id_unused;
    logic [BankIdSize-1:0]    l3_bank_id;
    logic [L3ShireIdSize-1:0] l3_shire_id_unused;

    shirecache_pipe_l3_swizzle_get u_l3_swizzle (
      .address_i      (mesh_master_bank_req_info_i[gb].address),
      .swizzle_ctl_i  (esr_sc_l3_shire_swizzle_ctl_i),
      .sub_bank_id_o  (l3_sub_bank_id_unused),
      .bank_id_o      (l3_bank_id),
      .shire_id_o     (l3_shire_id_unused)
    );

    logic req_addr_is_remote_scp;
    assign req_addr_is_remote_scp =
      (mesh_master_bank_req_info_i[gb].address[ScpRegionLsb +: ScpRegionIdSize] == ScpRegionId);

    logic [BankIdSize-1:0] scp_bank_id;
    assign scp_bank_id = mesh_master_bank_req_info_i[gb].address[BankLsb +: BankIdSize];

    logic [BankIdSize-1:0] req_bank_id;
    assign req_bank_id = req_addr_is_remote_scp ? scp_bank_id : l3_bank_id;

    logic [PortIdSize-1:0] req_port_id;
    logic [NumPorts-1:0]   req_port_hot;
    assign req_port_id  = req_bank_id[0 +: PortIdSize];
    assign req_port_hot = NumPorts'(1'b1) << req_port_id;

    // Opcode classification
    logic opcode_read;
    assign opcode_read = (mesh_master_bank_req_info_i[gb].opcode == ReqRead);

    logic [NumPorts-1:0] port_hot_sel;
    assign port_hot_sel = (NumPorts == 1) ? NumPorts'(1'b1) : req_port_hot;

    // Req data: zero data for AW when wdata=0 (cache ops without data)
    mesh_master_req_t bank_req_info_masked;
    always_comb begin
      bank_req_info_masked = mesh_master_bank_req_info_i[gb];
      // synthesis translate_off
      // For synthesis, mask trans_id to zero (matching original's ifdef EVL_SIMULATION)
      // synthesis translate_on
      if (!opcode_read && !mesh_master_bank_req_info_i[gb].wdata)
        bank_req_info_masked.data = '0;
    end

    // FIFO stores {opcode_read, port_hot, req_info}
    localparam int unsigned BankReqFifoW = 1 + NumPorts + ReqInfoSize;
    logic [BankReqFifoW-1:0] fifo_wdata, fifo_rdata;
    logic fifo_full, fifo_valid;
    logic [2:0] fifo_level_unused;

    assign fifo_wdata = {opcode_read, port_hot_sel, bank_req_info_masked};

    prim_fifo_reg #(.Width(BankReqFifoW), .Depth(2)) u_req_fifo (
      .clk_i(clk_g), .rst_ni,
      .push_i(mesh_master_bank_req_valid_i[gb]),
      .wdata_i(fifo_wdata), .full_o(fifo_full),
      .level_o(fifo_level_unused),
      .pop_i(bank_req_fifo_pop[gb]),
      .valid_o(fifo_valid), .rdata_o(fifo_rdata)
    );

    assign {bank_req_fifo_opcode_read[gb], bank_req_fifo_port_hot[gb], bank_req_fifo_info[gb]} = fifo_rdata;
    assign bank_req_fifo_valid[gb] = fifo_valid;
    assign mesh_master_bank_req_ready_o[gb] = ~fifo_full;

    assign bank_req_fifo_ar_req_valid[gb] =
      {NumPorts{bank_req_fifo_opcode_read[gb] & fifo_valid}} & bank_req_fifo_port_hot[gb];
    assign bank_req_fifo_aw_req_valid[gb] =
      {NumPorts{~bank_req_fifo_opcode_read[gb] & fifo_valid}} & bank_req_fifo_port_hot[gb];
  end

  // ══════════════════════════════════════════════════════════
  // Per-port request arbitration (bank → port)
  // ══════════════════════════════════════════════════════════

  // Reorder [bank][port] → [port][bank]
  logic [NumPorts-1:0][NumBanks-1:0] port_ar_req_valid;
  logic [NumPorts-1:0][NumBanks-1:0] port_aw_req_valid;
  always_comb begin
    for (int b = 0; b < NumBanks; b++) begin
      for (int p = 0; p < NumPorts; p++) begin
        port_ar_req_valid[p][b] = bank_req_fifo_ar_req_valid[b][p];
        port_aw_req_valid[p][b] = bank_req_fifo_aw_req_valid[b][p];
      end
    end
  end

  logic [NumPorts-1:0][NumBanks-1:0]     port_ar_req_grant;
  logic [NumPorts-1:0]                   port_ar_req_ready;
  logic [NumPorts-1:0][ReqInfoSize-1:0]  port_ar_req_info;

  logic [NumPorts-1:0][NumBanks-1:0]     port_aw_req_grant;
  logic [NumPorts-1:0]                   port_aw_req_ready;
  logic [NumPorts-1:0][ReqInfoSize-1:0]  port_aw_req_info;

  for (genvar gp = 0; gp < NumPorts; gp++) begin : gen_req_arb
    prim_arb_rr #(.NumReqs(NumBanks), .Width(ReqInfoSize)) u_ar_arb (
      .clk_i(clk_g), .rst_ni,
      .reqs_i(port_ar_req_valid[gp]), .grants_o(port_ar_req_grant[gp]),
      .pop_i(port_ar_req_ready[gp]),
      .data_i(bank_req_fifo_info), .data_o(port_ar_req_info[gp])
    );
    prim_arb_rr #(.NumReqs(NumBanks), .Width(ReqInfoSize)) u_aw_arb (
      .clk_i(clk_g), .rst_ni,
      .reqs_i(port_aw_req_valid[gp]), .grants_o(port_aw_req_grant[gp]),
      .pop_i(port_aw_req_ready[gp]),
      .data_i(bank_req_fifo_info), .data_o(port_aw_req_info[gp])
    );
  end

  // ══════════════════════════════════════════════════════════
  // Per-port AXI port instances
  // ══════════════════════════════════════════════════════════

  logic [NumPorts-1:0]                   port_rsp_ready;
  mesh_master_rsp_t [NumPorts-1:0]       port_rsp_info;
  logic [NumPorts-1:0][NumBanks-1:0]     port_rsp_valid;

  for (genvar gp = 0; gp < NumPorts; gp++) begin : gen_axi_port
    shirecache_mesh_master_axi_port #(
      .IsToL3(IsToL3), .NumBanks(NumBanks), .PortId(gp)
    ) u_axi_port (
      .clk_i      (clk_i),
      .clk_g_i    (clk_g),
      .rst_ni     (rst_ni),
      .noc_clk_i  (noc_clk_i),
      .noc_clk_g_i(noc_clk_g),
      .noc_rst_ni (noc_rst_ni),
      .dft_i      (dft_i),
      .port_ar_req_ready_o(port_ar_req_ready[gp]),
      .port_ar_req_valid_i(|port_ar_req_valid[gp]),
      .port_ar_req_info_i (mesh_master_req_t'(port_ar_req_info[gp])),
      .port_aw_req_ready_o(port_aw_req_ready[gp]),
      .port_aw_req_valid_i(|port_aw_req_valid[gp]),
      .port_aw_req_info_i (mesh_master_req_t'(port_aw_req_info[gp])),
      .port_rsp_ready_i   (port_rsp_ready[gp]),
      .port_rsp_valid_o   (port_rsp_valid[gp]),
      .port_rsp_info_o    (port_rsp_info[gp]),
      .axi_ar_ready_i(axi_ar_ready_i[gp]),
      .axi_ar_valid_o(axi_ar_valid_o[gp]),
      .axi_ar_o      (axi_ar_o[gp]),
      .axi_aw_ready_i(axi_aw_ready_i[gp]),
      .axi_aw_valid_o(axi_aw_valid_o[gp]),
      .axi_aw_o      (axi_aw_o[gp]),
      .axi_w_ready_i (axi_w_ready_i[gp]),
      .axi_w_valid_o (axi_w_valid_o[gp]),
      .axi_w_o       (axi_w_o[gp]),
      .axi_r_ready_o (axi_r_ready_o[gp]),
      .axi_r_valid_i (axi_r_valid_i[gp]),
      .axi_r_i       (axi_r_i[gp]),
      .axi_b_ready_o (axi_b_ready_o[gp]),
      .axi_b_valid_i (axi_b_valid_i[gp]),
      .axi_b_i       (axi_b_i[gp])
    );
  end

  // ══════════════════════════════════════════════════════════
  // Per-bank response arbitration (port → bank) + response FIFOs
  // ══════════════════════════════════════════════════════════

  // Reorder [port][bank] → [bank][port]
  logic [NumBanks-1:0][NumPorts-1:0] bank_port_rsp_valid;
  always_comb begin
    for (int b = 0; b < NumBanks; b++)
      for (int p = 0; p < NumPorts; p++)
        bank_port_rsp_valid[b][p] = port_rsp_valid[p][b];
  end

  localparam int unsigned RspInfoSize = $bits(mesh_master_rsp_t);

  logic [NumBanks-1:0][NumPorts-1:0]     bank_rsp_grant;
  logic [NumBanks-1:0]                   bank_rsp_ready;
  logic [NumBanks-1:0][RspInfoSize-1:0]  bank_rsp_info;

  for (genvar gb = 0; gb < NumBanks; gb++) begin : gen_rsp_arb
    prim_arb_rr #(.NumReqs(NumPorts), .Width(RspInfoSize)) u_rsp_arb (
      .clk_i(clk_i), .rst_ni,
      .reqs_i(bank_port_rsp_valid[gb]), .grants_o(bank_rsp_grant[gb]),
      .pop_i(bank_rsp_ready[gb]),
      .data_i(port_rsp_info), .data_o(bank_rsp_info[gb])
    );
  end

  // Response FIFOs (depth 2)
  for (genvar gb = 0; gb < NumBanks; gb++) begin : gen_rsp_fifo
    logic [2:0] rsp_fifo_level_unused;
    logic       rsp_fifo_full;

    prim_fifo_reg #(.Width(RspInfoSize), .Depth(2)) u_rsp_fifo (
      .clk_i(clk_i), .rst_ni,
      .push_i(|bank_port_rsp_valid[gb]),
      .wdata_i(bank_rsp_info[gb]),
      .full_o(rsp_fifo_full),
      .level_o(rsp_fifo_level_unused),
      .pop_i(mesh_master_bank_rsp_ready_i[gb]),
      .valid_o(mesh_master_bank_rsp_valid_o[gb]),
      .rdata_o(mesh_master_bank_rsp_info_o[gb])
    );

    assign bank_rsp_ready[gb] = ~rsp_fifo_full;
  end

  // ══════════════════════════════════════════════════════════
  // Request FIFO pop logic — pop when the winning arbiter grants
  // ══════════════════════════════════════════════════════════

  always_comb begin
    for (int b = 0; b < NumBanks; b++) begin
      bank_req_fifo_pop[b] = 1'b0;
      for (int p = 0; p < NumPorts; p++) begin
        bank_req_fifo_pop[b] |=
          (port_ar_req_ready[p] & port_ar_req_grant[p][b]) |
          (port_aw_req_ready[p] & port_aw_req_grant[p][b]);
      end
    end
  end

  // Response ready to ports — pop when target bank's rsp FIFO has room
  always_comb begin
    for (int p = 0; p < NumPorts; p++) begin
      port_rsp_ready[p] = 1'b0;
      for (int b = 0; b < NumBanks; b++)
        port_rsp_ready[p] |= bank_rsp_ready[b] & bank_rsp_grant[b][p];
    end
  end

  /* verilator lint_on UNUSEDSIGNAL */
  /* verilator lint_on UNUSEDPARAM */

endmodule
