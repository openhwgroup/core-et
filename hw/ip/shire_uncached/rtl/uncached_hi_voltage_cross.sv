// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module uncached_hi_voltage_cross
  import axi_pkg::*;
  import dft_pkg::*;
  import shire_uncached_pkg::*;
(
  input  logic           clk_hv_i,
  input  logic           rst_hv_ni,
  input  logic           clk_lv_i,
  input  logic           rst_lv_ni,

  input  sc_master_ar_t  to_axi_l3_ar_i,
  input  logic           to_axi_l3_ar_valid_i,
  output logic           to_axi_l3_ar_ready_o,
  input  sc_master_aw_t  to_axi_l3_aw_i,
  input  logic           to_axi_l3_aw_valid_i,
  output logic           to_axi_l3_aw_ready_o,
  input  sc_master_w_t   to_axi_l3_w_i,
  input  logic           to_axi_l3_w_valid_i,
  output logic           to_axi_l3_w_ready_o,
  output sc_master_ar_t  to_axi_l3_ar_lo_o,
  output logic           to_axi_l3_ar_valid_lo_o,
  input  logic           to_axi_l3_ar_ready_lo_i,
  output sc_master_aw_t  to_axi_l3_aw_lo_o,
  output logic           to_axi_l3_aw_valid_lo_o,
  input  logic           to_axi_l3_aw_ready_lo_i,
  output sc_master_w_t   to_axi_l3_w_lo_o,
  output logic           to_axi_l3_w_valid_lo_o,
  input  logic           to_axi_l3_w_ready_lo_i,

  input  sc_master_ar_t  to_axi_sys_ar_i,
  input  logic           to_axi_sys_ar_valid_i,
  output logic           to_axi_sys_ar_ready_o,
  input  sc_master_aw_t  to_axi_sys_aw_i,
  input  logic           to_axi_sys_aw_valid_i,
  output logic           to_axi_sys_aw_ready_o,
  input  sc_master_w_t   to_axi_sys_w_i,
  input  logic           to_axi_sys_w_valid_i,
  output logic           to_axi_sys_w_ready_o,
  output sc_master_ar_t  to_axi_sys_ar_lo_o,
  output logic           to_axi_sys_ar_valid_lo_o,
  input  logic           to_axi_sys_ar_ready_lo_i,
  output sc_master_aw_t  to_axi_sys_aw_lo_o,
  output logic           to_axi_sys_aw_valid_lo_o,
  input  logic           to_axi_sys_aw_ready_lo_i,
  output sc_master_w_t   to_axi_sys_w_lo_o,
  output logic           to_axi_sys_w_valid_lo_o,
  input  logic           to_axi_sys_w_ready_lo_i,

  input  sys_slave_r_t   from_axi_sys_r_i,
  input  logic           from_axi_sys_r_valid_i,
  output logic           from_axi_sys_r_ready_o,
  output sys_slave_r_t   from_axi_sys_r_lo_o,
  output logic           from_axi_sys_r_valid_lo_o,
  input  logic           from_axi_sys_r_ready_lo_i,
  input  sys_slave_b_t   from_axi_sys_b_i,
  input  logic [1:0]     from_axi_sys_credit_i,
  input  logic           from_axi_sys_b_valid_i,
  output logic           from_axi_sys_b_ready_o,
  output sys_slave_b_t   from_axi_sys_b_lo_o,
  output logic [1:0]     from_axi_sys_credit_lo_o,
  output logic           from_axi_sys_b_valid_lo_o,
  input  logic           from_axi_sys_b_ready_lo_i,

  input  dft_t           dft_hv_i,
  input  dft_t           dft_lv_i
);

  localparam int unsigned AxiFifoDepth = 6;
  localparam int unsigned SyncStages = 2;
  localparam int unsigned SbmSysSlaveVsiId = 1;
  localparam int unsigned UcSysSlaveVsiId = 0;

  logic to_axi_l3_ar_vcfifo_valid;
  logic to_axi_l3_ar_vcfifo_pop;
  logic to_axi_l3_aw_vcfifo_valid;
  logic to_axi_l3_aw_vcfifo_pop;
  logic to_axi_l3_w_vcfifo_valid;
  logic to_axi_l3_w_vcfifo_pop;
  logic to_axi_sys_ar_vcfifo_valid;
  logic to_axi_sys_ar_vcfifo_pop;
  logic to_axi_sys_aw_vcfifo_valid;
  logic to_axi_sys_aw_vcfifo_pop;
  logic to_axi_sys_w_vcfifo_valid;
  logic to_axi_sys_w_vcfifo_pop;
  logic from_axi_sys_b_vcfifo_valid;
  logic from_axi_sys_b_vcfifo_pop;
  logic from_axi_sys_r_vcfifo_valid;
  logic from_axi_sys_r_vcfifo_pop;
  sys_slave_b_t from_axi_sys_b_w;

  prim_fifo_async_hiv #(.Width($bits(sc_master_ar_t)), .Depth(AxiFifoDepth), .SyncStages(SyncStages))
  u_l3_ar_fifo (
    .clk_wr_i(clk_hv_i), .rst_wr_ni(rst_hv_ni), .push_i(to_axi_l3_ar_valid_i),
    .wdata_i(to_axi_l3_ar_i), .ready_o(to_axi_l3_ar_ready_o),
    .clk_rd_i(clk_lv_i), .rst_rd_ni(rst_lv_ni), .pop_i(to_axi_l3_ar_vcfifo_pop),
    .rdata_o(to_axi_l3_ar_lo_o), .valid_o(to_axi_l3_ar_vcfifo_valid),
    .dft_hv_i(dft_hv_i), .dft_lv_i(dft_lv_i));

  prim_fifo_async_hiv #(.Width($bits(sc_master_aw_t)), .Depth(AxiFifoDepth), .SyncStages(SyncStages))
  u_l3_aw_fifo (
    .clk_wr_i(clk_hv_i), .rst_wr_ni(rst_hv_ni), .push_i(to_axi_l3_aw_valid_i),
    .wdata_i(to_axi_l3_aw_i), .ready_o(to_axi_l3_aw_ready_o),
    .clk_rd_i(clk_lv_i), .rst_rd_ni(rst_lv_ni), .pop_i(to_axi_l3_aw_vcfifo_pop),
    .rdata_o(to_axi_l3_aw_lo_o), .valid_o(to_axi_l3_aw_vcfifo_valid),
    .dft_hv_i(dft_hv_i), .dft_lv_i(dft_lv_i));

  prim_fifo_async_hiv #(.Width($bits(sc_master_w_t)), .Depth(AxiFifoDepth), .SyncStages(SyncStages))
  u_l3_w_fifo (
    .clk_wr_i(clk_hv_i), .rst_wr_ni(rst_hv_ni), .push_i(to_axi_l3_w_valid_i),
    .wdata_i(to_axi_l3_w_i), .ready_o(to_axi_l3_w_ready_o),
    .clk_rd_i(clk_lv_i), .rst_rd_ni(rst_lv_ni), .pop_i(to_axi_l3_w_vcfifo_pop),
    .rdata_o(to_axi_l3_w_lo_o), .valid_o(to_axi_l3_w_vcfifo_valid),
    .dft_hv_i(dft_hv_i), .dft_lv_i(dft_lv_i));

  prim_fifo_async_hiv #(.Width($bits(sc_master_ar_t)), .Depth(AxiFifoDepth), .SyncStages(SyncStages))
  u_sys_ar_fifo (
    .clk_wr_i(clk_hv_i), .rst_wr_ni(rst_hv_ni), .push_i(to_axi_sys_ar_valid_i),
    .wdata_i(to_axi_sys_ar_i), .ready_o(to_axi_sys_ar_ready_o),
    .clk_rd_i(clk_lv_i), .rst_rd_ni(rst_lv_ni), .pop_i(to_axi_sys_ar_vcfifo_pop),
    .rdata_o(to_axi_sys_ar_lo_o), .valid_o(to_axi_sys_ar_vcfifo_valid),
    .dft_hv_i(dft_hv_i), .dft_lv_i(dft_lv_i));

  prim_fifo_async_hiv #(.Width($bits(sc_master_aw_t)), .Depth(AxiFifoDepth), .SyncStages(SyncStages))
  u_sys_aw_fifo (
    .clk_wr_i(clk_hv_i), .rst_wr_ni(rst_hv_ni), .push_i(to_axi_sys_aw_valid_i),
    .wdata_i(to_axi_sys_aw_i), .ready_o(to_axi_sys_aw_ready_o),
    .clk_rd_i(clk_lv_i), .rst_rd_ni(rst_lv_ni), .pop_i(to_axi_sys_aw_vcfifo_pop),
    .rdata_o(to_axi_sys_aw_lo_o), .valid_o(to_axi_sys_aw_vcfifo_valid),
    .dft_hv_i(dft_hv_i), .dft_lv_i(dft_lv_i));

  prim_fifo_async_hiv #(.Width($bits(sc_master_w_t)), .Depth(AxiFifoDepth), .SyncStages(SyncStages))
  u_sys_w_fifo (
    .clk_wr_i(clk_hv_i), .rst_wr_ni(rst_hv_ni), .push_i(to_axi_sys_w_valid_i),
    .wdata_i(to_axi_sys_w_i), .ready_o(to_axi_sys_w_ready_o),
    .clk_rd_i(clk_lv_i), .rst_rd_ni(rst_lv_ni), .pop_i(to_axi_sys_w_vcfifo_pop),
    .rdata_o(to_axi_sys_w_lo_o), .valid_o(to_axi_sys_w_vcfifo_valid),
    .dft_hv_i(dft_hv_i), .dft_lv_i(dft_lv_i));

  prim_fifo_async_hiv #(.Width($bits(sys_slave_b_t)), .Depth(AxiFifoDepth), .SyncStages(SyncStages))
  u_sys_b_fifo (
    .clk_wr_i(clk_hv_i), .rst_wr_ni(rst_hv_ni), .push_i(from_axi_sys_b_valid_i),
    .wdata_i(from_axi_sys_b_i), .ready_o(from_axi_sys_b_ready_o),
    .clk_rd_i(clk_lv_i), .rst_rd_ni(rst_lv_ni), .pop_i(from_axi_sys_b_vcfifo_pop),
    .rdata_o(from_axi_sys_b_w), .valid_o(from_axi_sys_b_vcfifo_valid),
    .dft_hv_i(dft_hv_i), .dft_lv_i(dft_lv_i));

  logic credit_sbm_valid;
  logic credit_uc_valid;
  logic credit_sbm_data_unused;
  logic credit_uc_data_unused;
  logic credit_sbm_ready_unused;
  logic credit_uc_ready_unused;

  prim_fifo_async_hiv #(.Width(1), .Depth(AxiFifoDepth), .SyncStages(SyncStages))
  u_credit_sbm_fifo (
    .clk_wr_i(clk_hv_i), .rst_wr_ni(rst_hv_ni), .push_i(from_axi_sys_credit_i[SbmSysSlaveVsiId]),
    .wdata_i(1'b1), .ready_o(credit_sbm_ready_unused),
    .clk_rd_i(clk_lv_i), .rst_rd_ni(rst_lv_ni), .pop_i(credit_sbm_valid),
    .rdata_o(credit_sbm_data_unused), .valid_o(credit_sbm_valid),
    .dft_hv_i(dft_hv_i), .dft_lv_i(dft_lv_i));

  prim_fifo_async_hiv #(.Width(1), .Depth(AxiFifoDepth), .SyncStages(SyncStages))
  u_credit_uc_fifo (
    .clk_wr_i(clk_hv_i), .rst_wr_ni(rst_hv_ni), .push_i(from_axi_sys_credit_i[UcSysSlaveVsiId]),
    .wdata_i(1'b1), .ready_o(credit_uc_ready_unused),
    .clk_rd_i(clk_lv_i), .rst_rd_ni(rst_lv_ni), .pop_i(credit_uc_valid),
    .rdata_o(credit_uc_data_unused), .valid_o(credit_uc_valid),
    .dft_hv_i(dft_hv_i), .dft_lv_i(dft_lv_i));

  assign from_axi_sys_credit_lo_o[SbmSysSlaveVsiId] = credit_sbm_valid;
  assign from_axi_sys_credit_lo_o[UcSysSlaveVsiId] = credit_uc_valid;

  prim_fifo_async_hiv #(.Width($bits(sys_slave_r_t)), .Depth(AxiFifoDepth), .SyncStages(SyncStages))
  u_sys_r_fifo (
    .clk_wr_i(clk_hv_i), .rst_wr_ni(rst_hv_ni), .push_i(from_axi_sys_r_valid_i),
    .wdata_i(from_axi_sys_r_i), .ready_o(from_axi_sys_r_ready_o),
    .clk_rd_i(clk_lv_i), .rst_rd_ni(rst_lv_ni), .pop_i(from_axi_sys_r_vcfifo_pop),
    .rdata_o(from_axi_sys_r_lo_o), .valid_o(from_axi_sys_r_vcfifo_valid),
    .dft_hv_i(dft_hv_i), .dft_lv_i(dft_lv_i));

  uncached_hi_voltage_cross_lv_logic u_lv_logic (
    .to_axi_l3_ar_ready_lo_i(to_axi_l3_ar_ready_lo_i),
    .to_axi_l3_ar_vcfifo_valid_i(to_axi_l3_ar_vcfifo_valid),
    .to_axi_l3_ar_valid_lo_o(to_axi_l3_ar_valid_lo_o),
    .to_axi_l3_ar_vcfifo_pop_o(to_axi_l3_ar_vcfifo_pop),
    .to_axi_l3_aw_ready_lo_i(to_axi_l3_aw_ready_lo_i),
    .to_axi_l3_w_ready_lo_i(to_axi_l3_w_ready_lo_i),
    .to_axi_l3_aw_vcfifo_valid_i(to_axi_l3_aw_vcfifo_valid),
    .to_axi_l3_w_vcfifo_valid_i(to_axi_l3_w_vcfifo_valid),
    .to_axi_l3_aw_valid_lo_o(to_axi_l3_aw_valid_lo_o),
    .to_axi_l3_aw_vcfifo_pop_o(to_axi_l3_aw_vcfifo_pop),
    .to_axi_l3_w_valid_lo_o(to_axi_l3_w_valid_lo_o),
    .to_axi_l3_w_vcfifo_pop_o(to_axi_l3_w_vcfifo_pop),
    .to_axi_sys_ar_ready_lo_i(to_axi_sys_ar_ready_lo_i),
    .to_axi_sys_ar_vcfifo_valid_i(to_axi_sys_ar_vcfifo_valid),
    .to_axi_sys_ar_valid_lo_o(to_axi_sys_ar_valid_lo_o),
    .to_axi_sys_ar_vcfifo_pop_o(to_axi_sys_ar_vcfifo_pop),
    .to_axi_sys_aw_ready_lo_i(to_axi_sys_aw_ready_lo_i),
    .to_axi_sys_w_ready_lo_i(to_axi_sys_w_ready_lo_i),
    .to_axi_sys_aw_vcfifo_valid_i(to_axi_sys_aw_vcfifo_valid),
    .to_axi_sys_w_vcfifo_valid_i(to_axi_sys_w_vcfifo_valid),
    .to_axi_sys_aw_valid_lo_o(to_axi_sys_aw_valid_lo_o),
    .to_axi_sys_aw_vcfifo_pop_o(to_axi_sys_aw_vcfifo_pop),
    .to_axi_sys_w_valid_lo_o(to_axi_sys_w_valid_lo_o),
    .to_axi_sys_w_vcfifo_pop_o(to_axi_sys_w_vcfifo_pop),
    .from_axi_sys_b_ready_lo_i(from_axi_sys_b_ready_lo_i),
    .from_axi_sys_b_vcfifo_valid_i(from_axi_sys_b_vcfifo_valid),
    .from_axi_sys_b_valid_lo_o(from_axi_sys_b_valid_lo_o),
    .from_axi_sys_b_vcfifo_pop_o(from_axi_sys_b_vcfifo_pop),
    .from_axi_sys_b_w_i(from_axi_sys_b_w),
    .from_axi_sys_b_lo_o(from_axi_sys_b_lo_o),
    .from_axi_sys_r_ready_lo_i(from_axi_sys_r_ready_lo_i),
    .from_axi_sys_r_vcfifo_valid_i(from_axi_sys_r_vcfifo_valid),
    .from_axi_sys_r_valid_lo_o(from_axi_sys_r_valid_lo_o),
    .from_axi_sys_r_vcfifo_pop_o(from_axi_sys_r_vcfifo_pop)
  );

  /* verilator lint_off UNUSEDSIGNAL */
  logic unused_credit_data;
  assign unused_credit_data = credit_sbm_data_unused ^ credit_uc_data_unused ^
                              credit_sbm_ready_unused ^ credit_uc_ready_unused;
  /* verilator lint_on UNUSEDSIGNAL */

endmodule : uncached_hi_voltage_cross
