// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module esr_cache_bank #(
  parameter int unsigned AdWidth = shire_esr_pkg::EsrCacheBankApbPpAddrWidth,
  parameter int unsigned SetsPerSubBank = shirecache_pkg::SetsPerSubBank,
  parameter int unsigned NumReqqEntries = shirecache_pkg::ReqqDepth
) (
  input  logic                            clk_i,
  input  logic                            rst_c_ni,
  input  logic                            rst_w_ni,
  /* verilator lint_off UNUSEDSIGNAL */  // Original reset_d and test_en are preserved; cache-bank ESR state is reset by reset_c/reset_w only.
  input  logic                            rst_d_ni,
  input  logic                            test_en_i,
  /* verilator lint_on UNUSEDSIGNAL */

  output shirecache_pkg::bank_esr_ctl_t   esr_ctl_o,
  input  shirecache_pkg::bank_esr_status_t esr_status_i,
  output shire_esr_pkg::esr_sc_l3_shire_swizzle_ctl_t esr_sc_l3_shire_swizzle_ctl_o,
  output logic [7:0]                     esr_sc_eco_ctl_o,

  input  logic [AdWidth-1:0]              apb_paddr_i,
  input  logic                            apb_pwrite_i,
  input  logic                            apb_psel_i,
  input  logic                            apb_penable_i,
  input  logic [shire_esr_pkg::EsrApbDataWidth-1:0] apb_pwdata_i,
  output logic                            apb_pready_o,
  output logic [shire_esr_pkg::EsrApbDataWidth-1:0] apb_prdata_o,
  output logic                            apb_pslverr_o
);
  import shire_esr_pkg::*;

  localparam int unsigned NumL3ReqqEntriesDefault = NumReqqEntries / 3;
  localparam int unsigned RamDelayDefault = shirecache_pkg::RamDelayDefault;

  localparam int unsigned EsrSwzShireBaseDefault = 0;
  localparam int unsigned EsrSwzBankBaseDefault =
      EsrSwzShireBaseDefault + shirecache_pkg::L3ShireIdSize;
  localparam int unsigned EsrSwzSubBankBaseDefault =
      EsrSwzBankBaseDefault + shirecache_pkg::BankIdSize;
  localparam int unsigned EsrSwzSubBankSelB2Default = EsrSwzSubBankBaseDefault + 2;
  localparam int unsigned EsrSwzSubBankSelB1Default = EsrSwzSubBankBaseDefault + 1;
  localparam int unsigned EsrSwzSubBankSelB0Default = EsrSwzSubBankBaseDefault;
  localparam int unsigned EsrSwzBankSelB2Default = EsrSwzBankBaseDefault + 2;
  localparam int unsigned EsrSwzBankSelB1Default = EsrSwzBankBaseDefault + 1;
  localparam int unsigned EsrSwzBankSelB0Default = EsrSwzBankBaseDefault;
  localparam int unsigned EsrSwzShireSelB5Default = EsrSwzShireBaseDefault + 5;
  localparam int unsigned EsrSwzShireSelB4Default = EsrSwzShireBaseDefault + 4;
  localparam int unsigned EsrSwzShireSelB3Default = EsrSwzShireBaseDefault + 3;
  localparam int unsigned EsrSwzShireSelB2Default = EsrSwzShireBaseDefault + 2;
  localparam int unsigned EsrSwzShireSelB1Default = EsrSwzShireBaseDefault + 1;
  localparam int unsigned EsrSwzShireSelB0Default = EsrSwzShireBaseDefault;

  localparam int unsigned ScTotalSets = SetsPerSubBank;
  localparam int unsigned ScpSetBaseDefault = ScTotalSets * 0 / 32;
  localparam int unsigned ScpSetSizeDefault = ScTotalSets * 20 / 32;
  localparam int unsigned L2SetBaseDefault = ScTotalSets * 20 / 32;
  localparam int unsigned L2SetSizeDefault = ScTotalSets * 4 / 32;
  localparam int unsigned L3SetBaseDefault = ScTotalSets * 24 / 32;
  localparam int unsigned L3SetSizeDefault = ScTotalSets * 8 / 32;
  localparam int unsigned ScpSetSizeLog2 = $clog2(ScpSetSizeDefault);
  localparam int unsigned L2SetSizeLog2 = $clog2(L2SetSizeDefault);
  localparam int unsigned L3SetSizeLog2 = $clog2(L3SetSizeDefault);
  localparam logic [31:0] L2SetMaskDefault = (32'd1 << L2SetSizeLog2) - 32'd1;
  localparam logic [31:0] L2TagMaskDefault =
      ((32'd1 << L2SetSizeLog2) == L2SetSizeDefault) ?
      ((32'd1 << L2SetSizeLog2) - 32'd1) : ((32'd1 << (L2SetSizeLog2 - 1)) - 32'd1);
  localparam logic [31:0] L3SetMaskDefault = (32'd1 << L3SetSizeLog2) - 32'd1;
  localparam logic [31:0] L3TagMaskDefault =
      ((32'd1 << L3SetSizeLog2) == L3SetSizeDefault) ?
      ((32'd1 << L3SetSizeLog2) - 32'd1) : ((32'd1 << (L3SetSizeLog2 - 1)) - 32'd1);
  localparam logic [31:0] ScpSetMaskDefault = (32'd1 << ScpSetSizeLog2) - 32'd1;
  localparam logic [31:0] ScpTagMaskDefault =
      ((32'd1 << ScpSetSizeLog2) == ScpSetSizeDefault) ?
      ((32'd1 << ScpSetSizeLog2) - 32'd1) : ((32'd1 << (ScpSetSizeLog2 - 1)) - 32'd1);

  localparam logic [AdWidth-1:0] AddrScL3ShireSwizzleCtl =
      EsrCacheBankAddrScL3ShireSwizzleCtl[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScReqqCtl = EsrCacheBankAddrScReqqCtl[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScPipeCtl = EsrCacheBankAddrScPipeCtl[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScL2CacheCtl = EsrCacheBankAddrScL2CacheCtl[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScL3CacheCtl = EsrCacheBankAddrScL3CacheCtl[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScScpCacheCtl = EsrCacheBankAddrScScpCacheCtl[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScIdxCopSmCtl = EsrCacheBankAddrScIdxCopSmCtl[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScIdxCopSmPhysicalIndex =
      EsrCacheBankAddrScIdxCopSmPhysicalIndex[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScIdxCopSmData0 = EsrCacheBankAddrScIdxCopSmData0[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScIdxCopSmData1 = EsrCacheBankAddrScIdxCopSmData1[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScIdxCopSmEcc = EsrCacheBankAddrScIdxCopSmEcc[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScErrLogCtl = EsrCacheBankAddrScErrLogCtl[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScErrLogInfo = EsrCacheBankAddrScErrLogInfo[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScErrLogAddress = EsrCacheBankAddrScErrLogAddress[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScSbeDbeCounts = EsrCacheBankAddrScSbeDbeCounts[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScReqqDebugCtl = EsrCacheBankAddrScReqqDebugCtl[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScReqqDebug0 = EsrCacheBankAddrScReqqDebug0[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScReqqDebug1 = EsrCacheBankAddrScReqqDebug1[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScReqqDebug2 = EsrCacheBankAddrScReqqDebug2[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScReqqDebug3 = EsrCacheBankAddrScReqqDebug3[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScEcoCtl = EsrCacheBankAddrScEcoCtl[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScPerfmonCtlStatus =
      EsrCacheBankAddrScPerfmonCtlStatus[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScPerfmonCycCntr = EsrCacheBankAddrScPerfmonCycCntr[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScPerfmonP0Cntr = EsrCacheBankAddrScPerfmonP0Cntr[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScPerfmonP1Cntr = EsrCacheBankAddrScPerfmonP1Cntr[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScPerfmonP0Qual = EsrCacheBankAddrScPerfmonP0Qual[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScPerfmonP1Qual = EsrCacheBankAddrScPerfmonP1Qual[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScIdxCopSmCtlUser =
      EsrCacheBankAddrScIdxCopSmCtlUser[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScTraceAddressEnable =
      EsrCacheBankAddrScTraceAddressEnable[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScTraceAddressValue =
      EsrCacheBankAddrScTraceAddressValue[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrScTraceCtl = EsrCacheBankAddrScTraceCtl[AdWidth-1:0];

  logic s0_en;
  logic s0_addr_valid;
  logic [EsrApbDataWidth-1:0] s0_rdata;
  logic s1_en;
  logic s1_addr_valid_q;
  logic [EsrApbDataWidth-1:0] s1_rdata_q;
  logic [EsrApbDataWidth-1:0] s1_wdata_q;

  esr_sc_l3_shire_swizzle_ctl_t reg_sc_l3_shire_swizzle_ctl_q, reg_sc_l3_shire_swizzle_ctl_d;
  esr_sc_reqq_ctl_t             reg_sc_reqq_ctl_q, reg_sc_reqq_ctl_d;
  esr_sc_pipe_ctl_t             reg_sc_pipe_ctl_q, reg_sc_pipe_ctl_d;
  esr_sc_pipe_cache_ctl_t       reg_sc_l2_cache_ctl_q, reg_sc_l2_cache_ctl_d;
  esr_sc_pipe_cache_ctl_t       reg_sc_l3_cache_ctl_q, reg_sc_l3_cache_ctl_d;
  esr_sc_pipe_cache_ctl_t       reg_sc_scp_cache_ctl_q, reg_sc_scp_cache_ctl_d;
  esr_sc_err_log_ctl_t          reg_sc_err_log_ctl_q, reg_sc_err_log_ctl_d;
  logic [7:0]                   reg_sc_eco_ctl_q, reg_sc_eco_ctl_d;
  logic [39:0]                  reg_sc_trace_address_enable_q, reg_sc_trace_address_enable_d;
  logic [39:0]                  reg_sc_trace_address_value_q, reg_sc_trace_address_value_d;
  esr_sc_trace_ctl_t            reg_sc_trace_ctl_q, reg_sc_trace_ctl_d;

  logic reg_sc_l3_shire_swizzle_ctl_en_q, reg_sc_l3_shire_swizzle_ctl_en_d;
  logic reg_sc_reqq_ctl_en_q, reg_sc_reqq_ctl_en_d;
  logic reg_sc_pipe_ctl_en_q, reg_sc_pipe_ctl_en_d;
  logic reg_sc_l2_cache_ctl_en_q, reg_sc_l2_cache_ctl_en_d;
  logic reg_sc_l3_cache_ctl_en_q, reg_sc_l3_cache_ctl_en_d;
  logic reg_sc_scp_cache_ctl_en_q, reg_sc_scp_cache_ctl_en_d;
  logic reg_sc_idx_cop_sm_ctl_en_q, reg_sc_idx_cop_sm_ctl_en_d;
  logic reg_sc_idx_cop_sm_physical_index_en_q, reg_sc_idx_cop_sm_physical_index_en_d;
  logic reg_sc_idx_cop_sm_data0_en_q, reg_sc_idx_cop_sm_data0_en_d;
  logic reg_sc_idx_cop_sm_data1_en_q, reg_sc_idx_cop_sm_data1_en_d;
  logic reg_sc_idx_cop_sm_ecc_en_q, reg_sc_idx_cop_sm_ecc_en_d;
  logic reg_sc_err_log_ctl_en_q, reg_sc_err_log_ctl_en_d;
  logic reg_sc_err_log_info_en_q, reg_sc_err_log_info_en_d;
  logic reg_sc_sbe_dbe_counts_en_q, reg_sc_sbe_dbe_counts_en_d;
  logic reg_sc_reqq_debug_ctl_en_q, reg_sc_reqq_debug_ctl_en_d;
  logic reg_sc_eco_ctl_en_q, reg_sc_eco_ctl_en_d;
  logic reg_sc_perfmon_ctl_status_en_q, reg_sc_perfmon_ctl_status_en_d;
  logic reg_sc_perfmon_cyc_cntr_en_q, reg_sc_perfmon_cyc_cntr_en_d;
  logic reg_sc_perfmon_p0_cntr_en_q, reg_sc_perfmon_p0_cntr_en_d;
  logic reg_sc_perfmon_p1_cntr_en_q, reg_sc_perfmon_p1_cntr_en_d;
  logic reg_sc_perfmon_p0_qual_en_q, reg_sc_perfmon_p0_qual_en_d;
  logic reg_sc_perfmon_p1_qual_en_q, reg_sc_perfmon_p1_qual_en_d;
  logic reg_sc_idx_cop_sm_ctl_user_en_q, reg_sc_idx_cop_sm_ctl_user_en_d;
  logic reg_sc_trace_address_enable_en_q, reg_sc_trace_address_enable_en_d;
  logic reg_sc_trace_address_value_en_q, reg_sc_trace_address_value_en_d;
  logic reg_sc_trace_ctl_en_q, reg_sc_trace_ctl_en_d;

  logic [3:0] esr_access_cnt_q;
  logic [3:0] esr_access_cnt_d;
  logic       esr_access_cnt_en;
  logic       reg_sc_idx_cop_sm_ctl_user_en_qual;

  function automatic esr_sc_l3_shire_swizzle_ctl_t reset_swizzle();
    esr_sc_l3_shire_swizzle_ctl_t ret;
    begin
      ret = '0;
      ret.esr_sc_sub_bank_sel_b2 = EsrSwzSubBankSelB2Default[3:0];
      ret.esr_sc_sub_bank_sel_b1 = EsrSwzSubBankSelB1Default[3:0];
      ret.esr_sc_sub_bank_sel_b0 = EsrSwzSubBankSelB0Default[3:0];
      ret.esr_sc_bank_sel_b2 = EsrSwzBankSelB2Default[3:0];
      ret.esr_sc_bank_sel_b1 = EsrSwzBankSelB1Default[3:0];
      ret.esr_sc_bank_sel_b0 = EsrSwzBankSelB0Default[3:0];
      ret.esr_sc_shire_sel_b5 = EsrSwzShireSelB5Default[3:0];
      ret.esr_sc_shire_sel_b4 = EsrSwzShireSelB4Default[3:0];
      ret.esr_sc_shire_sel_b3 = EsrSwzShireSelB3Default[3:0];
      ret.esr_sc_shire_sel_b2 = EsrSwzShireSelB2Default[3:0];
      ret.esr_sc_shire_sel_b1 = EsrSwzShireSelB1Default[3:0];
      ret.esr_sc_shire_sel_b0 = EsrSwzShireSelB0Default[3:0];
      return ret;
    end
  endfunction

  function automatic esr_sc_reqq_ctl_t reset_reqq_ctl();
    esr_sc_reqq_ctl_t ret;
    begin
      ret = '0;
      ret.esr_sc_cbuf_enable = 1'b1;
      ret.esr_sc_remote_l3_enable = 1'b1;
      ret.esr_sc_remote_scp_enable = 1'b1;
      ret.esr_sc_num_l3_reqq_entries = NumL3ReqqEntriesDefault[shirecache_pkg::ReqqIdSize-1:0];
      return ret;
    end
  endfunction

  function automatic esr_sc_pipe_ctl_t reset_pipe_ctl();
    esr_sc_pipe_ctl_t ret;
    begin
      ret = '0;
      ret.esr_sc_idx_cop_sm_ctl_user_en = 1'b1;
      ret.esr_sc_ram_delay = RamDelayDefault[shirecache_pkg::RamDelaySize-1:0];
      ret.esr_sc_l2_rbuf_enable = 1'b1;
      ret.esr_sc_scp_rbuf_enable = 1'b1;
      ret.esr_sc_zero_state_enable = 1'b1;
      ret.esr_sc_cbuf_entry_enable = '1;
      return ret;
    end
  endfunction

  function automatic esr_sc_pipe_cache_ctl_t cache_ctl_from_parts(
    input logic [11:0] set_base,
    input logic [12:0] set_size,
    input logic [11:0] set_mask,
    input logic [11:0] tag_mask
  );
    esr_sc_pipe_cache_ctl_t ret;
    begin
      ret = '0;
      ret.set_base = set_base;
      ret.set_size = set_size;
      ret.set_mask = set_mask;
      ret.tag_mask = tag_mask;
      return ret;
    end
  endfunction

  function automatic esr_sc_pipe_cache_ctl_t write_sc_pipe_cache_ctl(
    input logic [$bits(esr_sc_pipe_cache_ctl_t)-1:0] data
  );
    esr_sc_pipe_cache_ctl_t ret;
    begin
      ret = data;
      ret.set_base_rsvd = '0;
      ret.set_size_rsvd = '0;
      ret.set_mask_rsvd = '0;
      ret.tag_mask_rsvd = '0;
      return ret;
    end
  endfunction

  function automatic esr_sc_err_log_ctl_t reset_err_log_ctl();
    esr_sc_err_log_ctl_t ret;
    begin
      ret = '0;
      ret.esr_sc_err_rsp_enable = 1'b1;
      ret.esr_sc_err_interrupt_enable = 8'hfe;
      return ret;
    end
  endfunction

  always_comb begin
    s0_en = apb_psel_i && !apb_penable_i;
    s0_addr_valid = 1'b1;
    s0_rdata = '0;

    reg_sc_l3_shire_swizzle_ctl_en_d = 1'b0;
    reg_sc_reqq_ctl_en_d = 1'b0;
    reg_sc_pipe_ctl_en_d = 1'b0;
    reg_sc_l2_cache_ctl_en_d = 1'b0;
    reg_sc_l3_cache_ctl_en_d = 1'b0;
    reg_sc_scp_cache_ctl_en_d = 1'b0;
    reg_sc_idx_cop_sm_ctl_en_d = 1'b0;
    reg_sc_idx_cop_sm_physical_index_en_d = 1'b0;
    reg_sc_idx_cop_sm_data0_en_d = 1'b0;
    reg_sc_idx_cop_sm_data1_en_d = 1'b0;
    reg_sc_idx_cop_sm_ecc_en_d = 1'b0;
    reg_sc_err_log_ctl_en_d = 1'b0;
    reg_sc_err_log_info_en_d = 1'b0;
    reg_sc_sbe_dbe_counts_en_d = 1'b0;
    reg_sc_reqq_debug_ctl_en_d = 1'b0;
    reg_sc_eco_ctl_en_d = 1'b0;
    reg_sc_perfmon_ctl_status_en_d = 1'b0;
    reg_sc_perfmon_cyc_cntr_en_d = 1'b0;
    reg_sc_perfmon_p0_cntr_en_d = 1'b0;
    reg_sc_perfmon_p1_cntr_en_d = 1'b0;
    reg_sc_perfmon_p0_qual_en_d = 1'b0;
    reg_sc_perfmon_p1_qual_en_d = 1'b0;
    reg_sc_idx_cop_sm_ctl_user_en_d = 1'b0;
    reg_sc_trace_address_enable_en_d = 1'b0;
    reg_sc_trace_address_value_en_d = 1'b0;
    reg_sc_trace_ctl_en_d = 1'b0;

    if (s0_en) begin
      unique case (apb_paddr_i)
        AddrScL3ShireSwizzleCtl: begin
          reg_sc_l3_shire_swizzle_ctl_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_sc_l3_shire_swizzle_ctl_t)-1:0] = reg_sc_l3_shire_swizzle_ctl_q;
        end
        AddrScReqqCtl: begin
          reg_sc_reqq_ctl_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_sc_reqq_ctl_t)-1:0] = reg_sc_reqq_ctl_q;
        end
        AddrScPipeCtl: begin
          reg_sc_pipe_ctl_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_sc_pipe_ctl_t)-1:0] = reg_sc_pipe_ctl_q;
        end
        AddrScL2CacheCtl: begin
          reg_sc_l2_cache_ctl_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_sc_pipe_cache_ctl_t)-1:0] = reg_sc_l2_cache_ctl_q;
        end
        AddrScL3CacheCtl: begin
          reg_sc_l3_cache_ctl_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_sc_pipe_cache_ctl_t)-1:0] = reg_sc_l3_cache_ctl_q;
        end
        AddrScScpCacheCtl: begin
          reg_sc_scp_cache_ctl_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_sc_pipe_cache_ctl_t)-1:0] = reg_sc_scp_cache_ctl_q;
        end
        AddrScIdxCopSmCtl: begin
          reg_sc_idx_cop_sm_ctl_en_d = apb_pwrite_i;
          s0_rdata = esr_status_i.pipe_idx_cop_sm.ctl;
        end
        AddrScIdxCopSmPhysicalIndex: begin
          reg_sc_idx_cop_sm_physical_index_en_d = apb_pwrite_i;
          s0_rdata = esr_status_i.pipe_idx_cop_sm.physical_index;
        end
        AddrScIdxCopSmData0: begin
          reg_sc_idx_cop_sm_data0_en_d = apb_pwrite_i;
          s0_rdata = esr_status_i.pipe_idx_cop_sm.data0;
        end
        AddrScIdxCopSmData1: begin
          reg_sc_idx_cop_sm_data1_en_d = apb_pwrite_i;
          s0_rdata = esr_status_i.pipe_idx_cop_sm.data1;
        end
        AddrScIdxCopSmEcc: begin
          reg_sc_idx_cop_sm_ecc_en_d = apb_pwrite_i;
          s0_rdata = esr_status_i.pipe_idx_cop_sm.ecc;
        end
        AddrScErrLogCtl: begin
          reg_sc_err_log_ctl_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_sc_err_log_ctl_t)-1:0] = reg_sc_err_log_ctl_q;
        end
        AddrScErrLogInfo: begin
          reg_sc_err_log_info_en_d = apb_pwrite_i;
          s0_rdata = esr_status_i.err_log.info;
        end
        AddrScErrLogAddress: begin
          s0_rdata = esr_status_i.err_log.address;
        end
        AddrScSbeDbeCounts: begin
          reg_sc_sbe_dbe_counts_en_d = apb_pwrite_i;
          s0_rdata = esr_status_i.sbe_dbe_counts;
        end
        AddrScReqqDebugCtl: begin
          reg_sc_reqq_debug_ctl_en_d = apb_pwrite_i;
          s0_rdata = '0;
        end
        AddrScReqqDebug0: begin
          s0_rdata = esr_status_i.err_log.reqq_debug0;
        end
        AddrScReqqDebug1: begin
          s0_rdata = esr_status_i.err_log.reqq_debug1;
        end
        AddrScReqqDebug2: begin
          s0_rdata = esr_status_i.err_log.reqq_debug2;
        end
        AddrScReqqDebug3: begin
          s0_rdata = esr_status_i.err_log.reqq_debug3;
        end
        AddrScEcoCtl: begin
          reg_sc_eco_ctl_en_d = apb_pwrite_i;
          s0_rdata[7:0] = reg_sc_eco_ctl_q;
        end
        AddrScPerfmonCtlStatus: begin
          reg_sc_perfmon_ctl_status_en_d = apb_pwrite_i;
          s0_rdata = esr_status_i.perfmon.ctl_status;
        end
        AddrScPerfmonCycCntr: begin
          reg_sc_perfmon_cyc_cntr_en_d = apb_pwrite_i;
          s0_rdata = esr_status_i.perfmon.cyc_cntr;
        end
        AddrScPerfmonP0Cntr: begin
          reg_sc_perfmon_p0_cntr_en_d = apb_pwrite_i;
          s0_rdata = esr_status_i.perfmon.p0_cntr;
        end
        AddrScPerfmonP1Cntr: begin
          reg_sc_perfmon_p1_cntr_en_d = apb_pwrite_i;
          s0_rdata = esr_status_i.perfmon.p1_cntr;
        end
        AddrScPerfmonP0Qual: begin
          reg_sc_perfmon_p0_qual_en_d = apb_pwrite_i;
          s0_rdata = esr_status_i.perfmon.p0_qual;
        end
        AddrScPerfmonP1Qual: begin
          reg_sc_perfmon_p1_qual_en_d = apb_pwrite_i;
          s0_rdata = esr_status_i.perfmon.p1_qual;
        end
        AddrScIdxCopSmCtlUser: begin
          reg_sc_idx_cop_sm_ctl_user_en_d = apb_pwrite_i;
          if (reg_sc_pipe_ctl_q.esr_sc_idx_cop_sm_ctl_user_en) begin
            s0_rdata = esr_status_i.pipe_idx_cop_sm.ctl;
          end else begin
            s0_rdata = '0;
          end
        end
        AddrScTraceAddressEnable: begin
          reg_sc_trace_address_enable_en_d = apb_pwrite_i;
          s0_rdata[39:0] = reg_sc_trace_address_enable_q;
        end
        AddrScTraceAddressValue: begin
          reg_sc_trace_address_value_en_d = apb_pwrite_i;
          s0_rdata[39:0] = reg_sc_trace_address_value_q;
        end
        AddrScTraceCtl: begin
          reg_sc_trace_ctl_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_sc_trace_ctl_t)-1:0] = reg_sc_trace_ctl_q;
        end
        default: begin
          s0_addr_valid = 1'b0;
        end
      endcase
    end
  end

  assign s1_en = apb_psel_i && apb_penable_i;

  always_comb begin
    reg_sc_l3_shire_swizzle_ctl_d = reg_sc_l3_shire_swizzle_ctl_q;
    reg_sc_reqq_ctl_d = reg_sc_reqq_ctl_q;
    reg_sc_pipe_ctl_d = reg_sc_pipe_ctl_q;
    reg_sc_l2_cache_ctl_d = reg_sc_l2_cache_ctl_q;
    reg_sc_l3_cache_ctl_d = reg_sc_l3_cache_ctl_q;
    reg_sc_scp_cache_ctl_d = reg_sc_scp_cache_ctl_q;
    reg_sc_err_log_ctl_d = reg_sc_err_log_ctl_q;
    reg_sc_eco_ctl_d = reg_sc_eco_ctl_q;
    reg_sc_trace_address_enable_d = reg_sc_trace_address_enable_q;
    reg_sc_trace_address_value_d = reg_sc_trace_address_value_q;
    reg_sc_trace_ctl_d = reg_sc_trace_ctl_q;

    if (reg_sc_l3_shire_swizzle_ctl_en_q) begin
      reg_sc_l3_shire_swizzle_ctl_d = s1_wdata_q[$bits(esr_sc_l3_shire_swizzle_ctl_t)-1:0];
    end
    if (reg_sc_reqq_ctl_en_q) begin
      reg_sc_reqq_ctl_d = s1_wdata_q[$bits(esr_sc_reqq_ctl_t)-1:0];
    end
    if (reg_sc_pipe_ctl_en_q) begin
      reg_sc_pipe_ctl_d = s1_wdata_q[$bits(esr_sc_pipe_ctl_t)-1:0];
    end
    if (reg_sc_l2_cache_ctl_en_q) begin
      reg_sc_l2_cache_ctl_d = write_sc_pipe_cache_ctl(s1_wdata_q[$bits(esr_sc_pipe_cache_ctl_t)-1:0]);
    end
    if (reg_sc_l3_cache_ctl_en_q) begin
      reg_sc_l3_cache_ctl_d = write_sc_pipe_cache_ctl(s1_wdata_q[$bits(esr_sc_pipe_cache_ctl_t)-1:0]);
    end
    if (reg_sc_scp_cache_ctl_en_q) begin
      reg_sc_scp_cache_ctl_d = write_sc_pipe_cache_ctl(s1_wdata_q[$bits(esr_sc_pipe_cache_ctl_t)-1:0]);
    end
    if (reg_sc_err_log_ctl_en_q) begin
      reg_sc_err_log_ctl_d = s1_wdata_q[$bits(esr_sc_err_log_ctl_t)-1:0];
    end
    if (reg_sc_eco_ctl_en_q) begin
      reg_sc_eco_ctl_d = s1_wdata_q[7:0];
    end
    if (reg_sc_trace_address_enable_en_q) begin
      reg_sc_trace_address_enable_d = s1_wdata_q[39:0];
    end
    if (reg_sc_trace_address_value_en_q) begin
      reg_sc_trace_address_value_d = s1_wdata_q[39:0];
    end
    if (reg_sc_trace_ctl_en_q) begin
      reg_sc_trace_ctl_d = s1_wdata_q[$bits(esr_sc_trace_ctl_t)-1:0];
    end
  end

  always_ff @(posedge clk_i) begin
    if (s0_en && apb_pwrite_i) begin
      s1_wdata_q <= apb_pwdata_i;
    end
    if (s0_en && !apb_pwrite_i) begin
      s1_rdata_q <= s0_rdata;
    end
  end

  always_ff @(posedge clk_i or negedge rst_c_ni) begin
    if (!rst_c_ni) begin
      s1_addr_valid_q <= 1'b0;
      reg_sc_l3_shire_swizzle_ctl_en_q <= 1'b0;
      reg_sc_reqq_ctl_en_q <= 1'b0;
      reg_sc_pipe_ctl_en_q <= 1'b0;
      reg_sc_l2_cache_ctl_en_q <= 1'b0;
      reg_sc_l3_cache_ctl_en_q <= 1'b0;
      reg_sc_scp_cache_ctl_en_q <= 1'b0;
      reg_sc_idx_cop_sm_ctl_en_q <= 1'b0;
      reg_sc_idx_cop_sm_physical_index_en_q <= 1'b0;
      reg_sc_idx_cop_sm_data0_en_q <= 1'b0;
      reg_sc_idx_cop_sm_data1_en_q <= 1'b0;
      reg_sc_idx_cop_sm_ecc_en_q <= 1'b0;
      reg_sc_err_log_ctl_en_q <= 1'b0;
      reg_sc_err_log_info_en_q <= 1'b0;
      reg_sc_sbe_dbe_counts_en_q <= 1'b0;
      reg_sc_reqq_debug_ctl_en_q <= 1'b0;
      reg_sc_eco_ctl_en_q <= 1'b0;
      reg_sc_perfmon_ctl_status_en_q <= 1'b0;
      reg_sc_perfmon_cyc_cntr_en_q <= 1'b0;
      reg_sc_perfmon_p0_cntr_en_q <= 1'b0;
      reg_sc_perfmon_p1_cntr_en_q <= 1'b0;
      reg_sc_perfmon_p0_qual_en_q <= 1'b0;
      reg_sc_perfmon_p1_qual_en_q <= 1'b0;
      reg_sc_idx_cop_sm_ctl_user_en_q <= 1'b0;
      reg_sc_trace_address_enable_en_q <= 1'b0;
      reg_sc_trace_address_value_en_q <= 1'b0;
      reg_sc_trace_ctl_en_q <= 1'b0;
    end else begin
      if (s0_en) begin
        s1_addr_valid_q <= s0_addr_valid;
      end
      if (s0_en || s1_en) begin
        reg_sc_l3_shire_swizzle_ctl_en_q <= reg_sc_l3_shire_swizzle_ctl_en_d;
        reg_sc_reqq_ctl_en_q <= reg_sc_reqq_ctl_en_d;
        reg_sc_pipe_ctl_en_q <= reg_sc_pipe_ctl_en_d;
        reg_sc_l2_cache_ctl_en_q <= reg_sc_l2_cache_ctl_en_d;
        reg_sc_l3_cache_ctl_en_q <= reg_sc_l3_cache_ctl_en_d;
        reg_sc_scp_cache_ctl_en_q <= reg_sc_scp_cache_ctl_en_d;
        reg_sc_idx_cop_sm_ctl_en_q <= reg_sc_idx_cop_sm_ctl_en_d;
        reg_sc_idx_cop_sm_physical_index_en_q <= reg_sc_idx_cop_sm_physical_index_en_d;
        reg_sc_idx_cop_sm_data0_en_q <= reg_sc_idx_cop_sm_data0_en_d;
        reg_sc_idx_cop_sm_data1_en_q <= reg_sc_idx_cop_sm_data1_en_d;
        reg_sc_idx_cop_sm_ecc_en_q <= reg_sc_idx_cop_sm_ecc_en_d;
        reg_sc_err_log_ctl_en_q <= reg_sc_err_log_ctl_en_d;
        reg_sc_err_log_info_en_q <= reg_sc_err_log_info_en_d;
        reg_sc_sbe_dbe_counts_en_q <= reg_sc_sbe_dbe_counts_en_d;
        reg_sc_reqq_debug_ctl_en_q <= reg_sc_reqq_debug_ctl_en_d;
        reg_sc_eco_ctl_en_q <= reg_sc_eco_ctl_en_d;
        reg_sc_perfmon_ctl_status_en_q <= reg_sc_perfmon_ctl_status_en_d;
        reg_sc_perfmon_cyc_cntr_en_q <= reg_sc_perfmon_cyc_cntr_en_d;
        reg_sc_perfmon_p0_cntr_en_q <= reg_sc_perfmon_p0_cntr_en_d;
        reg_sc_perfmon_p1_cntr_en_q <= reg_sc_perfmon_p1_cntr_en_d;
        reg_sc_perfmon_p0_qual_en_q <= reg_sc_perfmon_p0_qual_en_d;
        reg_sc_perfmon_p1_qual_en_q <= reg_sc_perfmon_p1_qual_en_d;
        reg_sc_idx_cop_sm_ctl_user_en_q <= reg_sc_idx_cop_sm_ctl_user_en_d;
        reg_sc_trace_address_enable_en_q <= reg_sc_trace_address_enable_en_d;
        reg_sc_trace_address_value_en_q <= reg_sc_trace_address_value_en_d;
        reg_sc_trace_ctl_en_q <= reg_sc_trace_ctl_en_d;
      end
    end
  end

  always_ff @(posedge clk_i or negedge rst_c_ni) begin
    if (!rst_c_ni) begin
      reg_sc_l3_shire_swizzle_ctl_q <= reset_swizzle();
      reg_sc_reqq_ctl_q <= reset_reqq_ctl();
      reg_sc_pipe_ctl_q <= reset_pipe_ctl();
      reg_sc_l2_cache_ctl_q <= cache_ctl_from_parts(
          L2SetBaseDefault[11:0], L2SetSizeDefault[12:0],
          L2SetMaskDefault[11:0], L2TagMaskDefault[11:0]);
      reg_sc_l3_cache_ctl_q <= cache_ctl_from_parts(
          L3SetBaseDefault[11:0], L3SetSizeDefault[12:0],
          L3SetMaskDefault[11:0], L3TagMaskDefault[11:0]);
      reg_sc_scp_cache_ctl_q <= cache_ctl_from_parts(
          ScpSetBaseDefault[11:0], ScpSetSizeDefault[12:0],
          ScpSetMaskDefault[11:0], ScpTagMaskDefault[11:0]);
      reg_sc_err_log_ctl_q <= reset_err_log_ctl();
      reg_sc_eco_ctl_q <= '0;
      reg_sc_trace_address_enable_q <= '0;
      reg_sc_trace_address_value_q <= '0;
      reg_sc_trace_ctl_q <= '0;
    end else begin
      reg_sc_l3_shire_swizzle_ctl_q <= reg_sc_l3_shire_swizzle_ctl_d;
      reg_sc_reqq_ctl_q <= reg_sc_reqq_ctl_d;
      reg_sc_pipe_ctl_q <= reg_sc_pipe_ctl_d;
      reg_sc_l2_cache_ctl_q <= reg_sc_l2_cache_ctl_d;
      reg_sc_l3_cache_ctl_q <= reg_sc_l3_cache_ctl_d;
      reg_sc_scp_cache_ctl_q <= reg_sc_scp_cache_ctl_d;
      reg_sc_err_log_ctl_q <= reg_sc_err_log_ctl_d;
      reg_sc_eco_ctl_q <= reg_sc_eco_ctl_d;
      reg_sc_trace_address_enable_q <= reg_sc_trace_address_enable_d;
      reg_sc_trace_address_value_q <= reg_sc_trace_address_value_d;
      reg_sc_trace_ctl_q <= reg_sc_trace_ctl_d;
    end
  end

  assign esr_access_cnt_d = apb_psel_i ? 4'hf : (esr_access_cnt_q - 4'h1);
  assign esr_access_cnt_en = apb_psel_i || (|esr_access_cnt_q);

  always_ff @(posedge clk_i or negedge rst_w_ni) begin
    if (!rst_w_ni) begin
      esr_access_cnt_q <= '0;
    end else if (esr_access_cnt_en) begin
      esr_access_cnt_q <= esr_access_cnt_d;
    end
  end

  assign reg_sc_idx_cop_sm_ctl_user_en_qual =
      reg_sc_idx_cop_sm_ctl_user_en_q &&
      esr_ctl_o.esr_sc_idx_cop_sm_ctl_user_en &&
      (s1_wdata_q[11:8] == 4'hA);

  always_comb begin
    esr_ctl_o = '0;

    esr_ctl_o.esr_sc_clk_gate_disable = reg_sc_reqq_ctl_q.esr_sc_clk_gate_disable;
    esr_ctl_o.esr_sc_axi_qos = reg_sc_reqq_ctl_q.esr_sc_axi_qos;
    esr_ctl_o.esr_sc_cbuf_enable = reg_sc_reqq_ctl_q.esr_sc_cbuf_enable;
    esr_ctl_o.esr_sc_remote_l3_enable = reg_sc_reqq_ctl_q.esr_sc_remote_l3_enable;
    esr_ctl_o.esr_sc_remote_scp_enable = reg_sc_reqq_ctl_q.esr_sc_remote_scp_enable;
    esr_ctl_o.esr_sc_l2_bypass = reg_sc_reqq_ctl_q.esr_sc_l2_bypass;
    esr_ctl_o.esr_sc_l3_bypass = reg_sc_reqq_ctl_q.esr_sc_l3_bypass;
    esr_ctl_o.esr_sc_num_l3_reqq_entries = reg_sc_reqq_ctl_q.esr_sc_num_l3_reqq_entries;
    esr_ctl_o.esr_sc_reqq_no_link_list = reg_sc_reqq_ctl_q.esr_sc_reqq_no_link_list;
    esr_ctl_o.esr_sc_ecc_scrub_enable = reg_sc_reqq_ctl_q.esr_sc_ecc_scrub_enable;
    esr_ctl_o.esr_sc_l3_yield_priority = reg_sc_reqq_ctl_q.esr_sc_l3_yield_priority;

    esr_ctl_o.esr_sc_idx_cop_sm_ctl_user_en = reg_sc_pipe_ctl_q.esr_sc_idx_cop_sm_ctl_user_en;
    esr_ctl_o.esr_sc_ram_deep_sleep = reg_sc_pipe_ctl_q.esr_sc_ram_deep_sleep;
    esr_ctl_o.esr_sc_ram_shut_down = reg_sc_pipe_ctl_q.esr_sc_ram_shut_down;
    esr_ctl_o.esr_sc_ram_delay = reg_sc_pipe_ctl_q.esr_sc_ram_delay;
    esr_ctl_o.esr_sc_l2_rbuf_enable = reg_sc_pipe_ctl_q.esr_sc_l2_rbuf_enable;
    esr_ctl_o.esr_sc_scp_rbuf_enable = reg_sc_pipe_ctl_q.esr_sc_scp_rbuf_enable;
    esr_ctl_o.esr_sc_zero_state_enable = reg_sc_pipe_ctl_q.esr_sc_zero_state_enable;
    esr_ctl_o.esr_sc_allow_only_1_req_per_sub_bank = reg_sc_pipe_ctl_q.esr_sc_allow_only_1_req_per_sub_bank;
    esr_ctl_o.esr_sc_allow_only_1_req_per_bank = reg_sc_pipe_ctl_q.esr_sc_allow_only_1_req_per_bank;
    esr_ctl_o.esr_sc_cbuf_entry_enable = reg_sc_pipe_ctl_q.esr_sc_cbuf_entry_enable;

    esr_ctl_o.esr_sc_two_shire_aliasing_use_shire_lsb =
        reg_sc_l3_shire_swizzle_ctl_q.esr_sc_two_shire_aliasing_use_shire_lsb;
    esr_ctl_o.esr_sc_all_shire_aliasing = reg_sc_l3_shire_swizzle_ctl_q.esr_sc_all_shire_aliasing;
    esr_ctl_o.esr_sc_sub_bank_sel_b2 = reg_sc_l3_shire_swizzle_ctl_q.esr_sc_sub_bank_sel_b2;
    esr_ctl_o.esr_sc_sub_bank_sel_b1 = reg_sc_l3_shire_swizzle_ctl_q.esr_sc_sub_bank_sel_b1;
    esr_ctl_o.esr_sc_sub_bank_sel_b0 = reg_sc_l3_shire_swizzle_ctl_q.esr_sc_sub_bank_sel_b0;
    esr_ctl_o.esr_sc_bank_sel_b2 = reg_sc_l3_shire_swizzle_ctl_q.esr_sc_bank_sel_b2;
    esr_ctl_o.esr_sc_bank_sel_b1 = reg_sc_l3_shire_swizzle_ctl_q.esr_sc_bank_sel_b1;
    esr_ctl_o.esr_sc_bank_sel_b0 = reg_sc_l3_shire_swizzle_ctl_q.esr_sc_bank_sel_b0;
    esr_ctl_o.esr_sc_shire_sel_b5 = reg_sc_l3_shire_swizzle_ctl_q.esr_sc_shire_sel_b5;
    esr_ctl_o.esr_sc_shire_sel_b4 = reg_sc_l3_shire_swizzle_ctl_q.esr_sc_shire_sel_b4;
    esr_ctl_o.esr_sc_shire_sel_b3 = reg_sc_l3_shire_swizzle_ctl_q.esr_sc_shire_sel_b3;
    esr_ctl_o.esr_sc_shire_sel_b2 = reg_sc_l3_shire_swizzle_ctl_q.esr_sc_shire_sel_b2;
    esr_ctl_o.esr_sc_shire_sel_b1 = reg_sc_l3_shire_swizzle_ctl_q.esr_sc_shire_sel_b1;
    esr_ctl_o.esr_sc_shire_sel_b0 = reg_sc_l3_shire_swizzle_ctl_q.esr_sc_shire_sel_b0;

    esr_ctl_o.esr_sc_l2_set_base = reg_sc_l2_cache_ctl_q.set_base[shirecache_pkg::SetIdSize-1:0];
    esr_ctl_o.esr_sc_l2_set_size = reg_sc_l2_cache_ctl_q.set_size[shirecache_pkg::SetAllocSize-1:0];
    esr_ctl_o.esr_sc_l2_set_mask = reg_sc_l2_cache_ctl_q.set_mask[shirecache_pkg::SetIdSize-1:0];
    esr_ctl_o.esr_sc_l2_tag_mask = reg_sc_l2_cache_ctl_q.tag_mask[shirecache_pkg::SetIdSize-1:0];
    esr_ctl_o.esr_sc_l3_set_base = reg_sc_l3_cache_ctl_q.set_base[shirecache_pkg::SetIdSize-1:0];
    esr_ctl_o.esr_sc_l3_set_size = reg_sc_l3_cache_ctl_q.set_size[shirecache_pkg::SetAllocSize-1:0];
    esr_ctl_o.esr_sc_l3_set_mask = reg_sc_l3_cache_ctl_q.set_mask[shirecache_pkg::SetIdSize-1:0];
    esr_ctl_o.esr_sc_l3_tag_mask = reg_sc_l3_cache_ctl_q.tag_mask[shirecache_pkg::SetIdSize-1:0];
    esr_ctl_o.esr_sc_scp_set_base = reg_sc_scp_cache_ctl_q.set_base[shirecache_pkg::SetIdSize-1:0];
    esr_ctl_o.esr_sc_scp_set_size = reg_sc_scp_cache_ctl_q.set_size[shirecache_pkg::SetAllocSize-1:0];
    esr_ctl_o.esr_sc_scp_set_mask = reg_sc_scp_cache_ctl_q.set_mask[shirecache_pkg::SetIdSize-1:0];
    esr_ctl_o.esr_sc_scp_tag_mask = reg_sc_scp_cache_ctl_q.tag_mask[shirecache_pkg::SetIdSize-1:0];

    esr_ctl_o.esr_sc_err_rsp_enable = reg_sc_err_log_ctl_q.esr_sc_err_rsp_enable;
    esr_ctl_o.esr_sc_err_interrupt_enable = reg_sc_err_log_ctl_q.esr_sc_err_interrupt_enable;

    esr_ctl_o.esr_sc_trace_filter_address_enable = reg_sc_trace_address_enable_q;
    esr_ctl_o.esr_sc_trace_filter_address_value = reg_sc_trace_address_value_q;
    esr_ctl_o.esr_sc_trace_filter_reqq_id_enable = reg_sc_trace_ctl_q.esr_sc_trace_filter_reqq_id_enable;
    esr_ctl_o.esr_sc_trace_filter_reqq_id = reg_sc_trace_ctl_q.esr_sc_trace_filter_reqq_id;
    esr_ctl_o.esr_sc_trace_filter_port_enable = reg_sc_trace_ctl_q.esr_sc_trace_filter_port_enable;
    esr_ctl_o.esr_sc_trace_filter_port = reg_sc_trace_ctl_q.esr_sc_trace_filter_port;
    esr_ctl_o.esr_sc_trace_filter_source_enable = reg_sc_trace_ctl_q.esr_sc_trace_filter_source_enable;
    esr_ctl_o.esr_sc_trace_filter_source = reg_sc_trace_ctl_q.esr_sc_trace_filter_source[shirecache_pkg::ReqqSourceSize-1:0];
    esr_ctl_o.esr_sc_trace_filter_l2_enable = reg_sc_trace_ctl_q.esr_sc_trace_filter_l2_enable;
    esr_ctl_o.esr_sc_trace_filter_l3_enable = reg_sc_trace_ctl_q.esr_sc_trace_filter_l3_enable;
    esr_ctl_o.esr_sc_trace_filter_fsm_enable = reg_sc_trace_ctl_q.esr_sc_trace_filter_fsm_enable;
    esr_ctl_o.esr_sc_trace_type_hot_enable = reg_sc_trace_ctl_q.esr_sc_trace_type_hot_enable;

    esr_ctl_o.wr_en_reqq_ctl = reg_sc_reqq_ctl_en_q;
    esr_ctl_o.wr_en_idx_cop_sm_ctl = reg_sc_idx_cop_sm_ctl_en_q || reg_sc_idx_cop_sm_ctl_user_en_qual;
    esr_ctl_o.wr_en_idx_cop_sm_physical_index = reg_sc_idx_cop_sm_physical_index_en_q;
    esr_ctl_o.wr_en_idx_cop_sm_data0 = reg_sc_idx_cop_sm_data0_en_q;
    esr_ctl_o.wr_en_idx_cop_sm_data1 = reg_sc_idx_cop_sm_data1_en_q;
    esr_ctl_o.wr_en_idx_cop_sm_ecc = reg_sc_idx_cop_sm_ecc_en_q;
    esr_ctl_o.wr_en_err_log_info = reg_sc_err_log_info_en_q;
    esr_ctl_o.wr_en_sbe_dbe_counts = reg_sc_sbe_dbe_counts_en_q;
    esr_ctl_o.wr_en_reqq_debug_ctl = reg_sc_reqq_debug_ctl_en_q;
    esr_ctl_o.wr_en_perfmon_ctl_status = reg_sc_perfmon_ctl_status_en_q;
    esr_ctl_o.wr_en_perfmon_cyc_cntr = reg_sc_perfmon_cyc_cntr_en_q;
    esr_ctl_o.wr_en_perfmon_p0_cntr = reg_sc_perfmon_p0_cntr_en_q;
    esr_ctl_o.wr_en_perfmon_p1_cntr = reg_sc_perfmon_p1_cntr_en_q;
    esr_ctl_o.wr_en_perfmon_p0_qual = reg_sc_perfmon_p0_qual_en_q;
    esr_ctl_o.wr_en_perfmon_p1_qual = reg_sc_perfmon_p1_qual_en_q;
    esr_ctl_o.wr_data = s1_wdata_q & {{(shirecache_pkg::DwDataSize-2){1'b1}},
                                      !reg_sc_idx_cop_sm_ctl_user_en_qual, 1'b1};
    esr_ctl_o.access_in_flight = apb_psel_i || (|esr_access_cnt_q);
  end

  assign esr_sc_l3_shire_swizzle_ctl_o = reg_sc_l3_shire_swizzle_ctl_q;
  assign esr_sc_eco_ctl_o = reg_sc_eco_ctl_q;

  assign apb_prdata_o = s1_rdata_q;
  assign apb_pslverr_o = !s1_addr_valid_q;
  assign apb_pready_o = 1'b1;

  logic unused_inputs;
  assign unused_inputs = &{
    1'b0,
    rst_d_ni,
    test_en_i,
    reg_sc_l2_cache_ctl_q.set_base_rsvd,
    reg_sc_l2_cache_ctl_q.set_size_rsvd,
    reg_sc_l2_cache_ctl_q.set_mask_rsvd,
    reg_sc_l2_cache_ctl_q.tag_mask_rsvd,
    reg_sc_l3_cache_ctl_q.set_base_rsvd,
    reg_sc_l3_cache_ctl_q.set_size_rsvd,
    reg_sc_l3_cache_ctl_q.set_mask_rsvd,
    reg_sc_l3_cache_ctl_q.tag_mask_rsvd,
    reg_sc_scp_cache_ctl_q.set_base_rsvd,
    reg_sc_scp_cache_ctl_q.set_size_rsvd,
    reg_sc_scp_cache_ctl_q.set_mask_rsvd,
    reg_sc_scp_cache_ctl_q.tag_mask_rsvd,
    reg_sc_eco_ctl_q,
    &NumL3ReqqEntriesDefault,
    &RamDelayDefault,
    &EsrSwzSubBankSelB2Default,
    &EsrSwzSubBankSelB1Default,
    &EsrSwzSubBankSelB0Default,
    &EsrSwzBankSelB2Default,
    &EsrSwzBankSelB1Default,
    &EsrSwzBankSelB0Default,
    &EsrSwzShireSelB5Default,
    &EsrSwzShireSelB4Default,
    &EsrSwzShireSelB3Default,
    &EsrSwzShireSelB2Default,
    &EsrSwzShireSelB1Default,
    &EsrSwzShireSelB0Default,
    &L2SetMaskDefault,
    &L2TagMaskDefault,
    &L3SetMaskDefault,
    &L3TagMaskDefault,
    &ScpSetMaskDefault,
    &ScpTagMaskDefault
  };
endmodule
