// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Frontend thread buffer — per-thread fetch engine.
//
// Manages PC generation, ICache request pipeline (F0→F5),
// dual instruction buffers (256-bit fetch blocks), instruction
// read with RVC expansion (F6→F7), debug program fetch buffer,
// and clock gating.
//
// Each thread has 2 instruction buffers. Instructions are read
// from buffers in 16-bit aligned words, expanded through the
// RVC expander, and output to the frontend top for decode.
//
// Line-by-line translation of: frontend_thread_buffer (etcore-soc)
//
// Differences from original:
//   - Active-low async-assert/sync-deassert reset (rst_ni)
//   - lowRISC naming (_i/_o, _d/_q)
//   - prim_clk_gate instead of et_clk_gate
//   - prim_rf_1r1w_diff_preview instead of rf_latch_1r_1w_diff_widths + external LATCH_P2
//   - minion_frontend_pkg types instead of soc.vh defines

module minion_frontend_thread_buffer
  import minion_pkg::*;
  import minion_frontend_pkg::*;
  import dft_pkg::*;
#(
  parameter int unsigned ThreadId = 0
) (
  // System signals
  input  logic                        clk_i,       // always-on clock (clock_aon)
  input  logic                        rst_ni,
  input  logic                        reset_debug_i,
  input  logic                        chicken_bit_i,

  // DFT
  input  dft_t                        dft_i,

  // Thread enable and reset PC
  input  logic                        f0_thread_enabled_i,
  input  logic [VaSizeExt-1:0]        f0_reset_vector_i,
  input  logic                        f0_disable_thread_i,  // Thread is disabled this cycle
  input  logic                        f0_enable_thread_i,   // Thread is enabled this cycle
  input  logic                        in_debug_mode_i,

  // TLB control
  input  vm_status_t                  vm_status_i,

  // ICache requests
  /* verilator lint_off UNOPTFLAT */  // Cross-module combinational loop: arbiter grant -> f0_icache_req_ready_i -> thread buffer (same as original)
  input  logic                        f0_icache_req_ready_i,
  /* verilator lint_on UNOPTFLAT */
  input  logic                        f1_icache_req_ready_i,
  output logic                        f0_icache_req_valid_o,
  output fe_icache_req_t              f0_icache_req_o,

  // ICache response
  input  logic                        f5_icache_resp_miss_i,
  input  logic                        f6_icache_resp_valid_i,
  input  logic                        f6_icache_resp_miss_i,
  input  icache_fe_resp_t             f6_icache_resp_i,
  input  logic                        f7_icache_fill_done_i,

  // Mispredict from core
  input  logic                        f0_core_req_valid_i,
  input  fe_req_t                     f0_core_req_i,

  // Fetch resp
  /* verilator lint_off UNOPTFLAT */  // The F6/F7 ready-valid handshake closes through buffered state, but Verilator flattens the queue muxing into a false loop on the downstream ready input.
  input  logic                        f7_inst_ready_i,
  /* verilator lint_on UNOPTFLAT */
  output logic                        f7_inst_rvc_o,
  output logic                        f7_inst_valid_o,
  output fe_operation_t               f7_inst_data_o,
  output fe_thread_debug_t            debug_o,

  // Interface to write program buffer for debug
  input  logic [63:0]                 debug_ffb_wdata_i,
  input  logic [3:0]                  debug_ffb_en_i,
  input  logic                        debug_ffb_exec_i,
  input  logic                        io_halt_i
);

  ////////////////////////////////////////////////////////////////////////////////
  // This module takes care of fetching the instructions from the icache for one
  // thread and it sends the valid instructions in program order to the core.
  // There are FeFetchBuffers (currently 2) buffers that store
  // FeFetchValidInsts instructions (currently 16 halfwords = 8 words).
  // When the thread is active and one of the buffers has no pending
  // instructions to be sent, the thread requests the next fetch PC block to
  // the Icache.
  ////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////
  // signals
  ////////////////////////////////////////////////////////////////////////////////

  // buffer status
  logic [FeFetchBuffers-1:0]                    buffer_empty;
  logic [FeFetchBuffers-1:0]                    buffer_empty_next;
  logic [VaSizeExt-1:0]                         buffer_pc       [FeFetchBuffers];
  logic [FeFetchBuffers-1:0]                    buffer_page_fault;
  logic [FeFetchBuffers-1:0]                    buffer_access_fault;
  logic [FeFetchBuffers-1:0]                    buffer_cacheable;
  logic [FeFetchBuffers-1:0]                    buffer_ecc_err;
  logic [FeFetchBuffers-1:0]                    buffer_l2_err;
  logic [FeFetchReadSize/32-1:0]                buffer_word_en  [FeFetchBuffers];

  // F0
  logic                                         f0_new_inst_req;
  logic [VaSizeExt-1:0]                         f0_pc, f0_pc_next;
  logic [0:0]                                   f0_req_buffer;
  logic [FeFetchBuffers-1:0]                    f0_buffer_pc_en;
  logic                                         f0_icache_req_acc;
  logic                                         f0_pc_update;
  logic                                         f0_miss_pending;
  logic                                         f0_miss_pending_next;

  // F1
  logic [0:0]                                   f1_req_buffer;
  logic                                         f0_icache_req_kill_f1;
  logic                                         f1_icache_req_kill_f1;
  logic                                         f1_req_valid_to_f2;
  logic                                         f1_req_valid;

  // F2
  logic [0:0]                                   f2_req_buffer;
  logic                                         f2_req_valid_to_f3;
  logic                                         f2_req_valid;

  // F3
  logic [0:0]                                   f3_req_buffer;
  logic                                         f3_req_valid_to_f4;
  logic                                         f3_req_valid;

  // F4
  logic [0:0]                                   f4_req_buffer;
  logic                                         f4_req_valid_to_f5;
  logic                                         f4_req_valid;

  // F5
  logic [0:0]                                   f5_req_buffer;
  logic                                         f5_req_valid_to_f6;
  logic                                         f5_req_valid;

  // F6 write
  logic [FeFetchBuffers-1:0]                    f6_buffer_en;
  logic [FeFetchBuffers-1:0]                    f6_fault_en;
  logic [0:0]                                   f6_req_buffer;
  logic [FeFetchReadSize/32-1:0]                f6_buffer_wr_1p_next;
  logic                                         f6_req_valid;
  logic                                         f6_req_resp_valid;
  logic                                         f6_buffer_wr;
  logic [FeFetchReadSize-1:0]                   f6_wr_data;
  logic [FeFetchReadSize/32-1:0]                f6_buffer_wren;
  logic [0:0]                                   f6_write_ad;
  logic [FeFetchBuffers-1:0]                    f6_clear_buffer;

  // F6 read
  logic                                         f6_valid;
  /* verilator lint_off UNOPTFLAT */  // This stall path remains in the original latch/phase-sensitive fetch-buffer feedback loop and only looks cyclic after flattening.
  logic                                         f6_stall;
  /* verilator lint_on UNOPTFLAT */
  logic                                         f6_split;
  logic [31:0]                                  f6_buffer_inst_data;
  logic [31:0]                                  f6_inst_bits;
  logic [31:0]                                  f6_inst_bits_expanded;
  logic [FeFetchPtrSize-1:0]                    f6_buffer_ptr;
  logic [FeFetchPtrSize-1:0]                    f6_buffer_ptr_next;
  logic [FeBufferPosSz-1:0]                     f6_buffer_pos;
  logic [0:0]                                   f6_buffer_id;
  logic [0:0]                                   f6_buffer_id_next;
  logic                                         f6_last_from_buffer;
  /* verilator lint_off UNOPTFLAT */  // f6_inst_rvc → f6_buffer_ptr_next → f6_buffer_id → f6_inst_rvc: same loop as original
  logic                                         f6_inst_rvc;
  /* verilator lint_on UNOPTFLAT */
  logic [VaSizeExt-FeBufferPosSz-2:0]           f6_inst_pc_msb;

  // F7
  logic                                         f7_valid;
  logic                                         f7_stall;
  logic [FeFetchPtrSize-1:0]                    f7_buffer_ptr;
  logic [FeBufferPosSz-1:0]                     f7_buffer_pos;
  logic [0:0]                                   f7_buffer_id;
  logic [31:0]                                  f7_inst_bits;
  logic                                         f7_split;
  logic [VaSizeExt-FeBufferPosSz-2:0]           f7_inst_pc_msb;

  // DEBUG
  logic                                         using_pfb;
  logic                                         using_pfb_reg;
  logic                                         using_pfb_next;
  logic [3:0]                                   debug_ffb_en_2;
  logic                                         debug_ffb_exec_2;
  logic                                         debug_mode_start;
  logic                                         debug_mode_start_ff;
  logic                                         debug_mode_end;
  /* verilator lint_off UNOPTFLAT */  // dbg_clear_f7 is intentionally fed back into the F6/F7 control path exactly as in the original RTL and only looks cyclic after flattening.
  logic                                         dbg_clear_f7;
  /* verilator lint_on UNOPTFLAT */
  logic                                         reset_prog_buffer;
  logic                                         reset_debug_ff;

  // gated clock
  logic                                         clock_enable;
  logic                                         clock_gated;
  logic [3:0]                                   debug_gated_clk_ticks_q;

  ////////////////////////////////////////////////////////////////////////////////
  // latches with the buffers
  ////////////////////////////////////////////////////////////////////////////////

  // Instruction data memory with gated-clock low-phase write preview and
  // ungated RF storage clock, matching the original LATCH_P2(clock) +
  // RF(clock_aon).
  // Replaces the original rf_latch_1r_1w_diff_widths plus the external
  // LATCH_P2(clock, f6_buffer_wr_1p, f6_buffer_wr_1p_next) helper.
  prim_rf_1r1w_diff_preview #(
    .RWidth     (32),
    .RAlignment (16),
    .WWidth     (FeFetchReadSize),   // 256
    .Entries    (FeFetchBuffers),     // 2
    .PreviewClkSameAsRf (1'b0)
  ) u_inst_data (
    .preview_clk_i        (clock_gated),
    .rf_clk_i             (clk_i),
    .wr_data_en_1p_next_i (f6_buffer_wr_1p_next),
    .rd_data_o            (f6_buffer_inst_data),
    .rd_addr_i            (f6_buffer_ptr),
    .wr_data_i            (f6_wr_data),
    .wr_addr_i            (f6_write_ad),
    .wr_en_i              (f6_buffer_wren)
  );

  // debug_ffb_en/exec FFs:
  // RST_EN_FF(clock, reset_debug, ffb_ff_en_2, debug_ffb_en_2, debug_ffb_en, '0)
  // RST_EN_FF(clock, reset_debug, ffb_ff_en_2, debug_ffb_exec_2, debug_ffb_exec, 1'b0)
  logic ffb_ff_en_2;
  assign ffb_ff_en_2 = (|debug_ffb_en_2) | (|debug_ffb_en_i) | debug_ffb_exec_2 | debug_ffb_exec_i;

  // `reset_debug_i` is the original RST_EN_FF reset argument for this
  // debug-program-buffer staging pair. It is a synchronous local clear on the
  // gated frontend clock, not the thread-buffer `reset` domain.
  always_ff @(posedge clock_gated) begin
    if (reset_debug_i) begin
      debug_ffb_en_2   <= '0;
      debug_ffb_exec_2 <= 1'b0;
    end else if (ffb_ff_en_2) begin
      debug_ffb_en_2   <= debug_ffb_en_i;
      debug_ffb_exec_2 <= debug_ffb_exec_i;
    end
  end

  // mux wr data between icache and debug, and word wr enable
  /* verilator lint_off UNOPTFLAT */  // The F6 pointer/data path intentionally keeps the original combinational fetch advance loop
  always_comb begin
    for (int i = 0; i < FeFetchBuffers; i++)
      buffer_word_en[i] = {(FeFetchReadSize/32){1'b1}} << buffer_pc[i][2+:$clog2(FeFetchReadSize/32)];

    f6_buffer_wr_1p_next = f5_req_valid_to_f6 && !f5_icache_resp_miss_i ? buffer_word_en[f5_req_buffer] : '0;

    if (using_pfb) begin
      f6_buffer_wr_1p_next = (debug_mode_start || reset_debug_i) ? 8'hff : (FeFetchReadSize/32)'(debug_ffb_en_i);
      f6_buffer_wren = (FeFetchReadSize/32)'(debug_ffb_en_2);
      f6_write_ad = f6_req_buffer;
      if (in_debug_mode_i || reset_prog_buffer)
        f6_write_ad = '0;
      // When we trap to debug mode, fill the buffer with ebreaks so the break is implicit
      if (reset_prog_buffer)
        f6_buffer_wren = 8'hff;
    end
    else begin
      f6_buffer_wren = f6_buffer_wr ? buffer_word_en[f6_req_buffer] : '0;
      f6_write_ad = f6_req_buffer;
    end

    f6_wr_data = f6_icache_resp_i.data;
    // On debug mode, store APB data in to the buffer
    if (using_pfb) begin
      f6_wr_data[(64*0)+:64] = debug_ffb_wdata_i;
      f6_wr_data[(64*1)+:32] = debug_ffb_wdata_i[31:0];
      f6_wr_data[(64*1+32)+:32] = debug_ffb_wdata_i[31:0];
      if (reset_prog_buffer) begin
        for (int i = 0; i < 5; ++i)
          f6_wr_data[32*i+:32] = 32'h00100073;  // EBREAK
        for (int i = 5; i < (FeFetchReadSize/32); ++i)
          f6_wr_data[32*i+:2] = 2'b11; // Reset the lsb to avoid X propagation when checking for compressed instructions
      end
    end
  end

  ////////////////////////////////////////////////////////////////////////////////
  // Controls for clock gating
  ////////////////////////////////////////////////////////////////////////////////
  logic icache_inflight;
  logic icache_inflight_next;

  always_comb begin
    icache_inflight_next = icache_inflight;
    if (f0_icache_req_valid_o) icache_inflight_next = 1'b1;
    else if (f0_core_req_valid_i || f0_icache_req_kill_f1 || f6_req_valid)
      icache_inflight_next = 1'b0;

    clock_enable = ( f0_core_req_valid_i                                              // request from core
                   || f0_enable_thread_i                                              // Thread is being enabled
                   || f0_icache_req_acc                                               // Sending a request to icache
                   || icache_inflight                                                 // pending req to the icache
                   || f0_thread_enabled_i && !f0_miss_pending && |buffer_empty         // buffers not full => there might be new request to icache
                   || f6_valid && !f6_stall                                           // pipe advances
                   || f7_valid && !f7_stall                                           // pipe advances
                   || !rst_ni || reset_debug_i                                        // not resetting
                   || in_debug_mode_i                                                 // in debug mode, always on
                   || io_halt_i                                                       // must send valid rsp to intpipe to trigger a halt interrupt
                   || chicken_bit_i);
  end

  // RST_FF(clock_aon, reset, icache_inflight, icache_inflight_next, 1'b0)
  always_ff @(posedge clk_i or negedge rst_ni)
    if (!rst_ni) icache_inflight <= 1'b0;
    else         icache_inflight <= icache_inflight_next;

  // et_clk_gate ck_gate(.enclk(clock), .en(clock_enable), .clk(clock_aon), .te(1'b0));
  prim_clk_gate u_clk_gate (
    .clk_i  (clk_i),
    .en_i   (clock_enable),
    .dft_i  (dft_i),
    .clk_o  (clock_gated)
  );

  // Debug-only observation of gated-domain activity.
  always_ff @(posedge clock_gated or negedge rst_ni)
    if (!rst_ni) debug_gated_clk_ticks_q <= '0;
    else         debug_gated_clk_ticks_q <= debug_gated_clk_ticks_q + 4'd1;

  ////////////////////////////////////////////////////////////////////////////////
  // F0 Stage
  ////////////////////////////////////////////////////////////////////////////////

  // EN_FF(clock, f0_pc_update, f0_pc, f0_pc_next)
  always_ff @(posedge clock_gated)
    if (f0_pc_update) f0_pc <= f0_pc_next;

  // RST_FF(clock_aon, reset, f0_miss_pending, f0_miss_pending_next, 1'b0)
  always_ff @(posedge clk_i or negedge rst_ni)
    if (!rst_ni) f0_miss_pending <= 1'b0;
    else         f0_miss_pending <= f0_miss_pending_next;

  always_comb begin

    // Sends valid request when
    f0_new_inst_req = f0_thread_enabled_i         // Thread is enabled
                      && !f0_enable_thread_i      // Do not request during the cycle the thread is enabled
                      && !(f0_miss_pending && !f7_icache_fill_done_i) // There's no miss pending
                      && !f0_core_req_valid_i     // Core is not having a mispredict
                      && !f1_req_valid            // No request in-flight already
                      && !f2_req_valid
                      && !f3_req_valid
                      && !f4_req_valid
                      && !f5_req_valid
                      && !f6_req_valid
                      && ( (|buffer_empty) || (|f6_clear_buffer) ) // Buffer available
                      && rst_ni                   // Wait until we are out of reset
                      && !using_pfb;              // buffer contents are the 'program fetch buffer' for debug

    f0_icache_req_valid_o = f0_new_inst_req && !io_halt_i; // don't send requests to the ICACHE if we are halting

    // Sends the request to the Icache
    f0_icache_req_o.thread_id = ThreadId[0:0];
    f0_icache_req_o.vm_status = vm_status_i;
    f0_icache_req_o.addr      = f0_pc;

    // Selects which is the request buffer that will be used to store the insts
    f0_req_buffer = '0;
    for (int unsigned i = 0; i < FeFetchBuffers; i++)
      if (buffer_empty[i] ||                          // buffer is empty
          f6_clear_buffer[i] && !f6_buffer_en[i])     // buffer is about to be empty
        f0_req_buffer = i[0:0];

    // Generates the next PC
    f0_pc_next = VaSizeExt'($signed(f0_reset_vector_i));
    // Update to the PC when data coming back from icache
    if (f6_buffer_wr) begin
      f0_pc_next = '0;
      f0_pc_next[VaSizeExt-1:FeFetchReadLsb] = buffer_pc[f6_req_buffer][VaSizeExt-1:FeFetchReadLsb] + 1'b1;
    end
    // If core has a mispredict, get this PC (highest prio)
    if (f0_core_req_valid_i)
      f0_pc_next = f0_core_req_i.pc;

    // Update PC when
    f0_pc_update = !rst_ni                // Doing reset
                   || f0_enable_thread_i  // Thread is being enabled
                   || f6_buffer_wr        // New data coming
                   || f0_core_req_valid_i; // Mispredict

    // Updates the miss pending
    f0_miss_pending_next = f0_miss_pending;
    // Received a miss of a non-killed request, then set the bit
    if (f6_req_valid && f6_icache_resp_miss_i && !f0_core_req_valid_i)
      f0_miss_pending_next = 1'b1;
    // Upon mispredict, thread disable or fill done, clear the pending miss bit
    if (f0_core_req_valid_i || f0_disable_thread_i || f7_icache_fill_done_i || io_halt_i)
      f0_miss_pending_next = 1'b0;
  end

  always_comb begin
    // Request is accepted
    f0_icache_req_acc = (f0_icache_req_valid_o && f0_icache_req_ready_i) || (f0_new_inst_req && io_halt_i);

    // Update PC for buffer
    for (int i = 0; i < FeFetchBuffers; i++)
      f0_buffer_pc_en[i] = f0_icache_req_acc && (i[0:0] == f0_req_buffer);
  end

  ////////////////////////////////////////////////////////////////////////////////
  // F1 Stage
  ////////////////////////////////////////////////////////////////////////////////

  // RST_EN_FF(clock, reset, f1_icache_req_ready, f1_req_valid, f0_icache_req_acc, 1'b0)
  always_ff @(posedge clock_gated or negedge rst_ni)
    if (!rst_ni)                    f1_req_valid <= 1'b0;
    else if (f1_icache_req_ready_i) f1_req_valid <= f0_icache_req_acc;

  // EN_FF(clock, f1_icache_req_ready, f1_req_buffer, f0_req_buffer)
  always_ff @(posedge clock_gated)
    if (f1_icache_req_ready_i) f1_req_buffer <= f0_req_buffer;

  // FF(clock, f1_icache_req_kill_f1, f1_icache_req_ready ? 1'b0 : f0_icache_req_kill_f1)
  always_ff @(posedge clock_gated)
    f1_icache_req_kill_f1 <= f1_icache_req_ready_i ? 1'b0 : f0_icache_req_kill_f1;

  always_comb begin
    f0_icache_req_kill_f1 = (f1_req_valid && f0_core_req_valid_i) || f1_icache_req_kill_f1;
    f1_req_valid_to_f2    = f1_req_valid && f1_icache_req_ready_i && !f0_icache_req_kill_f1;
  end

  ////////////////////////////////////////////////////////////////////////////////
  // F2 Stage
  ////////////////////////////////////////////////////////////////////////////////

  // RST_FF(clock, reset, f2_req_valid, f1_req_valid_to_f2, 1'b0)
  always_ff @(posedge clock_gated or negedge rst_ni)
    if (!rst_ni) f2_req_valid <= 1'b0;
    else         f2_req_valid <= f1_req_valid_to_f2;

  // FF(clock, f2_req_buffer, f1_req_buffer)
  always_ff @(posedge clock_gated)
    f2_req_buffer <= f1_req_buffer;

  assign f2_req_valid_to_f3 = f2_req_valid && !f0_core_req_valid_i;

  ////////////////////////////////////////////////////////////////////////////////
  // F3 Stage
  ////////////////////////////////////////////////////////////////////////////////

  // RST_FF(clock, reset, f3_req_valid, f2_req_valid_to_f3, 1'b0)
  always_ff @(posedge clock_gated or negedge rst_ni)
    if (!rst_ni) f3_req_valid <= 1'b0;
    else         f3_req_valid <= f2_req_valid_to_f3;

  // FF(clock, f3_req_buffer, f2_req_buffer)
  always_ff @(posedge clock_gated)
    f3_req_buffer <= f2_req_buffer;

  assign f3_req_valid_to_f4 = f3_req_valid && !f0_core_req_valid_i;

  ////////////////////////////////////////////////////////////////////////////////
  // F4 Stage
  ////////////////////////////////////////////////////////////////////////////////

  // RST_FF(clock, reset, f4_req_valid, f3_req_valid_to_f4, 1'b0)
  always_ff @(posedge clock_gated or negedge rst_ni)
    if (!rst_ni) f4_req_valid <= 1'b0;
    else         f4_req_valid <= f3_req_valid_to_f4;

  // FF(clock, f4_req_buffer, f3_req_buffer)
  always_ff @(posedge clock_gated)
    f4_req_buffer <= f3_req_buffer;

  assign f4_req_valid_to_f5 = f4_req_valid && !f0_core_req_valid_i;

  ////////////////////////////////////////////////////////////////////////////////
  // F5 Stage
  ////////////////////////////////////////////////////////////////////////////////

  // RST_FF(clock, reset, f5_req_valid, f4_req_valid_to_f5, 1'b0)
  always_ff @(posedge clock_gated or negedge rst_ni)
    if (!rst_ni) f5_req_valid <= 1'b0;
    else         f5_req_valid <= f4_req_valid_to_f5;

  // FF(clock, f5_req_buffer, f4_req_buffer)
  always_ff @(posedge clock_gated)
    f5_req_buffer <= f4_req_buffer;

  assign f5_req_valid_to_f6 = f5_req_valid && !f0_core_req_valid_i;

  ////////////////////////////////////////////////////////////////////////////////
  // F6 Stage - writing
  ////////////////////////////////////////////////////////////////////////////////

  // RST_FF(clock, reset, f6_req_valid, f5_req_valid_to_f6, 1'b0)
  always_ff @(posedge clock_gated or negedge rst_ni)
    if (!rst_ni) f6_req_valid <= 1'b0;
    else         f6_req_valid <= f5_req_valid_to_f6;

  // FF(clock, f6_req_buffer, f5_req_buffer)
  always_ff @(posedge clock_gated)
    f6_req_buffer <= f5_req_buffer;

  always_comb begin
    f6_buffer_wr      = f6_req_valid && !f0_core_req_valid_i && ((f6_icache_resp_valid_i && !f6_icache_resp_miss_i) || io_halt_i);
    f6_req_resp_valid = f6_req_valid && !f0_core_req_valid_i && ((f6_icache_resp_valid_i) || io_halt_i);

    // Generates the enable
    for (int i = 0; i < FeFetchBuffers; i++) begin
      f6_buffer_en[i] = f6_buffer_wr && (i[0:0] == f6_req_buffer);
      f6_fault_en[i]  = f6_req_resp_valid && (i[0:0] == f6_req_buffer);
    end
  end

  // Per-buffer fault/cacheable/PC tracking
  for (genvar buffer = 0; buffer < FeFetchBuffers; buffer++) begin : gen_buf_ff
    // EN_FF(clock, f6_fault_en[buffer], buffer_page_fault[buffer], f6_icache_resp.page_fault && !io_halt)
    always_ff @(posedge clock_gated)
      if (f6_fault_en[buffer]) buffer_page_fault[buffer] <= f6_icache_resp_i.page_fault && !io_halt_i;

    // EN_FF(clock, f6_fault_en[buffer], buffer_access_fault[buffer], f6_icache_resp.access_fault && !io_halt)
    always_ff @(posedge clock_gated)
      if (f6_fault_en[buffer]) buffer_access_fault[buffer] <= f6_icache_resp_i.access_fault && !io_halt_i;

    // EN_FF(clock, f6_fault_en[buffer], buffer_l2_err[buffer], f6_icache_resp.bus_err && !io_halt)
    always_ff @(posedge clock_gated)
      if (f6_fault_en[buffer]) buffer_l2_err[buffer] <= f6_icache_resp_i.bus_err && !io_halt_i;

    // EN_FF(clock, f6_fault_en[buffer], buffer_ecc_err[buffer], f6_icache_resp.ecc_err && !io_halt)
    always_ff @(posedge clock_gated)
      if (f6_fault_en[buffer]) buffer_ecc_err[buffer] <= f6_icache_resp_i.ecc_err && !io_halt_i;

    // EN_FF(clock, f6_buffer_en[buffer], buffer_cacheable[buffer], f6_icache_resp.cacheable)
    always_ff @(posedge clock_gated)
      if (f6_buffer_en[buffer]) buffer_cacheable[buffer] <= f6_icache_resp_i.cacheable;

    // EN_FF(clock, f0_buffer_pc_en[buffer], buffer_pc[buffer], f0_pc)
    always_ff @(posedge clock_gated)
      if (f0_buffer_pc_en[buffer]) buffer_pc[buffer] <= f0_pc;
  end

  ////////////////////////////////////////////////////////////////////////////////
  // Computes the buffer available bit based on fetches coming from icache and
  // instruction consumption from core
  ////////////////////////////////////////////////////////////////////////////////

  // RST_FF(clock_aon, reset, buffer_empty, buffer_empty_next, {FeFetchBuffers{1'b1}})
  always_ff @(posedge clk_i or negedge rst_ni)
    if (!rst_ni) buffer_empty <= {FeFetchBuffers{1'b1}};
    else         buffer_empty <= buffer_empty_next;

  always_comb begin
    // Computes next valid bit
    buffer_empty_next = buffer_empty;

    for (int i = 0; i < FeFetchBuffers; i++) begin
      f6_clear_buffer[i] = f6_last_from_buffer && f6_valid && !f6_stall && f6_buffer_id == i[0:0];

      // If last instruction, make buffer available again
      if (f6_clear_buffer[i])
        buffer_empty_next[i] = 1'b1;
      // writing new data => buffer not empty
      else if (f6_buffer_en[i])
        buffer_empty_next[i] = 1'b0;
      // there was a fault.. not writing data to buffer, but set not empty so that the fault is sent to the intpipe
      else if (f6_fault_en[i] && (f6_icache_resp_i.page_fault || f6_icache_resp_i.access_fault || f6_icache_resp_i.ecc_err || f6_icache_resp_i.bus_err))
        buffer_empty_next[i] = 1'b0;
      // Mispredict or disabling thread, makes all buffers available again
      else if (f0_core_req_valid_i || f0_disable_thread_i)
        buffer_empty_next[i] = 1'b1;
    end

    // If we are being halted, send instructions so we can generate an interrupt with a valid PC
    dbg_clear_f7 = 1'b0;
    if (debug_mode_start_ff) begin
      buffer_empty_next = 2'b11;
      dbg_clear_f7 = 1'b1;
    end

    // if writing program fetch buffer, setting empty
    // When we want to execute, we set it full
    if (debug_ffb_exec_2)
      buffer_empty_next = 2'b00;
    else if (|debug_ffb_en_i)
      buffer_empty_next = 2'b01;
  end

  ////////////////////////////////////////////////////////////////////////////////
  // F6 Stage - reading
  ////////////////////////////////////////////////////////////////////////////////
  logic ffb_update_read_pointer;

  // RST_EN_FF(clock, reset,
  //   f6_valid && !f6_stall || &buffer_empty && f5_req_valid || ffb_update_read_pointer,
  //   f6_buffer_ptr,
  //   (ffb_update_read_pointer) ? '0 : f6_buffer_ptr_next,
  //   '0)
  //
  // CRITICAL: f6_buffer_ptr_next is computed BEFORE the ffb_update_read_pointer
  // override.  f6_buffer_id_next always comes from f6_buffer_ptr_next (the
  // normal advance), NOT from the overridden FF value.
  always_ff @(posedge clock_gated or negedge rst_ni)
    if (!rst_ni)
      f6_buffer_ptr <= '0;
    else if (f6_valid && !f6_stall || &buffer_empty && f5_req_valid || ffb_update_read_pointer)
      f6_buffer_ptr <= (ffb_update_read_pointer) ? '0 : f6_buffer_ptr_next;

  assign ffb_update_read_pointer = debug_ffb_exec_i;

  always_comb begin
    f6_inst_bits = f6_buffer_inst_data;

    {f6_buffer_id, f6_buffer_pos} = f6_buffer_ptr;

    if (&buffer_empty && f5_req_valid) begin
      f6_buffer_ptr_next = {f5_req_buffer, buffer_pc[f5_req_buffer][FeBufferPosSz:1]};
    end
    else begin
      f6_buffer_ptr_next = f6_buffer_ptr + (f6_inst_rvc ? FeFetchPtrSize'(1) : FeFetchPtrSize'(2));
    end
    f6_buffer_id_next = f6_buffer_ptr_next[FeFetchPtrSize-1:FeBufferPosSz];

    // is data from 2 buffers required? => 32b instruction, at read_pos=all 1's
    f6_split = !f6_inst_rvc && &f6_buffer_pos;

    // determine valid
    if (!f6_split) begin
      f6_valid = !buffer_empty[f6_buffer_id];
      // maybe bypass data directly from icache
      if (!f6_valid && f6_buffer_wr) begin
        f6_valid = 1'b1;
        f6_inst_bits = f6_icache_resp_i.data[f6_buffer_pos*16 +: 32];
      end
    end
    else begin
      f6_valid = !buffer_empty[f6_buffer_id] && !buffer_empty[f6_buffer_id_next];
      // maybe bypass data directly from icache
      if (!f6_valid && !buffer_empty[f6_buffer_id] && buffer_empty[f6_buffer_id_next] && f6_buffer_wr) begin
        f6_valid = 1'b1;
        f6_inst_bits[31:16] = f6_icache_resp_i.data[15:0];
      end
    end
    f6_valid = f6_valid && !dbg_clear_f7;

    f6_last_from_buffer = (f6_buffer_id_next != f6_buffer_id);

    f6_inst_pc_msb = buffer_pc[f6_buffer_id][VaSizeExt-1:FeFetchReadLsb];
  end
  /* verilator lint_on UNOPTFLAT */

  // instruction is compressed => take either from buffer, or from icache data if bypassing
  // in case of halt, force it to a non rvc instruction
  /* verilator lint_off CASEX */
  assign f6_inst_rvc = (!io_halt_i && (!buffer_empty[f6_buffer_id] ? f6_buffer_inst_data[1:0] : f6_icache_resp_i.data[f6_buffer_pos*16 +: 2]) != 2'b11);
  /* verilator lint_on CASEX */

  // RVC expander
  minion_frontend_rvc_expander u_rvc_exp (
    // Input data
    .in_bits  (f6_inst_bits),
    // Expanded output data
    .out_bits (f6_inst_bits_expanded)
  );

  ////////////////////////////////////////////////////////////////////////////////
  // F7 Stage
  ////////////////////////////////////////////////////////////////////////////////

  // RST_FF(clock, reset, f7_valid, (f0_core_req_valid || dbg_clear_f7) ? 1'b0 : f6_valid ? 1'b1 : f7_inst_ready ? 1'b0 : f7_valid, 1'b0)
  always_ff @(posedge clock_gated or negedge rst_ni)
    if (!rst_ni) f7_valid <= 1'b0;
    else         f7_valid <= (f0_core_req_valid_i || dbg_clear_f7) ? 1'b0 : f6_valid ? 1'b1 : f7_inst_ready_i ? 1'b0 : f7_valid;

  // EN_FF(clock, f6_valid && !f6_stall, f7_inst_bits, f6_inst_bits_expanded)
  always_ff @(posedge clock_gated)
    if (f6_valid && !f6_stall) f7_inst_bits <= f6_inst_bits_expanded;

  // EN_FF(clock, f6_valid && !f6_stall, f7_inst_rvc, f6_inst_rvc)
  logic f7_inst_rvc;
  always_ff @(posedge clock_gated)
    if (f6_valid && !f6_stall) f7_inst_rvc <= f6_inst_rvc;

  // EN_FF(clock, f6_valid && !f6_stall, f7_buffer_ptr, f6_buffer_ptr)
  always_ff @(posedge clock_gated)
    if (f6_valid && !f6_stall) f7_buffer_ptr <= f6_buffer_ptr;

  // EN_FF(clock, f6_valid && !f6_stall, f7_split, f6_split)
  always_ff @(posedge clock_gated)
    if (f6_valid && !f6_stall) f7_split <= f6_split;

  // EN_FF(clock, f6_valid && !f6_stall, f7_inst_pc_msb, f6_inst_pc_msb)
  always_ff @(posedge clock_gated)
    if (f6_valid && !f6_stall) f7_inst_pc_msb <= f6_inst_pc_msb;

  always_comb begin
    f7_stall = !f7_inst_ready_i;
    f6_stall = f7_valid && f7_stall;

    // fill structure to next stage
    {f7_buffer_id, f7_buffer_pos} = f7_buffer_ptr;

    f7_inst_data_o.pc = {f7_inst_pc_msb, f7_buffer_pos, 1'b0};

    // Exception from TLB and replay
    f7_inst_data_o.page_fault0   = buffer_page_fault[f7_buffer_id] && !using_pfb_reg;
    f7_inst_data_o.access_fault0 = buffer_access_fault[f7_buffer_id] && !using_pfb_reg;
    // page faults in case instruction has data from 2 buffers
    if (!f7_inst_rvc && f7_split) begin
      f7_inst_data_o.page_fault1   = buffer_page_fault[f6_buffer_id] && !using_pfb_reg;
      f7_inst_data_o.access_fault1 = buffer_access_fault[f6_buffer_id] && !using_pfb_reg;
      // output errors => or of both buffers if instruction is half in each buffer
      f7_inst_data_o.bus_err  = |buffer_l2_err && !using_pfb_reg;
      f7_inst_data_o.ecc_err  = |buffer_ecc_err && !using_pfb_reg;
    end
    else begin
      f7_inst_data_o.page_fault1   = 1'b0;
      f7_inst_data_o.access_fault1 = 1'b0;
      // and output errors
      f7_inst_data_o.bus_err  = buffer_l2_err[f7_buffer_id] && !using_pfb_reg;
      f7_inst_data_o.ecc_err  = buffer_ecc_err[f7_buffer_id] && !using_pfb_reg;
    end

    f7_inst_data_o.replay = !buffer_cacheable[f7_buffer_id] && f0_core_req_i.speculative; // Replay if not cacheable and speculative instruction

    if (using_pfb) begin
      f7_inst_data_o.page_fault0   = '0;
      f7_inst_data_o.page_fault1   = '0;
      f7_inst_data_o.access_fault0 = '0;
      f7_inst_data_o.access_fault1 = '0;
      f7_inst_data_o.bus_err  = '0;
      f7_inst_data_o.ecc_err  = '0;
      f7_inst_data_o.replay   = '0;
    end

    f7_inst_data_o.inst_bits = f7_inst_bits;

    f7_inst_valid_o = f7_valid && !dbg_clear_f7;
  end

  // f7_inst_rvc_o output
  assign f7_inst_rvc_o = f7_inst_rvc;

  ////////////////////////////////////////////////////////////////////////////////
  // DEBUG control
  ////////////////////////////////////////////////////////////////////////////////
  logic using_pfb_en;

  // FF(clock, reset_debug_ff, reset_debug)
  always_ff @(posedge clock_gated)
    reset_debug_ff <= reset_debug_i;

  // RST_EN_FF(clock, reset, using_pfb_en, using_pfb_reg, using_pfb_next, 1'b0)
  always_ff @(posedge clock_gated or negedge rst_ni)
    if (!rst_ni)            using_pfb_reg <= 1'b0;
    else if (using_pfb_en)  using_pfb_reg <= using_pfb_next;

  // FF(clock, debug_mode_start_ff, debug_mode_start)
  always_ff @(posedge clock_gated)
    debug_mode_start_ff <= debug_mode_start;

  assign debug_mode_start = f0_core_req_i.debug_info.halt && f0_core_req_valid_i;
  assign debug_mode_end   = f0_core_req_i.debug_info.resume && f0_core_req_valid_i;

  always_comb begin
    // if writing program fetch buffer from APB, update read_id and pos_id (will be reset to 0)
    using_pfb_en = debug_mode_start || debug_mode_end;

    using_pfb_next = using_pfb_reg;
    if (debug_mode_start)
      using_pfb_next = 1'b1;
    else if (debug_mode_end)
      using_pfb_next = 1'b0;
  end

  assign using_pfb = using_pfb_reg || using_pfb_next;

  assign reset_prog_buffer = (in_debug_mode_i && reset_debug_ff) || debug_mode_start_ff;

  always_comb begin
    debug_o.core_req_valid   = f0_core_req_valid_i;
    debug_o.core_req_pc      = f0_core_req_i.pc;
    debug_o.f0_pc           = f0_pc;
    debug_o.f0_pc_next      = f0_pc_next;
    debug_o.f0_pc_update    = f0_pc_update;
    debug_o.clock_enable    = clock_enable;
    debug_o.gated_clk_ticks = debug_gated_clk_ticks_q;
    debug_o.buffer_empty    = buffer_empty;
    debug_o.buffer_pc0      = buffer_pc[0];
    debug_o.buffer_pc1      = buffer_pc[1];
    debug_o.req_buffer      = f0_req_buffer;
    debug_o.f6_req_buffer   = f6_req_buffer;
    debug_o.f6_buffer_id    = f6_buffer_id;
    debug_o.f7_buffer_id    = f7_buffer_id;
    debug_o.f6_buffer_ptr   = f6_buffer_ptr;
    debug_o.f6_buffer_ptr_next = f6_buffer_ptr_next;
    debug_o.f7_buffer_ptr   = f7_buffer_ptr;
    debug_o.icache_inflight = icache_inflight;
    debug_o.miss_pending    = f0_miss_pending;
    debug_o.f1_req_valid    = f1_req_valid;
    debug_o.f5_req_valid    = f5_req_valid;
    debug_o.f6_req_valid    = f6_req_valid;
    debug_o.f6_buffer_wr    = f6_buffer_wr;
    debug_o.f6_valid        = f6_valid;
    debug_o.f6_stall        = f6_stall;
    debug_o.f7_valid        = f7_valid;
    debug_o.f7_stall        = f7_stall;
  end

  ////////////////////////////////////////////////////////////////////////////////
  // Asserts
  ////////////////////////////////////////////////////////////////////////////////

  // synopsys translate_off
  // synopsys translate_on

endmodule
