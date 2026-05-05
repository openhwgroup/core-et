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

module vpu_tensorreduce_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic reduce_start_i,
  input  logic tensorstore_start_i,
  input  logic tensorfma_start_i,
  input  logic tensorquant_start_i,
  input  logic [$bits(reduce_tensorstore_control_t)-1:0] reduce_ctrl_bits_i,
  input  logic scoreboard_pend_i,
  input  logic tensorfma_pend_i,
  input  logic tensorquant_pend_i,
  input  logic [$bits(dcache_vpu_reduce_ctrl_t)-1:0] dcache_reduce_ctrl_bits_i,
  output logic enabled_o,
  output logic reduce_wait_o,
  output logic [$bits(vpu_ml_load_ctrl_t)-1:0] load_ctrl_bits_o,
  output logic reduce_inst_en_next_o,
  output logic [INST_SIZE-1:0] reduce_inst_next_o,
  output logic [24:0] vpu_tensorreduce_debug_o
);

  reduce_tensorstore_control_t reduce_ctrl;
  dcache_vpu_reduce_ctrl_t dcache_reduce_ctrl;
  vpu_ml_load_ctrl_t load_ctrl;

  assign reduce_ctrl = reduce_tensorstore_control_t'(reduce_ctrl_bits_i);
  assign dcache_reduce_ctrl = dcache_vpu_reduce_ctrl_t'(dcache_reduce_ctrl_bits_i);
  assign load_ctrl_bits_o = load_ctrl;

  vpu_tensorreduce dut (
    .clock                  (clk_i),
    .reset                  (~rst_ni),
    .reduce_start           (reduce_start_i),
    .tensorstore_start      (tensorstore_start_i),
    .tensorfma_start        (tensorfma_start_i),
    .tensorquant_start      (tensorquant_start_i),
    .reduce_ctrl            (reduce_ctrl),
    .enabled                (enabled_o),
    .scoreboard_pend        (scoreboard_pend_i),
    .tensorfma_pend         (tensorfma_pend_i),
    .tensorquant_pend       (tensorquant_pend_i),
    .reduce_wait            (reduce_wait_o),
    .dcache_reduce_ctrl     (dcache_reduce_ctrl),
    .load_ctrl              (load_ctrl),
    .reduce_inst_en_next    (reduce_inst_en_next_o),
    .reduce_inst_next       (reduce_inst_next_o),
    .vpu_tensorreduce_debug (vpu_tensorreduce_debug_o)
  );

endmodule
