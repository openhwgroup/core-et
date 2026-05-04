// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// minion_dcache_metadata_array — Verilator wrapper.

module minion_dcache_metadata_array_tb (
  input  logic        clk_i,
  input  logic        rst_ni,
  input  logic [3:0]  s1_read_set_i,
  input  logic        s1_write_valid_i,
  input  logic        s1_write_valid_prev_i,
  input  logic [3:0]  s1_write_set_i,
  input  logic [3:0]  s1_write_way_en_i,
  input  logic [1:0]  s1_write_coh_state_i,
  input  logic [32:0] s1_write_tag_i,
  input  logic        cfg_clear_all_i,
  input  logic        cfg_clear_low_i,
  output logic [3:0]  s1_read_resp_valid_o,
  output logic [1:0]  way0_coh_state_o,
  output logic [32:0] way0_tag_o,
  output logic [1:0]  way1_coh_state_o,
  output logic [32:0] way1_tag_o,
  output logic [1:0]  way2_coh_state_o,
  output logic [32:0] way2_tag_o,
  output logic [1:0]  way3_coh_state_o,
  output logic [32:0] way3_tag_o
);

  minion_dcache_pkg::dcache_meta_write_req_t s1_write_req;
  minion_dcache_pkg::dcache_meta_data_t      s1_read_resp [minion_dcache_pkg::DcacheWays];

  always_comb begin
    s1_write_req = '0;
    s1_write_req.set_idx = s1_write_set_i;
    s1_write_req.way_en = s1_write_way_en_i;
    s1_write_req.data.coh_state = minion_dcache_pkg::dcache_line_state_e'(s1_write_coh_state_i);
    s1_write_req.data.tag = s1_write_tag_i;

    way0_coh_state_o = s1_read_resp[0].coh_state;
    way0_tag_o = s1_read_resp[0].tag;
    way1_coh_state_o = s1_read_resp[1].coh_state;
    way1_tag_o = s1_read_resp[1].tag;
    way2_coh_state_o = s1_read_resp[2].coh_state;
    way2_tag_o = s1_read_resp[2].tag;
    way3_coh_state_o = s1_read_resp[3].coh_state;
    way3_tag_o = s1_read_resp[3].tag;
  end

  minion_dcache_metadata_array u_dut (
    .clk_i,
    .rst_ni,
    .s1_read_set_i,
    .s1_write_valid_i,
    .s1_write_valid_prev_i,
    .s1_write_req_i       (s1_write_req),
    .s1_read_resp_valid_o,
    .s1_read_resp_o       (s1_read_resp),
    .cfg_clear_all_i,
    .cfg_clear_low_i
  );

endmodule
