# neigh_pkg

Neighborhood-shell package for the standalone Minion path.

This package collects the small shell-local constants and typedefs that are
shared by the translated neighborhood response path:

- response-agent indexing (`minions + icache + PTWs + tbox`)
- cooperative tensor-load controller sizing, sideband, tag-table, and debug payloads
- fill FIFO entry payloads
- neighborhood channel leaf constants for FLB, local-message, TBOX FIFOs, and tensor-store buffering

The current contents are:

| Item | Meaning |
|------|---------|
| `MinPerN` | Minions per neighborhood (`8`) |
| `IcachePerN` | I-cache clients per neighborhood (`1`) |
| `PtwPerN` | PTW clients per neighborhood (`2`) |
| `NeighNumRespAgents` | Total response agents (`12`) |
| `NeighAgentMinion/Icache/Ptw/Tbox` | Packed response-agent index layout |
| `NeighFillFifoSize` | Standalone fill FIFO depth (`4`) |
| `NeighFlbElemsPerNeigh` | Fast-local-barrier outstanding request FIFO depth (`4`) |
| `NeighMaxLocalConnect` | Maximum local-message fanout per minion (`3`) |
| `NeighMinTboxReqFifoSize` / `TboxNeighReqFifoSize` / `NeighScTboxRspFifoSize` | TBOX router FIFO depths (`2`) |
| `TstoreReqFifoDepth` | Tensor-store north/south merge FIFO depth (`2`) |
| `NumNeigh` / `NumOtherNeigh` / `NumNeighWidth` | Neighborhood-count constants for cooperative sideband ports (`4`, `3`, `2`) |
| `ShireCoopMissReqs` / `ShireCoopNumIds` / `ShireCoopRdyCreds` / `ShireCoopTagTableDepth` | Cooperative TLoad controller sizing and credit constants (`2`, `32`, `3`, `32`) |
| `ShireCoop*Start/Size` constants | Bit layout of the cooperative TLoad minion request payload and load tag |
| `coop_tload_slv_req_t` / `coop_tload_slv_rdy_req_t` | Cooperative TLoad slave path and slave-ready sideband payloads |
| `coop_tload_tag_table_data_t` / `coop_tload_tag_table_req_t` | Cooperative tensor-load CAM payload and write request |
| `coop_tload_dbg_sm_t` | Cooperative TLoad debug-monitor payload exported to the status monitor |
| `fill_fifo_data_t` | Stored fill FIFO entry payload |
| `ptw_state_e` | Shared PTW controller FSM states |
| `ptw_dbg_sm_t` | PTW debug-monitor payload exported to the status monitor |
| `fln_dbg_sm_t` | Fast local-message-network debug-monitor payload |
| `tstore_block_dbg_sm_t` / `tstore_dbg_sm_t` | Tensor-store block and north/south debug-monitor payloads |

This package replaces the neighborhood-local typedefs that originally lived in
`shire_types.vh`.
