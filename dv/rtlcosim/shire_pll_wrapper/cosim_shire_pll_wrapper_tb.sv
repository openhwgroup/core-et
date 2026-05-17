// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_shire_pll_wrapper_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [3:0]  clk_sel_bits_i,
  input  logic [3:0]  clk_gate_neigh_disable_i,
  input  logic        clk_gate_debug_disable_i,
  input  logic [16:0] pll_cfg_bits_i,
  input  logic [13:0] dll_cfg_bits_i,
  input  logic [255:0] pll_wdata_i,
  input  logic [15:0]  dll_wdata_i,
  output logic [3:0]  orig_clk_neigh_o,
  output logic [3:0]  new_clk_neigh_o,
  output logic        orig_clk_shire_o,
  output logic        new_clk_shire_o,
  output logic        orig_clk_debug_o,
  output logic        new_clk_debug_o,
  output logic        orig_pll_lock_o,
  output logic        new_pll_lock_o,
  output logic        orig_dll_lock_o,
  output logic        new_dll_lock_o,
  output logic        orig_pll_busy_o,
  output logic        new_pll_busy_o,
  output logic        orig_pll_rrdy_o,
  output logic        new_pll_rrdy_o,
  output logic [15:0] orig_pll_rdata_o,
  output logic [15:0] new_pll_rdata_o,
  output logic        orig_dll_busy_o,
  output logic        new_dll_busy_o,
  output logic        orig_dll_rrdy_o,
  output logic        new_dll_rrdy_o,
  output logic [15:0] orig_dll_rdata_o,
  output logic [15:0] new_dll_rdata_o
);
  esr_pll_auto_config_t orig_pll_cfg;
  esr_dll_auto_config_t orig_dll_cfg;
  esr_clk_dly_ctl_t orig_clk_dly_ctl;
  et_tdr_cntl_t orig_tdr_cntl;
  shire_tdr_clk_cntl_t orig_tdr_clk_cntl;
  logic orig_tdo_pll;
  logic orig_sram_clock;
  logic orig_clk_override;
  logic orig_yin_out;
  logic orig_yang_out;

  dft_pkg::dft_t new_dft;
  shire_esr_pkg::esr_pll_auto_config_t new_pll_cfg;
  shire_esr_pkg::esr_dll_auto_config_t new_dll_cfg;
  shire_esr_pkg::esr_clkmux_ctl_t new_clk_sel;

  logic clk_ref_new;
  logic [3:0] clk_pll_new;

  assign orig_pll_cfg = pll_cfg_bits_i;
  assign orig_dll_cfg = dll_cfg_bits_i;
  assign orig_clk_dly_ctl = '0;
  assign orig_tdr_cntl = '{trstn: 1'b1, default: '0};
  assign orig_tdr_clk_cntl = '0;

  assign new_dft = '0;
  assign new_pll_cfg = pll_cfg_bits_i;
  assign new_dll_cfg = dll_cfg_bits_i;
  assign new_clk_sel = clk_sel_bits_i;
  assign clk_ref_new = ~clk_i;
  assign clk_pll_new = {4{~clk_i}};

  shire_pll_wrapper_orig u_orig (
    .clock                     (clk_i),
    .reset                     (!rst_ni),
    .i_clkref                  (clk_i),
    .o_clkpll_neigh            (orig_clk_neigh_o),
    .o_clkpll_shire            (orig_clk_shire_o),
    .o_clkpll_shire_debug      (orig_clk_debug_o),
    .o_pll_lock_detect_sys     (orig_pll_lock_o),
    .o_dll_lock_detect_sys     (orig_dll_lock_o),
    .clk_gate_neigh_disable    (clk_gate_neigh_disable_i),
    .clk_gate_debug_disable    (clk_gate_debug_disable_i),
    .i_pll_auto_cfg            (orig_pll_cfg),
    .o_pll_auto_busy           (orig_pll_busy_o),
    .i_pll_auto_wdata          (pll_wdata_i),
    .o_pll_auto_rdata          (orig_pll_rdata_o),
    .o_pll_auto_rrdy           (orig_pll_rrdy_o),
    .i_dll_auto_cfg            (orig_dll_cfg),
    .o_dll_auto_busy           (orig_dll_busy_o),
    .i_dll_auto_wdata          (dll_wdata_i),
    .o_dll_auto_rdata          (orig_dll_rdata_o),
    .o_dll_auto_rrdy           (orig_dll_rrdy_o),
    .i_clk_dly_ctl             (orig_clk_dly_ctl),
    .i_dll_feedback_shire      (clk_i),
    .i_dll_feedback_neigh      (clk_i),
    .i_step_clock              (clk_i),
    .i_clk_sel                 (clk_sel_bits_i),
    .testmode                  (1'b0),
    .dft__tdr_cntl             (orig_tdr_cntl),
    .dft__tdr_tck              (clk_i),
    .dft__tdr_tdi_pll          (1'b0),
    .dft__tdr_tdo_pll          (orig_tdo_pll),
    .dft__sram_clock           (orig_sram_clock),
    .dft__clk_override         (orig_clk_override),
    .dft__occ_ovr              (1'b0),
    .dft__occ_ovr_clk          (clk_i),
    .yin_in                    (1'b0),
    .yin_out                   (orig_yin_out),
    .yang_in                   (1'b0),
    .yang_out                  (orig_yang_out),
    .use_shire_tdr_clk_cntl    (1'b0),
    .shire_tdr_clk_cntl        (orig_tdr_clk_cntl),
    .dft__pll_test_mode        (1'b0),
    .dft__dll_test_mode        (1'b0),
    .dft__pll_tdr_select       ('0),
    .dft__dll_tdr_select       ('0),
    .dft__pll_tdr_mode         (1'b0),
    .dft__dll_tdr_mode         (1'b0),
    .dft__sram_clock_override  (1'b0),
    .dft__rc_start_count       (1'b0),
    .dft__pll_dbg_resetn       (1'b1),
    .dft__reset_byp            (1'b0),
    .dft__reset                (1'b0),
    .clk__noc                  (clk_i)
  );

  shire_pll_wrapper u_new (
    .clk_i                    (clk_i),
    .rst_ni                   (rst_ni),
    .dft_i                    (new_dft),
    .clk_ref_i                (clk_ref_new),
    .clk_step_i               (clk_ref_new),
    .clk_pll_i                (clk_pll_new),
    .clk_dll_i                (clk_i),
    .clk_neigh_o              (new_clk_neigh_o),
    .clk_shire_o              (new_clk_shire_o),
    .clk_shire_debug_o        (new_clk_debug_o),
    .pll_lock_i               (1'b1),
    .dll_lock_i               (1'b0),
    .pll_lock_detect_sys_o    (new_pll_lock_o),
    .dll_lock_detect_sys_o    (new_dll_lock_o),
    .clk_gate_neigh_disable_i (clk_gate_neigh_disable_i),
    .clk_gate_debug_disable_i (clk_gate_debug_disable_i),
    .pll_auto_cfg_i           (new_pll_cfg),
    .pll_auto_busy_o          (new_pll_busy_o),
    .pll_auto_wdata_i         (pll_wdata_i),
    .pll_auto_rdata_o         (new_pll_rdata_o),
    .pll_auto_rrdy_o          (new_pll_rrdy_o),
    .dll_auto_cfg_i           (new_dll_cfg),
    .dll_auto_busy_o          (new_dll_busy_o),
    .dll_auto_wdata_i         (dll_wdata_i),
    .dll_auto_rdata_o         (new_dll_rdata_o),
    .dll_auto_rrdy_o          (new_dll_rrdy_o),
    .clk_sel_i                (new_clk_sel)
  );

  logic unused_orig_dft;
  assign unused_orig_dft = ^{orig_tdo_pll, orig_sram_clock, orig_clk_override,
                             orig_yin_out, orig_yang_out};
endmodule
