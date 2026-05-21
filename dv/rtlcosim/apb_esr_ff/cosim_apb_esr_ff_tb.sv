// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_apb_esr_ff_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  input  logic [15:0] in0_paddr_i,
  input  logic        in0_pwrite_i,
  input  logic        in0_psel_i,
  input  logic        in0_penable_i,
  input  logic [63:0] in0_pwdata_i,
  input  logic        rsp0_pready_i,
  input  logic [63:0] rsp0_prdata_i,
  input  logic        rsp0_pslverr_i,

  input  logic [15:0] in1_paddr_i,
  input  logic        in1_pwrite_i,
  input  logic        in1_psel_i,
  input  logic        in1_penable_i,
  input  logic [63:0] in1_pwdata_i,
  input  logic        rsp1_pready_i,
  input  logic [63:0] rsp1_prdata_i,
  input  logic        rsp1_pslverr_i,

  output logic [15:0] orig_out0_paddr_o,
  output logic        orig_out0_pwrite_o,
  output logic        orig_out0_psel_o,
  output logic        orig_out0_penable_o,
  output logic [63:0] orig_out0_pwdata_o,
  output logic        orig_in0_pready_o,
  output logic [63:0] orig_in0_prdata_o,
  output logic        orig_in0_pslverr_o,
  output logic [15:0] new_out0_paddr_o,
  output logic        new_out0_pwrite_o,
  output logic        new_out0_psel_o,
  output logic        new_out0_penable_o,
  output logic [63:0] new_out0_pwdata_o,
  output logic        new_in0_pready_o,
  output logic [63:0] new_in0_prdata_o,
  output logic        new_in0_pslverr_o,

  output logic [15:0] orig_out1_paddr_o,
  output logic        orig_out1_pwrite_o,
  output logic        orig_out1_psel_o,
  output logic        orig_out1_penable_o,
  output logic [63:0] orig_out1_pwdata_o,
  output logic        orig_in1_pready_o,
  output logic [63:0] orig_in1_prdata_o,
  output logic        orig_in1_pslverr_o,
  output logic [15:0] new_out1_paddr_o,
  output logic        new_out1_pwrite_o,
  output logic        new_out1_psel_o,
  output logic        new_out1_penable_o,
  output logic [63:0] new_out1_pwdata_o,
  output logic        new_in1_pready_o,
  output logic [63:0] new_in1_prdata_o,
  output logic        new_in1_pslverr_o
);
  APB_to_all_ESRs_t [1:0] orig_req_in;
  APB_to_all_ESRs_t [1:0] orig_req_out;
  APB_from_all_ESRs_t [1:0] orig_rsp_in;
  APB_from_all_ESRs_t [1:0] orig_rsp_out;

  apb_pkg::req_t [1:0] new_req_in;
  apb_pkg::req_t [1:0] new_req_out;
  apb_pkg::rsp_t [1:0] new_rsp_in;
  apb_pkg::rsp_t [1:0] new_rsp_out;

  assign orig_req_in[0].apb_paddr = in0_paddr_i;
  assign orig_req_in[0].apb_pwrite = in0_pwrite_i;
  assign orig_req_in[0].apb_psel = in0_psel_i;
  assign orig_req_in[0].apb_penable = in0_penable_i;
  assign orig_req_in[0].apb_pwdata = in0_pwdata_i;
  assign orig_rsp_in[0].apb_pready = rsp0_pready_i;
  assign orig_rsp_in[0].apb_prdata = rsp0_prdata_i;
  assign orig_rsp_in[0].apb_pslverr = rsp0_pslverr_i;

  assign orig_req_in[1].apb_paddr = in1_paddr_i;
  assign orig_req_in[1].apb_pwrite = in1_pwrite_i;
  assign orig_req_in[1].apb_psel = in1_psel_i;
  assign orig_req_in[1].apb_penable = in1_penable_i;
  assign orig_req_in[1].apb_pwdata = in1_pwdata_i;
  assign orig_rsp_in[1].apb_pready = rsp1_pready_i;
  assign orig_rsp_in[1].apb_prdata = rsp1_prdata_i;
  assign orig_rsp_in[1].apb_pslverr = rsp1_pslverr_i;

  assign new_req_in[0].paddr = in0_paddr_i;
  assign new_req_in[0].pwrite = in0_pwrite_i;
  assign new_req_in[0].psel = in0_psel_i;
  assign new_req_in[0].penable = in0_penable_i;
  assign new_req_in[0].pwdata = in0_pwdata_i;
  assign new_rsp_in[0].pready = rsp0_pready_i;
  assign new_rsp_in[0].prdata = rsp0_prdata_i;
  assign new_rsp_in[0].pslverr = rsp0_pslverr_i;

  assign new_req_in[1].paddr = in1_paddr_i;
  assign new_req_in[1].pwrite = in1_pwrite_i;
  assign new_req_in[1].psel = in1_psel_i;
  assign new_req_in[1].penable = in1_penable_i;
  assign new_req_in[1].pwdata = in1_pwdata_i;
  assign new_rsp_in[1].pready = rsp1_pready_i;
  assign new_rsp_in[1].prdata = rsp1_prdata_i;
  assign new_rsp_in[1].pslverr = rsp1_pslverr_i;

  apb_esr_ff_orig #(.NUM_INTERFACES(2)) u_orig (
    .clock           (clk_i),
    .reset           (!rst_ni),
    .APB_ESR_req_in  (orig_req_in),
    .APB_ESR_req_out (orig_req_out),
    .APB_ESR_rsp_in  (orig_rsp_in),
    .APB_ESR_rsp_out (orig_rsp_out)
  );

  apb_esr_ff #(.NumInterfaces(2)) u_new (
    .clk_i     (clk_i),
    .rst_ni    (rst_ni),
    .apb_req_i (new_req_in),
    .apb_req_o (new_req_out),
    .apb_rsp_i (new_rsp_in),
    .apb_rsp_o (new_rsp_out)
  );

  assign orig_out0_paddr_o = orig_req_out[0].apb_paddr[15:0];
  assign orig_out0_pwrite_o = orig_req_out[0].apb_pwrite;
  assign orig_out0_psel_o = orig_req_out[0].apb_psel;
  assign orig_out0_penable_o = orig_req_out[0].apb_penable;
  assign orig_out0_pwdata_o = orig_req_out[0].apb_pwdata;
  assign orig_in0_pready_o = orig_rsp_out[0].apb_pready;
  assign orig_in0_prdata_o = orig_rsp_out[0].apb_prdata;
  assign orig_in0_pslverr_o = orig_rsp_out[0].apb_pslverr;
  assign new_out0_paddr_o = new_req_out[0].paddr;
  assign new_out0_pwrite_o = new_req_out[0].pwrite;
  assign new_out0_psel_o = new_req_out[0].psel;
  assign new_out0_penable_o = new_req_out[0].penable;
  assign new_out0_pwdata_o = new_req_out[0].pwdata;
  assign new_in0_pready_o = new_rsp_out[0].pready;
  assign new_in0_prdata_o = new_rsp_out[0].prdata;
  assign new_in0_pslverr_o = new_rsp_out[0].pslverr;

  assign orig_out1_paddr_o = orig_req_out[1].apb_paddr[15:0];
  assign orig_out1_pwrite_o = orig_req_out[1].apb_pwrite;
  assign orig_out1_psel_o = orig_req_out[1].apb_psel;
  assign orig_out1_penable_o = orig_req_out[1].apb_penable;
  assign orig_out1_pwdata_o = orig_req_out[1].apb_pwdata;
  assign orig_in1_pready_o = orig_rsp_out[1].apb_pready;
  assign orig_in1_prdata_o = orig_rsp_out[1].apb_prdata;
  assign orig_in1_pslverr_o = orig_rsp_out[1].apb_pslverr;
  assign new_out1_paddr_o = new_req_out[1].paddr;
  assign new_out1_pwrite_o = new_req_out[1].pwrite;
  assign new_out1_psel_o = new_req_out[1].psel;
  assign new_out1_penable_o = new_req_out[1].penable;
  assign new_out1_pwdata_o = new_req_out[1].pwdata;
  assign new_in1_pready_o = new_rsp_out[1].pready;
  assign new_in1_prdata_o = new_rsp_out[1].prdata;
  assign new_in1_pslverr_o = new_rsp_out[1].pslverr;
endmodule
