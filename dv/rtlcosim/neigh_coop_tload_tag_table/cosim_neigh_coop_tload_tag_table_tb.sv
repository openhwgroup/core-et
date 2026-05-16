// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_neigh_coop_tload_tag_table_tb
  import neigh_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,

  input  logic [2*$bits(neigh_pkg::coop_tload_tag_table_req_t)-1:0] wr_req_bits_i,
  input  logic [1:0] wr_valid_i,
  input  logic cam_pop_i,
  input  logic [ShireCoopLoadIdSize-1:0] cam_tag_i,

  output logic [1:0] orig_wr_ready_o,
  output logic [1:0] new_wr_ready_o,
  output logic [$bits(neigh_pkg::coop_tload_tag_table_data_t)-1:0] orig_cam_data_bits_o,
  output logic [$bits(neigh_pkg::coop_tload_tag_table_data_t)-1:0] new_cam_data_bits_o
);

  localparam int unsigned ReqBits = $bits(neigh_pkg::coop_tload_tag_table_req_t);

  coop_tload_tag_table_req_t [1:0] orig_wr_req;
  coop_tload_tag_table_data_t orig_cam_data;
  neigh_pkg::coop_tload_tag_table_req_t [1:0] new_wr_req;
  neigh_pkg::coop_tload_tag_table_data_t new_cam_data;

  always_comb begin
    for (int unsigned p = 0; p < 2; p++) begin
      orig_wr_req[p] = coop_tload_tag_table_req_t'(wr_req_bits_i[p*ReqBits +: ReqBits]);
      new_wr_req[p] = neigh_pkg::coop_tload_tag_table_req_t'(wr_req_bits_i[p*ReqBits +: ReqBits]);
    end
    orig_cam_data_bits_o = orig_cam_data;
    new_cam_data_bits_o = new_cam_data;
  end

  neigh_coop_tload_tag_table_orig u_orig (
    .clock    (clk_i),
    .reset    (!rst_ni),
    .wr_req   (orig_wr_req),
    .wr_valid (wr_valid_i),
    .wr_ready (orig_wr_ready_o),
    .cam_pop  (cam_pop_i),
    .cam_tag  (cam_tag_i),
    .cam_data (orig_cam_data)
  );

  neigh_coop_tload_tag_table u_new (
    .clk_i,
    .rst_ni,
    .wr_req_i   (new_wr_req),
    .wr_valid_i (wr_valid_i),
    .wr_ready_o (new_wr_ready_o),
    .cam_pop_i  (cam_pop_i),
    .cam_tag_i  (cam_tag_i),
    .cam_data_o (new_cam_data)
  );

endmodule : cosim_neigh_coop_tload_tag_table_tb
