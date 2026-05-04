# neigh_fill_fifo

Standalone neighborhood fill FIFO.

This module stores incoming neighborhood ET-Link responses and delivers them to
the appropriate response agents:

- minion fill FIFOs
- icache
- PTW instances
- tbox

It also preserves the original cooperative tensor-load behavior:

- cooperative fills may target multiple minions
- partially ready cooperative fills update the stored minion mask in place
- cooperative minion IDs are pre-expanded from the cooperative tag-table data

Key dependencies:

| Dependency | Role |
|------------|------|
| `neigh_pkg` | Neighborhood response-agent constants and fill/cooperative types |
| `prim_rf_1r2w_par` | Stored fill entries with one parallel read port and two write ports |
| `prim_queue_arb_lru` | Oldest-entry selection based on insertion order |

Ports:

| Port | Dir | Type | Meaning |
|------|-----|------|---------|
| `clk_i` | input | `logic` | Clock |
| `rst_ni` | input | `logic` | Active-low async reset |
| `push_i` | input | `logic` | Push a new fill response |
| `push_data_i` | input | `etlink_pkg::rsp_t` | Incoming full-width ET-Link response |
| `coop_data_i` | input | `neigh_pkg::coop_tload_tag_table_data_t` | Cooperative tag-table payload used to expand cooperative minion IDs |
| `full_o` | output | `logic` | FIFO full |
| `out_rsp_ready_i` | input | `logic[NeighNumRespAgents-1:0]` | Per-agent ready |
| `out_rsp_valid_o` | output | `logic[NeighNumRespAgents-1:0]` | Per-agent valid |
| `out_rsp_info_o` | output | `etlink_pkg::rsp_t[NeighNumRespAgents-1:0]` | Per-agent response payloads |

Differences from the original:

| Original | New |
|----------|-----|
| Active-high reset `reset` | Active-low async reset `rst_ni` |
| Global typedefs from `soc.vh` | `etlink_pkg`, `minion_pkg`, `minion_dcache_pkg`, `neigh_pkg` |
| `rf_latch_1r_2w_par` / `queue_arb_lru` | `prim_rf_1r2w_par` / `prim_queue_arb_lru` |
