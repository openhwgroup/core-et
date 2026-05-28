// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_hv_logic_neigh_sc #(
  parameter int unsigned ScBanks = neigh_hv_logic_pkg::ScBanks
) (
  input  logic                         clk_shire_i,
  input  logic                         rst_shire_ndm_ni,

  input  etlink_pkg::req_t [ScBanks-1:0] miss_req_bank_info_i,
  input  logic [ScBanks-1:0]             miss_req_bank_valid_i,
  output logic [ScBanks-1:0]             miss_req_bank_ready_o,

  input  etlink_pkg::req_nodata_t      uc_req_info_out_i,
  input  logic [etlink_pkg::NeighDataSize-1:0] uc_req_data_out_i,
  input  logic                         uc_req_info_pop_valid_i,
  output logic                         uc_req_info_pop_o,
  output logic                         uc_req_data_pop_o,

  output etlink_pkg::req_t             neigh_sc_req_info_o,
  output logic [ScBanks:0]             neigh_sc_req_valid_o,
  input  logic [ScBanks:0]             neigh_sc_req_ready_i
);

  etlink_pkg::req_t out_req_info_from_uc;
  etlink_pkg::req_t [ScBanks:0] out_req_info_by_port;
  etlink_pkg::req_t out_req_info_arb;
  logic [ScBanks:0] out_req_bid;
  logic [ScBanks:0] out_req_ready;
  logic [ScBanks:0] out_req_valid;
  logic             out_req_fifo_empty;

  assign out_req_fifo_empty = ~uc_req_info_pop_valid_i;

  assign out_req_valid[ScBanks] = ~out_req_fifo_empty;
  assign uc_req_info_pop_o = out_req_ready[ScBanks];
  assign uc_req_data_pop_o = uc_req_info_pop_o & uc_req_info_out_i.wdata;

  always_comb begin
    out_req_info_from_uc.id        = uc_req_info_out_i.id;
    out_req_info_from_uc.source    = uc_req_info_out_i.source;
    out_req_info_from_uc.wdata     = uc_req_info_out_i.wdata;
    out_req_info_from_uc.opcode    = uc_req_info_out_i.opcode;
    out_req_info_from_uc.subopcode = uc_req_info_out_i.subopcode;
    out_req_info_from_uc.address   = uc_req_info_out_i.address;
    out_req_info_from_uc.data      = {uc_req_data_out_i, uc_req_data_out_i};
    out_req_info_from_uc.size      = uc_req_info_out_i.size;
    out_req_info_from_uc.qwen      = uc_req_info_out_i.qwen;
  end

  always_comb begin
    for (int unsigned i = 0; i < ScBanks; i++) begin
      out_req_info_by_port[i] = miss_req_bank_info_i[i];
    end
    out_req_info_by_port[ScBanks] = out_req_info_from_uc;
  end

  assign out_req_valid[ScBanks-1:0] = miss_req_bank_valid_i;
  assign out_req_bid = out_req_valid & neigh_sc_req_ready_i;

  prim_arb_rr #(
    .NumReqs (ScBanks + 1),
    .Width   ($bits(etlink_pkg::req_t))
  ) u_out_req_arbiter (
    .clk_i    (clk_shire_i),
    .rst_ni   (rst_shire_ndm_ni),
    .reqs_i   (out_req_bid),
    .pop_i    (|out_req_ready),
    .grants_o (out_req_ready),
    .data_i   (out_req_info_by_port),
    .data_o   (out_req_info_arb)
  );

  assign miss_req_bank_ready_o = out_req_ready[ScBanks-1:0];
  assign neigh_sc_req_info_o = (|out_req_ready) ? out_req_info_arb : '0;
  assign neigh_sc_req_valid_o = out_req_ready;

endmodule : neigh_hv_logic_neigh_sc
