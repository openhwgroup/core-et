# tech_generic

Behavioral/default implementations of the true technology primitives.

This tree is the semantic reference for each primitive:
- module name
- parameter set
- port list
- functional contract

`mk/prim.mk` selects these modules for `TECH=generic`, and any target that
does not provide its own override falls back here automatically.

The modules in this tree are intentionally pure SystemVerilog:
- portable in Verilator and default RTL cosim
- close to the original CORE-ET behavioral models
- explicit about the contract that FPGA and ASIC overrides must preserve

These are the current technology seams in the system:
- Local clock generation: `prim_clk_gate` and `prim_clk_gate_n` are used for
  real gated clocks in shirecache, rbox, and minion blocks.
- SRAM clock selection: `prim_clk_mux` is used in SRAM wrappers to switch
  between the functional SRAM clock and the DFT override clock.
- SRAM replacement: `prim_ram_1p` and `prim_ram_2p` are the only storage seam
  for shirecache, rbox, minion local RAM wrappers, and `prim_fifo_sram`.
- Latch-timed register files: `prim_rf_1r1w`, `prim_rf_1r1w_dec`,
  `prim_rf_1r1w_diff`, `prim_rf_2r1w`, and `prim_rf_3r2w` preserve the
  original half-cycle RF write protocol used by minion and shirecache blocks.
- Reset-domain handoff: `prim_rst_sync` is the project reset repeater and is
  used heavily in shirecache to derive local cold, warm, and debug resets.
- Cross-domain transport: `prim_fifo_async_hiv` and `prim_fifo_async_lov` are
  the shire/NOC GCD-style CDC seam today, and the future level-shifter insertion
  point for arbitrary asynchronous crossings.
- Semi-synchronous voltage transport: `prim_fifo_semisync_hiv` and
  `prim_fifo_semisync_lov` preserve the CORE-ET `_ss` FIFO contract used by the
  neighborhood HV/LV crossings with 1:1 phase-controlled clocks.

This list is not a promise that no other technology primitives will ever be
added. The rule is narrower: add a true tech primitive only when the module is
the correct insertion point for behavior that genuinely differs by technology.

Each primitive lives in its own module directory:

```text
tech_generic/
  prim_clk_gate/
    README.md
    rtl/prim_clk_gate.sv
  ...
```

Raw high-phase/low-phase latch leaves are intentionally not public seams in
the current tree. If a technology block still needs direct transparent-latch
behavior internally, keep that behavior inside the owning named primitive or
technology-specific block; shared RTL should depend on the named phase/write
seams below.

## Primitive Contract Rules

This README is the contract source for all technology implementations. A
`tech_<target>` implementation may change cells, inference style, replication,
or clocking internals, but it must not change the parent-visible contract.

Every implementation of a primitive must preserve:

- the exact module name, parameter names, parameter meanings, port names, port
  widths, and port directions
- the same cycle timing at every output observed by shared RTL
- the same phase ordering for preview, commit, and RF write paths
- the same read-during-write and same-address write priority rules documented
  below or in the primitive README
- the same reset behavior; if a primitive has no reset port, its internal state
  is intentionally not reset
- all DFT and RAM configuration ports, even if a target implementation ties off
  unused behavior internally

Technology implementations must be validated against this contract directly.
Passing a parent block or Verilator-only test is not enough for FPGA-specific
primitive changes. A backend override must be exercised by the smallest FPGA
test that can observe its contract, and any new primitive must also have the
standalone unit test and RTL cosim required by the project rules.

Do not silently approximate a primitive because a target tool dislikes the
generic latch structure. If a target cannot implement the contract directly,
the target implementation must use a behavior-equivalent structure behind the
same primitive boundary, and the target README must document the mapping.

## Shared Port Semantics

These names have fixed meaning across primitive families:

