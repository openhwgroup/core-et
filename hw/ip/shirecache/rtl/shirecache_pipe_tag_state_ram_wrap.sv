// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Tag state RAM wrapper (LRU + flags).
//
// Supports single-port and dual-port modes via DualPort parameter.
// Dual-port mode preserves the original sram_clock_pre pulse generator;
// ram_delay_i is assertion/control-plane state, not a clock extender.

/* verilator lint_off UNUSEDSIGNAL */  // dft_t/ram_cfg_t passed whole; wrapper only uses a subset of fields
/* verilator lint_off VARHIDDEN */  // local parameters intentionally shadow shirecache_pkg constants
/* verilator lint_off PINCONNECTEMPTY */  // alert_o and write-port rdata_o intentionally unconnected
module shirecache_pipe_tag_state_ram_wrap
  import dft_pkg::*;
  import ram_cfg_pkg::*;
  import shirecache_pkg::*;
#(
  parameter int unsigned SetsPerSubBank = 256,
  parameter bit          DualPort       = 1'b1,
  // Derived
  localparam int unsigned AddrSize = $clog2(SetsPerSubBank),
  localparam int unsigned DataSize = TagStateRamDataSize
) (
  input  logic                    clk_i,
  input  logic                    rst_ni,

  input  logic                    dft_sram_clk_i,

  input  logic                    clock_en_i,
  input  logic [RamDelaySize-1:0] ram_delay_i,

  input  logic                    rd_en_i,
  input  logic                    wr_en_i,
  input  logic [AddrSize-1:0]     rd_addr_i,
  input  logic [AddrSize-1:0]     wr_addr_i,
  input  logic [DataSize-1:0]     wr_data_i,
  output logic [DataSize-1:0]     rd_data_o,

  input  ram_cfg_t                ram_cfg_i,
  input  dft_t                    dft_i
);

  // ── Clock generation ──────────────────────────────────

  logic sram_clk;

  generate
    if (DualPort) begin : gen_dp_clk
      // Faithful translation of the original dual-port configuration:
      // sram_clock_pre produces one SRAM clock pulse for each accepted
      // clock_en_i request; ram_delay_i is assertion/control-plane state only.
      logic sram_clock_pre;
      logic sram_clock_en;

      assign sram_clock_en = sram_clock_pre | clock_en_i;

      always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni)
          sram_clock_pre <= 1'b0;
        else if (sram_clock_en)
          sram_clock_pre <= clock_en_i;
      end

      prim_clk_mux u_clk_mux (
        .clk0_i (sram_clock_pre),
        .clk1_i (dft_sram_clk_i),
        .sel_i  (dft_i.sram_clk_override),
        .clk_o  (sram_clk)
      );
    end else begin : gen_sp_clk
      // Original single-port configuration used et_clk_gate(clock_en_i).
      // Keep the same enable contract while retaining the translated DFT
      // clock-mux hook at this wrapper boundary.
      logic func_clk_gated;

      prim_clk_gate u_clk_gate (
        .clk_i  (clk_i),
        .en_i   (clock_en_i),
        .dft_i  (dft_i),
        .clk_o  (func_clk_gated)
      );

      prim_clk_mux u_clk_mux (
        .clk0_i (func_clk_gated),
        .clk1_i (dft_sram_clk_i),
        .sel_i  (dft_i.sram_clk_override),
        .clk_o  (sram_clk)
      );
    end
  endgenerate

  // ── RAM instance ──────────────────────────────────────

  logic rd_en_gated, wr_en_gated;
  assign rd_en_gated = rd_en_i & ~dft_i.ram_rei;
  assign wr_en_gated = wr_en_i & ~dft_i.ram_wei;

  if (DualPort) begin : gen_dp

    prim_ram_2p #(
      .Width (DataSize),
      .Depth (SetsPerSubBank)
    ) u_tag_state_ram (
      .clk_i   (sram_clk),
      .rst_ni,
      .a_req_i   (wr_en_gated),
      .a_we_i    (1'b1),
      .a_addr_i  (wr_addr_i),
      .a_wdata_i (wr_data_i),
      .a_rdata_o (),
      .b_req_i   (rd_en_gated),
      .b_we_i    (1'b0),
      .b_addr_i  (rd_addr_i),
      .b_wdata_i ('0),
      .b_rdata_o (rd_data_o),
      .cfg_i     (ram_cfg_i),
      .dft_i     (dft_i),
      .alert_o   ()
    );

  end else begin : gen_sp

    logic ram_req;
    logic [AddrSize-1:0] ram_addr;
    assign ram_req  = rd_en_gated | wr_en_gated;
    assign ram_addr = wr_en_gated ? wr_addr_i : rd_addr_i;

    prim_ram_1p #(
      .Width (DataSize),
      .Depth (SetsPerSubBank)
    ) u_tag_state_ram (
      .clk_i   (sram_clk),
      .rst_ni,
      .req_i   (ram_req),
      .we_i    (wr_en_gated),
      .addr_i  (ram_addr),
      .wdata_i (wr_data_i),
      .rdata_o (rd_data_o),
      .cfg_i   (ram_cfg_i),
      .dft_i   (dft_i),
      .alert_o ()
    );

  end

endmodule
/* verilator lint_on PINCONNECTEMPTY */
/* verilator lint_on VARHIDDEN */
/* verilator lint_on UNUSEDSIGNAL */
