// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: shire_cache_mesh_master vs shirecache_mesh_master.
// Uses scalar inputs to avoid type conflicts between original and new.
// NumBanks=4, NumPorts=1, ToL3=1.
// Same clock for both domains to eliminate CDC variability.

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off WIDTHCONCAT */

module cosim_mesh_master_tb
  import shirecache_pkg::*;
  import axi_pkg::*;
  import dft_pkg::*;
  import etlink_pkg::*;
(
  input  logic                              clk_i,
  input  logic                              rst_ni,

  // ── Per-bank request inputs (scalars) ──────────────────────
  input  logic [3:0]                        bank_req_valid_i,
  input  logic [3:0]                        bank_clk_en_i,

  // Bank 0 request fields
  input  logic [DvTransIdSize-1:0]          b0_req_trans_id_i,
  input  logic [ReqqIdSize-1:0]             b0_req_id_i,
  input  logic [MaxL3SlavePortIdSize-1:0]   b0_req_port_id_i,
  input  logic [MeshMasterSourceSize-1:0]   b0_req_source_i,
  input  logic [4:0]                        b0_req_opcode_i,
  input  logic [PaSize-1:0]                 b0_req_address_i,
  input  logic [2:0]                        b0_req_size_i,
  input  logic [AxiAxqosSize-1:0]           b0_req_qos_i,
  input  logic                              b0_req_wdata_i,
  input  logic [LineQwSize-1:0]             b0_req_qwen_i,
  input  logic [LineDataSize-1:0]           b0_req_data_i,
  input  logic                              b0_req_ben_en_i,
  input  logic [LineByteSize-1:0]           b0_req_ben_i,

  // Bank 1 request fields
  input  logic [DvTransIdSize-1:0]          b1_req_trans_id_i,
  input  logic [ReqqIdSize-1:0]             b1_req_id_i,
  input  logic [MaxL3SlavePortIdSize-1:0]   b1_req_port_id_i,
  input  logic [MeshMasterSourceSize-1:0]   b1_req_source_i,
  input  logic [4:0]                        b1_req_opcode_i,
  input  logic [PaSize-1:0]                 b1_req_address_i,
  input  logic [2:0]                        b1_req_size_i,
  input  logic [AxiAxqosSize-1:0]           b1_req_qos_i,
  input  logic                              b1_req_wdata_i,
  input  logic [LineQwSize-1:0]             b1_req_qwen_i,
  input  logic [LineDataSize-1:0]           b1_req_data_i,
  input  logic                              b1_req_ben_en_i,
  input  logic [LineByteSize-1:0]           b1_req_ben_i,

  // Bank 2 request fields
  input  logic [DvTransIdSize-1:0]          b2_req_trans_id_i,
  input  logic [ReqqIdSize-1:0]             b2_req_id_i,
  input  logic [MaxL3SlavePortIdSize-1:0]   b2_req_port_id_i,
  input  logic [MeshMasterSourceSize-1:0]   b2_req_source_i,
  input  logic [4:0]                        b2_req_opcode_i,
  input  logic [PaSize-1:0]                 b2_req_address_i,
  input  logic [2:0]                        b2_req_size_i,
  input  logic [AxiAxqosSize-1:0]           b2_req_qos_i,
  input  logic                              b2_req_wdata_i,
  input  logic [LineQwSize-1:0]             b2_req_qwen_i,
  input  logic [LineDataSize-1:0]           b2_req_data_i,
  input  logic                              b2_req_ben_en_i,
  input  logic [LineByteSize-1:0]           b2_req_ben_i,

  // Bank 3 request fields
  input  logic [DvTransIdSize-1:0]          b3_req_trans_id_i,
  input  logic [ReqqIdSize-1:0]             b3_req_id_i,
  input  logic [MaxL3SlavePortIdSize-1:0]   b3_req_port_id_i,
  input  logic [MeshMasterSourceSize-1:0]   b3_req_source_i,
  input  logic [4:0]                        b3_req_opcode_i,
  input  logic [PaSize-1:0]                 b3_req_address_i,
  input  logic [2:0]                        b3_req_size_i,
  input  logic [AxiAxqosSize-1:0]           b3_req_qos_i,
  input  logic                              b3_req_wdata_i,
  input  logic [LineQwSize-1:0]             b3_req_qwen_i,
  input  logic [LineDataSize-1:0]           b3_req_data_i,
  input  logic                              b3_req_ben_en_i,
  input  logic [LineByteSize-1:0]           b3_req_ben_i,

  // ── Per-bank response ready ────────────────────────────────
  input  logic [3:0]                        bank_rsp_ready_i,

  // ── AXI port (shared inputs) ───────────────────────────────
  input  logic                              axi_ar_ready_i,
  input  logic                              axi_aw_ready_i,
  input  logic                              axi_w_ready_i,
  input  logic                              axi_r_valid_i,
  input  logic [ScMasterIdSize-1:0]         axi_r_id_i,
  input  logic [1:0]                        axi_r_resp_i,
  input  logic [ScMasterDataSize-1:0]       axi_r_data_i,
  input  logic                              axi_b_valid_i,
  input  logic [ScMasterIdSize-1:0]         axi_b_id_i,
  input  logic [1:0]                        axi_b_resp_i,

  // ── Swizzle control (scalar) ───────────────────────────────
  input  logic [$bits(l3_swizzle_ctl_t)-1:0] swizzle_ctl_i,

  // ══════════════════════════════════════════════════════════
  // New module outputs
  // ══════════════════════════════════════════════════════════
  output logic [3:0]                        new_bank_req_ready_o,
  output logic [3:0]                        new_bank_rsp_valid_o,
  output logic                              new_axi_ar_valid_o,
  output logic                              new_axi_aw_valid_o,
  output logic                              new_axi_w_valid_o,
  output logic                              new_axi_r_ready_o,
  output logic                              new_axi_b_ready_o,

  // ══════════════════════════════════════════════════════════
  // Original module outputs
  // ══════════════════════════════════════════════════════════
  output logic [3:0]                        orig_bank_req_ready_o,
  output logic [3:0]                        orig_bank_rsp_valid_o,
  output logic                              orig_axi_ar_valid_o,
  output logic                              orig_axi_aw_valid_o,
  output logic                              orig_axi_w_valid_o,
  output logic                              orig_axi_r_ready_o,
  output logic                              orig_axi_b_ready_o
);

  logic reset;
  assign reset = ~rst_ni;

  // ══════════════════════════════════════════════════════════
  // Build new (shirecache_pkg) request structs from scalars
  // ══════════════════════════════════════════════════════════

  mesh_master_req_t [3:0] new_bank_req_info;

  always_comb begin
    new_bank_req_info[0].trans_id = b0_req_trans_id_i;
    new_bank_req_info[0].id      = b0_req_id_i;
    new_bank_req_info[0].port_id = b0_req_port_id_i;
    new_bank_req_info[0].source  = b0_req_source_i;
    new_bank_req_info[0].opcode  = etlink_pkg::req_opcode_e'(b0_req_opcode_i);
    new_bank_req_info[0].address = b0_req_address_i;
    new_bank_req_info[0].size    = sc_size_t'(b0_req_size_i);
    new_bank_req_info[0].qos     = b0_req_qos_i;
    new_bank_req_info[0].wdata   = b0_req_wdata_i;
    new_bank_req_info[0].qwen    = b0_req_qwen_i;
    new_bank_req_info[0].data    = b0_req_data_i;
    new_bank_req_info[0].ben_en  = b0_req_ben_en_i;
    new_bank_req_info[0].ben     = b0_req_ben_i;

    new_bank_req_info[1].trans_id = b1_req_trans_id_i;
    new_bank_req_info[1].id      = b1_req_id_i;
    new_bank_req_info[1].port_id = b1_req_port_id_i;
    new_bank_req_info[1].source  = b1_req_source_i;
    new_bank_req_info[1].opcode  = etlink_pkg::req_opcode_e'(b1_req_opcode_i);
    new_bank_req_info[1].address = b1_req_address_i;
    new_bank_req_info[1].size    = sc_size_t'(b1_req_size_i);
    new_bank_req_info[1].qos     = b1_req_qos_i;
    new_bank_req_info[1].wdata   = b1_req_wdata_i;
    new_bank_req_info[1].qwen    = b1_req_qwen_i;
    new_bank_req_info[1].data    = b1_req_data_i;
    new_bank_req_info[1].ben_en  = b1_req_ben_en_i;
    new_bank_req_info[1].ben     = b1_req_ben_i;

    new_bank_req_info[2].trans_id = b2_req_trans_id_i;
    new_bank_req_info[2].id      = b2_req_id_i;
    new_bank_req_info[2].port_id = b2_req_port_id_i;
    new_bank_req_info[2].source  = b2_req_source_i;
    new_bank_req_info[2].opcode  = etlink_pkg::req_opcode_e'(b2_req_opcode_i);
    new_bank_req_info[2].address = b2_req_address_i;
    new_bank_req_info[2].size    = sc_size_t'(b2_req_size_i);
    new_bank_req_info[2].qos     = b2_req_qos_i;
    new_bank_req_info[2].wdata   = b2_req_wdata_i;
    new_bank_req_info[2].qwen    = b2_req_qwen_i;
    new_bank_req_info[2].data    = b2_req_data_i;
    new_bank_req_info[2].ben_en  = b2_req_ben_en_i;
    new_bank_req_info[2].ben     = b2_req_ben_i;

    new_bank_req_info[3].trans_id = b3_req_trans_id_i;
    new_bank_req_info[3].id      = b3_req_id_i;
    new_bank_req_info[3].port_id = b3_req_port_id_i;
    new_bank_req_info[3].source  = b3_req_source_i;
    new_bank_req_info[3].opcode  = etlink_pkg::req_opcode_e'(b3_req_opcode_i);
    new_bank_req_info[3].address = b3_req_address_i;
    new_bank_req_info[3].size    = sc_size_t'(b3_req_size_i);
    new_bank_req_info[3].qos     = b3_req_qos_i;
    new_bank_req_info[3].wdata   = b3_req_wdata_i;
    new_bank_req_info[3].qwen    = b3_req_qwen_i;
    new_bank_req_info[3].data    = b3_req_data_i;
    new_bank_req_info[3].ben_en  = b3_req_ben_en_i;
    new_bank_req_info[3].ben     = b3_req_ben_i;
  end

  // ══════════════════════════════════════════════════════════
  // Build original (soc.vh) request structs from same scalars
  // ══════════════════════════════════════════════════════════

  sc_mesh_master_req_t [3:0] orig_bank_req_info;

  always_comb begin
    orig_bank_req_info[0].trans_id = b0_req_trans_id_i;
    orig_bank_req_info[0].id      = b0_req_id_i;
    orig_bank_req_info[0].port_id = b0_req_port_id_i;
    orig_bank_req_info[0].source  = b0_req_source_i;
    orig_bank_req_info[0].opcode  = et_link_req_opcode_t'(b0_req_opcode_i);
    orig_bank_req_info[0].address = b0_req_address_i;
    orig_bank_req_info[0].size    = sc_size_t'(b0_req_size_i);
    orig_bank_req_info[0].qos     = b0_req_qos_i;
    orig_bank_req_info[0].wdata   = b0_req_wdata_i;
    orig_bank_req_info[0].qwen    = b0_req_qwen_i;
    orig_bank_req_info[0].data    = b0_req_data_i;
    orig_bank_req_info[0].ben_en  = b0_req_ben_en_i;
    orig_bank_req_info[0].ben     = b0_req_ben_i;

    orig_bank_req_info[1].trans_id = b1_req_trans_id_i;
    orig_bank_req_info[1].id      = b1_req_id_i;
    orig_bank_req_info[1].port_id = b1_req_port_id_i;
    orig_bank_req_info[1].source  = b1_req_source_i;
    orig_bank_req_info[1].opcode  = et_link_req_opcode_t'(b1_req_opcode_i);
    orig_bank_req_info[1].address = b1_req_address_i;
    orig_bank_req_info[1].size    = sc_size_t'(b1_req_size_i);
    orig_bank_req_info[1].qos     = b1_req_qos_i;
    orig_bank_req_info[1].wdata   = b1_req_wdata_i;
    orig_bank_req_info[1].qwen    = b1_req_qwen_i;
    orig_bank_req_info[1].data    = b1_req_data_i;
    orig_bank_req_info[1].ben_en  = b1_req_ben_en_i;
    orig_bank_req_info[1].ben     = b1_req_ben_i;

    orig_bank_req_info[2].trans_id = b2_req_trans_id_i;
    orig_bank_req_info[2].id      = b2_req_id_i;
    orig_bank_req_info[2].port_id = b2_req_port_id_i;
    orig_bank_req_info[2].source  = b2_req_source_i;
    orig_bank_req_info[2].opcode  = et_link_req_opcode_t'(b2_req_opcode_i);
    orig_bank_req_info[2].address = b2_req_address_i;
    orig_bank_req_info[2].size    = sc_size_t'(b2_req_size_i);
    orig_bank_req_info[2].qos     = b2_req_qos_i;
    orig_bank_req_info[2].wdata   = b2_req_wdata_i;
    orig_bank_req_info[2].qwen    = b2_req_qwen_i;
    orig_bank_req_info[2].data    = b2_req_data_i;
    orig_bank_req_info[2].ben_en  = b2_req_ben_en_i;
    orig_bank_req_info[2].ben     = b2_req_ben_i;

    orig_bank_req_info[3].trans_id = b3_req_trans_id_i;
    orig_bank_req_info[3].id      = b3_req_id_i;
    orig_bank_req_info[3].port_id = b3_req_port_id_i;
    orig_bank_req_info[3].source  = b3_req_source_i;
    orig_bank_req_info[3].opcode  = et_link_req_opcode_t'(b3_req_opcode_i);
    orig_bank_req_info[3].address = b3_req_address_i;
    orig_bank_req_info[3].size    = sc_size_t'(b3_req_size_i);
    orig_bank_req_info[3].qos     = b3_req_qos_i;
    orig_bank_req_info[3].wdata   = b3_req_wdata_i;
    orig_bank_req_info[3].qwen    = b3_req_qwen_i;
    orig_bank_req_info[3].data    = b3_req_data_i;
    orig_bank_req_info[3].ben_en  = b3_req_ben_en_i;
    orig_bank_req_info[3].ben     = b3_req_ben_i;
  end

  // ══════════════════════════════════════════════════════════
  // Build AXI R/B input structs
  // ══════════════════════════════════════════════════════════

  sc_master_r_t new_axi_r_in;
  assign new_axi_r_in.id   = axi_r_id_i;
  assign new_axi_r_in.resp = axi_pkg::resp_e'(axi_r_resp_i);
  assign new_axi_r_in.last = 1'b1;
  assign new_axi_r_in.data = axi_r_data_i;

  sc_master_b_t new_axi_b_in;
  assign new_axi_b_in.id   = axi_b_id_i;
  assign new_axi_b_in.resp = axi_pkg::resp_e'(axi_b_resp_i);

  sc_master_axi_R_channel_t orig_axi_r_in;
  assign orig_axi_r_in.ID   = axi_r_id_i;
  assign orig_axi_r_in.RESP = axi_r_resp_i;
  assign orig_axi_r_in.LAST = 1'b1;
  assign orig_axi_r_in.DATA = axi_r_data_i;

  sc_master_axi_B_channel_t orig_axi_b_in;
  assign orig_axi_b_in.ID   = axi_b_id_i;
  assign orig_axi_b_in.RESP = axi_b_resp_i;

  // ══════════════════════════════════════════════════════════
  // New module instantiation
  // ══════════════════════════════════════════════════════════

  sc_master_ar_t [0:0] new_axi_ar;
  sc_master_aw_t [0:0] new_axi_aw;
  sc_master_w_t  [0:0] new_axi_w;
  mesh_master_rsp_t [3:0] new_rsp_info;

  shirecache_mesh_master #(
    .IsToL3(1), .NumBanks(4), .NumPorts(1), .UseNocClkGating(1)
  ) u_new (
    .clk_i,
    .rst_ni,
    .noc_clk_i  (clk_i),
    .noc_rst_ni (rst_ni),
    .dft_i      ('0),
    .esr_sc_l3_shire_swizzle_ctl_i(l3_swizzle_ctl_t'(swizzle_ctl_i)),
    .mesh_master_bank_clk_en_i(bank_clk_en_i),
    .mesh_master_bank_req_ready_o(new_bank_req_ready_o),
    .mesh_master_bank_req_valid_i(bank_req_valid_i),
    .mesh_master_bank_req_info_i(new_bank_req_info),
    .mesh_master_bank_rsp_ready_i(bank_rsp_ready_i),
    .mesh_master_bank_rsp_valid_o(new_bank_rsp_valid_o),
    .mesh_master_bank_rsp_info_o(new_rsp_info),
    .axi_ar_ready_i(axi_ar_ready_i),
    .axi_ar_valid_o(new_axi_ar_valid_o),
    .axi_ar_o(new_axi_ar),
    .axi_aw_ready_i(axi_aw_ready_i),
    .axi_aw_valid_o(new_axi_aw_valid_o),
    .axi_aw_o(new_axi_aw),
    .axi_w_ready_i(axi_w_ready_i),
    .axi_w_valid_o(new_axi_w_valid_o),
    .axi_w_o(new_axi_w),
    .axi_r_ready_o(new_axi_r_ready_o),
    .axi_r_valid_i(axi_r_valid_i),
    .axi_r_i(new_axi_r_in),
    .axi_b_ready_o(new_axi_b_ready_o),
    .axi_b_valid_i(axi_b_valid_i),
    .axi_b_i(new_axi_b_in)
  );

  // ══════════════════════════════════════════════════════════
  // Original module instantiation
  // ══════════════════════════════════════════════════════════

  sc_master_axi_AR_channel_t [0:0] orig_axi_ar;
  sc_master_axi_AW_channel_t [0:0] orig_axi_aw;
  sc_master_axi_W_channel_t  [0:0] orig_axi_w;
  sc_mesh_master_rsp_t [3:0] orig_rsp_info;

  debug_axi_port_ctl_t orig_debug_ctl;
  assign orig_debug_ctl = '0;
  debug_axi_port_t orig_debug_out;

  shire_cache_mesh_master #(
    .TO_L3(1), .NUM_BANKS(4), .NUM_PORTS(1), .USE_NOC_CLK_GATING(1)
  ) u_orig (
    .clock     (clk_i),
    .reset     (reset),
    .noc_clock (clk_i),
    .noc_reset (reset),
    .esr_sc_l3_shire_swizzle_ctl(esr_sc_l3_shire_swizzle_ctl_t'(swizzle_ctl_i)),
    .mesh_master_bank_clk_en(bank_clk_en_i),
    .mesh_master_bank_req_ready(orig_bank_req_ready_o),
    .mesh_master_bank_req_valid(bank_req_valid_i),
    .mesh_master_bank_req_info(orig_bank_req_info),
    .mesh_master_bank_rsp_ready(bank_rsp_ready_i),
    .mesh_master_bank_rsp_valid(orig_bank_rsp_valid_o),
    .mesh_master_bank_rsp_info(orig_rsp_info),
    .mesh_master_axi_ARREADY(axi_ar_ready_i),
    .mesh_master_axi_ARVALID(orig_axi_ar_valid_o),
    .mesh_master_axi_AR(orig_axi_ar),
    .mesh_master_axi_AWREADY(axi_aw_ready_i),
    .mesh_master_axi_AWVALID(orig_axi_aw_valid_o),
    .mesh_master_axi_AW(orig_axi_aw),
    .mesh_master_axi_WREADY(axi_w_ready_i),
    .mesh_master_axi_WVALID(orig_axi_w_valid_o),
    .mesh_master_axi_W(orig_axi_w),
    .mesh_master_axi_RREADY(orig_axi_r_ready_o),
    .mesh_master_axi_RVALID(axi_r_valid_i),
    .mesh_master_axi_R(orig_axi_r_in),
    .mesh_master_axi_BREADY(orig_axi_b_ready_o),
    .mesh_master_axi_BVALID(axi_b_valid_i),
    .mesh_master_axi_B(orig_axi_b_in),
    .debug_axi_port_ctl(orig_debug_ctl),
    .mesh_master_debug_axi_port(orig_debug_out),
    .dft__reset_byp_hv(1'b0),
    .dft__reset_hv(1'b0),
    .dft__reset_byp_lv(1'b0),
    .dft__reset_lv(1'b0)
  );

  /* verilator lint_on WIDTHCONCAT */
  /* verilator lint_on UNOPTFLAT */
  /* verilator lint_on UNUSEDSIGNAL */

endmodule
