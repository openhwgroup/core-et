// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module neigh_coop_tload (
  // System signals
  input  logic                                                                      clock,
  input  logic                                                                      reset,
  input  logic [`NUM_NEIGH_R]                                                       neigh_id,
  // Cooperative requests from minions
  input  et_link_minion_miss_req_info_t [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0] min_coop_miss_req,
  input  logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                          min_coop_miss_req_valid,
  output logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                          min_coop_miss_req_ready,
  // Combined cooperative request to L2
  input  logic                                                                      coop_tload_disable_next,
  output et_link_neigh_req_info_t                                                   coop_tload_req,
  output logic                                                                      coop_tload_req_bid,
  input  logic                                                                      coop_tload_req_ready,
  // Cooperative TLoad slave ready output bus
  output coop_tload_slv_rdy_req_t                                                   coop_tload_slv_rdy_out_data,
  output logic                                                                      coop_tload_slv_rdy_out_valid,
  // Cooperative TLoad slave ready input buses
  input  coop_tload_slv_rdy_req_t [(`NUM_NEIGH-1)-1:0]                              coop_tload_slv_rdy_in_data,
  input  logic [(`NUM_NEIGH-1)-1:0]                                                 coop_tload_slv_rdy_in_valid,
  // Cooperative TLoad master done output bus
  output logic [`SHIRE_COOP_ID_RANGE]                                               coop_tload_mst_done_out_coop_id,
  output logic [(`NUM_NEIGH-1)-1:0]                                                 coop_tload_mst_done_out_valid,
  // Cooperative TLoad master done input buses
  input  logic [(`NUM_NEIGH-1)-1:0][`SHIRE_COOP_ID_RANGE]                           coop_tload_mst_done_in_coop_id,
  input  logic [(`NUM_NEIGH-1)-1:0]                                                 coop_tload_mst_done_in_valid,
  // Cooperative TLoad tag table CAM port
  input  logic                                                                      tag_table_cam_pop,
  input  logic [`SHIRE_COOP_LOAD_ID_RANGE]                                          tag_table_cam_tag,
  output coop_tload_tag_table_data_t                                                tag_table_cam_data,
  // Output debug signals going to Status Monitor
  output coop_tload_dbg_sm_t                                                        dbg_sm_signals

);

localparam TAG_TABLE_SLV_PORT = 0;
localparam TAG_TABLE_MST_PORT = 1;

// INTERNAL DECLARATIONS
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0][`SHIRE_COOP_ID_RANGE]      local_coop_id;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0][(`NUM_NEIGH-1)-1:0]        local_other_neigh_mask;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0][`MIN_PER_N-1:0]            local_coop_id_match;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0][`MIN_PER_N-1:0]            local_coop_id_match_tmp;
// Local copy of cooperative requests from minions
et_link_minion_miss_req_info_t [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]   local_ff_req;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                            local_ff_req_valid;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                            local_ff_available;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                            local_ff_available_next;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                            local_ff_in_accepted;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                            local_ff_out_accepted;
// Slave cooperative request per TLoad miss FF arbiter
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                            slv_per_miss_bid;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                            slv_per_miss_bid_next;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0][`SHIRE_COOP_MISS_ID_RANGE] slv_per_miss_miss_id;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0][`SHIRE_COOP_MISS_ID_RANGE] slv_per_miss_miss_id_next;
coop_tload_slv_req_t [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]             slv_per_miss_data_in;
coop_tload_slv_req_t [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]             slv_per_miss_data_in_next;
coop_tload_slv_req_t [`MIN_PER_N-1:0]                                        slv_per_miss_data_winner;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                            slv_per_miss_grant;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                            slv_per_miss_ready;
// Slave cooperative request per minion arbiter
logic [`MIN_PER_N-1:0]                                                       slv_per_min_bid;
logic                                                                        slv_per_min_stall;
coop_tload_slv_req_t [`MIN_PER_N-1:0]                                        slv_per_min_data_in;
logic [`MIN_PER_N-1:0]                                                       slv_per_min_grant;
coop_tload_slv_req_t                                                         slv_per_min_data_winner;
logic [`MIN_PER_N_R]                                                         slv_per_min_winner;
// Master cooperative request per TLoad miss FF arbiter
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                            mst_per_miss_bid;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                            mst_per_miss_bid_next;
et_link_minion_miss_req_info_t [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]   mst_per_miss_data_in;
et_link_minion_miss_req_info_t [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]   mst_per_miss_data_in_next;
et_link_minion_miss_req_info_t [`MIN_PER_N-1:0]                              mst_per_miss_data_winner;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                            mst_per_miss_grant;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                            mst_per_miss_ready;
// Master cooperative request per minion arbiter
logic [`MIN_PER_N-1:0]                                                       mst_per_min_bid;
logic                                                                        mst_per_min_stall;
et_link_minion_miss_req_info_t [`MIN_PER_N-1:0]                              mst_per_min_data_in;
logic [`MIN_PER_N-1:0]                                                       mst_per_min_grant;
et_link_minion_miss_req_info_t                                               mst_per_min_data_winner;
logic [`NUM_NEIGH-1:0]                                                       mst_per_min_winner_coop_neigh_mask;
logic [(`NUM_NEIGH-1)-1:0]                                                   mst_per_min_winner_other_neigh_mask;
logic [`SHIRE_COOP_ID_RANGE]                                                 mst_per_min_winner_coop_id;
logic [`SHIRE_COOP_SEQ_ID_RANGE]                                             mst_per_min_winner_coop_seq_id;
logic [`MIN_PER_N-1:0]                                                       mst_per_min_winner_coop_min_mask;
logic [`SHIRE_COOP_LOAD_ID_RANGE]                                            mst_per_min_winner_tag;
logic [`MIN_PER_N_R]                                                         mst_per_min_winner;
// Combined cooperative request to L2
logic                                                                        coop_tload_req_ready_int;
// rdy_avail_creds table
logic [`SHIRE_COOP_NUM_IDS-1:0][`SHIRE_COOP_RDY_CREDS_R]                     rdy_avail_creds;
logic [`SHIRE_COOP_NUM_IDS-1:0][`SHIRE_COOP_RDY_CREDS_R]                     rdy_avail_creds_next;
logic [`SHIRE_COOP_NUM_IDS-1:0]                                              rdy_avail_creds_up_inc;
logic [`SHIRE_COOP_NUM_IDS-1:0]                                              rdy_avail_creds_up_dec;
logic [`SHIRE_COOP_NUM_IDS-1:0]                                              rdy_avail_creds_up;
// rdy_avail_creds table increment
logic [(`NUM_NEIGH-1)-1:0][`SHIRE_COOP_ID_RANGE]                             rdy_avail_creds_inc_coop_id;
logic [(`NUM_NEIGH-1)-1:0]                                                   rdy_avail_creds_inc_valid;
// rdy_avail_creds table decrement
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0][`SHIRE_COOP_ID_RANGE]      rdy_avail_creds_dec_coop_id;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                            rdy_avail_creds_dec_valid;
// coop_tload_ext_rdy table
logic [`SHIRE_COOP_NUM_IDS-1:0][(`NUM_NEIGH-1)-1:0][`SHIRE_COOP_RDY_CREDS_R] coop_tload_ext_rdy;
logic [`SHIRE_COOP_NUM_IDS-1:0][(`NUM_NEIGH-1)-1:0][`SHIRE_COOP_RDY_CREDS_R] coop_tload_ext_rdy_next;
logic [`SHIRE_COOP_NUM_IDS-1:0][(`NUM_NEIGH-1)-1:0]                          coop_tload_ext_rdy_up_inc;
logic [`SHIRE_COOP_NUM_IDS-1:0][(`NUM_NEIGH-1)-1:0]                          coop_tload_ext_rdy_up_dec;
logic [`SHIRE_COOP_NUM_IDS-1:0][(`NUM_NEIGH-1)-1:0]                          coop_tload_ext_rdy_up;
// coop_tload_ext_rdy table increment
logic [(`NUM_NEIGH-1)-1:0][`SHIRE_COOP_ID_RANGE]                             coop_tload_ext_rdy_inc_coop_id;
logic [(`NUM_NEIGH-1)-1:0]                                                   coop_tload_ext_rdy_inc_valid;
// coop_tload_ext_rdy table decrement
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0][`SHIRE_COOP_ID_RANGE]      coop_tload_ext_rdy_dec_coop_id;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0][(`NUM_NEIGH-1)-1:0]        coop_tload_ext_rdy_dec_valid;
// coop_tload_slv_rdy output bus
coop_tload_slv_rdy_req_t                                                     coop_tload_slv_rdy_out_data_next;
logic                                                                        coop_tload_slv_rdy_out_valid_next;
// coop_tload_mst_done output bus
logic [`SHIRE_COOP_ID_RANGE]                                                 coop_tload_mst_done_out_coop_id_next;
logic [(`NUM_NEIGH-1)-1:0]                                                   coop_tload_mst_done_out_valid_next;
// Cooperative TLoad tag table
coop_tload_tag_table_req_t [1:0]                                             tag_table_wr_req;
logic [1:0]                                                                  tag_table_wr_valid;
logic [1:0]                                                                  tag_table_wr_ready;

////////////////////////////////////////////////////////////////////////////////
// Check cooperative requests for each minion
////////////////////////////////////////////////////////////////////////////////
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                     coop_tload_local_rdy; // Cooperative request is ready per Tload miss FF
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0][`NUM_NEIGH_R]       master_neigh;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0]                     slv_has_rdy_creds;
logic [`MIN_PER_N-1:0][`SHIRE_COOP_MISS_REQS-1:0][(`NUM_NEIGH-1)-1:0] mst_ext_is_rdy;

for (genvar min_i = 0; min_i < `MIN_PER_N; min_i++) begin : MINIONS
    // Replicate cooperative logic for every TLoad miss FF
  for (genvar miss_i = 0; miss_i < `SHIRE_COOP_MISS_REQS; miss_i++) begin : MISS_FF
    logic [`MIN_PER_N-1:0]           local_coop_min_mask;
    logic [`SHIRE_COOP_SEQ_ID_RANGE] local_coop_seq_id;
    logic [`NUM_NEIGH-1:0]           local_coop_neigh_mask;
    logic                            mst_all_ext_are_rdy;
    logic [`MIN_PER_N-1:0]           slv_per_miss_coop_min_mask;
    logic [`MIN_PER_N-1:0]           mst_per_miss_coop_min_mask;

    // Send ready signal to miss FF
    assign min_coop_miss_req_ready[min_i][miss_i] = local_ff_available[min_i][miss_i];

    ////////////////////////////////////////////////////////////////////////////////
    // Register cooperative requests from minions locally
    ////////////////////////////////////////////////////////////////////////////////

    //          CLK    RST    EN                                   DOUT                               DIN                                     DEF
    `RST_FF    (clock, reset,                                      local_ff_available[min_i][miss_i], local_ff_available_next[min_i][miss_i], 1'b1)
    `EN_FF     (clock,        local_ff_in_accepted[min_i][miss_i], local_ff_req[min_i][miss_i],       min_coop_miss_req[min_i][miss_i])

    assign local_ff_in_accepted[min_i][miss_i] = min_coop_miss_req_valid[min_i][miss_i] & local_ff_available[min_i][miss_i];

    always_comb begin
      // If FF still available
      if (local_ff_available[min_i][miss_i])
          // Not available anymore when input is valid
          local_ff_available_next[min_i][miss_i] = ~min_coop_miss_req_valid[min_i][miss_i];
      // Not available, wait for arbiter grant
      else
          // Grant received, FF available again
          local_ff_available_next[min_i][miss_i] = local_ff_out_accepted[min_i][miss_i];              
    end

    assign local_ff_req_valid[min_i][miss_i] = ~local_ff_available[min_i][miss_i];

    // Access is granted to local request if it is granted to either slave or master cooperative requests
    assign local_ff_out_accepted[min_i][miss_i] = (slv_per_miss_bid_next[min_i][miss_i] & slv_per_miss_ready[min_i][miss_i])
                                                    | (mst_per_miss_bid_next[min_i][miss_i] & mst_per_miss_ready[min_i][miss_i]);

    ////////////////////////////////////////////////////////////////////////////////
    // Cooperative logic
    ////////////////////////////////////////////////////////////////////////////////

    // Extract fields from data
    assign local_coop_min_mask                   = local_ff_req[min_i][miss_i].data[`SHIRE_COOP_MIN_MASK_START   +: `SHIRE_COOP_MIN_MASK_SIZE];
    assign local_coop_seq_id                     = local_ff_req[min_i][miss_i].data[`SHIRE_COOP_SEQ_ID_START     +: `SHIRE_COOP_SEQ_ID_SIZE];
    assign local_coop_id[min_i][miss_i]          = local_ff_req[min_i][miss_i].data[`SHIRE_COOP_ID_START         +: `SHIRE_COOP_ID_SIZE];
    assign local_coop_neigh_mask                 = local_ff_req[min_i][miss_i].data[`SHIRE_COOP_NEIGH_MASK_START +: `SHIRE_COOP_NEIGH_MASK_SIZE];
    assign local_other_neigh_mask[min_i][miss_i] = otherNeighMask(local_coop_neigh_mask,neigh_id);


     // Compare coop_id of this minion with those from other minions
     // lower triangle is computed in local_coop_id_match_tmp.  The
     // computation is propagated to local_coop_id_match. Lower
     // triangle is copied as it is and then propagated to the upper
     // triangle since is the same comparison. Diagonal is kept at 0
     // since we don't want to compare agains itself.
     //
     // This split into 2 signals/always_combs bypass verilator
     // limitation where reading back a modified variable is not possible.
     /* === Reference code === 
         always_comb begin
           local_coop_id_match[min_i][miss_i] = '0;
         
           for (integer min_j = 0; min_j < min_i; min_j++)
               local_coop_id_match[min_i][miss_i][min_j] = local_coop_id[min_i][miss_i] == local_coop_id[min_j][miss_i];
      
           // Save backwards comparison by taking the same result
           for (integer min_j = min_i+1; min_j < `MIN_PER_N; min_j++)
               local_coop_id_match[min_i][miss_i][min_j] = local_coop_id_match[min_j][miss_i][min_i];
         end
      */

     // First compute - lower trinagle
     always_comb begin
        for (integer min_j = 0; min_j < min_i; min_j++)
          local_coop_id_match_tmp[min_i][miss_i][min_j] = local_coop_id[min_i][miss_i] == local_coop_id[min_j][miss_i];
     end

     // Set diagonal an propagate lower an upper triangle
     always_comb begin
        local_coop_id_match[min_i][miss_i] = '0;
        
        for (integer min_j = 0; min_j < min_i; min_j++)
          local_coop_id_match[min_i][miss_i][min_j] = local_coop_id_match_tmp[min_i][miss_i][min_j];
       
        for (integer min_j = min_i+1; min_j < `MIN_PER_N; min_j++)
          local_coop_id_match[min_i][miss_i][min_j] = local_coop_id_match_tmp[min_j][miss_i][min_i];
     end
     
     // Compute if cooperative miss request is ready locally
    always_comb begin
      coop_tload_local_rdy[min_i][miss_i] = local_ff_req_valid[min_i][miss_i];
      // Check that the other minions specified in the mask are also ready
      for (integer min_j = 0; min_j < `MIN_PER_N; min_j++) begin
        if (min_i != min_j)
          coop_tload_local_rdy[min_i][miss_i] &= ~local_coop_min_mask[min_j] ||                                                    // Minion j does not cooperate with minion i
                                              (local_ff_req_valid[min_j][miss_i] && local_coop_id_match[min_i][miss_i][min_j]); // Or it is also ready for the same coop_id
      end
    end

    // Compute the master neighborhood
    assign master_neigh[min_i][miss_i] = masterNeigh(local_ff_req[min_i][miss_i].address,local_coop_neigh_mask);

    ////////////////////////////////////////////////////////////////////////////////
    // If this is a slave neighborhood, it must send a 'ready' notification
    // to the master neighborhood through the coop_tload_slv_rdy bus.
    // To send the 'ready' notification, the rdy_avail_creds table
    // must be checked first to know if there are credits to do it
    ////////////////////////////////////////////////////////////////////////////////
    // Check rdy_avail_creds table
    assign slv_has_rdy_creds[min_i][miss_i] = rdy_avail_creds[local_coop_id[min_i][miss_i]] != '0;

    //          CLK    RST    EN                                                                        DOUT                                 DIN                                   DEF
    `RST_EN_FF (clock, reset, slv_per_miss_ready[min_i][miss_i],                                        slv_per_miss_bid[min_i][miss_i],     slv_per_miss_bid_next[min_i][miss_i], 1'b0)
    `EN_FF     (clock,        slv_per_miss_ready[min_i][miss_i] & slv_per_miss_bid_next[min_i][miss_i], slv_per_miss_miss_id[min_i][miss_i], slv_per_miss_miss_id_next[min_i][miss_i])
    `EN_FF     (clock,        slv_per_miss_ready[min_i][miss_i] & slv_per_miss_bid_next[min_i][miss_i], slv_per_miss_data_in[min_i][miss_i], slv_per_miss_data_in_next[min_i][miss_i])

    // Check if a slave cooperative request is ready to send the 'ready' notification
    // In that case, bid for access to the coop_tload_slv_rdy bus
    assign slv_per_miss_bid_next[min_i][miss_i]     = coop_tload_local_rdy[min_i][miss_i] && (master_neigh[min_i][miss_i] != neigh_id) && slv_has_rdy_creds[min_i][miss_i];
    assign slv_per_miss_miss_id_next[min_i][miss_i] = local_ff_req[min_i][miss_i].id[`SHIRE_COOP_MISS_ID_RANGE];
    assign slv_per_miss_data_in_next[min_i][miss_i] = '{master_neigh:  master_neigh[min_i][miss_i],
                                                      coop_id:       local_coop_id[min_i][miss_i],
                                                      coop_seq_id:   local_coop_seq_id,
                                                      coop_min_mask: local_coop_min_mask};

    // Access is granted if it was granted to the same TLoad miss FF of any of the cooperating minions
    assign slv_per_miss_coop_min_mask        = slv_per_miss_data_in[min_i][miss_i].coop_min_mask;
    assign slv_per_miss_ready[min_i][miss_i] = (|(slv_per_min_grant & slv_per_miss_coop_min_mask) & slv_per_miss_grant[slv_per_min_winner][miss_i])
                                                 | ~slv_per_miss_bid[min_i][miss_i];

    ////////////////////////////////////////////////////////////////////////////////
    // If this is a master neighborhood, it must send the request out to the L2.
    // Before doing so the coop_tload_ext_rdy table must be checked to see if all
    // the cooperating neighborhoods have already sent the 'ready' notification
    ////////////////////////////////////////////////////////////////////////////////

    // Check that 'ready' notification counters from the other neighborhoods
    // for the specified coop_id are greater than 0
    always_comb begin
      for (integer neigh_i = 0; neigh_i < `NUM_NEIGH-1; neigh_i++)
        mst_ext_is_rdy[min_i][miss_i][neigh_i] = coop_tload_ext_rdy[local_coop_id[min_i][miss_i]][neigh_i] != '0;
    end
    // Check if counters are non-zero for all the neighborhoods set in the local_coop_neigh_mask
    assign mst_all_ext_are_rdy = &(mst_ext_is_rdy[min_i][miss_i] | ~local_other_neigh_mask[min_i][miss_i]);

    //         CLK    RST    EN                                                                        DOUT                                 DIN                                   DEF
    `RST_EN_FF(clock, reset, mst_per_miss_ready[min_i][miss_i],                                        mst_per_miss_bid[min_i][miss_i],     mst_per_miss_bid_next[min_i][miss_i], 1'b0)
    `EN_FF    (clock,        mst_per_miss_ready[min_i][miss_i] & mst_per_miss_bid_next[min_i][miss_i], mst_per_miss_data_in[min_i][miss_i], mst_per_miss_data_in_next[min_i][miss_i])

    // Check if a master cooperative request is ready to be sent out to the L2
    // In that case, bid for access to the ET Link request interface
    assign mst_per_miss_bid_next[min_i][miss_i]     = coop_tload_local_rdy[min_i][miss_i] && (master_neigh[min_i][miss_i] == neigh_id) && mst_all_ext_are_rdy;
    assign mst_per_miss_data_in_next[min_i][miss_i] = local_ff_req[min_i][miss_i];

    // Access is granted if it was granted to the same TLoad miss FF of any of the cooperating minions
    assign mst_per_miss_coop_min_mask        = mst_per_miss_data_in[min_i][miss_i].data[`SHIRE_COOP_MIN_MASK_START +: `SHIRE_COOP_MIN_MASK_SIZE];
    assign mst_per_miss_ready[min_i][miss_i] = (|(mst_per_min_grant & mst_per_miss_coop_min_mask) & mst_per_miss_grant[mst_per_min_winner][miss_i])
                                                  | ~mst_per_miss_bid[min_i][miss_i];
  end


  ////////////////////////////////////////////////////////////////////////////////
  // Arbitrate all the TLoad miss FFs of each minion
  // for slave requests to access the coop_tload_slv_rdy bus
  ////////////////////////////////////////////////////////////////////////////////
  logic slv_per_miss_arb_winner_unused_ok;
  arb_lru_data #(
      .WIDTH       ( $bits(coop_tload_slv_req_t)     ),
      .NUM_CLIENTS ( `SHIRE_COOP_MISS_REQS           )
    ) slv_per_miss_arb  (
      // System signals
      .clock       ( clock                           ),
      .reset       ( reset                           ),
      // Bidding
      .bid         ( slv_per_miss_bid[min_i]         ),
      .stall       ( ~slv_per_min_grant[min_i]       ),
      // Data inputs
      .data_in     ( slv_per_miss_data_in[min_i]     ),
      // Grant
      .grant       ( slv_per_miss_grant[min_i]       ),
      .data_winner ( slv_per_miss_data_winner[min_i] ),
      .winner      ( slv_per_miss_arb_winner_unused_ok )
    );

  // Winner bids for access to the coop_tload_slv_rdy bus among all minions
  assign slv_per_min_bid[min_i]     = |slv_per_miss_bid[min_i];
  assign slv_per_min_data_in[min_i] = slv_per_miss_data_winner[min_i];

  ////////////////////////////////////////////////////////////////////////////////
  // Arbitrate all the TLoad miss FFs of each minion
  // for master requests to access the ET Link request interface
  ////////////////////////////////////////////////////////////////////////////////
  logic mst_per_miss_arb_winner_unused_ok;
  arb_lru_data #(
   .WIDTH       ( $bits(et_link_minion_miss_req_info_t) ),
   .NUM_CLIENTS ( `SHIRE_COOP_MISS_REQS                 )
  ) mst_per_miss_arb (
    // System signals
    .clock       ( clock                             ),
    .reset       ( reset                             ),
    // Bidding
    .bid         ( mst_per_miss_bid[min_i]           ),
    .stall       ( ~mst_per_min_grant[min_i]         ),
    // Data inputs
    .data_in     ( mst_per_miss_data_in[min_i]       ),
    // Grant
    .grant       ( mst_per_miss_grant[min_i]         ),
    .data_winner ( mst_per_miss_data_winner[min_i]   ),
    .winner      ( mst_per_miss_arb_winner_unused_ok )
    );

    // Winner bids for access to the ET Link request interface among all minions
  assign mst_per_min_bid[min_i]     = |mst_per_miss_bid[min_i];
  assign mst_per_min_data_in[min_i] = mst_per_miss_data_winner[min_i];
end


////////////////////////////////////////////////////////////////////////////////
// Arbitrate all the slave cooperative miss requests that are ready
// to send the 'ready' notification to the master neighborhood
////////////////////////////////////////////////////////////////////////////////
logic slv_per_min_valid;

arb_lru_data #(
  .WIDTH       ( $bits(coop_tload_slv_req_t) ),
  .NUM_CLIENTS ( `MIN_PER_N                  )
) slv_per_min_arb (
  // System signals
  .clock       ( clock                       ),
  .reset       ( reset                       ),
  // Bidding
  .bid         ( slv_per_min_bid             ),
  .stall       ( slv_per_min_stall           ),
  // Data inputs
  .data_in     ( slv_per_min_data_in         ),
  // Grant
  .grant       ( slv_per_min_grant           ),
  .data_winner ( slv_per_min_data_winner     ),
  .winner      ( slv_per_min_winner          )
);

