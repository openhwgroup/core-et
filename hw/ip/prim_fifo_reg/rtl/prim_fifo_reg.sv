// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Register-based synchronous FIFO for small depths (1-4).
//
// DEPTH includes the output register stage. Data is available
// combinationally at rdata_o when valid_o is high.
//
// Behaviorally equivalent to the CORE-ET gen_fifo_reg module.
// Level tracking uses a one-hot shift register: level_o[0] = empty,
// level_o[Depth] = full.

module prim_fifo_reg #(
  parameter int unsigned Width = 8,
  parameter int unsigned Depth = 4,
  // Derived — do not override.
  localparam int unsigned MemDepth = Depth - 1,
  localparam int unsigned PtrW    = (MemDepth == 1) ? 1 : $clog2(MemDepth)
) (
  input  logic              clk_i,
  input  logic              rst_ni,

  // Write port
  input  logic              push_i,
  input  logic [Width-1:0]  wdata_i,
  output logic              full_o,

  // Read port
  input  logic              pop_i,
  output logic              valid_o,
  output logic [Width-1:0]  rdata_o,

  // Occupancy level (one-hot thermometer)
  // level_o[0] = 1 means empty, level_o[Depth] = 1 means full.
  output logic [Depth:0]    level_o
);

  if (Depth == 1) begin : gen_depth_1
    // ── Single-entry FIFO ─────────────────────────────
    // No internal memory — just the output register.

    assign full_o = valid_o & ~pop_i;

    logic valid_d;
    assign valid_d = push_i;

    always_ff @(posedge clk_i or negedge rst_ni) begin
      if (!rst_ni) begin
        valid_o <= 1'b0;
      end else if (!full_o) begin
        valid_o <= valid_d;
      end
    end

    always_ff @(posedge clk_i) begin
      if (push_i & ~full_o) begin
        rdata_o <= wdata_i;
      end
    end

    assign level_o = {valid_o, ~valid_o};

  end else begin : gen_depth_n
    // ── Multi-entry FIFO (Depth >= 2) ─────────────────
    // Internal memory of (Depth-1) entries + output register.

    // ── Level tracking (one-hot shift register) ───────
    // Shift left on write-taken, shift right on read-taken.
    // Reset to level_o = 1 (empty: level_o[0] = 1).

    logic wdata_taken, rdata_taken, level_en;
    assign wdata_taken = push_i & ~full_o;
    assign rdata_taken = pop_i & valid_o;
    assign level_en    = wdata_taken ^ rdata_taken;

    logic [Depth:0] level_d;
    assign level_d = wdata_taken ? {level_o[Depth-1:0], 1'b0}
                                 : {1'b0, level_o[Depth:1]};

    always_ff @(posedge clk_i or negedge rst_ni) begin
      if (!rst_ni) begin
        level_o <= (Depth + 1)'(1);  // level_o[0] = 1, rest = 0
      end else if (level_en) begin
        level_o <= level_d;
      end
    end

    logic mem_valid;
    assign mem_valid = ~(|level_o[1:0]);  // more than 1 entry occupied
    assign full_o    = level_o[Depth];
    assign valid_o   = ~level_o[0];

    // ── Pointers ──────────────────────────────────────

    logic [PtrW-1:0] wptr_q, wptr_d;
    logic [PtrW-1:0] rptr_q, rptr_d;
    logic            inc_wptr, inc_rptr;

    // Push into memory when: data is going to mem (not directly to output)
    assign inc_wptr = push_i & (mem_valid | (valid_o & ~pop_i)) & ~full_o;
    // Pop from memory when: output stage consumed and memory has data
    assign inc_rptr = pop_i & mem_valid;

    assign wptr_d = (wptr_q == PtrW'(MemDepth - 1)) ? '0 : wptr_q + PtrW'(1);
    assign rptr_d = (rptr_q == PtrW'(MemDepth - 1)) ? '0 : rptr_q + PtrW'(1);

    always_ff @(posedge clk_i or negedge rst_ni) begin
      if (!rst_ni) begin
        wptr_q <= '0;
      end else if (inc_wptr) begin
        wptr_q <= wptr_d;
      end
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
      if (!rst_ni) begin
        rptr_q <= '0;
      end else if (inc_rptr) begin
        rptr_q <= rptr_d;
      end
    end

    // ── Memory array ──────────────────────────────────

    logic [Width-1:0] mem [MemDepth];

    always_ff @(posedge clk_i) begin
      if (inc_wptr) begin
        mem[wptr_q] <= wdata_i;
      end
    end

    // ── Output register ───────────────────────────────
    // Load from push (bypass) or from memory (pop refill).

    logic            rdata_en;
    logic [Width-1:0] rdata_d;

    always_comb begin
      if ((push_i & ~valid_o) | (push_i & pop_i & ~mem_valid)) begin
        // Direct push to output: either FIFO was empty, or output
        // is being popped and memory is empty — bypass to output.
        rdata_en = 1'b1;
        rdata_d  = wdata_i;
      end else if (inc_rptr) begin
        // Output popped, refill from memory.
        rdata_en = 1'b1;
        rdata_d  = mem[rptr_q];
      end else begin
        rdata_en = 1'b0;
        rdata_d  = rdata_o;
      end
    end

    always_ff @(posedge clk_i) begin
      if (rdata_en) begin
        rdata_o <= rdata_d;
      end
    end

  end

endmodule
