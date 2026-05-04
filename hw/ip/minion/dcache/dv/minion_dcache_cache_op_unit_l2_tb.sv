// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit-test wrapper for minion_dcache_cache_op_unit_l2.

module minion_dcache_cache_op_unit_l2_tb
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                             clk_i,
  input  logic                             rst_ni,
  input  logic [NumShireIdsBits-1:0]       shire_id_i,
  input  logic [DcacheCoL2MaxOpsBits-1:0]  cacheop_max_i,
  input  logic [DcacheCoL2ReprateBits-1:0] cacheop_reprate_i,
  input  logic                             chicken_bit_dcache_i,
  output logic                             busy_o,
  output logic [NrThreads-1:0][1:0]        th_id_complete_o,
  input  logic                             l2_resp_valid_i,
  input  logic [7:0]                       l2_resp_id_i,
  input  logic [1:0]                       l2_resp_opcode_i,
  output logic                             cache_op_ready_o,
  input  logic                             cache_op_valid_i,
  input  logic [3:0]                       cache_op_cmd_i,
  input  logic [1:0]                       cache_op_start_level_i,
  input  logic [1:0]                       cache_op_dest_level_i,
  input  logic [3:0]                       cache_op_count_i,
  input  logic [VaSize-1:0]                cache_op_addr_i,
  input  logic [VaSize-1:0]                cache_op_stride_i,
  input  logic                             cache_op_thread_id_i,
  input  logic                             cache_op_use_tmask_i,
  input  logic                             cache_op_tmask_ready_i,
  input  logic [TensorMaskBits-1:0]        cache_op_tmask_bits_i,
  input  logic [DcacheL2ScpDstBits-1:0]    cache_op_l2_scp_dest_i,
  input  logic                             cache_op_l2_scp_fill_i,
  input  logic                             l2_req_ready_i,
  output logic                             l2_req_valid_o,
  output logic [4:0]                       l2_req_id_o,
  output logic [4:0]                       l2_req_opcode_o,
  output logic [6:0]                       l2_req_subopcode_o,
  output logic [PaSize-1:0]                l2_req_address_o,
  output logic [CoreL2BlockExtSize-1:0]    l2_req_data_o,
  output logic [6:0]                       l2_req_data_low_o,
  output logic [2:0]                       l2_req_size_o,
  output logic [EtLinkQwenSize-1:0]        l2_req_qwen_o,
  output logic                             cache_op_ready_byp_o,
  input  logic                             cache_op_valid_byp_i,
  input  logic [3:0]                       cache_op_byp_cmd_i,
  input  logic [1:0]                       cache_op_byp_start_level_i,
  input  logic [1:0]                       cache_op_byp_dest_level_i,
  input  logic [3:0]                       cache_op_byp_count_i,
  input  logic [VaSize-1:0]                cache_op_byp_addr_i,
  input  logic [VaSize-1:0]                cache_op_byp_stride_i,
  input  logic                             cache_op_byp_thread_id_i,
  input  logic                             cache_op_byp_use_tmask_i,
  input  logic [TensorMaskBits-1:0]        cache_op_byp_tmask_bits_i,
  input  logic [DcacheL2ScpDstBits-1:0]    cache_op_byp_l2_scp_dest_i,
  input  logic                             cache_op_byp_l2_scp_fill_i,
  input  logic                             s0_available_i,
  output logic                             s0_addr_load_o,
  output logic [VaSize-1:0]                s0_addr_o,
  output logic                             s0_addr_msb_err_o,
  output logic                             s0_thread_id_o,
  input  logic                             s1_available_i,
  output logic                             s1_addr_load_o,
  input  logic                             s1_tlb_wait_i,
  input  logic                             s1_tlb_fail_i,
  input  logic [PaSize-PaUntransSize-1:0]  s1_tlb_addr_i,
  input  logic [NrThreads-1:0]             vm_status_debug_i,
  input  logic [NrThreads-1:0]             vm_status_sum_i,
  input  logic [NrThreads-1:0]             vm_status_mxr_i,
  input  logic [NrThreads-1:0][1:0]        vm_status_prv_i,
  input  logic [NrThreads-1:0][1:0]        vm_status_mpp_i,
  input  logic [NrThreads-1:0]             vm_status_mprv_i,
  input  logic [1:0]                       vm_status_co_prv_i,
  input  logic [1:0]                       vm_status_co_mpp_i,
  input  logic                             vm_status_co_mprv_i,
  input  logic                             vm_status_co_sum_i,
  input  logic                             vm_status_co_mxr_i,
  input  logic                             vm_status_co_debug_i,
  output logic [1:0]                       vm_status_l2_prv_o,
  output logic [1:0]                       vm_status_l2_mpp_o,
  output logic                             vm_status_l2_mprv_o,
  output logic                             vm_status_l2_sum_o,
  output logic                             vm_status_l2_mxr_o,
  output logic                             vm_status_l2_debug_o,
  output logic [NrThreads-1:0]             err_flags_o,
  output logic [NrThreads-1:0]             bus_err_o,
  output logic [DcacheDbgCsrCoul2Size-1:0] csr_debug_signals_o
);

  core_dcache_op_req_t      cache_op_req;
  core_dcache_op_req_next_t cache_op_req_next;
  core_dcache_op_req_t      cache_op_req_byp;
  et_link_minion_rsp_info_t l2_resp;
  /* verilator lint_off UNUSEDSIGNAL */  // The wrapper only exposes the cache-op-L2 fields this test checks.
  et_link_minion_evict_req_info_t l2_req_data;
  vm_status_t [NrThreads-1:0] vm_status;
  vm_status_t               vm_status_co;
  vm_status_t               vm_status_l2;
  logic [DcacheDbgSmMatchBits-1:0] sm_match_debug_signals;
  logic [DcacheDbgSmFilterBits-1:0] sm_filter_debug_signals;
  logic [DcacheDbgSmDataBits-1:0] sm_data_debug_signals;
  /* verilator lint_on UNUSEDSIGNAL */

  always_comb begin
    cache_op_req = '0;
    cache_op_req.cmd = cache_op_cmd_e'(cache_op_cmd_i);
    cache_op_req.start_level = cache_op_level_e'(cache_op_start_level_i);
    cache_op_req.dest_level = cache_op_level_e'(cache_op_dest_level_i);
    cache_op_req.count = cache_op_count_i;
    cache_op_req.addr = cache_op_addr_i;
    cache_op_req.stride = cache_op_stride_i;
    cache_op_req.thread_id = cache_op_thread_id_i;
    cache_op_req.use_tmask = cache_op_use_tmask_i;
    cache_op_req.tmask_ready = cache_op_tmask_ready_i;
    cache_op_req.tmask_bits = cache_op_tmask_bits_i;
    cache_op_req.l2_scp_dest = cache_op_l2_scp_dest_i;
    cache_op_req.l2_scp_fill = cache_op_l2_scp_fill_i;

    cache_op_req_next = '0;
    cache_op_req_next.cmd = cache_op_cmd_e'(cache_op_cmd_i);
    cache_op_req_next.start_level = cache_op_level_e'(cache_op_start_level_i);
    cache_op_req_next.dest_level = cache_op_level_e'(cache_op_dest_level_i);

    cache_op_req_byp = '0;
    cache_op_req_byp.cmd = cache_op_cmd_e'(cache_op_byp_cmd_i);
    cache_op_req_byp.start_level = cache_op_level_e'(cache_op_byp_start_level_i);
    cache_op_req_byp.dest_level = cache_op_level_e'(cache_op_byp_dest_level_i);
    cache_op_req_byp.count = cache_op_byp_count_i;
    cache_op_req_byp.addr = cache_op_byp_addr_i;
    cache_op_req_byp.stride = cache_op_byp_stride_i;
    cache_op_req_byp.thread_id = cache_op_byp_thread_id_i;
    cache_op_req_byp.use_tmask = cache_op_byp_use_tmask_i;
    cache_op_req_byp.tmask_bits = cache_op_byp_tmask_bits_i;
    cache_op_req_byp.l2_scp_dest = cache_op_byp_l2_scp_dest_i;
    cache_op_req_byp.l2_scp_fill = cache_op_byp_l2_scp_fill_i;

    l2_resp = '0;
    l2_resp.id = l2_resp_id_i;
    l2_resp.opcode = et_link_rsp_opcode_e'(l2_resp_opcode_i);

    for (int unsigned i = 0; i < NrThreads; i++) begin
      vm_status[i] = '0;
      vm_status[i].debug = vm_status_debug_i[i];
      vm_status[i].sum = vm_status_sum_i[i];
      vm_status[i].mxr = vm_status_mxr_i[i];
      vm_status[i].prv = vm_status_prv_i[i];
      vm_status[i].mpp = vm_status_mpp_i[i];
      vm_status[i].mprv = vm_status_mprv_i[i];
    end

    vm_status_co = '0;
    vm_status_co.prv = vm_status_co_prv_i;
    vm_status_co.mpp = vm_status_co_mpp_i;
    vm_status_co.mprv = vm_status_co_mprv_i;
    vm_status_co.sum = vm_status_co_sum_i;
    vm_status_co.mxr = vm_status_co_mxr_i;
    vm_status_co.debug = vm_status_co_debug_i;

    l2_req_id_o = l2_req_data.id;
    l2_req_opcode_o = l2_req_data.opcode;
    l2_req_subopcode_o = l2_req_data.subopcode;
    l2_req_address_o = l2_req_data.address;
    l2_req_data_o = l2_req_data.data;
    l2_req_data_low_o = l2_req_data.data[6:0];
    l2_req_size_o = l2_req_data.size;
    l2_req_qwen_o = l2_req_data.qwen;

    vm_status_l2_prv_o = vm_status_l2.prv;
    vm_status_l2_mpp_o = vm_status_l2.mpp;
    vm_status_l2_mprv_o = vm_status_l2.mprv;
    vm_status_l2_sum_o = vm_status_l2.sum;
    vm_status_l2_mxr_o = vm_status_l2.mxr;
    vm_status_l2_debug_o = vm_status_l2.debug;
  end

  minion_dcache_cache_op_unit_l2 u_dut (
    .clk_i,
    .rst_ni,
    .shire_id_i,
    .cacheop_max_i,
    .cacheop_reprate_i,
    .chicken_bit_dcache_i,
    .busy_o,
    .th_id_complete_o,
    .l2_resp_valid_i,
    .l2_resp_i            (l2_resp),
    .cache_op_ready_o,
    .cache_op_valid_i,
    .cache_op_req_i       (cache_op_req),
    .cache_op_req_next_i  (cache_op_req_next),
    .l2_req_ready_i,
    .l2_req_valid_o,
    .l2_req_data_o         (l2_req_data),
    .cache_op_ready_byp_o,
    .cache_op_valid_byp_i,
    .cache_op_req_byp_i   (cache_op_req_byp),
    .s0_available_i,
    .s0_addr_load_o,
    .s0_addr_o,
    .s0_addr_msb_err_o,
    .s0_thread_id_o,
    .s1_available_i,
    .s1_addr_load_o,
    .s1_tlb_wait_i,
    .s1_tlb_fail_i,
    .s1_tlb_addr_i,
    .vm_status_i           (vm_status),
    .vm_status_co_i        (vm_status_co),
    .vm_status_l2_o        (vm_status_l2),
    .err_flags_o,
    .bus_err_o,
    .csr_debug_signals_o,
    .sm_match_debug_signals_o(sm_match_debug_signals),
    .sm_filter_debug_signals_o(sm_filter_debug_signals),
    .sm_data_debug_signals_o(sm_data_debug_signals)
  );

endmodule : minion_dcache_cache_op_unit_l2_tb
