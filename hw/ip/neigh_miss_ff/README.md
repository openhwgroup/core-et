<!--
Copyright (c) 2026 Ainekko
SPDX-License-Identifier: Apache-2.0
-->

# neigh_miss_ff

Neighborhood-side miss request holding registers for `standalone_minion`.

## Description

`neigh_miss_ff` stores miss requests that have already left the Minion dcache
but have not yet been consumed by either:

- the regular neighborhood-to-L2 miss path, or
- one of the two cooperative tensor-load miss paths

The original design keeps one holding register per outgoing miss class:

- `DcacheL2MissIdxTenb` = cooperative TenB miss
- `DcacheL2MissIdxScp` = cooperative Scp miss
- `DcacheL2MissIdxComm` = regular miss to the L2 arbiter

Each slot captures the shared incoming request bundle when its corresponding
valid bit is accepted. A slot remains busy until its matching downstream ready
signal consumes it.

## Parameters

None.

## Ports

| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `clk_i` | input | `logic` | Clock |
| `rst_ni` | input | `logic` | Active-low async reset |
| `l2_dcache_req_i` | input | `et_link_minion_miss_req_info_t` | Shared miss request payload from dcache |
| `l2_dcache_req_valid_i` | input | `logic[DcacheL2MissReqPorts-1:0]` | Per-slot valid bits |
| `l2_dcache_req_ready_o` | output | `logic[DcacheL2MissReqPorts-1:0]` | Per-slot availability |
| `miss_req_o` | output | `etlink_pkg::neigh_req_t` | Regular miss request toward neighborhood L2 arbitration |
| `miss_valid_o` | output | `logic` | Regular miss slot occupied |
| `miss_ready_i` | input | `logic` | Regular miss consumer accepted the request |
| `coop_miss_req_o` | output | `et_link_minion_miss_req_info_t[ShireCoopMissReqs-1:0]` | Cooperative miss payloads |
| `coop_miss_valid_o` | output | `logic[ShireCoopMissReqs-1:0]` | Cooperative slots occupied |
| `coop_miss_ready_i` | input | `logic[ShireCoopMissReqs-1:0]` | Cooperative consumers accepted the requests |
| `dbg_sm_signals_o` | output | `logic[DcacheL2MissReqPorts-1:0]` | Raw slot availability state, matching the original debug output |

## Usage Notes

- The input request payload is shared across all three slots, exactly like the
  original module.
- If multiple valid bits are asserted in the same cycle while multiple slots
  are free, the same payload is captured into multiple slots. This is original
  behavior and is preserved intentionally.
- `miss_req_o.source` is forced to zero, matching the original neighborhood
  shell. The true source tag is assigned later by the surrounding wrapper.

## Differences From Original

| Original | This repo | Reason |
|----------|-----------|--------|
| `clock` / `reset` | `clk_i` / `rst_ni` | Repository naming + active-low reset convention |
| `RST_FF` / `EN_FF` macros | explicit `always_ff` / `always_comb` | Repository coding style |

Behavior and cycle timing are intended to match the original
`rtl/shire/neigh/neigh_miss_ff.v`.
