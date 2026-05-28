// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module uncached_lo_voltage_cross
  import axi_pkg::*;
  import dft_pkg::*;
  import shire_uncached_pkg::*;
(
  input  logic           clk_lv_i,
  input  logic           rst_lv_ni,
  input  logic           clk_hv_i,
  input  logic           rst_hv_ni,

  output sc_master_b_t   to_axi_l3_b_o,
  output logic           to_axi_l3_b_valid_o,
  input  logic           to_axi_l3_b_ready_i,
  output sc_master_r_t   to_axi_l3_r_o,
  output logic           to_axi_l3_r_valid_o,
  input  logic           to_axi_l3_r_ready_i,
  input  sc_master_b_t   to_axi_l3_b_lo_i,
  input  logic           to_axi_l3_b_valid_lo_i,
  output logic           to_axi_l3_b_ready_lo_o,
  input  sc_master_r_t   to_axi_l3_r_lo_i,
  input  logic           to_axi_l3_r_valid_lo_i,
  output logic           to_axi_l3_r_ready_lo_o,

  output sc_master_b_t   to_axi_sys_b_o,
  output logic           to_axi_sys_b_valid_o,
  input  logic           to_axi_sys_b_ready_i,
  output sc_master_r_t   to_axi_sys_r_o,
  output logic           to_axi_sys_r_valid_o,
  input  logic           to_axi_sys_r_ready_i,
  input  sc_master_b_t   to_axi_sys_b_lo_i,
  input  logic           to_axi_sys_b_valid_lo_i,
  output logic           to_axi_sys_b_ready_lo_o,
  input  sc_master_r_t   to_axi_sys_r_lo_i,
  input  logic           to_axi_sys_r_valid_lo_i,
  output logic           to_axi_sys_r_ready_lo_o,

  output sys_slave_ar_t  from_axi_sys_ar_o,
  output logic           from_axi_sys_ar_valid_o,
  input  logic           from_axi_sys_ar_ready_i,
  output sys_slave_aw_t  from_axi_sys_uc_aw_o,
  output logic           from_axi_sys_uc_aw_valid_o,
  input  logic           from_axi_sys_uc_aw_ready_i,
  output sys_slave_aw_t  from_axi_sys_sbm_aw_o,
  output logic           from_axi_sys_sbm_aw_valid_o,
  input  logic           from_axi_sys_sbm_aw_ready_i,
  output sys_slave_w_t   from_axi_sys_uc_w_o,
  output logic           from_axi_sys_uc_w_valid_o,
  input  logic           from_axi_sys_uc_w_ready_i,
  output sys_slave_w_t   from_axi_sys_sbm_w_o,
  output logic           from_axi_sys_sbm_w_valid_o,
  input  logic           from_axi_sys_sbm_w_ready_i,

  input  sys_slave_ar_t  from_axi_sys_ar_lo_i,
  input  logic           from_axi_sys_ar_valid_lo_i,
  output logic           from_axi_sys_ar_ready_lo_o,
  input  sys_slave_aw_t  from_axi_sys_aw_lo_i,
  input  logic           from_axi_sys_aw_valid_lo_i,
  input  logic [1:0]     from_axi_sys_aw_vcvalid_lo_i,
  output logic           from_axi_sys_aw_ready_lo_o,
  input  sys_slave_w_t   from_axi_sys_w_lo_i,
  input  logic           from_axi_sys_w_valid_lo_i,
  input  logic [1:0]     from_axi_sys_w_vcvalid_lo_i,
  output logic           from_axi_sys_w_ready_lo_o,

  input  dft_t           dft_lv_i,
  input  dft_t           dft_hv_i
);

  localparam int unsigned SyncStages = 2;
  localparam int unsigned AxiFifoDepth = SyncStages + SyncStages + 1;
  localparam int unsigned SlvAxiFifoDepth = UcNumCredits;

  logic to_axi_l3_b_pop;
  logic to_axi_l3_r_pop;
  logic to_axi_sys_b_pop;
  logic to_axi_sys_r_pop;
  logic from_axi_sys_ar_pop;
  logic from_axi_sys_sbm_aw_vcfifo_valid;
  logic from_axi_sys_sbm_aw_vcfifo_push;
  logic from_axi_sys_sbm_aw_vcfifo_pop;
  logic from_axi_sys_sbm_aw_vcfifo_ready;
  logic from_axi_sys_uc_aw_vcfifo_valid;
  logic from_axi_sys_uc_aw_vcfifo_push;
  logic from_axi_sys_uc_aw_vcfifo_pop;
  logic from_axi_sys_uc_aw_vcfifo_ready;
  logic from_axi_sys_sbm_w_vcfifo_valid;
  logic from_axi_sys_sbm_w_vcfifo_push;
  logic from_axi_sys_sbm_w_vcfifo_pop;
  logic from_axi_sys_sbm_w_vcfifo_ready;
  logic from_axi_sys_uc_w_vcfifo_valid;
  logic from_axi_sys_uc_w_vcfifo_push;
  logic from_axi_sys_uc_w_vcfifo_pop;
  logic from_axi_sys_uc_w_vcfifo_ready;

  assign to_axi_l3_b_pop = to_axi_l3_b_ready_i;
  assign to_axi_l3_r_pop = to_axi_l3_r_ready_i;
  assign to_axi_sys_b_pop = to_axi_sys_b_ready_i;
  assign to_axi_sys_r_pop = to_axi_sys_r_ready_i;
  assign from_axi_sys_ar_pop = from_axi_sys_ar_ready_i;

  prim_fifo_async_lov #(.Width($bits(sc_master_b_t)), .Depth(AxiFifoDepth), .SyncStages(SyncStages))
  u_l3_b_fifo (
    .clk_wr_i(clk_lv_i), .rst_wr_ni(rst_lv_ni), .push_i(to_axi_l3_b_valid_lo_i),
    .wdata_i(to_axi_l3_b_lo_i), .ready_o(to_axi_l3_b_ready_lo_o),
    .clk_rd_i(clk_hv_i), .rst_rd_ni(rst_hv_ni), .pop_i(to_axi_l3_b_pop),
    .rdata_o(to_axi_l3_b_o), .valid_o(to_axi_l3_b_valid_o),
    .dft_lv_i(dft_lv_i), .dft_hv_i(dft_hv_i));

  prim_fifo_async_lov #(.Width($bits(sc_master_r_t)), .Depth(AxiFifoDepth), .SyncStages(SyncStages))
  u_l3_r_fifo (
    .clk_wr_i(clk_lv_i), .rst_wr_ni(rst_lv_ni), .push_i(to_axi_l3_r_valid_lo_i),
    .wdata_i(to_axi_l3_r_lo_i), .ready_o(to_axi_l3_r_ready_lo_o),
    .clk_rd_i(clk_hv_i), .rst_rd_ni(rst_hv_ni), .pop_i(to_axi_l3_r_pop),
    .rdata_o(to_axi_l3_r_o), .valid_o(to_axi_l3_r_valid_o),
    .dft_lv_i(dft_lv_i), .dft_hv_i(dft_hv_i));

  prim_fifo_async_lov #(.Width($bits(sc_master_b_t)), .Depth(AxiFifoDepth), .SyncStages(SyncStages))
  u_sys_b_fifo (
    .clk_wr_i(clk_lv_i), .rst_wr_ni(rst_lv_ni), .push_i(to_axi_sys_b_valid_lo_i),
    .wdata_i(to_axi_sys_b_lo_i), .ready_o(to_axi_sys_b_ready_lo_o),
    .clk_rd_i(clk_hv_i), .rst_rd_ni(rst_hv_ni), .pop_i(to_axi_sys_b_pop),
    .rdata_o(to_axi_sys_b_o), .valid_o(to_axi_sys_b_valid_o),
    .dft_lv_i(dft_lv_i), .dft_hv_i(dft_hv_i));

  prim_fifo_async_lov #(.Width($bits(sc_master_r_t)), .Depth(AxiFifoDepth), .SyncStages(SyncStages))
  u_sys_r_fifo (
    .clk_wr_i(clk_lv_i), .rst_wr_ni(rst_lv_ni), .push_i(to_axi_sys_r_valid_lo_i),
    .wdata_i(to_axi_sys_r_lo_i), .ready_o(to_axi_sys_r_ready_lo_o),
    .clk_rd_i(clk_hv_i), .rst_rd_ni(rst_hv_ni), .pop_i(to_axi_sys_r_pop),
    .rdata_o(to_axi_sys_r_o), .valid_o(to_axi_sys_r_valid_o),
    .dft_lv_i(dft_lv_i), .dft_hv_i(dft_hv_i));

  prim_fifo_async_lov #(.Width($bits(sys_slave_ar_t)), .Depth(SlvAxiFifoDepth), .SyncStages(SyncStages))
  u_sys_ar_fifo (
    .clk_wr_i(clk_lv_i), .rst_wr_ni(rst_lv_ni), .push_i(from_axi_sys_ar_valid_lo_i),
    .wdata_i(from_axi_sys_ar_lo_i), .ready_o(from_axi_sys_ar_ready_lo_o),
    .clk_rd_i(clk_hv_i), .rst_rd_ni(rst_hv_ni), .pop_i(from_axi_sys_ar_pop),
    .rdata_o(from_axi_sys_ar_o), .valid_o(from_axi_sys_ar_valid_o),
    .dft_lv_i(dft_lv_i), .dft_hv_i(dft_hv_i));

  uncached_lo_voltage_cross_lv_logic u_lv_logic (
    .from_axi_sys_sbm_aw_vcfifo_valid_i(from_axi_sys_sbm_aw_vcfifo_valid),
    .from_axi_sys_sbm_w_vcfifo_valid_i(from_axi_sys_sbm_w_vcfifo_valid),
    .from_axi_sys_aw_valid_lo_i(from_axi_sys_aw_valid_lo_i),
    .from_axi_sys_w_valid_lo_i(from_axi_sys_w_valid_lo_i),
    .from_axi_sys_aw_vcvalid_lo_i(from_axi_sys_aw_vcvalid_lo_i),
    .from_axi_sys_w_vcvalid_lo_i(from_axi_sys_w_vcvalid_lo_i),
    .from_axi_sys_sbm_aw_ready_i(from_axi_sys_sbm_aw_ready_i),
    .from_axi_sys_sbm_w_ready_i(from_axi_sys_sbm_w_ready_i),
    .from_axi_sys_uc_aw_vcfifo_valid_i(from_axi_sys_uc_aw_vcfifo_valid),
    .from_axi_sys_uc_w_vcfifo_valid_i(from_axi_sys_uc_w_vcfifo_valid),
    .from_axi_sys_uc_aw_ready_i(from_axi_sys_uc_aw_ready_i),
    .from_axi_sys_uc_w_ready_i(from_axi_sys_uc_w_ready_i),
    .from_axi_sys_sbm_aw_vcfifo_ready_i(from_axi_sys_sbm_aw_vcfifo_ready),
    .from_axi_sys_uc_aw_vcfifo_ready_i(from_axi_sys_uc_aw_vcfifo_ready),
    .from_axi_sys_sbm_w_vcfifo_ready_i(from_axi_sys_sbm_w_vcfifo_ready),
    .from_axi_sys_uc_w_vcfifo_ready_i(from_axi_sys_uc_w_vcfifo_ready),
    .from_axi_sys_sbm_aw_valid_o(from_axi_sys_sbm_aw_valid_o),
    .from_axi_sys_sbm_aw_vcfifo_push_o(from_axi_sys_sbm_aw_vcfifo_push),
    .from_axi_sys_sbm_aw_vcfifo_pop_o(from_axi_sys_sbm_aw_vcfifo_pop),
    .from_axi_sys_uc_aw_valid_o(from_axi_sys_uc_aw_valid_o),
    .from_axi_sys_uc_aw_vcfifo_push_o(from_axi_sys_uc_aw_vcfifo_push),
    .from_axi_sys_uc_aw_vcfifo_pop_o(from_axi_sys_uc_aw_vcfifo_pop),
    .from_axi_sys_sbm_w_valid_o(from_axi_sys_sbm_w_valid_o),
    .from_axi_sys_sbm_w_vcfifo_push_o(from_axi_sys_sbm_w_vcfifo_push),
    .from_axi_sys_sbm_w_vcfifo_pop_o(from_axi_sys_sbm_w_vcfifo_pop),
    .from_axi_sys_uc_w_valid_o(from_axi_sys_uc_w_valid_o),
    .from_axi_sys_uc_w_vcfifo_push_o(from_axi_sys_uc_w_vcfifo_push),
    .from_axi_sys_uc_w_vcfifo_pop_o(from_axi_sys_uc_w_vcfifo_pop),
    .from_axi_sys_aw_ready_lo_o(from_axi_sys_aw_ready_lo_o),
    .from_axi_sys_w_ready_lo_o(from_axi_sys_w_ready_lo_o)
  );

  prim_fifo_async_lov #(.Width($bits(sys_slave_aw_t)), .Depth(SlvAxiFifoDepth), .SyncStages(SyncStages))
  u_sys_sbm_aw_fifo (
    .clk_wr_i(clk_lv_i), .rst_wr_ni(rst_lv_ni), .push_i(from_axi_sys_sbm_aw_vcfifo_push),
    .wdata_i(from_axi_sys_aw_lo_i), .ready_o(from_axi_sys_sbm_aw_vcfifo_ready),
    .clk_rd_i(clk_hv_i), .rst_rd_ni(rst_hv_ni), .pop_i(from_axi_sys_sbm_aw_vcfifo_pop),
    .rdata_o(from_axi_sys_sbm_aw_o), .valid_o(from_axi_sys_sbm_aw_vcfifo_valid),
    .dft_lv_i(dft_lv_i), .dft_hv_i(dft_hv_i));

  prim_fifo_async_lov #(.Width($bits(sys_slave_aw_t)), .Depth(SlvAxiFifoDepth), .SyncStages(SyncStages))
  u_sys_uc_aw_fifo (
    .clk_wr_i(clk_lv_i), .rst_wr_ni(rst_lv_ni), .push_i(from_axi_sys_uc_aw_vcfifo_push),
    .wdata_i(from_axi_sys_aw_lo_i), .ready_o(from_axi_sys_uc_aw_vcfifo_ready),
    .clk_rd_i(clk_hv_i), .rst_rd_ni(rst_hv_ni), .pop_i(from_axi_sys_uc_aw_vcfifo_pop),
    .rdata_o(from_axi_sys_uc_aw_o), .valid_o(from_axi_sys_uc_aw_vcfifo_valid),
    .dft_lv_i(dft_lv_i), .dft_hv_i(dft_hv_i));

  prim_fifo_async_lov #(.Width($bits(sys_slave_w_t)), .Depth(SlvAxiFifoDepth), .SyncStages(SyncStages))
  u_sys_sbm_w_fifo (
    .clk_wr_i(clk_lv_i), .rst_wr_ni(rst_lv_ni), .push_i(from_axi_sys_sbm_w_vcfifo_push),
    .wdata_i(from_axi_sys_w_lo_i), .ready_o(from_axi_sys_sbm_w_vcfifo_ready),
    .clk_rd_i(clk_hv_i), .rst_rd_ni(rst_hv_ni), .pop_i(from_axi_sys_sbm_w_vcfifo_pop),
    .rdata_o(from_axi_sys_sbm_w_o), .valid_o(from_axi_sys_sbm_w_vcfifo_valid),
    .dft_lv_i(dft_lv_i), .dft_hv_i(dft_hv_i));

  prim_fifo_async_lov #(.Width($bits(sys_slave_w_t)), .Depth(SlvAxiFifoDepth), .SyncStages(SyncStages))
  u_sys_uc_w_fifo (
    .clk_wr_i(clk_lv_i), .rst_wr_ni(rst_lv_ni), .push_i(from_axi_sys_uc_w_vcfifo_push),
    .wdata_i(from_axi_sys_w_lo_i), .ready_o(from_axi_sys_uc_w_vcfifo_ready),
    .clk_rd_i(clk_hv_i), .rst_rd_ni(rst_hv_ni), .pop_i(from_axi_sys_uc_w_vcfifo_pop),
    .rdata_o(from_axi_sys_uc_w_o), .valid_o(from_axi_sys_uc_w_vcfifo_valid),
    .dft_lv_i(dft_lv_i), .dft_hv_i(dft_hv_i));

endmodule : uncached_lo_voltage_cross
