# prim_arb_lru — LRU Arbiter

## Description

LRU (Least Recently Used) arbiter using a triangle priority-bits matrix.
Among all clients asserting `bid_i`, grants the one that was used least recently.
After a grant, the winner moves to MRU (lowest priority) position.

Replaces: `arb_lru` from the CORE-ET source tree (`rtl/libs/arbiters/arb_lru.v`).

## Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `NumClients` | 8 | Number of arbiter clients |
| `NumClientsLog` | $clog2(NumClients) | Bit width of winner index |

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_i` | input | 1 | Clock |
| `rst_ni` | input | 1 | Active-low async reset |
| `bid_i` | input | NumClients | Bid vector — asserted bits are requesting |
| `stall_i` | input | 1 | Stall — suppresses grant and priority update |
| `grant_o` | output | NumClients | One-hot grant vector |
| `winner_o` | output | NumClientsLog | Index of the granted client |

## Behaviour

- `grant_o` is one-hot-zero: exactly one bit set when there is a winner, zero when stalled or no bids.
- When `stall_i` is high, no grant is issued and the priority matrix is not updated.
- When `stall_i` is low and at least one `bid_i` bit is set, the LRU bidder wins and the matrix updates.
- After reset, all priority bits are zero — client 0 has initial highest priority.

## Differences from original

| Aspect | Original (`arb_lru`) | This module |
|--------|---------------------|-------------|
| Reset | Active-high synchronous | Active-low async-assert/sync-deassert |
| Naming | `clock`/`reset`, `bid`/`grant`/`winner` | `clk_i`/`rst_ni`, `bid_i`/`grant_o`/`winner_o` |
| Coding | `RST_EN_FF` macro | Explicit `always_ff` |

---

# prim_arb_lru_2bid — Dual-Bid LRU Arbiter

## Description

Dual-bid LRU arbiter using the same triangle priority-bits matrix as `prim_arb_lru`.
Computes two independent grants from two bid vectors each cycle.
Only one pop per cycle (either pop0 or pop1, never both).
The popped grant updates the LRU state.

This supports a priority/opportunistic arbitration pattern: bid0 identifies
the LRU winner among all valid requestors, while bid1 identifies the LRU
winner among requestors whose resources are currently available.

Replaces: `arb_lru_2_bid` from the CORE-ET source tree (`rtl/libs/arbiters/arb_lru_2_bid.v`).

## Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `NumClients` | 8 | Number of arbiter clients |

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_i` | input | 1 | Clock |
| `rst_ni` | input | 1 | Active-low async reset |
| `bid0_i` | input | NumClients | Bid vector 0 (priority channel) |
| `grant0_o` | output | NumClients | One-hot grant for bid0 |
| `pop0_i` | input | 1 | Accept grant0 and update LRU |
| `bid1_i` | input | NumClients | Bid vector 1 (opportunistic channel) |
| `grant1_o` | output | NumClients | One-hot grant for bid1 |
| `pop1_i` | input | 1 | Accept grant1 and update LRU |

## Behaviour

- `grant0_o` and `grant1_o` are one-hot-zero.
- Only one of `pop0_i` or `pop1_i` may be asserted per cycle.
- The popped channel's winner moves to MRU (lowest priority) position.
- After reset, all priority bits are zero — client 0 has initial highest priority.

## Differences from original

| Aspect | Original (`arb_lru_2_bid`) | This module |
|--------|---------------------------|-------------|
| Reset | Active-high synchronous | Active-low async-assert/sync-deassert |
| Naming | `clock`/`reset`, `bid`/`grant`/`pop` | `clk_i`/`rst_ni`, `bid0_i`/`grant0_o`/`pop0_i` |
| Coding | `RST_EN_FF` macro | Explicit `always_ff` |
