// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// null_vpu -- integer-only bring-up helper for minion.
//
// This is intentionally not a translation of the original CORE-ET VPU.
// It provides a fail-closed minion-side VPU boundary for scalar-only bring-up:
// every returned control/debug/perf interface stays idle, while the EX-stage
// control bundle reports illegal_rm so any FP/VPU opcode that reaches EX traps
// as an illegal instruction.

module null_vpu
  import minion_pkg::*;
(
  input  logic                               clk_i,
  input  logic                               rst_ni,
  input  minion_vpu_id_req                   id_core_req_i,
  input  vpu_ctrl_sigs_t                     id_vpu_decoder_sigs_i,
  input  minion_vpu_ex_req                   ex_core_req_i,
  input  logic                               f2_core_kill_i,
  input  logic                               f3_core_kill_i,
  input  logic                               f4_core_kill_i,
  input  core_vpu_ctrl                       f0_core_ctrl_i,
  input  logic                               chicken_bit_vpulane_i,
  input  logic                               chicken_bit_vputima_i,
  input  logic                               chicken_bit_vputrans_i,
  input  logic [MinionMemCtrlBits-1:0]       mem_ctrl_override_i,
  input  logic                               wb_dcache_resp_valid_i,
  input  dcache_vpu_resp                     wb_dcache_resp_i,
  input  dcache_vpu_scp_resp                 dcache_scp_resp_i,
  input  logic [VpuDataSz-1:0]               dcache_scp_data_i,
  input  logic [VpuDataSz-1:0]               dcache_tenb_data_i,
  input  dcache_vpu_reduce_ctrl              dcache_reduce_ctrl_i,
  output vpu_id_ctrl_t                       id_core_ctrl_o,
  output vpu_minion_ex_ctrl                  ex_core_ctrl_o,
  output vpu_minion_tag_ctrl                 f2_core_ctrl_o,
  output vpu_minion_mem_ctrl                 f3_core_ctrl_o,
  output vpu_minion_wb_ctrl                  wb_core_ctrl_o,
  output logic [CsrNrEventsVpu-1:0]          io_events_o,
  output logic [NeighDebugMatchWidth-1:0]    vpu_dbg_match_o,
  output logic [NeighDebugFilterWidth-1:0]   vpu_dbg_filter_o,
  output logic [4:0][NeighDebugDataWidth-1:0] vpu_dbg_data_o,
  output vpu_dcache_ctrl                     dcache_ctrl_o
);

  logic unused_inputs;

  always_comb begin
    // Consume the full request-side boundary so Verilator sees this helper as
    // intentionally ignoring the VPU traffic while still synthesizing a
    // fail-closed constant illegal_rm response.
    unused_inputs = &{
      1'b0,
      clk_i,
      rst_ni,
      id_core_req_i,
      id_vpu_decoder_sigs_i,
      ex_core_req_i,
      f2_core_kill_i,
      f3_core_kill_i,
      f4_core_kill_i,
      f0_core_ctrl_i,
      chicken_bit_vpulane_i,
      chicken_bit_vputima_i,
      chicken_bit_vputrans_i,
      mem_ctrl_override_i,
      wb_dcache_resp_valid_i,
      wb_dcache_resp_i,
      dcache_scp_resp_i,
      dcache_scp_data_i,
      dcache_tenb_data_i,
      dcache_reduce_ctrl_i
    };

    id_core_ctrl_o = '0;

    ex_core_ctrl_o = '0;
    ex_core_ctrl_o.illegal_rm = 1'b1 || unused_inputs;

    f2_core_ctrl_o = '0;
    f3_core_ctrl_o = '0;
    wb_core_ctrl_o = '0;
    io_events_o = '0;
    vpu_dbg_match_o = '0;
    vpu_dbg_filter_o = '0;
    vpu_dbg_data_o = '0;
    dcache_ctrl_o = '0;
  end

endmodule
