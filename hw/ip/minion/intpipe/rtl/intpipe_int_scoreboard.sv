// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// intpipe_int_scoreboard — Integer register hazard scoreboard.
//
// Pure combinational. Checks if any read port address matches an
// in-flight destination in the dcache replayQ, integer divider,
// fast local barrier, or VPU toint pipeline.
//
// Line-by-line translation of: intpipe_int_scoreboard (etcore-soc)

module intpipe_int_scoreboard
  import minion_pkg::*;
#(
  parameter int unsigned ReadPorts = 3
) (
  // Read port
  input  logic                                       rd_thread_id,
  input  logic [ReadPorts-1:0][XregAddrSize-1:0]     rd_addr,
  output logic [ReadPorts-1:0]                       rd_data,
  output logic [ReadPorts-1:0]                       rd_data_dcache_div,
  output logic                                       rd_x31,
  // Scoreboard from different units
  input  dcache_scoreboard_t                         dcache_scoreboard,
  input  logic                                       int_div_valid,
  input  minion_reg_dest_t                           int_div_dest,
  input  logic                                       int_flb_valid,
  input  minion_reg_dest_t                           int_flb_dest,
  /* verilator lint_off UNUSEDSIGNAL */  // Only toint fields used from full scoreboard struct
  input  vpu_scoreboard_t                            vpu_scoreboard
  /* verilator lint_on UNUSEDSIGNAL */
);

  minion_reg_dest_t rd_dest;

  always_comb begin
    // No FP, thread Id is the same for all ports
    rd_dest.fp        = 1'b0;
    rd_dest.thread_id = rd_thread_id;

    // For all the read ports
    for (integer rport = 0; rport < ReadPorts; rport++) begin
      // Gets the addr
      rd_dest.addr = rd_addr[rport];

      // Checks if div is the same dest
      rd_data_dcache_div[rport] = int_div_valid && (int_div_dest == rd_dest);

      // For all the dcache dests check if the same
      for (integer i = 0; i < minion_dcache_pkg::DcacheReplayqSize; i++)
        rd_data_dcache_div[rport] |= dcache_scoreboard.valid[i] && (dcache_scoreboard.dest[i] == rd_dest);

      rd_data[rport] = rd_data_dcache_div[rport];

      // Checks if flb is the same dest
      rd_data[rport] |= int_flb_valid && (int_flb_dest == rd_dest);

      // For all the fp to int destinations
      for (integer i = 0; i < VpuTointScoreboardSize; i++)
        rd_data[rport] |= vpu_scoreboard.toint_valid[i] && (vpu_scoreboard.toint_dest[i] == rd_dest);
    end

    // Check for X31 access
    rd_dest.addr = {XregAddrSize{1'b1}};

    // Checks if div is the same dest
    rd_x31 = int_div_valid && (int_div_dest == rd_dest);

    // For all the dcache dests check if the same
    for (integer i = 0; i < minion_dcache_pkg::DcacheReplayqSize; i++)
      rd_x31 |= dcache_scoreboard.valid[i] && (dcache_scoreboard.dest[i] == rd_dest);

    // Checks if flb is the same dest
    rd_x31 |= int_flb_valid && (int_flb_dest == rd_dest);

    // For all the fp to int destinations
    for (integer i = 0; i < VpuTointScoreboardSize; i++)
      rd_x31 |= vpu_scoreboard.toint_valid[i] && (vpu_scoreboard.toint_dest[i] == rd_dest);
  end

endmodule
