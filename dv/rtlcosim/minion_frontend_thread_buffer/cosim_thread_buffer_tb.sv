// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation testbench: minion_frontend_thread_buffer vs
// frontend_thread_buffer.
//
// Drives identical stimulus to both modules. The original uses
// active-high synchronous reset; the new uses active-low async.
// The wrapper handles the reset inversion.

`include "soc.vh"

module cosim_thread_buffer_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  // Stimulus
  input  logic                  f0_thread_enabled_i,
  input  logic [47:0]           f0_reset_vector_i,
  input  logic                  f0_disable_thread_i,
  input  logic                  f0_enable_thread_i,
  input  logic                  in_debug_mode_i,
  input  logic                  reset_debug_i,

  input  logic [7:0]            vm_status_i,       // packed minion_vm_status

  input  logic                  f0_icache_req_ready_i,
  input  logic                  f1_icache_req_ready_i,
  input  logic                  f5_icache_resp_miss_i,
  input  logic                  f6_icache_resp_valid_i,
  input  logic                  f6_icache_resp_miss_i,
  input  logic [260:0]          f6_icache_resp_i,  // packed icache_frontend_resp
  input  logic                  f7_icache_fill_done_i,

  input  logic                  f0_core_req_valid_i,
  input  logic [51:0]           f0_core_req_i,     // packed minion_fe_req

  input  logic                  f7_inst_ready_i,

  input  logic [63:0]           debug_ffb_wdata_i,
  input  logic [3:0]            debug_ffb_en_i,
  input  logic                  debug_ffb_exec_i,
  input  logic                  io_halt_i,

  // New module outputs
  output logic                  new_f0_icache_req_valid_o,
  output logic                  new_f7_inst_rvc_o,
  output logic                  new_f7_inst_valid_o,
  output logic [90:0]           new_f7_inst_data_o, // packed fe_operation_t

  // Original module outputs
  output logic                  orig_f0_icache_req_valid_o,
  output logic                  orig_f7_inst_rvc_o,
  output logic                  orig_f7_inst_valid_o,
  output logic [90:0]           orig_f7_inst_data_o, // packed frontend_operation

  // Internal probes for preview/write-path debugging
  output logic [4:0]            new_f6_buffer_ptr_o,
  output logic [4:0]            orig_f6_buffer_ptr_o,
  output logic [1:0]            new_buffer_empty_o,
  output logic [1:0]            orig_buffer_empty_o,
  output logic                  new_f6_valid_o,
  output logic                  orig_f6_valid_o,
  output logic                  new_f6_buffer_wr_o,
  output logic                  orig_f6_buffer_wr_o,
  output logic [7:0]            new_f6_buffer_wren_o,
  output logic [7:0]            orig_f6_buffer_wren_o,
  output logic                  new_f6_write_ad_o,
  output logic                  orig_f6_write_ad_o,
  output logic                  new_f0_req_buffer_o,
  output logic                  orig_f0_req_buffer_o,
  output logic                  new_f5_req_buffer_o,
  output logic                  orig_f5_req_buffer_o,
  output logic                  new_f6_req_buffer_o,
  output logic                  orig_f6_req_buffer_o,
  output logic                  new_using_pfb_o,
  output logic                  orig_using_pfb_o,
  output logic                  new_clock_enable_o,
  output logic                  orig_clock_enable_o,
  output logic                  new_f4_req_buffer_o,
  output logic                  orig_f4_req_buffer_o,
  output logic                  new_f5_req_valid_o,
  output logic                  orig_f5_req_valid_o,
  output logic                  new_f6_req_valid_o,
  output logic                  orig_f6_req_valid_o,
  output logic                  new_f6_inst_rvc_o,
  output logic                  orig_f6_inst_rvc_o,
  output logic [4:0]            new_f6_buffer_ptr_next_o,
  output logic [4:0]            orig_f6_buffer_ptr_next_o,
  output logic                  new_f6_buffer_id_o,
  output logic                  orig_f6_buffer_id_o,
  output logic                  new_f6_buffer_id_next_o,
  output logic                  orig_f6_buffer_id_next_o,
  output logic                  new_f6_last_from_buffer_o,
  output logic                  orig_f6_last_from_buffer_o,
  output logic                  new_f6_split_o,
  output logic                  orig_f6_split_o,
  output logic                  new_clock_gated_o,
  output logic                  orig_clock_gated_o,
  output logic [7:0]            new_f6_buffer_wr_1p_next_o,
  output logic [7:0]            orig_f6_buffer_wr_1p_next_o,
  output logic [7:0]            new_f6_buffer_wr_1p_o,
  output logic [7:0]            orig_f6_buffer_wr_1p_o,
  output logic [31:0]           new_f6_buffer_inst_data_o,
  output logic [31:0]           orig_f6_buffer_inst_data_o,
  output logic                  new_f6_stall_o,
  output logic                  orig_f6_stall_o,
  output logic                  new_dbg_clear_f7_o,
  output logic                  orig_dbg_clear_f7_o,
  output logic                  new_ffb_update_read_pointer_o,
  output logic                  orig_ffb_update_read_pointer_o,
  output logic [3:0]            new_debug_ffb_en_2_o,
  output logic [3:0]            orig_debug_ffb_en_2_o,
  output logic                  new_debug_ffb_exec_2_o,
  output logic                  orig_debug_ffb_exec_2_o
);

  // Reset inversion: original uses active-high synchronous.
  logic reset_orig;
  assign reset_orig = ~rst_ni;

  // Reset vector: original is 48-bit, new is 49-bit (sign-extended).
  logic [48:0] reset_vector_ext;
  assign reset_vector_ext = {f0_reset_vector_i[47], f0_reset_vector_i};

  // ── New module ────────────────────────────────────────

  import minion_pkg::*;
  import minion_frontend_pkg::*;
  import dft_pkg::*;

  vm_status_t         new_vm_status;
  fe_icache_req_t     new_icache_req;
  icache_fe_resp_t    new_icache_resp;
  fe_req_t            new_core_req;
  fe_operation_t      new_inst_data;

  assign new_vm_status   = vm_status_i;
  assign new_icache_resp = f6_icache_resp_i;
  assign new_core_req    = f0_core_req_i;

  minion_frontend_thread_buffer #(.ThreadId(0)) u_new (
    .clk_i               (clk_i),
    .rst_ni              (rst_ni),
    .reset_debug_i       (reset_debug_i),
    .chicken_bit_i       (1'b0),
    .dft_i               ('0),
    .f0_thread_enabled_i (f0_thread_enabled_i),
    .f0_reset_vector_i   (reset_vector_ext),
    .f0_disable_thread_i (f0_disable_thread_i),
    .f0_enable_thread_i  (f0_enable_thread_i),
    .in_debug_mode_i     (in_debug_mode_i),
    .vm_status_i         (new_vm_status),
    .f0_icache_req_ready_i (f0_icache_req_ready_i),
    .f1_icache_req_ready_i (f1_icache_req_ready_i),
    .f0_icache_req_valid_o (new_f0_icache_req_valid_o),
    .f0_icache_req_o     (new_icache_req),
    .f5_icache_resp_miss_i (f5_icache_resp_miss_i),
    .f6_icache_resp_valid_i (f6_icache_resp_valid_i),
    .f6_icache_resp_miss_i  (f6_icache_resp_miss_i),
    .f6_icache_resp_i    (new_icache_resp),
    .f7_icache_fill_done_i (f7_icache_fill_done_i),
    .f0_core_req_valid_i (f0_core_req_valid_i),
    .f0_core_req_i       (new_core_req),
    .f7_inst_ready_i     (f7_inst_ready_i),
    .f7_inst_rvc_o       (new_f7_inst_rvc_o),
    .f7_inst_valid_o     (new_f7_inst_valid_o),
    .f7_inst_data_o      (new_inst_data),
    .debug_o             (),
    .debug_ffb_wdata_i   (debug_ffb_wdata_i),
    .debug_ffb_en_i      (debug_ffb_en_i),
    .debug_ffb_exec_i    (debug_ffb_exec_i),
    .io_halt_i           (io_halt_i)
  );

  assign new_f7_inst_data_o = new_inst_data;

  // ── Original module ───────────────────────────────────

  frontend_icache_req   orig_icache_req;
  frontend_operation    orig_inst_data;

  frontend_thread_buffer #(.THREAD_ID(0)) u_orig (
    .clock_aon           (clk_i),
    .reset               (reset_orig),
    .reset_debug         (reset_debug_i),
    .chicken_bit         (1'b0),
    .f0_thread_enabled   (f0_thread_enabled_i),
    .f0_reset_vector     (f0_reset_vector_i),
    .f0_disable_thread   (f0_disable_thread_i),
    .f0_enable_thread    (f0_enable_thread_i),
    .in_debug_mode       (in_debug_mode_i),
    .vm_status           (vm_status_i),
    .f0_icache_req_ready (f0_icache_req_ready_i),
    .f1_icache_req_ready (f1_icache_req_ready_i),
    .f0_icache_req_valid (orig_f0_icache_req_valid_o),
    .f0_icache_req       (orig_icache_req),
    .f5_icache_resp_miss (f5_icache_resp_miss_i),
    .f6_icache_resp_valid (f6_icache_resp_valid_i),
    .f6_icache_resp_miss  (f6_icache_resp_miss_i),
    .f6_icache_resp      (f6_icache_resp_i),
    .f7_icache_fill_done (f7_icache_fill_done_i),
    .f0_core_req_valid   (f0_core_req_valid_i),
    .f0_core_req         (f0_core_req_i),
    .f7_inst_ready       (f7_inst_ready_i),
    .f7_inst_rvc         (orig_f7_inst_rvc_o),
    .f7_inst_valid       (orig_f7_inst_valid_o),
    .f7_inst_data        (orig_inst_data),
    .debug_ffb_wdata     (debug_ffb_wdata_i),
    .debug_ffb_en        (debug_ffb_en_i),
    .debug_ffb_exec      (debug_ffb_exec_i),
    .io_halt             (io_halt_i)
  );

  assign orig_f7_inst_data_o = orig_inst_data;

  assign new_f6_buffer_ptr_o  = u_new.f6_buffer_ptr;
  assign orig_f6_buffer_ptr_o = u_orig.f6_buffer_ptr;
  assign new_buffer_empty_o   = u_new.buffer_empty;
  assign orig_buffer_empty_o  = u_orig.buffer_empty;
  assign new_f6_valid_o       = u_new.f6_valid;
  assign orig_f6_valid_o      = u_orig.f6_valid;
  assign new_f6_buffer_wr_o   = u_new.f6_buffer_wr;
  assign orig_f6_buffer_wr_o  = u_orig.f6_buffer_wr;
  assign new_f6_buffer_wren_o = u_new.f6_buffer_wren;
  assign orig_f6_buffer_wren_o = u_orig.f6_buffer_wren;
  assign new_f6_write_ad_o    = u_new.f6_write_ad;
  assign orig_f6_write_ad_o   = u_orig.f6_write_ad;
  assign new_f0_req_buffer_o  = u_new.f0_req_buffer;
  assign orig_f0_req_buffer_o = u_orig.f0_req_buffer;
  assign new_f5_req_buffer_o  = u_new.f5_req_buffer;
  assign orig_f5_req_buffer_o = u_orig.f5_req_buffer;
  assign new_f6_req_buffer_o  = u_new.f6_req_buffer;
  assign orig_f6_req_buffer_o = u_orig.f6_req_buffer;
  assign new_using_pfb_o      = u_new.using_pfb;
  assign orig_using_pfb_o     = u_orig.using_pfb;
  assign new_clock_enable_o   = u_new.clock_enable;
  assign orig_clock_enable_o  = u_orig.clock_enable;
  assign new_f4_req_buffer_o  = u_new.f4_req_buffer;
  assign orig_f4_req_buffer_o = u_orig.f4_req_buffer;
  assign new_f5_req_valid_o   = u_new.f5_req_valid;
  assign orig_f5_req_valid_o  = u_orig.f5_req_valid;
  assign new_f6_req_valid_o   = u_new.f6_req_valid;
  assign orig_f6_req_valid_o  = u_orig.f6_req_valid;
  assign new_f6_inst_rvc_o    = u_new.f6_inst_rvc;
  assign orig_f6_inst_rvc_o   = u_orig.f6_inst_rvc;
  assign new_f6_buffer_ptr_next_o  = u_new.f6_buffer_ptr_next;
  assign orig_f6_buffer_ptr_next_o = u_orig.f6_buffer_ptr_next;
  assign new_f6_buffer_id_o   = u_new.f6_buffer_id;
  assign orig_f6_buffer_id_o  = u_orig.f6_buffer_id;
  assign new_f6_buffer_id_next_o   = u_new.f6_buffer_id_next;
  assign orig_f6_buffer_id_next_o  = u_orig.f6_buffer_id_next;
  assign new_f6_last_from_buffer_o = u_new.f6_last_from_buffer;
  assign orig_f6_last_from_buffer_o = u_orig.f6_last_from_buffer;
  assign new_f6_split_o       = u_new.f6_split;
  assign orig_f6_split_o      = u_orig.f6_split;
  assign new_clock_gated_o    = u_new.clock_gated;
  assign orig_clock_gated_o   = u_orig.clock;
  assign new_f6_buffer_wr_1p_next_o = u_new.f6_buffer_wr_1p_next;
  assign orig_f6_buffer_wr_1p_next_o = u_orig.f6_buffer_wr_1p_next;
  assign new_f6_buffer_wr_1p_o = u_new.u_inst_data.wr_data_en_1p_q;
  assign orig_f6_buffer_wr_1p_o = u_orig.f6_buffer_wr_1p;
  assign new_f6_buffer_inst_data_o = u_new.f6_buffer_inst_data;
  assign orig_f6_buffer_inst_data_o = u_orig.f6_buffer_inst_data;
  assign new_f6_stall_o = u_new.f6_stall;
  assign orig_f6_stall_o = u_orig.f6_stall;
  assign new_dbg_clear_f7_o = u_new.dbg_clear_f7;
  assign orig_dbg_clear_f7_o = u_orig.dbg_clear_f7;
  assign new_ffb_update_read_pointer_o = u_new.ffb_update_read_pointer;
  assign orig_ffb_update_read_pointer_o = u_orig.ffb_update_read_pointer;
  assign new_debug_ffb_en_2_o = u_new.debug_ffb_en_2;
  assign orig_debug_ffb_en_2_o = u_orig.debug_ffb_en_2;
  assign new_debug_ffb_exec_2_o = u_new.debug_ffb_exec_2;
  assign orig_debug_ffb_exec_2_o = u_orig.debug_ffb_exec_2;

endmodule
