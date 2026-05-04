# icache_lru_array

Standalone-Icache LRU state array.

This block stores one LRU matrix per I-cache set, returns the victim way for
the set being accessed in F0/F1, and updates the stored state from the F1
accessed-way information. It is the direct translation of the original
`icache_lru_array` from the standalone Minion I-cache path.

## Parameters

None. Geometry comes from:
- `icache_geom_pkg`
- `icache_pkg`

## Ports

| Port | Dir | Width | Description |
|------|-----|-------|-------------|
| `clk_i` | in | 1 | Clock |
| `rst_ni` | in | 1 | Active-low reset for the initialized-set bitmap |
| `f0_valid_i` | in | 1 | F0 victim-read request valid |
| `f0_access_set_i` | in | `IcacheSetAddrWidth` | Set index being accessed in F0 |
| `f1_victim_o` | out | `IcacheWays` | One-hot victim way for the F0 request, registered into F1 |
| `f1_update_en_early_i` | in | 1 | Early F1 write arm for the LRU RF |
| `f1_update_en_i` | in | 1 | F1 LRU update commit enable |
| `f1_update_set_i` | in | `IcacheSetAddrWidth` | Set whose LRU state is being updated |
| `f1_update_access_way_i` | in | `IcacheWays` | One-hot accessed way used to compute the next LRU state |

## Implementation notes

- Same-set F1->F0 bypass is preserved: if an update and a read target the same
  set in the same cycle, the F0 victim selection sees the updated LRU state.
- The original low-phase write-preview plus `rf_latch_1r_1w` storage contract
  is translated with the coarser `prim_rf_1r1w_preview` seam.
- Until a set is written once, the read LRU state is forced to zero and the
  victim defaults to way 0, matching the original initialized-bitmap behavior.

## Differences from original

| Aspect | Original | New | Rationale |
|--------|----------|-----|-----------|
| Reset | active-high `reset` | active-low `rst_ni` | Repo reset convention |
| Includes/macros | `` `include "soc.vh" `` and FF macros | package imports + explicit primitives/`always_ff` | Repo coding style |
| RF/latch cells | `rf_latch_1r_1w` + `` `LATCH_P2 `` | `prim_rf_1r1w_preview` | Coarser write-preview seam keeps the original phase contract together |

## Verification

- Unit test: `make -C hw/ip/icache_lru_array/dv test`
- RTL cosim: `ORIG_ROOT=/path/to/etcore-soc make -C dv/rtlcosim/icache_lru_array test`
