// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module uncached_hi_voltage_cross_lv_logic
  import axi_pkg::*;
(
  input  logic        to_axi_l3_ar_ready_lo_i,
  input  logic        to_axi_l3_ar_vcfifo_valid_i,
  output logic        to_axi_l3_ar_valid_lo_o,
  output logic        to_axi_l3_ar_vcfifo_pop_o,

  input  logic        to_axi_l3_aw_ready_lo_i,
  input  logic        to_axi_l3_w_ready_lo_i,
  input  logic        to_axi_l3_aw_vcfifo_valid_i,
  input  logic        to_axi_l3_w_vcfifo_valid_i,
  output logic        to_axi_l3_aw_valid_lo_o,
  output logic        to_axi_l3_aw_vcfifo_pop_o,
  output logic        to_axi_l3_w_valid_lo_o,
  output logic        to_axi_l3_w_vcfifo_pop_o,

  input  logic        to_axi_sys_ar_ready_lo_i,
  input  logic        to_axi_sys_ar_vcfifo_valid_i,
  output logic        to_axi_sys_ar_valid_lo_o,
  output logic        to_axi_sys_ar_vcfifo_pop_o,

  input  logic        to_axi_sys_aw_ready_lo_i,
  input  logic        to_axi_sys_w_ready_lo_i,
  input  logic        to_axi_sys_aw_vcfifo_valid_i,
  input  logic        to_axi_sys_w_vcfifo_valid_i,
  output logic        to_axi_sys_aw_valid_lo_o,
  output logic        to_axi_sys_aw_vcfifo_pop_o,
  output logic        to_axi_sys_w_valid_lo_o,
  output logic        to_axi_sys_w_vcfifo_pop_o,

  input  logic        from_axi_sys_b_ready_lo_i,
  input  logic        from_axi_sys_b_vcfifo_valid_i,
  output logic        from_axi_sys_b_valid_lo_o,
  output logic        from_axi_sys_b_vcfifo_pop_o,
  input  sys_slave_b_t from_axi_sys_b_w_i,
  output sys_slave_b_t from_axi_sys_b_lo_o,

  input  logic        from_axi_sys_r_ready_lo_i,
  input  logic        from_axi_sys_r_vcfifo_valid_i,
  output logic        from_axi_sys_r_valid_lo_o,
  output logic        from_axi_sys_r_vcfifo_pop_o
);

  logic to_axi_l3_aw_w_ready_lo;
  logic to_axi_l3_aw_w_valid_lo;
  logic to_axi_sys_aw_w_ready_lo;
  logic to_axi_sys_aw_w_valid_lo;

  assign to_axi_l3_ar_valid_lo_o    = to_axi_l3_ar_vcfifo_valid_i;
  assign to_axi_l3_ar_vcfifo_pop_o = to_axi_l3_ar_ready_lo_i & to_axi_l3_ar_vcfifo_valid_i;

  assign to_axi_l3_aw_w_ready_lo = to_axi_l3_aw_ready_lo_i & to_axi_l3_w_ready_lo_i;
  assign to_axi_l3_aw_w_valid_lo = to_axi_l3_aw_vcfifo_valid_i & to_axi_l3_w_vcfifo_valid_i;
  assign to_axi_l3_aw_valid_lo_o = to_axi_l3_aw_w_valid_lo;
  assign to_axi_l3_aw_vcfifo_pop_o = to_axi_l3_aw_w_ready_lo & to_axi_l3_aw_w_valid_lo;
  assign to_axi_l3_w_valid_lo_o = to_axi_l3_aw_w_valid_lo;
  assign to_axi_l3_w_vcfifo_pop_o = to_axi_l3_aw_w_ready_lo & to_axi_l3_aw_w_valid_lo;

  assign to_axi_sys_ar_valid_lo_o = to_axi_sys_ar_vcfifo_valid_i;
  assign to_axi_sys_ar_vcfifo_pop_o = to_axi_sys_ar_ready_lo_i;

  assign to_axi_sys_aw_w_ready_lo = to_axi_sys_aw_ready_lo_i & to_axi_sys_w_ready_lo_i;
  assign to_axi_sys_aw_w_valid_lo = to_axi_sys_aw_vcfifo_valid_i & to_axi_sys_w_vcfifo_valid_i;
  assign to_axi_sys_aw_valid_lo_o = to_axi_sys_aw_w_valid_lo;
  assign to_axi_sys_aw_vcfifo_pop_o = to_axi_sys_aw_w_ready_lo & to_axi_sys_aw_w_valid_lo;
  assign to_axi_sys_w_valid_lo_o = to_axi_sys_aw_w_valid_lo;
  assign to_axi_sys_w_vcfifo_pop_o = to_axi_sys_aw_w_ready_lo & to_axi_sys_aw_w_valid_lo;

  assign from_axi_sys_b_valid_lo_o = from_axi_sys_b_vcfifo_valid_i;
  assign from_axi_sys_b_vcfifo_pop_o = from_axi_sys_b_ready_lo_i;
  assign from_axi_sys_b_lo_o = from_axi_sys_b_w_i;

  assign from_axi_sys_r_valid_lo_o = from_axi_sys_r_vcfifo_valid_i;
  assign from_axi_sys_r_vcfifo_pop_o = from_axi_sys_r_ready_lo_i;

endmodule : uncached_hi_voltage_cross_lv_logic
