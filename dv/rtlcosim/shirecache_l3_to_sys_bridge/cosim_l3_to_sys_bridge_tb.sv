// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: shire_cache_l3_to_sys_bridge vs shirecache_l3_to_sys_bridge.
// NumL3SlavePorts=4. Same clock for both domains.
// Scalar inputs for all AXI channels.

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off WIDTHCONCAT */
/* verilator lint_off WIDTHEXPAND */

module cosim_l3_to_sys_bridge_tb
  import shirecache_pkg::*;
  import axi_pkg::*;
  import dft_pkg::*;
  import etlink_pkg::*;
(
  input  logic                          clk_i,
  input  logic                          rst_ni,

  // ── Per-port L3 slave AR (scalar) ─────────────────────────
  input  logic [3:0]                    l3_ar_valid_i,
  input  logic [ScSlaveIdSize-1:0]      p0_ar_id_i,
  input  logic [ScMasterAddrSize-1:0]   p0_ar_addr_i,
  input  logic [7:0]                    p0_ar_len_i,
  input  logic [2:0]                    p0_ar_size_i,
  input  logic [1:0]                    p0_ar_burst_i,
  input  logic                          p0_ar_lock_i,
  input  logic [3:0]                    p0_ar_cache_i,
  input  logic [2:0]                    p0_ar_prot_i,
  input  logic [3:0]                    p0_ar_qos_i,
  input  logic [ScMasterArUserSize-1:0] p0_ar_user_i,
  // Port 1 AR (minimal)
  input  logic [ScSlaveIdSize-1:0]      p1_ar_id_i,
  input  logic [ScMasterAddrSize-1:0]   p1_ar_addr_i,

  // ── Per-port L3 slave AW (scalar) ─────────────────────────
  input  logic [3:0]                    l3_aw_valid_i,
  input  logic [ScSlaveIdSize-1:0]      p0_aw_id_i,
  input  logic [ScMasterAddrSize-1:0]   p0_aw_addr_i,
  input  logic [7:0]                    p0_aw_len_i,
  input  logic [2:0]                    p0_aw_size_i,
  input  logic [1:0]                    p0_aw_burst_i,
  input  logic                          p0_aw_lock_i,
  input  logic [3:0]                    p0_aw_cache_i,
  input  logic [2:0]                    p0_aw_prot_i,
  input  logic [3:0]                    p0_aw_qos_i,
  input  logic [ScMasterAwUserSize-1:0] p0_aw_user_i,
  // Port 1 AW (minimal)
  input  logic [ScSlaveIdSize-1:0]      p1_aw_id_i,
  input  logic [ScMasterAddrSize-1:0]   p1_aw_addr_i,
  input  logic [ScMasterAwUserSize-1:0] p1_aw_user_i,

  // ── Per-port L3 slave W (scalar) ──────────────────────────
  input  logic [3:0]                    l3_w_valid_i,
  input  logic [ScMasterStrbSize-1:0]   p0_w_strb_i,
  input  logic                          p0_w_last_i,

  // ── L3 R/B ready ──────────────────────────────────────────
  input  logic [3:0]                    l3_r_ready_i,
  input  logic [3:0]                    l3_b_ready_i,

  // ── Sys AXI inputs ────────────────────────────────────────
  input  logic                          sys_ar_ready_i,
  input  logic                          sys_aw_ready_i,
  input  logic                          sys_w_ready_i,
  input  logic                          sys_r_valid_i,
  input  logic [ScMasterIdSize-1:0]     sys_r_id_i,
  input  logic [1:0]                    sys_r_resp_i,
  input  logic                          sys_r_last_i,
  input  logic                          sys_b_valid_i,
  input  logic [ScMasterIdSize-1:0]     sys_b_id_i,
  input  logic [1:0]                    sys_b_resp_i,

  // ══════════════════════════════════════════════════════════
  // New module outputs
  // ══════════════════════════════════════════════════════════
  output logic [3:0]                    new_l3_ar_ready_o,
  output logic [3:0]                    new_l3_aw_ready_o,
  output logic [3:0]                    new_l3_w_ready_o,
  output logic [3:0]                    new_l3_r_valid_o,
  output logic [3:0]                    new_l3_b_valid_o,
  output logic                          new_sys_ar_valid_o,
  output logic                          new_sys_aw_valid_o,
  output logic                          new_sys_w_valid_o,
  output logic                          new_sys_r_ready_o,
  output logic                          new_sys_b_ready_o,

  // ══════════════════════════════════════════════════════════
  // Original module outputs
  // ══════════════════════════════════════════════════════════
  output logic [3:0]                    orig_l3_ar_ready_o,
  output logic [3:0]                    orig_l3_aw_ready_o,
  output logic [3:0]                    orig_l3_w_ready_o,
  output logic [3:0]                    orig_l3_r_valid_o,
  output logic [3:0]                    orig_l3_b_valid_o,
  output logic                          orig_sys_ar_valid_o,
  output logic                          orig_sys_aw_valid_o,
  output logic                          orig_sys_w_valid_o,
  output logic                          orig_sys_r_ready_o,
  output logic                          orig_sys_b_ready_o,

  // Sys AR/AW/W field outputs for data-path comparison
  output logic [ScMasterIdSize-1:0]     new_sys_ar_id_o,
  output logic [ScMasterIdSize-1:0]     orig_sys_ar_id_o,
  output logic [ScMasterAwUserSize-1:0] new_sys_aw_user_o,
  output logic [ScMasterAwUserSize-1:0] orig_sys_aw_user_o,
  output logic [ScMasterIdSize-1:0]     new_sys_aw_id_o,
  output logic [ScMasterIdSize-1:0]     orig_sys_aw_id_o,
  output logic                          new_sys_w_last_o,
  output logic                          orig_sys_w_last_o,

  // L3 R/B field outputs (port 0 only for simplicity)
  output logic [ScSlaveIdSize-1:0]      new_l3_r0_id_o,
  output logic [ScSlaveIdSize-1:0]      orig_l3_r0_id_o,
  output logic [1:0]                    new_l3_r0_resp_o,
  output logic [1:0]                    orig_l3_r0_resp_o,
  output logic [ScSlaveIdSize-1:0]      new_l3_b0_id_o,
  output logic [ScSlaveIdSize-1:0]      orig_l3_b0_id_o,
  output logic [1:0]                    new_l3_b0_resp_o,
  output logic [1:0]                    orig_l3_b0_resp_o
);

  logic reset;
  assign reset = ~rst_ni;

  // ══════════════════════════════════════════════════════════
  // Build new (axi_pkg) L3 slave structs from scalars
  // ══════════════════════════════════════════════════════════

  sc_slave_ar_t [3:0] new_l3_ar;
  sc_slave_aw_t [3:0] new_l3_aw;
  sc_master_w_t [3:0] new_l3_w;

  always_comb begin
    new_l3_ar = '0;
    new_l3_ar[0].id    = p0_ar_id_i;
    new_l3_ar[0].addr  = p0_ar_addr_i;
    new_l3_ar[0].len   = p0_ar_len_i;
    new_l3_ar[0].size  = p0_ar_size_i;
    new_l3_ar[0].burst = p0_ar_burst_i;
    new_l3_ar[0].lock  = p0_ar_lock_i;
    new_l3_ar[0].cache = p0_ar_cache_i;
    new_l3_ar[0].prot  = p0_ar_prot_i;
    new_l3_ar[0].qos   = p0_ar_qos_i;
    new_l3_ar[0].user  = p0_ar_user_i;
    new_l3_ar[1].id    = p1_ar_id_i;
    new_l3_ar[1].addr  = p1_ar_addr_i;
    new_l3_ar[1].burst = 2'b01;
  end

  always_comb begin
    new_l3_aw = '0;
    new_l3_aw[0].id    = p0_aw_id_i;
    new_l3_aw[0].addr  = p0_aw_addr_i;
    new_l3_aw[0].len   = p0_aw_len_i;
    new_l3_aw[0].size  = p0_aw_size_i;
    new_l3_aw[0].burst = p0_aw_burst_i;
    new_l3_aw[0].lock  = p0_aw_lock_i;
    new_l3_aw[0].cache = p0_aw_cache_i;
    new_l3_aw[0].prot  = p0_aw_prot_i;
    new_l3_aw[0].qos   = p0_aw_qos_i;
    new_l3_aw[0].user  = p0_aw_user_i;
    new_l3_aw[1].id    = p1_aw_id_i;
    new_l3_aw[1].addr  = p1_aw_addr_i;
    new_l3_aw[1].user  = p1_aw_user_i;
    new_l3_aw[1].burst = 2'b01;
  end

  always_comb begin
    new_l3_w = '0;
    new_l3_w[0].strb = p0_w_strb_i;
    new_l3_w[0].last = p0_w_last_i;
    new_l3_w[1].last = 1'b1;
    new_l3_w[2].last = 1'b1;
    new_l3_w[3].last = 1'b1;
  end

  // Sys R/B input structs (new)
  sc_master_r_t new_sys_r;
  assign new_sys_r.id   = sys_r_id_i;
  assign new_sys_r.resp = axi_pkg::resp_e'(sys_r_resp_i);
  assign new_sys_r.last = sys_r_last_i;
  assign new_sys_r.data = '0;

  sc_master_b_t new_sys_b;
  assign new_sys_b.id   = sys_b_id_i;
  assign new_sys_b.resp = axi_pkg::resp_e'(sys_b_resp_i);

  // ══════════════════════════════════════════════════════════
  // Build original (soc.vh) L3 slave structs from same scalars
  // ══════════════════════════════════════════════════════════

  sc_slave_axi_AR_channel_t [3:0] orig_l3_ar;
  sc_slave_axi_AW_channel_t [3:0] orig_l3_aw;
  sc_slave_axi_W_channel_t  [3:0] orig_l3_w;

  always_comb begin
    orig_l3_ar = '0;
    orig_l3_ar[0].ID    = p0_ar_id_i;
    orig_l3_ar[0].ADDR  = p0_ar_addr_i;
    orig_l3_ar[0].LEN   = p0_ar_len_i;
    orig_l3_ar[0].SIZE  = p0_ar_size_i;
    orig_l3_ar[0].BURST = p0_ar_burst_i;
    orig_l3_ar[0].LOCK  = p0_ar_lock_i;
    orig_l3_ar[0].CACHE = p0_ar_cache_i;
    orig_l3_ar[0].PROT  = p0_ar_prot_i;
    orig_l3_ar[0].QOS   = p0_ar_qos_i;
    orig_l3_ar[0].USER  = p0_ar_user_i;
    orig_l3_ar[1].ID    = p1_ar_id_i;
    orig_l3_ar[1].ADDR  = p1_ar_addr_i;
    orig_l3_ar[1].BURST = 2'b01;
  end

  always_comb begin
    orig_l3_aw = '0;
    orig_l3_aw[0].ID    = p0_aw_id_i;
    orig_l3_aw[0].ADDR  = p0_aw_addr_i;
    orig_l3_aw[0].LEN   = p0_aw_len_i;
    orig_l3_aw[0].SIZE  = p0_aw_size_i;
    orig_l3_aw[0].BURST = p0_aw_burst_i;
    orig_l3_aw[0].LOCK  = p0_aw_lock_i;
    orig_l3_aw[0].CACHE = p0_aw_cache_i;
    orig_l3_aw[0].PROT  = p0_aw_prot_i;
    orig_l3_aw[0].QOS   = p0_aw_qos_i;
    orig_l3_aw[0].USER  = p0_aw_user_i;
    orig_l3_aw[1].ID    = p1_aw_id_i;
    orig_l3_aw[1].ADDR  = p1_aw_addr_i;
    orig_l3_aw[1].USER  = p1_aw_user_i;
    orig_l3_aw[1].BURST = 2'b01;
  end

  always_comb begin
    orig_l3_w = '0;
    orig_l3_w[0].STRB = p0_w_strb_i;
    orig_l3_w[0].LAST = p0_w_last_i;
    orig_l3_w[1].LAST = 1'b1;
    orig_l3_w[2].LAST = 1'b1;
    orig_l3_w[3].LAST = 1'b1;
  end

  // Sys R/B input structs (original)
  sc_master_axi_R_channel_t orig_sys_r;
  assign orig_sys_r.ID   = sys_r_id_i;
  assign orig_sys_r.RESP = sys_r_resp_i;
  assign orig_sys_r.LAST = sys_r_last_i;
  assign orig_sys_r.DATA = '0;

  sc_master_axi_B_channel_t orig_sys_b;
  assign orig_sys_b.ID   = sys_b_id_i;
  assign orig_sys_b.RESP = sys_b_resp_i;

  // ══════════════════════════════════════════════════════════
  // New module instantiation
  // ══════════════════════════════════════════════════════════

  sc_slave_r_t  [3:0] new_l3_r;
  sc_slave_b_t  [3:0] new_l3_b;
  sc_master_ar_t      new_sys_ar;
  sc_master_aw_t      new_sys_aw;
  sc_master_w_t       new_sys_w;

  shirecache_l3_to_sys_bridge #(
    .NumL3SlavePorts(4)
  ) u_new (
    .clk_i,
    .rst_ni,
    .noc_clk_i  (clk_i),
    .noc_rst_ni (rst_ni),
    .dft_i      ('0),
    .l3_ar_valid_i (l3_ar_valid_i),
    .l3_ar_ready_o (new_l3_ar_ready_o),
    .l3_ar_i       (new_l3_ar),
    .l3_aw_valid_i (l3_aw_valid_i),
    .l3_aw_ready_o (new_l3_aw_ready_o),
    .l3_aw_i       (new_l3_aw),
    .l3_w_valid_i  (l3_w_valid_i),
    .l3_w_ready_o  (new_l3_w_ready_o),
    .l3_w_i        (new_l3_w),
    .l3_r_valid_o  (new_l3_r_valid_o),
    .l3_r_ready_i  (l3_r_ready_i),
    .l3_r_o        (new_l3_r),
    .l3_b_valid_o  (new_l3_b_valid_o),
    .l3_b_ready_i  (l3_b_ready_i),
    .l3_b_o        (new_l3_b),
    .sys_ar_valid_o(new_sys_ar_valid_o),
    .sys_ar_ready_i(sys_ar_ready_i),
    .sys_ar_o      (new_sys_ar),
    .sys_aw_valid_o(new_sys_aw_valid_o),
    .sys_aw_ready_i(sys_aw_ready_i),
    .sys_aw_o      (new_sys_aw),
    .sys_w_valid_o (new_sys_w_valid_o),
    .sys_w_ready_i (sys_w_ready_i),
    .sys_w_o       (new_sys_w),
    .sys_r_valid_i (sys_r_valid_i),
    .sys_r_ready_o (new_sys_r_ready_o),
    .sys_r_i       (new_sys_r),
    .sys_b_valid_i (sys_b_valid_i),
    .sys_b_ready_o (new_sys_b_ready_o),
    .sys_b_i       (new_sys_b)
  );

  // ══════════════════════════════════════════════════════════
  // Original module instantiation
  // ══════════════════════════════════════════════════════════

  sc_slave_axi_R_channel_t  [3:0] orig_l3_r;
  sc_slave_axi_B_channel_t  [3:0] orig_l3_b;
  sc_master_axi_AR_channel_t      orig_sys_ar;
  sc_master_axi_AW_channel_t      orig_sys_aw;
  sc_master_axi_W_channel_t       orig_sys_w;

  shire_cache_l3_to_sys_bridge #(
    .NUM_L3_SLAVE_PORTS(4)
  ) u_orig (
    .clock     (clk_i),
    .reset     (reset),
    .noc_clock (clk_i),
    .noc_reset (reset),

    .l3_mesh_slave_axi_ARVALID (l3_ar_valid_i),
    .l3_mesh_slave_axi_ARREADY (orig_l3_ar_ready_o),
    .l3_mesh_slave_axi_AR      (orig_l3_ar),
    .l3_mesh_slave_axi_AWVALID (l3_aw_valid_i),
    .l3_mesh_slave_axi_AWREADY (orig_l3_aw_ready_o),
    .l3_mesh_slave_axi_AW      (orig_l3_aw),
    .l3_mesh_slave_axi_WVALID  (l3_w_valid_i),
    .l3_mesh_slave_axi_WREADY  (orig_l3_w_ready_o),
    .l3_mesh_slave_axi_W       (orig_l3_w),
    .l3_mesh_slave_axi_RVALID  (orig_l3_r_valid_o),
    .l3_mesh_slave_axi_RREADY  (l3_r_ready_i),
    .l3_mesh_slave_axi_R       (orig_l3_r),
    .l3_mesh_slave_axi_BVALID  (orig_l3_b_valid_o),
    .l3_mesh_slave_axi_BREADY  (l3_b_ready_i),
    .l3_mesh_slave_axi_B       (orig_l3_b),

    .to_sys_mesh_master_axi_ARVALID (orig_sys_ar_valid_o),
    .to_sys_mesh_master_axi_ARREADY (sys_ar_ready_i),
    .to_sys_mesh_master_axi_AR      (orig_sys_ar),
    .to_sys_mesh_master_axi_AWVALID (orig_sys_aw_valid_o),
    .to_sys_mesh_master_axi_AWREADY (sys_aw_ready_i),
    .to_sys_mesh_master_axi_AW      (orig_sys_aw),
    .to_sys_mesh_master_axi_WVALID  (orig_sys_w_valid_o),
    .to_sys_mesh_master_axi_WREADY  (sys_w_ready_i),
    .to_sys_mesh_master_axi_W       (orig_sys_w),
    .to_sys_mesh_master_axi_RVALID  (sys_r_valid_i),
    .to_sys_mesh_master_axi_RREADY  (orig_sys_r_ready_o),
    .to_sys_mesh_master_axi_R       (orig_sys_r),
    .to_sys_mesh_master_axi_BVALID  (sys_b_valid_i),
    .to_sys_mesh_master_axi_BREADY  (orig_sys_b_ready_o),
    .to_sys_mesh_master_axi_B       (orig_sys_b),

    .dft__reset_byp_hv (1'b0),
    .dft__reset_hv     (1'b0),
    .dft__reset_byp_lv (1'b0),
    .dft__reset_lv     (1'b0)
  );

  // ══════════════════════════════════════════════════════════
  // Field extraction for data-path comparison
  // ══════════════════════════════════════════════════════════

  // Sys AR
  assign new_sys_ar_id_o   = new_sys_ar.id;
  assign orig_sys_ar_id_o  = orig_sys_ar.ID;
  // Sys AW
  assign new_sys_aw_id_o   = new_sys_aw.id;
  assign orig_sys_aw_id_o  = orig_sys_aw.ID;
  assign new_sys_aw_user_o = new_sys_aw.user;
  assign orig_sys_aw_user_o = orig_sys_aw.USER;
  // Sys W
  assign new_sys_w_last_o  = new_sys_w.last;
  assign orig_sys_w_last_o = orig_sys_w.LAST;
  // L3 R port 0
  assign new_l3_r0_id_o    = new_l3_r[0].id;
  assign orig_l3_r0_id_o   = orig_l3_r[0].ID;
  assign new_l3_r0_resp_o  = new_l3_r[0].resp;
  assign orig_l3_r0_resp_o = orig_l3_r[0].RESP;
  // L3 B port 0
  assign new_l3_b0_id_o    = new_l3_b[0].id;
  assign orig_l3_b0_id_o   = orig_l3_b[0].ID;
  assign new_l3_b0_resp_o  = new_l3_b[0].resp;
  assign orig_l3_b0_resp_o = orig_l3_b[0].RESP;

  /* verilator lint_on WIDTHEXPAND */
  /* verilator lint_on WIDTHCONCAT */
  /* verilator lint_on UNOPTFLAT */
  /* verilator lint_on UNUSEDSIGNAL */

endmodule
