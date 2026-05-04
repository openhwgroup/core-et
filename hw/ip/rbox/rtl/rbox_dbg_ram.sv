// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// RBOX debug RAM interface — APB address decode mux.
//
// Routes APB transactions between the ESR register block and the depth
// buffer debug interface. High addresses (bit 15 set) are decoded as
// debug accesses to depth buffer data or metadata. Low addresses pass
// through to the ESR register block.
//
// The debug path is a 3-stage pipeline:
//   D0: Address decode — determine debug vs ESR path, extract pointers
//   D1: Access phase — drive read/write valid to depth buffer
//   D2: Capture response — latch read data from depth buffer or ESR
//
// Reset domains (matching original rbox_dbg_ram.v):
//   - rst_d_ni (debug reset): ALL debug pipeline FFs (d0-d3 stages)
//   - rst_w_ni (warm reset): used ONLY in the ESR pready/pslverr tracking
//     FFs, combined with rst_d_ni (the original uses reset_w || reset_d)
//
// Differences from original (rbox_dbg_ram.v):
//   - Reset: active-low async-assert/sync-deassert (rst_d_ni, rst_w_ni)
//     instead of active-high synchronous (reset_d, reset_w)
//   - Naming: lowRISC _i/_o/_d/_q conventions
//   - Types/constants: apb_pkg/rbox_pkg instead of `include "soc.vh"
//   - `define address decode constants converted to localparam
//   - Flip-flop macros replaced with explicit always_ff

/* verilator lint_off UNUSEDSIGNAL */  // APB struct fields not all used (paddr upper bits)
/* verilator lint_off WIDTHTRUNC */    // Bit-select on address fields for pointer extraction
/* verilator lint_off WIDTHEXPAND */   // Zero-extension of debug metadata to 64-bit APB data

module rbox_dbg_ram
  import apb_pkg::*;
  import rbox_pkg::*;
