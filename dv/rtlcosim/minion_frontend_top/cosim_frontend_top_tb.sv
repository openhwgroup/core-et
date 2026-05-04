// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation testbench: minion_frontend vs frontend_top.
//
// Drives identical stimulus to both modules and exposes their outputs
// for cycle-by-cycle comparison in C++. The original uses active-high
// synchronous reset; the new uses active-low async reset.

`include "soc.vh"

module cosim_frontend_top_tb
  import minion_pkg::*;
  import minion_frontend_pkg::*;
  import dft_pkg::*;
(
  input  logic        clk_i,
  input  logic        rst_ni,

  // ── Stimulus inputs ──────────────────────────────────

  input  logic [1:0]              f0_thread_enabled_i,
  input  logic [47:0]             f0_reset_vector_i,       // VA_SIZE=48
  input  logic [15:0]             vm_status_i,             // packed [1:0] vm_status (8 bits each)

  // ICache request handshake
  input  logic                    f1_icache_req_ready_i,

  // ICache response (F5 stage)
  input  logic                    f5_icache_resp_valid_i,
  input  logic                    f5_icache_resp_miss_i,
  input  logic [260:0]            f5_icache_resp_i,        // packed icache_fe_resp_t / icache_frontend_resp
  input  logic                    f6_icache_fill_done_i,

  // Core redirect / mispredict (per thread)
  input  logic [1:0]              f0_core_req_valid_i,
  input  logic [103:0]            f0_core_req_i,           // packed [1:0] fe_req_t (52 bits each)

  // Core stall
  input  logic [1:0]              id_core_stall_i,

  // Instruction consumer ready
  input  logic                    id_inst_ready_i,

  // Debug
  input  logic                    reset_debug_i,

  // Debug program fetch buffer
  input  logic [63:0]             debug_ffb_wdata_i,
  input  logic [3:0]              debug_ffb_en_i,
  input  logic                    debug_ffb_thread_sel_i,
  input  logic [1:0]              debug_ffb_exec_i,
  input  logic [1:0]              halt_i,
  input  logic [1:0]              halted_i,

  // ── New module outputs (flattened) ───────────────────

  output logic                    new_f1_icache_req_valid_o,
  output logic [57:0]             new_f1_icache_req_o,     // $bits(fe_icache_req_t)
  output logic                    new_id_inst_valid_o,
  output logic                    new_id_inst_thread_id_o,
  output logic [88:0]             new_id_inst_data_o,      // $bits(fe_core_resp_t)
  output logic [44:0]             new_id_intpipe_ctrl_o,   // $bits(minion_control_t)
  output logic [44:0]             new_id_vpu_sigs_o,       // $bits(vpu_ctrl_sigs_t)

  // ── Original module outputs (flattened) ──────────────

  output logic                    orig_f1_icache_req_valid_o,
  output logic [57:0]             orig_f1_icache_req_o,
  output logic                    orig_id_inst_valid_o,
  output logic                    orig_id_inst_thread_id_o,
  output logic [88:0]             orig_id_inst_data_o,
  output logic [44:0]             orig_id_intpipe_ctrl_o,
  output logic [44:0]             orig_id_vpu_sigs_o,

  // Debug: thread buffer probes
  output logic [1:0]              new_tb_inst_valid_o,
  output logic [1:0]              orig_tb_inst_valid_o,
  output logic [31:0]             new_tb_inst_bits_0_o,
  output logic [31:0]             new_tb_inst_bits_1_o,
  output logic [31:0]             orig_tb_inst_bits_0_o,
  output logic [31:0]             orig_tb_inst_bits_1_o,

  // Internal state probes
  output logic [4:0]              new_tb0_rd_ptr_o,
  output logic [4:0]              orig_tb0_rd_ptr_o,
  output logic [1:0]              new_tb0_buf_empty_o,
  output logic [1:0]              orig_tb0_buf_empty_o,
  output logic                    new_tb0_f6_valid_o,
  output logic                    orig_tb0_f6_valid_o,
  output logic                    new_tb0_f6_wr_o,
  output logic                    orig_tb0_f6_wr_o,
  output logic [7:0]              new_tb0_wr_en_o,
  output logic [7:0]              orig_tb0_wr_en_o,
  output logic                    new_tb0_wr_buf_o,
  output logic                    orig_tb0_wr_buf_o,
  output logic                    new_tb0_clock_enable_o,
  output logic                    orig_tb0_clock_enable_o,
  output logic                    new_tb0_f4_req_buffer_o,
  output logic                    orig_tb0_f4_req_buffer_o,
  output logic                    new_tb0_f5_req_buffer_o,
  output logic                    orig_tb0_f5_req_buffer_o,
  output logic                    new_tb0_f6_req_buffer_o,
  output logic                    orig_tb0_f6_req_buffer_o,
  output logic                    new_tb0_f5_req_valid_o,
  output logic                    orig_tb0_f5_req_valid_o,
  output logic                    new_tb0_f6_req_valid_o,
  output logic                    orig_tb0_f6_req_valid_o,
  output logic                    new_tb0_f6_inst_rvc_o,
  output logic                    orig_tb0_f6_inst_rvc_o,
  output logic [4:0]              new_tb0_f6_buffer_ptr_next_o,
  output logic [4:0]              orig_tb0_f6_buffer_ptr_next_o,
  output logic                    new_tb0_f6_buffer_id_o,
  output logic                    orig_tb0_f6_buffer_id_o,
  output logic                    new_tb0_f6_buffer_id_next_o,
  output logic                    orig_tb0_f6_buffer_id_next_o,
  output logic                    new_tb0_f6_last_from_buffer_o,
  output logic                    orig_tb0_f6_last_from_buffer_o,
  output logic                    new_tb0_f6_split_o,
  output logic                    orig_tb0_f6_split_o,
  output logic                    new_tb0_clock_gated_o,
  output logic                    orig_tb0_clock_gated_o,
  output logic [7:0]              new_tb0_f6_buffer_wr_1p_next_o,
  output logic [7:0]              orig_tb0_f6_buffer_wr_1p_next_o,
  output logic [7:0]              new_tb0_f6_buffer_wr_1p_o,
  output logic [7:0]              orig_tb0_f6_buffer_wr_1p_o,
  output logic [31:0]             new_tb0_f6_buffer_inst_data_o,
  output logic [31:0]             orig_tb0_f6_buffer_inst_data_o,
  output logic                    new_tb0_f6_stall_o,
  output logic                    orig_tb0_f6_stall_o,
  output logic                    new_tb0_dbg_clear_f7_o,
  output logic                    orig_tb0_dbg_clear_f7_o,
  output logic                    new_tb0_ffb_update_read_pointer_o,
  output logic                    orig_tb0_ffb_update_read_pointer_o,

  // Thread 1 probes
  output logic [4:0]              new_tb1_rd_ptr_o,
  output logic [4:0]              orig_tb1_rd_ptr_o,
  output logic [1:0]              new_tb1_buf_empty_o,
  output logic [1:0]              orig_tb1_buf_empty_o,
  output logic                    new_tb1_using_pfb_o,
  output logic                    orig_tb1_using_pfb_o,
  output logic                    new_tb1_dbg_start_ff_o,
  output logic                    orig_tb1_dbg_start_ff_o
);

  // Reset inversion: original uses active-high synchronous.
  logic reset_orig;
  assign reset_orig = ~rst_ni;

  // ════════════════════════════════════════════════════════
  // New module: minion_frontend
  // ════════════════════════════════════════════════════════

  vm_status_t [1:0]  new_vm_status;
  fe_req_t    [1:0]  new_core_req;
  fe_icache_req_t    new_icache_req;
  icache_fe_resp_t   new_icache_resp;
  fe_core_resp_t     new_inst_data;
  minion_control_t   new_intpipe_ctrl;
  vpu_ctrl_sigs_t    new_vpu_sigs;

  assign new_vm_status   = vm_status_i;
  assign new_icache_resp = f5_icache_resp_i;
  assign new_core_req    = f0_core_req_i;

  vpu_id_ctrl_t      new_vpu_core_ctrl;

  minion_frontend #(.NrThreads(2)) u_new (
    .clk_i,
    .rst_ni,
    .reset_debug_i       (reset_debug_i),
    .chicken_bit_i       (1'b0),
    .dft_i               ('0),
    .f0_thread_enabled_i,
    .f0_reset_vector_i,
    .vm_status_i         (new_vm_status),
    .f1_icache_req_ready_i,
    .f1_icache_req_valid_o (new_f1_icache_req_valid_o),
    .f1_icache_req_o     (new_icache_req),
    .f5_icache_resp_valid_i,
    .f5_icache_resp_miss_i,
    .f5_icache_resp_i    (new_icache_resp),
    .f6_icache_fill_done_i,
    .f0_core_req_valid_i,
    .f0_core_req_i       (new_core_req),
    .id_core_stall_i,
    .id_inst_ready_i,
    .id_inst_valid_o     (new_id_inst_valid_o),
    .id_inst_thread_id_o (new_id_inst_thread_id_o),
    .id_inst_data_o      (new_inst_data),
    .id_intpipe_ctrl_o   (new_intpipe_ctrl),
    .id_vpu_decoder_sigs_o (new_vpu_sigs),
    .id_vpu_core_ctrl_o  (new_vpu_core_ctrl),
    .debug_thread_o      (),
    .debug_inst_fifo_empty_o(),
    .debug_inst_fifo_full_o (),
    .debug_tb_inst_valid_o  (),
    .debug_tb_inst_ready_o  (),
    .debug_ffb_wdata_i,
    .debug_ffb_en_i,
    .debug_ffb_thread_sel_i,
    .debug_ffb_exec_i,
    .halt_i,
    .halted_i
  );

  assign new_f1_icache_req_o  = new_icache_req;
  assign new_id_inst_data_o   = new_inst_data;
  assign new_id_intpipe_ctrl_o = new_intpipe_ctrl;
  assign new_id_vpu_sigs_o    = new_vpu_sigs;

  // ════════════════════════════════════════════════════════
  // Original module: frontend_top
  // ════════════════════════════════════════════════════════

  frontend_icache_req    orig_icache_req;
  frontend_core_resp     orig_inst_data;
  minion_control         orig_intpipe_ctrl;
  vpu_ctrl_sigs_t        orig_vpu_sigs;

  // id_vpu_core_ctrl is output by original but not by new — not compared.
  vpu_minion_id_ctrl     orig_vpu_core_ctrl;

  frontend_top u_orig (
    .clock               (clk_i),
    .reset               (reset_orig),
    .reset_debug         (reset_debug_i),
    .chicken_bit         (1'b0),
    .f0_thread_enabled   (f0_thread_enabled_i),
    .f0_reset_vector     (f0_reset_vector_i),
    .vm_status           (vm_status_i),
    .f1_icache_req_ready (f1_icache_req_ready_i),
    .f1_icache_req_valid (orig_f1_icache_req_valid_o),
    .f1_icache_req       (orig_icache_req),
    .f5_icache_resp_valid (f5_icache_resp_valid_i),
    .f5_icache_resp_miss  (f5_icache_resp_miss_i),
    .f5_icache_resp      (f5_icache_resp_i),
    .f6_icache_fill_done (f6_icache_fill_done_i),
    .f0_core_req_valid   (f0_core_req_valid_i),
    .f0_core_req         (f0_core_req_i),
    .id_core_stall       (id_core_stall_i),
    .id_inst_ready       (id_inst_ready_i),
    .id_inst_valid       (orig_id_inst_valid_o),
    .id_inst_thread_id   (orig_id_inst_thread_id_o),
    .id_inst_data        (orig_inst_data),
    .id_intpipe_ctrl     (orig_intpipe_ctrl),
    .id_vpu_decoder_sigs (orig_vpu_sigs),
    .id_vpu_core_ctrl    (orig_vpu_core_ctrl),
    .debug_ffb_wdata     (debug_ffb_wdata_i),
    .debug_ffb_en        (debug_ffb_en_i),
    .debug_ffb_thread_sel (debug_ffb_thread_sel_i),
    .debug_ffb_exec      (debug_ffb_exec_i),
    .halt                (halt_i),
    .halted              (halted_i)
  );

  assign orig_f1_icache_req_o  = orig_icache_req;
  assign orig_id_inst_data_o   = orig_inst_data;
  assign orig_id_intpipe_ctrl_o = orig_intpipe_ctrl;
  assign orig_id_vpu_sigs_o    = orig_vpu_sigs;

  // ── Debug: internal state probes ──────────────────
  assign new_tb0_rd_ptr_o    = u_new.gen_thread_buf[0].u_tb.f6_buffer_ptr;
  assign orig_tb0_rd_ptr_o   = u_orig.FRONTEND_THREAD[0].thread_buffer.f6_buffer_ptr;
  assign new_tb0_buf_empty_o = u_new.gen_thread_buf[0].u_tb.buffer_empty;
  assign orig_tb0_buf_empty_o = u_orig.FRONTEND_THREAD[0].thread_buffer.buffer_empty;
  assign new_tb0_f6_valid_o  = u_new.gen_thread_buf[0].u_tb.f6_valid;
  assign orig_tb0_f6_valid_o = u_orig.FRONTEND_THREAD[0].thread_buffer.f6_valid;
  assign new_tb0_f6_wr_o     = u_new.gen_thread_buf[0].u_tb.f6_buffer_wr;
  assign orig_tb0_f6_wr_o    = u_orig.FRONTEND_THREAD[0].thread_buffer.f6_buffer_wr;
  assign new_tb0_wr_en_o     = u_new.gen_thread_buf[0].u_tb.f6_buffer_wren;
  assign orig_tb0_wr_en_o    = u_orig.FRONTEND_THREAD[0].thread_buffer.f6_buffer_wren;
  assign new_tb0_wr_buf_o    = u_new.gen_thread_buf[0].u_tb.f6_write_ad;
  assign orig_tb0_wr_buf_o   = u_orig.FRONTEND_THREAD[0].thread_buffer.f6_write_ad;
  assign new_tb0_clock_enable_o   = u_new.gen_thread_buf[0].u_tb.clock_enable;
  assign orig_tb0_clock_enable_o  = u_orig.FRONTEND_THREAD[0].thread_buffer.clock_enable;
  assign new_tb0_f4_req_buffer_o  = u_new.gen_thread_buf[0].u_tb.f4_req_buffer;
  assign orig_tb0_f4_req_buffer_o = u_orig.FRONTEND_THREAD[0].thread_buffer.f4_req_buffer;
  assign new_tb0_f5_req_buffer_o  = u_new.gen_thread_buf[0].u_tb.f5_req_buffer;
  assign orig_tb0_f5_req_buffer_o = u_orig.FRONTEND_THREAD[0].thread_buffer.f5_req_buffer;
  assign new_tb0_f6_req_buffer_o  = u_new.gen_thread_buf[0].u_tb.f6_req_buffer;
  assign orig_tb0_f6_req_buffer_o = u_orig.FRONTEND_THREAD[0].thread_buffer.f6_req_buffer;
  assign new_tb0_f5_req_valid_o   = u_new.gen_thread_buf[0].u_tb.f5_req_valid;
  assign orig_tb0_f5_req_valid_o  = u_orig.FRONTEND_THREAD[0].thread_buffer.f5_req_valid;
  assign new_tb0_f6_req_valid_o   = u_new.gen_thread_buf[0].u_tb.f6_req_valid;
  assign orig_tb0_f6_req_valid_o  = u_orig.FRONTEND_THREAD[0].thread_buffer.f6_req_valid;
  assign new_tb0_f6_inst_rvc_o    = u_new.gen_thread_buf[0].u_tb.f6_inst_rvc;
  assign orig_tb0_f6_inst_rvc_o   = u_orig.FRONTEND_THREAD[0].thread_buffer.f6_inst_rvc;
  assign new_tb0_f6_buffer_ptr_next_o  = u_new.gen_thread_buf[0].u_tb.f6_buffer_ptr_next;
  assign orig_tb0_f6_buffer_ptr_next_o = u_orig.FRONTEND_THREAD[0].thread_buffer.f6_buffer_ptr_next;
  assign new_tb0_f6_buffer_id_o   = u_new.gen_thread_buf[0].u_tb.f6_buffer_id;
  assign orig_tb0_f6_buffer_id_o  = u_orig.FRONTEND_THREAD[0].thread_buffer.f6_buffer_id;
  assign new_tb0_f6_buffer_id_next_o   = u_new.gen_thread_buf[0].u_tb.f6_buffer_id_next;
  assign orig_tb0_f6_buffer_id_next_o  = u_orig.FRONTEND_THREAD[0].thread_buffer.f6_buffer_id_next;
  assign new_tb0_f6_last_from_buffer_o = u_new.gen_thread_buf[0].u_tb.f6_last_from_buffer;
  assign orig_tb0_f6_last_from_buffer_o = u_orig.FRONTEND_THREAD[0].thread_buffer.f6_last_from_buffer;
  assign new_tb0_f6_split_o       = u_new.gen_thread_buf[0].u_tb.f6_split;
  assign orig_tb0_f6_split_o      = u_orig.FRONTEND_THREAD[0].thread_buffer.f6_split;
  assign new_tb0_clock_gated_o    = u_new.gen_thread_buf[0].u_tb.clock_gated;
  assign orig_tb0_clock_gated_o   = u_orig.FRONTEND_THREAD[0].thread_buffer.clock;
  assign new_tb0_f6_buffer_wr_1p_next_o = u_new.gen_thread_buf[0].u_tb.f6_buffer_wr_1p_next;
  assign orig_tb0_f6_buffer_wr_1p_next_o = u_orig.FRONTEND_THREAD[0].thread_buffer.f6_buffer_wr_1p_next;
  assign new_tb0_f6_buffer_wr_1p_o = u_new.gen_thread_buf[0].u_tb.u_inst_data.wr_data_en_1p_q;
  assign orig_tb0_f6_buffer_wr_1p_o = u_orig.FRONTEND_THREAD[0].thread_buffer.f6_buffer_wr_1p;
  assign new_tb0_f6_buffer_inst_data_o = u_new.gen_thread_buf[0].u_tb.f6_buffer_inst_data;
  assign orig_tb0_f6_buffer_inst_data_o = u_orig.FRONTEND_THREAD[0].thread_buffer.f6_buffer_inst_data;
  assign new_tb0_f6_stall_o       = u_new.gen_thread_buf[0].u_tb.f6_stall;
  assign orig_tb0_f6_stall_o      = u_orig.FRONTEND_THREAD[0].thread_buffer.f6_stall;
  assign new_tb0_dbg_clear_f7_o   = u_new.gen_thread_buf[0].u_tb.dbg_clear_f7;
  assign orig_tb0_dbg_clear_f7_o  = u_orig.FRONTEND_THREAD[0].thread_buffer.dbg_clear_f7;
  assign new_tb0_ffb_update_read_pointer_o = u_new.gen_thread_buf[0].u_tb.ffb_update_read_pointer;
  assign orig_tb0_ffb_update_read_pointer_o = u_orig.FRONTEND_THREAD[0].thread_buffer.ffb_update_read_pointer;

  // Thread 1 probes
  assign new_tb1_rd_ptr_o     = u_new.gen_thread_buf[1].u_tb.f6_buffer_ptr;
  assign orig_tb1_rd_ptr_o    = u_orig.FRONTEND_THREAD[1].thread_buffer.f6_buffer_ptr;
  assign new_tb1_buf_empty_o  = u_new.gen_thread_buf[1].u_tb.buffer_empty;
  assign orig_tb1_buf_empty_o = u_orig.FRONTEND_THREAD[1].thread_buffer.buffer_empty;
  assign new_tb1_using_pfb_o  = u_new.gen_thread_buf[1].u_tb.using_pfb;
  assign orig_tb1_using_pfb_o = u_orig.FRONTEND_THREAD[1].thread_buffer.using_pfb;
  assign new_tb1_dbg_start_ff_o  = u_new.gen_thread_buf[1].u_tb.debug_mode_start_ff;
  assign orig_tb1_dbg_start_ff_o = u_orig.FRONTEND_THREAD[1].thread_buffer.debug_mode_start_ff;

  // ── Debug: thread buffer output probes ──────────────────
  assign new_tb_inst_valid_o[0] = u_new.tb_inst_valid[0];
  assign new_tb_inst_valid_o[1] = u_new.tb_inst_valid[1];
  assign new_tb_inst_bits_0_o   = u_new.tb_inst_data[0].inst_bits;
  assign new_tb_inst_bits_1_o   = u_new.tb_inst_data[1].inst_bits;

  // Original module: access thread buffer outputs via hierarchy
  assign orig_tb_inst_valid_o[0] = u_orig.f7_tb_inst_valid[0];
  assign orig_tb_inst_valid_o[1] = u_orig.f7_tb_inst_valid[1];
  assign orig_tb_inst_bits_0_o   = u_orig.f7_tb_inst_data[0].inst_bits;
  assign orig_tb_inst_bits_1_o   = u_orig.f7_tb_inst_data[1].inst_bits;

endmodule
