# tech_ecp5

Lattice ECP5 FPGA implementations of technology primitives.

Optimized for ECP5 synthesis via Yosys `synth_ecp5`. Uses ECP5-specific RAM
inference where that does not change the primitive contract.

This tree is a target-mapping tree, not a generic Verilator behavioral model.
Functional translation signoff remains the generic `dv/rtlcosim` flow. The
ECP5 tree exists to map those same primitive contracts onto ECP5 resources and
tool constraints for synthesis/PnR.

The mapping choices follow Lattice's own guidance:
- `prim_clk_gate` maps to the documented `DCCA` dynamic clock-control
  primitive on the quadrant primary clock network.
- Clock-enable style mappings are preferred over PFU-local gated clocks where
  the primitive contract allows it; Lattice's FPGA design guide explicitly
  recommends clock enables over gated PFU clocks.

## Primitives

| Module | Description | Difference from generic |
|--------|-------------|------------------------|
| `prim_clk_gate` | Clock gating cell | Uses ECP5 `DCCA.CE`; no fabric-generated clock |
| `prim_clk_gate_n` | Negative-phase clock gating cell | Uses a DCCA-gated root clock, disabled-high shaping, and a second DCCA to export the generated local clock |
| `prim_clk_mux` | Clock multiplexer | Uses ECP5 `DCSC` with `SEL0=~sel_i`, `SEL1=sel_i`, `MODESEL=0`, `DCSMODE="POS"` when `UseTechClockMux=1`; the default passthrough mode forwards `clk0_i` for tied-off DFT override paths |
| `prim_mul_div` | Mul/div technology seam | Uses an ECP5-specific FF-based implementation that keeps the intpipe-visible contract while intentionally diverging from the generic latch-based body |
| `prim_write_preview_en` | Narrow write-pipeline preview primitive | Uses a rising-edge capture for the narrowed preview contract, without depending on the raw latch family |
| `prim_write_commit_en` | Narrow write-pipeline commit primitive | Uses a falling-edge capture for the narrowed commit contract, without depending on the raw latch family |
| `prim_write_commit_rst_en` | Narrow write-pipeline resettable commit primitive | Uses a falling-edge capture with reset for the narrowed resettable commit contract |
| `prim_phase_pair_lo_hi` | Low-phase to high-phase handoff pair | Retained only for translated sites that still keep both phase nodes architecturally live; not the preferred ECP5 path for newly narrowed write seams |
| `prim_phase_pair_hi_lo` | High-phase to low-phase handoff pair | Retained only for translated sites that still keep both phase nodes architecturally live; not the preferred ECP5 path for newly narrowed write seams |
| `prim_rf_1r1w_preview` | Same-width RF write-preview seam | Uses self-contained distributed-RAM-oriented storage while preserving the narrowed preview contract |
| `prim_rf_1r1w_dec_preview` | Decoded-address RF write-preview seam | Uses self-contained FF storage because the decoded contract allows multi-hot writes and OR-reduced multi-hot reads, which are not a legal ECP5 RAM inference shape |
| `prim_rf_1r1w_par_preview` | Parallel-readback RF write-preview seam | Uses self-contained distributed-RAM-oriented storage; the full parallel output still requires readout logic |
| `prim_rf_1r1w_diff_preview` | Mixed-width RF write-preview seam | Uses self-contained distributed-RAM-oriented storage while preserving overlapping aligned reads |
| `prim_rf_1r1w_reg_preview` | Registered-read RF write-preview seam | Uses self-contained BRAM-oriented storage for the preview-plus-registered-read RF contract |
| `prim_rf_single_1r1w_par_preview` | Single-entry write-preview seam | Uses an explicit edge-staged ECP5 realization of the preview-plus-single-entry-capture contract |
| `prim_rf_2r1w_preview` | 2R1W RF write-preview seam | Uses duplicated distributed-RAM-oriented storage, including zero and parallel-read handling |
| `prim_rf_3r2w_preview` | 3R2W RF write-preview seam | Uses one distributed-RAM-oriented replica per read port and write bank, plus a last-writer table |
| `prim_ram_1p` | Single-port RAM | `(* ram_style = "block" *)` for BRAM inference |
| `prim_ram_2p` | Dual-port RAM | `(* ram_style = "block" *)` for BRAM inference |
| `prim_rf_1r1w` | Latch-timed 1R1W register file | Uses `ram_style="distributed"` storage for LUTRAM-friendly inference |
| `prim_rf_1r1w_reg` | Latch-timed 1R1W register file with registered read address | Uses `ram_style="block"` storage to bias `DP16KD` inference while keeping the generic/original timing contract |
| `prim_rf_1r1w_dec` | Decoded-address latch-timed 1R1W register file | Uses FF storage because the decoded contract allows multi-hot writes and OR-reduced multi-hot reads, which are not a legal ECP5 RAM inference shape |
| `prim_rf_1r1w_par` | Latch-timed 1R1W register file with parallel readback | Uses distributed-RAM-oriented storage; the full parallel output still requires readout logic |
| `prim_rf_1r2w_par` | Latch-timed 1R2W register file with parallel readback | Uses one distributed-RAM-oriented bank per write port plus a last-writer table, preserving write-A priority over write-B |
| `prim_rf_1r1w_diff` | Mixed-width latch-timed 1R1W register file | Uses distributed-RAM-oriented storage while preserving overlapping aligned reads |
| `prim_rf_2r1w` | Latch-timed 2R1W register file | Uses duplicated `ram_style="distributed"` storage plus a small shadow bank for `Parallel` taps |
| `prim_rf_3r2w` | Latch-timed 3R2W register file | Uses one distributed-RAM-oriented replica per read port and write bank, plus a last-writer table |
| `prim_rf_single_1r1w_par` | Latch-timed single-entry write-capture primitive | Uses a direct ECP5 FF implementation because a one-entry RF is not a useful RAM inference target |
| `prim_rst_sync` | Reset synchronizer | Same functional behavior, explicit ECP5 implementation |
| `prim_fifo_async_hiv` | Async CDC FIFO (HV->LV) | Same functional behavior, explicit ECP5 implementation |
| `prim_fifo_async_lov` | Async CDC FIFO (LV->HV) | Same functional behavior, explicit ECP5 implementation |
| `prim_fifo_semisync_hiv` | Semi-synchronous FIFO (HV->LV) for 1:1 phase-controlled clocks | Same functional behavior, explicit ECP5 implementation |
| `prim_fifo_semisync_lov` | Semi-synchronous FIFO (LV->HV) for 1:1 phase-controlled clocks | Same functional behavior, explicit ECP5 implementation |