// Stall arbiter if tag table is not ready
assign slv_per_min_stall = ~tag_table_wr_ready[TAG_TABLE_SLV_PORT];
assign slv_per_min_valid = |slv_per_min_bid;

////////////////////////////////////////////////////////////////////////////////
// Send 'ready' notification through coop_tload_slv_rdy output bus
////////////////////////////////////////////////////////////////////////////////

// Send only when slave request is accepted by the tag table
assign coop_tload_slv_rdy_out_valid_next = slv_per_min_valid & tag_table_wr_ready[TAG_TABLE_SLV_PORT];
assign coop_tload_slv_rdy_out_data_next  = '{dest_neigh: slv_per_min_data_winner.master_neigh,
                                             coop_id:    slv_per_min_data_winner.coop_id};

//      CLK    RST    EN                                 DOUT                          DIN                                DEF
`RST_FF(clock, reset,                                    coop_tload_slv_rdy_out_valid, coop_tload_slv_rdy_out_valid_next, 1'b0)
`EN_FF (clock,        coop_tload_slv_rdy_out_valid_next, coop_tload_slv_rdy_out_data,  coop_tload_slv_rdy_out_data_next)

////////////////////////////////////////////////////////////////////////////////
// Arbitrate all the master cooperative miss requests that are ready
// to send the request through the ET Link interface
////////////////////////////////////////////////////////////////////////////////
logic mst_per_min_valid;
logic mst_per_min_accepted;

