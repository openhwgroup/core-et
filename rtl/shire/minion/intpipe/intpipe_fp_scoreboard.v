// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"
module intpipe_fp_scoreboard #(
  parameter READ_PORTS  = 3
) (
  // Read port
  input  logic                                    rd_thread_id,
  input  logic [READ_PORTS-1:0][`XREG_ADDR_RANGE] rd_addr,
  output logic [READ_PORTS-1:0]                   rd_data,
  output logic [READ_PORTS-1:0]                   rd_data_dcache,
  // Scoreboard from different units
  input  dcache_minion_scoreboard                 dcache_scoreboard,
  input  vpu_minion_scoreboard                    vpu_scoreboard
);

  minion_reg_dest rd_dest;

  always_comb begin
    // FP, thread id is same for all ports
    rd_dest.fp        = 1'b1;
    rd_dest.thread_id = rd_thread_id;

    //default values
    rd_data_dcache = '0;

    // For all the read ports
    for(integer rport = 0; rport < READ_PORTS; rport++) begin
      // Gets the addr
      rd_dest.addr = rd_addr[rport];

      // For all the dcache dests check if the same
      for(integer i = 0; i < `DCACHE_REPLAYQ_SIZE; i++)
        rd_data_dcache[rport] |= dcache_scoreboard.valid[i] && (dcache_scoreboard.dest[i] == rd_dest);
      rd_data[rport] = rd_data_dcache[rport];

      // For all the trans destinations
      for(integer i = 0; i < `VPU_FP_SCOREBOARD_SIZE; i++)
        rd_data[rport] |= vpu_scoreboard.fp_valid[i] && (vpu_scoreboard.fp_dest[i] == rd_dest);
    end
  end

endmodule


