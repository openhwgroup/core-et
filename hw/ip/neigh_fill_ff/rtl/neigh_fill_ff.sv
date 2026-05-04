// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_fill_ff
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                               clk_i,
  input  logic                               rst_ni,

  input  etlink_pkg::rsp_t                   fill_fifo_resp_i,
  input  logic                               fill_fifo_valid_i,
  output logic                               fill_fifo_ready_o,

  /* verilator lint_off UNUSEDSIGNAL */  // This shell forwards only id/dest/opcode/size/qwen; the remaining tensor-store response fields are intentionally ignored here.
  input  et_link_minion_rsp_info_t           tstore_min_resp_i,
  /* verilator lint_on UNUSEDSIGNAL */
  input  logic                               tstore_min_resp_valid_i,
  output logic                               tstore_min_resp_ready_o,

  /* verilator lint_off UNUSEDSIGNAL */  // Neighborhood shell only consumes the encoded thread id plus payload metadata.
  input  etlink_pkg::neigh_rsp_t             local_message_info_i,
  /* verilator lint_on UNUSEDSIGNAL */
  input  logic                               local_message_valid_i,
  output logic                               local_message_ready_o,

  /* verilator lint_off UNUSEDSIGNAL */  // Neighborhood shell only consumes the encoded thread id plus payload metadata.
  input  etlink_pkg::neigh_rsp_t             tbox_min_rsp_info_i,
  /* verilator lint_on UNUSEDSIGNAL */
  input  logic                               tbox_min_rsp_valid_i,
  output logic                               tbox_min_rsp_ready_o,

  output et_link_minion_rsp_info_t           l2_resp_o,
  output logic                               l2_resp_valid_o,
  input  logic                               l2_resp_ready_i,

  output logic                               tstore_min_ack_o,
  output fill_ff_dbg_sm_t                    dbg_sm_signals_o
);

  localparam int unsigned LineDataSize = minion_dcache_pkg::DcacheLineSize;
  localparam int unsigned BlockSize = minion_dcache_pkg::CoreL2BlockExtSize;
  localparam int unsigned Passes = minion_dcache_pkg::CoreL2BlockExtPasses;
  localparam int unsigned PassCountWidth = (Passes > 1) ? $clog2(Passes) : 1;
  /* verilator lint_off WIDTHTRUNC */  // Pass-count constants are intentionally truncated to the narrow sequencing counter width.
  localparam logic [PassCountWidth-1:0] LastFillLine = Passes - 1;
  /* verilator lint_on WIDTHTRUNC */

  logic                                  fill_write_l2;
  logic                                  fill_write_tstore;
  logic                                  fill_write_fln;
  logic                                  fill_write_tbox;
  logic                                  fill_write;
  et_link_minion_nodata_rsp_info_t       fill_info_mux;
  logic [LineDataSize-1:0]               fill_data_mux;
  logic                                  fill_pop;
  logic                                  fill_full_next;
  et_link_minion_nodata_rsp_info_t       fill_rsp_info;
  logic [LineDataSize-1:0]               fill_rsp_data_flat;
  logic                                  fill_rsp_valid;
  logic                                  fill_rsp_ready;
  logic                                  fill_out_accepted;
  logic [PassCountWidth-1:0]             fill_out_count_q;
  logic [PassCountWidth-1:0]             fill_out_count_d;
  et_link_minion_rsp_info_t              l2_resp_q;
  et_link_minion_rsp_info_t              l2_resp_d;
  logic                                  l2_resp_valid_q;
  logic                                  l2_resp_valid_d;
  logic                                  tstore_min_ack_d;

  logic                                  local_message_ready_d;
  logic                                  tbox_min_rsp_ready_d;
  logic                                  fill_ff_ready_for_no_msg_q;
  logic                                  fill_ff_ready_for_no_msg_d;

  /* verilator lint_off UNUSEDSIGNAL */  // Only thread_id is consumed; the higher dest bits are carried for exact type fidelity.
  et_link_neigh_msg_dest_t               fill_fifo_resp_msg_dest;
  et_link_neigh_msg_dest_t               local_message_info_msg_dest;
  et_link_neigh_msg_dest_t               tbox_min_rsp_info_msg_dest;
  /* verilator lint_on UNUSEDSIGNAL */

  logic                                  fill_info_fifo_full_unused;
  logic                                  fill_info_fifo_valid_next_unused;
  logic                                  fill_data_fifo_full_unused;
  logic                                  fill_data_fifo_full_next_unused;
  logic                                  fill_data_fifo_valid_unused;
  logic                                  fill_data_fifo_valid_next_unused;

  assign fill_fifo_ready_o = fill_ff_ready_for_no_msg_q;
  assign tstore_min_resp_ready_o = fill_ff_ready_for_no_msg_q && !fill_fifo_valid_i;

  assign fill_write_l2 = fill_fifo_valid_i && fill_fifo_ready_o;
  assign fill_write_tstore = tstore_min_resp_valid_i && tstore_min_resp_ready_o;
  assign fill_write_fln = local_message_valid_i && local_message_ready_o;
  assign fill_write_tbox = tbox_min_rsp_valid_i && tbox_min_rsp_ready_o;
  assign fill_write = fill_write_l2 || fill_write_tstore || fill_write_fln || fill_write_tbox;

  always_comb begin
    local_message_ready_d = !fill_full_next && local_message_valid_i;
    tbox_min_rsp_ready_d = !fill_full_next && !local_message_valid_i && tbox_min_rsp_valid_i;
    fill_ff_ready_for_no_msg_d = !fill_full_next && !local_message_valid_i && !tbox_min_rsp_valid_i;
  end

  always_comb begin
    fill_fifo_resp_msg_dest = et_link_neigh_msg_dest_t'(fill_fifo_resp_i.dest);
    local_message_info_msg_dest = '0;
    tbox_min_rsp_info_msg_dest = '0;

    fill_info_mux = '0;
    fill_info_mux.id = fill_fifo_resp_i.id;
    fill_info_mux.dest = fill_fifo_resp_msg_dest.thread_id;
    fill_info_mux.wdata = fill_fifo_resp_i.wdata;
    fill_info_mux.opcode = et_link_rsp_opcode_e'(fill_fifo_resp_i.opcode);
    fill_info_mux.size = et_link_size_e'(fill_fifo_resp_i.size);
    fill_info_mux.qwen = fill_fifo_resp_i.qwen;

    fill_data_mux = '0;
    if (|fill_fifo_resp_i.qwen[1:0]) begin
      fill_data_mux[BlockSize-1:0] = fill_fifo_resp_i.data[BlockSize-1:0];
    end else begin
      fill_data_mux[BlockSize-1:0] = fill_fifo_resp_i.data[LineDataSize-1:BlockSize];
    end
    fill_data_mux[LineDataSize-1:BlockSize] = fill_fifo_resp_i.data[LineDataSize-1:BlockSize];

    if (fill_write_fln) begin
      local_message_info_msg_dest = et_link_neigh_msg_dest_t'(local_message_info_i.dest);
      fill_info_mux.id = local_message_info_i.id;
      fill_info_mux.dest = local_message_info_msg_dest.thread_id;
      fill_info_mux.wdata = 1'b1;
      fill_info_mux.opcode = EtLinkRspMsgRcvData;
      fill_info_mux.size = et_link_size_e'(local_message_info_i.size);
      fill_info_mux.qwen = local_message_info_i.qwen;
      fill_data_mux[etlink_pkg::NeighDataSize-1:0] = local_message_info_i.data;
    end else if (fill_write_tbox) begin
      tbox_min_rsp_info_msg_dest = et_link_neigh_msg_dest_t'(tbox_min_rsp_info_i.dest);
      fill_info_mux.id = tbox_min_rsp_info_i.id;
      fill_info_mux.dest = tbox_min_rsp_info_msg_dest.thread_id;
      fill_info_mux.wdata = 1'b1;
      fill_info_mux.opcode = EtLinkRspMsgRcvData;
      fill_info_mux.size = et_link_size_e'(tbox_min_rsp_info_i.size);
      fill_info_mux.qwen = tbox_min_rsp_info_i.qwen;
      fill_data_mux[etlink_pkg::NeighDataSize-1:0] = tbox_min_rsp_info_i.data;
    end else if (fill_write_tstore) begin
      fill_info_mux.id = tstore_min_resp_i.id;
      fill_info_mux.dest = tstore_min_resp_i.dest;
      fill_info_mux.wdata = tstore_min_resp_i.wdata;
      fill_info_mux.opcode = tstore_min_resp_i.opcode;
      fill_info_mux.size = tstore_min_resp_i.size;
      fill_info_mux.qwen = tstore_min_resp_i.qwen;
    end
  end

  prim_fifo #(
    .Width($bits(et_link_minion_nodata_rsp_info_t)),
    .Depth(2),
    .UseLatches(1'b0)
  ) u_fill_ff_info_fifo (
    .clk_i,
    .rst_ni,
    .push_early_i(1'b0),
    .push_i      (fill_write),
    .push_data_i (fill_info_mux),
    .full_o      (fill_info_fifo_full_unused),
    .full_next_o (fill_full_next),
    .pop_i       (fill_pop),
    .pop_data_o  (fill_rsp_info),
    .valid_o     (fill_rsp_valid),
    .valid_next_o(fill_info_fifo_valid_next_unused)
  );

  prim_fifo #(
    .Width(LineDataSize),
    .Depth(2),
    .UseLatches(1'b0)
  ) u_fill_ff_data_fifo (
    .clk_i,
    .rst_ni,
    .push_early_i(1'b0),
    .push_i      (fill_write && fill_info_mux.wdata),
    .push_data_i (fill_data_mux),
    .full_o      (fill_data_fifo_full_unused),
    .full_next_o (fill_data_fifo_full_next_unused),
    .pop_i       (fill_pop && fill_rsp_info.wdata),
    .pop_data_o  (fill_rsp_data_flat),
    .valid_o     (fill_data_fifo_valid_unused),
    .valid_next_o(fill_data_fifo_valid_next_unused)
  );

  always_comb begin
    fill_out_count_d = fill_out_count_q + {{(PassCountWidth-1){1'b0}}, 1'b1};
    if (fill_pop) begin
      fill_out_count_d = '0;
    end
  end

  always_comb begin
    if (fill_out_count_q == '0) begin
      l2_resp_d.data = fill_rsp_data_flat[BlockSize-1:0];
    end else begin
      l2_resp_d.data = fill_rsp_data_flat[LineDataSize-1:BlockSize];
    end
    l2_resp_d.id = fill_rsp_info.id;
    l2_resp_d.dest = fill_rsp_info.dest;
    l2_resp_d.wdata = fill_rsp_info.wdata;
    l2_resp_d.opcode = fill_rsp_info.opcode;
    l2_resp_d.size = fill_rsp_info.size;
    l2_resp_d.qwen = fill_rsp_info.qwen;
  end

  assign fill_rsp_ready = l2_resp_ready_i || !l2_resp_valid_q;
  assign fill_out_accepted = fill_rsp_valid && fill_rsp_ready;
  assign fill_pop = fill_out_accepted
                  && (fill_out_count_q == last_fill_counter(fill_rsp_info.wdata, fill_rsp_info.size));
  assign l2_resp_valid_d = fill_rsp_valid;

  assign tstore_min_ack_d = fill_out_accepted
                         && (fill_rsp_info.opcode == EtLinkRspAck)
                         && (fill_rsp_info.id[$bits(et_link_core_miss_id_e)-1:0]
                             == EtLinkCoreMissIdTensorStoreCoop);

  assign l2_resp_o = l2_resp_q;
  assign l2_resp_valid_o = l2_resp_valid_q;

  assign dbg_sm_signals_o = '{
    local_message_ready: local_message_ready_o,
    tbox_min_rsp_ready: tbox_min_rsp_ready_o,
    fill_ff_ready_for_no_msg: fill_ff_ready_for_no_msg_q
  };

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      local_message_ready_o <= 1'b0;
      tbox_min_rsp_ready_o <= 1'b0;
      fill_ff_ready_for_no_msg_q <= 1'b0;
      fill_out_count_q <= '0;
      l2_resp_q <= '0;
      l2_resp_valid_q <= 1'b0;
      tstore_min_ack_o <= 1'b0;
    end else begin
      local_message_ready_o <= local_message_ready_d;
      tbox_min_rsp_ready_o <= tbox_min_rsp_ready_d;
      fill_ff_ready_for_no_msg_q <= fill_ff_ready_for_no_msg_d;
      tstore_min_ack_o <= tstore_min_ack_d;

      if (fill_out_accepted) begin
        fill_out_count_q <= fill_out_count_d;
        l2_resp_q <= l2_resp_d;
      end

      if (fill_rsp_ready) begin
        l2_resp_valid_q <= l2_resp_valid_d;
      end
    end
  end

  function automatic logic [PassCountWidth-1:0] last_fill_counter(
      input logic       fill_wdata_i,
      input logic [2:0] fill_size_i
  );
    begin
      if (fill_wdata_i) begin
        unique case (fill_size_i)
          EtLinkLine: last_fill_counter = LastFillLine;
          default:    last_fill_counter = '0;
        endcase
      end else begin
        last_fill_counter = '0;
      end
    end
  endfunction

endmodule
