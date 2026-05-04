// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Minion dcache cache-op L2 unit.
//
// Replaces: dcache_cache_op_unit_l2

/* verilator lint_off UNUSEDSIGNAL */  // This preserved cache-op leaf receives full original structs; several fields remain intentionally unused in this block.
module minion_dcache_cache_op_unit_l2
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                                         clk_i,
  input  logic                                         rst_ni,
  input  logic [NumShireIdsBits-1:0]                   shire_id_i,
  input  logic [DcacheCoL2MaxOpsBits-1:0]              cacheop_max_i,
  input  logic [DcacheCoL2ReprateBits-1:0]             cacheop_reprate_i,
  input  logic                                         chicken_bit_dcache_i,
  output logic                                         busy_o,
  output logic [NrThreads-1:0][1:0]                    th_id_complete_o,
  input  logic                                         l2_resp_valid_i,
  input  et_link_minion_rsp_info_t                     l2_resp_i,
  output logic                                         cache_op_ready_o,
  input  logic                                         cache_op_valid_i,
  input  core_dcache_op_req_t                          cache_op_req_i,
  input  core_dcache_op_req_next_t                     cache_op_req_next_i,
  input  logic                                         l2_req_ready_i,
  output logic                                         l2_req_valid_o,
  output et_link_minion_evict_req_info_t               l2_req_data_o,
  output logic                                         cache_op_ready_byp_o,
  input  logic                                         cache_op_valid_byp_i,
  input  core_dcache_op_req_t                          cache_op_req_byp_i,
  input  logic                                         s0_available_i,
  output logic                                         s0_addr_load_o,
  output logic [minion_dcache_pkg::VaSize-1:0]                            s0_addr_o,
  output logic                                         s0_addr_msb_err_o,
  output logic                                         s0_thread_id_o,
  input  logic                                         s1_available_i,
  output logic                                         s1_addr_load_o,
  input  logic                                         s1_tlb_wait_i,
  input  logic                                         s1_tlb_fail_i,
  input  logic [minion_dcache_pkg::PaSize-minion_dcache_pkg::PaUntransSize-1:0]              s1_tlb_addr_i,
  input  minion_pkg::vm_status_t [NrThreads-1:0]       vm_status_i,
  input  minion_pkg::vm_status_t                       vm_status_co_i,
  output minion_pkg::vm_status_t                       vm_status_l2_o,
  output logic [NrThreads-1:0]                         err_flags_o,
  output logic [NrThreads-1:0]                         bus_err_o,
  output logic [minion_dcache_pkg::DcacheDbgCsrCoul2Size-1:0]             csr_debug_signals_o,
  output logic [DcacheDbgSmMatchBits-1:0]              sm_match_debug_signals_o,
  output logic [DcacheDbgSmFilterBits-1:0]             sm_filter_debug_signals_o,
  output logic [DcacheDbgSmDataBits-1:0]               sm_data_debug_signals_o
);

  localparam int unsigned TlbAddrWidth = minion_dcache_pkg::PaSize - minion_dcache_pkg::PaUntransSize;
  localparam int unsigned FilterRawWidth = 1 + 1 + 1 + DcacheCoL2MaxOpsBits + 1 + 1 + minion_dcache_pkg::VaSize
      + $bits(l2_cache_op_req_t) + $bits(l2_cache_op_state_e);
  localparam int unsigned Data0RawWidth = DcacheL2ScpDstBits + 4 + minion_dcache_pkg::VaSize + $bits(l2_cache_op_req_t);
  localparam int unsigned Data1RawWidth =
      1 + 1 + (NrThreads * 2 * DcacheCoL2MaxOpsBits) + DcacheCoL2MaxOpsBits;

  l2_cache_op_state_e                      state_q, state_d;
  l2_cache_op_req_t                        req_q, req_d;
  logic [minion_dcache_pkg::VaSize-1:0]                       req_addr_q, req_addr_d;
  logic                                    req_addr_msb_err_q, req_addr_msb_err_d;
  logic [3:0]                              req_count_q, req_count_d;
  logic [TensorMaskBits-1:0]               req_tmask_bits_q, req_tmask_bits_d;
  logic                                    id_running_q, id_running_d;
  logic [DcacheL2ScpDstBits-1:0]           l2_scp_dest_q, l2_scp_dest_d;
  logic                                    err_mem_q, err_mem_d;
  logic                                    cache_op_req_is_l2_q, cache_op_req_is_l2_d;
  logic                                    cache_op_req_is_nop_q, cache_op_req_is_nop_d;
  logic                                    new_req;
  logic                                    up_addr;
  logic                                    up_id;
  logic                                    en_count;
  logic                                    tmask_en;
  logic [minion_dcache_pkg::VaSize-1:0]                       adder_res;
  logic [minion_dcache_pkg::VaSize-1:0]                       virtual_addr;
  logic [minion_dcache_pkg::PaSize-1:0]                       translated_addr;
  logic [minion_dcache_pkg::PaSize-1:0]                       l2_scp_address;
  logic                                    l2_scp_bad_source;
  logic                                    s1_addr_load_pre_q;
  logic [TlbAddrWidth-1:0]                 reg_tlb_addr_q;
  logic [DcacheCoL2MaxOpsBits-1:0]         l2_cache_op_cnt_q, l2_cache_op_cnt_d;
  logic                                    l2_resp_is_prefetch;
  logic [DcacheCoL2CycleBits-1:0]          cacheop_reprate_count_q, cacheop_reprate_count_d;
  logic                                    cacheop_reprate_count_up;
  logic [DcacheCoL2GrantBits-1:0]          cacheop_reprate_grant_q, cacheop_reprate_grant_d;
  logic                                    cacheop_reprate_grant_up;
  logic                                    l2_req_valid_started_q, l2_req_valid_started_d;
  logic                                    l2_req_valid_started_up;
  et_link_req_opcode_e                     co_cmd_up;
  logic                                    l2_req_throttle;
  logic [NrThreads-1:0][1:0][DcacheCoL2MaxOpsBits-1:0] ready_counters_q, ready_counters_d;
  logic [NrThreads-1:0][1:0]               increment_ready_counters;
  logic [NrThreads-1:0][1:0]               decrement_ready_counters;
  logic                                    ready_counters_up;
  logic                                    resp_th;
  logic                                    resp_id;
  logic                                    clkgt;
  logic                                    clkgt_en;
  logic [FilterRawWidth-1:0]               sm_filter_raw;
  logic [Data0RawWidth-1:0]                sm_data0_raw;
  logic [Data1RawWidth-1:0]                sm_data1_raw;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      cache_op_req_is_l2_q <= 1'b0;
      cache_op_req_is_nop_q <= 1'b0;
    end else begin
      cache_op_req_is_l2_q <= cache_op_req_is_l2_d;
      cache_op_req_is_nop_q <= cache_op_req_is_nop_d;
    end
  end

  always_ff @(posedge clkgt or negedge rst_ni) begin
    if (!rst_ni) begin
      state_q <= L2CacheOpInvalid;
      req_q <= '0;
      req_addr_q <= '0;
      req_addr_msb_err_q <= 1'b0;
      req_count_q <= '0;
      req_tmask_bits_q <= '0;
      id_running_q <= 1'b0;
      l2_scp_dest_q <= '0;
      err_mem_q <= 1'b0;
      vm_status_l2_o <= '0;
      s1_addr_load_pre_q <= 1'b0;
      reg_tlb_addr_q <= '0;
      l2_cache_op_cnt_q <= '0;
      cacheop_reprate_count_q <= '0;
      cacheop_reprate_grant_q <= '0;
      l2_req_valid_started_q <= 1'b0;
      ready_counters_q <= '0;
    end else begin
      state_q <= state_d;
      if (new_req) begin
        req_q <= req_d;
        vm_status_l2_o <= cache_op_valid_byp_i ? vm_status_co_i
                     : (cache_op_req_i.thread_id ? vm_status_i[1] : vm_status_i[0]);
      end
      if (up_addr) begin
        req_addr_q <= req_addr_d;
        req_addr_msb_err_q <= req_addr_msb_err_d;
        l2_scp_dest_q <= l2_scp_dest_d;
      end
      if (en_count) begin
        req_count_q <= req_count_d;
      end
      if (tmask_en) begin
        req_tmask_bits_q <= req_tmask_bits_d;
      end
      if (up_id) begin
        id_running_q <= id_running_d;
      end
      err_mem_q <= err_mem_d;
      s1_addr_load_pre_q <= s0_addr_load_o;
      if (s1_addr_load_o) begin
        reg_tlb_addr_q <= s1_tlb_addr_i;
      end
      l2_cache_op_cnt_q <= l2_cache_op_cnt_d;
      if (cacheop_reprate_count_up) begin
        cacheop_reprate_count_q <= cacheop_reprate_count_d;
      end
      if (cacheop_reprate_grant_up) begin
        cacheop_reprate_grant_q <= cacheop_reprate_grant_d;
      end
      if (l2_req_valid_started_up) begin
        l2_req_valid_started_q <= l2_req_valid_started_d;
      end
      if (ready_counters_up) begin
        ready_counters_q <= ready_counters_d;
      end
    end
  end

  always_comb begin
    cache_op_req_is_l2_d =
        ((cache_op_req_next_i.cmd == cache_op_Pref) && (cache_op_req_next_i.dest_level >= cache_op_L2)
            && (cache_op_req_next_i.dest_level < cache_op_MEM))
        || ((cache_op_req_next_i.cmd == cache_op_Flush_VA)
            && (cache_op_req_next_i.start_level >= cache_op_L2)
            && (cache_op_req_next_i.dest_level > cache_op_req_next_i.start_level))
        || ((cache_op_req_next_i.cmd == cache_op_Evict_VA)
            && (cache_op_req_next_i.start_level >= cache_op_L2)
            && (cache_op_req_next_i.dest_level > cache_op_req_next_i.start_level));

    cache_op_req_is_nop_d =
        ((cache_op_req_next_i.cmd == cache_op_Pref) && (cache_op_req_next_i.dest_level == cache_op_MEM))
        || ((cache_op_req_next_i.cmd == cache_op_Flush_VA)
            && (cache_op_req_next_i.start_level >= cache_op_L2)
            && (cache_op_req_next_i.dest_level <= cache_op_req_next_i.start_level))
        || ((cache_op_req_next_i.cmd == cache_op_Evict_VA)
            && (cache_op_req_next_i.start_level >= cache_op_L2)
            && (cache_op_req_next_i.dest_level <= cache_op_req_next_i.start_level));
  end

  always_comb begin
    state_d = state_q;
    err_mem_d = 1'b0;

    unique case (state_q)
      L2CacheOpInvalid: begin
        if (cache_op_valid_byp_i) begin
          state_d = L2CacheOpRequest;
        end else if (cache_op_valid_i && cache_op_req_is_l2_q) begin
          if (!cache_op_req_i.use_tmask) begin
            state_d = L2CacheOpTranslate;
          end else begin
            state_d = L2CacheOpWaitTmask;
          end
        end
      end
      L2CacheOpWaitTmask: begin
        if (cache_op_req_i.tmask_ready) begin
          if (cache_op_req_i.tmask_bits[0]) begin
            state_d = L2CacheOpTranslate;
          end else if (req_count_q != req_q.count) begin
            state_d = L2CacheOpNextOperation;
          end else begin
            state_d = L2CacheOpInvalid;
          end
        end
      end
      L2CacheOpTranslate: begin
        if (s1_addr_load_o) begin
          if (s1_tlb_fail_i) begin
            state_d = L2CacheOpInvalid;
            err_mem_d = 1'b1;
          end else if (s1_tlb_wait_i) begin
            state_d = L2CacheOpTranslateWait;
          end else begin
            state_d = L2CacheOpRequest;
          end
        end
      end
      L2CacheOpTranslateWait: begin
        if (!s1_tlb_wait_i) begin
          state_d = L2CacheOpTranslate;
        end
      end
      L2CacheOpRequest: begin
        if ((l2_req_valid_o && l2_req_ready_i) || (req_q.is_l2_scp && l2_scp_bad_source)) begin
          if (req_count_q == req_q.count) begin
            state_d = L2CacheOpInvalid;
          end else begin
            state_d = L2CacheOpNextOperation;
          end
        end
      end
      L2CacheOpNextOperation: begin
        if (!req_q.use_tmask) begin
          state_d = L2CacheOpTranslate;
        end else if (req_tmask_bits_q[req_count_q]) begin
          state_d = L2CacheOpTranslate;
        end else if (req_count_q == req_q.count) begin
          state_d = L2CacheOpInvalid;
        end
      end
      default: begin
        state_d = L2CacheOpInvalid;
      end
    endcase
  end

  assign err_flags_o = {err_mem_q && req_q.thread_id, err_mem_q && !req_q.thread_id};

  always_comb begin
    new_req = (state_q == L2CacheOpInvalid) && (state_d != L2CacheOpInvalid);
    up_addr = new_req;
    up_id = (state_q == L2CacheOpInvalid)
         && ((cache_op_valid_i && cache_op_ready_o) || (cache_op_valid_byp_i && cache_op_ready_byp_o));

    if (cache_op_valid_byp_i) begin
      req_addr_d = cache_op_req_byp_i.addr;
      req_d.thread_id = cache_op_req_byp_i.thread_id;
      req_d.cmd = cache_op_req_byp_i.cmd;
      req_d.start_level = cache_op_req_byp_i.start_level;
      req_d.dest_level = cache_op_req_byp_i.dest_level;
      req_d.stride = cache_op_req_byp_i.stride[minion_dcache_pkg::VaSize-1:6];
      req_d.use_tmask = cache_op_req_byp_i.use_tmask;
      req_d.count = cache_op_req_byp_i.count;
      req_d.is_l2_scp = cache_op_req_byp_i.l2_scp_fill;
      req_d.is_byp = 1'b1;
      id_running_d = cache_op_req_byp_i.stride[0];
    end else begin
      req_addr_d = cache_op_req_i.addr;
      req_d.thread_id = cache_op_req_i.thread_id;
      req_d.cmd = cache_op_req_i.cmd;
      req_d.start_level = cache_op_req_i.start_level;
      req_d.dest_level = cache_op_req_i.dest_level;
      req_d.stride = cache_op_req_i.stride[minion_dcache_pkg::VaSize-1:6];
      req_d.use_tmask = cache_op_req_i.use_tmask;
      req_d.count = cache_op_req_i.count;
      req_d.is_l2_scp = cache_op_req_i.l2_scp_fill;
      req_d.is_byp = 1'b0;
      id_running_d = cache_op_req_i.stride[0];
    end

    if (req_q.is_byp) begin
      req_tmask_bits_d = cache_op_req_byp_i.tmask_bits;
    end else begin
      req_tmask_bits_d = cache_op_req_i.tmask_bits;
    end

    req_addr_msb_err_d = 1'b0;
    tmask_en = state_q == L2CacheOpWaitTmask;

    adder_res = virtual_addr + {req_q.stride, 6'b0};
    if (state_d == L2CacheOpNextOperation) begin
      up_addr = 1'b1;
      req_addr_d = adder_res;
      req_addr_msb_err_d = req_addr_q[minion_dcache_pkg::VaSize-1] && !req_addr_d[minion_dcache_pkg::VaSize-1];
    end

    if (new_req) begin
      req_count_d = '0;
      l2_scp_dest_d = cache_op_req_i.l2_scp_dest;
    end else begin
      req_count_d = req_count_q + 4'b1;
      l2_scp_dest_d = l2_scp_dest_q + DcacheL2ScpDstBits'(1'b1);
    end
    en_count = new_req || (state_d == L2CacheOpNextOperation);
  end

  always_comb begin
    s0_addr_load_o = s0_available_i && (state_q == L2CacheOpTranslate) && !s1_addr_load_pre_q;
    s0_thread_id_o = req_q.thread_id;
    s1_addr_load_o = s1_addr_load_pre_q && s1_available_i;
    virtual_addr = {req_addr_q[minion_dcache_pkg::VaSize-1:6], 6'b0};
    s0_addr_o = virtual_addr;
    s0_addr_msb_err_o = req_addr_msb_err_q;
    translated_addr = {reg_tlb_addr_q, virtual_addr[minion_dcache_pkg::PaUntransSize-1:0]};
  end

  assign l2_resp_is_prefetch =
      (et_link_core_miss_id_e'(l2_resp_i.id[4:0]) >= EtLinkCoreMissIdL2Prefetch0)
      && (et_link_core_miss_id_e'(l2_resp_i.id[4:0]) <= EtLinkCoreMissIdL2Prefetch3);

  always_comb begin
    l2_cache_op_cnt_d = l2_cache_op_cnt_q;
    if (l2_req_ready_i) begin
      l2_cache_op_cnt_d = l2_cache_op_cnt_d + DcacheCoL2MaxOpsBits'(1'b1);
    end
    if (l2_resp_valid_i && l2_resp_is_prefetch) begin
      l2_cache_op_cnt_d = l2_cache_op_cnt_d - DcacheCoL2MaxOpsBits'(1'b1);
    end
  end

  always_comb begin
    if (state_q == L2CacheOpInvalid) begin
      cacheop_reprate_count_d = (DcacheCoL2CycleBits'(1'b1) << cacheop_reprate_i)
                              - DcacheCoL2CycleBits'(1'b1);
    end else begin
      cacheop_reprate_count_d = cacheop_reprate_count_q;
      if (l2_req_valid_started_q) begin
        if (cacheop_reprate_count_q != '0) begin
          cacheop_reprate_count_d = cacheop_reprate_count_q - DcacheCoL2CycleBits'(1'b1);
        end else begin
          cacheop_reprate_count_d = (DcacheCoL2CycleBits'(1'b1) << cacheop_reprate_i)
                                  - DcacheCoL2CycleBits'(1'b1);
        end
      end
    end
    cacheop_reprate_count_up = new_req || (state_q != L2CacheOpInvalid);
  end

  always_comb begin
    l2_req_valid_started_d = l2_req_valid_started_q;
    if (state_d == L2CacheOpRequest) begin
      l2_req_valid_started_d = 1'b1;
    end else if (state_q == L2CacheOpInvalid) begin
      l2_req_valid_started_d = 1'b0;
    end
    l2_req_valid_started_up = new_req || (state_q != L2CacheOpInvalid);
  end

  always_comb begin
    if (state_q == L2CacheOpInvalid) begin
      cacheop_reprate_grant_d = DcacheCoL2GrantBits'(1'b1);
    end else begin
      cacheop_reprate_grant_d = cacheop_reprate_grant_q;
      if (l2_req_valid_started_q && (cacheop_reprate_count_q == '0)) begin
        if (cacheop_reprate_grant_d != {DcacheCoL2GrantBits{1'b1}}) begin
          cacheop_reprate_grant_d = cacheop_reprate_grant_d + DcacheCoL2GrantBits'(1'b1);
        end
      end
      if (l2_req_valid_o && l2_req_ready_i) begin
        cacheop_reprate_grant_d = cacheop_reprate_grant_d - DcacheCoL2GrantBits'(1'b1);
      end
    end
    cacheop_reprate_grant_up = new_req || (state_q != L2CacheOpInvalid);
  end

  always_comb begin
    bus_err_o = '0;
    if (l2_resp_valid_i && l2_resp_is_prefetch
        && (l2_resp_i.opcode == minion_pkg::EtLinkRspErr)) begin
      bus_err_o = {req_q.thread_id, !req_q.thread_id};
    end
  end

  always_comb begin
    if ((req_q.cmd == cache_op_Flush_VA) || (req_q.cmd == cache_op_Flush_SW)) begin
      co_cmd_up = EtLinkReqFlush;
    end else if ((req_q.cmd == cache_op_Evict_VA) || (req_q.cmd == cache_op_Evict_SW)) begin
      co_cmd_up = EtLinkReqEvict;
    end else begin
      co_cmd_up = EtLinkReqPrefetch;
    end
  end

  assign l2_scp_address = {AdScpRegion, shire_id_i, l2_scp_dest_q, 6'b0};
  assign l2_scp_bad_source = 1'b0;

  always_comb begin
    cache_op_ready_o = (state_q == L2CacheOpInvalid) && (cache_op_req_is_l2_q || cache_op_req_is_nop_q)
                    && !cache_op_valid_byp_i;
    cache_op_ready_byp_o = state_q == L2CacheOpInvalid;

    l2_req_throttle = ((l2_cache_op_cnt_q >= cacheop_max_i) && (cacheop_max_i != '0))
                   || (cacheop_reprate_grant_q == '0);

    l2_req_valid_o = (state_q == L2CacheOpRequest) && !l2_scp_bad_source && !l2_req_throttle;
    unique case ({req_q.thread_id, id_running_q})
      2'b00: l2_req_data_o.id = EtLinkCoreMissIdL2Prefetch0;
      2'b01: l2_req_data_o.id = EtLinkCoreMissIdL2Prefetch1;
      2'b10: l2_req_data_o.id = EtLinkCoreMissIdL2Prefetch2;
      default: l2_req_data_o.id = EtLinkCoreMissIdL2Prefetch3;
    endcase
    l2_req_data_o.source = 1'b0;
    l2_req_data_o.wdata = 1'b1;
    l2_req_data_o.opcode = req_q.is_l2_scp ? EtLinkReqScpFill : co_cmd_up;
    l2_req_data_o.subopcode = '0;
    l2_req_data_o.address = req_q.is_byp ? virtual_addr[minion_dcache_pkg::PaSize-1:0]
                          : (req_q.is_l2_scp ? l2_scp_address : translated_addr);
    l2_req_data_o.data = '0;
    if (req_q.is_l2_scp) begin
      l2_req_data_o.data[minion_dcache_pkg::PaSize-1:0] = translated_addr;
    end else begin
      l2_req_data_o.data[6:0] = {req_q.dest_level, req_q.start_level, 3'b000};
    end
    l2_req_data_o.size = req_q.is_l2_scp ? EtLinkDWord : EtLinkByte;
    l2_req_data_o.qwen = 4'b0001;
  end

  always_comb begin
    busy_o = state_q != L2CacheOpInvalid;

    increment_ready_counters = '0;
    if (l2_req_ready_i) begin
      increment_ready_counters[req_q.thread_id][id_running_q] = 1'b1;
    end

    decrement_ready_counters = '0;
    if (l2_resp_valid_i && l2_resp_is_prefetch) begin
      unique case (et_link_core_miss_id_e'(l2_resp_i.id[4:0]))
        EtLinkCoreMissIdL2Prefetch0: {resp_th, resp_id} = 2'b00;
        EtLinkCoreMissIdL2Prefetch1: {resp_th, resp_id} = 2'b01;
        EtLinkCoreMissIdL2Prefetch2: {resp_th, resp_id} = 2'b10;
        default: {resp_th, resp_id} = 2'b11;
      endcase
      decrement_ready_counters[resp_th][resp_id] = 1'b1;
    end else begin
      resp_th = 1'b0;
      resp_id = 1'b0;
    end

    ready_counters_up = (|increment_ready_counters) || (|decrement_ready_counters);
    ready_counters_d = ready_counters_q;
    for (int unsigned th = 0; th < NrThreads; th++) begin
      for (int unsigned id = 0; id <= 1; id++) begin
        if (increment_ready_counters[th][id] && !decrement_ready_counters[th][id]
            && !(&ready_counters_q[th][id])) begin
          ready_counters_d[th][id] = ready_counters_q[th][id] + DcacheCoL2MaxOpsBits'(1'b1);
        end else if (!increment_ready_counters[th][id] && decrement_ready_counters[th][id]
                   && (|ready_counters_q[th][id])) begin
          ready_counters_d[th][id] = ready_counters_q[th][id] - DcacheCoL2MaxOpsBits'(1'b1);
        end
      end
    end

    for (int unsigned th = 0; th < NrThreads; th++) begin
      for (int unsigned id = 0; id <= 1; id++) begin
        if ((id_running_q == id[0]) && (req_q.thread_id == th[0])) begin
          th_id_complete_o[th][id] = !busy_o && (ready_counters_q[th][id] == '0);
        end else begin
          th_id_complete_o[th][id] = (ready_counters_q[th][id] == '0);
        end
      end
    end
  end

  assign clkgt_en = new_req || busy_o || !(&th_id_complete_o) || (|err_flags_o);

  prim_clk_gate u_clk_gate (
    .clk_i (clk_i),
    .en_i  (clkgt_en || chicken_bit_dcache_i),
    .dft_i ('0),
    .clk_o (clkgt)
  );

  assign csr_debug_signals_o = state_q;
  assign sm_match_debug_signals_o = '0;

  always_comb begin
    sm_filter_raw = {
      cache_op_valid_byp_i,
      cache_op_valid_i,
      ready_counters_up,
      l2_cache_op_cnt_q,
      err_mem_q,
      new_req,
      req_addr_q,
      req_q,
      state_q
    };
    sm_filter_debug_signals_o =
        {{(DcacheDbgSmFilterBits - FilterRawWidth){1'b0}}, sm_filter_raw};

    sm_data_debug_signals_o = '0;
    sm_data1_raw = {
      cache_op_req_is_l2_q,
      cache_op_req_is_nop_q,
      ready_counters_q,
      l2_cache_op_cnt_q
    };
    sm_data0_raw = {
      l2_scp_dest_q,
      req_count_q,
      req_addr_q,
      req_q
    };
    sm_data_debug_signals_o[(1 * DcacheDbgSmDataGroupBits) +: DcacheDbgSmDataGroupBits] =
        {{(DcacheDbgSmDataGroupBits - Data1RawWidth){1'b0}}, sm_data1_raw};
    sm_data_debug_signals_o[(0 * DcacheDbgSmDataGroupBits) +: DcacheDbgSmDataGroupBits] =
        {{(DcacheDbgSmDataGroupBits - Data0RawWidth){1'b0}}, sm_data0_raw};
  end

endmodule : minion_dcache_cache_op_unit_l2
/* verilator lint_on UNUSEDSIGNAL */
