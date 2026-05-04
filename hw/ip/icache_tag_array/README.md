<!--
Copyright (c) 2026 Ainekko
SPDX-License-Identifier: Apache-2.0
-->

# icache_tag_array

Standalone-Icache tag RAM, valid-bit tracking, and debug/APB tag window.

## Description

`icache_tag_array` is the Icache tag-side helper from the original standalone
neighborhood Icache. It stores one physical-address tag per set/way, tracks the
per-line valid bits, invalidates a selected victim on miss, and exposes the tag
state through the old APB debug path.

The module preserves the original half-cycle write protocol:

- the per-way tag memories use the coarser `prim_rf_2r1w_preview` seam
- the miss-invalidated bookkeeping bit remains intentionally unreset, matching
  the original `FF` macro usage

## Parameters

This module has no public parameters. Geometry and widths come from:

- `icache_geom_pkg`
- `icache_pkg`
- `minion_pkg`

## Ports

| Port | Dir | Description |
|------|-----|-------------|
| `clk_i` | in | Clock |
| `rst_ni` | in | Active-low reset for the resettable state |
| `f0_read_valid_i` | in | F0 read request valid |
| `f0_read_paddr_i` | in | Physical address for the F0 read request |
| `f1_read_tag_hit_o` | out | One-hot per-way tag-hit result in F1 |
| `f0_write_early_valid_i` | in | Early fill/debug write preview pulse |
| `f0_write_valid_i` | in | Actual fill/debug write pulse |
| `f0_write_req_i` | in | Tag fill write request |
| `f1_miss_i` | in | Victim invalidation request from F1 |
| `f1_miss_idx_i` | in | Set index of the victim to invalidate |
| `f1_miss_way_i` | in | Way of the victim to invalidate |
| `f0_miss_state_i` | in | Miss FSM state used by the original invalidation bookkeeping |
| `f0_flush_data_i` | in | Flush-all-tags control |
| `apb_paddr_i` | in | Tag-window APB address |
| `apb_pwrite_i` | in | APB write/not-read |
| `apb_psel_i` | in | APB select |
| `apb_penable_i` | in | APB enable |
| `apb_pwdata_i` | in | APB write data |
| `apb_pready_o` | out | APB ready |
| `apb_prdata_o` | out | APB read data |
| `apb_pslverr_o` | out | APB slave error (always 0) |

## Usage Notes

- The APB tag window exposes two ways per 64-bit beat, matching the original
  tag-array packing.
- The valid bit for each half-word lives at bit `IcachePaTagSize` inside the
  corresponding APB half-word.
- `f0_flush_data_i` clears all valid bits immediately and also forces misses on
  the concurrent F1 compare, again matching the original behavior.

## Differences From Original

| Original | This repo | Reason |
|----------|-----------|--------|
| `LATCH_P2` + `rf_latch_2r_1w` | `prim_rf_2r1w_preview` | Coarser seam preserves the low-phase write-preview contract explicitly |
| macro header ranges | package constants/types | Replace global defines with typed package imports |
