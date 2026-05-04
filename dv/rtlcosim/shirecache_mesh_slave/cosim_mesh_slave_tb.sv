// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_mesh_slave
// vs new shirecache_mesh_slave.
//
// Both modules receive identical stimulus via scalar inputs.
// Uses same clock for both domains (clk_i = noc_clk_i).

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off WIDTHCONCAT */
/* verilator lint_off WIDTHEXPAND */

module cosim_mesh_slave_tb
  import shirecache_pkg::*;
  import axi_pkg::*;
  import etlink_pkg::*;
#(
  parameter int unsigned NumBanks = 4,
  parameter int unsigned NumPorts = 1
) (
  input  logic                          clk_i,
  input  logic                          rst_ni,

  // ── AXI AR slave port (scalar) ──────────────────────────────
  input  logic                          axi_ar_valid_i,
  input  logic [ScSlaveIdSize-1:0]      axi_ar_id_i,
  input  logic [ScMasterAddrSize-1:0]   axi_ar_addr_i,
  input  logic [7:0]                    axi_ar_len_i,
  input  logic [2:0]                    axi_ar_size_i,
  input  logic [1:0]                    axi_ar_burst_i,
  input  logic                          axi_ar_lock_i,
  input  logic [3:0]                    axi_ar_cache_i,
  input  logic [2:0]                    axi_ar_prot_i,
  input  logic [3:0]                    axi_ar_qos_i,
  input  logic [0:0]                    axi_ar_user_i,

  // ── AXI AW slave port (scalar) ──────────────────────────────
  input  logic                          axi_aw_valid_i,
  input  logic [ScSlaveIdSize-1:0]      axi_aw_id_i,
  input  logic [ScMasterAddrSize-1:0]   axi_aw_addr_i,
  input  logic [7:0]                    axi_aw_len_i,
  input  logic [2:0]                    axi_aw_size_i,
  input  logic [1:0]                    axi_aw_burst_i,
  input  logic                          axi_aw_lock_i,
  input  logic [3:0]                    axi_aw_cache_i,
  input  logic [2:0]                    axi_aw_prot_i,
  input  logic [3:0]                    axi_aw_qos_i,
  input  logic [4:0]                    axi_aw_user_i,

  // ── AXI W slave port (scalar) ───────────────────────────────
  input  logic                          axi_w_valid_i,
  input  logic [ScMasterDataSize-1:0]   axi_w_data_i,
  input  logic [ScMasterStrbSize-1:0]   axi_w_strb_i,
  input  logic                          axi_w_last_i,

  // ── AXI R/B ready ───────────────────────────────────────────
  input  logic                          axi_r_ready_i,
  input  logic                          axi_b_ready_i,

  // ── Per-bank req ready ──────────────────────────────────────
  input  logic [NumBanks-1:0]           bank_req_ready_i,

  // ── Per-bank rsp (scalar) ───────────────────────────────────
  input  logic [NumBanks-1:0]           bank_rsp_valid_i,
  input  logic [DvTransIdSize-1:0]          rsp_trans_id_i,
  input  logic [MeshMasterAxiIdSize-1:0]    rsp_id_i,
  input  logic [MaxL3SlavePortIdSize-1:0]   rsp_port_id_i,
  input  logic [MeshSourceBridgeIdSize-1:0] rsp_source_i,
  input  logic                              rsp_err_i,
  input  logic                              rsp_wdata_i,
  input  logic [LineDataSize-1:0]           rsp_data_i,

  // ── L3 swizzle control (scalar fields) ──────────────────────
  input  logic                              swz_all_shire_aliasing_i,
  input  logic                              swz_two_shire_aliasing_i,
  input  logic [3:0]                        swz_sub_bank_sel_b2_i,
  input  logic [3:0]                        swz_sub_bank_sel_b1_i,
  input  logic [3:0]                        swz_sub_bank_sel_b0_i,
  input  logic [3:0]                        swz_bank_sel_b2_i,
  input  logic [3:0]                        swz_bank_sel_b1_i,
  input  logic [3:0]                        swz_bank_sel_b0_i,
  input  logic [3:0]                        swz_shire_sel_b5_i,
  input  logic [3:0]                        swz_shire_sel_b4_i,
  input  logic [3:0]                        swz_shire_sel_b3_i,
  input  logic [3:0]                        swz_shire_sel_b2_i,
  input  logic [3:0]                        swz_shire_sel_b1_i,
  input  logic [3:0]                        swz_shire_sel_b0_i,

  // ══════════════════════════════════════════════════════════
  // Outputs — original
  // ══════════════════════════════════════════════════════════
  output logic [NumBanks-1:0]           orig_bank_clk_en_o,
  output logic [NumBanks-1:0]           orig_bank_req_valid_o,
  output logic [NumBanks-1:0]           orig_bank_rsp_ready_o,
  output logic                          orig_axi_ar_ready_o,
  output logic                          orig_axi_aw_ready_o,
  output logic                          orig_axi_w_ready_o,
  output logic                          orig_axi_r_valid_o,
  output logic                          orig_axi_b_valid_o,

  // ══════════════════════════════════════════════════════════
  // Outputs — new
  // ══════════════════════════════════════════════════════════
  output logic [NumBanks-1:0]           new_bank_clk_en_o,
  output logic [NumBanks-1:0]           new_bank_req_valid_o,
  output logic [NumBanks-1:0]           new_bank_rsp_ready_o,
  output logic                          new_axi_ar_ready_o,
  output logic                          new_axi_aw_ready_o,
  output logic                          new_axi_w_ready_o,
  output logic                          new_axi_r_valid_o,
  output logic                          new_axi_b_valid_o
);

  logic reset;
  assign reset = ~rst_ni;

  // ================================================================
  // Build original (sc_*) structs from scalar inputs
  // ================================================================

  // -- AXI AR --
  sc_slave_axi_AR_channel_t orig_axi_ar;
  always_comb begin
    orig_axi_ar.ID    = axi_ar_id_i;
    orig_axi_ar.ADDR  = axi_ar_addr_i;
    orig_axi_ar.LEN   = axi_ar_len_i;
    orig_axi_ar.SIZE  = axi_ar_size_i;
    orig_axi_ar.BURST = axi_ar_burst_i;
    orig_axi_ar.LOCK  = axi_ar_lock_i;
    orig_axi_ar.CACHE = axi_ar_cache_i;
    orig_axi_ar.PROT  = axi_ar_prot_i;
    orig_axi_ar.QOS   = axi_ar_qos_i;
    orig_axi_ar.USER  = axi_ar_user_i;
  end

  // -- AXI AW --
  sc_slave_axi_AW_channel_t orig_axi_aw;
  always_comb begin
    orig_axi_aw.ID    = axi_aw_id_i;
    orig_axi_aw.ADDR  = axi_aw_addr_i;
    orig_axi_aw.LEN   = axi_aw_len_i;
    orig_axi_aw.SIZE  = axi_aw_size_i;
    orig_axi_aw.BURST = axi_aw_burst_i;
    orig_axi_aw.LOCK  = axi_aw_lock_i;
    orig_axi_aw.CACHE = axi_aw_cache_i;
    orig_axi_aw.PROT  = axi_aw_prot_i;
    orig_axi_aw.QOS   = axi_aw_qos_i;
    orig_axi_aw.USER  = axi_aw_user_i;
  end

  // -- AXI W --
  sc_slave_axi_W_channel_t orig_axi_w;
  always_comb begin
    orig_axi_w.DATA = axi_w_data_i;
    orig_axi_w.STRB = axi_w_strb_i;
    orig_axi_w.LAST = axi_w_last_i;
  end

  // -- Bank rsp (original struct) --
  sc_mesh_slave_rsp_t [NumBanks-1:0] orig_bank_rsp_info;
  always_comb begin
    for (int b = 0; b < NumBanks; b++) begin
      orig_bank_rsp_info[b].trans_id = rsp_trans_id_i;
      orig_bank_rsp_info[b].id       = rsp_id_i;
      orig_bank_rsp_info[b].port_id  = rsp_port_id_i;
      orig_bank_rsp_info[b].source   = rsp_source_i;
      orig_bank_rsp_info[b].err      = rsp_err_i;
      orig_bank_rsp_info[b].wdata    = rsp_wdata_i;
      orig_bank_rsp_info[b].data     = rsp_data_i;
    end
  end

  // -- Swizzle control (original: two_shire first, all_shire second) --
  esr_sc_l3_shire_swizzle_ctl_t orig_swizzle_ctl;
  always_comb begin
    orig_swizzle_ctl.esr_sc_two_shire_aliasing_use_shire_lsb = swz_two_shire_aliasing_i;
    orig_swizzle_ctl.esr_sc_all_shire_aliasing                = swz_all_shire_aliasing_i;
    orig_swizzle_ctl.esr_sc_sub_bank_sel_b2 = swz_sub_bank_sel_b2_i;
    orig_swizzle_ctl.esr_sc_sub_bank_sel_b1 = swz_sub_bank_sel_b1_i;
    orig_swizzle_ctl.esr_sc_sub_bank_sel_b0 = swz_sub_bank_sel_b0_i;
    orig_swizzle_ctl.esr_sc_bank_sel_b2     = swz_bank_sel_b2_i;
    orig_swizzle_ctl.esr_sc_bank_sel_b1     = swz_bank_sel_b1_i;
    orig_swizzle_ctl.esr_sc_bank_sel_b0     = swz_bank_sel_b0_i;
    orig_swizzle_ctl.esr_sc_shire_sel_b5    = swz_shire_sel_b5_i;
    orig_swizzle_ctl.esr_sc_shire_sel_b4    = swz_shire_sel_b4_i;
    orig_swizzle_ctl.esr_sc_shire_sel_b3    = swz_shire_sel_b3_i;
    orig_swizzle_ctl.esr_sc_shire_sel_b2    = swz_shire_sel_b2_i;
    orig_swizzle_ctl.esr_sc_shire_sel_b1    = swz_shire_sel_b1_i;
    orig_swizzle_ctl.esr_sc_shire_sel_b0    = swz_shire_sel_b0_i;
  end

  // ================================================================
  // Build new (shirecache_pkg) structs from scalar inputs
  // ================================================================

  // -- AXI AR --
  sc_slave_ar_t new_axi_ar;
  always_comb begin
    new_axi_ar.id    = axi_ar_id_i;
    new_axi_ar.addr  = axi_ar_addr_i;
    new_axi_ar.len   = axi_ar_len_i;
    new_axi_ar.size  = axi_ar_size_i;
    new_axi_ar.burst = axi_ar_burst_i;
    new_axi_ar.lock  = axi_ar_lock_i;
    new_axi_ar.cache = axi_ar_cache_i;
    new_axi_ar.prot  = axi_ar_prot_i;
    new_axi_ar.qos   = axi_ar_qos_i;
    new_axi_ar.user  = axi_ar_user_i;
  end

  // -- AXI AW --
  sc_slave_aw_t new_axi_aw;
  always_comb begin
    new_axi_aw.id    = axi_aw_id_i;
    new_axi_aw.addr  = axi_aw_addr_i;
    new_axi_aw.len   = axi_aw_len_i;
    new_axi_aw.size  = axi_aw_size_i;
    new_axi_aw.burst = axi_aw_burst_i;
    new_axi_aw.lock  = axi_aw_lock_i;
    new_axi_aw.cache = axi_aw_cache_i;
    new_axi_aw.prot  = axi_aw_prot_i;
    new_axi_aw.qos   = axi_aw_qos_i;
    new_axi_aw.user  = axi_aw_user_i;
  end

  // -- AXI W --
  sc_master_w_t new_axi_w;
  always_comb begin
    new_axi_w.data = axi_w_data_i;
    new_axi_w.strb = axi_w_strb_i;
    new_axi_w.last = axi_w_last_i;
  end

  // -- Bank rsp (new struct) --
  mesh_slave_rsp_t [NumBanks-1:0] new_bank_rsp_info;
  always_comb begin
    for (int b = 0; b < NumBanks; b++) begin
      new_bank_rsp_info[b].trans_id = rsp_trans_id_i;
      new_bank_rsp_info[b].id       = rsp_id_i;
      new_bank_rsp_info[b].port_id  = rsp_port_id_i;
      new_bank_rsp_info[b].source   = rsp_source_i;
      new_bank_rsp_info[b].err      = rsp_err_i;
      new_bank_rsp_info[b].wdata    = rsp_wdata_i;
      new_bank_rsp_info[b].data     = rsp_data_i;
    end
  end

  // -- Swizzle control (new: all_shire first, two_shire second) --
  l3_swizzle_ctl_t new_swizzle_ctl;
  always_comb begin
    new_swizzle_ctl.all_shire_aliasing                = swz_all_shire_aliasing_i;
    new_swizzle_ctl.two_shire_aliasing_use_shire_lsb  = swz_two_shire_aliasing_i;
    new_swizzle_ctl.sub_bank_sel_b2 = swz_sub_bank_sel_b2_i;
    new_swizzle_ctl.sub_bank_sel_b1 = swz_sub_bank_sel_b1_i;
    new_swizzle_ctl.sub_bank_sel_b0 = swz_sub_bank_sel_b0_i;
    new_swizzle_ctl.bank_sel_b2     = swz_bank_sel_b2_i;
    new_swizzle_ctl.bank_sel_b1     = swz_bank_sel_b1_i;
    new_swizzle_ctl.bank_sel_b0     = swz_bank_sel_b0_i;
    new_swizzle_ctl.shire_sel_b5    = swz_shire_sel_b5_i;
    new_swizzle_ctl.shire_sel_b4    = swz_shire_sel_b4_i;
    new_swizzle_ctl.shire_sel_b3    = swz_shire_sel_b3_i;
    new_swizzle_ctl.shire_sel_b2    = swz_shire_sel_b2_i;
    new_swizzle_ctl.shire_sel_b1    = swz_shire_sel_b1_i;
    new_swizzle_ctl.shire_sel_b0    = swz_shire_sel_b0_i;
  end

  // ================================================================
  // Original module instantiation
  // ================================================================

  logic [NumBanks-1:0] orig_bank_req_valid;
  logic [NumBanks-1:0] orig_bank_rsp_ready;
  logic [NumBanks-1:0] orig_bank_clk_en;
  logic                orig_ar_ready, orig_aw_ready, orig_w_ready;
  logic                orig_r_valid, orig_b_valid;

  // Debug/DFT ports tied off
  debug_axi_port_ctl_t orig_debug_ctl;
  assign orig_debug_ctl = '0;

  shire_cache_mesh_slave #(
    .NUM_BANKS(NumBanks),
    .NUM_PORTS(NumPorts),
    .DEBUG_PORT_OFFSET(0)
  ) u_orig (
    .clock                       (clk_i),
    .reset                       (reset),
    .noc_clock                   (clk_i),
    .noc_reset                   (reset),
    .esr_sc_l3_shire_swizzle_ctl (orig_swizzle_ctl),

    .mesh_slave_bank_clk_en      (orig_bank_clk_en),

    .mesh_slave_bank_req_ready   (bank_req_ready_i),
    .mesh_slave_bank_req_valid   (orig_bank_req_valid),
    .mesh_slave_bank_req_info    (),

    .mesh_slave_bank_rsp_ready   (orig_bank_rsp_ready),
    .mesh_slave_bank_rsp_valid   (bank_rsp_valid_i),
    .mesh_slave_bank_rsp_info    (orig_bank_rsp_info),

    .mesh_slave_axi_ARREADY      (orig_ar_ready),
    .mesh_slave_axi_ARVALID      (axi_ar_valid_i),
    .mesh_slave_axi_AR           (orig_axi_ar),

    .mesh_slave_axi_AWREADY      (orig_aw_ready),
    .mesh_slave_axi_AWVALID      (axi_aw_valid_i),
    .mesh_slave_axi_AW           (orig_axi_aw),

    .mesh_slave_axi_WREADY       (orig_w_ready),
    .mesh_slave_axi_WVALID       (axi_w_valid_i),
    .mesh_slave_axi_W            (orig_axi_w),

    .mesh_slave_axi_RREADY       (axi_r_ready_i),
    .mesh_slave_axi_RVALID       (orig_r_valid),
    .mesh_slave_axi_R            (),

    .mesh_slave_axi_BREADY       (axi_b_ready_i),
    .mesh_slave_axi_BVALID       (orig_b_valid),
    .mesh_slave_axi_B            (),

    .debug_axi_port_ctl          (orig_debug_ctl),
    .mesh_slave_debug_axi_port   (),

    .dft__reset_byp_hv           (1'b0),
    .dft__reset_hv               (1'b0),
    .dft__reset_byp_lv           (1'b0),
    .dft__reset_lv               (1'b0)
  );

  // ================================================================
  // New module instantiation
  // ================================================================

  logic [NumBanks-1:0] new_bank_req_valid;
  logic [NumBanks-1:0] new_bank_rsp_ready;
  logic [NumBanks-1:0] new_bank_clk_en;
  logic                new_ar_ready, new_aw_ready, new_w_ready;
  logic                new_r_valid, new_b_valid;

  shirecache_mesh_slave #(
    .NumBanks(NumBanks),
    .NumPorts(NumPorts)
  ) u_new (
    .clk_i     (clk_i),
    .rst_ni    (rst_ni),
    .noc_clk_i (clk_i),
    .noc_rst_ni(rst_ni),
    .dft_i     ('0),
    .esr_sc_l3_shire_swizzle_ctl_i(new_swizzle_ctl),

    .mesh_slave_bank_clk_en_o  (new_bank_clk_en),

    .mesh_slave_bank_req_ready_i(bank_req_ready_i),
    .mesh_slave_bank_req_valid_o(new_bank_req_valid),
    .mesh_slave_bank_req_info_o (),

    .mesh_slave_bank_rsp_ready_o(new_bank_rsp_ready),
    .mesh_slave_bank_rsp_valid_i(bank_rsp_valid_i),
    .mesh_slave_bank_rsp_info_i (new_bank_rsp_info),

    .axi_ar_ready_o(new_ar_ready),
    .axi_ar_valid_i(axi_ar_valid_i),
    .axi_ar_i      (new_axi_ar),

    .axi_aw_ready_o(new_aw_ready),
    .axi_aw_valid_i(axi_aw_valid_i),
    .axi_aw_i      (new_axi_aw),

    .axi_w_ready_o (new_w_ready),
    .axi_w_valid_i (axi_w_valid_i),
    .axi_w_i       (new_axi_w),

    .axi_r_ready_i (axi_r_ready_i),
    .axi_r_valid_o (new_r_valid),
    .axi_r_o       (),

    .axi_b_ready_i (axi_b_ready_i),
    .axi_b_valid_o (new_b_valid),
    .axi_b_o       ()
  );

  // ================================================================
  // Output assignments for comparison
  // ================================================================

  assign orig_bank_clk_en_o   = orig_bank_clk_en;
  assign orig_bank_req_valid_o = orig_bank_req_valid;
  assign orig_bank_rsp_ready_o = orig_bank_rsp_ready;
  assign orig_axi_ar_ready_o   = orig_ar_ready;
  assign orig_axi_aw_ready_o   = orig_aw_ready;
  assign orig_axi_w_ready_o    = orig_w_ready;
  assign orig_axi_r_valid_o    = orig_r_valid;
  assign orig_axi_b_valid_o    = orig_b_valid;

  assign new_bank_clk_en_o    = new_bank_clk_en;
  assign new_bank_req_valid_o  = new_bank_req_valid;
  assign new_bank_rsp_ready_o  = new_bank_rsp_ready;
  assign new_axi_ar_ready_o    = new_ar_ready;
  assign new_axi_aw_ready_o    = new_aw_ready;
  assign new_axi_w_ready_o     = new_w_ready;
  assign new_axi_r_valid_o     = new_r_valid;
  assign new_axi_b_valid_o     = new_b_valid;

  /* verilator lint_on WIDTHEXPAND */
  /* verilator lint_on WIDTHCONCAT */
  /* verilator lint_on UNOPTFLAT */
  /* verilator lint_on UNUSEDSIGNAL */

endmodule
