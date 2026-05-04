// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original dcache_metadata_array vs new minion_dcache_metadata_array.

`include "soc.vh"

module cosim_minion_dcache_metadata_array_tb (
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
  output logic [3:0]  new_s1_read_resp_valid_o,
  output logic [1:0]  new_way0_coh_state_o,
  output logic [32:0] new_way0_tag_o,
  output logic [1:0]  new_way1_coh_state_o,
  output logic [32:0] new_way1_tag_o,
  output logic [1:0]  new_way2_coh_state_o,
  output logic [32:0] new_way2_tag_o,
  output logic [1:0]  new_way3_coh_state_o,
  output logic [32:0] new_way3_tag_o,
  output logic [3:0]  orig_s1_read_resp_valid_o,
  output logic [1:0]  orig_way0_coh_state_o,
  output logic [32:0] orig_way0_tag_o,
  output logic [1:0]  orig_way1_coh_state_o,
  output logic [32:0] orig_way1_tag_o,
  output logic [1:0]  orig_way2_coh_state_o,
  output logic [32:0] orig_way2_tag_o,
  output logic [1:0]  orig_way3_coh_state_o,
  output logic [32:0] orig_way3_tag_o
);

  logic reset;
  minion_dcache_pkg::dcache_meta_write_req_t new_write_req;
  minion_dcache_pkg::dcache_meta_data_t      new_read_resp [minion_dcache_pkg::DcacheWays];
  dcache_meta_write_req                      orig_write_req;
  logic [`DCACHE_WAYS-1:0][$bits(dcache_meta_data)-1:0] orig_read_resp_raw;
  dcache_meta_data                           orig_read_resp [minion_dcache_pkg::DcacheWays];

  assign reset = ~rst_ni;

  always_comb begin
    new_write_req = '0;
    new_write_req.set_idx = s1_write_set_i;
    new_write_req.way_en = s1_write_way_en_i;
    new_write_req.data.coh_state = minion_dcache_pkg::dcache_line_state_e'(s1_write_coh_state_i);
    new_write_req.data.tag = s1_write_tag_i;

    orig_write_req = '0;
    orig_write_req.set = s1_write_set_i;
    orig_write_req.way_en = s1_write_way_en_i;
    orig_write_req.data.coh_state = dcache_line_state'(s1_write_coh_state_i);
    orig_write_req.data.tag = s1_write_tag_i;

    orig_read_resp[0] = dcache_meta_data'(orig_read_resp_raw[0]);
    orig_read_resp[1] = dcache_meta_data'(orig_read_resp_raw[1]);
    orig_read_resp[2] = dcache_meta_data'(orig_read_resp_raw[2]);
    orig_read_resp[3] = dcache_meta_data'(orig_read_resp_raw[3]);

    new_way0_coh_state_o = new_read_resp[0].coh_state;
    new_way0_tag_o = new_read_resp[0].tag;
    new_way1_coh_state_o = new_read_resp[1].coh_state;
    new_way1_tag_o = new_read_resp[1].tag;
    new_way2_coh_state_o = new_read_resp[2].coh_state;
    new_way2_tag_o = new_read_resp[2].tag;
    new_way3_coh_state_o = new_read_resp[3].coh_state;
    new_way3_tag_o = new_read_resp[3].tag;

    orig_way0_coh_state_o = orig_read_resp[0].coh_state;
    orig_way0_tag_o = orig_read_resp[0].tag;
    orig_way1_coh_state_o = orig_read_resp[1].coh_state;
    orig_way1_tag_o = orig_read_resp[1].tag;
    orig_way2_coh_state_o = orig_read_resp[2].coh_state;
    orig_way2_tag_o = orig_read_resp[2].tag;
    orig_way3_coh_state_o = orig_read_resp[3].coh_state;
    orig_way3_tag_o = orig_read_resp[3].tag;
  end

  minion_dcache_metadata_array u_new (
    .clk_i,
    .rst_ni,
    .s1_read_set_i,
    .s1_write_valid_i,
    .s1_write_valid_prev_i,
    .s1_write_req_i        (new_write_req),
    .s1_read_resp_valid_o  (new_s1_read_resp_valid_o),
    .s1_read_resp_o        (new_read_resp),
    .cfg_clear_all_i,
    .cfg_clear_low_i
  );

  dcache_metadata_array u_orig (
    .clock               (clk_i),
    .reset               (reset),
    .s1_read_set         (s1_read_set_i),
    .s1_write_valid      (s1_write_valid_i),
    .s1_write_valid_prev (s1_write_valid_prev_i),
    .s1_write_req        (orig_write_req),
    .s1_read_resp_valid  (orig_s1_read_resp_valid_o),
    .s1_read_resp        (orig_read_resp_raw),
    .cfg_clear_all       (cfg_clear_all_i),
    .cfg_clear_low       (cfg_clear_low_i)
  );

endmodule
