// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Tag RAM wrapper.
//
// Single-port RAM storing 4-way tag data per set. Read and write
// are mutually exclusive. Output is registered (1-cycle read latency).
//
// Replaces: shire_cache_pipe_tag_ram_wrap (etcore-soc)
//
// Differences from original:
//   - Active-low async-assert/sync-deassert reset (rst_ni)
//   - prim_ram_1p instead of gen_mem1p
//   - prim_clk_mux instead of et_clk_mux2
//   - DFT consolidated into dft_t + dft_sram_clk_i
//   - ram_delay_i explicit port (replaces hierarchical ref pipe.sub_bank.*)
//   - ram_cfg_t instead of esr_shire_cache_ram_cfg_t

/* verilator lint_off UNUSEDSIGNAL */  // dft_t/ram_cfg_t passed whole; wrapper only uses a subset of fields
/* verilator lint_off PINCONNECTEMPTY */  // alert_o intentionally unconnected
module shirecache_pipe_tag_ram_wrap
  import dft_pkg::*;
  import ram_cfg_pkg::*;
  import shirecache_pkg::*;
#(
  parameter int unsigned SetsPerSubBank = 256,
  localparam int unsigned AddrSize = $clog2(SetsPerSubBank),
  localparam int unsigned DataSize = TagRamDataSize
) (
  input  logic                 clk_i,
  input  logic                 rst_ni,

  // DFT SRAM clock override
  input  logic                 dft_sram_clk_i,

  // Clock enable and RAM delay
  input  logic                 clock_en_i,
  input  logic [RamDelaySize-1:0] ram_delay_i,  // replaces pipe.sub_bank.esr_sc_ram_delay_*

  // RAM access
  input  logic                 rd_en_i,
  input  logic                 wr_en_i,
  input  logic [AddrSize-1:0]  addr_i,
  input  logic [DataSize-1:0]  wr_data_i,
  output logic [DataSize-1:0]  rd_data_o,

  // Technology hooks
  input  ram_cfg_t             ram_cfg_i,
  input  dft_t                 dft_i
);

  // ── SRAM clock generation ──────────────────────────────
  // Faithful translation of original:
  //   wire sram_clock_en = sram_clock_pre || clock_en;
  //   RST_EN_FF(clock, reset, sram_clock_en, sram_clock_pre, clock_en, '0)
  //   et_clk_mux2 clk_mux (.x(sram_clock), .d0(sram_clock_pre),
  //                          .d1(dft__sram_clock), .s(dft__clk_override));
  //
  // The original does NOT use et_clk_gate here. sram_clock_pre is a
  // data-derived FF output used directly as clock source via et_clk_mux2.
  // The ram_delay / vec_clock_en_dN pipeline in the original is
  // assertion-only (inside ifndef ET_ASSERT_OFF).

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

  // ── RAM instance ──────────────────────────────────────
  // Faithful translation of original:
  //   gen_mem1p(.clock(sram_clock),
  //             .en(1'b1 && !ram_rei),
  //             .we(wr_en && !ram_wei), ...)

  prim_ram_1p #(
    .Width (DataSize),
    .Depth (SetsPerSubBank)
  ) u_tag_ram (
    .clk_i   (sram_clk),
    .rst_ni,
    .req_i   (~dft_i.ram_rei),           // always enabled (matches .en(1'b1 && !ram_rei))
    .we_i    (wr_en_i & ~dft_i.ram_wei), // write when wr_en and not inhibited
    .addr_i,
    .wdata_i (wr_data_i),
    .rdata_o (rd_data_o),
    .cfg_i   (ram_cfg_i),
    .dft_i   (dft_i),
    .alert_o ()
  );

  // ── Assertions ────────────────────────────────────────
  // synthesis translate_off
`ifdef VERILATOR
  /* verilator lint_off SYNCASYNCNET */  // rst_ni is async-assert/sync-deassert, used here only as assertion guard
  always_ff @(posedge clk_i) begin
    if (rst_ni && clock_en_i && rd_en_i && wr_en_i)
      $error("shirecache_pipe_tag_ram_wrap: simultaneous rd_en and wr_en");
  end
  /* verilator lint_on SYNCASYNCNET */
`endif
  // synthesis translate_on

  // Unused
  logic unused_ok;
  assign unused_ok = &{1'b0, rd_en_i, ram_delay_i};

endmodule
/* verilator lint_on PINCONNECTEMPTY */
/* verilator lint_on UNUSEDSIGNAL */
