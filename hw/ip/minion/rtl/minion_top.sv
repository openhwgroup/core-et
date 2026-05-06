// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// minion_top -- Minion top-level integration wrapper.
//
// Mechanical translation of rtl/shire/minion/minion_top.v. This wrapper keeps
// the original reset-repeaters, debug-status muxing, memory-override glue, and
// core/VPU split. The default VpuEn=1 configuration instantiates the translated
// VPU; VpuEn=0 preserves the intentional integer-only null_vpu bring-up path.

/* verilator lint_off UNUSEDPARAM */  // `UseMmi` is a preserved original top-level parameter kept for interface compatibility.
module minion_top
  import dft_pkg::*;
  import minion_pkg::*;
  import minion_frontend_pkg::*;
  import minion_dcache_pkg::*;
#(
  parameter int unsigned UseMmi = 1,  // Preserved for interface compatibility.
  parameter bit          DebugApbEn = 1'b1,
  parameter bit          DebugMonEn = 1'b1,
  parameter bit          TraceEn = 1'b1,
  parameter bit          VpuEn = 1'b1
) (
  input  logic                                              clk_i,
  input  logic                                              rst_c_ni,
  input  logic                                              rst_d_ni,
  input  logic                                              rst_w_ni,
  input  dft_t                                              dft_i,

  input  logic [EtEcoMinionInputWidth-1:0]                  eco_i,
  output logic [EtEcoMinionOutputWidth-1:0]                 eco_o,

  input  logic                                              ioshire_i,

  input  logic                                              nsleepin_i,
  input  logic                                              iso_enable_i,
  output logic                                              nsleepout_o,

  input  minion_chicken_bits_t                              chicken_bits_i,

  input  logic [DcacheL2EvictReqPorts-1:0]                  l2_dcache_evict_req_ready_i,
  output logic [DcacheL2EvictReqPorts-1:0]                  l2_dcache_evict_req_valid_o,
  output et_link_minion_evict_req_info_t                    l2_dcache_evict_req_o,

  input  logic [DcacheL2MissReqPorts-1:0]                   l2_dcache_miss_req_ready_i,
  output logic [DcacheL2MissReqPorts-1:0]                   l2_dcache_miss_req_valid_o,
  output et_link_minion_miss_req_info_t                     l2_dcache_miss_req_o,

  output logic                                              l2_dcache_resp_ready_o,
  input  logic                                              l2_dcache_resp_valid_i,
  input  et_link_minion_rsp_info_t                          l2_dcache_resp_i,

  input  logic                                              icache_req_ready_i,
  output logic                                              icache_req_valid_o,
  output fe_icache_req_t                                    icache_req_o,

  input  logic                                              icache_resp_valid_i,
  input  logic                                              icache_resp_miss_i,
  input  icache_fe_resp_t                                   icache_resp_i,
  input  logic                                              icache_fill_done_i,
  output logic                                              icache_flush_data_o,

  output minion_satp_info                                   satp_info_o,
  output minion_satp_info                                   matp_info_o,
  output logic                                              tlb_invalidate_o,

  output minion_ptw_req                                     dc_ptw_req_data_o,
  output logic                                              dc_ptw_req_valid_o,
  input  logic                                              dc_ptw_req_ready_i,
  input  minion_ptw_pte                                     ptw_dc_resp_data_i,
  input  logic                                              ptw_dc_resp_valid_i,

  input  minion_interrupts                                  interrupts_i,
  input  logic [NumShireIdsBits-1:0]                        shire_id_i,
  input  logic [MinPerSBits-1:0]                            shire_min_id_i,
  input  logic [NrThreads-1:0]                              enabled_i,
  input  logic [minion_pkg::VaSize-1:0]                     reset_vector_i,
  input  esr_mprot_t                                        mprot_i,
  input  tlb_entry_type                                     vmspagesize_i,

  output logic                                              flb_neigh_req_valid_o,
  output logic [CsrFlBarrierSize-1:0]                       flb_neigh_req_data_o,
  input  logic                                              flb_neigh_resp_valid_i,
  input  logic                                              flb_neigh_resp_data_i,

  input  logic                                              te_thread_sel_i,
  output trace_encoder_signals_t                            trace_encoder_o,
  input  logic                                              te_enable_i,

  input  logic [MinDmApbAddrWidth-1:0]                      apb_paddr_i,
  input  logic                                              apb_penable_i,
  output logic [BpamShireApbDataWidth-1:0]                  apb_prdata_o,
  output logic                                              apb_pready_o,
  input  logic                                              apb_psel_i,
  output logic                                              apb_pslverr_o,
  input  logic [BpamShireApbDataWidth-1:0]                  apb_pwdata_i,
  input  logic                                              apb_pwrite_i,

  input  minion_debug_in_t                                  debug_in_i,
  output minion_debug_out_t                                 debug_out_o,
  output neigh_sm_dbg_monitor_t                             minion_dbg_signals_o,
  /* verilator lint_off UNUSEDSIGNAL */  // `DebugMonEn=0` structurally removes the top-level debug monitor fabric, leaving the preserved debug-select inputs intentionally unused in stripped FPGA builds.
  input  logic [NeighDebugSmMuxWidth-1:0]                   minion_dbg_signals_mux_i,
  input  logic                                              minion_dbg_sig_enable_i,
  /* verilator lint_on UNUSEDSIGNAL */

  input  esr_minion_features_t                              esr_features_i,
  input  logic                                              esr_bypass_dcache_i,
  input  logic                                              esr_shire_coop_mode_i,
  input  logic [ShireMinionMemOverrideBits-1:0]             esr_minion_mem_override_i,

  output logic [PmuMinionCountersNum-1:0]                   pmu_count_up_o,
  input  logic [NrThreads-1:0][minion_pkg::XregSize-1:0]    pmu_read_data_i,
  output logic [NrThreads-1:0][PmuCountersSelectBits-1:0]   pmu_read_sel_o,
  output logic [PmuTotalCountersNum-1:0]                    pmu_write_en_o,
  output logic [minion_pkg::XregSize-1:0]                   pmu_write_data_o,
  output logic [PmuNeighEventCntSelBits-1:0]                pmu_neigh_event_sel_o
);
  /* verilator lint_on UNUSEDPARAM */

  logic                 rst_c_sync_ni;
  logic                 rst_d_sync_ni;
  logic                 rst_w_sync_ni;

  minion_vpu_id_req     id_core_vpu_req;
  vpu_ctrl_sigs_t       id_vpu_decoder_sigs;
  minion_vpu_ex_req     ex_core_vpu_req;
  logic                 tag_core_vpu_kill;
  logic                 mem_core_vpu_kill;
  logic                 wb_core_vpu_kill;
  core_vpu_ctrl         f0_core_vpu_ctrl;
  logic                 wb_dcache_vpu_resp_valid;
  dcache_vpu_resp       wb_dcache_vpu_resp;
  vpu_dcache_ctrl       vpu_dcache_ctl;
  dcache_vpu_scp_resp   dcache_vpu_scp_res;
  logic [VpuDataSz-1:0] dcache_vpu_scp_data;
  logic [VpuDataSz-1:0] dcache_vpu_tenb_data;
  dcache_vpu_reduce_ctrl vpu_reduce_ctrl;
  vpu_id_ctrl_t         id_vpu_core_ctrl;
  vpu_minion_ex_ctrl    ex_vpu_core_ctrl;
  vpu_minion_tag_ctrl   f2_vpu_core_ctrl;
  vpu_minion_mem_ctrl   f3_vpu_core_ctrl;
  vpu_minion_wb_ctrl    wb_vpu_core_ctrl;
  logic [NrThreads-1:0] enabled_reg_q;

  logic [CsrNrEventsVpu-1:0]            io_events_vpu;
  /* verilator lint_off UNUSEDSIGNAL */  // `DebugMonEn=0` strips the external debug monitor mux, so the preserved per-block debug buses become intentionally unused in that build.
  logic [NeighDebugMatchWidth-1:0]      vpu_dbg_match;
  logic [NeighDebugFilterWidth-1:0]     vpu_dbg_filter;
  logic [4:0][NeighDebugDataWidth-1:0]  vpu_dbg_data;
  minion_debug_in_t                     debug_in_reg_q;

  logic [DcacheDbgSmMatchBits-1:0]      sm_match_debug_signals_tl0;
  logic [DcacheDbgSmFilterBits-1:0]     sm_filter_debug_signals_tl0;
  logic [DcacheDbgSmDataBits-1:0]       sm_data_debug_signals_tl0;
  logic [DcacheDbgSmMatchBits-1:0]      sm_match_debug_signals_tl1;
  logic [DcacheDbgSmFilterBits-1:0]     sm_filter_debug_signals_tl1;
  logic [DcacheDbgSmDataBits-1:0]       sm_data_debug_signals_tl1;
  logic [DcacheDbgSmMatchBits-1:0]      sm_match_debug_signals_ts;
  logic [DcacheDbgSmFilterBits-1:0]     sm_filter_debug_signals_ts;
  logic [DcacheDbgSmDataBits-1:0]       sm_data_debug_signals_ts;
  logic [DcacheDbgSmMatchBits-1:0]      sm_match_debug_signals_co;
  logic [DcacheDbgSmFilterBits-1:0]     sm_filter_debug_signals_co;
  logic [DcacheDbgSmDataBits-1:0]       sm_data_debug_signals_co;
  logic [DcacheDbgSmMatchBits-1:0]      sm_match_debug_signals_col2;
  logic [DcacheDbgSmFilterBits-1:0]     sm_filter_debug_signals_col2;
  logic [DcacheDbgSmDataBits-1:0]       sm_data_debug_signals_col2;
  logic [DcacheMhFileSize-1:0][DcacheDbgSmMatchBits-1:0]  sm_match_debug_signals_mh;
  logic [DcacheMhFileSize-1:0][DcacheDbgSmFilterBits-1:0] sm_filter_debug_signals_mh;
  logic [DcacheMhFileSize-1:0][DcacheDbgSmDataBits-1:0]   sm_data_debug_signals_mh;
  logic [DcacheDbgSmMatchBits-1:0]      sm_match_debug_signals_dc;
  logic [DcacheDbgSmFilterBits-1:0]     sm_filter_debug_signals_dc;
  logic [DcacheDbgSmDataBits-1:0]       sm_data_debug_signals_dc;

  intpipe_dbg_monitor_t                 intpipe_debug_monitor_out;
  intpipe_dbg_match_t                   intpipe_dbg_match;

  /* verilator lint_off UNUSEDSIGNAL */  // The original top preserves two reserved chicken-bit fields that intentionally remain unused in the translated clock-gate controls.
  minion_chicken_bits_t                 chicken_bits_clock_gate_disable_q;
  /* verilator lint_on UNUSEDSIGNAL */
  logic                                 dcache_clock_gate_disable;
  logic                                 intpipe_clock_gate_disable;
  logic                                 frontend_clock_gate_disable;
  logic                                 vpulane_clock_gate_disable;
  logic                                 vputima_clock_gate_disable;
  logic                                 vputrans_clock_gate_disable;

  /* verilator lint_off UNUSEDSIGNAL */  // `DebugMonEn=0` strips the top-level debug monitor state, so the preserved monitor staging registers become intentionally unused in stripped builds.
  neigh_sm_dbg_monitor_t                minion_dbg_signals_next;
  logic [NeighDebugSmMuxWidth-1:0]      minion_dbg_signals_mux_ff;
  neigh_sm_dbg_monitor_t                minion_dbg_signals_next_0;
  neigh_sm_dbg_monitor_t                minion_dbg_signals_next_1;
  neigh_sm_dbg_monitor_t                minion_dbg_signals_0_q;
  neigh_sm_dbg_monitor_t                minion_dbg_signals_1_q;
  /* verilator lint_on UNUSEDSIGNAL */

  logic [MinionMemCtrlBits-1:0]         mem_ctrl_override;
  logic                                 unused_iso_enable;

  function automatic logic [NeighDebugDataWidth-1:0] dcache_dbg_data_group(
      input logic [DcacheDbgSmDataBits-1:0] data_i,
      input logic [NeighDebugSmMuxDataWidth-1:0] sel_i);
    dcache_dbg_data_group =
        data_i[sel_i * DcacheDbgSmDataGroupBits +: DcacheDbgSmDataGroupBits];
  endfunction

  prim_rst_sync u_rst_sync_c (
    .clk_i (clk_i),
    .rst_ni(rst_c_ni),
    .dft_i (dft_i),
    .rst_no(rst_c_sync_ni)
  );

  prim_rst_sync u_rst_sync_d (
    .clk_i (clk_i),
    .rst_ni(rst_d_ni),
    .dft_i (dft_i),
    .rst_no(rst_d_sync_ni)
  );

  prim_rst_sync u_rst_sync_w (
    .clk_i (clk_i),
    .rst_ni(rst_w_ni),
    .dft_i (dft_i),
    .rst_no(rst_w_sync_ni)
  );

  prim_eco_ports #(
    .InputWidth (EtEcoMinionInputWidth),
    .OutputWidth(EtEcoMinionOutputWidth)
  ) u_eco_ports (
    .eco_i(eco_i),
    .eco_o(eco_o)
  );

  assign nsleepout_o = nsleepin_i;
  assign unused_iso_enable = &{1'b0, iso_enable_i};

  always_ff @(posedge clk_i or negedge rst_c_ni) begin
    if (!rst_c_ni) begin
      chicken_bits_clock_gate_disable_q <= '0;
      debug_in_reg_q <= '0;
      enabled_reg_q <= '0;
    end else begin
      chicken_bits_clock_gate_disable_q <= chicken_bits_i;
      debug_in_reg_q <= debug_in_i;
      enabled_reg_q <= enabled_i;
    end
  end

  assign dcache_clock_gate_disable = chicken_bits_clock_gate_disable_q.min_dcache_clock_gate_disable;
  assign intpipe_clock_gate_disable = chicken_bits_clock_gate_disable_q.min_intpipe_clock_gate_disable;
  assign frontend_clock_gate_disable = chicken_bits_clock_gate_disable_q.min_frontend_clock_gate_disable;
  assign vpulane_clock_gate_disable = chicken_bits_clock_gate_disable_q.min_vpulane_clock_gate_disable;
  assign vputima_clock_gate_disable = chicken_bits_clock_gate_disable_q.min_vputima_clock_gate_disable;
  assign vputrans_clock_gate_disable = chicken_bits_clock_gate_disable_q.min_vputrans_clock_gate_disable;

  if (DebugMonEn) begin : gen_debug_monitor
    always_comb begin
      intpipe_dbg_match = '0;
      intpipe_dbg_match.id_csr_interrupt = intpipe_debug_monitor_out.id_csr_interrupt;
      intpipe_dbg_match.excl_mode_stall = intpipe_debug_monitor_out.excl_mode_stall;
      intpipe_dbg_match.ipi_with_pc = intpipe_debug_monitor_out.ipi_with_pc;
      intpipe_dbg_match.id_xcpt = intpipe_debug_monitor_out.id_xcpt;
      intpipe_dbg_match.wb_xcpt = intpipe_debug_monitor_out.wb_xcpt;
      intpipe_dbg_match.csr_fe_stall = intpipe_debug_monitor_out.csr_fe_stall;
      intpipe_dbg_match.id_do_fence = intpipe_debug_monitor_out.id_do_fence;
    end

    always_comb begin
      minion_dbg_signals_next_0 = '0;

      unique case (minion_dbg_signals_mux_ff[NeighDebugSmMuxWidth-1:NeighDebugSmMuxDataWidth])
        5'h0: begin
          minion_dbg_signals_next_0.filter_valid = 1'b1;
          minion_dbg_signals_next_0.match_valid = 1'b1;
          minion_dbg_signals_next_0.data_valid = 1'b1;
          minion_dbg_signals_next_0.match_signals = vpu_dbg_match;
          minion_dbg_signals_next_0.filter_signals = vpu_dbg_filter;
          minion_dbg_signals_next_0.data_signals =
              vpu_dbg_data[minion_dbg_signals_mux_ff[NeighDebugSmMuxDataWidth-1:0]];
        end
        5'h1: begin
          minion_dbg_signals_next_0.filter_valid = 1'b1;
          minion_dbg_signals_next_0.match_valid = 1'b1;
          minion_dbg_signals_next_0.data_valid = 1'b1;
          minion_dbg_signals_next_0.match_signals = vpu_dbg_match;
          minion_dbg_signals_next_0.filter_signals = vpu_dbg_filter;
          minion_dbg_signals_next_0.data_signals = vpu_dbg_data[4];
        end
        5'h2: begin
          minion_dbg_signals_next_0.filter_valid = 1'b1;
          minion_dbg_signals_next_0.match_valid = 1'b1;
          minion_dbg_signals_next_0.data_valid = 1'b1;
          minion_dbg_signals_next_0.match_signals = sm_match_debug_signals_tl0;
          minion_dbg_signals_next_0.filter_signals = sm_filter_debug_signals_tl0;
          minion_dbg_signals_next_0.data_signals = dcache_dbg_data_group(
              sm_data_debug_signals_tl0, minion_dbg_signals_mux_ff[NeighDebugSmMuxDataWidth-1:0]);
        end
        5'h3: begin
          minion_dbg_signals_next_0.filter_valid = 1'b1;
          minion_dbg_signals_next_0.match_valid = 1'b1;
          minion_dbg_signals_next_0.data_valid = 1'b1;
          minion_dbg_signals_next_0.match_signals = sm_match_debug_signals_tl1;
          minion_dbg_signals_next_0.filter_signals = sm_filter_debug_signals_tl1;
          minion_dbg_signals_next_0.data_signals = dcache_dbg_data_group(
              sm_data_debug_signals_tl1, minion_dbg_signals_mux_ff[NeighDebugSmMuxDataWidth-1:0]);
        end
        5'h4: begin
          minion_dbg_signals_next_0.filter_valid = 1'b1;
          minion_dbg_signals_next_0.match_valid = 1'b1;
          minion_dbg_signals_next_0.data_valid = 1'b1;
          minion_dbg_signals_next_0.match_signals = sm_match_debug_signals_ts;
          minion_dbg_signals_next_0.filter_signals = sm_filter_debug_signals_ts;
          minion_dbg_signals_next_0.data_signals = dcache_dbg_data_group(
              sm_data_debug_signals_ts, minion_dbg_signals_mux_ff[NeighDebugSmMuxDataWidth-1:0]);
        end
        5'h5: begin
          minion_dbg_signals_next_0.filter_valid = 1'b1;
          minion_dbg_signals_next_0.match_valid = 1'b1;
          minion_dbg_signals_next_0.data_valid = 1'b1;
          minion_dbg_signals_next_0.match_signals = sm_match_debug_signals_co;
          minion_dbg_signals_next_0.filter_signals = sm_filter_debug_signals_co;
          minion_dbg_signals_next_0.data_signals = dcache_dbg_data_group(
              sm_data_debug_signals_co, minion_dbg_signals_mux_ff[NeighDebugSmMuxDataWidth-1:0]);
        end
        default: begin
        end
      endcase
    end

    always_comb begin
      minion_dbg_signals_next_1 = '0;

      unique case (minion_dbg_signals_mux_ff[NeighDebugSmMuxWidth-1:NeighDebugSmMuxDataWidth])
        5'h6: begin
          minion_dbg_signals_next_1.filter_valid = 1'b1;
          minion_dbg_signals_next_1.match_valid = 1'b1;
          minion_dbg_signals_next_1.data_valid = 1'b1;
          minion_dbg_signals_next_1.match_signals = sm_match_debug_signals_col2;
          minion_dbg_signals_next_1.filter_signals = sm_filter_debug_signals_col2;
          minion_dbg_signals_next_1.data_signals = dcache_dbg_data_group(
              sm_data_debug_signals_col2, minion_dbg_signals_mux_ff[NeighDebugSmMuxDataWidth-1:0]);
        end
        5'h7: begin
          minion_dbg_signals_next_1.filter_valid = 1'b1;
          minion_dbg_signals_next_1.match_valid = 1'b1;
          minion_dbg_signals_next_1.data_valid = 1'b1;
          minion_dbg_signals_next_1.match_signals = sm_match_debug_signals_mh[0];
          minion_dbg_signals_next_1.filter_signals = sm_filter_debug_signals_mh[0];
          minion_dbg_signals_next_1.data_signals = dcache_dbg_data_group(
              sm_data_debug_signals_mh[0], minion_dbg_signals_mux_ff[NeighDebugSmMuxDataWidth-1:0]);
        end
        5'h8: begin
          minion_dbg_signals_next_1.filter_valid = 1'b1;
          minion_dbg_signals_next_1.match_valid = 1'b1;
          minion_dbg_signals_next_1.data_valid = 1'b1;
          minion_dbg_signals_next_1.match_signals = sm_match_debug_signals_mh[1];
          minion_dbg_signals_next_1.filter_signals = sm_filter_debug_signals_mh[1];
          minion_dbg_signals_next_1.data_signals = dcache_dbg_data_group(
              sm_data_debug_signals_mh[1], minion_dbg_signals_mux_ff[NeighDebugSmMuxDataWidth-1:0]);
        end
        5'h9: begin
          minion_dbg_signals_next_1.filter_valid = 1'b1;
          minion_dbg_signals_next_1.match_valid = 1'b1;
          minion_dbg_signals_next_1.data_valid = 1'b1;
          minion_dbg_signals_next_1.match_signals = sm_match_debug_signals_dc;
          minion_dbg_signals_next_1.filter_signals = sm_filter_debug_signals_dc;
          minion_dbg_signals_next_1.data_signals = dcache_dbg_data_group(
              sm_data_debug_signals_dc, minion_dbg_signals_mux_ff[NeighDebugSmMuxDataWidth-1:0]);
        end
        5'ha: begin
          minion_dbg_signals_next_1.filter_valid = 1'b1;
          minion_dbg_signals_next_1.match_valid = 1'b1;
          minion_dbg_signals_next_1.data_valid = 1'b1;
          minion_dbg_signals_next_1.filter_signals = '0;
          minion_dbg_signals_next_1.filter_signals[$bits(intpipe_debug_monitor_out)-1:0] =
              intpipe_debug_monitor_out;
          minion_dbg_signals_next_1.match_signals = '0;
          minion_dbg_signals_next_1.match_signals[$bits(intpipe_dbg_match)-1:0] = intpipe_dbg_match;
          minion_dbg_signals_next_1.data_signals = '0;
          minion_dbg_signals_next_1.data_signals[$bits(intpipe_debug_monitor_out)-1:0] =
              intpipe_debug_monitor_out;
        end
        default: begin
        end
      endcase
    end

    always_comb begin
      // Preserve the original CORE-ET mux select quirk: the case decode uses
      // bits [6:2], but the final bank select compares only bits [5:2].
      if (minion_dbg_signals_mux_ff[NeighDebugSmMuxWidth-2:NeighDebugSmMuxDataWidth] > 4'h5) begin
        minion_dbg_signals_next = minion_dbg_signals_1_q;
      end else begin
        minion_dbg_signals_next = minion_dbg_signals_0_q;
      end
    end

    always_ff @(posedge clk_i) begin
      if (minion_dbg_sig_enable_i) begin
        minion_dbg_signals_0_q <= minion_dbg_signals_next_0;
        minion_dbg_signals_1_q <= minion_dbg_signals_next_1;
        minion_dbg_signals_o <= minion_dbg_signals_next;
        minion_dbg_signals_mux_ff <= minion_dbg_signals_mux_i;
      end
    end
  end else begin : gen_no_debug_monitor
    assign intpipe_dbg_match = '0;
    assign minion_dbg_signals_o = '0;
  end

  always_comb begin
    // The original top uses DFT spare-bus bit 9 to force both memory override
    // controls. The repo-native DFT bundle carries explicit RAM read/write
    // inhibit bits, so use those as the same top-level override source.
    mem_ctrl_override = '0;
    mem_ctrl_override[MinionMemCtrlWe] = dft_i.ram_wei;
    mem_ctrl_override[MinionMemCtrlRe] = dft_i.ram_rei;
    if (esr_minion_mem_override_i[ShireMinionMemOverrideSel]) begin
      mem_ctrl_override[MinionMemCtrlWe] |= esr_minion_mem_override_i[ShireMinionMemOverrideWe];
      mem_ctrl_override[MinionMemCtrlRe] |= esr_minion_mem_override_i[ShireMinionMemOverrideRe];
    end
  end

  /* verilator lint_off PINCONNECTEMPTY */  // The top-level wrapper intentionally preserves the original external interface while leaving no local-only observability points connected.
  core_top #(
    .UseMmi (UseMmi),
    .DebugApbEn(DebugApbEn),
    .DebugMonEn(DebugMonEn),
    .TraceEn(TraceEn),
    .VpuEn  (VpuEn)
  ) u_core (
    .clk_i                      (clk_i),
    .rst_c_ni                   (rst_c_sync_ni),
    .rst_w_ni                   (rst_w_sync_ni),
    .rst_d_ni                   (rst_d_sync_ni),
    .dft_i                      (dft_i),
    .ioshire_i                  (ioshire_i),
    .chicken_bit_dcache_i       (dcache_clock_gate_disable),
    .chicken_bit_intpipe_i      (intpipe_clock_gate_disable),
    .chicken_bit_frontend_i     (frontend_clock_gate_disable),
    .l2_dcache_evict_req_ready_i(l2_dcache_evict_req_ready_i),
    .l2_dcache_evict_req_valid_o(l2_dcache_evict_req_valid_o),
    .l2_dcache_evict_req_o      (l2_dcache_evict_req_o),
    .l2_dcache_miss_req_ready_i (l2_dcache_miss_req_ready_i),
    .l2_dcache_miss_req_valid_o (l2_dcache_miss_req_valid_o),
    .l2_dcache_miss_req_o       (l2_dcache_miss_req_o),
    .l2_dcache_resp_ready_o     (l2_dcache_resp_ready_o),
    .l2_dcache_resp_valid_i     (l2_dcache_resp_valid_i),
    .l2_dcache_resp_i           (l2_dcache_resp_i),
    .icache_req_ready_i         (icache_req_ready_i),
    .icache_req_valid_o         (icache_req_valid_o),
    .icache_req_o               (icache_req_o),
    .icache_resp_valid_i        (icache_resp_valid_i),
    .icache_resp_miss_i         (icache_resp_miss_i),
    .icache_resp_i              (icache_resp_i),
    .icache_fill_done_i         (icache_fill_done_i),
    .icache_flush_data_o        (icache_flush_data_o),
    .satp_info_o                (satp_info_o),
    .matp_info_o                (matp_info_o),
    .tlb_invalidate_o           (tlb_invalidate_o),
    .dc_ptw_req_data_o          (dc_ptw_req_data_o),
    .dc_ptw_req_valid_o         (dc_ptw_req_valid_o),
    .dc_ptw_req_ready_i         (dc_ptw_req_ready_i),
    .ptw_dc_resp_data_i         (ptw_dc_resp_data_i),
    .ptw_dc_resp_valid_i        (ptw_dc_resp_valid_i),
    .interrupts_i               (interrupts_i),
    .shire_id_i                 (shire_id_i),
    .shire_min_id_i             (shire_min_id_i),
    .enabled_i                  (enabled_reg_q),
    .reset_vector_i             (reset_vector_i),
    .mprot_i                    (mprot_i),
    .vmspagesize_i              (vmspagesize_i),
    .id_vpu_req_o               (id_core_vpu_req),
    .id_vpu_decoder_sigs_o      (id_vpu_decoder_sigs),
    .ex_vpu_req_o               (ex_core_vpu_req),
    .tag_vpu_kill_o             (tag_core_vpu_kill),
    .mem_vpu_kill_o             (mem_core_vpu_kill),
    .wb_vpu_kill_o              (wb_core_vpu_kill),
    .f0_vpu_ctrl_o              (f0_core_vpu_ctrl),
    .wb_dcache_vpu_resp_valid_o (wb_dcache_vpu_resp_valid),
    .wb_dcache_vpu_resp_o       (wb_dcache_vpu_resp),
    .vpu_dcache_ctl_i           (vpu_dcache_ctl),
    .dcache_vpu_scp_res_o       (dcache_vpu_scp_res),
    .dcache_vpu_scp_data_o      (dcache_vpu_scp_data),
    .dcache_vpu_tenb_data_o     (dcache_vpu_tenb_data),
    .vpu_reduce_ctrl_o          (vpu_reduce_ctrl),
    .id_vpu_ctrl_i              (id_vpu_core_ctrl),
    .ex_vpu_ctrl_i              (ex_vpu_core_ctrl),
    .f2_vpu_ctrl_i              (f2_vpu_core_ctrl),
    .f3_vpu_ctrl_i              (f3_vpu_core_ctrl),
    .wb_vpu_ctrl_i              (wb_vpu_core_ctrl),
    .flb_neigh_req_valid_o      (flb_neigh_req_valid_o),
    .flb_neigh_req_data_o       (flb_neigh_req_data_o),
    .flb_neigh_resp_valid_i     (flb_neigh_resp_valid_i),
    .flb_neigh_resp_data_i      (flb_neigh_resp_data_i),
    .te_thread_sel_i            (te_thread_sel_i),
    .trace_encoder_o            (trace_encoder_o),
    .te_enable_i                (te_enable_i),
    .apb_paddr_i                (apb_paddr_i),
    .apb_pwrite_i               (apb_pwrite_i),
    .apb_psel_i                 (apb_psel_i),
    .apb_penable_i              (apb_penable_i),
    .apb_pwdata_i               (apb_pwdata_i),
    .apb_pready_o               (apb_pready_o),
    .apb_prdata_o               (apb_prdata_o),
    .apb_pslverr_o              (apb_pslverr_o),
    .debug_in_i                 (debug_in_reg_q),
    .debug_out_o                (debug_out_o),
    .esr_features_i             (esr_features_i),
    .esr_bypass_dcache_i        (esr_bypass_dcache_i),
    .esr_shire_coop_mode_i      (esr_shire_coop_mode_i),
    .mem_ctrl_override_i        (mem_ctrl_override),
    .pmu_count_up_o             (pmu_count_up_o),
    .pmu_read_data_i            (pmu_read_data_i),
    .pmu_read_sel_o             (pmu_read_sel_o),
    .pmu_write_en_o             (pmu_write_en_o),
    .pmu_write_data_o           (pmu_write_data_o),
    .pmu_neigh_event_sel_o      (pmu_neigh_event_sel_o),
    .io_events_vpu_i            (io_events_vpu),
    .sm_match_debug_signals_tl0_o(sm_match_debug_signals_tl0),
    .sm_filter_debug_signals_tl0_o(sm_filter_debug_signals_tl0),
    .sm_data_debug_signals_tl0_o(sm_data_debug_signals_tl0),
    .sm_match_debug_signals_tl1_o(sm_match_debug_signals_tl1),
    .sm_filter_debug_signals_tl1_o(sm_filter_debug_signals_tl1),
    .sm_data_debug_signals_tl1_o(sm_data_debug_signals_tl1),
    .sm_match_debug_signals_ts_o(sm_match_debug_signals_ts),
    .sm_filter_debug_signals_ts_o(sm_filter_debug_signals_ts),
    .sm_data_debug_signals_ts_o (sm_data_debug_signals_ts),
    .sm_match_debug_signals_co_o(sm_match_debug_signals_co),
    .sm_filter_debug_signals_co_o(sm_filter_debug_signals_co),
    .sm_data_debug_signals_co_o (sm_data_debug_signals_co),
    .sm_match_debug_signals_col2_o(sm_match_debug_signals_col2),
    .sm_filter_debug_signals_col2_o(sm_filter_debug_signals_col2),
    .sm_data_debug_signals_col2_o(sm_data_debug_signals_col2),
    .sm_match_debug_signals_mh_o(sm_match_debug_signals_mh),
    .sm_filter_debug_signals_mh_o(sm_filter_debug_signals_mh),
    .sm_data_debug_signals_mh_o (sm_data_debug_signals_mh),
    .sm_match_debug_signals_dc_o(sm_match_debug_signals_dc),
    .sm_filter_debug_signals_dc_o(sm_filter_debug_signals_dc),
    .sm_data_debug_signals_dc_o (sm_data_debug_signals_dc),
    .intpipe_debug_monitor_o    (intpipe_debug_monitor_out)
  );

  if (VpuEn) begin : gen_real_vpu
    vpu_top u_vpu (
      .clock                 (clk_i),
      .reset                 (!rst_c_sync_ni),
      .id_core_req           (id_core_vpu_req),
      .id_vpu_decoder_sigs   (id_vpu_decoder_sigs),
      .ex_core_req           (ex_core_vpu_req),
      .f2_core_kill          (tag_core_vpu_kill),
      .f3_core_kill          (mem_core_vpu_kill),
      .f4_core_kill          (wb_core_vpu_kill),
      .f0_core_ctrl          (f0_core_vpu_ctrl),
      .chicken_bit_vpulane   (vpulane_clock_gate_disable),
      .chicken_bit_vputima   (vputima_clock_gate_disable),
      .chicken_bit_vputrans  (vputrans_clock_gate_disable),
      .mem_ctrl_override     (mem_ctrl_override),
      .wb_dcache_resp_valid  (wb_dcache_vpu_resp_valid),
      .wb_dcache_resp        (wb_dcache_vpu_resp),
      .dcache_ctrl           (vpu_dcache_ctl),
      .dcache_scp_resp       (dcache_vpu_scp_res),
      .dcache_scp_data       (dcache_vpu_scp_data),
      .dcache_tenb_data      (dcache_vpu_tenb_data),
      .dcache_reduce_ctrl    (vpu_reduce_ctrl),
      .id_core_ctrl          (id_vpu_core_ctrl),
      .ex_core_ctrl          (ex_vpu_core_ctrl),
      .f2_core_ctrl          (f2_vpu_core_ctrl),
      .f3_core_ctrl          (f3_vpu_core_ctrl),
      .wb_core_ctrl          (wb_vpu_core_ctrl),
      .io_events             (io_events_vpu),
      .vpu_dbg_match         (vpu_dbg_match),
      .vpu_dbg_filter        (vpu_dbg_filter),
      .vpu_dbg_data          (vpu_dbg_data)
    );
  end else begin : gen_null_vpu
    null_vpu u_vpu (
      .clk_i                    (clk_i),
      .rst_ni                   (rst_c_sync_ni),
      .id_core_req_i            (id_core_vpu_req),
      .id_vpu_decoder_sigs_i    (id_vpu_decoder_sigs),
      .ex_core_req_i            (ex_core_vpu_req),
      .f2_core_kill_i           (tag_core_vpu_kill),
      .f3_core_kill_i           (mem_core_vpu_kill),
      .f4_core_kill_i           (wb_core_vpu_kill),
      .f0_core_ctrl_i           (f0_core_vpu_ctrl),
      .chicken_bit_vpulane_i    (vpulane_clock_gate_disable),
      .chicken_bit_vputima_i    (vputima_clock_gate_disable),
      .chicken_bit_vputrans_i   (vputrans_clock_gate_disable),
      .mem_ctrl_override_i      (mem_ctrl_override),
      .wb_dcache_resp_valid_i   (wb_dcache_vpu_resp_valid),
      .wb_dcache_resp_i         (wb_dcache_vpu_resp),
      .dcache_scp_resp_i        (dcache_vpu_scp_res),
      .dcache_scp_data_i        (dcache_vpu_scp_data),
      .dcache_tenb_data_i       (dcache_vpu_tenb_data),
      .dcache_reduce_ctrl_i     (vpu_reduce_ctrl),
      .id_core_ctrl_o           (id_vpu_core_ctrl),
      .ex_core_ctrl_o           (ex_vpu_core_ctrl),
      .f2_core_ctrl_o           (f2_vpu_core_ctrl),
      .f3_core_ctrl_o           (f3_vpu_core_ctrl),
      .wb_core_ctrl_o           (wb_vpu_core_ctrl),
      .io_events_o              (io_events_vpu),
      .vpu_dbg_match_o          (vpu_dbg_match),
      .vpu_dbg_filter_o         (vpu_dbg_filter),
      .vpu_dbg_data_o           (vpu_dbg_data),
      .dcache_ctrl_o            (vpu_dcache_ctl)
    );
  end
  /* verilator lint_on PINCONNECTEMPTY */

endmodule
