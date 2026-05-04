<!--
Copyright (c) 2026 Ainekko
SPDX-License-Identifier: Apache-2.0
-->

# icache_micro_data_array

`icache_micro_data_array` is the standalone Minion micro-Icache data-store
helper. It multiplexes normal micro-cache reads and fills with the local debug
access path, preserves the original half-cycle write-preview protocol, and
returns zero-latency line reads from the decoded-entry storage.

## Interface

### System

- `clk_i`
  Functional clock. The original block has no reset and the translation keeps
  that contract.

### Normal micro-cache side

- `f3_rd_en_i`, `f3_rd_addr_dec_i`, `f3_rd_data_o`
  Normal read request and combinational read data return.
- `f0_wr_en_i`, `f0_wr_en_early_i`, `f0_wr_addr_dec_i`, `f0_wr_data_i`
  Normal fill/write path. `f0_wr_en_early_i` is the phase-early write preview
  that precedes the real write controls by one phase, matching the original
  latch-timed register-file seam.

### Debug side

- `dbg_addr_dec_i`, `dbg_write_en_i`, `dbg_write_en_early_i`,
  `dbg_write_data_i`, `dbg_write_ready_o`
  Debug write path multiplexed onto the same decoded-entry array.
- `dbg_read_en_i`, `dbg_read_data_o`, `dbg_read_ready_o`
  Debug read path. Debug reads are only marked ready when they are not blocked
  by a normal micro-cache read in the same cycle.

## Integration notes

- Read and write priority matches the original:
  - normal `f3` reads win over debug reads
  - normal `f0` writes win over debug writes
- The storage seam uses `prim_rf_1r1w_dec_preview`, which preserves the
  original `LATCH_P2` plus `rf_latch_1r_1w_dec` half-cycle timing contract as
  one named primitive boundary.

## Intentional differences from the original

| Original | Translation |
|----------|-------------|
| `clock` | `clk_i` |
| `WIDTH` / `ENTRIES` macro defaults | `Width` / `Entries` parameters with package-backed defaults |
| `LATCH_P2` + `rf_latch_1r_1w_dec` | `prim_rf_1r1w_dec_preview` |

No intentional functional differences are introduced.
