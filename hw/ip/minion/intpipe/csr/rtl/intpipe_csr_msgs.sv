// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// intpipe_csr_msgs — Message port control and configuration.
//
// Manages messaging port FIFOs, configuration registers, and
// PGET/PGETNB/enable/disable CSR operations.
//
// Replaces: intpipe_csr_msgs (etcore-soc)

module intpipe_csr_msgs
  import minion_pkg::*;
(
  input  logic                                                       clk_i,
  input  logic                                                       clk_wr_i,
  input  logic                                                       rst_ni,

  // core interface
  input  logic                                                       cpu_mem_porthead_en_any,
  input  logic                                                       cpu_mem_portheadnb_en_any,
  /* verilator lint_off UNOPTFLAT */  // Message-port CSR traffic remains in the original replay/backpressure cone and only appears cyclic after full flattening.
  input  logic [NrMsgPorts-1:0][NrThreads-1:0]                      cpu_mem_sen,
  /* verilator lint_on UNOPTFLAT */
  input  logic [$clog2(NrMsgPorts)-1:0]                              mem_addr,
  input  csr_cmd_e                                                   mem_cmd,
  input  logic                                                       mem_valid_qual,
  input  logic                                                       mem_thread_id,
  input  logic [3:0]                                                 mem_wdata,
  output logic [XregSize-1:0]                                        mem_rdata_next,
  input  logic [1:0]                                                 mpp,

  input  logic [NrMsgPorts-1:0][NrThreads-1:0]                      cpu_wb_sen,
  input  logic                                                       wb_thread_id,
  /* verilator lint_off UNUSEDSIGNAL */  // Message-port control reads only the protocol-relevant subset of the preserved packed inputs/counters.
  input  logic [XregSize-1:0]                                        wb_wdata,
  output logic                                                       msg_port_pget_block,
  output logic                                                       msg_port_xcpt,
  output logic [NrThreads-1:0][NrMsgPorts-1:0]                      msg_port_stall,

  // l2 interface
  input  logic                                                       l2_resp_valid,
  input  logic                                                       l2_resp_ready,
  input  et_link_minion_rsp_info_t                                   l2_resp,
  /* verilator lint_on UNUSEDSIGNAL */
  output logic                                                       msg_port_wr_en_reg_next,
  output logic [DcacheWayBits-1:0]                                   msg_port_scp_way_reg_next,
  output logic [DcacheLramAddrBits-1:0]                              msg_port_wr_addr_reg_next,
  output logic                                                       l2_resp_msg_valid,

  // interface to consult information from ports
  input  logic                                                       req_port_info_thread_id,
  input  logic [$clog2(NrMsgPorts)-1:0]                              req_port_info_port_id,
  output logic                                                       req_port_info_enabled,
  output core_msg_port_conf_t                                        req_port_info_reply,

  // ctrl regs
  output logic [XregSize-1:0]                                        portctl
);

  // signals for messaging ports
  logic [NrThreads-1:0][NrMsgPorts-1:0]                                  msg_port_enabled;
  core_msg_port_conf_t                                                    msg_port_conf [NrThreads-1:0][NrMsgPorts-1:0];
  // queue control
  logic [NrThreads-1:0][NrMsgPorts-1:0][CoreMsgPortMaxMsgsSz:0]          msg_port_wr_ptr;
  logic [NrThreads-1:0][NrMsgPorts-1:0][CoreMsgPortMaxMsgsSz:0]          msg_port_rd_ptr;
  logic [NrThreads-1:0][NrMsgPorts-1:0]                                  msg_port_buffer_valid;
  // write interface
  logic                                                                   msg_port_wr_thread;
  logic [$clog2(NrMsgPorts)-1:0]                                          msg_port_wr_id;
  /* verilator lint_off UNUSEDSIGNAL */  // The fill counter remains 2 bits to match the original datapath, though only one bit is consumed in this leaf.
  logic [1:0]                                                             msg_fill_cnt;
  /* verilator lint_on UNUSEDSIGNAL */
  logic                                                                   msg_fill_cnt_prev;
  logic                                                                   msg_fill_cnt_en;
  logic                                                                   msg_fill_cnt_last;

  // core interface
  logic [3:0]                                                             mem_wdata_c;
  logic [$clog2(NrMsgPorts)-1:0]                                          msg_port_mem_id;
  logic [$clog2(NrMsgPorts)-1:0]                                          msg_port_wb_id;
  logic                                                                   msg_port_pget;
  logic                                                                   msg_port_pget_valid;
  logic [NrMsgPorts-1:0][NrThreads-1:0]                                  msg_port_conf_en;
  logic [NrMsgPorts-1:0][NrThreads-1:0]                                  msg_port_conf_en_next;

  logic                                                                   cpu_mem_heads_en_any;
  logic                                                                   cpu_mem_sen_any;

  logic [NrThreads-1:0][NrMsgPorts-1:0]                                  msg_port_stall_next;
  logic                                                                   msg_port_xcpt_next;

  // dcache and port enable control
  logic [6 + CoreMsgPortMaxMsgsSz - 1:0]                                  msg_port_wr_offset;
  logic                                                                   msg_port_pget_next;
  logic                                                                   msg_port_pget_valid_next;

  // oob bit storage
  logic [NrThreads-1:0][NrMsgPorts-1:0]                                  msg_port_oob;
  logic                                                                   msg_port_oob_next;
  logic [NrThreads-1:0][NrMsgPorts-1:0]                                  msg_port_oob_push;
  logic [NrThreads-1:0][NrMsgPorts-1:0]                                  msg_port_oob_pop;
  logic [NrThreads-1:0][NrMsgPorts-1:0]                                  msg_port_oob_full;
  logic                                                                   msg_port_oob_rdata;

  // Write enable for ports
  logic                                                                   msg_port_wr_en;

  // read from PORTCTRL
  assign portctl = {{
                     (XregSize - (
                       DcacheWayBits + 4 + DcacheSetBits + 1 + 3 +
                       CoreMsgPortMaxMsgsSz + $bits(core_msg_port_logsize_e) +
                       1 + 2 + 1 + 1
                     )){1'b0}},
                     msg_port_conf[wb_thread_id][mem_addr].scp_way,
                     4'b0000,
                     msg_port_conf[wb_thread_id][mem_addr].scp_set,
                     1'b1,
                     3'b000,
                     msg_port_conf[wb_thread_id][mem_addr].max_msgs,
                     msg_port_conf[wb_thread_id][mem_addr].logsize,
                     msg_port_conf[wb_thread_id][mem_addr].umode,
                     2'b00,
                     msg_port_conf[wb_thread_id][mem_addr].enable_oob,
                     msg_port_enabled[wb_thread_id][mem_addr]};

  // ── Core interface ──────────────────────────────────────────────────
  always_comb begin
    cpu_mem_sen_any = cpu_mem_sen != '0;
    cpu_mem_heads_en_any = cpu_mem_porthead_en_any || cpu_mem_portheadnb_en_any;
    mem_wdata_c = mem_cmd == CsrCmdC ? '0 : mem_wdata;
  end

  // compute next read data
  always_comb begin
    msg_port_pget_next = 1'b0;
    mem_rdata_next = '1;
    msg_port_xcpt_next = cpu_mem_heads_en_any;
    msg_port_conf_en_next = |cpu_wb_sen ? '0 : msg_port_conf_en;
    msg_port_pget_valid_next = msg_port_pget_valid;
    msg_port_oob_rdata = 1'b0;

    casez (mem_wdata_c)
      4'b??00, 4'b??01: begin // PGET, PGETNB
        msg_port_xcpt_next = (!msg_port_conf[mem_thread_id][msg_port_mem_id].umode && (cpu_mem_porthead_en_any || cpu_mem_portheadnb_en_any) && mpp == PrvU) ||
                              !msg_port_enabled[mem_thread_id][msg_port_mem_id] && (cpu_mem_porthead_en_any || cpu_mem_portheadnb_en_any);

        msg_port_pget_next = !msg_port_xcpt_next && cpu_mem_heads_en_any;
        msg_port_pget_valid_next = msg_port_buffer_valid[mem_thread_id][msg_port_mem_id];
        msg_port_oob_rdata = msg_port_conf[mem_thread_id][msg_port_mem_id].enable_oob ? msg_port_oob[mem_thread_id][msg_port_mem_id] : 1'b0;
        if (msg_port_buffer_valid[mem_thread_id][msg_port_mem_id]) begin
          case (msg_port_conf[mem_thread_id][msg_port_mem_id].logsize)
            MsgPort32:  mem_rdata_next = {{(XregSize-CoreMsgPortMaxMsgsSz-2){1'b0}}, msg_port_rd_ptr[mem_thread_id][msg_port_mem_id][CoreMsgPortMaxMsgsSz-1:0], 1'b0, msg_port_oob_rdata};
            MsgPort64:  mem_rdata_next = {{(XregSize-CoreMsgPortMaxMsgsSz-3){1'b0}}, msg_port_rd_ptr[mem_thread_id][msg_port_mem_id][CoreMsgPortMaxMsgsSz-1:0], 2'b0, msg_port_oob_rdata};
            MsgPort128: mem_rdata_next = {{(XregSize-CoreMsgPortMaxMsgsSz-4){1'b0}}, msg_port_rd_ptr[mem_thread_id][msg_port_mem_id][CoreMsgPortMaxMsgsSz-1:0], 3'b0, msg_port_oob_rdata};
            default:    mem_rdata_next = {{(XregSize-CoreMsgPortMaxMsgsSz-5){1'b0}}, msg_port_rd_ptr[mem_thread_id][msg_port_mem_id][CoreMsgPortMaxMsgsSz-1:0], 4'b0, msg_port_oob_rdata};
          endcase
        end else begin
          mem_rdata_next = '1;
        end
      end

      4'b0111, 4'b?011: begin // ENABLE_R, DISABLE
        mem_rdata_next = '0;
        if (cpu_mem_heads_en_any || (cpu_mem_sen_any && mpp == PrvU))
          msg_port_xcpt_next = 1'b1;
        else begin
          msg_port_conf_en_next = cpu_mem_sen;
          msg_port_xcpt_next = 1'b0;
        end
      end

      default: begin
      end
    endcase
  end

  always_comb begin
    msg_port_stall_next = msg_port_stall;
    if (msg_port_pget_next)
      msg_port_stall_next[mem_thread_id][msg_port_mem_id] = cpu_mem_porthead_en_any;
    msg_port_stall_next &= ~msg_port_buffer_valid;
  end

  // Pipeline registers
  // msg_port_xcpt
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)       msg_port_xcpt <= 1'b0;
    else               msg_port_xcpt <= msg_port_xcpt_next && cpu_mem_heads_en_any;
  end

  // msg_port_pget
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)       msg_port_pget <= 1'b0;
    else               msg_port_pget <= msg_port_pget_next && cpu_mem_heads_en_any;
  end

  // msg_port_pget_valid
  always_ff @(posedge clk_i) begin
    if (cpu_mem_heads_en_any)
      msg_port_pget_valid <= msg_port_pget_valid_next;
  end

  // msg_port_conf_en
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)
      msg_port_conf_en <= '0;
    else if ((|msg_port_conf_en) || (|msg_port_conf_en_next))
      msg_port_conf_en <= msg_port_conf_en_next;
  end

  // msg_port_stall
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)       msg_port_stall <= '0;
    else               msg_port_stall <= msg_port_stall_next;
  end

  // msg_port_pget_block
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)       msg_port_pget_block <= 1'b0;
    else               msg_port_pget_block <= cpu_mem_porthead_en_any && !msg_port_buffer_valid[mem_thread_id][msg_port_mem_id];
  end

  // ── Messaging ports ─────────────────────────────────────────────────

  // msg_port_wb_id pipeline
  always_ff @(posedge clk_i) begin
    if (mem_valid_qual)
      msg_port_wb_id <= msg_port_mem_id;
  end

  assign msg_port_mem_id = mem_addr;

  // legalize_logsize function
  function automatic core_msg_port_logsize_e legalize_logsize(input [2:0] size);
    case (size)
      3'd0, 3'd1:       return MsgPort32;
      3'd6, 3'd7:       return MsgPort256;
      default:           return core_msg_port_logsize_e'(size);
    endcase
  endfunction

  // Configuration write (clock_wr domain)
  always_ff @(posedge clk_wr_i) begin
    for (int t = 0; t < NrThreads; t++) begin
      for (int p = 0; p < NrMsgPorts; p++) begin
        if (cpu_wb_sen[p][t]) begin
          msg_port_enabled[t][p] <= wb_wdata[0];
          msg_port_conf[t][p] <= '{
            umode:      wb_wdata[4],
            logsize:    legalize_logsize(wb_wdata[7:5]),
            max_msgs:   wb_wdata[11:8],
            scp_set:    wb_wdata[16 +: DcacheSetBits],
            scp_way:    wb_wdata[24 +: DcacheWayBits],
            enable_oob: wb_wdata[1]
          };
        end
      end
    end

    if (!rst_ni) begin
      msg_port_enabled <= '0;
    end
  end

  // inc_wrap_rbox_ptrs function
  function automatic logic [CoreMsgPortMaxMsgsSz:0] inc_wrap_rbox_ptrs(
    input logic [CoreMsgPortMaxMsgsSz:0]   ptr,
    input logic [CoreMsgPortMaxMsgsSz-1:0] max_ptr
  );
    logic [CoreMsgPortMaxMsgsSz-1:0] lsb;
    logic                            msb;
    lsb = ptr[CoreMsgPortMaxMsgsSz-1:0];
    msb = ptr[CoreMsgPortMaxMsgsSz];
    if (lsb == max_ptr) begin
      lsb = '0;
      msb = !msb;
    end else begin
      lsb = lsb + 1'b1;
    end
    return {msb, lsb};
  endfunction

  always_comb begin
    msg_port_wr_thread = l2_resp.dest;
    msg_port_wr_id     = l2_resp.id[EtMsgPortIdStart +: EtMsgPortIdSize];
    l2_resp_msg_valid  = l2_resp_valid && (l2_resp.opcode == EtLinkRspMsgRcvData) && (et_link_min_msg_id_e'(l2_resp.id) < EtLinkMinMsgIdReduceReady);
    msg_fill_cnt_en    = l2_resp_msg_valid && l2_resp_ready;
    msg_fill_cnt_last  = 1'b1;
    msg_port_oob_next  = l2_resp.id[EtMsgValidBit];

    for (int i = 0; i < NrThreads; i++)
      for (int p = 0; p < NrMsgPorts; p++)
        msg_port_buffer_valid[i][p] = msg_port_wr_ptr[i][p] != msg_port_rd_ptr[i][p];
  end

  assign msg_fill_cnt = '0;

  // msg port queue pointers and dcache config signals
  logic [CoreMsgPortMaxMsgsSz:0] msg_port_wr_ptr_next;
  assign msg_port_wr_ptr_next = inc_wrap_rbox_ptrs(
    msg_port_wr_ptr[msg_port_wr_thread][msg_port_wr_id],
    msg_port_conf[msg_port_wr_thread][msg_port_wr_id].max_msgs
  );
  assign msg_port_wr_en = msg_port_enabled[msg_port_wr_thread][msg_port_wr_id] &&
                           (msg_port_wr_ptr[msg_port_wr_thread][msg_port_wr_id] ^
                            msg_port_rd_ptr[msg_port_wr_thread][msg_port_wr_id]) !=
                           {1'b1, {(CoreMsgPortMaxMsgsSz){1'b0}}};
  assign msg_port_wr_en_reg_next = msg_port_wr_en;

  for (genvar gen_t = 0; gen_t < NrThreads; gen_t++) begin : gen_msg_ptrs_thread
    for (genvar gen_p = 0; gen_p < NrMsgPorts; gen_p++) begin : gen_msg_ptrs_port
      logic [CoreMsgPortMaxMsgsSz:0] rd_ptr_next;
      logic                          wr_ptr_up;
      logic                          rd_ptr_up;

      always_comb begin
        rd_ptr_next = inc_wrap_rbox_ptrs(msg_port_rd_ptr[gen_t][gen_p], msg_port_conf[gen_t][gen_p].max_msgs);

        rd_ptr_up = !msg_port_stall[gen_t][gen_p] && msg_port_pget &&
                    msg_port_pget_valid && gen_t == wb_thread_id && gen_p == msg_port_wb_id &&
                    msg_port_buffer_valid[gen_t][gen_p];

        wr_ptr_up = msg_fill_cnt_en && msg_fill_cnt_last &&
                    msg_port_wr_en &&
                    gen_p == msg_port_wr_id && gen_t == msg_port_wr_thread;

        msg_port_oob_push[gen_t][gen_p] = wr_ptr_up && !msg_port_oob_full[gen_t][gen_p] && msg_port_conf[gen_t][gen_p].enable_oob;
        msg_port_oob_pop[gen_t][gen_p]  = rd_ptr_up && msg_port_conf[gen_t][gen_p].enable_oob;
      end

      // rd_ptr
      always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni)
          msg_port_rd_ptr[gen_t][gen_p] <= '0;
        else if (msg_port_conf_en[gen_p][gen_t])
          msg_port_rd_ptr[gen_t][gen_p] <= '0;
        else if (rd_ptr_up)
          msg_port_rd_ptr[gen_t][gen_p] <= rd_ptr_next;
      end

      // wr_ptr
      always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni)
          msg_port_wr_ptr[gen_t][gen_p] <= '0;
        else if (msg_port_conf_en[gen_p][gen_t])
          msg_port_wr_ptr[gen_t][gen_p] <= '0;
        else if (wr_ptr_up)
          msg_port_wr_ptr[gen_t][gen_p] <= msg_port_wr_ptr_next;
      end

      // OOB storage. This local FIFO mirrors the original gen_fifo reset and
      // pointer behavior for the one-bit message-port sideband.
      logic [(2**CoreMsgPortMaxMsgsSz)-2:0] oob_mem;
      logic [$clog2((2**CoreMsgPortMaxMsgsSz)-1)-1:0] oob_wr_ptr, oob_rd_ptr;

      always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
          oob_wr_ptr <= '0;
          oob_rd_ptr <= '0;
        end else if (msg_port_conf_en[gen_p][gen_t]) begin
          oob_wr_ptr <= '0;
          oob_rd_ptr <= '0;
        end else begin
          if (msg_port_oob_push[gen_t][gen_p]) begin
            oob_mem[oob_wr_ptr] <= msg_port_oob_next;
            oob_wr_ptr <= oob_wr_ptr + 1'b1;
          end
          if (msg_port_oob_pop[gen_t][gen_p]) begin
            oob_rd_ptr <= oob_rd_ptr + 1'b1;
          end
        end
      end

      assign msg_port_oob[gen_t][gen_p] = oob_mem[oob_rd_ptr];
      assign msg_port_oob_full[gen_t][gen_p] = (oob_wr_ptr - oob_rd_ptr) >= ((2**CoreMsgPortMaxMsgsSz) - 1);
    end
  end

  // signals for dcache configuration
  always_comb begin
    msg_port_scp_way_reg_next = msg_port_conf[msg_port_wr_thread][msg_port_wr_id].scp_way;
    case (msg_port_conf[msg_port_wr_thread][msg_port_wr_id].logsize)
      MsgPort32:  msg_port_wr_offset = {4'h0, msg_port_wr_ptr[msg_port_wr_thread][msg_port_wr_id][CoreMsgPortMaxMsgsSz-1:0], 2'b0};
      MsgPort64:  msg_port_wr_offset = {3'h0, msg_port_wr_ptr[msg_port_wr_thread][msg_port_wr_id][CoreMsgPortMaxMsgsSz-1:0], 3'b0};
      MsgPort128: msg_port_wr_offset = {2'h0, msg_port_wr_ptr[msg_port_wr_thread][msg_port_wr_id][CoreMsgPortMaxMsgsSz-1:0], 4'b0};
      default:    msg_port_wr_offset = {msg_port_wr_ptr[msg_port_wr_thread][msg_port_wr_id][CoreMsgPortMaxMsgsSz-1:0], 6'b0};
    endcase

    msg_fill_cnt_prev = msg_fill_cnt_en ^ msg_fill_cnt[0];
    msg_port_wr_addr_reg_next = {msg_port_conf[msg_port_wr_thread][msg_port_wr_id].scp_set, {(DcacheLineAddrLsb){1'b0}}} +
                                 msg_port_wr_offset +
                                 {{CoreMsgPortMaxMsgsSz{1'b0}}, msg_fill_cnt_prev, 5'b0};
  end

  // return port information
  always_comb begin
    req_port_info_reply   = msg_port_conf[req_port_info_thread_id][req_port_info_port_id];
    req_port_info_enabled = msg_port_enabled[req_port_info_thread_id][req_port_info_port_id];
  end

endmodule
