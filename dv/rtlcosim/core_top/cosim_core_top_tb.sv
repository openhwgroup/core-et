// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original core_top vs new core_top.

`include "soc.vh"

/* verilator lint_off SYNCASYNCNET */  // This wrapper intentionally fans the same async reset stimulus into both the translated active-low ports and the legacy active-high ports.
module cosim_core_top_tb
  import minion_pkg::*;
  import minion_frontend_pkg::*;
  import intpipe_csr_pkg::*;
  import minion_dcache_pkg::*;
  import dft_pkg::*;
(
  input  logic                                               clk_i,
  input  logic                                               rst_c_ni,
  input  logic                                               rst_w_ni,
  input  logic                                               rst_d_ni,
  input  dft_t                                               stim_dft,
  input  logic                                               stim_ioshire,
  input  logic                                               stim_chicken_bit_dcache,
  input  logic                                               stim_chicken_bit_intpipe,
  input  logic                                               stim_chicken_bit_frontend,
  input  logic [DcacheL2EvictReqPorts-1:0]                   stim_l2_dcache_evict_req_ready,
  input  logic [DcacheL2MissReqPorts-1:0]                    stim_l2_dcache_miss_req_ready,
  input  logic                                               stim_l2_dcache_resp_valid,
  input  et_link_minion_rsp_info_t                           stim_l2_dcache_resp,
  input  logic                                               stim_icache_req_ready,
  input  logic                                               stim_icache_resp_valid,
  input  logic                                               stim_icache_resp_miss,
  input  logic [$bits(icache_fe_resp_t)-1:0]                 stim_icache_resp,
  input  logic                                               stim_icache_fill_done,
  input  logic                                               stim_dc_ptw_req_ready,
  input  minion_ptw_pte                                      stim_ptw_dc_resp_data,
  input  logic                                               stim_ptw_dc_resp_valid,
  input  minion_interrupts                                   stim_interrupts,
  input  logic [NumShireIdsBits-1:0]                         stim_shire_id,
  input  logic [MinPerSBits-1:0]                             stim_shire_min_id,
  input  logic [NrThreads-1:0]                               stim_enabled,
  input  logic [VaSize-1:0]                                  stim_reset_vector,
  input  esr_mprot_t                                         stim_mprot,
  input  tlb_entry_type                                      stim_vmspagesize,
  input  vpu_dcache_ctrl                                     stim_vpu_dcache_ctl,
  input  logic [$bits(vpu_id_ctrl_t)-1:0]                    stim_id_vpu_ctrl,
  input  vpu_minion_ex_ctrl                                  stim_ex_vpu_ctrl,
  input  vpu_minion_tag_ctrl                                 stim_f2_vpu_ctrl,
  input  vpu_minion_mem_ctrl                                 stim_f3_vpu_ctrl,
  input  vpu_minion_wb_ctrl                                  stim_wb_vpu_ctrl,
  input  logic                                               stim_flb_neigh_resp_valid,
  input  logic                                               stim_flb_neigh_resp_data,
  input  logic                                               stim_te_thread_sel,
  input  logic                                               stim_te_enable,
  input  logic [MinDmApbAddrWidth-1:0]                       stim_apb_paddr,
  input  logic                                               stim_apb_pwrite,
  input  logic                                               stim_apb_psel,
  input  logic                                               stim_apb_penable,
  input  logic [BpamShireApbDataWidth-1:0]                   stim_apb_pwdata,
  input  minion_debug_in_t                                   stim_debug_in,
  input  esr_minion_features_t                               stim_esr_features,
  input  logic                                               stim_esr_bypass_dcache,
  input  logic                                               stim_esr_shire_coop_mode,
  input  logic [MinionMemCtrlBits-1:0]                       stim_mem_ctrl_override,
  input  logic [NrThreads-1:0][XregSize-1:0]                 stim_pmu_read_data,
  input  logic [CsrNrEventsVpu-1:0]                          stim_io_events_vpu,

  output logic [DcacheL2EvictReqPorts-1:0]                   orig_l2_dcache_evict_req_valid,
  output et_link_minion_evict_req_info_t                     orig_l2_dcache_evict_req,
  output logic [DcacheL2EvictReqPorts-1:0]                   new_l2_dcache_evict_req_valid,
  output et_link_minion_evict_req_info_t                     new_l2_dcache_evict_req,
  output logic [DcacheL2MissReqPorts-1:0]                    orig_l2_dcache_miss_req_valid,
  output et_link_minion_miss_req_info_t                      orig_l2_dcache_miss_req,
  output logic [DcacheL2MissReqPorts-1:0]                    new_l2_dcache_miss_req_valid,
  output et_link_minion_miss_req_info_t                      new_l2_dcache_miss_req,
  output logic                                               orig_l2_dcache_resp_ready,
  output logic                                               new_l2_dcache_resp_ready,
  output logic                                               orig_icache_req_valid,
  output logic [$bits(fe_icache_req_t)-1:0]                  orig_icache_req,
  output logic                                               new_icache_req_valid,
  output logic [$bits(fe_icache_req_t)-1:0]                  new_icache_req,
  output logic                                               orig_icache_flush_data,
  output logic                                               new_icache_flush_data,
  output minion_satp_info                                    orig_satp_info,
  output minion_satp_info                                    new_satp_info,
  output minion_satp_info                                    orig_matp_info,
  output minion_satp_info                                    new_matp_info,
  output logic                                               orig_tlb_invalidate,
  output logic                                               new_tlb_invalidate,
  output minion_ptw_req                                      orig_dc_ptw_req_data,
  output minion_ptw_req                                      new_dc_ptw_req_data,
  output logic                                               orig_dc_ptw_req_valid,
  output logic                                               new_dc_ptw_req_valid,
  output minion_vpu_id_req                                   orig_id_vpu_req,
  output minion_vpu_id_req                                   new_id_vpu_req,
  output vpu_ctrl_sigs_t                                     orig_id_vpu_decoder_sigs,
  output vpu_ctrl_sigs_t                                     new_id_vpu_decoder_sigs,
  output minion_vpu_ex_req                                   orig_ex_vpu_req,
  output minion_vpu_ex_req                                   new_ex_vpu_req,
  output logic                                               orig_tag_vpu_kill,
  output logic                                               new_tag_vpu_kill,
  output logic                                               orig_mem_vpu_kill,
  output logic                                               new_mem_vpu_kill,
  output logic                                               orig_wb_vpu_kill,
  output logic                                               new_wb_vpu_kill,
  output core_vpu_ctrl                                       orig_f0_vpu_ctrl,
  output core_vpu_ctrl                                       new_f0_vpu_ctrl,
  output logic                                               orig_wb_dcache_vpu_resp_valid,
  output logic                                               new_wb_dcache_vpu_resp_valid,
  output dcache_vpu_resp                                     orig_wb_dcache_vpu_resp,
  output dcache_vpu_resp                                     new_wb_dcache_vpu_resp,
  output dcache_vpu_scp_resp                                 orig_dcache_vpu_scp_res,
  output dcache_vpu_scp_resp                                 new_dcache_vpu_scp_res,
  output logic [VpuDataSz-1:0]                               orig_dcache_vpu_scp_data,
  output logic [VpuDataSz-1:0]                               new_dcache_vpu_scp_data,
  output logic [VpuDataSz-1:0]                               orig_dcache_vpu_tenb_data,
  output logic [VpuDataSz-1:0]                               new_dcache_vpu_tenb_data,
  output dcache_vpu_reduce_ctrl                              orig_vpu_reduce_ctrl,
  output dcache_vpu_reduce_ctrl                              new_vpu_reduce_ctrl,
  output logic                                               orig_flb_neigh_req_valid,
  output logic                                               new_flb_neigh_req_valid,
  output logic [CsrFlBarrierSize-1:0]                        orig_flb_neigh_req_data,
  output logic [CsrFlBarrierSize-1:0]                        new_flb_neigh_req_data,
  output trace_encoder_signals_t                             orig_trace_encoder,
  output trace_encoder_signals_t                             new_trace_encoder,
  output logic                                               orig_apb_pready,
  output logic                                               new_apb_pready,
  output logic [BpamShireApbDataWidth-1:0]                   orig_apb_prdata,
  output logic [BpamShireApbDataWidth-1:0]                   new_apb_prdata,
  output logic                                               orig_apb_pslverr,
  output logic                                               new_apb_pslverr,
  output minion_debug_out_t                                  orig_debug_out,
  output minion_debug_out_t                                  new_debug_out,
  output logic [PmuMinionCountersNum-1:0]                    orig_pmu_count_up,
  output logic [PmuMinionCountersNum-1:0]                    new_pmu_count_up,
  output logic [NrThreads-1:0][PmuCountersSelectBits-1:0]    orig_pmu_read_sel,
  output logic [NrThreads-1:0][PmuCountersSelectBits-1:0]    new_pmu_read_sel,
  output logic [PmuTotalCountersNum-1:0]                     orig_pmu_write_en,
  output logic [PmuTotalCountersNum-1:0]                     new_pmu_write_en,
  output logic [XregSize-1:0]                                orig_pmu_write_data,
  output logic [XregSize-1:0]                                new_pmu_write_data,
  output logic [PmuNeighEventCntSelBits-1:0]                 orig_pmu_neigh_event_sel,
  output logic [PmuNeighEventCntSelBits-1:0]                 new_pmu_neigh_event_sel,
  output logic [DcacheDbgSmMatchBits-1:0]                    orig_sm_match_debug_signals_tl0,
  output logic [DcacheDbgSmMatchBits-1:0]                    new_sm_match_debug_signals_tl0,
  output logic [DcacheDbgSmFilterBits-1:0]                   orig_sm_filter_debug_signals_tl0,
  output logic [DcacheDbgSmFilterBits-1:0]                   new_sm_filter_debug_signals_tl0,
  output logic [DcacheDbgSmDataBits-1:0]                     orig_sm_data_debug_signals_tl0,
  output logic [DcacheDbgSmDataBits-1:0]                     new_sm_data_debug_signals_tl0,
  output logic [DcacheDbgSmMatchBits-1:0]                    orig_sm_match_debug_signals_tl1,
  output logic [DcacheDbgSmMatchBits-1:0]                    new_sm_match_debug_signals_tl1,
  output logic [DcacheDbgSmFilterBits-1:0]                   orig_sm_filter_debug_signals_tl1,
  output logic [DcacheDbgSmFilterBits-1:0]                   new_sm_filter_debug_signals_tl1,
  output logic [DcacheDbgSmDataBits-1:0]                     orig_sm_data_debug_signals_tl1,
  output logic [DcacheDbgSmDataBits-1:0]                     new_sm_data_debug_signals_tl1,
  output logic [DcacheDbgSmMatchBits-1:0]                    orig_sm_match_debug_signals_ts,
  output logic [DcacheDbgSmMatchBits-1:0]                    new_sm_match_debug_signals_ts,
  output logic [DcacheDbgSmFilterBits-1:0]                   orig_sm_filter_debug_signals_ts,
  output logic [DcacheDbgSmFilterBits-1:0]                   new_sm_filter_debug_signals_ts,
  output logic [DcacheDbgSmDataBits-1:0]                     orig_sm_data_debug_signals_ts,
  output logic [DcacheDbgSmDataBits-1:0]                     new_sm_data_debug_signals_ts,
  output logic [DcacheDbgSmMatchBits-1:0]                    orig_sm_match_debug_signals_co,
  output logic [DcacheDbgSmMatchBits-1:0]                    new_sm_match_debug_signals_co,
  output logic [DcacheDbgSmFilterBits-1:0]                   orig_sm_filter_debug_signals_co,
  output logic [DcacheDbgSmFilterBits-1:0]                   new_sm_filter_debug_signals_co,
  output logic [DcacheDbgSmDataBits-1:0]                     orig_sm_data_debug_signals_co,
  output logic [DcacheDbgSmDataBits-1:0]                     new_sm_data_debug_signals_co,
  output logic [DcacheDbgSmMatchBits-1:0]                    orig_sm_match_debug_signals_col2,
  output logic [DcacheDbgSmMatchBits-1:0]                    new_sm_match_debug_signals_col2,
  output logic [DcacheDbgSmFilterBits-1:0]                   orig_sm_filter_debug_signals_col2,
  output logic [DcacheDbgSmFilterBits-1:0]                   new_sm_filter_debug_signals_col2,
  output logic [DcacheDbgSmDataBits-1:0]                     orig_sm_data_debug_signals_col2,
  output logic [DcacheDbgSmDataBits-1:0]                     new_sm_data_debug_signals_col2,
  output logic [DcacheMhFileSize-1:0][DcacheDbgSmMatchBits-1:0]  orig_sm_match_debug_signals_mh,
  output logic [DcacheMhFileSize-1:0][DcacheDbgSmMatchBits-1:0]  new_sm_match_debug_signals_mh,
  output logic [DcacheMhFileSize-1:0][DcacheDbgSmFilterBits-1:0] orig_sm_filter_debug_signals_mh,
  output logic [DcacheMhFileSize-1:0][DcacheDbgSmFilterBits-1:0] new_sm_filter_debug_signals_mh,
  output logic [DcacheMhFileSize-1:0][DcacheDbgSmDataBits-1:0]   orig_sm_data_debug_signals_mh,
  output logic [DcacheMhFileSize-1:0][DcacheDbgSmDataBits-1:0]   new_sm_data_debug_signals_mh,
  output logic [DcacheDbgSmMatchBits-1:0]                    orig_sm_match_debug_signals_dc,
  output logic [DcacheDbgSmMatchBits-1:0]                    new_sm_match_debug_signals_dc,
  output logic [DcacheDbgSmFilterBits-1:0]                   orig_sm_filter_debug_signals_dc,
  output logic [DcacheDbgSmFilterBits-1:0]                   new_sm_filter_debug_signals_dc,
  output logic [DcacheDbgSmDataBits-1:0]                     orig_sm_data_debug_signals_dc,
  output logic [DcacheDbgSmDataBits-1:0]                     new_sm_data_debug_signals_dc,
  output intpipe_dbg_monitor_t                               orig_intpipe_debug_monitor,
  output intpipe_dbg_monitor_t                               new_intpipe_debug_monitor,
  output logic [NrThreads-1:0]                               orig_fe_req_valid_int,
  output logic [NrThreads-1:0]                               new_fe_req_valid_int,
  output logic [NrThreads*$bits(minion_fe_req)-1:0]          orig_fe_req_int,
  output logic [NrThreads*$bits(minion_fe_req)-1:0]          new_fe_req_int,
  output logic [NrThreads-1:0]                               orig_fe_stall_int,
  output logic [NrThreads-1:0]                               new_fe_stall_int,
  output logic                                               orig_fe_resp_ready_int,
  output logic                                               new_fe_resp_ready_int,
  output logic                                               orig_fe_resp_valid_int,
  output logic                                               new_fe_resp_valid_int,
  output logic                                               orig_fe_resp_thread_id_int,
  output logic                                               new_fe_resp_thread_id_int,
  output logic [$bits(frontend_core_resp)-1:0]               orig_fe_resp_int,
  output logic [$bits(frontend_core_resp)-1:0]               new_fe_resp_int,
  output logic [NrThreads-1:0]                               orig_fe_thread_enabled_f_int,
  output logic [NrThreads-1:0]                               new_fe_thread_enabled_f_int,
  output logic [NrThreads-1:0]                               orig_fe_thread_enabled_ff_int,
  output logic [NrThreads-1:0]                               new_fe_thread_enabled_ff_int,
  output logic [NrThreads-1:0]                               orig_fe_tb_req_valid_int,
  output logic [NrThreads-1:0]                               new_fe_tb_req_valid_int,
  output logic [NrThreads*$bits(fe_icache_req_t)-1:0]        orig_fe_tb_req_int,
  output logic [NrThreads*$bits(fe_icache_req_t)-1:0]        new_fe_tb_req_int,
  output logic [NrThreads-1:0]                               orig_fe_tb_req_ready_int,
  output logic [NrThreads-1:0]                               new_fe_tb_req_ready_int,
  output logic                                               orig_fe_req_ready_arb_int,
  output logic                                               new_fe_req_ready_arb_int,
  output logic [NrThreads-1:0]                               orig_fe_tb_inst_valid_int,
  output logic [NrThreads-1:0]                               new_fe_tb_inst_valid_int,
  output logic [NrThreads*32-1:0]                            orig_fe_tb_inst_bits_int,
  output logic [NrThreads*32-1:0]                            new_fe_tb_inst_bits_int,
  output logic [NrThreads*$bits(fe_operation_t)-1:0]         orig_fe_tb_data_int,
  output logic [NrThreads*$bits(fe_operation_t)-1:0]         new_fe_tb_data_int,
  output logic [NrThreads*$bits(fe_thread_data_t)-1:0]       orig_fe_fifo_head_data_int,
  output logic [NrThreads*$bits(fe_thread_data_t)-1:0]       new_fe_fifo_head_data_int,
  output logic [NrThreads*$bits(fe_thread_data_t)-1:0]       orig_fe_fifo_pending_data_int,
  output logic [NrThreads*$bits(fe_thread_data_t)-1:0]       new_fe_fifo_pending_data_int,
  output logic [NrThreads*2-1:0]                             orig_fe_fifo_rd_ptr_int,
  output logic [NrThreads*2-1:0]                             new_fe_fifo_rd_ptr_int,
  output logic [NrThreads*2-1:0]                             orig_fe_fifo_wr_ptr_int,
  output logic [NrThreads*2-1:0]                             new_fe_fifo_wr_ptr_int,
  output logic [NrThreads-1:0]                               orig_fe_inst_fifo_empty_int,
  output logic [NrThreads-1:0]                               new_fe_inst_fifo_empty_int,
  output logic [NrThreads-1:0]                               orig_fe_inst_fifo_full_int,
  output logic [NrThreads-1:0]                               new_fe_inst_fifo_full_int,
  output logic                                               orig_fe_exp_thread_id_int,
  output logic                                               new_fe_exp_thread_id_int,
  output logic [NrThreads*$bits(vm_status_t)-1:0]            orig_vm_status_int,
  output logic [NrThreads*$bits(vm_status_t)-1:0]            new_vm_status_int,
  output logic [$bits(minion_control_t)-1:0]                 orig_id_intpipe_ctrl_int,
  output logic [$bits(minion_control_t)-1:0]                 new_id_intpipe_ctrl_int,
  output logic [$bits(vpu_id_ctrl_t)-1:0]                    orig_id_frontend_vpu_ctrl_int,
  output logic [$bits(vpu_id_ctrl_t)-1:0]                    new_id_frontend_vpu_ctrl_int,
  output logic [63:0]                                        orig_debug_ffb_wdata_int,
  output logic [63:0]                                        new_debug_ffb_wdata_int,
  output logic [MinionDebugFfbWords-1:0]                     orig_debug_ffb_en_int,
  output logic [MinionDebugFfbWords-1:0]                     new_debug_ffb_en_int,
  output logic                                               orig_debug_ffb_thread_sel_int,
  output logic                                               new_debug_ffb_thread_sel_int,
  output logic [NrThreads-1:0]                               orig_debug_ex_program_buffer_int,
  output logic [NrThreads-1:0]                               new_debug_ex_program_buffer_int,
  output logic [NrThreads-1:0]                               orig_debug_halt_int,
  output logic [NrThreads-1:0]                               new_debug_halt_int,
  output logic [NrThreads-1:0]                               orig_pending_halt_int,
  output logic [NrThreads-1:0]                               new_pending_halt_int,
  output core_dcache_ctrl                                    orig_dcache_core_ctrl_int,
  output core_dcache_ctrl                                    new_dcache_core_ctrl_int,
  output dcache_core_ctrl                                    orig_dcache_core_ctrl_resp_int,
  output dcache_core_ctrl                                    new_dcache_core_ctrl_resp_int,
  output minion_dcache_req                                   orig_dcache_req_int,
  output minion_dcache_req                                   new_dcache_req_int,
  output logic                                               orig_dcache_req_valid_int,
  output logic                                               new_dcache_req_valid_int,
  output logic                                               orig_dcache_req_gsc_int,
  output logic                                               new_dcache_req_gsc_int,
  output dcache_minion_resp                                  orig_dcache_resp_int,
  output dcache_minion_resp                                  new_dcache_resp_int,
  output logic                                               orig_dcache_resp_valid_int,
  output logic                                               new_dcache_resp_valid_int,
  output dcache_minion_scoreboard                            orig_dcache_scoreboard_int,
  output dcache_minion_scoreboard                            new_dcache_scoreboard_int,
  output dcache_debug_sigs                                   orig_dcache_debug_sigs_int,
  output dcache_debug_sigs                                   new_dcache_debug_sigs_int,
  output logic [$bits(l2_cache_op_state_e)-1:0]              orig_dcache_col2_state_int,
  output logic [$bits(l2_cache_op_state_e)-1:0]              new_dcache_col2_state_int,
  output logic [$bits(l2_cache_op_req_t)-1:0]                orig_dcache_col2_req_int,
  output logic [$bits(l2_cache_op_req_t)-1:0]                new_dcache_col2_req_int,
  output logic [VaSize-1:0]                                  orig_dcache_col2_req_addr_int,
  output logic [VaSize-1:0]                                  new_dcache_col2_req_addr_int,
  output logic [3:0]                                         orig_dcache_col2_req_count_int,
  output logic [3:0]                                         new_dcache_col2_req_count_int,
  output logic                                               orig_dcache_col2_id_running_int,
  output logic                                               new_dcache_col2_id_running_int,
  output logic [DcacheL2ScpDstBits-1:0]                      orig_dcache_col2_l2_scp_dest_int,
  output logic [DcacheL2ScpDstBits-1:0]                      new_dcache_col2_l2_scp_dest_int,
  output logic                                               orig_dcache_col2_err_mem_int,
  output logic                                               new_dcache_col2_err_mem_int,
  output logic                                               orig_dcache_col2_cache_op_req_is_l2_int,
  output logic                                               new_dcache_col2_cache_op_req_is_l2_int,
  output logic                                               orig_dcache_col2_cache_op_req_is_nop_int,
  output logic                                               new_dcache_col2_cache_op_req_is_nop_int,
  output logic [DcacheCoL2MaxOpsBits-1:0]                    orig_dcache_col2_l2_cache_op_cnt_int,
  output logic [DcacheCoL2MaxOpsBits-1:0]                    new_dcache_col2_l2_cache_op_cnt_int,
  output logic [DcacheCoL2CycleBits-1:0]                     orig_dcache_col2_reprate_count_int,
  output logic [DcacheCoL2CycleBits-1:0]                     new_dcache_col2_reprate_count_int,
  output logic [DcacheCoL2GrantBits-1:0]                     orig_dcache_col2_reprate_grant_int,
  output logic [DcacheCoL2GrantBits-1:0]                     new_dcache_col2_reprate_grant_int,
  output logic                                               orig_dcache_col2_l2_req_valid_started_int,
  output logic                                               new_dcache_col2_l2_req_valid_started_int,
  output logic [NrThreads*2*DcacheCoL2MaxOpsBits-1:0]        orig_dcache_col2_ready_counters_int,
  output logic [NrThreads*2*DcacheCoL2MaxOpsBits-1:0]        new_dcache_col2_ready_counters_int,
  output logic                                               orig_dcache_col2_l2_resp_valid_int,
  output logic                                               new_dcache_col2_l2_resp_valid_int,
  output logic [EtLinkIdSize-1:0]                            orig_dcache_col2_l2_resp_id_int,
  output logic [EtLinkIdSize-1:0]                            new_dcache_col2_l2_resp_id_int,
  output logic                                               orig_dcache_col2_l2_resp_dest_int,
  output logic                                               new_dcache_col2_l2_resp_dest_int,
  output logic                                               orig_dcache_col2_l2_resp_wdata_int,
  output logic                                               new_dcache_col2_l2_resp_wdata_int,
  output logic [1:0]                                         orig_dcache_col2_l2_resp_opcode_int,
  output logic [1:0]                                         new_dcache_col2_l2_resp_opcode_int,
  output logic [CoreL2BlockExtSize-1:0]                      orig_dcache_col2_l2_resp_data_int,
  output logic [CoreL2BlockExtSize-1:0]                      new_dcache_col2_l2_resp_data_int,
  output logic [2:0]                                         orig_dcache_col2_l2_resp_size_int,
  output logic [2:0]                                         new_dcache_col2_l2_resp_size_int,
  output logic [3:0]                                         orig_dcache_col2_l2_resp_qwen_int,
  output logic [3:0]                                         new_dcache_col2_l2_resp_qwen_int,
  output logic [15:0]                                        orig_dcache_col2_l2_resp_data_hi_int,
  output logic [15:0]                                        new_dcache_col2_l2_resp_data_hi_int,
  output logic [15:0]                                        orig_dcache_l2_resp_in_data_hi_int,
  output logic [15:0]                                        new_dcache_l2_resp_in_data_hi_int,
  output logic                                               orig_dcache_col2_l2_resp_is_prefetch_int,
  output logic                                               new_dcache_col2_l2_resp_is_prefetch_int,
  output logic                                               orig_dcache_col2_pf_l2_evict_ready_int,
  output logic                                               new_dcache_col2_pf_l2_evict_ready_int,
  output logic                                               orig_dcache_s3_lrsc_valid_int,
  output logic                                               new_dcache_s3_lrsc_valid_int
);

  logic                reset_c_orig;
  logic                reset_w_orig;
  logic                reset_d_orig;
  icache_fe_resp_t     stim_icache_resp_new;
  icache_frontend_resp stim_icache_resp_orig;
  vpu_id_ctrl_t        stim_id_vpu_ctrl_new;
  vpu_minion_id_ctrl   stim_id_vpu_ctrl_orig;
  frontend_icache_req  orig_icache_req_s;
  fe_icache_req_t      new_icache_req_s;

  assign reset_c_orig = ~rst_c_ni;
  assign reset_w_orig = ~rst_w_ni;
  assign reset_d_orig = ~rst_d_ni;
  assign stim_icache_resp_new = stim_icache_resp;
  assign stim_icache_resp_orig = icache_frontend_resp'(stim_icache_resp);
  assign stim_id_vpu_ctrl_new = stim_id_vpu_ctrl;
  assign stim_id_vpu_ctrl_orig = vpu_minion_id_ctrl'(stim_id_vpu_ctrl);
  assign orig_icache_req = orig_icache_req_s;
  assign new_icache_req = new_icache_req_s;

  core_top_orig u_orig (
    .clock                    (clk_i),
    .reset_c                  (reset_c_orig),
    .reset_w                  (reset_w_orig),
    .reset_d                  (reset_d_orig),
    .dft__reset_byp           (stim_dft.scanmode),
    .dft__reset               (!stim_dft.scan_reset_n),
    .ioshire                  (stim_ioshire),
    .chicken_bit_dcache       (stim_chicken_bit_dcache),
    .chicken_bit_intpipe      (stim_chicken_bit_intpipe),
    .chicken_bit_frontend     (stim_chicken_bit_frontend),
    .l2_dcache_evict_req_ready(stim_l2_dcache_evict_req_ready),
    .l2_dcache_evict_req_valid(orig_l2_dcache_evict_req_valid),
    .l2_dcache_evict_req      (orig_l2_dcache_evict_req),
    .l2_dcache_miss_req_ready (stim_l2_dcache_miss_req_ready),
    .l2_dcache_miss_req_valid (orig_l2_dcache_miss_req_valid),
    .l2_dcache_miss_req       (orig_l2_dcache_miss_req),
    .l2_dcache_resp_ready     (orig_l2_dcache_resp_ready),
    .l2_dcache_resp_valid     (stim_l2_dcache_resp_valid),
    .l2_dcache_resp           (stim_l2_dcache_resp),
    .icache_req_ready         (stim_icache_req_ready),
    .icache_req_valid         (orig_icache_req_valid),
    .icache_req               (orig_icache_req_s),
    .icache_resp_valid        (stim_icache_resp_valid),
    .icache_resp_miss         (stim_icache_resp_miss),
    .icache_resp              (stim_icache_resp_orig),
    .icache_fill_done         (stim_icache_fill_done),
    .icache_flush_data        (orig_icache_flush_data),
    .satp_info                (orig_satp_info),
    .matp_info                (orig_matp_info),
    .tlb_invalidate           (orig_tlb_invalidate),
    .dc_ptw_req_data          (orig_dc_ptw_req_data),
    .dc_ptw_req_valid         (orig_dc_ptw_req_valid),
    .dc_ptw_req_ready         (stim_dc_ptw_req_ready),
    .ptw_dc_resp_data         (stim_ptw_dc_resp_data),
    .ptw_dc_resp_valid        (stim_ptw_dc_resp_valid),
    .interrupts               (stim_interrupts),
    .shire_id                 (stim_shire_id),
    .shire_min_id             (stim_shire_min_id),
    .enabled                  (stim_enabled),
    .reset_vector             (stim_reset_vector),
    .mprot                    (stim_mprot),
    .vmspagesize              (stim_vmspagesize),
    .id_vpu_req               (orig_id_vpu_req),
    .id_vpu_decoder_sigs      (orig_id_vpu_decoder_sigs),
    .ex_vpu_req               (orig_ex_vpu_req),
    .tag_vpu_kill             (orig_tag_vpu_kill),
    .mem_vpu_kill             (orig_mem_vpu_kill),
    .wb_vpu_kill              (orig_wb_vpu_kill),
    .f0_vpu_ctrl              (orig_f0_vpu_ctrl),
    .wb_dcache_vpu_resp_valid (orig_wb_dcache_vpu_resp_valid),
    .wb_dcache_vpu_resp       (orig_wb_dcache_vpu_resp),
    .vpu_dcache_ctl           (stim_vpu_dcache_ctl),
    .dcache_vpu_scp_res       (orig_dcache_vpu_scp_res),
    .dcache_vpu_scp_data      (orig_dcache_vpu_scp_data),
    .dcache_vpu_tenb_data     (orig_dcache_vpu_tenb_data),
    .vpu_reduce_ctrl          (orig_vpu_reduce_ctrl),
    .id_vpu_ctrl              (stim_id_vpu_ctrl_orig),
    .ex_vpu_ctrl              (stim_ex_vpu_ctrl),
    .f2_vpu_ctrl              (stim_f2_vpu_ctrl),
    .f3_vpu_ctrl              (stim_f3_vpu_ctrl),
    .wb_vpu_ctrl              (stim_wb_vpu_ctrl),
    .flb_neigh_req_valid      (orig_flb_neigh_req_valid),
    .flb_neigh_req_data       (orig_flb_neigh_req_data),
    .flb_neigh_resp_valid     (stim_flb_neigh_resp_valid),
    .flb_neigh_resp_data      (stim_flb_neigh_resp_data),
    .te_thread_sel            (stim_te_thread_sel),
    .traceEncoder             (orig_trace_encoder),
    .te_enable                (stim_te_enable),
    .apb_paddr                (stim_apb_paddr),
    .apb_pwrite               (stim_apb_pwrite),
    .apb_psel                 (stim_apb_psel),
    .apb_penable              (stim_apb_penable),
    .apb_pwdata               (stim_apb_pwdata),
    .apb_pready               (orig_apb_pready),
    .apb_prdata               (orig_apb_prdata),
    .apb_pslverr              (orig_apb_pslverr),
    .debug_in                 (stim_debug_in),
    .debug_out                (orig_debug_out),
    .esr_features             (stim_esr_features),
    .esr_bypass_dcache        (stim_esr_bypass_dcache),
    .esr_shire_coop_mode      (stim_esr_shire_coop_mode),
    .mem_ctrl_override        (stim_mem_ctrl_override),
    .pmu_count_up             (orig_pmu_count_up),
    .pmu_read_data            (stim_pmu_read_data),
    .pmu_read_sel             (orig_pmu_read_sel),
    .pmu_write_en             (orig_pmu_write_en),
    .pmu_write_data           (orig_pmu_write_data),
    .pmu_neigh_event_sel      (orig_pmu_neigh_event_sel),
    .io_events_vpu            (stim_io_events_vpu),
    .sm_match_debug_signals_tl0(orig_sm_match_debug_signals_tl0),
    .sm_filter_debug_signals_tl0(orig_sm_filter_debug_signals_tl0),
    .sm_data_debug_signals_tl0(orig_sm_data_debug_signals_tl0),
    .sm_match_debug_signals_tl1(orig_sm_match_debug_signals_tl1),
    .sm_filter_debug_signals_tl1(orig_sm_filter_debug_signals_tl1),
    .sm_data_debug_signals_tl1(orig_sm_data_debug_signals_tl1),
    .sm_match_debug_signals_ts(orig_sm_match_debug_signals_ts),
    .sm_filter_debug_signals_ts(orig_sm_filter_debug_signals_ts),
    .sm_data_debug_signals_ts (orig_sm_data_debug_signals_ts),
    .sm_match_debug_signals_co(orig_sm_match_debug_signals_co),
    .sm_filter_debug_signals_co(orig_sm_filter_debug_signals_co),
    .sm_data_debug_signals_co (orig_sm_data_debug_signals_co),
    .sm_match_debug_signals_col2(orig_sm_match_debug_signals_col2),
    .sm_filter_debug_signals_col2(orig_sm_filter_debug_signals_col2),
    .sm_data_debug_signals_col2(orig_sm_data_debug_signals_col2),
    .sm_match_debug_signals_mh(orig_sm_match_debug_signals_mh),
    .sm_filter_debug_signals_mh(orig_sm_filter_debug_signals_mh),
    .sm_data_debug_signals_mh (orig_sm_data_debug_signals_mh),
    .sm_match_debug_signals_dc(orig_sm_match_debug_signals_dc),
    .sm_filter_debug_signals_dc(orig_sm_filter_debug_signals_dc),
    .sm_data_debug_signals_dc (orig_sm_data_debug_signals_dc),
    .intpipe_debug_monitor_out(orig_intpipe_debug_monitor)
  );

  core_top u_new (
    .clk_i                      (clk_i),
    .rst_c_ni                   (rst_c_ni),
    .rst_w_ni                   (rst_w_ni),
    .rst_d_ni                   (rst_d_ni),
    .dft_i                      (stim_dft),
    .ioshire_i                  (stim_ioshire),
    .chicken_bit_dcache_i       (stim_chicken_bit_dcache),
    .chicken_bit_intpipe_i      (stim_chicken_bit_intpipe),
    .chicken_bit_frontend_i     (stim_chicken_bit_frontend),
    .l2_dcache_evict_req_ready_i(stim_l2_dcache_evict_req_ready),
    .l2_dcache_evict_req_valid_o(new_l2_dcache_evict_req_valid),
    .l2_dcache_evict_req_o      (new_l2_dcache_evict_req),
    .l2_dcache_miss_req_ready_i (stim_l2_dcache_miss_req_ready),
    .l2_dcache_miss_req_valid_o (new_l2_dcache_miss_req_valid),
    .l2_dcache_miss_req_o       (new_l2_dcache_miss_req),
    .l2_dcache_resp_ready_o     (new_l2_dcache_resp_ready),
    .l2_dcache_resp_valid_i     (stim_l2_dcache_resp_valid),
    .l2_dcache_resp_i           (stim_l2_dcache_resp),
    .icache_req_ready_i         (stim_icache_req_ready),
    .icache_req_valid_o         (new_icache_req_valid),
    .icache_req_o               (new_icache_req_s),
    .icache_resp_valid_i        (stim_icache_resp_valid),
    .icache_resp_miss_i         (stim_icache_resp_miss),
    .icache_resp_i              (stim_icache_resp_new),
    .icache_fill_done_i         (stim_icache_fill_done),
    .icache_flush_data_o        (new_icache_flush_data),
    .satp_info_o                (new_satp_info),
    .matp_info_o                (new_matp_info),
    .tlb_invalidate_o           (new_tlb_invalidate),
    .dc_ptw_req_data_o          (new_dc_ptw_req_data),
    .dc_ptw_req_valid_o         (new_dc_ptw_req_valid),
    .dc_ptw_req_ready_i         (stim_dc_ptw_req_ready),
    .ptw_dc_resp_data_i         (stim_ptw_dc_resp_data),
    .ptw_dc_resp_valid_i        (stim_ptw_dc_resp_valid),
    .interrupts_i               (stim_interrupts),
    .shire_id_i                 (stim_shire_id),
    .shire_min_id_i             (stim_shire_min_id),
    .enabled_i                  (stim_enabled),
    .reset_vector_i             (stim_reset_vector),
    .mprot_i                    (stim_mprot),
    .vmspagesize_i              (stim_vmspagesize),
    .id_vpu_req_o               (new_id_vpu_req),
    .id_vpu_decoder_sigs_o      (new_id_vpu_decoder_sigs),
    .ex_vpu_req_o               (new_ex_vpu_req),
    .tag_vpu_kill_o             (new_tag_vpu_kill),
    .mem_vpu_kill_o             (new_mem_vpu_kill),
    .wb_vpu_kill_o              (new_wb_vpu_kill),
    .f0_vpu_ctrl_o              (new_f0_vpu_ctrl),
    .wb_dcache_vpu_resp_valid_o (new_wb_dcache_vpu_resp_valid),
    .wb_dcache_vpu_resp_o       (new_wb_dcache_vpu_resp),
    .vpu_dcache_ctl_i           (stim_vpu_dcache_ctl),
    .dcache_vpu_scp_res_o       (new_dcache_vpu_scp_res),
    .dcache_vpu_scp_data_o      (new_dcache_vpu_scp_data),
    .dcache_vpu_tenb_data_o     (new_dcache_vpu_tenb_data),
    .vpu_reduce_ctrl_o          (new_vpu_reduce_ctrl),
    .id_vpu_ctrl_i              (stim_id_vpu_ctrl_new),
    .ex_vpu_ctrl_i              (stim_ex_vpu_ctrl),
    .f2_vpu_ctrl_i              (stim_f2_vpu_ctrl),
    .f3_vpu_ctrl_i              (stim_f3_vpu_ctrl),
    .wb_vpu_ctrl_i              (stim_wb_vpu_ctrl),
    .flb_neigh_req_valid_o      (new_flb_neigh_req_valid),
    .flb_neigh_req_data_o       (new_flb_neigh_req_data),
    .flb_neigh_resp_valid_i     (stim_flb_neigh_resp_valid),
    .flb_neigh_resp_data_i      (stim_flb_neigh_resp_data),
    .te_thread_sel_i            (stim_te_thread_sel),
    .trace_encoder_o            (new_trace_encoder),
    .te_enable_i                (stim_te_enable),
    .apb_paddr_i                (stim_apb_paddr),
    .apb_pwrite_i               (stim_apb_pwrite),
    .apb_psel_i                 (stim_apb_psel),
    .apb_penable_i              (stim_apb_penable),
    .apb_pwdata_i               (stim_apb_pwdata),
    .apb_pready_o               (new_apb_pready),
    .apb_prdata_o               (new_apb_prdata),
    .apb_pslverr_o              (new_apb_pslverr),
    .debug_in_i                 (stim_debug_in),
    .debug_out_o                (new_debug_out),
    .esr_features_i             (stim_esr_features),
    .esr_bypass_dcache_i        (stim_esr_bypass_dcache),
    .esr_shire_coop_mode_i      (stim_esr_shire_coop_mode),
    .mem_ctrl_override_i        (stim_mem_ctrl_override),
    .pmu_count_up_o             (new_pmu_count_up),
    .pmu_read_data_i            (stim_pmu_read_data),
    .pmu_read_sel_o             (new_pmu_read_sel),
    .pmu_write_en_o             (new_pmu_write_en),
    .pmu_write_data_o           (new_pmu_write_data),
    .pmu_neigh_event_sel_o      (new_pmu_neigh_event_sel),
    .io_events_vpu_i            (stim_io_events_vpu),
    .sm_match_debug_signals_tl0_o(new_sm_match_debug_signals_tl0),
    .sm_filter_debug_signals_tl0_o(new_sm_filter_debug_signals_tl0),
    .sm_data_debug_signals_tl0_o(new_sm_data_debug_signals_tl0),
    .sm_match_debug_signals_tl1_o(new_sm_match_debug_signals_tl1),
    .sm_filter_debug_signals_tl1_o(new_sm_filter_debug_signals_tl1),
    .sm_data_debug_signals_tl1_o(new_sm_data_debug_signals_tl1),
    .sm_match_debug_signals_ts_o(new_sm_match_debug_signals_ts),
    .sm_filter_debug_signals_ts_o(new_sm_filter_debug_signals_ts),
    .sm_data_debug_signals_ts_o (new_sm_data_debug_signals_ts),
    .sm_match_debug_signals_co_o(new_sm_match_debug_signals_co),
    .sm_filter_debug_signals_co_o(new_sm_filter_debug_signals_co),
    .sm_data_debug_signals_co_o (new_sm_data_debug_signals_co),
    .sm_match_debug_signals_col2_o(new_sm_match_debug_signals_col2),
    .sm_filter_debug_signals_col2_o(new_sm_filter_debug_signals_col2),
    .sm_data_debug_signals_col2_o(new_sm_data_debug_signals_col2),
    .sm_match_debug_signals_mh_o(new_sm_match_debug_signals_mh),
    .sm_filter_debug_signals_mh_o(new_sm_filter_debug_signals_mh),
    .sm_data_debug_signals_mh_o (new_sm_data_debug_signals_mh),
    .sm_match_debug_signals_dc_o(new_sm_match_debug_signals_dc),
    .sm_filter_debug_signals_dc_o(new_sm_filter_debug_signals_dc),
    .sm_data_debug_signals_dc_o (new_sm_data_debug_signals_dc),
    .intpipe_debug_monitor_o    (new_intpipe_debug_monitor)
  );

  assign orig_fe_req_valid_int = u_orig.id_core_fe_req_valid;
  assign new_fe_req_valid_int = u_new.id_core_fe_req_valid;
  assign orig_fe_req_int = {u_orig.id_core_fe_req[1], u_orig.id_core_fe_req[0]};
  assign new_fe_req_int = {u_new.id_core_fe_req_intpipe[1], u_new.id_core_fe_req_intpipe[0]};
  assign orig_fe_stall_int = u_orig.id_core_fe_stall;
  assign new_fe_stall_int = u_new.id_core_fe_stall;
  assign orig_fe_resp_ready_int = u_orig.id_core_fe_resp_ready;
  assign new_fe_resp_ready_int = u_new.id_core_fe_resp_ready;
  assign orig_fe_resp_valid_int = u_orig.id_fe_core_resp_valid;
  assign new_fe_resp_valid_int = u_new.id_fe_core_resp_valid;
  assign orig_fe_resp_thread_id_int = u_orig.id_fe_core_resp_thread_id;
  assign new_fe_resp_thread_id_int = u_new.id_fe_core_resp_thread_id;
  assign orig_fe_resp_int = u_orig.id_fe_core_resp;
  assign new_fe_resp_int = u_new.id_fe_core_resp_intpipe;
  assign orig_fe_thread_enabled_f_int = u_orig.frontend.f0_thread_enabled_f;
  assign new_fe_thread_enabled_f_int = u_new.u_frontend.thread_enabled_f_q;
  assign orig_fe_thread_enabled_ff_int = u_orig.frontend.f1_thread_enabled;
  assign new_fe_thread_enabled_ff_int = u_new.u_frontend.thread_enabled_ff_q;
  assign orig_fe_tb_req_valid_int = u_orig.frontend.f0_icache_req_valid_arb;
  assign new_fe_tb_req_valid_int = u_new.u_frontend.tb_icache_req_valid;
  assign orig_fe_tb_req_int = {u_orig.frontend.f0_icache_req_arb[1], u_orig.frontend.f0_icache_req_arb[0]};
  assign new_fe_tb_req_int = {u_new.u_frontend.tb_icache_req[1], u_new.u_frontend.tb_icache_req[0]};
  assign orig_fe_tb_req_ready_int = u_orig.frontend.f0_icache_req_ready_arb;
  assign new_fe_tb_req_ready_int = u_new.u_frontend.tb_icache_req_ready;
  assign orig_fe_req_ready_arb_int = u_orig.frontend.f1_icache_req_ready_arb;
  assign new_fe_req_ready_arb_int = u_new.u_frontend.f1_icache_req_ready_arb;
  assign orig_fe_tb_inst_valid_int = u_orig.frontend.f7_tb_inst_valid;
  assign new_fe_tb_inst_valid_int = u_new.u_frontend.tb_inst_valid;
  assign orig_fe_tb_inst_bits_int = {u_orig.frontend.f7_tb_inst_data[1].inst_bits, u_orig.frontend.f7_tb_inst_data[0].inst_bits};
  assign new_fe_tb_inst_bits_int = {u_new.u_frontend.tb_inst_data[1].inst_bits, u_new.u_frontend.tb_inst_data[0].inst_bits};
  assign orig_fe_tb_data_int = {u_orig.frontend.f7_tb_inst_data[1], u_orig.frontend.f7_tb_inst_data[0]};
  assign new_fe_tb_data_int = {u_new.u_frontend.tb_inst_data[1], u_new.u_frontend.tb_inst_data[0]};
  assign orig_fe_fifo_head_data_int = {u_orig.frontend.inst_fifo_data[1], u_orig.frontend.inst_fifo_data[0]};
  assign new_fe_fifo_head_data_int = {u_new.u_frontend.inst_fifo_data[1], u_new.u_frontend.inst_fifo_data[0]};
  assign orig_fe_fifo_pending_data_int = {u_orig.frontend.INST_FIFOS[1].pending_data, u_orig.frontend.INST_FIFOS[0].pending_data};
  assign new_fe_fifo_pending_data_int = {u_new.u_frontend.gen_fifo[1].pending_data, u_new.u_frontend.gen_fifo[0].pending_data};
  assign orig_fe_fifo_rd_ptr_int = {u_orig.frontend.INST_FIFOS[1].rd_ptr, u_orig.frontend.INST_FIFOS[0].rd_ptr};
  assign new_fe_fifo_rd_ptr_int = {u_new.u_frontend.fifo_rd_ptr[1], u_new.u_frontend.fifo_rd_ptr[0]};
  assign orig_fe_fifo_wr_ptr_int = {u_orig.frontend.INST_FIFOS[1].wr_ptr, u_orig.frontend.INST_FIFOS[0].wr_ptr};
  assign new_fe_fifo_wr_ptr_int = {u_new.u_frontend.fifo_wr_ptr[1], u_new.u_frontend.fifo_wr_ptr[0]};
  assign orig_fe_inst_fifo_empty_int = u_orig.frontend.inst_fifo_empty;
  assign new_fe_inst_fifo_empty_int = u_new.u_frontend.inst_fifo_empty;
  assign orig_fe_inst_fifo_full_int = u_orig.frontend.inst_fifo_full;
  assign new_fe_inst_fifo_full_int = u_new.u_frontend.inst_fifo_full;
  assign orig_fe_exp_thread_id_int = u_orig.frontend.f7_exp_thread_id;
  assign new_fe_exp_thread_id_int = u_new.u_frontend.f7_exp_thread_id;
  assign orig_vm_status_int = {u_orig.vm_status[1], u_orig.vm_status[0]};
  assign new_vm_status_int = {u_new.vm_status[1], u_new.vm_status[0]};
  assign orig_id_intpipe_ctrl_int = u_orig.id_intpipe_ctrl;
  assign new_id_intpipe_ctrl_int = u_new.id_intpipe_ctrl;
  assign orig_id_frontend_vpu_ctrl_int = u_orig.id_frontend_vpu_ctrl;
  assign new_id_frontend_vpu_ctrl_int = u_new.id_frontend_vpu_ctrl;
  assign orig_debug_ffb_wdata_int = u_orig.debug_ffb_wdata;
  assign new_debug_ffb_wdata_int = u_new.debug_ffb_wdata;
  assign orig_debug_ffb_en_int = u_orig.debug_ffb_en;
  assign new_debug_ffb_en_int = u_new.debug_ffb_en;
  assign orig_debug_ffb_thread_sel_int = u_orig.debug_ffb_thread_sel;
  assign new_debug_ffb_thread_sel_int = u_new.debug_ffb_thread_sel;
  assign orig_debug_ex_program_buffer_int = u_orig.debug_ex_program_buffer;
  assign new_debug_ex_program_buffer_int = u_new.debug_ex_program_buffer;
  assign orig_debug_halt_int = u_orig.debug_in_core.halt;
  assign new_debug_halt_int = u_new.debug_in_core.halt;
  assign orig_pending_halt_int = u_orig.pending_halt;
  assign new_pending_halt_int = u_new.pending_halt;
  assign orig_dcache_core_ctrl_int = u_orig.s0_core_dcache_ctrl;
  assign new_dcache_core_ctrl_int = u_new.s0_core_dcache_ctrl;
  assign orig_dcache_core_ctrl_resp_int = u_orig.s0_dcache_core_ctrl_resp;
  assign new_dcache_core_ctrl_resp_int = u_new.s0_dcache_core_ctrl_resp;
  assign orig_dcache_req_int = u_orig.ex_core_dcache_req;
  assign new_dcache_req_int = u_new.ex_core_dcache_req;
  assign orig_dcache_req_valid_int = u_orig.ex_core_dcache_req_valid;
  assign new_dcache_req_valid_int = u_new.ex_core_dcache_req_valid;
  assign orig_dcache_req_gsc_int = u_orig.ex_core_dcache_gsc;
  assign new_dcache_req_gsc_int = u_new.ex_core_dcache_gsc;
  assign orig_dcache_resp_int = u_orig.wb_dcache_core_resp;
  assign new_dcache_resp_int = u_new.wb_dcache_core_resp;
  assign orig_dcache_resp_valid_int = u_orig.wb_dcache_core_resp_valid;
  assign new_dcache_resp_valid_int = u_new.wb_dcache_core_resp_valid;
  assign orig_dcache_scoreboard_int = u_orig.id_dcache_core_scoreboard;
  assign new_dcache_scoreboard_int = u_new.id_dcache_core_scoreboard_dcache;
  assign orig_dcache_debug_sigs_int = u_orig.dcache_debug_signals;
  assign new_dcache_debug_sigs_int = u_new.dcache_debug_signals;
  assign orig_dcache_col2_state_int = u_orig.dcache.cache_op_unit_l2.state;
  assign new_dcache_col2_state_int = u_new.u_dcache.u_cache_op_unit_l2.state_q;
  assign orig_dcache_col2_req_int = u_orig.dcache.cache_op_unit_l2.req;
  assign new_dcache_col2_req_int = u_new.u_dcache.u_cache_op_unit_l2.req_q;
  assign orig_dcache_col2_req_addr_int = u_orig.dcache.cache_op_unit_l2.req_addr;
  assign new_dcache_col2_req_addr_int = u_new.u_dcache.u_cache_op_unit_l2.req_addr_q;
  assign orig_dcache_col2_req_count_int = u_orig.dcache.cache_op_unit_l2.req_count;
  assign new_dcache_col2_req_count_int = u_new.u_dcache.u_cache_op_unit_l2.req_count_q;
  assign orig_dcache_col2_id_running_int = u_orig.dcache.cache_op_unit_l2.id_running;
  assign new_dcache_col2_id_running_int = u_new.u_dcache.u_cache_op_unit_l2.id_running_q;
  assign orig_dcache_col2_l2_scp_dest_int = u_orig.dcache.cache_op_unit_l2.l2_scp_dest;
  assign new_dcache_col2_l2_scp_dest_int = u_new.u_dcache.u_cache_op_unit_l2.l2_scp_dest_q;
  assign orig_dcache_col2_err_mem_int = u_orig.dcache.cache_op_unit_l2.err_mem;
  assign new_dcache_col2_err_mem_int = u_new.u_dcache.u_cache_op_unit_l2.err_mem_q;
  assign orig_dcache_col2_cache_op_req_is_l2_int = u_orig.dcache.cache_op_unit_l2.cache_op_req_is_l2;
  assign new_dcache_col2_cache_op_req_is_l2_int = u_new.u_dcache.u_cache_op_unit_l2.cache_op_req_is_l2_q;
  assign orig_dcache_col2_cache_op_req_is_nop_int = u_orig.dcache.cache_op_unit_l2.cache_op_req_is_NOP;
  assign new_dcache_col2_cache_op_req_is_nop_int = u_new.u_dcache.u_cache_op_unit_l2.cache_op_req_is_nop_q;
  assign orig_dcache_col2_l2_cache_op_cnt_int = u_orig.dcache.cache_op_unit_l2.l2_cache_op_cnt;
  assign new_dcache_col2_l2_cache_op_cnt_int = u_new.u_dcache.u_cache_op_unit_l2.l2_cache_op_cnt_q;
  assign orig_dcache_col2_reprate_count_int = u_orig.dcache.cache_op_unit_l2.cacheop_reprate_count;
  assign new_dcache_col2_reprate_count_int = u_new.u_dcache.u_cache_op_unit_l2.cacheop_reprate_count_q;
  assign orig_dcache_col2_reprate_grant_int = u_orig.dcache.cache_op_unit_l2.cacheop_reprate_grant;
  assign new_dcache_col2_reprate_grant_int = u_new.u_dcache.u_cache_op_unit_l2.cacheop_reprate_grant_q;
  assign orig_dcache_col2_l2_req_valid_started_int = u_orig.dcache.cache_op_unit_l2.l2_req_valid_started;
  assign new_dcache_col2_l2_req_valid_started_int = u_new.u_dcache.u_cache_op_unit_l2.l2_req_valid_started_q;
  assign orig_dcache_col2_ready_counters_int = u_orig.dcache.cache_op_unit_l2.ready_counters;
  assign new_dcache_col2_ready_counters_int = u_new.u_dcache.u_cache_op_unit_l2.ready_counters_q;
  assign orig_dcache_col2_l2_resp_valid_int = u_orig.dcache.l2_resp_valid_to_co_l2;
  assign new_dcache_col2_l2_resp_valid_int = u_new.u_dcache.l2_resp_valid_to_co_l2;
  assign orig_dcache_col2_l2_resp_id_int = u_orig.dcache.l2_resp_int.id;
  assign new_dcache_col2_l2_resp_id_int = u_new.u_dcache.l2_resp_int.id;
  assign orig_dcache_col2_l2_resp_dest_int = u_orig.dcache.l2_resp_int.dest;
  assign new_dcache_col2_l2_resp_dest_int = u_new.u_dcache.l2_resp_int.dest;
  assign orig_dcache_col2_l2_resp_wdata_int = u_orig.dcache.l2_resp_int.wdata;
  assign new_dcache_col2_l2_resp_wdata_int = u_new.u_dcache.l2_resp_int.wdata;
  assign orig_dcache_col2_l2_resp_opcode_int = u_orig.dcache.l2_resp_int.opcode;
  assign new_dcache_col2_l2_resp_opcode_int = u_new.u_dcache.l2_resp_int.opcode;
  assign orig_dcache_col2_l2_resp_data_int = u_orig.dcache.l2_resp_int.data;
  assign new_dcache_col2_l2_resp_data_int = u_new.u_dcache.l2_resp_int.data;
  assign orig_dcache_col2_l2_resp_size_int = u_orig.dcache.l2_resp_int.size;
  assign new_dcache_col2_l2_resp_size_int = u_new.u_dcache.l2_resp_int.size;
  assign orig_dcache_col2_l2_resp_qwen_int = u_orig.dcache.l2_resp_int.qwen;
  assign new_dcache_col2_l2_resp_qwen_int = u_new.u_dcache.l2_resp_int.qwen;
  assign orig_dcache_col2_l2_resp_data_hi_int = u_orig.dcache.l2_resp_int.data[255:240];
  assign new_dcache_col2_l2_resp_data_hi_int = u_new.u_dcache.l2_resp_int.data[255:240];
  assign orig_dcache_l2_resp_in_data_hi_int = u_orig.dcache.l2_resp.data[255:240];
  assign new_dcache_l2_resp_in_data_hi_int = u_new.u_dcache.l2_resp_i.data[255:240];
  assign orig_dcache_col2_l2_resp_is_prefetch_int = u_orig.dcache.cache_op_unit_l2.l2_resp_is_prefetch;
  assign new_dcache_col2_l2_resp_is_prefetch_int = u_new.u_dcache.u_cache_op_unit_l2.l2_resp_is_prefetch;
  assign orig_dcache_col2_pf_l2_evict_ready_int = u_orig.dcache.pf_l2_evict_ready;
  assign new_dcache_col2_pf_l2_evict_ready_int = u_new.u_dcache.pf_l2_evict_ready;
  assign orig_dcache_s3_lrsc_valid_int = u_orig.dcache.s3_lrsc_valid;
  assign new_dcache_s3_lrsc_valid_int = u_new.u_dcache.s3_lrsc_valid;

endmodule
/* verilator lint_on SYNCASYNCNET */
