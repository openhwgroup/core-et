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

module cosim_vpu_tensorreduce_tb (
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
  output logic orig_enabled_o,
  output logic new_enabled_o,
  output logic orig_reduce_wait_o,
  output logic new_reduce_wait_o,
  output logic [$bits(vpu_ml_load_ctrl_t)-1:0] orig_load_ctrl_bits_o,
  output logic [$bits(vpu_ml_load_ctrl_t)-1:0] new_load_ctrl_bits_o,
  output logic orig_reduce_inst_en_next_o,
  output logic new_reduce_inst_en_next_o,
  output logic [INST_SIZE-1:0] orig_reduce_inst_next_o,
  output logic [INST_SIZE-1:0] new_reduce_inst_next_o,
  output logic [24:0] orig_vpu_tensorreduce_debug_o,
  output logic [24:0] new_vpu_tensorreduce_debug_o
);

  reduce_tensorstore_control_t reduce_ctrl_new;
  reduce_tensorstore_control reduce_ctrl_orig;
  dcache_vpu_reduce_ctrl_t dcache_reduce_ctrl_new;
  dcache_vpu_reduce_ctrl dcache_reduce_ctrl_orig;
  vpu_ml_load_ctrl_t load_ctrl_new;
  vpu_ml_load_ctrl load_ctrl_orig;

  assign reduce_ctrl_new = reduce_tensorstore_control_t'(reduce_ctrl_bits_i);
  assign reduce_ctrl_orig = reduce_tensorstore_control'(reduce_ctrl_bits_i);
  assign dcache_reduce_ctrl_new = dcache_vpu_reduce_ctrl_t'(dcache_reduce_ctrl_bits_i);
  assign dcache_reduce_ctrl_orig = dcache_vpu_reduce_ctrl'(dcache_reduce_ctrl_bits_i);
  assign orig_load_ctrl_bits_o = load_ctrl_orig;
  assign new_load_ctrl_bits_o = load_ctrl_new;

  vpu_tensorreduce_orig u_orig (
    .clock                  (clk_i),
    .reset                  (~rst_ni),
    .reduce_start           (reduce_start_i),
    .tensorstore_start      (tensorstore_start_i),
    .tensorfma_start        (tensorfma_start_i),
    .tensorquant_start      (tensorquant_start_i),
    .reduce_ctrl            (reduce_ctrl_orig),
    .enabled                (orig_enabled_o),
    .scoreboard_pend        (scoreboard_pend_i),
    .tensorfma_pend         (tensorfma_pend_i),
    .tensorquant_pend       (tensorquant_pend_i),
    .reduce_wait            (orig_reduce_wait_o),
    .dcache_reduce_ctrl     (dcache_reduce_ctrl_orig),
    .load_ctrl              (load_ctrl_orig),
    .reduce_inst_en_next    (orig_reduce_inst_en_next_o),
    .reduce_inst_next       (orig_reduce_inst_next_o),
    .vpu_tensorreduce_orig_debug (orig_vpu_tensorreduce_debug_o)
  );

  vpu_tensorreduce u_new (
    .clock                  (clk_i),
    .reset                  (~rst_ni),
    .reduce_start           (reduce_start_i),
    .tensorstore_start      (tensorstore_start_i),
    .tensorfma_start        (tensorfma_start_i),
    .tensorquant_start      (tensorquant_start_i),
    .reduce_ctrl            (reduce_ctrl_new),
    .enabled                (new_enabled_o),
    .scoreboard_pend        (scoreboard_pend_i),
    .tensorfma_pend         (tensorfma_pend_i),
    .tensorquant_pend       (tensorquant_pend_i),
    .reduce_wait            (new_reduce_wait_o),
    .dcache_reduce_ctrl     (dcache_reduce_ctrl_new),
    .load_ctrl              (load_ctrl_new),
    .reduce_inst_en_next    (new_reduce_inst_en_next_o),
    .reduce_inst_next       (new_reduce_inst_next_o),
    .vpu_tensorreduce_debug (new_vpu_tensorreduce_debug_o)
  );

endmodule
