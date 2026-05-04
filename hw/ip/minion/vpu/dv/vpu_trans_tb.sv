// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module vpu_trans_tb
  import minion_pkg::*;
  import vpu_pkg::*;
  #(
    parameter bit EnableExtraTrans = 1'b0
  )
(
  input  logic                                                      clk_i,
  input  logic                                                      rst_ni,
  input  logic                                                      id_valid_i,
  input  logic                                                      id_core_thread_id_i,
  input  logic [InstSize-1:0]                                       id_instruction_i,
  input  logic                                                      ex_valid_i,
  input  logic                                                      ex_core_thread_id_i,
  input  logic [InstSize-1:0]                                       ex_instruction_i,
  input  logic [VpuLaneNum-1:0]                                     ex_mask_rf0_i,
  input  logic                                                      chicken_bit_vputrans_i,
  input  logic                                                      f6_killed_i,
  input  logic                                                      f7_killed_i,
  input  logic                                                      f8_killed_i,
  output logic                                                      id_insert_en_next_o,
  output logic [InstSize-1:0]                                       id_insert_inst_next_o,
  output logic                                                      id_insert_en_o,
  output logic                                                      id_trans_thread_id_o,
  output logic                                                      ex_trans_thread_id_o,
  output logic                                                      f6_trans_ctrl_en_o,
  output logic [VpuLaneNum-1:0]                                     f6_trans_wmask_o,
  output logic                                                      f6_trans_thread_id_o,
  output logic                                                      f8_trans_ctrl_en_o,
  output logic                                                      f8_trans_fcsr_en_o,
  output logic                                                      id_trans_busy_o,
  output logic [($bits(trans_scoreboard_slot) * (VpuTransScoreboardSize / 2))-1:0] id_trans_scoreboard_packed_o,
  output logic [$bits(trans_state_t)-1:0]                           state_packed_o,
  output logic                                                      enable_extra_trans_o
);

  trans_scoreboard_slot id_trans_scoreboard [VpuTransScoreboardSize/2-1:0];
  trans_state_t         state;

  vpu_trans #(
    .EnableExtraTrans (EnableExtraTrans)
  ) u_dut (
    .clk_aon_i               (clk_i),
    .rst_ni,
    .id_valid_i,
    .id_core_thread_id_i,
    .id_instruction_i,
    .ex_valid_i,
    .ex_core_thread_id_i,
    .ex_instruction_i,
    .ex_mask_rf0_i,
    .chicken_bit_vputrans_i,
    .f6_killed_i,
    .f7_killed_i,
    .f8_killed_i,
    .id_insert_en_next_o,
    .id_insert_inst_next_o,
    .id_insert_en_o,
    .id_trans_thread_id_o,
    .ex_trans_thread_id_o,
    .f6_trans_ctrl_en_o,
    .id_trans_scoreboard_o   (id_trans_scoreboard),
    .f6_trans_wmask_o,
    .f6_trans_thread_id_o,
    .f8_trans_ctrl_en_o,
    .f8_trans_fcsr_en_o,
    .id_trans_busy_o,
    .state_o                 (state)
  );

  always_comb begin
    id_trans_scoreboard_packed_o = '0;
    for (int i = 0; i < (VpuTransScoreboardSize / 2); ++i) begin
      id_trans_scoreboard_packed_o[(i+1)*$bits(trans_scoreboard_slot)-1 -: $bits(trans_scoreboard_slot)] = id_trans_scoreboard[i];
    end
    state_packed_o = state;
    enable_extra_trans_o = EnableExtraTrans;
  end

endmodule
