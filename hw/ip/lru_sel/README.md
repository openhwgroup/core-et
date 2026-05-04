# lru_sel

`lru_sel` is the small one-hot least-recently-used selector used by the
standalone Minion micro-Icache.

The block stores a triangular priority matrix. Each asserted bit in
`access_entry_i` marks the corresponding client as most recently used, and the
module returns the least-recently-used client as a one-hot vector on
`victim_o`.

## Interface

- `clk_i`, `rst_ni`
  Functional clock and active-low asynchronous reset.
- `access_entry_i`
  One-hot or sparse-hot access update vector.
- `victim_o`
  One-hot victim selection result.

## Intentional differences from the original

| Original | Translation |
|----------|-------------|
| `clock`, active-high `reset` | `clk_i`, active-low `rst_ni` |
| Macro-based flops/assertions | Explicit `always_ff` / `always_comb` |

No intentional functional differences are introduced.
