# CORE-ET Translation Guide

This document defines how original CORE-ET RTL is translated into ETASP. It
exists to keep translation choices consistent across blocks and to
make FPGA-specific bring-up compromises explicit instead of implicit.

This is a translation guide, not a license to redesign blocks. The default rule
remains:

- preserve the original behavior cycle-by-cycle
- preserve phase/latch semantics where they matter
- express the behavior with named primitives and explicit SystemVerilog
- confine technology-specific approximations to the technology trees

## Translation Contract

The translation should preserve:

- interface shape and protocol timing
- latch/phase behavior used by the original design
- reset-domain separation
- DFT- and SRAM-related control surfaces
- the original helper/control structure unless a named primitive exists for it

The translation may intentionally change only the repo-wide surface
conventions:

- active-high reset naming -> active-low `_ni` naming
- individual `dft__*` ports -> `dft_pkg::dft_t`
- library/foundry cell names -> project primitives
- macro-heavy coding style -> explicit SystemVerilog

Any other intentional functional divergence must be documented in:

- the affected IP `README.md`
- the relevant `BUGS.md`, if it is fixing or documenting an original bug

## Core Naming Translation

| Original | ETASP form |
|----------|--------------|
| `clock` | `clk_i` |
| block local clock | name preserved where meaningful, usually `clock_*` in translated shared RTL |
| `reset` | `rst_ni` |
| `reset_c`, `reset_w`, `reset_d` | `rst_c_ni`, `rst_w_ni`, `rst_d_ni` |
| `*_ff` | `*_q` |
| `*_nxt` | `*_d` |
| input/output without suffix | `_i` / `_o` suffixes |

The reset rename is a convention change only. Separate original reset domains
must remain separate reset domains in the translation.

## Macro Translation

| Original | ETASP form |
|----------|--------------|
| `` `RST_FF `` | `always_ff @(posedge clk_i or negedge rst_ni)` |
| `` `EN_FF `` | `always_ff @(posedge clk_i)` with explicit enable |
| `` `RST_EN_FF `` | explicit `always_ff` with async-low reset and enable |
| `` `ZX(W, sig) `` | explicit sizing/slicing/zero-padding, not parameterized cast |
| `` `SX(W, sig) `` | explicit sign extension or safe signed expression |
| `CSR_WRITE_LATCH_*` macro family | `prim_write_commit_en` / `prim_write_commit_rst_en` when the site has been narrowed to a write-pipeline seam |
| `` `include "soc.vh" `` | package imports |

Do not mechanically use parameterized casts like `W'(expr)` in translated RTL.
The repo synthesis flow is Yosys with the slang frontend, and those casts are
fragile in constant-sensitive synthesis contexts.

## Primitive Translation

These are the standard CORE-ET-to-ETASP primitive mappings.

| Original | ETASP form | Notes |
|----------|--------------|-------|
| `gen_mem1p` | `prim_ram_1p` | all functional memories go through RAM wrappers |
| `gen_mem2p` | `prim_ram_2p` | same policy as above |
| `rbox_fifo` | `prim_fifo_sram` | preserve original handshake/latency |
| `gen_fifo_reg` | `prim_fifo_reg` | register FIFO |
| `rst_repeat` | `prim_rst_sync` | DFT-aware reset seam |
| `et_clk_gate` | `prim_clk_gate` | real local clock gate |
| `et_clk_gate_n` | `prim_clk_gate_n` | negative-phase helper gate |
| `et_clk_mux2` | `prim_clk_mux` | DFT SRAM clock select seam |
| `rf_latch_1r_1w` | `prim_rf_1r1w` | latch-timed RF primitive |
| `rf_latch_1r_1w_reg` | `prim_rf_1r1w_reg` | registered-address variant |
| `rf_latch_1r_1w_par` | `prim_rf_1r1w_par` | parallel-readback variant |
| `rf_latch_single_1r_1w_par` | `prim_rf_single_1r1w_par` | single-entry write-capture primitive |
| `rf_latch_1r_1w_diff_widths` | `prim_rf_1r1w_diff` | mixed-width RF |
| `rf_latch_2r_1w` | `prim_rf_2r1w` | 2R1W RF |
| `r32cmp` | `prim_cmp_32` | compressor primitive |
| `r42cmp` | `prim_cmp_42` | compressor primitive |

DFT translation preserves existing original DFT surfaces; it does not create
new module-level DFT behavior at unrelated sites. If the original module has
`dft__*` ports, SRAM clock override ports, or an `et_clk_mux2`, translate that
surface to `dft_pkg::dft_t` and the appropriate primitive. If the original path
has only a functional `clock`, keep a single functional clock in the translated
module unless the added wrapper boundary is explicitly documented as a
technology/tapeout hook.

## Phase/Latch Translation

