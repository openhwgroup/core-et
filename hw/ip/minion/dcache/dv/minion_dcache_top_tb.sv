// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit-test wrapper for minion_dcache_top.

/* verilator lint_off SYNCASYNCNET */  // The wrapper intentionally fans one reset into child async-reset arbiters and preserved synchronous-reset dcache state, so Verilator reports the shared harness reset as mixed sync/async.
module minion_dcache_top_tb #(
  parameter bit DebugApbEn = 1'b1,
  parameter bit DebugMonEn = 1'b1,
  parameter bit VpuEn = 1'b1
) (
  input  logic                          clk_i,
  input  logic                          rst_ni,
  input  logic                          id_core_alloc_rq_pre_i,
  input  logic                          s0_core_alloc_rq_val_i,
  input  logic                          id_core_gsc_i,
  input  logic                          s0_core_req_valid_i,
  input  logic [minion_pkg::VaSize-1:0] req_addr_i,
  input  logic                          req_addr_msb_err_i,
  input  logic                          req_dest_fp_i,
  input  logic [minion_pkg::XregAddrSize-1:0] req_dest_addr_i,
  input  logic                          req_thread_id_i,
  input  logic [4:0]                    req_cmd_i,
  input  logic [3:0]                    req_typ_i,
  input  logic [63:0]                   store_data_lo_i,
  input  logic [minion_pkg::CoreGscCntBits-1:0] req_gsc_cnt_i,
  input  logic [minion_pkg::DmemReqPsMaskSz-1:0] req_ps_mask_i,
  input  logic                          req_phys_i,
  input  logic                          req_mem_global_i,
  input  logic [minion_pkg::CoreGsc32IdxVSize-1:0] req_gsc32_idx_i,
  input  logic [1:0]                    vmspagesize_i,
  input  logic [1:0]                    vm_prv_t0_i,
  input  logic [1:0]                    vm_prv_t1_i,
  input  logic [3:0]                    satp_mode_i,
  input  logic [minion_pkg::PaTransSize-1:0] satp_ppn_i,
  input  logic [3:0]                    matp_mode_i,
  input  logic [minion_pkg::PaTransSize-1:0] matp_ppn_i,
  input  logic                          satp_info_en_i,
  input  logic                          matp_info_en_i,
  input  logic                          ptw_req_ready_i,
  input  logic                          ptw_resp_canceled_req_i,
  input  logic                          ptw_resp_access_fault_i,
  input  logic [7:0]                    ptw_resp_rsvd_for_hw_i,
  input  logic [minion_pkg::CsrSatpPpnSz-1:0] ptw_resp_ppn_i,
  input  logic [1:0]                    ptw_resp_rsvd_for_sw_i,
  input  logic                          ptw_resp_d_i,
  input  logic                          ptw_resp_a_i,
  input  logic                          ptw_resp_g_i,
  input  logic                          ptw_resp_u_i,
  input  logic                          ptw_resp_x_i,
  input  logic                          ptw_resp_w_i,
  input  logic                          ptw_resp_r_i,
  input  logic                          ptw_resp_v_i,
  input  logic                          ptw_resp_valid_i,
  input  logic [minion_pkg::DcacheDbgAddrBits-1:0] apb_paddr_i,
  input  logic                          apb_pwrite_i,
  input  logic                          apb_psel_i,
  input  logic                          apb_penable_i,
  input  logic [minion_pkg::BpamShireApbDataWidth-1:0] apb_pwdata_i,
  output logic [minion_pkg::DcacheL2MissReqPorts-1:0] l2_miss_req_valid_o,
  output minion_dcache_pkg::et_link_minion_miss_req_info_t l2_miss_req_o,
  output logic                          l2_resp_ready_o,
  input  logic                          l2_resp_valid_i,
  input  minion_pkg::et_link_minion_rsp_info_t l2_resp_i,
  output logic                          id_core_ready_o,
  output logic                          s1_bp_conf_valid_o,
  output logic [2:0]                    s1_bp_conf_cmd_o,
  output logic                          s1_bp_conf_thread_id_o,
  output logic [minion_pkg::VaSizeExt-1:0] s1_bp_conf_addr_o,
  output logic                          ptw_req_valid_o,
  output logic [3:0]                    ptw_req_satp_mode_o,
  output logic [minion_pkg::PaTransSize-1:0] ptw_req_satp_ppn_o,
  output logic [1:0]                    ptw_req_prv_o,
  output logic [minion_pkg::VaSize-minion_pkg::VaUntransSize-1:0] ptw_req_addr_o,
  output logic                          apb_pready_o,
  output logic [minion_pkg::BpamShireApbDataWidth-1:0] apb_prdata_o,
  output logic                          apb_pslverr_o,
  output logic [minion_pkg::CsrNrEventsDcache-1:0] io_events_o,
  output logic [minion_pkg::NrThreads-1:0] bus_err_o,
  output logic                          s1_core_replay_next_o,
  output logic                          s2_core_resp_int_valid_o,
  output logic                          s3_core_resp_valid_o,
  output logic [63:0]                   s3_core_resp_data_lo_o,
  output logic [minion_pkg::VaSizeExt-1:0] s3_core_resp_addr_o,
  output logic [63:0]                   metadata_mask_o,
  output logic [63:0]                   dcache_hit_diag_o,
  output logic [15:0]                   dcache_hit_sync_diag_o,
  output logic [15:0]                   dcache_hit_gate_diag_o
);

  minion_pkg::core_dcache_ctrl core_ctrl;
  minion_pkg::minion_dcache_req core_req;
  minion_dcache_pkg::vm_status_t [minion_pkg::NrThreads-1:0] vm_status;
  minion_pkg::minion_satp_info satp_info;
  minion_pkg::minion_satp_info matp_info;
  minion_pkg::minion_ptw_pte ptw_resp_data;
  minion_pkg::dcache_core_bp_t s1_bp_conf;
  minion_pkg::minion_ptw_req ptw_req_data;
  logic [minion_pkg::NrThreads-1:0][minion_pkg::PaSize-1:0] unused_bus_err_addr;
  minion_pkg::dcache_core_ctrl unused_core_ctrl_resp;
  minion_pkg::dcache_vpu_scp_resp unused_vpu_scp_resp;
  logic [minion_pkg::VpuDataSz-1:0] unused_vpu_scp_data;
  logic [minion_pkg::VpuDataSz-1:0] unused_vpu_tenb_data;
  minion_pkg::dcache_vpu_reduce_ctrl unused_vpu_reduce_ctrl;
  logic                 unused_s3_core_resp_valid;
  minion_pkg::dcache_minion_resp unused_s3_core_resp;
  minion_pkg::dcache_minion_scoreboard unused_scoreboard;
  logic [minion_pkg::NrThreads-1:0] unused_sb_fp_dealloc;
  logic [minion_pkg::NrThreads-1:0] unused_sb_int_dealloc;
  minion_pkg::dcache_tag_xcpt unused_s1_core_xcpt;
  logic                 unused_s2_core_flush_pipeline;
  logic [minion_pkg::NrThreads-1:0] unused_s3_ordered;
  logic [minion_pkg::DcacheL2EvictReqPorts-1:0] unused_l2_evict_req_valid;
  minion_dcache_pkg::et_link_minion_evict_req_info_t unused_l2_evict_req;
  logic                 unused_l2_resp_ready_int;
  logic                 unused_l2_resp_valid_int;
  minion_pkg::et_link_minion_rsp_info_t unused_l2_resp_int;
  logic                 unused_dcache_idle_excl_mode;
  logic [minion_pkg::DcacheTlErrorBits-3:0] unused_tensor_load_err_flags;
  logic [minion_pkg::DcacheErrFlagBits-1:0][minion_pkg::NrThreads-1:0] unused_cache_ops_err_flags;
  logic                 unused_tensor_reduce_err_flags;
  minion_pkg::dcache_debug_sigs unused_csr_debug_signals;
  logic [minion_dcache_pkg::DcacheDbgSmMatchBits-1:0] unused_sm_match_debug_signals_tl0;
  logic [minion_dcache_pkg::DcacheDbgSmFilterBits-1:0] unused_sm_filter_debug_signals_tl0;
  logic [minion_dcache_pkg::DcacheDbgSmDataBits-1:0] unused_sm_data_debug_signals_tl0;
  logic [minion_dcache_pkg::DcacheDbgSmMatchBits-1:0] unused_sm_match_debug_signals_tl1;
  logic [minion_dcache_pkg::DcacheDbgSmFilterBits-1:0] unused_sm_filter_debug_signals_tl1;
  logic [minion_dcache_pkg::DcacheDbgSmDataBits-1:0] unused_sm_data_debug_signals_tl1;
  logic [minion_dcache_pkg::DcacheDbgSmMatchBits-1:0] unused_sm_match_debug_signals_ts;
  logic [minion_dcache_pkg::DcacheDbgSmFilterBits-1:0] unused_sm_filter_debug_signals_ts;
  logic [minion_dcache_pkg::DcacheDbgSmDataBits-1:0] unused_sm_data_debug_signals_ts;
  logic [minion_dcache_pkg::DcacheDbgSmMatchBits-1:0] unused_sm_match_debug_signals_co;
  logic [minion_dcache_pkg::DcacheDbgSmFilterBits-1:0] unused_sm_filter_debug_signals_co;
  logic [minion_dcache_pkg::DcacheDbgSmDataBits-1:0] unused_sm_data_debug_signals_co;
  logic [minion_dcache_pkg::DcacheDbgSmMatchBits-1:0] unused_sm_match_debug_signals_col2;
  logic [minion_dcache_pkg::DcacheDbgSmFilterBits-1:0] unused_sm_filter_debug_signals_col2;
  logic [minion_dcache_pkg::DcacheDbgSmDataBits-1:0] unused_sm_data_debug_signals_col2;
  logic [minion_dcache_pkg::DcacheMhFileSize-1:0][minion_dcache_pkg::DcacheDbgSmMatchBits-1:0] unused_sm_match_debug_signals_mh;
  logic [minion_dcache_pkg::DcacheMhFileSize-1:0][minion_dcache_pkg::DcacheDbgSmFilterBits-1:0] unused_sm_filter_debug_signals_mh;
  logic [minion_dcache_pkg::DcacheMhFileSize-1:0][minion_dcache_pkg::DcacheDbgSmDataBits-1:0] unused_sm_data_debug_signals_mh;
  logic [minion_dcache_pkg::DcacheDbgSmMatchBits-1:0] unused_sm_match_debug_signals_dc;
  logic [minion_dcache_pkg::DcacheDbgSmFilterBits-1:0] unused_sm_filter_debug_signals_dc;
  logic [minion_dcache_pkg::DcacheDbgSmDataBits-1:0] unused_sm_data_debug_signals_dc;

  localparam int unsigned DcDbgFilterAddrBits =
      minion_dcache_pkg::PaSize + (2 * minion_dcache_pkg::VaSize);
  localparam int unsigned DcDbgFilterS1ValidBit = DcDbgFilterAddrBits + 11;
  localparam int unsigned DcDbgFilterS1ValidToS2Bit = DcDbgFilterAddrBits + 13;
  localparam int unsigned DcDbgFilterS2ValidQualBit = DcDbgFilterAddrBits + 16;
  localparam int unsigned DcDbgDataGroup1Lsb = minion_dcache_pkg::DcacheDbgSmDataGroupBits;
  localparam int unsigned DcDbgDataGroup2Lsb = 2 * minion_dcache_pkg::DcacheDbgSmDataGroupBits;
  logic [3:0] dbg_s1_tag_match;
  logic [3:0] dbg_s2_tag_match_qual;

  /* verilator lint_off UNUSEDSIGNAL */  // The wrapper intentionally exposes only the top signals needed by the C++ test.
  always_comb begin
    core_ctrl = '0;

    core_req = '0;
    core_req.addr = req_addr_i;
    core_req.addr_msb_err = req_addr_msb_err_i;
    core_req.dest.fp = req_dest_fp_i;
    core_req.dest.addr = req_dest_addr_i;
    core_req.dest.thread_id = req_thread_id_i;
    core_req.cmd = minion_dcache_pkg::dcache_cmd_e'(req_cmd_i);
    core_req.typ = minion_dcache_pkg::dcache_type_e'(req_typ_i);
    core_req.gsc_cnt = req_gsc_cnt_i;
    core_req.ps_mask = req_ps_mask_i;
    core_req.phys = req_phys_i;
    core_req.gsc32_idx = req_gsc32_idx_i;
    core_req.mem_global = req_mem_global_i;

    vm_status = '{default: '0};
    vm_status[0].prv = vm_prv_t0_i;
    vm_status[1].prv = vm_prv_t1_i;

    satp_info = '{mode: satp_mode_i, ppn: satp_ppn_i};
    matp_info = '{mode: matp_mode_i, ppn: matp_ppn_i};

    ptw_resp_data = '0;
    ptw_resp_data.canceled_req = ptw_resp_canceled_req_i;
    ptw_resp_data.access_fault = ptw_resp_access_fault_i;
    ptw_resp_data.rsvd_for_hw = ptw_resp_rsvd_for_hw_i;
    ptw_resp_data.ppn = ptw_resp_ppn_i;
    ptw_resp_data.rsvd_for_sw = ptw_resp_rsvd_for_sw_i;
    ptw_resp_data.d = ptw_resp_d_i;
    ptw_resp_data.a = ptw_resp_a_i;
    ptw_resp_data.g = ptw_resp_g_i;
    ptw_resp_data.u = ptw_resp_u_i;
    ptw_resp_data.x = ptw_resp_x_i;
    ptw_resp_data.w = ptw_resp_w_i;
    ptw_resp_data.r = ptw_resp_r_i;
    ptw_resp_data.v = ptw_resp_v_i;

    s1_bp_conf_cmd_o = s1_bp_conf.cmd;
    s1_bp_conf_thread_id_o = s1_bp_conf.thread_id;
    s1_bp_conf_addr_o = s1_bp_conf.address;
    ptw_req_satp_mode_o = ptw_req_data.satp_mode;
    ptw_req_satp_ppn_o = ptw_req_data.satp_ppn;
    ptw_req_prv_o = ptw_req_data.prv;
    ptw_req_addr_o = ptw_req_data.addr;
    s3_core_resp_data_lo_o = unused_s3_core_resp.data[63:0];
    s3_core_resp_addr_o = unused_s3_core_resp.addr;

    metadata_mask_o = '0;
    metadata_mask_o[$bits(minion_dcache_pkg::dcache_meta_data_t)-1:0] =
        {$bits(minion_dcache_pkg::dcache_meta_data_t){1'b1}};

    dcache_hit_diag_o = '0;
    // Compact decode of the original dcache SM data/filter maps. Valid only
    // when DebugMonEn is set by the test head.
    dcache_hit_diag_o[3:0]   = unused_sm_data_debug_signals_dc[DcDbgDataGroup1Lsb + 103 +: 4];
    dcache_hit_diag_o[7:4]   = unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 100 +: 4];
    dcache_hit_diag_o[8]     = unused_sm_filter_debug_signals_dc[DcDbgFilterS1ValidToS2Bit];
    dcache_hit_diag_o[9]     = unused_sm_filter_debug_signals_dc[DcDbgFilterS2ValidQualBit];
    dcache_hit_diag_o[10]    = unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 99];
    dcache_hit_diag_o[11]    = unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 104];
    dcache_hit_diag_o[12]    = unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 97];
    dcache_hit_diag_o[13]    = unused_sm_data_debug_signals_dc[DcDbgDataGroup1Lsb + 100];
    dcache_hit_diag_o[14]    = unused_sm_data_debug_signals_dc[DcDbgDataGroup1Lsb + 97];
    dcache_hit_diag_o[15]    = unused_sm_data_debug_signals_dc[DcDbgDataGroup1Lsb + 96];
    dcache_hit_diag_o[16]    = unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 9];
    dcache_hit_diag_o[17]    = unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 8];
    dcache_hit_diag_o[18]    = unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 93];
    dcache_hit_diag_o[19]    = unused_sm_data_debug_signals_dc[DcDbgDataGroup1Lsb + 93];
    dcache_hit_diag_o[23:20] = unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 77 +: 4];
    dcache_hit_diag_o[28:24] = unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 81 +: 5];
    dcache_hit_diag_o[35:32] = unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 105 +: 4];
    dcache_hit_diag_o[39:36] = {
      unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 112],
      unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 111],
      unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 110],
      unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 109]
    };
    dcache_hit_diag_o[40] = unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 113];
    dcache_hit_diag_o[42:41] = unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 114 +: 2];
    dcache_hit_diag_o[44:43] = unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 116 +: 2];
    dcache_hit_diag_o[47:45] = {
      unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 120],
      unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 119],
      unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 118]
    };
    dcache_hit_diag_o[48] = unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 121];

    dbg_s1_tag_match = unused_sm_data_debug_signals_dc[DcDbgDataGroup1Lsb + 103 +: 4];
    dbg_s2_tag_match_qual = unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 100 +: 4];
    dcache_hit_sync_diag_o = {
      dbg_s2_tag_match_qual & {4{unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 99]}},
      dbg_s2_tag_match_qual & {4{unused_sm_filter_debug_signals_dc[DcDbgFilterS2ValidQualBit]}},
      dbg_s1_tag_match & {4{unused_sm_filter_debug_signals_dc[DcDbgFilterS1ValidToS2Bit]}},
      dbg_s1_tag_match & {4{unused_sm_filter_debug_signals_dc[DcDbgFilterS1ValidBit]}}
    };
    dcache_hit_gate_diag_o = {
      dbg_s1_tag_match & {4{unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 124]}},
      dbg_s1_tag_match & {4{unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 123]}},
      dbg_s1_tag_match & {4{unused_sm_data_debug_signals_dc[DcDbgDataGroup2Lsb + 122]}},
      dbg_s1_tag_match & {4{unused_sm_filter_debug_signals_dc[DcDbgFilterS1ValidToS2Bit]}}
    };
  end
  /* verilator lint_on UNUSEDSIGNAL */

  minion_dcache_top #(
    .DebugApbEn(DebugApbEn),
    .DebugMonEn(DebugMonEn),
    .VpuEn(VpuEn)
  ) u_dut (
    .clk_i,
    .rst_ni,
    .shire_id_i               ('0),
    .shire_min_id_i           ('0),
    .ioshire_i                (1'b0),
    .chicken_bit_dcache_i     (1'b0),
    .core_ctrl_i              (core_ctrl),
    .core_ctrl_resp_o         (unused_core_ctrl_resp),
    .id_core_alloc_rq_pre_i,
    .s0_core_alloc_rq_val_i,
    .id_core_gsc_i,
    .id_core_ready_o,
    .s0_core_req_valid_i,
    .s0_core_req_i            (core_req),
    .s0_core_gsc_i            (1'b0),
    .s1_core_kill_i           (1'b0),
    .s1_core_store_data_i     ({{(minion_pkg::VpuDataSz-64){1'b0}}, store_data_lo_i}),
    .s1_mprot_i               (minion_dcache_pkg::esr_mprot_t'('0)),
    .s2_core_kill_i           (1'b0),
    .s3_core_x31_i            ('0),
    .s1_vpu_ctrl_i            (minion_pkg::vpu_dcache_ctrl'('0)),
    .s3_vpu_scp_resp_o        (unused_vpu_scp_resp),
    .s3_vpu_scp_data_o        (unused_vpu_scp_data),
    .s3_vpu_tenb_data_o       (unused_vpu_tenb_data),
    .vpu_reduce_ctrl_o        (unused_vpu_reduce_ctrl),
    .s2_core_resp_int_valid_o,
    .s3_core_resp_valid_o,
    .s3_core_resp_o           (unused_s3_core_resp),
    .id_core_scoreboard_o     (unused_scoreboard),
    .id_core_sb_fp_dealloc_o  (unused_sb_fp_dealloc),
    .id_core_sb_int_dealloc_o (unused_sb_int_dealloc),
    .s1_core_replay_next_o,
    .s1_core_xcpt_o           (unused_s1_core_xcpt),
    .s2_core_flush_pipeline_o (unused_s2_core_flush_pipeline),
    .s3_ordered_o             (unused_s3_ordered),
    .s3_invalidate_lr_i       (1'b0),
    .wb_dcache_fp_toint_i     (1'b0),
    .vmspagesize_i            (minion_pkg::tlb_entry_type'(vmspagesize_i)),
    .bypass_dcache_i          (1'b0),
    .mem_ctrl_override_i      ('0),
    .satp_info_i              (satp_info),
    .matp_info_i              (matp_info),
    .satp_info_en_i,
    .matp_info_en_i,
    .vm_status_i              (vm_status),
    .tlb_invalidate_i         (1'b0),
    .ptw_req_data_o           (ptw_req_data),
    .ptw_req_valid_o,
    .ptw_req_ready_i,
    .ptw_resp_data_i          (ptw_resp_data),
    .ptw_resp_valid_i,
    .l2_evict_req_ready_i     ({minion_pkg::DcacheL2EvictReqPorts{1'b1}}),
    .l2_evict_req_valid_o     (unused_l2_evict_req_valid),
    .l2_evict_req_o           (unused_l2_evict_req),
    .l2_miss_req_ready_i      ({minion_pkg::DcacheL2MissReqPorts{1'b1}}),
    .l2_miss_req_valid_o,
    .l2_miss_req_o,
    .l2_resp_ready_o,
    .l2_resp_valid_i,
    .l2_resp_i,
    .l2_resp_ready_int_o      (unused_l2_resp_ready_int),
    .l2_resp_valid_int_o      (unused_l2_resp_valid_int),
    .l2_resp_int_o            (unused_l2_resp_int),
    .s1_bp_conf_o             (s1_bp_conf),
    .s1_bp_conf_valid_o,
    .dcache_idle_excl_mode_o  (unused_dcache_idle_excl_mode),
    .tensor_load_err_flags_o  (unused_tensor_load_err_flags),
    .cache_ops_err_flags_o    (unused_cache_ops_err_flags),
    .tensor_reduce_err_flags_o(unused_tensor_reduce_err_flags),
    .apb_paddr_i,
    .apb_pwrite_i,
    .apb_psel_i,
    .apb_penable_i,
    .apb_pwdata_i,
    .apb_pready_o,
    .apb_prdata_o,
    .apb_pslverr_o,
    .io_events_o,
    .bus_err_o,
    .bus_err_addr_o           (unused_bus_err_addr),
    .csr_debug_sigs_o         (unused_csr_debug_signals),
    .sm_match_debug_signals_tl0_o(unused_sm_match_debug_signals_tl0),
    .sm_filter_debug_signals_tl0_o(unused_sm_filter_debug_signals_tl0),
    .sm_data_debug_signals_tl0_o(unused_sm_data_debug_signals_tl0),
    .sm_match_debug_signals_tl1_o(unused_sm_match_debug_signals_tl1),
    .sm_filter_debug_signals_tl1_o(unused_sm_filter_debug_signals_tl1),
    .sm_data_debug_signals_tl1_o(unused_sm_data_debug_signals_tl1),
    .sm_match_debug_signals_ts_o(unused_sm_match_debug_signals_ts),
    .sm_filter_debug_signals_ts_o(unused_sm_filter_debug_signals_ts),
    .sm_data_debug_signals_ts_o(unused_sm_data_debug_signals_ts),
    .sm_match_debug_signals_co_o(unused_sm_match_debug_signals_co),
    .sm_filter_debug_signals_co_o(unused_sm_filter_debug_signals_co),
    .sm_data_debug_signals_co_o(unused_sm_data_debug_signals_co),
    .sm_match_debug_signals_col2_o(unused_sm_match_debug_signals_col2),
    .sm_filter_debug_signals_col2_o(unused_sm_filter_debug_signals_col2),
    .sm_data_debug_signals_col2_o(unused_sm_data_debug_signals_col2),
    .sm_match_debug_signals_mh_o(unused_sm_match_debug_signals_mh),
    .sm_filter_debug_signals_mh_o(unused_sm_filter_debug_signals_mh),
    .sm_data_debug_signals_mh_o(unused_sm_data_debug_signals_mh),
    .sm_match_debug_signals_dc_o(unused_sm_match_debug_signals_dc),
    .sm_filter_debug_signals_dc_o(unused_sm_filter_debug_signals_dc),
    .sm_data_debug_signals_dc_o(unused_sm_data_debug_signals_dc)
  );

endmodule
/* verilator lint_on SYNCASYNCNET */
