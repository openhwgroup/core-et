// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// Shire cache BIST mailbox — stitches MBIST signals to logical memory bus.
//
// Feedthrough path: routes mbist_* signals to mbx_* design ports with
// back-to-back access blocking for RAMs with multi-cycle delay.
//
// The original's et_bist_tester (DV-only ASIC BIST) is not instantiated;
// on ASIC the BIST insertion tool replaces this with actual BIST logic.
//
// Replaces: shire_cache_bist_mbx (etcore-soc)

/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNUSEDPARAM */
module shirecache_bist_mbx #(
  parameter int unsigned DataSize         = 16,
  parameter int unsigned AddrSize         = 8,
  parameter int unsigned RamDelay         = 1,
  parameter int unsigned LogicalRamDepth  = (1 << AddrSize)
) (
  input  logic                    clk_i,
  input  logic                    rst_ni,
  input  logic                    mbx_impl_i,

  // MBIST interface (from bist_wrapper)
  input  logic                    mbist_sel_i,
  input  logic                    mbist_rd_en_i,
  input  logic                    mbist_wr_en_i,
  input  logic [AddrSize-1:0]     mbist_addr_i,
  input  logic [DataSize-1:0]     mbist_wdata_i,
  output logic [DataSize-1:0]     mbist_rdata_o,

  // Design memory interface
  output logic                    mbx_sel_o,
  output logic                    mbx_rd_en_o,
  output logic                    mbx_wr_en_o,
  output logic [AddrSize-1:0]     mbx_addr_o,
  output logic [DataSize-1:0]     mbx_wdata_o,
  input  logic [DataSize-1:0]     mbx_rdata_i
);

  // Block back-to-back operations for multi-cycle RAM delay.
  // Prevents BIST auto-detect from finding single-cycle-capable operations.
  logic block_access;

  if (RamDelay > 1) begin : gen_ram_delay_gt1
    logic [RamDelay-2:0] vec_active_q;
    logic                active_d0;
    assign active_d0 = mbist_sel_i & (mbist_rd_en_i | mbist_wr_en_i) & ~block_access;

    logic [RamDelay-2:0] vec_active_d;
    if (RamDelay >= 3) begin : gen_shift
      assign vec_active_d = {vec_active_q[0 +: RamDelay-2], active_d0};
    end else begin : gen_single
      assign vec_active_d = active_d0;
    end

    always_ff @(posedge clk_i or negedge rst_ni) begin
      if (!rst_ni)
        vec_active_q <= '0;
      else if (|vec_active_q | active_d0)
        vec_active_q <= vec_active_d;
    end

    assign block_access = |vec_active_q;
  end else begin : gen_ram_delay1
    assign block_access = 1'b0;
  end

  // Feedthrough with blocking
  assign mbx_sel_o     = mbist_sel_i;
  assign mbx_rd_en_o   = mbist_rd_en_i & ~block_access;
  assign mbx_wr_en_o   = mbist_wr_en_i & ~block_access;
  assign mbx_addr_o    = mbist_addr_i;
  assign mbx_wdata_o   = mbist_wdata_i;
  assign mbist_rdata_o = mbx_impl_i ? mbx_rdata_i : '0;

  logic unused_ok;
  assign unused_ok = &{1'b0, LogicalRamDepth[0], unused_ok};

  /* verilator lint_on UNUSEDPARAM */
  /* verilator lint_on UNUSEDSIGNAL */

endmodule
