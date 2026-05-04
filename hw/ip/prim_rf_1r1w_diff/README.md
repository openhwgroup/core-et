# prim_rf_1r1w_diff

Register file with different read and write widths and sub-word read alignment.

Replaces: `rf_latch_1r_1w_diff_widths` from the CORE-ET source tree.

Implementation selection:
- `mk/prim.mk` exposes this primitive as `PRIM_RF_1R1W_DIFF`
- Current implementations live under `tech_generic/prim_rf_1r1w_diff` and
  `tech_ice40/prim_rf_1r1w_diff` and `tech_ecp5/prim_rf_1r1w_diff` and
  `tech_xilinx/prim_rf_1r1w_diff`

## Description

A 1-read, 1-write register file where the write port is wider than the read port. The read port supports sub-word alignment, allowing reads at finer granularity than the read width. Wrap-around is supported at the last entry boundary.

Used by the minion frontend thread buffer to store 256-bit ICache fetch blocks and read 32-bit instructions at 16-bit (halfword) alignment.

## Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `RWidth` | 32 | Read data width (bits) |
| `RAlignment` | 32 | Read address alignment (bits). Must be <= RWidth. |
| `WWidth` | 32 | Write data width (bits). Must be a multiple of RWidth. |
| `Entries` | 32 | Number of write-width entries |

Derived:
- `R2WRatio = WWidth / RWidth` — number of read-width chunks per write entry
- `REntries = (Entries * WWidth) / RWidth * (RWidth / RAlignment)` — total read addresses
- `RAddrW = $clog2(REntries)` — read address width
- `WAddrW = $clog2(Entries)` — write address width

## Port interface

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_i` | input | 1 | Clock (always-on) |
| `rd_data_o` | output | RWidth | Read data |
| `rd_addr_i` | input | RAddrW | Read address (indexes at RAlignment boundaries) |
| `wr_data_i` | input | WWidth | Write data |
| `wr_data_en_1p_i` | input | R2WRatio | Negedge data latch enable (1 phase early prediction) |
| `wr_addr_i` | input | WAddrW | Write address (selects entry) |
| `wr_en_i` | input | R2WRatio | Posedge write enable (per read-width chunk) |

## Timing

1. **Negedge**: `wr_data_i` latched into internal `wr_data_del`, gated by `wr_data_en_1p_i`
2. **Posedge**: `wr_data_del` written to storage, gated by `wr_en_i`
3. **Read**: Combinational from storage at `rd_addr_i`

The two-phase write (negedge latch + posedge commit) matches the original latch-based RF timing for Verilator simulation.

## Differences from original

| Aspect | Original | Reimplementation | Rationale |
|--------|----------|-----------------|-----------|
| Implementation | Latch array with clock-gated write enables | Technology-selected RF primitive body | Keeps the storage contract behind the same seam as other tech primitives |
| Clock gating | `et_clk_gate` / `et_clk_gate_n` per word | No explicit gated clocks on FPGA/generic targets | Future: foundry RF macro |
| DFT | `test_en` wire | Not needed (no clock gates to override) | — |

## Verification

RTL co-simulation: 50,101 comparisons vs `rf_latch_1r_1w_diff_widths`, 0 mismatches. Tests full writes, partial enables, wrap-around reads, and 50K random cycles.
