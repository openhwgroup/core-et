// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// ET System Register block for RBOX.
//
// Auto-generated from esr.csv by gen_esr.py.
// Do not edit manually.
//
// Registers:
//   0x000  config                          RW  64-bit  RBOX Configuration ESR
//   0x001  in_buf_pg                       RW  64-bit  RBOX Input Buffer Pages ESR
//   0x002  in_buf_cfg                      RW  64-bit  RBOX Input Buffer Configuration ESR
//   0x003  out_buf_pg                      RW  32-bit  RBOX Output Buffer Page ESR
//   0x004  out_buf_cfg                     RW  64-bit  RBOX Output Buffer Configuration ESR
//   0x005  status                          RO  32-bit  RBOX Status ESR
//   0x006  start                           RW  32-bit  RBOX Start ESR
//   0x007  consume                         RW  32-bit  RBOX Consume Output ESR
//
// APB protocol: 2-phase (setup + access).
// Write enable pulses (_en_o) are asserted for one cycle after a write.

module rbox_esr
  import apb_pkg::*;
(
  input  logic       clk_i,
  input  logic       rst_w_ni,

  // APB interface
  input  apb_pkg::req_t  apb_req_i,
  output apb_pkg::rsp_t  apb_rsp_o,

  // Register ports
  output logic [63:0]     config_o,
  output logic            config_en_o,
  output logic [63:0]     in_buf_pg_o,
  output logic            in_buf_pg_en_o,
  output logic [63:0]     in_buf_cfg_o,
  output logic            in_buf_cfg_en_o,
  output logic [31:0]     out_buf_pg_o,
  output logic            out_buf_pg_en_o,
  output logic [63:0]     out_buf_cfg_o,
  output logic            out_buf_cfg_en_o,
  output logic [31:0]     start_o,
  output logic            start_en_o,
  output logic [31:0]     consume_o,
  output logic            consume_en_o,
  input  logic [31:0]     status_i
);

  // ── Address constants ────────────────────────────────
  localparam logic [15:0] AddrConfig = 16'h0;
  localparam logic [15:0] AddrInBufPg = 16'h1;
  localparam logic [15:0] AddrInBufCfg = 16'h2;
  localparam logic [15:0] AddrOutBufPg = 16'h3;
  localparam logic [15:0] AddrOutBufCfg = 16'h4;
  localparam logic [15:0] AddrStatus = 16'h5;
  localparam logic [15:0] AddrStart = 16'h6;
  localparam logic [15:0] AddrConsume = 16'h7;

  // ── Register storage ─────────────────────────────────
  logic [63:0]     reg_config_q;
  logic [63:0]     reg_in_buf_pg_q;
  logic [63:0]     reg_in_buf_cfg_q;
  logic [31:0]     reg_out_buf_pg_q;
  logic [63:0]     reg_out_buf_cfg_q;
  logic [31:0]     reg_start_q;
  logic [31:0]     reg_consume_q;

  // ── Write enable pulses (delayed one cycle) ──────────
  // Internal enables (gate register writes):
  logic en_config_d;
  logic en_in_buf_pg_d;
  logic en_in_buf_cfg_d;
  logic en_out_buf_pg_d;
  logic en_out_buf_cfg_d;
  logic en_start_d;
  logic en_consume_d;
  // Output enables (wdata-gated, match original CUSTOM STATEMENTS):
  logic out_en_config_d;
  logic out_en_config_q;
  logic out_en_in_buf_pg_d;
  logic out_en_in_buf_pg_q;
  logic out_en_in_buf_cfg_d;
  logic out_en_in_buf_cfg_q;
  logic out_en_out_buf_pg_d;
  logic out_en_out_buf_pg_q;
  logic out_en_out_buf_cfg_d;
  logic out_en_out_buf_cfg_q;
  logic out_en_start_d;
  logic out_en_start_q;
  logic out_en_consume_d;
  logic out_en_consume_q;

  // ── APB decode ───────────────────────────────────────
  logic s0_setup;
  logic s1_access;
  logic [15:0] s0_addr;
  logic s0_write;
  logic s0_addr_valid;
  logic [63:0] s0_rdata;

  always_comb begin
    s0_setup = apb_req_i.psel && !apb_req_i.penable;
    s1_access = apb_req_i.psel && apb_req_i.penable;
    s0_addr = apb_req_i.paddr;
    s0_write = apb_req_i.pwrite;
    s0_addr_valid = 1'b0;
    s0_rdata = '0;

    if (s0_setup) begin
      unique case (s0_addr)
        AddrConfig: begin s0_addr_valid = 1'b1; s0_rdata = 64'(reg_config_q); end
        AddrInBufPg: begin s0_addr_valid = 1'b1; s0_rdata = 64'(reg_in_buf_pg_q); end
        AddrInBufCfg: begin s0_addr_valid = 1'b1; s0_rdata = 64'(reg_in_buf_cfg_q); end
        AddrOutBufPg: begin s0_addr_valid = 1'b1; s0_rdata = 64'(reg_out_buf_pg_q); end
        AddrOutBufCfg: begin s0_addr_valid = 1'b1; s0_rdata = 64'(reg_out_buf_cfg_q); end
        AddrStatus: begin s0_addr_valid = 1'b1; s0_rdata = 64'(status_i); end
        AddrStart: begin s0_addr_valid = 1'b1; s0_rdata = 64'(reg_start_q); end
        AddrConsume: begin s0_addr_valid = 1'b1; s0_rdata = 64'(reg_consume_q); end
        default: s0_addr_valid = 1'b0;
      endcase
    end

    en_config_d = s1_access && s0_write && (s0_addr == AddrConfig);
    en_in_buf_pg_d = s1_access && s0_write && (s0_addr == AddrInBufPg);
    en_in_buf_cfg_d = s1_access && s0_write && (s0_addr == AddrInBufCfg);
    en_out_buf_pg_d = s1_access && s0_write && (s0_addr == AddrOutBufPg);
    en_out_buf_cfg_d = s1_access && s0_write && (s0_addr == AddrOutBufCfg);
    en_start_d = s1_access && s0_write && (s0_addr == AddrStart);
    en_consume_d = s1_access && s0_write && (s0_addr == AddrConsume);

    out_en_config_d = s1_access && (|s1_wdata_q) && (s0_addr == AddrConfig);
    out_en_in_buf_pg_d = s1_access && (|s1_wdata_q) && (s0_addr == AddrInBufPg);
    out_en_in_buf_cfg_d = s1_access && (|s1_wdata_q) && (s0_addr == AddrInBufCfg);
    out_en_out_buf_pg_d = s1_access && (|s1_wdata_q) && (s0_addr == AddrOutBufPg);
    out_en_out_buf_cfg_d = s1_access && (|s1_wdata_q) && (s0_addr == AddrOutBufCfg);
    out_en_start_d = s1_access && (|s1_wdata_q) && (s0_addr == AddrStart);
    out_en_consume_d = s1_access && (|s1_wdata_q) && (s0_addr == AddrConsume);
  end

  // ── Pipeline registers ───────────────────────────────
  logic [63:0] s1_rdata_q;
  logic s1_addr_valid_q;
  logic [63:0] s1_wdata_q;

  always_ff @(posedge clk_i or negedge rst_w_ni) begin
    if (!rst_w_ni)
      s1_addr_valid_q <= 1'b0;
    else if (s0_setup)
      s1_addr_valid_q <= s0_addr_valid;
  end

  always_ff @(posedge clk_i) begin
    if (s0_setup && !s0_write)
      s1_rdata_q <= s0_rdata;
    if (s0_setup && s0_write)
      s1_wdata_q <= apb_req_i.pwdata;
  end

  // ── APB outputs ──────────────────────────────────────
  assign apb_rsp_o.prdata  = s1_rdata_q;
  assign apb_rsp_o.pslverr = !s1_addr_valid_q;
  assign apb_rsp_o.pready  = 1'b1;

  // ── Register write + reset ───────────────────────────
  always_ff @(posedge clk_i or negedge rst_w_ni) begin
    if (!rst_w_ni) begin
      reg_config_q <= 64'('0);
      reg_in_buf_pg_q <= 64'('0);
      reg_in_buf_cfg_q <= 64'('0);
      reg_out_buf_pg_q <= 32'('0);
      reg_out_buf_cfg_q <= 64'('0);
      reg_start_q <= 32'('0);
      reg_consume_q <= 32'('0);
      out_en_config_q <= 1'b0;
      out_en_in_buf_pg_q <= 1'b0;
      out_en_in_buf_cfg_q <= 1'b0;
      out_en_out_buf_pg_q <= 1'b0;
      out_en_out_buf_cfg_q <= 1'b0;
      out_en_start_q <= 1'b0;
      out_en_consume_q <= 1'b0;
    end else begin
      out_en_config_q <= out_en_config_d;
      out_en_in_buf_pg_q <= out_en_in_buf_pg_d;
      out_en_in_buf_cfg_q <= out_en_in_buf_cfg_d;
      out_en_out_buf_pg_q <= out_en_out_buf_pg_d;
      out_en_out_buf_cfg_q <= out_en_out_buf_cfg_d;
      out_en_start_q <= out_en_start_d;
      out_en_consume_q <= out_en_consume_d;
      if (en_config_d) reg_config_q <= s1_wdata_q;
      if (en_in_buf_pg_d) reg_in_buf_pg_q <= s1_wdata_q;
      if (en_in_buf_cfg_d) reg_in_buf_cfg_q <= s1_wdata_q;
      if (en_out_buf_pg_d) reg_out_buf_pg_q <= s1_wdata_q[31:0];
      if (en_out_buf_cfg_d) reg_out_buf_cfg_q <= s1_wdata_q;
      if (en_start_d) reg_start_q <= s1_wdata_q[31:0];
      if (en_consume_d) reg_consume_q <= s1_wdata_q[31:0];
    end
  end

  // ── Output assignments ───────────────────────────────
  assign config_o    = reg_config_q;
  assign config_en_o = out_en_config_q;
  assign in_buf_pg_o    = reg_in_buf_pg_q;
  assign in_buf_pg_en_o = out_en_in_buf_pg_q;
  assign in_buf_cfg_o    = reg_in_buf_cfg_q;
  assign in_buf_cfg_en_o = out_en_in_buf_cfg_q;
  assign out_buf_pg_o    = reg_out_buf_pg_q;
  assign out_buf_pg_en_o = out_en_out_buf_pg_q;
  assign out_buf_cfg_o    = reg_out_buf_cfg_q;
  assign out_buf_cfg_en_o = out_en_out_buf_cfg_q;
  assign start_o    = reg_start_q;
  assign start_en_o = out_en_start_q;
  assign consume_o    = reg_consume_q;
  assign consume_en_o = out_en_consume_q;

endmodule
