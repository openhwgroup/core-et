// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_vpu_trans_tb
  #(
    parameter bit EnableExtraTrans = 1'b0
  )
(
  input  logic                                                     clk_i,
  input  logic                                                     rst_ni,
  input  logic                                                     id_valid_i,
  input  logic                                                     id_core_thread_id_i,
  input  logic [minion_pkg::InstSize-1:0]                          id_instruction_i,
  input  logic                                                     ex_valid_i,
  input  logic                                                     ex_core_thread_id_i,
  input  logic [minion_pkg::InstSize-1:0]                          ex_instruction_i,
  input  logic [minion_pkg::VpuLaneNum-1:0]                        ex_mask_rf0_i,
  input  logic                                                     chicken_bit_vputrans_i,
  input  logic                                                     f6_killed_i,
  input  logic                                                     f7_killed_i,
  input  logic                                                     f8_killed_i,
  output logic                                                     orig_id_insert_en_next_o,
  output logic [minion_pkg::InstSize-1:0]                          orig_id_insert_inst_next_o,
  output logic                                                     orig_id_insert_en_o,
  output logic                                                     orig_id_trans_thread_id_o,
  output logic                                                     orig_ex_trans_thread_id_o,
  output logic                                                     orig_f6_trans_ctrl_en_o,
  output logic [$bits(vpu_pkg::trans_scoreboard_slot)-1:0]         orig_id_trans_scoreboard_0_o,
  output logic [$bits(vpu_pkg::trans_scoreboard_slot)-1:0]         orig_id_trans_scoreboard_1_o,
  output logic [$bits(vpu_pkg::trans_scoreboard_slot)-1:0]         orig_id_trans_scoreboard_2_o,
  output logic [$bits(vpu_pkg::trans_scoreboard_slot)-1:0]         orig_id_trans_scoreboard_3_o,
  output logic [$bits(vpu_pkg::trans_scoreboard_slot)-1:0]         orig_id_trans_scoreboard_4_o,
  output logic [$bits(vpu_pkg::trans_scoreboard_slot)-1:0]         orig_id_trans_scoreboard_5_o,
  output logic [minion_pkg::VpuLaneNum-1:0]                        orig_f6_trans_wmask_o,
  output logic                                                     orig_f6_trans_thread_id_o,
  output logic                                                     orig_f8_trans_ctrl_en_o,
  output logic                                                     orig_f8_trans_fcsr_en_o,
  output logic                                                     orig_id_trans_busy_o,
  output logic [$bits(vpu_pkg::trans_state_t)-1:0]                 orig_state_packed_o,
  output logic                                                     new_id_insert_en_next_o,
  output logic [minion_pkg::InstSize-1:0]                          new_id_insert_inst_next_o,
  output logic                                                     new_id_insert_en_o,
  output logic                                                     new_id_trans_thread_id_o,
  output logic                                                     new_ex_trans_thread_id_o,
  output logic                                                     new_f6_trans_ctrl_en_o,
  output logic [$bits(vpu_pkg::trans_scoreboard_slot)-1:0]         new_id_trans_scoreboard_0_o,
  output logic [$bits(vpu_pkg::trans_scoreboard_slot)-1:0]         new_id_trans_scoreboard_1_o,
  output logic [$bits(vpu_pkg::trans_scoreboard_slot)-1:0]         new_id_trans_scoreboard_2_o,
  output logic [$bits(vpu_pkg::trans_scoreboard_slot)-1:0]         new_id_trans_scoreboard_3_o,
  output logic [$bits(vpu_pkg::trans_scoreboard_slot)-1:0]         new_id_trans_scoreboard_4_o,
  output logic [$bits(vpu_pkg::trans_scoreboard_slot)-1:0]         new_id_trans_scoreboard_5_o,
  output logic [minion_pkg::VpuLaneNum-1:0]                        new_f6_trans_wmask_o,
  output logic                                                     new_f6_trans_thread_id_o,
  output logic                                                     new_f8_trans_ctrl_en_o,
  output logic                                                     new_f8_trans_fcsr_en_o,
  output logic                                                     new_id_trans_busy_o,
  output logic [$bits(vpu_pkg::trans_state_t)-1:0]                 new_state_packed_o
);

  localparam int unsigned ScoreboardSlots = minion_pkg::VpuTransScoreboardSize / 2;
  localparam int unsigned ScoreboardSlotWidth = $bits(vpu_pkg::trans_scoreboard_slot);

  logic [ScoreboardSlots*ScoreboardSlotWidth-1:0] orig_id_trans_scoreboard_packed;
  trans_state                     orig_state;
  vpu_pkg::trans_scoreboard_slot  new_id_trans_scoreboard [ScoreboardSlots-1:0];
  vpu_pkg::trans_state_t          new_state;

  vpu_trans_orig u_orig (
    .clock_aon              (clk_i),
    .reset                  (!rst_ni),
    .id_valid               (id_valid_i),
    .id_core_thread_id      (id_core_thread_id_i),
    .id_instruction         (id_instruction_i),
    .ex_valid               (ex_valid_i),
    .ex_core_thread_id      (ex_core_thread_id_i),
    .ex_instruction         (ex_instruction_i),
    .ex_mask_rf0            (ex_mask_rf0_i),
    .chicken_bit_vputrans   (chicken_bit_vputrans_i),
    .f6_killed              (f6_killed_i),
    .f7_killed              (f7_killed_i),
    .f8_killed              (f8_killed_i),
    .id_insert_en_next      (orig_id_insert_en_next_o),
    .id_insert_inst_next    (orig_id_insert_inst_next_o),
    .id_insert_en           (orig_id_insert_en_o),
    .id_trans_thread_id     (orig_id_trans_thread_id_o),
    .ex_trans_thread_id     (orig_ex_trans_thread_id_o),
    .f6_trans_ctrl_en       (orig_f6_trans_ctrl_en_o),
    .id_trans_scoreboard    (orig_id_trans_scoreboard_packed),
    .f6_trans_wmask         (orig_f6_trans_wmask_o),
    .f6_trans_thread_id     (orig_f6_trans_thread_id_o),
    .f8_trans_ctrl_en       (orig_f8_trans_ctrl_en_o),
    .f8_trans_fcsr_en       (orig_f8_trans_fcsr_en_o),
    .id_trans_busy          (orig_id_trans_busy_o),
    .state                  (orig_state)
  );

  vpu_trans #(
    .EnableExtraTrans       (EnableExtraTrans)
  ) u_new (
    .clk_aon_i              (clk_i),
    .rst_ni                 (rst_ni),
    .id_valid_i             (id_valid_i),
    .id_core_thread_id_i    (id_core_thread_id_i),
    .id_instruction_i       (id_instruction_i),
    .ex_valid_i             (ex_valid_i),
    .ex_core_thread_id_i    (ex_core_thread_id_i),
    .ex_instruction_i       (ex_instruction_i),
    .ex_mask_rf0_i          (ex_mask_rf0_i),
    .chicken_bit_vputrans_i (chicken_bit_vputrans_i),
    .f6_killed_i            (f6_killed_i),
    .f7_killed_i            (f7_killed_i),
    .f8_killed_i            (f8_killed_i),
    .id_insert_en_next_o    (new_id_insert_en_next_o),
    .id_insert_inst_next_o  (new_id_insert_inst_next_o),
    .id_insert_en_o         (new_id_insert_en_o),
    .id_trans_thread_id_o   (new_id_trans_thread_id_o),
    .ex_trans_thread_id_o   (new_ex_trans_thread_id_o),
    .f6_trans_ctrl_en_o     (new_f6_trans_ctrl_en_o),
    .id_trans_scoreboard_o  (new_id_trans_scoreboard),
    .f6_trans_wmask_o       (new_f6_trans_wmask_o),
    .f6_trans_thread_id_o   (new_f6_trans_thread_id_o),
    .f8_trans_ctrl_en_o     (new_f8_trans_ctrl_en_o),
    .f8_trans_fcsr_en_o     (new_f8_trans_fcsr_en_o),
    .id_trans_busy_o        (new_id_trans_busy_o),
    .state_o                (new_state)
  );

  always_comb begin
    orig_id_trans_scoreboard_0_o = orig_id_trans_scoreboard_packed[ScoreboardSlotWidth-1 -: ScoreboardSlotWidth];
    orig_id_trans_scoreboard_1_o = orig_id_trans_scoreboard_packed[(2*ScoreboardSlotWidth)-1 -: ScoreboardSlotWidth];
    orig_id_trans_scoreboard_2_o = orig_id_trans_scoreboard_packed[(3*ScoreboardSlotWidth)-1 -: ScoreboardSlotWidth];
    orig_id_trans_scoreboard_3_o = orig_id_trans_scoreboard_packed[(4*ScoreboardSlotWidth)-1 -: ScoreboardSlotWidth];
    orig_id_trans_scoreboard_4_o = orig_id_trans_scoreboard_packed[(5*ScoreboardSlotWidth)-1 -: ScoreboardSlotWidth];
    orig_id_trans_scoreboard_5_o = orig_id_trans_scoreboard_packed[(6*ScoreboardSlotWidth)-1 -: ScoreboardSlotWidth];
    new_id_trans_scoreboard_0_o = new_id_trans_scoreboard[0];
    new_id_trans_scoreboard_1_o = new_id_trans_scoreboard[1];
    new_id_trans_scoreboard_2_o = new_id_trans_scoreboard[2];
    new_id_trans_scoreboard_3_o = new_id_trans_scoreboard[3];
    new_id_trans_scoreboard_4_o = new_id_trans_scoreboard[4];
    new_id_trans_scoreboard_5_o = new_id_trans_scoreboard[5];
    orig_state_packed_o = orig_state;
    new_state_packed_o = new_state;
  end

endmodule
