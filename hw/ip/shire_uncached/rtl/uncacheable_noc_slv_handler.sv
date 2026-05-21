// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSEDSIGNAL */  // Response handlers intentionally inspect selected fields of packed CORE-ET request/AXI structs.
module uncacheable_noc_slv_handler
  import axi_pkg::*;
  import etlink_pkg::*;
  import shirecache_pkg::*;
  import shire_uncached_pkg::*;
#(
  parameter int unsigned ReqqSize = UcReqqSize,
  localparam int unsigned ReqqIndexWidth = $clog2(ReqqSize)
) (
  input  uc_reqq_entry_t               reqq_entry_i,
  output logic [ReqqIndexWidth-1:0]    reqq_index_o,
  input  logic                         reqq_entry_valid_i,
  output uc_reqq_valid_t               reqq_entry_clear_o,

  output shirecache_pkg::xbar_rsp_t    rsp_info_o,
  output logic                         new_rsp_o,

  input  logic                         credit_i,

  input  sys_slave_aw_t                from_sys_aw_i,
  input  logic                         from_sys_aw_valid_i,
  input  sys_slave_w_t                 from_sys_w_i,
  input  logic                         from_sys_w_valid_i,
  output sys_slave_b_t                 from_sys_b_o,
  output logic                         from_sys_b_valid_o
);

  logic slv_rsp_error_invalid_size;
  logic slv_rsp_error_burst_set;
  logic slv_rsp_invalid_axi_user;
  logic slv_error_without_neigh_rsp;
  logic slv_rsp_error;
  logic new_req_axi;
  logic new_req_error;
  logic new_req_msg;
  logic new_req_atomic;
  logic new_req;
  logic send_rsp_to_neigh;
  logic [AdEsrNeighIdSize-1:0] msg_neigh;
  shirecache_pkg::xbar_rsp_t rsp_info_msg;
  shirecache_pkg::xbar_rsp_t rsp_info_atomic;

  assign slv_rsp_error_invalid_size  = etlink_pkg::size_e'(from_sys_aw_i.size) == etlink_pkg::SizeLine;
  assign slv_rsp_error_burst_set     = from_sys_aw_i.len != '0;
  assign slv_rsp_invalid_axi_user    = !new_req_msg && !reqq_entry_valid_i;
  assign slv_rsp_error               = slv_rsp_error_invalid_size || slv_rsp_error_burst_set ||
                                       slv_rsp_invalid_axi_user;
  assign slv_error_without_neigh_rsp = slv_rsp_invalid_axi_user;

  assign reqq_index_o    = from_sys_aw_i.user[ReqqIndexWidth-1:0];
  assign new_req_axi     = from_sys_aw_valid_i && from_sys_w_valid_i && credit_i;
  assign new_req_atomic  = new_req_axi && uc_slv_is_atomic(from_sys_aw_i.addr);
  assign new_req_msg     = new_req_axi && uc_slv_is_msg(from_sys_aw_i.addr);
  assign new_req_error   = new_req_axi && (!new_req_atomic && !new_req_msg);
  assign new_req         = new_req_atomic || new_req_msg || new_req_error;

  assign send_rsp_to_neigh = !new_req_error && !slv_error_without_neigh_rsp;
  assign new_rsp_o = new_req && send_rsp_to_neigh;
  assign reqq_entry_clear_o.wait_ack = 1'b0;
  assign reqq_entry_clear_o.wait_from_sys = new_req_atomic;

  assign msg_neigh = from_sys_aw_i.addr[AdEsrNeighIdStart +: AdEsrNeighIdSize];

  always_comb begin
    rsp_info_msg = '0;
    rsp_info_msg.rsp_info.id     = from_sys_aw_i.addr[AdEsrMessageIdStart +: AdEsrMessageIdSize];
    rsp_info_msg.rsp_info.dest   = from_sys_aw_i.addr[AdEsrNeighHartIdStart +: AdEsrNeighHartIdSize];
    rsp_info_msg.rsp_info.opcode = etlink_pkg::RspMsgRcvData;
    rsp_info_msg.rsp_info.data[axi_pkg::SysSlaveDataSize-1:0] = from_sys_w_i.data;
    rsp_info_msg.port_id         = msg_neigh[shirecache_pkg::PortIdSize-1:0];
    rsp_info_msg.trans_id        = '0;
    rsp_info_msg.src_is_uc       = 1'b0;
    rsp_info_msg.rsp_info.size   = etlink_pkg::size_e'(from_sys_aw_i.size);
    rsp_info_msg.rsp_info.qwen   = uc_qwen_unalign(rsp_info_msg.rsp_info.size);
    rsp_info_msg.rsp_info.wdata  = 1'b1;

    rsp_info_atomic = '0;
    rsp_info_atomic.rsp_info.id     = reqq_entry_i.id;
    rsp_info_atomic.rsp_info.dest   = reqq_entry_i.source;
    rsp_info_atomic.rsp_info.opcode = etlink_pkg::RspAckData;
    rsp_info_atomic.rsp_info.data   = {{(etlink_pkg::DataSize-axi_pkg::SysSlaveDataSize){1'b0}}, from_sys_w_i.data} <<
                                      (from_sys_aw_i.addr[UcAlignBitsEnd:UcAlignBitsEnd] * axi_pkg::SysSlaveDataSize);
    rsp_info_atomic.port_id         = reqq_entry_i.port_id;
    rsp_info_atomic.trans_id        = reqq_entry_i.trans_id;
    rsp_info_atomic.src_is_uc       = 1'b0;
    rsp_info_atomic.rsp_info.size   = etlink_pkg::size_e'(from_sys_aw_i.size);
    rsp_info_atomic.rsp_info.qwen   = uc_qwen(rsp_info_atomic.rsp_info.size,
                                              from_sys_aw_i.addr[UcAlignBitsEnd:UcAlignBitsStart]);
    rsp_info_atomic.rsp_info.wdata  = 1'b1;
  end

  assign rsp_info_o = new_req_msg ? rsp_info_msg : rsp_info_atomic;

  assign from_sys_b_o.id   = from_sys_aw_i.id;
  assign from_sys_b_o.resp = ((uc_slv_is_msg(from_sys_aw_i.addr) || uc_slv_is_atomic(from_sys_aw_i.addr)) &&
                              !slv_rsp_error) ? axi_pkg::RespOkay : axi_pkg::RespSlvErr;
  assign from_sys_b_valid_o = new_req;

  function automatic logic uc_slv_is_atomic(input logic [etlink_pkg::PaSize-1:0] addr);
    logic atomic_rsp;
    atomic_rsp = (addr[etlink_pkg::PaSize-1:AdEsrRegionStart] == AdEsrRegion) &&
                 (addr[AdEsrProtBitsStart +: AdEsrProtBitsSize] == AdEsrProtBitsMsg) &&
                 (addr[AdEsrAgentTypeStart +: AdEsrAgentTypeSize] == AdEsrAgentTypeShire) &&
                 (addr[UcAtomicAdRspLsbStart +: UcAtomicAdRspLsbSize] == UcAtomicAdRsp);
    return atomic_rsp;
  endfunction

endmodule : uncacheable_noc_slv_handler
/* verilator lint_on UNUSEDSIGNAL */
