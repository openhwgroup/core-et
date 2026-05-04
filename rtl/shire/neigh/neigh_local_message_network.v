// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module neigh_local_message_network
  (
  // System signals
  input  logic                                                              clock,
  input  logic                                                              reset,
  // Configuration signals
  output logic [`MIN_PER_N-1:0][`NEIGH_MAX_LOCAL_CONNECT-1:0]               local_channel_mask,
  output logic [`MIN_PER_N-1:0][`NEIGH_MAX_LOCAL_CONNECT-1:0][`MIN_PER_N_R] local_dest_id,
  // Local message outputs from evict FFs
  output logic [`MIN_PER_N-1:0]                                             local_message_ready_out,
  input  logic [`MIN_PER_N-1:0][`NEIGH_MAX_LOCAL_CONNECT-1:0]               local_message_valid_out,
  input  et_link_neigh_rsp_info_t [`MIN_PER_N-1:0]                          local_message_info_out,
  // Local message inputs to fill FFs
  input  logic [`MIN_PER_N-1:0]                                             local_message_ready_in,
  output logic [`MIN_PER_N-1:0]                                             local_message_valid_in,
  output et_link_neigh_rsp_info_t [`MIN_PER_N-1:0]                          local_message_info_in,
  // Output debug signals going to Status Monitor
  output fln_dbg_sm_t                                                       dbg_sm_signals
  );

  // Configure output channels to connect minions
  // --------------------------------------------
  // Channels used to senda data from minion 7
  localparam LOCAL_7_6_CH = 0;
  // Channels used to senda data from minion 6
  localparam LOCAL_6_4_CH = 0;
  localparam LOCAL_6_7_CH = 1;
  // Channels used to senda data from minion 5
  localparam LOCAL_5_4_CH = 0;
  // Channels used to senda data from minion 4
  localparam LOCAL_4_0_CH = 0;
  localparam LOCAL_4_5_CH = 1;
  localparam LOCAL_4_6_CH = 2;
  // Channels used to senda data from minion 3
  localparam LOCAL_3_2_CH = 0;
  // Channels used to senda data from minion 2
  localparam LOCAL_2_0_CH = 0;
  localparam LOCAL_2_3_CH = 1;
  // Channels used to senda data from minion 1
  localparam LOCAL_1_0_CH = 0;
  // Channels used to senda data from minion 0
  localparam LOCAL_0_1_CH = 0;
  localparam LOCAL_0_2_CH = 1;
  localparam LOCAL_0_4_CH = 2;

  // INTERNAL DECLARATIONS
  logic [`MIN_PER_N-1:0][`NEIGH_MAX_LOCAL_CONNECT-1:0] local_message_ready_out_reg;
  logic [`MIN_PER_N-1:0][`NEIGH_MAX_LOCAL_CONNECT-1:0] local_message_valid_out_reg;
  et_link_neigh_rsp_info_t [`MIN_PER_N-1:0]            local_message_info_out_reg;
  logic                                                local_message_ready_in6_from4;
  logic                                                local_message_ready_in6_from7;
  logic                                                local_message_ready_in4_from0;
  logic                                                local_message_ready_in4_from5;
  logic                                                local_message_ready_in4_from6;
  logic                                                local_message_ready_in2_from0;
  logic                                                local_message_ready_in2_from3;
  logic                                                local_message_ready_in0_from1;
  logic                                                local_message_ready_in0_from2;
  logic                                                local_message_ready_in0_from4;


  for (genvar i = 0; i < `MIN_PER_N; i++) begin : LOCAL_MESSAGE_OUT_FF
    // CLK    RST    EN                          DOUT                            DIN                         DEF
    `RST_EN_FF(clock, reset, local_message_ready_out[i], local_message_valid_out_reg[i], local_message_valid_out[i], '0)
    `EN_FF    (clock,        local_message_ready_out[i], local_message_info_out_reg[i],  local_message_info_out[i])

    assign local_message_ready_out[i] = |(local_message_valid_out_reg[i] & local_message_ready_out_reg[i]) | ~(|local_message_valid_out_reg[i]);
  end


  // Minion 7 is connected to minion 6
  assign local_channel_mask[7] = `NEIGH_MAX_LOCAL_CONNECT'b001;

  always_comb begin
    local_dest_id[7] = '0;
    local_message_ready_out_reg[7] = '0;

    local_dest_id[7][LOCAL_7_6_CH] = (`MIN_PER_N_L)'(unsigned'(6));
    local_message_ready_out_reg[7][LOCAL_7_6_CH] = local_message_ready_in6_from7;
  end

  assign local_message_info_in[7]  = local_message_info_out_reg[6];
  assign local_message_valid_in[7] = local_message_valid_out_reg[6][LOCAL_6_7_CH];

  // Minion 6 is connected to minions 4 and 7
  assign local_channel_mask[6] = `NEIGH_MAX_LOCAL_CONNECT'b011;

  always_comb begin
    local_dest_id[6] = '0;
    local_message_ready_out_reg[6] = '0;

    local_dest_id[6][LOCAL_6_4_CH] = (`MIN_PER_N_L)'(unsigned'(4));
    local_dest_id[6][LOCAL_6_7_CH] = (`MIN_PER_N_L)'(unsigned'(7));
    local_message_ready_out_reg[6][LOCAL_6_4_CH] = local_message_ready_in4_from6;
    local_message_ready_out_reg[6][LOCAL_6_7_CH] = local_message_ready_in[7];
  end

  arb_prio_data #(
    .WIDTH       ( $bits(et_link_neigh_rsp_info_t)                 ),
    .NUM_CLIENTS ( 2                                               )
  ) local_message_min6_arb (
    // Bidding
    .bid             ( {local_message_valid_out_reg[4][LOCAL_4_6_CH],
                       local_message_valid_out_reg[7][LOCAL_7_6_CH]} ),
    .stall           ( !local_message_ready_in[6]                    ),
    // Data inputs
    .data_in         ( {local_message_info_out_reg[4], 
                      local_message_info_out_reg[7]}                 ),
    // Result 
    .grant           ( {local_message_ready_in6_from4, 
                      local_message_ready_in6_from7}                 ),
    .data_winner     ( local_message_info_in[6]                      )
  );

  assign local_message_valid_in[6] = local_message_valid_out_reg[4][LOCAL_4_6_CH] |
  local_message_valid_out_reg[7][LOCAL_7_6_CH];

  // Minion 5 is connected to minion 4
  assign local_channel_mask[5] = `NEIGH_MAX_LOCAL_CONNECT'b001;

  always_comb begin
    local_dest_id[5] = '0;
    local_message_ready_out_reg[5] = '0;

    local_dest_id[5][LOCAL_5_4_CH] = (`MIN_PER_N_L)'(unsigned'(4));
    local_message_ready_out_reg[5][LOCAL_5_4_CH] = local_message_ready_in4_from5;
  end

  assign local_message_info_in[5]  = local_message_info_out_reg[4];
  assign local_message_valid_in[5] = local_message_valid_out_reg[4][LOCAL_4_5_CH];

  // Minion 4 is connected to minions 0, 5 and 6
  assign local_channel_mask[4] = `NEIGH_MAX_LOCAL_CONNECT'b111;

  always_comb begin
    local_dest_id[4] = '0;
    local_message_ready_out_reg[4] = '0;

    local_dest_id[4][LOCAL_4_0_CH] = (`MIN_PER_N_L)'(unsigned'(0));
    local_dest_id[4][LOCAL_4_5_CH] = (`MIN_PER_N_L)'(unsigned'(5));
    local_dest_id[4][LOCAL_4_6_CH] = (`MIN_PER_N_L)'(unsigned'(6));
    local_message_ready_out_reg[4][LOCAL_4_0_CH] = local_message_ready_in0_from4;
    local_message_ready_out_reg[4][LOCAL_4_5_CH] = local_message_ready_in[5];
    local_message_ready_out_reg[4][LOCAL_4_6_CH] = local_message_ready_in6_from4;
  end

  arb_prio_data #(
    .WIDTH       ( $bits(et_link_neigh_rsp_info_t)                ),
    .NUM_CLIENTS ( 3                                              )
  ) local_message_min4_arb (
    // Bidding
    .bid             ( {local_message_valid_out_reg[0][LOCAL_0_4_CH],
    local_message_valid_out_reg[5][LOCAL_5_4_CH],
    local_message_valid_out_reg[6][LOCAL_6_4_CH]} ),
    .stall           ( !local_message_ready_in[4]                     ),
    // Data inputs
    .data_in         ( {local_message_info_out_reg[0],
    local_message_info_out_reg[5],
    local_message_info_out_reg[6]}                ),
    // Result
    .grant           ( {local_message_ready_in4_from0,
    local_message_ready_in4_from5,
    local_message_ready_in4_from6}                ),
    .data_winner     ( local_message_info_in[4]                       )
  );

  assign local_message_valid_in[4] = local_message_valid_out_reg[0][LOCAL_0_4_CH] |
                                     local_message_valid_out_reg[5][LOCAL_5_4_CH] |
                                     local_message_valid_out_reg[6][LOCAL_6_4_CH];

  // Minion 3 is connected to minion 2
  assign local_channel_mask[3] = `NEIGH_MAX_LOCAL_CONNECT'b001;

  always_comb begin
    local_dest_id[3] = '0;
    local_message_ready_out_reg[3] = '0;

    local_dest_id[3][LOCAL_3_2_CH] = (`MIN_PER_N_L)'(unsigned'(2));
    local_message_ready_out_reg[3][LOCAL_3_2_CH] = local_message_ready_in2_from3;
  end

  assign local_message_info_in[3]  = local_message_info_out_reg[2];
  assign local_message_valid_in[3] = local_message_valid_out_reg[2][LOCAL_2_3_CH];

  // Minion 2 is connected to minions 0 and 3
  assign local_channel_mask[2] = `NEIGH_MAX_LOCAL_CONNECT'b011;

  always_comb begin
    local_dest_id[2] = '0;
    local_message_ready_out_reg[2] = '0;

    local_dest_id[2][LOCAL_2_0_CH] = (`MIN_PER_N_L)'(unsigned'(0));
    local_dest_id[2][LOCAL_2_3_CH] = (`MIN_PER_N_L)'(unsigned'(3));
    local_message_ready_out_reg[2][LOCAL_2_0_CH] = local_message_ready_in0_from2;
    local_message_ready_out_reg[2][LOCAL_2_3_CH] = local_message_ready_in[3];
  end

  arb_prio_data #(
    .WIDTH       ( $bits(et_link_neigh_rsp_info_t)                ),
    .NUM_CLIENTS ( 2                                              )
  ) local_message_min2_arb (
    // Bidding
    .bid             ( {local_message_valid_out_reg[0][LOCAL_0_2_CH],
                     local_message_valid_out_reg[3][LOCAL_3_2_CH]} ),
    .stall           ( !local_message_ready_in[2]                  ),
    // Data inputs
    .data_in         ( {local_message_info_out_reg[0],
                     local_message_info_out_reg[3]}                ),
    // Result
    .grant           ( {local_message_ready_in2_from0,
                      local_message_ready_in2_from3}                ),
    .data_winner     ( local_message_info_in[2]                     )
  );

  assign local_message_valid_in[2] = local_message_valid_out_reg[0][LOCAL_0_2_CH] |
  local_message_valid_out_reg[3][LOCAL_3_2_CH];

  // Minion 1 is connected to minion 0
  assign local_channel_mask[1] = `NEIGH_MAX_LOCAL_CONNECT'b001;

  always_comb begin
    local_dest_id[1] = '0;
    local_message_ready_out_reg[1] = '0;

    local_dest_id[1][LOCAL_1_0_CH] = (`MIN_PER_N_L)'(unsigned'(0));
    local_message_ready_out_reg[1][LOCAL_1_0_CH] = local_message_ready_in0_from1;
  end

  assign local_message_info_in[1]  = local_message_info_out_reg[0];
  assign local_message_valid_in[1] = local_message_valid_out_reg[0][LOCAL_0_1_CH];

  // Minion 0 is connected to minions 1, 2 and 4
  assign local_channel_mask[0] = `NEIGH_MAX_LOCAL_CONNECT'b111;

  always_comb begin
    local_dest_id[0] = '0;
    local_message_ready_out_reg[0] = '0;

    local_dest_id[0][LOCAL_0_1_CH] = (`MIN_PER_N_L)'(unsigned'(1));
    local_dest_id[0][LOCAL_0_2_CH] = (`MIN_PER_N_L)'(unsigned'(2));
    local_dest_id[0][LOCAL_0_4_CH] = (`MIN_PER_N_L)'(unsigned'(4));
    local_message_ready_out_reg[0][LOCAL_0_1_CH] = local_message_ready_in[1];
    local_message_ready_out_reg[0][LOCAL_0_2_CH] = local_message_ready_in2_from0;
    local_message_ready_out_reg[0][LOCAL_0_4_CH] = local_message_ready_in4_from0;
  end

  arb_prio_data #(
    .WIDTH       ( $bits(et_link_neigh_rsp_info_t)                ),
    .NUM_CLIENTS ( 3                                              )
  ) local_message_min0_arb (
    // Bidding
    .bid             ( {local_message_valid_out_reg[1][LOCAL_1_0_CH],
                        local_message_valid_out_reg[2][LOCAL_2_0_CH],
                        local_message_valid_out_reg[4][LOCAL_4_0_CH]} ),
    .stall           ( !local_message_ready_in[0]                     ),
    // Data inputs
    .data_in         ( {local_message_info_out_reg[1],
                        local_message_info_out_reg[2],
                        local_message_info_out_reg[4]}                ),
    // Result
    .grant           ( {local_message_ready_in0_from1,
                       local_message_ready_in0_from2,
                       local_message_ready_in0_from4}                 ),
    .data_winner     ( local_message_info_in[0]                       )
  );

  assign local_message_valid_in[0] = local_message_valid_out_reg[1][LOCAL_1_0_CH] |
  local_message_valid_out_reg[2][LOCAL_2_0_CH] |
  local_message_valid_out_reg[4][LOCAL_4_0_CH];

  // //////////////////////////////////////////////////////////////////////////////
  // Connect output debug signals
  // //////////////////////////////////////////////////////////////////////////////
  always_comb begin
    // Connect cooperative TLoad signals
    dbg_sm_signals.local_message_valid_out = {local_message_valid_out_reg[7][LOCAL_7_6_CH] & local_message_ready_out_reg[7][LOCAL_7_6_CH],
    local_message_valid_out_reg[6][LOCAL_6_4_CH] & local_message_ready_out_reg[6][LOCAL_6_4_CH],
    local_message_valid_out_reg[6][LOCAL_6_7_CH] & local_message_ready_out_reg[6][LOCAL_6_7_CH],
    local_message_valid_out_reg[5][LOCAL_5_4_CH] & local_message_ready_out_reg[5][LOCAL_5_4_CH],
    local_message_valid_out_reg[4][LOCAL_4_0_CH] & local_message_ready_out_reg[4][LOCAL_4_0_CH],
    local_message_valid_out_reg[4][LOCAL_4_5_CH] & local_message_ready_out_reg[4][LOCAL_4_5_CH],
    local_message_valid_out_reg[4][LOCAL_4_6_CH] & local_message_ready_out_reg[4][LOCAL_4_6_CH],
    local_message_valid_out_reg[3][LOCAL_3_2_CH] & local_message_ready_out_reg[3][LOCAL_3_2_CH],
    local_message_valid_out_reg[2][LOCAL_2_0_CH] & local_message_ready_out_reg[2][LOCAL_2_0_CH],
    local_message_valid_out_reg[2][LOCAL_2_3_CH] & local_message_ready_out_reg[2][LOCAL_2_3_CH],
    local_message_valid_out_reg[1][LOCAL_1_0_CH] & local_message_ready_out_reg[1][LOCAL_1_0_CH],
    local_message_valid_out_reg[0][LOCAL_0_1_CH] & local_message_ready_out_reg[0][LOCAL_0_1_CH],
    local_message_valid_out_reg[0][LOCAL_0_2_CH] & local_message_ready_out_reg[0][LOCAL_0_2_CH],
    local_message_valid_out_reg[0][LOCAL_0_4_CH] & local_message_ready_out_reg[0][LOCAL_0_4_CH]};
  end

endmodule

