// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit-test wrapper for minion_debug_apb_slv.

module minion_debug_apb_slv_tb
  import minion_pkg::*;
(
  input  logic                                      clk_i,
  input  logic                                      rst_ni,

  input  logic [MinRegApbAddrWidth-1:0]             apb_paddr_i,
  input  logic                                      apb_pwrite_i,
  input  logic                                      apb_psel_i,
  input  logic                                      apb_penable_i,
  input  logic [MinionDebugApbDataWidth-1:0]        apb_pwdata_i,

  output logic                                      apb_pready_o,
  output logic [MinionDebugApbDataWidth-1:0]        apb_prdata_o,
  output logic                                      apb_pslverr_o,

  input  logic [63:0]                               csr_wdata_i,
  input  logic [NrThreads-1:0]                      update_ddata0_i,
  output logic [63:0]                               read_ddata0_t0_o,
  output logic [63:0]                               read_ddata0_t1_o,

  output logic [MinionDebugApbDataWidth-1:0]        ffb_wdata_o,
  output logic [MinionDebugFfbWords-1:0]            ffb_en_o,
  output logic                                      ffb_thread_sel_o,
  output logic [NrThreads-1:0]                      ex_program_buffer_o,

  input  logic [NrThreads-1:0]                      in_debug_mode_i
);

  logic [NrThreads-1:0][63:0] read_ddata0;

  minion_debug_apb_slv u_dut (
    .clk_i               (clk_i),
    .rst_ni              (rst_ni),
    .apb_paddr_i         (apb_paddr_i),
    .apb_pwrite_i        (apb_pwrite_i),
    .apb_psel_i          (apb_psel_i),
    .apb_penable_i       (apb_penable_i),
    .apb_pwdata_i        (apb_pwdata_i),
    .apb_pready_o        (apb_pready_o),
    .apb_prdata_o        (apb_prdata_o),
    .apb_pslverr_o       (apb_pslverr_o),
    .csr_wdata_i         (csr_wdata_i),
    .update_ddata0_i     (update_ddata0_i),
    .read_ddata0_o       (read_ddata0),
    .ffb_wdata_o         (ffb_wdata_o),
    .ffb_en_o            (ffb_en_o),
    .ffb_thread_sel_o    (ffb_thread_sel_o),
    .ex_program_buffer_o (ex_program_buffer_o),
    .in_debug_mode_i     (in_debug_mode_i)
  );

  assign read_ddata0_t0_o = read_ddata0[0];
  assign read_ddata0_t1_o = read_ddata0[1];

endmodule
