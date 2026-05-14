// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper for minion_debug_apb_slv.

`include "soc.vh"

module cosim_minion_debug_apb_slv_tb
  import minion_pkg::*;
(
  input  logic                               clk_i,
  input  logic                               rst_ni,

  input  logic [MinRegApbAddrWidth-1:0]      stim_apb_paddr,
  input  logic                               stim_apb_pwrite,
  input  logic                               stim_apb_psel,
  input  logic                               stim_apb_penable,
  input  logic [MinionDebugApbDataWidth-1:0] stim_apb_pwdata,

  input  logic [63:0]                        stim_csr_wdata,
  input  logic [NrThreads-1:0]               stim_update_ddata0,
  input  logic [NrThreads-1:0]               stim_in_debug_mode,

  output logic                               new_apb_pready_o,
  output logic [MinionDebugApbDataWidth-1:0] new_apb_prdata_o,
  output logic                               new_apb_pslverr_o,
  output logic [63:0]                        new_read_ddata0_t0_o,
  output logic [63:0]                        new_read_ddata0_t1_o,
  output logic [MinionDebugApbDataWidth-1:0] new_ffb_wdata_o,
  output logic [MinionDebugFfbWords-1:0]     new_ffb_en_o,
  output logic                               new_ffb_thread_sel_o,
  output logic [NrThreads-1:0]               new_ex_program_buffer_o,

  output logic                               orig_apb_pready_o,
  output logic [`MINION_DEBUG_APB_D_WIDTH-1:0] orig_apb_prdata_o,
  output logic                               orig_apb_pslverr_o,
  output logic [63:0]                        orig_read_ddata0_t0_o,
  output logic [63:0]                        orig_read_ddata0_t1_o,
  output logic [`MINION_DEBUG_APB_D_WIDTH-1:0] orig_ffb_wdata_o,
  output logic [`MINION_DEBUG_FFB_WORDS-1:0] orig_ffb_en_o,
  output logic                               orig_ffb_thread_sel_o,
  output logic [`CORE_NR_THREADS-1:0]        orig_ex_program_buffer_o
);

  logic [NrThreads-1:0][63:0]       new_read_ddata0;
  logic [`CORE_NR_THREADS-1:0][63:0] orig_read_ddata0;

  minion_debug_apb_slv u_new (
    .clk_i               (clk_i),
    .rst_ni              (rst_ni),
    .apb_paddr_i         (stim_apb_paddr),
    .apb_pwrite_i        (stim_apb_pwrite),
    .apb_psel_i          (stim_apb_psel),
    .apb_penable_i       (stim_apb_penable),
    .apb_pwdata_i        (stim_apb_pwdata),
    .apb_pready_o        (new_apb_pready_o),
    .apb_prdata_o        (new_apb_prdata_o),
    .apb_pslverr_o       (new_apb_pslverr_o),
    .csr_wdata_i         (stim_csr_wdata),
    .update_ddata0_i     (stim_update_ddata0),
    .read_ddata0_o       (new_read_ddata0),
    .ffb_wdata_o         (new_ffb_wdata_o),
    .ffb_en_o            (new_ffb_en_o),
    .ffb_thread_sel_o    (new_ffb_thread_sel_o),
    .ex_program_buffer_o (new_ex_program_buffer_o),
    .in_debug_mode_i     (stim_in_debug_mode)
  );

  minion_debug_apb_slv_orig u_orig (
    .clock              (clk_i),
    .reset              (!rst_ni),
    .apb_paddr          (stim_apb_paddr),
    .apb_pwrite         (stim_apb_pwrite),
    .apb_psel           (stim_apb_psel),
    .apb_penable        (stim_apb_penable),
    .apb_pwdata         (stim_apb_pwdata),
    .apb_pready         (orig_apb_pready_o),
    .apb_prdata         (orig_apb_prdata_o),
    .apb_pslverr        (orig_apb_pslverr_o),
    .csr_wdata_ip       (stim_csr_wdata),
    .update_ddata0_ip   (stim_update_ddata0),
    .read_ddata0_op     (orig_read_ddata0),
    .ffb_wdata_op       (orig_ffb_wdata_o),
    .ffb_en_op          (orig_ffb_en_o),
    .ffb_thread_sel_op  (orig_ffb_thread_sel_o),
    .ex_program_buffer  (orig_ex_program_buffer_o),
    .in_debug_mode      (stim_in_debug_mode)
  );

  assign new_read_ddata0_t0_o = new_read_ddata0[0];
  assign new_read_ddata0_t1_o = new_read_ddata0[1];
  assign orig_read_ddata0_t0_o = orig_read_ddata0[0];
  assign orig_read_ddata0_t1_o = orig_read_ddata0[1];

endmodule
