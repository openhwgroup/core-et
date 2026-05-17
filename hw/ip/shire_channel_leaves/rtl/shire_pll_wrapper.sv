// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module shire_pll_wrapper #(
  parameter int unsigned NumNeigh = shire_channel_leaves_pkg::NumNeigh
) (
  input  logic                                        clk_i,
  input  logic                                        rst_ni,
  input  dft_pkg::dft_t                               dft_i,

  input  logic                                        clk_ref_i,
  input  logic                                        clk_step_i,
  input  logic [3:0]                                  clk_pll_i,
  input  logic                                        clk_dll_i,
  output logic [NumNeigh-1:0]                         clk_neigh_o,
  output logic                                        clk_shire_o,
  output logic                                        clk_shire_debug_o,

  input  logic                                        pll_lock_i,
  input  logic                                        dll_lock_i,
  output logic                                        pll_lock_detect_sys_o,
  output logic                                        dll_lock_detect_sys_o,

  input  logic [NumNeigh-1:0]                         clk_gate_neigh_disable_i,
  input  logic                                        clk_gate_debug_disable_i,

  input  shire_esr_pkg::esr_pll_auto_config_t         pll_auto_cfg_i,
  output logic                                        pll_auto_busy_o,
  input  logic [shire_channel_leaves_pkg::PllConfBits-1:0] pll_auto_wdata_i,
  output logic [shire_channel_leaves_pkg::PllDataBits-1:0] pll_auto_rdata_o,
  output logic                                        pll_auto_rrdy_o,

  input  shire_esr_pkg::esr_dll_auto_config_t         dll_auto_cfg_i,
  output logic                                        dll_auto_busy_o,
  input  logic [shire_channel_leaves_pkg::DllConfBits-1:0] dll_auto_wdata_i,
  output logic [shire_channel_leaves_pkg::DllDataBits-1:0] dll_auto_rdata_o,
  output logic                                        dll_auto_rrdy_o,

  input  shire_esr_pkg::esr_clkmux_ctl_t              clk_sel_i
);

  logic [1:0] rst_sync_q;
  logic       rst_ctrl_ni;
  logic [1:0] rst_ref_sync_q;
  logic       rst_ref_ni;
  logic [1:0] pll_lock_sync_q;
  logic [1:0] dll_lock_sync_q;
  logic       unused_hard_macro_cfg;

  logic [3:0] clk_sel_in_bits;
  logic [1:0][3:0] clk_sel_sync_q;
  logic [3:0] clk_sel_bits;
  logic       clk_pll_a;
  logic       clk_pll_b;
  logic       clk_pll_o;
  logic       clk_ref_step;
  logic       clk_neigh_pre;

  logic [1:0][NumNeigh-1:0] clk_gate_neigh_disable_sync_q;
  logic [1:0]               clk_gate_debug_disable_sync_q;

  /* verilator lint_off UNUSEDSIGNAL */  // The native wrapper keeps the retained APB-control sequencer but omits hard-macro APB pins.
  logic pll_pclk;
  logic pll_preset_n;
  logic [shire_channel_leaves_pkg::PllAddrBits-1:0] pll_paddr;
  logic pll_psel;
  logic pll_penable;
  logic pll_pwrite;
  logic [shire_channel_leaves_pkg::PllDataBits-1:0] pll_pwdata;
  logic [shire_channel_leaves_pkg::PllNumDataBytes-1:0] pll_pstrb;
  logic dll_pclk;
  logic dll_preset_n;
  logic [shire_channel_leaves_pkg::DllAddrBits-1:0] dll_paddr;
  logic dll_psel;
  logic dll_penable;
  logic dll_pwrite;
  logic [shire_channel_leaves_pkg::DllDataBits-1:0] dll_pwdata;
  logic [shire_channel_leaves_pkg::DllNumDataBytes-1:0] dll_pstrb;
  logic unused_xll_apb;
  /* verilator lint_on UNUSEDSIGNAL */

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      rst_sync_q <= '0;
    end else begin
      rst_sync_q <= {rst_sync_q[0], 1'b1};
    end
  end

  assign rst_ctrl_ni = dft_i.scanmode ? dft_i.scan_reset_n : rst_sync_q[1];

  always_ff @(posedge clk_ref_i or negedge rst_ni) begin
    if (!rst_ni) begin
      rst_ref_sync_q <= '0;
    end else begin
      rst_ref_sync_q <= {rst_ref_sync_q[0], 1'b1};
    end
  end

  assign rst_ref_ni = dft_i.scanmode ? dft_i.scan_reset_n : rst_ref_sync_q[1];
  assign clk_sel_in_bits = clk_sel_i;

  always_ff @(posedge clk_ref_i or negedge rst_ref_ni) begin
    if (!rst_ref_ni) begin
      clk_sel_sync_q <= '0;
      clk_sel_bits   <= '0;
    end else begin
      clk_sel_sync_q[0] <= clk_sel_in_bits;
      clk_sel_sync_q[1] <= clk_sel_sync_q[0];
      clk_sel_bits      <= clk_sel_sync_q[1];
    end
  end

  always_ff @(posedge clk_i or negedge rst_ctrl_ni) begin
    if (!rst_ctrl_ni) begin
      pll_lock_sync_q <= '0;
      dll_lock_sync_q <= '0;
    end else begin
      pll_lock_sync_q <= {pll_lock_sync_q[0], pll_lock_i};
      dll_lock_sync_q <= {dll_lock_sync_q[0], dll_lock_i};
    end
  end

  assign pll_lock_detect_sys_o = pll_lock_sync_q[1];
  assign dll_lock_detect_sys_o = dll_lock_sync_q[1];
  assign unused_hard_macro_cfg = ^{pll_auto_cfg_i.lock_reset_disable,
                                   dll_auto_cfg_i.dll_enable,
                                   dll_auto_cfg_i.lock_reset_disable};

  shire_xll_control #(
    .ClockDivideBits    (shire_channel_leaves_pkg::PllClockDivideBits),
    .ClockDivideSelBits ($clog2(shire_channel_leaves_pkg::PllClockDivideBits)),
    .RegisterPtrBits    (shire_channel_leaves_pkg::PllClockRegPtrBits),
    .RegisterNumBits    (shire_channel_leaves_pkg::PllClockRegNumBits),
    .RegisterAddrBits   (shire_channel_leaves_pkg::PllAddrBits),
    .RegisterDataBits   (shire_channel_leaves_pkg::PllDataBits),
    .RegisterDataBytes  (shire_channel_leaves_pkg::PllNumDataBytes),
    .RegisterWdataBits  (shire_channel_leaves_pkg::PllConfBits)
  ) u_pll_control (
    .clk_i           (clk_i),
    .rst_ni          (rst_ctrl_ni),
    .esr_enable_i    (pll_auto_cfg_i.enable),
    .esr_reset_i     (pll_auto_cfg_i.reset),
    .esr_run_i       (pll_auto_cfg_i.run),
    .esr_write_i     (pll_auto_cfg_i.write),
    .esr_reg_first_i (pll_auto_cfg_i.reg_first),
    .esr_reg_num_i   (pll_auto_cfg_i.reg_num),
    .esr_pclk_sel_i  (pll_auto_cfg_i.pclk_sel),
    .esr_wdata_i     (pll_auto_wdata_i),
    .esr_busy_o      (pll_auto_busy_o),
    .esr_rrdy_o      (pll_auto_rrdy_o),
    .esr_rdata_o     (pll_auto_rdata_o),
    .pclk_o          (pll_pclk),
    .preset_no       (pll_preset_n),
    .paddr_o         (pll_paddr),
    .psel_o          (pll_psel),
    .penable_o       (pll_penable),
    .pwrite_o        (pll_pwrite),
    .pwdata_o        (pll_pwdata),
    .pstrb_o         (pll_pstrb),
    .prdata_i        ('0),
    .pready_i        (1'b1)
  );

  shire_xll_control #(
    .ClockDivideBits    (shire_channel_leaves_pkg::DllClockDivideBits),
    .ClockDivideSelBits ($clog2(shire_channel_leaves_pkg::DllClockDivideBits)),
    .RegisterPtrBits    (shire_channel_leaves_pkg::DllClockRegPtrBits),
    .RegisterNumBits    (shire_channel_leaves_pkg::DllClockRegNumBits),
    .RegisterAddrBits   (shire_channel_leaves_pkg::DllAddrBits),
    .RegisterDataBits   (shire_channel_leaves_pkg::DllDataBits),
    .RegisterDataBytes  (shire_channel_leaves_pkg::DllNumDataBytes),
    .RegisterWdataBits  (shire_channel_leaves_pkg::DllConfBits)
  ) u_dll_control (
    .clk_i           (clk_i),
    .rst_ni          (rst_ctrl_ni),
    .esr_enable_i    (dll_auto_cfg_i.enable),
    .esr_reset_i     (dll_auto_cfg_i.reset),
    .esr_run_i       (dll_auto_cfg_i.run),
    .esr_write_i     (dll_auto_cfg_i.write),
    .esr_reg_first_i (dll_auto_cfg_i.reg_first),
    .esr_reg_num_i   (dll_auto_cfg_i.reg_num),
    .esr_pclk_sel_i  (dll_auto_cfg_i.pclk_sel),
    .esr_wdata_i     (dll_auto_wdata_i),
    .esr_busy_o      (dll_auto_busy_o),
    .esr_rrdy_o      (dll_auto_rrdy_o),
    .esr_rdata_o     (dll_auto_rdata_o),
    .pclk_o          (dll_pclk),
    .preset_no       (dll_preset_n),
    .paddr_o         (dll_paddr),
    .psel_o          (dll_psel),
    .penable_o       (dll_penable),
    .pwrite_o        (dll_pwrite),
    .pwdata_o        (dll_pwdata),
    .pstrb_o         (dll_pstrb),
    .prdata_i        ('0),
    .pready_i        (1'b1)
  );

  assign unused_xll_apb = ^{pll_pclk, pll_preset_n, pll_paddr, pll_psel, pll_penable,
                           pll_pwrite, pll_pwdata, pll_pstrb, dll_pclk, dll_preset_n,
                           dll_paddr, dll_psel, dll_penable, dll_pwrite, dll_pwdata,
                           dll_pstrb, unused_hard_macro_cfg};

  prim_clk_mux u_pll_mux_a (
    .clk0_i (clk_pll_i[0]),
    .clk1_i (clk_pll_i[1]),
    .sel_i  (clk_sel_bits[0]),
    .clk_o  (clk_pll_a)
  );

  prim_clk_mux u_pll_mux_b (
    .clk0_i (clk_pll_i[2]),
    .clk1_i (clk_pll_i[3]),
    .sel_i  (clk_sel_bits[0]),
    .clk_o  (clk_pll_b)
  );

  prim_clk_mux u_pll_mux_o (
    .clk0_i (clk_pll_a),
    .clk1_i (clk_pll_b),
    .sel_i  (clk_sel_bits[1]),
    .clk_o  (clk_pll_o)
  );

  prim_clk_mux u_ref_step_mux (
    .clk0_i (clk_ref_i),
    .clk1_i (clk_step_i),
    .sel_i  (clk_sel_bits[1]),
    .clk_o  (clk_ref_step)
  );

  prim_clk_mux u_shire_mux (
    .clk0_i (clk_ref_step),
    .clk1_i (clk_pll_o),
    .sel_i  (clk_sel_bits[2]),
    .clk_o  (clk_shire_o)
  );

  prim_clk_mux u_neigh_dll_mux (
    .clk0_i (clk_shire_o),
    .clk1_i (clk_dll_i),
    .sel_i  (clk_sel_bits[3]),
    .clk_o  (clk_neigh_pre)
  );

  always_ff @(posedge clk_neigh_pre or negedge rst_ni) begin
    if (!rst_ni) begin
      clk_gate_neigh_disable_sync_q <= '0;
    end else begin
      clk_gate_neigh_disable_sync_q[0] <= clk_gate_neigh_disable_i;
      clk_gate_neigh_disable_sync_q[1] <= clk_gate_neigh_disable_sync_q[0];
    end
  end

  for (genvar neigh_idx = 0; neigh_idx < NumNeigh; neigh_idx++) begin : gen_neigh_gate
    prim_clk_gate u_neigh_clk_gate (
      .clk_i (clk_neigh_pre),
      .en_i  (~clk_gate_neigh_disable_sync_q[1][neigh_idx]),
      .dft_i (dft_i),
      .clk_o (clk_neigh_o[neigh_idx])
    );
  end

  always_ff @(posedge clk_shire_o or negedge rst_ni) begin
    if (!rst_ni) begin
      clk_gate_debug_disable_sync_q <= '0;
    end else begin
      clk_gate_debug_disable_sync_q <= {clk_gate_debug_disable_sync_q[0],
                                        clk_gate_debug_disable_i};
    end
  end

  prim_clk_gate u_debug_clk_gate (
    .clk_i (clk_shire_o),
    .en_i  (~clk_gate_debug_disable_sync_q[1]),
    .dft_i (dft_i),
    .clk_o (clk_shire_debug_o)
  );

endmodule
