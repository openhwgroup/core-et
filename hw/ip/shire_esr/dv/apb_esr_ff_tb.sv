// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module apb_esr_ff_tb (
  input  logic        clk_i,
  input  logic        rst_ni,

  input  logic [15:0] in0_paddr_i,
  input  logic        in0_pwrite_i,
  input  logic        in0_psel_i,
  input  logic        in0_penable_i,
  input  logic [63:0] in0_pwdata_i,
  input  logic        out0_pready_i,
  input  logic [63:0] out0_prdata_i,
  input  logic        out0_pslverr_i,

  input  logic [15:0] in1_paddr_i,
  input  logic        in1_pwrite_i,
  input  logic        in1_psel_i,
  input  logic        in1_penable_i,
  input  logic [63:0] in1_pwdata_i,
  input  logic        out1_pready_i,
  input  logic [63:0] out1_prdata_i,
  input  logic        out1_pslverr_i,

  output logic [15:0] out0_paddr_o,
  output logic        out0_pwrite_o,
  output logic        out0_psel_o,
  output logic        out0_penable_o,
  output logic [63:0] out0_pwdata_o,
  output logic        in0_pready_o,
  output logic [63:0] in0_prdata_o,
  output logic        in0_pslverr_o,

  output logic [15:0] out1_paddr_o,
  output logic        out1_pwrite_o,
  output logic        out1_psel_o,
  output logic        out1_penable_o,
  output logic [63:0] out1_pwdata_o,
  output logic        in1_pready_o,
  output logic [63:0] in1_prdata_o,
  output logic        in1_pslverr_o
);
  apb_pkg::req_t [1:0] req_i;
  apb_pkg::req_t [1:0] req_o;
  apb_pkg::rsp_t [1:0] rsp_i;
  apb_pkg::rsp_t [1:0] rsp_o;

  assign req_i[0].paddr = in0_paddr_i;
  assign req_i[0].pwrite = in0_pwrite_i;
  assign req_i[0].psel = in0_psel_i;
  assign req_i[0].penable = in0_penable_i;
  assign req_i[0].pwdata = in0_pwdata_i;
  assign rsp_i[0].pready = out0_pready_i;
  assign rsp_i[0].prdata = out0_prdata_i;
  assign rsp_i[0].pslverr = out0_pslverr_i;

  assign req_i[1].paddr = in1_paddr_i;
  assign req_i[1].pwrite = in1_pwrite_i;
  assign req_i[1].psel = in1_psel_i;
  assign req_i[1].penable = in1_penable_i;
  assign req_i[1].pwdata = in1_pwdata_i;
  assign rsp_i[1].pready = out1_pready_i;
  assign rsp_i[1].prdata = out1_prdata_i;
  assign rsp_i[1].pslverr = out1_pslverr_i;

  apb_esr_ff #(.NumInterfaces(2)) u_dut (
    .clk_i     (clk_i),
    .rst_ni    (rst_ni),
    .apb_req_i (req_i),
    .apb_req_o (req_o),
    .apb_rsp_i (rsp_i),
    .apb_rsp_o (rsp_o)
  );

  assign out0_paddr_o = req_o[0].paddr;
  assign out0_pwrite_o = req_o[0].pwrite;
  assign out0_psel_o = req_o[0].psel;
  assign out0_penable_o = req_o[0].penable;
  assign out0_pwdata_o = req_o[0].pwdata;
  assign in0_pready_o = rsp_o[0].pready;
  assign in0_prdata_o = rsp_o[0].prdata;
  assign in0_pslverr_o = rsp_o[0].pslverr;

  assign out1_paddr_o = req_o[1].paddr;
  assign out1_pwrite_o = req_o[1].pwrite;
  assign out1_psel_o = req_o[1].psel;
  assign out1_penable_o = req_o[1].penable;
  assign out1_pwdata_o = req_o[1].pwdata;
  assign in1_pready_o = rsp_o[1].pready;
  assign in1_prdata_o = rsp_o[1].prdata;
  assign in1_pslverr_o = rsp_o[1].pslverr;
endmodule
