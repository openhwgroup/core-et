// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// intpipe_mask_scoreboard — Mask register hazard scoreboard.
//
// Pure combinational. Checks if any mask register read/write address
// overlaps with in-flight VPU mask destinations.
//
// Line-by-line translation of: intpipe_mask_scoreboard (etcore-soc)

module intpipe_mask_scoreboard
  import minion_pkg::*;
(
  // Read port
  input  logic                          rd_thread_id,
  input  logic [VpuRegmaskNum-1:0]      rd_addr,
  input  logic [VpuRegmaskNum-1:0]      wd_addr,
  output logic [VpuRegmaskNum-1:0]      rd_data,
  output logic [VpuRegmaskNum-1:0]      wd_data,
  // Scoreboard from vpu unit
  /* verilator lint_off UNUSEDSIGNAL */  // Only mask fields used from full scoreboard struct
  input  vpu_scoreboard_t               vpu_scoreboard
  /* verilator lint_on UNUSEDSIGNAL */
);

  always_comb begin
    rd_data = '0;
    wd_data = '0;

    // For all the mask destinations
    for (integer i = 0; i < VpuMaskScoreboardSize; i++) begin
      rd_data[vpu_scoreboard.mask_dest[i].addr] |= vpu_scoreboard.mask_valid[i]
            && (vpu_scoreboard.mask_dest[i].thread_id == rd_thread_id)
            /* verilator lint_off WIDTHEXPAND */  // 1-bit shifted to 8-bit mask (same as original)
            && (((1'b1 << vpu_scoreboard.mask_dest[i].addr) & rd_addr) != VpuRegmaskNum'(0));
            /* verilator lint_on WIDTHEXPAND */
    end

    for (integer i = 0; i < VpuMaskScoreboardSize; i++) begin
      wd_data[vpu_scoreboard.mask_dest[i].addr] |= vpu_scoreboard.mask_valid[i]
            && (vpu_scoreboard.mask_dest[i].thread_id == rd_thread_id)
            /* verilator lint_off WIDTHEXPAND */
            && (((1'b1 << vpu_scoreboard.mask_dest[i].addr) & wd_addr) != VpuRegmaskNum'(0));
            /* verilator lint_on WIDTHEXPAND */
    end
  end

endmodule
