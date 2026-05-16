# Neighborhood cooperative TLoad

This IP block translates the CORE-ET cooperative tensor-load neighborhood leaves.
The block tracks outstanding cooperative tensor-load tags, arbitrates minion miss
requests into local or remote neighborhood requests, and carries the sideband
slave-ready / master-done notifications used to return cooperative-load credits.

## Modules

| Module | Original module | Function |
|--------|-----------------|----------|
| `neigh_coop_tload_tag_table` | `neigh_coop_tload_tag_table` | Two-write-port CAM/tag table for outstanding cooperative tensor-load IDs. A CAM pop returns the stored per-minion miss IDs and invalidates matching entries. |
| `neigh_coop_tload_ports` | `neigh_coop_tload_ports` | Registering voltage-boundary wrapper for slave-ready and master-done sideband ports. |
| `neigh_coop_tload` | `neigh_coop_tload` | Main cooperative TLoad controller: decodes minion cooperative miss requests, arbitrates slave/local-master/remote-master work, updates tag-table state, emits ET-Link requests, and exposes debug state. |

## Parameters

`neigh_coop_tload_tag_table` has one parameter:

| Parameter | Default | Meaning |
|-----------|---------|---------|
| `Depth` | `8` | Number of outstanding cooperative-load CAM entries. This matches the original module default; the top controller overrides it to `neigh_pkg::ShireCoopTagTableDepth` (`32`) exactly like the original channel path. |

`neigh_coop_tload` and `neigh_coop_tload_ports` have no module parameters. Their
sizes come from package constants in `neigh_pkg`, `minion_pkg`, and
`minion_dcache_pkg`, including `MinPerN=8`, `NumNeigh=4`, `NumOtherNeigh=3`,
`ShireCoopMissReqs=2`, `ShireCoopIdSize=5`, and `ShireCoopLoadIdSize=8`.

## Port interface

All modules use `clk_i` and active-low `rst_ni`. Original packed structs from
`soc.vh` are represented by package types.

### `neigh_coop_tload_tag_table`

- `wr_req_i[1:0]`, `wr_valid_i[1:0]`, `wr_ready_o[1:0]`: two write request
  ports. Port 0 is the slave-table writer and port 1 is the master-table writer
  in the top controller.
- `cam_pop_i`, `cam_tag_i`, `cam_data_o`: CAM lookup/pop interface. The
  OR-encoded matching address is invalidated when `cam_pop_i` is asserted;
  `cam_data_o` exposes the data at that encoded address, preserving the original
  behavior if duplicate tags are present.

### `neigh_coop_tload_ports`

- High-voltage side outputs: `coop_tload_slv_rdy_out_*_o` and
  `coop_tload_mst_done_out_*_o`.
- High-voltage side inputs: `coop_tload_slv_rdy_in_*_i` and
  `coop_tload_mst_done_in_*_i`.
- Low-voltage side inputs/outputs with `_lv_` in the port names connect to the
  neighborhood channel logic. The module preserves the original single-register
  level-shift wrapper behavior for every sideband signal.

### `neigh_coop_tload`

- Minion request inputs: `min_coop_miss_req_i[minion][miss]` and valid/ready
  vectors. Ready asserts for the granted source when the corresponding slave or
  master table path has space and downstream request/sideband conditions allow
  the original request to be accepted.
- ET-Link output: `coop_tload_req_o`, `coop_tload_req_bid_o`, and
  `coop_tload_req_ready_i` form the original ready/bid request protocol.
- Sideband ports: slave-ready output/input and master-done output/input vectors
  connect to the voltage-crossing port wrapper or channel-level sideband fabric.
- Tag-table CAM: `tag_table_cam_pop_i`, `tag_table_cam_tag_i`, and
  `tag_table_cam_data_o` expose completion lookup data to the parent.
- `dbg_sm_signals_o` mirrors the original debug/status-monitor fields for the
  cooperative TLoad state machine.

## Integration notes

Source order for simulation/synthesis:

1. `hw/ip/etlink/rtl/etlink_pkg.sv`
2. `hw/ip/minion/rtl/minion_pkg.sv`
3. `hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv`
4. `hw/ip/neigh_pkg/rtl/neigh_pkg.sv`
5. `hw/ip/prim_arb_lru/rtl/prim_arb_lru.sv`
6. `hw/ip/neigh_coop_tload/rtl/neigh_coop_tload_tag_table.sv`
7. `hw/ip/neigh_coop_tload/rtl/neigh_coop_tload_ports.sv`
8. `hw/ip/neigh_coop_tload/rtl/neigh_coop_tload.sv`

The main controller maps the original `arb_lru` sites to `prim_arb_lru` while
keeping the original two-bid grant/update protocol. The ports wrapper translates
original level-shifter/combo-buffer cells into equivalent registered/shared RTL
behavior because the original cells are functional register/buffer wrappers in
this context; technology-specific voltage cells remain a parent/crossing concern.

## Design constraints and assumptions

- The translation targets the original four-neighborhood, eight-minion CORE-ET
  configuration represented by `neigh_pkg` constants.
- Original unreset data-register behavior is preserved for equivalence; only
  original reset-controlled state is reset by `rst_ni`.
- The sideband credit accounting and tag-table write/CAM/pop timing are
  cycle-equivalent to the original. The implementation is not a protocol
  redesign and does not add buffering or pipeline stages.

## Intentional differences from original CORE-ET

| Aspect | Original | This implementation | Rationale |
|--------|----------|---------------------|-----------|
| Reset | Active-high `reset` | Active-low `rst_ni` | Repository reset convention; original reset behavior is preserved with inverted polarity. |
| Names | `clock`, bare signal names, `_ff`/`_next` | `clk_i`, `_i`/`_o`, `_q`/`_d` where local | lowRISC style. |
| Types | `soc.vh` typedefs/macros | `etlink_pkg`, `minion_pkg`, `minion_dcache_pkg`, `neigh_pkg` | Package-based source order and no synthesizable macro definitions. |
| Arbiter | `arb_lru` | `prim_arb_lru` | Project primitive mapping, standalone primitive cosim covered separately. |
| Port wrapper cells | `vclevel_shft_l2h`, `vclevel_shft_h2l`, `et_combo_buf` | Equivalent registered/buffered RTL in `neigh_coop_tload_ports` | The original wrapper behavior is preserved; actual voltage cells are abstracted at crossing boundaries. |

No functional timing, packet-format, tag-table, credit-accounting, or debug
status divergence is intentional.

## Verification

Unit DV lives in `hw/ip/neigh_coop_tload/dv/` and exercises tag-table writes,
CAM hits/misses/pops, port-wrapper registration, slave-ready sideband, local
master request, remote `ReadCoop` request, master-done credit return, and debug
outputs.

Standalone RTL cosims compare each translated original module against the
CORE-ET source with all outputs checked every cycle:

- `dv/rtlcosim/neigh_coop_tload_tag_table`
- `dv/rtlcosim/neigh_coop_tload_ports`
- `dv/rtlcosim/neigh_coop_tload`
