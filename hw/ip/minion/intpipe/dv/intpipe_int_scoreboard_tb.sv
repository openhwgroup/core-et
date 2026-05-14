// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSED */  // clk_i/rst_ni are present for the standard sim_ctrl harness; scoreboard is combinational.
module intpipe_int_scoreboard_tb
  import minion_pkg::*;
(
  input  logic        clk_i,
  input  logic        rst_ni,

  input  logic        rd_thread_id_i,
  input  logic [4:0]  rd_addr0_i,
  input  logic [4:0]  rd_addr1_i,
  input  logic [4:0]  rd_addr2_i,
  output logic [2:0]  rd_data_o,
  output logic [2:0]  rd_data_dcache_div_o,
  output logic        rd_x31_o,

  input  logic [7:0]  dcache_valid_i,
  input  logic [7:0]  dcache_fp_i,
  input  logic [7:0]  dcache_thread_i,
  input  logic [4:0]  dcache_addr0_i,
  input  logic [4:0]  dcache_addr1_i,
  input  logic [4:0]  dcache_addr2_i,
  input  logic [4:0]  dcache_addr3_i,
  input  logic [4:0]  dcache_addr4_i,
  input  logic [4:0]  dcache_addr5_i,
  input  logic [4:0]  dcache_addr6_i,
  input  logic [4:0]  dcache_addr7_i,

  input  logic        int_div_valid_i,
  input  logic        int_div_fp_i,
  input  logic [4:0]  int_div_addr_i,
  input  logic        int_div_thread_i,
  input  logic        int_flb_valid_i,
  input  logic        int_flb_fp_i,
  input  logic [4:0]  int_flb_addr_i,
  input  logic        int_flb_thread_i,

  input  logic [3:0]  vpu_toint_valid_i,
  input  logic [3:0]  vpu_toint_fp_i,
  input  logic [3:0]  vpu_toint_thread_i,
  input  logic [4:0]  vpu_toint_addr0_i,
  input  logic [4:0]  vpu_toint_addr1_i,
  input  logic [4:0]  vpu_toint_addr2_i,
  input  logic [4:0]  vpu_toint_addr3_i
);
/* verilator lint_on UNUSED */

  logic [2:0][XregAddrSize-1:0] rd_addr;
  dcache_scoreboard_t           dcache_scoreboard;
  minion_reg_dest_t             int_div_dest;
  minion_reg_dest_t             int_flb_dest;
  vpu_scoreboard_t              vpu_scoreboard;

  always_comb begin
    rd_addr[0] = rd_addr0_i;
    rd_addr[1] = rd_addr1_i;
    rd_addr[2] = rd_addr2_i;

    dcache_scoreboard = '0;
    dcache_scoreboard.valid = dcache_valid_i;
    dcache_scoreboard.dest[0] = '{fp: dcache_fp_i[0], addr: dcache_addr0_i, thread_id: dcache_thread_i[0]};
    dcache_scoreboard.dest[1] = '{fp: dcache_fp_i[1], addr: dcache_addr1_i, thread_id: dcache_thread_i[1]};
    dcache_scoreboard.dest[2] = '{fp: dcache_fp_i[2], addr: dcache_addr2_i, thread_id: dcache_thread_i[2]};
    dcache_scoreboard.dest[3] = '{fp: dcache_fp_i[3], addr: dcache_addr3_i, thread_id: dcache_thread_i[3]};
    dcache_scoreboard.dest[4] = '{fp: dcache_fp_i[4], addr: dcache_addr4_i, thread_id: dcache_thread_i[4]};
    dcache_scoreboard.dest[5] = '{fp: dcache_fp_i[5], addr: dcache_addr5_i, thread_id: dcache_thread_i[5]};
    dcache_scoreboard.dest[6] = '{fp: dcache_fp_i[6], addr: dcache_addr6_i, thread_id: dcache_thread_i[6]};
    dcache_scoreboard.dest[7] = '{fp: dcache_fp_i[7], addr: dcache_addr7_i, thread_id: dcache_thread_i[7]};

    int_div_dest = '{fp: int_div_fp_i, addr: int_div_addr_i, thread_id: int_div_thread_i};
    int_flb_dest = '{fp: int_flb_fp_i, addr: int_flb_addr_i, thread_id: int_flb_thread_i};

    vpu_scoreboard = '0;
    vpu_scoreboard.toint_valid = vpu_toint_valid_i;
    vpu_scoreboard.toint_dest[0] = '{fp: vpu_toint_fp_i[0], addr: vpu_toint_addr0_i, thread_id: vpu_toint_thread_i[0]};
    vpu_scoreboard.toint_dest[1] = '{fp: vpu_toint_fp_i[1], addr: vpu_toint_addr1_i, thread_id: vpu_toint_thread_i[1]};
    vpu_scoreboard.toint_dest[2] = '{fp: vpu_toint_fp_i[2], addr: vpu_toint_addr2_i, thread_id: vpu_toint_thread_i[2]};
    vpu_scoreboard.toint_dest[3] = '{fp: vpu_toint_fp_i[3], addr: vpu_toint_addr3_i, thread_id: vpu_toint_thread_i[3]};
  end

  intpipe_int_scoreboard u_dut (
    .rd_thread_id       (rd_thread_id_i),
    .rd_addr            (rd_addr),
    .rd_data            (rd_data_o),
    .rd_data_dcache_div (rd_data_dcache_div_o),
    .rd_x31             (rd_x31_o),
    .dcache_scoreboard  (dcache_scoreboard),
    .int_div_valid      (int_div_valid_i),
    .int_div_dest       (int_div_dest),
    .int_flb_valid      (int_flb_valid_i),
    .int_flb_dest       (int_flb_dest),
    .vpu_scoreboard     (vpu_scoreboard)
  );

endmodule
