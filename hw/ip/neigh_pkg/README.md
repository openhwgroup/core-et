# neigh_pkg

Neighborhood-shell package for the standalone Minion path.

This package collects the small shell-local constants and typedefs that are
shared by the translated neighborhood response path:

- response-agent indexing (`minions + icache + PTWs + tbox`)
- cooperative tensor-load tag-table payloads
- fill FIFO entry payloads

The current contents are:

| Item | Meaning |
|------|---------|
| `MinPerN` | Minions per neighborhood (`8`) |
| `IcachePerN` | I-cache clients per neighborhood (`1`) |
| `PtwPerN` | PTW clients per neighborhood (`2`) |
| `NeighNumRespAgents` | Total response agents (`12`) |
| `NeighAgentMinion/Icache/Ptw/Tbox` | Packed response-agent index layout |
| `NeighFillFifoSize` | Standalone fill FIFO depth (`4`) |
| `coop_tload_tag_table_data_t` | Cooperative tensor-load CAM payload |
| `fill_fifo_data_t` | Stored fill FIFO entry payload |
| `ptw_state_e` | Shared PTW controller FSM states |
| `ptw_dbg_sm_t` | PTW debug-monitor payload exported to the status monitor |

This package replaces the neighborhood-local typedefs that originally lived in
`shire_types.vh`.
