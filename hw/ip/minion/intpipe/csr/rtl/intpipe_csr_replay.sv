// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// intpipe_csr_replay — CSR replay condition detection.
//
// Monitors replay conditions for CSR writes that cannot be handled
// immediately (cache ops busy, barriers not ready, tensor units busy,
// message ports stalled, etc.) and generates replay/stall signals.
//
// Replaces: intpipe_csr_replay (etcore-soc)

module intpipe_csr_replay
  import minion_pkg::*;
  #(
    parameter bit VpuEn = 1'b1
  )
(
  input  logic                                                 clk_i,
  input  logic                                                 rst_ni,

  // input control signals
  /* verilator lint_off UNUSEDSIGNAL */  // Replay detection only inspects the architectural subset of the preserved write-data and dcache-control structs.
  input  logic [XregSize-1:0]                                  wb_wdata,
  input  logic                                                 wb_thread_id,
  input  logic [NrThreads-1:0]                                 io_interrupt,
  input  logic [NrThreads-1:0]                                 io_redirect,

  // output control signals
  output logic                                                 io_replay,
  output logic [NrThreads-1:0]                                 io_replay_stall,

  // pulses when accessing a CSR that can trigger a replay
  input  logic                                                 reg_co_evict_va_en,
  input  logic                                                 reg_co_flush_va_en,
  input  logic                                                 reg_co_lock_va_en_qual,
  input  logic                                                 reg_co_unlock_va_en_qual,
  input  logic                                                 reg_co_lock_sw_en,
  input  logic                                                 reg_co_unlock_sw_en,
  input  logic                                                 reg_co_prefetch_va_en,
  input  logic                                                 reg_co_evict_sw_en,
  input  logic                                                 reg_co_flush_sw_en,
  input  logic                                                 reg_tensor_load_l2scp_en,
  input  logic                                                 reg_flb_en,
  input  logic                                                 reg_tensor_load_en,
  input  logic                                                 reg_tensor_fma_en,
  input  logic                                                 reg_tensor_quant_en,
  input  logic                                                 reg_tensor_reduce_en,
  input  logic                                                 reg_tensor_store_en,
  input  logic [NrThreads-1:0]                                 reg_tensor_conv_ctrl_en,
  input  logic [NrThreads-1:0]                                 reg_tensor_conv_size_en,
  input  logic [NrThreads-1:0]                                 reg_fcc_en,
  input  logic [NrThreads-1:0]                                 reg_tensor_wait_en,
  input  logic                                                 reg_tex_send_en,
  input  logic                                                 reg_tensor_mask_read_invalid,
  input  logic                                                 pmu_read_en_wb,
  input  logic [NrThreads-1:0]                                 pmu_read_mem_ready,

  // signals to check for conditions
  input  dcache_core_ctrl_t                                    dcache_ctrl_resp,
  /* verilator lint_on UNUSEDSIGNAL */
  input  logic                                                 fl_barrier_ready,
  input  logic                                                 tensor_fma_replay_condition,
  input  logic                                                 tensor_quant_replay_condition,
  input  logic                                                 vpu_treduce_enabled,
  input  logic                                                 tenb_tl_pend_tfma_en,
  input  logic                                                 tenb_restart_process,
  input  logic                                                 tload_is_for_tenb,
  input  logic [NrThreads-1:0]                                 conv_can_accept_new_write,
  input  logic [(NrThreads*FccPerMin)-1:0]                     fcc_ready,
  input  logic [$clog2(NrThreads*FccPerMin)-1:0]               wb_fcc_addr,
  input  logic                                                 msg_port_pget_block,
  input  logic [NrThreads-1:0][NrMsgPorts-1:0]                msg_port_stall,
  input  logic [NrThreads-1:0]                                 reg_tensor_mask_ready
);

  logic [NrThreads-1:0][CsrReplaySz-1:0]  csr_replay_stall;
  logic [NrThreads-1:0][CsrReplaySz-1:0]  csr_replay_stall_next;
  logic [NrThreads-1:0][CsrReplaySz-1:0]  clear_stall;
  logic [CsrReplaySz-1:0]                 wb_csr_replay;
  logic [3:0]                              wb_replay_tensor_wait_cond;

  // ── Compute replay in current WB cycle ─────────────────────────────
  always_comb begin
    wb_csr_replay = '0;

    // Cache op unit not ready
    if (reg_co_evict_va_en       && !dcache_ctrl_resp.cache_op_ready) wb_csr_replay[CSR_REPLAY_CACHE_OP] = 1'b1;
    if (reg_co_flush_va_en       && !dcache_ctrl_resp.cache_op_ready) wb_csr_replay[CSR_REPLAY_CACHE_OP] = 1'b1;
    if (reg_co_lock_va_en_qual   && !dcache_ctrl_resp.cache_op_ready) wb_csr_replay[CSR_REPLAY_CACHE_OP] = 1'b1;
    if (reg_co_unlock_va_en_qual && !dcache_ctrl_resp.cache_op_ready) wb_csr_replay[CSR_REPLAY_CACHE_OP] = 1'b1;
    if (reg_co_lock_sw_en        && !dcache_ctrl_resp.cache_op_ready) wb_csr_replay[CSR_REPLAY_CACHE_OP] = 1'b1;
    if (reg_co_unlock_sw_en      && !dcache_ctrl_resp.cache_op_ready) wb_csr_replay[CSR_REPLAY_CACHE_OP] = 1'b1;
    if (reg_co_prefetch_va_en    && !dcache_ctrl_resp.cache_op_ready) wb_csr_replay[CSR_REPLAY_CACHE_OP] = 1'b1;
    if (reg_co_evict_sw_en       && !dcache_ctrl_resp.cache_op_ready) wb_csr_replay[CSR_REPLAY_CACHE_OP] = 1'b1;
    if (reg_co_flush_sw_en       && !dcache_ctrl_resp.cache_op_ready) wb_csr_replay[CSR_REPLAY_CACHE_OP] = 1'b1;
    if (VpuEn && reg_tensor_load_l2scp_en && !dcache_ctrl_resp.cache_op_ready) wb_csr_replay[CSR_REPLAY_CACHE_OP] = 1'b1;

    // FLB and barrier unit not ready
    if (reg_flb_en && !fl_barrier_ready) wb_csr_replay[CSR_REPLAY_FLB] = 1'b1;

    // Tensor load
    if (VpuEn && reg_tensor_load_en &&  !dcache_ctrl_resp.tl_ready_0                          && !tload_is_for_tenb) wb_csr_replay[CSR_REPLAY_TL0] = 1'b1;
    if (VpuEn && reg_tensor_load_en && (!dcache_ctrl_resp.tl_ready_1 || tenb_tl_pend_tfma_en) &&  tload_is_for_tenb)  wb_csr_replay[CSR_REPLAY_TL1] = 1'b1;

    // Tensor FMA
    if (reg_tensor_fma_en && (tensor_fma_replay_condition || tenb_restart_process)) wb_csr_replay[CSR_REPLAY_TFMA] = 1'b1;

    // Tensor quant
    if (reg_tensor_quant_en && tensor_quant_replay_condition) wb_csr_replay[CSR_REPLAY_TQUANT] = 1'b1;

    // Tensor reduce
    if (VpuEn && reg_tensor_reduce_en && (!dcache_ctrl_resp.reduce_ready || vpu_treduce_enabled)) wb_csr_replay[CSR_REPLAY_TREDUCE] = 1'b1;

    // Tensor store
    if (VpuEn && reg_tensor_store_en && (!dcache_ctrl_resp.ts_ready || vpu_treduce_enabled)) wb_csr_replay[CSR_REPLAY_TS] = 1'b1;

    // Conv CSR busy
    if ((|reg_tensor_conv_ctrl_en) && !conv_can_accept_new_write[wb_thread_id]) wb_csr_replay[CSR_REPLAY_CONV] = 1'b1;
    if ((|reg_tensor_conv_size_en) && !conv_can_accept_new_write[wb_thread_id]) wb_csr_replay[CSR_REPLAY_CONV] = 1'b1;

    // FCC no credits
    if ((|reg_fcc_en) && !fcc_ready[wb_fcc_addr] && wb_wdata[$clog2(FccPerMin)-1:0] == 1'b0) wb_csr_replay[CSR_REPLAY_FCC0] = 1'b1;
    if ((|reg_fcc_en) && !fcc_ready[wb_fcc_addr] && wb_wdata[$clog2(FccPerMin)-1:0] == 1'b1) wb_csr_replay[CSR_REPLAY_FCC1] = 1'b1;

    // Tensor mask
    if (reg_tensor_mask_read_invalid) wb_csr_replay[CSR_REPLAY_TMASK] = 1'b1;

    // PMU read
    if (pmu_read_en_wb && !pmu_read_mem_ready[wb_thread_id]) wb_csr_replay[CSR_REPLAY_PMU_READ] = 1'b1;

    // Tex send
    if (VpuEn && reg_tex_send_en && !dcache_ctrl_resp.tex_send_ready) wb_csr_replay[CSR_REPLAY_TEX] = 1'b1;

    // PGET block
    wb_csr_replay[CSR_REPLAY_PGET] = msg_port_pget_block;

    // ── Tensor wait replays ────────────────────────────────────────
    wb_replay_tensor_wait_cond = wb_wdata[3:0];

    if (VpuEn && (|reg_tensor_wait_en) && (!dcache_ctrl_resp.tl_complete_0[0] && wb_thread_id == 1'b0) && (wb_replay_tensor_wait_cond == 4'd0))
      wb_csr_replay[CSR_REPLAY_TW_TL0] = 1'b1;
    if (VpuEn && (|reg_tensor_wait_en) && (!dcache_ctrl_resp.tl_complete_0[1] && wb_thread_id == 1'b0) && (wb_replay_tensor_wait_cond == 4'd1))
      wb_csr_replay[CSR_REPLAY_TW_TL1] = 1'b1;
    if (VpuEn && (|reg_tensor_wait_en) && (!dcache_ctrl_resp.cache_op_complete_l2[wb_thread_id][0]) && (wb_replay_tensor_wait_cond == 4'd2))
      wb_csr_replay[CSR_REPLAY_TW_PREFETCH0] = 1'b1;
    if (VpuEn && (|reg_tensor_wait_en) && (!dcache_ctrl_resp.cache_op_complete_l2[wb_thread_id][1]) && (wb_replay_tensor_wait_cond == 4'd3))
      wb_csr_replay[CSR_REPLAY_TW_PREFETCH1] = 1'b1;
    if (VpuEn && (|reg_tensor_wait_en) && (!dcache_ctrl_resp.cache_op_complete_l2[wb_thread_id][0]) && (wb_replay_tensor_wait_cond == 4'd4))
      wb_csr_replay[CSR_REPLAY_TW_PREFETCH0] = 1'b1;
    if (VpuEn && (|reg_tensor_wait_en) && (!dcache_ctrl_resp.cache_op_complete_l2[wb_thread_id][1]) && (wb_replay_tensor_wait_cond == 4'd5))
      wb_csr_replay[CSR_REPLAY_TW_PREFETCH1] = 1'b1;
    if (VpuEn && (|reg_tensor_wait_en) && (&dcache_ctrl_resp.cache_op_complete_l2[wb_thread_id] == 1'b0 || &dcache_ctrl_resp.cache_op_complete_l1[wb_thread_id] == 1'b0) && (wb_replay_tensor_wait_cond == 4'd6))
      wb_csr_replay[CSR_REPLAY_TW_CACHE_OP] = 1'b1;
    if ((|reg_tensor_wait_en) && (tensor_fma_replay_condition && wb_thread_id == 1'b0) && (wb_replay_tensor_wait_cond == 4'd7))
      wb_csr_replay[CSR_REPLAY_TW_TENSOR_FMA] = 1'b1;
    if (VpuEn && (|reg_tensor_wait_en) && (!dcache_ctrl_resp.ts_ready && wb_thread_id == 1'b0) && (wb_replay_tensor_wait_cond == 4'd8))
      wb_csr_replay[CSR_REPLAY_TW_TENSOR_STORE] = 1'b1;
    if (VpuEn && (|reg_tensor_wait_en) && (!dcache_ctrl_resp.reduce_ready && wb_thread_id == 1'b0) && (wb_replay_tensor_wait_cond == 4'd9))
      wb_csr_replay[CSR_REPLAY_TW_TENSOR_REDUCE] = 1'b1;
    if ((|reg_tensor_wait_en) && (tensor_quant_replay_condition && wb_thread_id == 1'b0) && (wb_replay_tensor_wait_cond == 4'd10))
      wb_csr_replay[CSR_REPLAY_TW_TENSOR_QUANT] = 1'b1;
  end

  // ── Clear stalls ────────────────────────────────────────────────────
  always_comb begin
    clear_stall = '0;

    for (int thread = 0; thread < NrThreads; thread++) begin
      if (dcache_ctrl_resp.cache_op_busy_changed[thread])                                       clear_stall[thread][CSR_REPLAY_CACHE_OP] = 1'b1;
      if (fl_barrier_ready)                                                                      clear_stall[thread][CSR_REPLAY_FLB] = 1'b1;
      if (!VpuEn || dcache_ctrl_resp.tl_ready_0)                                                 clear_stall[thread][CSR_REPLAY_TL0] = 1'b1;
      if (!VpuEn || dcache_ctrl_resp.tl_ready_1)                                                 clear_stall[thread][CSR_REPLAY_TL1] = 1'b1;
      if (!tensor_fma_replay_condition)                                                          clear_stall[thread][CSR_REPLAY_TFMA] = 1'b1;
      if (!tensor_quant_replay_condition)                                                        clear_stall[thread][CSR_REPLAY_TQUANT] = 1'b1;
      if ((!VpuEn || dcache_ctrl_resp.reduce_ready) && !vpu_treduce_enabled)                     clear_stall[thread][CSR_REPLAY_TREDUCE] = 1'b1;
      if ((!VpuEn || dcache_ctrl_resp.ts_ready) && !vpu_treduce_enabled)                         clear_stall[thread][CSR_REPLAY_TS] = 1'b1;
      if (conv_can_accept_new_write[thread])                                                     clear_stall[thread][CSR_REPLAY_CONV] = 1'b1;
      if (|msg_port_stall[thread] == 1'b0)                                                       clear_stall[thread][CSR_REPLAY_PGET] = 1'b1;
      if (fcc_ready[{thread[0], 1'b0}])                                                          clear_stall[thread][CSR_REPLAY_FCC0] = 1'b1;
      if (fcc_ready[{thread[0], 1'b1}])                                                          clear_stall[thread][CSR_REPLAY_FCC1] = 1'b1;
      if (reg_tensor_mask_ready[thread])                                                         clear_stall[thread][CSR_REPLAY_TMASK] = 1'b1;
      if (pmu_read_mem_ready[thread])                                                            clear_stall[thread][CSR_REPLAY_PMU_READ] = 1'b1;
      if (!VpuEn || dcache_ctrl_resp.tex_send_ready)                                             clear_stall[thread][CSR_REPLAY_TEX] = 1'b1;
      if ((!VpuEn || dcache_ctrl_resp.tl_complete_0[0]) && thread == 0)                         clear_stall[thread][CSR_REPLAY_TW_TL0] = 1'b1;
      if ((!VpuEn || dcache_ctrl_resp.tl_complete_0[1]) && thread == 0)                         clear_stall[thread][CSR_REPLAY_TW_TL1] = 1'b1;
      if ((!VpuEn || dcache_ctrl_resp.cache_op_complete_l2[thread][0]))                          clear_stall[thread][CSR_REPLAY_TW_PREFETCH0] = 1'b1;
      if ((!VpuEn || dcache_ctrl_resp.cache_op_complete_l2[thread][1]))                          clear_stall[thread][CSR_REPLAY_TW_PREFETCH1] = 1'b1;
      if ((!VpuEn || (&dcache_ctrl_resp.cache_op_complete_l2[thread] && &dcache_ctrl_resp.cache_op_complete_l1[thread]))) clear_stall[thread][CSR_REPLAY_TW_CACHE_OP] = 1'b1;
      if (!tensor_fma_replay_condition && thread == 0)                                           clear_stall[thread][CSR_REPLAY_TW_TENSOR_FMA] = 1'b1;
      if ((!VpuEn || dcache_ctrl_resp.ts_ready) && thread == 0)                                  clear_stall[thread][CSR_REPLAY_TW_TENSOR_STORE] = 1'b1;
      if ((!VpuEn || dcache_ctrl_resp.reduce_ready) && thread == 0)                              clear_stall[thread][CSR_REPLAY_TW_TENSOR_REDUCE] = 1'b1;
      if (!tensor_quant_replay_condition && thread == 0)                                         clear_stall[thread][CSR_REPLAY_TW_TENSOR_QUANT] = 1'b1;

      // clear stall on interrupt or redirect
      if (io_interrupt[thread] || io_redirect[thread]) clear_stall[thread] = '1;
    end
  end

  // ── Compute stall per thread ────────────────────────────────────────
  always_comb begin
    csr_replay_stall_next = csr_replay_stall & ~clear_stall;
    csr_replay_stall_next[wb_thread_id] |= wb_csr_replay;

    io_replay = |wb_csr_replay;

    for (int thread = 0; thread < NrThreads; thread++) begin
      io_replay_stall[thread] = |csr_replay_stall[thread];
    end
  end

  // ── Stall FF with self-XOR gating ──────────────────────────────────
  localparam int SelfXorMinWidth = 8;

  for (genvar t = 0; t < NrThreads; t++) begin : gen_stall_t
    for (genvar i = 0; i < CsrReplaySz; i += SelfXorMinWidth) begin : gen_stall_i
      localparam int W = (CsrReplaySz - i < SelfXorMinWidth) ? (CsrReplaySz - i) : SelfXorMinWidth;
      logic en;
      assign en = |(csr_replay_stall[t][i +: W] ^ csr_replay_stall_next[t][i +: W]);

      always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni)
          csr_replay_stall[t][i +: W] <= '0;
        else if (en)
          csr_replay_stall[t][i +: W] <= csr_replay_stall_next[t][i +: W];
      end
    end
  end

endmodule
