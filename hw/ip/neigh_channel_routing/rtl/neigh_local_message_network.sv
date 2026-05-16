// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_local_message_network #(
  parameter int unsigned NrMinions       = neigh_pkg::MinPerN,
  parameter int unsigned MaxLocalConnect = neigh_pkg::NeighMaxLocalConnect,
  localparam int unsigned MinionIdxWidth = (NrMinions > 1) ? $clog2(NrMinions) : 1
) (
  input  logic                                                        clk_i,
  input  logic                                                        rst_ni,

  output logic [NrMinions-1:0][MaxLocalConnect-1:0]                   local_channel_mask_o,
  output logic [NrMinions-1:0][MaxLocalConnect-1:0][MinionIdxWidth-1:0] local_dest_id_o,

  output logic [NrMinions-1:0]                                       local_message_ready_out_o,
  input  logic [NrMinions-1:0][MaxLocalConnect-1:0]                   local_message_valid_out_i,
  input  etlink_pkg::neigh_rsp_t [NrMinions-1:0]                      local_message_info_out_i,

  input  logic [NrMinions-1:0]                                       local_message_ready_in_i,
  output logic [NrMinions-1:0]                                       local_message_valid_in_o,
  output etlink_pkg::neigh_rsp_t [NrMinions-1:0]                      local_message_info_in_o,

  output neigh_pkg::fln_dbg_sm_t                                      dbg_sm_signals_o
);

  localparam int unsigned Local76Ch = 0;
  localparam int unsigned Local64Ch = 0;
  localparam int unsigned Local67Ch = 1;
  localparam int unsigned Local54Ch = 0;
  localparam int unsigned Local40Ch = 0;
  localparam int unsigned Local45Ch = 1;
  localparam int unsigned Local46Ch = 2;
  localparam int unsigned Local32Ch = 0;
  localparam int unsigned Local20Ch = 0;
  localparam int unsigned Local23Ch = 1;
  localparam int unsigned Local10Ch = 0;
  localparam int unsigned Local01Ch = 0;
  localparam int unsigned Local02Ch = 1;
  localparam int unsigned Local04Ch = 2;

  localparam logic [MinionIdxWidth-1:0] MinId0 = 3'd0;
  localparam logic [MinionIdxWidth-1:0] MinId1 = 3'd1;
  localparam logic [MinionIdxWidth-1:0] MinId2 = 3'd2;
  localparam logic [MinionIdxWidth-1:0] MinId3 = 3'd3;
  localparam logic [MinionIdxWidth-1:0] MinId4 = 3'd4;
  localparam logic [MinionIdxWidth-1:0] MinId5 = 3'd5;
  localparam logic [MinionIdxWidth-1:0] MinId6 = 3'd6;
  localparam logic [MinionIdxWidth-1:0] MinId7 = 3'd7;

  logic [NrMinions-1:0][MaxLocalConnect-1:0] local_message_ready_out_reg;
  logic [NrMinions-1:0][MaxLocalConnect-1:0] local_message_valid_out_q;
  etlink_pkg::neigh_rsp_t [NrMinions-1:0]    local_message_info_out_q;

  logic local_message_ready_in6_from4;
  logic local_message_ready_in6_from7;
  logic local_message_ready_in4_from0;
  logic local_message_ready_in4_from5;
  logic local_message_ready_in4_from6;
  logic local_message_ready_in2_from0;
  logic local_message_ready_in2_from3;
  logic local_message_ready_in0_from1;
  logic local_message_ready_in0_from2;
  logic local_message_ready_in0_from4;

  function automatic logic [1:0] prio2_grant(
    input logic [1:0] bid,
    input logic       stall
  );
    logic [1:0] grant;
    begin
      grant = '0;
      if (!stall) begin
        if (bid[1]) begin
          grant[1] = 1'b1;
        end else if (bid[0]) begin
          grant[0] = 1'b1;
        end
      end
      return grant;
    end
  endfunction

  function automatic logic [2:0] prio3_grant(
    input logic [2:0] bid,
    input logic       stall
  );
    logic [2:0] grant;
    begin
      grant = '0;
      if (!stall) begin
        if (bid[2]) begin
          grant[2] = 1'b1;
        end else if (bid[1]) begin
          grant[1] = 1'b1;
        end else if (bid[0]) begin
          grant[0] = 1'b1;
        end
      end
      return grant;
    end
  endfunction

  for (genvar i = 0; i < NrMinions; i++) begin : gen_local_message_out_ff
    always_ff @(posedge clk_i or negedge rst_ni) begin
      if (!rst_ni) begin
        local_message_valid_out_q[i] <= '0;
      end else if (local_message_ready_out_o[i]) begin
        local_message_valid_out_q[i] <= local_message_valid_out_i[i];
      end
    end

    // Original EN_FF storage has no reset; preserve the unreset payload registers.
    always_ff @(posedge clk_i) begin
      if (local_message_ready_out_o[i]) begin
        local_message_info_out_q[i] <= local_message_info_out_i[i];
      end
    end

    assign local_message_ready_out_o[i] = |(local_message_valid_out_q[i] & local_message_ready_out_reg[i])
                                        | ~(|local_message_valid_out_q[i]);
  end

  logic [1:0] arb_min6_grant;
  logic [2:0] arb_min4_grant;
  logic [1:0] arb_min2_grant;
  logic [2:0] arb_min0_grant;

  always_comb begin
    local_channel_mask_o = '0;
    local_dest_id_o = '0;
    local_message_ready_out_reg = '0;
    local_message_valid_in_o = '0;
    local_message_info_in_o = '0;

    // Minion 7 is connected to minion 6.
    local_channel_mask_o[7] = 3'b001;
    local_dest_id_o[7][Local76Ch] = MinId6;
    local_message_ready_out_reg[7][Local76Ch] = local_message_ready_in6_from7;
    local_message_info_in_o[7] = local_message_info_out_q[6];
    local_message_valid_in_o[7] = local_message_valid_out_q[6][Local67Ch];

    // Minion 6 is connected to minions 4 and 7.
    local_channel_mask_o[6] = 3'b011;
    local_dest_id_o[6][Local64Ch] = MinId4;
    local_dest_id_o[6][Local67Ch] = MinId7;
    local_message_ready_out_reg[6][Local64Ch] = local_message_ready_in4_from6;
    local_message_ready_out_reg[6][Local67Ch] = local_message_ready_in_i[7];
    local_message_valid_in_o[6] = local_message_valid_out_q[4][Local46Ch]
                                | local_message_valid_out_q[7][Local76Ch];
    if (local_message_valid_out_q[4][Local46Ch]) begin
      local_message_info_in_o[6] = local_message_info_out_q[4];
    end else begin
      local_message_info_in_o[6] = local_message_info_out_q[7];
    end

    // Minion 5 is connected to minion 4.
    local_channel_mask_o[5] = 3'b001;
    local_dest_id_o[5][Local54Ch] = MinId4;
    local_message_ready_out_reg[5][Local54Ch] = local_message_ready_in4_from5;
    local_message_info_in_o[5] = local_message_info_out_q[4];
    local_message_valid_in_o[5] = local_message_valid_out_q[4][Local45Ch];

    // Minion 4 is connected to minions 0, 5, and 6.
    local_channel_mask_o[4] = 3'b111;
    local_dest_id_o[4][Local40Ch] = MinId0;
    local_dest_id_o[4][Local45Ch] = MinId5;
    local_dest_id_o[4][Local46Ch] = MinId6;
    local_message_ready_out_reg[4][Local40Ch] = local_message_ready_in0_from4;
    local_message_ready_out_reg[4][Local45Ch] = local_message_ready_in_i[5];
    local_message_ready_out_reg[4][Local46Ch] = local_message_ready_in6_from4;
    local_message_valid_in_o[4] = local_message_valid_out_q[0][Local04Ch]
                                | local_message_valid_out_q[5][Local54Ch]
                                | local_message_valid_out_q[6][Local64Ch];
    if (local_message_valid_out_q[0][Local04Ch]) begin
      local_message_info_in_o[4] = local_message_info_out_q[0];
    end else if (local_message_valid_out_q[5][Local54Ch]) begin
      local_message_info_in_o[4] = local_message_info_out_q[5];
    end else begin
      local_message_info_in_o[4] = local_message_info_out_q[6];
    end

    // Minion 3 is connected to minion 2.
    local_channel_mask_o[3] = 3'b001;
    local_dest_id_o[3][Local32Ch] = MinId2;
    local_message_ready_out_reg[3][Local32Ch] = local_message_ready_in2_from3;
    local_message_info_in_o[3] = local_message_info_out_q[2];
    local_message_valid_in_o[3] = local_message_valid_out_q[2][Local23Ch];

    // Minion 2 is connected to minions 0 and 3.
    local_channel_mask_o[2] = 3'b011;
    local_dest_id_o[2][Local20Ch] = MinId0;
    local_dest_id_o[2][Local23Ch] = MinId3;
    local_message_ready_out_reg[2][Local20Ch] = local_message_ready_in0_from2;
    local_message_ready_out_reg[2][Local23Ch] = local_message_ready_in_i[3];
    local_message_valid_in_o[2] = local_message_valid_out_q[0][Local02Ch]
                                | local_message_valid_out_q[3][Local32Ch];
    if (local_message_valid_out_q[0][Local02Ch]) begin
      local_message_info_in_o[2] = local_message_info_out_q[0];
    end else begin
      local_message_info_in_o[2] = local_message_info_out_q[3];
    end

    // Minion 1 is connected to minion 0.
    local_channel_mask_o[1] = 3'b001;
    local_dest_id_o[1][Local10Ch] = MinId0;
    local_message_ready_out_reg[1][Local10Ch] = local_message_ready_in0_from1;
    local_message_info_in_o[1] = local_message_info_out_q[0];
    local_message_valid_in_o[1] = local_message_valid_out_q[0][Local01Ch];

    // Minion 0 is connected to minions 1, 2, and 4.
    local_channel_mask_o[0] = 3'b111;
    local_dest_id_o[0][Local01Ch] = MinId1;
    local_dest_id_o[0][Local02Ch] = MinId2;
    local_dest_id_o[0][Local04Ch] = MinId4;
    local_message_ready_out_reg[0][Local01Ch] = local_message_ready_in_i[1];
    local_message_ready_out_reg[0][Local02Ch] = local_message_ready_in2_from0;
    local_message_ready_out_reg[0][Local04Ch] = local_message_ready_in4_from0;
    local_message_valid_in_o[0] = local_message_valid_out_q[1][Local10Ch]
                                | local_message_valid_out_q[2][Local20Ch]
                                | local_message_valid_out_q[4][Local40Ch];
    if (local_message_valid_out_q[1][Local10Ch]) begin
      local_message_info_in_o[0] = local_message_info_out_q[1];
    end else if (local_message_valid_out_q[2][Local20Ch]) begin
      local_message_info_in_o[0] = local_message_info_out_q[2];
    end else begin
      local_message_info_in_o[0] = local_message_info_out_q[4];
    end
  end

  always_comb begin
    arb_min6_grant = prio2_grant({local_message_valid_out_q[4][Local46Ch],
                                  local_message_valid_out_q[7][Local76Ch]},
                                 !local_message_ready_in_i[6]);
    {local_message_ready_in6_from4, local_message_ready_in6_from7} = arb_min6_grant;

    arb_min4_grant = prio3_grant({local_message_valid_out_q[0][Local04Ch],
                                  local_message_valid_out_q[5][Local54Ch],
                                  local_message_valid_out_q[6][Local64Ch]},
                                 !local_message_ready_in_i[4]);
    {local_message_ready_in4_from0, local_message_ready_in4_from5,
     local_message_ready_in4_from6} = arb_min4_grant;

    arb_min2_grant = prio2_grant({local_message_valid_out_q[0][Local02Ch],
                                  local_message_valid_out_q[3][Local32Ch]},
                                 !local_message_ready_in_i[2]);
    {local_message_ready_in2_from0, local_message_ready_in2_from3} = arb_min2_grant;

    arb_min0_grant = prio3_grant({local_message_valid_out_q[1][Local10Ch],
                                  local_message_valid_out_q[2][Local20Ch],
                                  local_message_valid_out_q[4][Local40Ch]},
                                 !local_message_ready_in_i[0]);
    {local_message_ready_in0_from1, local_message_ready_in0_from2,
     local_message_ready_in0_from4} = arb_min0_grant;
  end

  always_comb begin
    dbg_sm_signals_o.local_message_valid_out = {
      local_message_valid_out_q[7][Local76Ch] & local_message_ready_out_reg[7][Local76Ch],
      local_message_valid_out_q[6][Local64Ch] & local_message_ready_out_reg[6][Local64Ch],
      local_message_valid_out_q[6][Local67Ch] & local_message_ready_out_reg[6][Local67Ch],
      local_message_valid_out_q[5][Local54Ch] & local_message_ready_out_reg[5][Local54Ch],
      local_message_valid_out_q[4][Local40Ch] & local_message_ready_out_reg[4][Local40Ch],
      local_message_valid_out_q[4][Local45Ch] & local_message_ready_out_reg[4][Local45Ch],
      local_message_valid_out_q[4][Local46Ch] & local_message_ready_out_reg[4][Local46Ch],
      local_message_valid_out_q[3][Local32Ch] & local_message_ready_out_reg[3][Local32Ch],
      local_message_valid_out_q[2][Local20Ch] & local_message_ready_out_reg[2][Local20Ch],
      local_message_valid_out_q[2][Local23Ch] & local_message_ready_out_reg[2][Local23Ch],
      local_message_valid_out_q[1][Local10Ch] & local_message_ready_out_reg[1][Local10Ch],
      local_message_valid_out_q[0][Local01Ch] & local_message_ready_out_reg[0][Local01Ch],
      local_message_valid_out_q[0][Local02Ch] & local_message_ready_out_reg[0][Local02Ch],
      local_message_valid_out_q[0][Local04Ch] & local_message_ready_out_reg[0][Local04Ch]
    };
  end

endmodule : neigh_local_message_network
