// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original rbox_dbg_ram vs new rbox_dbg_ram.
//
// The original uses RBOX_SHIRE_APB_ADDR_WIDTH=17 (bit 16 = debug flag).
// The new uses apb_pkg::DefaultAddrWidth=16 (bit 15 = debug flag).
// Stimulus uses a 16-bit address where bit 15 is the debug flag.
// For the original, we map: orig_paddr[16:0] = {addr[15], addr[14:0]}
// so bit 16 = addr[15] = debug flag.
// For the new, addr[15:0] directly -> bit 15 = debug flag.

`include "soc.vh"

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off WIDTHEXPAND */
/* verilator lint_off WIDTHTRUNC */
/* verilator lint_off UNOPTFLAT */
/* verilator lint_off BLKSEQ */
/* verilator lint_off PINCONNECTEMPTY */

module cosim_rbox_dbg_ram_tb (
  input  logic        clk_i,
  input  logic        rst_d_ni,
  input  logic        rst_w_ni,

  // ── Shared stimulus: APB request (16-bit addr) ──────────
  input  logic [15:0] apb_paddr_i,
  input  logic        apb_pwrite_i,
  input  logic        apb_psel_i,
  input  logic        apb_penable_i,
  input  logic [63:0] apb_pwdata_i,

  // ── Shared stimulus: ESR response ───────────────────────
  input  logic        esr_pready_i,
  input  logic [63:0] esr_prdata_i,
  input  logic        esr_pslverr_i,

  // ── Shared stimulus: debug read data ────────────────────
  input  logic [511:0] dbg_db_read_data_i,
  input  logic [40:0]  dbg_db_meta_read_data_i,

  // ── New module outputs ──────────────────────────────────
  output logic        new_apb_pready_o,
  output logic [63:0] new_apb_prdata_o,
  output logic        new_apb_pslverr_o,
  output logic [15:0] new_esr_paddr_o,
  output logic        new_esr_pwrite_o,
  output logic        new_esr_psel_o,
  output logic        new_esr_penable_o,
  output logic [63:0] new_esr_pwdata_o,
  output logic [6:0]  new_dbg_ram_ptr_o,
  output logic        new_dbg_db_read_valid_o,
  output logic        new_dbg_db_write_valid_o,
  output logic        new_dbg_db_meta_read_valid_o,
  output logic        new_dbg_db_meta_write_valid_o,

  // ── Original module outputs ─────────────────────────────
  output logic        orig_apb_pready_o,
  output logic [63:0] orig_apb_prdata_o,
  output logic        orig_apb_pslverr_o,
  output logic [15:0] orig_esr_paddr_o,
  output logic        orig_esr_pwrite_o,
  output logic        orig_esr_psel_o,
  output logic        orig_esr_penable_o,
  output logic [63:0] orig_esr_pwdata_o,
  output logic [6:0]  orig_dbg_ram_ptr_o,
  output logic        orig_dbg_db_read_valid_o,
  output logic        orig_dbg_db_write_valid_o,
  output logic        orig_dbg_db_meta_read_valid_o,
  output logic        orig_dbg_db_meta_write_valid_o
);

  import apb_pkg::*;
  import rbox_pkg::*;

  // ── Reset adaptation ────────────────────────────────────
  logic reset_d;
  logic reset_w;
  assign reset_d = ~rst_d_ni;
  assign reset_w = ~rst_w_ni;

  // ── APB struct for original ─────────────────────────────
  APB_to_rbox_ESRs_t  orig_apb_req;
  APB_from_rbox_ESRs_t orig_apb_rsp;

  // 17-bit address: bit 16 = debug flag = apb_paddr_i[15], bit 15 = 0, bits[14:0] = addr[14:0]
  assign orig_apb_req.apb_paddr   = {apb_paddr_i[15], 1'b0, apb_paddr_i[14:0]};
  assign orig_apb_req.apb_pwrite  = apb_pwrite_i;
  assign orig_apb_req.apb_psel    = apb_psel_i;
  assign orig_apb_req.apb_penable = apb_penable_i;
  assign orig_apb_req.apb_pwdata  = apb_pwdata_i;

  assign orig_apb_pready_o  = orig_apb_rsp.apb_pready;
  assign orig_apb_prdata_o  = orig_apb_rsp.apb_prdata;
  assign orig_apb_pslverr_o = orig_apb_rsp.apb_pslverr;

  // ── APB for new module ──────────────────────────────────
  apb_pkg::req_t new_apb_req;
  apb_pkg::rsp_t new_apb_rsp;
  apb_pkg::req_t new_esr_apb_req;
  apb_pkg::rsp_t new_esr_apb_rsp;

  assign new_apb_req.paddr   = apb_paddr_i;
  assign new_apb_req.pwrite  = apb_pwrite_i;
  assign new_apb_req.psel    = apb_psel_i;
  assign new_apb_req.penable = apb_penable_i;
  assign new_apb_req.pwdata  = apb_pwdata_i;

  assign new_apb_pready_o  = new_apb_rsp.pready;
  assign new_apb_prdata_o  = new_apb_rsp.prdata;
  assign new_apb_pslverr_o = new_apb_rsp.pslverr;

  assign new_esr_paddr_o   = new_esr_apb_req.paddr;
  assign new_esr_pwrite_o  = new_esr_apb_req.pwrite;
  assign new_esr_psel_o    = new_esr_apb_req.psel;
  assign new_esr_penable_o = new_esr_apb_req.penable;
  assign new_esr_pwdata_o  = new_esr_apb_req.pwdata;

  assign new_esr_apb_rsp.pready  = esr_pready_i;
  assign new_esr_apb_rsp.prdata  = esr_prdata_i;
  assign new_esr_apb_rsp.pslverr = esr_pslverr_i;

  // ── Original outputs (flat) ─────────────────────────────
  logic [`ESR_RBOX_APB_AD_PP_WIDTH-1:0] orig_esr_paddr_raw;
  assign orig_esr_paddr_o = orig_esr_paddr_raw[15:0];

  // ── Original instance (inlined as rbox_dbg_ram_orig) ────
  rbox_dbg_ram_orig u_orig (
    .clock                   (clk_i),
    .reset_d                 (reset_d),
    .reset_w                 (reset_w),
    .test_en                 (1'b0),
    .APB_ESR_req             (orig_apb_req),
    .APB_ESR_rsp             (orig_apb_rsp),
    .apb_req_to_esr_paddr    (orig_esr_paddr_raw),
    .apb_req_to_esr_pwrite   (orig_esr_pwrite_o),
    .apb_req_to_esr_psel     (orig_esr_psel_o),
    .apb_req_to_esr_penable  (orig_esr_penable_o),
    .apb_req_to_esr_pwdata   (orig_esr_pwdata_o),
    .apb_rsp_from_esr_pready (esr_pready_i),
    .apb_rsp_from_esr_prdata (esr_prdata_i),
    .apb_rsp_from_esr_pslverr(esr_pslverr_i),
    .dbg_ram_ptr             (orig_dbg_ram_ptr_o),
    .dbg_db_read_valid       (orig_dbg_db_read_valid_o),
    .dbg_db_read_data        (dbg_db_read_data_i),
    .dbg_db_write_data       (),
    .dbg_db_write_valid      (orig_dbg_db_write_valid_o),
    .dbg_db_meta_read_valid  (orig_dbg_db_meta_read_valid_o),
    .dbg_db_meta_read_data   (dbg_db_meta_read_data_i),
    .dbg_db_meta_write_data  (),
    .dbg_db_meta_write_valid (orig_dbg_db_meta_write_valid_o)
  );

  // ── New instance ────────────────────────────────────────
  logic [DepthLineSz-1:0]     new_dbg_db_write_data;
  logic [DbgDbMetaDataSz-1:0] new_dbg_db_meta_write_data;

  rbox_dbg_ram u_new (
    .clk_i                    (clk_i),
    .rst_d_ni                 (rst_d_ni),
    .rst_w_ni                 (rst_w_ni),
    .apb_req_i                (new_apb_req),
    .apb_rsp_o                (new_apb_rsp),
    .esr_apb_req_o            (new_esr_apb_req),
    .esr_apb_rsp_i            (new_esr_apb_rsp),
    .dbg_ram_ptr_o            (new_dbg_ram_ptr_o),
    .dbg_db_read_valid_o      (new_dbg_db_read_valid_o),
    .dbg_db_read_data_i       (dbg_db_read_data_i),
    .dbg_db_write_data_o      (new_dbg_db_write_data),
    .dbg_db_write_valid_o     (new_dbg_db_write_valid_o),
    .dbg_db_meta_read_valid_o (new_dbg_db_meta_read_valid_o),
    .dbg_db_meta_read_data_i  (dbg_db_meta_read_data_i),
    .dbg_db_meta_write_data_o (new_dbg_db_meta_write_data),
    .dbg_db_meta_write_valid_o(new_dbg_db_meta_write_valid_o)
  );

endmodule

// ════════════════════════════════════════════════════════════
// Original rbox_dbg_ram inlined with _orig suffix.
// This is the original CORE-ET Verilog, renamed to avoid
// name collision with the new SystemVerilog rbox_dbg_ram.
// ════════════════════════════════════════════════════════════

/* verilator lint_off MULTIDRIVEN */
/* verilator lint_off BLKSEQ */
/* verilator lint_off CASEINCOMPLETE */

module rbox_dbg_ram_orig
(
    input  logic                                 clock,
    input  logic                                 reset_d,
    input  logic                                 reset_w,
    input  logic                                 test_en,

    input   APB_to_rbox_ESRs_t                   APB_ESR_req,
    output  APB_from_rbox_ESRs_t                 APB_ESR_rsp,

    output logic [`ESR_RBOX_APB_AD_PP_WIDTH-1:0] apb_req_to_esr_paddr,
    output logic                                 apb_req_to_esr_pwrite,
    output logic                                 apb_req_to_esr_psel,
    output logic                                 apb_req_to_esr_penable,
    output logic [`ESR_APB_D_WIDTH-1:0]          apb_req_to_esr_pwdata,

    input  logic                                 apb_rsp_from_esr_pready,
    input  logic [`ESR_APB_D_WIDTH-1:0]          apb_rsp_from_esr_prdata,
    input  logic                                 apb_rsp_from_esr_pslverr,

    output logic [`RBOX_DBG_PTR_R]               dbg_ram_ptr,

    output logic                                 dbg_db_read_valid,
    input  logic [`RBOX_DEPTH_LINE_R]            dbg_db_read_data,

    output logic [`RBOX_DEPTH_LINE_R]            dbg_db_write_data,
    output logic                                 dbg_db_write_valid,

    output logic                                 dbg_db_meta_read_valid,
    input  logic [`RBOX_DBG_DB_META_DATA_R]      dbg_db_meta_read_data,

    output logic [`RBOX_DBG_DB_META_DATA_R]      dbg_db_meta_write_data,
    output logic                                 dbg_db_meta_write_valid
);

`define RBOX_APB_DBG_DEC_R 13:9
`define RBOX_APB_DBG_DEPTH_BUF_META 5'b000??
`define RBOX_APB_DBG_DEPTH_BUF_DATA 5'b001??
`define RBOX_APB_DBG_DEPTH_BUF_META_I_R 5:0
`define RBOX_APB_DBG_DEPTH_BUF_DATA_I_R 8:3
`define RBOX_APB_DBG_DEPTH_BUF_META_DATA_R 40:0

logic                             d0_dbg_access;
logic                             d0_dbg_error;
logic [`RBOX_DBG_PTR_R]           d0_dbg_ram_ptr;
logic [`RBOX_DBG_MAX_DATA_QW_I_R] d0_dbg_ram_qw;
logic                             d0_dbg_db_wr_valid;
logic                             d0_dbg_db_rd_valid;
logic                             d0_dbg_db_meta_wr_valid;
logic                             d0_dbg_db_meta_rd_valid;
logic [`RBOX_DBG_MAX_DATA_QW_R]   d0_dbg_write_buf_en;

always_comb
begin
    d0_dbg_access = APB_ESR_req.apb_psel && !APB_ESR_req.apb_penable && APB_ESR_req.apb_paddr[`RBOX_SHIRE_APB_ADDR_WIDTH-1];

    d0_dbg_ram_ptr          = '0;
    d0_dbg_ram_qw           = '0;
    d0_dbg_write_buf_en     = '0;
    d0_dbg_db_wr_valid      = 1'b0;
    d0_dbg_db_rd_valid      = 1'b0;
    d0_dbg_db_meta_wr_valid = 1'b0;
    d0_dbg_db_meta_rd_valid = 1'b0;
    d0_dbg_error            = 1'b1;

    casez (APB_ESR_req.apb_paddr[`RBOX_APB_DBG_DEC_R])
        `RBOX_APB_DBG_DEPTH_BUF_DATA :
            begin
                d0_dbg_ram_ptr = `ZX(`RBOX_DBG_PTR_SZ, APB_ESR_req.apb_paddr[`RBOX_APB_DBG_DEPTH_BUF_META_I_R]);
                d0_dbg_ram_qw  = `ZX(`RBOX_DBG_MAX_DATA_QW_I_SZ, APB_ESR_req.apb_paddr[`RBOX_DEPTH_LINE_QW_I_R]);
                d0_dbg_db_wr_valid =  APB_ESR_req.apb_pwrite;
                d0_dbg_db_rd_valid = ~APB_ESR_req.apb_pwrite;
                d0_dbg_write_buf_en[d0_dbg_ram_qw] = 1'b1;
                d0_dbg_error = 1'b0;
            end
        `RBOX_APB_DBG_DEPTH_BUF_META :
            begin
                d0_dbg_ram_ptr = `ZX(`RBOX_DBG_PTR_SZ, APB_ESR_req.apb_paddr[`RBOX_DEPTH_BUF_I_R]);
                d0_dbg_db_meta_wr_valid =  APB_ESR_req.apb_pwrite;
                d0_dbg_db_meta_rd_valid = ~APB_ESR_req.apb_pwrite;
                d0_dbg_error = 1'b0;
            end
    endcase
end

logic                             d1_dbg_access_ff;
logic                             d1_dbg_error_ff;
logic [`RBOX_DBG_PTR_R]           d1_dbg_ram_ptr_ff;
logic [`RBOX_DBG_MAX_DATA_QW_I_R] d1_dbg_ram_qw_ff;
logic                             d1_dbg_db_wr_valid_ff;
logic                             d1_dbg_db_rd_valid_ff;
logic                             d1_dbg_db_meta_wr_valid_ff;
logic                             d1_dbg_db_meta_rd_valid_ff;
logic [`RBOX_DBG_MAX_DATA_QW_R]   d1_dbg_write_buf_en_ff;

`RST_FF(clock, reset_d, d1_dbg_access_ff, d0_dbg_access, 1'b0)

`RST_EN_FF(clock, reset_d, d0_dbg_access, d1_dbg_error_ff,            d0_dbg_error,            1'b0)
`RST_EN_FF(clock, reset_d, d0_dbg_access, d1_dbg_ram_ptr_ff,          d0_dbg_ram_ptr,          '0  )
`RST_EN_FF(clock, reset_d, d0_dbg_access, d1_dbg_ram_qw_ff,           d0_dbg_ram_qw,           '0  )
`RST_EN_FF(clock, reset_d, d0_dbg_access, d1_dbg_db_wr_valid_ff,      d0_dbg_db_wr_valid,      1'b0)
`RST_EN_FF(clock, reset_d, d0_dbg_access, d1_dbg_db_rd_valid_ff,      d0_dbg_db_rd_valid,      1'b0)
`RST_EN_FF(clock, reset_d, d0_dbg_access, d1_dbg_db_meta_wr_valid_ff, d0_dbg_db_meta_wr_valid, 1'b0)
`RST_EN_FF(clock, reset_d, d0_dbg_access, d1_dbg_db_meta_rd_valid_ff, d0_dbg_db_meta_rd_valid, 1'b0)
`RST_EN_FF(clock, reset_d, d0_dbg_access, d1_dbg_write_buf_en_ff,     d0_dbg_write_buf_en,     '0  )

logic [`RBOX_DBG_MAX_DATA_QW_R][63:0] d2_dbg_write_buf_ff;
logic [`RBOX_DBG_MAX_DATA_QW_R]       d1_dbg_write_buf_en;

logic                               d1_dbg_access_phase1;
logic [`RBOX_DEPTH_LINE_QW_R][63:0] d1_dbg_db_wr_data;

always_comb
begin
    dbg_ram_ptr       = d1_dbg_ram_ptr_ff;

    dbg_db_read_valid = d1_dbg_db_rd_valid_ff && APB_ESR_req.apb_penable;

    d1_dbg_db_wr_data = d2_dbg_write_buf_ff;
    d1_dbg_db_wr_data[d1_dbg_ram_qw_ff] = APB_ESR_req.apb_pwdata;

    dbg_db_write_valid = d1_dbg_db_wr_valid_ff && APB_ESR_req.apb_penable;
    dbg_db_write_data  = d1_dbg_db_wr_data;

    dbg_db_meta_read_valid = d1_dbg_db_meta_rd_valid_ff && APB_ESR_req.apb_penable;

    dbg_db_meta_write_valid = d1_dbg_db_meta_wr_valid_ff && APB_ESR_req.apb_penable;
    dbg_db_meta_write_data  = APB_ESR_req.apb_pwdata[`RBOX_DBG_DB_META_DATA_R];

    d1_dbg_access_phase1 = d1_dbg_access_ff && APB_ESR_req.apb_penable;

    d1_dbg_write_buf_en = d1_dbg_write_buf_en_ff & {`RBOX_DBG_MAX_DATA_QW{APB_ESR_req.apb_penable && APB_ESR_req.apb_pwrite}};
end

genvar gv_qw;
generate
    for (gv_qw = 0; gv_qw < `RBOX_DBG_MAX_DATA_QW; gv_qw++)
        `RST_EN_FF(clock, reset_d, d1_dbg_write_buf_en[gv_qw], d2_dbg_write_buf_ff[gv_qw], APB_ESR_req.apb_pwdata, '0);
endgenerate

logic                             d2_dbg_access_ff;
logic                             d2_dbg_error_ff;
logic [`RBOX_DBG_MAX_DATA_QW_I_R] d2_dbg_ram_qw_ff;
logic                             d2_dbg_db_rd_valid_ff;
logic                             d2_dbg_db_meta_rd_valid_ff;

`RST_FF(clock, reset_d, d2_dbg_access_ff, d1_dbg_access_phase1, 1'b0)

`RST_EN_FF(clock, reset_d, d1_dbg_access_phase1, d2_dbg_error_ff,            d1_dbg_error_ff,            1'b0)
`RST_EN_FF(clock, reset_d, d1_dbg_access_phase1, d2_dbg_db_rd_valid_ff,      d1_dbg_db_rd_valid_ff,      1'b0)
`RST_EN_FF(clock, reset_d, d1_dbg_access_phase1, d2_dbg_ram_qw_ff,           d1_dbg_ram_qw_ff,           '0  )
`RST_EN_FF(clock, reset_d, d1_dbg_access_phase1, d2_dbg_db_meta_rd_valid_ff, d1_dbg_db_meta_rd_valid_ff, 1'b0)

logic [63:0] d3_dbg_db_read_data_nxt;
logic        d3_dbg_db_read_data_en;

always_comb
begin
    case (1'b1)
        d2_dbg_db_rd_valid_ff      : {d3_dbg_db_read_data_en, d3_dbg_db_read_data_nxt} = {1'b1, dbg_db_read_data[d2_dbg_ram_qw_ff[`RBOX_DEPTH_LINE_QW_I_R]]};
        d2_dbg_db_meta_rd_valid_ff : {d3_dbg_db_read_data_en, d3_dbg_db_read_data_nxt} = {1'b1, `ZX(`ESR_APB_D_WIDTH, dbg_db_meta_read_data)};
        default                    : {d3_dbg_db_read_data_en, d3_dbg_db_read_data_nxt} = {apb_rsp_from_esr_pready, apb_rsp_from_esr_prdata};
    endcase
end

logic [63:0] d3_dbg_db_read_data_ff;
logic        d3_dbg_access_ff;
logic        d3_dbg_error_ff;

`RST_FF(clock, reset_d, d3_dbg_access_ff, d2_dbg_access_ff, 1'b0)
`RST_FF(clock, reset_d, d3_dbg_error_ff,  d2_dbg_error_ff,  1'b0)

logic apb_rsp_from_esr_pready_ff;
logic apb_rsp_from_esr_pready_nxt;
logic apb_rsp_from_esr_pready_en;
logic apb_rsp_from_esr_pslverr_ff;
logic apb_rsp_from_esr_pslverr_nxt;
logic apb_rsp_from_esr_pslverr_en;

always_comb
begin
    apb_rsp_from_esr_pready_en  = reset_w || reset_d || (apb_rsp_from_esr_pready_ff  ^ apb_rsp_from_esr_pready );
    apb_rsp_from_esr_pslverr_en = reset_w || reset_d || (apb_rsp_from_esr_pslverr_ff ^ apb_rsp_from_esr_pslverr);

    apb_rsp_from_esr_pready_nxt  = !reset_w && !reset_d && apb_rsp_from_esr_pready;
    apb_rsp_from_esr_pslverr_nxt = !reset_w && !reset_d && apb_rsp_from_esr_pslverr;
end

`EN_FF(clock, d3_dbg_db_read_data_en, d3_dbg_db_read_data_ff, d3_dbg_db_read_data_nxt)

`EN_FF(clock, apb_rsp_from_esr_pready_en,  apb_rsp_from_esr_pready_ff,  apb_rsp_from_esr_pready_nxt )
`EN_FF(clock, apb_rsp_from_esr_pslverr_en, apb_rsp_from_esr_pslverr_ff, apb_rsp_from_esr_pslverr_nxt)

always_comb
begin
    APB_ESR_rsp.apb_pready  = d3_dbg_access_ff || (apb_rsp_from_esr_pready_ff && !d0_dbg_access && !d1_dbg_access_ff && !d2_dbg_access_ff);
    APB_ESR_rsp.apb_prdata  = d3_dbg_db_read_data_ff;
    APB_ESR_rsp.apb_pslverr =  d3_dbg_access_ff && d3_dbg_error_ff
                            || (apb_rsp_from_esr_pslverr_ff && !d0_dbg_access && !d1_dbg_access_ff && !d2_dbg_access_ff && !d3_dbg_access_ff);
end

always_comb
begin
    apb_req_to_esr_paddr   = APB_ESR_req.apb_paddr[`ESR_RBOX_APB_AD_PP_WIDTH-1:0];
    apb_req_to_esr_pwrite  = APB_ESR_req.apb_pwrite;
    apb_req_to_esr_psel    = APB_ESR_req.apb_psel    && !APB_ESR_req.apb_paddr[`RBOX_SHIRE_APB_ADDR_WIDTH-1];
    apb_req_to_esr_penable = APB_ESR_req.apb_penable && !APB_ESR_req.apb_paddr[`RBOX_SHIRE_APB_ADDR_WIDTH-1];
    apb_req_to_esr_pwdata  = APB_ESR_req.apb_pwdata;
end

wire unused_ok = &{
  `ifndef ET_ASCENT_LINT
   1'b0,
   `endif
   test_en
};

endmodule

/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on UNOPTFLAT */
