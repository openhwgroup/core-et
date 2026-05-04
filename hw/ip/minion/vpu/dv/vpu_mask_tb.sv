// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module vpu_mask_tb
  import minion_pkg::*;
  import vpu_pkg::*;
(
  input  logic                           clk_i,
  input  logic                           rst_ni,
  input  logic                           ex_in_valid_i,
  input  logic                           ex_inst_valid_i,
  input  logic [MregAddrSize-1:0]        ex_regfile_raddr1_i,
  input  logic [MregAddrSize-1:0]        ex_regfile_raddr2_i,
  input  logic [$bits(vpu_cmd_e)-1:0]    ex_cmd_i,
  input  logic [VpuLaneNum-1:0]          ex_imm_i,
  input  logic                           ex_fromint_i,
  input  logic [XregSize-1:0]            ex_fromint_data_i,
  input  logic                           ex_thread_id_i,
  input  logic                           ex_ignore_mask_i,
  input  logic                           f2_thread_id_i,
  input  logic                           f2_wen_i,
  input  logic                           f2_core_kill_i,
  input  logic [MregAddrSize-1:0]        f2_waddr_i,
  input  logic                           f2_maskop_i,
  input  logic                           f2_tointm_i,
  input  logic                           f3_thread_id_i,
  input  logic                           f3_regmask_from_txfma_i,
  input  logic                           f3_regmask_from_short_i,
  input  logic                           f3_core_kill_i,
  input  logic [VpuLaneNum-1:0]          f3_regmask_wdata_lane_i,
  input  logic                           f4_regmask_from_txfma_i,
  input  logic                           f4_core_kill_i,
  input  logic                           f5_thread_id_i,
  input  logic                           f6_thread_id_i,
  input  logic                           f7_thread_id_i,
  input  logic                           f8_thread_id_i,
  input  logic [VpuLaneNum-1:0]          f8_regmask_comp_res_i,

  output logic [VpuLaneNum-1:0]          ex_mask_rf0_o,
  output logic [VpuLaneNum-1:0]          ex_mask_in1_o,
  output logic [VpuLaneNum-1:0]          f2_mask_rf0_o,
  output logic [XregSize-1:0]            f8_toint_data_o,
  output logic [VpuLaneNum-1:0]          f8_regmask_store_o,
  output logic [VpuMaskScoreboardSize-1:0] wb_mask_valid_o,
  output logic [VpuMaskScoreboardSize*$bits(minion_mreg_dest_t)-1:0] wb_mask_dest_packed_o,
  output logic [VpuRegmaskNum*VpuLaneNum-1:0] mask_rf_thread0_o,
  output logic [VpuRegmaskNum*VpuLaneNum-1:0] mask_rf_thread1_o
);

  minion_mreg_dest_t [VpuMaskScoreboardSize-1:0] wb_mask_dest;

  vpu_mask u_dut (
    .clk_i,
    .rst_ni,
    .ex_in_valid_i,
    .ex_inst_valid_i,
    .ex_regfile_raddr1_i,
    .ex_regfile_raddr2_i,
    .ex_cmd_i                (vpu_cmd_e'(ex_cmd_i)),
    .ex_imm_i,
    .ex_fromint_i,
    .ex_fromint_data_i,
    .ex_thread_id_i,
    .ex_ignore_mask_i,
    .f2_thread_id_i,
    .f2_wen_i,
    .f2_core_kill_i,
    .f2_waddr_i,
    .f2_maskop_i,
    .f2_tointm_i,
    .f3_thread_id_i,
    .f3_regmask_from_txfma_i,
    .f3_regmask_from_short_i,
    .f3_core_kill_i,
    .f3_regmask_wdata_lane_i,
    .f4_regmask_from_txfma_i,
    .f4_core_kill_i,
    .f5_thread_id_i,
    .f6_thread_id_i,
    .f7_thread_id_i,
    .f8_thread_id_i,
    .f8_regmask_comp_res_i,
    .ex_mask_rf0_o,
    .ex_mask_in1_o,
    .f2_mask_rf0_o,
    .f8_toint_data_o,
    .f8_regmask_store_o,
    .wb_mask_valid_o,
    .wb_mask_dest_o          (wb_mask_dest)
  );

  assign wb_mask_dest_packed_o = wb_mask_dest;
  assign mask_rf_thread0_o = u_dut.ex_mask_rf_q[0];
  assign mask_rf_thread1_o = u_dut.ex_mask_rf_q[1];

endmodule
