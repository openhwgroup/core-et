// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire cache L2 hardware prefetcher interface.
//
// Monitors pipe tag response and provides sideband signals to L2 HPF.
// Only monitors L2 Read/Write; assumes shire cache bank read buffer not enabled.
// Conditional generate: L2HpfImplemented=0 outputs zero (default).
//
// Replaces: shire_cache_bank_l2hpf (etcore-soc)

/* verilator lint_off UNUSEDSIGNAL */
module shirecache_bank_l2hpf
  import shirecache_pkg::*;
  import etlink_pkg::*;
#(
  parameter int unsigned NumEntries      = ReqqDepth,
  parameter bit          L2HpfImplemented = 0
) (
  input  logic                                clk_i,
  input  logic                                rst_ni,

  // Pipeline tag response
  input  logic                                pipe_tag_rsp_valid_i,
  input  tag_rsp_t                            pipe_tag_rsp_info_i,

  // L2 HPF request output
  output logic                                l2hpf_req_valid_o,
  output neigh_l2hpf_req_t                    l2hpf_req_info_o,

  // Reqq state
  input  reqq_entry_state_t [NumEntries-1:0]  reqq_state_i
);

  if (L2HpfImplemented) begin : gen_l2hpf

    logic               req_valid_d;
    neigh_l2hpf_req_t   req_info_d;

    always_comb begin
      req_valid_d = pipe_tag_rsp_valid_i &
        (pipe_tag_rsp_info_i.opcode inside {L2Read, L2WritePartial, L2Write});

      req_info_d = '0;
      req_info_d.source  = SourceSize'(reqq_state_i[pipe_tag_rsp_info_i.reqq_id].source);
      req_info_d.id      = IdSize'(reqq_state_i[pipe_tag_rsp_info_i.reqq_id].tag_id);
      req_info_d.address = pipe_tag_rsp_info_i.orig_address;
      req_info_d.opcode  =
        (reqq_state_i[pipe_tag_rsp_info_i.reqq_id].orig_opcode inside {OrigRead, OrigReadCoop})
          ? ReqRead : ReqWrite;
      req_info_d.hit     = pipe_tag_rsp_info_i.hit
                         & ~pipe_tag_rsp_info_i.victim
                         & (pipe_tag_rsp_info_i.err_rsp != ErrNone);
    end

    // Register output (RST_EN_FF pattern)
    logic req_en;
    assign req_en = pipe_tag_rsp_valid_i | l2hpf_req_valid_o;

    always_ff @(posedge clk_i or negedge rst_ni) begin
      if (!rst_ni)
        l2hpf_req_valid_o <= 1'b0;
      else if (req_en)
        l2hpf_req_valid_o <= req_valid_d;
    end

    always_ff @(posedge clk_i) begin
      if (req_en)
        l2hpf_req_info_o <= req_info_d;
    end

  end else begin : gen_l2hpf_stub

    assign l2hpf_req_valid_o = 1'b0;
    assign l2hpf_req_info_o  = '0;

  end

  logic unused_ok;
  assign unused_ok = &{1'b0,
    clk_i, rst_ni,
    pipe_tag_rsp_valid_i,
    pipe_tag_rsp_info_i,
    reqq_state_i,
    unused_ok};

  /* verilator lint_on UNUSEDSIGNAL */

endmodule
