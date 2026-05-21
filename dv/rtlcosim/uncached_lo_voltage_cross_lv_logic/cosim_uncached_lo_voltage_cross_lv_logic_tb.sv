// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_uncached_lo_voltage_cross_lv_logic_tb (
  input  logic       clk_i,
  input  logic       rst_ni,
  input  logic       from_axi_sys_sbm_aw_vcfifo_valid_i,
  input  logic       from_axi_sys_sbm_w_vcfifo_valid_i,
  input  logic       from_axi_sys_aw_valid_lo_i,
  input  logic       from_axi_sys_w_valid_lo_i,
  input  logic [1:0] from_axi_sys_aw_vcvalid_lo_i,
  input  logic [1:0] from_axi_sys_w_vcvalid_lo_i,
  input  logic       from_axi_sys_sbm_aw_ready_i,
  input  logic       from_axi_sys_sbm_w_ready_i,
  input  logic       from_axi_sys_uc_aw_vcfifo_valid_i,
  input  logic       from_axi_sys_uc_w_vcfifo_valid_i,
  input  logic       from_axi_sys_uc_aw_ready_i,
  input  logic       from_axi_sys_uc_w_ready_i,
  input  logic       from_axi_sys_sbm_aw_vcfifo_ready_i,
  input  logic       from_axi_sys_uc_aw_vcfifo_ready_i,
  input  logic       from_axi_sys_sbm_w_vcfifo_ready_i,
  input  logic       from_axi_sys_uc_w_vcfifo_ready_i,
  output logic [13:0] orig_ctrl_o,
  output logic [13:0] new_ctrl_o
);

  logic orig_from_axi_sys_sbm_aw_valid;
  logic orig_from_axi_sys_sbm_aw_vcfifo_push;
  logic orig_from_axi_sys_sbm_aw_vcfifo_pop;
  logic orig_from_axi_sys_uc_aw_valid;
  logic orig_from_axi_sys_uc_aw_vcfifo_push;
  logic orig_from_axi_sys_uc_aw_vcfifo_pop;
  logic orig_from_axi_sys_sbm_w_valid;
  logic orig_from_axi_sys_sbm_w_vcfifo_push;
  logic orig_from_axi_sys_sbm_w_vcfifo_pop;
  logic orig_from_axi_sys_uc_w_valid;
  logic orig_from_axi_sys_uc_w_vcfifo_push;
  logic orig_from_axi_sys_uc_w_vcfifo_pop;
  logic orig_from_axi_sys_aw_ready_lo;
  logic orig_from_axi_sys_w_ready_lo;

  logic new_from_axi_sys_sbm_aw_valid;
  logic new_from_axi_sys_sbm_aw_vcfifo_push;
  logic new_from_axi_sys_sbm_aw_vcfifo_pop;
  logic new_from_axi_sys_uc_aw_valid;
  logic new_from_axi_sys_uc_aw_vcfifo_push;
  logic new_from_axi_sys_uc_aw_vcfifo_pop;
  logic new_from_axi_sys_sbm_w_valid;
  logic new_from_axi_sys_sbm_w_vcfifo_push;
  logic new_from_axi_sys_sbm_w_vcfifo_pop;
  logic new_from_axi_sys_uc_w_valid;
  logic new_from_axi_sys_uc_w_vcfifo_push;
  logic new_from_axi_sys_uc_w_vcfifo_pop;
  logic new_from_axi_sys_aw_ready_lo;
  logic new_from_axi_sys_w_ready_lo;

  assign orig_ctrl_o = {
    orig_from_axi_sys_sbm_aw_valid,
    orig_from_axi_sys_sbm_aw_vcfifo_push,
    orig_from_axi_sys_sbm_aw_vcfifo_pop,
    orig_from_axi_sys_uc_aw_valid,
    orig_from_axi_sys_uc_aw_vcfifo_push,
    orig_from_axi_sys_uc_aw_vcfifo_pop,
    orig_from_axi_sys_sbm_w_valid,
    orig_from_axi_sys_sbm_w_vcfifo_push,
    orig_from_axi_sys_sbm_w_vcfifo_pop,
    orig_from_axi_sys_uc_w_valid,
    orig_from_axi_sys_uc_w_vcfifo_push,
    orig_from_axi_sys_uc_w_vcfifo_pop,
    orig_from_axi_sys_aw_ready_lo,
    orig_from_axi_sys_w_ready_lo
  };

  assign new_ctrl_o = {
    new_from_axi_sys_sbm_aw_valid,
    new_from_axi_sys_sbm_aw_vcfifo_push,
    new_from_axi_sys_sbm_aw_vcfifo_pop,
    new_from_axi_sys_uc_aw_valid,
    new_from_axi_sys_uc_aw_vcfifo_push,
    new_from_axi_sys_uc_aw_vcfifo_pop,
    new_from_axi_sys_sbm_w_valid,
    new_from_axi_sys_sbm_w_vcfifo_push,
    new_from_axi_sys_sbm_w_vcfifo_pop,
    new_from_axi_sys_uc_w_valid,
    new_from_axi_sys_uc_w_vcfifo_push,
    new_from_axi_sys_uc_w_vcfifo_pop,
    new_from_axi_sys_aw_ready_lo,
    new_from_axi_sys_w_ready_lo
  };

  uncached_lo_voltage_cross_lv_logic_orig u_orig (
    .from_axi_sys_sbm_AW_vcfifo_valid(from_axi_sys_sbm_aw_vcfifo_valid_i),
    .from_axi_sys_sbm_W_vcfifo_valid(from_axi_sys_sbm_w_vcfifo_valid_i),
    .from_axi_sys_AWVALID_lo(from_axi_sys_aw_valid_lo_i),
    .from_axi_sys_WVALID_lo(from_axi_sys_w_valid_lo_i),
    .from_axi_sys_AWvcvalid_lo(from_axi_sys_aw_vcvalid_lo_i),
    .from_axi_sys_Wvcvalid_lo(from_axi_sys_w_vcvalid_lo_i),
    .from_axi_sys_sbm_AWREADY(from_axi_sys_sbm_aw_ready_i),
    .from_axi_sys_sbm_WREADY(from_axi_sys_sbm_w_ready_i),
    .from_axi_sys_uc_AW_vcfifo_valid(from_axi_sys_uc_aw_vcfifo_valid_i),
    .from_axi_sys_uc_W_vcfifo_valid(from_axi_sys_uc_w_vcfifo_valid_i),
    .from_axi_sys_uc_AWREADY(from_axi_sys_uc_aw_ready_i),
    .from_axi_sys_uc_WREADY(from_axi_sys_uc_w_ready_i),
    .from_axi_sys_sbm_AW_vcfifo_ready(from_axi_sys_sbm_aw_vcfifo_ready_i),
    .from_axi_sys_uc_AW_vcfifo_ready(from_axi_sys_uc_aw_vcfifo_ready_i),
    .from_axi_sys_sbm_W_vcfifo_ready(from_axi_sys_sbm_w_vcfifo_ready_i),
    .from_axi_sys_uc_W_vcfifo_ready(from_axi_sys_uc_w_vcfifo_ready_i),
    .from_axi_sys_sbm_AWVALID(orig_from_axi_sys_sbm_aw_valid),
    .from_axi_sys_sbm_AW_vcfifo_push(orig_from_axi_sys_sbm_aw_vcfifo_push),
    .from_axi_sys_sbm_AW_vcfifo_pop(orig_from_axi_sys_sbm_aw_vcfifo_pop),
    .from_axi_sys_uc_AWVALID(orig_from_axi_sys_uc_aw_valid),
    .from_axi_sys_uc_AW_vcfifo_push(orig_from_axi_sys_uc_aw_vcfifo_push),
    .from_axi_sys_uc_AW_vcfifo_pop(orig_from_axi_sys_uc_aw_vcfifo_pop),
    .from_axi_sys_sbm_WVALID(orig_from_axi_sys_sbm_w_valid),
    .from_axi_sys_sbm_W_vcfifo_push(orig_from_axi_sys_sbm_w_vcfifo_push),
    .from_axi_sys_sbm_W_vcfifo_pop(orig_from_axi_sys_sbm_w_vcfifo_pop),
    .from_axi_sys_uc_WVALID(orig_from_axi_sys_uc_w_valid),
    .from_axi_sys_uc_W_vcfifo_push(orig_from_axi_sys_uc_w_vcfifo_push),
    .from_axi_sys_uc_W_vcfifo_pop(orig_from_axi_sys_uc_w_vcfifo_pop),
    .from_axi_sys_AWREADY_lo(orig_from_axi_sys_aw_ready_lo),
    .from_axi_sys_WREADY_lo(orig_from_axi_sys_w_ready_lo)
  );

  uncached_lo_voltage_cross_lv_logic u_new (
    .from_axi_sys_sbm_aw_vcfifo_valid_i(from_axi_sys_sbm_aw_vcfifo_valid_i),
    .from_axi_sys_sbm_w_vcfifo_valid_i(from_axi_sys_sbm_w_vcfifo_valid_i),
    .from_axi_sys_aw_valid_lo_i(from_axi_sys_aw_valid_lo_i),
    .from_axi_sys_w_valid_lo_i(from_axi_sys_w_valid_lo_i),
    .from_axi_sys_aw_vcvalid_lo_i(from_axi_sys_aw_vcvalid_lo_i),
    .from_axi_sys_w_vcvalid_lo_i(from_axi_sys_w_vcvalid_lo_i),
    .from_axi_sys_sbm_aw_ready_i(from_axi_sys_sbm_aw_ready_i),
    .from_axi_sys_sbm_w_ready_i(from_axi_sys_sbm_w_ready_i),
    .from_axi_sys_uc_aw_vcfifo_valid_i(from_axi_sys_uc_aw_vcfifo_valid_i),
    .from_axi_sys_uc_w_vcfifo_valid_i(from_axi_sys_uc_w_vcfifo_valid_i),
    .from_axi_sys_uc_aw_ready_i(from_axi_sys_uc_aw_ready_i),
    .from_axi_sys_uc_w_ready_i(from_axi_sys_uc_w_ready_i),
    .from_axi_sys_sbm_aw_vcfifo_ready_i(from_axi_sys_sbm_aw_vcfifo_ready_i),
    .from_axi_sys_uc_aw_vcfifo_ready_i(from_axi_sys_uc_aw_vcfifo_ready_i),
    .from_axi_sys_sbm_w_vcfifo_ready_i(from_axi_sys_sbm_w_vcfifo_ready_i),
    .from_axi_sys_uc_w_vcfifo_ready_i(from_axi_sys_uc_w_vcfifo_ready_i),
    .from_axi_sys_sbm_aw_valid_o(new_from_axi_sys_sbm_aw_valid),
    .from_axi_sys_sbm_aw_vcfifo_push_o(new_from_axi_sys_sbm_aw_vcfifo_push),
    .from_axi_sys_sbm_aw_vcfifo_pop_o(new_from_axi_sys_sbm_aw_vcfifo_pop),
    .from_axi_sys_uc_aw_valid_o(new_from_axi_sys_uc_aw_valid),
    .from_axi_sys_uc_aw_vcfifo_push_o(new_from_axi_sys_uc_aw_vcfifo_push),
    .from_axi_sys_uc_aw_vcfifo_pop_o(new_from_axi_sys_uc_aw_vcfifo_pop),
    .from_axi_sys_sbm_w_valid_o(new_from_axi_sys_sbm_w_valid),
    .from_axi_sys_sbm_w_vcfifo_push_o(new_from_axi_sys_sbm_w_vcfifo_push),
    .from_axi_sys_sbm_w_vcfifo_pop_o(new_from_axi_sys_sbm_w_vcfifo_pop),
    .from_axi_sys_uc_w_valid_o(new_from_axi_sys_uc_w_valid),
    .from_axi_sys_uc_w_vcfifo_push_o(new_from_axi_sys_uc_w_vcfifo_push),
    .from_axi_sys_uc_w_vcfifo_pop_o(new_from_axi_sys_uc_w_vcfifo_pop),
    .from_axi_sys_aw_ready_lo_o(new_from_axi_sys_aw_ready_lo),
    .from_axi_sys_w_ready_lo_o(new_from_axi_sys_w_ready_lo)
  );

  /* verilator lint_off UNUSEDSIGNAL */
  logic unused_clock_reset;
  assign unused_clock_reset = clk_i ^ rst_ni;
  /* verilator lint_on UNUSEDSIGNAL */

endmodule : cosim_uncached_lo_voltage_cross_lv_logic_tb
