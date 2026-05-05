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

module vpu_tensorfma_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [$bits(core_vpu_ctrl)-1:0] core_ctrl_bits_i,
  input  logic tensorquant_pend_i,
  input  logic tensorstore_pend_i,
  input  logic [$bits(dcache_vpu_scp_resp)-1:0] dcache_scp_resp_bits_i,
  input  logic [INST_SIZE-1:0] fma_inst_i,
  output logic enabled_o,
  output logic [$bits(vpu_dcache_ctrl)-1:0] dcache_ctrl_bits_o,
  output logic [$bits(vpu_ml_load_ctrl_t)-1:0] load_ctrl_bits_o,
  output logic load_ctrl_pre_tena_wen_o,
  output logic load_ctrl_pre_tenb_wen_o,
  output logic fma_en_next_o,
  output logic fma_en_o,
  output logic [INST_SIZE-1:0] fma_inst_next_o,
  output logic [VPU_LANE_NUM-1:0] fma_mask_next_o,
  output logic ex_tena_rdata_mask_o,
  output logic pending_vpu_rf_write_o,
  output logic pending_vpu_tenc_vrf_write_o,
  output logic f2_tentmp_wen_early_o,
  output logic f3_tentmp_wen_o,
  output logic [VPU_TENTMP_ADDR_W-1:0] f3_tentmp_waddr_o,
  output logic ex_tentmp_ren_o,
  output logic [VPU_TENTMP_ADDR_W-1:0] ex_tentmp_raddr_o,
  output logic ex_tima_valid_o,
  output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] ex_tima_mask_o,
  output logic [VPU_TENA_ADDR_W-1:0] ex_tima_tena_raddr_o,
  output logic [VPU_TENB_ADDR_W-1:0] ex_tima_tenb_raddr_o,
  output logic ex_tima_last_pass_o,
  output logic ex_tima_ren3_o,
  output logic ex_tima_sa_o,
  output logic ex_tima_sb_o,
  output logic f2_tima_ren3_o,
  output logic [VPU_TENC_ADDR_MSB:1] f2_tima_tenc_raddr_o,
  output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] f2_tima_tenc_wen_o,
  output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] f2_tima_rf_wen_o,
  output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] f3_tima_tenc_wen_o,
  output logic [VPU_TENC_ADDR_MSB:1] f3_tima_tenc_waddr_o,
  output logic [VPU_LANE_NUM-1:0][VPU_NUM_TIMA-1:0] f3_tima_rf_wen_o,
  output logic io_events_wait_tenb_o,
  output logic [24:0] vpu_tensorfma_debug_o
);

  core_vpu_ctrl core_ctrl;
  dcache_vpu_scp_resp dcache_scp_resp;
  vpu_dcache_ctrl dcache_ctrl;
  vpu_ml_load_ctrl_t load_ctrl;

  assign core_ctrl = core_vpu_ctrl'(core_ctrl_bits_i);
  assign dcache_scp_resp = dcache_vpu_scp_resp'(dcache_scp_resp_bits_i);
  assign dcache_ctrl_bits_o = dcache_ctrl;
  assign load_ctrl_bits_o = load_ctrl;

  vpu_tensorfma dut (
    .clock                       (clk_i),
    .reset                       (~rst_ni),
    .f0_core_ctrl                (core_ctrl),
    .enabled                     (enabled_o),
    .tensorquant_pend            (tensorquant_pend_i),
    .tensorstore_pend            (tensorstore_pend_i),
    .dcache_scp_resp             (dcache_scp_resp),
    .dcache_ctrl                 (dcache_ctrl),
    .load_ctrl                   (load_ctrl),
    .load_ctrl_pre_tena_wen      (load_ctrl_pre_tena_wen_o),
    .load_ctrl_pre_tenb_wen      (load_ctrl_pre_tenb_wen_o),
    .fma_en_next                 (fma_en_next_o),
    .fma_en                      (fma_en_o),
    .fma_inst_next               (fma_inst_next_o),
    .fma_mask_next               (fma_mask_next_o),
    .fma_inst                    (fma_inst_i),
    .ex_tena_rdata_mask          (ex_tena_rdata_mask_o),
    .pending_vpu_rf_write        (pending_vpu_rf_write_o),
    .pending_vpu_tenc_vrf_write  (pending_vpu_tenc_vrf_write_o),
    .f2_tentmp_wen_early         (f2_tentmp_wen_early_o),
    .f3_tentmp_wen               (f3_tentmp_wen_o),
    .f3_tentmp_waddr             (f3_tentmp_waddr_o),
    .ex_tentmp_ren               (ex_tentmp_ren_o),
    .ex_tentmp_raddr             (ex_tentmp_raddr_o),
    .ex_tima_valid               (ex_tima_valid_o),
    .ex_tima_mask                (ex_tima_mask_o),
    .ex_tima_tena_raddr          (ex_tima_tena_raddr_o),
    .ex_tima_tenb_raddr          (ex_tima_tenb_raddr_o),
    .ex_tima_last_pass           (ex_tima_last_pass_o),
    .ex_tima_ren3                (ex_tima_ren3_o),
    .ex_tima_sa                  (ex_tima_sa_o),
    .ex_tima_sb                  (ex_tima_sb_o),
    .f2_tima_ren3                (f2_tima_ren3_o),
    .f2_tima_tenc_raddr          (f2_tima_tenc_raddr_o),
    .f2_tima_tenc_wen            (f2_tima_tenc_wen_o),
    .f2_tima_rf_wen              (f2_tima_rf_wen_o),
    .f3_tima_tenc_wen            (f3_tima_tenc_wen_o),
    .f3_tima_tenc_waddr          (f3_tima_tenc_waddr_o),
    .f3_tima_rf_wen              (f3_tima_rf_wen_o),
    .io_events_wait_tenb         (io_events_wait_tenb_o),
    .vpu_tensorfma_debug         (vpu_tensorfma_debug_o)
  );

endmodule