Some original CORE-ET patterns are not just “state with an enable”. They
encode a specific latch or half-cycle capture contract and must stay explicit.

### Latch-Heavy RTL Policy

The repository no longer treats raw high-phase/low-phase latch leaves as
public translation seams.

The current rule is:

- shared RTL should depend on named phase/write seams, not on standalone raw
  latch leaves
- if a translated site is only an internal latch step inside a larger
  primitive or tech-specific block, keep that transparency local to the owner
- if the site is a reusable contract at the translated block boundary, give it
  a named seam and keep that seam self-contained
- technology-specific approximations belong behind those named seams only
- one public primitive should not remain layered on top of deleted raw latch
  leaves just to spell out its internals

### Raw latch macros

| Original macro/site | Translation rule |
|---------------------|------------------|
| `rlatch` | Preserve the high-phase transparent behavior inside the owning named primitive or tech-specific implementation unless the site has one of the named seams below |
| resettable high-phase latch macro | Preserve the resettable high-phase behavior inside the owning named primitive or tech-specific implementation unless the site has one of the named seams below |
| `` `LATCH_P2(CLK, Q, D) `` | Preserve the low-phase transparent behavior inside the owning named primitive or tech-specific implementation unless the site has one of the named seams below |

### Named capture/phase seams

When the original pattern is a gate feeding a latch, translate it to the
public semantic seam that matches the preserved behavior rather than exposing a
raw helper-clock latch primitive in shared RTL.

| Original pattern | ETASP form |
|------------------|--------------|
| `et_clk_gate` + `rlatch` narrowed to a write-state commit | `prim_write_commit_en` |
| `et_clk_gate` + resettable `rlatch` narrowed to a write-state commit | `prim_write_commit_rst_en` |
| `CSR_WRITE_LATCH_*` expansion in the active narrowed CSR path | `prim_write_preview_en` + `prim_write_commit_en` / `prim_write_commit_rst_en` |
| `et_clk_gate_n` + `rlatchn` / low-phase capture narrowed to preview staging | `prim_write_preview_en` |
| direct `rlatchn` -> `rlatch` pair with both phase outputs live | `prim_phase_pair_lo_hi` |
| direct `rlatch` -> `rlatchn` pair with both phase outputs live | `prim_phase_pair_hi_lo` |
| low-phase write-preview latch feeding `rf_latch_1r_1w` | `prim_rf_1r1w_preview` |
| low-phase write-preview latch feeding `rf_latch_1r_1w_dec` | `prim_rf_1r1w_dec_preview` |
| low-phase write-preview latch feeding `rf_latch_1r_1w_par` | `prim_rf_1r1w_par_preview` |
| low-phase write-preview latch feeding `rf_latch_1r_1w_diff_widths` | `prim_rf_1r1w_diff_preview` |
| low-phase write-preview latch feeding `rf_latch_1r_1w_reg` | `prim_rf_1r1w_reg_preview` |
| low-phase write-preview latch feeding `rf_latch_single_1r_1w_par` | `prim_rf_single_1r1w_par_preview` |
| low-phase write-preview latch feeding `rf_latch_2r_1w` | `prim_rf_2r1w_preview` |
| paired low-phase write-preview seams feeding `rf_latch_3r_2w` | `prim_rf_3r2w_preview` |

These seams are the preferred public translation form for current Minion RTL.
If a site still needs exact transparent behavior that does not fit one of them,
keep that behavior private inside the owning primitive or technology-specific
implementation instead of reviving a public `prim_latch_*` seam.

### Behavior-named write seams

When a translated site has already been narrowed to a pure write-preview or
write-commit role, use the behavior-named primitives directly.

| Original/translated role | ETASP form |
|--------------------------|--------------|
| low-phase write-preview latch staging a 1-bit qualifier or payload bus | `prim_write_preview_en` |
| enabled write-state commit without its own reset contract | `prim_write_commit_en` |
| enabled write-state commit with a defined reset value | `prim_write_commit_rst_en` |

These seams are intentionally narrower than the old raw latch leaves. They
keep the shared RTL honest about behavior while letting FPGA technology trees
re-map the write path without redefining a public latch family.

### Public vs private latch policy

- Raw latch leaves are retired as public translation seams.
- Shared RTL should use only:
  - `prim_write_preview_en`
  - `prim_write_commit_en`
  - `prim_write_commit_rst_en`
  - `prim_phase_pair_lo_hi`
  - `prim_phase_pair_hi_lo`
  - RF preview composites such as `prim_rf_1r1w_preview`,
    `prim_rf_1r1w_dec_preview`, `prim_rf_1r1w_par_preview`,
    `prim_rf_1r1w_diff_preview`, `prim_rf_1r1w_reg_preview`,
    `prim_rf_single_1r1w_par_preview`, `prim_rf_2r1w_preview`, and
    `prim_rf_3r2w_preview`
- If a block still needs exact transparent behavior that does not fit those
  seams, that behavior belongs inside the owning primitive implementation, not
  as a public dependency of translated shared RTL.
- Public primitives should be self-contained. Do not build one public seam by
  instantiating another retired public latch seam underneath it.

### When to introduce a named seam

When translated shared RTL contains a repeated raw two-phase latch chain and
both phase nodes remain architecturally live, prefer a named semantic
composite over re-spelling the individual latches at every site.

Use this technique when all of the following are true:

- the pattern is repeated enough that the raw latch chain obscures the owning
  block's intent
- the pattern has a stable meaning at the translated block level
- the parent-visible timing is defined by the full-cycle interface, not by
  exposing raw helper clocks outside the block

Typical examples include:

- a two-phase loop counter
- a phase-local status handoff
- a ping-pong accumulator pair where both phase outputs remain live
- a low-phase RF write-preview seam where the preview latch is not meant to
  remain a separately visible owner-level signal

See also [latch_translation.md](./latch_translation.md) for the project
decision table that maps common original latch patterns to the preferred
public seams.

The rule is still translation, not redesign:

- keep the same parent-visible cycle timing
- keep the same original phase ordering
- keep the same reset-domain ownership
- move only the implementation seam into a named primitive

In practice this means:

- `tech_generic` should preserve the original gate/latch semantics exactly
- FPGA technology trees may implement the same named composite with a
  phase-correct acyclic form if the backend cannot build the raw latch chain
- behavior-named write seams may be re-mapped independently from the raw latch
  family because they intentionally encode a narrower contract

## FPGA-Specific Rule For Helper Clocks

The current project rule is:

- real functional/local clocks remain real clocks on FPGA
- helper-clock behavior remains explicit in shared RTL through named
  primitives rather than open-coded generated clocks
- when a site can be honestly narrowed to a write-preview or write-commit
  role, use the behavior-named seam instead of keeping the full raw latch
  contract

This is an important distinction.

Examples of clocks that should stay real clocks:

- frontend local gated clocks
- dcache local gated clocks
- `mul_div.clock`
- CSR local block clocks such as `clock_wb` and `clock_dec`

Examples of helper-clock behaviors that should be expressed via named seams:

- narrowed `et_clk_gate` + `rlatch` write commits
- narrowed `et_clk_gate_n` + `rlatchn` write previews
- generated CSR write-latch helper clocks

The current ECP5 implementations are mixed and this is intentional.

- `prim_phase_pair_lo_hi` and `prim_phase_pair_hi_lo`
  preserve the non-resettable transparent semantics directly inside the named
  primitive implementation.
- `prim_write_preview_en`, `prim_write_commit_en`, and
  `prim_write_commit_rst_en`
  use edge-triggered ECP5 mappings because those contracts are intentionally
  narrower than the old raw latch leaves.
- raw latch leaves are no longer public seams; any remaining direct
  transparency lives inside the owning named primitive or technology-specific
  block.

See `hw/ip/tech_ecp5/README.md` for the current platform-specific split.

## Current Live Examples

These current translated sites use the named phase-capture or behavior-named
write seams:

- [`hw/ip/tech_generic/prim_mul_div/rtl/intpipe_mul_div_ctl.sv`](../hw/ip/tech_generic/prim_mul_div/rtl/intpipe_mul_div_ctl.sv)
  embeds the remaining direct phase-transparent control latches locally inside
  the owning `prim_mul_div` implementation rather than depending on public raw
  latch leaves
- [`hw/ip/tech_generic/prim_mul_div/rtl/intpipe_mul_div_dp.sv`](../hw/ip/tech_generic/prim_mul_div/rtl/intpipe_mul_div_dp.sv)
  uses the named phase-capture primitives directly
- [`hw/ip/minion/intpipe/csr/rtl/intpipe_csr_file.sv`](../hw/ip/minion/intpipe/csr/rtl/intpipe_csr_file.sv)
  uses `prim_write_preview_en` for the active CSR write-preview path
- generated CSR write-commit logic in
  [`hw/ip/minion/intpipe/csr/rtl/intpipe_csr_file_auto_write_seq.svh`](../hw/ip/minion/intpipe/csr/rtl/intpipe_csr_file_auto_write_seq.svh)
  uses `prim_write_commit_en` / `prim_write_commit_rst_en`

## Where To Document Block-Specific Choices

Project-wide translation policy belongs here.

Block-specific usage belongs in the block README:

- primitive mapping used by that block
- naming/reset/DFT differences from the original
- any intentional divergence from the original

Technology-specific implementation notes belong in:

- `hw/ip/tech_generic/README.md`
- `hw/ip/tech_<target>/README.md`

That split keeps:

- semantic translation policy in one place
- block-specific mapping local to the IP
- FPGA/ASIC approximations local to the technology tree