## ECP5 Bring-Up Notes

The current ECP5 tree is intentionally mixed. It does not apply a blanket
"collapse every latch into an FF" rule anymore.

- Real functional/local clocks remain real clocks. `prim_clk_gate` uses
  `DCCA.CE` directly and does not generate a fabric-gated clock.
  `prim_clk_gate_n` preserves the negative-phase disabled-high contract by
  gating the root clock with `DCCA.CE`, forming the disabled-high waveform, and
  exporting that waveform through a second DCCA.
- `prim_clk_mux` defaults to the passthrough mode on ECP5 because `DCSC` blocks
  are scarce and most FPGA SRAM/DFT override paths keep `sel_i=0`. Bring-up
  projects that intentionally test live mux selection must instantiate it with
  `UseTechClockMux=1`.
- Board/project heads that instantiate these primitives should allow nextpnr to
  promote the PLL/root clock normally. Do not add `--no-promote-globals` as a
  blanket workaround in full-clock ECP5 heads: the root clock and explicit
  `DCCA` outputs are different global-clock demands, and blocking root
  promotion can create a worse routing problem than the original crash.
- The explicit phase-pair seams still exist for the rare translated sites that
  genuinely keep both phase nodes architecturally live:
  `prim_phase_pair_lo_hi` and `prim_phase_pair_hi_lo`. They are retained as
  exceptional seams, not as the preferred ECP5 shape for new translations.
- The behavior-named write seams are also edge-based by design on ECP5:
  `prim_write_preview_en` uses rising-edge capture, while
  `prim_write_commit_en` and `prim_write_commit_rst_en` use falling-edge
  capture.
