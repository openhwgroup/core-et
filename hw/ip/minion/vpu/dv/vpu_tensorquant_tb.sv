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

module vpu_tensorquant_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [$bits(core_vpu_ctrl)-1:0] core_ctrl_bits_i,
  input  logic tensorfma_pend_i,
  input  logic tensorstore_pend_i,
  input  logic dcache_scp_available_i,
  input  logic [DCACHE_DATA_SIZE-1:0] dcache_scp_data_i,
  output logic enabled_o,
  output logic [$bits(vpu_dcache_scp_req)-1:0] dcache_scp_req_bits_o,
  output logic quant_en_next_o,
  output logic [INST_SIZE-1:0] quant_inst_next_o,
  output logic [VPU_LANE_NUM-1:0] quant_mask_next_o,
  output logic quant_tena_en_o,
  output logic ex_ren_o,
  output logic [VPU_LANE_NUM-1:0][VPU_DATA_S_SZ-1:0] ex_rdata_o,
  output logic [10:0] vpu_tensorquant_debug_o
);

  core_vpu_ctrl core_ctrl;
  vpu_dcache_scp_req dcache_scp_req;

  assign core_ctrl = core_vpu_ctrl'(core_ctrl_bits_i);
  assign dcache_scp_req_bits_o = dcache_scp_req;

  vpu_tensorquant dut (
    .clock                    (clk_i),
    .reset                    (~rst_ni),
    .f0_core_ctrl             (core_ctrl),
    .enabled                  (enabled_o),
    .tensorfma_pend           (tensorfma_pend_i),
    .tensorstore_pend         (tensorstore_pend_i),
    .dcache_scp_available     (dcache_scp_available_i),
    .dcache_scp_req           (dcache_scp_req),
    .dcache_scp_data          (dcache_scp_data_i),
    .quant_en_next            (quant_en_next_o),
    .quant_inst_next          (quant_inst_next_o),
    .quant_mask_next          (quant_mask_next_o),
    .quant_tena_en            (quant_tena_en_o),
    .ex_ren                   (ex_ren_o),
    .ex_rdata                 (ex_rdata_o),
    .vpu_tensorquant_debug    (vpu_tensorquant_debug_o)
  );

endmodule
