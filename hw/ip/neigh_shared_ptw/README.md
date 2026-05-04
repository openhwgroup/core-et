<!--
Copyright (c) 2026 Ainekko
SPDX-License-Identifier: Apache-2.0
-->

# neigh_shared_ptw

Shared neighborhood PTW wrapper for `standalone_minion`.

## Description

`neigh_shared_ptw` arbitrates page-table-walk requests from multiple requestors,
forwards the winning request into a single shared `ptw_top`, and routes the
resulting response pulse back to the requestor that won arbitration.

The structure matches the original shell exactly:

- `arb_lru_data`-style arbitration over `req_valid`
- one shared `ptw_top`
- a registered request destination tag used to fan out `resp_valid`
- debug/status output taken from the PTW and then overridden with the current
  arbitration winner's request fields

## Parameters

| Parameter | Description |
|-----------|-------------|
| `NReqs` | Number of PTW request ports. Defaults to the original neighborhood value of `5`. |

## Ports

| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `clk_i` | input | `logic` | Clock |
| `rst_ni` | input | `logic` | Active-low async reset |
| `ioshire_i` | input | `logic` | IOShire select, forwarded into `ptw_top` |
| `req_data_i` | input | `minion_pkg::minion_ptw_req[NReqs]` | Per-requestor PTW request payloads |
| `req_valid_i` | input | `logic[NReqs-1:0]` | Per-requestor valid bits |
| `req_ready_o` | output | `logic[NReqs-1:0]` | One-hot grant/ready back to the winning requestor |
| `invalidate_i` | input | `logic[NReqs-1:0]` | Per-requestor invalidate requests, OR-reduced into the shared PTW |
| `mprot_i` | input | `minion_dcache_pkg::esr_mprot_t` | Neighborhood memory-protection ESRs |
| `vmspagesize_i` | input | `minion_pkg::tlb_entry_type` | Shared page-size control |
| `resp_data_o` | output | `minion_pkg::minion_ptw_pte` | Shared PTW response payload |
| `resp_valid_o` | output | `logic[NReqs-1:0]` | Response pulse routed back to the requestor that issued the walk |
| `mem_miss_req_disable_next_i` | input | `logic` | PTW miss suppression hint |
| `mem_miss_req_info_o` | output | `etlink_pkg::neigh_req_t` | PTW ET-Link miss request |
| `mem_miss_req_valid_o` | output | `logic` | PTW ET-Link miss valid |
| `mem_miss_req_ready_i` | input | `logic` | PTW ET-Link miss ready |
| `mem_fill_req_info_i` | input | `etlink_pkg::rsp_t` | ET-Link fill/ack response |
| `mem_fill_req_valid_i` | input | `logic` | ET-Link fill/ack valid |
| `mem_fill_req_ready_o` | output | `logic` | ET-Link fill/ack ready |
| `dbg_sm_signals_o` | output | `neigh_pkg::ptw_dbg_sm_t` | PTW debug/status monitor payload |

## Usage Notes

- `req_valid_i` is forwarded to the shared `ptw_top` as a single OR-reduced
  valid bit, exactly like the original wrapper.
- `req_ready_o` is the arbiter grant bus. When the shared PTW is stalled, the
  grant bus is suppressed, again matching the original shell.
- `resp_valid_o` is one-hot and uses the last accepted request destination.

## Differences From Original

| Original | This repo | Reason |
|----------|-----------|--------|
| `clock` / `reset` | `clk_i` / `rst_ni` | Repository naming + active-low reset convention |
| `arb_lru_data` + separate data mux | `prim_arb_lru` + direct array index | Same arbitration/result behavior expressed with the repo primitive set |

Behavior and cycle timing are intended to match the original
`rtl/shire/neigh/neigh_shared_ptw.v`.