- The RF preview composites are the preferred ECP5-safe place to absorb the
  full preview-plus-storage contract when a raw split seam is too fine:
  `prim_rf_1r1w_preview`, `prim_rf_1r1w_dec_preview`,
  `prim_rf_1r1w_par_preview`, `prim_rf_1r1w_diff_preview`,
  `prim_rf_1r1w_reg_preview`, `prim_rf_single_1r1w_par_preview`,
  `prim_rf_2r1w_preview`, and `prim_rf_3r2w_preview` are self-contained. They
  use `PreviewClkSameAsRf` to select the FPGA-safe edge implementation for the
  clock relationship at that site. For same-clock uses, the ECP5
  implementation stages payload on the RF falling edge using the raw
  `*_next_i` qualifier. For split or gated-preview uses, it stages payload on
  the preview-clock falling edge, so a held or gated-off preview clock cannot
  admit a new payload while `rf_clk_i` continues toggling. The staged payload is
  then committed on the rising edge of `rf_clk_i`. Treating `preview_clk_i` as
  an ignorable alias, using the clock level as a same-edge mux select, or
  sampling current-cycle `wr_data_i` directly in the commit stage violates the
  translated RF contract.
- Raw high-phase/low-phase latch leaves are retired as public seams. Any
  remaining direct transparent behavior now lives inside the owning named
  primitive or technology-specific block.
- RF/storage primitives now have ECP5-specific implementations instead of
  silently falling back to generic storage. RAM-like single-read or replicated
  multi-read arrays are tagged for distributed RAM, and registered-read 1R1W
  arrays are tagged for BRAM. Whole-array parallel outputs still require
  readout logic. Decoded-address RFs and single-entry captures remain FFs
  because their contracts are not useful RAM inference targets.
- `prim_rf_1r1w_diff` and `prim_rf_1r1w_diff_preview` deliberately reject
  unvalidated parameter shapes in the ECP5 tree. The current supported shapes
  are the default same-width shape
  `RWidth=32, RAlignment=32, WWidth=32, Entries=32`, the small same-width test
  shape `RWidth=8, RAlignment=8, WWidth=8, Entries=2`, and the
  `test_rf_preview` mixed-width shape
  `RWidth=8, RAlignment=8, WWidth=16, Entries=2`, plus the frontend fetch
  buffer shape `RWidth=32, RAlignment=16, WWidth=256, Entries=2`. The mixed-width
  implementation overlays active write lanes onto the asynchronous read image
  so same-cycle read/write overlap sees the just-committed staged data instead
  of relying on backend-specific LUTRAM collision behavior. Any new shape must
  first get a primitive-level FPGA test that proves the read/write collision and
  overlapping-read contract before the ECP5 allowlist is extended.
- The registered-read BRAM-oriented RF mapping must preserve the original
  `_reg` RF collision contract: same-edge same-address read/write returns the
  newly committed staged data, different-address read/write returns the selected
  read address, and writes to a held registered read address become visible even
  when `rd_en_i` is low. The ULX3S `test_rf_preview` project checks these cases
  directly in its `B0`-`B4` UART lines.

## Known Backend Bugs And Workarounds

The current ECP5 tree still carries targeted workarounds for open-source
backend limitations, but they are selective rather than blanket latch removal.

- Transparent-latch-heavy feedback paths remain structurally difficult for FPGA
  implementation and timing analysis, especially in the current open-source
  ECP5 flow.
- The resettable high-phase latch primitives therefore remain on falling-edge
  FF mappings.
- High-risk blocks that are structurally unfriendly to the ECP5 flow should be
  isolated behind behavior-named seams such as `prim_mul_div` or the
  `prim_write_*` family, rather than by redefining the whole latch family.

Generic/ASIC semantics are still localized correctly:

- `tech_generic` keeps the original gate + latch composition explicit
- shared RTL still names the original phase-sensitive behaviors
- ECP5-specific divergences are confined to the primitives whose contracts have
  already been narrowed or explicitly documented

## Usage

Set `TECH := ecp5` in the project head Makefile before including `mk/prim.mk`.
