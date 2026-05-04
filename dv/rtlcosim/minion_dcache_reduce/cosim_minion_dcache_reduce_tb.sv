// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original dcache_reduce vs new minion_dcache_reduce.

`include "soc.vh"

module cosim_minion_dcache_reduce_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                       clk_i,
  input  logic                       rst_ni,
  input  logic [MaxMinionIdBits-1:0] global_min_id_i,
  input  logic                       chicken_bit_dcache_i,
  input  logic                       reduce_start_i,
  input  logic                       tensorstore_start_i,
  input  logic [VaSize-1:0]          tensorstore_stride_i,
  input  logic [63:0]                ctrl_bits_i,
  input  logic                       thread_id_i,
  input  logic                       l2_resp_valid_i,
  input  logic [1:0]                 l2_resp_opcode_i,
  input  logic [7:0]                 l2_resp_id_i,
  input  logic [255:0]               l2_resp_data_i,
  input  logic                       re_l2_req_ready_i,
  input  logic                       ts_l2_req_ready_i,
  input  logic                       s0_available_i,
  input  logic                       s1_available_i,
  input  logic                       s1_tlb_wait_i,
  input  logic                       s1_tlb_fail_i,
  input  logic [PaSize-1:0]          s2_addr_i,
  input  logic                       ba_full_i,
  input  logic                       s2_valid_i,
  input  logic [DcacheBufferIdxWidth-1:0] s2_ba_written_entry_i,
  input  logic                       s1_valid_i,
  input  logic                       ba_read_used_i,
  input  logic [DcacheDataBits-1:0]  s1_ba_data_i,
  input  logic                       scp_ctrl_on_i,
  input  logic                       vpu_reduce_wait_i,
  input  logic                       mh_going_to_uc_store_i,
  output logic                       orig_reduce_ready_o,
  output logic                       new_reduce_ready_o,
  output logic                       orig_tensorstore_busy_o,
  output logic                       new_tensorstore_busy_o,
  output logic                       orig_buffer_avail_o,
  output logic                       new_buffer_avail_o,
  output logic                       orig_re_l2_req_valid_o,
  output logic                       new_re_l2_req_valid_o,
  output logic                       orig_ts_l2_req_valid_o,
  output logic                       new_ts_l2_req_valid_o,
  output logic [4:0]                 orig_re_l2_req_id_o,
  output logic [4:0]                 new_re_l2_req_id_o,
  output logic [4:0]                 orig_ts_l2_req_id_o,
  output logic [4:0]                 new_ts_l2_req_id_o,
  output logic [2:0]                 orig_re_l2_req_opcode_o,
  output logic [2:0]                 new_re_l2_req_opcode_o,
  output logic [2:0]                 orig_ts_l2_req_opcode_o,
  output logic [2:0]                 new_ts_l2_req_opcode_o,
  output logic [EtLinkSubopcodeSize-1:0] orig_re_l2_req_subopcode_o,
  output logic [EtLinkSubopcodeSize-1:0] new_re_l2_req_subopcode_o,
  output logic [EtLinkSubopcodeSize-1:0] orig_ts_l2_req_subopcode_o,
  output logic [EtLinkSubopcodeSize-1:0] new_ts_l2_req_subopcode_o,
  output logic [PaSize-1:0]          orig_re_l2_req_address_o,
  output logic [PaSize-1:0]          new_re_l2_req_address_o,
  output logic [PaSize-1:0]          orig_ts_l2_req_address_o,
  output logic [PaSize-1:0]          new_ts_l2_req_address_o,
  output logic [255:0]               orig_re_l2_req_data_o,
  output logic [255:0]               new_re_l2_req_data_o,
  output logic [255:0]               orig_ts_l2_req_data_o,
  output logic [255:0]               new_ts_l2_req_data_o,
  output logic [2:0]                 orig_re_l2_req_size_o,
  output logic [2:0]                 new_re_l2_req_size_o,
  output logic [2:0]                 orig_ts_l2_req_size_o,
  output logic [2:0]                 new_ts_l2_req_size_o,
  output logic [3:0]                 orig_re_l2_req_qwen_o,
  output logic [3:0]                 new_re_l2_req_qwen_o,
  output logic [3:0]                 orig_ts_l2_req_qwen_o,
  output logic [3:0]                 new_ts_l2_req_qwen_o,
  output logic                       orig_re_ts_l2_req_inflight_o,
  output logic                       new_re_ts_l2_req_inflight_o,
  output logic                       orig_pipe_used_o,
  output logic                       new_pipe_used_o,
  output logic                       orig_s1_inst_send_val_o,
  output logic                       new_s1_inst_send_val_o,
  output logic                       orig_s2_inst_send_val_o,
  output logic                       new_s2_inst_send_val_o,
  output logic                       orig_s3_inst_send_val_o,
  output logic                       new_s3_inst_send_val_o,
  output logic                       orig_s1_inst_op_val_o,
  output logic                       new_s1_inst_op_val_o,
  output logic                       orig_s2_inst_op_val_o,
  output logic                       new_s2_inst_op_val_o,
  output logic                       orig_s3_inst_op_val_o,
  output logic                       new_s3_inst_op_val_o,
  output logic                       orig_ts_scp_read_o,
  output logic                       new_ts_scp_read_o,
  output logic [DcacheWayIdxWidth-1:0] orig_ts_scp_read_way_o,
  output logic [DcacheWayIdxWidth-1:0] new_ts_scp_read_way_o,
  output logic [DcacheLramAddrWidth-1:0] orig_ts_scp_read_addr_o,
  output logic [DcacheLramAddrWidth-1:0] new_ts_scp_read_addr_o,
  output logic                       orig_ts_col_size_is_128b_o,
  output logic                       new_ts_col_size_is_128b_o,
  output logic                       orig_s0_addr_load_o,
  output logic                       new_s0_addr_load_o,
  output logic [VaSize-1:0]          orig_s0_addr_o,
  output logic [VaSize-1:0]          new_s0_addr_o,
  output logic                       orig_s0_addr_msb_err_o,
  output logic                       new_s0_addr_msb_err_o,
  output logic                       orig_s0_thread_id_o,
  output logic                       new_s0_thread_id_o,
  output logic                       orig_s1_addr_load_o,
  output logic                       new_s1_addr_load_o,
  output logic                       orig_ba_alloc_o,
  output logic                       new_ba_alloc_o,
  output logic                       orig_ba_write_needed_o,
  output logic                       new_ba_write_needed_o,
  output logic [DcacheBufferIdxWidth-1:0] orig_s3_ba_write_addr_o,
  output logic [DcacheBufferIdxWidth-1:0] new_s3_ba_write_addr_o,
  output logic                       orig_wait_l2_to_ba_resp_o,
  output logic                       new_wait_l2_to_ba_resp_o,
  output logic                       orig_ba_entries_required_o,
  output logic                       new_ba_entries_required_o,
  output logic                       orig_ba_read_en_o,
  output logic                       new_ba_read_en_o,
  output logic                       orig_ba_read_en_spec_o,
  output logic                       new_ba_read_en_spec_o,
  output logic [DcacheBufferIdxWidth-1:0] orig_ba_read_addr_o,
  output logic [DcacheBufferIdxWidth-1:0] new_ba_read_addr_o,
  output logic                       orig_vpu_send_reg_o,
  output logic                       new_vpu_send_reg_o,
  output logic                       orig_vpu_exec_op_o,
  output logic                       new_vpu_exec_op_o,
  output logic                       orig_vpu_nothing_o,
  output logic                       new_vpu_nothing_o,
  output logic [7:0]                 orig_vm_status_ts_o,
  output logic [7:0]                 new_vm_status_ts_o,
  output logic                       orig_ts_start_o,
  output logic                       new_ts_start_o,
  output logic                       orig_ts_op_o,
  output logic                       new_ts_op_o,
  output logic [3:0]                 orig_err_flags_o,
  output logic [3:0]                 new_err_flags_o,
  output logic [NrThreads-1:0]       orig_bus_err_o,
  output logic [NrThreads-1:0]       new_bus_err_o,
  output logic [DcacheDbgCsrTsSize-1:0] orig_csr_debug_signals_o,
  output logic [DcacheDbgCsrTsSize-1:0] new_csr_debug_signals_o
);

  reduce_tensorstore_control_t reduce_ctrl_new;
  reduce_tensorstore_control   reduce_ctrl_orig;
  vm_status_t [NrThreads-1:0]  vm_status_new;
  minion_vm_status [NrThreads-1:0] vm_status_orig;
  et_link_minion_rsp_info_t    l2_resp_new;
  et_link_minion_rsp_info_t    l2_resp_orig;
  et_link_minion_evict_req_info_t re_l2_req_new, ts_l2_req_new;
  et_link_minion_evict_req_info_t re_l2_req_orig, ts_l2_req_orig;
  dcache_vpu_reduce_ctrl_t     vpu_ctrl_new;
  dcache_vpu_reduce_ctrl       vpu_ctrl_orig;
  vm_status_t                  vm_status_ts_new;
  minion_vm_status             vm_status_ts_orig;
  logic [DcacheErrFlagRange:0][NrThreads-1:0] err_flags_new;
  logic [`DCACHE_ERR_FLAG_RANGE][`CORE_NR_THREADS-1:0] err_flags_orig;
  logic [DcacheDbgSmMatchBits-1:0] sm_match_debug_signals_new;
  logic [`DCACHE_DBG_SM_MATCH_BITS-1:0] sm_match_debug_signals_orig;
  logic [DcacheDbgSmFilterBits-1:0] sm_filter_debug_signals_new;
  logic [`DCACHE_DBG_SM_FILTER_BITS-1:0] sm_filter_debug_signals_orig;
  logic [DcacheDbgSmDataBits-1:0] sm_data_debug_signals_new;
  logic [`DCACHE_DBG_SM_DATA_BITS-1:0] sm_data_debug_signals_orig;

  assign reduce_ctrl_new = reduce_tensorstore_control_t'(ctrl_bits_i);
  assign reduce_ctrl_orig = reduce_tensorstore_control'(ctrl_bits_i);

  for (genvar i = 0; i < NrThreads; i++) begin : gen_vm_status
    assign vm_status_new[i] = '0;
    assign vm_status_orig[i] = '0;
  end

  assign l2_resp_new = '0;
  assign l2_resp_new.opcode = et_link_rsp_opcode_e'(l2_resp_opcode_i);
  assign l2_resp_new.id = l2_resp_id_i;
  assign l2_resp_new.data = l2_resp_data_i;

  assign l2_resp_orig = '0;
  assign l2_resp_orig.opcode = et_link_rsp_opcode_e'(l2_resp_opcode_i);
  assign l2_resp_orig.id = l2_resp_id_i;
  assign l2_resp_orig.data = l2_resp_data_i;

  minion_dcache_reduce u_new (
    .clk_i,
    .rst_ni,
    .global_min_id_i,
    .chicken_bit_dcache_i,
    .reduce_ready_o         (new_reduce_ready_o),
    .reduce_start_i,
    .tensorstore_busy_o     (new_tensorstore_busy_o),
    .tensorstore_start_i,
    .tensorstore_stride_i,
    .reduce_tensorstore_ctrl_i(reduce_ctrl_new),
    .thread_id_i,
    .l2_resp_valid_i,
    .l2_resp_i              (l2_resp_new),
    .buffer_avail_o         (new_buffer_avail_o),
    .re_l2_req_ready_i,
    .re_l2_req_valid_o      (new_re_l2_req_valid_o),
    .re_l2_req_o            (re_l2_req_new),
    .ts_l2_req_ready_i,
    .ts_l2_req_valid_o      (new_ts_l2_req_valid_o),
    .ts_l2_req_o            (ts_l2_req_new),
    .re_ts_l2_req_inflight_o(new_re_ts_l2_req_inflight_o),
    .pipe_used_o            (new_pipe_used_o),
    .s1_inst_send_val_o     (new_s1_inst_send_val_o),
    .s2_inst_send_val_o     (new_s2_inst_send_val_o),
    .s3_inst_send_val_o     (new_s3_inst_send_val_o),
    .s1_inst_op_val_o       (new_s1_inst_op_val_o),
    .s2_inst_op_val_o       (new_s2_inst_op_val_o),
    .s3_inst_op_val_o       (new_s3_inst_op_val_o),
    .ts_scp_read_o          (new_ts_scp_read_o),
    .ts_scp_read_way_o      (new_ts_scp_read_way_o),
    .ts_scp_read_addr_o     (new_ts_scp_read_addr_o),
    .ts_col_size_is_128b_o  (new_ts_col_size_is_128b_o),
    .s0_available_i,
    .s0_addr_load_o         (new_s0_addr_load_o),
    .s0_addr_o              (new_s0_addr_o),
    .s0_addr_msb_err_o      (new_s0_addr_msb_err_o),
    .s0_thread_id_o         (new_s0_thread_id_o),
    .s1_available_i,
    .s1_addr_load_o         (new_s1_addr_load_o),
    .s1_tlb_wait_i,
    .s1_tlb_fail_i,
    .s2_addr_i,
    .ba_full_i,
    .ba_alloc_o             (new_ba_alloc_o),
    .ba_write_needed_o      (new_ba_write_needed_o),
    .s2_valid_i,
    .s2_ba_written_entry_i,
    .s3_ba_write_addr_o     (new_s3_ba_write_addr_o),
    .wait_l2_to_ba_resp_o   (new_wait_l2_to_ba_resp_o),
    .ba_entries_required_o  (new_ba_entries_required_o),
    .s1_valid_i,
    .ba_read_used_i,
    .s1_ba_data_i,
    .ba_read_en_o           (new_ba_read_en_o),
    .ba_read_en_spec_o      (new_ba_read_en_spec_o),
    .ba_read_addr_o         (new_ba_read_addr_o),
    .scp_ctrl_on_i,
    .vpu_reduce_wait_i,
    .vpu_ctrl_o             (vpu_ctrl_new),
    .vm_status_i            (vm_status_new),
    .vm_status_ts_o         (vm_status_ts_new),
    .ts_start_o             (new_ts_start_o),
    .ts_op_o                (new_ts_op_o),
    .mh_going_to_uc_store_i,
    .err_flags_o            (err_flags_new),
    .bus_err_o              (new_bus_err_o),
    .csr_debug_signals_o    (new_csr_debug_signals_o),
    .sm_match_debug_signals_o(sm_match_debug_signals_new),
    .sm_filter_debug_signals_o(sm_filter_debug_signals_new),
    .sm_data_debug_signals_o(sm_data_debug_signals_new)
  );

  dcache_reduce u_orig (
    .clock                  (clk_i),
    .reset                  (!rst_ni),
    .global_min_id          (global_min_id_i),
    .chicken_bit_dcache     (chicken_bit_dcache_i),
    .reduce_ready           (orig_reduce_ready_o),
    .reduce_start           (reduce_start_i),
    .tensorstore_busy       (orig_tensorstore_busy_o),
    .tensorstore_start      (tensorstore_start_i),
    .tensorstore_stride     (tensorstore_stride_i),
    .reduce_tensorstore_ctrl(reduce_ctrl_orig),
    .thread_id              (thread_id_i),
    .l2_resp_valid          (l2_resp_valid_i),
    .l2_resp                (l2_resp_orig),
    .buffer_avail           (orig_buffer_avail_o),
    .re_l2_req_ready        (re_l2_req_ready_i),
    .re_l2_req_valid        (orig_re_l2_req_valid_o),
    .re_l2_req              (re_l2_req_orig),
    .ts_l2_req_ready        (ts_l2_req_ready_i),
    .ts_l2_req_valid        (orig_ts_l2_req_valid_o),
    .ts_l2_req              (ts_l2_req_orig),
    .re_ts_l2_req_inflight  (orig_re_ts_l2_req_inflight_o),
    .pipe_used              (orig_pipe_used_o),
    .s1_inst_send_val       (orig_s1_inst_send_val_o),
    .s2_inst_send_val       (orig_s2_inst_send_val_o),
    .s3_inst_send_val       (orig_s3_inst_send_val_o),
    .s1_inst_op_val         (orig_s1_inst_op_val_o),
    .s2_inst_op_val         (orig_s2_inst_op_val_o),
    .s3_inst_op_val         (orig_s3_inst_op_val_o),
    .ts_scp_read            (orig_ts_scp_read_o),
    .ts_scp_read_way        (orig_ts_scp_read_way_o),
    .ts_scp_read_addr       (orig_ts_scp_read_addr_o),
    .ts_col_size_is_128b    (orig_ts_col_size_is_128b_o),
    .s0_available           (s0_available_i),
    .s0_addr_load           (orig_s0_addr_load_o),
    .s0_addr                (orig_s0_addr_o),
    .s0_addr_msb_err        (orig_s0_addr_msb_err_o),
    .s0_thread_id           (orig_s0_thread_id_o),
    .s1_available           (s1_available_i),
    .s1_addr_load           (orig_s1_addr_load_o),
    .s1_tlb_wait            (s1_tlb_wait_i),
    .s1_tlb_fail            (s1_tlb_fail_i),
    .s2_addr                (s2_addr_i),
    .ba_full                (ba_full_i),
    .ba_alloc               (orig_ba_alloc_o),
    .ba_write_needed        (orig_ba_write_needed_o),
    .s2_valid               (s2_valid_i),
    .s2_ba_written_entry    (s2_ba_written_entry_i),
    .s3_ba_write_addr       (orig_s3_ba_write_addr_o),
    .wait_l2_to_ba_resp     (orig_wait_l2_to_ba_resp_o),
    .ba_entries_required    (orig_ba_entries_required_o),
    .s1_valid               (s1_valid_i),
    .ba_read_used           (ba_read_used_i),
    .s1_ba_data             (s1_ba_data_i),
    .ba_read_en             (orig_ba_read_en_o),
    .ba_read_en_spec        (orig_ba_read_en_spec_o),
    .ba_read_addr           (orig_ba_read_addr_o),
    .scp_ctrl_on            (scp_ctrl_on_i),
    .vpu_reduce_wait        (vpu_reduce_wait_i),
    .vpu_ctrl               (vpu_ctrl_orig),
    .vm_status              (vm_status_orig),
    .vm_status_ts           (vm_status_ts_orig),
    .ts_start               (orig_ts_start_o),
    .ts_op                  (orig_ts_op_o),
    .mh_going_to_uc_store   (mh_going_to_uc_store_i),
    .err_flags              (err_flags_orig),
    .bus_err                (orig_bus_err_o),
    .csr_debug_signals      (orig_csr_debug_signals_o),
    .sm_match_debug_signals (sm_match_debug_signals_orig),
    .sm_filter_debug_signals(sm_filter_debug_signals_orig),
    .sm_data_debug_signals  (sm_data_debug_signals_orig)
  );

  assign orig_re_l2_req_id_o = re_l2_req_orig.id;
  assign new_re_l2_req_id_o = re_l2_req_new.id;
  assign orig_ts_l2_req_id_o = ts_l2_req_orig.id;
  assign new_ts_l2_req_id_o = ts_l2_req_new.id;
  assign orig_re_l2_req_opcode_o = re_l2_req_orig.opcode;
  assign new_re_l2_req_opcode_o = re_l2_req_new.opcode;
  assign orig_ts_l2_req_opcode_o = ts_l2_req_orig.opcode;
  assign new_ts_l2_req_opcode_o = ts_l2_req_new.opcode;
  assign orig_re_l2_req_subopcode_o = re_l2_req_orig.subopcode;
  assign new_re_l2_req_subopcode_o = re_l2_req_new.subopcode;
  assign orig_ts_l2_req_subopcode_o = ts_l2_req_orig.subopcode;
  assign new_ts_l2_req_subopcode_o = ts_l2_req_new.subopcode;
  assign orig_re_l2_req_address_o = re_l2_req_orig.address;
  assign new_re_l2_req_address_o = re_l2_req_new.address;
  assign orig_ts_l2_req_address_o = ts_l2_req_orig.address;
  assign new_ts_l2_req_address_o = ts_l2_req_new.address;
  assign orig_re_l2_req_data_o = re_l2_req_orig.data;
  assign new_re_l2_req_data_o = re_l2_req_new.data;
  assign orig_ts_l2_req_data_o = ts_l2_req_orig.data;
  assign new_ts_l2_req_data_o = ts_l2_req_new.data;
  assign orig_re_l2_req_size_o = re_l2_req_orig.size;
  assign new_re_l2_req_size_o = re_l2_req_new.size;
  assign orig_ts_l2_req_size_o = ts_l2_req_orig.size;
  assign new_ts_l2_req_size_o = ts_l2_req_new.size;
  assign orig_re_l2_req_qwen_o = re_l2_req_orig.qwen;
  assign new_re_l2_req_qwen_o = re_l2_req_new.qwen;
  assign orig_ts_l2_req_qwen_o = ts_l2_req_orig.qwen;
  assign new_ts_l2_req_qwen_o = ts_l2_req_new.qwen;

  assign orig_vpu_send_reg_o = vpu_ctrl_orig.send_reg;
  assign new_vpu_send_reg_o = vpu_ctrl_new.send_reg;
  assign orig_vpu_exec_op_o = vpu_ctrl_orig.exec_op;
  assign new_vpu_exec_op_o = vpu_ctrl_new.exec_op;
  assign orig_vpu_nothing_o = vpu_ctrl_orig.nothing;
  assign new_vpu_nothing_o = vpu_ctrl_new.nothing;

  assign orig_vm_status_ts_o = {vm_status_ts_orig.prv, vm_status_ts_orig.mprv,
      vm_status_ts_orig.mpp, vm_status_ts_orig.sum, vm_status_ts_orig.mxr, vm_status_ts_orig.debug};
  assign new_vm_status_ts_o = {vm_status_ts_new.prv, vm_status_ts_new.mprv,
      vm_status_ts_new.mpp, vm_status_ts_new.sum, vm_status_ts_new.mxr, vm_status_ts_new.debug};

  assign orig_err_flags_o = {err_flags_orig[1][1], err_flags_orig[1][0], err_flags_orig[0][1], err_flags_orig[0][0]};
  assign new_err_flags_o = {err_flags_new[1][1], err_flags_new[1][0], err_flags_new[0][1], err_flags_new[0][0]};

endmodule : cosim_minion_dcache_reduce_tb
