// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Unit test wrapper for rbox_dbg_ram.
// Exposes APB interface and debug RAM/meta ports for C++ test driver.
// ESR response is driven externally (simple loopback stub).

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off WIDTHTRUNC */
/* verilator lint_off WIDTHEXPAND */
module rbox_dbg_ram_tb (
  input  logic        clk_i,
  input  logic        rst_d_ni,
  input  logic        rst_w_ni,

  // APB request (from test)
  input  logic [15:0] apb_paddr_i,
  input  logic        apb_pwrite_i,
  input  logic        apb_psel_i,
  input  logic        apb_penable_i,
  input  logic [63:0] apb_pwdata_i,

  // APB response (to test)
  output logic        apb_pready_o,
  output logic [63:0] apb_prdata_o,
  output logic        apb_pslverr_o,

  // ESR APB passthrough — observe
  output logic [15:0] esr_apb_paddr_o,
  output logic        esr_apb_pwrite_o,
  output logic        esr_apb_psel_o,
  output logic        esr_apb_penable_o,
  output logic [63:0] esr_apb_pwdata_o,

  // ESR APB response — drive from test
  input  logic        esr_apb_pready_i,
  input  logic [63:0] esr_apb_prdata_i,
  input  logic        esr_apb_pslverr_i,

  // Debug RAM pointer
  output logic [6:0]  dbg_ram_ptr_o,

  // Debug data interface
  output logic        dbg_db_read_valid_o,
  output logic        dbg_db_write_valid_o,

  // Debug meta interface
  output logic        dbg_db_meta_read_valid_o,
  output logic        dbg_db_meta_write_valid_o
);

  import apb_pkg::*;
  import rbox_pkg::*;

  apb_pkg::req_t apb_req;
  apb_pkg::rsp_t apb_rsp;
  apb_pkg::req_t esr_apb_req;
  apb_pkg::rsp_t esr_apb_rsp;

  assign apb_req.paddr   = apb_paddr_i;
  assign apb_req.pwrite  = apb_pwrite_i;
  assign apb_req.psel    = apb_psel_i;
  assign apb_req.penable = apb_penable_i;
  assign apb_req.pwdata  = apb_pwdata_i;

  assign apb_pready_o  = apb_rsp.pready;
  assign apb_prdata_o  = apb_rsp.prdata;
  assign apb_pslverr_o = apb_rsp.pslverr;

  assign esr_apb_paddr_o   = esr_apb_req.paddr;
  assign esr_apb_pwrite_o  = esr_apb_req.pwrite;
  assign esr_apb_psel_o    = esr_apb_req.psel;
  assign esr_apb_penable_o = esr_apb_req.penable;
  assign esr_apb_pwdata_o  = esr_apb_req.pwdata;

  assign esr_apb_rsp.pready  = esr_apb_pready_i;
  assign esr_apb_rsp.prdata  = esr_apb_prdata_i;
  assign esr_apb_rsp.pslverr = esr_apb_pslverr_i;

  logic [DepthLineSz-1:0]      dbg_db_read_data;
  logic [DepthLineSz-1:0]      dbg_db_write_data;
  logic [DbgDbMetaDataSz-1:0]  dbg_db_meta_read_data;
  logic [DbgDbMetaDataSz-1:0]  dbg_db_meta_write_data;

  // Tie read data to known patterns
  assign dbg_db_read_data = {8{64'hDEADBEEF_CAFEBABE}};
  assign dbg_db_meta_read_data = '1;

  rbox_dbg_ram u_dut (
    .clk_i                    (clk_i),
    .rst_d_ni                 (rst_d_ni),
    .rst_w_ni                 (rst_w_ni),
    .apb_req_i                (apb_req),
    .apb_rsp_o                (apb_rsp),
    .esr_apb_req_o            (esr_apb_req),
    .esr_apb_rsp_i            (esr_apb_rsp),
    .dbg_ram_ptr_o            (dbg_ram_ptr_o),
    .dbg_db_read_valid_o      (dbg_db_read_valid_o),
    .dbg_db_read_data_i       (dbg_db_read_data),
    .dbg_db_write_data_o      (dbg_db_write_data),
    .dbg_db_write_valid_o     (dbg_db_write_valid_o),
    .dbg_db_meta_read_valid_o (dbg_db_meta_read_valid_o),
    .dbg_db_meta_read_data_i  (dbg_db_meta_read_data),
    .dbg_db_meta_write_data_o (dbg_db_meta_write_data),
    .dbg_db_meta_write_valid_o(dbg_db_meta_write_valid_o)
  );

endmodule
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on WIDTHEXPAND */
