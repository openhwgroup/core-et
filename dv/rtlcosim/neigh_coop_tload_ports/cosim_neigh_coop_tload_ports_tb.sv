// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_neigh_coop_tload_ports_tb
  import neigh_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,

  input  logic [$bits(neigh_pkg::coop_tload_slv_rdy_req_t)-1:0] slv_rdy_out_data_lv_bits_i,
  input  logic slv_rdy_out_valid_lv_i,
  input  logic [NumOtherNeigh*$bits(neigh_pkg::coop_tload_slv_rdy_req_t)-1:0] slv_rdy_in_data_bits_i,
  input  logic [NumOtherNeigh-1:0] slv_rdy_in_valid_i,
  input  logic [ShireCoopIdSize-1:0] mst_done_out_coop_id_lv_i,
  input  logic [NumOtherNeigh-1:0] mst_done_out_valid_lv_i,
  input  logic [NumOtherNeigh*ShireCoopIdSize-1:0] mst_done_in_coop_id_bits_i,
  input  logic [NumOtherNeigh-1:0] mst_done_in_valid_i,

  output logic [$bits(neigh_pkg::coop_tload_slv_rdy_req_t)-1:0] orig_slv_rdy_out_data_bits_o,
  output logic [$bits(neigh_pkg::coop_tload_slv_rdy_req_t)-1:0] new_slv_rdy_out_data_bits_o,
  output logic orig_slv_rdy_out_valid_o,
  output logic new_slv_rdy_out_valid_o,
  output logic [NumOtherNeigh*$bits(neigh_pkg::coop_tload_slv_rdy_req_t)-1:0] orig_slv_rdy_in_data_lv_bits_o,
  output logic [NumOtherNeigh*$bits(neigh_pkg::coop_tload_slv_rdy_req_t)-1:0] new_slv_rdy_in_data_lv_bits_o,
  output logic [NumOtherNeigh-1:0] orig_slv_rdy_in_valid_lv_o,
  output logic [NumOtherNeigh-1:0] new_slv_rdy_in_valid_lv_o,
  output logic [ShireCoopIdSize-1:0] orig_mst_done_out_coop_id_o,
  output logic [ShireCoopIdSize-1:0] new_mst_done_out_coop_id_o,
  output logic [NumOtherNeigh-1:0] orig_mst_done_out_valid_o,
  output logic [NumOtherNeigh-1:0] new_mst_done_out_valid_o,
  output logic [NumOtherNeigh*ShireCoopIdSize-1:0] orig_mst_done_in_coop_id_lv_bits_o,
  output logic [NumOtherNeigh*ShireCoopIdSize-1:0] new_mst_done_in_coop_id_lv_bits_o,
  output logic [NumOtherNeigh-1:0] orig_mst_done_in_valid_lv_o,
  output logic [NumOtherNeigh-1:0] new_mst_done_in_valid_lv_o
);

  localparam int unsigned SlvRdyBits = $bits(neigh_pkg::coop_tload_slv_rdy_req_t);

  coop_tload_slv_rdy_req_t orig_slv_rdy_out_data;
  coop_tload_slv_rdy_req_t orig_slv_rdy_out_data_lv;
  coop_tload_slv_rdy_req_t [NumOtherNeigh-1:0] orig_slv_rdy_in_data;
  coop_tload_slv_rdy_req_t [NumOtherNeigh-1:0] orig_slv_rdy_in_data_lv;
  logic [NumOtherNeigh-1:0][ShireCoopIdSize-1:0] orig_mst_done_in_coop_id;
  logic [NumOtherNeigh-1:0][ShireCoopIdSize-1:0] orig_mst_done_in_coop_id_lv;

  neigh_pkg::coop_tload_slv_rdy_req_t new_slv_rdy_out_data;
  neigh_pkg::coop_tload_slv_rdy_req_t new_slv_rdy_out_data_lv;
  neigh_pkg::coop_tload_slv_rdy_req_t [NumOtherNeigh-1:0] new_slv_rdy_in_data;
  neigh_pkg::coop_tload_slv_rdy_req_t [NumOtherNeigh-1:0] new_slv_rdy_in_data_lv;
  logic [NumOtherNeigh-1:0][ShireCoopIdSize-1:0] new_mst_done_in_coop_id;
  logic [NumOtherNeigh-1:0][ShireCoopIdSize-1:0] new_mst_done_in_coop_id_lv;

  always_comb begin
    orig_slv_rdy_out_data_lv = coop_tload_slv_rdy_req_t'(slv_rdy_out_data_lv_bits_i);
    new_slv_rdy_out_data_lv = neigh_pkg::coop_tload_slv_rdy_req_t'(slv_rdy_out_data_lv_bits_i);
    orig_slv_rdy_out_data_bits_o = orig_slv_rdy_out_data;
    new_slv_rdy_out_data_bits_o = new_slv_rdy_out_data;

    for (int unsigned n = 0; n < NumOtherNeigh; n++) begin
      orig_slv_rdy_in_data[n] = coop_tload_slv_rdy_req_t'(
          slv_rdy_in_data_bits_i[n*SlvRdyBits +: SlvRdyBits]);
      new_slv_rdy_in_data[n] = neigh_pkg::coop_tload_slv_rdy_req_t'(
          slv_rdy_in_data_bits_i[n*SlvRdyBits +: SlvRdyBits]);
      orig_slv_rdy_in_data_lv_bits_o[n*SlvRdyBits +: SlvRdyBits] = orig_slv_rdy_in_data_lv[n];
      new_slv_rdy_in_data_lv_bits_o[n*SlvRdyBits +: SlvRdyBits] = new_slv_rdy_in_data_lv[n];
      orig_mst_done_in_coop_id[n] = mst_done_in_coop_id_bits_i[n*ShireCoopIdSize +: ShireCoopIdSize];
      new_mst_done_in_coop_id[n] = mst_done_in_coop_id_bits_i[n*ShireCoopIdSize +: ShireCoopIdSize];
      orig_mst_done_in_coop_id_lv_bits_o[n*ShireCoopIdSize +: ShireCoopIdSize] =
          orig_mst_done_in_coop_id_lv[n];
      new_mst_done_in_coop_id_lv_bits_o[n*ShireCoopIdSize +: ShireCoopIdSize] =
          new_mst_done_in_coop_id_lv[n];
    end
  end

  neigh_coop_tload_ports_orig u_orig (
    .clock                               (clk_i),
    .reset                               (!rst_ni),
    .coop_tload_slv_rdy_out_data         (orig_slv_rdy_out_data),
    .coop_tload_slv_rdy_out_valid        (orig_slv_rdy_out_valid_o),
    .coop_tload_slv_rdy_in_data          (orig_slv_rdy_in_data),
    .coop_tload_slv_rdy_in_valid         (slv_rdy_in_valid_i),
    .coop_tload_mst_done_out_coop_id     (orig_mst_done_out_coop_id_o),
    .coop_tload_mst_done_out_valid       (orig_mst_done_out_valid_o),
    .coop_tload_mst_done_in_coop_id      (orig_mst_done_in_coop_id),
    .coop_tload_mst_done_in_valid        (mst_done_in_valid_i),
    .coop_tload_slv_rdy_out_data_lv      (orig_slv_rdy_out_data_lv),
    .coop_tload_slv_rdy_out_valid_lv     (slv_rdy_out_valid_lv_i),
    .coop_tload_slv_rdy_in_data_lv       (orig_slv_rdy_in_data_lv),
    .coop_tload_slv_rdy_in_valid_lv      (orig_slv_rdy_in_valid_lv_o),
    .coop_tload_mst_done_out_coop_id_lv  (mst_done_out_coop_id_lv_i),
    .coop_tload_mst_done_out_valid_lv    (mst_done_out_valid_lv_i),
    .coop_tload_mst_done_in_coop_id_lv   (orig_mst_done_in_coop_id_lv),
    .coop_tload_mst_done_in_valid_lv     (orig_mst_done_in_valid_lv_o)
  );

  neigh_coop_tload_ports u_new (
    .clk_i,
    .rst_ni,
    .coop_tload_slv_rdy_out_data_o        (new_slv_rdy_out_data),
    .coop_tload_slv_rdy_out_valid_o       (new_slv_rdy_out_valid_o),
    .coop_tload_slv_rdy_in_data_i         (new_slv_rdy_in_data),
    .coop_tload_slv_rdy_in_valid_i        (slv_rdy_in_valid_i),
    .coop_tload_mst_done_out_coop_id_o    (new_mst_done_out_coop_id_o),
    .coop_tload_mst_done_out_valid_o      (new_mst_done_out_valid_o),
    .coop_tload_mst_done_in_coop_id_i     (new_mst_done_in_coop_id),
    .coop_tload_mst_done_in_valid_i       (mst_done_in_valid_i),
    .coop_tload_slv_rdy_out_data_lv_i     (new_slv_rdy_out_data_lv),
    .coop_tload_slv_rdy_out_valid_lv_i    (slv_rdy_out_valid_lv_i),
    .coop_tload_slv_rdy_in_data_lv_o      (new_slv_rdy_in_data_lv),
    .coop_tload_slv_rdy_in_valid_lv_o     (new_slv_rdy_in_valid_lv_o),
    .coop_tload_mst_done_out_coop_id_lv_i (mst_done_out_coop_id_lv_i),
    .coop_tload_mst_done_out_valid_lv_i   (mst_done_out_valid_lv_i),
    .coop_tload_mst_done_in_coop_id_lv_o  (new_mst_done_in_coop_id_lv),
    .coop_tload_mst_done_in_valid_lv_o    (new_mst_done_in_valid_lv_o)
  );

endmodule : cosim_neigh_coop_tload_ports_tb
