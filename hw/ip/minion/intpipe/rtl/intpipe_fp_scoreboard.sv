// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// intpipe_fp_scoreboard — FP register hazard scoreboard.
//
// Pure combinational. Checks if any read port address matches an
// in-flight FP destination in the dcache replayQ or VPU FP pipeline.
//
// Line-by-line translation of: intpipe_fp_scoreboard (etcore-soc)

module intpipe_fp_scoreboard
  import minion_pkg::*;
#(
  parameter int unsigned ReadPorts = 3
) (
  // Read port
  input  logic                                       rd_thread_id,
  input  logic [ReadPorts-1:0][XregAddrSize-1:0]     rd_addr,
  output logic [ReadPorts-1:0]                       rd_data,
  output logic [ReadPorts-1:0]                       rd_data_dcache,
  // Scoreboard from different units
  input  dcache_scoreboard_t                         dcache_scoreboard,
  /* verilator lint_off UNUSEDSIGNAL */  // Only fp fields used from full scoreboard struct
  input  vpu_scoreboard_t                            vpu_scoreboard
  /* verilator lint_on UNUSEDSIGNAL */
);

  minion_reg_dest_t rd_dest;

  always_comb begin
    // FP, thread id is same for all ports
    rd_dest.fp        = 1'b1;
    rd_dest.thread_id = rd_thread_id;

    // default values
    rd_data_dcache = '0;

    // For all the read ports
    for (integer rport = 0; rport < ReadPorts; rport++) begin
      // Gets the addr
      rd_dest.addr = rd_addr[rport];

      // For all the dcache dests check if the same
      for (integer i = 0; i < minion_dcache_pkg::DcacheReplayqSize; i++)
        rd_data_dcache[rport] |= dcache_scoreboard.valid[i] && (dcache_scoreboard.dest[i] == rd_dest);
      rd_data[rport] = rd_data_dcache[rport];

      // For all the trans destinations
      for (integer i = 0; i < VpuFpScoreboardSize; i++)
        rd_data[rport] |= vpu_scoreboard.fp_valid[i] && (vpu_scoreboard.fp_dest[i] == rd_dest);
    end
  end

endmodule
