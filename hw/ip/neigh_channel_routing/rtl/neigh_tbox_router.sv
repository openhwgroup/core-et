// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_tbox_router #(
  parameter int unsigned MinTboxReqFifoDepth = neigh_pkg::NeighMinTboxReqFifoSize,
  parameter int unsigned TboxNeighReqFifoDepth = neigh_pkg::TboxNeighReqFifoSize,
  parameter int unsigned ScTboxRspFifoDepth = neigh_pkg::NeighScTboxRspFifoSize
) (
  input  logic                    clk_i,
  input  logic                    rst_ni,

  input  etlink_pkg::neigh_req_t  min_tbox_req_info_i,
  input  logic                    min_tbox_req_valid_early_i,
  input  logic                    min_tbox_req_valid_i,
  output logic                    min_tbox_req_ready_o,

  output etlink_pkg::neigh_rsp_t  tbox_min_rsp_info_o,
  output logic                    tbox_min_rsp_valid_o,
  input  logic                    tbox_min_rsp_ready_i,

  input  logic                    tbox_sc_req_disable_next_i,
  output etlink_pkg::req_nodata_t tbox_sc_req_info_o,
  output logic                    tbox_sc_req_valid_o,
  input  logic                    tbox_sc_req_ready_i,

  input  etlink_pkg::rsp_t        sc_tbox_rsp_info_i,
  input  logic                    sc_tbox_rsp_valid_i,
  output logic                    sc_tbox_rsp_ready_o,

  input  etlink_pkg::neigh_req_t  tbox_neigh_req_info_i,
  input  logic                    tbox_neigh_req_valid_i,
  output logic                    tbox_neigh_req_ready_o,

  output etlink_pkg::rsp_t        tbox_neigh_rsp_info_o,
  output logic                    tbox_neigh_rsp_valid_o,
  input  logic                    tbox_neigh_rsp_ready_i
);

  localparam int unsigned NeighReqWidth = $bits(etlink_pkg::neigh_req_t);
  localparam int unsigned RspWidth      = $bits(etlink_pkg::rsp_t);

  logic                    min_tbox_req_push_early;
  logic                    min_tbox_req_push;
  logic                    min_tbox_req_fifo_full;
  logic                    min_tbox_req_pop_valid;
  logic                    min_tbox_req_pop_valid_next;
  logic                    min_tbox_req_pop;
  logic                    min_req_fifo_full_next_unused;
  /* verilator lint_off UNUSEDSIGNAL */  // Converted minion->TBOX messages only consume address/data/size/qwen, matching the original router.
  etlink_pkg::neigh_req_t  min_tbox_req_pop_data;
  /* verilator lint_on UNUSEDSIGNAL */
  etlink_pkg::rsp_t        min_tbox_rsp_info;
  logic                    min_tbox_rsp_valid;
  logic                    min_tbox_rsp_valid_next;
  logic                    min_tbox_rsp_ready;

  assign min_tbox_req_push_early = min_tbox_req_valid_early_i;
  assign min_tbox_req_push = min_tbox_req_valid_i && !min_tbox_req_fifo_full;
  assign min_tbox_req_ready_o = !min_tbox_req_fifo_full;

  prim_fifo #(
    .Width     (NeighReqWidth),
    .Depth     (MinTboxReqFifoDepth),
    .UseLatches(1'b1)
  ) u_min_tbox_req_fifo (
    .clk_i         (clk_i),
    .rst_ni        (rst_ni),
    .push_early_i  (min_tbox_req_push_early),
    .push_i        (min_tbox_req_push),
    .push_data_i   (min_tbox_req_info_i),
    .full_o        (min_tbox_req_fifo_full),
    .full_next_o   (min_req_fifo_full_next_unused),
    .pop_i         (min_tbox_req_pop),
    .pop_data_o    (min_tbox_req_pop_data),
    .valid_o       (min_tbox_req_pop_valid),
    .valid_next_o  (min_tbox_req_pop_valid_next)
  );

  always_comb begin
    min_tbox_rsp_info = '0;
    min_tbox_rsp_info.id = min_tbox_req_pop_data.address[3 +: 8];
    min_tbox_rsp_info.dest = '0;
    min_tbox_rsp_info.wdata = 1'b1;
    min_tbox_rsp_info.opcode = etlink_pkg::RspMsgRcvData;
    min_tbox_rsp_info.data = {{(etlink_pkg::DataSize-etlink_pkg::NeighDataSize){1'b0}},
                              min_tbox_req_pop_data.data};
    min_tbox_rsp_info.size = min_tbox_req_pop_data.size;
    min_tbox_rsp_info.qwen = min_tbox_req_pop_data.qwen;
  end

  assign min_tbox_rsp_valid = min_tbox_req_pop_valid;
  assign min_tbox_rsp_valid_next = min_tbox_req_pop_valid_next;
  assign min_tbox_req_pop = min_tbox_rsp_ready;

  logic                    tbox_neigh_req_push;
  logic                    tbox_neigh_req_fifo_full;
  etlink_pkg::neigh_req_t  tbox_neigh_req_pop_data;
  logic                    tbox_neigh_req_pop_valid;
  logic                    tbox_neigh_req_pop;
  logic                    tbox_neigh_req_goes_to_sc;
  logic                    tbox_sc_req_valid_d;
  logic                    tbox_min_rsp_valid_d;
  logic                    tbox_req_fifo_full_next_unused;
  logic                    tbox_req_valid_next_unused;

  assign tbox_neigh_req_push = tbox_neigh_req_valid_i && !tbox_neigh_req_fifo_full;
  assign tbox_neigh_req_ready_o = !tbox_neigh_req_fifo_full;

  prim_fifo #(
    .Width     (NeighReqWidth),
    .Depth     (TboxNeighReqFifoDepth),
    .UseLatches(1'b0)
  ) u_tbox_neigh_req_fifo (
    .clk_i         (clk_i),
    .rst_ni        (rst_ni),
    .push_early_i  (1'b0),
    .push_i        (tbox_neigh_req_push),
    .push_data_i   (tbox_neigh_req_info_i),
    .full_o        (tbox_neigh_req_fifo_full),
    .full_next_o   (tbox_req_fifo_full_next_unused),
    .pop_i         (tbox_neigh_req_pop),
    .pop_data_o    (tbox_neigh_req_pop_data),
    .valid_o       (tbox_neigh_req_pop_valid),
    .valid_next_o  (tbox_req_valid_next_unused)
  );

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      tbox_sc_req_valid_o <= 1'b0;
      tbox_min_rsp_valid_o <= 1'b0;
    end else begin
      tbox_sc_req_valid_o <= tbox_sc_req_valid_d;
      tbox_min_rsp_valid_o <= tbox_min_rsp_valid_d;
    end
  end

  assign tbox_neigh_req_goes_to_sc = tbox_neigh_req_pop_data.opcode != etlink_pkg::ReqMsgSendData;
  assign tbox_neigh_req_pop = tbox_neigh_req_goes_to_sc ? (tbox_sc_req_valid_o && tbox_sc_req_ready_i)
                                                        : (tbox_min_rsp_valid_o && tbox_min_rsp_ready_i);

  always_comb begin
    tbox_sc_req_valid_d = tbox_sc_req_valid_o;
    tbox_min_rsp_valid_d = tbox_min_rsp_valid_o;

    if (!tbox_sc_req_valid_o) begin
      if (tbox_neigh_req_pop_valid && tbox_neigh_req_goes_to_sc && !tbox_sc_req_disable_next_i) begin
        tbox_sc_req_valid_d = 1'b1;
      end
    end else begin
      if (tbox_sc_req_ready_i || tbox_sc_req_disable_next_i) begin
        tbox_sc_req_valid_d = 1'b0;
      end
    end

    if (!tbox_min_rsp_valid_o) begin
      if (tbox_neigh_req_pop_valid && !tbox_neigh_req_goes_to_sc) begin
        tbox_min_rsp_valid_d = 1'b1;
      end
    end else begin
      if (tbox_min_rsp_ready_i) begin
        tbox_min_rsp_valid_d = 1'b0;
      end
    end
  end

  always_comb begin
    tbox_sc_req_info_o = '0;
    tbox_sc_req_info_o.id = tbox_neigh_req_pop_data.id;
    tbox_sc_req_info_o.source = tbox_neigh_req_pop_data.source;
    tbox_sc_req_info_o.wdata = tbox_neigh_req_pop_data.wdata;
    tbox_sc_req_info_o.opcode = tbox_neigh_req_pop_data.opcode;
    tbox_sc_req_info_o.subopcode = tbox_neigh_req_pop_data.subopcode;
    tbox_sc_req_info_o.address = tbox_neigh_req_pop_data.address;
    tbox_sc_req_info_o.size = tbox_neigh_req_pop_data.size;
    tbox_sc_req_info_o.qwen = tbox_neigh_req_pop_data.qwen;

    tbox_min_rsp_info_o = '0;
    tbox_min_rsp_info_o.id = tbox_neigh_req_pop_data.address[3 +: 8];
    tbox_min_rsp_info_o.dest = tbox_neigh_req_pop_data.address[12 +: 4];
    tbox_min_rsp_info_o.wdata = 1'b1;
    tbox_min_rsp_info_o.opcode = etlink_pkg::RspMsgRcvData;
    tbox_min_rsp_info_o.data = tbox_neigh_req_pop_data.data;
    tbox_min_rsp_info_o.size = tbox_neigh_req_pop_data.size;
    tbox_min_rsp_info_o.qwen = tbox_neigh_req_pop_data.qwen;
  end

  logic              sc_tbox_rsp_push;
  logic              sc_tbox_rsp_fifo_full;
  etlink_pkg::rsp_t  sc_tbox_rsp_pop_data;
  logic              sc_tbox_rsp_pop_valid;
  logic              sc_tbox_rsp_pop_valid_next;
  logic              sc_tbox_rsp_pop;
  logic              sc_rsp_fifo_full_next_unused;

  assign sc_tbox_rsp_push = sc_tbox_rsp_valid_i && !sc_tbox_rsp_fifo_full;
  assign sc_tbox_rsp_ready_o = !sc_tbox_rsp_fifo_full;

  prim_fifo #(
    .Width     (RspWidth),
    .Depth     (ScTboxRspFifoDepth),
    .UseLatches(1'b0)
  ) u_sc_tbox_rsp_fifo (
    .clk_i         (clk_i),
    .rst_ni        (rst_ni),
    .push_early_i  (1'b0),
    .push_i        (sc_tbox_rsp_push),
    .push_data_i   (sc_tbox_rsp_info_i),
    .full_o        (sc_tbox_rsp_fifo_full),
    .full_next_o   (sc_rsp_fifo_full_next_unused),
    .pop_i         (sc_tbox_rsp_pop),
    .pop_data_o    (sc_tbox_rsp_pop_data),
    .valid_o       (sc_tbox_rsp_pop_valid),
    .valid_next_o  (sc_tbox_rsp_pop_valid_next)
  );

  logic              sc_tbox_rsp_grant;
  logic              min_tbox_rsp_grant;
  logic              tbox_neigh_rsp_push_early;
  logic              tbox_neigh_rsp_push;
  etlink_pkg::rsp_t  tbox_neigh_rsp_push_data;
  logic              tbox_neigh_rsp_full;
  logic              tbox_neigh_rsp_pop;
  etlink_pkg::rsp_t  tbox_neigh_rsp_pop_data;
  logic              tbox_neigh_rsp_pop_valid;
  logic              tbox_neigh_rsp_pop_ready;
  logic              tbox_rsp_fifo_full_next_unused;
  logic              tbox_rsp_valid_next_unused;

  prim_arb_rr #(
    .NumReqs(2),
    .Width   (RspWidth)
  ) u_tbox_rsp_arb (
    .clk_i    (clk_i),
    .rst_ni   (rst_ni),
    .reqs_i   ({sc_tbox_rsp_pop_valid, min_tbox_rsp_valid}),
    .pop_i    (!tbox_neigh_rsp_full),
    .grants_o ({sc_tbox_rsp_grant, min_tbox_rsp_grant}),
    .data_i   ({sc_tbox_rsp_pop_data, min_tbox_rsp_info}),
    .data_o   (tbox_neigh_rsp_push_data)
  );

  assign tbox_neigh_rsp_push_early = sc_tbox_rsp_pop_valid_next | min_tbox_rsp_valid_next
      | ((|{sc_tbox_rsp_pop_valid, min_tbox_rsp_valid}) && tbox_neigh_rsp_full);
  assign tbox_neigh_rsp_push = (|{sc_tbox_rsp_pop_valid, min_tbox_rsp_valid}) && !tbox_neigh_rsp_full;
  assign sc_tbox_rsp_pop = sc_tbox_rsp_grant && !tbox_neigh_rsp_full;
  assign min_tbox_rsp_ready = min_tbox_rsp_grant && !tbox_neigh_rsp_full;

  prim_fifo #(
    .Width     (RspWidth),
    .Depth     (ScTboxRspFifoDepth),
    .UseLatches(1'b1)
  ) u_tbox_neigh_rsp_fifo (
    .clk_i         (clk_i),
    .rst_ni        (rst_ni),
    .push_early_i  (tbox_neigh_rsp_push_early),
    .push_i        (tbox_neigh_rsp_push),
    .push_data_i   (tbox_neigh_rsp_push_data),
    .full_o        (tbox_neigh_rsp_full),
    .full_next_o   (tbox_rsp_fifo_full_next_unused),
    .pop_i         (tbox_neigh_rsp_pop),
    .pop_data_o    (tbox_neigh_rsp_pop_data),
    .valid_o       (tbox_neigh_rsp_pop_valid),
    .valid_next_o  (tbox_rsp_valid_next_unused)
  );

  assign tbox_neigh_rsp_pop = tbox_neigh_rsp_pop_valid && tbox_neigh_rsp_pop_ready;
  assign tbox_neigh_rsp_pop_ready = tbox_neigh_rsp_ready_i || !tbox_neigh_rsp_valid_o;

  // Original EN_FF storage has no reset; preserve the unreset response payload register.
  always_ff @(posedge clk_i) begin
    if (tbox_neigh_rsp_pop) begin
      tbox_neigh_rsp_info_o <= tbox_neigh_rsp_pop_data;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      tbox_neigh_rsp_valid_o <= 1'b0;
    end else if (tbox_neigh_rsp_pop_ready) begin
      tbox_neigh_rsp_valid_o <= tbox_neigh_rsp_pop_valid;
    end
  end

  logic unused_ok;
  assign unused_ok = ^{1'b0, min_req_fifo_full_next_unused, tbox_req_fifo_full_next_unused,
                       tbox_req_valid_next_unused, sc_rsp_fifo_full_next_unused,
                       tbox_rsp_fifo_full_next_unused, tbox_rsp_valid_next_unused};

endmodule : neigh_tbox_router
