// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench wrapper for shirecache_l3_to_sys_bridge.
// NumL3SlavePorts=4. Uses same clock for both domains.

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off WIDTHCONCAT */

module shirecache_l3_to_sys_bridge_tb
  import shirecache_pkg::*;
  import axi_pkg::*;
  import dft_pkg::*;
  import etlink_pkg::*;
(
  input  logic                          clk_i,
  input  logic                          rst_ni,

  // ── Per-port L3 slave AR (port 0 scalar) ──────────────────
  input  logic [3:0]                    l3_ar_valid_i,
  input  logic [ScSlaveIdSize-1:0]      p0_ar_id_i,
  input  logic [ScMasterAddrSize-1:0]   p0_ar_addr_i,
  input  logic [AxSizeSize-1:0]         p0_ar_size_i,
  input  logic [AxQosSize-1:0]          p0_ar_qos_i,
  input  logic [ScMasterArUserSize-1:0] p0_ar_user_i,

  // ── Per-port L3 slave AW (port 0 scalar) ──────────────────
  input  logic [3:0]                    l3_aw_valid_i,
  input  logic [ScSlaveIdSize-1:0]      p0_aw_id_i,
  input  logic [ScMasterAddrSize-1:0]   p0_aw_addr_i,
  input  logic [AxSizeSize-1:0]         p0_aw_size_i,
  input  logic [AxQosSize-1:0]          p0_aw_qos_i,
  input  logic [ScMasterAwUserSize-1:0] p0_aw_user_i,

  // Port 1 AW
  input  logic [ScSlaveIdSize-1:0]      p1_aw_id_i,
  input  logic [ScMasterAddrSize-1:0]   p1_aw_addr_i,
  input  logic [ScMasterAwUserSize-1:0] p1_aw_user_i,
  // Port 1 AR
  input  logic [ScSlaveIdSize-1:0]      p1_ar_id_i,
  input  logic [ScMasterAddrSize-1:0]   p1_ar_addr_i,

  // ── Per-port L3 slave W (port 0 scalar) ───────────────────
  input  logic [3:0]                    l3_w_valid_i,
  input  logic [ScMasterStrbSize-1:0]   p0_w_strb_i,
  input  logic                          p0_w_last_i,

  // ── L3 R/B ready ──────────────────────────────────────────
  input  logic [3:0]                    l3_r_ready_i,
  input  logic [3:0]                    l3_b_ready_i,

  // ── Sys AXI master responses (inputs to DUT) ──────────────
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
  // Outputs
  // ══════════════════════════════════════════════════════════

  // Per-port L3 AR/AW/W ready
  output logic [3:0]                    l3_ar_ready_o,
  output logic [3:0]                    l3_aw_ready_o,
  output logic [3:0]                    l3_w_ready_o,

  // Per-port L3 R valid + port 0 fields
  output logic [3:0]                    l3_r_valid_o,
  output logic [ScSlaveIdSize-1:0]      p0_r_id_o,
  output logic [1:0]                    p0_r_resp_o,
  output logic                          p0_r_last_o,

  // Per-port L3 B valid + port 0 fields
  output logic [3:0]                    l3_b_valid_o,
  output logic [ScSlaveIdSize-1:0]      p0_b_id_o,
  output logic [1:0]                    p0_b_resp_o,

  // Sys AR
  output logic                          sys_ar_valid_o,
  output logic [ScMasterIdSize-1:0]     sys_ar_id_o,
  output logic [ScMasterAddrSize-1:0]   sys_ar_addr_o,
  output logic [AxSizeSize-1:0]         sys_ar_size_o,
  output logic [AxQosSize-1:0]          sys_ar_qos_o,
  output logic [ScMasterArUserSize-1:0] sys_ar_user_o,

  // Sys AW
  output logic                          sys_aw_valid_o,
  output logic [ScMasterIdSize-1:0]     sys_aw_id_o,
  output logic [ScMasterAddrSize-1:0]   sys_aw_addr_o,
  output logic [ScMasterAwUserSize-1:0] sys_aw_user_o,

  // Sys W
  output logic                          sys_w_valid_o,
  output logic                          sys_w_last_o,

  // Sys R/B ready
  output logic                          sys_r_ready_o,
  output logic                          sys_b_ready_o
);

  // ── Build per-port AR structs from scalars ──────────────────
  sc_slave_ar_t [3:0] l3_ar_in;
  always_comb begin
    l3_ar_in = '0;
    l3_ar_in[0].id    = p0_ar_id_i;
    l3_ar_in[0].addr  = p0_ar_addr_i;
    l3_ar_in[0].size  = p0_ar_size_i;
    l3_ar_in[0].qos   = p0_ar_qos_i;
    l3_ar_in[0].user  = p0_ar_user_i;
    l3_ar_in[0].len   = '0;
    l3_ar_in[0].burst = 2'b01;
    l3_ar_in[1].id    = p1_ar_id_i;
    l3_ar_in[1].addr  = p1_ar_addr_i;
    l3_ar_in[1].len   = '0;
    l3_ar_in[1].burst = 2'b01;
  end

  // ── Build per-port AW structs from scalars ──────────────────
  sc_slave_aw_t [3:0] l3_aw_in;
  always_comb begin
    l3_aw_in = '0;
    l3_aw_in[0].id    = p0_aw_id_i;
    l3_aw_in[0].addr  = p0_aw_addr_i;
    l3_aw_in[0].size  = p0_aw_size_i;
    l3_aw_in[0].qos   = p0_aw_qos_i;
    l3_aw_in[0].user  = p0_aw_user_i;
    l3_aw_in[0].len   = '0;
    l3_aw_in[0].burst = 2'b01;
    l3_aw_in[1].id    = p1_aw_id_i;
    l3_aw_in[1].addr  = p1_aw_addr_i;
    l3_aw_in[1].user  = p1_aw_user_i;
    l3_aw_in[1].len   = '0;
    l3_aw_in[1].burst = 2'b01;
  end

  // ── Build per-port W structs from scalars ───────────────────
  sc_master_w_t [3:0] l3_w_in;
  always_comb begin
    l3_w_in = '0;
    l3_w_in[0].strb = p0_w_strb_i;
    l3_w_in[0].last = p0_w_last_i;
    // Other ports: default data=0, strb=0, last=1
    l3_w_in[1].last = 1'b1;
    l3_w_in[2].last = 1'b1;
    l3_w_in[3].last = 1'b1;
  end

  // ── Build sys R/B input structs ─────────────────────────────
  sc_master_r_t sys_r_in;
  assign sys_r_in.id   = sys_r_id_i;
  assign sys_r_in.resp = axi_pkg::resp_e'(sys_r_resp_i);
  assign sys_r_in.last = sys_r_last_i;
  assign sys_r_in.data = '0;

  sc_master_b_t sys_b_in;
  assign sys_b_in.id   = sys_b_id_i;
  assign sys_b_in.resp = axi_pkg::resp_e'(sys_b_resp_i);

  // ── DUT output wires ────────────────────────────────────────
  sc_slave_r_t [3:0] l3_r_out;
  sc_slave_b_t [3:0] l3_b_out;
  sc_master_ar_t     sys_ar_out;
  sc_master_aw_t     sys_aw_out;
  sc_master_w_t      sys_w_out;

  // ── DUT instantiation ───────────────────────────────────────
  shirecache_l3_to_sys_bridge #(
    .NumL3SlavePorts(4)
  ) u_dut (
    .clk_i,
    .rst_ni,
    .noc_clk_i  (clk_i),
    .noc_rst_ni (rst_ni),
    .dft_i      ('0),

    .l3_ar_valid_i (l3_ar_valid_i),
    .l3_ar_ready_o (l3_ar_ready_o),
    .l3_ar_i       (l3_ar_in),

    .l3_aw_valid_i (l3_aw_valid_i),
    .l3_aw_ready_o (l3_aw_ready_o),
    .l3_aw_i       (l3_aw_in),

    .l3_w_valid_i  (l3_w_valid_i),
    .l3_w_ready_o  (l3_w_ready_o),
    .l3_w_i        (l3_w_in),

    .l3_r_valid_o  (l3_r_valid_o),
    .l3_r_ready_i  (l3_r_ready_i),
    .l3_r_o        (l3_r_out),

    .l3_b_valid_o  (l3_b_valid_o),
    .l3_b_ready_i  (l3_b_ready_i),
    .l3_b_o        (l3_b_out),

    .sys_ar_valid_o (sys_ar_valid_o),
    .sys_ar_ready_i (sys_ar_ready_i),
    .sys_ar_o       (sys_ar_out),

    .sys_aw_valid_o (sys_aw_valid_o),
    .sys_aw_ready_i (sys_aw_ready_i),
    .sys_aw_o       (sys_aw_out),

    .sys_w_valid_o  (sys_w_valid_o),
    .sys_w_ready_i  (sys_w_ready_i),
    .sys_w_o        (sys_w_out),

    .sys_r_valid_i  (sys_r_valid_i),
    .sys_r_ready_o  (sys_r_ready_o),
    .sys_r_i        (sys_r_in),

    .sys_b_valid_i  (sys_b_valid_i),
    .sys_b_ready_o  (sys_b_ready_o),
    .sys_b_i        (sys_b_in)
  );

  // ── Extract port 0 R/B output fields ────────────────────────
  assign p0_r_id_o   = l3_r_out[0].id;
  assign p0_r_resp_o = l3_r_out[0].resp;
  assign p0_r_last_o = l3_r_out[0].last;

  assign p0_b_id_o   = l3_b_out[0].id;
  assign p0_b_resp_o = l3_b_out[0].resp;

  // ── Extract sys AXI output fields ───────────────────────────
  assign sys_ar_id_o   = sys_ar_out.id;
  assign sys_ar_addr_o = sys_ar_out.addr;
  assign sys_ar_size_o = sys_ar_out.size;
  assign sys_ar_qos_o  = sys_ar_out.qos;
  assign sys_ar_user_o = sys_ar_out.user;

  assign sys_aw_id_o   = sys_aw_out.id;
  assign sys_aw_addr_o = sys_aw_out.addr;
  assign sys_aw_user_o = sys_aw_out.user;

  assign sys_w_last_o  = sys_w_out.last;

  /* verilator lint_on WIDTHCONCAT */
  /* verilator lint_on UNOPTFLAT */
  /* verilator lint_on UNUSEDSIGNAL */

endmodule
