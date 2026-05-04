// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module sp_apb_mux #(
  parameter int unsigned master_apb_addr   = 16,
  parameter int unsigned master_apb_data   = 64,
  parameter int unsigned esr_apb_addr      = 4,
  parameter int unsigned esr_apb_data      = 64,
  parameter int unsigned icache_apb_addr   = 10,
  parameter int unsigned icache_apb_data   = 64,
  parameter int unsigned minion_apb_addr   = 13,
  parameter int unsigned minion_apb_data   = 64,
  parameter int unsigned icache_d_apb_addr = 13,
  parameter int unsigned icache_d_apb_data = 64
) (
  input  logic [master_apb_addr-1:0]   apb_paddr,
  input  logic                         apb_penable,
  output logic [master_apb_data-1:0]   apb_prdata,
  output logic                         apb_pready,
  input  logic                         apb_psel,
  output logic                         apb_pslverr,
  input  logic [master_apb_data-1:0]   apb_pwdata,
  input  logic                         apb_pwrite,

  output logic [esr_apb_addr-1:0]      esr_apb_paddr,
  output logic                         esr_apb_penable,
  input  logic [esr_apb_data-1:0]      esr_apb_prdata,
  input  logic                         esr_apb_pready,
  output logic                         esr_apb_psel,
  input  logic                         esr_apb_pslverr,
  output logic [esr_apb_data-1:0]      esr_apb_pwdata,
  output logic                         esr_apb_pwrite,

  output logic [icache_apb_addr-1:0]   icache_apb_paddr,
  output logic                         icache_apb_pwrite,
  output logic                         icache_apb_psel,
  output logic                         icache_apb_penable,
  output logic [icache_apb_data-1:0]   icache_apb_pwdata,
  input  logic                         icache_apb_pready,
  input  logic [icache_apb_data-1:0]   icache_apb_prdata,
  input  logic                         icache_apb_pslverr,

  output logic [minion_apb_addr-1:0]   minion_apb_paddr,
  output logic                         minion_apb_penable,
  input  logic [minion_apb_data-1:0]   minion_apb_prdata,
  input  logic                         minion_apb_pready,
  output logic                         minion_apb_psel,
  input  logic                         minion_apb_pslverr,
  output logic [minion_apb_data-1:0]   minion_apb_pwdata,
  output logic                         minion_apb_pwrite,

  output logic [icache_d_apb_addr-1:0] ic_data_apb_paddr,
  output logic                         ic_data_apb_pwrite,
  output logic                         ic_data_apb_psel,
  output logic                         ic_data_apb_penable,
  output logic [icache_d_apb_data-1:0] ic_data_apb_pwdata,
  input  logic                         ic_data_apb_pready,
  input  logic [icache_d_apb_data-1:0] ic_data_apb_prdata,
  input  logic                         ic_data_apb_pslverr
);

  always_comb begin
    minion_apb_psel    = '0;
    minion_apb_penable = '0;
    minion_apb_paddr   = '0;

    icache_apb_psel    = '0;
    icache_apb_penable = '0;
    icache_apb_paddr   = '0;

    ic_data_apb_psel    = '0;
    ic_data_apb_penable = '0;
    ic_data_apb_paddr   = '0;

    esr_apb_psel    = '0;
    esr_apb_penable = '0;
    esr_apb_paddr   = '0;

    apb_prdata  = '0;
    apb_pready  = 1'b1;
    apb_pslverr = 1'b1;

    if ((apb_paddr[15:13] == 3'b000) || (apb_paddr[15:13] == 3'b100)) begin
      if (((apb_paddr[15:13] == 3'b100) && (apb_paddr[12:10] != '0)) ||
          ((apb_paddr[15:13] == 3'b000) && (apb_paddr[10:9]  != '0))) begin
        apb_prdata  = '0;
        apb_pready  = 1'b1;
        apb_pslverr = 1'b1;
      end else begin
        minion_apb_psel    = apb_psel;
        minion_apb_penable = apb_penable;
        if (!apb_paddr[15]) begin
          minion_apb_paddr = {1'b0, apb_paddr[12:11], 1'b0, apb_paddr[8:0]};
        end else begin
          minion_apb_paddr = {1'b1, 2'b00, apb_paddr[9:0]};
        end
        apb_prdata  = minion_apb_prdata;
        apb_pready  = minion_apb_pready;
        apb_pslverr = apb_psel && apb_penable && minion_apb_pready && minion_apb_pslverr;
      end
    end else if (apb_paddr[15:13] == 3'b001) begin
      if (apb_paddr[10:2] != '0) begin
        apb_prdata  = '0;
        apb_pready  = 1'b1;
        apb_pslverr = 1'b1;
      end else begin
        esr_apb_psel    = apb_psel;
        esr_apb_penable = apb_penable;
        esr_apb_paddr   = {apb_paddr[12:11], apb_paddr[1:0]};
        apb_prdata      = esr_apb_prdata;
        apb_pready      = esr_apb_pready;
        apb_pslverr     = apb_psel && apb_penable && esr_apb_pready && esr_apb_pslverr;
      end
    end else if (apb_paddr[15:13] == 3'b101) begin
      if (apb_paddr[12:icache_apb_addr] != '0) begin
        apb_prdata  = '0;
        apb_pready  = 1'b1;
        apb_pslverr = 1'b1;
      end else begin
        icache_apb_psel    = apb_psel;
        icache_apb_penable = apb_penable;
        icache_apb_paddr   = apb_paddr[icache_apb_addr-1:0];
        apb_prdata         = icache_apb_prdata;
        apb_pready         = icache_apb_pready;
        apb_pslverr        = apb_psel && apb_penable && icache_apb_pready &&
                             icache_apb_pslverr;
      end
    end else if (apb_paddr[15:13] == 3'b110) begin
      ic_data_apb_psel    = apb_psel;
      ic_data_apb_penable = apb_penable;
      ic_data_apb_paddr   = apb_paddr[12:0];
      apb_prdata          = ic_data_apb_prdata;
      apb_pready          = ic_data_apb_pready;
      apb_pslverr         = apb_psel && apb_penable && ic_data_apb_pready &&
                            ic_data_apb_pslverr;
    end
  end

  assign minion_apb_pwrite = apb_pwrite;
  assign minion_apb_pwdata = apb_pwdata;

  assign icache_apb_pwrite = apb_pwrite;
  assign icache_apb_pwdata = apb_pwdata;

  assign ic_data_apb_pwrite = apb_pwrite;
  assign ic_data_apb_pwdata = apb_pwdata;

  assign esr_apb_pwrite = apb_pwrite;
  assign esr_apb_pwdata = apb_pwdata;

endmodule

