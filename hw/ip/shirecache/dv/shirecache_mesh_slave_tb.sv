// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench wrapper for shirecache_mesh_slave.
// NumBanks=4, NumPorts=1.
// Uses same clock for both domains to eliminate CDC variability.

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off WIDTHCONCAT */

module shirecache_mesh_slave_tb
  import shirecache_pkg::*;
  import axi_pkg::*;
  import dft_pkg::*;
  import etlink_pkg::*;
(
  input  logic                          clk_i,
  input  logic                          rst_ni,

  // ── AXI AR slave port scalar inputs ─────────────────────────
  input  logic                          axi_ar_valid_i,
  input  logic [ScSlaveIdSize-1:0]      axi_ar_id_i,
  input  logic [ScMasterAddrSize-1:0]   axi_ar_addr_i,
  input  logic [AxSizeSize-1:0]         axi_ar_size_i,
  input  logic [AxQosSize-1:0]          axi_ar_qos_i,

  // ── AXI AW slave port scalar inputs ─────────────────────────
  input  logic                          axi_aw_valid_i,
  input  logic [ScSlaveIdSize-1:0]      axi_aw_id_i,
  input  logic [ScMasterAddrSize-1:0]   axi_aw_addr_i,
  input  logic [AxSizeSize-1:0]         axi_aw_size_i,
  input  logic [AxQosSize-1:0]          axi_aw_qos_i,
  input  logic [ScMasterAwUserSize-1:0] axi_aw_user_i,

  // ── AXI W slave port scalar inputs ──────────────────────────
  input  logic                          axi_w_valid_i,
  input  logic [ScMasterStrbSize-1:0]   axi_w_strb_i,

  // ── AXI R/B ready ───────────────────────────────────────────
  input  logic                          axi_r_ready_i,
  input  logic                          axi_b_ready_i,

  // ── Per-bank req ready (from reqq downstream) ───────────────
  input  logic [3:0]                    bank_req_ready_i,

  // ── Per-bank rsp inputs ─────────────────────────────────────
  input  logic [3:0]                    bank_rsp_valid_i,
  input  logic [MeshMasterAxiIdSize-1:0]    b0_rsp_id_i,
  input  logic [MeshSourceBridgeIdSize-1:0] b0_rsp_source_i,
  input  logic [MaxL3SlavePortIdSize-1:0]   b0_rsp_port_id_i,
  input  logic                              b0_rsp_err_i,
  input  logic                              b0_rsp_wdata_i,

  // ── L3 swizzle control ──────────────────────────────────────
  input  logic [$bits(l3_swizzle_ctl_t)-1:0] swizzle_ctl_i,

  // ══════════════════════════════════════════════════════════
  // Outputs
  // ══════════════════════════════════════════════════════════

  // AXI AR/AW/W ready
  output logic                          axi_ar_ready_o,
  output logic                          axi_aw_ready_o,
  output logic                          axi_w_ready_o,

  // AXI R channel
  output logic                          axi_r_valid_o,
  output logic [ScSlaveIdSize-1:0]      axi_r_id_o,
  output logic [1:0]                    axi_r_resp_o,
  output logic                          axi_r_last_o,

  // AXI B channel
  output logic                          axi_b_valid_o,
  output logic [ScSlaveIdSize-1:0]      axi_b_id_o,
  output logic [1:0]                    axi_b_resp_o,

  // Per-bank req outputs
  output logic [3:0]                    bank_req_valid_o,
  output logic [4:0]                    b0_req_opcode_o,
  output logic [MeshMasterAxiIdSize-1:0]    b0_req_id_o,
  output logic [MeshSourceBridgeIdSize-1:0] b0_req_source_o,
  output logic [MaxL3SlavePortIdSize-1:0]   b0_req_port_id_o,
  output logic [PaSize-1:0]                 b0_req_address_o,
  output logic                              b0_req_wdata_o,
  output logic [LineQwSize-1:0]             b0_req_qwen_o,
  output logic [LineByteSize-1:0]           b0_req_ben_o,
  output logic                              b0_req_ben_en_o,

  // Per-bank rsp ready
  output logic [3:0]                    bank_rsp_ready_o,

  // Bank clock enable
  output logic [3:0]                    bank_clk_en_o
);

  // ── Build AXI AR struct from scalars ────────────────────────
  sc_slave_ar_t axi_ar_in;
  always_comb begin
    axi_ar_in        = '0;
    axi_ar_in.id     = axi_ar_id_i;
    axi_ar_in.addr   = axi_ar_addr_i;
    axi_ar_in.size   = axi_ar_size_i;
    axi_ar_in.qos    = axi_ar_qos_i;
    axi_ar_in.len    = '0;
    axi_ar_in.burst  = 2'b01;
    axi_ar_in.lock   = 1'b0;
    axi_ar_in.cache  = '0;
    axi_ar_in.prot   = '0;
    axi_ar_in.user   = '0;
  end

  // ── Build AXI AW struct from scalars ────────────────────────
  sc_slave_aw_t axi_aw_in;
  always_comb begin
    axi_aw_in        = '0;
    axi_aw_in.id     = axi_aw_id_i;
    axi_aw_in.addr   = axi_aw_addr_i;
    axi_aw_in.size   = axi_aw_size_i;
    axi_aw_in.qos    = axi_aw_qos_i;
    axi_aw_in.user   = axi_aw_user_i;
    axi_aw_in.len    = '0;
    axi_aw_in.burst  = 2'b01;
    axi_aw_in.lock   = 1'b0;
    axi_aw_in.cache  = '0;
    axi_aw_in.prot   = '0;
  end

  // ── Build AXI W struct from scalars ─────────────────────────
  sc_master_w_t axi_w_in;
  always_comb begin
    axi_w_in.data = '0;
    axi_w_in.strb = axi_w_strb_i;
    axi_w_in.last = 1'b1;
  end

  // ── Per-bank rsp structs ────────────────────────────────────
  mesh_slave_rsp_t [3:0] bank_rsp_info;
  always_comb begin
    bank_rsp_info = '0;
    bank_rsp_info[0].id       = b0_rsp_id_i;
    bank_rsp_info[0].source   = b0_rsp_source_i;
    bank_rsp_info[0].port_id  = b0_rsp_port_id_i;
    bank_rsp_info[0].err      = b0_rsp_err_i;
    bank_rsp_info[0].wdata    = b0_rsp_wdata_i;
  end

  // ── DUT output wires ────────────────────────────────────────
  sc_slave_r_t [0:0] axi_r_out;
  sc_slave_b_t [0:0] axi_b_out;
  mesh_slave_req_t [3:0] bank_req_info;

  // ── DUT instantiation ───────────────────────────────────────
  shirecache_mesh_slave #(
    .NumBanks(4), .NumPorts(1)
  ) u_dut (
    .clk_i,
    .rst_ni,
    .noc_clk_i  (clk_i),
    .noc_rst_ni (rst_ni),
    .dft_i      ('0),
    .esr_sc_l3_shire_swizzle_ctl_i(l3_swizzle_ctl_t'(swizzle_ctl_i)),

    .mesh_slave_bank_clk_en_o  (bank_clk_en_o),

    .mesh_slave_bank_req_ready_i(bank_req_ready_i),
    .mesh_slave_bank_req_valid_o(bank_req_valid_o),
    .mesh_slave_bank_req_info_o (bank_req_info),

    .mesh_slave_bank_rsp_ready_o(bank_rsp_ready_o),
    .mesh_slave_bank_rsp_valid_i(bank_rsp_valid_i),
    .mesh_slave_bank_rsp_info_i (bank_rsp_info),

    .axi_ar_ready_o(axi_ar_ready_o),
    .axi_ar_valid_i(axi_ar_valid_i),
    .axi_ar_i      (axi_ar_in),

    .axi_aw_ready_o(axi_aw_ready_o),
    .axi_aw_valid_i(axi_aw_valid_i),
    .axi_aw_i      (axi_aw_in),

    .axi_w_ready_o (axi_w_ready_o),
    .axi_w_valid_i (axi_w_valid_i),
    .axi_w_i       (axi_w_in),

    .axi_r_ready_i (axi_r_ready_i),
    .axi_r_valid_o (axi_r_valid_o),
    .axi_r_o       (axi_r_out),

    .axi_b_ready_i (axi_b_ready_i),
    .axi_b_valid_o (axi_b_valid_o),
    .axi_b_o       (axi_b_out)
  );

  // ── Extract bank 0 request fields ───────────────────────────
  assign b0_req_opcode_o  = bank_req_info[0].opcode;
  assign b0_req_id_o      = bank_req_info[0].id;
  assign b0_req_source_o  = bank_req_info[0].source;
  assign b0_req_port_id_o = bank_req_info[0].port_id;
  assign b0_req_address_o = bank_req_info[0].address;
  assign b0_req_wdata_o   = bank_req_info[0].wdata;
  assign b0_req_qwen_o    = bank_req_info[0].qwen;
  assign b0_req_ben_o     = bank_req_info[0].ben;
  assign b0_req_ben_en_o  = bank_req_info[0].ben_en;

  // ── Extract AXI R/B output fields ──────────────────────────
  assign axi_r_id_o   = axi_r_out[0].id;
  assign axi_r_resp_o = axi_r_out[0].resp;
  assign axi_r_last_o = axi_r_out[0].last;

  assign axi_b_id_o   = axi_b_out[0].id;
  assign axi_b_resp_o = axi_b_out[0].resp;

  /* verilator lint_on WIDTHCONCAT */
  /* verilator lint_on UNOPTFLAT */
  /* verilator lint_on UNUSEDSIGNAL */

endmodule