(
  input  logic          clk_i,
  input  logic          rst_d_ni,   // Debug reset (active-low async)
  input  logic          rst_w_ni,   // Warm reset (active-low async)

  // APB interface (from Shire channel)
  input  apb_pkg::req_t   apb_req_i,
  output apb_pkg::rsp_t   apb_rsp_o,

  // APB passthrough to ESR register block
  output apb_pkg::req_t   esr_apb_req_o,
  input  apb_pkg::rsp_t   esr_apb_rsp_i,

  // Debug RAM pointer (shared with depth buffer)
  output logic [DbgPtrIdxSz-1:0]    dbg_ram_ptr_o,

  // Debug depth buffer data interface
  output logic                       dbg_db_read_valid_o,
  input  logic [DepthLineSz-1:0]     dbg_db_read_data_i,

  output logic [DepthLineSz-1:0]     dbg_db_write_data_o,
  output logic                       dbg_db_write_valid_o,

  // Debug depth buffer metadata interface
  output logic                       dbg_db_meta_read_valid_o,
  input  logic [DbgDbMetaDataSz-1:0] dbg_db_meta_read_data_i,

  output logic [DbgDbMetaDataSz-1:0] dbg_db_meta_write_data_o,
  output logic                       dbg_db_meta_write_valid_o
);

  // ── Local address decode constants ──────────────────────
  // Original used RBOX_SHIRE_APB_ADDR_WIDTH = 17, so bit [16] is the debug flag.
  // In our APB pkg, DefaultAddrWidth = 16, which means paddr is [15:0].
  // Use bit [15] as the debug decode bit (highest bit of 16-bit addr space).
  localparam int unsigned DbgAddrBit = apb_pkg::DefaultAddrWidth - 1; // bit 15

  // Address field decode ranges [13:9]
  localparam int unsigned DbgDecHi = 13;
  localparam int unsigned DbgDecLo = 9;

  // Index ranges for pointer extraction
  // For data access: meta_i range [5:0] for ram_ptr, line_qw range for qw select
  localparam int unsigned DbgMetaIdxHi = 5;
  localparam int unsigned DbgMetaIdxLo = 0;
  localparam int unsigned DbgDataIdxLo = 3;

  // ── D0: Address decode ──────────────────────────────────

  logic                             d0_dbg_access;
  logic                             d0_dbg_error;
  logic [DbgPtrIdxSz-1:0]          d0_dbg_ram_ptr;
  logic [DbgMaxDataQwIdxSz-1:0]    d0_dbg_ram_qw;
  logic                             d0_dbg_db_wr_valid;
  logic                             d0_dbg_db_rd_valid;
  logic                             d0_dbg_db_meta_wr_valid;
  logic                             d0_dbg_db_meta_rd_valid;
  logic [DbgMaxDataQw-1:0]         d0_dbg_write_buf_en;

  always_comb begin
    d0_dbg_access = apb_req_i.psel && !apb_req_i.penable && apb_req_i.paddr[DbgAddrBit];

    d0_dbg_ram_ptr          = '0;
    d0_dbg_ram_qw           = '0;
    d0_dbg_write_buf_en     = '0;
    d0_dbg_db_wr_valid      = 1'b0;
    d0_dbg_db_rd_valid      = 1'b0;
    d0_dbg_db_meta_wr_valid = 1'b0;
    d0_dbg_db_meta_rd_valid = 1'b0;
    d0_dbg_error            = 1'b1;

    casez (apb_req_i.paddr[DbgDecHi:DbgDecLo])
      5'b001?? : begin
        d0_dbg_ram_ptr = DbgPtrIdxSz'(apb_req_i.paddr[DbgMetaIdxHi:DbgMetaIdxLo]);
        d0_dbg_ram_qw  = DbgMaxDataQwIdxSz'(apb_req_i.paddr[DepthLineQwIdxSz+DbgDataIdxLo-1:DbgDataIdxLo]);
        d0_dbg_db_wr_valid =  apb_req_i.pwrite;
        d0_dbg_db_rd_valid = ~apb_req_i.pwrite;
        d0_dbg_write_buf_en[d0_dbg_ram_qw] = 1'b1;
        d0_dbg_error = 1'b0;
      end
      5'b000?? : begin
        d0_dbg_ram_ptr = DbgPtrIdxSz'(apb_req_i.paddr[DepthBufIdxSz-1:0]);
        d0_dbg_db_meta_wr_valid =  apb_req_i.pwrite;
        d0_dbg_db_meta_rd_valid = ~apb_req_i.pwrite;
        d0_dbg_error = 1'b0;
      end
      default : ;
    endcase
  end

  // ── D1: Pipeline stage 1 registers (reset_d domain) ─────

  logic                             d1_dbg_access_q;
  logic                             d1_dbg_error_q;
  logic [DbgPtrIdxSz-1:0]          d1_dbg_ram_ptr_q;
  logic [DbgMaxDataQwIdxSz-1:0]    d1_dbg_ram_qw_q;
  logic                             d1_dbg_db_wr_valid_q;
  logic                             d1_dbg_db_rd_valid_q;
  logic                             d1_dbg_db_meta_wr_valid_q;
  logic                             d1_dbg_db_meta_rd_valid_q;
  logic [DbgMaxDataQw-1:0]         d1_dbg_write_buf_en_q;

  always_ff @(posedge clk_i or negedge rst_d_ni) begin
    if (!rst_d_ni) begin
      d1_dbg_access_q          <= 1'b0;
      d1_dbg_error_q           <= 1'b0;
      d1_dbg_ram_ptr_q         <= '0;
      d1_dbg_ram_qw_q          <= '0;
      d1_dbg_db_wr_valid_q     <= 1'b0;
      d1_dbg_db_rd_valid_q     <= 1'b0;
      d1_dbg_db_meta_wr_valid_q <= 1'b0;
      d1_dbg_db_meta_rd_valid_q <= 1'b0;
      d1_dbg_write_buf_en_q    <= '0;
    end else begin
      d1_dbg_access_q <= d0_dbg_access;
      if (d0_dbg_access) begin
        d1_dbg_error_q           <= d0_dbg_error;
        d1_dbg_ram_ptr_q         <= d0_dbg_ram_ptr;
        d1_dbg_ram_qw_q          <= d0_dbg_ram_qw;
        d1_dbg_db_wr_valid_q     <= d0_dbg_db_wr_valid;
        d1_dbg_db_rd_valid_q     <= d0_dbg_db_rd_valid;
        d1_dbg_db_meta_wr_valid_q <= d0_dbg_db_meta_wr_valid;
        d1_dbg_db_meta_rd_valid_q <= d0_dbg_db_meta_rd_valid;
        d1_dbg_write_buf_en_q    <= d0_dbg_write_buf_en;
      end
    end
  end

  // ── D1: Write buffer ────────────────────────────────────

  logic [DbgMaxDataQw-1:0][63:0]   d2_dbg_write_buf_q;
  logic [DbgMaxDataQw-1:0]         d1_dbg_write_buf_en;

  logic                             d1_dbg_access_phase1;
  logic [DepthLineQw-1:0][63:0]    d1_dbg_db_wr_data;

  always_comb begin
    dbg_ram_ptr_o = d1_dbg_ram_ptr_q;

    dbg_db_read_valid_o = d1_dbg_db_rd_valid_q && apb_req_i.penable;

    d1_dbg_db_wr_data = d2_dbg_write_buf_q[DepthLineQw-1:0];
    d1_dbg_db_wr_data[d1_dbg_ram_qw_q[DepthLineQwIdxSz-1:0]] = apb_req_i.pwdata;

    dbg_db_write_valid_o = d1_dbg_db_wr_valid_q && apb_req_i.penable;
    dbg_db_write_data_o  = d1_dbg_db_wr_data;

    dbg_db_meta_read_valid_o = d1_dbg_db_meta_rd_valid_q && apb_req_i.penable;

    dbg_db_meta_write_valid_o = d1_dbg_db_meta_wr_valid_q && apb_req_i.penable;
    dbg_db_meta_write_data_o  = apb_req_i.pwdata[DbgDbMetaDataSz-1:0];

    d1_dbg_access_phase1 = d1_dbg_access_q && apb_req_i.penable;

    d1_dbg_write_buf_en = d1_dbg_write_buf_en_q & {DbgMaxDataQw{apb_req_i.penable && apb_req_i.pwrite}};
  end

  // Write buffer registers (per-QW enable, reset_d domain)
  for (genvar gv_qw = 0; gv_qw < DbgMaxDataQw; gv_qw++) begin : gen_write_buf
    always_ff @(posedge clk_i or negedge rst_d_ni) begin
      if (!rst_d_ni) begin
        d2_dbg_write_buf_q[gv_qw] <= '0;
      end else if (d1_dbg_write_buf_en[gv_qw]) begin
        d2_dbg_write_buf_q[gv_qw] <= apb_req_i.pwdata;
      end
    end
  end

  // ── D2: Pipeline stage 2 registers (reset_d domain) ─────

  logic                             d2_dbg_access_q;
  logic                             d2_dbg_error_q;
  logic [DbgMaxDataQwIdxSz-1:0]    d2_dbg_ram_qw_q;
  logic                             d2_dbg_db_rd_valid_q;
  logic                             d2_dbg_db_meta_rd_valid_q;

  always_ff @(posedge clk_i or negedge rst_d_ni) begin
    if (!rst_d_ni) begin
      d2_dbg_access_q          <= 1'b0;
      d2_dbg_error_q           <= 1'b0;
      d2_dbg_ram_qw_q          <= '0;
      d2_dbg_db_rd_valid_q     <= 1'b0;
      d2_dbg_db_meta_rd_valid_q <= 1'b0;
    end else begin
      d2_dbg_access_q <= d1_dbg_access_phase1;
      if (d1_dbg_access_phase1) begin
        d2_dbg_error_q           <= d1_dbg_error_q;
        d2_dbg_db_rd_valid_q     <= d1_dbg_db_rd_valid_q;
        d2_dbg_ram_qw_q          <= d1_dbg_ram_qw_q;
        d2_dbg_db_meta_rd_valid_q <= d1_dbg_db_meta_rd_valid_q;
      end
    end
  end

  // ── D2: Read data mux ──────────────────────────────────

  // Replicate the original's exact concatenation pattern for the read data mux.
  // The original uses: {en, data} = {1'b1, dbg_db_read_data[index]}
  // Since dbg_db_read_data[index] is a 1-bit select from a flat 512-bit vector,
  // the RHS {1'b1, 1'bit} = 2 bits, zero-extended to 65 bits. This means
  // en = 0 (MSB) and data = {62'b0, 1'b1, bit}. The debug data read path
  // in the original is effectively broken (enable is always 0), but we
  // replicate it exactly for functional equivalence.

  logic [63:0] d3_dbg_db_read_data_d;
  logic        d3_dbg_db_read_data_en;

  always_comb begin
    case (1'b1)
      d2_dbg_db_rd_valid_q      : {d3_dbg_db_read_data_en, d3_dbg_db_read_data_d} = {1'b1, dbg_db_read_data_i[d2_dbg_ram_qw_q[DepthLineQwIdxSz-1:0]]};
      d2_dbg_db_meta_rd_valid_q : {d3_dbg_db_read_data_en, d3_dbg_db_read_data_d} = {1'b1, 64'(dbg_db_meta_read_data_i)};
      default                   : {d3_dbg_db_read_data_en, d3_dbg_db_read_data_d} = {esr_apb_rsp_i.pready, esr_apb_rsp_i.prdata};
    endcase
  end

  // ── D3: Read data + access/error registers (reset_d domain) ──

  logic [63:0] d3_dbg_db_read_data_q;
  logic        d3_dbg_access_q;
  logic        d3_dbg_error_q;

  always_ff @(posedge clk_i or negedge rst_d_ni) begin
    if (!rst_d_ni) begin
      d3_dbg_access_q <= 1'b0;
      d3_dbg_error_q  <= 1'b0;
    end else begin
      d3_dbg_access_q <= d2_dbg_access_q;
      d3_dbg_error_q  <= d2_dbg_error_q;
    end
  end

  always_ff @(posedge clk_i) begin
    if (d3_dbg_db_read_data_en)
      d3_dbg_db_read_data_q <= d3_dbg_db_read_data_d;
  end

  // ── ESR response tracking (reset_w || reset_d domain) ──
  // The original uses: enable = reset_w || reset_d || (ff ^ input)
  //                    next   = !reset_w && !reset_d && input
  // This means EITHER reset clears these FFs to 0.
  // We replicate with active-low: (!rst_w_ni || !rst_d_ni) = reset active.

  logic esr_pready_q;
  logic esr_pready_d;
  logic esr_pready_en;
  logic esr_pslverr_q;
  logic esr_pslverr_d;
  logic esr_pslverr_en;

  always_comb begin
    esr_pready_en  = (!rst_w_ni || !rst_d_ni) || (esr_pready_q  ^ esr_apb_rsp_i.pready);
    esr_pslverr_en = (!rst_w_ni || !rst_d_ni) || (esr_pslverr_q ^ esr_apb_rsp_i.pslverr);

    esr_pready_d  = rst_w_ni && rst_d_ni && esr_apb_rsp_i.pready;
    esr_pslverr_d = rst_w_ni && rst_d_ni && esr_apb_rsp_i.pslverr;
  end

  /* verilator lint_off SYNCASYNCNET */  // rst_d_ni/rst_w_ni used in comb enable + sensitivity list — expected
  always_ff @(posedge clk_i) begin
    if (esr_pready_en)
      esr_pready_q <= esr_pready_d;
    if (esr_pslverr_en)
      esr_pslverr_q <= esr_pslverr_d;
  end
  /* verilator lint_on SYNCASYNCNET */

  // ── APB response output ────────────────────────────────

  always_comb begin
    apb_rsp_o.pready  = d3_dbg_access_q ||
                        (esr_pready_q && !d0_dbg_access && !d1_dbg_access_q && !d2_dbg_access_q);
    apb_rsp_o.prdata  = d3_dbg_db_read_data_q;
    apb_rsp_o.pslverr = (d3_dbg_access_q && d3_dbg_error_q) ||
                        (esr_pslverr_q && !d0_dbg_access && !d1_dbg_access_q && !d2_dbg_access_q && !d3_dbg_access_q);
  end

  // ── APB passthrough to ESR ─────────────────────────────

  always_comb begin
    esr_apb_req_o.paddr   = apb_req_i.paddr;
    esr_apb_req_o.pwrite  = apb_req_i.pwrite;
    esr_apb_req_o.psel    = apb_req_i.psel    && !apb_req_i.paddr[DbgAddrBit];
    esr_apb_req_o.penable = apb_req_i.penable && !apb_req_i.paddr[DbgAddrBit];
    esr_apb_req_o.pwdata  = apb_req_i.pwdata;
  end

endmodule
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on WIDTHTRUNC */
/* verilator lint_on WIDTHEXPAND */
