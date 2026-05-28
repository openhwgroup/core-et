// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

# neigh_ch_dbg

`neigh_ch_dbg` is the translated CORE-ET neighborhood channel debug leaf from
`rtl/shire/neigh/neigh_ch_dbg.v`. It collects neighborhood debug run-control
requests, drives TBOX and minion debug request outputs, reports ESR debug status
reductions, generates warm-reset requests, and muxes status-monitor payloads from
minions, TBOX, or the neighborhood-channel debug source.

The IP block contains:

- `neigh_ch_dbg_pkg` — translated TBOX run-control struct and resume-FSM enum.
- `neigh_ch_dbg` — synthesizable RTL for run control, HASTATUS/AND-OR-tree
  generation, warm resets, and status-monitor selection.

## Parameters

The defaults match the audited original neighborhood configuration.

| Parameter | Default | Meaning |
|-----------|---------|---------|
| `NumMinions` | `neigh_voltage_cross_pkg::MinPerN` (`8`) | Number of minions in the neighborhood. |
| `NumThreads` | `minion_pkg::NrThreads` (`2`) | Threads per minion. |
| `ThreadCount` | `NumMinions * NumThreads` (`16`) | Flat hart status/control width. |
| `SmTargetSelWidth` | `neigh_voltage_cross_pkg::NeighSmTargetSel` (`4`) | Status-monitor target select bits in `neigh_sm_gpio_i`. |
| `SmGpioWidth` | `neigh_voltage_cross_pkg::NeighSmGpioWidth` (`11`) | Status-monitor GPIO width (`target` plus mux select). |
| `SmMuxWidth` | `minion_pkg::NeighDebugSmMuxWidth` (`7`) | Status-monitor mux select width. |

The RTL includes a Verilator-time geometry assertion for the original 8-minion,
2-thread neighborhood because several preserved original expressions are
hard-wired to the 16-hart layout.

## Ports and protocol

| Port group | Direction | Meaning |
|------------|-----------|---------|
| `clk_i`, `rst_w_ni`, `rst_d_ni` | inputs | Base clock plus separate warm-reset and debug-reset domains translated from original `reset_w` and `reset_d`. |
| `reset_w_per_minion_o`, `reset_w_neigh_o` | outputs | Warm reset requests. Per-minion reset is asserted by warm reset, BPAM GPIO hart reset, or selected DM `hartreset`; neighborhood reset follows warm reset only. |
| `bpam_run_control_i` | input | BPAM GPIO/event run-control bundle from `neigh_voltage_cross_pkg`. |
| `tbox_bpam_run_control_o`, `tbox_bpam_run_control_ack_i` | output/input | Latched TBOX halt/resume/single-step request and TBOX halt/resume acknowledgements. Halt and resume requests stay latched until the corresponding registered ack clears them. |
| `debug_minion_req_o`, `debug_minion_status_i` | output/input | Per-minion halt/resume/resethalt/ackhavereset requests and minion halted/running/have-reset/busy/exception/error status. |
| `dmctrl_i`, `esr_hactrl_i` | inputs | Debug-module control and selected-hart masks. `hasel` selects `hawindow`; `hartmask` is always ORed into the selected set, matching the original. |
| `esr_hastatus0_o`, `esr_hastatus1_min_o`, `esr_and_or_tree_l0_o` | outputs | ESR debug status and one-cycle registered selected-hart reductions. |
| Status monitor ports | mixed | `neigh_sm_gpio_i[10:7]` selects minion 0-7, TBOX target 8, or neighborhood-channel target 9+; `[6:0]` is forwarded as mux select. Monitor inputs are captured only while `status_monitor_enable_i` is high. |

## Integration notes

List sources in this order:

1. `hw/ip/etlink/rtl/etlink_pkg.sv`
2. `hw/ip/icache_pkg/rtl/icache_geom_pkg.sv`
3. `hw/ip/shirecache/rtl/shirecache_pkg.sv`
4. `hw/ip/minion/rtl/minion_pkg.sv`
5. `hw/ip/esr/rtl/esr_pkg.sv`
6. `hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_pkg.sv`
7. `hw/ip/neigh_voltage_cross/rtl/neigh_voltage_cross_pkg.sv`
8. `hw/ip/neigh_ch_dbg/rtl/neigh_ch_dbg_pkg.sv`
9. `hw/ip/neigh_ch_dbg/rtl/neigh_ch_dbg.sv`

`bpam_run_control_i.gpio.ndmreset`, `dmctrl_i.ndmreset`,
`dmctrl_i.clresethaltreq`, and `dmctrl_i.setresethaltreq` are carried for
type/interface equivalence but are ignored by the original leaf and by this
translation.

## Differences from original CORE-ET

| Difference | Rationale |
|------------|-----------|
| `clock`, `reset_w`, and `reset_d` become `clk_i`, `rst_w_ni`, and `rst_d_ni`. | Repository naming/reset convention; the two original reset domains remain separate. |
| Original `soc.vh` typedefs are imported from translated packages. | Repository package/type convention while preserving packed field order and widths. |
| Original flip-flop macros become explicit `always_ff` blocks. | Repository RTL style requirement. |
| `hartReset_req` is named `hart_reset_req` in the translated BPAM GPIO struct. | lowRISC naming convention; cosim adapts the original field name. |
| Random cosim reset pulses are limited to quiescent directed phases. | Avoids a reset-style artifact from async-assert translated resets versus original synchronous reset macros while still exercising both reset inputs high and low. |

No intentional functional divergence from the original `neigh_ch_dbg` is
introduced. The reimplementation intentionally preserves the original nonzero-
minion resume-status indexing behavior documented in `BUGS.md`.

## Verification

- Unit DV: `make -C hw/ip/neigh_ch_dbg/dv clean test` — 50 explicit checks for
  resets, DM/BPAM minion requests, preserved resume-status behavior, TBOX run
  control, ESR reductions, and status-monitor muxing.
- RTL cosim: `ORIG_ROOT=/path/to/et-soc make -C dv/rtlcosim/neigh_ch_dbg clean test`
  — 132,288 output comparisons against the original `neigh_ch_dbg`, zero
  mismatches in the local run.

The cosim compares every output every cycle. Directed and randomized phases drive
every scalar/vector control input and every status-monitor payload bit low and
high/non-zero at least once.
