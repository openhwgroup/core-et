// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit-test wrapper for minion_dcache_reduce.

module minion_dcache_reduce_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                       clk_i,
  input  logic                       rst_ni,
  input  logic [MaxMinionIdBits-1:0] global_min_id_i,
  input  logic                       chicken_bit_dcache_i,
  output logic                       reduce_ready_o,
  input  logic                       reduce_start_i,
  output logic                       tensorstore_busy_o,
  input  logic                       tensorstore_start_i,
  input  logic [VaSize-1:0]          tensorstore_stride_i,
  input  logic [63:0]                ctrl_bits_i,
  input  logic                       thread_id_i,
  input  logic                       re_l2_req_ready_i,
  input  logic                       ts_l2_req_ready_i,
  output logic                       re_l2_req_valid_o,
  output logic                       ts_l2_req_valid_o,
  output logic                       ba_alloc_o,
  output logic                       s1_inst_send_val_o,
  output logic                       s1_inst_op_val_o,
  input  logic                       s0_available_i,
  input  logic                       s1_available_i,
  input  logic                       s1_tlb_wait_i,
  input  logic                       s1_tlb_fail_i,
  input  logic [PaSize-1:0]          s2_addr_i,
  input  logic                       ba_full_i,
  input  logic                       s2_valid_i,
  input  logic                       s1_valid_i,
  input  logic                       ba_read_used_i,
  input  logic                       scp_ctrl_on_i,
  input  logic                       vpu_reduce_wait_i,
  output logic                       vpu_send_reg_o,
  output logic                       vpu_exec_op_o,
  output logic                       vpu_nothing_o,
  output logic                       ts_start_o,
  output logic                       ts_op_o,
  input  logic                       mh_going_to_uc_store_i,
  output logic                       err_mem_fault_t0_o,
  output logic                       err_mem_fault_t1_o,
  output logic                       bus_err_t0_o,
  output logic                       bus_err_t1_o
);

  /* verilator lint_off UNUSEDSIGNAL */  // The wrapper exposes only the subset of DUT state that this directed test checks.
  reduce_tensorstore_control_t reduce_tensorstore_ctrl;
  vm_status_t [NrThreads-1:0]  vm_status;
  et_link_minion_rsp_info_t    l2_resp;
  dcache_vpu_reduce_ctrl_t     vpu_ctrl;
  dcache_meta_data_t           unused_meta_read_resp [DcacheWays];
  et_link_minion_evict_req_info_t unused_re_l2_req;
  et_link_minion_evict_req_info_t unused_ts_l2_req;
  vm_status_t                  vm_status_ts;
  logic                        unused_buffer_avail;
  logic                        re_ts_l2_req_inflight;
  logic                        pipe_used;
  logic                        s2_inst_send_val;
  logic                        s3_inst_send_val;
  logic                        s2_inst_op_val;
  logic                        s3_inst_op_val;
  logic                        ts_scp_read;
  logic [DcacheWayIdxWidth-1:0] unused_ts_scp_read_way;
  logic [DcacheLramAddrWidth-1:0] unused_ts_scp_read_addr;
  logic                        ts_col_size_is_128b;
  logic                        s0_addr_load;
  logic [VaSize-1:0]           unused_s0_addr;
  logic                        unused_s0_addr_msb_err;
  logic                        unused_s0_thread_id;
  logic                        s1_addr_load;
  logic [DcacheBufferIdxWidth-1:0] unused_s3_ba_write_addr;
  logic                        wait_l2_to_ba_resp;
  logic                        ba_entries_required;
  logic                        ba_read_en;
  logic                        ba_read_en_spec;
  logic [DcacheBufferIdxWidth-1:0] unused_ba_read_addr;
  logic [DcacheErrFlagRange:0][NrThreads-1:0] err_flags;
  logic [DcacheDbgCsrTsSize-1:0] unused_csr_debug_signals;
  logic [DcacheDbgSmMatchBits-1:0] unused_sm_match_debug_signals;
  logic [DcacheDbgSmFilterBits-1:0] unused_sm_filter_debug_signals;
  logic [DcacheDbgSmDataBits-1:0] unused_sm_data_debug_signals;
  /* verilator lint_on UNUSEDSIGNAL */

  always_comb begin
    reduce_tensorstore_ctrl = reduce_tensorstore_control_t'(ctrl_bits_i);
    vm_status = '{default: '0};
    l2_resp = '0;

    vpu_send_reg_o = vpu_ctrl.send_reg;
    vpu_exec_op_o = vpu_ctrl.exec_op;
    vpu_nothing_o = vpu_ctrl.nothing;

    err_mem_fault_t0_o = err_flags[DcacheErrFlagMemFault][0];
    err_mem_fault_t1_o = err_flags[DcacheErrFlagMemFault][1];
    bus_err_t0_o = bus_err_t0[0];
    bus_err_t1_o = bus_err_t0[1];
  end

  logic [NrThreads-1:0] bus_err_t0;

  /* verilator lint_off PINCONNECTEMPTY */  // The wrapper observes only the subset needed by the directed unit test.
  minion_dcache_reduce u_dut (
    .clk_i,
    .rst_ni,
    .global_min_id_i,
    .chicken_bit_dcache_i,
    .reduce_ready_o,
    .reduce_start_i,
    .tensorstore_busy_o,
    .tensorstore_start_i,
    .tensorstore_stride_i,
    .reduce_tensorstore_ctrl_i  (reduce_tensorstore_ctrl),
    .thread_id_i,
    .l2_resp_valid_i            (1'b0),
    .l2_resp_i                  (l2_resp),
    .buffer_avail_o             (unused_buffer_avail),
    .re_l2_req_ready_i,
    .re_l2_req_valid_o,
    .re_l2_req_o                (unused_re_l2_req),
    .ts_l2_req_ready_i,
    .ts_l2_req_valid_o,
    .ts_l2_req_o                (unused_ts_l2_req),
    .re_ts_l2_req_inflight_o    (re_ts_l2_req_inflight),
    .pipe_used_o                (pipe_used),
    .s1_inst_send_val_o,
    .s2_inst_send_val_o         (s2_inst_send_val),
    .s3_inst_send_val_o         (s3_inst_send_val),
    .s1_inst_op_val_o,
    .s2_inst_op_val_o           (s2_inst_op_val),
    .s3_inst_op_val_o           (s3_inst_op_val),
    .ts_scp_read_o              (ts_scp_read),
    .ts_scp_read_way_o          (unused_ts_scp_read_way),
    .ts_scp_read_addr_o         (unused_ts_scp_read_addr),
    .ts_col_size_is_128b_o      (ts_col_size_is_128b),
    .s0_available_i,
    .s0_addr_load_o             (s0_addr_load),
    .s0_addr_o                  (unused_s0_addr),
    .s0_addr_msb_err_o          (unused_s0_addr_msb_err),
    .s0_thread_id_o             (unused_s0_thread_id),
    .s1_available_i,
    .s1_addr_load_o             (s1_addr_load),
    .s1_tlb_wait_i,
    .s1_tlb_fail_i,
    .s2_addr_i,
    .ba_full_i,
    .ba_alloc_o,
    .ba_write_needed_o          (),
    .s2_valid_i,
    .s2_ba_written_entry_i      ('0),
    .s3_ba_write_addr_o         (unused_s3_ba_write_addr),
    .wait_l2_to_ba_resp_o       (wait_l2_to_ba_resp),
    .ba_entries_required_o      (ba_entries_required),
    .s1_valid_i,
    .ba_read_used_i,
    .s1_ba_data_i               ('0),
    .ba_read_en_o               (ba_read_en),
    .ba_read_en_spec_o          (ba_read_en_spec),
    .ba_read_addr_o             (unused_ba_read_addr),
    .scp_ctrl_on_i,
    .vpu_reduce_wait_i,
    .vpu_ctrl_o                 (vpu_ctrl),
    .vm_status_i                (vm_status),
    .vm_status_ts_o             (vm_status_ts),
    .ts_start_o,
    .ts_op_o,
    .mh_going_to_uc_store_i,
    .err_flags_o                (err_flags),
    .bus_err_o                  (bus_err_t0),
    .csr_debug_signals_o        (unused_csr_debug_signals),
    .sm_match_debug_signals_o   (unused_sm_match_debug_signals),
    .sm_filter_debug_signals_o  (unused_sm_filter_debug_signals),
    .sm_data_debug_signals_o    (unused_sm_data_debug_signals)
  );
  /* verilator lint_on PINCONNECTEMPTY */

endmodule
