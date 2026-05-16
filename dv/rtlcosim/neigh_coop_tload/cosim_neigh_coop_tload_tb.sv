// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_neigh_coop_tload_tb
  import etlink_pkg::*;
  import minion_pkg::*;
  import minion_dcache_pkg::*;
  import neigh_pkg::*;
(
  input  logic clk_i,
  input  logic rst_ni,

  input  logic [NumNeighWidth-1:0] neigh_id_i,
  input  logic [MinPerN*ShireCoopMissReqs*$bits(minion_dcache_pkg::et_link_minion_miss_req_info_t)-1:0] min_req_bits_i,
  input  logic [MinPerN*ShireCoopMissReqs-1:0] min_valid_bits_i,
  input  logic coop_tload_disable_next_i,
  input  logic coop_tload_req_ready_i,
  input  logic [NumOtherNeigh*$bits(neigh_pkg::coop_tload_slv_rdy_req_t)-1:0] slv_rdy_in_data_bits_i,
  input  logic [NumOtherNeigh-1:0] slv_rdy_in_valid_i,
  input  logic [NumOtherNeigh*ShireCoopIdSize-1:0] mst_done_in_coop_id_bits_i,
  input  logic [NumOtherNeigh-1:0] mst_done_in_valid_i,
  input  logic tag_table_cam_pop_i,
  input  logic [ShireCoopLoadIdSize-1:0] tag_table_cam_tag_i,

  output logic [MinPerN*ShireCoopMissReqs-1:0] orig_min_ready_bits_o,
  output logic [MinPerN*ShireCoopMissReqs-1:0] new_min_ready_bits_o,
  output logic [$bits(etlink_pkg::neigh_req_t)-1:0] orig_req_bits_o,
  output logic [$bits(etlink_pkg::neigh_req_t)-1:0] new_req_bits_o,
  output logic orig_req_bid_o,
  output logic new_req_bid_o,
  output logic [$bits(neigh_pkg::coop_tload_slv_rdy_req_t)-1:0] orig_slv_rdy_out_data_bits_o,
  output logic [$bits(neigh_pkg::coop_tload_slv_rdy_req_t)-1:0] new_slv_rdy_out_data_bits_o,
  output logic orig_slv_rdy_out_valid_o,
  output logic new_slv_rdy_out_valid_o,
  output logic [ShireCoopIdSize-1:0] orig_mst_done_out_coop_id_o,
  output logic [ShireCoopIdSize-1:0] new_mst_done_out_coop_id_o,
  output logic [NumOtherNeigh-1:0] orig_mst_done_out_valid_o,
  output logic [NumOtherNeigh-1:0] new_mst_done_out_valid_o,
  output logic [$bits(neigh_pkg::coop_tload_tag_table_data_t)-1:0] orig_cam_data_bits_o,
  output logic [$bits(neigh_pkg::coop_tload_tag_table_data_t)-1:0] new_cam_data_bits_o,
  output logic [$bits(neigh_pkg::coop_tload_dbg_sm_t)-1:0] orig_dbg_bits_o,
  output logic [$bits(neigh_pkg::coop_tload_dbg_sm_t)-1:0] new_dbg_bits_o
);

  localparam int unsigned ReqBits = $bits(minion_dcache_pkg::et_link_minion_miss_req_info_t);
  localparam int unsigned SlvRdyBits = $bits(neigh_pkg::coop_tload_slv_rdy_req_t);

  et_link_minion_miss_req_info_t [MinPerN-1:0][ShireCoopMissReqs-1:0] orig_min_req;
  et_link_neigh_req_info_t orig_req;
  coop_tload_slv_rdy_req_t [NumOtherNeigh-1:0] orig_slv_rdy_in_data;
  coop_tload_slv_rdy_req_t orig_slv_rdy_out_data;
  logic [NumOtherNeigh-1:0][ShireCoopIdSize-1:0] orig_mst_done_in_coop_id;
  coop_tload_tag_table_data_t orig_cam_data;
  coop_tload_dbg_sm_t orig_dbg;

  minion_dcache_pkg::et_link_minion_miss_req_info_t [MinPerN-1:0][ShireCoopMissReqs-1:0] new_min_req;
  etlink_pkg::neigh_req_t new_req;
  neigh_pkg::coop_tload_slv_rdy_req_t [NumOtherNeigh-1:0] new_slv_rdy_in_data;
  neigh_pkg::coop_tload_slv_rdy_req_t new_slv_rdy_out_data;
  logic [NumOtherNeigh-1:0][ShireCoopIdSize-1:0] new_mst_done_in_coop_id;
  neigh_pkg::coop_tload_tag_table_data_t new_cam_data;
  neigh_pkg::coop_tload_dbg_sm_t new_dbg;

  always_comb begin
    for (int unsigned min_i = 0; min_i < MinPerN; min_i++) begin
      for (int unsigned miss_i = 0; miss_i < ShireCoopMissReqs; miss_i++) begin
        int unsigned slot;
        slot = min_i*ShireCoopMissReqs + miss_i;
        orig_min_req[min_i][miss_i] = et_link_minion_miss_req_info_t'(
            min_req_bits_i[slot*ReqBits +: ReqBits]);
        new_min_req[min_i][miss_i] = minion_dcache_pkg::et_link_minion_miss_req_info_t'(
            min_req_bits_i[slot*ReqBits +: ReqBits]);
      end
    end

    for (int unsigned n = 0; n < NumOtherNeigh; n++) begin
      orig_slv_rdy_in_data[n] = coop_tload_slv_rdy_req_t'(
          slv_rdy_in_data_bits_i[n*SlvRdyBits +: SlvRdyBits]);
      new_slv_rdy_in_data[n] = neigh_pkg::coop_tload_slv_rdy_req_t'(
          slv_rdy_in_data_bits_i[n*SlvRdyBits +: SlvRdyBits]);
      orig_mst_done_in_coop_id[n] = mst_done_in_coop_id_bits_i[n*ShireCoopIdSize +: ShireCoopIdSize];
      new_mst_done_in_coop_id[n] = mst_done_in_coop_id_bits_i[n*ShireCoopIdSize +: ShireCoopIdSize];
    end

    orig_req_bits_o = orig_req;
    new_req_bits_o = new_req;
    orig_slv_rdy_out_data_bits_o = orig_slv_rdy_out_data;
    new_slv_rdy_out_data_bits_o = new_slv_rdy_out_data;
    orig_cam_data_bits_o = orig_cam_data;
    new_cam_data_bits_o = new_cam_data;
    orig_dbg_bits_o = orig_dbg;
    new_dbg_bits_o = new_dbg;
  end

  neigh_coop_tload_orig u_orig (
    .clock                         (clk_i),
    .reset                         (!rst_ni),
    .neigh_id                      (neigh_id_i),
    .min_coop_miss_req             (orig_min_req),
    .min_coop_miss_req_valid       (min_valid_bits_i),
    .min_coop_miss_req_ready       (orig_min_ready_bits_o),
    .coop_tload_disable_next       (coop_tload_disable_next_i),
    .coop_tload_req                (orig_req),
    .coop_tload_req_bid            (orig_req_bid_o),
    .coop_tload_req_ready          (coop_tload_req_ready_i),
    .coop_tload_slv_rdy_out_data   (orig_slv_rdy_out_data),
    .coop_tload_slv_rdy_out_valid  (orig_slv_rdy_out_valid_o),
    .coop_tload_slv_rdy_in_data    (orig_slv_rdy_in_data),
    .coop_tload_slv_rdy_in_valid   (slv_rdy_in_valid_i),
    .coop_tload_mst_done_out_coop_id (orig_mst_done_out_coop_id_o),
    .coop_tload_mst_done_out_valid   (orig_mst_done_out_valid_o),
    .coop_tload_mst_done_in_coop_id  (orig_mst_done_in_coop_id),
    .coop_tload_mst_done_in_valid    (mst_done_in_valid_i),
    .tag_table_cam_pop             (tag_table_cam_pop_i),
    .tag_table_cam_tag             (tag_table_cam_tag_i),
    .tag_table_cam_data            (orig_cam_data),
    .dbg_sm_signals                (orig_dbg)
  );

  neigh_coop_tload u_new (
    .clk_i,
    .rst_ni,
    .neigh_id_i                    (neigh_id_i),
    .min_coop_miss_req_i           (new_min_req),
    .min_coop_miss_req_valid_i     (min_valid_bits_i),
    .min_coop_miss_req_ready_o     (new_min_ready_bits_o),
    .coop_tload_disable_next_i     (coop_tload_disable_next_i),
    .coop_tload_req_o              (new_req),
    .coop_tload_req_bid_o          (new_req_bid_o),
    .coop_tload_req_ready_i        (coop_tload_req_ready_i),
    .coop_tload_slv_rdy_out_data_o (new_slv_rdy_out_data),
    .coop_tload_slv_rdy_out_valid_o(new_slv_rdy_out_valid_o),
    .coop_tload_slv_rdy_in_data_i  (new_slv_rdy_in_data),
    .coop_tload_slv_rdy_in_valid_i (slv_rdy_in_valid_i),
    .coop_tload_mst_done_out_coop_id_o (new_mst_done_out_coop_id_o),
    .coop_tload_mst_done_out_valid_o   (new_mst_done_out_valid_o),
    .coop_tload_mst_done_in_coop_id_i  (new_mst_done_in_coop_id),
    .coop_tload_mst_done_in_valid_i    (mst_done_in_valid_i),
    .tag_table_cam_pop_i           (tag_table_cam_pop_i),
    .tag_table_cam_tag_i           (tag_table_cam_tag_i),
    .tag_table_cam_data_o          (new_cam_data),
    .dbg_sm_signals_o              (new_dbg)
  );

endmodule : cosim_neigh_coop_tload_tb
