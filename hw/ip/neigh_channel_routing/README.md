# neigh_channel_routing

Neighborhood channel routing/control leaves translated from CORE-ET.

## Modules

| Module | Purpose |
|--------|---------|
| `neigh_fl_barrier` | Arbitrates per-minion fast-local-barrier CSR requests into the L2 FLB request path and routes L2 responses back to the originating minion. |
| `neigh_local_message_network` | Fixed local-message crossbar between the eight minions, including local routing masks/destination IDs and status-monitor debug bits. |
| `neigh_pmu` | Neighborhood PMU counter block. It merges minion counter events, selects neighborhood events, handles writes, pre-counter overflow, post-counter increments, and readback. |
| `neigh_tbox_router` | Routes TBOX/minion/neighborhood message and L2 request/response traffic through the original FIFO and arbitration structure. |

## Parameters

All parameters default to the original CORE-ET neighborhood configuration:

- `NrMinions = neigh_pkg::MinPerN` (`8`)
- `MaxLocalConnect = neigh_pkg::NeighMaxLocalConnect` (`3`)
- FLB/TBOX FIFO depths from `neigh_pkg` (`4` for FLB outstanding requests, `2` for TBOX FIFOs)
- PMU counter/event widths from `minion_pkg` (`8` minion counters, `4` neighborhood counters, `12` total counters, `25` neighborhood events)

## Interfaces

All modules use `clk_i` and active-low `rst_ni` instead of original `clock` and active-high `reset`.

- `neigh_fl_barrier` keeps the original minion FLB request/response vectors and L2 request/response handshake.
- `neigh_local_message_network` keeps the original local-message valid/ready/data vectors. `local_channel_mask_o` and `local_dest_id_o` are constant routing configuration outputs.
- `neigh_pmu` keeps the original PMU minion read/write/count-up vectors, neighborhood event inputs, and `esr_pkg::esr_pmu_ctrl_t` control input.
- `neigh_tbox_router` keeps the minion TBOX request/response bus, TBOX L2 request/response bus, TBOX neighborhood request bus, and TBOX neighborhood response bus using `etlink_pkg` packed structs.

## Integration notes

Source order for simulation/synthesis:

1. `hw/ip/etlink/rtl/etlink_pkg.sv`
2. `hw/ip/minion/rtl/minion_pkg.sv`
3. `hw/ip/esr/rtl/esr_pkg.sv`
4. `hw/ip/neigh_pkg/rtl/neigh_pkg.sv`
5. `hw/ip/dft/rtl/dft_pkg.sv`
6. selected `prim_clk_gate`, `prim_fifo`, `prim_rf_1r1w_preview`, `prim_arb_lru`, and `prim_arb_rr` sources
7. these four RTL files

`neigh_fl_barrier` and `neigh_tbox_router` use `prim_fifo` for original `gen_fifo` instances. `UseLatches` and `push_early` match each original FIFO site. `neigh_fl_barrier` uses `prim_arb_lru`; `neigh_tbox_router` uses `prim_arb_rr`.

`neigh_pmu` maps the original `et_clk_gate` site to `prim_clk_gate`. The original gate tied `te` low and exposed no PMU-level DFT port, so this standalone translation ties the primitive `dft_i` to zero to preserve the original module interface and functional equivalence. A parent-level scan wrapper may still provide a broader DFT surface.

## Intentional differences from original CORE-ET

| Aspect | Original | This implementation | Rationale |
|--------|----------|---------------------|-----------|
| Reset | Active-high `reset` through CORE-ET macros | Active-low `rst_ni` async-assert style for true reset state | Repository convention |
| Names | `clock`, bare signal names, `_ff`/`_next` | `clk_i`, `_i`/`_o`, `_q`/`_d` where local | lowRISC style |
| Types | `soc.vh` typedefs/macros | `etlink_pkg`, `minion_pkg`, `esr_pkg`, `neigh_pkg` | Package-based source order |
| FIFOs/arbiters | `gen_fifo`, `arb_lru`, `arb_rr_data` | `prim_fifo`, `prim_arb_lru`, `prim_arb_rr` | Project primitive mapping, cycle-equivalent cosim checked |
| PMU clock gate | `et_clk_gate` with `te=1'b0` | `prim_clk_gate` with zeroed `dft_t` | Technology abstraction while preserving original no-DFT module interface |

No functional protocol or timing divergence is intentional.

## Preserved original behavior

`neigh_tbox_router` intentionally preserves an original stale-route behavior on the `tbox_neigh_req` path: the first request after an empty-to-nonempty FIFO transition can select SC-vs-minion valid generation from the previously exposed request opcode while the output payload reflects the current FIFO data. This is documented in `BUGS.md`; the reimplementation preserves it for cosim equivalence rather than inserting a new route register.

## Verification

Unit DV lives in `hw/ip/neigh_channel_routing/dv/` and exercises all four modules with explicit checks. Standalone RTL cosims live in:

- `dv/rtlcosim/neigh_fl_barrier`
- `dv/rtlcosim/neigh_local_message_network`
- `dv/rtlcosim/neigh_pmu`
- `dv/rtlcosim/neigh_tbox_router`