arb_lru_data #(
  .WIDTH       ( $bits(et_link_minion_miss_req_info_t) ),
  .NUM_CLIENTS ( `MIN_PER_N                            )
) mst_per_min_arb (
  // System signals
  .clock       ( clock                                 ),
  .reset       ( reset                                 ),
  // Bidding
  .bid         ( mst_per_min_bid                       ),
  .stall       ( mst_per_min_stall                     ),
  // Data inputs
  .data_in     ( mst_per_min_data_in                   ),
  // Grant
  .grant       ( mst_per_min_grant                     ),
  .data_winner ( mst_per_min_data_winner               ),
  .winner      ( mst_per_min_winner                    )
);

// Stall arbiter if either ET-Link request interface or tag table are not ready
assign mst_per_min_stall    = ~coop_tload_req_ready_int | ~tag_table_wr_ready[TAG_TABLE_MST_PORT];
assign mst_per_min_valid    = |mst_per_min_bid;
assign mst_per_min_accepted = mst_per_min_valid & ~mst_per_min_stall;

// Extract fields from data
assign mst_per_min_winner_coop_min_mask    = mst_per_min_data_winner.data[`SHIRE_COOP_MIN_MASK_START   +: `SHIRE_COOP_MIN_MASK_SIZE];
assign mst_per_min_winner_coop_seq_id      = mst_per_min_data_winner.data[`SHIRE_COOP_SEQ_ID_START     +: `SHIRE_COOP_SEQ_ID_SIZE];
assign mst_per_min_winner_coop_id          = mst_per_min_data_winner.data[`SHIRE_COOP_ID_START         +: `SHIRE_COOP_ID_SIZE];
assign mst_per_min_winner_coop_neigh_mask  = mst_per_min_data_winner.data[`SHIRE_COOP_NEIGH_MASK_START +: `SHIRE_COOP_NEIGH_MASK_SIZE];
assign mst_per_min_winner_other_neigh_mask = otherNeighMask(mst_per_min_winner_coop_neigh_mask,neigh_id);
assign mst_per_min_winner_tag              = {mst_per_min_winner_coop_id,mst_per_min_winner_coop_seq_id};

////////////////////////////////////////////////////////////////////////////////
// Flop combined cooperative request and send it through the ET-Link interface
////////////////////////////////////////////////////////////////////////////////
et_link_neigh_req_info_t coop_tload_req_next;
logic                    coop_tload_req_bid_next;
logic                    coop_tload_req_valid;      // Internal valid signal (we may have a valid request but output bid is disabled through coop_tload_disable_next)
logic                    coop_tload_req_valid_next;

// Send only when request is accepted by the tag table
assign coop_tload_req_valid_next = mst_per_min_valid & tag_table_wr_ready[TAG_TABLE_MST_PORT];
// Send bid to output only if request is not disabled
assign coop_tload_req_bid_next = (coop_tload_req_ready_int ? coop_tload_req_valid_next : coop_tload_req_valid) & ~coop_tload_disable_next;
// Map request to output format
always_comb begin
  coop_tload_req_next.id     = mst_per_min_winner_tag;
  coop_tload_req_next.source = '0; // Unused (assigned at neighborhood channel)
  // Convert cooperative into regular reads if they do not cooperate inter-neighborhood
  if (mst_per_min_winner_other_neigh_mask == '0) begin
    coop_tload_req_next.wdata  = 1'b0;
    coop_tload_req_next.opcode = ET_LINK_REQ_Read;
    coop_tload_req_next.qwen   = 4'b0000;
  end else begin
    coop_tload_req_next.wdata  = 1'b1;
    coop_tload_req_next.opcode = ET_LINK_REQ_ReadCoop;
    coop_tload_req_next.qwen   = 4'b0001;
  end
    coop_tload_req_next.subopcode = mst_per_min_data_winner.subopcode;
    coop_tload_req_next.address   = mst_per_min_data_winner.address;
    coop_tload_req_next.data      = `NEIGH_DATA_SIZE'(mst_per_min_data_winner.data);
    coop_tload_req_next.size      = mst_per_min_data_winner.size;
end

// Internally, request pipeline is ready if either output is ready
// or if the output FF does not contain a valid request
assign coop_tload_req_ready_int = coop_tload_req_ready | ~coop_tload_req_valid;

//         CLK    RST    EN                                                    DOUT                  DIN                        DEF
`EN_FF    (clock,        coop_tload_req_ready_int & coop_tload_req_valid_next, coop_tload_req,       coop_tload_req_next)
`RST_EN_FF(clock, reset, coop_tload_req_ready_int,                             coop_tload_req_valid, coop_tload_req_valid_next, 1'b0)
`RST_FF   (clock, reset,                                                       coop_tload_req_bid,   coop_tload_req_bid_next,   1'b0)

////////////////////////////////////////////////////////////////////////////////
// Send 'done' notification through coop_tload_mst_done output bus
////////////////////////////////////////////////////////////////////////////////

// Send only when master request is accepted
assign coop_tload_mst_done_out_valid_next   = {(`NUM_NEIGH-1){mst_per_min_accepted}} & mst_per_min_winner_other_neigh_mask;
assign coop_tload_mst_done_out_coop_id_next = mst_per_min_winner_coop_id;

