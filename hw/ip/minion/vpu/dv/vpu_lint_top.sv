// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module vpu_lint_top
  import minion_pkg::*;
  import vpu_pkg::*;
;

  logic clk_i;
  logic rst_ni;
  logic [CoreGscCntBits-1:0] lane_id_i;
  logic [2:0][FregAddrSize-1:0] ex_rf_raddr_i;
  logic [2:0] ex_regfile_thread_id_i;
  logic [2:0][VpuDataSSz-1:0] ex_rf_rdata_i;
  vpu_input_t ex_req_lane_i;
  logic [VpuDataSSz-1:0] ex_fromint_data_i;
  logic [VpuDataSSz-1:0] ex_tena_rf_rdata_i;
  logic [VpuDataSSz-1:0] ex_tenb_rf_rdata_i;
  logic ex_tena_regfile_bypass_en_i;
  logic ex_tenb_regfile_bypass_en_i;
  vpu_bypass_force_ctrl_t ex_bypass_force_ctrl_i;
  vpu_output_t f3_sh_sw_bits_i;
  logic [$clog2(VpuRegfileNum)-1:0] f3_regfile_waddr_i;
  logic f3_regfile_wmask_i;
  logic f3_thread_id_bypass_i;
  logic f3_bypass_clk_en_i;
  logic f3_data_vector_i;
  logic [$clog2(VpuRegfileNum)-1:0] f4_regfile_waddr_i;
  logic f4_regfile_wmask_i;
  logic f4_thread_id_bypass_i;
  logic f4_bypass_clk_en_i;
  logic [$clog2(VpuRegfileNum)-1:0] f4_regfile_waddr_l_i;
  logic [1:0] f4_regfile_wmask_l_i;
  logic f4_regfile_thread_id_l_i;
  logic [VpuDataSSz-1:0] f4_regfile_wdata_l_i;
  logic [$clog2(VpuRegfileNum)-1:0] f5_regfile_waddr_i;
  logic f5_regfile_wmask_i;
  logic f5_thread_id_bypass_i;
  logic f5_bypass_clk_en_i;
  logic [$clog2(VpuRegfileNum)-1:0] f6_regfile_waddr_i;
  logic f6_regfile_wmask_i;
  logic f6_thread_id_bypass_i;
  logic f6_bypass_clk_en_i;
  logic [$clog2(VpuRegfileNum)-1:0] f7_regfile_waddr_i;
  logic f7_regfile_wmask_i;
  logic f7_thread_id_bypass_i;
  logic f7_bypass_clk_en_i;
  logic [$clog2(VpuRegfileNum)-1:0] f8_regfile_waddr_i;
  logic f8_regfile_wmask_i;
  logic f8_thread_id_bypass_i;
  vpu_output_t f8_txfma_res_i;
  logic f8_data_vector_i;
  logic f8_txfma_op_i;
  vpu_input_t ex_req_o;
  logic f3_maskop_bit_o;
  logic [CoreFcsrFlagBitsSz-1:0] f8_wflags_o;
  logic [VpuDataSSz-1:0] f8_wdata_o;
  logic [VpuDataSSz-1:0] f8_sh_sw_wdata_o;

  logic ex_in_valid_i;
  logic [$bits(vpu_cmd_e)-1:0] ex_cmd_i;
  logic [VpuDataSSz-1:0] ex_in1_i;
  logic [VpuDataSSz-1:0] ex_in2_i;
  logic [VpuDataSSz-1:0] ex_in3_i;
  logic [19:0] ex_imm_sh_sw_i;
  logic [$bits(vpu_dtype_e)-1:0] ex_dtype_i;
  logic ex_mask_in1_i;
  logic [VpuDataSSz-1:0] f2_swizz_in1_i;
  logic [VpuDataSSz-1:0] f2_swizz_in2_i;
  logic [VpuDataSSz-1:0] f2_swizz_in3_i;
  logic [VpuDataSSz-1:0] f2_fswizz_rdata_o;
  vpu_output_t f3_out_bits_o;
  logic ex_inst_valid_i;
  logic [MregAddrSize-1:0] ex_regfile_raddr1_i;
  logic [MregAddrSize-1:0] ex_regfile_raddr2_i;
  logic [VpuLaneNum-1:0] ex_imm_mask_i;
  logic ex_fromint_mask_i;
  logic [XregSize-1:0] ex_fromint_data_mask_i;
  logic ex_thread_id_i;
  logic ex_ignore_mask_i;
  logic f2_thread_id_i;
  logic f2_wen_i;
  logic f2_core_kill_i;
  logic [MregAddrSize-1:0] f2_waddr_i;
  logic f2_maskop_i;
  logic f2_tointm_i;
  logic f3_thread_id_i;
  logic f3_regmask_from_txfma_i;
  logic f3_regmask_from_short_i;
  logic f3_core_kill_i;
  logic [VpuLaneNum-1:0] f3_regmask_wdata_lane_i;
  logic f4_regmask_from_txfma_i;
  logic f4_core_kill_i;
  logic f5_thread_id_i;
  logic f6_thread_id_i;
  logic f7_thread_id_i;
  logic f8_thread_id_i;
  logic [VpuLaneNum-1:0] f8_regmask_comp_res_i;
  logic [VpuLaneNum-1:0] ex_mask_rf0_o;
  logic [VpuLaneNum-1:0] ex_mask_in1_o;
  logic [VpuLaneNum-1:0] f2_mask_rf0_o;
  logic [XregSize-1:0] f8_toint_data_mask_o;
  logic [VpuLaneNum-1:0] f8_regmask_store_o;
  logic [VpuMaskScoreboardSize-1:0] wb_mask_valid_o;
  minion_mreg_dest_t [VpuMaskScoreboardSize-1:0] wb_mask_dest_o;
  logic [VpuDataSSz-1:0] tensora_rd_data_o;
  logic [VpuDataSSz-1:0] tensorb_rd_data_o;
  logic [VpuDataSSz-1:0] tensorc_rd_data_o;
  logic [VpuDataSSz-1:0] tensortmp_rd_data_o;
  logic [2:0][VpuDataSSz-1:0] vpu_rf_rd_data_o;
  logic [31:0] tima_sum_o;
  logic [VpuDataSSz-1:0] tima_out_o;
  logic [VpuDataSSz-1:0] lane_tima_tenb_out_o;
  logic [VpuDataSSz-1:0] lane_tima_out_o;
  logic unused_ok;

  assign clk_i = 1'b0;
  assign rst_ni = 1'b1;
  assign lane_id_i = '0;
  assign ex_rf_raddr_i = '0;
  assign ex_regfile_thread_id_i = '0;
  assign ex_rf_rdata_i = '0;
  assign ex_req_lane_i = '0;
  assign ex_fromint_data_i = '0;
  assign ex_tena_rf_rdata_i = '0;
  assign ex_tenb_rf_rdata_i = '0;
  assign ex_tena_regfile_bypass_en_i = 1'b0;
  assign ex_tenb_regfile_bypass_en_i = 1'b0;
  assign ex_bypass_force_ctrl_i = '0;
  assign f3_sh_sw_bits_i = '0;
  assign f3_regfile_waddr_i = '0;
  assign f3_regfile_wmask_i = 1'b0;
  assign f3_thread_id_bypass_i = 1'b0;
  assign f3_bypass_clk_en_i = 1'b0;
  assign f3_data_vector_i = 1'b0;
  assign f4_regfile_waddr_i = '0;
  assign f4_regfile_wmask_i = 1'b0;
  assign f4_thread_id_bypass_i = 1'b0;
  assign f4_bypass_clk_en_i = 1'b0;
  assign f4_regfile_waddr_l_i = '0;
  assign f4_regfile_wmask_l_i = '0;
  assign f4_regfile_thread_id_l_i = 1'b0;
  assign f4_regfile_wdata_l_i = '0;
  assign f5_regfile_waddr_i = '0;
  assign f5_regfile_wmask_i = 1'b0;
  assign f5_thread_id_bypass_i = 1'b0;
  assign f5_bypass_clk_en_i = 1'b0;
  assign f6_regfile_waddr_i = '0;
  assign f6_regfile_wmask_i = 1'b0;
  assign f6_thread_id_bypass_i = 1'b0;
  assign f6_bypass_clk_en_i = 1'b0;
  assign f7_regfile_waddr_i = '0;
  assign f7_regfile_wmask_i = 1'b0;
  assign f7_thread_id_bypass_i = 1'b0;
  assign f7_bypass_clk_en_i = 1'b0;
  assign f8_regfile_waddr_i = '0;
  assign f8_regfile_wmask_i = 1'b0;
  assign f8_thread_id_bypass_i = 1'b0;
  assign f8_txfma_res_i = '0;
  assign f8_data_vector_i = 1'b0;
  assign f8_txfma_op_i = 1'b0;
  assign ex_in_valid_i = 1'b0;
  assign ex_inst_valid_i = 1'b0;
  assign ex_regfile_raddr1_i = '0;
  assign ex_regfile_raddr2_i = '0;
  assign ex_cmd_i = '0;
  assign ex_imm_sh_sw_i = '0;
  assign ex_in1_i = '0;
  assign ex_in2_i = '0;
  assign ex_in3_i = '0;
  assign ex_dtype_i = '0;
  assign ex_mask_in1_i = 1'b0;
  assign ex_imm_mask_i = '0;
  assign ex_fromint_mask_i = 1'b0;
  assign ex_fromint_data_mask_i = '0;
  assign ex_thread_id_i = 1'b0;
  assign ex_ignore_mask_i = 1'b0;
  assign f2_thread_id_i = 1'b0;
  assign f2_wen_i = 1'b0;
  assign f2_core_kill_i = 1'b0;
  assign f2_waddr_i = '0;
  assign f2_maskop_i = 1'b0;
  assign f2_tointm_i = 1'b0;
  assign f3_thread_id_i = 1'b0;
  assign f3_regmask_from_txfma_i = 1'b0;
  assign f3_regmask_from_short_i = 1'b0;
  assign f3_core_kill_i = 1'b0;
  assign f3_regmask_wdata_lane_i = '0;
  assign f4_regmask_from_txfma_i = 1'b0;
  assign f4_core_kill_i = 1'b0;
  assign f5_thread_id_i = 1'b0;
  assign f6_thread_id_i = 1'b0;
  assign f7_thread_id_i = 1'b0;
  assign f8_thread_id_i = 1'b0;
  assign f8_regmask_comp_res_i = '0;
  assign f2_swizz_in1_i = '0;
  assign f2_swizz_in2_i = '0;
  assign f2_swizz_in3_i = '0;

  vpu_bypass u_vpu_bypass (
    .lane_id_i,
    .clk_i,
    .ex_rf_raddr_i,
    .ex_regfile_thread_id_i,
    .ex_rf_rdata_i,
    .ex_req_lane_i,
    .ex_fromint_data_i,
    .ex_tena_rf_rdata_i,
    .ex_tenb_rf_rdata_i,
    .ex_tena_regfile_bypass_en_i,
    .ex_tenb_regfile_bypass_en_i,
    .ex_bypass_force_ctrl_i,
    .f3_sh_sw_bits_i,
    .f3_regfile_waddr_i,
    .f3_regfile_wmask_i,
    .f3_thread_id_i         (f3_thread_id_bypass_i),
    .f3_bypass_clk_en_i,
    .f3_data_vector_i,
    .f4_regfile_waddr_i,
    .f4_regfile_wmask_i,
    .f4_thread_id_i         (f4_thread_id_bypass_i),
    .f4_bypass_clk_en_i,
    .f4_regfile_waddr_l_i,
    .f4_regfile_wmask_l_i,
    .f4_regfile_thread_id_l_i,
    .f4_regfile_wdata_l_i,
    .f5_regfile_waddr_i,
    .f5_regfile_wmask_i,
    .f5_thread_id_i         (f5_thread_id_bypass_i),
    .f5_bypass_clk_en_i,
    .f6_regfile_waddr_i,
    .f6_regfile_wmask_i,
    .f6_thread_id_i         (f6_thread_id_bypass_i),
    .f6_bypass_clk_en_i,
    .f7_regfile_waddr_i,
    .f7_regfile_wmask_i,
    .f7_thread_id_i         (f7_thread_id_bypass_i),
    .f7_bypass_clk_en_i,
    .f8_regfile_waddr_i,
    .f8_regfile_wmask_i,
    .f8_thread_id_i         (f8_thread_id_bypass_i),
    .f8_txfma_res_i,
    .f8_data_vector_i,
    .f8_txfma_op_i,
    .ex_req_o,
    .f3_maskop_bit_o,
    .f8_wflags_o,
    .f8_wdata_o,
    .f8_sh_sw_wdata_o
  );

  vpu_sh_sw u_vpu_sh_sw (
    .lane_id_i,
    .clk_i,
    .rst_ni,
    .ex_in_valid_i,
    .ex_cmd_i          (vpu_cmd_e'(ex_cmd_i)),
    .ex_in1_i,
    .ex_in2_i,
    .ex_in3_i,
    .ex_imm_i          (ex_imm_sh_sw_i),
    .ex_dtype_i        (vpu_dtype_e'(ex_dtype_i)),
    .ex_mask_in1_i,
    .f2_swizz_in1_i,
    .f2_swizz_in2_i,
    .f2_swizz_in3_i,
    .f2_fswizz_rdata_o,
    .f3_out_bits_o
  );

  vpu_mask u_vpu_mask (
    .clk_i,
    .rst_ni,
    .ex_in_valid_i,
    .ex_inst_valid_i,
    .ex_regfile_raddr1_i,
    .ex_regfile_raddr2_i,
    .ex_cmd_i                (vpu_cmd_e'(ex_cmd_i)),
    .ex_imm_i                (ex_imm_mask_i),
    .ex_fromint_i            (ex_fromint_mask_i),
    .ex_fromint_data_i       (ex_fromint_data_mask_i),
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
    .f8_toint_data_o         (f8_toint_data_mask_o),
    .f8_regmask_store_o,
    .wb_mask_valid_o,
    .wb_mask_dest_o
  );

  vpu_tensora_rf u_vpu_tensora_rf (
    .clk_i,
    .rd_addr_i     ('0),
    .rd_data_o     (tensora_rd_data_o),
    .wr_en_i       (1'b0),
    .wr_en_early_i (1'b0),
    .wr_addr_i     ('0),
    .wr_data_i     ('0)
  );

  vpu_tensorb_rf u_vpu_tensorb_rf (
    .clk_i,
    .rd_addr_i     ('0),
    .rd_data_o     (tensorb_rd_data_o),
    .wr_en_i       (1'b0),
    .wr_en_early_i (1'b0),
    .wr_addr_i     ('0),
    .wr_data_i     ('0)
  );

  vpu_tensorc_rf u_vpu_tensorc_rf (
    .clk_i,
    .rd_addr_i     ('0),
    .rd_data_o     (tensorc_rd_data_o),
    .wr_en_i       (1'b0),
    .wr_en_early_i (1'b0),
    .wr_addr_i     ('0),
    .wr_data_i     ('0)
  );

  vpu_tensortmp_rf u_vpu_tensortmp_rf (
    .clk_i,
    .rd_addr_i     ('0),
    .rd_data_o     (tensortmp_rd_data_o),
    .wr_en_i       (1'b0),
    .wr_en_early_i (1'b0),
    .wr_addr_i     ('0),
    .wr_data_i     ('0)
  );

  vpu_rf u_vpu_rf (
    .rd_en_i        ('0),
    .rd_thread_id_i ('0),
    .rd_addr_i      ('0),
    .rd_data_o      (vpu_rf_rd_data_o),
    .clk_i,
    .wr_en_i        ('0),
    .wr_en_early_i  ('0),
    .wr_thread_id_i ('0),
    .wr_mask_i      ('0),
    .wr_addr_i      ('0),
    .wr_data_i      ('0)
  );

  tima_adder u_tima_adder (
    .a_i   ('0),
    .b_i   ('0),
    .sum_o (tima_sum_o)
  );

  tima_top u_tima_top (
    .clk_i           (clk_i),
    .f1_mul_en_i     (1'b0),
    .f1_in1_i        ('0),
    .f1_in2_i        ('0),
    .f1_in1_signed_i (1'b0),
    .f1_in2_signed_i (1'b0),
    .f2_in3_i        ('0),
    .f2_zap_opd3_i   (1'b0),
    .f2_tima_en_i    (1'b0),
    .f2_in3_en_i     (1'b0),
    .f3_out_o        (tima_out_o)
  );

  vpu_lane_tima u_vpu_lane_tima (
    .te_i              (1'b0),
    .clk_i,
    .tima_clk_en_i     (1'b0),
    .f1_tima_mul_en_i  (1'b0),
    .tima_in1_signed_i (1'b0),
    .tima_in2_signed_i (1'b0),
    .f2_tima_en_i      (1'b0),
    .f2_tima_in3_en_i  (1'b0),
    .f2_tima_zap_opdc_i(1'b0),
    .tena_rd_data_i    ('0),
    .tenb_wr_en_i      (1'b0),
    .tenb_wr_en_early_i(1'b0),
    .tenb_rd_addr_i    ('0),
    .tenb_wr_addr_i    ('0),
    .tenb_wr_data_i    ('0),
    .tenc_wr_en_i      (1'b0),
    .tenc_wr_en_early_i(1'b0),
    .tenc_rd_addr_i    ('0),
    .tenc_wr_addr_i    ('0),
    .tenb_out_o        (lane_tima_tenb_out_o),
    .tima_out_o        (lane_tima_out_o)
  );

  assign unused_ok = ^{
    ex_req_o, f3_maskop_bit_o, f8_wflags_o, f8_wdata_o, f8_sh_sw_wdata_o,
    f2_fswizz_rdata_o, f3_out_bits_o, ex_mask_rf0_o, ex_mask_in1_o, f2_mask_rf0_o,
    f8_toint_data_mask_o, f8_regmask_store_o, wb_mask_valid_o, wb_mask_dest_o,
    tensora_rd_data_o, tensorb_rd_data_o, tensorc_rd_data_o, tensortmp_rd_data_o,
    vpu_rf_rd_data_o, tima_sum_o, tima_out_o, lane_tima_tenb_out_o, lane_tima_out_o
  };

endmodule