| Port pattern | Contract |
|--------------|----------|
| `clk_i` | Functional base clock for the primitive. The primitive defines whether state is captured on the high phase, low phase, positive edge, or negative edge. |
| `rst_ni` | Active-low reset. Unless a primitive explicitly documents a different contract, reset asynchronously asserts and synchronously releases at the integration boundary. |
| `dft_i` | DFT control surface. Functional mode is `dft_i = '0`; scan/test behavior must not affect normal-mode timing. |
| `cfg_i` | RAM timing/test configuration surface. Generic and FPGA targets may ignore fields in normal mode, but ASIC targets use them for macro configuration. |
| `preview_clk_i` | Preview-phase clock for low-phase write-preview behavior. It is a real contract input, not an ignorable alias for `rf_clk_i`. |
| `rf_clk_i` | RF storage/commit clock. It is the clock that commits an already-previewed payload into the RF storage state. |
| `wr_data_en_1p_next_i` | Next preview qualifier. It is sampled by the preview phase before it may affect the RF write-data staging point. |
| `wr_data_en_1p_i` | Already-previewed write-data qualifier. It controls whether `wr_data_i` is captured into the RF write-data staging point. |
| `wr_en_i` / `wr_en_*_i` | Write commit qualifier. It commits the previously staged write data to the addressed storage entry; it does not by itself make current-cycle `wr_data_i` visible. |

`preview_clk_i` and `rf_clk_i` may be driven by related phases of the same
system clock, but a primitive implementation must still treat them as distinct
contract signals. In particular, if `preview_clk_i` is held in a phase that
does not accept a new preview value, changes on `wr_data_en_1p_next_i` must not
update the staged write qualifier, even if `rf_clk_i` continues toggling. This
is the failure mode checked by the FPGA primitive preview test.

Preview RF primitives also expose `PreviewClkSameAsRf`. It is a technology
hint, not a functional bypass. Set it to `1` only when `preview_clk_i` and
`rf_clk_i` are the same physical clock. Set it to `0` when the preview clock
can be gated, held, or otherwise separated while the RF clock continues.

## Write Preview And Commit Contracts

The write-preview and write-commit primitives replace specific original
CORE-ET latch/gate patterns. They are not generic enabled flops.

`prim_write_preview_en` contract:

- `en_i` is sampled on the high phase of `clk_i`.
- When the following low phase is active and the sampled enable is set, `d_i`
  is captured into `q_o`.
- If the high-phase enable sample is not set, the low phase must hold the
  previous `q_o`.
- Target implementations may use edges instead of transparent latches only if
  the parent-visible preview timing remains equivalent.

`prim_write_commit_en` contract:

- `en_i` is sampled on the low phase of `clk_i`.
- When the following high phase is active and the sampled enable is set, `d_i`
  is committed into `q_o`.
- If the low-phase enable sample is not set, the high phase must hold the
  previous `q_o`.

`prim_write_commit_rst_en` has the same commit behavior as
`prim_write_commit_en`, plus its documented reset value. Reset behavior is part
of the primitive contract, not a target-specific choice.

## RF Primitive Contracts

The `prim_rf_*` family preserves the original latch-timed RF protocol. The
common write path is two-stage:

1. The write-data preview stage captures `wr_data_i` into an internal staging
   point when the relevant `wr_data_en_1p_i` qualifier is active.
2. The write commit stage writes that staged value into the addressed RF entry
   when the relevant `wr_en_i` qualifier is active.

The commit stage must never bypass directly from current `wr_data_i` unless the
generic primitive contract for that exact module already does so. In the
current RF family, writes commit the staged write data.

RF reads are asynchronous/combinational unless the primitive name includes
`_reg`, in which case `rd_en_i` controls the registered read address and
`rd_data_o` reflects the storage entry selected by that registered address.
When `rd_en_i` is low, the registered read address must hold even if
`rd_addr_i` changes.

For `_reg` RFs, read/write collision behavior is part of the contract and
comes from the original `rf_latch_1r_1w_reg` timing: after a commit edge,
`rd_data_o` is a combinational view of the storage selected by the registered
read address. Therefore:

- if `rd_en_i` and `wr_en_i` are asserted in the same commit edge with the same
  address, `rd_data_o` must show the newly committed staged write data after
  that edge
