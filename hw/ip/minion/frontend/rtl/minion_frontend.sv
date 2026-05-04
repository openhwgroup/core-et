// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Minion frontend top — instruction fetch, decode, and issue.
//
// Instantiates per-thread fetch buffers, ICache request arbiter,
// ICache response pipeline, intpipe/VPU decoders, per-thread 2-entry
// instruction FIFOs, and round-robin thread scheduler.
//
// Replaces: frontend_top (etcore-soc)
//
// Differences from original:
//   - Active-low async-assert/sync-deassert reset (rst_ni)
//   - lowRISC naming conventions
//   - DFT struct instead of individual ports
//   - prim_arb_lru instead of arb_lru_data
//   - id_vpu_core_ctrl not output (consumer does mapping)

/* verilator lint_off VARHIDDEN */  // Module parameter intentionally shadows the shared package constant so instances can override thread count
module minion_frontend
  import minion_pkg::*;
  import minion_frontend_pkg::*;
  import dft_pkg::*;
#(
  /* verilator lint_off VARHIDDEN */  // The frontend remains parameterized by thread count even though the default comes from the shared minion package.
  parameter int unsigned NrThreads = NrThreadsDefault,  // 2
  parameter bit          VpuEn = 1'b1,
  parameter bit          EnableExtraTrans = 1'b0
  /* verilator lint_on VARHIDDEN */
) (
  input  logic                        clk_i,
  input  logic                        rst_ni,
  input  logic                        reset_debug_i,
  input  logic                        chicken_bit_i,
  input  dft_t                        dft_i,

  // Thread control
  input  logic [NrThreads-1:0]        f0_thread_enabled_i,
  input  logic [VaSize-1:0]           f0_reset_vector_i,
  input  vm_status_t [NrThreads-1:0]  vm_status_i,

  // ICache request interface (to ICache)
  input  logic                        f1_icache_req_ready_i,
  output logic                        f1_icache_req_valid_o,
  output fe_icache_req_t              f1_icache_req_o,

  // ICache response interface (from ICache, F5 stage)
  input  logic                        f5_icache_resp_valid_i,
  input  logic                        f5_icache_resp_miss_i,
  input  icache_fe_resp_t             f5_icache_resp_i,
  input  logic                        f6_icache_fill_done_i,

  // Core redirect / mispredict (per thread)
  input  logic [NrThreads-1:0]        f0_core_req_valid_i,
  input  fe_req_t [NrThreads-1:0]     f0_core_req_i,

  // Core stall (per thread)
  input  logic [NrThreads-1:0]        id_core_stall_i,

  // ID stage output: instruction
  input  logic                        id_inst_ready_i,
  output logic                        id_inst_valid_o,
  output logic                        id_inst_thread_id_o,
  output fe_core_resp_t               id_inst_data_o,

  // ID stage output: intpipe decode
  output minion_control_t             id_intpipe_ctrl_o,

  // ID stage output: VPU decode
  output vpu_ctrl_sigs_t              id_vpu_decoder_sigs_o,

  // ID stage output: VPU core control
  output vpu_id_ctrl_t                id_vpu_core_ctrl_o,

  // Debug-only frontend state exposure
  output fe_thread_debug_t [NrThreads-1:0] debug_thread_o,
  output logic [NrThreads-1:0]        debug_inst_fifo_empty_o,
  output logic [NrThreads-1:0]        debug_inst_fifo_full_o,
  output logic [NrThreads-1:0]        debug_tb_inst_valid_o,
  output logic [NrThreads-1:0]        debug_tb_inst_ready_o,

  // Debug program fetch buffer
  input  logic [63:0]                 debug_ffb_wdata_i,
  input  logic [3:0]                  debug_ffb_en_i,
  input  logic                        debug_ffb_thread_sel_i,
  input  logic [NrThreads-1:0]        debug_ffb_exec_i,
  input  logic [NrThreads-1:0]        halt_i,
  input  logic [NrThreads-1:0]        halted_i
);

  // Per-thread debug FFB enable routing (original: thread_debug_ffb_en)
  logic [3:0] thread_debug_ffb_en [NrThreads];
  for (genvar t = 0; t < NrThreads; t++) begin : gen_debug_ffb_en
    assign thread_debug_ffb_en[t] = (debug_ffb_thread_sel_i == t[0:0]) ? debug_ffb_en_i : '0;
  end

  // ════════════════════════════════════════════════════════
  // Thread enable detection
  // ════════════════════════════════════════════════════════

  logic [NrThreads-1:0] thread_enabled_f_q, thread_enabled_ff_q;
  logic                 thread_enable_changes;
  logic [NrThreads-1:0] f0_disable_thread, f0_enable_thread;

  assign thread_enable_changes = (f0_thread_enabled_i != thread_enabled_f_q)
                              || (thread_enabled_f_q != thread_enabled_ff_q)
                              || !rst_ni;
  assign f0_disable_thread = (~thread_enabled_f_q) & thread_enabled_ff_q;
  assign f0_enable_thread  = thread_enabled_f_q & (~thread_enabled_ff_q);

  // Original frontend_top uses EN_FF here, not reset flops. The enable pipeline
  // must sample f0_thread_enabled_i during reset rather than clearing to zero.
  always_ff @(posedge clk_i) begin
    if (thread_enable_changes) begin
      thread_enabled_f_q  <= f0_thread_enabled_i;
      thread_enabled_ff_q <= thread_enabled_f_q;
    end
  end

  // ════════════════════════════════════════════════════════
  // ICache response pipeline (F5 → F6 → F7)
  // ════════════════════════════════════════════════════════

  logic           f6_resp_valid_q;
  logic           f6_resp_miss_q;
  icache_fe_resp_t f6_resp_q;
  logic           f7_fill_done_q;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f6_resp_valid_q <= 1'b0;
      f7_fill_done_q  <= 1'b0;
    end else begin
      f6_resp_valid_q <= f5_icache_resp_valid_i;
      f7_fill_done_q  <= f6_icache_fill_done_i;
    end
  end

  always_ff @(posedge clk_i) begin
    if (f5_icache_resp_valid_i) begin
      f6_resp_miss_q <= f5_icache_resp_miss_i;
      f6_resp_q      <= f5_icache_resp_i;
    end
  end

  // ════════════════════════════════════════════════════════
  // Per-thread fetch buffers
  // ════════════════════════════════════════════════════════

  logic [NrThreads-1:0]   tb_icache_req_valid;
  fe_icache_req_t          tb_icache_req [NrThreads];
  logic [NrThreads-1:0]   tb_icache_req_ready;
  logic [NrThreads-1:0]   tb_inst_valid;
  logic [NrThreads-1:0]   tb_inst_rvc;
  fe_operation_t           tb_inst_data [NrThreads];
  /* verilator lint_off UNOPTFLAT */
  logic [NrThreads-1:0]   tb_inst_ready;
  /* verilator lint_on UNOPTFLAT */

  for (genvar t = 0; t < NrThreads; t++) begin : gen_thread_buf
    minion_frontend_thread_buffer #(.ThreadId(t)) u_tb (
      .clk_i,
      .rst_ni,
      .reset_debug_i,
      .chicken_bit_i,
      .dft_i,
      .f0_thread_enabled_i  (thread_enabled_f_q[t]),
      .f0_reset_vector_i    ({f0_reset_vector_i[VaSize-1], f0_reset_vector_i}),
      .f0_disable_thread_i  (f0_disable_thread[t]),
      .f0_enable_thread_i   (f0_enable_thread[t]),
      .in_debug_mode_i      (halted_i[t]),
      .vm_status_i          (vm_status_i[t]),
      .f0_icache_req_ready_i(tb_icache_req_ready[t]),
      .f1_icache_req_ready_i(f1_icache_req_ready_arb),
      .f0_icache_req_valid_o(tb_icache_req_valid[t]),
      .f0_icache_req_o      (tb_icache_req[t]),
      .f5_icache_resp_miss_i (f5_icache_resp_miss_i),
      .f6_icache_resp_valid_i(f6_resp_valid_q),
      .f6_icache_resp_miss_i (f6_resp_miss_q),
      .f6_icache_resp_i      (f6_resp_q),
      .f7_icache_fill_done_i (f7_fill_done_q),
      .f0_core_req_valid_i  (f0_core_req_valid_i[t]),
      .f0_core_req_i        (f0_core_req_i[t]),
      .f7_inst_ready_i      (tb_inst_ready[t]),
      .f7_inst_rvc_o        (tb_inst_rvc[t]),
      .f7_inst_valid_o      (tb_inst_valid[t]),
      .f7_inst_data_o       (tb_inst_data[t]),
      .debug_o              (debug_thread_o[t]),
      .debug_ffb_wdata_i    (debug_ffb_wdata_i),
      .debug_ffb_en_i       (thread_debug_ffb_en[t]),
      .debug_ffb_exec_i     (debug_ffb_exec_i[t]),
      .io_halt_i            (halt_i[t])
    );
  end

  // ════════════════════════════════════════════════════════
  // ICache request arbitration
  // ════════════════════════════════════════════════════════

  logic f1_icache_req_ready_arb;
  assign f1_icache_req_ready_arb = f1_icache_req_ready_i || !f1_icache_req_valid_o;

  generate
    if (NrThreads == 1) begin : gen_single_thread_arb
      assign tb_icache_req_ready[0] = f1_icache_req_ready_arb;

      always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
          f1_icache_req_valid_o <= 1'b0;
        end else if (f1_icache_req_ready_arb) begin
          f1_icache_req_valid_o <= tb_icache_req_valid[0];
        end
      end

      always_ff @(posedge clk_i) begin
        if (f1_icache_req_ready_arb && tb_icache_req_valid[0])
          f1_icache_req_o <= tb_icache_req[0];
      end
    end else begin : gen_multi_thread_arb
      // LRU arbiter for ICache requests (matches original's arb_lru_data)
      /* verilator lint_off UNOPTFLAT */
      logic [NrThreads-1:0] arb_grant;
      /* verilator lint_on UNOPTFLAT */
      logic [$clog2(NrThreads)-1:0] arb_winner_idx;

      prim_arb_lru #(
        .NumClients(NrThreads)
      ) u_icache_arb (
        .clk_i,
        .rst_ni,
        .bid_i   (tb_icache_req_valid),
        .stall_i (!f1_icache_req_ready_arb),
        .grant_o (arb_grant),
        .winner_o(arb_winner_idx)
      );

      assign tb_icache_req_ready = arb_grant;

      // Data mux: select winner's request (matches arb_lru_data.data_winner)
      fe_icache_req_t arb_winner_req;
      assign arb_winner_req = tb_icache_req[arb_winner_idx];

      // F1 pipeline register (RST_EN_FF pattern)
      always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni)
          f1_icache_req_valid_o <= 1'b0;
        else if (f1_icache_req_ready_arb)
          f1_icache_req_valid_o <= |tb_icache_req_valid;
      end

      always_ff @(posedge clk_i) begin
        if (f1_icache_req_ready_arb && |tb_icache_req_valid)
          f1_icache_req_o <= arb_winner_req;
      end
    end
  endgenerate

  // ════════════════════════════════════════════════════════
  // Per-thread instruction conversion (F7)
  // ════════════════════════════════════════════════════════

  fe_core_resp_t f7_core_data [NrThreads];

  always_comb begin
    for (int t = 0; t < NrThreads; t++) begin
      f7_core_data[t].pc         = tb_inst_data[t].pc;
      f7_core_data[t].inst_bits  = tb_inst_data[t].inst_bits;
      f7_core_data[t].rvc        = tb_inst_rvc[t];
      f7_core_data[t].pf0        = tb_inst_valid[t] && tb_inst_data[t].page_fault0;
      f7_core_data[t].af0        = tb_inst_valid[t] && tb_inst_data[t].access_fault0;
      f7_core_data[t].pf1        = tb_inst_valid[t] && tb_inst_data[t].page_fault1;
      f7_core_data[t].af1        = tb_inst_valid[t] && tb_inst_data[t].access_fault1;
      f7_core_data[t].bus_error  = tb_inst_valid[t] && tb_inst_data[t].bus_err;
      f7_core_data[t].ecc_error  = tb_inst_valid[t] && tb_inst_data[t].ecc_err;
      f7_core_data[t].replay     = tb_inst_data[t].replay;
    end
  end

  // ════════════════════════════════════════════════════════
  // Expander picker: select which thread's instruction to decode
  // ════════════════════════════════════════════════════════

  /* verilator lint_off UNOPTFLAT */  // Circular combinational path: inst_fifo_full → f7_exp_thread_id → inst_fifo_push → inst_fifo_full (same as original)
  logic [NrThreads-1:0]   inst_fifo_full;
  logic [NrThreads-1:0]   inst_fifo_empty;
  logic [NrThreads-1:0]   inst_fifo_push;
  /* verilator lint_on UNOPTFLAT */

  fe_thread_data_t         inst_fifo_data [NrThreads];

  /* verilator lint_off UNOPTFLAT */
  logic f7_exp_thread_id;
  /* verilator lint_on UNOPTFLAT */
  fe_core_resp_t f7_arb_data;

  generate
    if (NrThreads == 1) begin : gen_single_exp
      always_comb begin
        f7_exp_thread_id = 1'b0;
        f7_arb_data      = f7_core_data[0];
      end
    end else begin : gen_multi_exp
      logic [NrThreads-1:0] f7_thread_awake;

      always_comb begin
        unique case (~inst_fifo_full & f7_thread_awake & tb_inst_valid)
          2'b01:   f7_exp_thread_id = 1'b0;
          2'b10:   f7_exp_thread_id = 1'b1;
          default: f7_exp_thread_id = ~id_inst_thread_id_o;
        endcase
        f7_arb_data = f7_core_data[f7_exp_thread_id];
      end
    end
  endgenerate

  // ════════════════════════════════════════════════════════
  // Intpipe & VPU decoders (F7 stage, combinational)
  // ════════════════════════════════════════════════════════

  minion_control_t f7_intpipe_ctrl;
  vpu_ctrl_sigs_t  f7_vpu_decoder_sigs;
  logic            f7_is_vpu_insn;

  intpipe_decode #(
    .EnableExtraTrans (EnableExtraTrans)
  ) u_intpipe_dec (
    .inst_bits (f7_arb_data.inst_bits),
    .inst_ctrl (f7_intpipe_ctrl)
  );

  generate
    if (VpuEn) begin : gen_vpu_dec
      vpu_decoder #(
        .EnableExtraTrans (EnableExtraTrans)
      ) u_vpu_dec (
        .id_inst     (f7_arb_data.inst_bits),
        .id_sigs     (f7_vpu_decoder_sigs),
        .id_vpu_insn (f7_is_vpu_insn)
      );
    end else begin : gen_no_vpu_dec
      always_comb begin
        f7_vpu_decoder_sigs = '0;
        f7_is_vpu_insn = 1'b0;
      end
    end
  endgenerate

  // ════════════════════════════════════════════════════════
  // Per-thread 2-entry instruction FIFO
  // ════════════════════════════════════════════════════════

  logic [1:0]          fifo_rd_ptr  [NrThreads];
  logic [1:0]          fifo_wr_ptr  [NrThreads];

  // FIFO state and push/pop/ready
  logic [NrThreads-1:0] f7_inst_valid_th;

  // FIFO sequential logic: stores decoded instruction data (fe_thread_data_t)
  for (genvar t = 0; t < NrThreads; t++) begin : gen_fifo
    logic pop;
    fe_thread_data_t pending_data;
    logic            pending_data_is_fp;

    assign inst_fifo_push[t] = (f7_exp_thread_id == t[0]) && tb_inst_valid[t] && !inst_fifo_full[t];
    assign pop = id_inst_valid_o && id_inst_ready_i && (id_inst_thread_id_o == t[0]);
    assign inst_fifo_full[t] = (fifo_rd_ptr[t] ^ fifo_wr_ptr[t]) == 2'b10;
    assign inst_fifo_empty[t] = (fifo_rd_ptr[t] ^ fifo_wr_ptr[t]) == 2'b00;

    // Original uses synchronous-reset macros here. Keep the reset decision on
    // the clock edge so the unreset FIFO payload flops see the same control
    // state on a warm-reset cycle as the original design.
    always_ff @(posedge clk_i) begin
      if (!rst_ni) begin
        fifo_rd_ptr[t] <= 2'b00;
        fifo_wr_ptr[t] <= 2'b00;
      end else if (f0_core_req_valid_i[t]) begin
        fifo_rd_ptr[t] <= 2'b00;
        fifo_wr_ptr[t] <= 2'b00;
      end else begin
        fifo_rd_ptr[t] <= fifo_rd_ptr[t] + {1'b0, pop};
        fifo_wr_ptr[t] <= fifo_wr_ptr[t] + {1'b0, inst_fifo_push[t]};
      end
    end

    always_ff @(posedge clk_i) begin
      // FIFO empty: write to head slot
      if (inst_fifo_empty[t]) begin
        if (inst_fifo_push[t]) begin
          inst_fifo_data[t].core_resp    <= f7_arb_data;
          inst_fifo_data[t].intpipe_ctrl <= f7_intpipe_ctrl;
          if (f7_is_vpu_insn) inst_fifo_data[t].vpu_ctrl_sigs <= f7_vpu_decoder_sigs;
        end
      end
      // FIFO has 1 entry (not full)
      else if (!inst_fifo_full[t]) begin
        if (pop && inst_fifo_push[t]) begin
          // Simultaneous pop+push: write directly to head
          inst_fifo_data[t].core_resp    <= f7_arb_data;
          inst_fifo_data[t].intpipe_ctrl <= f7_intpipe_ctrl;
          if (f7_is_vpu_insn) inst_fifo_data[t].vpu_ctrl_sigs <= f7_vpu_decoder_sigs;
        end
        else if (inst_fifo_push[t]) begin
          // Push only: write to pending slot
          pending_data.core_resp    <= f7_arb_data;
          pending_data.intpipe_ctrl <= f7_intpipe_ctrl;
          if (f7_is_vpu_insn) pending_data.vpu_ctrl_sigs <= f7_vpu_decoder_sigs;
          pending_data_is_fp <= f7_is_vpu_insn;
        end
      end
      // FIFO full
      else begin
        if (pop) begin
          // Pop: move pending to head
          inst_fifo_data[t].core_resp    <= pending_data.core_resp;
          inst_fifo_data[t].intpipe_ctrl <= pending_data.intpipe_ctrl;
          if (pending_data_is_fp) inst_fifo_data[t].vpu_ctrl_sigs <= pending_data.vpu_ctrl_sigs;
        end
        if (inst_fifo_push[t]) begin
          // Push: write to pending
          pending_data.core_resp    <= f7_arb_data;
          pending_data.intpipe_ctrl <= f7_intpipe_ctrl;
          if (f7_is_vpu_insn) pending_data.vpu_ctrl_sigs <= f7_vpu_decoder_sigs;
          pending_data_is_fp <= f7_is_vpu_insn;
        end
      end
    end
  end

  always_comb begin
    for (int t = 0; t < NrThreads; t++) begin
      tb_inst_ready[t] = (f7_exp_thread_id == t[0]) && !inst_fifo_full[t];
      f7_inst_valid_th[t] = (!inst_fifo_empty[t] || inst_fifo_push[t]) && !f0_core_req_valid_i[t];
    end
  end

  always_comb begin
    debug_inst_fifo_empty_o = inst_fifo_empty;
    debug_inst_fifo_full_o  = inst_fifo_full;
    debug_tb_inst_valid_o   = tb_inst_valid;
    debug_tb_inst_ready_o   = tb_inst_ready;
  end

  // ════════════════════════════════════════════════════════
  // Thread scheduler
  // ════════════════════════════════════════════════════════

  logic f7_thread_id;

  generate
    if (NrThreads == 1) begin : gen_single_sched
      assign f7_thread_id = 1'b0;
    end else begin : gen_multi_sched
      minion_frontend_thread_sched #(.NrThreads(NrThreads)) u_sched (
        .clk_i,
        .rst_ni,
        .f7_valid_tid_i       (f7_inst_valid_th),
        .f0_thread_enabled_i  (thread_enabled_f_q),
        .id_core_stall_i      (id_core_stall_i),
        .f7_thread_awake_o    (gen_multi_exp.f7_thread_awake),
        .f7_thread_id_o       (f7_thread_id)
      );
    end
  endgenerate

  // ════════════════════════════════════════════════════════
  // ID stage output
  // ════════════════════════════════════════════════════════

  // Match the original synchronous-reset behavior so ID selection and FIFO pop
  // qualification stay aligned during warm-reset cycles.
  always_ff @(posedge clk_i) begin
    if (!rst_ni)
      id_inst_thread_id_o <= 1'b0;
    else
      id_inst_thread_id_o <= f7_thread_id;
  end

  always_comb begin
    id_inst_valid_o       = !inst_fifo_empty[id_inst_thread_id_o];
    id_inst_data_o        = inst_fifo_data[id_inst_thread_id_o].core_resp;
    id_intpipe_ctrl_o     = inst_fifo_data[id_inst_thread_id_o].intpipe_ctrl;
    id_vpu_decoder_sigs_o = VpuEn ? inst_fifo_data[id_inst_thread_id_o].vpu_ctrl_sigs : '0;
  end

  // ════════════════════════════════════════════════════════
  // VPU core control (combinational mapping from decoder sigs)
  // ════════════════════════════════════════════════════════

  always_comb begin
    id_vpu_core_ctrl_o          = '0;
    id_vpu_core_ctrl_o.m0ren    = id_vpu_decoder_sigs_o.m0ren;
    id_vpu_core_ctrl_o.mallren  = id_vpu_decoder_sigs_o.mallren;
    id_vpu_core_ctrl_o.mren1    = id_vpu_decoder_sigs_o.mren1;
    id_vpu_core_ctrl_o.mren2    = id_vpu_decoder_sigs_o.mren2;
    id_vpu_core_ctrl_o.wen      = id_vpu_decoder_sigs_o.wen | id_vpu_decoder_sigs_o.rend;
    id_vpu_core_ctrl_o.ren1     = id_vpu_decoder_sigs_o.ren1;
    id_vpu_core_ctrl_o.ren2     = id_vpu_decoder_sigs_o.ren2;
    id_vpu_core_ctrl_o.ren3     = id_vpu_decoder_sigs_o.ren3;
    id_vpu_core_ctrl_o.rend     = id_vpu_decoder_sigs_o.rend;
    id_vpu_core_ctrl_o.is_trans = id_vpu_decoder_sigs_o.trans;
    id_vpu_core_ctrl_o.fromint  = id_vpu_decoder_sigs_o.fromint;
  end

endmodule
/* verilator lint_on VARHIDDEN */
