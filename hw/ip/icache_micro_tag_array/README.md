<!--
Copyright (c) 2026 Ainekko
SPDX-License-Identifier: Apache-2.0
-->

# icache_micro_tag_array

Micro-cache tag array for the standalone Icache path.

## Description

`icache_micro_tag_array` stores the standalone L0/micro-cache tags, tracks the
valid-entry bitmap, clears selected victims on miss, and provides the APB debug
window for the micro-cache tag state.

This is a direct translation of the original `icache_micro_tag_array`:

- `prim_rf_1r1w_par_preview` replaces the original `LATCH_P2` plus
  `rf_latch_1r_1w_par` storage seam
- the invalidation bookkeeping bit remains intentionally unreset

## Parameters

This module has no public parameters. Geometry and widths come from
`icache_geom_pkg`, `icache_pkg`, and `minion_pkg`.

## Ports

| Port | Dir | Description |
|------|-----|-------------|
| `clk_i` | in | Clock |
| `rst_ni` | in | Active-low reset for the valid-entry state |
| `f2_rd_tag_i` | in | Micro-cache tag to compare |
| `f2_rd_hit_array_o` | out | Per-entry hit vector |
| `f0_wr_early_valid_i` | in | Early fill/debug write preview pulse |
| `f0_wr_valid_i` | in | Actual fill write pulse |
| `f0_wr_victim_i` | in | One-hot victim entry for a fill |
| `f0_wr_tag_i` | in | Tag written during a fill |
| `f2_miss_i` | in | Victim invalidation request |
| `f2_miss_victim_i` | in | One-hot victim entry to invalidate |
| `f0_miss_state_i` | in | Miss FSM state used by the invalidation bookkeeping |
| `f0_flush_data_i` | in | Flush-all-entries control |
| `apb_*` | in/out | Micro-tag debug APB window |

## Differences From Original

| Original | This repo | Reason |
|----------|-----------|--------|
| `LATCH_P2` + `rf_latch_1r_1w_par` | `prim_rf_1r1w_par_preview` | Coarser seam preserves the original low-phase write-preview timing |
| macro headers | typed packages | Replace global defines/types with package imports |