- if `rd_en_i` and `wr_en_i` are asserted in the same commit edge with different
  addresses, `rd_data_o` must show the selected read address, not the write
  address
- if `rd_en_i` is low while `wr_en_i` writes the currently held registered read
  address, `rd_data_o` must update to the newly committed staged write data
- if a wider storage path is split across multiple RF instances, each instance
  must preserve its own collision behavior so masked low/high writes update only
  the enabled slice

Full-parallel variants expose the current storage contents through their
parallel readback outputs.

Multi-write priority is part of the contract:

- `prim_rf_3r2w` gives write port A priority over write port B when both write
  the same entry in the same commit phase.
- `prim_rf_1r2w_par` gives write port A priority over write port B when both
  write the same entry in the same commit phase.
- Mixed-width primitives preserve the original sub-word write enables and
  aligned overlapping read behavior.

The `prim_rf_*_preview` composites add a preview-clocked qualifier in front of
the corresponding RF primitive. Their contract is the full composite behavior:

- `wr_data*_en_1p_next_i` is sampled by `preview_clk_i`.
- The sampled qualifier, not the raw `*_next_i` input, controls the later RF
  write-data staging point.
- The RF clock, not the preview clock, controls write-data staging and commit:
  the previewed qualifier allows `wr_data_i` to enter the staging point during
  the RF write-data capture phase, and `wr_en_i` commits that staged payload
  during the RF write phase.
- `wr_data_i` / `wr_data_*_i`, write addresses, and `wr_en_i` / `wr_en_*_i`
  keep the same meaning as the non-preview RF primitive they wrap.
- A target implementation may be self-contained and need not instantiate the
  generic sub-primitives, but it must implement the same composite timing.

This distinction is mandatory. Treating `wr_data_en_1p_next_i` as a direct
negedge RF-clock enable changes behavior when the preview phase is stalled or
held and is not a legal implementation of an RF preview composite.

## RAM Primitive Contracts

All functional SRAM-like storage goes through `prim_ram_1p` or `prim_ram_2p`.
Shared RTL must not instantiate bare unpacked memories when the storage is a
RAM/macro candidate.

`prim_ram_1p` contract:

- `req_i` qualifies the access.
- `we_i` selects write when `req_i` is set.
- reads have one-cycle registered latency.
- when `req_i` is low, `rdata_o` holds its previous value.
- `alert_o` reports ECC/macro alerts when implemented; generic targets tie it
  low.

`prim_ram_2p` contract:

- ports A and B are symmetric request/write/read ports.
- reads have one-cycle registered latency and hold when their request is low.
- simultaneous read and write of the same address returns the old value on the
  read port unless the primitive README documents a narrower target-specific
  restriction.
- same-address simultaneous writes are not made safe by the wrapper; parent RTL
  must avoid or explicitly define those cases.

## Clock And Phase Primitive Contracts

Clock primitives are functional primitives, not cosmetic synthesis hints.

- `prim_clk_gate` preserves positive-phase local clock gating. In scan mode,
  `dft_i.scanmode` forces the gate open.
- `prim_clk_gate_n` preserves the negative-phase helper-clock contract used by
  translated phase-2 logic.
- `prim_clk_mux` is the clock-select seam for DFT SRAM clock override.
- `prim_phase_pair_lo_hi` and `prim_phase_pair_hi_lo` preserve the named
  two-phase handoff where both phase outputs remain architecturally live.

Backend implementations may use dedicated FPGA or ASIC cells, but the output
phase relationship visible to shared RTL must remain the same.

## Primitives

