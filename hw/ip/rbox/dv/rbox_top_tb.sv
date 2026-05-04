// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test wrapper for rbox_top.
// Exposes APB, run control, and key status outputs for C++ test driver.
// SC request/response and most internal interfaces are stubbed.

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off WIDTHEXPAND */
/* verilator lint_off WIDTHTRUNC */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off PINCONNECTEMPTY */
module rbox_top_tb (
  input  logic        clk_i,
  input  logic        rst_d_ni,
  input  logic        rst_w_ni,

  // APB interface
  input  logic [15:0] apb_paddr_i,
  input  logic        apb_pwrite_i,
  input  logic        apb_psel_i,
  input  logic        apb_penable_i,
  input  logic [63:0] apb_pwdata_i,
  output logic        apb_pready_o,
  output logic [63:0] apb_prdata_o,
  output logic        apb_pslverr_o,

  // Run control
  input  logic        rc_halt_req_events_i,
  input  logic        rc_resume_req_events_i,
  input  logic        rc_halt_req_gpio_i,
  input  logic        rc_resume_req_gpio_i,
  output logic        rc_halt_ack_o,
  output logic        rc_resume_ack_o,
  output logic        rc_reset_ack_o,

  // SC request
  output logic        sc_req_valid_any_o,
  input  logic        sc_req_ready_i,

  // Observe status (read back via APB, but also observe from ESR)
  output logic        rbox_sc_rsp_ready_o
);

  import rbox_pkg::*;
  import etlink_pkg::*;
  import apb_pkg::*;
  import dft_pkg::*;
  import ram_cfg_pkg::*;

  apb_pkg::req_t apb_req;
  apb_pkg::rsp_t apb_rsp;

  assign apb_req.paddr   = apb_paddr_i;
  assign apb_req.pwrite  = apb_pwrite_i;
  assign apb_req.psel    = apb_psel_i;
  assign apb_req.penable = apb_penable_i;
  assign apb_req.pwdata  = apb_pwdata_i;

  assign apb_pready_o  = apb_rsp.pready;
  assign apb_prdata_o  = apb_rsp.prdata;
  assign apb_pslverr_o = apb_rsp.pslverr;

  rc_t rc;
  rc_ack_t rc_ack;

  assign rc.events.halt_req   = rc_halt_req_events_i;
  assign rc.events.resume_req = rc_resume_req_events_i;
  assign rc.gpio.halt_req     = rc_halt_req_gpio_i;
  assign rc.gpio.resume_req   = rc_resume_req_gpio_i;
  assign rc.gpio.reset_req    = 1'b0;
  assign rc.single_step       = 1'b0;

  assign rc_halt_ack_o   = rc_ack.halt_ack;
  assign rc_resume_ack_o = rc_ack.resume_ack;
  assign rc_reset_ack_o  = rc_ack.reset_ack;

  etlink_pkg::req_t rbox_sc_req;
  logic [ScBanks:0]  rbox_sc_req_valid;
  logic [ScBanks:0]  rbox_sc_req_ready;

  assign rbox_sc_req_ready = {(ScBanks+1){sc_req_ready_i}};
  assign sc_req_valid_any_o = |rbox_sc_req_valid;

  etlink_pkg::rsp_t sc_rbox_rsp;
  assign sc_rbox_rsp = '0;

  rbox_top u_dut (
    .clk_i              (clk_i),
    .rst_d_ni           (rst_d_ni),
    .rst_w_ni           (rst_w_ni),
    .shire_id_i         (8'd0),
    .rbox_sc_req_o      (rbox_sc_req),
    .rbox_sc_req_valid_o(rbox_sc_req_valid),
    .rbox_sc_req_ready_i(rbox_sc_req_ready),
    .sc_rbox_rsp_i      (sc_rbox_rsp),
    .sc_rbox_rsp_valid_i(1'b0),
    .sc_rbox_rsp_ready_o(rbox_sc_rsp_ready_o),
    .rc_i               (rc),
    .rc_ack_o           (rc_ack),
    .apb_req_i          (apb_req),
    .apb_rsp_o          (apb_rsp),
    .ram_cfg_i          ('0),
    .dft_i              ('0)
  );

endmodule
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on UNOPTFLAT */
