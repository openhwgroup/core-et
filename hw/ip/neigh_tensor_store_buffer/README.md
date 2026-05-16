# Neighborhood tensor-store buffer

This IP translates the CORE-ET cooperative tensor-store buffer leaves. The block
collects write-around evict requests from minions, waits for matching 2-way or
4-way cooperative partners, merges their data into one or two neighborhood
write-around requests, tracks outstanding acknowledgements, and generates
synthetic ACK responses for non-master slave minions.

## Modules

| Module | Original module | Function |
|--------|-----------------|----------|
| `neigh_tensor_store_buffer_block` | `neigh_tensor_store_buffer_block` | Four-minion tensor-store block. Performs cooperative-mask qualification, round-robin selection of master minions, first/second-pass output generation, per-minion outstanding write-around counters, and slave ACK generation. |
| `neigh_tensor_store_buffer` | `neigh_tensor_store_buffer` | Top-level eight-minion wrapper. Instantiates north/south four-minion blocks, arbitrates their output requests, and inserts the original two-entry request FIFO. |

## Parameters

`neigh_tensor_store_buffer_block` parameters mirror the original behavior:

| Parameter | Default | Meaning |
|-----------|---------|---------|
| `NumMinions` | `4` | Number of minions handled by one block. The translated and cosim-verified configuration is the original four-minion block. |
| `SourceOffset` | `0` | Offset added to the ET-Link source field. The top uses `0` for the north half and `MinPerN/2` for the south half. |

`neigh_tensor_store_buffer` has no module parameters; it uses
`neigh_pkg::MinPerN` (`8`) and `neigh_pkg::TstoreReqFifoDepth` (`2`).

## Port interface

All modules use `clk_i` and active-low `rst_ni` instead of original `clock` and
active-high `reset`.

### `neigh_tensor_store_buffer_block`

- `evict_req_i[NumMinions]`, `evict_valid_i`, `evict_ready_o`: minion evict
  input handshake. Inputs are `etlink_pkg::neigh_req_t`; only the original
  tensor-store fields are interpreted (`subopcode[0]`, `size`, `qwen`, `data`,
  `address`, `wdata`).
- `evict_out_ready_i`, `evict_out_disable_next_i`, `evict_out_valid_o`,
  `evict_out_valid_1st_o`, `evict_out_valid_2nd_o`, `evict_out_req_o`: merged
  neighborhood write-around request output. Line-size cooperative transfers use
  the same first/second-pass sequencing as the original RTL.
- `tstore_min_ack_i`: ACK return pulses from minions for issued tensor-store
  requests.
- `tstore_min_resp_ready_i`, `tstore_min_resp_valid_o`, `tstore_min_resp_o`:
  synthetic ACK response path to slave minions. Response payloads use
  `minion_pkg::et_link_minion_rsp_info_t`.
- `dbg_sm_signals_o`: packed `neigh_pkg::tstore_block_dbg_sm_t` status monitor
  payload.

### `neigh_tensor_store_buffer`

The top-level wrapper has the same eight-minion request, ACK, slave-response,
and output request interfaces, without exposing block-local first/second-pass
signals. It arbitrates north/south output requests and queues them through a
two-entry `prim_fifo` matching the original `gen_fifo` site.

## Integration notes

Source order for simulation/synthesis:

1. `hw/ip/etlink/rtl/etlink_pkg.sv`
2. `hw/ip/minion/rtl/minion_pkg.sv`
3. `hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv`
4. `hw/ip/neigh_pkg/rtl/neigh_pkg.sv`
5. selected `prim_arb_rr`, `prim_fifo`, and `prim_rf_1r1w_preview` sources
6. `hw/ip/neigh_tensor_store_buffer/rtl/neigh_tensor_store_buffer_block.sv`
7. `hw/ip/neigh_tensor_store_buffer/rtl/neigh_tensor_store_buffer.sv`

Primitive mappings:

| Original | Translation | Notes |
|----------|-------------|-------|
| `arb_rr` | `prim_arb_rr` with `Width=1` and unused data output | Grant behavior and pointer update match the original arbiter. |
| `gen_fifo` (`NUM_ELEMS=2`, `USE_LATCHES=0`, `push_early=1'b0`) | `prim_fifo` (`Depth=2`, `UseLatches=0`) | Preserves registered `valid`/`full` and show-ahead pop-data timing. |
| `` `RST_FF`` / `` `RST_EN_FF`` true reset state | Explicit `always_ff` with active-low reset | Repository reset convention. |
| `` `EN_FF`` unreset state | Explicit enabled `always_ff` without reset | Preserves original unreset payload/metadata registers. |

## Design constraints and assumptions

- The block preserves the original cooperative-mask rules: 2-way stores group
  minion pairs and 4-way stores group minion quads; only the group master
  arbitrates.
- QWord 2-way stores merge two 128-bit lanes into one HLine request. HLine 2-way
  and QWord 4-way stores produce two-pass Line requests exactly as in the
  original.
- Per-minion outstanding write-around and pending slave-ACK counters preserve the
  original widths and update ordering.
- No RAMs, DFT ports, clock gates, or new feature switches are introduced; the
  original modules did not expose those surfaces.

## Intentional differences from original CORE-ET

| Aspect | Original | This implementation | Rationale |
|--------|----------|---------------------|-----------|
| Reset | Active-high `reset` | Active-low `rst_ni` | Repository convention; reset behavior is otherwise preserved. |
| Names | `clock`, bare signal names, `_ff`/`_next` | `clk_i`, `_i`/`_o`, `_q`/`_d` where local | lowRISC style. |
| Types/constants | `soc.vh` typedefs/macros and local `` `define`` cooperative-mask constants | `etlink_pkg`, `minion_pkg`, `minion_dcache_pkg`, `neigh_pkg`, and localparams | Package-based source order and no synthesizable macro constants. |
| Arbiter/FIFO | `arb_rr` and `gen_fifo` | `prim_arb_rr` and `prim_fifo` | Project primitive mapping with standalone primitive cosim coverage. |

No functional timing, handshake, cooperative merge, ACK-accounting, source-offset,
or debug-status divergence is intentional.

## Verification

Unit DV lives in `hw/ip/neigh_tensor_store_buffer/dv/` and exercises block-level
2-way/4-way cooperative stores, first/second-pass output behavior, slave ACK
responses, top-level north/south arbitration, FIFO backpressure, and output
valid disable.

Standalone RTL cosims compare each translated module against the CORE-ET source
with all outputs checked every cycle:

- `dv/rtlcosim/neigh_tensor_store_buffer_block`
- `dv/rtlcosim/neigh_tensor_store_buffer`
