// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench wrapper for shirecache_mesh_master.
// NumBanks=4, NumPorts=1, ToL3=1.
// Uses same clock for both domains to eliminate CDC variability.

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off WIDTHCONCAT */

module shirecache_mesh_master_tb
  import shirecache_pkg::*;
  import axi_pkg::*;
  import dft_pkg::*;
  import etlink_pkg::*;
(
  input  logic                          clk_i,
  input  logic                          rst_ni,

  // ── Per-bank request inputs ────────────────────────────────
  input  logic [3:0]                    bank_req_valid_i,
  input  logic [3:0]                    bank_clk_en_i,

  // Bank 0 request scalar fields
  input  logic [ReqqIdSize-1:0]         b0_req_id_i,
  input  logic [MeshMasterSourceSize-1:0] b0_req_source_i,
  input  logic [4:0]                    b0_req_opcode_i,
  input  logic [PaSize-1:0]             b0_req_address_i,
  input  logic [2:0]                    b0_req_size_i,
  input  logic [AxiAxqosSize-1:0]       b0_req_qos_i,
  input  logic                          b0_req_wdata_i,
  input  logic [LineQwSize-1:0]         b0_req_qwen_i,
  input  logic                          b0_req_ben_en_i,
  input  logic [LineByteSize-1:0]       b0_req_ben_i,

  // Bank 1 request scalar fields (simplified — share same shape)
  input  logic [ReqqIdSize-1:0]         b1_req_id_i,
  input  logic [MeshMasterSourceSize-1:0] b1_req_source_i,
  input  logic [4:0]                    b1_req_opcode_i,
  input  logic [PaSize-1:0]             b1_req_address_i,
  input  logic [2:0]                    b1_req_size_i,
  input  logic [AxiAxqosSize-1:0]       b1_req_qos_i,
  input  logic                          b1_req_wdata_i,

  // Bank 2 request scalar fields
  input  logic [ReqqIdSize-1:0]         b2_req_id_i,
  input  logic [4:0]                    b2_req_opcode_i,
  input  logic [PaSize-1:0]             b2_req_address_i,

  // Bank 3 request scalar fields
  input  logic [ReqqIdSize-1:0]         b3_req_id_i,
  input  logic [4:0]                    b3_req_opcode_i,
  input  logic [PaSize-1:0]             b3_req_address_i,

  // ── Per-bank response ──────────────────────────────────────
  input  logic [3:0]                    bank_rsp_ready_i,

  // ── AXI port 0 ─────────────────────────────────────────────
  input  logic                          axi_ar_ready_i,
  input  logic                          axi_aw_ready_i,
  input  logic                          axi_w_ready_i,
  input  logic                          axi_r_valid_i,
  input  logic [ScMasterIdSize-1:0]     axi_r_id_i,
  input  logic [1:0]                    axi_r_resp_i,
  input  logic [ScMasterDataSize-1:0]   axi_r_data_i,
  input  logic                          axi_b_valid_i,
  input  logic [ScMasterIdSize-1:0]     axi_b_id_i,
  input  logic [1:0]                    axi_b_resp_i,

  // ── L3 swizzle control ─────────────────────────────────────
  input  logic [$bits(l3_swizzle_ctl_t)-1:0] swizzle_ctl_i,

  // ══════════════════════════════════════════════════════════
  // Outputs
  // ══════════════════════════════════════════════════════════
  output logic [3:0]                    bank_req_ready_o,
  output logic [3:0]                    bank_rsp_valid_o,

  // Bank 0 response fields
  output logic [ReqqIdSize-1:0]         b0_rsp_id_o,
  output logic [MeshMasterSourceSize-1:0] b0_rsp_source_o,
  output logic                          b0_rsp_err_o,
  output logic                          b0_rsp_wdata_o,

  // AXI AR
  output logic                          axi_ar_valid_o,
  output logic [ScMasterIdSize-1:0]     axi_ar_id_o,
  output logic [ScMasterAddrSize-1:0]   axi_ar_addr_o,
  output logic [AxLenSize-1:0]          axi_ar_len_o,
  output logic [AxSizeSize-1:0]         axi_ar_size_o,
  output logic [AxBurstSize-1:0]        axi_ar_burst_o,
  output logic [AxCacheSize-1:0]        axi_ar_cache_o,
  output logic [AxProtSize-1:0]         axi_ar_prot_o,
  output logic [AxQosSize-1:0]          axi_ar_qos_o,

  // AXI AW
  output logic                          axi_aw_valid_o,
  output logic [ScMasterIdSize-1:0]     axi_aw_id_o,
  output logic [ScMasterAddrSize-1:0]   axi_aw_addr_o,
  output logic [AxSizeSize-1:0]         axi_aw_size_o,
  output logic [AxQosSize-1:0]          axi_aw_qos_o,
  output logic [ScMasterAwUserSize-1:0] axi_aw_user_o,

  // AXI W
  output logic                          axi_w_valid_o,
  output logic                          axi_w_last_o,

  // AXI R/B ready
  output logic                          axi_r_ready_o,
  output logic                          axi_b_ready_o
);

  // ── Build per-bank request structs from scalars ────────────

  mesh_master_req_t [3:0] bank_req_info;

  always_comb begin
    bank_req_info = '0;

    bank_req_info[0].id       = b0_req_id_i;
    bank_req_info[0].source   = b0_req_source_i;
    bank_req_info[0].opcode   = etlink_pkg::req_opcode_e'(b0_req_opcode_i);
    bank_req_info[0].address  = b0_req_address_i;
    bank_req_info[0].size     = sc_size_t'(b0_req_size_i);
    bank_req_info[0].qos      = b0_req_qos_i;
    bank_req_info[0].wdata    = b0_req_wdata_i;
    bank_req_info[0].qwen     = b0_req_qwen_i;
    bank_req_info[0].ben_en   = b0_req_ben_en_i;
    bank_req_info[0].ben      = b0_req_ben_i;

    bank_req_info[1].id       = b1_req_id_i;
    bank_req_info[1].source   = b1_req_source_i;
    bank_req_info[1].opcode   = etlink_pkg::req_opcode_e'(b1_req_opcode_i);
    bank_req_info[1].address  = b1_req_address_i;
    bank_req_info[1].size     = sc_size_t'(b1_req_size_i);
    bank_req_info[1].qos      = b1_req_qos_i;
    bank_req_info[1].wdata    = b1_req_wdata_i;

    bank_req_info[2].id       = b2_req_id_i;
    bank_req_info[2].opcode   = etlink_pkg::req_opcode_e'(b2_req_opcode_i);
    bank_req_info[2].address  = b2_req_address_i;

    bank_req_info[3].id       = b3_req_id_i;
    bank_req_info[3].opcode   = etlink_pkg::req_opcode_e'(b3_req_opcode_i);
    bank_req_info[3].address  = b3_req_address_i;
  end

  // ── Build AXI response structs ─────────────────────────────

  sc_master_r_t axi_r_in;
  assign axi_r_in.id   = axi_r_id_i;
  assign axi_r_in.resp = axi_pkg::resp_e'(axi_r_resp_i);
  assign axi_r_in.last = 1'b1;
  assign axi_r_in.data = axi_r_data_i;

  sc_master_b_t axi_b_in;
  assign axi_b_in.id   = axi_b_id_i;
  assign axi_b_in.resp = axi_pkg::resp_e'(axi_b_resp_i);

  // ── AXI output structs ─────────────────────────────────────

  sc_master_ar_t [0:0] axi_ar_out;
  sc_master_aw_t [0:0] axi_aw_out;
  sc_master_w_t  [0:0] axi_w_out;

  // ── Response info (wired to DUT) ───────────────────────────

  mesh_master_rsp_t [3:0] bank_rsp_info;

  // ── DUT instantiation ──────────────────────────────────────

  shirecache_mesh_master #(
    .IsToL3(1), .NumBanks(4), .NumPorts(1), .UseNocClkGating(1)
  ) u_dut (
    .clk_i,
    .rst_ni,
    .noc_clk_i   (clk_i),    // same clock for both domains
    .noc_rst_ni  (rst_ni),
    .dft_i       ('0),
    .esr_sc_l3_shire_swizzle_ctl_i(l3_swizzle_ctl_t'(swizzle_ctl_i)),

    .mesh_master_bank_clk_en_i (bank_clk_en_i),

    .mesh_master_bank_req_ready_o(bank_req_ready_o),
    .mesh_master_bank_req_valid_i(bank_req_valid_i),
    .mesh_master_bank_req_info_i (bank_req_info),

    .mesh_master_bank_rsp_ready_i(bank_rsp_ready_i),
    .mesh_master_bank_rsp_valid_o(bank_rsp_valid_o),
    .mesh_master_bank_rsp_info_o (bank_rsp_info),

    .axi_ar_ready_i(axi_ar_ready_i),
    .axi_ar_valid_o(axi_ar_valid_o),
    .axi_ar_o      (axi_ar_out),

    .axi_aw_ready_i(axi_aw_ready_i),
    .axi_aw_valid_o(axi_aw_valid_o),
    .axi_aw_o      (axi_aw_out),

    .axi_w_ready_i (axi_w_ready_i),
    .axi_w_valid_o (axi_w_valid_o),
    .axi_w_o       (axi_w_out),

    .axi_r_ready_o (axi_r_ready_o),
    .axi_r_valid_i (axi_r_valid_i),
    .axi_r_i       (axi_r_in),

    .axi_b_ready_o (axi_b_ready_o),
    .axi_b_valid_i (axi_b_valid_i),
    .axi_b_i       (axi_b_in)
  );

  // ── Extract response info for bank 0 ──────────────────────

  assign b0_rsp_id_o     = bank_rsp_info[0].id;
  assign b0_rsp_source_o = bank_rsp_info[0].source;
  assign b0_rsp_err_o    = bank_rsp_info[0].err;
  assign b0_rsp_wdata_o  = bank_rsp_info[0].wdata;

  // ── Extract AXI fields ─────────────────────────────────────

  assign axi_ar_id_o    = axi_ar_out[0].id;
  assign axi_ar_addr_o  = axi_ar_out[0].addr;
  assign axi_ar_len_o   = axi_ar_out[0].len;
  assign axi_ar_size_o  = axi_ar_out[0].size;
  assign axi_ar_burst_o = axi_ar_out[0].burst;
  assign axi_ar_cache_o = axi_ar_out[0].cache;
  assign axi_ar_prot_o  = axi_ar_out[0].prot;
  assign axi_ar_qos_o   = axi_ar_out[0].qos;

  assign axi_aw_id_o    = axi_aw_out[0].id;
  assign axi_aw_addr_o  = axi_aw_out[0].addr;
  assign axi_aw_size_o  = axi_aw_out[0].size;
  assign axi_aw_qos_o   = axi_aw_out[0].qos;
  assign axi_aw_user_o  = axi_aw_out[0].user;

  assign axi_w_last_o   = axi_w_out[0].last;

  /* verilator lint_on WIDTHCONCAT */
  /* verilator lint_on UNOPTFLAT */
  /* verilator lint_on UNUSEDSIGNAL */

endmodule
