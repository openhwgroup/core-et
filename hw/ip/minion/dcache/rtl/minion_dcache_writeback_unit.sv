// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Minion dcache writeback unit.
//
// Replaces: dcache_writeback_unit

module minion_dcache_writeback_unit
  import minion_dcache_pkg::*;
(
  input  logic                         clk_i,
  input  logic                         rst_ni,
  output logic                         req_ready_o,
  input  logic                         req_valid_i,
  input  dcache_wb_req_t               req_data_i,
  output logic                         data_req_bid_o,
  input  logic                         data_req_grant_i,
  output logic                         data_req_valid_o,
  output dcache_da_read_req_t          data_req_o,
  input  logic                         data_req_nack_i,
  input  dcache_da_data_t              data_resp_i,
  input  logic                         l2_req_ready_i,
  output logic                         l2_req_valid_o,
  output et_link_minion_evict_req_info_t l2_req_data_o
);

  localparam int unsigned PassCountWidth = $clog2(CoreL2BlockExtPasses) + 1;

  dcache_wb_req_t         req_q;
  logic [PassCountWidth-1:0] data_req_cnt_d;
  logic [PassCountWidth-1:0] data_req_cnt_q;
  logic                    data_req_cnt_idx_q;
  logic [PassCountWidth-1:0] data_req_cnt_limit;
  logic                    active_d;
  logic                    active_q;
  logic                    new_req;
  logic                    data_req_valid_d;
  logic                    r1_data_req_fired_d;
  logic                    r1_data_req_fired_q;
  logic                    r2_data_req_fired_d;
  logic                    r2_data_req_fired_q;
  logic                    r3_data_req_fired_d;
  logic                    r3_data_req_fired_q;
  logic                    r2_invalid_q;
  logic                    r3_invalid_q;

  assign new_req = req_ready_o && req_valid_i;
  assign req_ready_o = !active_q;

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      active_q <= 1'b0;
    end else begin
      active_q <= active_d;
    end
  end

  always_ff @(posedge clk_i) begin
    data_req_cnt_q <= data_req_cnt_d;
    data_req_cnt_idx_q <= data_req_cnt_q[0];
    if (new_req) begin
      req_q <= req_data_i;
    end
  end

  always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
      r1_data_req_fired_q <= 1'b0;
      r2_data_req_fired_q <= 1'b0;
      r3_data_req_fired_q <= 1'b0;
      data_req_valid_o <= 1'b0;
      r2_invalid_q <= 1'b0;
      r3_invalid_q <= 1'b0;
    end else begin
      r1_data_req_fired_q <= r1_data_req_fired_d;
      r2_data_req_fired_q <= r2_data_req_fired_d;
      r3_data_req_fired_q <= r3_data_req_fired_d;
      data_req_valid_o <= data_req_valid_d;
      r2_invalid_q <= data_req_nack_i;
      r3_invalid_q <= r2_invalid_q;
    end
  end

  assign data_req_cnt_limit = req_q.is_tbox ? {{PassCountWidth-1{1'b0}}, 1'b1}
                                             : CoreL2BlockExtPasses[PassCountWidth-1:0];

  always_comb begin
    r1_data_req_fired_d = 1'b0;
    r2_data_req_fired_d = 1'b0;
    r3_data_req_fired_d = 1'b0;
    l2_req_valid_o = 1'b0;
    data_req_cnt_d = data_req_cnt_q;
    data_req_valid_d = data_req_grant_i;
    active_d = active_q;
    data_req_bid_o = 1'b0;

    if (new_req) begin
      active_d = 1'b1;
      data_req_cnt_d = '0;
    end else if (active_q) begin
      data_req_bid_o = (data_req_cnt_q < data_req_cnt_limit);
      r2_data_req_fired_d = r1_data_req_fired_q;
      r3_data_req_fired_d = r2_data_req_fired_q;
      if (data_req_grant_i) begin
        data_req_cnt_d = data_req_cnt_q + 1'b1;
      end
      if (data_req_valid_o) begin
        r1_data_req_fired_d = 1'b1;
      end
      if (r3_data_req_fired_q) begin
        l2_req_valid_o = !r3_invalid_q;
        if (!l2_req_ready_i || r3_invalid_q) begin
          data_req_valid_d = 1'b0;
          r1_data_req_fired_d = 1'b0;
          r2_data_req_fired_d = 1'b0;
          r3_data_req_fired_d = 1'b0;
          data_req_cnt_d = data_req_cnt_d - data_req_grant_i;
          data_req_cnt_d = data_req_cnt_d - data_req_valid_o;
          data_req_cnt_d = data_req_cnt_d - r1_data_req_fired_q;
          data_req_cnt_d = data_req_cnt_d - r2_data_req_fired_q;
          data_req_cnt_d = data_req_cnt_d - r3_data_req_fired_q;
        end else if (!r3_invalid_q && !r1_data_req_fired_q && !r2_data_req_fired_q && !data_req_valid_o) begin
          active_d = (data_req_cnt_q < data_req_cnt_limit);
        end
      end
    end
  end

  always_comb begin
    data_req_o.valid_l = 1'b1;
    data_req_o.valid_h = 1'b1;
    data_req_o.way = dcache_way_encode(req_q.way_en);
    if (!req_q.is_tbox) begin
      data_req_o.addr = {req_q.set, data_req_cnt_idx_q, {CoreL2BlockExtAddrOffsetBits{1'b0}}};
    end else begin
      data_req_o.addr = {req_q.set, req_q.mem_idx, {CoreL2BlockExtAddrOffsetBits{1'b0}}};
    end
  end

  always_comb begin
    l2_req_data_o = '0;
    if (!req_q.is_tbox) begin
      l2_req_data_o.id = EtLinkCoreMissIdWriteBack;
      l2_req_data_o.source = 1'b0;
      l2_req_data_o.wdata = 1'b1;
      l2_req_data_o.opcode = EtLinkReqWrite;
      l2_req_data_o.subopcode = '0;
      l2_req_data_o.address = {req_q.tag, req_q.set[$clog2(DcacheSetRed)-1:0], 6'b0};
      l2_req_data_o.data = data_resp_i;
      l2_req_data_o.size = EtLinkLine;
      l2_req_data_o.qwen = 4'b1111;
    end else begin
      l2_req_data_o.id = EtLinkCoreMissIdWriteBack;
      l2_req_data_o.source = 1'b0;
      l2_req_data_o.wdata = 1'b1;
      l2_req_data_o.opcode = EtLinkReqMsgSendData;
      l2_req_data_o.subopcode = '0;
      l2_req_data_o.address = {req_q.tag, req_q.addr_low};
      l2_req_data_o.data = data_resp_i;
      l2_req_data_o.size = EtLinkHLine;
      l2_req_data_o.qwen = 4'b0011;
    end
  end

endmodule : minion_dcache_writeback_unit
