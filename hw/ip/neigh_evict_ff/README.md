<!-- Copyright (c) 2026 Ainekko -->
<!-- SPDX-License-Identifier: Apache-2.0 -->

# neigh_evict_ff

Neighborhood-side buffering for Minion L2 evict traffic.

This IP contains two translated modules from the original standalone Minion
shell:

- `neigh_evict_ff_unit`: one-pass/two-pass evict buffer for a single dcache
  evict source
- `neigh_evict_ff`: thin wrapper that instantiates one regular evict buffer and
  one cooperative tensor-store evict buffer

The logic matches the original behavior:

- incoming 256-bit evict chunks are collected until the full request is ready
- regular evicts are forwarded as neighborhood ET-Link requests
- selected 256-bit ESR message evicts are redirected onto the local fast
  message network
- cooperative tensor-store evicts use a second identical buffer with local
  routing disabled

## Modules

### `neigh_evict_ff_unit`

Single outstanding evict buffer for one request source.

| Port | Dir | Type | Description |
|------|-----|------|-------------|
| `clk_i` | input | `logic` | Clock |
| `rst_ni` | input | `logic` | Active-low reset |
| `shire_id_i` | input | `logic [7:0]` | Local shire ID used for local-message address matching |
| `neigh_id_i` | input | `logic [1:0]` | Local neighborhood ID used for local-message address matching |
| `l2_dcache_req_i` | input | `et_link_minion_evict_req_info_t` | Minion dcache evict request chunk |
| `l2_dcache_req_valid_i` | input | `logic` | Request-valid for the current chunk |
| `l2_dcache_req_ready_o` | output | `logic` | Buffer-ready backpressure |
| `local_channel_mask_i` | input | `logic [2:0]` | Which local fast-message channels are wired for this source |
| `local_dest_id_i` | input | `logic [2:0][2:0]` | Per-channel local destination minion IDs |
| `local_message_ready_i` | input | `logic` | Local fast-message network ready |
| `local_message_valid_o` | output | `logic [2:0]` | Per-channel local fast-message valid |
| `local_message_info_o` | output | `etlink_pkg::neigh_rsp_t` | Local fast-message payload |
| `evict_req_o` | output | `etlink_pkg::neigh_req_t` | Neighborhood ET-Link evict request |
| `evict_valid_o` | output | `logic` | Neighborhood ET-Link valid |
| `evict_ready_i` | input | `logic` | Neighborhood ET-Link ready |
| `dbg_sm_signals_o` | output | `logic` | Status-monitor availability bit |

### `neigh_evict_ff`

Wrapper that preserves the original two-buffer split:

- `l2_dcache_req_valid_i[1]` is the regular/common evict path
- `l2_dcache_req_valid_i[0]` is the cooperative tensor-store evict path

The cooperative path reuses `neigh_evict_ff_unit` but disables local-message
routing, matching the original wrapper.

## Integration notes

- The module depends on `etlink_pkg` for neighborhood request/response structs
  and on `minion_dcache_pkg` for the original Minion evict-request struct.
- Local-message routing is only taken for ESR message-send requests targeting a
  local minion and carrying at most 256 bits (`SizeHLine`).
- Full-width 512-bit evicts are emitted as two 256-bit neighborhood passes.

## Differences from original

| Area | Original | Ainekko |
|------|----------|---------|
| Reset/style | `clock` / active-high `reset` with macros | `clk_i` / active-low `rst_ni` with explicit `always_ff` |
| ET-Link types | global typedefs from `soc.vh` | `etlink_pkg::neigh_req_t`, `etlink_pkg::neigh_rsp_t`, `etlink_pkg::req_nodata_t` |
| Naming | CORE-ET signal names | lowRISC-style `_i` / `_o` / `_q` / `_d` |

## Verification

- Unit tests: `make -C hw/ip/neigh_evict_ff/dv test`
- RTL cosim:
  - `ORIG_ROOT=/path/to/etcore-soc make -C dv/rtlcosim/neigh_evict_ff_unit test`
  - `ORIG_ROOT=/path/to/etcore-soc make -C dv/rtlcosim/neigh_evict_ff test`