| Module | System role |
|--------|-------------|
| `prim_clk_gate` | Positive-phase local clock gate, with scan override for gated-clock domains |
| `prim_clk_gate_n` | Negative-phase clock gate used by translated phase-2/latch-timed minion logic |
| `prim_clk_mux` | Static clock select, used primarily for DFT SRAM clock override |
| `prim_eco_ports` | Spare/ECO port wrapper: tie output spares low and preserve input spares for future ASIC buffering |
| `prim_mul_div` | Technology seam for the Minion mul/div unit, preserving the current request/response contract while allowing target-specific implementations |
| `prim_write_preview_en` | Narrow write-pipeline preview primitive: captures staged write payloads for later commit on the same base clock |
| `prim_write_commit_en` | Narrow write-pipeline commit primitive: enabled state update without its own reset contract |
| `prim_write_commit_rst_en` | Narrow write-pipeline commit primitive with defined reset value |
| `prim_phase_pair_lo_hi` | Self-contained low-phase-to-high-phase handoff seam that keeps both phase outputs live without open-coded raw latch chains in shared RTL |
| `prim_phase_pair_hi_lo` | Self-contained high-phase-to-low-phase handoff seam that keeps both phase outputs live without open-coded raw latch chains in shared RTL |
| `prim_rf_1r1w_preview` | Named composite for a low-phase write-preview seam feeding a same-width latch-timed 1R1W RF |
| `prim_rf_1r1w_dec_preview` | Named composite for a low-phase write-preview seam feeding a decoded-address latch-timed 1R1W RF |
| `prim_rf_1r1w_par_preview` | Named composite for a low-phase write-preview seam feeding a parallel-readback latch-timed 1R1W RF |
| `prim_rf_1r1w_diff_preview` | Named composite for a low-phase write-preview seam feeding a mixed-width latch-timed 1R1W RF |
| `prim_rf_1r1w_reg_preview` | Named composite for a low-phase write-preview seam feeding a same-width latch-timed 1R1W RF with registered read address |
| `prim_rf_single_1r1w_par_preview` | Named composite for a low-phase write-preview seam feeding a single-entry write-capture primitive with direct readback |
| `prim_rf_2r1w_preview` | Named composite for a low-phase write-preview seam feeding a latch-timed 2R1W RF |
| `prim_rf_3r2w_preview` | Named composite for paired low-phase write-preview seams feeding a latch-timed 3R2W RF |
| `prim_ram_1p` | Single-port SRAM wrapper for tag/data/FIFO storage |
| `prim_ram_2p` | Dual-port SRAM wrapper, used mostly as single-clock 1R1W storage |
| `prim_rf_1r1w` | Latch-timed 1-read/1-write register-file primitive |
| `prim_rf_1r1w_dec` | Latch-timed 1-read/1-write register-file primitive with decoded one-hot addresses |
| `prim_rf_1r1w_reg` | Latch-timed 1-read/1-write register-file primitive with registered read address |
| `prim_rf_1r1w_par` | Latch-timed 1-read/1-write register-file primitive with full parallel readback |
| `prim_rf_1r2w_par` | Latch-timed 1-read/2-write register-file primitive with full parallel readback |
| `prim_rf_1r1w_diff` | Mixed-width latch-timed 1-read/1-write register-file primitive |
| `prim_rf_2r1w` | Latch-timed 2-read/1-write register-file primitive |
| `prim_rf_3r2w` | Latch-timed 3-read/2-write register-file primitive |
| `prim_rf_single_1r1w_par` | Latch-timed single-entry write-capture primitive with direct readback |
| `prim_rst_sync` | Reset repeater: async assert, sync release, scan-reset bypass |
| `prim_fifo_async_hiv` | GCD-style CDC FIFO from shire/HV write side to NOC/LV read side |
| `prim_fifo_async_lov` | GCD-style CDC FIFO from NOC/LV write side to shire/HV read side |
| `prim_fifo_semisync_hiv` | Semi-synchronous FIFO from HV write side to LV read side for 1:1 phase-controlled clocks |
| `prim_fifo_semisync_lov` | Semi-synchronous FIFO from LV write side to HV read side for 1:1 phase-controlled clocks |

`mk/prim.mk` selects these by default (`TECH=generic`). DV and simulation
heads should consume the primitive variables rather than hard-coding paths, so
technology overrides and new primitive seams stay centralized in one place.
