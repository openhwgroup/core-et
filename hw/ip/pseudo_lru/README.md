# `pseudo_lru`

Pseudo-LRU replacement selector for power-of-two way counts.

This is a direct translation of the original CORE-ET `pseudo_lru` helper used
by the standalone Minion PTW. It tracks a binary-tree PLRU state and produces
the index that should be replaced next.

## Parameters

- `NWays`
  Number of ways. Must be a power of two.

## Ports

- `clk_i`
  Clock.
- `rst_ni`
  Active-low asynchronous reset.
- `access_i`
  When high on a clock edge, updates the PLRU tree using `index_in_i`.
- `index_in_i`
  Accessed way index.
- `index_out_o`
  Current replacement candidate derived from the stored PLRU tree.

## Notes

- Reset polarity is the intentional repo-wide difference from the original.
- The replacement index is purely combinational from the current PLRU state.
