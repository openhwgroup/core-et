// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off IMPORTSTAR */
import dft_pkg::*;
import etlink_pkg::*;
import minion_pkg::*;
import minion_dcache_pkg::*;
import vpu_pkg::*;
import vpu_defs_pkg::*;
/* verilator lint_on IMPORTSTAR */

module vpu_ml_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [$bits(core_vpu_ctrl)-1:0] core_ctrl_bits_i,
  input  logic [$bits(dcache_vpu_scp_resp)-1:0] dcache_scp_resp_bits_i,
  input  logic [DCACHE_DATA_SIZE-1:0] dcache_scp_data_i,
  input  logic [$bits(dcache_vpu_reduce_ctrl_t)-1:0] dcache_reduce_ctrl_bits_i,
  input  logic [INST_SIZE-1:0] id_inst_i,
  output logic tfma_enabled_o,
  output logic tfma_wrrf_enabled_o,
  output logic tquant_enabled_o,
  output logic tstore_reduce_enabled_o,
  output logic [$bits(vpu_dcache_ctrl)-1:0] dcache_ctrl_bits_o,
  output logic [$bits(vpu_ml_load_ctrl_t)-1:0] reduce_load_ctrl_bits_o,
  output logic [$bits(vpu_ml_load_ctrl_t)-1:0] tensorfma_load_ctrl_bits_o,
  output logic load_ctrl_pre_tena_wen_o,
  output logic load_ctrl_pre_tenb_wen_o,
  output logic id_inst_en_next_o,
  output logic [INST_SIZE-1:0] id_inst_next_o,
  output logic [VPU_LANE_NUM-1:0] id_mask_next_o,
  output logic id_inst_en_o,
  output logic id_inst_fma_en_o,
  output logic id_inst_tena_quant_en_o,
  output logic f2_tentmp_wen_early_o,
  output logic f3_tentmp_wen_o,
  output logic [VPU_TENTMP_ADDR_W-1:0] f3_tentmp_waddr_o,
  output logic ex_tentmp_ren_o,
  output logic [VPU_TENTMP_ADDR_W-1:0] ex_tentmp_raddr_o,
  output logic ex_tquant_ren_o,
  output logic [VPU_LANE_NUM-1:0][VPU_DATA_S_SZ-1:0] ex_tquant_rdata_o,
  output logic ex_tima_valid_o,
  output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] ex_tima_mask_o,
  output logic [VPU_TENA_ADDR_W-1:0] ex_tima_tena_raddr_o,
  output logic [VPU_TENB_ADDR_W-1:0] ex_tima_tenb_raddr_o,
  output logic ex_tima_last_pass_o,
  output logic ex_tima_ren3_o,
  output logic ex_tima_sa_o,
  output logic ex_tima_sb_o,
  output logic ex_tena_rdata_mask_o,
  output logic f2_tima_ren3_o,
  output logic [VPU_TENC_ADDR_MSB:1] f2_tima_tenc_raddr_o,
  output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] f2_tima_tenc_wen_o,
  output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] f2_tima_rf_wen_o,
  output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] f3_tima_tenc_wen_o,
  output logic [VPU_TENC_ADDR_MSB:1] f3_tima_tenc_waddr_o,
  output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] f3_tima_rf_wen_o,
  output logic io_events_wait_tenb_o,
  output logic [10:0] vpu_tensorquant_debug_o,
  output logic [24:0] vpu_tensorreduce_debug_o,
  output logic [24:0] vpu_tensorfma_debug_o
);

  core_vpu_ctrl core_ctrl;
  dcache_vpu_scp_resp dcache_scp_resp;
  dcache_vpu_reduce_ctrl_t dcache_reduce_ctrl;
  vpu_dcache_ctrl dcache_ctrl;
  vpu_ml_load_ctrl_t reduce_load_ctrl;
  vpu_ml_load_ctrl_t tensorfma_load_ctrl;

  assign core_ctrl = core_vpu_ctrl'(core_ctrl_bits_i);
  assign dcache_scp_resp = dcache_vpu_scp_resp'(dcache_scp_resp_bits_i);
  assign dcache_reduce_ctrl = dcache_vpu_reduce_ctrl_t'(dcache_reduce_ctrl_bits_i);
  assign dcache_ctrl_bits_o = dcache_ctrl;
  assign reduce_load_ctrl_bits_o = reduce_load_ctrl;
  assign tensorfma_load_ctrl_bits_o = tensorfma_load_ctrl;

  vpu_ml dut (
    .clock                       (clk_i),
    .reset                       (~rst_ni),
    .f0_core_ctrl                (core_ctrl),
    .tfma_enabled                (tfma_enabled_o),
    .tfma_wrrf_enabled           (tfma_wrrf_enabled_o),
    .tquant_enabled              (tquant_enabled_o),
    .tstore_reduce_enabled       (tstore_reduce_enabled_o),
    .dcache_scp_resp             (dcache_scp_resp),
    .dcache_scp_data             (dcache_scp_data_i),
    .dcache_ctrl                 (dcache_ctrl),
    .dcache_reduce_ctrl          (dcache_reduce_ctrl),
    .reduce_load_ctrl            (reduce_load_ctrl),
    .tensorfma_load_ctrl         (tensorfma_load_ctrl),
    .load_ctrl_pre_tena_wen      (load_ctrl_pre_tena_wen_o),
    .load_ctrl_pre_tenb_wen      (load_ctrl_pre_tenb_wen_o),
    .id_inst_en_next             (id_inst_en_next_o),
    .id_inst_next                (id_inst_next_o),
    .id_mask_next                (id_mask_next_o),
    .id_inst_en                  (id_inst_en_o),
    .id_inst                     (id_inst_i),
    .id_inst_fma_en              (id_inst_fma_en_o),
    .id_inst_tena_quant_en       (id_inst_tena_quant_en_o),
    .f2_tentmp_wen_early         (f2_tentmp_wen_early_o),
    .f3_tentmp_wen               (f3_tentmp_wen_o),
    .f3_tentmp_waddr             (f3_tentmp_waddr_o),
    .ex_tentmp_ren               (ex_tentmp_ren_o),
    .ex_tentmp_raddr             (ex_tentmp_raddr_o),
    .ex_tquant_ren               (ex_tquant_ren_o),
    .ex_tquant_rdata             (ex_tquant_rdata_o),
    .ex_tima_valid               (ex_tima_valid_o),
    .ex_tima_mask                (ex_tima_mask_o),
    .ex_tima_tena_raddr          (ex_tima_tena_raddr_o),
    .ex_tima_tenb_raddr          (ex_tima_tenb_raddr_o),
    .ex_tima_last_pass           (ex_tima_last_pass_o),
    .ex_tima_ren3                (ex_tima_ren3_o),
    .ex_tima_sa                  (ex_tima_sa_o),
    .ex_tima_sb                  (ex_tima_sb_o),
    .ex_tena_rdata_mask          (ex_tena_rdata_mask_o),
    .f2_tima_ren3                (f2_tima_ren3_o),
    .f2_tima_tenc_raddr          (f2_tima_tenc_raddr_o),
    .f2_tima_tenc_wen            (f2_tima_tenc_wen_o),
    .f2_tima_rf_wen              (f2_tima_rf_wen_o),
    .f3_tima_tenc_wen            (f3_tima_tenc_wen_o),
    .f3_tima_tenc_waddr          (f3_tima_tenc_waddr_o),
    .f3_tima_rf_wen              (f3_tima_rf_wen_o),
    .io_events_wait_tenb         (io_events_wait_tenb_o),
    .vpu_tensorquant_debug       (vpu_tensorquant_debug_o),
    .vpu_tensorreduce_debug      (vpu_tensorreduce_debug_o),
    .vpu_tensorfma_debug         (vpu_tensorfma_debug_o)
  );

endmodule
