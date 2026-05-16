// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off SYNCASYNCNET */  // The wrapper drives the preserved intpipe_csr_msgs reset into both async-reset state and original synchronous-reset clock_wr state.
module intpipe_csr_msgs_tb
  import minion_pkg::*;
(
  input  logic        clk_i,
  input  logic        rst_ni,

  input  logic        cpu_mem_porthead_en_any_i,
  input  logic        cpu_mem_portheadnb_en_any_i,
  input  logic [7:0]  cpu_mem_sen_i,
  input  logic [1:0]  mem_addr_i,
  input  logic [2:0]  mem_cmd_i,
  input  logic        mem_valid_qual_i,
  input  logic        mem_thread_id_i,
  input  logic [3:0]  mem_wdata_i,
  output logic [63:0] mem_rdata_next_o,
  input  logic [1:0]  mpp_i,

  input  logic [7:0]  cpu_wb_sen_i,
  input  logic        wb_thread_id_i,
  input  logic [63:0] wb_wdata_i,
  output logic        msg_port_pget_block_o,
  output logic        msg_port_xcpt_o,
  output logic [7:0]  msg_port_stall_o,

  input  logic        l2_resp_valid_i,
  input  logic        l2_resp_ready_i,
  input  logic [7:0]  l2_resp_id_i,
  input  logic        l2_resp_dest_i,
  input  logic        l2_resp_wdata_i,
  input  logic [1:0]  l2_resp_opcode_i,
  input  logic [2:0]  l2_resp_size_i,
  input  logic [3:0]  l2_resp_qwen_i,
  output logic        msg_port_wr_en_reg_next_o,
  output logic [1:0]  msg_port_scp_way_reg_next_o,
  output logic [9:0]  msg_port_wr_addr_reg_next_o,
  output logic        l2_resp_msg_valid_o,

  input  logic        req_port_info_thread_id_i,
  input  logic [1:0]  req_port_info_port_id_i,
  output logic        req_port_info_enabled_o,
  output logic [14:0] req_port_info_reply_o,
  output logic        req_port_info_umode_o,
  output logic [2:0]  req_port_info_logsize_o,
  output logic [3:0]  req_port_info_max_msgs_o,
  output logic [3:0]  req_port_info_scp_set_o,
  output logic [1:0]  req_port_info_scp_way_o,
  output logic        req_port_info_enable_oob_o,

  output logic [63:0] portctl_o
);

  et_link_minion_rsp_info_t l2_resp;
  core_msg_port_conf_t      req_port_info_reply;
  logic [NrThreads-1:0][NrMsgPorts-1:0] msg_port_stall;

  always_comb begin
    l2_resp = '0;
    l2_resp.id = l2_resp_id_i;
    l2_resp.dest = l2_resp_dest_i;
    l2_resp.wdata = l2_resp_wdata_i;
    l2_resp.opcode = et_link_rsp_opcode_e'(l2_resp_opcode_i);
    l2_resp.size = et_link_size_e'(l2_resp_size_i);
    l2_resp.qwen = l2_resp_qwen_i;

    msg_port_stall_o = msg_port_stall;
    req_port_info_reply_o = req_port_info_reply;
    req_port_info_umode_o = req_port_info_reply.umode;
    req_port_info_logsize_o = req_port_info_reply.logsize;
    req_port_info_max_msgs_o = req_port_info_reply.max_msgs;
    req_port_info_scp_set_o = req_port_info_reply.scp_set;
    req_port_info_scp_way_o = req_port_info_reply.scp_way;
    req_port_info_enable_oob_o = req_port_info_reply.enable_oob;
  end

  intpipe_csr_msgs u_dut (
    .clk_i,
    .clk_wr_i                    (clk_i),
    .rst_ni,
    .cpu_mem_porthead_en_any     (cpu_mem_porthead_en_any_i),
    .cpu_mem_portheadnb_en_any   (cpu_mem_portheadnb_en_any_i),
    .cpu_mem_sen                 (cpu_mem_sen_i),
    .mem_addr                    (mem_addr_i),
    .mem_cmd                     (csr_cmd_e'(mem_cmd_i)),
    .mem_valid_qual              (mem_valid_qual_i),
    .mem_thread_id               (mem_thread_id_i),
    .mem_wdata                   (mem_wdata_i),
    .mem_rdata_next              (mem_rdata_next_o),
    .mpp                         (mpp_i),
    .cpu_wb_sen                  (cpu_wb_sen_i),
    .wb_thread_id                (wb_thread_id_i),
    .wb_wdata                    (wb_wdata_i),
    .msg_port_pget_block         (msg_port_pget_block_o),
    .msg_port_xcpt               (msg_port_xcpt_o),
    .msg_port_stall              (msg_port_stall),
    .l2_resp_valid               (l2_resp_valid_i),
    .l2_resp_ready               (l2_resp_ready_i),
    .l2_resp                     (l2_resp),
    .msg_port_wr_en_reg_next     (msg_port_wr_en_reg_next_o),
    .msg_port_scp_way_reg_next   (msg_port_scp_way_reg_next_o),
    .msg_port_wr_addr_reg_next   (msg_port_wr_addr_reg_next_o),
    .l2_resp_msg_valid           (l2_resp_msg_valid_o),
    .req_port_info_thread_id     (req_port_info_thread_id_i),
    .req_port_info_port_id       (req_port_info_port_id_i),
    .req_port_info_enabled       (req_port_info_enabled_o),
    .req_port_info_reply         (req_port_info_reply),
    .portctl                     (portctl_o)
  );

endmodule
/* verilator lint_on SYNCASYNCNET */
