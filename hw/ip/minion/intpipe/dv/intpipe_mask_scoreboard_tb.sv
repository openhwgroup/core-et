// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSED */  // clk_i/rst_ni are present for the standard sim_ctrl harness; scoreboard is combinational.
module intpipe_mask_scoreboard_tb
  import minion_pkg::*;
(
  input  logic       clk_i,
  input  logic       rst_ni,

  input  logic       rd_thread_id_i,
  input  logic [7:0] rd_addr_i,
  input  logic [7:0] wd_addr_i,
  output logic [7:0] rd_data_o,
  output logic [7:0] wd_data_o,

  input  logic [3:0] vpu_mask_valid_i,
  input  logic [3:0] vpu_mask_thread_i,
  input  logic [2:0] vpu_mask_addr0_i,
  input  logic [2:0] vpu_mask_addr1_i,
  input  logic [2:0] vpu_mask_addr2_i,
  input  logic [2:0] vpu_mask_addr3_i
);
/* verilator lint_on UNUSED */

  vpu_scoreboard_t vpu_scoreboard;

  always_comb begin
    vpu_scoreboard = '0;
    vpu_scoreboard.mask_valid = vpu_mask_valid_i;
    vpu_scoreboard.mask_dest[0] = '{addr: vpu_mask_addr0_i, thread_id: vpu_mask_thread_i[0]};
    vpu_scoreboard.mask_dest[1] = '{addr: vpu_mask_addr1_i, thread_id: vpu_mask_thread_i[1]};
    vpu_scoreboard.mask_dest[2] = '{addr: vpu_mask_addr2_i, thread_id: vpu_mask_thread_i[2]};
    vpu_scoreboard.mask_dest[3] = '{addr: vpu_mask_addr3_i, thread_id: vpu_mask_thread_i[3]};
  end

  intpipe_mask_scoreboard u_dut (
    .rd_thread_id   (rd_thread_id_i),
    .rd_addr        (rd_addr_i),
    .wd_addr        (wd_addr_i),
    .rd_data        (rd_data_o),
    .wd_data        (wd_data_o),
    .vpu_scoreboard (vpu_scoreboard)
  );

endmodule
