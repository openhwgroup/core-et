// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation: original shire_cache_bist_wrapper vs new shirecache_bist_wrapper.
//
// Both modules receive identical stimulus.  Key outputs are compared
// cycle-by-cycle via scalar ports driven/sampled by C++.
//
// Struct ports (pipe_bist_req/rsp, icache_bist_req/rsp) are decomposed
// to scalars because the old and new type systems cannot be shared.
// Only control signals and low 32 bits of data are compared (the full
// data paths are identical by construction since both use the same mbx).

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off UNUSEDPARAM */
/* verilator lint_off VARHIDDEN */
module cosim_bist_wrapper_tb
  import shirecache_pkg::*;
(
  input  logic                    clk_i,
  input  logic                    rst_ni,

  // ── Shared stimulus inputs ───────────────────────────────────
  input  logic                    mbi_implemented_const_i,
  input  logic                    dft_mbist_en_i,

  input  logic                    mbist_en_i,
  input  logic [MbistSelSize-1:0] mbist_sel_i,
  input  logic                    mbist_rd_en_i,
  input  logic                    mbist_wr_en_i,
  input  logic [MbistAddrSize-1:0] mbist_addr_i,
  input  logic [31:0]             mbist_wdata_lo_i,

  // Response rdata inputs (low 32 bits only, fed to both modules)
  input  logic [31:0]             rsp_mbs_rdata_lo_i,
  input  logic [31:0]             rsp_mbt_rdata_lo_i,
  input  logic [31:0]             rsp_mbd_rdata_lo_i,
  input  logic [31:0]             rsp_mbi_rdata_lo_i,

  // ── Original outputs (scalar decomposition) ──────────────────
  output logic [31:0]             orig_mbist_rdata_lo_o,
  output logic                    orig_mbist_on_o,

  output logic                    orig_mbs_sel_o,
  output logic                    orig_mbs_rd_en_o,
  output logic                    orig_mbs_wr_en_o,
  output logic [MbsAddrSize-1:0]  orig_mbs_addr_o,
  output logic [31:0]             orig_mbs_wdata_lo_o,

  output logic                    orig_mbt_sel_o,
  output logic                    orig_mbt_rd_en_o,
  output logic                    orig_mbt_wr_en_o,
  output logic [MbtAddrSize-1:0]  orig_mbt_addr_o,
  output logic [31:0]             orig_mbt_wdata_lo_o,

  output logic                    orig_mbd_sel_o,
  output logic                    orig_mbd_rd_en_o,
  output logic                    orig_mbd_wr_en_o,
  output logic [MbdAddrSize-1:0]  orig_mbd_addr_o,
  output logic [31:0]             orig_mbd_wdata_lo_o,

  output logic                    orig_mbi_sel_o,
  output logic                    orig_mbi_rd_en_o,
  output logic                    orig_mbi_wr_en_o,
  output logic [MbiAddrSize-1:0]  orig_mbi_addr_o,
  output logic [31:0]             orig_mbi_wdata_lo_o,

  // ── New outputs (scalar decomposition) ───────────────────────
  output logic [31:0]             new_mbist_rdata_lo_o,
  output logic                    new_mbist_on_o,

  output logic                    new_mbs_sel_o,
  output logic                    new_mbs_rd_en_o,
  output logic                    new_mbs_wr_en_o,
  output logic [MbsAddrSize-1:0]  new_mbs_addr_o,
  output logic [31:0]             new_mbs_wdata_lo_o,

  output logic                    new_mbt_sel_o,
  output logic                    new_mbt_rd_en_o,
  output logic                    new_mbt_wr_en_o,
  output logic [MbtAddrSize-1:0]  new_mbt_addr_o,
  output logic [31:0]             new_mbt_wdata_lo_o,

  output logic                    new_mbd_sel_o,
  output logic                    new_mbd_rd_en_o,
  output logic                    new_mbd_wr_en_o,
  output logic [MbdAddrSize-1:0]  new_mbd_addr_o,
  output logic [31:0]             new_mbd_wdata_lo_o,

  output logic                    new_mbi_sel_o,
  output logic                    new_mbi_rd_en_o,
  output logic                    new_mbi_wr_en_o,
  output logic [MbiAddrSize-1:0]  new_mbi_addr_o,
  output logic [31:0]             new_mbi_wdata_lo_o
);

  // ── Build full-width wdata from low 32 bits ───────────────────
  logic [MbistDataSize-1:0] mbist_wdata_full;
  assign mbist_wdata_full = MbistDataSize'(mbist_wdata_lo_i);

  // ══════════════════════════════════════════════════════════════
  // Original module (active-HIGH reset)
  // ══════════════════════════════════════════════════════════════

  // Build rsp structs from scalar inputs
  sc_pipe_bist_rsp_t   orig_pipe_bist_rsp;
  sc_icache_bist_rsp_t orig_icache_bist_rsp;

  assign orig_pipe_bist_rsp.mbs_rdata = `SC_MBS_DATA_SIZE'(rsp_mbs_rdata_lo_i);
  assign orig_pipe_bist_rsp.mbt_rdata = `SC_MBT_DATA_SIZE'(rsp_mbt_rdata_lo_i);
  assign orig_pipe_bist_rsp.mbd_rdata = `SC_MBD_DATA_SIZE'(rsp_mbd_rdata_lo_i);
  assign orig_icache_bist_rsp.mbi_rdata = `SC_MBI_DATA_SIZE'(rsp_mbi_rdata_lo_i);

  // Output structs
  logic [`SC_MBIST_DATA_SIZE-1:0] orig_mbist_rdata;
  logic                           orig_mbist_on;
  sc_pipe_bist_req_t              orig_pipe_bist_req;
  sc_icache_bist_req_t            orig_icache_bist_req;

  shire_cache_bist_wrapper u_orig (
    .clock                (clk_i),
    .reset                (~rst_ni),
    .mbi_implemented_const(mbi_implemented_const_i),
    .dft__mbist_en        (dft_mbist_en_i),

    .mbist_en             (mbist_en_i),
    .mbist_sel            (mbist_sel_i),
    .mbist_rd_en          (mbist_rd_en_i),
    .mbist_wr_en          (mbist_wr_en_i),
    .mbist_addr           (mbist_addr_i),
    .mbist_wdata          (mbist_wdata_full),
    .mbist_rdata          (orig_mbist_rdata),

    .mbist_on             (orig_mbist_on),
    .pipe_bist_req_info   (orig_pipe_bist_req),
    .pipe_bist_rsp_info   (orig_pipe_bist_rsp),
    .icache_bist_req_info (orig_icache_bist_req),
    .icache_bist_rsp_info (orig_icache_bist_rsp)
  );

  // Decompose original outputs to scalars
  assign orig_mbist_rdata_lo_o = orig_mbist_rdata[31:0];
  assign orig_mbist_on_o       = orig_mbist_on;

  assign orig_mbs_sel_o        = orig_pipe_bist_req.mbs_sel;
  assign orig_mbs_rd_en_o      = orig_pipe_bist_req.mbs_rd_en;
  assign orig_mbs_wr_en_o      = orig_pipe_bist_req.mbs_wr_en;
  assign orig_mbs_addr_o       = orig_pipe_bist_req.mbs_addr;
  assign orig_mbs_wdata_lo_o   = orig_pipe_bist_req.mbs_wdata[31:0];

  assign orig_mbt_sel_o        = orig_pipe_bist_req.mbt_sel;
  assign orig_mbt_rd_en_o      = orig_pipe_bist_req.mbt_rd_en;
  assign orig_mbt_wr_en_o      = orig_pipe_bist_req.mbt_wr_en;
  assign orig_mbt_addr_o       = orig_pipe_bist_req.mbt_addr;
  assign orig_mbt_wdata_lo_o   = orig_pipe_bist_req.mbt_wdata[31:0];

  assign orig_mbd_sel_o        = orig_pipe_bist_req.mbd_sel;
  assign orig_mbd_rd_en_o      = orig_pipe_bist_req.mbd_rd_en;
  assign orig_mbd_wr_en_o      = orig_pipe_bist_req.mbd_wr_en;
  assign orig_mbd_addr_o       = orig_pipe_bist_req.mbd_addr;
  assign orig_mbd_wdata_lo_o   = orig_pipe_bist_req.mbd_wdata[31:0];

  assign orig_mbi_sel_o        = orig_icache_bist_req.mbi_sel;
  assign orig_mbi_rd_en_o      = orig_icache_bist_req.mbi_rd_en;
  assign orig_mbi_wr_en_o      = orig_icache_bist_req.mbi_wr_en;
  assign orig_mbi_addr_o       = orig_icache_bist_req.mbi_addr;
  assign orig_mbi_wdata_lo_o   = orig_icache_bist_req.mbi_wdata[31:0];

  // ══════════════════════════════════════════════════════════════
  // New module (active-LOW reset)
  // ══════════════════════════════════════════════════════════════

  // Build rsp structs from scalar inputs
  bist_rsp_t         new_pipe_bist_rsp;
  icache_bist_rsp_t  new_icache_bist_rsp;

  assign new_pipe_bist_rsp.mbs_rdata = MbsDataSize'(rsp_mbs_rdata_lo_i);
  assign new_pipe_bist_rsp.mbt_rdata = MbtDataSize'(rsp_mbt_rdata_lo_i);
  assign new_pipe_bist_rsp.mbd_rdata = MbdDataSize'(rsp_mbd_rdata_lo_i);
  assign new_icache_bist_rsp.mbi_rdata = MbiDataSize'(rsp_mbi_rdata_lo_i);

  // Output structs
  logic [MbistDataSize-1:0] new_mbist_rdata;
  logic                     new_mbist_on;
  bist_req_t                new_pipe_bist_req;
  icache_bist_req_t         new_icache_bist_req;

  shirecache_bist_wrapper u_new (
    .clk_i                (clk_i),
    .rst_ni               (rst_ni),
    .mbi_implemented_const_i(mbi_implemented_const_i),
    .dft_mbist_en_i       (dft_mbist_en_i),

    .mbist_en_i           (mbist_en_i),
    .mbist_sel_i          (mbist_sel_i),
    .mbist_rd_en_i        (mbist_rd_en_i),
    .mbist_wr_en_i        (mbist_wr_en_i),
    .mbist_addr_i         (mbist_addr_i),
    .mbist_wdata_i        (mbist_wdata_full),
    .mbist_rdata_o        (new_mbist_rdata),

    .mbist_on_o           (new_mbist_on),
    .pipe_bist_req_info_o (new_pipe_bist_req),
    .pipe_bist_rsp_info_i (new_pipe_bist_rsp),
    .icache_bist_req_info_o(new_icache_bist_req),
    .icache_bist_rsp_info_i(new_icache_bist_rsp)
  );

  // Decompose new outputs to scalars
  assign new_mbist_rdata_lo_o = new_mbist_rdata[31:0];
  assign new_mbist_on_o       = new_mbist_on;

  assign new_mbs_sel_o        = new_pipe_bist_req.mbs_sel;
  assign new_mbs_rd_en_o      = new_pipe_bist_req.mbs_rd_en;
  assign new_mbs_wr_en_o      = new_pipe_bist_req.mbs_wr_en;
  assign new_mbs_addr_o       = new_pipe_bist_req.mbs_addr;
  assign new_mbs_wdata_lo_o   = new_pipe_bist_req.mbs_wdata[31:0];

  assign new_mbt_sel_o        = new_pipe_bist_req.mbt_sel;
  assign new_mbt_rd_en_o      = new_pipe_bist_req.mbt_rd_en;
  assign new_mbt_wr_en_o      = new_pipe_bist_req.mbt_wr_en;
  assign new_mbt_addr_o       = new_pipe_bist_req.mbt_addr;
  assign new_mbt_wdata_lo_o   = new_pipe_bist_req.mbt_wdata[31:0];

  assign new_mbd_sel_o        = new_pipe_bist_req.mbd_sel;
  assign new_mbd_rd_en_o      = new_pipe_bist_req.mbd_rd_en;
  assign new_mbd_wr_en_o      = new_pipe_bist_req.mbd_wr_en;
  assign new_mbd_addr_o       = new_pipe_bist_req.mbd_addr;
  assign new_mbd_wdata_lo_o   = new_pipe_bist_req.mbd_wdata[31:0];

  assign new_mbi_sel_o        = new_icache_bist_req.mbi_sel;
  assign new_mbi_rd_en_o      = new_icache_bist_req.mbi_rd_en;
  assign new_mbi_wr_en_o      = new_icache_bist_req.mbi_wr_en;
  assign new_mbi_addr_o       = new_icache_bist_req.mbi_addr;
  assign new_mbi_wdata_lo_o   = new_icache_bist_req.mbi_wdata[31:0];

  /* verilator lint_on UNUSEDPARAM */
  /* verilator lint_on VARHIDDEN */
  /* verilator lint_on UNOPTFLAT */
  /* verilator lint_on UNUSEDSIGNAL */

endmodule
