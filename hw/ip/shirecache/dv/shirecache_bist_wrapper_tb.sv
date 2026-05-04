// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Testbench for shirecache_bist_wrapper.
// Decomposes struct ports to scalars for C++ driving.

/* verilator lint_off UNUSEDSIGNAL */
module shirecache_bist_wrapper_tb
  import shirecache_pkg::*;
(
  input  logic                     clk_i,
  input  logic                     rst_ni,
  input  logic                     mbi_implemented_const_i,
  input  logic                     dft_mbist_en_i,

  // MBIST shared interface
  input  logic                     mbist_en_i,
  input  logic [MbistSelSize-1:0]  mbist_sel_i,
  input  logic                     mbist_rd_en_i,
  input  logic                     mbist_wr_en_i,
  input  logic [MbistAddrSize-1:0] mbist_addr_i,
  input  logic [31:0]              mbist_wdata_lo_i,
  output logic [31:0]              mbist_rdata_lo_o,

  output logic                     mbist_on_o,

  // Pipe BIST req (decomposed)
  output logic                     pipe_req_mbist_on_o,
  output logic                     pipe_req_mbs_sel_o,
  output logic                     pipe_req_mbs_rd_en_o,
  output logic                     pipe_req_mbs_wr_en_o,
  output logic [MbsAddrSize-1:0]   pipe_req_mbs_addr_o,
  output logic [31:0]              pipe_req_mbs_wdata_lo_o,
  output logic                     pipe_req_mbt_sel_o,
  output logic                     pipe_req_mbt_rd_en_o,
  output logic                     pipe_req_mbt_wr_en_o,
  output logic [MbtAddrSize-1:0]   pipe_req_mbt_addr_o,
  output logic [31:0]              pipe_req_mbt_wdata_lo_o,
  output logic                     pipe_req_mbd_sel_o,
  output logic                     pipe_req_mbd_rd_en_o,
  output logic                     pipe_req_mbd_wr_en_o,
  output logic [MbdAddrSize-1:0]   pipe_req_mbd_addr_o,
  output logic [31:0]              pipe_req_mbd_wdata_lo_o,

  // Pipe BIST rsp (scalar inputs)
  input  logic [31:0]              pipe_rsp_mbs_rdata_lo_i,
  input  logic [31:0]              pipe_rsp_mbt_rdata_lo_i,
  input  logic [31:0]              pipe_rsp_mbd_rdata_lo_i,

  // Icache BIST req (decomposed)
  output logic                     icache_req_mbist_on_o,
  output logic                     icache_req_mbi_sel_o,
  output logic                     icache_req_mbi_rd_en_o,
  output logic                     icache_req_mbi_wr_en_o,
  output logic [MbiAddrSize-1:0]   icache_req_mbi_addr_o,
  output logic [31:0]              icache_req_mbi_wdata_lo_o,

  // Icache BIST rsp
  input  logic [31:0]              icache_rsp_mbi_rdata_lo_i
);

  // Build wide wdata from low 32 bits (rest zero)
  logic [MbistDataSize-1:0] mbist_wdata;
  assign mbist_wdata = MbistDataSize'(mbist_wdata_lo_i);

  logic [MbistDataSize-1:0] mbist_rdata;

  bist_req_t        pipe_bist_req;
  bist_rsp_t        pipe_bist_rsp;
  icache_bist_req_t icache_bist_req;
  icache_bist_rsp_t icache_bist_rsp;

  // Build rsp structs from scalar inputs
  always_comb begin
    pipe_bist_rsp.mbs_rdata = MbsDataSize'(pipe_rsp_mbs_rdata_lo_i);
    pipe_bist_rsp.mbt_rdata = MbtDataSize'(pipe_rsp_mbt_rdata_lo_i);
    pipe_bist_rsp.mbd_rdata = MbdDataSize'(pipe_rsp_mbd_rdata_lo_i);
    icache_bist_rsp.mbi_rdata = MbiDataSize'(icache_rsp_mbi_rdata_lo_i);
  end

  shirecache_bist_wrapper u_dut (
    .clk_i, .rst_ni,
    .mbi_implemented_const_i,
    .dft_mbist_en_i,
    .mbist_en_i, .mbist_sel_i, .mbist_rd_en_i, .mbist_wr_en_i,
    .mbist_addr_i,
    .mbist_wdata_i           (mbist_wdata),
    .mbist_rdata_o           (mbist_rdata),
    .mbist_on_o,
    .pipe_bist_req_info_o    (pipe_bist_req),
    .pipe_bist_rsp_info_i    (pipe_bist_rsp),
    .icache_bist_req_info_o  (icache_bist_req),
    .icache_bist_rsp_info_i  (icache_bist_rsp)
  );

  // Decompose outputs
  assign mbist_rdata_lo_o      = mbist_rdata[31:0];

  assign pipe_req_mbist_on_o   = pipe_bist_req.mbist_on;
  assign pipe_req_mbs_sel_o    = pipe_bist_req.mbs_sel;
  assign pipe_req_mbs_rd_en_o  = pipe_bist_req.mbs_rd_en;
  assign pipe_req_mbs_wr_en_o  = pipe_bist_req.mbs_wr_en;
  assign pipe_req_mbs_addr_o   = pipe_bist_req.mbs_addr;
  assign pipe_req_mbs_wdata_lo_o = pipe_bist_req.mbs_wdata[31:0];
  assign pipe_req_mbt_sel_o    = pipe_bist_req.mbt_sel;
  assign pipe_req_mbt_rd_en_o  = pipe_bist_req.mbt_rd_en;
  assign pipe_req_mbt_wr_en_o  = pipe_bist_req.mbt_wr_en;
  assign pipe_req_mbt_addr_o   = pipe_bist_req.mbt_addr;
  assign pipe_req_mbt_wdata_lo_o = pipe_bist_req.mbt_wdata[31:0];
  assign pipe_req_mbd_sel_o    = pipe_bist_req.mbd_sel;
  assign pipe_req_mbd_rd_en_o  = pipe_bist_req.mbd_rd_en;
  assign pipe_req_mbd_wr_en_o  = pipe_bist_req.mbd_wr_en;
  assign pipe_req_mbd_addr_o   = pipe_bist_req.mbd_addr;
  assign pipe_req_mbd_wdata_lo_o = pipe_bist_req.mbd_wdata[31:0];

  assign icache_req_mbist_on_o    = icache_bist_req.mbist_on;
  assign icache_req_mbi_sel_o     = icache_bist_req.mbi_sel;
  assign icache_req_mbi_rd_en_o   = icache_bist_req.mbi_rd_en;
  assign icache_req_mbi_wr_en_o   = icache_bist_req.mbi_wr_en;
  assign icache_req_mbi_addr_o    = icache_bist_req.mbi_addr;
  assign icache_req_mbi_wdata_lo_o = icache_bist_req.mbi_wdata[31:0];

  logic unused_ok;
  assign unused_ok = &{1'b0, mbist_rdata, pipe_bist_req, icache_bist_req, unused_ok};

  /* verilator lint_on UNUSEDSIGNAL */
endmodule
