// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_shire_coop_tload_bus_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [27:0] slv_rdy_data_i,
  input  logic [3:0]  slv_rdy_valid_i,
  input  logic [19:0] mst_done_id_i,
  input  logic [11:0] mst_done_valid_i,
  output logic [83:0] orig_slv_rdy_data_o,
  output logic [83:0] new_slv_rdy_data_o,
  output logic [11:0] orig_slv_rdy_valid_o,
  output logic [11:0] new_slv_rdy_valid_o,
  output logic [59:0] orig_mst_done_id_o,
  output logic [59:0] new_mst_done_id_o,
  output logic [11:0] orig_mst_done_valid_o,
  output logic [11:0] new_mst_done_valid_o
);
  localparam int unsigned NumNeigh = 4;
  localparam int unsigned NumOther = 3;
  localparam int unsigned RdyWidth = $bits(coop_tload_slv_rdy_req_t);
  localparam int unsigned IdWidth = 5;

  coop_tload_slv_rdy_req_t [NumNeigh-1:0] orig_slv_out;
  coop_tload_slv_rdy_req_t [NumNeigh-1:0][NumOther-1:0] orig_slv_in;
  logic [NumNeigh-1:0][IdWidth-1:0] orig_done_id_out;
  logic [NumNeigh-1:0][NumOther-1:0] orig_done_valid_out;
  logic [NumNeigh-1:0][NumOther-1:0][IdWidth-1:0] orig_done_id_in;
  logic [NumNeigh-1:0][NumOther-1:0] orig_done_valid_in;
  logic [NumNeigh-1:0][NumOther-1:0] orig_slv_valid_in;

  neigh_pkg::coop_tload_slv_rdy_req_t new_slv_out [NumNeigh-1:0];
  neigh_pkg::coop_tload_slv_rdy_req_t new_slv_in [NumNeigh-1:0][NumOther-1:0];
  logic [NumNeigh-1:0][IdWidth-1:0] new_done_id_out;
  logic [NumNeigh-1:0][NumOther-1:0] new_done_valid_out;
  logic [NumNeigh-1:0][NumOther-1:0][IdWidth-1:0] new_done_id_in;
  logic [NumNeigh-1:0][NumOther-1:0] new_done_valid_in;
  logic [NumNeigh-1:0][NumOther-1:0] new_slv_valid_in;

  always_comb begin
    for (int unsigned n = 0; n < NumNeigh; n++) begin
      orig_slv_out[n] = slv_rdy_data_i[(n*RdyWidth) +: RdyWidth];
      new_slv_out[n] = slv_rdy_data_i[(n*RdyWidth) +: RdyWidth];
      orig_done_id_out[n] = mst_done_id_i[(n*IdWidth) +: IdWidth];
      new_done_id_out[n] = mst_done_id_i[(n*IdWidth) +: IdWidth];
      orig_done_valid_out[n] = mst_done_valid_i[(n*NumOther) +: NumOther];
      new_done_valid_out[n] = mst_done_valid_i[(n*NumOther) +: NumOther];
      for (int unsigned m = 0; m < NumOther; m++) begin
        orig_slv_rdy_data_o[((n*NumOther+m)*RdyWidth) +: RdyWidth] = orig_slv_in[n][m];
        new_slv_rdy_data_o[((n*NumOther+m)*RdyWidth) +: RdyWidth] = new_slv_in[n][m];
        orig_mst_done_id_o[((n*NumOther+m)*IdWidth) +: IdWidth] = orig_done_id_in[n][m];
        new_mst_done_id_o[((n*NumOther+m)*IdWidth) +: IdWidth] = new_done_id_in[n][m];
      end
    end
    orig_slv_rdy_valid_o = orig_slv_valid_in;
    new_slv_rdy_valid_o = new_slv_valid_in;
    orig_mst_done_valid_o = orig_done_valid_in;
    new_mst_done_valid_o = new_done_valid_in;
  end

  shire_coop_tload_bus_orig u_orig (
    .coop_tload_slv_rdy_out_data      (orig_slv_out),
    .coop_tload_slv_rdy_out_valid     (slv_rdy_valid_i),
    .coop_tload_slv_rdy_in_data       (orig_slv_in),
    .coop_tload_slv_rdy_in_valid      (orig_slv_valid_in),
    .coop_tload_mst_done_out_coop_id  (orig_done_id_out),
    .coop_tload_mst_done_out_valid    (orig_done_valid_out),
    .coop_tload_mst_done_in_coop_id   (orig_done_id_in),
    .coop_tload_mst_done_in_valid     (orig_done_valid_in)
  );

  shire_coop_tload_bus u_new (
    .coop_tload_slv_rdy_out_data_i      (new_slv_out),
    .coop_tload_slv_rdy_out_valid_i     (slv_rdy_valid_i),
    .coop_tload_slv_rdy_in_data_o       (new_slv_in),
    .coop_tload_slv_rdy_in_valid_o      (new_slv_valid_in),
    .coop_tload_mst_done_out_coop_id_i  (new_done_id_out),
    .coop_tload_mst_done_out_valid_i    (new_done_valid_out),
    .coop_tload_mst_done_in_coop_id_o   (new_done_id_in),
    .coop_tload_mst_done_in_valid_o     (new_done_valid_in)
  );

  logic unused_clk_rst;
  assign unused_clk_rst = ^{clk_i, rst_ni};
endmodule
