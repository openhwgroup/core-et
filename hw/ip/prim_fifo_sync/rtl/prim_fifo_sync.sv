// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Synchronous FIFO with ready/valid handshake.
//
// - FPGA-friendly: infers BRAM for large depths, LUT RAM for small.
// - Async-assert / sync-deassert reset (FPGA + ASIC safe).
// - Synchronous clear input for software-initiated flush.

module prim_fifo_sync #(
  parameter int unsigned Width = 8,
  parameter int unsigned Depth = 4,
  // Derived — do not override.
  localparam int unsigned DepthW = $clog2(Depth + 1),
  localparam int unsigned PtrW   = Depth > 1 ? $clog2(Depth) : 1
) (
  input  logic              clk_i,
  input  logic              rst_ni,

  input  logic              clr_i,     // synchronous clear

  // Write port
  input  logic              wvalid_i,
  output logic              wready_o,
  input  logic [Width-1:0]  wdata_i,

  // Read port
  output logic              rvalid_o,
  input  logic              rready_i,
  output logic [Width-1:0]  rdata_o,

  // Status
  output logic [DepthW-1:0] depth_o,
  output logic              full_o,
  output logic              empty_o
);

  // ── Internal signals ──────────────────────────────────

  logic [DepthW-1:0] depth_d, depth_q;
  logic [PtrW-1:0]   wptr_d,  wptr_q;
  logic [PtrW-1:0]   rptr_d,  rptr_q;

  logic do_write, do_read;

  // ── Handshake ─────────────────────────────────────────

  assign full_o   = (depth_q == DepthW'(Depth));
  assign empty_o  = (depth_q == '0);
  assign wready_o = ~full_o;
  assign rvalid_o = ~empty_o;
  assign depth_o  = depth_q;

  assign do_write = wvalid_i & wready_o;
  assign do_read  = rvalid_o & rready_i;

  // ── Depth counter ─────────────────────────────────────

  always_comb begin
    depth_d = depth_q;
    unique case ({do_write, do_read})
      2'b10:   depth_d = depth_q + 1;
      2'b01:   depth_d = depth_q - 1;
      default: depth_d = depth_q;  // 00 or 11: no change
    endcase
  end

  // ── Pointers ──────────────────────────────────────────

  always_comb begin
    wptr_d = wptr_q;
    if (do_write) begin
      wptr_d = (wptr_q == PtrW'(Depth - 1)) ? '0 : wptr_q + 1;
    end
  end

  always_comb begin
    rptr_d = rptr_q;
    if (do_read) begin
      rptr_d = (rptr_q == PtrW'(Depth - 1)) ? '0 : rptr_q + 1;
    end
  end

  // ── Memory ────────────────────────────────────────────
  // Infers distributed RAM (LUT) for small Depth, BRAM for large Depth.

  logic [Width-1:0] mem [Depth];

  always_ff @(posedge clk_i) begin
    if (do_write) begin
      mem[wptr_q] <= wdata_i;
    end
  end

  assign rdata_o = mem[rptr_q];

  // ── Sequential ────────────────────────────────────────

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      depth_q <= '0;
      wptr_q  <= '0;
      rptr_q  <= '0;
    end else if (clr_i) begin
      depth_q <= '0;
      wptr_q  <= '0;
      rptr_q  <= '0;
    end else begin
      depth_q <= depth_d;
      wptr_q  <= wptr_d;
      rptr_q  <= rptr_d;
    end
  end

  // ── Assertions ────────────────────────────────────────

  // synthesis translate_off
  // pragma coverage off
`ifdef VERILATOR
  /* verilator lint_off SYNCASYNCNET */  // rst_ni is async-assert/sync-deassert, used here only as assertion guard
  always_ff @(posedge clk_i) begin
    if (rst_ni) begin
      if (wvalid_i && full_o)
        $warning("prim_fifo_sync: write while full (data dropped)");
      if (rready_i && empty_o)
        $warning("prim_fifo_sync: read while empty");
    end
  end
`else
  assert property (@(posedge clk_i) disable iff (!rst_ni)
    wvalid_i |-> wready_o)
  else $warning("prim_fifo_sync: write while full (data dropped)");

  assert property (@(posedge clk_i) disable iff (!rst_ni)
    rready_i |-> rvalid_o)
  else $warning("prim_fifo_sync: read while empty");
`endif
  /* verilator lint_on SYNCASYNCNET */
  // pragma coverage on
  // synthesis translate_on

endmodule
