// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_coop_tload (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [neigh_pkg::NumNeighWidth-1:0] neigh_id_i,

  input  minion_dcache_pkg::et_link_minion_miss_req_info_t
      [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0] min_coop_miss_req_i,
  input  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0] min_coop_miss_req_valid_i,
  output logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0] min_coop_miss_req_ready_o,

  input  logic coop_tload_disable_next_i,
  output etlink_pkg::neigh_req_t coop_tload_req_o,
  output logic coop_tload_req_bid_o,
  input  logic coop_tload_req_ready_i,

  output neigh_pkg::coop_tload_slv_rdy_req_t coop_tload_slv_rdy_out_data_o,
  output logic coop_tload_slv_rdy_out_valid_o,

  input  neigh_pkg::coop_tload_slv_rdy_req_t [neigh_pkg::NumOtherNeigh-1:0] coop_tload_slv_rdy_in_data_i,
  input  logic [neigh_pkg::NumOtherNeigh-1:0] coop_tload_slv_rdy_in_valid_i,

  output logic [neigh_pkg::ShireCoopIdSize-1:0] coop_tload_mst_done_out_coop_id_o,
  output logic [neigh_pkg::NumOtherNeigh-1:0] coop_tload_mst_done_out_valid_o,

  input  logic [neigh_pkg::NumOtherNeigh-1:0][neigh_pkg::ShireCoopIdSize-1:0] coop_tload_mst_done_in_coop_id_i,
  input  logic [neigh_pkg::NumOtherNeigh-1:0] coop_tload_mst_done_in_valid_i,

  input  logic tag_table_cam_pop_i,
  input  logic [neigh_pkg::ShireCoopLoadIdSize-1:0] tag_table_cam_tag_i,
  output neigh_pkg::coop_tload_tag_table_data_t tag_table_cam_data_o,

  output neigh_pkg::coop_tload_dbg_sm_t dbg_sm_signals_o
);

  localparam int unsigned TagTableSlvPort = 0;
  localparam int unsigned TagTableMstPort = 1;
  localparam int unsigned MinionIdxWidth = (neigh_pkg::MinPerN > 1) ?
                                           $clog2(neigh_pkg::MinPerN) : 1;
  localparam int unsigned MissIdxWidth = (neigh_pkg::ShireCoopMissReqs > 1) ?
                                         $clog2(neigh_pkg::ShireCoopMissReqs) : 1;
  localparam int unsigned MinionMissDataWidth = $bits(minion_pkg::tensorcoop_control);

  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0]
      [neigh_pkg::ShireCoopIdSize-1:0] local_coop_id;
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0]
      [neigh_pkg::NumOtherNeigh-1:0] local_other_neigh_mask;
  /* verilator lint_off UNOPTFLAT */  // Mirrors the original symmetric minion-id match matrix.
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0]
      [neigh_pkg::MinPerN-1:0] local_coop_id_match;
  /* verilator lint_on UNOPTFLAT */

  minion_dcache_pkg::et_link_minion_miss_req_info_t
      [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0] local_ff_req_q;
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0] local_ff_req_valid;
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0] local_ff_available_q;
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0] local_ff_available_d;
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0] local_ff_in_accepted;
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0] local_ff_out_accepted;

  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0] slv_per_miss_bid_q;
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0] slv_per_miss_bid_d;
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0]
      [neigh_pkg::ShireCoopMissIdSize-1:0] slv_per_miss_miss_id_q;
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0]
      [neigh_pkg::ShireCoopMissIdSize-1:0] slv_per_miss_miss_id_d;
  neigh_pkg::coop_tload_slv_req_t
      [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0] slv_per_miss_data_in_q;
  neigh_pkg::coop_tload_slv_req_t
      [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0] slv_per_miss_data_in_d;
  neigh_pkg::coop_tload_slv_req_t [neigh_pkg::MinPerN-1:0] slv_per_miss_data_winner;
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0] slv_per_miss_grant;
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0] slv_per_miss_ready;

  logic [neigh_pkg::MinPerN-1:0] slv_per_min_bid;
  logic slv_per_min_stall;
  neigh_pkg::coop_tload_slv_req_t [neigh_pkg::MinPerN-1:0] slv_per_min_data_in;
  logic [neigh_pkg::MinPerN-1:0] slv_per_min_grant;
  neigh_pkg::coop_tload_slv_req_t slv_per_min_data_winner;
  logic [MinionIdxWidth-1:0] slv_per_min_winner;

  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0] mst_per_miss_bid_q;
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0] mst_per_miss_bid_d;
  minion_dcache_pkg::et_link_minion_miss_req_info_t
      [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0] mst_per_miss_data_in_q;
  minion_dcache_pkg::et_link_minion_miss_req_info_t
      [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0] mst_per_miss_data_in_d;
  minion_dcache_pkg::et_link_minion_miss_req_info_t
      [neigh_pkg::MinPerN-1:0] mst_per_miss_data_winner;
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0] mst_per_miss_grant;
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0] mst_per_miss_ready;

  logic [neigh_pkg::MinPerN-1:0] mst_per_min_bid;
  logic mst_per_min_stall;
  minion_dcache_pkg::et_link_minion_miss_req_info_t
      [neigh_pkg::MinPerN-1:0] mst_per_min_data_in;
  logic [neigh_pkg::MinPerN-1:0] mst_per_min_grant;
  minion_dcache_pkg::et_link_minion_miss_req_info_t mst_per_min_data_winner;
  logic [neigh_pkg::NumNeigh-1:0] mst_per_min_winner_coop_neigh_mask;
  logic [neigh_pkg::NumOtherNeigh-1:0] mst_per_min_winner_other_neigh_mask;
  logic [neigh_pkg::ShireCoopIdSize-1:0] mst_per_min_winner_coop_id;
  logic [neigh_pkg::ShireCoopSeqIdSize-1:0] mst_per_min_winner_coop_seq_id;
  logic [neigh_pkg::MinPerN-1:0] mst_per_min_winner_coop_min_mask;
  logic [neigh_pkg::ShireCoopLoadIdSize-1:0] mst_per_min_winner_tag;
  logic [MinionIdxWidth-1:0] mst_per_min_winner;

  logic coop_tload_req_ready_int;

  logic [neigh_pkg::ShireCoopNumIds-1:0][neigh_pkg::ShireCoopRdyCredsWidth-1:0]
      rdy_avail_creds_q;
  logic [neigh_pkg::ShireCoopNumIds-1:0][neigh_pkg::ShireCoopRdyCredsWidth-1:0]
      rdy_avail_creds_d;
  logic [neigh_pkg::ShireCoopNumIds-1:0] rdy_avail_creds_up_inc;
  logic [neigh_pkg::ShireCoopNumIds-1:0] rdy_avail_creds_up_dec;
  logic [neigh_pkg::ShireCoopNumIds-1:0] rdy_avail_creds_up;
  logic [neigh_pkg::NumOtherNeigh-1:0][neigh_pkg::ShireCoopIdSize-1:0]
      rdy_avail_creds_inc_coop_id_q;
  logic [neigh_pkg::NumOtherNeigh-1:0] rdy_avail_creds_inc_valid_q;
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0]
      [neigh_pkg::ShireCoopIdSize-1:0] rdy_avail_creds_dec_coop_id_q;
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0]
      rdy_avail_creds_dec_valid_q;

  logic [neigh_pkg::ShireCoopNumIds-1:0][neigh_pkg::NumOtherNeigh-1:0]
      [neigh_pkg::ShireCoopRdyCredsWidth-1:0] coop_tload_ext_rdy_q;
  logic [neigh_pkg::ShireCoopNumIds-1:0][neigh_pkg::NumOtherNeigh-1:0]
      [neigh_pkg::ShireCoopRdyCredsWidth-1:0] coop_tload_ext_rdy_d;
  logic [neigh_pkg::ShireCoopNumIds-1:0][neigh_pkg::NumOtherNeigh-1:0]
      coop_tload_ext_rdy_up_inc;
  logic [neigh_pkg::ShireCoopNumIds-1:0][neigh_pkg::NumOtherNeigh-1:0]
      coop_tload_ext_rdy_up_dec;
  logic [neigh_pkg::ShireCoopNumIds-1:0][neigh_pkg::NumOtherNeigh-1:0]
      coop_tload_ext_rdy_up;
  logic [neigh_pkg::NumOtherNeigh-1:0][neigh_pkg::ShireCoopIdSize-1:0]
      coop_tload_ext_rdy_inc_coop_id_q;
  logic [neigh_pkg::NumOtherNeigh-1:0] coop_tload_ext_rdy_inc_valid_q;
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0]
      [neigh_pkg::ShireCoopIdSize-1:0] coop_tload_ext_rdy_dec_coop_id_q;
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0]
      [neigh_pkg::NumOtherNeigh-1:0] coop_tload_ext_rdy_dec_valid_q;

  neigh_pkg::coop_tload_slv_rdy_req_t coop_tload_slv_rdy_out_data_d;
  logic coop_tload_slv_rdy_out_valid_d;
  logic [neigh_pkg::ShireCoopIdSize-1:0] coop_tload_mst_done_out_coop_id_d;
  logic [neigh_pkg::NumOtherNeigh-1:0] coop_tload_mst_done_out_valid_d;

  neigh_pkg::coop_tload_tag_table_req_t [1:0] tag_table_wr_req;
  logic [1:0] tag_table_wr_valid;
  logic [1:0] tag_table_wr_ready;

  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0]
      coop_tload_local_rdy;
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0]
      [neigh_pkg::NumNeighWidth-1:0] master_neigh;
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0]
      slv_has_rdy_creds;
  logic [neigh_pkg::MinPerN-1:0][neigh_pkg::ShireCoopMissReqs-1:0]
      [neigh_pkg::NumOtherNeigh-1:0] mst_ext_is_rdy;

  generate
    for (genvar min_i = 0; min_i < neigh_pkg::MinPerN; min_i++) begin : gen_minions
      logic [MissIdxWidth-1:0] slv_per_miss_winner;
      logic [MissIdxWidth-1:0] mst_per_miss_winner;

      for (genvar miss_i = 0; miss_i < neigh_pkg::ShireCoopMissReqs; miss_i++) begin : gen_miss_ff
        logic [neigh_pkg::MinPerN-1:0] local_coop_min_mask;
        logic [neigh_pkg::ShireCoopSeqIdSize-1:0] local_coop_seq_id;
        logic [neigh_pkg::NumNeigh-1:0] local_coop_neigh_mask;
        logic mst_all_ext_are_rdy;
        logic [neigh_pkg::MinPerN-1:0] slv_per_miss_coop_min_mask;
        logic [neigh_pkg::MinPerN-1:0] mst_per_miss_coop_min_mask;

        assign min_coop_miss_req_ready_o[min_i][miss_i] = local_ff_available_q[min_i][miss_i];
        assign local_ff_in_accepted[min_i][miss_i] =
            min_coop_miss_req_valid_i[min_i][miss_i] & local_ff_available_q[min_i][miss_i];

        always_comb begin
          if (local_ff_available_q[min_i][miss_i]) begin
            local_ff_available_d[min_i][miss_i] = ~min_coop_miss_req_valid_i[min_i][miss_i];
          end else begin
            local_ff_available_d[min_i][miss_i] = local_ff_out_accepted[min_i][miss_i];
          end
        end

        assign local_ff_req_valid[min_i][miss_i] = ~local_ff_available_q[min_i][miss_i];
        assign local_ff_out_accepted[min_i][miss_i] =
            (slv_per_miss_bid_d[min_i][miss_i] & slv_per_miss_ready[min_i][miss_i]) |
            (mst_per_miss_bid_d[min_i][miss_i] & mst_per_miss_ready[min_i][miss_i]);

        always_ff @(posedge clk_i or negedge rst_ni) begin
          if (!rst_ni) begin
            local_ff_available_q[min_i][miss_i] <= 1'b1;
          end else begin
            local_ff_available_q[min_i][miss_i] <= local_ff_available_d[min_i][miss_i];
          end
        end

        // Original EN_FF storage has no reset; preserve the unreset request register.
        always_ff @(posedge clk_i) begin
          if (local_ff_in_accepted[min_i][miss_i]) begin
            local_ff_req_q[min_i][miss_i] <= min_coop_miss_req_i[min_i][miss_i];
          end
        end

        assign local_coop_min_mask = local_ff_req_q[min_i][miss_i]
            .data[neigh_pkg::ShireCoopMinMaskStart +: neigh_pkg::ShireCoopMinMaskSize];
        assign local_coop_seq_id = local_ff_req_q[min_i][miss_i]
            .data[neigh_pkg::ShireCoopSeqIdStart +: neigh_pkg::ShireCoopSeqIdSize];
        assign local_coop_id[min_i][miss_i] = local_ff_req_q[min_i][miss_i]
            .data[neigh_pkg::ShireCoopIdStart +: neigh_pkg::ShireCoopIdSize];
        assign local_coop_neigh_mask = local_ff_req_q[min_i][miss_i]
            .data[neigh_pkg::ShireCoopNeighMaskStart +: neigh_pkg::ShireCoopNeighMaskSize];
        assign local_other_neigh_mask[min_i][miss_i] = other_neigh_mask_fn(
            local_coop_neigh_mask, neigh_id_i);

        /* verilator lint_off UNSIGNED */  // min_i is a generate constant; min_i==0 makes the lower-side comparison constant in that instance.
        always_comb begin
          local_coop_id_match[min_i][miss_i] = '0;

          for (int unsigned min_j = 0; min_j < neigh_pkg::MinPerN; min_j++) begin
            if (min_j < min_i) begin
              local_coop_id_match[min_i][miss_i][min_j] =
                  local_coop_id[min_i][miss_i] == local_coop_id[min_j][miss_i];
            end else if (min_j > min_i) begin
              local_coop_id_match[min_i][miss_i][min_j] =
                  local_coop_id_match[min_j][miss_i][min_i];
            end
          end
        end
        /* verilator lint_on UNSIGNED */

        always_comb begin
          coop_tload_local_rdy[min_i][miss_i] = local_ff_req_valid[min_i][miss_i];
          for (int unsigned min_j = 0; min_j < neigh_pkg::MinPerN; min_j++) begin
            if (min_i != min_j) begin
              coop_tload_local_rdy[min_i][miss_i] =
                  coop_tload_local_rdy[min_i][miss_i] &
                  (~local_coop_min_mask[min_j] |
                   (local_ff_req_valid[min_j][miss_i] &
                    local_coop_id_match[min_i][miss_i][min_j]));
            end
          end
        end

        assign master_neigh[min_i][miss_i] = master_neigh_fn(
            local_ff_req_q[min_i][miss_i].address, local_coop_neigh_mask);

        assign slv_has_rdy_creds[min_i][miss_i] =
            rdy_avail_creds_q[local_coop_id[min_i][miss_i]] != '0;

        always_ff @(posedge clk_i or negedge rst_ni) begin
          if (!rst_ni) begin
            slv_per_miss_bid_q[min_i][miss_i] <= 1'b0;
          end else if (slv_per_miss_ready[min_i][miss_i]) begin
            slv_per_miss_bid_q[min_i][miss_i] <= slv_per_miss_bid_d[min_i][miss_i];
          end
        end

        // Original EN_FF storage has no reset; preserve unreset payload registers.
        always_ff @(posedge clk_i) begin
          if (slv_per_miss_ready[min_i][miss_i] & slv_per_miss_bid_d[min_i][miss_i]) begin
            slv_per_miss_miss_id_q[min_i][miss_i] <= slv_per_miss_miss_id_d[min_i][miss_i];
            slv_per_miss_data_in_q[min_i][miss_i] <= slv_per_miss_data_in_d[min_i][miss_i];
          end
        end

        assign slv_per_miss_bid_d[min_i][miss_i] =
            coop_tload_local_rdy[min_i][miss_i] &
            (master_neigh[min_i][miss_i] != neigh_id_i) &
            slv_has_rdy_creds[min_i][miss_i];
        assign slv_per_miss_miss_id_d[min_i][miss_i] =
            local_ff_req_q[min_i][miss_i].id[neigh_pkg::ShireCoopMissIdSize-1:0];
        assign slv_per_miss_data_in_d[min_i][miss_i] = '{
          master_neigh:  master_neigh[min_i][miss_i],
          coop_id:       local_coop_id[min_i][miss_i],
          coop_seq_id:   local_coop_seq_id,
          coop_min_mask: local_coop_min_mask
        };

        assign slv_per_miss_coop_min_mask =
            slv_per_miss_data_in_q[min_i][miss_i].coop_min_mask;
        assign slv_per_miss_ready[min_i][miss_i] =
            ((|(slv_per_min_grant & slv_per_miss_coop_min_mask)) &
             slv_per_miss_grant[slv_per_min_winner][miss_i]) |
            ~slv_per_miss_bid_q[min_i][miss_i];

        always_comb begin
          for (int unsigned neigh_i = 0; neigh_i < neigh_pkg::NumOtherNeigh; neigh_i++) begin
            mst_ext_is_rdy[min_i][miss_i][neigh_i] =
                coop_tload_ext_rdy_q[local_coop_id[min_i][miss_i]][neigh_i] != '0;
          end
        end
        assign mst_all_ext_are_rdy =
            &(mst_ext_is_rdy[min_i][miss_i] | ~local_other_neigh_mask[min_i][miss_i]);

        always_ff @(posedge clk_i or negedge rst_ni) begin
          if (!rst_ni) begin
            mst_per_miss_bid_q[min_i][miss_i] <= 1'b0;
          end else if (mst_per_miss_ready[min_i][miss_i]) begin
            mst_per_miss_bid_q[min_i][miss_i] <= mst_per_miss_bid_d[min_i][miss_i];
          end
        end

        // Original EN_FF storage has no reset; preserve the unreset payload register.
        always_ff @(posedge clk_i) begin
          if (mst_per_miss_ready[min_i][miss_i] & mst_per_miss_bid_d[min_i][miss_i]) begin
            mst_per_miss_data_in_q[min_i][miss_i] <= mst_per_miss_data_in_d[min_i][miss_i];
          end
        end

        assign mst_per_miss_bid_d[min_i][miss_i] =
            coop_tload_local_rdy[min_i][miss_i] &
            (master_neigh[min_i][miss_i] == neigh_id_i) &
            mst_all_ext_are_rdy;
        assign mst_per_miss_data_in_d[min_i][miss_i] = local_ff_req_q[min_i][miss_i];

        assign mst_per_miss_coop_min_mask = mst_per_miss_data_in_q[min_i][miss_i]
            .data[neigh_pkg::ShireCoopMinMaskStart +: neigh_pkg::ShireCoopMinMaskSize];
        assign mst_per_miss_ready[min_i][miss_i] =
            ((|(mst_per_min_grant & mst_per_miss_coop_min_mask)) &
             mst_per_miss_grant[mst_per_min_winner][miss_i]) |
            ~mst_per_miss_bid_q[min_i][miss_i];
      end

      prim_arb_lru #(
        .NumClients   (neigh_pkg::ShireCoopMissReqs),
        .NumClientsLog(MissIdxWidth)
      ) u_slv_per_miss_arb (
        .clk_i    (clk_i),
        .rst_ni   (rst_ni),
        .bid_i    (slv_per_miss_bid_q[min_i]),
        .stall_i  (~slv_per_min_grant[min_i]),
        .grant_o  (slv_per_miss_grant[min_i]),
        .winner_o (slv_per_miss_winner)
      );

      assign slv_per_miss_data_winner[min_i] =
          slv_per_miss_data_in_q[min_i][slv_per_miss_winner];
      assign slv_per_min_bid[min_i] = |slv_per_miss_bid_q[min_i];
      assign slv_per_min_data_in[min_i] = slv_per_miss_data_winner[min_i];

      prim_arb_lru #(
        .NumClients   (neigh_pkg::ShireCoopMissReqs),
        .NumClientsLog(MissIdxWidth)
      ) u_mst_per_miss_arb (
        .clk_i    (clk_i),
        .rst_ni   (rst_ni),
        .bid_i    (mst_per_miss_bid_q[min_i]),
        .stall_i  (~mst_per_min_grant[min_i]),
        .grant_o  (mst_per_miss_grant[min_i]),
        .winner_o (mst_per_miss_winner)
      );

      assign mst_per_miss_data_winner[min_i] =
          mst_per_miss_data_in_q[min_i][mst_per_miss_winner];
      assign mst_per_min_bid[min_i] = |mst_per_miss_bid_q[min_i];
      assign mst_per_min_data_in[min_i] = mst_per_miss_data_winner[min_i];
    end
  endgenerate

  logic slv_per_min_valid;

  prim_arb_lru #(
    .NumClients   (neigh_pkg::MinPerN),
    .NumClientsLog(MinionIdxWidth)
  ) u_slv_per_min_arb (
    .clk_i    (clk_i),
    .rst_ni   (rst_ni),
    .bid_i    (slv_per_min_bid),
    .stall_i  (slv_per_min_stall),
    .grant_o  (slv_per_min_grant),
    .winner_o (slv_per_min_winner)
  );

  assign slv_per_min_stall = ~tag_table_wr_ready[TagTableSlvPort];
  assign slv_per_min_valid = |slv_per_min_bid;
  assign slv_per_min_data_winner = slv_per_min_data_in[slv_per_min_winner];

  assign coop_tload_slv_rdy_out_valid_d =
      slv_per_min_valid & tag_table_wr_ready[TagTableSlvPort];
  assign coop_tload_slv_rdy_out_data_d = '{
    dest_neigh: slv_per_min_data_winner.master_neigh,
    coop_id:    slv_per_min_data_winner.coop_id
  };

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      coop_tload_slv_rdy_out_valid_o <= 1'b0;
    end else begin
      coop_tload_slv_rdy_out_valid_o <= coop_tload_slv_rdy_out_valid_d;
    end
  end

  // Original EN_FF storage has no reset; preserve the unreset data register.
  always_ff @(posedge clk_i) begin
    if (coop_tload_slv_rdy_out_valid_d) begin
      coop_tload_slv_rdy_out_data_o <= coop_tload_slv_rdy_out_data_d;
    end
  end

  logic mst_per_min_valid;
  logic mst_per_min_accepted;

  prim_arb_lru #(
    .NumClients   (neigh_pkg::MinPerN),
    .NumClientsLog(MinionIdxWidth)
  ) u_mst_per_min_arb (
    .clk_i    (clk_i),
    .rst_ni   (rst_ni),
    .bid_i    (mst_per_min_bid),
    .stall_i  (mst_per_min_stall),
    .grant_o  (mst_per_min_grant),
    .winner_o (mst_per_min_winner)
  );

  assign mst_per_min_stall = ~coop_tload_req_ready_int | ~tag_table_wr_ready[TagTableMstPort];
  assign mst_per_min_valid = |mst_per_min_bid;
  assign mst_per_min_accepted = mst_per_min_valid & ~mst_per_min_stall;
  assign mst_per_min_data_winner = mst_per_min_data_in[mst_per_min_winner];

  assign mst_per_min_winner_coop_min_mask = mst_per_min_data_winner
      .data[neigh_pkg::ShireCoopMinMaskStart +: neigh_pkg::ShireCoopMinMaskSize];
  assign mst_per_min_winner_coop_seq_id = mst_per_min_data_winner
      .data[neigh_pkg::ShireCoopSeqIdStart +: neigh_pkg::ShireCoopSeqIdSize];
  assign mst_per_min_winner_coop_id = mst_per_min_data_winner
      .data[neigh_pkg::ShireCoopIdStart +: neigh_pkg::ShireCoopIdSize];
  assign mst_per_min_winner_coop_neigh_mask = mst_per_min_data_winner
      .data[neigh_pkg::ShireCoopNeighMaskStart +: neigh_pkg::ShireCoopNeighMaskSize];
  assign mst_per_min_winner_other_neigh_mask = other_neigh_mask_fn(
      mst_per_min_winner_coop_neigh_mask, neigh_id_i);
  assign mst_per_min_winner_tag = {mst_per_min_winner_coop_id, mst_per_min_winner_coop_seq_id};

  etlink_pkg::neigh_req_t coop_tload_req_d;
  logic coop_tload_req_bid_d;
  logic coop_tload_req_valid_q;
  logic coop_tload_req_valid_d;

  assign coop_tload_req_valid_d = mst_per_min_valid & tag_table_wr_ready[TagTableMstPort];
  assign coop_tload_req_bid_d =
      (coop_tload_req_ready_int ? coop_tload_req_valid_d : coop_tload_req_valid_q) &
      ~coop_tload_disable_next_i;

  always_comb begin
    coop_tload_req_d.id = mst_per_min_winner_tag;
    coop_tload_req_d.source = '0;
    if (mst_per_min_winner_other_neigh_mask == '0) begin
      coop_tload_req_d.wdata = 1'b0;
      coop_tload_req_d.opcode = etlink_pkg::ReqRead;
      coop_tload_req_d.qwen = 4'b0000;
    end else begin
      coop_tload_req_d.wdata = 1'b1;
      coop_tload_req_d.opcode = etlink_pkg::ReqReadCoop;
      coop_tload_req_d.qwen = 4'b0001;
    end
    coop_tload_req_d.subopcode = mst_per_min_data_winner.subopcode;
    coop_tload_req_d.address = mst_per_min_data_winner.address;
    coop_tload_req_d.data = {{(etlink_pkg::NeighDataSize-MinionMissDataWidth){1'b0}},
                             mst_per_min_data_winner.data};
    coop_tload_req_d.size = etlink_pkg::size_e'(mst_per_min_data_winner.size);
  end

  assign coop_tload_req_ready_int = coop_tload_req_ready_i | ~coop_tload_req_valid_q;

  // Original EN_FF storage has no reset; preserve the unreset request-data register.
  always_ff @(posedge clk_i) begin
    if (coop_tload_req_ready_int & coop_tload_req_valid_d) begin
      coop_tload_req_o <= coop_tload_req_d;
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      coop_tload_req_valid_q <= 1'b0;
      coop_tload_req_bid_o   <= 1'b0;
    end else begin
      if (coop_tload_req_ready_int) begin
        coop_tload_req_valid_q <= coop_tload_req_valid_d;
      end
      coop_tload_req_bid_o <= coop_tload_req_bid_d;
    end
  end

  assign coop_tload_mst_done_out_valid_d =
      {neigh_pkg::NumOtherNeigh{mst_per_min_accepted}} & mst_per_min_winner_other_neigh_mask;
  assign coop_tload_mst_done_out_coop_id_d = mst_per_min_winner_coop_id;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      coop_tload_mst_done_out_valid_o <= '0;
    end else begin
      coop_tload_mst_done_out_valid_o <= coop_tload_mst_done_out_valid_d;
    end
  end

  // Original EN_FF storage has no reset; preserve the unreset data register.
  always_ff @(posedge clk_i) begin
    if (|coop_tload_mst_done_out_valid_d) begin
      coop_tload_mst_done_out_coop_id_o <= coop_tload_mst_done_out_coop_id_d;
    end
  end

  generate
    for (genvar neigh_i = 0; neigh_i < neigh_pkg::NumOtherNeigh; neigh_i++) begin : gen_mst_done_in
      logic [neigh_pkg::ShireCoopIdSize-1:0] rdy_avail_creds_inc_coop_id_d;
      logic rdy_avail_creds_inc_valid_d;

      assign rdy_avail_creds_inc_coop_id_d = coop_tload_mst_done_in_coop_id_i[neigh_i];
      assign rdy_avail_creds_inc_valid_d = coop_tload_mst_done_in_valid_i[neigh_i];

      always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
          rdy_avail_creds_inc_valid_q[neigh_i] <= 1'b0;
        end else begin
          rdy_avail_creds_inc_valid_q[neigh_i] <= rdy_avail_creds_inc_valid_d;
        end
      end

      // Original EN_FF storage has no reset; preserve the unreset data register.
      always_ff @(posedge clk_i) begin
        if (rdy_avail_creds_inc_valid_d) begin
          rdy_avail_creds_inc_coop_id_q[neigh_i] <= rdy_avail_creds_inc_coop_id_d;
        end
      end
    end
  endgenerate

  generate
    for (genvar i = 0; i < neigh_pkg::ShireCoopNumIds; i++) begin : gen_rdy_avail_creds
      always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
          rdy_avail_creds_q[i] <= neigh_pkg::ShireCoopRdyCreds;
        end else if (rdy_avail_creds_up[i]) begin
          rdy_avail_creds_q[i] <= rdy_avail_creds_d[i];
        end
      end
    end
  endgenerate

  generate
    for (genvar min_i = 0; min_i < neigh_pkg::MinPerN; min_i++) begin : gen_rdy_avail_creds_dec_min
      for (genvar miss_i = 0; miss_i < neigh_pkg::ShireCoopMissReqs; miss_i++) begin : gen_rdy_avail_creds_dec_miss
        logic [neigh_pkg::ShireCoopIdSize-1:0] rdy_avail_creds_dec_coop_id_d;
        logic rdy_avail_creds_dec_valid_d;

        assign rdy_avail_creds_dec_coop_id_d = local_coop_id[min_i][miss_i];
        assign rdy_avail_creds_dec_valid_d =
            slv_per_miss_bid_d[min_i][miss_i] & slv_per_miss_ready[min_i][miss_i];

        always_ff @(posedge clk_i or negedge rst_ni) begin
          if (!rst_ni) begin
            rdy_avail_creds_dec_valid_q[min_i][miss_i] <= 1'b0;
          end else begin
            rdy_avail_creds_dec_valid_q[min_i][miss_i] <= rdy_avail_creds_dec_valid_d;
          end
        end

        // Original EN_FF storage has no reset; preserve the unreset data register.
        always_ff @(posedge clk_i) begin
          if (rdy_avail_creds_dec_valid_d) begin
            rdy_avail_creds_dec_coop_id_q[min_i][miss_i] <= rdy_avail_creds_dec_coop_id_d;
          end
        end
      end
    end
  endgenerate

  always_comb begin
    rdy_avail_creds_up_dec = '0;
    rdy_avail_creds_up_inc = '0;

    for (int unsigned i = 0; i < neigh_pkg::ShireCoopNumIds; i++) begin
      rdy_avail_creds_d[i] = rdy_avail_creds_q[i] - 1'b1;
    end

    for (int unsigned min_i = 0; min_i < neigh_pkg::MinPerN; min_i++) begin
      for (int unsigned miss_i = 0; miss_i < neigh_pkg::ShireCoopMissReqs; miss_i++) begin
        if (rdy_avail_creds_dec_valid_q[min_i][miss_i]) begin
          rdy_avail_creds_up_dec[rdy_avail_creds_dec_coop_id_q[min_i][miss_i]] = 1'b1;
          rdy_avail_creds_d[rdy_avail_creds_dec_coop_id_q[min_i][miss_i]] =
              rdy_avail_creds_q[rdy_avail_creds_dec_coop_id_q[min_i][miss_i]] - 1'b1;
        end
      end
    end

    for (int unsigned neigh_i = 0; neigh_i < neigh_pkg::NumOtherNeigh; neigh_i++) begin
      if (rdy_avail_creds_inc_valid_q[neigh_i]) begin
        rdy_avail_creds_up_inc[rdy_avail_creds_inc_coop_id_q[neigh_i]] = 1'b1;
        rdy_avail_creds_d[rdy_avail_creds_inc_coop_id_q[neigh_i]] =
            rdy_avail_creds_q[rdy_avail_creds_inc_coop_id_q[neigh_i]] + 1'b1;
      end
    end
  end

  assign rdy_avail_creds_up = rdy_avail_creds_up_dec ^ rdy_avail_creds_up_inc;

  logic unused_req_fields;
  assign unused_req_fields = ^{1'b0, mst_per_min_data_winner.id, mst_per_min_data_winner.source,
                               mst_per_min_data_winner.wdata, mst_per_min_data_winner.opcode,
                               mst_per_min_data_winner.qwen};

  generate
    for (genvar neigh_i = 0; neigh_i < neigh_pkg::NumOtherNeigh; neigh_i++) begin : gen_slv_rdy_in
      logic [neigh_pkg::ShireCoopIdSize-1:0] coop_tload_ext_rdy_inc_coop_id_d;
      logic coop_tload_ext_rdy_inc_valid_d;

      assign coop_tload_ext_rdy_inc_coop_id_d = coop_tload_slv_rdy_in_data_i[neigh_i].coop_id;
      assign coop_tload_ext_rdy_inc_valid_d =
          coop_tload_slv_rdy_in_valid_i[neigh_i] &
          (coop_tload_slv_rdy_in_data_i[neigh_i].dest_neigh == neigh_id_i);

      always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
          coop_tload_ext_rdy_inc_valid_q[neigh_i] <= 1'b0;
        end else begin
          coop_tload_ext_rdy_inc_valid_q[neigh_i] <= coop_tload_ext_rdy_inc_valid_d;
        end
      end

      // Original EN_FF storage has no reset; preserve the unreset data register.
      always_ff @(posedge clk_i) begin
        if (coop_tload_ext_rdy_inc_valid_d) begin
          coop_tload_ext_rdy_inc_coop_id_q[neigh_i] <= coop_tload_ext_rdy_inc_coop_id_d;
        end
      end
    end
  endgenerate

  generate
    for (genvar i = 0; i < neigh_pkg::ShireCoopNumIds; i++) begin : gen_ext_rdy_coop_id
      for (genvar neigh_i = 0; neigh_i < neigh_pkg::NumOtherNeigh; neigh_i++) begin : gen_ext_rdy_neigh
        always_ff @(posedge clk_i or negedge rst_ni) begin
          if (!rst_ni) begin
            coop_tload_ext_rdy_q[i][neigh_i] <= '0;
          end else if (coop_tload_ext_rdy_up[i][neigh_i]) begin
            coop_tload_ext_rdy_q[i][neigh_i] <= coop_tload_ext_rdy_d[i][neigh_i];
          end
        end
      end
    end
  endgenerate

  generate
    for (genvar min_i = 0; min_i < neigh_pkg::MinPerN; min_i++) begin : gen_ext_rdy_dec_min
      for (genvar miss_i = 0; miss_i < neigh_pkg::ShireCoopMissReqs; miss_i++) begin : gen_ext_rdy_dec_miss
        logic [neigh_pkg::ShireCoopIdSize-1:0] coop_tload_ext_rdy_dec_coop_id_d;
        logic [neigh_pkg::NumOtherNeigh-1:0] coop_tload_ext_rdy_dec_valid_d;

        assign coop_tload_ext_rdy_dec_coop_id_d = local_coop_id[min_i][miss_i];
        assign coop_tload_ext_rdy_dec_valid_d =
            {neigh_pkg::NumOtherNeigh{mst_per_miss_bid_d[min_i][miss_i] &
                                      mst_per_miss_ready[min_i][miss_i]}} &
            local_other_neigh_mask[min_i][miss_i];

        always_ff @(posedge clk_i or negedge rst_ni) begin
          if (!rst_ni) begin
            coop_tload_ext_rdy_dec_valid_q[min_i][miss_i] <= '0;
          end else begin
            coop_tload_ext_rdy_dec_valid_q[min_i][miss_i] <= coop_tload_ext_rdy_dec_valid_d;
          end
        end

        // Original EN_FF storage has no reset; preserve the unreset data register.
        always_ff @(posedge clk_i) begin
          if (|coop_tload_ext_rdy_dec_valid_d) begin
            coop_tload_ext_rdy_dec_coop_id_q[min_i][miss_i] <= coop_tload_ext_rdy_dec_coop_id_d;
          end
        end
      end
    end
  endgenerate

  always_comb begin
    coop_tload_ext_rdy_up_inc = '0;
    coop_tload_ext_rdy_up_dec = '0;

    for (int unsigned neigh_i = 0; neigh_i < neigh_pkg::NumOtherNeigh; neigh_i++) begin
      for (int unsigned i = 0; i < neigh_pkg::ShireCoopNumIds; i++) begin
        coop_tload_ext_rdy_d[i][neigh_i] = coop_tload_ext_rdy_q[i][neigh_i] + 1'b1;
      end
    end

    for (int unsigned neigh_i = 0; neigh_i < neigh_pkg::NumOtherNeigh; neigh_i++) begin
      if (coop_tload_ext_rdy_inc_valid_q[neigh_i]) begin
        coop_tload_ext_rdy_up_inc[coop_tload_ext_rdy_inc_coop_id_q[neigh_i]][neigh_i] = 1'b1;
        coop_tload_ext_rdy_d[coop_tload_ext_rdy_inc_coop_id_q[neigh_i]][neigh_i] =
            coop_tload_ext_rdy_q[coop_tload_ext_rdy_inc_coop_id_q[neigh_i]][neigh_i] + 1'b1;
      end

      for (int unsigned min_i = 0; min_i < neigh_pkg::MinPerN; min_i++) begin
        for (int unsigned miss_i = 0; miss_i < neigh_pkg::ShireCoopMissReqs; miss_i++) begin
          if (coop_tload_ext_rdy_dec_valid_q[min_i][miss_i][neigh_i]) begin
            coop_tload_ext_rdy_up_dec[coop_tload_ext_rdy_dec_coop_id_q[min_i][miss_i]][neigh_i] = 1'b1;
            coop_tload_ext_rdy_d[coop_tload_ext_rdy_dec_coop_id_q[min_i][miss_i]][neigh_i] =
                coop_tload_ext_rdy_q[coop_tload_ext_rdy_dec_coop_id_q[min_i][miss_i]][neigh_i] - 1'b1;
          end
        end
      end
    end
  end

  assign coop_tload_ext_rdy_up = coop_tload_ext_rdy_up_inc ^ coop_tload_ext_rdy_up_dec;

  always_comb begin
    tag_table_wr_req = '0;

    tag_table_wr_req[TagTableSlvPort].tag =
        {slv_per_min_data_winner.coop_id, slv_per_min_data_winner.coop_seq_id};
    tag_table_wr_req[TagTableSlvPort].data.coop_min_mask =
        slv_per_min_data_winner.coop_min_mask;
    for (int unsigned min_i = 0; min_i < neigh_pkg::MinPerN; min_i++) begin
      for (int unsigned miss_i = 0; miss_i < neigh_pkg::ShireCoopMissReqs; miss_i++) begin
        tag_table_wr_req[TagTableSlvPort].data.min_miss_id[min_i] =
            tag_table_wr_req[TagTableSlvPort].data.min_miss_id[min_i] |
            (slv_per_miss_miss_id_q[min_i][miss_i] &
             {neigh_pkg::ShireCoopMissIdSize{slv_per_miss_grant[slv_per_min_winner][miss_i]}});
      end
    end

    tag_table_wr_req[TagTableMstPort].tag = mst_per_min_winner_tag;
    tag_table_wr_req[TagTableMstPort].data.coop_min_mask =
        mst_per_min_winner_coop_min_mask;
    for (int unsigned min_i = 0; min_i < neigh_pkg::MinPerN; min_i++) begin
      for (int unsigned miss_i = 0; miss_i < neigh_pkg::ShireCoopMissReqs; miss_i++) begin
        tag_table_wr_req[TagTableMstPort].data.min_miss_id[min_i] =
            tag_table_wr_req[TagTableMstPort].data.min_miss_id[min_i] |
            (mst_per_miss_data_in_q[min_i][miss_i].id[neigh_pkg::ShireCoopMissIdSize-1:0] &
             {neigh_pkg::ShireCoopMissIdSize{mst_per_miss_grant[mst_per_min_winner][miss_i]}});
      end
    end
  end

  assign tag_table_wr_valid[TagTableSlvPort] = slv_per_min_valid;
  assign tag_table_wr_valid[TagTableMstPort] = mst_per_min_valid & coop_tload_req_ready_int;

  neigh_coop_tload_tag_table #(
    .Depth(neigh_pkg::ShireCoopTagTableDepth)
  ) u_coop_tload_tag_table (
    .clk_i,
    .rst_ni,
    .wr_req_i   (tag_table_wr_req),
    .wr_valid_i (tag_table_wr_valid),
    .wr_ready_o (tag_table_wr_ready),
    .cam_pop_i  (tag_table_cam_pop_i),
    .cam_tag_i  (tag_table_cam_tag_i),
    .cam_data_o (tag_table_cam_data_o)
  );

  always_comb begin
    dbg_sm_signals_o.mst_ext_is_rdy       = mst_ext_is_rdy;
    dbg_sm_signals_o.slv_has_rdy_creds    = slv_has_rdy_creds;
    dbg_sm_signals_o.master_neigh         = master_neigh;
    dbg_sm_signals_o.coop_tload_local_rdy = coop_tload_local_rdy;
  end

  /* verilator lint_off UNUSEDSIGNAL */  // The original master selector uses only address bits [7:6].
  function automatic logic [neigh_pkg::NumNeighWidth-1:0] master_neigh_fn(
      input logic [minion_pkg::PaSize-1:0] pa,
      input logic [neigh_pkg::NumNeigh-1:0] coop_neigh_mask);
    logic [neigh_pkg::NumNeighWidth-1:0] addr_sel;
    begin
      addr_sel = pa[neigh_pkg::ShireCoopMstNeighAddrStart +: neigh_pkg::ShireCoopMstNeighAddrSize];
      unique case (addr_sel)
        2'd0: begin
          master_neigh_fn = coop_neigh_mask[0] ? 2'd0 :
                            coop_neigh_mask[3] ? 2'd3 :
                            coop_neigh_mask[1] ? 2'd1 : 2'd2;
        end
        2'd1: begin
          master_neigh_fn = coop_neigh_mask[1] ? 2'd1 :
                            coop_neigh_mask[0] ? 2'd0 :
                            coop_neigh_mask[2] ? 2'd2 : 2'd3;
        end
        2'd2: begin
          master_neigh_fn = coop_neigh_mask[2] ? 2'd2 :
                            coop_neigh_mask[1] ? 2'd1 :
                            coop_neigh_mask[3] ? 2'd3 : 2'd0;
        end
        default: begin
          master_neigh_fn = coop_neigh_mask[3] ? 2'd3 :
                            coop_neigh_mask[2] ? 2'd2 :
                            coop_neigh_mask[0] ? 2'd0 : 2'd1;
        end
      endcase
    end
  endfunction
  /* verilator lint_on UNUSEDSIGNAL */

  function automatic logic [neigh_pkg::NumOtherNeigh-1:0] other_neigh_mask_fn(
      input logic [neigh_pkg::NumNeigh-1:0] neigh_mask,
      input logic [neigh_pkg::NumNeighWidth-1:0] neigh_id);
    begin
      for (int unsigned neigh_i = 0; neigh_i < neigh_pkg::NumOtherNeigh; neigh_i++) begin
        if (neigh_i >= neigh_id) begin
          other_neigh_mask_fn[neigh_i] = neigh_mask[neigh_i + 1];
        end else begin
          other_neigh_mask_fn[neigh_i] = neigh_mask[neigh_i];
        end
      end
    end
  endfunction

endmodule : neigh_coop_tload
