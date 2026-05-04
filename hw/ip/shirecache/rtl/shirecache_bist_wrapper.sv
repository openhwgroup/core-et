// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire cache BIST wrapper — virtual memory wrapper for MMB BIST insertion.
//
// Contains all logical memory bist_mbx sub-modules (MBS, MBT, MBD, MBI).
// Decodes mbist_sel to one-hot, routes signals to the appropriate mailbox,
// and muxes read data back.
//
// The BIST insertion tool requires all logical memories below this hierarchy.
//
// Replaces: shire_cache_bist_wrapper (etcore-soc)

/* verilator lint_off UNUSEDSIGNAL */
module shirecache_bist_wrapper
  import shirecache_pkg::*;
(
  input  logic                          clk_i,
  input  logic                          rst_ni,
  input  logic                          mbi_implemented_const_i,
  input  logic                          dft_mbist_en_i,

  // MBIST shared interface
  input  logic                          mbist_en_i,
  input  logic [MbistSelSize-1:0]       mbist_sel_i,
  input  logic                          mbist_rd_en_i,
  input  logic                          mbist_wr_en_i,
  input  logic [MbistAddrSize-1:0]      mbist_addr_i,
  input  logic [MbistDataSize-1:0]      mbist_wdata_i,
  output logic [MbistDataSize-1:0]      mbist_rdata_o,

  // Internal BIST req/rsp
  output logic                          mbist_on_o,
  output bist_req_t                     pipe_bist_req_info_o,
  input  bist_rsp_t                     pipe_bist_rsp_info_i,
  output icache_bist_req_t              icache_bist_req_info_o,
  input  icache_bist_rsp_t              icache_bist_rsp_info_i
);

  // One-hot sel IDs (matching original)
  // localparam RSVD_SEL_ID = 0; // reserved — deselects bist
  localparam int unsigned MbsSelId     = 1;
  localparam int unsigned MbtSelId     = 2;
  localparam int unsigned MbdSelId     = 3;
  localparam int unsigned MbiSelId     = 4;
  localparam int unsigned MbxSelIdSize = 5;

  // Decode mbist_sel to one-hot
  logic [MbxSelIdSize-1:0] mbist_sel_oh;
  always_comb begin
    mbist_sel_oh = '0;
    for (int i = 0; i < MbxSelIdSize; i++)
      mbist_sel_oh[i] = (i == int'(mbist_sel_i)) ? mbist_en_i : 1'b0;
  end

  // mbist_on register (RST_FF pattern)
  logic mbist_on_d;
  assign mbist_on_d = mbist_en_i | dft_mbist_en_i;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) mbist_on_o <= 1'b0;
    else         mbist_on_o <= mbist_on_d;
  end

  assign pipe_bist_req_info_o.mbist_on   = mbist_on_o;
  assign icache_bist_req_info_o.mbist_on = mbist_on_o;

  // ── MBS (tag state RAM) ────────────────────────────────
  logic [MbsDataSize-1:0] mbs_rdata;

  shirecache_bist_mbx #(
    .DataSize(MbsDataSize), .AddrSize(MbsAddrSize),
    .RamDelay(MbsRamDelay), .LogicalRamDepth(MbsLogicalDepth)
  ) u_bist_mbs (
    .clk_i, .rst_ni, .mbx_impl_i(1'b1),
    .mbist_sel_i  (mbist_sel_oh[MbsSelId]),
    .mbist_rd_en_i, .mbist_wr_en_i,
    .mbist_addr_i (mbist_addr_i[MbsAddrSize-1:0]),
    .mbist_wdata_i(mbist_wdata_i[MbsDataSize-1:0]),
    .mbist_rdata_o(mbs_rdata),
    .mbx_sel_o    (pipe_bist_req_info_o.mbs_sel),
    .mbx_rd_en_o  (pipe_bist_req_info_o.mbs_rd_en),
    .mbx_wr_en_o  (pipe_bist_req_info_o.mbs_wr_en),
    .mbx_addr_o   (pipe_bist_req_info_o.mbs_addr),
    .mbx_wdata_o  (pipe_bist_req_info_o.mbs_wdata),
    .mbx_rdata_i  (pipe_bist_rsp_info_i.mbs_rdata)
  );

  // ── MBT (tag RAM) ─────────────────────────────────────
  logic [MbtDataSize-1:0] mbt_rdata;

  shirecache_bist_mbx #(
    .DataSize(MbtDataSize), .AddrSize(MbtAddrSize),
    .RamDelay(MbtRamDelay), .LogicalRamDepth(MbtLogicalDepth)
  ) u_bist_mbt (
    .clk_i, .rst_ni, .mbx_impl_i(1'b1),
    .mbist_sel_i  (mbist_sel_oh[MbtSelId]),
    .mbist_rd_en_i, .mbist_wr_en_i,
    .mbist_addr_i (mbist_addr_i[MbtAddrSize-1:0]),
    .mbist_wdata_i(mbist_wdata_i[MbtDataSize-1:0]),
    .mbist_rdata_o(mbt_rdata),
    .mbx_sel_o    (pipe_bist_req_info_o.mbt_sel),
    .mbx_rd_en_o  (pipe_bist_req_info_o.mbt_rd_en),
    .mbx_wr_en_o  (pipe_bist_req_info_o.mbt_wr_en),
    .mbx_addr_o   (pipe_bist_req_info_o.mbt_addr),
    .mbx_wdata_o  (pipe_bist_req_info_o.mbt_wdata),
    .mbx_rdata_i  (pipe_bist_rsp_info_i.mbt_rdata)
  );

  // ── MBD (data RAM) ─────────────────────────────────────
  logic [MbdDataSize-1:0] mbd_rdata;

  shirecache_bist_mbx #(
    .DataSize(MbdDataSize), .AddrSize(MbdAddrSize),
    .RamDelay(MbdRamDelay), .LogicalRamDepth(MbdLogicalDepth)
  ) u_bist_mbd (
    .clk_i, .rst_ni, .mbx_impl_i(1'b1),
    .mbist_sel_i  (mbist_sel_oh[MbdSelId]),
    .mbist_rd_en_i, .mbist_wr_en_i,
    .mbist_addr_i (mbist_addr_i[MbdAddrSize-1:0]),
    .mbist_wdata_i(mbist_wdata_i[MbdDataSize-1:0]),
    .mbist_rdata_o(mbd_rdata),
    .mbx_sel_o    (pipe_bist_req_info_o.mbd_sel),
    .mbx_rd_en_o  (pipe_bist_req_info_o.mbd_rd_en),
    .mbx_wr_en_o  (pipe_bist_req_info_o.mbd_wr_en),
    .mbx_addr_o   (pipe_bist_req_info_o.mbd_addr),
    .mbx_wdata_o  (pipe_bist_req_info_o.mbd_wdata),
    .mbx_rdata_i  (pipe_bist_rsp_info_i.mbd_rdata)
  );

  // ── MBI (icache RAM) ───────────────────────────────────
  logic [MbiDataSize-1:0] mbi_rdata;

  shirecache_bist_mbx #(
    .DataSize(MbiDataSize), .AddrSize(MbiAddrSize),
    .RamDelay(MbiRamDelay), .LogicalRamDepth(MbiLogicalDepth)
  ) u_bist_mbi (
    .clk_i, .rst_ni, .mbx_impl_i(mbi_implemented_const_i),
    .mbist_sel_i  (mbist_sel_oh[MbiSelId]),
    .mbist_rd_en_i, .mbist_wr_en_i,
    .mbist_addr_i (mbist_addr_i[MbiAddrSize-1:0]),
    .mbist_wdata_i(mbist_wdata_i[MbiDataSize-1:0]),
    .mbist_rdata_o(mbi_rdata),
    .mbx_sel_o    (icache_bist_req_info_o.mbi_sel),
    .mbx_rd_en_o  (icache_bist_req_info_o.mbi_rd_en),
    .mbx_wr_en_o  (icache_bist_req_info_o.mbi_wr_en),
    .mbx_addr_o   (icache_bist_req_info_o.mbi_addr),
    .mbx_wdata_o  (icache_bist_req_info_o.mbi_wdata),
    .mbx_rdata_i  (icache_bist_rsp_info_i.mbi_rdata)
  );

  // Mux read data based on one-hot sel
  assign mbist_rdata_o =
    ({MbistDataSize{mbist_sel_oh[MbsSelId]}} & MbistDataSize'(mbs_rdata)) |
    ({MbistDataSize{mbist_sel_oh[MbtSelId]}} & MbistDataSize'(mbt_rdata)) |
    ({MbistDataSize{mbist_sel_oh[MbdSelId]}} & MbistDataSize'(mbd_rdata)) |
    ({MbistDataSize{mbist_sel_oh[MbiSelId]}} & MbistDataSize'(mbi_rdata));

  logic unused_ok;
  assign unused_ok = &{1'b0,
    mbi_implemented_const_i,
    icache_bist_rsp_info_i,
    mbist_sel_oh[0],  // reserved LSB
    mbist_addr_i,     // upper MSBs may be unused
    unused_ok};

  /* verilator lint_on UNUSEDSIGNAL */

endmodule
