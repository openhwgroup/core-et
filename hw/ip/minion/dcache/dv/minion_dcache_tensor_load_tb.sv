// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit-test wrapper for minion_dcache_tensor_load.

module minion_dcache_tensor_load_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                         clk_i,
  input  logic                         rst_ni,
  input  logic [MinPerSBits-1:0]       shire_min_id_i,
  input  logic                         chicken_bit_dcache_i,
  input  logic                         tl_use_tmask_i,
  input  logic                         tl_use_coop_i,
  input  logic [2:0]                   tl_transform_i,
  input  logic [DcacheTensorCtrlDestBits-1:0] tl_scp_dest_i,
  input  logic                         tl_sel_i,
  input  logic [VaCaBits-1:0]          tl_addr_i,
  input  logic [1:0]                   tl_offset_i,
  input  logic [DcacheTensorCtrlNlinesBits-1:0] tl_num_lines_i,
  input  logic [VaSize-1:0]            tl_stride_i,
  input  logic                         tl_start_i,
  input  logic                         tl_restart_b_i,
  input  logic                         tl_tmask_ready_i,
  input  logic [TensorMaskBits-1:0]    tl_tmask_bits_i,
  input  logic [ShireCoopNeighMaskSize-1:0] tl_neigh_mask_i,
  input  logic [ShireCoopMinMaskSize-1:0] tl_minion_mask_i,
  input  logic [ShireCoopIdSize-1:0]   tl_coop_id_i,
  input  logic                         thread_id_i,
  input  logic                         s0_available_i,
  input  logic                         s1_available_i,
  input  logic                         s1_tlb_wait_i,
  input  logic                         s1_tlb_fail_i,
  input  logic [PaSize-1:0]            s2_addr_i,
  input  logic                         l2_req_ready_coop_i,
  input  logic                         l2_req_ready_no_coop_i,
  input  logic                         l2_resp_valid_i,
  input  logic [4:0]                   l2_resp_id_i,
  input  logic [1:0]                   l2_resp_opcode_i,
  input  logic [63:0]                  l2_resp_data0_i,
  input  logic [63:0]                  l2_resp_data1_i,
  input  logic [63:0]                  l2_resp_data2_i,
  input  logic [63:0]                  l2_resp_data3_i,
  input  logic                         load_ready_i,
  input  logic                         tfma_enabled_i,
  input  logic                         tenb_tstore_busy_i,
  input  logic                         tenb_credit_i,
  input  logic [DcacheTenbBits-1:0]    tenb_credit_entry_i,
  input  logic                         tenb_written_i,
  output logic                         busy0_o,
  output logic [1:0]                   id_complete0_o,
  output logic                         s0_addr_load0_o,
  output logic [VaSize-1:0]            s0_addr0_o,
  output logic                         s1_addr_load0_o,
  output logic                         l2_req_valid_coop0_o,
  output logic                         l2_req_valid_no_coop0_o,
  output logic [4:0]                   l2_req_id0_o,
  output logic [PaSize-1:0]            l2_req_addr0_o,
  output logic [DcacheWayIdxWidth-1:0] load_way0_o,
  output logic [DcacheLramAddrWidth-1:0] load_addr0_o,
  output logic [DcacheLramNumBanks-1:0] load_valid0_o,
  output logic                         load_valid_pre0_o,
  output logic                         load_capture0_o,
  output logic                         tenb_ready0_o,
  output logic                         tenb_flush0_o,
  output logic [DcacheTlErrorBits-1:0] err_flags0_o,
  output logic [NrThreads-1:0]         bus_err0_o,
  output logic                         tl_start0_o,
  output logic                         tl_op0_o,
  output logic                         busy1_o,
  output logic [1:0]                   id_complete1_o,
  output logic                         s0_addr_load1_o,
  output logic [VaSize-1:0]            s0_addr1_o,
  output logic                         s1_addr_load1_o,
  output logic                         l2_req_valid_coop1_o,
  output logic                         l2_req_valid_no_coop1_o,
  output logic [4:0]                   l2_req_id1_o,
  output logic [PaSize-1:0]            l2_req_addr1_o,
  output logic [DcacheWayIdxWidth-1:0] load_way1_o,
  output logic [DcacheLramAddrWidth-1:0] load_addr1_o,
  output logic [DcacheLramNumBanks-1:0] load_valid1_o,
  output logic                         load_valid_pre1_o,
  output logic                         load_capture1_o,
  output logic                         tenb_ready1_o,
  output logic                         tenb_flush1_o,
  output logic [DcacheTlErrorBits-1:0] err_flags1_o,
  output logic [NrThreads-1:0]         bus_err1_o,
  output logic                         tl_start1_o,
  output logic                         tl_op1_o
);

  core_dcache_ctrl          tensor_ctrl;
  et_link_minion_rsp_info_t l2_resp;
  vm_status_t [NrThreads-1:0] vm_status;
  /* verilator lint_off UNUSEDSIGNAL */  // The directed unit test only inspects the request id/address subset.
  et_link_minion_miss_req_info_t l2_req0;
  et_link_minion_miss_req_info_t l2_req1;
  /* verilator lint_on UNUSEDSIGNAL */
  logic                     unused_s0_addr_msb_err0;
  logic                     unused_s0_thread_id0;
  logic [CoreL2BlockExtSize-1:0] unused_load_data0;
  vm_status_t               unused_vm_status_tl0;
  logic [DcacheDbgCsrTlSize-1:0] unused_csr_debug_signals0;
  logic [DcacheDbgSmMatchBits-1:0] unused_sm_match_debug_signals0;
  logic [DcacheDbgSmFilterBits-1:0] unused_sm_filter_debug_signals0;
  logic [DcacheDbgSmDataBits-1:0] unused_sm_data_debug_signals0;
  logic                     unused_s0_addr_msb_err1;
  logic                     unused_s0_thread_id1;
  logic [CoreL2BlockExtSize-1:0] unused_load_data1;
  vm_status_t               unused_vm_status_tl1;
  logic [DcacheDbgCsrTlSize-1:0] unused_csr_debug_signals1;
  logic [DcacheDbgSmMatchBits-1:0] unused_sm_match_debug_signals1;
  logic [DcacheDbgSmFilterBits-1:0] unused_sm_filter_debug_signals1;
  logic [DcacheDbgSmDataBits-1:0] unused_sm_data_debug_signals1;

  always_comb begin
    tensor_ctrl = '0;
    tensor_ctrl.tensorload_ctrl.use_tmask = tl_use_tmask_i;
    tensor_ctrl.tensorload_ctrl.use_coop = tl_use_coop_i;
    tensor_ctrl.tensorload_ctrl.transform = tensorload_transform_e'(tl_transform_i);
    tensor_ctrl.tensorload_ctrl.scp_dest = tl_scp_dest_i;
    tensor_ctrl.tensorload_ctrl.tl_sel = tl_sel_i;
    tensor_ctrl.tensorload_ctrl.addr = tl_addr_i;
    tensor_ctrl.tensorload_ctrl.offset = tensorload_offset'(tl_offset_i);
    tensor_ctrl.tensorload_ctrl.num_lines = tl_num_lines_i;
    tensor_ctrl.tensorcoop_ctrl.neigh_mask = tl_neigh_mask_i;
    tensor_ctrl.tensorcoop_ctrl.minion_mask = tl_minion_mask_i;
    tensor_ctrl.tensorcoop_ctrl.padding = '0;
    tensor_ctrl.tensorcoop_ctrl.id = tl_coop_id_i;
    tensor_ctrl.tensorload_stride = tl_stride_i;
    tensor_ctrl.tensorload_start = tl_start_i;
    tensor_ctrl.tensorload_restart_b = tl_restart_b_i;
    tensor_ctrl.tensorload_tmask_ready = tl_tmask_ready_i;
    tensor_ctrl.tensorload_tmask_bits = tl_tmask_bits_i;
    tensor_ctrl.thread_id = thread_id_i;

    l2_resp = '0;
    l2_resp.id = {{3{1'b0}}, l2_resp_id_i};
    l2_resp.opcode = et_link_rsp_opcode_e'(l2_resp_opcode_i);
    l2_resp.data[0*64 +: 64] = l2_resp_data0_i;
    l2_resp.data[1*64 +: 64] = l2_resp_data1_i;
    l2_resp.data[2*64 +: 64] = l2_resp_data2_i;
    l2_resp.data[3*64 +: 64] = l2_resp_data3_i;
    vm_status = '{default: '0};
    l2_req_id0_o = l2_req0.id;
    l2_req_addr0_o = l2_req0.address;
    l2_req_id1_o = l2_req1.id;
    l2_req_addr1_o = l2_req1.address;
  end

  minion_dcache_tensor_load #(
    .ModuleIdx (0)
  ) u_dut0 (
    .clk_i,
    .rst_ni,
    .shire_min_id_i         (shire_min_id_i),
    .chicken_bit_dcache_i   (chicken_bit_dcache_i),
    .busy_o                 (busy0_o),
    .id_complete_o          (id_complete0_o),
    .tensor_ctrl_i          (tensor_ctrl),
    .s0_available_i,
    .s0_addr_load_o         (s0_addr_load0_o),
    .s0_addr_o              (s0_addr0_o),
    .s0_addr_msb_err_o      (unused_s0_addr_msb_err0),
    .s0_thread_id_o         (unused_s0_thread_id0),
    .s1_available_i,
    .s1_addr_load_o         (s1_addr_load0_o),
    .s1_tlb_wait_i,
    .s1_tlb_fail_i,
    .s2_addr_i,
    .l2_req_ready_coop_i,
    .l2_req_valid_coop_o    (l2_req_valid_coop0_o),
    .l2_req_ready_no_coop_i,
    .l2_req_valid_no_coop_o (l2_req_valid_no_coop0_o),
    .l2_req_o               (l2_req0),
    .l2_resp_valid_i,
    .l2_resp_i              (l2_resp),
    .load_way_o             (load_way0_o),
    .load_addr_o            (load_addr0_o),
    .load_data_o            (unused_load_data0),
    .load_valid_o           (load_valid0_o),
    .load_valid_pre_o       (load_valid_pre0_o),
    .load_ready_i,
    .load_capture_o         (load_capture0_o),
    .tfma_enabled_i,
    .tenb_tstore_busy_i,
    .tenb_credit_i,
    .tenb_credit_entry_i,
    .tenb_written_i,
    .tenb_ready_o           (tenb_ready0_o),
    .tenb_flush_o           (tenb_flush0_o),
    .vm_status_i            (vm_status),
    .vm_status_tl_o         (unused_vm_status_tl0),
    .err_flags_o            (err_flags0_o),
    .tl_start_o             (tl_start0_o),
    .tl_op_o                (tl_op0_o),
    .bus_err_o              (bus_err0_o),
    .csr_debug_signals_o    (unused_csr_debug_signals0),
    .sm_match_debug_signals_o(unused_sm_match_debug_signals0),
    .sm_filter_debug_signals_o(unused_sm_filter_debug_signals0),
    .sm_data_debug_signals_o(unused_sm_data_debug_signals0)
  );

  minion_dcache_tensor_load #(
    .ModuleIdx (1)
  ) u_dut1 (
    .clk_i,
    .rst_ni,
    .shire_min_id_i         (shire_min_id_i),
    .chicken_bit_dcache_i   (chicken_bit_dcache_i),
    .busy_o                 (busy1_o),
    .id_complete_o          (id_complete1_o),
    .tensor_ctrl_i          (tensor_ctrl),
    .s0_available_i,
    .s0_addr_load_o         (s0_addr_load1_o),
    .s0_addr_o              (s0_addr1_o),
    .s0_addr_msb_err_o      (unused_s0_addr_msb_err1),
    .s0_thread_id_o         (unused_s0_thread_id1),
    .s1_available_i,
    .s1_addr_load_o         (s1_addr_load1_o),
    .s1_tlb_wait_i,
    .s1_tlb_fail_i,
    .s2_addr_i,
    .l2_req_ready_coop_i,
    .l2_req_valid_coop_o    (l2_req_valid_coop1_o),
    .l2_req_ready_no_coop_i,
    .l2_req_valid_no_coop_o (l2_req_valid_no_coop1_o),
    .l2_req_o               (l2_req1),
    .l2_resp_valid_i,
    .l2_resp_i              (l2_resp),
    .load_way_o             (load_way1_o),
    .load_addr_o            (load_addr1_o),
    .load_data_o            (unused_load_data1),
    .load_valid_o           (load_valid1_o),
    .load_valid_pre_o       (load_valid_pre1_o),
    .load_ready_i,
    .load_capture_o         (load_capture1_o),
    .tfma_enabled_i,
    .tenb_tstore_busy_i,
    .tenb_credit_i,
    .tenb_credit_entry_i,
    .tenb_written_i,
    .tenb_ready_o           (tenb_ready1_o),
    .tenb_flush_o           (tenb_flush1_o),
    .vm_status_i            (vm_status),
    .vm_status_tl_o         (unused_vm_status_tl1),
    .err_flags_o            (err_flags1_o),
    .tl_start_o             (tl_start1_o),
    .tl_op_o                (tl_op1_o),
    .bus_err_o              (bus_err1_o),
    .csr_debug_signals_o    (unused_csr_debug_signals1),
    .sm_match_debug_signals_o(unused_sm_match_debug_signals1),
    .sm_filter_debug_signals_o(unused_sm_filter_debug_signals1),
    .sm_data_debug_signals_o(unused_sm_data_debug_signals1)
  );

endmodule
