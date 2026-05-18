// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_uncached_hi_voltage_cross_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic dft_hv_scanmode_i,
  input  logic dft_hv_scan_reset_ni,
  input  logic dft_lv_scanmode_i,
  input  logic dft_lv_scan_reset_ni,
  input  logic [$bits(axi_pkg::sc_master_ar_t)-1:0] to_axi_l3_ar_bits_i,
  input  logic to_axi_l3_ar_valid_i,
  input  logic [$bits(axi_pkg::sc_master_aw_t)-1:0] to_axi_l3_aw_bits_i,
  input  logic to_axi_l3_aw_valid_i,
  input  logic [$bits(axi_pkg::sc_master_w_t)-1:0] to_axi_l3_w_bits_i,
  input  logic to_axi_l3_w_valid_i,
  input  logic to_axi_l3_ar_ready_lo_i,
  input  logic to_axi_l3_aw_ready_lo_i,
  input  logic to_axi_l3_w_ready_lo_i,
  input  logic [$bits(axi_pkg::sc_master_ar_t)-1:0] to_axi_sys_ar_bits_i,
  input  logic to_axi_sys_ar_valid_i,
  input  logic [$bits(axi_pkg::sc_master_aw_t)-1:0] to_axi_sys_aw_bits_i,
  input  logic to_axi_sys_aw_valid_i,
  input  logic [$bits(axi_pkg::sc_master_w_t)-1:0] to_axi_sys_w_bits_i,
  input  logic to_axi_sys_w_valid_i,
  input  logic to_axi_sys_ar_ready_lo_i,
  input  logic to_axi_sys_aw_ready_lo_i,
  input  logic to_axi_sys_w_ready_lo_i,
  input  logic [$bits(axi_pkg::sys_slave_r_t)-1:0] from_axi_sys_r_bits_i,
  input  logic from_axi_sys_r_valid_i,
  input  logic from_axi_sys_r_ready_lo_i,
  input  logic [$bits(axi_pkg::sys_slave_b_t)-1:0] from_axi_sys_b_bits_i,
  input  logic [1:0] from_axi_sys_credit_i,
  input  logic from_axi_sys_b_valid_i,
  input  logic from_axi_sys_b_ready_lo_i,
  output logic orig_to_axi_l3_ar_ready_o,
  output logic new_to_axi_l3_ar_ready_o,
  output logic orig_to_axi_l3_aw_ready_o,
  output logic new_to_axi_l3_aw_ready_o,
  output logic orig_to_axi_l3_w_ready_o,
  output logic new_to_axi_l3_w_ready_o,
  output logic [$bits(axi_pkg::sc_master_ar_t)-1:0] orig_to_axi_l3_ar_lo_bits_o,
  output logic [$bits(axi_pkg::sc_master_ar_t)-1:0] new_to_axi_l3_ar_lo_bits_o,
  output logic orig_to_axi_l3_ar_valid_lo_o,
  output logic new_to_axi_l3_ar_valid_lo_o,
  output logic [$bits(axi_pkg::sc_master_aw_t)-1:0] orig_to_axi_l3_aw_lo_bits_o,
  output logic [$bits(axi_pkg::sc_master_aw_t)-1:0] new_to_axi_l3_aw_lo_bits_o,
  output logic orig_to_axi_l3_aw_valid_lo_o,
  output logic new_to_axi_l3_aw_valid_lo_o,
  output logic [$bits(axi_pkg::sc_master_w_t)-1:0] orig_to_axi_l3_w_lo_bits_o,
  output logic [$bits(axi_pkg::sc_master_w_t)-1:0] new_to_axi_l3_w_lo_bits_o,
  output logic orig_to_axi_l3_w_valid_lo_o,
  output logic new_to_axi_l3_w_valid_lo_o,
  output logic orig_to_axi_sys_ar_ready_o,
  output logic new_to_axi_sys_ar_ready_o,
  output logic orig_to_axi_sys_aw_ready_o,
  output logic new_to_axi_sys_aw_ready_o,
  output logic orig_to_axi_sys_w_ready_o,
  output logic new_to_axi_sys_w_ready_o,
  output logic [$bits(axi_pkg::sc_master_ar_t)-1:0] orig_to_axi_sys_ar_lo_bits_o,
  output logic [$bits(axi_pkg::sc_master_ar_t)-1:0] new_to_axi_sys_ar_lo_bits_o,
  output logic orig_to_axi_sys_ar_valid_lo_o,
  output logic new_to_axi_sys_ar_valid_lo_o,
  output logic [$bits(axi_pkg::sc_master_aw_t)-1:0] orig_to_axi_sys_aw_lo_bits_o,
  output logic [$bits(axi_pkg::sc_master_aw_t)-1:0] new_to_axi_sys_aw_lo_bits_o,
  output logic orig_to_axi_sys_aw_valid_lo_o,
  output logic new_to_axi_sys_aw_valid_lo_o,
  output logic [$bits(axi_pkg::sc_master_w_t)-1:0] orig_to_axi_sys_w_lo_bits_o,
  output logic [$bits(axi_pkg::sc_master_w_t)-1:0] new_to_axi_sys_w_lo_bits_o,
  output logic orig_to_axi_sys_w_valid_lo_o,
  output logic new_to_axi_sys_w_valid_lo_o,
  output logic orig_from_axi_sys_r_ready_o,
  output logic new_from_axi_sys_r_ready_o,
  output logic [$bits(axi_pkg::sys_slave_r_t)-1:0] orig_from_axi_sys_r_lo_bits_o,
  output logic [$bits(axi_pkg::sys_slave_r_t)-1:0] new_from_axi_sys_r_lo_bits_o,
  output logic orig_from_axi_sys_r_valid_lo_o,
  output logic new_from_axi_sys_r_valid_lo_o,
  output logic orig_from_axi_sys_b_ready_o,
  output logic new_from_axi_sys_b_ready_o,
  output logic [$bits(axi_pkg::sys_slave_b_t)-1:0] orig_from_axi_sys_b_lo_bits_o,
  output logic [$bits(axi_pkg::sys_slave_b_t)-1:0] new_from_axi_sys_b_lo_bits_o,
  output logic [1:0] orig_from_axi_sys_credit_lo_o,
  output logic [1:0] new_from_axi_sys_credit_lo_o,
  output logic orig_from_axi_sys_b_valid_lo_o,
  output logic new_from_axi_sys_b_valid_lo_o
);

  import dft_pkg::*;
  dft_t dft_hv, dft_lv;
  assign dft_hv = '{scanmode: dft_hv_scanmode_i, scan_reset_n: dft_hv_scan_reset_ni,
                    sram_clk_override: 1'b0, ram_rei: 1'b0, ram_wei: 1'b0};
  assign dft_lv = '{scanmode: dft_lv_scanmode_i, scan_reset_n: dft_lv_scan_reset_ni,
                    sram_clk_override: 1'b0, ram_rei: 1'b0, ram_wei: 1'b0};

  sc_master_axi_AR_channel_t orig_l3_ar, orig_l3_ar_lo, orig_sys_ar, orig_sys_ar_lo;
  axi_pkg::sc_master_ar_t new_l3_ar, new_l3_ar_lo, new_sys_ar, new_sys_ar_lo;
  sc_master_axi_AW_channel_t orig_l3_aw, orig_l3_aw_lo, orig_sys_aw, orig_sys_aw_lo;
  axi_pkg::sc_master_aw_t new_l3_aw, new_l3_aw_lo, new_sys_aw, new_sys_aw_lo;
  sc_master_axi_W_channel_t orig_l3_w, orig_l3_w_lo, orig_sys_w, orig_sys_w_lo;
  axi_pkg::sc_master_w_t new_l3_w, new_l3_w_lo, new_sys_w, new_sys_w_lo;
  sys_slave_axi_R_channel_t orig_sys_r, orig_sys_r_lo;
  axi_pkg::sys_slave_r_t new_sys_r, new_sys_r_lo;
  sys_slave_axi_B_channel_t orig_sys_b, orig_sys_b_lo;
  axi_pkg::sys_slave_b_t new_sys_b, new_sys_b_lo;

  assign orig_l3_ar = to_axi_l3_ar_bits_i[$bits(sc_master_axi_AR_channel_t)-1:0];
  assign new_l3_ar = to_axi_l3_ar_bits_i;
  assign orig_l3_aw = to_axi_l3_aw_bits_i[$bits(sc_master_axi_AW_channel_t)-1:0];
  assign new_l3_aw = to_axi_l3_aw_bits_i;
  assign orig_l3_w = to_axi_l3_w_bits_i[$bits(sc_master_axi_W_channel_t)-1:0];
  assign new_l3_w = to_axi_l3_w_bits_i;
  assign orig_sys_ar = to_axi_sys_ar_bits_i[$bits(sc_master_axi_AR_channel_t)-1:0];
  assign new_sys_ar = to_axi_sys_ar_bits_i;
  assign orig_sys_aw = to_axi_sys_aw_bits_i[$bits(sc_master_axi_AW_channel_t)-1:0];
  assign new_sys_aw = to_axi_sys_aw_bits_i;
  assign orig_sys_w = to_axi_sys_w_bits_i[$bits(sc_master_axi_W_channel_t)-1:0];
  assign new_sys_w = to_axi_sys_w_bits_i;
  assign orig_sys_r = from_axi_sys_r_bits_i[$bits(sys_slave_axi_R_channel_t)-1:0];
  assign new_sys_r = from_axi_sys_r_bits_i;
  assign orig_sys_b = from_axi_sys_b_bits_i[$bits(sys_slave_axi_B_channel_t)-1:0];
  assign new_sys_b = from_axi_sys_b_bits_i;

  assign orig_to_axi_l3_ar_lo_bits_o = orig_l3_ar_lo;
  assign new_to_axi_l3_ar_lo_bits_o = new_l3_ar_lo;
  assign orig_to_axi_l3_aw_lo_bits_o = orig_l3_aw_lo;
  assign new_to_axi_l3_aw_lo_bits_o = new_l3_aw_lo;
  assign orig_to_axi_l3_w_lo_bits_o = orig_l3_w_lo;
  assign new_to_axi_l3_w_lo_bits_o = new_l3_w_lo;
  assign orig_to_axi_sys_ar_lo_bits_o = orig_sys_ar_lo;
  assign new_to_axi_sys_ar_lo_bits_o = new_sys_ar_lo;
  assign orig_to_axi_sys_aw_lo_bits_o = orig_sys_aw_lo;
  assign new_to_axi_sys_aw_lo_bits_o = new_sys_aw_lo;
  assign orig_to_axi_sys_w_lo_bits_o = orig_sys_w_lo;
  assign new_to_axi_sys_w_lo_bits_o = new_sys_w_lo;
  assign orig_from_axi_sys_r_lo_bits_o = orig_sys_r_lo;
  assign new_from_axi_sys_r_lo_bits_o = new_sys_r_lo;
  assign orig_from_axi_sys_b_lo_bits_o = orig_sys_b_lo;
  assign new_from_axi_sys_b_lo_bits_o = new_sys_b_lo;

  uncached_hi_voltage_cross_orig u_orig (
    .reset_push(!rst_ni), .clock_push(clk_i), .reset_pop(!rst_ni), .clock_pop(clk_i),
    .to_axi_l3_AR(orig_l3_ar), .to_axi_l3_ARVALID(to_axi_l3_ar_valid_i), .to_axi_l3_ARREADY(orig_to_axi_l3_ar_ready_o),
    .to_axi_l3_AW(orig_l3_aw), .to_axi_l3_AWVALID(to_axi_l3_aw_valid_i), .to_axi_l3_AWREADY(orig_to_axi_l3_aw_ready_o),
    .to_axi_l3_W(orig_l3_w), .to_axi_l3_WVALID(to_axi_l3_w_valid_i), .to_axi_l3_WREADY(orig_to_axi_l3_w_ready_o),
    .to_axi_l3_AR_lo(orig_l3_ar_lo), .to_axi_l3_ARVALID_lo(orig_to_axi_l3_ar_valid_lo_o), .to_axi_l3_ARREADY_lo(to_axi_l3_ar_ready_lo_i),
    .to_axi_l3_AW_lo(orig_l3_aw_lo), .to_axi_l3_AWVALID_lo(orig_to_axi_l3_aw_valid_lo_o), .to_axi_l3_AWREADY_lo(to_axi_l3_aw_ready_lo_i),
    .to_axi_l3_W_lo(orig_l3_w_lo), .to_axi_l3_WVALID_lo(orig_to_axi_l3_w_valid_lo_o), .to_axi_l3_WREADY_lo(to_axi_l3_w_ready_lo_i),
    .to_axi_sys_AR(orig_sys_ar), .to_axi_sys_ARVALID(to_axi_sys_ar_valid_i), .to_axi_sys_ARREADY(orig_to_axi_sys_ar_ready_o),
    .to_axi_sys_AW(orig_sys_aw), .to_axi_sys_AWVALID(to_axi_sys_aw_valid_i), .to_axi_sys_AWREADY(orig_to_axi_sys_aw_ready_o),
    .to_axi_sys_W(orig_sys_w), .to_axi_sys_WVALID(to_axi_sys_w_valid_i), .to_axi_sys_WREADY(orig_to_axi_sys_w_ready_o),
    .to_axi_sys_AR_lo(orig_sys_ar_lo), .to_axi_sys_ARVALID_lo(orig_to_axi_sys_ar_valid_lo_o), .to_axi_sys_ARREADY_lo(to_axi_sys_ar_ready_lo_i),
    .to_axi_sys_AW_lo(orig_sys_aw_lo), .to_axi_sys_AWVALID_lo(orig_to_axi_sys_aw_valid_lo_o), .to_axi_sys_AWREADY_lo(to_axi_sys_aw_ready_lo_i),
    .to_axi_sys_W_lo(orig_sys_w_lo), .to_axi_sys_WVALID_lo(orig_to_axi_sys_w_valid_lo_o), .to_axi_sys_WREADY_lo(to_axi_sys_w_ready_lo_i),
    .from_axi_sys_R(orig_sys_r), .from_axi_sys_RVALID(from_axi_sys_r_valid_i), .from_axi_sys_RREADY(orig_from_axi_sys_r_ready_o),
    .from_axi_sys_R_lo(orig_sys_r_lo), .from_axi_sys_RVALID_lo(orig_from_axi_sys_r_valid_lo_o), .from_axi_sys_RREADY_lo(from_axi_sys_r_ready_lo_i),
    .from_axi_sys_B(orig_sys_b), .from_axi_sys_credit(from_axi_sys_credit_i), .from_axi_sys_BVALID(from_axi_sys_b_valid_i), .from_axi_sys_BREADY(orig_from_axi_sys_b_ready_o),
    .from_axi_sys_B_lo(orig_sys_b_lo), .from_axi_sys_credit_lo(orig_from_axi_sys_credit_lo_o), .from_axi_sys_BVALID_lo(orig_from_axi_sys_b_valid_lo_o), .from_axi_sys_BREADY_lo(from_axi_sys_b_ready_lo_i),
    .dft__reset_byp_push(dft_hv_scanmode_i), .dft__reset_push(!dft_hv_scan_reset_ni),
    .dft__reset_byp_pop(dft_lv_scanmode_i), .dft__reset_pop(!dft_lv_scan_reset_ni)
  );

  uncached_hi_voltage_cross u_new (
    .clk_hv_i(clk_i), .rst_hv_ni(rst_ni), .clk_lv_i(clk_i), .rst_lv_ni(rst_ni),
    .to_axi_l3_ar_i(new_l3_ar), .to_axi_l3_ar_valid_i(to_axi_l3_ar_valid_i), .to_axi_l3_ar_ready_o(new_to_axi_l3_ar_ready_o),
    .to_axi_l3_aw_i(new_l3_aw), .to_axi_l3_aw_valid_i(to_axi_l3_aw_valid_i), .to_axi_l3_aw_ready_o(new_to_axi_l3_aw_ready_o),
    .to_axi_l3_w_i(new_l3_w), .to_axi_l3_w_valid_i(to_axi_l3_w_valid_i), .to_axi_l3_w_ready_o(new_to_axi_l3_w_ready_o),
    .to_axi_l3_ar_lo_o(new_l3_ar_lo), .to_axi_l3_ar_valid_lo_o(new_to_axi_l3_ar_valid_lo_o), .to_axi_l3_ar_ready_lo_i(to_axi_l3_ar_ready_lo_i),
    .to_axi_l3_aw_lo_o(new_l3_aw_lo), .to_axi_l3_aw_valid_lo_o(new_to_axi_l3_aw_valid_lo_o), .to_axi_l3_aw_ready_lo_i(to_axi_l3_aw_ready_lo_i),
    .to_axi_l3_w_lo_o(new_l3_w_lo), .to_axi_l3_w_valid_lo_o(new_to_axi_l3_w_valid_lo_o), .to_axi_l3_w_ready_lo_i(to_axi_l3_w_ready_lo_i),
    .to_axi_sys_ar_i(new_sys_ar), .to_axi_sys_ar_valid_i(to_axi_sys_ar_valid_i), .to_axi_sys_ar_ready_o(new_to_axi_sys_ar_ready_o),
    .to_axi_sys_aw_i(new_sys_aw), .to_axi_sys_aw_valid_i(to_axi_sys_aw_valid_i), .to_axi_sys_aw_ready_o(new_to_axi_sys_aw_ready_o),
    .to_axi_sys_w_i(new_sys_w), .to_axi_sys_w_valid_i(to_axi_sys_w_valid_i), .to_axi_sys_w_ready_o(new_to_axi_sys_w_ready_o),
    .to_axi_sys_ar_lo_o(new_sys_ar_lo), .to_axi_sys_ar_valid_lo_o(new_to_axi_sys_ar_valid_lo_o), .to_axi_sys_ar_ready_lo_i(to_axi_sys_ar_ready_lo_i),
    .to_axi_sys_aw_lo_o(new_sys_aw_lo), .to_axi_sys_aw_valid_lo_o(new_to_axi_sys_aw_valid_lo_o), .to_axi_sys_aw_ready_lo_i(to_axi_sys_aw_ready_lo_i),
    .to_axi_sys_w_lo_o(new_sys_w_lo), .to_axi_sys_w_valid_lo_o(new_to_axi_sys_w_valid_lo_o), .to_axi_sys_w_ready_lo_i(to_axi_sys_w_ready_lo_i),
    .from_axi_sys_r_i(new_sys_r), .from_axi_sys_r_valid_i(from_axi_sys_r_valid_i), .from_axi_sys_r_ready_o(new_from_axi_sys_r_ready_o),
    .from_axi_sys_r_lo_o(new_sys_r_lo), .from_axi_sys_r_valid_lo_o(new_from_axi_sys_r_valid_lo_o), .from_axi_sys_r_ready_lo_i(from_axi_sys_r_ready_lo_i),
    .from_axi_sys_b_i(new_sys_b), .from_axi_sys_credit_i(from_axi_sys_credit_i), .from_axi_sys_b_valid_i(from_axi_sys_b_valid_i), .from_axi_sys_b_ready_o(new_from_axi_sys_b_ready_o),
    .from_axi_sys_b_lo_o(new_sys_b_lo), .from_axi_sys_credit_lo_o(new_from_axi_sys_credit_lo_o), .from_axi_sys_b_valid_lo_o(new_from_axi_sys_b_valid_lo_o), .from_axi_sys_b_ready_lo_i(from_axi_sys_b_ready_lo_i),
    .dft_hv_i(dft_hv), .dft_lv_i(dft_lv)
  );

endmodule : cosim_uncached_hi_voltage_cross_tb
