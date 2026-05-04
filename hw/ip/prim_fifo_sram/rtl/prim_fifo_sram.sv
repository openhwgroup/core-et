// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// SRAM-backed FIFO with 1-cycle read latency.
//
// Write: assert wr_en_i, data is written, wr_done_o acknowledges same cycle.
// Read:  assert rd_en_i, rd_valid_o + rd_data_o appear NEXT cycle.
//
// No backpressure — caller must track occupancy externally.
// Depth must be a power of 2 (pointers wrap via truncation).
//
// Uses prim_ram_1p (Ports=1) or prim_ram_2p (Ports=2) for the
// backing store, providing ASIC SRAM swap and configuration hooks.
//
// Behaviorally equivalent to the CORE-ET rbox_fifo module.

module prim_fifo_sram
  import dft_pkg::*;
  import ram_cfg_pkg::*;
#(
  parameter int unsigned Width = 64,
  parameter int unsigned Depth = 64,
  parameter int unsigned Ports = 2,
  // Derived — do not override.
  localparam int unsigned PtrW = $clog2(Depth)
) (
  input  logic              clk_i,
  input  logic              rst_ni,

  // Write port
  input  logic              wr_en_i,
  input  logic [Width-1:0]  wr_data_i,
  output logic              wr_done_o,

  // Read port (1-cycle latency)
  input  logic              rd_en_i,
  output logic              rd_valid_o,
  output logic [Width-1:0]  rd_data_o,

  // Technology hooks
  input  ram_cfg_t          ram_cfg_i,
  input  dft_t              dft_i
);

  // ── Write gating ─────────────────────────────────────
  logic do_write;
  assign do_write = (Ports == 2) ? wr_en_i : (wr_en_i & ~rd_en_i);
  assign wr_done_o = do_write;

  // ── Pointers ──────────────────────────────────────────

  logic [PtrW-1:0] wptr_d, wptr_q;
  logic [PtrW-1:0] rptr_d, rptr_q;

  always_comb begin
    wptr_d = wptr_q;
    if (do_write) wptr_d = wptr_q + PtrW'(1);
  end

  always_comb begin
    rptr_d = rptr_q;
    if (rd_en_i) rptr_d = rptr_q + PtrW'(1);
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      wptr_q <= '0;
      rptr_q <= '0;
    end else begin
      wptr_q <= wptr_d;
      rptr_q <= rptr_d;
    end
  end

  // ── Memory via RAM primitives ─────────────────────────

  /* verilator lint_off PINCONNECTEMPTY */  // write-only port a_rdata_o and alert_o intentionally unconnected
  logic [Width-1:0] rd_data_ram;

  if (Ports == 2) begin : gen_mem_2p
    prim_ram_2p #(
      .Width (Width),
      .Depth (Depth)
    ) u_mem (
      .clk_i,
      .rst_ni,
      // Port A: write
      .a_req_i   (do_write),
      .a_we_i    (1'b1),
      .a_addr_i  (wptr_q),
      .a_wdata_i (wr_data_i),
      .a_rdata_o (),          // unused — write-only port
      // Port B: read
      .b_req_i   (rd_en_i),
      .b_we_i    (1'b0),
      .b_addr_i  (rptr_q),
      .b_wdata_i ('0),
      .b_rdata_o (rd_data_ram),
      .cfg_i     (ram_cfg_i),
      .dft_i     (dft_i),
      .alert_o   ()
    );
  end else begin : gen_mem_1p
    // Single-port: read and write share the port.
    // do_write already gates out write during read.
    logic        ram_req;
    logic        ram_we;
    logic [PtrW-1:0] ram_addr;

    assign ram_req  = do_write | rd_en_i;
    assign ram_we   = do_write;
    assign ram_addr = do_write ? wptr_q : rptr_q;

    prim_ram_1p #(
      .Width (Width),
      .Depth (Depth)
    ) u_mem (
      .clk_i,
      .rst_ni,
      .req_i   (ram_req),
      .we_i    (ram_we),
      .addr_i  (ram_addr),
      .wdata_i (wr_data_i),
      .rdata_o (rd_data_ram),
      .cfg_i   (ram_cfg_i),
      .dft_i   (dft_i),
      .alert_o ()
    );
  end

  /* verilator lint_on PINCONNECTEMPTY */

  // ── Read valid tracking ───────────────────────────────

  logic rd_valid_q;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) rd_valid_q <= 1'b0;
    else         rd_valid_q <= rd_en_i;
  end

  // ── Outputs ───────────────────────────────────────────

  assign rd_valid_o = rd_valid_q;
  assign rd_data_o  = rd_valid_q ? rd_data_ram : '0;

endmodule
