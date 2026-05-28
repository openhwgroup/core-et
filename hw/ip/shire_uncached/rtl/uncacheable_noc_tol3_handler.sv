// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSEDSIGNAL */  // Response handlers intentionally inspect selected fields of packed CORE-ET request/AXI structs.
module uncacheable_noc_tol3_handler
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

  output shirecache_pkg::xbar_rsp_t  wr_rsp_info_o,
  output logic                       wr_new_rsp_o,
  output shirecache_pkg::xbar_rsp_t  rd_rsp_info_o,
  output logic                       rd_new_rsp_o,

  input  logic                       l3_enabled_i,
  input  logic                       remote_scp_enabled_i,
  input  sc_master_b_t               to_l3_b_i,
  input  logic                       to_l3_b_valid_i,
  input  sc_master_r_t               to_l3_r_i,
  input  logic                       to_l3_r_valid_i
);

  logic wr_rsp_is_atomic;
  logic expected_rsp;
  logic new_rd_req;
  logic new_wr_req;
  logic wr_rsp_to_neigh;
  logic rd_rsp_to_neigh;
  logic wr_rsp_error;
  logic rd_rsp_error;

  assign wr_reqq_index_o = to_l3_b_i.id[ReqqIndexWidth-1:0];
  assign rd_reqq_index_o = to_l3_r_i.id[ReqqIndexWidth-1:0];

  assign expected_rsp = ((reqq_entry_i.address[etlink_pkg::PaSize-1:AdDdrRegionStart] == AdDdrRegion) && l3_enabled_i) ||
                        (uc_is_scp(reqq_entry_i.address) && remote_scp_enabled_i);
  assign new_wr_req = wr_reqq_entry_valid_i && expected_rsp && to_l3_b_valid_i;
  assign new_rd_req = rd_reqq_entry_valid_i && expected_rsp && to_l3_r_valid_i;

  assign wr_rsp_is_atomic = uc_is_atomic(reqq_entry_i.opcode);
  assign wr_rsp_error = to_l3_b_i.resp != axi_pkg::RespOkay;
  assign rd_rsp_error = to_l3_r_i.resp != axi_pkg::RespOkay;

  assign wr_rsp_to_neigh = (new_wr_req && reqq_entry_i.resp) ||
                           (new_wr_req && wr_rsp_is_atomic && wr_rsp_error);
  assign rd_rsp_to_neigh = new_rd_req && reqq_entry_i.resp;

  assign wr_reqq_entry_clear_o.wait_ack      = wr_rsp_to_neigh || (new_wr_req && wr_rsp_is_atomic);
  assign wr_reqq_entry_clear_o.wait_from_sys = new_wr_req && wr_rsp_is_atomic && wr_rsp_error;
  assign rd_reqq_entry_clear_o.wait_ack      = rd_rsp_to_neigh;
  assign rd_reqq_entry_clear_o.wait_from_sys = 1'b0;

  assign wr_new_rsp_o = wr_rsp_to_neigh;
  assign rd_new_rsp_o = rd_rsp_to_neigh;

  always_comb begin
    wr_rsp_info_o = '0;
    wr_rsp_info_o.rsp_info.id     = reqq_entry_i.id;
    wr_rsp_info_o.rsp_info.size   = reqq_entry_i.size;
    wr_rsp_info_o.rsp_info.dest   = reqq_entry_i.source;
    wr_rsp_info_o.rsp_info.opcode = !wr_rsp_error ? etlink_pkg::RspAck : etlink_pkg::RspErr;
    wr_rsp_info_o.port_id         = reqq_entry_i.port_id;
    wr_rsp_info_o.trans_id        = reqq_entry_i.trans_id;
    wr_rsp_info_o.src_is_uc       = 1'b0;
    wr_rsp_info_o.rsp_info.qwen   = 4'b0000;
    wr_rsp_info_o.rsp_info.wdata  = 1'b0;
    wr_rsp_info_o.rsp_info.data   = '0;

    rd_rsp_info_o = '0;
    rd_rsp_info_o.rsp_info.id     = reqq_entry_i.id;
    rd_rsp_info_o.rsp_info.size   = reqq_entry_i.size;
    rd_rsp_info_o.rsp_info.dest   = reqq_entry_i.source;
    rd_rsp_info_o.rsp_info.opcode = !rd_rsp_error ? etlink_pkg::RspAckData : etlink_pkg::RspErr;
    rd_rsp_info_o.rsp_info.data   = reqq_entry_i.address[5] ?
        to_l3_r_i.data[axi_pkg::ScMasterDataSize-1 -: etlink_pkg::DataSize] :
        to_l3_r_i.data[etlink_pkg::DataSize-1:0];
    rd_rsp_info_o.port_id         = reqq_entry_i.port_id;
    rd_rsp_info_o.trans_id        = reqq_entry_i.trans_id;
    rd_rsp_info_o.src_is_uc       = 1'b0;
    rd_rsp_info_o.rsp_info.qwen   = uc_qwen(reqq_entry_i.size,
                                            reqq_entry_i.address[UcAlignBitsEnd:UcAlignBitsStart]);
    rd_rsp_info_o.rsp_info.wdata  = !rd_rsp_error;
  end

endmodule : uncacheable_noc_tol3_handler
/* verilator lint_on UNUSEDSIGNAL */
