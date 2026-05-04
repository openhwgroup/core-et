// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"
module intpipe_mask_scoreboard (
  // Read port
  input  logic                        rd_thread_id,
  input  logic [`VPU_REGMASK_NUM-1:0] rd_addr,
  input  logic [`VPU_REGMASK_NUM-1:0] wd_addr,
  output logic [`VPU_REGMASK_NUM-1:0] rd_data,
  output logic [`VPU_REGMASK_NUM-1:0] wd_data,
  // Scoreboard from vpu unit
  input  vpu_minion_scoreboard        vpu_scoreboard
);

  always_comb begin
    rd_data = '0;
    wd_data = '0;

    // For all the mask destinations
    for(integer i = 0; i < `VPU_MASK_SCOREBOARD_SIZE; i++) begin
      rd_data[vpu_scoreboard.mask_dest[i].addr] |=  vpu_scoreboard.mask_valid[i]
                                                    && (vpu_scoreboard.mask_dest[i].thread_id==rd_thread_id)
                                                    && (((1'b1<<vpu_scoreboard.mask_dest[i].addr) & rd_addr) != `VPU_REGMASK_NUM'b0);
    end

    for(integer i = 0; i < `VPU_MASK_SCOREBOARD_SIZE; i++) begin
      wd_data[vpu_scoreboard.mask_dest[i].addr] |=  vpu_scoreboard.mask_valid[i]
                                                    && (vpu_scoreboard.mask_dest[i].thread_id==rd_thread_id)
                                                    && (((1'b1<<vpu_scoreboard.mask_dest[i].addr) & wd_addr) != `VPU_REGMASK_NUM'b0);
    end
  end

endmodule


