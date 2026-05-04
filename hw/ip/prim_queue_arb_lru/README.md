# prim_queue_arb_lru

Queue-style LRU arbiter.

This is the direct reimplementation of the original `queue_arb_lru` helper
used by queue-like structures that want insertion order, not “winner becomes
MRU” grant order.

Interface summary:

| Port | Dir | Meaning |
|------|-----|---------|
| `clk_i` | input | Clock |
| `rst_ni` | input | Active-low async reset |
| `prio_up_i` | input | A new entry was inserted this cycle |
| `prio_entry_i` | input | Inserted entry index |
| `bid_i` | input | Current bidders |
| `winner_o` | output | Least-recently-inserted bidder |

Behavior:

- reset clears the priority matrix, which makes client `0` oldest
- asserting `prio_up_i` with `prio_entry_i = k` moves client `k` to MRU
- bids do not mutate priority state by themselves

This matches the original `rtl/libs/arbiters/queue_arb_lru.v` contract.
