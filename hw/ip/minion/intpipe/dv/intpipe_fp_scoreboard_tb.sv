// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSED */  // clk_i/rst_ni are present for the standard sim_ctrl harness; scoreboard is combinational.
module intpipe_fp_scoreboard_tb
  import minion_pkg::*;
(
  input  logic         clk_i,
  input  logic         rst_ni,

  input  logic         rd_thread_id_i,
  input  logic [4:0]   rd_addr0_i,
  input  logic [4:0]   rd_addr1_i,
  input  logic [4:0]   rd_addr2_i,
  output logic [2:0]   rd_data_o,
  output logic [2:0]   rd_data_dcache_o,

  input  logic [7:0]   dcache_valid_i,
  input  logic [7:0]   dcache_fp_i,
  input  logic [7:0]   dcache_thread_i,
  input  logic [4:0]   dcache_addr0_i,
  input  logic [4:0]   dcache_addr1_i,
  input  logic [4:0]   dcache_addr2_i,
  input  logic [4:0]   dcache_addr3_i,
  input  logic [4:0]   dcache_addr4_i,
  input  logic [4:0]   dcache_addr5_i,
  input  logic [4:0]   dcache_addr6_i,
  input  logic [4:0]   dcache_addr7_i,

  input  logic [13:0]  vpu_fp_valid_i,
  input  logic [13:0]  vpu_fp_fp_i,
  input  logic [13:0]  vpu_fp_thread_i,
  input  logic [4:0]   vpu_fp_addr0_i,
  input  logic [4:0]   vpu_fp_addr1_i,
  input  logic [4:0]   vpu_fp_addr2_i,
  input  logic [4:0]   vpu_fp_addr3_i,
  input  logic [4:0]   vpu_fp_addr4_i,
  input  logic [4:0]   vpu_fp_addr5_i,
  input  logic [4:0]   vpu_fp_addr6_i,
  input  logic [4:0]   vpu_fp_addr7_i,
  input  logic [4:0]   vpu_fp_addr8_i,
  input  logic [4:0]   vpu_fp_addr9_i,
  input  logic [4:0]   vpu_fp_addr10_i,
  input  logic [4:0]   vpu_fp_addr11_i,
  input  logic [4:0]   vpu_fp_addr12_i,
  input  logic [4:0]   vpu_fp_addr13_i
);
/* verilator lint_on UNUSED */

  logic [2:0][XregAddrSize-1:0] rd_addr;
  dcache_scoreboard_t           dcache_scoreboard;
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

    vpu_scoreboard = '0;
    vpu_scoreboard.fp_valid = vpu_fp_valid_i;
    vpu_scoreboard.fp_dest[0]  = '{fp: vpu_fp_fp_i[0],  addr: vpu_fp_addr0_i,  thread_id: vpu_fp_thread_i[0]};
    vpu_scoreboard.fp_dest[1]  = '{fp: vpu_fp_fp_i[1],  addr: vpu_fp_addr1_i,  thread_id: vpu_fp_thread_i[1]};
    vpu_scoreboard.fp_dest[2]  = '{fp: vpu_fp_fp_i[2],  addr: vpu_fp_addr2_i,  thread_id: vpu_fp_thread_i[2]};
    vpu_scoreboard.fp_dest[3]  = '{fp: vpu_fp_fp_i[3],  addr: vpu_fp_addr3_i,  thread_id: vpu_fp_thread_i[3]};
    vpu_scoreboard.fp_dest[4]  = '{fp: vpu_fp_fp_i[4],  addr: vpu_fp_addr4_i,  thread_id: vpu_fp_thread_i[4]};
    vpu_scoreboard.fp_dest[5]  = '{fp: vpu_fp_fp_i[5],  addr: vpu_fp_addr5_i,  thread_id: vpu_fp_thread_i[5]};
    vpu_scoreboard.fp_dest[6]  = '{fp: vpu_fp_fp_i[6],  addr: vpu_fp_addr6_i,  thread_id: vpu_fp_thread_i[6]};
    vpu_scoreboard.fp_dest[7]  = '{fp: vpu_fp_fp_i[7],  addr: vpu_fp_addr7_i,  thread_id: vpu_fp_thread_i[7]};
    vpu_scoreboard.fp_dest[8]  = '{fp: vpu_fp_fp_i[8],  addr: vpu_fp_addr8_i,  thread_id: vpu_fp_thread_i[8]};
    vpu_scoreboard.fp_dest[9]  = '{fp: vpu_fp_fp_i[9],  addr: vpu_fp_addr9_i,  thread_id: vpu_fp_thread_i[9]};
    vpu_scoreboard.fp_dest[10] = '{fp: vpu_fp_fp_i[10], addr: vpu_fp_addr10_i, thread_id: vpu_fp_thread_i[10]};
    vpu_scoreboard.fp_dest[11] = '{fp: vpu_fp_fp_i[11], addr: vpu_fp_addr11_i, thread_id: vpu_fp_thread_i[11]};
    vpu_scoreboard.fp_dest[12] = '{fp: vpu_fp_fp_i[12], addr: vpu_fp_addr12_i, thread_id: vpu_fp_thread_i[12]};
    vpu_scoreboard.fp_dest[13] = '{fp: vpu_fp_fp_i[13], addr: vpu_fp_addr13_i, thread_id: vpu_fp_thread_i[13]};
  end

  intpipe_fp_scoreboard u_dut (
    .rd_thread_id      (rd_thread_id_i),
    .rd_addr           (rd_addr),
    .rd_data           (rd_data_o),
    .rd_data_dcache    (rd_data_dcache_o),
    .dcache_scoreboard (dcache_scoreboard),
    .vpu_scoreboard    (vpu_scoreboard)
  );

endmodule
