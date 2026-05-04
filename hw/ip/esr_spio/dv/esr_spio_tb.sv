// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module esr_spio_tb (
  input  logic        clk_i,
  input  logic        rst_c_ni,
  input  logic        rst_w_ni,
  input  logic        rst_d_ni,
  input  logic        test_en_i,

  input  logic [1:0]  dbg_halted_i,
  input  logic [1:0]  dbg_running_i,
  input  logic [1:0]  dbg_have_reset_i,
  input  logic [1:0]  dbg_busy_i,
  input  logic [1:0]  dbg_exception_i,
  input  logic [1:0]  dbg_error_i,
  input  logic        spio_rc_resumeack_i,

  input  logic [3:0]  apb_paddr_i,
  input  logic        apb_pwrite_i,
  input  logic        apb_psel_i,
  input  logic        apb_penable_i,
  input  logic [63:0] apb_pwdata_i,
  output logic        apb_pready_o,
  output logic [63:0] apb_prdata_o,
  output logic        apb_pslverr_o,

  output logic [63:0] esr_spdmctrl_o,
  output logic [63:0] esr_sphastatus_o
);
  logic [11:0] dbg_status_flat;
  esr_pkg::esr_spdmctrl_t esr_spdmctrl;
  esr_pkg::esr_sphastatus_t esr_sphastatus;

  assign dbg_status_flat = {
    dbg_halted_i,
    dbg_running_i,
    dbg_have_reset_i,
    dbg_busy_i,
    dbg_exception_i,
    dbg_error_i
  };

  esr_spio u_dut (
    .clk_i,
    .rst_c_ni,
    .rst_w_ni,
    .rst_d_ni,
    .test_en_i,
    .esr_spdmctrl_o      (esr_spdmctrl),
    .svcproc_dbg_status_i(dbg_status_flat),
    .esr_sphastatus_o    (esr_sphastatus),
    .spio_rc_resumeack_i,
    .apb_paddr_i,
    .apb_pwrite_i,
    .apb_psel_i,
    .apb_penable_i,
    .apb_pwdata_i,
    .apb_pready_o,
    .apb_prdata_o,
    .apb_pslverr_o
  );

  assign esr_spdmctrl_o   = esr_spdmctrl;
  assign esr_sphastatus_o = esr_sphastatus;
endmodule
