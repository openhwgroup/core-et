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

module cosim_vpu_tensorfma_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [$bits(core_vpu_ctrl)-1:0] core_ctrl_bits_i,
  input  logic tensorquant_pend_i,
  input  logic tensorstore_pend_i,
  input  logic [$bits(dcache_vpu_scp_resp)-1:0] dcache_scp_resp_bits_i,
  input  logic [INST_SIZE-1:0] fma_inst_i,
  output logic orig_enabled_o,
  output logic new_enabled_o,
  output logic [$bits(vpu_dcache_ctrl)-1:0] orig_dcache_ctrl_bits_o,
  output logic [$bits(vpu_dcache_ctrl)-1:0] new_dcache_ctrl_bits_o,
  output logic [$bits(vpu_ml_load_ctrl_t)-1:0] orig_load_ctrl_bits_o,
  output logic [$bits(vpu_ml_load_ctrl_t)-1:0] new_load_ctrl_bits_o,
  output logic orig_load_ctrl_pre_tena_wen_o,
  output logic new_load_ctrl_pre_tena_wen_o,
  output logic orig_load_ctrl_pre_tenb_wen_o,
  output logic new_load_ctrl_pre_tenb_wen_o,
  output logic orig_fma_en_next_o,
  output logic new_fma_en_next_o,
  output logic orig_fma_en_o,
  output logic new_fma_en_o,
  output logic [INST_SIZE-1:0] orig_fma_inst_next_o,
  output logic [INST_SIZE-1:0] new_fma_inst_next_o,
  output logic [VPU_LANE_NUM-1:0] orig_fma_mask_next_o,
  output logic [VPU_LANE_NUM-1:0] new_fma_mask_next_o,
  output logic orig_ex_tena_rdata_mask_o,
  output logic new_ex_tena_rdata_mask_o,
  output logic orig_pending_vpu_rf_write_o,
  output logic new_pending_vpu_rf_write_o,
  output logic orig_pending_vpu_tenc_vrf_write_o,
  output logic new_pending_vpu_tenc_vrf_write_o,
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
  output logic [24:0] orig_vpu_tensorfma_debug_o,
  output logic [24:0] new_vpu_tensorfma_debug_o
);

  minion_pkg::core_vpu_ctrl core_ctrl_new;
  core_vpu_ctrl core_ctrl_orig;
  dcache_vpu_scp_resp dcache_scp_resp_new;
  dcache_vpu_scp_resp dcache_scp_resp_orig;
  vpu_dcache_ctrl dcache_ctrl_new;
  vpu_dcache_ctrl dcache_ctrl_orig;
  vpu_ml_load_ctrl_t load_ctrl_new;
  vpu_ml_load_ctrl load_ctrl_orig;

  assign core_ctrl_new = minion_pkg::core_vpu_ctrl'(core_ctrl_bits_i);
  assign core_ctrl_orig = core_vpu_ctrl'(core_ctrl_bits_i);
  assign dcache_scp_resp_new = dcache_vpu_scp_resp'(dcache_scp_resp_bits_i);
  assign dcache_scp_resp_orig = dcache_vpu_scp_resp'(dcache_scp_resp_bits_i);
  assign new_dcache_ctrl_bits_o = dcache_ctrl_new;
  assign orig_dcache_ctrl_bits_o = dcache_ctrl_orig;
  assign new_load_ctrl_bits_o = load_ctrl_new;
  assign orig_load_ctrl_bits_o = load_ctrl_orig;

  vpu_tensorfma_orig u_orig (
    .clock                       (clk_i),
    .reset                       (~rst_ni),
    .f0_core_ctrl                (core_ctrl_orig),
    .enabled                     (orig_enabled_o),
    .tensorquant_pend            (tensorquant_pend_i),
    .tensorstore_pend            (tensorstore_pend_i),
    .dcache_scp_resp             (dcache_scp_resp_orig),
    .dcache_ctrl                 (dcache_ctrl_orig),
    .load_ctrl                   (load_ctrl_orig),
    .load_ctrl_pre_tena_wen      (orig_load_ctrl_pre_tena_wen_o),
    .load_ctrl_pre_tenb_wen      (orig_load_ctrl_pre_tenb_wen_o),
    .fma_en_next                 (orig_fma_en_next_o),
    .fma_en                      (orig_fma_en_o),
    .fma_inst_next               (orig_fma_inst_next_o),
    .fma_mask_next               (orig_fma_mask_next_o),
    .fma_inst                    (fma_inst_i),
    .ex_tena_rdata_mask          (orig_ex_tena_rdata_mask_o),
    .pending_vpu_rf_write        (orig_pending_vpu_rf_write_o),
    .pending_vpu_tenc_vrf_write  (orig_pending_vpu_tenc_vrf_write_o),
    .f2_tentmp_wen_early         (orig_f2_tentmp_wen_early_o),
    .f3_tentmp_wen               (orig_f3_tentmp_wen_o),
    .f3_tentmp_waddr             (orig_f3_tentmp_waddr_o),
    .ex_tentmp_ren               (orig_ex_tentmp_ren_o),
    .ex_tentmp_raddr             (orig_ex_tentmp_raddr_o),
    .ex_tima_valid               (orig_ex_tima_valid_o),
    .ex_tima_mask                (orig_ex_tima_mask_o),
    .ex_tima_tena_raddr          (orig_ex_tima_tena_raddr_o),
    .ex_tima_tenb_raddr          (orig_ex_tima_tenb_raddr_o),
    .ex_tima_last_pass           (orig_ex_tima_last_pass_o),
    .ex_tima_ren3                (orig_ex_tima_ren3_o),
    .ex_tima_sa                  (orig_ex_tima_sa_o),
    .ex_tima_sb                  (orig_ex_tima_sb_o),
    .f2_tima_ren3                (orig_f2_tima_ren3_o),
    .f2_tima_tenc_raddr          (orig_f2_tima_tenc_raddr_o),
    .f2_tima_tenc_wen            (orig_f2_tima_tenc_wen_o),
    .f2_tima_rf_wen              (orig_f2_tima_rf_wen_o),
    .f3_tima_tenc_wen            (orig_f3_tima_tenc_wen_o),
    .f3_tima_tenc_waddr          (orig_f3_tima_tenc_waddr_o),
    .f3_tima_rf_wen              (orig_f3_tima_rf_wen_o),
    .io_events_wait_tenb         (orig_io_events_wait_tenb_o),
    .vpu_tensorfma_debug         (orig_vpu_tensorfma_debug_o)
  );

  vpu_tensorfma u_new (
    .clock                       (clk_i),
    .reset                       (~rst_ni),
    .f0_core_ctrl                (core_ctrl_new),
    .enabled                     (new_enabled_o),
    .tensorquant_pend            (tensorquant_pend_i),
    .tensorstore_pend            (tensorstore_pend_i),
    .dcache_scp_resp             (dcache_scp_resp_new),
    .dcache_ctrl                 (dcache_ctrl_new),
    .load_ctrl                   (load_ctrl_new),
    .load_ctrl_pre_tena_wen      (new_load_ctrl_pre_tena_wen_o),
    .load_ctrl_pre_tenb_wen      (new_load_ctrl_pre_tenb_wen_o),
    .fma_en_next                 (new_fma_en_next_o),
    .fma_en                      (new_fma_en_o),
    .fma_inst_next               (new_fma_inst_next_o),
    .fma_mask_next               (new_fma_mask_next_o),
    .fma_inst                    (fma_inst_i),
    .ex_tena_rdata_mask          (new_ex_tena_rdata_mask_o),
    .pending_vpu_rf_write        (new_pending_vpu_rf_write_o),
    .pending_vpu_tenc_vrf_write  (new_pending_vpu_tenc_vrf_write_o),
    .f2_tentmp_wen_early         (new_f2_tentmp_wen_early_o),
    .f3_tentmp_wen               (new_f3_tentmp_wen_o),
    .f3_tentmp_waddr             (new_f3_tentmp_waddr_o),
    .ex_tentmp_ren               (new_ex_tentmp_ren_o),
    .ex_tentmp_raddr             (new_ex_tentmp_raddr_o),
    .ex_tima_valid               (new_ex_tima_valid_o),
    .ex_tima_mask                (new_ex_tima_mask_o),
    .ex_tima_tena_raddr          (new_ex_tima_tena_raddr_o),
    .ex_tima_tenb_raddr          (new_ex_tima_tenb_raddr_o),
    .ex_tima_last_pass           (new_ex_tima_last_pass_o),
    .ex_tima_ren3                (new_ex_tima_ren3_o),
    .ex_tima_sa                  (new_ex_tima_sa_o),
    .ex_tima_sb                  (new_ex_tima_sb_o),
    .f2_tima_ren3                (new_f2_tima_ren3_o),
    .f2_tima_tenc_raddr          (new_f2_tima_tenc_raddr_o),
    .f2_tima_tenc_wen            (new_f2_tima_tenc_wen_o),
    .f2_tima_rf_wen              (new_f2_tima_rf_wen_o),
    .f3_tima_tenc_wen            (new_f3_tima_tenc_wen_o),
    .f3_tima_tenc_waddr          (new_f3_tima_tenc_waddr_o),
    .f3_tima_rf_wen              (new_f3_tima_rf_wen_o),
    .io_events_wait_tenb         (new_io_events_wait_tenb_o),
    .vpu_tensorfma_debug         (new_vpu_tensorfma_debug_o)
  );

endmodule
