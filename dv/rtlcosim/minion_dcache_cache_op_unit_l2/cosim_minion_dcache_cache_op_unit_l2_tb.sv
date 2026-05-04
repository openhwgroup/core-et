// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original dcache_cache_op_unit_l2 vs new
// minion_dcache_cache_op_unit_l2.

`include "soc.vh"

module cosim_minion_dcache_cache_op_unit_l2_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                            clk_i,
  input  logic                            rst_ni,
  input  logic [NumShireIdsBits-1:0]      shire_id_i,
  input  logic [DcacheCoL2MaxOpsBits-1:0] cacheop_max_i,
  input  logic [DcacheCoL2ReprateBits-1:0] cacheop_reprate_i,
  input  logic                            chicken_bit_dcache_i,
  input  logic                            l2_resp_valid_i,
  input  logic [7:0]                      l2_resp_id_i,
  input  logic [1:0]                      l2_resp_opcode_i,
  input  logic                            cache_op_valid_i,
  input  logic [3:0]                      cache_op_cmd_i,
  input  logic [1:0]                      cache_op_start_level_i,
  input  logic [1:0]                      cache_op_dest_level_i,
  input  logic [3:0]                      cache_op_count_i,
  input  logic [VaSize-1:0]               cache_op_addr_i,
  input  logic [VaSize-1:0]               cache_op_stride_i,
  input  logic                            cache_op_thread_id_i,
  input  logic                            cache_op_use_tmask_i,
  input  logic                            cache_op_tmask_ready_i,
  input  logic [TensorMaskBits-1:0]       cache_op_tmask_bits_i,
  input  logic [DcacheL2ScpDstBits-1:0]   cache_op_l2_scp_dest_i,
  input  logic                            cache_op_l2_scp_fill_i,
  input  logic                            l2_req_ready_i,
  input  logic                            cache_op_valid_byp_i,
  input  logic [3:0]                      cache_op_byp_cmd_i,
  input  logic [1:0]                      cache_op_byp_start_level_i,
  input  logic [1:0]                      cache_op_byp_dest_level_i,
  input  logic [3:0]                      cache_op_byp_count_i,
  input  logic [VaSize-1:0]               cache_op_byp_addr_i,
  input  logic [VaSize-1:0]               cache_op_byp_stride_i,
  input  logic                            cache_op_byp_thread_id_i,
  input  logic                            cache_op_byp_use_tmask_i,
  input  logic [TensorMaskBits-1:0]       cache_op_byp_tmask_bits_i,
  input  logic [DcacheL2ScpDstBits-1:0]   cache_op_byp_l2_scp_dest_i,
  input  logic                            cache_op_byp_l2_scp_fill_i,
  input  logic                            s0_available_i,
  input  logic                            s1_available_i,
  input  logic                            s1_tlb_wait_i,
  input  logic                            s1_tlb_fail_i,
  input  logic [PaSize-PaUntransSize-1:0] s1_tlb_addr_i,
  input  logic [NrThreads-1:0]            vm_status_debug_i,
  input  logic [NrThreads-1:0]            vm_status_sum_i,
  input  logic [NrThreads-1:0]            vm_status_mxr_i,
  input  logic [NrThreads-1:0][1:0]       vm_status_prv_i,
  input  logic [NrThreads-1:0][1:0]       vm_status_mpp_i,
  input  logic [NrThreads-1:0]            vm_status_mprv_i,
  input  logic [1:0]                      vm_status_co_prv_i,
  input  logic [1:0]                      vm_status_co_mpp_i,
  input  logic                            vm_status_co_mprv_i,
  input  logic                            vm_status_co_sum_i,
  input  logic                            vm_status_co_mxr_i,
  input  logic                            vm_status_co_debug_i,
  output logic                            orig_busy_o,
  output logic                            new_busy_o,
  output logic [NrThreads-1:0][1:0]       orig_th_id_complete_o,
  output logic [NrThreads-1:0][1:0]       new_th_id_complete_o,
  output logic                            orig_cache_op_ready_o,
  output logic                            new_cache_op_ready_o,
  output logic                            orig_l2_req_valid_o,
  output logic                            new_l2_req_valid_o,
  output logic [4:0]                      orig_l2_req_id_o,
  output logic [4:0]                      new_l2_req_id_o,
  output logic [4:0]                      orig_l2_req_opcode_o,
  output logic [4:0]                      new_l2_req_opcode_o,
  output logic [6:0]                      orig_l2_req_subopcode_o,
  output logic [6:0]                      new_l2_req_subopcode_o,
  output logic [PaSize-1:0]               orig_l2_req_address_o,
  output logic [PaSize-1:0]               new_l2_req_address_o,
  output logic [63:0]                     orig_l2_req_data0_o,
  output logic [63:0]                     orig_l2_req_data1_o,
  output logic [63:0]                     orig_l2_req_data2_o,
  output logic [63:0]                     orig_l2_req_data3_o,
  output logic [63:0]                     new_l2_req_data0_o,
  output logic [63:0]                     new_l2_req_data1_o,
  output logic [63:0]                     new_l2_req_data2_o,
  output logic [63:0]                     new_l2_req_data3_o,
  output logic [2:0]                      orig_l2_req_size_o,
  output logic [2:0]                      new_l2_req_size_o,
  output logic [EtLinkQwenSize-1:0]       orig_l2_req_qwen_o,
  output logic [EtLinkQwenSize-1:0]       new_l2_req_qwen_o,
  output logic                            orig_cache_op_ready_byp_o,
  output logic                            new_cache_op_ready_byp_o,
  output logic                            orig_s0_addr_load_o,
  output logic                            new_s0_addr_load_o,
  output logic [VaSize-1:0]               orig_s0_addr_o,
  output logic [VaSize-1:0]               new_s0_addr_o,
  output logic                            orig_s0_addr_msb_err_o,
  output logic                            new_s0_addr_msb_err_o,
  output logic                            orig_s0_thread_id_o,
  output logic                            new_s0_thread_id_o,
  output logic                            orig_s1_addr_load_o,
  output logic                            new_s1_addr_load_o,
  output logic [1:0]                      orig_vm_status_l2_prv_o,
  output logic [1:0]                      new_vm_status_l2_prv_o,
  output logic [1:0]                      orig_vm_status_l2_mpp_o,
  output logic [1:0]                      new_vm_status_l2_mpp_o,
  output logic                            orig_vm_status_l2_mprv_o,
  output logic                            new_vm_status_l2_mprv_o,
  output logic                            orig_vm_status_l2_sum_o,
  output logic                            new_vm_status_l2_sum_o,
  output logic                            orig_vm_status_l2_mxr_o,
  output logic                            new_vm_status_l2_mxr_o,
  output logic                            orig_vm_status_l2_debug_o,
  output logic                            new_vm_status_l2_debug_o,
  output logic [NrThreads-1:0]            orig_err_flags_o,
  output logic [NrThreads-1:0]            new_err_flags_o,
  output logic [NrThreads-1:0]            orig_bus_err_o,
  output logic [NrThreads-1:0]            new_bus_err_o,
  output logic [DcacheDbgCsrCoul2Size-1:0] orig_csr_debug_signals_o,
  output logic [DcacheDbgCsrCoul2Size-1:0] new_csr_debug_signals_o,
  output logic [63:0]                     orig_sm_match_chunk0_o,
  output logic [63:0]                     new_sm_match_chunk0_o,
  output logic [63:0]                     orig_sm_filter_chunk0_o,
  output logic [63:0]                     orig_sm_filter_chunk1_o,
  output logic [63:0]                     orig_sm_filter_chunk2_o,
  output logic [63:0]                     orig_sm_filter_chunk3_o,
  output logic [63:0]                     new_sm_filter_chunk0_o,
  output logic [63:0]                     new_sm_filter_chunk1_o,
  output logic [63:0]                     new_sm_filter_chunk2_o,
  output logic [63:0]                     new_sm_filter_chunk3_o,
  output logic [63:0]                     orig_sm_data_chunk0_o,
  output logic [63:0]                     orig_sm_data_chunk1_o,
  output logic [63:0]                     orig_sm_data_chunk2_o,
  output logic [63:0]                     orig_sm_data_chunk3_o,
  output logic [63:0]                     orig_sm_data_chunk4_o,
  output logic [63:0]                     orig_sm_data_chunk5_o,
  output logic [63:0]                     orig_sm_data_chunk6_o,
  output logic [63:0]                     orig_sm_data_chunk7_o,
  output logic [63:0]                     new_sm_data_chunk0_o,
  output logic [63:0]                     new_sm_data_chunk1_o,
  output logic [63:0]                     new_sm_data_chunk2_o,
  output logic [63:0]                     new_sm_data_chunk3_o,
  output logic [63:0]                     new_sm_data_chunk4_o,
  output logic [63:0]                     new_sm_data_chunk5_o,
  output logic [63:0]                     new_sm_data_chunk6_o,
  output logic [63:0]                     new_sm_data_chunk7_o
);
  core_dcache_op_req_t req_new, req_byp_new;
  core_dcache_op_req_next_t req_next_new;
  core_dcache_op_req req_orig, req_byp_orig;
  core_dcache_op_req_next req_next_orig;
  et_link_minion_rsp_info_t l2_resp_new, l2_resp_orig;
  et_link_minion_evict_req_info_t l2_req_new, l2_req_orig;
  vm_status_t [NrThreads-1:0] vm_status_new;
  minion_vm_status [`CORE_NR_THREADS-1:0] vm_status_orig;
  vm_status_t vm_status_co_new, vm_status_l2_new;
  minion_vm_status vm_status_co_orig, vm_status_l2_orig;
  logic [DcacheDbgSmMatchBits-1:0] sm_match_new;
  logic [DcacheDbgSmFilterBits-1:0] sm_filter_new;
  logic [DcacheDbgSmDataBits-1:0] sm_data_new;
  logic [`DCACHE_DBG_SM_MATCH_BITS-1:0] sm_match_orig;
  logic [`DCACHE_DBG_SM_FILTER_BITS-1:0] sm_filter_orig;
  logic [`DCACHE_DBG_SM_DATA_BITS-1:0] sm_data_orig;

  always_comb begin
    req_new = '0;
    req_new.cmd = cache_op_cmd_e'(cache_op_cmd_i);
    req_new.start_level = cache_op_level_e'(cache_op_start_level_i);
    req_new.dest_level = cache_op_level_e'(cache_op_dest_level_i);
    req_new.count = cache_op_count_i;
    req_new.addr = cache_op_addr_i;
    req_new.stride = cache_op_stride_i;
    req_new.thread_id = cache_op_thread_id_i;
    req_new.use_tmask = cache_op_use_tmask_i;
    req_new.tmask_ready = cache_op_tmask_ready_i;
    req_new.tmask_bits = cache_op_tmask_bits_i;
    req_new.l2_scp_dest = cache_op_l2_scp_dest_i;
    req_new.l2_scp_fill = cache_op_l2_scp_fill_i;

    req_next_new = '0;
    req_next_new.cmd = cache_op_cmd_e'(cache_op_cmd_i);
    req_next_new.start_level = cache_op_level_e'(cache_op_start_level_i);
    req_next_new.dest_level = cache_op_level_e'(cache_op_dest_level_i);

    req_byp_new = '0;
    req_byp_new.cmd = cache_op_cmd_e'(cache_op_byp_cmd_i);
    req_byp_new.start_level = cache_op_level_e'(cache_op_byp_start_level_i);
    req_byp_new.dest_level = cache_op_level_e'(cache_op_byp_dest_level_i);
    req_byp_new.count = cache_op_byp_count_i;
    req_byp_new.addr = cache_op_byp_addr_i;
    req_byp_new.stride = cache_op_byp_stride_i;
    req_byp_new.thread_id = cache_op_byp_thread_id_i;
    req_byp_new.use_tmask = cache_op_byp_use_tmask_i;
    req_byp_new.tmask_bits = cache_op_byp_tmask_bits_i;
    req_byp_new.l2_scp_dest = cache_op_byp_l2_scp_dest_i;
    req_byp_new.l2_scp_fill = cache_op_byp_l2_scp_fill_i;

    req_orig = '0;
    req_orig.cmd = CacheOpControl_t'(cache_op_cmd_i);
    req_orig.start_level = CacheOpLevel_t'(cache_op_start_level_i);
    req_orig.dest_level = CacheOpLevel_t'(cache_op_dest_level_i);
    req_orig.count = cache_op_count_i;
    req_orig.addr = cache_op_addr_i;
    req_orig.stride = cache_op_stride_i;
    req_orig.thread_id = cache_op_thread_id_i;
    req_orig.use_tmask = cache_op_use_tmask_i;
    req_orig.tmask_ready = cache_op_tmask_ready_i;
    req_orig.tmask_bits = cache_op_tmask_bits_i;
    req_orig.l2_scp_dest = cache_op_l2_scp_dest_i;
    req_orig.l2_scp_fill = cache_op_l2_scp_fill_i;

    req_next_orig = '0;
    req_next_orig.cmd = CacheOpControl_t'(cache_op_cmd_i);
    req_next_orig.start_level = CacheOpLevel_t'(cache_op_start_level_i);
    req_next_orig.dest_level = CacheOpLevel_t'(cache_op_dest_level_i);

    req_byp_orig = '0;
    req_byp_orig.cmd = CacheOpControl_t'(cache_op_byp_cmd_i);
    req_byp_orig.start_level = CacheOpLevel_t'(cache_op_byp_start_level_i);
    req_byp_orig.dest_level = CacheOpLevel_t'(cache_op_byp_dest_level_i);
    req_byp_orig.count = cache_op_byp_count_i;
    req_byp_orig.addr = cache_op_byp_addr_i;
    req_byp_orig.stride = cache_op_byp_stride_i;
    req_byp_orig.thread_id = cache_op_byp_thread_id_i;
    req_byp_orig.use_tmask = cache_op_byp_use_tmask_i;
    req_byp_orig.tmask_bits = cache_op_byp_tmask_bits_i;
    req_byp_orig.l2_scp_dest = cache_op_byp_l2_scp_dest_i;
    req_byp_orig.l2_scp_fill = cache_op_byp_l2_scp_fill_i;

    l2_resp_new = '0;
    l2_resp_new.id = et_link_core_miss_id_e'(l2_resp_id_i[4:0]);
    l2_resp_new.opcode = et_link_rsp_opcode_e'(l2_resp_opcode_i);

    l2_resp_orig = '0;
    l2_resp_orig.id = et_link_core_miss_id_t'(l2_resp_id_i[4:0]);
    l2_resp_orig.opcode = et_link_rsp_opcode_t'(l2_resp_opcode_i);

    for (int unsigned i = 0; i < NrThreads; i++) begin
      vm_status_new[i] = '0;
      vm_status_new[i].debug = vm_status_debug_i[i];
      vm_status_new[i].sum = vm_status_sum_i[i];
      vm_status_new[i].mxr = vm_status_mxr_i[i];
      vm_status_new[i].prv = vm_status_prv_i[i];
      vm_status_new[i].mpp = vm_status_mpp_i[i];
      vm_status_new[i].mprv = vm_status_mprv_i[i];

      vm_status_orig[i] = '0;
      vm_status_orig[i].debug = vm_status_debug_i[i];
      vm_status_orig[i].sum = vm_status_sum_i[i];
      vm_status_orig[i].mxr = vm_status_mxr_i[i];
      vm_status_orig[i].prv = vm_status_prv_i[i];
      vm_status_orig[i].mpp = vm_status_mpp_i[i];
      vm_status_orig[i].mprv = vm_status_mprv_i[i];
    end

    vm_status_co_new = '0;
    vm_status_co_new.prv = vm_status_co_prv_i;
    vm_status_co_new.mpp = vm_status_co_mpp_i;
    vm_status_co_new.mprv = vm_status_co_mprv_i;
    vm_status_co_new.sum = vm_status_co_sum_i;
    vm_status_co_new.mxr = vm_status_co_mxr_i;
    vm_status_co_new.debug = vm_status_co_debug_i;

    vm_status_co_orig = '0;
    vm_status_co_orig.prv = vm_status_co_prv_i;
    vm_status_co_orig.mpp = vm_status_co_mpp_i;
    vm_status_co_orig.mprv = vm_status_co_mprv_i;
    vm_status_co_orig.sum = vm_status_co_sum_i;
    vm_status_co_orig.mxr = vm_status_co_mxr_i;
    vm_status_co_orig.debug = vm_status_co_debug_i;
  end

  assign orig_l2_req_id_o = l2_req_orig.id;
  assign new_l2_req_id_o = l2_req_new.id;
  assign orig_l2_req_opcode_o = l2_req_orig.opcode;
  assign new_l2_req_opcode_o = l2_req_new.opcode;
  assign orig_l2_req_subopcode_o = l2_req_orig.subopcode;
  assign new_l2_req_subopcode_o = l2_req_new.subopcode;
  assign orig_l2_req_address_o = l2_req_orig.address;
  assign new_l2_req_address_o = l2_req_new.address;
  assign {orig_l2_req_data3_o, orig_l2_req_data2_o, orig_l2_req_data1_o, orig_l2_req_data0_o} = l2_req_orig.data;
  assign {new_l2_req_data3_o, new_l2_req_data2_o, new_l2_req_data1_o, new_l2_req_data0_o} = l2_req_new.data;
  assign orig_l2_req_size_o = l2_req_orig.size;
  assign new_l2_req_size_o = l2_req_new.size;
  assign orig_l2_req_qwen_o = l2_req_orig.qwen;
  assign new_l2_req_qwen_o = l2_req_new.qwen;

  assign orig_vm_status_l2_prv_o = vm_status_l2_orig.prv;
  assign new_vm_status_l2_prv_o = vm_status_l2_new.prv;
  assign orig_vm_status_l2_mpp_o = vm_status_l2_orig.mpp;
  assign new_vm_status_l2_mpp_o = vm_status_l2_new.mpp;
  assign orig_vm_status_l2_mprv_o = vm_status_l2_orig.mprv;
  assign new_vm_status_l2_mprv_o = vm_status_l2_new.mprv;
  assign orig_vm_status_l2_sum_o = vm_status_l2_orig.sum;
  assign new_vm_status_l2_sum_o = vm_status_l2_new.sum;
  assign orig_vm_status_l2_mxr_o = vm_status_l2_orig.mxr;
  assign new_vm_status_l2_mxr_o = vm_status_l2_new.mxr;
  assign orig_vm_status_l2_debug_o = vm_status_l2_orig.debug;
  assign new_vm_status_l2_debug_o = vm_status_l2_new.debug;

  assign orig_sm_match_chunk0_o = sm_match_orig;
  assign new_sm_match_chunk0_o = sm_match_new;
  assign orig_sm_filter_chunk0_o = sm_filter_orig[63:0];
  assign orig_sm_filter_chunk1_o = sm_filter_orig[127:64];
  assign orig_sm_filter_chunk2_o = sm_filter_orig[191:128];
  assign orig_sm_filter_chunk3_o = {56'b0, sm_filter_orig[199:192]};
  assign new_sm_filter_chunk0_o = sm_filter_new[63:0];
  assign new_sm_filter_chunk1_o = sm_filter_new[127:64];
  assign new_sm_filter_chunk2_o = sm_filter_new[191:128];
  assign new_sm_filter_chunk3_o = {56'b0, sm_filter_new[199:192]};
  assign orig_sm_data_chunk0_o = sm_data_orig[63:0];
  assign orig_sm_data_chunk1_o = sm_data_orig[127:64];
  assign orig_sm_data_chunk2_o = sm_data_orig[191:128];
  assign orig_sm_data_chunk3_o = sm_data_orig[255:192];
  assign orig_sm_data_chunk4_o = sm_data_orig[319:256];
  assign orig_sm_data_chunk5_o = sm_data_orig[383:320];
  assign orig_sm_data_chunk6_o = sm_data_orig[447:384];
  assign orig_sm_data_chunk7_o = sm_data_orig[511:448];
  assign new_sm_data_chunk0_o = sm_data_new[63:0];
  assign new_sm_data_chunk1_o = sm_data_new[127:64];
  assign new_sm_data_chunk2_o = sm_data_new[191:128];
  assign new_sm_data_chunk3_o = sm_data_new[255:192];
  assign new_sm_data_chunk4_o = sm_data_new[319:256];
  assign new_sm_data_chunk5_o = sm_data_new[383:320];
  assign new_sm_data_chunk6_o = sm_data_new[447:384];
  assign new_sm_data_chunk7_o = sm_data_new[511:448];

  minion_dcache_cache_op_unit_l2 u_new (
    .clk_i,
    .rst_ni,
    .shire_id_i,
    .cacheop_max_i,
    .cacheop_reprate_i,
    .chicken_bit_dcache_i,
    .busy_o(new_busy_o),
    .th_id_complete_o(new_th_id_complete_o),
    .l2_resp_valid_i,
    .l2_resp_i(l2_resp_new),
    .cache_op_ready_o(new_cache_op_ready_o),
    .cache_op_valid_i,
    .cache_op_req_i(req_new),
    .cache_op_req_next_i(req_next_new),
    .l2_req_ready_i,
    .l2_req_valid_o(new_l2_req_valid_o),
    .l2_req_data_o(l2_req_new),
    .cache_op_ready_byp_o(new_cache_op_ready_byp_o),
    .cache_op_valid_byp_i,
    .cache_op_req_byp_i(req_byp_new),
    .s0_available_i,
    .s0_addr_load_o(new_s0_addr_load_o),
    .s0_addr_o(new_s0_addr_o),
    .s0_addr_msb_err_o(new_s0_addr_msb_err_o),
    .s0_thread_id_o(new_s0_thread_id_o),
    .s1_available_i,
    .s1_addr_load_o(new_s1_addr_load_o),
    .s1_tlb_wait_i,
    .s1_tlb_fail_i,
    .s1_tlb_addr_i,
    .vm_status_i(vm_status_new),
    .vm_status_co_i(vm_status_co_new),
    .vm_status_l2_o(vm_status_l2_new),
    .err_flags_o(new_err_flags_o),
    .bus_err_o(new_bus_err_o),
    .csr_debug_signals_o(new_csr_debug_signals_o),
    .sm_match_debug_signals_o(sm_match_new),
    .sm_filter_debug_signals_o(sm_filter_new),
    .sm_data_debug_signals_o(sm_data_new)
  );

  dcache_cache_op_unit_l2 u_orig (
    .clock(clk_i),
    .reset(!rst_ni),
    .shire_id(shire_id_i),
    .cacheop_max(cacheop_max_i),
    .cacheop_reprate(cacheop_reprate_i),
    .chicken_bit_dcache(chicken_bit_dcache_i),
    .busy(orig_busy_o),
    .th_id_complete(orig_th_id_complete_o),
    .l2_resp_valid(l2_resp_valid_i),
    .l2_resp(l2_resp_orig),
    .cache_op_ready(orig_cache_op_ready_o),
    .cache_op_valid(cache_op_valid_i),
    .cache_op_req(req_orig),
    .cache_op_req_next(req_next_orig),
    .l2_req_ready(l2_req_ready_i),
    .l2_req_valid(orig_l2_req_valid_o),
    .l2_req_data(l2_req_orig),
    .cache_op_ready_byp(orig_cache_op_ready_byp_o),
    .cache_op_valid_byp(cache_op_valid_byp_i),
    .cache_op_req_byp(req_byp_orig),
    .s0_available(s0_available_i),
    .s0_addr_load(orig_s0_addr_load_o),
    .s0_addr(orig_s0_addr_o),
    .s0_addr_msb_err(orig_s0_addr_msb_err_o),
    .s0_thread_id(orig_s0_thread_id_o),
    .s1_available(s1_available_i),
    .s1_addr_load(orig_s1_addr_load_o),
    .s1_tlb_wait(s1_tlb_wait_i),
    .s1_tlb_fail(s1_tlb_fail_i),
    .s1_tlb_addr(s1_tlb_addr_i),
    .vm_status(vm_status_orig),
    .vm_status_co(vm_status_co_orig),
    .vm_status_l2(vm_status_l2_orig),
    .err_flags(orig_err_flags_o),
    .bus_err(orig_bus_err_o),
    .csr_debug_signals(orig_csr_debug_signals_o),
    .sm_match_debug_signals(sm_match_orig),
    .sm_filter_debug_signals(sm_filter_orig),
    .sm_data_debug_signals(sm_data_orig)
  );

endmodule : cosim_minion_dcache_cache_op_unit_l2_tb
