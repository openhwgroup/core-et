// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Co-simulation wrapper: original dcache_data_array vs new minion_dcache_data_array.

`include "soc.vh"

module cosim_minion_dcache_data_array_tb
  import ram_cfg_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic        clk_i,
  input  logic        rst_ni,
  input  logic        s1_read_valid_i,
  input  logic        bank0_read_valid_l_i,
  input  logic        bank0_read_valid_h_i,
  input  logic [1:0]  bank0_read_way_i,
  input  logic [9:0]  bank0_read_addr_i,
  input  logic        bank1_read_valid_l_i,
  input  logic        bank1_read_valid_h_i,
  input  logic [1:0]  bank1_read_way_i,
  input  logic [9:0]  bank1_read_addr_i,
  input  logic        bank2_read_valid_l_i,
  input  logic        bank2_read_valid_h_i,
  input  logic [1:0]  bank2_read_way_i,
  input  logic [9:0]  bank2_read_addr_i,
  input  logic        bank3_read_valid_l_i,
  input  logic        bank3_read_valid_h_i,
  input  logic [1:0]  bank3_read_way_i,
  input  logic [9:0]  bank3_read_addr_i,
  output logic [63:0] new_bank0_read_data_o,
  output logic [63:0] new_bank1_read_data_o,
  output logic [63:0] new_bank2_read_data_o,
  output logic [63:0] new_bank3_read_data_o,
  output logic [63:0] orig_bank0_read_data_o,
  output logic [63:0] orig_bank1_read_data_o,
  output logic [63:0] orig_bank2_read_data_o,
  output logic [63:0] orig_bank3_read_data_o,
  input  logic        s3_write_valid_i,
  input  logic        s4_write_valid_i,
  input  logic        bank0_write_valid_l_i,
  input  logic        bank0_write_valid_h_i,
  input  logic [1:0]  bank0_write_way_i,
  input  logic [9:0]  bank0_write_addr_i,
  input  logic [63:0] bank0_write_data_i,
  input  logic        bank1_write_valid_l_i,
  input  logic        bank1_write_valid_h_i,
  input  logic [1:0]  bank1_write_way_i,
  input  logic [9:0]  bank1_write_addr_i,
  input  logic [63:0] bank1_write_data_i,
  input  logic        bank2_write_valid_l_i,
  input  logic        bank2_write_valid_h_i,
  input  logic [1:0]  bank2_write_way_i,
  input  logic [9:0]  bank2_write_addr_i,
  input  logic [63:0] bank2_write_data_i,
  input  logic        bank3_write_valid_l_i,
  input  logic        bank3_write_valid_h_i,
  input  logic [1:0]  bank3_write_way_i,
  input  logic [9:0]  bank3_write_addr_i,
  input  logic [63:0] bank3_write_data_i
);

  logic unused;
  dcache_da_read_req_t  new_s1_read_req [DcacheLramNumBanks];
  dcache_da_write_req_t new_s4_write_req [DcacheLramNumBanks];
  dcache_da_data_t      new_s2_read_resp;

  dcache_da_read_req [`DCACHE_LRAM_BANK_RANGE]  orig_s1_read_req;
  dcache_da_write_req [`DCACHE_LRAM_BANK_RANGE] orig_s4_write_req;
  dcache_da_data        orig_s2_read_resp;
  ram_cfg_t             ram_cfg;

  assign unused = rst_ni;
  assign ram_cfg = '0;

  always_comb begin
    new_s1_read_req[0].valid_l = bank0_read_valid_l_i;
    new_s1_read_req[0].valid_h = bank0_read_valid_h_i;
    new_s1_read_req[0].way = bank0_read_way_i;
    new_s1_read_req[0].addr = bank0_read_addr_i;
    new_s1_read_req[1].valid_l = bank1_read_valid_l_i;
    new_s1_read_req[1].valid_h = bank1_read_valid_h_i;
    new_s1_read_req[1].way = bank1_read_way_i;
    new_s1_read_req[1].addr = bank1_read_addr_i;
    new_s1_read_req[2].valid_l = bank2_read_valid_l_i;
    new_s1_read_req[2].valid_h = bank2_read_valid_h_i;
    new_s1_read_req[2].way = bank2_read_way_i;
    new_s1_read_req[2].addr = bank2_read_addr_i;
    new_s1_read_req[3].valid_l = bank3_read_valid_l_i;
    new_s1_read_req[3].valid_h = bank3_read_valid_h_i;
    new_s1_read_req[3].way = bank3_read_way_i;
    new_s1_read_req[3].addr = bank3_read_addr_i;

    new_s4_write_req[0].valid_l = bank0_write_valid_l_i;
    new_s4_write_req[0].valid_h = bank0_write_valid_h_i;
    new_s4_write_req[0].way = bank0_write_way_i;
    new_s4_write_req[0].addr = bank0_write_addr_i;
    new_s4_write_req[0].data = bank0_write_data_i;
    new_s4_write_req[1].valid_l = bank1_write_valid_l_i;
    new_s4_write_req[1].valid_h = bank1_write_valid_h_i;
    new_s4_write_req[1].way = bank1_write_way_i;
    new_s4_write_req[1].addr = bank1_write_addr_i;
    new_s4_write_req[1].data = bank1_write_data_i;
    new_s4_write_req[2].valid_l = bank2_write_valid_l_i;
    new_s4_write_req[2].valid_h = bank2_write_valid_h_i;
    new_s4_write_req[2].way = bank2_write_way_i;
    new_s4_write_req[2].addr = bank2_write_addr_i;
    new_s4_write_req[2].data = bank2_write_data_i;
    new_s4_write_req[3].valid_l = bank3_write_valid_l_i;
    new_s4_write_req[3].valid_h = bank3_write_valid_h_i;
    new_s4_write_req[3].way = bank3_write_way_i;
    new_s4_write_req[3].addr = bank3_write_addr_i;
    new_s4_write_req[3].data = bank3_write_data_i;

    orig_s1_read_req[0] = '{
      valid_l: bank0_read_valid_l_i,
      valid_h: bank0_read_valid_h_i,
      way: bank0_read_way_i,
      addr: bank0_read_addr_i
    };
    orig_s1_read_req[1] = '{
      valid_l: bank1_read_valid_l_i,
      valid_h: bank1_read_valid_h_i,
      way: bank1_read_way_i,
      addr: bank1_read_addr_i
    };
    orig_s1_read_req[2] = '{
      valid_l: bank2_read_valid_l_i,
      valid_h: bank2_read_valid_h_i,
      way: bank2_read_way_i,
      addr: bank2_read_addr_i
    };
    orig_s1_read_req[3] = '{
      valid_l: bank3_read_valid_l_i,
      valid_h: bank3_read_valid_h_i,
      way: bank3_read_way_i,
      addr: bank3_read_addr_i
    };

    orig_s4_write_req[0] = '{
      valid_l: bank0_write_valid_l_i,
      valid_h: bank0_write_valid_h_i,
      way: bank0_write_way_i,
      addr: bank0_write_addr_i,
      data: bank0_write_data_i
    };
    orig_s4_write_req[1] = '{
      valid_l: bank1_write_valid_l_i,
      valid_h: bank1_write_valid_h_i,
      way: bank1_write_way_i,
      addr: bank1_write_addr_i,
      data: bank1_write_data_i
    };
    orig_s4_write_req[2] = '{
      valid_l: bank2_write_valid_l_i,
      valid_h: bank2_write_valid_h_i,
      way: bank2_write_way_i,
      addr: bank2_write_addr_i,
      data: bank2_write_data_i
    };
    orig_s4_write_req[3] = '{
      valid_l: bank3_write_valid_l_i,
      valid_h: bank3_write_valid_h_i,
      way: bank3_write_way_i,
      addr: bank3_write_addr_i,
      data: bank3_write_data_i
    };

    new_bank0_read_data_o = new_s2_read_resp[0];
    new_bank1_read_data_o = new_s2_read_resp[1];
    new_bank2_read_data_o = new_s2_read_resp[2];
    new_bank3_read_data_o = new_s2_read_resp[3];
    orig_bank0_read_data_o = orig_s2_read_resp[0];
    orig_bank1_read_data_o = orig_s2_read_resp[1];
    orig_bank2_read_data_o = orig_s2_read_resp[2];
    orig_bank3_read_data_o = orig_s2_read_resp[3];
  end

  minion_dcache_data_array u_new (
    .clk_i            (clk_i),
    .s1_read_valid_i  (s1_read_valid_i),
    .s1_read_req_i    (new_s1_read_req),
    .s2_read_resp_o   (new_s2_read_resp),
    .s3_write_valid_i (s3_write_valid_i),
    .s4_write_valid_i (s4_write_valid_i),
    .s4_write_req_i   (new_s4_write_req),
    .ram_cfg_i        (ram_cfg)
  );

  dcache_data_array u_orig (
    .clock         (clk_i),
    .s1_read_valid (s1_read_valid_i),
    .s1_read_req   (orig_s1_read_req),
    .s2_read_resp  (orig_s2_read_resp),
    .s3_write_valid(s3_write_valid_i),
    .s4_write_valid(s4_write_valid_i),
    .s4_write_req  (orig_s4_write_req)
  );

endmodule : cosim_minion_dcache_data_array_tb
