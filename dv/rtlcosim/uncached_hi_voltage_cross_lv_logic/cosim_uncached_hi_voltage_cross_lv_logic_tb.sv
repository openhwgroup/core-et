// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_uncached_hi_voltage_cross_lv_logic_tb
  import axi_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,

  input  logic to_axi_l3_ar_ready_lo_i,
  input  logic to_axi_l3_ar_vcfifo_valid_i,
  input  logic to_axi_l3_aw_ready_lo_i,
  input  logic to_axi_l3_w_ready_lo_i,
  input  logic to_axi_l3_aw_vcfifo_valid_i,
  input  logic to_axi_l3_w_vcfifo_valid_i,
  input  logic to_axi_sys_ar_ready_lo_i,
  input  logic to_axi_sys_ar_vcfifo_valid_i,
  input  logic to_axi_sys_aw_ready_lo_i,
  input  logic to_axi_sys_w_ready_lo_i,
  input  logic to_axi_sys_aw_vcfifo_valid_i,
  input  logic to_axi_sys_w_vcfifo_valid_i,
  input  logic from_axi_sys_b_ready_lo_i,
  input  logic from_axi_sys_b_vcfifo_valid_i,
  input  logic [$bits(sys_slave_b_t)-1:0] from_axi_sys_b_w_bits_i,
  input  logic from_axi_sys_r_ready_lo_i,
  input  logic from_axi_sys_r_vcfifo_valid_i,

  output logic [15:0] orig_ctrl_o,
  output logic [15:0] new_ctrl_o,
  output logic [$bits(sys_slave_b_t)-1:0] orig_b_bits_o,
  output logic [$bits(sys_slave_b_t)-1:0] new_b_bits_o
);

  sys_slave_axi_B_channel_t orig_b_w;
  sys_slave_axi_B_channel_t orig_b_lo;
  sys_slave_b_t new_b_w;
  sys_slave_b_t new_b_lo;

  assign orig_b_w = from_axi_sys_b_w_bits_i[$bits(sys_slave_axi_B_channel_t)-1:0];
  assign new_b_w = from_axi_sys_b_w_bits_i;
  assign orig_b_bits_o = orig_b_lo;
  assign new_b_bits_o = new_b_lo;

  logic orig_to_axi_l3_ar_valid_lo;
  logic orig_to_axi_l3_ar_vcfifo_pop;
  logic orig_to_axi_l3_aw_valid_lo;
  logic orig_to_axi_l3_aw_vcfifo_pop;
  logic orig_to_axi_l3_w_valid_lo;
  logic orig_to_axi_l3_w_vcfifo_pop;
  logic orig_to_axi_sys_ar_valid_lo;
  logic orig_to_axi_sys_ar_vcfifo_pop;
  logic orig_to_axi_sys_aw_valid_lo;
  logic orig_to_axi_sys_aw_vcfifo_pop;
  logic orig_to_axi_sys_w_valid_lo;
  logic orig_to_axi_sys_w_vcfifo_pop;
  logic orig_from_axi_sys_b_valid_lo;
  logic orig_from_axi_sys_b_vcfifo_pop;
  logic orig_from_axi_sys_r_valid_lo;
  logic orig_from_axi_sys_r_vcfifo_pop;

  logic new_to_axi_l3_ar_valid_lo;
  logic new_to_axi_l3_ar_vcfifo_pop;
  logic new_to_axi_l3_aw_valid_lo;
  logic new_to_axi_l3_aw_vcfifo_pop;
  logic new_to_axi_l3_w_valid_lo;
  logic new_to_axi_l3_w_vcfifo_pop;
  logic new_to_axi_sys_ar_valid_lo;
  logic new_to_axi_sys_ar_vcfifo_pop;
  logic new_to_axi_sys_aw_valid_lo;
  logic new_to_axi_sys_aw_vcfifo_pop;
  logic new_to_axi_sys_w_valid_lo;
  logic new_to_axi_sys_w_vcfifo_pop;
  logic new_from_axi_sys_b_valid_lo;
  logic new_from_axi_sys_b_vcfifo_pop;
  logic new_from_axi_sys_r_valid_lo;
  logic new_from_axi_sys_r_vcfifo_pop;

  assign orig_ctrl_o = {
    orig_to_axi_l3_ar_valid_lo,
    orig_to_axi_l3_ar_vcfifo_pop,
    orig_to_axi_l3_aw_valid_lo,
    orig_to_axi_l3_aw_vcfifo_pop,
    orig_to_axi_l3_w_valid_lo,
    orig_to_axi_l3_w_vcfifo_pop,
    orig_to_axi_sys_ar_valid_lo,
    orig_to_axi_sys_ar_vcfifo_pop,
    orig_to_axi_sys_aw_valid_lo,
    orig_to_axi_sys_aw_vcfifo_pop,
    orig_to_axi_sys_w_valid_lo,
    orig_to_axi_sys_w_vcfifo_pop,
    orig_from_axi_sys_b_valid_lo,
    orig_from_axi_sys_b_vcfifo_pop,
    orig_from_axi_sys_r_valid_lo,
    orig_from_axi_sys_r_vcfifo_pop
  };

  assign new_ctrl_o = {
    new_to_axi_l3_ar_valid_lo,
    new_to_axi_l3_ar_vcfifo_pop,
    new_to_axi_l3_aw_valid_lo,
    new_to_axi_l3_aw_vcfifo_pop,
    new_to_axi_l3_w_valid_lo,
    new_to_axi_l3_w_vcfifo_pop,
    new_to_axi_sys_ar_valid_lo,
    new_to_axi_sys_ar_vcfifo_pop,
    new_to_axi_sys_aw_valid_lo,
    new_to_axi_sys_aw_vcfifo_pop,
    new_to_axi_sys_w_valid_lo,
    new_to_axi_sys_w_vcfifo_pop,
    new_from_axi_sys_b_valid_lo,
    new_from_axi_sys_b_vcfifo_pop,
    new_from_axi_sys_r_valid_lo,
    new_from_axi_sys_r_vcfifo_pop
  };

  uncached_hi_voltage_cross_lv_logic_orig u_orig (
    .to_axi_l3_ARREADY_lo(to_axi_l3_ar_ready_lo_i),
    .to_axi_l3_AR_vcfifo_valid(to_axi_l3_ar_vcfifo_valid_i),
    .to_axi_l3_ARVALID_lo(orig_to_axi_l3_ar_valid_lo),
    .to_axi_l3_AR_vcfifo_pop(orig_to_axi_l3_ar_vcfifo_pop),
    .to_axi_l3_AWREADY_lo(to_axi_l3_aw_ready_lo_i),
    .to_axi_l3_WREADY_lo(to_axi_l3_w_ready_lo_i),
    .to_axi_l3_AW_vcfifo_valid(to_axi_l3_aw_vcfifo_valid_i),
    .to_axi_l3_W_vcfifo_valid(to_axi_l3_w_vcfifo_valid_i),
    .to_axi_l3_AWVALID_lo(orig_to_axi_l3_aw_valid_lo),
    .to_axi_l3_AW_vcfifo_pop(orig_to_axi_l3_aw_vcfifo_pop),
    .to_axi_l3_WVALID_lo(orig_to_axi_l3_w_valid_lo),
    .to_axi_l3_W_vcfifo_pop(orig_to_axi_l3_w_vcfifo_pop),
    .to_axi_sys_ARREADY_lo(to_axi_sys_ar_ready_lo_i),
    .to_axi_sys_AR_vcfifo_valid(to_axi_sys_ar_vcfifo_valid_i),
    .to_axi_sys_ARVALID_lo(orig_to_axi_sys_ar_valid_lo),
    .to_axi_sys_AR_vcfifo_pop(orig_to_axi_sys_ar_vcfifo_pop),
    .to_axi_sys_AWREADY_lo(to_axi_sys_aw_ready_lo_i),
    .to_axi_sys_WREADY_lo(to_axi_sys_w_ready_lo_i),
    .to_axi_sys_AW_vcfifo_valid(to_axi_sys_aw_vcfifo_valid_i),
    .to_axi_sys_W_vcfifo_valid(to_axi_sys_w_vcfifo_valid_i),
    .to_axi_sys_AWVALID_lo(orig_to_axi_sys_aw_valid_lo),
    .to_axi_sys_AW_vcfifo_pop(orig_to_axi_sys_aw_vcfifo_pop),
    .to_axi_sys_WVALID_lo(orig_to_axi_sys_w_valid_lo),
    .to_axi_sys_W_vcfifo_pop(orig_to_axi_sys_w_vcfifo_pop),
    .from_axi_sys_BREADY_lo(from_axi_sys_b_ready_lo_i),
    .from_axi_sys_B_vcfifo_valid(from_axi_sys_b_vcfifo_valid_i),
    .from_axi_sys_BVALID_lo(orig_from_axi_sys_b_valid_lo),
    .from_axi_sys_B_vcfifo_pop(orig_from_axi_sys_b_vcfifo_pop),
    .from_axi_sys_B_w(orig_b_w),
    .from_axi_sys_B_lo(orig_b_lo),
    .from_axi_sys_RREADY_lo(from_axi_sys_r_ready_lo_i),
    .from_axi_sys_R_vcfifo_valid(from_axi_sys_r_vcfifo_valid_i),
    .from_axi_sys_RVALID_lo(orig_from_axi_sys_r_valid_lo),
    .from_axi_sys_R_vcfifo_pop(orig_from_axi_sys_r_vcfifo_pop)
  );

  uncached_hi_voltage_cross_lv_logic u_new (
    .to_axi_l3_ar_ready_lo_i(to_axi_l3_ar_ready_lo_i),
    .to_axi_l3_ar_vcfifo_valid_i(to_axi_l3_ar_vcfifo_valid_i),
    .to_axi_l3_ar_valid_lo_o(new_to_axi_l3_ar_valid_lo),
    .to_axi_l3_ar_vcfifo_pop_o(new_to_axi_l3_ar_vcfifo_pop),
    .to_axi_l3_aw_ready_lo_i(to_axi_l3_aw_ready_lo_i),
    .to_axi_l3_w_ready_lo_i(to_axi_l3_w_ready_lo_i),
    .to_axi_l3_aw_vcfifo_valid_i(to_axi_l3_aw_vcfifo_valid_i),
    .to_axi_l3_w_vcfifo_valid_i(to_axi_l3_w_vcfifo_valid_i),
    .to_axi_l3_aw_valid_lo_o(new_to_axi_l3_aw_valid_lo),
    .to_axi_l3_aw_vcfifo_pop_o(new_to_axi_l3_aw_vcfifo_pop),
    .to_axi_l3_w_valid_lo_o(new_to_axi_l3_w_valid_lo),
    .to_axi_l3_w_vcfifo_pop_o(new_to_axi_l3_w_vcfifo_pop),
    .to_axi_sys_ar_ready_lo_i(to_axi_sys_ar_ready_lo_i),
    .to_axi_sys_ar_vcfifo_valid_i(to_axi_sys_ar_vcfifo_valid_i),
    .to_axi_sys_ar_valid_lo_o(new_to_axi_sys_ar_valid_lo),
    .to_axi_sys_ar_vcfifo_pop_o(new_to_axi_sys_ar_vcfifo_pop),
    .to_axi_sys_aw_ready_lo_i(to_axi_sys_aw_ready_lo_i),
    .to_axi_sys_w_ready_lo_i(to_axi_sys_w_ready_lo_i),
    .to_axi_sys_aw_vcfifo_valid_i(to_axi_sys_aw_vcfifo_valid_i),
    .to_axi_sys_w_vcfifo_valid_i(to_axi_sys_w_vcfifo_valid_i),
    .to_axi_sys_aw_valid_lo_o(new_to_axi_sys_aw_valid_lo),
    .to_axi_sys_aw_vcfifo_pop_o(new_to_axi_sys_aw_vcfifo_pop),
    .to_axi_sys_w_valid_lo_o(new_to_axi_sys_w_valid_lo),
    .to_axi_sys_w_vcfifo_pop_o(new_to_axi_sys_w_vcfifo_pop),
    .from_axi_sys_b_ready_lo_i(from_axi_sys_b_ready_lo_i),
    .from_axi_sys_b_vcfifo_valid_i(from_axi_sys_b_vcfifo_valid_i),
    .from_axi_sys_b_valid_lo_o(new_from_axi_sys_b_valid_lo),
    .from_axi_sys_b_vcfifo_pop_o(new_from_axi_sys_b_vcfifo_pop),
    .from_axi_sys_b_w_i(new_b_w),
    .from_axi_sys_b_lo_o(new_b_lo),
    .from_axi_sys_r_ready_lo_i(from_axi_sys_r_ready_lo_i),
    .from_axi_sys_r_vcfifo_valid_i(from_axi_sys_r_vcfifo_valid_i),
    .from_axi_sys_r_valid_lo_o(new_from_axi_sys_r_valid_lo),
    .from_axi_sys_r_vcfifo_pop_o(new_from_axi_sys_r_vcfifo_pop)
  );

  /* verilator lint_off UNUSEDSIGNAL */
  logic unused_bits;
  assign unused_bits = clk_i ^ rst_ni;
  /* verilator lint_on UNUSEDSIGNAL */

endmodule : cosim_uncached_hi_voltage_cross_lv_logic_tb
