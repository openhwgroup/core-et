// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSEDSIGNAL */  // Response handlers intentionally inspect selected fields of packed CORE-ET request/AXI structs.
module uncacheable_noc_tosys_handler
  import axi_pkg::*;
  import etlink_pkg::*;
  import shirecache_pkg::*;
  import shire_uncached_pkg::*;
#(
  parameter int unsigned ReqqSize = UcReqqSize,
  localparam int unsigned ReqqIndexWidth = $clog2(ReqqSize)
) (
  input  uc_reqq_entry_t             reqq_entry_i,
  output logic [ReqqIndexWidth-1:0]  wr_reqq_index_o,
  input  logic                       wr_reqq_entry_valid_i,
  output uc_reqq_valid_t             wr_reqq_entry_clear_o,
  output logic [ReqqIndexWidth-1:0]  rd_reqq_index_o,
  input  logic                       rd_reqq_entry_valid_i,
  output uc_reqq_valid_t             rd_reqq_entry_clear_o,

  input  logic [ReqqIndexWidth-1:0]  broadcast_req_index_i,
  output logic                       broadcast_new_ack_o,
  output logic                       broadcast_req_done_o,
  output logic                       broadcast_rsp_is_error_o,
  input  logic                       broadcast_rsp_is_error_reg_i,
  input  logic [BcastSendsSize-1:0]  broadcast_acks_i,

  output shirecache_pkg::xbar_rsp_t  wr_rsp_info_o,
  output logic                       wr_new_rsp_o,
  output shirecache_pkg::xbar_rsp_t  rd_rsp_info_o,
  output logic                       rd_new_rsp_o,

  input  sc_master_b_t               to_sys_b_i,
  input  logic                       to_sys_b_valid_i,
  input  sc_master_r_t               to_sys_r_i,
  input  logic                       to_sys_r_valid_i
);

  logic wr_rsp_is_atomic;
  logic wr_rsp_is_broadcast;
  logic wr_rsp_is_msg;
  logic new_rd_req;
  logic new_wr_req;
  logic wr_rsp_to_neigh;
  logic rd_rsp_to_neigh;
  logic wr_rsp_error;
  logic rd_rsp_error;
  logic [BcastSendsSize-1:0] broadcast_acks_next;
  logic last_broadcast_ack;

  assign wr_reqq_index_o = wr_rsp_is_broadcast ? broadcast_req_index_i :
                           to_sys_b_i.id[ReqqIndexWidth-1:0];
  assign rd_reqq_index_o = to_sys_r_i.id[ReqqIndexWidth-1:0];

  assign new_rd_req = rd_reqq_entry_valid_i && to_sys_r_valid_i;
  assign new_wr_req = wr_reqq_entry_valid_i && to_sys_b_valid_i;

  assign wr_rsp_error = to_sys_b_i.resp != axi_pkg::RespOkay;
  assign rd_rsp_error = to_sys_r_i.resp != axi_pkg::RespOkay;

  assign broadcast_acks_next = broadcast_acks_i - {{BcastSendsSize-1{1'b0}}, 1'b1};
  assign last_broadcast_ack  = broadcast_acks_next == '0;

  assign wr_rsp_is_msg = to_sys_b_valid_i && wr_reqq_entry_valid_i && uc_slv_is_msg(reqq_entry_i.address);
  assign wr_rsp_is_atomic = to_sys_b_valid_i && wr_reqq_entry_valid_i && uc_is_atomic(reqq_entry_i.opcode);
  assign wr_rsp_is_broadcast = to_sys_b_valid_i && (to_sys_b_i.id >= ReqqSize[axi_pkg::ScMasterIdSize-1:0]);

  always_comb begin
    wr_rsp_to_neigh = new_wr_req && reqq_entry_i.resp &&
                      (!wr_rsp_is_broadcast || (wr_rsp_is_broadcast && last_broadcast_ack));
    wr_rsp_to_neigh |= wr_rsp_is_atomic && wr_rsp_error;
    wr_rsp_to_neigh &= !wr_rsp_is_msg;
  end

  assign rd_rsp_to_neigh = to_sys_r_valid_i && reqq_entry_i.resp && new_rd_req;

  assign broadcast_new_ack_o = wr_rsp_is_broadcast;
  assign broadcast_req_done_o = broadcast_new_ack_o && (broadcast_acks_i == {{BcastSendsSize-1{1'b0}}, 1'b1});
  assign broadcast_rsp_is_error_o = wr_rsp_is_broadcast && wr_rsp_error;

  assign wr_reqq_entry_clear_o.wait_ack      = wr_rsp_to_neigh ||
                                               (new_wr_req && (wr_rsp_is_atomic || wr_rsp_is_msg));
  assign wr_reqq_entry_clear_o.wait_from_sys = new_wr_req && wr_rsp_is_atomic && wr_rsp_error;
  assign rd_reqq_entry_clear_o.wait_ack      = rd_rsp_to_neigh;
  assign rd_reqq_entry_clear_o.wait_from_sys = 1'b0;

  assign wr_new_rsp_o = wr_rsp_to_neigh;
  assign rd_new_rsp_o = rd_rsp_to_neigh;

  always_comb begin
    wr_rsp_info_o = '0;
    wr_rsp_info_o.rsp_info.id     = reqq_entry_i.id;
    wr_rsp_info_o.rsp_info.size   = reqq_entry_i.size;
    wr_rsp_info_o.rsp_info.qwen   = 4'b0000;
    wr_rsp_info_o.rsp_info.dest   = reqq_entry_i.source;
    wr_rsp_info_o.rsp_info.wdata  = 1'b0;
    wr_rsp_info_o.rsp_info.opcode = ((!wr_rsp_is_broadcast && !wr_rsp_error) ||
                                      (wr_rsp_is_broadcast && !broadcast_rsp_is_error_reg_i)) ?
                                      etlink_pkg::RspAck : etlink_pkg::RspErr;
    wr_rsp_info_o.rsp_info.data   = '0;
    wr_rsp_info_o.port_id         = reqq_entry_i.port_id;
    wr_rsp_info_o.trans_id        = reqq_entry_i.trans_id;
    wr_rsp_info_o.src_is_uc       = 1'b0;

    rd_rsp_info_o = '0;
    rd_rsp_info_o.rsp_info.id     = reqq_entry_i.id;
    rd_rsp_info_o.rsp_info.size   = reqq_entry_i.size;
    rd_rsp_info_o.rsp_info.qwen   = uc_qwen(reqq_entry_i.size,
                                            reqq_entry_i.address[UcAlignBitsEnd:UcAlignBitsStart]);
    rd_rsp_info_o.rsp_info.dest   = reqq_entry_i.source;
    rd_rsp_info_o.rsp_info.wdata  = !rd_rsp_error;
    rd_rsp_info_o.rsp_info.opcode = !rd_rsp_error ? etlink_pkg::RspAckData : etlink_pkg::RspErr;
    rd_rsp_info_o.rsp_info.data   = reqq_entry_i.address[5] ?
        to_sys_r_i.data[axi_pkg::ScMasterDataSize-1 -: etlink_pkg::DataSize] :
        to_sys_r_i.data[etlink_pkg::DataSize-1:0];
    rd_rsp_info_o.port_id         = reqq_entry_i.port_id;
    rd_rsp_info_o.trans_id        = reqq_entry_i.trans_id;
    rd_rsp_info_o.src_is_uc       = 1'b0;
  end

endmodule : uncacheable_noc_tosys_handler
/* verilator lint_on UNUSEDSIGNAL */
