// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module minion_dcache_tensor_load
  import dft_pkg::*;
  import minion_pkg::*;
  import minion_dcache_pkg::*;
#(
  parameter int unsigned ModuleIdx = 0
) (
  input  logic                         clk_i,
  input  logic                         rst_ni,
  input  logic [MinPerSBits-1:0]       shire_min_id_i,
  /* verilator lint_off UNUSEDSIGNAL */  // Original tensor-load uses only selected control/response fields from these packed interfaces.
  input  logic                         chicken_bit_dcache_i,
  output logic                         busy_o,
  output logic [1:0]                   id_complete_o,
  input  core_dcache_ctrl              tensor_ctrl_i,
  input  logic                         s0_available_i,
  output logic                         s0_addr_load_o,
  output logic [minion_dcache_pkg::VaSize-1:0]            s0_addr_o,
  output logic                         s0_addr_msb_err_o,
  output logic                         s0_thread_id_o,
  input  logic                         s1_available_i,
  output logic                         s1_addr_load_o,
  input  logic                         s1_tlb_wait_i,
  input  logic                         s1_tlb_fail_i,
  input  logic [minion_dcache_pkg::PaSize-1:0]            s2_addr_i,
  input  logic                         l2_req_ready_coop_i,
  output logic                         l2_req_valid_coop_o,
  input  logic                         l2_req_ready_no_coop_i,
  output logic                         l2_req_valid_no_coop_o,
  output et_link_minion_miss_req_info_t l2_req_o,
  input  logic                         l2_resp_valid_i,
  input  et_link_minion_rsp_info_t     l2_resp_i,
  /* verilator lint_on UNUSEDSIGNAL */
  output logic [DcacheWayIdxWidth-1:0] load_way_o,
  output logic [DcacheLramAddrWidth-1:0] load_addr_o,
  output logic [minion_dcache_pkg::CoreL2BlockExtSize-1:0] load_data_o,
  output logic [DcacheLramNumBanks-1:0] load_valid_o,
  output logic                         load_valid_pre_o,
  input  logic                         load_ready_i,
  output logic                         load_capture_o,
  input  logic                         tfma_enabled_i,
  input  logic                         tenb_tstore_busy_i,
  input  logic                         tenb_credit_i,
  input  logic [DcacheTenbBits-1:0]    tenb_credit_entry_i,
  input  logic                         tenb_written_i,
  output logic                         tenb_ready_o,
  output logic                         tenb_flush_o,
  input  minion_dcache_pkg::vm_status_t [NrThreads-1:0] vm_status_i,
  output minion_dcache_pkg::vm_status_t vm_status_tl_o,
  output logic [DcacheTlErrorBits-1:0] err_flags_o,
  output logic                         tl_start_o,
  output logic                         tl_op_o,
  output logic [NrThreads-1:0]         bus_err_o,
  output logic [DcacheDbgCsrTlSize-1:0] csr_debug_signals_o,
  output logic [DcacheDbgSmMatchBits-1:0] sm_match_debug_signals_o,
  output logic [DcacheDbgSmFilterBits-1:0] sm_filter_debug_signals_o,
  output logic [DcacheDbgSmDataBits-1:0] sm_data_debug_signals_o
);

  localparam int unsigned TransSplitCountBits = 3;
  localparam int unsigned TransSplitCycles = 8;
  localparam int unsigned TransSplitTermCount = TransSplitCycles - 1;
  localparam int unsigned DcacheTlTmpDataBlocks = 16;
  localparam int unsigned DcacheTlIntDataBlocks = 8;
  localparam int unsigned DcacheTlTrpDataBlocks = 16;

  typedef enum logic [2:0] {
    MlCtrlIdle,
    MlCtrlWaitStart,
    MlCtrlNoTReq,
    MlCtrlIntTransFlush,
    MlCtrlIntReq,
    MlCtrlIntInt,
    MlCtrlTraReq,
    MlCtrlTraTra
  } dcache_tl_ctrl_state_e;

  localparam int unsigned DcacheTlSequenceEntries = 1 << (ShireCoopSeqIdSize + 1);

  typedef enum logic [1:0] {
    MlL2iWaitReq = 2'b00,
    MlL2iL2Req   = 2'b01,
    MlL2iWaitAck = 2'b10,
    MlL2iL2Seq   = 2'b11
  } dcache_tl_l2i_state_e;

  logic clkgt;
  logic clkgt_en;
  dft_t dft;

  logic tensor_ctrl_start, tensor_ctrl_restart;
  logic start_pulse_pre_q, start_pulse;
  logic request_control_start, request_control_store_addr;
  logic [minion_dcache_pkg::VaSize-1:0] request_control_stride, request_control_address;
  logic request_control_use_tmask, request_control_use_coop;
  logic [TensorMaskBits-1:0] request_control_tmask_bits;
  logic [DcacheTensorCtrlNlinesBits-1:0] request_control_num_lines;
  logic [minion_dcache_pkg::DcacheTensorCtrlDestBits-1:0] request_control_scp_dest;
  logic request_control_ending, request_control_ending_l2, request_control_flush, request_control_flush_done_int;
  logic request_control_flush_done_cond;
  logic [DcacheSetIdxWidth-1:0] request_control_clear_set;
  logic [DcacheWayIdxWidth-1:0] request_control_clear_way, request_control_load_way;
  logic [DcacheLramAddrWidth-1:0] request_control_load_addr;
  logic request_control_l2_group_id, request_control_l2_op_idx, request_control_thread_id;

  logic save_tmask_pending_q, save_tmask_pending_d, save_tmask_bits;
  dcache_tl_ctrl_state_e ctrl_state_q, ctrl_state_d;
  logic ctrl_state_changed_q;

  logic [VaCaBits-1:0] tensor_load_stride_q, tensor_load_stride_d;
  logic trans_none, trans_int8, trans_int16, trans_trp8, trans_trp16, trans_trp32;
  tensorload_offset trans_offset;
  logic trans_offset_valid, trans_size_valid, trans_size8, trans_size16;
  logic [DcacheTensorCtrlNlinesBits-1:0] trans_nlines;
  logic [minion_dcache_pkg::VaSize-1:0] trans_stride;
  logic [minion_dcache_pkg::DcacheTensorCtrlDestBits-1:0] trans_scp_dest;
  logic trans_use_tmask, trans_use_coop;
  logic [TensorMaskBits-1:0] trans_tmask_bits_q;
  minion_dcache_pkg::vm_status_t vm_status_tl_q;

  logic [DcacheTensorCtrlNlinesBits-1:0] shared_count_q, shared_count_d;
  logic [TmaskCountBits-1:0] shared_tmask_idx;
  logic [minion_dcache_pkg::DcacheTensorCtrlDestBits-1:0] shared_dest_q, shared_dest_d;
  logic shared_load_idx_q, shared_load_idx_d;
  logic [DcacheSetIdxWidth-1:0] shared_clear_set;
  logic [DcacheWayIdxWidth-1:0] shared_clear_way, shared_load_way;
  logic [DcacheLramAddrWidth-1:0] shared_load_addr;
  logic [minion_dcache_pkg::CoreL2BlockExtSize-1:0] shared_load_data;
  logic [DcacheLramNumBanks-1:0] interleave_load_valid;
  logic [DcacheLramNumBanks-1:0] transpose_load_valid, transpose_load_valid_mask;
  logic transpose_load_valid_single;
  logic [TransSplitCountBits-1:0] trans_split_count_q, trans_split_count_d;
  logic drain_load_capture_q, drain_load_capture_d;
  logic l2_resp_is_for_me, l2_req_ready, l2_req_valid;
  /* verilator lint_off UNOPTFLAT */  // Tensor-load control enable sits in the preserved dcache/integer-writeback feedback path and is only cyclic after flattening.
  logic ctrl_en;
  /* verilator lint_on UNOPTFLAT */
  logic interleave_clear_valid_q, interleave_clear_valid_d, interleave_clear_valid_cken;
  logic err_unlock_flag, err_transf_flag_q, err_transf_set;
  logic err_offset_flag_q, err_offset_set;
  logic err_mem_q, err_mem_d, err_mem_sticky_q, err_mem_sticky_d;
  /* verilator lint_off UNUSEDSIGNAL */  // ModuleIdx!=0 intentionally elides the tmp-data RF path, leaving the shared write-capture staging unused in that specialization.
  logic rf_write_en_prev;
  logic [DcacheTlTmpDataBlocks-1:0][DcacheLramDataSize-1:0] tmp_data_q, tmp_data_d;
  /* verilator lint_on UNUSEDSIGNAL */
  logic [$bits(tensorcoop_control)-1:0] tensor_coop_data_q, tensor_coop_data_d, tensor_coop_data_sequenced;
  logic id_running_q, id_running_d, thread_id_q, thread_id_d;
  logic [$clog2(ShireCoopMinMaskSize)-1:0] minion_id;
  logic [$clog2(ShireCoopNeighMaskSize)-1:0] neigh_id;
  logic [ShireCoopMinMaskSize-1:0] self_minion_mask;
  logic [$bits(tensorcoop_control)-1:0] tensor_coop_data_bits;
  logic [$bits(tensorcoop_control)-1:0] tensor_coop_data_seq_bits;
  logic [DcacheDbgSmMatchBits-1:0] sm_match_debug_signals_d;
  logic [DcacheDbgSmFilterBits-1:0] sm_filter_debug_signals_d;
  logic [DcacheDbgSmDataBits-1:0] sm_data_debug_signals_d;
  logic [VpuTenbSize/2-1:0] tenb_avail_entry_q, tenb_avail_entry_d;
  logic tenb_avail_all_q, tenb_avail_all_d, tfma_started_q, tfma_started_d, tfma_active_start_q, tfma_active_start_d;
  logic l2_interface_busy, l2_interface_busy_prev_q;

  tensorload_transform_e tensor_ctrl_transform_q, tensor_ctrl_transform_d;
  tensorload_offset tensor_ctrl_offset_q, tensor_ctrl_offset_d;
  logic tensor_ctrl_use_tmask_q, tensor_ctrl_use_tmask_d;
  logic tensor_ctrl_use_coop_q, tensor_ctrl_use_coop_d;
  logic [DcacheTensorCtrlNlinesBits-1:0] tensor_ctrl_num_lines_q, tensor_ctrl_num_lines_d;
  logic tensor_ctrl_tenb;
  logic [minion_dcache_pkg::DcacheTensorCtrlDestBits-1:0] tensor_ctrl_scp_dest_q, tensor_ctrl_scp_dest_d;
  logic tenb_no_extra_credits_cond1_q, tenb_no_extra_credits_cond1_d;
  logic tenb_no_extra_credits_cond3_q, tenb_no_extra_credits_cond3_d;

  logic [minion_dcache_pkg::DcacheTensorCtrlDestBits-1:0] l1_scp_dest;
  logic [minion_dcache_pkg::DcacheTensorCtrlDestBits:0] l1_scp_dest_ext;
  logic ctrl_start;
  logic ctrl_start_tmask_ready;
  logic ctrl_start_tmask_wait;
  logic ctrl_start_previous_ready;
  logic ctrl_start_previous_wait;
  logic ctrl_wait_tmask_ready;
  logic ctrl_wait_previous_ready;
  logic [minion_dcache_pkg::VaSize-1:0] s0_addr_next;
  logic s0_addr_msb_err_next;
  logic [DcacheTensorCtrlNlinesBits-1:0] count_q, count_d, tenb_count_q, tenb_count_d;
  logic [DcacheTenbBits-1:0] tenb_idx, tenb_idx_last_q, tenb_idx_last_d;
  logic tenb_idx_save;
  logic [TmaskCountBits-1:0] tmask_idx_q, tmask_idx_d;
  logic [VpuTenbCreditBits-1:0] tenb_credits_q, tenb_credits_d;
  logic [VpuTenbRcvEntryBits-1:0] tenb_rcv_entry_q, tenb_rcv_entry_d;
  logic tenb_credits_ok, tenb_rcv_ok;
  logic [DcacheTenbBits-1:0] tenb_written_entry;
  logic s1_addr_load_next, s0_addr_load_skip_mask;
  logic enabled_q, enabled_d, enabled_req, addr_up, s2_req_valid_q, s2_req_valid_d, s2_req_ready;
  logic waiting_tlb_q, waiting_tlb_d, l2_seq_wait;
  logic s0_available_x, s0_addr_load_x, s1_available_x, s1_addr_load_x_q, s1_tlb_wait_x, s1_tlb_fail_x;
  /* verilator lint_off UNUSEDSIGNAL */  // Translation keeps the full physical address bus; only the cache-line-aligned slice is consumed here.
  logic [minion_dcache_pkg::PaSize-1:0] s2_addr_x;
  /* verilator lint_on UNUSEDSIGNAL */
  logic shadow_tlb_translate;
  logic tenb_flush_int;
  logic update_l2_operation_id;
  logic [1:0] l2_operates_in_id;
  logic bus_err_local;
  dcache_tl_l2i_state_e l2i_state_q[DcacheTlL2Transfers];
  dcache_tl_l2i_state_e l2i_state_d[DcacheTlL2Transfers];
  logic [DcacheTlL2Transfers-1:0] l2i_state_up;
  et_link_core_miss_id_e my_miss_id[DcacheTlL2Transfers];
  logic [DcacheTlL2Transfers-1:0] waiting_for_req;
  logic [DcacheTlL2Transfers-1:0] requesting_to_l2;
  logic [DcacheTlL2Transfers-1:0] accepted_to_l2;
  logic [DcacheTlL2Transfers-1:0] sequencing_to_l2;
  logic req_up;
  logic [minion_dcache_pkg::PaSize-7:0] req_addr_cline_q, req_addr_cline_d;
  logic [DcacheTlL2Transfers-1:0][DcacheWayIdxWidth-1:0] tl_load_way_q, tl_load_way_d;
  logic [DcacheTlL2Transfers-1:0][DcacheSetIdxWidth-1:0] tl_load_set_q, tl_load_set_d;
  logic [DcacheTlL2Transfers-1:0] new_l2_req;
  logic [DcacheTlL2Transfers-1:0] new_data_chunk;
  logic [DcacheTlL2Transfers-1:0] received_error;
  logic [DcacheTlL2Transfers-1:0] received_answer;
  logic [DcacheTlL2Transfers-1:0] l2i_group_q, l2i_group_d;
  logic [DcacheTlL2Transfers-1:0] l2i_op_idx_q, l2i_op_idx_d;
  logic [DcacheTlL2Transfers-1:0] l2i_op_is_coop_q, l2i_op_is_coop_d;
  logic [1:0] l2i_op_in_idx;
  logic l2i_current_idx_q, l2i_current_idx_d, l2i_current_idx_up;
  logic [DcacheTlL2Transfers-1:0][ShireCoopSeqIdSize:0] l2i_seq_saved_q, l2i_seq_saved_d;
  logic [DcacheTlSequenceEntries-1:0] sequence_data_q, sequence_data_d;
  logic [ShireCoopSeqIdSize-1:0] sequence_count_q, sequence_count_d;
  logic sequence_usable;
  logic l2i_seq_data_up, l2i_seq_cnt_up;
  logic l2i_seq_idx_q, l2i_seq_idx_d;
  logic [DcacheTlL2Transfers-1:0] tl_load_idx_q, tl_load_idx_d;
  logic [DcacheTlL2TransfersSelBits-1:0] l2_req_idx, sel_t;
  logic [ShireCoopIdSize-1:0] tensor_coop_id_curr, tensor_coop_id_prev_q, tensor_coop_id_prev_d;
  logic request_different_coop_id, tensor_coop_id_prev_up;
  logic shared_l2_use_p2;
  logic [DcacheTlTmpDataBlocks-1:0] shared_l2_store;
  logic [DcacheTlL2Transfers-1:0][3:0] l2i_stg_addr_q;
  logic [DcacheTlL2TransfersSelBits-1:0] shared_stg_idx;
  logic [3:0] shared_l2_counter_q;
  logic shared_l2_count;
  logic [3:0][DcacheLramDataSize-1:0] shared_l2_sel_data;
  /* verilator lint_off UNUSEDSIGNAL */  // Buffer index arithmetic keeps the original 7-bit intermediate even though only the low nibble indexes the tmp array.
  logic [6:0] idx_buf;
  /* verilator lint_on UNUSEDSIGNAL */
  logic [3:0] idx_bufl;
  logic [2:0] idx_byte;
  logic [$bits(tensorcoop_control)-1:0] own_neigh_mask_bits, coop_neigh_mask_bits, coop_id_bits, self_minion_mask_bits, coop_minion_mask_bits;
  localparam int unsigned ShireCoopSeqIdStart = ShireCoopMinMaskSize;
  localparam int unsigned ShireCoopIdStart = ShireCoopSeqIdStart + ShireCoopSeqIdSize;
  localparam int unsigned ShireCoopNeighMaskStart = ShireCoopIdStart + ShireCoopIdSize;
  localparam logic [minion_dcache_pkg::DcacheTensorCtrlDestBits:0] DcacheTlScpMaxIdxVal = 7'd48;
  localparam logic [VpuTenbCreditBits-1:0] DcacheTlTenbLookAheadVal = 3'd2;
  localparam logic [DcacheTensorCtrlNlinesBits-1:0] VpuTenbHalfVal = 4'd4;
  assign dft = '0;
  assign clkgt_en = tensor_ctrl_start | busy_o | ~(&id_complete_o) | ~rst_ni | (|err_flags_o);
  prim_clk_gate u_clk_gate (
    .clk_i (clk_i),
    .en_i  (clkgt_en | chicken_bit_dcache_i),
    .dft_i (dft),
    .clk_o (clkgt)
  );
  assign l2_req_ready = request_control_use_coop ? l2_req_ready_coop_i : l2_req_ready_no_coop_i;
  assign tensor_ctrl_start = tensor_ctrl_i.tensorload_start &&
                             (tensor_ctrl_i.tensorload_ctrl.tl_sel == (ModuleIdx != 0));
  assign tensor_ctrl_restart = tensor_ctrl_i.tensorload_restart_b && (ModuleIdx == 1);
  assign minion_id = shire_min_id_i[$clog2(ShireCoopMinMaskSize)-1:0];
  assign neigh_id = shire_min_id_i[$clog2(ShireCoopMinMaskSize) +: $clog2(ShireCoopNeighMaskSize)];
  always_comb begin
    self_minion_mask = '0;
    self_minion_mask[minion_id] = 1'b1;
  end

  always_comb begin
    own_neigh_mask_bits = '0;
    own_neigh_mask_bits[ShireCoopNeighMaskStart +: ShireCoopNeighMaskSize] =
        ({{(ShireCoopNeighMaskSize-1){1'b0}}, 1'b1} << neigh_id);
    coop_neigh_mask_bits = '0;
    coop_neigh_mask_bits[ShireCoopNeighMaskStart +: ShireCoopNeighMaskSize] = tensor_ctrl_i.tensorcoop_ctrl.neigh_mask;
    coop_id_bits = '0;
    coop_id_bits[ShireCoopIdStart +: ShireCoopIdSize] = tensor_ctrl_i.tensorcoop_ctrl.id;
    self_minion_mask_bits = '0;
    self_minion_mask_bits[ShireCoopMinMaskSize-1:0] = self_minion_mask;
    coop_minion_mask_bits = '0;
    coop_minion_mask_bits[ShireCoopMinMaskSize-1:0] = tensor_ctrl_i.tensorcoop_ctrl.minion_mask;
    tensor_coop_data_d = tensor_coop_data_q;
    if (tensor_ctrl_start) begin
      tensor_coop_data_d = '0;
      tensor_coop_data_d = own_neigh_mask_bits |
                           coop_neigh_mask_bits |
                           coop_id_bits |
                           self_minion_mask_bits |
                           coop_minion_mask_bits;
    end
  end

  always_comb begin
    tensor_ctrl_use_tmask_d = tensor_ctrl_use_tmask_q;
    tensor_ctrl_use_coop_d = tensor_ctrl_use_coop_q;
    tensor_ctrl_num_lines_d = tensor_ctrl_num_lines_q;
    tensor_ctrl_scp_dest_d = tensor_ctrl_scp_dest_q;
    tensor_load_stride_d = tensor_load_stride_q;
    id_running_d = id_running_q;
    thread_id_d = thread_id_q;
    tensor_ctrl_offset_d = tensor_ctrl_offset_q;
    tensor_ctrl_transform_d = tensor_ctrl_transform_q;
    tenb_no_extra_credits_cond1_d = tenb_no_extra_credits_cond1_q;
    tenb_no_extra_credits_cond3_d = tenb_no_extra_credits_cond3_q;
    if (tensor_ctrl_start) begin
      if (ModuleIdx == 0) begin
        tensor_ctrl_use_tmask_d = tensor_ctrl_i.tensorload_ctrl.use_tmask;
        tensor_ctrl_offset_d = tensor_ctrl_i.tensorload_ctrl.offset;
        tensor_ctrl_transform_d = tensor_ctrl_i.tensorload_ctrl.transform;
      end else begin
        tensor_ctrl_offset_d = tensorload_offset_0;
        tensor_ctrl_transform_d = TensorTransNone;
        tenb_no_extra_credits_cond1_d = tensor_ctrl_i.tensorload_stride[1];
        tenb_no_extra_credits_cond3_d = tensor_ctrl_i.tensorload_stride[2];
      end
      tensor_ctrl_use_coop_d = tensor_ctrl_i.tensorload_ctrl.use_coop;
      tensor_ctrl_num_lines_d = tensor_ctrl_i.tensorload_ctrl.num_lines;
      tensor_ctrl_scp_dest_d = tensor_ctrl_i.tensorload_ctrl.scp_dest;
      tensor_load_stride_d = tensor_ctrl_i.tensorload_stride[minion_dcache_pkg::VaSize-1:6];
      id_running_d = tensor_ctrl_i.tensorload_stride[0];
      thread_id_d = tensor_ctrl_i.thread_id;
    end
  end

  assign tensor_ctrl_tenb = (ModuleIdx == 1);
  assign start_pulse = start_pulse_pre_q & ~tensor_ctrl_restart;
  assign tl_start_o = start_pulse;
  assign tl_op_o = l2_req_valid & l2_req_ready;
  assign ctrl_en = (ctrl_state_q != MlCtrlIdle) | start_pulse | tensor_ctrl_restart;
  assign ctrl_start = start_pulse & trans_size_valid & trans_offset_valid;
  assign ctrl_start_tmask_ready = ctrl_start & (tensor_ctrl_i.tensorload_tmask_ready | !tensor_ctrl_use_tmask_q);
  assign ctrl_start_tmask_wait = ctrl_start & !tensor_ctrl_i.tensorload_tmask_ready & tensor_ctrl_use_tmask_q;
  assign ctrl_start_previous_ready = ctrl_start & (trans_none | !l2_interface_busy | (ModuleIdx == 1));
  assign ctrl_wait_previous_ready = (ModuleIdx == 0) ? (trans_none | !l2_interface_busy)
                                                     : ((tenb_avail_all_q && !l2_interface_busy) ||
                                                        !tenb_tstore_busy_i || tfma_enabled_i);
  assign ctrl_start_previous_wait = ctrl_start & !ctrl_wait_previous_ready;
  assign ctrl_wait_tmask_ready = !save_tmask_pending_q;

  assign trans_none = (tensor_ctrl_transform_q == TensorTransNone);
  assign trans_int8 = (tensor_ctrl_transform_q == TensorTransInt8);
  assign trans_int16 = (tensor_ctrl_transform_q == TensorTransInt16);
  assign trans_trp8 = (tensor_ctrl_transform_q == TensorTransTrp8);
  assign trans_trp16 = (tensor_ctrl_transform_q == TensorTransTrp16);
  assign trans_trp32 = (tensor_ctrl_transform_q == TensorTransTrp32);
  assign trans_offset = (trans_int8 | trans_trp8) ? tensor_ctrl_offset_q :
                        ((trans_int16 | trans_trp16) ? tensorload_offset'(tensor_ctrl_offset_q & 2'b10) :
                         tensorload_offset_0);
  assign trans_size8 = trans_int8 | trans_trp8;
  assign trans_size16 = trans_int16 | trans_trp16;
  assign trans_nlines = tensor_ctrl_num_lines_q;
  assign trans_stride = {tensor_load_stride_q, 6'b0};
  assign trans_scp_dest = tensor_ctrl_scp_dest_q;
  assign trans_use_tmask = tensor_ctrl_use_tmask_q;
  assign trans_use_coop = tensor_ctrl_use_coop_q;
  assign shared_tmask_idx = shared_count_q[TmaskCountBits-1:0];
  assign tensor_coop_data_bits = tensor_coop_data_q;

  always_comb begin
    trans_offset_valid = 1'b1;
    trans_size_valid = trans_none | trans_int8 | trans_int16 | trans_trp8 | trans_trp16 | trans_trp32;
  end

  always_comb begin
    ctrl_state_d = ctrl_state_q;
    shared_count_d = (ModuleIdx == 0) ? shared_count_q : '0;
    shared_dest_d = (ModuleIdx == 0) ? shared_dest_q : '0;
    shared_load_idx_d = (ModuleIdx == 0) ? shared_load_idx_q : 1'b0;
    trans_split_count_d = (ModuleIdx == 0) ? trans_split_count_q : '0;
    drain_load_capture_d = (ModuleIdx == 0) ? drain_load_capture_q : 1'b0;

    if ((ctrl_state_q == MlCtrlIdle) && (ctrl_start_tmask_wait || ctrl_start_previous_wait)) begin
      ctrl_state_d = MlCtrlWaitStart;
    end else if (((ctrl_state_q == MlCtrlIdle) && ctrl_start_tmask_ready && ctrl_start_previous_ready) ||
                 ((ctrl_state_q == MlCtrlWaitStart) && ctrl_wait_tmask_ready && ctrl_wait_previous_ready)) begin
      if (trans_none) ctrl_state_d = MlCtrlNoTReq;
      else if ((trans_int8 || trans_int16) && (ModuleIdx == 0)) begin
        ctrl_state_d = MlCtrlIntTransFlush;
        shared_count_d = '0;
      end else if ((trans_trp8 || trans_trp16 || trans_trp32) && (ModuleIdx == 0)) begin
        ctrl_state_d = MlCtrlTraReq;
        trans_split_count_d = '0;
      end
    end

    if ((ctrl_state_q == MlCtrlNoTReq) && request_control_ending) ctrl_state_d = MlCtrlIdle;

    if (ModuleIdx == 0) begin
      if (ctrl_state_q == MlCtrlIntTransFlush) begin
        if (shared_count_q <= trans_nlines) ctrl_state_d = MlCtrlIntReq;
        if (shared_count_q == '0) shared_dest_d = trans_scp_dest;
        else shared_dest_d = shared_dest_q + 1'b1;
        shared_load_idx_d = 1'b0;
      end
      if (ctrl_state_q == MlCtrlIntReq) begin
        if (drain_load_capture_q) begin
          if (!l2_interface_busy) begin
            ctrl_state_d = MlCtrlIdle;
            drain_load_capture_d = 1'b0;
          end
        end else if (request_control_ending_l2) begin
          if (err_mem_sticky_q | err_mem_d) drain_load_capture_d = 1'b1;
          else if (!trans_use_tmask || trans_tmask_bits_q[shared_tmask_idx]) ctrl_state_d = MlCtrlIntInt;
          else if (shared_count_q == trans_nlines) ctrl_state_d = MlCtrlIdle;
          else begin
            ctrl_state_d = MlCtrlIntTransFlush;
            shared_count_d = shared_count_q + 1'b1;
          end
        end
      end
      if ((ctrl_state_q == MlCtrlIntInt) && load_ready_i) begin
        shared_load_idx_d = ~shared_load_idx_q;
        if (shared_load_idx_q) begin
          if (shared_count_q == trans_nlines) ctrl_state_d = MlCtrlIdle;
          else begin
            ctrl_state_d = MlCtrlIntTransFlush;
            shared_count_d = shared_count_q + 1'b1;
          end
        end
      end
      if (ctrl_state_q == MlCtrlTraReq) begin
        if (drain_load_capture_q) begin
          if (!l2_interface_busy) begin
            ctrl_state_d = MlCtrlIdle;
            drain_load_capture_d = 1'b0;
          end
        end else if (request_control_ending_l2) begin
          if (err_mem_sticky_q | err_mem_d) drain_load_capture_d = 1'b1;
          else ctrl_state_d = MlCtrlTraTra;
        end
        shared_count_d = '0;
        shared_dest_d = trans_scp_dest;
        shared_load_idx_d = trans_split_count_q[TransSplitCountBits-1];
      end
      if ((ctrl_state_q == MlCtrlTraTra) && load_ready_i) begin
        shared_count_d = shared_count_q + 1'b1;
        shared_dest_d = shared_dest_q + 1'b1;
        if (shared_count_q == trans_nlines) begin
          if (trans_split_count_q == TransSplitTermCount[TransSplitCountBits-1:0]) ctrl_state_d = MlCtrlIdle;
          else ctrl_state_d = MlCtrlTraReq;
          trans_split_count_d = trans_split_count_q + 1'b1;
        end
      end
    end else begin
      if (tensor_ctrl_restart) ctrl_state_d = MlCtrlIntTransFlush;
      if ((ctrl_state_q == MlCtrlIntTransFlush) && request_control_flush_done_int) ctrl_state_d = MlCtrlIdle;
    end

    if (shared_dest_d >= DcacheTlScpMaxIdxVal[minion_dcache_pkg::DcacheTensorCtrlDestBits-1:0]) begin
      shared_dest_d = shared_dest_d - DcacheTlScpMaxIdxVal[minion_dcache_pkg::DcacheTensorCtrlDestBits-1:0];
    end
  end

  always_comb begin
    err_transf_set = start_pulse & !trans_size_valid;
    err_offset_set = start_pulse & !trans_offset_valid;
    err_mem_sticky_d = (ctrl_state_q == MlCtrlIdle) ? 1'b0 : ((err_mem_d) ? 1'b1 : err_mem_sticky_q);
    err_unlock_flag = 1'b0;
    err_flags_o = '0;
    err_flags_o[DcacheTlErrorUnlockLine] = err_unlock_flag;
    err_flags_o[DcacheTlErrorTransValid] = err_transf_flag_q;
    err_flags_o[DcacheTlErrorWrongOffst] = err_offset_flag_q;
    err_flags_o[DcacheTlErrorAccessFault0] = err_mem_q & ~thread_id_q;
    err_flags_o[DcacheTlErrorAccessFault1] = err_mem_q & thread_id_q;
  end

  assign request_control_start = ctrl_state_changed_q &&
      ((ctrl_state_q == MlCtrlNoTReq) || (ctrl_state_q == MlCtrlIntReq) || (ctrl_state_q == MlCtrlTraReq));
  assign request_control_flush = (ModuleIdx == 1) ? (ctrl_state_q == MlCtrlIntTransFlush) : 1'b0;

  always_comb begin
    if (ctrl_state_q == MlCtrlIntReq) begin
      request_control_num_lines = trans_size8 ? 4'd3 : 4'd1;
      request_control_use_tmask = trans_use_tmask;
      request_control_use_coop = trans_use_coop;
      request_control_tmask_bits = {TensorMaskBits{trans_tmask_bits_q[shared_tmask_idx]}};
      request_control_stride = trans_stride;
    end else if (ctrl_state_q == MlCtrlTraReq) begin
      request_control_num_lines = trans_size8 ? 4'd7 : (trans_size16 ? 4'd3 : 4'd1);
      request_control_use_tmask = 1'b0;
      request_control_use_coop = trans_use_coop;
      request_control_tmask_bits = '0;
      request_control_stride = trans_stride;
    end else begin
      request_control_num_lines = trans_nlines;
      request_control_use_tmask = trans_use_tmask;
      request_control_use_coop = trans_use_coop;
      request_control_tmask_bits = trans_tmask_bits_q;
      request_control_stride = trans_stride;
    end
  end

  assign request_control_scp_dest = tensor_ctrl_scp_dest_q;
  assign request_control_l2_group_id = id_running_q;
  assign update_l2_operation_id =
      ((ctrl_state_q == MlCtrlNoTReq) && request_control_ending && (!request_control_use_tmask || (|request_control_tmask_bits))) ||
      ((ctrl_state_q == MlCtrlIntReq) && request_control_ending_l2 && (!request_control_use_tmask || (|request_control_tmask_bits))) ||
      ((ctrl_state_q == MlCtrlTraReq) && request_control_ending_l2) ||
      ((ctrl_state_q == MlCtrlIntTransFlush) && request_control_ending && (ModuleIdx == 1));
  assign request_control_thread_id = thread_id_q;

  assign shared_clear_set = get_set_from_scp_dest(shared_dest_q);
  assign shared_clear_way = get_way_from_scp_dest(shared_dest_q);

  assign load_way_o = (ModuleIdx == 0) ? (((ctrl_state_q >= MlCtrlIntTransFlush) ? shared_load_way : request_control_load_way)) : request_control_load_way;
  assign load_addr_o = (ModuleIdx == 0) ? (((ctrl_state_q >= MlCtrlIntTransFlush) ? shared_load_addr : request_control_load_addr)) : request_control_load_addr;
  assign load_data_o = shared_load_data;
  assign load_valid_o = interleave_load_valid | transpose_load_valid;
  assign load_capture_o = (ctrl_state_q == MlCtrlIntReq) || (ctrl_state_q == MlCtrlTraReq);
  assign interleave_load_valid = {DcacheLramNumBanks{(ctrl_state_q == MlCtrlIntInt)}};
  assign transpose_load_valid_single = (ctrl_state_q == MlCtrlTraTra) && (trans_tmask_bits_q[shared_tmask_idx] | !trans_use_tmask);
  assign transpose_load_valid_mask = {{(DcacheLramNumBanks-1){1'b0}}, 1'b1} << trans_split_count_q[1:0];
  assign transpose_load_valid = {DcacheLramNumBanks{transpose_load_valid_single}} & transpose_load_valid_mask;
  assign load_valid_pre_o = (ctrl_state_d == MlCtrlIntInt) || (ctrl_state_d == MlCtrlTraTra);
  assign interleave_clear_valid_d = (ModuleIdx == 0) ? ((ctrl_state_q == MlCtrlIntTransFlush) && (shared_count_q <= trans_nlines)) : 1'b0;
  assign interleave_clear_valid_cken = (ModuleIdx == 0) ? ((ctrl_state_q == MlCtrlIntTransFlush) || (ctrl_state_q == MlCtrlIntReq)) : 1'b0;
  assign shared_load_way = shared_clear_way;
  assign shared_load_addr = {shared_clear_set, shared_load_idx_q, {CoreL2BlockExtAddrOffsetBits{1'b0}}};
  assign shared_l2_use_p2 = (trans_offset == tensorload_offset_2) || (trans_offset == tensorload_offset_3);
  assign shared_l2_count = (ModuleIdx == 0) ?
      (((ctrl_state_q == MlCtrlIntTransFlush) || ((ctrl_state_q == MlCtrlIntReq) && (|new_l2_req))) ||
       ((ctrl_state_q == MlCtrlTraReq) && ((|new_l2_req) || ctrl_state_changed_q))) : 1'b0;

  assign shared_l2_sel_data[0] =
      ((trans_offset == tensorload_offset_0) || (trans_offset == tensorload_offset_2)) ?
      l2_resp_i.data[0*DcacheLramDataSize +: DcacheLramDataSize] :
      l2_resp_i.data[2*DcacheLramDataSize +: DcacheLramDataSize];
  assign shared_l2_sel_data[1] =
      ((trans_offset == tensorload_offset_0) || (trans_offset == tensorload_offset_2)) ?
      l2_resp_i.data[1*DcacheLramDataSize +: DcacheLramDataSize] :
      l2_resp_i.data[3*DcacheLramDataSize +: DcacheLramDataSize];
  assign shared_l2_sel_data[2] = l2_resp_i.data[2*DcacheLramDataSize +: DcacheLramDataSize];
  assign shared_l2_sel_data[3] = l2_resp_i.data[3*DcacheLramDataSize +: DcacheLramDataSize];
  assign rf_write_en_prev = (ctrl_state_d == MlCtrlIntReq) || (ctrl_state_q == MlCtrlTraReq);

  always_comb begin
    shared_stg_idx = l2_resp_i.id[DcacheTlL2TransfersSelBits-1:0];
    shared_l2_store = '0;
    if (trans_int8 || trans_int16) begin
      for (int i = 0; i < DcacheTlIntDataBlocks; i += 2) begin
        if ((shared_l2_use_p2 == tl_load_idx_q[shared_stg_idx]) && l2_resp_is_for_me) begin
          if (trans_int8) shared_l2_store[i] = (l2i_stg_addr_q[shared_stg_idx][2:1] == i[2:1]);
          else shared_l2_store[i] = (l2i_stg_addr_q[shared_stg_idx][1] == i[2]);
        end
        shared_l2_store[i+1] = shared_l2_store[i];
      end
    end else begin
      for (int i = 0; i < DcacheTlTrpDataBlocks; i += 2) begin
        if (trans_trp32 && l2_resp_is_for_me) begin
          shared_l2_store[i] = ((l2i_stg_addr_q[shared_stg_idx][1:0] | {1'b0, tl_load_idx_q[shared_stg_idx]}) == i[3:2]);
        end else if ((shared_l2_use_p2 == tl_load_idx_q[shared_stg_idx]) && l2_resp_is_for_me) begin
          if (trans_trp8) shared_l2_store[i] = (l2i_stg_addr_q[shared_stg_idx][3:1] == i[3:1]);
          else if (trans_trp16) shared_l2_store[i] = (l2i_stg_addr_q[shared_stg_idx][2:1] == i[3:2]);
        end
        shared_l2_store[i+1] = shared_l2_store[i];
      end
    end
  end

  always_comb begin
    tmp_data_d = '0;
    if (trans_int8 || trans_trp8) begin
      for (int i = 0; i < DcacheTlTmpDataBlocks; i += 2) begin
        tmp_data_d[i] = shared_l2_sel_data[0];
        tmp_data_d[i+1] = shared_l2_sel_data[1];
      end
    end else begin
      for (int i = 0; i < DcacheTlTmpDataBlocks; i += 4) begin
        tmp_data_d[i] = shared_l2_sel_data[0];
        tmp_data_d[i+1] = shared_l2_sel_data[1];
        tmp_data_d[i+2] = shared_l2_sel_data[2];
        tmp_data_d[i+3] = shared_l2_sel_data[3];
      end
    end
  end

  generate
    if (ModuleIdx == 0) begin : gen_tmp_data_rf
      for (genvar i = 0; i < DcacheTlTmpDataBlocks; i++) begin : gen_tmp_data_blk
        prim_rf_single_1r1w_par_preview #(
          .Width (DcacheLramDataSize)
        ) u_tmp_data (
          .preview_clk_i        (clkgt),
          .rf_clk_i             (clkgt),
          .rd_data_o            (tmp_data_q[i]),
          .wr_data_i            (tmp_data_d[i]),
          .wr_data_en_1p_next_i (rf_write_en_prev),
          .wr_en_i              (shared_l2_store[i])
        );
      end
    end else begin : gen_tmp_data_zero
      always_comb begin
        tmp_data_q = '0;
      end
    end
  endgenerate

  always_comb begin
    shared_load_data = '0;
    idx_buf = '0;
    idx_bufl = '0;
    idx_byte = '0;
    if (trans_int8) begin
      for (int i = 0; i < minion_dcache_pkg::CoreL2BlockExtSize/8; i++) begin
        idx_buf = {4'b0, i[1:0], shared_load_idx_q};
        idx_byte = i[4:2];
        idx_bufl = idx_buf[3:0];
        shared_load_data[i*8 +: 8] = tmp_data_q[idx_bufl][idx_byte*8 +: 8];
      end
    end else if (trans_int16) begin
      for (int i = 0; i < minion_dcache_pkg::CoreL2BlockExtSize/8; i += 2) begin
        idx_buf = {4'b0, i[1:0], 1'b0} + {{6{1'b0}}, i[4]} + {5'b0, shared_load_idx_q, 1'b0};
        idx_byte = {i[3:2], 1'b0};
        idx_bufl = idx_buf[3:0];
        shared_load_data[i*8 +: 16] = tmp_data_q[idx_bufl][idx_byte*8 +: 16];
      end
    end else if (trans_trp8) begin
      for (int i = 0; i < minion_dcache_pkg::CoreL2BlockExtSize/8; i++) begin
        idx_buf = {i[4:0], 1'b0} + {shared_load_idx_q, 6'b0} + shared_count_q[DcacheTensorCtrlNlinesBits-1:3];
        idx_byte = shared_count_q[2:0];
        idx_bufl = idx_buf[3:0];
        shared_load_data[i*8 +: 8] = tmp_data_q[idx_bufl][idx_byte*8 +: 8];
      end
    end else if (trans_trp16) begin
      for (int i = 0; i < minion_dcache_pkg::CoreL2BlockExtSize/8; i += 2) begin
        idx_buf = {i[4:0], 1'b0} + {shared_load_idx_q, 6'b0} +
                  {{(7-(DcacheTensorCtrlNlinesBits-2)){1'b0}}, shared_count_q[DcacheTensorCtrlNlinesBits-1:2]};
        idx_byte = {shared_count_q[1:0], 1'b0};
        idx_bufl = idx_buf[3:0];
        shared_load_data[i*8 +: 16] = tmp_data_q[idx_bufl][idx_byte*8 +: 16];
      end
    end else begin
      for (int i = 0; i < minion_dcache_pkg::CoreL2BlockExtSize/8; i += 4) begin
        idx_buf = {i[4:0], 1'b0} + {shared_load_idx_q, 6'b0} +
                  {{(7-(DcacheTensorCtrlNlinesBits-1)){1'b0}}, shared_count_q[DcacheTensorCtrlNlinesBits-1:1]};
        idx_byte = {shared_count_q[0], 2'b0};
        idx_bufl = idx_buf[3:0];
        shared_load_data[i*8 +: 32] = tmp_data_q[idx_bufl][idx_byte*8 +: 32];
      end
    end
  end

  always_comb begin
    save_tmask_pending_d = save_tmask_pending_q;
    if ((ctrl_state_q == MlCtrlIdle) && ctrl_start_tmask_wait) save_tmask_pending_d = 1'b1;
    if (save_tmask_bits) save_tmask_pending_d = 1'b0;
  end
  assign save_tmask_bits = ((ctrl_state_q == MlCtrlIdle) && tensor_ctrl_i.tensorload_tmask_ready && start_pulse) ||
                           ((ctrl_state_q == MlCtrlWaitStart) && save_tmask_pending_q && tensor_ctrl_use_tmask_q && tensor_ctrl_i.tensorload_tmask_ready);

  assign request_control_store_addr = tensor_ctrl_start;
  assign request_control_address = {tensor_ctrl_i.tensorload_ctrl.addr, 6'b0};
  assign vm_status_tl_o = vm_status_tl_q;

  // Request-control path
  assign request_control_flush_done_cond = request_control_flush && !enabled_q && !l2_interface_busy;
  assign request_control_flush_done_int = request_control_flush_done_cond;
  assign tenb_flush_int = request_control_flush_done_cond;
  assign tenb_flush_o = tenb_flush_int;
  assign tenb_idx = tenb_count_q[DcacheTenbBits-1:0];

  always_comb begin
    tenb_rcv_ok = tenb_rcv_entry_q[tenb_idx];
    for (int i = 0; i < VpuTenbSize/2; i++) begin
      if ((i > tenb_idx_last_q) && (tenb_idx_last_q >= tenb_idx)) tenb_rcv_ok &= tenb_rcv_entry_q[i];
    end
  end

  always_comb begin
    enabled_d = enabled_q;
    addr_up = 1'b0;
    err_mem_d = 1'b0;
    s0_addr_next = s0_addr_o + request_control_stride;
    s0_addr_msb_err_next = s0_addr_o[minion_dcache_pkg::VaSize-1] & ~s0_addr_next[minion_dcache_pkg::VaSize-1];
    count_d = count_q + 1'b1;
    tmask_idx_d = tmask_idx_q + 1'b1;
    tenb_idx_save = 1'b0;
    tenb_credits_d = (ModuleIdx == 0) ? '0 : tenb_credits_q;
    tenb_rcv_entry_d = (ModuleIdx == 0) ? '0 : tenb_rcv_entry_q;
    tenb_count_d = (ModuleIdx == 0) ? '0 : tenb_count_q;
    tenb_idx_last_d = tenb_idx_last_q;

    if (!enabled_q && request_control_store_addr) begin
      s0_addr_next = request_control_address;
      s0_addr_msb_err_next = 1'b0;
      count_d = count_q;
      tmask_idx_d = tmask_idx_q;
      addr_up = 1'b1;
    end

    enabled_req = 1'b0;
    if (!enabled_q && request_control_start) begin
      s0_addr_next = s0_addr_o;
      s0_addr_msb_err_next = 1'b0;
      count_d = '0;
      tmask_idx_d = '0;
      addr_up = 1'b1;
      enabled_d = 1'b1;
      enabled_req = 1'b1;
      if (ModuleIdx == 1) tenb_count_d = '0;
    end

    if ((s2_req_valid_q && s2_req_ready) || s0_addr_load_skip_mask) begin
      addr_up = 1'b1;
      if (count_q == request_control_num_lines) enabled_d = 1'b0;
    end

    if ((ModuleIdx == 1) && s2_req_valid_q && s2_req_ready) begin
      tenb_credits_d = tenb_credits_q - tensor_ctrl_tenb;
      tenb_rcv_entry_d[tenb_idx] = 1'b0;
      tenb_count_d = tenb_count_q + 1'b1;
      tenb_idx_save = 1'b1;
    end
    if ((ModuleIdx == 1) && tenb_credit_i) tenb_credits_d = tenb_credits_d + 1'b1;
    if (tenb_written_i) tenb_rcv_entry_d[tenb_written_entry] = 1'b1;

    if (enabled_q && s1_addr_load_x_q && s1_tlb_fail_x) begin
      enabled_d = 1'b0;
      err_mem_d = 1'b1;
    end
    if (request_control_flush) enabled_d = 1'b0;
    if (request_control_flush_done_cond) begin
      tenb_credits_d = DcacheTlTenbCredits[VpuTenbCreditBits-1:0];
      tenb_rcv_entry_d = {VpuTenbSize/2{1'b1}};
    end
    if (tenb_idx_save) tenb_idx_last_d = tenb_idx;

    l1_scp_dest_ext = {1'b0, request_control_scp_dest} +
                      {{(minion_dcache_pkg::DcacheTensorCtrlDestBits + 1 - DcacheTensorCtrlNlinesBits){1'b0}}, count_q};
    if (l1_scp_dest_ext >= DcacheTlScpMaxIdxVal) l1_scp_dest_ext = l1_scp_dest_ext - DcacheTlScpMaxIdxVal;
    l1_scp_dest = l1_scp_dest_ext[minion_dcache_pkg::DcacheTensorCtrlDestBits-1:0];
    request_control_clear_set = tensor_ctrl_tenb ? count_q[DcacheSetIdxWidth-1:0] : get_set_from_scp_dest(l1_scp_dest);
    request_control_clear_way = tensor_ctrl_tenb ? tenb_idx : get_way_from_scp_dest(l1_scp_dest);

    s0_addr_load_x = s0_available_x && enabled_q && !s1_addr_load_x_q && !s2_req_valid_q &&
                     (!request_control_use_tmask || request_control_tmask_bits[tmask_idx_q]) &&
                     !waiting_tlb_q && (!tensor_ctrl_tenb || (tenb_credits_ok && tenb_rcv_ok));
    s1_addr_load_next = s0_addr_load_x && !request_control_flush;
    s2_req_valid_d = (s1_addr_load_x_q && !s1_tlb_wait_x && !s1_tlb_fail_x && !request_control_flush) ||
                     (s2_req_valid_q && !s2_req_ready && s1_available_x && !request_control_flush);
    waiting_tlb_d = (s1_addr_load_x_q || waiting_tlb_q) && s1_tlb_wait_x;

    request_control_ending_l2 = ((!enabled_q && l2_interface_busy_prev_q && !l2_interface_busy) ||
                                 (enabled_q && !enabled_d && !(|request_control_tmask_bits) && request_control_use_tmask) ||
                                 (enabled_q && !enabled_d && s1_addr_load_x_q && s1_tlb_fail_x));
    request_control_ending = !(enabled_q || enabled_d || l2_req_valid || l2_seq_wait);
  end

  always_comb begin
    s0_addr_load_skip_mask = s0_available_x && enabled_q && !s1_addr_load_x_q && !s2_req_valid_q &&
                             request_control_use_tmask && !request_control_tmask_bits[tmask_idx_q] && !waiting_tlb_q;
  end

  assign shadow_tlb_translate = 1'b0;
  assign s0_addr_load_o = s0_addr_load_x & !shadow_tlb_translate;
  assign s0_thread_id_o = request_control_thread_id;
  assign s1_addr_load_o = s1_addr_load_x_q & !shadow_tlb_translate;
  assign s0_available_x = s0_available_i | shadow_tlb_translate;
  assign s1_available_x = s1_available_i | shadow_tlb_translate;
  assign s1_tlb_wait_x = s1_tlb_wait_i & !shadow_tlb_translate;
  assign s1_tlb_fail_x = s1_tlb_fail_i & !shadow_tlb_translate;
  assign s2_addr_x = shadow_tlb_translate ? s0_addr_o[minion_dcache_pkg::PaSize-1:0] : s2_addr_i;

  // L2-side transaction control
  assign s2_req_ready = !(l2_req_valid | l2_seq_wait) &&
                        (|waiting_for_req) &&
                        (!l2i_op_in_idx[request_control_l2_op_idx] || (l2i_current_idx_q == request_control_l2_op_idx));
  assign l2_interface_busy = ~(&waiting_for_req);

  generate
    for (genvar i = 0; i < DcacheTlL2Transfers; i++) begin : gen_l2i
      if (i == 0) begin : gen_miss_id_0
        assign my_miss_id[i] = (ModuleIdx == 0) ? EtLinkCoreMissIdTensorLoadExt0 : EtLinkCoreMissIdTensorLoadExt4;
      end else if (i == 1) begin : gen_miss_id_1
        assign my_miss_id[i] = (ModuleIdx == 0) ? EtLinkCoreMissIdTensorLoadExt1 : EtLinkCoreMissIdTensorLoadExt5;
      end else if (i == 2) begin : gen_miss_id_2
        assign my_miss_id[i] = (ModuleIdx == 0) ? EtLinkCoreMissIdTensorLoadExt2 : EtLinkCoreMissIdTensorLoadExt6;
      end else begin : gen_miss_id_3
        assign my_miss_id[i] = (ModuleIdx == 0) ? EtLinkCoreMissIdTensorLoadExt3 : EtLinkCoreMissIdTensorLoadExt7;
      end
      if (i == 0) begin : gen_first_req
        assign new_l2_req[i] = (l2i_state_q[i] == MlL2iWaitReq) && !(l2_req_valid | l2_seq_wait) && s2_req_valid_q && s2_req_ready;
      end else begin : gen_other_req
        assign new_l2_req[i] = (l2i_state_q[i] == MlL2iWaitReq) && !(l2_req_valid | l2_seq_wait) &&
                               s2_req_valid_q && s2_req_ready && ((|new_l2_req[i-1:0]) == 1'b0);
      end
      assign l2i_state_up[i] = (l2i_state_q[i] != MlL2iWaitReq) | enabled_q;
      assign waiting_for_req[i] = (l2i_state_q[i] == MlL2iWaitReq);
      assign requesting_to_l2[i] = (l2i_state_q[i] == MlL2iL2Req);
      assign accepted_to_l2[i] = requesting_to_l2[i] & l2_req_ready;
      assign sequencing_to_l2[i] = (l2i_state_q[i] == MlL2iL2Seq);
      assign new_data_chunk[i] = (l2i_state_q[i] == MlL2iWaitAck) &&
                                 ((l2_resp_i.opcode == EtLinkRspAckData) ||
                                  (l2_resp_i.opcode == EtLinkRspAck)) &&
                                 (et_link_core_miss_id_e'(l2_resp_i.id) == my_miss_id[i]) && l2_resp_valid_i;
      assign received_error[i] = (l2i_state_q[i] == MlL2iWaitAck) &&
                                 (l2_resp_i.opcode == EtLinkRspErr) &&
                                 (et_link_core_miss_id_e'(l2_resp_i.id) == my_miss_id[i]) && l2_resp_valid_i;
      assign received_answer[i] = (l2i_state_q[i] == MlL2iWaitAck) &&
                                  (et_link_core_miss_id_e'(l2_resp_i.id) == my_miss_id[i]) && l2_resp_valid_i;
    end
  endgenerate

  always_comb begin
    l2i_op_in_idx = 2'b0;
    for (int idx = 0; idx < 2; idx++) begin
      for (int i = 0; i < DcacheTlL2Transfers; i++) begin
        l2i_op_in_idx[idx] |= (l2i_op_idx_q[i] == idx[0]) && (l2i_state_q[i] != MlL2iWaitReq);
      end
    end
  end

  assign sequence_usable = (sequence_data_q[{l2i_seq_idx_q, sequence_count_q}] == 1'b0);

  always_comb begin
    req_addr_cline_d = req_addr_cline_q;
    l2_req_idx = '0;
    l2i_current_idx_d = l2i_current_idx_q;
    sequence_data_d = sequence_data_q;
    sequence_count_d = sequence_count_q;
    l2i_seq_idx_d = l2i_seq_idx_q;
    tensor_coop_id_prev_d = tensor_coop_id_prev_q;
    tensor_coop_id_prev_up = 1'b0;
    l2i_seq_data_up = 1'b0;
    l2i_seq_cnt_up = 1'b0;
    req_up = |new_l2_req;

    for (int i = 0; i < DcacheTlL2Transfers; i++) begin
      l2i_state_d[i] = l2i_state_q[i];
      tl_load_way_d[i] = tl_load_way_q[i];
      tl_load_set_d[i] = tl_load_set_q[i];
      l2i_group_d[i] = l2i_group_q[i];
      l2i_op_idx_d[i] = l2i_op_idx_q[i];
      l2i_op_is_coop_d[i] = l2i_op_is_coop_q[i];
      l2i_seq_saved_d[i] = l2i_seq_saved_q[i];
      tl_load_idx_d[i] = tl_load_idx_q[i];

      if ((l2i_state_q[i] == MlL2iWaitReq) && new_l2_req[i] && !request_control_use_coop) l2i_state_d[i] = MlL2iL2Req;
      if ((l2i_state_q[i] == MlL2iWaitReq) && new_l2_req[i] && request_control_use_coop) l2i_state_d[i] = MlL2iL2Seq;
      if ((l2i_state_q[i] == MlL2iL2Seq) && sequence_usable) l2i_state_d[i] = MlL2iL2Req;
      if ((l2i_state_q[i] == MlL2iL2Req) && l2_req_ready) l2i_state_d[i] = MlL2iWaitAck;
      if (new_data_chunk[i] && tl_load_idx_q[i]) l2i_state_d[i] = MlL2iWaitReq;
      if (received_error[i]) l2i_state_d[i] = MlL2iWaitReq;

      if (new_l2_req[i]) begin
        tl_load_way_d[i] = request_control_clear_way;
        tl_load_set_d[i] = request_control_clear_set;
        l2i_group_d[i] = request_control_l2_group_id;
        l2i_op_idx_d[i] = request_control_l2_op_idx;
        l2i_op_is_coop_d[i] = request_control_use_coop;
      end
      if (accepted_to_l2[i]) begin
        l2i_seq_saved_d[i] = {l2i_seq_idx_q, sequence_count_q};
      end
      tl_load_idx_d[i] = (l2i_state_q[i] == MlL2iWaitReq) ? 1'b0 :
                         (new_data_chunk[i] ? ~tl_load_idx_q[i] : tl_load_idx_q[i]);
    end

    for (int i = DcacheTlL2Transfers - 1; i >= 0; i--) begin
      if (l2i_state_q[i] == MlL2iL2Req) l2_req_idx = i[DcacheTlL2TransfersSelBits-1:0];
    end

    l2i_current_idx_up = s2_req_valid_q && s2_req_ready &&
                         !l2i_op_in_idx[request_control_l2_op_idx] &&
                         (l2i_current_idx_q != request_control_l2_op_idx);
    if (l2i_current_idx_up) l2i_current_idx_d = request_control_l2_op_idx;

    if (enabled_req) sequence_count_d = '0;
    else if (l2_req_valid & l2_req_ready) sequence_count_d = sequence_count_q + 1'b1;
    l2i_seq_cnt_up = (enabled_req || (l2_req_valid & l2_req_ready)) && request_control_use_coop;
    tensor_coop_id_prev_up = l2i_current_idx_up & request_control_use_coop;
    tensor_coop_id_curr = tensor_coop_data_q[ShireCoopIdStart +: ShireCoopIdSize];
    request_different_coop_id = (tensor_coop_id_curr != tensor_coop_id_prev_q);
    if (request_different_coop_id && request_control_use_coop) l2i_seq_idx_d = ~l2i_seq_idx_q;
    if (tensor_coop_id_prev_up) tensor_coop_id_prev_d = tensor_coop_id_curr;

    l2i_seq_data_up = (l2_req_valid & l2_req_ready & request_control_use_coop) ||
                      (|(received_answer & l2i_op_is_coop_q));
    if (l2_req_valid & l2_req_ready & request_control_use_coop) begin
      sequence_data_d[{l2i_seq_idx_q, sequence_count_q}] = 1'b1;
    end
    for (int i = 0; i < DcacheTlL2Transfers; i++) begin
      if (received_answer[i] & l2i_op_is_coop_q[i]) sequence_data_d[l2i_seq_saved_q[i]] = 1'b0;
    end

    tensor_coop_data_seq_bits = tensor_coop_data_bits |
                                {{($bits(tensorcoop_control)-ShireCoopMinMaskSize-ShireCoopSeqIdSize){1'b0}},
                                 sequence_count_q,
                                 {ShireCoopMinMaskSize{1'b0}}};
    tensor_coop_data_sequenced = tensor_coop_data_seq_bits;

    if (req_up) req_addr_cline_d = s2_addr_x[minion_dcache_pkg::PaSize-1:6];
  end

  assign l2_req_valid = |requesting_to_l2;
  assign l2_req_valid_coop_o = l2_req_valid & request_control_use_coop;
  assign l2_req_valid_no_coop_o = l2_req_valid & ~request_control_use_coop;
  assign l2_seq_wait = |sequencing_to_l2;
  assign l2_req_o.id = my_miss_id[l2_req_idx];
  assign l2_req_o.source = 1'b0;
  assign l2_req_o.wdata = request_control_use_coop;
  assign l2_req_o.opcode = request_control_use_coop ? EtLinkReqReadCoop : EtLinkReqRead;
  assign l2_req_o.subopcode = '0;
  assign l2_req_o.address = {req_addr_cline_q, 6'b0};
  assign l2_req_o.data = request_control_use_coop ? tensor_coop_data_sequenced
                                                  : {{(ShireCoopNeighMaskSize+3+ShireCoopIdSize){1'b0}}, self_minion_mask};
  assign l2_req_o.size = EtLinkLine;
  assign l2_req_o.qwen = request_control_use_coop ? 4'b0001 : 4'b0000;
  assign l2_resp_is_for_me = (|new_data_chunk) | (|received_error);
  assign bus_err_local = |received_error;
  assign bus_err_o = {bus_err_local & thread_id_q, bus_err_local & ~thread_id_q};
  assign sel_t = l2_resp_i.id[DcacheTlL2TransfersSelBits-1:0];
  always_comb begin
    request_control_load_way = tl_load_way_q[sel_t];
    request_control_load_addr = {tl_load_set_q[sel_t], tl_load_idx_q[sel_t], {CoreL2BlockExtAddrOffsetBits{1'b0}}};
  end
  assign tenb_written_entry = request_control_load_way[DcacheTenbBits-1:0];

  always_comb begin
    tenb_avail_entry_d = tenb_avail_entry_q;
    tenb_avail_all_d = &tenb_avail_entry_q;
    if (tenb_written_i) tenb_avail_entry_d[tenb_written_entry] = 1'b0;
    if (tenb_credit_i) tenb_avail_entry_d[tenb_credit_entry_i] = 1'b1;
    if (tenb_flush_int) tenb_avail_entry_d = {VpuTenbSize/2{1'b1}};

    tfma_started_d = tfma_started_q;
    if (tensor_ctrl_start) tfma_started_d = 1'b0;
    if (tfma_enabled_i && !tensor_ctrl_start && !tfma_active_start_q) tfma_started_d = 1'b1;
    tfma_active_start_d = tfma_active_start_q;
    if (tensor_ctrl_start) tfma_active_start_d = tfma_enabled_i;
    if (!tfma_enabled_i) tfma_active_start_d = 1'b0;
  end

  always_comb begin
    tenb_ready_o = tenb_avail_entry_q[tenb_written_entry];
    tenb_credits_ok = ((tenb_credits_q > 0) && tfma_started_q && !tenb_no_extra_credits_cond1_q) ||
                      (tenb_credits_q > DcacheTlTenbLookAheadVal) ||
                      ((tenb_credits_q > 0) && (tenb_count_q < VpuTenbHalfVal) && !tenb_no_extra_credits_cond3_q);
  end

  assign busy_o = (ctrl_state_q != MlCtrlIdle) | start_pulse | tensor_ctrl_i.tensorload_restart_b;
  always_comb begin
    l2_operates_in_id = 2'b0;
    for (int id = 0; id < 2; id++) begin
      for (int i = 0; i < DcacheTlL2Transfers; i++) begin
        l2_operates_in_id[id] |= (l2i_state_q[i] != MlL2iWaitReq) && (l2i_group_q[i] == id[0]);
      end
    end
    for (int id = 0; id <= 1; id++) begin
      if (!thread_id_q) begin
        if (id_running_q == id[0]) id_complete_o[id] = !busy_o && (l2_operates_in_id[id] == 0);
        else id_complete_o[id] = (l2_operates_in_id[id] == 0);
      end else begin
        id_complete_o[id] = 1'b1;
      end
    end
  end

  assign csr_debug_signals_o = {{(DcacheDbgCsrTlSize-3){1'b0}}, ctrl_state_q};
  assign sm_match_debug_signals_o = sm_match_debug_signals_d;
  assign sm_filter_debug_signals_o = sm_filter_debug_signals_d;
  assign sm_data_debug_signals_o = sm_data_debug_signals_d;

  always_comb begin
    /* verilator lint_off WIDTHEXPAND */  // Original debug buses are zero-extended packed concatenations.
    sm_match_debug_signals_d = '0;
    sm_match_debug_signals_d = {
      l2i_state_up,
      new_l2_req,
      tfma_active_start_q,
      (s0_addr_load_o & s0_available_i),
      (s2_req_valid_q & s2_req_ready),
      ctrl_state_changed_q,
      interleave_clear_valid_q,
      (l2_req_valid & l2_req_ready),
      shared_l2_count,
      err_unlock_flag,
      err_transf_flag_q,
      err_offset_flag_q,
      load_valid_o,
      save_tmask_bits,
      addr_up,
      ctrl_en,
      start_pulse,
      request_control_start,
      tensor_ctrl_start
    };

    sm_filter_debug_signals_d = '0;
    sm_filter_debug_signals_d = {
      l2i_state_up,
      new_l2_req,
      tfma_active_start_q,
      s0_addr_load_o,
      s0_available_i,
      s2_req_valid_q,
      s2_req_ready,
      ctrl_state_changed_q,
      interleave_clear_valid_q,
      l2_req_valid,
      l2_req_ready,
      shared_l2_count,
      err_unlock_flag,
      err_transf_flag_q,
      err_offset_flag_q,
      load_valid_o,
      save_tmask_bits,
      addr_up,
      ctrl_en,
      start_pulse,
      request_control_start,
      tensor_ctrl_start,
      l2_req_o.id,
      l2_req_o.opcode,
      l2_req_o.address,
      waiting_tlb_q,
      l2i_op_idx_q,
      l2i_group_q,
      {l2i_state_q[3], l2i_state_q[2], l2i_state_q[1], l2i_state_q[0]},
      ctrl_state_q
    };

    sm_data_debug_signals_d = '0;
    sm_data_debug_signals_d[2*DcacheDbgSmDataGroupBits +: DcacheDbgSmDataGroupBits] = {
      count_q,
      tmask_idx_q,
      shared_l2_counter_q,
      shared_count_q,
      shared_dest_q,
      shared_load_idx_q,
      trans_split_count_q,
      load_way_o,
      load_addr_o,
      tl_load_way_q,
      tl_load_set_q,
      tl_load_idx_q,
      tenb_credits_q,
      tenb_rcv_entry_q,
      tenb_avail_entry_q,
      tfma_started_q,
      s2_addr_i
    };
    sm_data_debug_signals_d[1*DcacheDbgSmDataGroupBits +: DcacheDbgSmDataGroupBits] = {
      l2_req_o.id,
      l2_req_o.opcode,
      l2_req_o.address,
      l2_req_o.data,
      s0_addr_o
    };
    sm_data_debug_signals_d[0*DcacheDbgSmDataGroupBits +: DcacheDbgSmDataGroupBits] = {
      vm_status_tl_q,
      id_running_q,
      thread_id_q,
      trans_tmask_bits_q,
      tensor_load_stride_q,
      tensor_ctrl_use_tmask_q,
      tensor_ctrl_use_coop_q,
      tensor_coop_data_q,
      tensor_ctrl_scp_dest_q,
      tensor_ctrl_offset_q,
      tensor_ctrl_transform_q,
      tensor_ctrl_num_lines_q
    };
    /* verilator lint_on WIDTHEXPAND */
  end

  always_ff @(posedge clkgt or negedge rst_ni) begin
    if (!rst_ni) begin
      start_pulse_pre_q <= 1'b0;
      save_tmask_pending_q <= 1'b0;
      ctrl_state_q <= MlCtrlIdle;
      ctrl_state_changed_q <= 1'b0;
      tensor_load_stride_q <= '0;
      shared_count_q <= '0;
      shared_dest_q <= '0;
      shared_load_idx_q <= 1'b0;
      interleave_clear_valid_q <= 1'b0;
      trans_split_count_q <= '0;
      drain_load_capture_q <= 1'b0;
      err_transf_flag_q <= 1'b0;
      err_offset_flag_q <= 1'b0;
      err_mem_q <= 1'b0;
      err_mem_sticky_q <= 1'b0;
      tensor_coop_data_q <= '0;
      id_running_q <= 1'b0;
      thread_id_q <= 1'b0;
      l2_interface_busy_prev_q <= 1'b0;
      req_addr_cline_q <= '0;
      l2i_current_idx_q <= 1'b1;
      sequence_data_q <= '0;
      sequence_count_q <= '0;
      l2i_seq_idx_q <= 1'b0;
      tensor_coop_id_prev_q <= '0;
      tensor_ctrl_transform_q <= TensorTransNone;
      tensor_ctrl_offset_q <= tensorload_offset_0;
      tensor_ctrl_use_tmask_q <= 1'b0;
      tensor_ctrl_use_coop_q <= 1'b0;
      tensor_ctrl_num_lines_q <= '0;
      tensor_ctrl_scp_dest_q <= '0;
      tenb_no_extra_credits_cond1_q <= 1'b0;
      tenb_no_extra_credits_cond3_q <= 1'b0;
      trans_tmask_bits_q <= '0;
      vm_status_tl_q <= '0;
      l2i_stg_addr_q <= '0;
      shared_l2_counter_q <= '0;
      s0_addr_o <= '0;
      s0_addr_msb_err_o <= 1'b0;
      count_q <= '0;
      tenb_count_q <= '0;
      tenb_idx_last_q <= '0;
      tmask_idx_q <= '0;
      enabled_q <= 1'b0;
      s1_addr_load_x_q <= 1'b0;
      s2_req_valid_q <= 1'b0;
      waiting_tlb_q <= 1'b0;
      request_control_l2_op_idx <= 1'b0;
      for (int i = 0; i < DcacheTlL2Transfers; i++) begin
        l2i_state_q[i] <= MlL2iWaitReq;
        tl_load_way_q[i] <= '0;
        tl_load_set_q[i] <= '0;
        l2i_group_q[i] <= 1'b0;
        l2i_op_idx_q[i] <= 1'b0;
        l2i_op_is_coop_q[i] <= 1'b0;
        l2i_seq_saved_q[i] <= '0;
        tl_load_idx_q[i] <= 1'b0;
      end
    end else begin
      start_pulse_pre_q <= tensor_ctrl_start;
      save_tmask_pending_q <= save_tmask_pending_d;
      ctrl_state_q <= ctrl_state_d;
      if (ctrl_en) ctrl_state_changed_q <= (ctrl_state_d != ctrl_state_q);
      tensor_load_stride_q <= tensor_load_stride_d;
      shared_count_q <= shared_count_d;
      shared_dest_q <= shared_dest_d;
      shared_load_idx_q <= shared_load_idx_d;
      if (interleave_clear_valid_cken) interleave_clear_valid_q <= interleave_clear_valid_d;
      trans_split_count_q <= trans_split_count_d;
      drain_load_capture_q <= drain_load_capture_d;
      err_transf_flag_q <= err_transf_set;
      err_offset_flag_q <= err_offset_set;
      err_mem_q <= err_mem_d;
      err_mem_sticky_q <= err_mem_sticky_d;
      tensor_coop_data_q <= tensor_coop_data_d;
      id_running_q <= id_running_d;
      thread_id_q <= thread_id_d;
      l2_interface_busy_prev_q <= l2_interface_busy;
      req_addr_cline_q <= req_addr_cline_d;
      if (l2i_current_idx_up) l2i_current_idx_q <= l2i_current_idx_d;
      if (l2i_seq_data_up) sequence_data_q <= sequence_data_d;
      if (l2i_seq_cnt_up) sequence_count_q <= sequence_count_d;
      if (l2i_current_idx_up) l2i_seq_idx_q <= l2i_seq_idx_d;
      if (tensor_coop_id_prev_up) tensor_coop_id_prev_q <= tensor_coop_id_prev_d;
      tensor_ctrl_transform_q <= tensor_ctrl_transform_d;
      tensor_ctrl_offset_q <= tensor_ctrl_offset_d;
      tensor_ctrl_use_tmask_q <= tensor_ctrl_use_tmask_d;
      tensor_ctrl_use_coop_q <= tensor_ctrl_use_coop_d;
      tensor_ctrl_num_lines_q <= tensor_ctrl_num_lines_d;
      tensor_ctrl_scp_dest_q <= tensor_ctrl_scp_dest_d;
      tenb_no_extra_credits_cond1_q <= tenb_no_extra_credits_cond1_d;
      tenb_no_extra_credits_cond3_q <= tenb_no_extra_credits_cond3_d;
      if (save_tmask_bits) trans_tmask_bits_q <= tensor_ctrl_i.tensorload_tmask_bits;
      if (start_pulse) vm_status_tl_q <= thread_id_q ? vm_status_i[1] : vm_status_i[0];
      if (shared_l2_count) begin
        if ((ctrl_state_q == MlCtrlIntTransFlush) || ((ctrl_state_q == MlCtrlTraReq) && ctrl_state_changed_q)) begin
          shared_l2_counter_q <= '0;
        end else begin
          shared_l2_counter_q <= shared_l2_counter_q + 4'd2;
        end
      end
      if (addr_up) begin
        s0_addr_o <= s0_addr_next;
        s0_addr_msb_err_o <= s0_addr_msb_err_next;
        count_q <= count_d;
        tmask_idx_q <= tmask_idx_d;
        if (ModuleIdx == 1) tenb_count_q <= tenb_count_d;
      end
      enabled_q <= enabled_d;
      s1_addr_load_x_q <= s1_addr_load_next;
      s2_req_valid_q <= s2_req_valid_d;
      waiting_tlb_q <= waiting_tlb_d;
      for (int i = 0; i < DcacheTlL2Transfers; i++) begin
        if (l2i_state_up[i]) l2i_state_q[i] <= l2i_state_d[i];
        if (new_l2_req[i]) begin
          tl_load_way_q[i] <= tl_load_way_d[i];
          tl_load_set_q[i] <= tl_load_set_d[i];
          l2i_group_q[i] <= l2i_group_d[i];
          l2i_op_idx_q[i] <= l2i_op_idx_d[i];
          l2i_op_is_coop_q[i] <= l2i_op_is_coop_d[i];
          l2i_stg_addr_q[i] <= shared_l2_counter_q;
        end
        if (accepted_to_l2[i]) l2i_seq_saved_q[i] <= l2i_seq_saved_d[i];
        if (l2i_state_up[i]) tl_load_idx_q[i] <= tl_load_idx_d[i];
      end
      if (ModuleIdx == 1) tenb_idx_last_q <= tenb_idx_last_d;
      if (update_l2_operation_id) request_control_l2_op_idx <= ~request_control_l2_op_idx;
    end
  end

  // Original tensor-load keeps TENB bookkeeping on the ungated clock because
  // the VPU-facing credit/write pulses can arrive while the local clock gate is off.
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      tenb_avail_entry_q <= {VpuTenbSize/2{1'b1}};
      tenb_avail_all_q <= 1'b0;
      if (ModuleIdx == 1) begin
        tenb_credits_q <= DcacheTlTenbCredits[VpuTenbCreditBits-1:0];
        tenb_rcv_entry_q <= {VpuTenbSize/2{1'b1}};
      end else begin
        tenb_credits_q <= '0;
        tenb_rcv_entry_q <= '0;
      end
    end else begin
      tenb_avail_entry_q <= tenb_avail_entry_d;
      tenb_avail_all_q <= tenb_avail_all_d;
      if (ModuleIdx == 1) begin
        tenb_credits_q <= tenb_credits_d;
        tenb_rcv_entry_q <= tenb_rcv_entry_d;
      end else begin
        tenb_credits_q <= '0;
        tenb_rcv_entry_q <= '0;
      end
    end
  end

  // The original uses plain FF macros here, so these overlap trackers are not
  // reset with the rest of the TENB bookkeeping state.
  always_ff @(posedge clk_i) begin
    tfma_started_q <= tfma_started_d;
    tfma_active_start_q <= tfma_active_start_d;
  end

endmodule
