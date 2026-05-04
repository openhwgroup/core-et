# Latch Translation Guide

This project translates latch-heavy CORE-ET RTL without pretending the latch
behavior does not exist. The rule is:

- preserve the original parent-visible timing
- keep phase-sensitive behavior explicit where the parent contract depends on it
- move FPGA-specific compromises behind the smallest semantically complete
  primitive seam

Do not open-code new raw latch chains in shared RTL. Use the existing named
primitive that matches the original pattern.

## Decision Table

| Original pattern | Preferred primitive |
|------------------|---------------------|
| Low-phase preview latch staging a 1-bit qualifier or payload bus | `prim_write_preview_en` |
| Enabled write-state commit without its own reset contract | `prim_write_commit_en` |
| Enabled write-state commit with a defined reset value | `prim_write_commit_rst_en` |
| Direct `rlatchn -> rlatch` pair with both phase outputs architecturally live | `prim_phase_pair_lo_hi` |
| Direct `rlatch -> rlatchn` pair with both phase outputs architecturally live | `prim_phase_pair_hi_lo` |
| Low-phase write-preview latch feeding `rf_latch_1r_1w` | `prim_rf_1r1w_preview` |
| Low-phase write-preview latch feeding `rf_latch_1r_1w_diff_widths` | `prim_rf_1r1w_diff_preview` |
| Low-phase write-preview latch feeding `rf_latch_1r_1w_reg` | `prim_rf_1r1w_reg_preview` |

## When To Use A Narrow Seam

Use the narrow behavior-named seams only when the translated site has really
been narrowed to that role:

- `prim_write_preview_en` when the preview value remains meaningfully separate
  from its eventual consumer
- `prim_write_commit_en` / `prim_write_commit_rst_en` when the site is just an
  enabled state update

Examples:

- a one-phase-early write-valid qualifier that feeds ordinary combinational or
  sequential logic
- a CSR write-commit latch that is not part of an RF storage protocol

## When To Use A Coarse RF Preview Seam

Use an RF preview composite when the preview latch and the RF form one
semantic contract. In that case the preview latch is not a separately useful
piece of state; it is just part of the storage protocol.

Examples:

- `prim_rf_1r1w_preview`: metadata/tag arrays and other same-width latch-timed
  RF writes
- `prim_rf_1r1w_diff_preview`: mixed-width fetch-buffer/TLB-style latch-timed
  RF writes
- `prim_rf_1r1w_reg_preview`: BRAM-oriented registered-read wrappers such as
  the Minion D-cache local RAM wrappers

This is especially important on FPGA. If the backend cannot faithfully realize
the raw split seam, fix it by raising the seam to the smallest semantically
complete pattern, not by silently redesigning the shared RTL.

## FPGA Rule

If an FPGA backend cannot safely realize the raw latch form:

1. Keep `tech_generic` as the semantic reference.
2. Keep shared RTL on the named public seam.
3. Make the FPGA technology-specific primitive self-contained.

For example, an ECP5 implementation may realize an RF preview seam as:

- internal preview capture
- internal staged write-data storage
- normal edge-triggered RF commit

while keeping the same parent-visible cycle timing as the original latch path.

## What Not To Do

- Do not replace a latch-sensitive shared-RTL path with a plain posedge FF and
  call it equivalent.
- Do not expose retired raw latch leaves as new public dependencies.
- Do not keep a too-fine seam in shared RTL when the real contract is the
  coupled preview-plus-storage pattern.
- Do not hide FPGA-specific timing compromises in shared RTL. Keep them in the
  technology tree.

## Current Examples In This Repo

- [minion_dcache_metadata_array.sv](../hw/ip/minion/dcache/rtl/minion_dcache_metadata_array.sv)
  now uses `prim_rf_1r1w_preview` because the write preview and RF write form a
  single semantic storage contract.
- [minion_dcache_128x64_1r1w_lram.sv](../hw/ip/minion/dcache/rtl/minion_dcache_128x64_1r1w_lram.sv)
  and [minion_dcache_128x72_1r1w_lram.sv](../hw/ip/minion/dcache/rtl/minion_dcache_128x72_1r1w_lram.sv)
  use `prim_rf_1r1w_reg_preview` because their true contract is preview-plus-
  registered-read RF, not a standalone preview latch.
- [test_rf_preview.sv](../fpga/test_rf_preview/rtl/test_rf_preview.sv)
  is the focused FPGA bring-up harness for the same-width and registered-read
  RF preview seams.
