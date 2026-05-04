// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module sp_apb_mux_tb (
  /* verilator lint_off UNUSEDSIGNAL */
  input  logic        clk_i,
  input  logic        rst_ni,
  /* verilator lint_on UNUSEDSIGNAL */

  input  logic [15:0] apb_paddr,
  input  logic        apb_penable,
  output logic [63:0] apb_prdata,
  output logic        apb_pready,
  input  logic        apb_psel,
  output logic        apb_pslverr,
  input  logic [63:0] apb_pwdata,
  input  logic        apb_pwrite,

  output logic [3:0]  esr_apb_paddr,
  output logic        esr_apb_penable,
  input  logic [63:0] esr_apb_prdata,
  input  logic        esr_apb_pready,
  output logic        esr_apb_psel,
  input  logic        esr_apb_pslverr,
  output logic [63:0] esr_apb_pwdata,
  output logic        esr_apb_pwrite,

  output logic [9:0]  icache_apb_paddr,
  output logic        icache_apb_pwrite,
  output logic        icache_apb_psel,
  output logic        icache_apb_penable,
  output logic [63:0] icache_apb_pwdata,
  input  logic        icache_apb_pready,
  input  logic [63:0] icache_apb_prdata,
  input  logic        icache_apb_pslverr,

  output logic [12:0] minion_apb_paddr,
  output logic        minion_apb_penable,
  input  logic [63:0] minion_apb_prdata,
  input  logic        minion_apb_pready,
  output logic        minion_apb_psel,
  input  logic        minion_apb_pslverr,
  output logic [63:0] minion_apb_pwdata,
  output logic        minion_apb_pwrite,

  output logic [12:0] ic_data_apb_paddr,
  output logic        ic_data_apb_pwrite,
  output logic        ic_data_apb_psel,
  output logic        ic_data_apb_penable,
  output logic [63:0] ic_data_apb_pwdata,
  input  logic        ic_data_apb_pready,
  input  logic [63:0] ic_data_apb_prdata,
  input  logic        ic_data_apb_pslverr
);

  sp_apb_mux u_dut (
    .apb_paddr,
    .apb_penable,
    .apb_prdata,
    .apb_pready,
    .apb_psel,
    .apb_pslverr,
    .apb_pwdata,
    .apb_pwrite,
    .esr_apb_paddr,
    .esr_apb_penable,
    .esr_apb_prdata,
    .esr_apb_pready,
    .esr_apb_psel,
    .esr_apb_pslverr,
    .esr_apb_pwdata,
    .esr_apb_pwrite,
    .icache_apb_paddr,
    .icache_apb_pwrite,
    .icache_apb_psel,
    .icache_apb_penable,
    .icache_apb_pwdata,
    .icache_apb_pready,
    .icache_apb_prdata,
    .icache_apb_pslverr,
    .minion_apb_paddr,
    .minion_apb_penable,
    .minion_apb_prdata,
    .minion_apb_pready,
    .minion_apb_psel,
    .minion_apb_pslverr,
    .minion_apb_pwdata,
    .minion_apb_pwrite,
    .ic_data_apb_paddr,
    .ic_data_apb_pwrite,
    .ic_data_apb_psel,
    .ic_data_apb_penable,
    .ic_data_apb_pwdata,
    .ic_data_apb_pready,
    .ic_data_apb_prdata,
    .ic_data_apb_pslverr
  );

endmodule

