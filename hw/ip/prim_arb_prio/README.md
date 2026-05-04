# prim_arb_prio — Priority Arbiter with Data Selection

## Description

Priority arbiter that grants the highest-indexed bidder and selects the
winner's data.  When stall is asserted no grant is issued.  When no bidder
wins, `data_o` holds the previous winner's data (output hold).

Replaces: `arb_prio_data_held` + `arb_prio` from the CORE-ET source tree
(`rtl/libs/arbiters/`).

## Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `NumClients` | 2 | Number of arbiter clients |
| `Width` | 32 | Bit width of each client's data |

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_i` | input | 1 | Clock |
| `rst_ni` | input | 1 | Active-low async reset |
| `bid_i` | input | NumClients | Bid vector — asserted bits are requesting |
| `stall_i` | input | 1 | Stall — suppresses all grants |
| `grant_o` | output | NumClients | One-hot grant vector |
| `data_i` | input | NumClients x Width | Per-client data inputs |
| `data_o` | output | Width | Selected winner's data |

## Behaviour

- `grant_o` is one-hot-zero: exactly one bit set when there is a winner,
  zero when stalled or no bids.
- When `stall_i` is high, no grant is issued.
- When `stall_i` is low and at least one `bid_i` bit is set, the
  highest-indexed bidder wins.
- `data_o` selects the winner's `data_i` slice.  When no grant is active,
  `data_o` continues to output the previous winner's data (hold behaviour).
- After reset, the held winner index is zero, so `data_o` defaults to
  `data_i[0]`.

## Differences from original

| Aspect | Original (`arb_prio_data_held` + `arb_prio`) | This module |
|--------|----------------------------------------------|-------------|
| Reset | Active-high synchronous | Active-low async-assert/sync-deassert |
| Naming | `clock`/`reset`, `bid`/`grant`/`data_in`/`data_winner` | `clk_i`/`rst_ni`, `bid_i`/`grant_o`/`data_i`/`data_o` |
| Coding | `RST_EN_FF` macro, separate `arb_prio` sub-module | Explicit `always_ff`, single self-contained module |
