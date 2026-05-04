# neigh_fill_ff

Per-minion fill FIFO/front-end for the standalone neighborhood shell.

This block accepts fill responses from the neighborhood fill FIFO, cooperative
tensor-store responses, and local/TBOX message responses, then sequences them
onto the Minion L2 response port in 256-bit chunks.

## What It Does

- arbitrates four fill/message sources:
  - neighborhood fill FIFO
  - cooperative tensor-store response
  - local-message network
  - TBOX response
- stores accepted responses in 2-entry info/data FIFOs
- replays 512-bit line fills to Minion in `CoreL2BlockExtPasses` chunks
- generates a registered `tstore_min_ack_o` pulse when a tensor-store
  cooperative ACK is delivered

## Ports

| Port | Dir | Description |
|------|-----|-------------|
| `fill_fifo_resp_i` / `fill_fifo_valid_i` / `fill_fifo_ready_o` | in/out | Fill response stream from `neigh_fill_fifo` |
| `tstore_min_resp_i` / `tstore_min_resp_valid_i` / `tstore_min_resp_ready_o` | in/out | Cooperative tensor-store response |
| `local_message_info_i` / `local_message_valid_i` / `local_message_ready_o` | in/out | Local-message network response |
| `tbox_min_rsp_info_i` / `tbox_min_rsp_valid_i` / `tbox_min_rsp_ready_o` | in/out | TBOX message response |
| `l2_resp_o` / `l2_resp_valid_o` / `l2_resp_ready_i` | out/in | Sequenced Minion response output |
| `tstore_min_ack_o` | out | Registered ACK indication for tensor-store cooperative traffic |
| `dbg_sm_signals_o` | out | Minimal status-monitor signals preserved from the original |

## Differences from CORE-ET `neigh_fill_ff`

| Aspect | Original | New |
|--------|----------|-----|
| Reset | active-high `reset` | active-low `rst_ni` |
| FIFO primitive | inline `gen_fifo` | `prim_fifo` |
| Package/includes | `soc.vh` macros/types | `minion_pkg`, `minion_dcache_pkg`, `etlink_pkg` |

Cycle-by-cycle arbitration, reservation, and chunk sequencing are preserved.

## Tests

```bash
make -C hw/ip/neigh_fill_ff/dv test
ORIG_ROOT=/path/to/etcore-soc make -C dv/rtlcosim/neigh_fill_ff test
```
