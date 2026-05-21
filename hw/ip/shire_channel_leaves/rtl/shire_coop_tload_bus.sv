// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module shire_coop_tload_bus #(
  parameter int unsigned NumNeigh = shire_channel_leaves_pkg::NumNeigh,
  parameter int unsigned NumOtherNeigh = NumNeigh - 1,
  parameter int unsigned CoopIdWidth = neigh_pkg::ShireCoopIdSize
) (
  input  neigh_pkg::coop_tload_slv_rdy_req_t coop_tload_slv_rdy_out_data_i [NumNeigh-1:0],
  input  logic [NumNeigh-1:0]                coop_tload_slv_rdy_out_valid_i,

  output neigh_pkg::coop_tload_slv_rdy_req_t coop_tload_slv_rdy_in_data_o [NumNeigh-1:0][NumOtherNeigh-1:0],
  output logic [NumNeigh-1:0][NumOtherNeigh-1:0] coop_tload_slv_rdy_in_valid_o,

  input  logic [NumNeigh-1:0][CoopIdWidth-1:0]  coop_tload_mst_done_out_coop_id_i,
  input  logic [NumNeigh-1:0][NumOtherNeigh-1:0] coop_tload_mst_done_out_valid_i,

  output logic [NumNeigh-1:0][NumOtherNeigh-1:0][CoopIdWidth-1:0] coop_tload_mst_done_in_coop_id_o,
  output logic [NumNeigh-1:0][NumOtherNeigh-1:0] coop_tload_mst_done_in_valid_o
);

  generate
    for (genvar neigh_id = 0; neigh_id < NumNeigh; neigh_id++) begin : gen_neigh
      for (genvar other_neigh_id = 0; other_neigh_id < NumOtherNeigh; other_neigh_id++) begin : gen_other_neigh
        if (other_neigh_id >= neigh_id) begin : gen_after_or_equal
          assign coop_tload_slv_rdy_in_data_o[neigh_id][other_neigh_id] =
              coop_tload_slv_rdy_out_data_i[other_neigh_id+1];
          assign coop_tload_slv_rdy_in_valid_o[neigh_id][other_neigh_id] =
              coop_tload_slv_rdy_out_valid_i[other_neigh_id+1];
          assign coop_tload_mst_done_in_coop_id_o[neigh_id][other_neigh_id] =
              coop_tload_mst_done_out_coop_id_i[other_neigh_id+1];
          assign coop_tload_mst_done_in_valid_o[neigh_id][other_neigh_id] =
              coop_tload_mst_done_out_valid_i[other_neigh_id+1][neigh_id];
        end else begin : gen_before
          assign coop_tload_slv_rdy_in_data_o[neigh_id][other_neigh_id] =
              coop_tload_slv_rdy_out_data_i[other_neigh_id];
          assign coop_tload_slv_rdy_in_valid_o[neigh_id][other_neigh_id] =
              coop_tload_slv_rdy_out_valid_i[other_neigh_id];
          assign coop_tload_mst_done_in_coop_id_o[neigh_id][other_neigh_id] =
              coop_tload_mst_done_out_coop_id_i[other_neigh_id];
          assign coop_tload_mst_done_in_valid_o[neigh_id][other_neigh_id] =
              coop_tload_mst_done_out_valid_i[other_neigh_id][neigh_id-1];
        end
      end
    end
  endgenerate

endmodule
