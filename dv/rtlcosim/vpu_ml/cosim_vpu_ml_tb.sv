// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

/* verilator lint_off IMPORTSTAR */
import dft_pkg::*;
import etlink_pkg::*;
import minion_pkg::*;
import minion_dcache_pkg::*;
import vpu_pkg::*;
import vpu_defs_pkg::*;
/* verilator lint_on IMPORTSTAR */

// ORIG_RENAME appends _orig to every vpu_ml identifier in the original RTL,
// including the original packed typedef vpu_ml_load_ctrl. Recreate that renamed
// typedef so the original child and parent module port types still elaborate.
typedef vpu_ml_load_ctrl vpu_ml_orig_load_ctrl;

module cosim_vpu_ml_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [$bits(core_vpu_ctrl)-1:0] core_ctrl_bits_i,
  input  logic [$bits(dcache_vpu_scp_resp)-1:0] dcache_scp_resp_bits_i,
  input  logic [DCACHE_DATA_SIZE-1:0] dcache_scp_data_i,
  input  logic [$bits(dcache_vpu_reduce_ctrl_t)-1:0] dcache_reduce_ctrl_bits_i,
  input  logic [INST_SIZE-1:0] id_inst_i,
  output logic orig_tfma_enabled_o,
  output logic new_tfma_enabled_o,
  output logic orig_tfma_wrrf_enabled_o,
  output logic new_tfma_wrrf_enabled_o,
  output logic orig_tquant_enabled_o,
  output logic new_tquant_enabled_o,
  output logic orig_tstore_reduce_enabled_o,
  output logic new_tstore_reduce_enabled_o,
  output logic [$bits(vpu_dcache_ctrl)-1:0] orig_dcache_ctrl_bits_o,
  output logic [$bits(vpu_dcache_ctrl)-1:0] new_dcache_ctrl_bits_o,
  output logic [$bits(vpu_ml_load_ctrl_t)-1:0] orig_reduce_load_ctrl_bits_o,
  output logic [$bits(vpu_ml_load_ctrl_t)-1:0] new_reduce_load_ctrl_bits_o,
  output logic [$bits(vpu_ml_load_ctrl_t)-1:0] orig_tensorfma_load_ctrl_bits_o,
  output logic [$bits(vpu_ml_load_ctrl_t)-1:0] new_tensorfma_load_ctrl_bits_o,
  output logic orig_load_ctrl_pre_tena_wen_o,
  output logic new_load_ctrl_pre_tena_wen_o,
  output logic orig_load_ctrl_pre_tenb_wen_o,
  output logic new_load_ctrl_pre_tenb_wen_o,
  output logic orig_id_inst_en_next_o,
  output logic new_id_inst_en_next_o,
  output logic [INST_SIZE-1:0] orig_id_inst_next_o,
  output logic [INST_SIZE-1:0] new_id_inst_next_o,
  output logic [VPU_LANE_NUM-1:0] orig_id_mask_next_o,
  output logic [VPU_LANE_NUM-1:0] new_id_mask_next_o,
  output logic orig_id_inst_en_o,
  output logic new_id_inst_en_o,
  output logic orig_id_inst_fma_en_o,
  output logic new_id_inst_fma_en_o,
  output logic orig_id_inst_tena_quant_en_o,
  output logic new_id_inst_tena_quant_en_o,
  output logic orig_f2_tentmp_wen_early_o,
  output logic new_f2_tentmp_wen_early_o,
  output logic orig_f3_tentmp_wen_o,
  output logic new_f3_tentmp_wen_o,
  output logic [VPU_TENTMP_ADDR_W-1:0] orig_f3_tentmp_waddr_o,
  output logic [VPU_TENTMP_ADDR_W-1:0] new_f3_tentmp_waddr_o,
  output logic orig_ex_tentmp_ren_o,
  output logic new_ex_tentmp_ren_o,
  output logic [VPU_TENTMP_ADDR_W-1:0] orig_ex_tentmp_raddr_o,
  output logic [VPU_TENTMP_ADDR_W-1:0] new_ex_tentmp_raddr_o,
  output logic orig_ex_tquant_ren_o,
  output logic new_ex_tquant_ren_o,
  output logic [VPU_LANE_NUM-1:0][VPU_DATA_S_SZ-1:0] orig_ex_tquant_rdata_o,
  output logic [VPU_LANE_NUM-1:0][VPU_DATA_S_SZ-1:0] new_ex_tquant_rdata_o,
  output logic orig_ex_tima_valid_o,
  output logic new_ex_tima_valid_o,
  output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] orig_ex_tima_mask_o,
  output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] new_ex_tima_mask_o,
  output logic [VPU_TENA_ADDR_W-1:0] orig_ex_tima_tena_raddr_o,
  output logic [VPU_TENA_ADDR_W-1:0] new_ex_tima_tena_raddr_o,
  output logic [VPU_TENB_ADDR_W-1:0] orig_ex_tima_tenb_raddr_o,
  output logic [VPU_TENB_ADDR_W-1:0] new_ex_tima_tenb_raddr_o,
  output logic orig_ex_tima_last_pass_o,
  output logic new_ex_tima_last_pass_o,
  output logic orig_ex_tima_ren3_o,
  output logic new_ex_tima_ren3_o,
  output logic orig_ex_tima_sa_o,
  output logic new_ex_tima_sa_o,
  output logic orig_ex_tima_sb_o,
  output logic new_ex_tima_sb_o,
  output logic orig_ex_tena_rdata_mask_o,
  output logic new_ex_tena_rdata_mask_o,
  output logic orig_f2_tima_ren3_o,
  output logic new_f2_tima_ren3_o,
  output logic [VPU_TENC_ADDR_MSB:1] orig_f2_tima_tenc_raddr_o,
  output logic [VPU_TENC_ADDR_MSB:1] new_f2_tima_tenc_raddr_o,
  output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] orig_f2_tima_tenc_wen_o,
  output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] new_f2_tima_tenc_wen_o,
  output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] orig_f2_tima_rf_wen_o,
  output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] new_f2_tima_rf_wen_o,
  output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] orig_f3_tima_tenc_wen_o,
  output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] new_f3_tima_tenc_wen_o,
  output logic [VPU_TENC_ADDR_MSB:1] orig_f3_tima_tenc_waddr_o,
  output logic [VPU_TENC_ADDR_MSB:1] new_f3_tima_tenc_waddr_o,
  output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] orig_f3_tima_rf_wen_o,
  output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] new_f3_tima_rf_wen_o,
  output logic orig_io_events_wait_tenb_o,
  output logic new_io_events_wait_tenb_o,
  output logic [10:0] orig_vpu_tensorquant_debug_o,
  output logic [10:0] new_vpu_tensorquant_debug_o,
  output logic [24:0] orig_vpu_tensorreduce_debug_o,
  output logic [24:0] new_vpu_tensorreduce_debug_o,
  output logic [24:0] orig_vpu_tensorfma_debug_o,
  output logic [24:0] new_vpu_tensorfma_debug_o
);

  minion_pkg::core_vpu_ctrl core_ctrl_new;
  core_vpu_ctrl core_ctrl_orig;
  dcache_vpu_scp_resp dcache_scp_resp_new;
  dcache_vpu_scp_resp dcache_scp_resp_orig;
  minion_pkg::dcache_vpu_reduce_ctrl_t dcache_reduce_ctrl_new;
  dcache_vpu_reduce_ctrl dcache_reduce_ctrl_orig;
  vpu_dcache_ctrl dcache_ctrl_new;
  vpu_dcache_ctrl dcache_ctrl_orig;
  vpu_ml_load_ctrl_t reduce_load_ctrl_new;
  vpu_ml_load_ctrl_t tensorfma_load_ctrl_new;
  vpu_ml_load_ctrl reduce_load_ctrl_orig;
  vpu_ml_load_ctrl tensorfma_load_ctrl_orig;

  assign core_ctrl_new = minion_pkg::core_vpu_ctrl'(core_ctrl_bits_i);
  assign core_ctrl_orig = core_vpu_ctrl'(core_ctrl_bits_i);
  assign dcache_scp_resp_new = dcache_vpu_scp_resp'(dcache_scp_resp_bits_i);
  assign dcache_scp_resp_orig = dcache_vpu_scp_resp'(dcache_scp_resp_bits_i);
  assign dcache_reduce_ctrl_new = minion_pkg::dcache_vpu_reduce_ctrl_t'(dcache_reduce_ctrl_bits_i);
  assign dcache_reduce_ctrl_orig = dcache_vpu_reduce_ctrl'(dcache_reduce_ctrl_bits_i);
  assign new_dcache_ctrl_bits_o = dcache_ctrl_new;
  assign orig_dcache_ctrl_bits_o = dcache_ctrl_orig;
  assign new_reduce_load_ctrl_bits_o = reduce_load_ctrl_new;
  assign orig_reduce_load_ctrl_bits_o = reduce_load_ctrl_orig;
  assign new_tensorfma_load_ctrl_bits_o = tensorfma_load_ctrl_new;
  assign orig_tensorfma_load_ctrl_bits_o = tensorfma_load_ctrl_orig;

  vpu_ml_orig u_orig (
    .clock                       (clk_i),
    .reset                       (~rst_ni),
    .f0_core_ctrl                (core_ctrl_orig),
    .tfma_enabled                (orig_tfma_enabled_o),
    .tfma_wrrf_enabled           (orig_tfma_wrrf_enabled_o),
    .tquant_enabled              (orig_tquant_enabled_o),
    .tstore_reduce_enabled       (orig_tstore_reduce_enabled_o),
    .dcache_scp_resp             (dcache_scp_resp_orig),
    .dcache_scp_data             (dcache_scp_data_i),
    .dcache_ctrl                 (dcache_ctrl_orig),
    .dcache_reduce_ctrl          (dcache_reduce_ctrl_orig),
    .reduce_load_ctrl            (reduce_load_ctrl_orig),
    .tensorfma_load_ctrl         (tensorfma_load_ctrl_orig),
    .load_ctrl_pre_tena_wen      (orig_load_ctrl_pre_tena_wen_o),
    .load_ctrl_pre_tenb_wen      (orig_load_ctrl_pre_tenb_wen_o),
    .id_inst_en_next             (orig_id_inst_en_next_o),
    .id_inst_next                (orig_id_inst_next_o),
    .id_mask_next                (orig_id_mask_next_o),
    .id_inst_en                  (orig_id_inst_en_o),
    .id_inst                     (id_inst_i),
    .id_inst_fma_en              (orig_id_inst_fma_en_o),
    .id_inst_tena_quant_en       (orig_id_inst_tena_quant_en_o),
    .f2_tentmp_wen_early         (orig_f2_tentmp_wen_early_o),
    .f3_tentmp_wen               (orig_f3_tentmp_wen_o),
    .f3_tentmp_waddr             (orig_f3_tentmp_waddr_o),
    .ex_tentmp_ren               (orig_ex_tentmp_ren_o),
    .ex_tentmp_raddr             (orig_ex_tentmp_raddr_o),
    .ex_tquant_ren               (orig_ex_tquant_ren_o),
    .ex_tquant_rdata             (orig_ex_tquant_rdata_o),
    .ex_tima_valid               (orig_ex_tima_valid_o),
    .ex_tima_mask                (orig_ex_tima_mask_o),
    .ex_tima_tena_raddr          (orig_ex_tima_tena_raddr_o),
    .ex_tima_tenb_raddr          (orig_ex_tima_tenb_raddr_o),
    .ex_tima_last_pass           (orig_ex_tima_last_pass_o),
    .ex_tima_ren3                (orig_ex_tima_ren3_o),
    .ex_tima_sa                  (orig_ex_tima_sa_o),
    .ex_tima_sb                  (orig_ex_tima_sb_o),
    .ex_tena_rdata_mask          (orig_ex_tena_rdata_mask_o),
    .f2_tima_ren3                (orig_f2_tima_ren3_o),
    .f2_tima_tenc_raddr          (orig_f2_tima_tenc_raddr_o),
    .f2_tima_tenc_wen            (orig_f2_tima_tenc_wen_o),
    .f2_tima_rf_wen              (orig_f2_tima_rf_wen_o),
    .f3_tima_tenc_wen            (orig_f3_tima_tenc_wen_o),
    .f3_tima_tenc_waddr          (orig_f3_tima_tenc_waddr_o),
    .f3_tima_rf_wen              (orig_f3_tima_rf_wen_o),
    .io_events_wait_tenb         (orig_io_events_wait_tenb_o),
    .vpu_tensorquant_debug       (orig_vpu_tensorquant_debug_o),
    .vpu_tensorreduce_debug      (orig_vpu_tensorreduce_debug_o),
    .vpu_tensorfma_debug         (orig_vpu_tensorfma_debug_o)
  );

  vpu_ml u_new (
    .clock                       (clk_i),
    .reset                       (~rst_ni),
    .f0_core_ctrl                (core_ctrl_new),
    .tfma_enabled                (new_tfma_enabled_o),
    .tfma_wrrf_enabled           (new_tfma_wrrf_enabled_o),
    .tquant_enabled              (new_tquant_enabled_o),
    .tstore_reduce_enabled       (new_tstore_reduce_enabled_o),
    .dcache_scp_resp             (dcache_scp_resp_new),
    .dcache_scp_data             (dcache_scp_data_i),
    .dcache_ctrl                 (dcache_ctrl_new),
    .dcache_reduce_ctrl          (dcache_reduce_ctrl_new),
    .reduce_load_ctrl            (reduce_load_ctrl_new),
    .tensorfma_load_ctrl         (tensorfma_load_ctrl_new),
    .load_ctrl_pre_tena_wen      (new_load_ctrl_pre_tena_wen_o),
    .load_ctrl_pre_tenb_wen      (new_load_ctrl_pre_tenb_wen_o),
    .id_inst_en_next             (new_id_inst_en_next_o),
    .id_inst_next                (new_id_inst_next_o),
    .id_mask_next                (new_id_mask_next_o),
    .id_inst_en                  (new_id_inst_en_o),
    .id_inst                     (id_inst_i),
    .id_inst_fma_en              (new_id_inst_fma_en_o),
    .id_inst_tena_quant_en       (new_id_inst_tena_quant_en_o),
    .f2_tentmp_wen_early         (new_f2_tentmp_wen_early_o),
    .f3_tentmp_wen               (new_f3_tentmp_wen_o),
    .f3_tentmp_waddr             (new_f3_tentmp_waddr_o),
    .ex_tentmp_ren               (new_ex_tentmp_ren_o),
    .ex_tentmp_raddr             (new_ex_tentmp_raddr_o),
    .ex_tquant_ren               (new_ex_tquant_ren_o),
    .ex_tquant_rdata             (new_ex_tquant_rdata_o),
    .ex_tima_valid               (new_ex_tima_valid_o),
    .ex_tima_mask                (new_ex_tima_mask_o),
    .ex_tima_tena_raddr          (new_ex_tima_tena_raddr_o),
    .ex_tima_tenb_raddr          (new_ex_tima_tenb_raddr_o),
    .ex_tima_last_pass           (new_ex_tima_last_pass_o),
    .ex_tima_ren3                (new_ex_tima_ren3_o),
    .ex_tima_sa                  (new_ex_tima_sa_o),
    .ex_tima_sb                  (new_ex_tima_sb_o),
    .ex_tena_rdata_mask          (new_ex_tena_rdata_mask_o),
    .f2_tima_ren3                (new_f2_tima_ren3_o),
    .f2_tima_tenc_raddr          (new_f2_tima_tenc_raddr_o),
    .f2_tima_tenc_wen            (new_f2_tima_tenc_wen_o),
    .f2_tima_rf_wen              (new_f2_tima_rf_wen_o),
    .f3_tima_tenc_wen            (new_f3_tima_tenc_wen_o),
    .f3_tima_tenc_waddr          (new_f3_tima_tenc_waddr_o),
    .f3_tima_rf_wen              (new_f3_tima_rf_wen_o),
    .io_events_wait_tenb         (new_io_events_wait_tenb_o),
    .vpu_tensorquant_debug       (new_vpu_tensorquant_debug_o),
    .vpu_tensorreduce_debug      (new_vpu_tensorreduce_debug_o),
    .vpu_tensorfma_debug         (new_vpu_tensorfma_debug_o)
  );

endmodule
