// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_esr_spio_tb (
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

  output logic        orig_apb_pready_o,
  output logic [63:0] orig_apb_prdata_o,
  output logic        orig_apb_pslverr_o,
  output logic [63:0] orig_esr_spdmctrl_o,
  output logic [63:0] orig_esr_sphastatus_o,

  output logic        new_apb_pready_o,
  output logic [63:0] new_apb_prdata_o,
  output logic        new_apb_pslverr_o,
  output logic [63:0] new_esr_spdmctrl_o,
  output logic [63:0] new_esr_sphastatus_o
);
  logic [11:0] dbg_status_flat;
  logic        reset_c;
  logic        reset_w;
  logic        reset_d;

  assign dbg_status_flat = {
    dbg_halted_i,
    dbg_running_i,
    dbg_have_reset_i,
    dbg_busy_i,
    dbg_exception_i,
    dbg_error_i
  };

  assign reset_c = !rst_c_ni;
  assign reset_w = !rst_w_ni;
  assign reset_d = !rst_d_ni;

  esr_spio_orig u_orig (
    .clock             (clk_i),
    .reset_c           (reset_c),
    .reset_w           (reset_w),
    .reset_d           (reset_d),
    .test_en           (test_en_i),
    .esr_spdmctrl_op   (orig_esr_spdmctrl_o),
    .svcproc_dbg_status(dbg_status_flat),
    .esr_sphastatus_op (orig_esr_sphastatus_o),
    .spio_rc_resumeack (spio_rc_resumeack_i),
    .apb_paddr         (apb_paddr_i),
    .apb_pwrite        (apb_pwrite_i),
    .apb_psel          (apb_psel_i),
    .apb_penable       (apb_penable_i),
    .apb_pwdata        (apb_pwdata_i),
    .apb_pready        (orig_apb_pready_o),
    .apb_prdata        (orig_apb_prdata_o),
    .apb_pslverr       (orig_apb_pslverr_o)
  );

  esr_spio u_new (
    .clk_i,
    .rst_c_ni,
    .rst_w_ni,
    .rst_d_ni,
    .test_en_i,
    .esr_spdmctrl_o    (new_esr_spdmctrl_o),
    .svcproc_dbg_status_i(dbg_status_flat),
    .esr_sphastatus_o  (new_esr_sphastatus_o),
    .spio_rc_resumeack_i,
    .apb_paddr_i,
    .apb_pwrite_i,
    .apb_psel_i,
    .apb_penable_i,
    .apb_pwdata_i,
    .apb_pready_o      (new_apb_pready_o),
    .apb_prdata_o      (new_apb_prdata_o),
    .apb_pslverr_o     (new_apb_pslverr_o)
  );
endmodule
