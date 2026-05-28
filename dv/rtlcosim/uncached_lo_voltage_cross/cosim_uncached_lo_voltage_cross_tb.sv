// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_uncached_lo_voltage_cross_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic dft_lv_scanmode_i,
  input  logic dft_lv_scan_reset_ni,
  input  logic dft_hv_scanmode_i,
  input  logic dft_hv_scan_reset_ni,

  input  logic [$bits(axi_pkg::sc_master_b_t)-1:0] to_axi_l3_b_lo_bits_i,
  input  logic to_axi_l3_b_valid_lo_i,
  input  logic to_axi_l3_b_ready_i,
  input  logic [$bits(axi_pkg::sc_master_r_t)-1:0] to_axi_l3_r_lo_bits_i,
  input  logic to_axi_l3_r_valid_lo_i,
  input  logic to_axi_l3_r_ready_i,
  input  logic [$bits(axi_pkg::sc_master_b_t)-1:0] to_axi_sys_b_lo_bits_i,
  input  logic to_axi_sys_b_valid_lo_i,
  input  logic to_axi_sys_b_ready_i,
  input  logic [$bits(axi_pkg::sc_master_r_t)-1:0] to_axi_sys_r_lo_bits_i,
  input  logic to_axi_sys_r_valid_lo_i,
  input  logic to_axi_sys_r_ready_i,

  input  logic [$bits(axi_pkg::sys_slave_ar_t)-1:0] from_axi_sys_ar_lo_bits_i,
  input  logic from_axi_sys_ar_valid_lo_i,
  input  logic from_axi_sys_ar_ready_i,
  input  logic [$bits(axi_pkg::sys_slave_aw_t)-1:0] from_axi_sys_aw_lo_bits_i,
  input  logic from_axi_sys_aw_valid_lo_i,
  input  logic [1:0] from_axi_sys_aw_vcvalid_lo_i,
  input  logic from_axi_sys_uc_aw_ready_i,
  input  logic from_axi_sys_sbm_aw_ready_i,
  input  logic [$bits(axi_pkg::sys_slave_w_t)-1:0] from_axi_sys_w_lo_bits_i,
  input  logic from_axi_sys_w_valid_lo_i,
  input  logic [1:0] from_axi_sys_w_vcvalid_lo_i,
  input  logic from_axi_sys_uc_w_ready_i,
  input  logic from_axi_sys_sbm_w_ready_i,

  output logic [$bits(axi_pkg::sc_master_b_t)-1:0] orig_to_axi_l3_b_bits_o,
  output logic [$bits(axi_pkg::sc_master_b_t)-1:0] new_to_axi_l3_b_bits_o,
  output logic orig_to_axi_l3_b_valid_o,
  output logic new_to_axi_l3_b_valid_o,
  output logic orig_to_axi_l3_b_ready_lo_o,
  output logic new_to_axi_l3_b_ready_lo_o,
  output logic [$bits(axi_pkg::sc_master_r_t)-1:0] orig_to_axi_l3_r_bits_o,
  output logic [$bits(axi_pkg::sc_master_r_t)-1:0] new_to_axi_l3_r_bits_o,
  output logic orig_to_axi_l3_r_valid_o,
  output logic new_to_axi_l3_r_valid_o,
  output logic orig_to_axi_l3_r_ready_lo_o,
  output logic new_to_axi_l3_r_ready_lo_o,

  output logic [$bits(axi_pkg::sc_master_b_t)-1:0] orig_to_axi_sys_b_bits_o,
  output logic [$bits(axi_pkg::sc_master_b_t)-1:0] new_to_axi_sys_b_bits_o,
  output logic orig_to_axi_sys_b_valid_o,
  output logic new_to_axi_sys_b_valid_o,
  output logic orig_to_axi_sys_b_ready_lo_o,
  output logic new_to_axi_sys_b_ready_lo_o,
  output logic [$bits(axi_pkg::sc_master_r_t)-1:0] orig_to_axi_sys_r_bits_o,
  output logic [$bits(axi_pkg::sc_master_r_t)-1:0] new_to_axi_sys_r_bits_o,
  output logic orig_to_axi_sys_r_valid_o,
  output logic new_to_axi_sys_r_valid_o,
  output logic orig_to_axi_sys_r_ready_lo_o,
  output logic new_to_axi_sys_r_ready_lo_o,

  output logic [$bits(axi_pkg::sys_slave_ar_t)-1:0] orig_from_axi_sys_ar_bits_o,
  output logic [$bits(axi_pkg::sys_slave_ar_t)-1:0] new_from_axi_sys_ar_bits_o,
  output logic orig_from_axi_sys_ar_valid_o,
  output logic new_from_axi_sys_ar_valid_o,
  output logic orig_from_axi_sys_ar_ready_lo_o,
  output logic new_from_axi_sys_ar_ready_lo_o,
  output logic [$bits(axi_pkg::sys_slave_aw_t)-1:0] orig_from_axi_sys_uc_aw_bits_o,
  output logic [$bits(axi_pkg::sys_slave_aw_t)-1:0] new_from_axi_sys_uc_aw_bits_o,
  output logic orig_from_axi_sys_uc_aw_valid_o,
  output logic new_from_axi_sys_uc_aw_valid_o,
  output logic [$bits(axi_pkg::sys_slave_aw_t)-1:0] orig_from_axi_sys_sbm_aw_bits_o,
  output logic [$bits(axi_pkg::sys_slave_aw_t)-1:0] new_from_axi_sys_sbm_aw_bits_o,
  output logic orig_from_axi_sys_sbm_aw_valid_o,
  output logic new_from_axi_sys_sbm_aw_valid_o,
  output logic orig_from_axi_sys_aw_ready_lo_o,
  output logic new_from_axi_sys_aw_ready_lo_o,
  output logic [$bits(axi_pkg::sys_slave_w_t)-1:0] orig_from_axi_sys_uc_w_bits_o,
  output logic [$bits(axi_pkg::sys_slave_w_t)-1:0] new_from_axi_sys_uc_w_bits_o,
  output logic orig_from_axi_sys_uc_w_valid_o,
  output logic new_from_axi_sys_uc_w_valid_o,
  output logic [$bits(axi_pkg::sys_slave_w_t)-1:0] orig_from_axi_sys_sbm_w_bits_o,
  output logic [$bits(axi_pkg::sys_slave_w_t)-1:0] new_from_axi_sys_sbm_w_bits_o,
  output logic orig_from_axi_sys_sbm_w_valid_o,
  output logic new_from_axi_sys_sbm_w_valid_o,
  output logic orig_from_axi_sys_w_ready_lo_o,
  output logic new_from_axi_sys_w_ready_lo_o
);
  import axi_pkg::*;
  import dft_pkg::*;

  sc_master_axi_B_channel_t orig_to_axi_l3_b;
  sc_master_axi_R_channel_t orig_to_axi_l3_r;
  sc_master_axi_B_channel_t orig_to_axi_sys_b;
  sc_master_axi_R_channel_t orig_to_axi_sys_r;
  sys_slave_axi_AR_channel_t orig_from_axi_sys_ar;
  sys_slave_axi_AW_channel_t orig_from_axi_sys_uc_aw;
  sys_slave_axi_AW_channel_t orig_from_axi_sys_sbm_aw;
  sys_slave_axi_W_channel_t orig_from_axi_sys_uc_w;
  sys_slave_axi_W_channel_t orig_from_axi_sys_sbm_w;

  sc_master_b_t new_to_axi_l3_b;
  sc_master_r_t new_to_axi_l3_r;
  sc_master_b_t new_to_axi_sys_b;
  sc_master_r_t new_to_axi_sys_r;
  sys_slave_ar_t new_from_axi_sys_ar;
  sys_slave_aw_t new_from_axi_sys_uc_aw;
  sys_slave_aw_t new_from_axi_sys_sbm_aw;
  sys_slave_w_t new_from_axi_sys_uc_w;
  sys_slave_w_t new_from_axi_sys_sbm_w;

  sc_master_axi_B_channel_t orig_to_axi_l3_b_lo;
  sc_master_axi_R_channel_t orig_to_axi_l3_r_lo;
  sc_master_axi_B_channel_t orig_to_axi_sys_b_lo;
  sc_master_axi_R_channel_t orig_to_axi_sys_r_lo;
  sys_slave_axi_AR_channel_t orig_from_axi_sys_ar_lo;
  sys_slave_axi_AW_channel_t orig_from_axi_sys_aw_lo;
  sys_slave_axi_W_channel_t orig_from_axi_sys_w_lo;

  sc_master_b_t new_to_axi_l3_b_lo;
  sc_master_r_t new_to_axi_l3_r_lo;
  sc_master_b_t new_to_axi_sys_b_lo;
  sc_master_r_t new_to_axi_sys_r_lo;
  sys_slave_ar_t new_from_axi_sys_ar_lo;
  sys_slave_aw_t new_from_axi_sys_aw_lo;
  sys_slave_w_t new_from_axi_sys_w_lo;

  assign orig_to_axi_l3_b_lo = to_axi_l3_b_lo_bits_i;
  assign new_to_axi_l3_b_lo = to_axi_l3_b_lo_bits_i;
  assign orig_to_axi_l3_r_lo = to_axi_l3_r_lo_bits_i;
  assign new_to_axi_l3_r_lo = to_axi_l3_r_lo_bits_i;
  assign orig_to_axi_sys_b_lo = to_axi_sys_b_lo_bits_i;
  assign new_to_axi_sys_b_lo = to_axi_sys_b_lo_bits_i;
  assign orig_to_axi_sys_r_lo = to_axi_sys_r_lo_bits_i;
  assign new_to_axi_sys_r_lo = to_axi_sys_r_lo_bits_i;
  assign orig_from_axi_sys_ar_lo = from_axi_sys_ar_lo_bits_i;
  assign new_from_axi_sys_ar_lo = from_axi_sys_ar_lo_bits_i;
  assign orig_from_axi_sys_aw_lo = from_axi_sys_aw_lo_bits_i;
  assign new_from_axi_sys_aw_lo = from_axi_sys_aw_lo_bits_i;
  assign orig_from_axi_sys_w_lo = from_axi_sys_w_lo_bits_i;
  assign new_from_axi_sys_w_lo = from_axi_sys_w_lo_bits_i;

  dft_t dft_lv;
  dft_t dft_hv;
  assign dft_lv = '{scanmode: dft_lv_scanmode_i, scan_reset_n: dft_lv_scan_reset_ni,
                    sram_clk_override: 1'b0, ram_rei: 1'b0, ram_wei: 1'b0};
  assign dft_hv = '{scanmode: dft_hv_scanmode_i, scan_reset_n: dft_hv_scan_reset_ni,
                    sram_clk_override: 1'b0, ram_rei: 1'b0, ram_wei: 1'b0};

  uncached_lo_voltage_cross_orig orig (
    .reset_push(!rst_ni),
    .clock_push(clk_i),
    .reset_pop(!rst_ni),
    .clock_pop(clk_i),
    .to_axi_l3_B(orig_to_axi_l3_b),
    .to_axi_l3_BVALID(orig_to_axi_l3_b_valid_o),
    .to_axi_l3_BREADY(to_axi_l3_b_ready_i),
    .to_axi_l3_R(orig_to_axi_l3_r),
    .to_axi_l3_RVALID(orig_to_axi_l3_r_valid_o),
    .to_axi_l3_RREADY(to_axi_l3_r_ready_i),
    .to_axi_l3_B_lo(orig_to_axi_l3_b_lo),
    .to_axi_l3_BVALID_lo(to_axi_l3_b_valid_lo_i),
    .to_axi_l3_BREADY_lo(orig_to_axi_l3_b_ready_lo_o),
    .to_axi_l3_R_lo(orig_to_axi_l3_r_lo),
    .to_axi_l3_RVALID_lo(to_axi_l3_r_valid_lo_i),
    .to_axi_l3_RREADY_lo(orig_to_axi_l3_r_ready_lo_o),
    .to_axi_sys_B(orig_to_axi_sys_b),
    .to_axi_sys_BVALID(orig_to_axi_sys_b_valid_o),
    .to_axi_sys_BREADY(to_axi_sys_b_ready_i),
    .to_axi_sys_R(orig_to_axi_sys_r),
    .to_axi_sys_RVALID(orig_to_axi_sys_r_valid_o),
    .to_axi_sys_RREADY(to_axi_sys_r_ready_i),
    .to_axi_sys_B_lo(orig_to_axi_sys_b_lo),
    .to_axi_sys_BVALID_lo(to_axi_sys_b_valid_lo_i),
    .to_axi_sys_BREADY_lo(orig_to_axi_sys_b_ready_lo_o),
    .to_axi_sys_R_lo(orig_to_axi_sys_r_lo),
    .to_axi_sys_RVALID_lo(to_axi_sys_r_valid_lo_i),
    .to_axi_sys_RREADY_lo(orig_to_axi_sys_r_ready_lo_o),
    .from_axi_sys_AR(orig_from_axi_sys_ar),
    .from_axi_sys_ARVALID(orig_from_axi_sys_ar_valid_o),
    .from_axi_sys_ARREADY(from_axi_sys_ar_ready_i),
    .from_axi_sys_uc_AW(orig_from_axi_sys_uc_aw),
    .from_axi_sys_uc_AWVALID(orig_from_axi_sys_uc_aw_valid_o),
    .from_axi_sys_uc_AWREADY(from_axi_sys_uc_aw_ready_i),
    .from_axi_sys_sbm_AW(orig_from_axi_sys_sbm_aw),
    .from_axi_sys_sbm_AWVALID(orig_from_axi_sys_sbm_aw_valid_o),
    .from_axi_sys_sbm_AWREADY(from_axi_sys_sbm_aw_ready_i),
    .from_axi_sys_uc_W(orig_from_axi_sys_uc_w),
    .from_axi_sys_uc_WVALID(orig_from_axi_sys_uc_w_valid_o),
    .from_axi_sys_uc_WREADY(from_axi_sys_uc_w_ready_i),
    .from_axi_sys_sbm_W(orig_from_axi_sys_sbm_w),
    .from_axi_sys_sbm_WVALID(orig_from_axi_sys_sbm_w_valid_o),
    .from_axi_sys_sbm_WREADY(from_axi_sys_sbm_w_ready_i),
    .from_axi_sys_AR_lo(orig_from_axi_sys_ar_lo),
    .from_axi_sys_ARVALID_lo(from_axi_sys_ar_valid_lo_i),
    .from_axi_sys_ARREADY_lo(orig_from_axi_sys_ar_ready_lo_o),
    .from_axi_sys_AW_lo(orig_from_axi_sys_aw_lo),
    .from_axi_sys_AWVALID_lo(from_axi_sys_aw_valid_lo_i),
    .from_axi_sys_AWvcvalid_lo(from_axi_sys_aw_vcvalid_lo_i),
    .from_axi_sys_AWREADY_lo(orig_from_axi_sys_aw_ready_lo_o),
    .from_axi_sys_W_lo(orig_from_axi_sys_w_lo),
    .from_axi_sys_WVALID_lo(from_axi_sys_w_valid_lo_i),
    .from_axi_sys_Wvcvalid_lo(from_axi_sys_w_vcvalid_lo_i),
    .from_axi_sys_WREADY_lo(orig_from_axi_sys_w_ready_lo_o),
    .dft__reset_byp_push(dft_lv_scanmode_i),
    .dft__reset_push(!dft_lv_scan_reset_ni),
    .dft__reset_byp_pop(dft_hv_scanmode_i),
    .dft__reset_pop(!dft_hv_scan_reset_ni)
  );

  uncached_lo_voltage_cross new_dut (
    .clk_lv_i(clk_i),
    .rst_lv_ni(rst_ni),
    .clk_hv_i(clk_i),
    .rst_hv_ni(rst_ni),
    .to_axi_l3_b_o(new_to_axi_l3_b),
    .to_axi_l3_b_valid_o(new_to_axi_l3_b_valid_o),
    .to_axi_l3_b_ready_i(to_axi_l3_b_ready_i),
    .to_axi_l3_r_o(new_to_axi_l3_r),
    .to_axi_l3_r_valid_o(new_to_axi_l3_r_valid_o),
    .to_axi_l3_r_ready_i(to_axi_l3_r_ready_i),
    .to_axi_l3_b_lo_i(new_to_axi_l3_b_lo),
    .to_axi_l3_b_valid_lo_i(to_axi_l3_b_valid_lo_i),
    .to_axi_l3_b_ready_lo_o(new_to_axi_l3_b_ready_lo_o),
    .to_axi_l3_r_lo_i(new_to_axi_l3_r_lo),
    .to_axi_l3_r_valid_lo_i(to_axi_l3_r_valid_lo_i),
    .to_axi_l3_r_ready_lo_o(new_to_axi_l3_r_ready_lo_o),
    .to_axi_sys_b_o(new_to_axi_sys_b),
    .to_axi_sys_b_valid_o(new_to_axi_sys_b_valid_o),
    .to_axi_sys_b_ready_i(to_axi_sys_b_ready_i),
    .to_axi_sys_r_o(new_to_axi_sys_r),
    .to_axi_sys_r_valid_o(new_to_axi_sys_r_valid_o),
    .to_axi_sys_r_ready_i(to_axi_sys_r_ready_i),
    .to_axi_sys_b_lo_i(new_to_axi_sys_b_lo),
    .to_axi_sys_b_valid_lo_i(to_axi_sys_b_valid_lo_i),
    .to_axi_sys_b_ready_lo_o(new_to_axi_sys_b_ready_lo_o),
    .to_axi_sys_r_lo_i(new_to_axi_sys_r_lo),
    .to_axi_sys_r_valid_lo_i(to_axi_sys_r_valid_lo_i),
    .to_axi_sys_r_ready_lo_o(new_to_axi_sys_r_ready_lo_o),
    .from_axi_sys_ar_o(new_from_axi_sys_ar),
    .from_axi_sys_ar_valid_o(new_from_axi_sys_ar_valid_o),
    .from_axi_sys_ar_ready_i(from_axi_sys_ar_ready_i),
    .from_axi_sys_uc_aw_o(new_from_axi_sys_uc_aw),
    .from_axi_sys_uc_aw_valid_o(new_from_axi_sys_uc_aw_valid_o),
    .from_axi_sys_uc_aw_ready_i(from_axi_sys_uc_aw_ready_i),
    .from_axi_sys_sbm_aw_o(new_from_axi_sys_sbm_aw),
    .from_axi_sys_sbm_aw_valid_o(new_from_axi_sys_sbm_aw_valid_o),
    .from_axi_sys_sbm_aw_ready_i(from_axi_sys_sbm_aw_ready_i),
    .from_axi_sys_uc_w_o(new_from_axi_sys_uc_w),
    .from_axi_sys_uc_w_valid_o(new_from_axi_sys_uc_w_valid_o),
    .from_axi_sys_uc_w_ready_i(from_axi_sys_uc_w_ready_i),
    .from_axi_sys_sbm_w_o(new_from_axi_sys_sbm_w),
    .from_axi_sys_sbm_w_valid_o(new_from_axi_sys_sbm_w_valid_o),
    .from_axi_sys_sbm_w_ready_i(from_axi_sys_sbm_w_ready_i),
    .from_axi_sys_ar_lo_i(new_from_axi_sys_ar_lo),
    .from_axi_sys_ar_valid_lo_i(from_axi_sys_ar_valid_lo_i),
    .from_axi_sys_ar_ready_lo_o(new_from_axi_sys_ar_ready_lo_o),
    .from_axi_sys_aw_lo_i(new_from_axi_sys_aw_lo),
    .from_axi_sys_aw_valid_lo_i(from_axi_sys_aw_valid_lo_i),
    .from_axi_sys_aw_vcvalid_lo_i(from_axi_sys_aw_vcvalid_lo_i),
    .from_axi_sys_aw_ready_lo_o(new_from_axi_sys_aw_ready_lo_o),
    .from_axi_sys_w_lo_i(new_from_axi_sys_w_lo),
    .from_axi_sys_w_valid_lo_i(from_axi_sys_w_valid_lo_i),
    .from_axi_sys_w_vcvalid_lo_i(from_axi_sys_w_vcvalid_lo_i),
    .from_axi_sys_w_ready_lo_o(new_from_axi_sys_w_ready_lo_o),
    .dft_lv_i(dft_lv),
    .dft_hv_i(dft_hv)
  );

  assign orig_to_axi_l3_b_bits_o = orig_to_axi_l3_b;
  assign new_to_axi_l3_b_bits_o = new_to_axi_l3_b;
  assign orig_to_axi_l3_r_bits_o = orig_to_axi_l3_r;
  assign new_to_axi_l3_r_bits_o = new_to_axi_l3_r;
  assign orig_to_axi_sys_b_bits_o = orig_to_axi_sys_b;
  assign new_to_axi_sys_b_bits_o = new_to_axi_sys_b;
  assign orig_to_axi_sys_r_bits_o = orig_to_axi_sys_r;
  assign new_to_axi_sys_r_bits_o = new_to_axi_sys_r;
  assign orig_from_axi_sys_ar_bits_o = orig_from_axi_sys_ar;
  assign new_from_axi_sys_ar_bits_o = new_from_axi_sys_ar;
  assign orig_from_axi_sys_uc_aw_bits_o = orig_from_axi_sys_uc_aw;
  assign new_from_axi_sys_uc_aw_bits_o = new_from_axi_sys_uc_aw;
  assign orig_from_axi_sys_sbm_aw_bits_o = orig_from_axi_sys_sbm_aw;
  assign new_from_axi_sys_sbm_aw_bits_o = new_from_axi_sys_sbm_aw;
  assign orig_from_axi_sys_uc_w_bits_o = orig_from_axi_sys_uc_w;
  assign new_from_axi_sys_uc_w_bits_o = new_from_axi_sys_uc_w;
  assign orig_from_axi_sys_sbm_w_bits_o = orig_from_axi_sys_sbm_w;
  assign new_from_axi_sys_sbm_w_bits_o = new_from_axi_sys_sbm_w;
endmodule
