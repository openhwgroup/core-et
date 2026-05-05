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

module cosim_vpu_tensorquant_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [$bits(core_vpu_ctrl)-1:0] core_ctrl_bits_i,
  input  logic tensorfma_pend_i,
  input  logic tensorstore_pend_i,
  input  logic dcache_scp_available_i,
  input  logic [DCACHE_DATA_SIZE-1:0] dcache_scp_data_i,
  output logic orig_enabled_o,
  output logic new_enabled_o,
  output logic [$bits(vpu_dcache_scp_req)-1:0] orig_dcache_scp_req_bits_o,
  output logic [$bits(vpu_dcache_scp_req)-1:0] new_dcache_scp_req_bits_o,
  output logic orig_quant_en_next_o,
  output logic new_quant_en_next_o,
  output logic [INST_SIZE-1:0] orig_quant_inst_next_o,
  output logic [INST_SIZE-1:0] new_quant_inst_next_o,
  output logic [VPU_LANE_NUM-1:0] orig_quant_mask_next_o,
  output logic [VPU_LANE_NUM-1:0] new_quant_mask_next_o,
  output logic orig_quant_tena_en_o,
  output logic new_quant_tena_en_o,
  output logic orig_ex_ren_o,
  output logic new_ex_ren_o,
  output logic [VPU_LANE_NUM-1:0][VPU_DATA_S_SZ-1:0] orig_ex_rdata_o,
  output logic [VPU_LANE_NUM-1:0][VPU_DATA_S_SZ-1:0] new_ex_rdata_o,
  output logic [10:0] orig_vpu_tensorquant_debug_o,
  output logic [10:0] new_vpu_tensorquant_debug_o
);

  minion_pkg::core_vpu_ctrl core_ctrl_new;
  core_vpu_ctrl core_ctrl_orig;
  vpu_dcache_scp_req dcache_scp_req_new;
  vpu_dcache_scp_req dcache_scp_req_orig;

  assign core_ctrl_new = minion_pkg::core_vpu_ctrl'(core_ctrl_bits_i);
  assign core_ctrl_orig = core_vpu_ctrl'(core_ctrl_bits_i);
  assign new_dcache_scp_req_bits_o = dcache_scp_req_new;
  assign orig_dcache_scp_req_bits_o = dcache_scp_req_orig;

  vpu_tensorquant_orig u_orig (
    .clock                    (clk_i),
    .reset                    (~rst_ni),
    .f0_core_ctrl             (core_ctrl_orig),
    .enabled                  (orig_enabled_o),
    .tensorfma_pend           (tensorfma_pend_i),
    .tensorstore_pend         (tensorstore_pend_i),
    .dcache_scp_available     (dcache_scp_available_i),
    .dcache_scp_req           (dcache_scp_req_orig),
    .dcache_scp_data          (dcache_scp_data_i),
    .quant_en_next            (orig_quant_en_next_o),
    .quant_inst_next          (orig_quant_inst_next_o),
    .quant_mask_next          (orig_quant_mask_next_o),
    .quant_tena_en            (orig_quant_tena_en_o),
    .ex_ren                   (orig_ex_ren_o),
    .ex_rdata                 (orig_ex_rdata_o),
    .vpu_tensorquant_orig_debug (orig_vpu_tensorquant_debug_o)
  );

  vpu_tensorquant u_new (
    .clock                    (clk_i),
    .reset                    (~rst_ni),
    .f0_core_ctrl             (core_ctrl_new),
    .enabled                  (new_enabled_o),
    .tensorfma_pend           (tensorfma_pend_i),
    .tensorstore_pend         (tensorstore_pend_i),
    .dcache_scp_available     (dcache_scp_available_i),
    .dcache_scp_req           (dcache_scp_req_new),
    .dcache_scp_data          (dcache_scp_data_i),
    .quant_en_next            (new_quant_en_next_o),
    .quant_inst_next          (new_quant_inst_next_o),
    .quant_mask_next          (new_quant_mask_next_o),
    .quant_tena_en            (new_quant_tena_en_o),
    .ex_ren                   (new_ex_ren_o),
    .ex_rdata                 (new_ex_rdata_o),
    .vpu_tensorquant_debug    (new_vpu_tensorquant_debug_o)
  );

endmodule
