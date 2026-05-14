// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Data RAM wrapper.
//
// Per-quadword write enable. Read and write are mutually exclusive.
// Preserves the original sram_clock_pre pulse generator; ram_delay_i is
// assertion/control-plane state, not a clock extender.

/* verilator lint_off UNUSEDSIGNAL */  // dft_t/ram_cfg_t passed whole; wrapper only uses a subset of fields
/* verilator lint_off VARHIDDEN */  // local parameters intentionally shadow shirecache_pkg constants
/* verilator lint_off PINCONNECTEMPTY */  // alert_o and write-port rdata_o intentionally unconnected
module shirecache_pipe_data_ram_wrap
  import dft_pkg::*;
  import ram_cfg_pkg::*;
  import shirecache_pkg::*;
#(
  parameter int unsigned SetsPerSubBank = 256,
  // Derived
  localparam int unsigned Depth      = SetsPerSubBank * Ways,
  localparam int unsigned AddrSize   = $clog2(Depth),
  localparam int unsigned DataSize   = DataRamDataSize,
  localparam int unsigned QwSize     = LineQwSize,
  localparam int unsigned QwDataSize = DataSize / QwSize
) (
  input  logic                    clk_i,
  input  logic                    rst_ni,

  input  logic                    dft_sram_clk_i,

  input  logic                    clock_en_i,
  input  logic [RamDelaySize-1:0] ram_delay_i,

  input  logic                    rd_en_i,
  input  logic                    wr_en_i,
  input  logic [AddrSize-1:0]     addr_i,
  input  logic [DataSize-1:0]     wr_data_i,
  input  logic [QwSize-1:0]       wr_qwen_i,
  output logic [DataSize-1:0]     rd_data_o,

  input  ram_cfg_t                ram_cfg_i,
  input  dft_t                    dft_i
);

  // -- Clock generation --
  // Faithful translation of the original wrapper. A one-cycle clock_en_i
  // request creates one SRAM clock pulse via sram_clock_pre; ram_delay_i is
  // assertion/control-plane information and does not extend the SRAM clock.
  logic sram_clock_pre;
  logic sram_clock_en;

  assign sram_clock_en = sram_clock_pre | clock_en_i;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni)
      sram_clock_pre <= 1'b0;
    else if (sram_clock_en)
      sram_clock_pre <= clock_en_i;
  end

  logic sram_clk;

  prim_clk_mux u_clk_mux (
    .clk0_i (sram_clock_pre),
    .clk1_i (dft_sram_clk_i),
    .sel_i  (dft_i.sram_clk_override),
    .clk_o  (sram_clk)
  );

  // -- Per-quadword RAM instances --
  for (genvar qw = 0; qw < int'(QwSize); qw++) begin : gen_qw

    logic qw_req, qw_we;
    // Original gen_mem1p instances use .en(1'b1 && !dft__ram_rei)
    // on every generated SRAM clock edge. Disabled write lanes therefore
    // perform reads and update rd_data_o while enabled. Preserve that
    // cycle-visible wrapper behavior rather than narrowing req_i to rd/write.
    assign qw_req = ~dft_i.ram_rei;
    assign qw_we  = wr_en_i & wr_qwen_i[qw] & ~dft_i.ram_wei;

    prim_ram_1p #(
      .Width (QwDataSize),
      .Depth (Depth)
    ) u_data_ram (
      .clk_i   (sram_clk),
      .rst_ni,
      .req_i   (qw_req),
      .we_i    (qw_we),
      .addr_i,
      .wdata_i (wr_data_i[qw*QwDataSize +: QwDataSize]),
      .rdata_o (rd_data_o[qw*QwDataSize +: QwDataSize]),
      .cfg_i   (ram_cfg_i),
      .dft_i   (dft_i),
      .alert_o ()
    );

  end

  // -- Assertions --
  // synthesis translate_off
`ifdef VERILATOR
  /* verilator lint_off SYNCASYNCNET */  // rst_ni is async-assert/sync-deassert, used here only as assertion guard
  always_ff @(posedge clk_i) begin
    if (rst_ni && clock_en_i && rd_en_i && wr_en_i)
      $error("shirecache_pipe_data_ram_wrap: simultaneous rd_en and wr_en");
  end
  /* verilator lint_on SYNCASYNCNET */
`endif
  // synthesis translate_on

endmodule
/* verilator lint_on PINCONNECTEMPTY */
/* verilator lint_on VARHIDDEN */
/* verilator lint_on UNUSEDSIGNAL */