//      CLK    RST     EN                                  DOUT                             DIN                                 DEF
`RST_FF(clock, reset,                                      coop_tload_mst_done_out_valid,   coop_tload_mst_done_out_valid_next, 1'b0)
`EN_FF (clock,        |coop_tload_mst_done_out_valid_next, coop_tload_mst_done_out_coop_id, coop_tload_mst_done_out_coop_id_next)

////////////////////////////////////////////////////////////////////////////////
// coop_tload_mst_done input buses
////////////////////////////////////////////////////////////////////////////////

for (genvar neigh_i = 0; neigh_i < `NUM_NEIGH-1; neigh_i++) begin : MST_DONE_IN
  logic [`SHIRE_COOP_ID_RANGE] rdy_avail_creds_inc_coop_id_next;
  logic                        rdy_avail_creds_inc_valid_next;

  assign rdy_avail_creds_inc_coop_id_next = coop_tload_mst_done_in_coop_id[neigh_i];
  assign rdy_avail_creds_inc_valid_next   = coop_tload_mst_done_in_valid[neigh_i];

   //      CLK    RST    EN                              DOUT                                  DIN                             DEF
   `RST_FF(clock, reset,                                 rdy_avail_creds_inc_valid[neigh_i],   rdy_avail_creds_inc_valid_next, 1'b0)
   `EN_FF (clock,        rdy_avail_creds_inc_valid_next, rdy_avail_creds_inc_coop_id[neigh_i], rdy_avail_creds_inc_coop_id_next)
