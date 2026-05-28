// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_hv_logic_l2_bank (
  input  logic                    clk_shire_i,
  input  logic                    rst_shire_ndm_ni,

  input  logic                    bank_req_out_is_first_half_i,
  input  etlink_pkg::req_nodata_t bank_req_info_out_i,
  input  logic [etlink_pkg::NeighDataSize-1:0] bank_req_data_out_i,
  input  logic                    bank_req_info_valid_i,
  output logic                    bank_req_info_pop_o,
  output logic                    bank_req_data_pop_o,

  output etlink_pkg::req_t        miss_req_bank_info_o,
  output logic                    miss_req_bank_valid_o,
  input  logic                    miss_req_bank_ready_i
);

  logic bank_req_first_half_grant;
  logic bank_req_is_second_half_q;
  logic bank_req_is_second_half_d;
  logic [etlink_pkg::NeighDataSize-1:0] bank_evict_req_first_half_q;

  assign bank_req_first_half_grant = bank_req_info_valid_i & bank_req_out_is_first_half_i;
  assign bank_req_info_pop_o =
      bank_req_info_valid_i & (bank_req_first_half_grant | miss_req_bank_ready_i);
  assign bank_req_data_pop_o = bank_req_info_pop_o & bank_req_info_out_i.wdata;

  always_comb begin
    bank_req_is_second_half_d = bank_req_is_second_half_q;

    if (bank_req_first_half_grant) begin
      bank_req_is_second_half_d = 1'b1;
    end else if (bank_req_info_pop_o && bank_req_is_second_half_q) begin
      bank_req_is_second_half_d = 1'b0;
    end
  end

  always_ff @(posedge clk_shire_i or negedge rst_shire_ndm_ni) begin
    if (!rst_shire_ndm_ni) begin
      bank_req_is_second_half_q <= 1'b0;
    end else if (bank_req_is_second_half_q ^ bank_req_is_second_half_d) begin
      bank_req_is_second_half_q <= bank_req_is_second_half_d;
    end
  end

  always_ff @(posedge clk_shire_i) begin
    if (bank_req_first_half_grant) begin
      bank_evict_req_first_half_q <= bank_req_data_out_i;
    end
  end

  always_comb begin
    miss_req_bank_info_o.id        = bank_req_info_out_i.id;
    miss_req_bank_info_o.source    = bank_req_info_out_i.source;
    miss_req_bank_info_o.wdata     = bank_req_info_out_i.wdata;
    miss_req_bank_info_o.opcode    = bank_req_info_out_i.opcode;
    miss_req_bank_info_o.subopcode = bank_req_info_out_i.subopcode;
    miss_req_bank_info_o.address   = bank_req_info_out_i.address;
    miss_req_bank_info_o.data      = bank_req_is_second_half_q
        ? {bank_req_data_out_i, bank_evict_req_first_half_q}
        : {bank_req_data_out_i, bank_req_data_out_i};
    miss_req_bank_info_o.size      = bank_req_info_out_i.size;
    miss_req_bank_info_o.qwen      = bank_req_info_out_i.qwen;
  end

  assign miss_req_bank_valid_o = bank_req_info_valid_i & ~bank_req_out_is_first_half_i;

endmodule : neigh_hv_logic_l2_bank
