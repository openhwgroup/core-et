// Co-simulation testbench: intpipe_csr_msgs (new) vs original.

`include "soc.vh"

module cosim_msgs_tb (
  input  logic          clk_i,
  input  logic          rst_ni,

  // Stimulus — core interface
  input  logic          stim_cpu_mem_porthead_en_any,
  input  logic          stim_cpu_mem_portheadnb_en_any,
  input  logic [7:0]    stim_cpu_mem_sen,      // [NrMsgPorts-1:0][NrThreads-1:0] = 4*2 = 8 bits
  input  logic [1:0]    stim_mem_addr,
  input  logic [2:0]    stim_mem_cmd,          // csr_cmd_e / csr_cmd_t
  input  logic          stim_mem_valid_qual,
  input  logic          stim_mem_thread_id,
  input  logic [3:0]    stim_mem_wdata,
  input  logic [1:0]    stim_mpp,

  input  logic [7:0]    stim_cpu_wb_sen,       // [NrMsgPorts-1:0][NrThreads-1:0] = 8 bits
  input  logic          stim_wb_thread_id,
  input  logic [63:0]   stim_wb_wdata,

  // Stimulus — l2 interface
  input  logic          stim_l2_resp_valid,
  input  logic          stim_l2_resp_ready,
  input  logic [274:0]  stim_l2_resp,          // et_link_minion_rsp_info_t flat

  // Stimulus — port info query
  input  logic          stim_req_port_info_thread_id,
  input  logic [1:0]    stim_req_port_info_port_id,

  // New outputs
  output logic          new_msg_port_pget_block,
  output logic          new_msg_port_xcpt,
  output logic [7:0]    new_msg_port_stall,       // [NrThreads-1:0][NrMsgPorts-1:0] flat
  output logic [63:0]   new_portctl,
  output logic [63:0]   new_mem_rdata_next,
  output logic          new_msg_port_wr_en_reg_next,
  output logic [1:0]    new_msg_port_scp_way_reg_next,
  output logic [9:0]    new_msg_port_wr_addr_reg_next,
  output logic          new_l2_resp_msg_valid,
  output logic          new_req_port_info_enabled,
  output logic [14:0]   new_req_port_info_reply,   // core_msg_port_conf_t flat

  // Original outputs
  output logic          orig_msg_port_pget_block,
  output logic          orig_msg_port_xcpt,
  output logic [7:0]    orig_msg_port_stall,
  output logic [63:0]   orig_portctl,
  output logic [63:0]   orig_mem_rdata_next,
  output logic          orig_msg_port_wr_en_reg_next,
  output logic [1:0]    orig_msg_port_scp_way_reg_next,
  output logic [9:0]    orig_msg_port_wr_addr_reg_next,
  output logic          orig_l2_resp_msg_valid,
  output logic          orig_req_port_info_enabled,
  output logic [14:0]   orig_req_port_info_reply
);

  import minion_pkg::*;

  // -- Cast flat l2_resp to struct types --
  et_link_minion_rsp_info_t l2_resp_new;
  assign l2_resp_new = et_link_minion_rsp_info_t'(stim_l2_resp);

  // -- New module --
  core_msg_port_conf_t new_req_port_info_reply_s;
  logic [NrThreads-1:0][NrMsgPorts-1:0] new_msg_port_stall_s;

  intpipe_csr_msgs u_new (
    .clk_i                       (clk_i),
    .clk_wr_i                    (clk_i),
    .rst_ni                      (rst_ni),
    .cpu_mem_porthead_en_any     (stim_cpu_mem_porthead_en_any),
    .cpu_mem_portheadnb_en_any   (stim_cpu_mem_portheadnb_en_any),
    .cpu_mem_sen                 (stim_cpu_mem_sen),
    .mem_addr                    (stim_mem_addr),
    .mem_cmd                     (csr_cmd_e'(stim_mem_cmd)),
    .mem_valid_qual              (stim_mem_valid_qual),
    .mem_thread_id               (stim_mem_thread_id),
    .mem_wdata                   (stim_mem_wdata),
    .mem_rdata_next              (new_mem_rdata_next),
    .mpp                         (stim_mpp),
    .cpu_wb_sen                  (stim_cpu_wb_sen),
    .wb_thread_id                (stim_wb_thread_id),
    .wb_wdata                    (stim_wb_wdata),
    .msg_port_pget_block         (new_msg_port_pget_block),
    .msg_port_xcpt               (new_msg_port_xcpt),
    .msg_port_stall              (new_msg_port_stall_s),
    .l2_resp_valid               (stim_l2_resp_valid),
    .l2_resp_ready               (stim_l2_resp_ready),
    .l2_resp                     (l2_resp_new),
    .msg_port_wr_en_reg_next     (new_msg_port_wr_en_reg_next),
    .msg_port_scp_way_reg_next   (new_msg_port_scp_way_reg_next),
    .msg_port_wr_addr_reg_next   (new_msg_port_wr_addr_reg_next),
    .l2_resp_msg_valid           (new_l2_resp_msg_valid),
    .req_port_info_thread_id     (stim_req_port_info_thread_id),
    .req_port_info_port_id       (stim_req_port_info_port_id),
    .req_port_info_enabled       (new_req_port_info_enabled),
    .req_port_info_reply         (new_req_port_info_reply_s),
    .portctl                     (new_portctl)
  );

  assign new_msg_port_stall = new_msg_port_stall_s;
  assign new_req_port_info_reply = new_req_port_info_reply_s;

  // -- Original module --
  // The original uses the same packed types from soc.vh
  core_msg_port_conf_t orig_req_port_info_reply_s;
  logic [`CORE_NR_THREADS-1:0][`CORE_NR_MSG_PORTS-1:0] orig_msg_port_stall_s;

  intpipe_csr_msgs_orig u_orig (
    .clock                       (clk_i),
    .clock_wr                    (clk_i),
    .reset                       (~rst_ni),
    .cpu_mem_porthead_en_any     (stim_cpu_mem_porthead_en_any),
    .cpu_mem_portheadnb_en_any   (stim_cpu_mem_portheadnb_en_any),
    .cpu_mem_sen                 (stim_cpu_mem_sen),
    .mem_addr                    (stim_mem_addr),
    .mem_cmd                     (csr_cmd_t'(stim_mem_cmd)),
    .mem_valid_qual              (stim_mem_valid_qual),
    .mem_thread_id               (stim_mem_thread_id),
    .mem_wdata                   (stim_mem_wdata),
    .mem_rdata_next              (orig_mem_rdata_next),
    .mpp                         (stim_mpp),
    .cpu_wb_sen                  (stim_cpu_wb_sen),
    .wb_thread_id                (stim_wb_thread_id),
    .wb_wdata                    (stim_wb_wdata),
    .msg_port_pget_block         (orig_msg_port_pget_block),
    .msg_port_xcpt               (orig_msg_port_xcpt),
    .msg_port_stall              (orig_msg_port_stall_s),
    .l2_resp_valid               (stim_l2_resp_valid),
    .l2_resp_ready               (stim_l2_resp_ready),
    .l2_resp                     (stim_l2_resp),
    .msg_port_wr_en_reg_next     (orig_msg_port_wr_en_reg_next),
    .msg_port_scp_way_reg_next   (orig_msg_port_scp_way_reg_next),
    .msg_port_wr_addr_reg_next   (orig_msg_port_wr_addr_reg_next),
    .l2_resp_msg_valid           (orig_l2_resp_msg_valid),
    .req_port_info_thread_id     (stim_req_port_info_thread_id),
    .req_port_info_port_id       (stim_req_port_info_port_id),
    .req_port_info_enabled       (orig_req_port_info_enabled),
    .req_port_info_reply         (orig_req_port_info_reply_s),
    .portctl                     (orig_portctl)
  );

  assign orig_msg_port_stall = orig_msg_port_stall_s;
  assign orig_req_port_info_reply = orig_req_port_info_reply_s;

endmodule
