// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module uncacheable_noc_rsp
  import axi_pkg::*;
  import etlink_pkg::*;
  import shirecache_pkg::*;
  import shire_uncached_pkg::*;
#(
  parameter int unsigned ReqqSize = UcReqqSize,
  localparam int unsigned ReqqIndexWidth = $clog2(ReqqSize)
) (
  input  logic                       clk_i,
  input  logic                       rst_ni,

  output shirecache_pkg::xbar_rsp_t  rsp_data_o,
  output logic                       rsp_valid_o,
  input  logic                       rsp_send_i,

  input  uc_reqq_entry_t             reqq_entry_i,
  output logic [ReqqIndexWidth-1:0]  reqq_index_o,
  input  logic                       reqq_entry_valid_i,
  output uc_reqq_valid_t             reqq_entry_clear_o,

  output logic                       broadcast_new_ack_o,
  output logic                       broadcast_req_done_o,
  output logic                       broadcast_rsp_is_error_o,
  input  logic                       broadcast_rsp_is_error_reg_i,
  input  logic [BcastSendsSize-1:0]  broadcast_acks_i,
  input  logic [ReqqIndexWidth-1:0]  broadcast_req_index_i,

  input  logic                       credit_i,
  output logic                       credit_return_o,
  input  sys_slave_aw_t              from_sys_aw_i,
  input  logic                       from_sys_aw_valid_i,
  output logic                       from_sys_aw_ready_o,
  input  sys_slave_w_t               from_sys_w_i,
  input  logic                       from_sys_w_valid_i,
  output logic                       from_sys_w_ready_o,
  output sys_slave_b_t               from_sys_b_o,
  output logic                       from_sys_b_valid_o,
  input  logic                       from_sys_b_ready_i,

  input  logic                       remote_scp_enabled_i,
  input  logic                       l3_enabled_i,
  input  sc_master_b_t               to_l3_b_i,
  input  logic                       to_l3_b_valid_i,
  output logic                       to_l3_b_ready_o,
  input  sc_master_r_t               to_l3_r_i,
  input  logic                       to_l3_r_valid_i,
  output logic                       to_l3_r_ready_o,

  input  sc_master_b_t               to_sys_b_i,
  input  logic                       to_sys_b_valid_i,
  output logic                       to_sys_b_ready_o,
  input  sc_master_r_t               to_sys_r_i,
  input  logic                       to_sys_r_valid_i,
  output logic                       to_sys_r_ready_o
);

  localparam int unsigned RspFifoSize = 4;
  localparam int unsigned NumAxis = 5;
  localparam int unsigned NumAxisLog = $clog2(NumAxis);
  localparam int unsigned SlvIdx = 0;
  localparam int unsigned WrL3Idx = 1;
  localparam int unsigned RdL3Idx = 2;
  localparam int unsigned WrToSysIdx = 3;
  localparam int unsigned RdToSysIdx = 4;

  logic stall;
  logic [NumAxis-1:0] arb_req;
  logic [NumAxis-1:0] arb_granted;
  logic [NumAxisLog-1:0] bus_granted;

  logic [NumAxis-1:0] rsp_fifo_push_req;
  logic rsp_fifo_push;
  logic rsp_fifo_pop;
  logic rsp_fifo_valid;
  logic rsp_fifo_full;
  logic [RspFifoSize:0] rsp_fifo_level_unused;
  shirecache_pkg::xbar_rsp_t rsp_fifo_pop_data;
  shirecache_pkg::xbar_rsp_t [NumAxis-1:0] rsp_fifo_push_data;
  logic [NumAxis-1:0][ReqqIndexWidth-1:0] reqq_entries_idx;
  uc_reqq_valid_t [NumAxis-1:0] reqq_entries_clear;
  logic reqq_entry_clear_en;

  logic wrapper_broadcast_new_ack;
  logic wrapper_broadcast_req_done;
  logic wrapper_broadcast_rsp_is_error;
  logic slv_b_valid;

  assign reqq_index_o = reqq_entries_idx[bus_granted];
  assign reqq_entry_clear_en = |arb_granted && !stall;
  assign reqq_entry_clear_o = reqq_entries_clear[bus_granted] & {$bits(uc_reqq_valid_t){reqq_entry_clear_en}};

  assign from_sys_aw_ready_o = arb_granted[SlvIdx] && from_sys_b_ready_i;
  assign from_sys_w_ready_o  = arb_granted[SlvIdx] && from_sys_b_ready_i;
  assign from_sys_b_valid_o  = arb_granted[SlvIdx] && slv_b_valid && !rsp_fifo_full;
  assign to_l3_r_ready_o     = arb_granted[RdL3Idx] && !stall;
  assign to_l3_b_ready_o     = arb_granted[WrL3Idx] && !stall;
  assign to_sys_r_ready_o    = arb_granted[RdToSysIdx] && !stall;
  assign to_sys_b_ready_o    = arb_granted[WrToSysIdx] && !stall;

  assign rsp_data_o = rsp_fifo_pop_data;
  assign rsp_valid_o = rsp_fifo_valid;
  assign rsp_fifo_pop = rsp_send_i;
  assign rsp_fifo_push = arb_granted[bus_granted] && rsp_fifo_push_req[bus_granted] && !stall;

  assign stall = rsp_fifo_full || (arb_granted[SlvIdx] && !from_sys_b_ready_i);
  assign arb_req[SlvIdx] = from_sys_aw_valid_i && from_sys_w_valid_i;
  assign arb_req[WrL3Idx] = to_l3_b_valid_i;
  assign arb_req[RdL3Idx] = to_l3_r_valid_i;
  assign arb_req[WrToSysIdx] = to_sys_b_valid_i;
  assign arb_req[RdToSysIdx] = to_sys_r_valid_i;

  assign credit_return_o = from_sys_aw_valid_i && from_sys_aw_ready_o &&
                           from_sys_b_valid_o && from_sys_b_ready_i;

  assign broadcast_new_ack_o = wrapper_broadcast_new_ack && arb_granted[WrToSysIdx] && !stall;
  assign broadcast_req_done_o = wrapper_broadcast_req_done && arb_granted[WrToSysIdx] && !stall;
  assign broadcast_rsp_is_error_o = wrapper_broadcast_rsp_is_error && arb_granted[WrToSysIdx] && !stall;

  uncacheable_noc_slv_handler #(.ReqqSize(ReqqSize)) u_slv_handler (
    .reqq_entry_i(reqq_entry_i),
    .reqq_index_o(reqq_entries_idx[SlvIdx]),
    .reqq_entry_valid_i(reqq_entry_valid_i),
    .reqq_entry_clear_o(reqq_entries_clear[SlvIdx]),
    .rsp_info_o(rsp_fifo_push_data[SlvIdx]),
    .new_rsp_o(rsp_fifo_push_req[SlvIdx]),
    .credit_i(credit_i),
    .from_sys_aw_i(from_sys_aw_i),
    .from_sys_aw_valid_i(from_sys_aw_valid_i),
    .from_sys_w_i(from_sys_w_i),
    .from_sys_w_valid_i(from_sys_w_valid_i),
    .from_sys_b_o(from_sys_b_o),
    .from_sys_b_valid_o(slv_b_valid)
  );

  uncacheable_noc_tol3_handler #(.ReqqSize(ReqqSize)) u_tol3_handler (
    .reqq_entry_i(reqq_entry_i),
    .wr_reqq_index_o(reqq_entries_idx[WrL3Idx]),
    .wr_reqq_entry_valid_i(reqq_entry_valid_i),
    .wr_reqq_entry_clear_o(reqq_entries_clear[WrL3Idx]),
    .rd_reqq_index_o(reqq_entries_idx[RdL3Idx]),
    .rd_reqq_entry_valid_i(reqq_entry_valid_i),
    .rd_reqq_entry_clear_o(reqq_entries_clear[RdL3Idx]),
    .wr_rsp_info_o(rsp_fifo_push_data[WrL3Idx]),
    .wr_new_rsp_o(rsp_fifo_push_req[WrL3Idx]),
    .rd_rsp_info_o(rsp_fifo_push_data[RdL3Idx]),
    .rd_new_rsp_o(rsp_fifo_push_req[RdL3Idx]),
    .l3_enabled_i(l3_enabled_i),
    .remote_scp_enabled_i(remote_scp_enabled_i),
    .to_l3_b_i(to_l3_b_i),
    .to_l3_b_valid_i(to_l3_b_valid_i),
    .to_l3_r_i(to_l3_r_i),
    .to_l3_r_valid_i(to_l3_r_valid_i)
  );

  uncacheable_noc_tosys_handler #(.ReqqSize(ReqqSize)) u_tosys_handler (
    .reqq_entry_i(reqq_entry_i),
    .wr_reqq_index_o(reqq_entries_idx[WrToSysIdx]),
    .wr_reqq_entry_valid_i(reqq_entry_valid_i),
    .wr_reqq_entry_clear_o(reqq_entries_clear[WrToSysIdx]),
    .rd_reqq_index_o(reqq_entries_idx[RdToSysIdx]),
    .rd_reqq_entry_valid_i(reqq_entry_valid_i),
    .rd_reqq_entry_clear_o(reqq_entries_clear[RdToSysIdx]),
    .broadcast_req_index_i(broadcast_req_index_i),
    .broadcast_new_ack_o(wrapper_broadcast_new_ack),
    .broadcast_req_done_o(wrapper_broadcast_req_done),
    .broadcast_rsp_is_error_o(wrapper_broadcast_rsp_is_error),
    .broadcast_rsp_is_error_reg_i(broadcast_rsp_is_error_reg_i),
    .broadcast_acks_i(broadcast_acks_i),
    .wr_rsp_info_o(rsp_fifo_push_data[WrToSysIdx]),
    .wr_new_rsp_o(rsp_fifo_push_req[WrToSysIdx]),
    .rd_rsp_info_o(rsp_fifo_push_data[RdToSysIdx]),
    .rd_new_rsp_o(rsp_fifo_push_req[RdToSysIdx]),
    .to_sys_b_i(to_sys_b_i),
    .to_sys_b_valid_i(to_sys_b_valid_i),
    .to_sys_r_i(to_sys_r_i),
    .to_sys_r_valid_i(to_sys_r_valid_i)
  );

  uncached_arb_lru_grant #(.NumClients(NumAxis)) u_arb (
    .clk_i(clk_i),
    .rst_ni(rst_ni),
    .bid_i(arb_req),
    .stall_i(stall),
    .grant_o(arb_granted),
    .winner_o(bus_granted)
  );

  prim_fifo_reg #(
    .Width($bits(shirecache_pkg::xbar_rsp_t)),
    .Depth(RspFifoSize)
  ) u_rsp_fifo (
    .clk_i(clk_i),
    .rst_ni(rst_ni),
    .push_i(rsp_fifo_push),
    .wdata_i(rsp_fifo_push_data[bus_granted]),
    .full_o(rsp_fifo_full),
    .pop_i(rsp_fifo_pop),
    .valid_o(rsp_fifo_valid),
    .rdata_o(rsp_fifo_pop_data),
    .level_o(rsp_fifo_level_unused)
  );

  /* verilator lint_off UNUSEDSIGNAL */
  logic unused_level;
  assign unused_level = ^rsp_fifo_level_unused;
  /* verilator lint_on UNUSEDSIGNAL */

endmodule : uncacheable_noc_rsp
