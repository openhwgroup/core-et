// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module dcache_writeback_unit (
  // System signals
  input  logic                            clock,
  input  logic                            reset,
  // Request to start a writeback
  output logic                            req_ready,
  input  logic                            req_valid,
  input  dcache_wb_req                    req_data,
  // Port to read DCache data
  output logic                            data_req_bid,
  input  logic                            data_req_grant,
  output logic                            data_req_valid,
  output dcache_da_read_req               data_req,
  input  logic                            data_req_nack,
  input  dcache_da_data                   data_resp,
  // Writeback port to the L2
  input  logic                            l2_req_ready,
  output logic                            l2_req_valid,
  output et_link_minion_evict_req_info_t  l2_req_data
);

////////////////////////////////////////////////////////////////////////////////
// Accepting a new request
////////////////////////////////////////////////////////////////////////////////

  dcache_wb_req                               req;

  logic [$clog2(`CORE_L2_BLOCK_EXT_PASSES):0] data_req_cnt;
  logic [$clog2(`CORE_L2_BLOCK_EXT_PASSES):0] data_req_cnt_next;
  logic                                       data_req_cnt_idx;
  logic [$clog2(`CORE_L2_BLOCK_EXT_PASSES):0] data_req_cnt_limit;
  logic                                       active;
  logic                                       active_next;
  logic                                       new_req;

  logic                                       data_req_valid_next;

  assign new_req = req_ready && req_valid;

  //      CLK    RST    EN       DOUT               DIN               DEF
  `RST_FF(clock, reset,          active,            active_next,      1'b0)
  `FF    (clock,                 data_req_cnt,      data_req_cnt_next     )
  `FF    (clock,                 data_req_cnt_idx,  data_req_cnt[0]       )
  `EN_FF (clock,        new_req, req,               req_data              )

  assign req_ready = !active;

////////////////////////////////////////////////////////////////////////////////
// Dealing with current request
////////////////////////////////////////////////////////////////////////////////

  logic r1_data_req_fired, r1_data_req_fired_next;
  logic r2_data_req_fired, r2_data_req_fired_next;
  logic r3_data_req_fired, r3_data_req_fired_next;

  logic r2_invalid, r3_invalid;

  //      CLK    RST    DOUT               DIN                     DEF
  `RST_FF(clock, reset, r1_data_req_fired, r1_data_req_fired_next, 1'b0)
  `RST_FF(clock, reset, r2_data_req_fired, r2_data_req_fired_next, 1'b0)
  `RST_FF(clock, reset, r3_data_req_fired, r3_data_req_fired_next, 1'b0)
  `RST_FF(clock, reset, data_req_valid,    data_req_valid_next,    1'b0)

  `RST_FF(clock, reset, r2_invalid,        data_req_nack,          1'b0)
  `RST_FF(clock, reset, r3_invalid,        r2_invalid,             1'b0)

  // Single shot for Tbox messages
  assign data_req_cnt_limit = req.is_tbox ? 'b1 : `CORE_L2_BLOCK_EXT_PASSES;

  always_comb begin
    r1_data_req_fired_next = 1'b0;
    r2_data_req_fired_next = 1'b0;
    r3_data_req_fired_next = 1'b0;
    l2_req_valid           = 1'b0;
    data_req_cnt_next      = data_req_cnt;
    data_req_valid_next    = data_req_grant;
    active_next            = active;
    data_req_bid           = 1'b0;

    if (new_req) begin
      active_next       = 1'b1;
      data_req_cnt_next = 'b0;
    end else if (active) begin
      data_req_bid = (data_req_cnt < data_req_cnt_limit);
      r2_data_req_fired_next = r1_data_req_fired;
      r3_data_req_fired_next = r2_data_req_fired;
      // Upon grant, decrement
      if (data_req_grant) begin
        data_req_cnt_next = data_req_cnt + 'b1;
      end
      // If requests accepted, R1 stage is fired
      if (data_req_valid) begin
        r1_data_req_fired_next = 1'b1;
      end
      // If there's a request in R3
      if (r3_data_req_fired) begin
        l2_req_valid = !r3_invalid;
        // If release is not ready, cancel flying data requests
        if (!l2_req_ready || r3_invalid) begin
          // Kill all transactions
          //data_req_bid           = 1'b0; => TIMING: the request bid for the pipeline is not killed due timing
          data_req_valid_next    = 1'b0;
          r1_data_req_fired_next = 1'b0;
          r2_data_req_fired_next = 1'b0;
          r3_data_req_fired_next = 1'b0;
          // Rollback to pass that failed depending on the pipeline state (detemines the number of releases that finished)
          data_req_cnt_next = data_req_cnt_next - data_req_grant;
          data_req_cnt_next = data_req_cnt_next - data_req_valid;
          data_req_cnt_next = data_req_cnt_next - r1_data_req_fired;
          data_req_cnt_next = data_req_cnt_next - r2_data_req_fired;
          data_req_cnt_next = data_req_cnt_next - r3_data_req_fired;
        end
        // If we are no longer sending request
        else if (!r3_invalid && !r1_data_req_fired && !r2_data_req_fired && !data_req_valid) begin
          active_next = (data_req_cnt < data_req_cnt_limit);
        end
      end
    end
  end

  // Set address of the LRAM that we want to read
  always_comb begin
    data_req.valid_l = 1'b1;
    data_req.valid_h = 1'b1;

    data_req.way  = dcache_way_encode(req.way_en);

    if (!req.is_tbox) begin
      data_req.addr = { req.set, data_req_cnt_idx, {`CORE_L2_BLOCK_EXT_ADDR_OFFSET_BITS{1'b0}} };
    end else begin
      data_req.addr = { req.set, req.mem_idx, {`CORE_L2_BLOCK_EXT_ADDR_OFFSET_BITS{1'b0}} };
    end
  end

  // Generates the L2 request
  always_comb begin
    if (!req.is_tbox) begin
      // L2 Evict configuration
      l2_req_data.id        = ET_LINK_Core_Miss_Id_WriteBack;
      l2_req_data.source    = 1'b0;
      l2_req_data.wdata     = 1'b1;
      l2_req_data.opcode    = ET_LINK_REQ_Write;
      l2_req_data.subopcode = `ET_LINK_SUBOPCODE_SIZE'b0;
      l2_req_data.address   = { req.tag, req.set[`DCACHE_SET_RED_RANGE], 6'b0 }; // Tag is extended, so use reduced set bits
      l2_req_data.data      = data_resp;
      l2_req_data.size      = ET_LINK_Line; // 512 bits
      l2_req_data.qwen      = 4'b1111;
    end else begin
      // L2 Message Send configuration
      l2_req_data.id        = ET_LINK_Core_Miss_Id_WriteBack;
      l2_req_data.source    = 1'b0;
      l2_req_data.wdata     = 1'b1;
      l2_req_data.opcode    = ET_LINK_REQ_MsgSendData;
      l2_req_data.subopcode = `ET_LINK_SUBOPCODE_SIZE'b0;
      l2_req_data.address   = { req.tag, req.addr_low }; // Tag is extended, addr_low is the remaining bits
      l2_req_data.data      = data_resp;
      l2_req_data.size      = ET_LINK_HLine; // 256 bits, ls-aligned
      l2_req_data.qwen      = 4'b0011;
    end
  end

endmodule

