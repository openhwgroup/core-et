// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original dcache_tensor_load vs new minion_dcache_tensor_load.

`include "soc.vh"

module cosim_minion_dcache_tensor_load_tb
  import dft_pkg::*;
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
  input  logic [7:0]                   l2_resp_id_i,
  input  logic [1:0]                   l2_resp_opcode_i,
  input  logic [255:0]                 l2_resp_data_i,
  input  logic                         load_ready_i,
  input  logic                         tfma_enabled_i,
  input  logic                         tenb_tstore_busy_i,
  input  logic                         tenb_credit_i,
  input  logic [DcacheTenbBits-1:0]    tenb_credit_entry_i,
  input  logic                         tenb_written_i,
  output logic                         orig_busy0_o,
  output logic                         new_busy0_o,
  output logic [1:0]                   orig_id_complete0_o,
  output logic [1:0]                   new_id_complete0_o,
  output logic                         orig_s0_addr_load0_o,
  output logic                         new_s0_addr_load0_o,
  output logic [VaSize-1:0]            orig_s0_addr0_o,
  output logic [VaSize-1:0]            new_s0_addr0_o,
  output logic                         orig_s0_addr_msb_err0_o,
  output logic                         new_s0_addr_msb_err0_o,
  output logic                         orig_s0_thread_id0_o,
  output logic                         new_s0_thread_id0_o,
  output logic                         orig_s1_addr_load0_o,
  output logic                         new_s1_addr_load0_o,
  output logic                         orig_l2_req_valid_coop0_o,
  output logic                         new_l2_req_valid_coop0_o,
  output logic                         orig_l2_req_valid_no_coop0_o,
  output logic                         new_l2_req_valid_no_coop0_o,
  output logic [4:0]                   orig_l2_req_id0_o,
  output logic [4:0]                   new_l2_req_id0_o,
  output logic [2:0]                   orig_l2_req_opcode0_o,
  output logic [2:0]                   new_l2_req_opcode0_o,
  output logic [PaSize-1:0]            orig_l2_req_addr0_o,
  output logic [PaSize-1:0]            new_l2_req_addr0_o,
  output logic [$bits(tensorcoop_control)-1:0] orig_l2_req_data0_o,
  output logic [$bits(tensorcoop_control)-1:0] new_l2_req_data0_o,
  output logic [3:0]                   orig_l2_req_qwen0_o,
  output logic [3:0]                   new_l2_req_qwen0_o,
  output logic [DcacheWayIdxWidth-1:0] orig_load_way0_o,
  output logic [DcacheWayIdxWidth-1:0] new_load_way0_o,
  output logic [DcacheLramAddrWidth-1:0] orig_load_addr0_o,
  output logic [DcacheLramAddrWidth-1:0] new_load_addr0_o,
  output logic [63:0]                  orig_load_data0_o [4],
  output logic [63:0]                  new_load_data0_o [4],
  output logic [DcacheLramNumBanks-1:0] orig_load_valid0_o,
  output logic [DcacheLramNumBanks-1:0] new_load_valid0_o,
  output logic                         orig_load_valid_pre0_o,
  output logic                         new_load_valid_pre0_o,
  output logic                         orig_load_capture0_o,
  output logic                         new_load_capture0_o,
  output logic                         orig_tenb_ready0_o,
  output logic                         new_tenb_ready0_o,
  output logic                         orig_tenb_flush0_o,
  output logic                         new_tenb_flush0_o,
  output logic [7:0]                   orig_vm_status_tl0_o,
  output logic [7:0]                   new_vm_status_tl0_o,
  output logic [DcacheTlErrorBits-1:0] orig_err_flags0_o,
  output logic [DcacheTlErrorBits-1:0] new_err_flags0_o,
  output logic                         orig_tl_start0_o,
  output logic                         new_tl_start0_o,
  output logic                         orig_tl_op0_o,
  output logic                         new_tl_op0_o,
  output logic [NrThreads-1:0]         orig_bus_err0_o,
  output logic [NrThreads-1:0]         new_bus_err0_o,
  output logic [DcacheDbgCsrTlSize-1:0] orig_csr_debug0_o,
  output logic [DcacheDbgCsrTlSize-1:0] new_csr_debug0_o,
  output logic [DcacheDbgSmMatchBits-1:0] orig_sm_match0_o,
  output logic [DcacheDbgSmMatchBits-1:0] new_sm_match0_o,
  output logic [63:0]                  orig_sm_filter0_o [4],
  output logic [63:0]                  new_sm_filter0_o [4],
  output logic [63:0]                  orig_sm_data0_o [8],
  output logic [63:0]                  new_sm_data0_o [8],
  output logic [2:0]                   orig_ctrl_state0_dbg_o,
  output logic [2:0]                   new_ctrl_state0_dbg_o,
  output logic                         orig_enabled0_dbg_o,
  output logic                         new_enabled0_dbg_o,
  output logic [3:0]                   orig_count0_dbg_o,
  output logic [3:0]                   new_count0_dbg_o,
  output logic [3:0]                   orig_shared_count0_dbg_o,
  output logic [3:0]                   new_shared_count0_dbg_o,
  output logic                         orig_l2_busy0_dbg_o,
  output logic                         new_l2_busy0_dbg_o,
  output logic                         orig_shared_load_idx0_dbg_o,
  output logic                         new_shared_load_idx0_dbg_o,
  output logic [2:0]                   orig_trans_split0_dbg_o,
  output logic [2:0]                   new_trans_split0_dbg_o,
  output logic [3:0]                   orig_shared_l2_counter0_dbg_o,
  output logic [3:0]                   new_shared_l2_counter0_dbg_o,
  output logic [15:0]                  orig_shared_l2_store0_dbg_o,
  output logic [15:0]                  new_shared_l2_store0_dbg_o,
  output logic [2:0]                   orig_transform0_dbg_o,
  output logic [2:0]                   new_transform0_dbg_o,
  output logic [5:0]                   orig_trans_flags0_dbg_o,
  output logic [5:0]                   new_trans_flags0_dbg_o,
  output logic [63:0]                  orig_tmp_data00_dbg_o,
  output logic [63:0]                  new_tmp_data00_dbg_o,
  output logic                         orig_l2_resp_for_me0_dbg_o,
  output logic                         new_l2_resp_for_me0_dbg_o,
  output logic                         orig_busy1_o,
  output logic                         new_busy1_o,
  output logic [1:0]                   orig_id_complete1_o,
  output logic [1:0]                   new_id_complete1_o,
  output logic                         orig_s0_addr_load1_o,
  output logic                         new_s0_addr_load1_o,
  output logic [VaSize-1:0]            orig_s0_addr1_o,
  output logic [VaSize-1:0]            new_s0_addr1_o,
  output logic                         orig_s0_addr_msb_err1_o,
  output logic                         new_s0_addr_msb_err1_o,
  output logic                         orig_s0_thread_id1_o,
  output logic                         new_s0_thread_id1_o,
  output logic                         orig_s1_addr_load1_o,
  output logic                         new_s1_addr_load1_o,
  output logic                         orig_l2_req_valid_coop1_o,
  output logic                         new_l2_req_valid_coop1_o,
  output logic                         orig_l2_req_valid_no_coop1_o,
  output logic                         new_l2_req_valid_no_coop1_o,
  output logic [4:0]                   orig_l2_req_id1_o,
  output logic [4:0]                   new_l2_req_id1_o,
  output logic [2:0]                   orig_l2_req_opcode1_o,
  output logic [2:0]                   new_l2_req_opcode1_o,
  output logic [PaSize-1:0]            orig_l2_req_addr1_o,
  output logic [PaSize-1:0]            new_l2_req_addr1_o,
  output logic [$bits(tensorcoop_control)-1:0] orig_l2_req_data1_o,
  output logic [$bits(tensorcoop_control)-1:0] new_l2_req_data1_o,
  output logic [3:0]                   orig_l2_req_qwen1_o,
  output logic [3:0]                   new_l2_req_qwen1_o,
  output logic [DcacheWayIdxWidth-1:0] orig_load_way1_o,
  output logic [DcacheWayIdxWidth-1:0] new_load_way1_o,
  output logic [DcacheLramAddrWidth-1:0] orig_load_addr1_o,
  output logic [DcacheLramAddrWidth-1:0] new_load_addr1_o,
  output logic [63:0]                  orig_load_data1_o [4],
  output logic [63:0]                  new_load_data1_o [4],
  output logic [DcacheLramNumBanks-1:0] orig_load_valid1_o,
  output logic [DcacheLramNumBanks-1:0] new_load_valid1_o,
  output logic                         orig_load_valid_pre1_o,
  output logic                         new_load_valid_pre1_o,
  output logic                         orig_load_capture1_o,
  output logic                         new_load_capture1_o,
  output logic                         orig_tenb_ready1_o,
  output logic                         new_tenb_ready1_o,
  output logic                         orig_tenb_flush1_o,
  output logic                         new_tenb_flush1_o,
  output logic [7:0]                   orig_vm_status_tl1_o,
  output logic [7:0]                   new_vm_status_tl1_o,
  output logic [DcacheTlErrorBits-1:0] orig_err_flags1_o,
  output logic [DcacheTlErrorBits-1:0] new_err_flags1_o,
  output logic                         orig_tl_start1_o,
  output logic                         new_tl_start1_o,
  output logic                         orig_tl_op1_o,
  output logic                         new_tl_op1_o,
  output logic [NrThreads-1:0]         orig_bus_err1_o,
  output logic [NrThreads-1:0]         new_bus_err1_o,
  output logic [DcacheDbgCsrTlSize-1:0] orig_csr_debug1_o,
  output logic [DcacheDbgCsrTlSize-1:0] new_csr_debug1_o,
  output logic [DcacheDbgSmMatchBits-1:0] orig_sm_match1_o,
  output logic [DcacheDbgSmMatchBits-1:0] new_sm_match1_o,
  output logic [63:0]                  orig_sm_filter1_o [4],
  output logic [63:0]                  new_sm_filter1_o [4],
  output logic [63:0]                  orig_sm_data1_o [8],
  output logic [63:0]                  new_sm_data1_o [8],
  output logic [2:0]                   orig_ctrl_state1_dbg_o,
  output logic [2:0]                   new_ctrl_state1_dbg_o,
  output logic                         orig_enabled1_dbg_o,
  output logic                         new_enabled1_dbg_o,
  output logic [3:0]                   orig_count1_dbg_o,
  output logic [3:0]                   new_count1_dbg_o,
  output logic [DcacheWayIdxWidth-1:0] orig_load_way_sel1_dbg_o,
  output logic [DcacheWayIdxWidth-1:0] new_load_way_sel1_dbg_o,
  output logic [VpuTenbSize/2-1:0]     orig_tenb_avail1_dbg_o,
  output logic [VpuTenbSize/2-1:0]     new_tenb_avail1_dbg_o,
  output logic                         orig_tenb_rcv_ok1_dbg_o,
  output logic                         new_tenb_rcv_ok1_dbg_o,
  output logic                         orig_tenb_credits_ok1_dbg_o,
  output logic                         new_tenb_credits_ok1_dbg_o,
  output logic [2:0]                   orig_tenb_credits1_dbg_o,
  output logic [2:0]                   new_tenb_credits1_dbg_o,
  output logic                         orig_cond31_dbg_o,
  output logic                         new_cond31_dbg_o,
  output logic [1:0]                   orig_tenb_count1_dbg_o,
  output logic [1:0]                   new_tenb_count1_dbg_o,
  output logic                         orig_s2_req_valid1_dbg_o,
  output logic                         new_s2_req_valid1_dbg_o,
  output logic                         orig_s2_req_ready1_dbg_o,
  output logic                         new_s2_req_ready1_dbg_o,
  output logic [1:0]                   orig_sel_t1_dbg_o,
  output logic [1:0]                   new_sel_t1_dbg_o,
  output logic [DcacheSetIdxWidth-1:0] orig_sel_set1_dbg_o,
  output logic [DcacheSetIdxWidth-1:0] new_sel_set1_dbg_o,
  output logic                         orig_sel_idx1_dbg_o,
  output logic                         new_sel_idx1_dbg_o,
  output logic                         orig_tfma_started1_dbg_o,
  output logic                         new_tfma_started1_dbg_o,
  output logic                         orig_tfma_active_start1_dbg_o,
  output logic                         new_tfma_active_start1_dbg_o
);

  core_dcache_ctrl         tensor_ctrl_new;
  core_dcache_tensor_ctrl  tensor_ctrl_orig;
  vm_status_t [NrThreads-1:0] vm_status_new;
  minion_vm_status [NrThreads-1:0] vm_status_orig;
  et_link_minion_rsp_info_t l2_resp_new;
  et_link_minion_rsp_info_t l2_resp_orig;
  et_link_minion_miss_req_info_t l2_req_new_0, l2_req_new_1;
  et_link_minion_miss_req_info_t l2_req_orig_0, l2_req_orig_1;
  logic reset_orig;
  logic [DcacheDbgSmFilterBits-1:0] orig_sm_filter0_bits, new_sm_filter0_bits;
  logic [DcacheDbgSmFilterBits-1:0] orig_sm_filter1_bits, new_sm_filter1_bits;
  logic [DcacheDbgSmDataBits-1:0] orig_sm_data0_bits, new_sm_data0_bits;
  logic [DcacheDbgSmDataBits-1:0] orig_sm_data1_bits, new_sm_data1_bits;
  logic [CoreL2BlockExtSize-1:0] orig_load_data0_bits, new_load_data0_bits;
  logic [CoreL2BlockExtSize-1:0] orig_load_data1_bits, new_load_data1_bits;

  always_comb begin
    reset_orig = ~rst_ni;
    tensor_ctrl_new = '0;
    tensor_ctrl_new.tensorload_ctrl.use_tmask = tl_use_tmask_i;
    tensor_ctrl_new.tensorload_ctrl.use_coop = tl_use_coop_i;
    tensor_ctrl_new.tensorload_ctrl.transform = tensorload_transform_e'(tl_transform_i);
    tensor_ctrl_new.tensorload_ctrl.scp_dest = tl_scp_dest_i;
    tensor_ctrl_new.tensorload_ctrl.tl_sel = tl_sel_i;
    tensor_ctrl_new.tensorload_ctrl.addr = tl_addr_i;
    tensor_ctrl_new.tensorload_ctrl.offset = tensorload_offset'(tl_offset_i);
    tensor_ctrl_new.tensorload_ctrl.num_lines = tl_num_lines_i;
    tensor_ctrl_new.tensorcoop_ctrl.neigh_mask = tl_neigh_mask_i;
    tensor_ctrl_new.tensorcoop_ctrl.minion_mask = tl_minion_mask_i;
    tensor_ctrl_new.tensorcoop_ctrl.padding = '0;
    tensor_ctrl_new.tensorcoop_ctrl.id = tl_coop_id_i;
    tensor_ctrl_new.tensorload_stride = tl_stride_i;
    tensor_ctrl_new.tensorload_start = tl_start_i;
    tensor_ctrl_new.tensorload_restart_b = tl_restart_b_i;
    tensor_ctrl_new.tensorload_tmask_ready = tl_tmask_ready_i;
    tensor_ctrl_new.tensorload_tmask_bits = tl_tmask_bits_i;
    tensor_ctrl_new.thread_id = thread_id_i;

    tensor_ctrl_orig = '{
      ctrl: '{
        use_tmask: tl_use_tmask_i,
        use_coop: tl_use_coop_i,
        transform: tensorload_transform_e'(tl_transform_i),
        scp_dest: tl_scp_dest_i,
        tl_sel: tl_sel_i,
        addr: tl_addr_i,
        offset: tensorload_offset'(tl_offset_i),
        num_lines: tl_num_lines_i
      },
      coop: '{
        neigh_mask: tl_neigh_mask_i,
        minion_mask: tl_minion_mask_i,
        padding: '0,
        id: tl_coop_id_i
      },
      stride: tl_stride_i,
      start: tl_start_i,
      restart_b: tl_restart_b_i,
      tmask_ready: tl_tmask_ready_i,
      tmask_bits: tl_tmask_bits_i,
      thread_id: thread_id_i
    };

    for (int i = 0; i < NrThreads; i++) begin
      vm_status_new[i] = '0;
      vm_status_orig[i] = '0;
    end

    l2_resp_new = '0;
    l2_resp_new.id = l2_resp_id_i;
    l2_resp_new.opcode = et_link_rsp_opcode_e'(l2_resp_opcode_i);
    l2_resp_new.data = l2_resp_data_i;
    l2_resp_orig = '0;
    l2_resp_orig.id = l2_resp_id_i;
    l2_resp_orig.opcode = et_link_rsp_opcode_e'(l2_resp_opcode_i);
    l2_resp_orig.data = l2_resp_data_i;

    for (int i = 0; i < 4; i++) begin
      orig_load_data0_o[i] = orig_load_data0_bits[i*64 +: 64];
      new_load_data0_o[i] = new_load_data0_bits[i*64 +: 64];
      orig_load_data1_o[i] = orig_load_data1_bits[i*64 +: 64];
      new_load_data1_o[i] = new_load_data1_bits[i*64 +: 64];
      orig_sm_filter0_o[i] = orig_sm_filter0_bits[i*64 +: 64];
      new_sm_filter0_o[i] = new_sm_filter0_bits[i*64 +: 64];
      orig_sm_filter1_o[i] = orig_sm_filter1_bits[i*64 +: 64];
      new_sm_filter1_o[i] = new_sm_filter1_bits[i*64 +: 64];
    end
    for (int i = 0; i < 8; i++) begin
      orig_sm_data0_o[i] = orig_sm_data0_bits[i*64 +: 64];
      new_sm_data0_o[i] = new_sm_data0_bits[i*64 +: 64];
      orig_sm_data1_o[i] = orig_sm_data1_bits[i*64 +: 64];
      new_sm_data1_o[i] = new_sm_data1_bits[i*64 +: 64];
    end
  end

  minion_dcache_tensor_load #(.ModuleIdx(0)) u_new_0 (
    .clk_i, .rst_ni, .shire_min_id_i, .chicken_bit_dcache_i,
    .busy_o(new_busy0_o), .id_complete_o(new_id_complete0_o), .tensor_ctrl_i(tensor_ctrl_new),
    .s0_available_i, .s0_addr_load_o(new_s0_addr_load0_o), .s0_addr_o(new_s0_addr0_o),
    .s0_addr_msb_err_o(new_s0_addr_msb_err0_o), .s0_thread_id_o(new_s0_thread_id0_o),
    .s1_available_i, .s1_addr_load_o(new_s1_addr_load0_o), .s1_tlb_wait_i, .s1_tlb_fail_i,
    .s2_addr_i, .l2_req_ready_coop_i, .l2_req_valid_coop_o(new_l2_req_valid_coop0_o),
    .l2_req_ready_no_coop_i, .l2_req_valid_no_coop_o(new_l2_req_valid_no_coop0_o), .l2_req_o(l2_req_new_0),
    .l2_resp_valid_i, .l2_resp_i(l2_resp_new), .load_way_o(new_load_way0_o), .load_addr_o(new_load_addr0_o),
    .load_data_o(new_load_data0_bits), .load_valid_o(new_load_valid0_o), .load_valid_pre_o(new_load_valid_pre0_o),
    .load_ready_i, .load_capture_o(new_load_capture0_o), .tfma_enabled_i, .tenb_tstore_busy_i,
    .tenb_credit_i, .tenb_credit_entry_i, .tenb_written_i, .tenb_ready_o(new_tenb_ready0_o),
    .tenb_flush_o(new_tenb_flush0_o), .vm_status_i(vm_status_new), .vm_status_tl_o(new_vm_status_tl0_o),
    .err_flags_o(new_err_flags0_o), .tl_start_o(new_tl_start0_o), .tl_op_o(new_tl_op0_o), .bus_err_o(new_bus_err0_o),
    .csr_debug_signals_o(new_csr_debug0_o), .sm_match_debug_signals_o(new_sm_match0_o),
    .sm_filter_debug_signals_o(new_sm_filter0_bits), .sm_data_debug_signals_o(new_sm_data0_bits)
  );

  dcache_tensor_load #(.MODULE_IDX(0)) u_orig_0 (
    .clock(clk_i), .reset(reset_orig), .shire_min_id(shire_min_id_i), .chicken_bit_dcache(chicken_bit_dcache_i),
    .busy(orig_busy0_o), .id_complete(orig_id_complete0_o), .tensor_ctrl(tensor_ctrl_orig),
    .s0_available(s0_available_i), .s0_addr_load(orig_s0_addr_load0_o), .s0_addr(orig_s0_addr0_o),
    .s0_addr_msb_err(orig_s0_addr_msb_err0_o), .s0_thread_id(orig_s0_thread_id0_o),
    .s1_available(s1_available_i), .s1_addr_load(orig_s1_addr_load0_o), .s1_tlb_wait(s1_tlb_wait_i),
    .s1_tlb_fail(s1_tlb_fail_i), .s2_addr(s2_addr_i), .l2_req_ready_coop(l2_req_ready_coop_i),
    .l2_req_valid_coop(orig_l2_req_valid_coop0_o), .l2_req_ready_no_coop(l2_req_ready_no_coop_i),
    .l2_req_valid_no_coop(orig_l2_req_valid_no_coop0_o), .l2_req(l2_req_orig_0), .l2_resp_valid(l2_resp_valid_i),
    .l2_resp(l2_resp_orig), .load_way(orig_load_way0_o), .load_addr(orig_load_addr0_o),
    .load_data(orig_load_data0_bits), .load_valid(orig_load_valid0_o), .load_valid_pre(orig_load_valid_pre0_o),
    .load_ready(load_ready_i), .load_capture(orig_load_capture0_o), .tfma_enabled(tfma_enabled_i),
    .tenb_tstore_busy(tenb_tstore_busy_i), .tenb_credit(tenb_credit_i), .tenb_credit_entry(tenb_credit_entry_i),
    .tenb_written(tenb_written_i), .tenb_ready(orig_tenb_ready0_o), .tenb_flush(orig_tenb_flush0_o),
    .vm_status(vm_status_orig), .vm_status_tl(orig_vm_status_tl0_o), .err_flags(orig_err_flags0_o),
    .tl_start(orig_tl_start0_o), .tl_op(orig_tl_op0_o), .bus_err(orig_bus_err0_o),
    .csr_debug_signals(orig_csr_debug0_o), .sm_match_debug_signals(orig_sm_match0_o),
    .sm_filter_debug_signals(orig_sm_filter0_bits), .sm_data_debug_signals(orig_sm_data0_bits)
  );

  minion_dcache_tensor_load #(.ModuleIdx(1)) u_new_1 (
    .clk_i, .rst_ni, .shire_min_id_i, .chicken_bit_dcache_i,
    .busy_o(new_busy1_o), .id_complete_o(new_id_complete1_o), .tensor_ctrl_i(tensor_ctrl_new),
    .s0_available_i, .s0_addr_load_o(new_s0_addr_load1_o), .s0_addr_o(new_s0_addr1_o),
    .s0_addr_msb_err_o(new_s0_addr_msb_err1_o), .s0_thread_id_o(new_s0_thread_id1_o),
    .s1_available_i, .s1_addr_load_o(new_s1_addr_load1_o), .s1_tlb_wait_i, .s1_tlb_fail_i,
    .s2_addr_i, .l2_req_ready_coop_i, .l2_req_valid_coop_o(new_l2_req_valid_coop1_o),
    .l2_req_ready_no_coop_i, .l2_req_valid_no_coop_o(new_l2_req_valid_no_coop1_o), .l2_req_o(l2_req_new_1),
    .l2_resp_valid_i, .l2_resp_i(l2_resp_new), .load_way_o(new_load_way1_o), .load_addr_o(new_load_addr1_o),
    .load_data_o(new_load_data1_bits), .load_valid_o(new_load_valid1_o), .load_valid_pre_o(new_load_valid_pre1_o),
    .load_ready_i, .load_capture_o(new_load_capture1_o), .tfma_enabled_i, .tenb_tstore_busy_i,
    .tenb_credit_i, .tenb_credit_entry_i, .tenb_written_i, .tenb_ready_o(new_tenb_ready1_o),
    .tenb_flush_o(new_tenb_flush1_o), .vm_status_i(vm_status_new), .vm_status_tl_o(new_vm_status_tl1_o),
    .err_flags_o(new_err_flags1_o), .tl_start_o(new_tl_start1_o), .tl_op_o(new_tl_op1_o), .bus_err_o(new_bus_err1_o),
    .csr_debug_signals_o(new_csr_debug1_o), .sm_match_debug_signals_o(new_sm_match1_o),
    .sm_filter_debug_signals_o(new_sm_filter1_bits), .sm_data_debug_signals_o(new_sm_data1_bits)
  );

  dcache_tensor_load #(.MODULE_IDX(1)) u_orig_1 (
    .clock(clk_i), .reset(reset_orig), .shire_min_id(shire_min_id_i), .chicken_bit_dcache(chicken_bit_dcache_i),
    .busy(orig_busy1_o), .id_complete(orig_id_complete1_o), .tensor_ctrl(tensor_ctrl_orig),
    .s0_available(s0_available_i), .s0_addr_load(orig_s0_addr_load1_o), .s0_addr(orig_s0_addr1_o),
    .s0_addr_msb_err(orig_s0_addr_msb_err1_o), .s0_thread_id(orig_s0_thread_id1_o),
    .s1_available(s1_available_i), .s1_addr_load(orig_s1_addr_load1_o), .s1_tlb_wait(s1_tlb_wait_i),
    .s1_tlb_fail(s1_tlb_fail_i), .s2_addr(s2_addr_i), .l2_req_ready_coop(l2_req_ready_coop_i),
    .l2_req_valid_coop(orig_l2_req_valid_coop1_o), .l2_req_ready_no_coop(l2_req_ready_no_coop_i),
    .l2_req_valid_no_coop(orig_l2_req_valid_no_coop1_o), .l2_req(l2_req_orig_1), .l2_resp_valid(l2_resp_valid_i),
    .l2_resp(l2_resp_orig), .load_way(orig_load_way1_o), .load_addr(orig_load_addr1_o),
    .load_data(orig_load_data1_bits), .load_valid(orig_load_valid1_o), .load_valid_pre(orig_load_valid_pre1_o),
    .load_ready(load_ready_i), .load_capture(orig_load_capture1_o), .tfma_enabled(tfma_enabled_i),
    .tenb_tstore_busy(tenb_tstore_busy_i), .tenb_credit(tenb_credit_i), .tenb_credit_entry(tenb_credit_entry_i),
    .tenb_written(tenb_written_i), .tenb_ready(orig_tenb_ready1_o), .tenb_flush(orig_tenb_flush1_o),
    .vm_status(vm_status_orig), .vm_status_tl(orig_vm_status_tl1_o), .err_flags(orig_err_flags1_o),
    .tl_start(orig_tl_start1_o), .tl_op(orig_tl_op1_o), .bus_err(orig_bus_err1_o),
    .csr_debug_signals(orig_csr_debug1_o), .sm_match_debug_signals(orig_sm_match1_o),
    .sm_filter_debug_signals(orig_sm_filter1_bits), .sm_data_debug_signals(orig_sm_data1_bits)
  );

  assign orig_l2_req_id0_o = l2_req_orig_0.id;
  assign new_l2_req_id0_o = l2_req_new_0.id;
  assign orig_l2_req_opcode0_o = l2_req_orig_0.opcode;
  assign new_l2_req_opcode0_o = l2_req_new_0.opcode;
  assign orig_l2_req_addr0_o = l2_req_orig_0.address;
  assign new_l2_req_addr0_o = l2_req_new_0.address;
  assign orig_l2_req_data0_o = l2_req_orig_0.data;
  assign new_l2_req_data0_o = l2_req_new_0.data;
  assign orig_l2_req_qwen0_o = l2_req_orig_0.qwen;
  assign new_l2_req_qwen0_o = l2_req_new_0.qwen;
  assign orig_ctrl_state0_dbg_o = u_orig_0.ctrl_state;
  assign new_ctrl_state0_dbg_o = u_new_0.ctrl_state_q;
  assign orig_enabled0_dbg_o = u_orig_0.enabled;
  assign new_enabled0_dbg_o = u_new_0.enabled_q;
  assign orig_count0_dbg_o = u_orig_0.count;
  assign new_count0_dbg_o = u_new_0.count_q;
  assign orig_shared_count0_dbg_o = u_orig_0.shared_count;
  assign new_shared_count0_dbg_o = u_new_0.shared_count_q;
  assign orig_l2_busy0_dbg_o = u_orig_0.l2_interface_busy;
  assign new_l2_busy0_dbg_o = u_new_0.l2_interface_busy;
  assign orig_shared_load_idx0_dbg_o = u_orig_0.shared_load_idx;
  assign new_shared_load_idx0_dbg_o = u_new_0.shared_load_idx_q;
  assign orig_trans_split0_dbg_o = u_orig_0.trans_split_count;
  assign new_trans_split0_dbg_o = u_new_0.trans_split_count_q;
  assign orig_shared_l2_counter0_dbg_o = u_orig_0.shared_l2_counter;
  assign new_shared_l2_counter0_dbg_o = u_new_0.shared_l2_counter_q;
  assign orig_shared_l2_store0_dbg_o = u_orig_0.shared_l2_store;
  assign new_shared_l2_store0_dbg_o = u_new_0.shared_l2_store;
  assign orig_transform0_dbg_o = u_orig_0.tensor_ctrl_transform;
  assign new_transform0_dbg_o = u_new_0.tensor_ctrl_transform_q;
  assign orig_trans_flags0_dbg_o = {u_orig_0.trans_none, u_orig_0.trans_int8, u_orig_0.trans_int16,
                                    u_orig_0.trans_trp8, u_orig_0.trans_trp16, u_orig_0.trans_trp32};
  assign new_trans_flags0_dbg_o = {u_new_0.trans_none, u_new_0.trans_int8, u_new_0.trans_int16,
                                   u_new_0.trans_trp8, u_new_0.trans_trp16, u_new_0.trans_trp32};
  assign orig_tmp_data00_dbg_o = u_orig_0.tmp_data[0];
  assign new_tmp_data00_dbg_o = u_new_0.tmp_data_q[0];
  assign orig_l2_resp_for_me0_dbg_o = u_orig_0.l2_resp_is_for_me;
  assign new_l2_resp_for_me0_dbg_o = u_new_0.l2_resp_is_for_me;
  assign orig_l2_req_id1_o = l2_req_orig_1.id;
  assign new_l2_req_id1_o = l2_req_new_1.id;
  assign orig_l2_req_opcode1_o = l2_req_orig_1.opcode;
  assign new_l2_req_opcode1_o = l2_req_new_1.opcode;
  assign orig_l2_req_addr1_o = l2_req_orig_1.address;
  assign new_l2_req_addr1_o = l2_req_new_1.address;
  assign orig_l2_req_data1_o = l2_req_orig_1.data;
  assign new_l2_req_data1_o = l2_req_new_1.data;
  assign orig_l2_req_qwen1_o = l2_req_orig_1.qwen;
  assign new_l2_req_qwen1_o = l2_req_new_1.qwen;
  assign orig_ctrl_state1_dbg_o = u_orig_1.ctrl_state;
  assign new_ctrl_state1_dbg_o = u_new_1.ctrl_state_q;
  assign orig_enabled1_dbg_o = u_orig_1.enabled;
  assign new_enabled1_dbg_o = u_new_1.enabled_q;
  assign orig_count1_dbg_o = u_orig_1.count;
  assign new_count1_dbg_o = u_new_1.count_q;
  assign orig_load_way_sel1_dbg_o = u_orig_1.request_control_load_way;
  assign new_load_way_sel1_dbg_o = u_new_1.request_control_load_way;
  assign orig_tenb_avail1_dbg_o = u_orig_1.tenb_avail_entry;
  assign new_tenb_avail1_dbg_o = u_new_1.tenb_avail_entry_q;
  assign orig_tenb_rcv_ok1_dbg_o = u_orig_1.tenb_rcv_ok;
  assign new_tenb_rcv_ok1_dbg_o = u_new_1.tenb_rcv_ok;
  assign orig_tenb_credits_ok1_dbg_o = u_orig_1.tenb_credits_ok;
  assign new_tenb_credits_ok1_dbg_o = u_new_1.tenb_credits_ok;
  assign orig_tenb_credits1_dbg_o = u_orig_1.tenb_credits;
  assign new_tenb_credits1_dbg_o = u_new_1.tenb_credits_q;
  assign orig_cond31_dbg_o = u_orig_1.tenb_no_extra_credits_cond3;
  assign new_cond31_dbg_o = u_new_1.tenb_no_extra_credits_cond3_q;
  assign orig_tenb_count1_dbg_o = u_orig_1.tenb_count;
  assign new_tenb_count1_dbg_o = u_new_1.tenb_count_q;
  assign orig_s2_req_valid1_dbg_o = u_orig_1.s2_req_valid;
  assign new_s2_req_valid1_dbg_o = u_new_1.s2_req_valid_q;
  assign orig_s2_req_ready1_dbg_o = u_orig_1.s2_req_ready;
  assign new_s2_req_ready1_dbg_o = u_new_1.s2_req_ready;
  assign orig_sel_t1_dbg_o = u_orig_1.sel_t;
  assign new_sel_t1_dbg_o = u_new_1.sel_t;
  assign orig_sel_set1_dbg_o = u_orig_1.tl_load_set[u_orig_1.sel_t];
  assign new_sel_set1_dbg_o = u_new_1.tl_load_set_q[u_new_1.sel_t];
  assign orig_sel_idx1_dbg_o = u_orig_1.tl_load_idx[u_orig_1.sel_t];
  assign new_sel_idx1_dbg_o = u_new_1.tl_load_idx_q[u_new_1.sel_t];
  assign orig_tfma_started1_dbg_o = u_orig_1.tfma_started;
  assign new_tfma_started1_dbg_o = u_new_1.tfma_started_q;
  assign orig_tfma_active_start1_dbg_o = u_orig_1.tfma_active_start;
  assign new_tfma_active_start1_dbg_o = u_new_1.tfma_active_start_q;

endmodule
