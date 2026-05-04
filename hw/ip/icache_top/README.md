// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

# icache_top

Standalone-Minion shared instruction-cache top.

## What It Does

`icache_top` is the shared L1 I-cache shell used by the standalone Minion wrapper. It:

- instantiates one `icache_micro_cache` per requestor
- arbitrates miss requests from the micro-caches
- manages the shared L2-miss FSM
- owns the shared L1 tag, LRU, and data-array blocks
- routes PTW, APB debug, and status-monitor traffic

This is a direct translation of the original CORE-ET `icache_top`.

## Parameters

- `NrMinions`: total number of minions served by the shared I-cache
- `NrRequestors`: number of micro-cache requestors
- `NrMinPerReq`: derived minions per requestor

## Interface Notes

- `f0_req_*` / `f4_resp_*` / `f5_resp_fill_done_*` are the frontend request/response arrays, one slot per requestor.
- `f0_l2_miss_req_*` / `f0_l2_miss_resp_*` connect the shared miss path to L2.
- `f2_sram_req_*` / `f0_sram_resp_*` connect the shared data array to the external SRAM wrapper.
- `ptw_*` is one PTW channel per requestor.
- `apb_*` exposes the shared standalone-Icache debug window.

## Integration Notes

- The module expects translated `icache_micro_cache`, `icache_tag_array`, `icache_lru_array`, and `icache_data_array` leaves.
- The external SRAM wrapper is responsible for the backing-store timing seen on `f2_sram_req_*` and `f0_sram_resp_*`.
- Only requestor 0 receives the ESR prefetch control, matching the original shell.

## Differences From Original

- Reset polarity/style follows project convention: active-low async-assert / sync-deassert `rst_ni` instead of active-high `reset`.
- ET-Link and frontend types come from translated packages rather than macro headers.