end


////////////////////////////////////////////////////////////////////////////////
// rdy_avail_creds table
////////////////////////////////////////////////////////////////////////////////

for (genvar i = 0; i < `SHIRE_COOP_NUM_IDS; i++) begin : RDY_AVAIL_CREDS
    //         CLK    RST    EN                     DOUT                DIN                      DEF
    `RST_EN_FF(clock, reset, rdy_avail_creds_up[i], rdy_avail_creds[i], rdy_avail_creds_next[i], `SHIRE_COOP_RDY_CREDS)
end


// Flop signals controlling decrement of rdy_avail_creds table to improve timing
// This will add a one-cycle delay to the update of the table with respect to when the credit is consumed
// That will work as long as requests with the same coop_id never arrive in consecutive clock cycles
// (which, in principle, is ensured by the local FF having a throughput of 1 request every other cycle and
// all the requests with the same coop_id having the same minion mask)
for (genvar min_i = 0; min_i < `MIN_PER_N; min_i++) begin : RDY_AVAIL_CREDS_DEC_PER_MIN
  for (genvar miss_i = 0; miss_i < `SHIRE_COOP_MISS_REQS; miss_i++) begin : RDY_AVAIL_CREDS_DEC_PER_MISS
    logic [`SHIRE_COOP_ID_RANGE] rdy_avail_creds_dec_coop_id_next;
    logic                        rdy_avail_creds_dec_valid_next;

    assign rdy_avail_creds_dec_coop_id_next = local_coop_id[min_i][miss_i];
    assign rdy_avail_creds_dec_valid_next   = slv_per_miss_bid_next[min_i][miss_i] & slv_per_miss_ready[min_i][miss_i];

    //       CLK    RST    EN                              DOUT                                        DIN                             DEF
    `RST_FF (clock, reset,                                 rdy_avail_creds_dec_valid[min_i][miss_i],   rdy_avail_creds_dec_valid_next, '0)
    `EN_FF  (clock,        rdy_avail_creds_dec_valid_next, rdy_avail_creds_dec_coop_id[min_i][miss_i], rdy_avail_creds_dec_coop_id_next)
    end
end


// Compute update of table
always_comb begin
  rdy_avail_creds_up_dec = '0;
  rdy_avail_creds_up_inc = '0;
  // Default next value is rdy_avail_creds-1
  for (integer i = 0; i < `SHIRE_COOP_NUM_IDS; i++)
    rdy_avail_creds_next[i] = rdy_avail_creds[i] - 1'b1;

// Decrement counter for coop_id of slave cooperative requests that will
// bid for access to the coop_tload_slv_rdy bus in the next cycle
// (credits must be consumed in the local FF stage, where they are checked).
// This will only work if all the minions sharing the same coop_id belong
// to the same cooperative request
  for (integer min_i = 0; min_i < `MIN_PER_N; min_i++) begin
    for (integer miss_i = 0; miss_i < `SHIRE_COOP_MISS_REQS; miss_i++) begin
      if (rdy_avail_creds_dec_valid[min_i][miss_i]) begin
          rdy_avail_creds_up_dec[rdy_avail_creds_dec_coop_id[min_i][miss_i]] = 1'b1;
          rdy_avail_creds_next[rdy_avail_creds_dec_coop_id[min_i][miss_i]]   = rdy_avail_creds[rdy_avail_creds_dec_coop_id[min_i][miss_i]] - 1'b1;
      end
     end
  end

    // Increment counter for coop_id received through the coop_tload_mst_done bus
    // from each neighborhood
  for (integer neigh_i = 0; neigh_i < `NUM_NEIGH-1; neigh_i++) begin
    if (rdy_avail_creds_inc_valid[neigh_i]) begin
      rdy_avail_creds_up_inc[rdy_avail_creds_inc_coop_id[neigh_i]] = 1'b1;
      rdy_avail_creds_next[rdy_avail_creds_inc_coop_id[neigh_i]]   = rdy_avail_creds[rdy_avail_creds_inc_coop_id[neigh_i]] + 1'b1;
    end
  end
end

// Only update if either increment or decrement happens,
// but not both (in which case, counter is not modified)
assign rdy_avail_creds_up = rdy_avail_creds_up_dec ^ rdy_avail_creds_up_inc;

////////////////////////////////////////////////////////////////////////////////
// coop_tload_slv_rdy input buses
////////////////////////////////////////////////////////////////////////////////

for (genvar neigh_i = 0; neigh_i < `NUM_NEIGH-1; neigh_i++) begin : SLV_RDY_IN
  logic [`SHIRE_COOP_ID_RANGE] coop_tload_ext_rdy_inc_coop_id_next;
  logic                        coop_tload_ext_rdy_inc_valid_next;

  // Only accept request if it is addressed to this neighborhood
  assign coop_tload_ext_rdy_inc_coop_id_next = coop_tload_slv_rdy_in_data[neigh_i].coop_id;
  assign coop_tload_ext_rdy_inc_valid_next   = coop_tload_slv_rdy_in_valid[neigh_i] && (coop_tload_slv_rdy_in_data[neigh_i].dest_neigh == neigh_id);

  //      CLK    RST    EN                                 DOUT                                     DIN                                DEF
  `RST_FF(clock, reset,                                    coop_tload_ext_rdy_inc_valid[neigh_i],   coop_tload_ext_rdy_inc_valid_next, 1'b0)
  `EN_FF (clock,        coop_tload_ext_rdy_inc_valid_next, coop_tload_ext_rdy_inc_coop_id[neigh_i], coop_tload_ext_rdy_inc_coop_id_next)
end

////////////////////////////////////////////////////////////////////////////////
// coop_tload_ext_rdy table
////////////////////////////////////////////////////////////////////////////////
for (genvar i = 0; i < `SHIRE_COOP_NUM_IDS; i++) begin : EXT_RDY_COOP_ID
  for (genvar neigh_i = 0; neigh_i < `NUM_NEIGH-1; neigh_i++)
  begin : EXT_RDY_NEIGH
      //         CLK    RST    EN                                 DOUT                            DIN                                  DEF
      `RST_EN_FF(clock, reset, coop_tload_ext_rdy_up[i][neigh_i], coop_tload_ext_rdy[i][neigh_i], coop_tload_ext_rdy_next[i][neigh_i], '0)
  end
end


// Flop signals controlling decrement of coop_tload_ext_rdy table to improve timing
// This will add a one-cycle delay to the update of the table with respect to when the credit is consumed
// That will work as long as requests with the same coop_id never arrive in consecutive clock cycles
// (which, in principle, is ensured by the local FF having a throughput of 1 request every other cycle and
// all the requests with the same coop_id having the same minion mask)
for (genvar min_i = 0; min_i < `MIN_PER_N; min_i++) begin : EXT_RDY_DEC_PER_MIN
  for (genvar miss_i = 0; miss_i < `SHIRE_COOP_MISS_REQS; miss_i++) begin : EXT_RDY_DEC_PER_MISS
    logic [`SHIRE_COOP_ID_RANGE] coop_tload_ext_rdy_dec_coop_id_next;
    logic [(`NUM_NEIGH-1)-1:0]   coop_tload_ext_rdy_dec_valid_next;

    assign coop_tload_ext_rdy_dec_coop_id_next = local_coop_id[min_i][miss_i];
    assign coop_tload_ext_rdy_dec_valid_next   = {(`NUM_NEIGH-1){mst_per_miss_bid_next[min_i][miss_i] & mst_per_miss_ready[min_i][miss_i]}} & local_other_neigh_mask[min_i][miss_i];

    //       CLK    RST    EN                                  DOUT                                           DIN                                DEF
    `RST_FF (clock, reset,                                     coop_tload_ext_rdy_dec_valid[min_i][miss_i],   coop_tload_ext_rdy_dec_valid_next, '0)
    `EN_FF  (clock,        |coop_tload_ext_rdy_dec_valid_next, coop_tload_ext_rdy_dec_coop_id[min_i][miss_i], coop_tload_ext_rdy_dec_coop_id_next)
  end
