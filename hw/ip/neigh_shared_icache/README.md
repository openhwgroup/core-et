// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

# neigh_shared_icache

`neigh_shared_icache` is the neighborhood wrapper around the translated shared
ICache hierarchy.  It accepts fetch requests from the neighborhood minions,
stages one request per minion/thread, arbitrates each requestor group with the
same delayed-priority LRU policy as the original CORE-ET wrapper, and forwards
the winning requests to `icache_top`.

## Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `NrMinions` | `8` | Number of minions sharing the ICache wrapper. Matches original `NR_MINIONS`. |
| `NrRequestors` | `2` | Number of `icache_top` requestor ports. Matches original `NR_REQUESTORS`. |

Derived local parameters split minions evenly across requestors. The translated
wrapper preserves the original assumption that `NrMinions / NrRequestors` is an
integer and that the minion frontend has two threads (`minion_pkg::NrThreads`).

## Port interface

The module follows the original CORE-ET `neigh_shared_icache` protocol with
project naming and type conventions:

- `clk_i`, `rst_ni`: functional clock and active-low reset.
- ESR controls/status: prefetch configuration/start/done, error-log control and
  status, memory-protection, page-size, bypass, and cooperative-mode inputs are
  passed through to `icache_top`.
- Minion request inputs: `f0_req_valid_i[NrMinions]` and
  `f0_req_i[NrMinions]` feed per-minion/per-thread staging registers.
  `f0_req_ready_o[NrMinions]` pulses when any staged thread request from that
  minion is granted to the shared ICache requestor.
- Response outputs: `f4_resp_valid_o`, `f4_resp_miss_o`, `f4_resp_o`, and
  `f5_resp_fill_done_o` are the `icache_top` requestor responses.
- Flush: `f0_flush_data_i[NrMinions]` is flopped for one cycle and reduced to
  the single `icache_top` flush input, matching the original wrapper.
- L2 miss request/response: ET-Link no-data request output and full response
  input are passed through to `icache_top`.
- TLB/PTW: SATP/MATP, invalidates, PTW requests, and PTW responses are passed
  through to `icache_top`.
- SRAM side: request signals are passed through from `icache_top`; the SRAM
  response data port is the original flat `IcacheSramDataWidth` bus and is
  reshaped internally for `icache_top`.
- APB debug and `dbg_sm_signals_o`: APB debug is forwarded to `icache_top`.
  The debug status bundle starts from `icache_top` debug state and overrides
  the wrapper-level `f1_req_valid` / `f1_req_addr` fields exactly like the
  original wrapper.

## Integration notes

The requestor groups are contiguous: requestor `r` arbitrates minions
`r * (NrMinions/NrRequestors)` through
`(r + 1) * (NrMinions/NrRequestors) - 1`.  Within a group, the flattened hart ID
uses bit 0 as the thread ID and the upper bits as the local minion ID.  The
local minion ID is passed to `icache_top` as `f0_req_min_id_i`.

Recommended source order for simulation/lint is the same as `icache_top`, with
this wrapper appended after `icache_top.sv`:

1. `hw/ip/dft/rtl/dft_pkg.sv`
2. `hw/ip/etlink/rtl/etlink_pkg.sv`
3. `hw/ip/minion/rtl/minion_pkg.sv`
4. `hw/ip/minion/tlb/rtl/tlb_pkg.sv`
5. `hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv`
6. `hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv`
7. `hw/ip/icache_pkg/rtl/icache_geom_pkg.sv`
8. `hw/ip/icache_pkg/rtl/icache_pkg.sv`
9. ICache primitive dependencies from `mk/prim.mk`, ECC, `prim_arb_rr`, TLB,
   and translated ICache submodules (`icache_*`, `lru_sel`, `icache_top`)
10. `hw/ip/neigh_shared_icache/rtl/neigh_shared_icache.sv`

## Design constraints and assumptions

- Request staging preserves the original per-minion/per-thread valid bits. A new
  request is captured only when the matching staged thread slot is not already
  valid; a same-cycle grant clears the valid bit after the set branch, matching
  the original macro ordering.
- The wrapper implements the original `arb_lru_delayed_data` behavior locally:
  arbitration grants are combinational from the current priority matrix, but the
  priority matrix is updated later only when `f4_update_prio` is asserted using
  the delayed `f4_grant` vector. This intentionally does **not** use
  `prim_arb_lru`, because that primitive updates priority on the immediate
  grant handshake rather than on the delayed hit/error response.
- The livelock-avoidance counters and mask update preserve the original two-
  thread minion grouping: once any thread counter saturates, both threads of one
  saturating minion remain unmasked until the counters clear.
- No additional DFT or RAM configuration ports are introduced at this wrapper;
  the original wrapper had no independent DFT surface. DFT/RAM abstraction stays
  inside the reused translated `icache_top` hierarchy and primitive wrappers.

## Differences from original CORE-ET

| Original CORE-ET | This repository | Rationale |
|------------------|-----------------|-----------|
| `clock`, active-high `reset` | `clk_i`, active-low `rst_ni` | Repository reset/naming convention. Separate reset domains are not collapsed; the original wrapper has one reset domain. |
| `NR_MINIONS`, `NR_REQUESTORS` | `NrMinions`, `NrRequestors` | LowRISC parameter naming while preserving parameter behavior. |
| Textual ``soc.vh`` types | Package types from `minion_pkg`, `minion_frontend_pkg`, `minion_dcache_pkg`, `icache_pkg`, and `etlink_pkg` | Project-wide package migration. |
| `et_link_*` structs | `etlink_pkg::*` structs | Project ET-Link package. |
| Flat SRAM response into original `icache_top` | Flat wrapper port reshaped internally for translated `icache_top` | Keeps parent-visible wrapper bus shape while reusing the translated ICache hierarchy. |
| `arb_lru_delayed_data` instance | Local delayed-priority LRU translation | Preserves the delayed priority update semantics without introducing a new reusable primitive. |

No intentional functional divergence from the original CORE-ET
`neigh_shared_icache` is introduced.