end


// Compute update of table
always_comb begin
  coop_tload_ext_rdy_up_inc = '0;
  coop_tload_ext_rdy_up_dec = '0;

  // Default next value is coop_tload_ext_rdy+1
  for (integer neigh_i = 0; neigh_i < `NUM_NEIGH-1; neigh_i++)
    for (integer i = 0; i < `SHIRE_COOP_NUM_IDS; i++)
      coop_tload_ext_rdy_next[i][neigh_i] = coop_tload_ext_rdy[i][neigh_i] + 1'b1;

  for (integer neigh_i = 0; neigh_i < `NUM_NEIGH-1; neigh_i++) begin
    // Increment counter for coop_id received through the coop_tload_slv_rdy bus
    // from each neighborhood
    if (coop_tload_ext_rdy_inc_valid[neigh_i]) begin
            coop_tload_ext_rdy_up_inc[coop_tload_ext_rdy_inc_coop_id[neigh_i]][neigh_i] = 1'b1;
            coop_tload_ext_rdy_next[coop_tload_ext_rdy_inc_coop_id[neigh_i]][neigh_i]   = coop_tload_ext_rdy[coop_tload_ext_rdy_inc_coop_id[neigh_i]][neigh_i] + 1'b1;
    end

    // Decrement counter of cooperating neighborhoods for coop_id
    // of master cooperative miss requests that will bid for
    // access to the ET Link request interface in the next cycle
    // (credits must be consumed in the local FF stage, where they are checked).
    // This will only work if all the minions sharing the same coop_id belong
    // to the same cooperative request
    for (integer min_i = 0; min_i < `MIN_PER_N; min_i++) begin
      for (integer miss_i = 0; miss_i < `SHIRE_COOP_MISS_REQS; miss_i++) begin
        if (coop_tload_ext_rdy_dec_valid[min_i][miss_i][neigh_i]) begin
          coop_tload_ext_rdy_up_dec[coop_tload_ext_rdy_dec_coop_id[min_i][miss_i]][neigh_i] = 1'b1;
          coop_tload_ext_rdy_next[coop_tload_ext_rdy_dec_coop_id[min_i][miss_i]][neigh_i]   = coop_tload_ext_rdy[coop_tload_ext_rdy_dec_coop_id[min_i][miss_i]][neigh_i] - 1'b1;
        end
      end
    end
  end
end

// Only update if either increment or decrement happens,
// but not both (in which case, counter is not modified)
assign coop_tload_ext_rdy_up = coop_tload_ext_rdy_up_inc ^ coop_tload_ext_rdy_up_dec;

////////////////////////////////////////////////////////////////////////////////
// Cooperative TLoad tag table
// For each inflight cooperative TLoad request, it stores:
// - tag (8 bits): id of ET-Link request sent to L2. 
//   It is composed of coop_id + coop_seq_id of the request
// - coop_min_mask (8 bits): mask of minions cooperating in the request
// - min_miss_id (24 bits): 3 bits of miss_id for each of the
//   up to 8 cooperating minions
////////////////////////////////////////////////////////////////////////////////

// Generate slave request to write cooperative TLoad tag table
assign tag_table_wr_req[TAG_TABLE_SLV_PORT].tag                = {slv_per_min_data_winner.coop_id, slv_per_min_data_winner.coop_seq_id};
assign tag_table_wr_req[TAG_TABLE_SLV_PORT].data.coop_min_mask = slv_per_min_data_winner.coop_min_mask;
always_comb begin
    // Save `SHIRE_COOP_MISS_ID_SIZE LSBs of the id of each minion
    for (integer min_i = 0; min_i < `MIN_PER_N; min_i++)
    begin
        tag_table_wr_req[TAG_TABLE_SLV_PORT].data.min_miss_id[min_i] = '0;

        // Multiplex all the TLoad miss FFs of each minion in an and-or tree style
        // and take the one corresponding to the granted TLoad miss FF of the winner minion
        for (integer miss_i = 0; miss_i < `SHIRE_COOP_MISS_REQS; miss_i++)
            tag_table_wr_req[TAG_TABLE_SLV_PORT].data.min_miss_id[min_i] |= slv_per_miss_miss_id[min_i][miss_i]
                                                                          & {`SHIRE_COOP_MISS_ID_SIZE{slv_per_miss_grant[slv_per_min_winner][miss_i]}};
    end
end

assign tag_table_wr_valid[TAG_TABLE_SLV_PORT] = slv_per_min_valid;

// Generate master request to write cooperative TLoad tag table
assign tag_table_wr_req[TAG_TABLE_MST_PORT].tag                = mst_per_min_winner_tag;
assign tag_table_wr_req[TAG_TABLE_MST_PORT].data.coop_min_mask = mst_per_min_winner_coop_min_mask;
always_comb begin
  // Save `SHIRE_COOP_MISS_ID_SIZE LSBs of the id of each minion
  for (integer min_i = 0; min_i < `MIN_PER_N; min_i++) begin
    tag_table_wr_req[TAG_TABLE_MST_PORT].data.min_miss_id[min_i] = '0;

    // Multiplex all the TLoad miss FFs of each minion in an and-or tree style
    // and take the one corresponding to the granted TLoad miss FF of the winner minion
    for (integer miss_i = 0; miss_i < `SHIRE_COOP_MISS_REQS; miss_i++)
      tag_table_wr_req[TAG_TABLE_MST_PORT].data.min_miss_id[min_i] |= mst_per_miss_data_in[min_i][miss_i].id[`SHIRE_COOP_MISS_ID_RANGE]
                                                                          & {`SHIRE_COOP_MISS_ID_SIZE{mst_per_miss_grant[mst_per_min_winner][miss_i]}};
  end
end

// Write only when request is accepted by ET-Link request interface
assign tag_table_wr_valid[TAG_TABLE_MST_PORT] = mst_per_min_valid & coop_tload_req_ready_int;

// Cooperative TLoad tag table
neigh_coop_tload_tag_table #(
  .DEPTH       ( `SHIRE_COOP_TAG_TABLE_DEPTH )
)
coop_tload_tag_table (
  // System signals
  .clock       ( clock                       ),
  .reset       ( reset                       ),
  // Write port                       
  .wr_req      ( tag_table_wr_req            ),
  .wr_valid    ( tag_table_wr_valid          ),
  .wr_ready    ( tag_table_wr_ready          ),
  // CAM port
  .cam_pop     ( tag_table_cam_pop           ),
  .cam_tag     ( tag_table_cam_tag           ),
  .cam_data    ( tag_table_cam_data          )
);

////////////////////////////////////////////////////////////////////////////////
// Connect output debug signals
////////////////////////////////////////////////////////////////////////////////
always_comb begin
  // Connect cooperative TLoad signals
  dbg_sm_signals.mst_ext_is_rdy       = mst_ext_is_rdy;
  dbg_sm_signals.slv_has_rdy_creds    = slv_has_rdy_creds;
  dbg_sm_signals.master_neigh         = master_neigh;
  dbg_sm_signals.coop_tload_local_rdy = coop_tload_local_rdy;
end

////////////////////////////////////////////////////////////////////////////
// Helper functions
////////////////////////////////////////////////////////////////////////////
function automatic logic [`NUM_NEIGH_R] masterNeigh;
  input logic [`PA_SIZE-1:0]   pa;       // Physical address
  input logic [`NUM_NEIGH-1:0] cnm;      // Cooperative neighborhood mask
  begin
  case (pa[`SHIRE_COOP_MST_NEIGH_ADDR_RANGE])
      `SHIRE_COOP_MST_NEIGH_ADDR_SIZE'(unsigned'(0)): begin
          masterNeigh = cnm[0] ? `NUM_NEIGH_L'(unsigned'(0))
                      : cnm[3] ? `NUM_NEIGH_L'(unsigned'(3))
                      : cnm[1] ? `NUM_NEIGH_L'(unsigned'(1))
                      :          `NUM_NEIGH_L'(unsigned'(2));
        end

        `SHIRE_COOP_MST_NEIGH_ADDR_SIZE'(unsigned'(1)): begin
           masterNeigh = cnm[1] ? `NUM_NEIGH_L'(unsigned'(1))
                       : cnm[0] ? `NUM_NEIGH_L'(unsigned'(0))
                       : cnm[2] ? `NUM_NEIGH_L'(unsigned'(2))
                       :          `NUM_NEIGH_L'(unsigned'(3));
        end

        `SHIRE_COOP_MST_NEIGH_ADDR_SIZE'(unsigned'(2)): begin
          masterNeigh = cnm[2] ? `NUM_NEIGH_L'(unsigned'(2))
                      : cnm[1] ? `NUM_NEIGH_L'(unsigned'(1))
                      : cnm[3] ? `NUM_NEIGH_L'(unsigned'(3))
                      :          `NUM_NEIGH_L'(unsigned'(0));
        end

        `SHIRE_COOP_MST_NEIGH_ADDR_SIZE'(unsigned'(3)): begin
          masterNeigh = cnm[3] ? `NUM_NEIGH_L'(unsigned'(3))
                      : cnm[2] ? `NUM_NEIGH_L'(unsigned'(2))
                      : cnm[0] ? `NUM_NEIGH_L'(unsigned'(0))
                      :          `NUM_NEIGH_L'(unsigned'(1));
        end
    endcase
  end
endfunction

function automatic logic [(`NUM_NEIGH-1)-1:0] otherNeighMask;
    input logic [`NUM_NEIGH-1:0] neighMask;
    input logic [`NUM_NEIGH_R]   neighId;
    begin
        // Select mask for the other neighborhoods
        for (integer neigh_i = 0; neigh_i < `NUM_NEIGH-1; neigh_i++)
            otherNeighMask[neigh_i] = neigh_i >= neighId ? neighMask[neigh_i+1]
                                                         : neighMask[neigh_i];
    end
endfunction

endmodule
