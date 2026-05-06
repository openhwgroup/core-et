# Minion VPU to `standalone_minion` dependency map

This document records the dependency chain that closed the translated real-VPU
Minion path and the original-equivalent `standalone_minion` wrapper milestone.
It is now a milestone record and historical map; `STATUS.md` remains the
authoritative source for module-by-module completion counts.

## Current milestone state

The default translated `minion_top` path instantiates the real translated
`hw/ip/minion/vpu/rtl/vpu_top.sv` when `VpuEn=1` (the default), preserving the
original top-level core/VPU split. The Minion top-level unit tests and cosim now
exercise the real-VPU integration path and compare against the original
real-VPU `minion_top`.

`null_vpu` remains available only behind `VpuEn=0`. It is a bring-up helper, not
a CORE-ET translation target. It keeps the minion-side VPU boundary connected
while failing closed for non-integer work: VPU-facing outputs are idle where
possible, FP/VPU/ML operations that reach the boundary are reported as illegal,
and debug data is tied to benign values. This is useful for scalar Minion
integration and for proving that the shell does not hang when the VPU subtree is
intentionally absent, but it is not original-equivalent VPU behavior and is not
the standalone-shell target.

The bottom-up VPU subtree under `hw/ip/minion/vpu/` has standalone closure for
the translated real `vpu_top` path and is integrated into default `minion_top`.
The translated `standalone_minion` wrapper is integrated under
`hw/ip/standalone_minion/` and instantiates the default real-VPU `minion_top`.

Current verification evidence from `STATUS.md`:

| Integration point | Unit DV | RTL cosim | Status |
|-------------------|---------|-----------|--------|
| `minion_top` | 17 smoke checks + 5 `VpuEn=0` checks + 17 debug-APB-off checks + 15 debug-off checks + 9 execution checks | 118,468 comparisons | Done; default `VpuEn=1` uses translated real `vpu_top` |
| `vpu_top` | 19 checks | 4,823,376 comparisons | Done; standalone VPU-local DV/cosim plus real default `minion_top` integration |
| `standalone_minion` | 19 checks | 1,765,940 comparisons | Done; all standalone outputs compared cycle-by-cycle |

## Dependency graph

```text
translated VPU seed audit/adaptation              [done]
        |
        v
VPU top-half RTL import into hw/ip/minion/vpu/    [done]
        |
        v
standalone VPU unit tests and RTL cosims          [done]
        |
        v
real vpu_top integrated into minion_top           [done]
        |
        v
full-VPU minion_top unit tests and RTL cosim      [done]
        |
        v
standalone_minion wrapper RTL                     [done]
        |
        v
standalone_minion unit tests and RTL cosim        [done]
```

The standalone shell modules that are independent of the VPU were translated and
verified around the same sequence. The final shell closure is meaningful because
it uses the real-VPU default `minion_top`; a `VpuEn=0`/`null_vpu` configuration
would only prove the scalar/null-VPU shell and would not be original-equivalent
for VPU behavior.

## Historical work order and preserved decisions

The milestone closed through the following staged order:

1. **Audit the translated VPU seed before importing.**
   The existing translated VPU seed was used as an adaptation source for top-half
   RTL and collateral instead of restarting from the original RTL.

2. **Preserve target package/RF policy.**
   Imported RTL was merged into the target repository's
   `hw/ip/minion/vpu/rtl/vpu_pkg.sv` and `hw/ip/minion/vpu/rtl/vpu_rf.sv`
   policy instead of blindly overwriting it. The target tree keeps explicit MMI
   compatibility knobs for the VPU RF path and verifies both `UseMmi=0` and
   `UseMmi=1` behavior.

3. **Import and normalize the VPU top-half RTL.**
   The top-half modules were brought behind target package, primitive, reset,
   and naming conventions. Functional feature selection remains represented as
   explicit parameters or package defaults rather than translated preprocessor
   switches.

4. **Port standalone VPU DV/cosims.**
   Existing seed tests and cosims were adapted where available. Each completed
   translated VPU module has proportional unit-test and RTL-cosim evidence in
   `STATUS.md`.

5. **Add missing standalone VPU DV/cosims.**
   Imported top-half modules were not counted as done until standalone unit DV,
   standalone cosim, Makefile coverage, and `STATUS.md` entries existed.

6. **Update coverage and status after VPU collateral changes.**
   VPU and Minion cosim changes refreshed the checked-in cosim LCOV data under
   `dv/rtlcosim/coverage/`, and `STATUS.md` records current test and cosim
   counts.

7. **Replace the default `minion_top` VPU path with real `vpu_top`.**
   The default `VpuEn=1` path now instantiates translated real `vpu_top`. The
   `VpuEn=0` path intentionally preserves `null_vpu` for scalar-only bring-up.

8. **Add and verify the standalone shell wrapper.**
   The `standalone_minion` wrapper was translated after the real-VPU `minion_top` path and
   uses the default real-VPU configuration. Its unit DV and RTL cosim are now
   present and passing.

## Completion gates

The staged completion gates are now closed:

| Gate | Required evidence | Current state |
|------|-------------------|---------------|
| VPU top-half imported | RTL follows target package/primitive policy and does not regress existing VPU leaf tests/cosims | Done |
| VPU top-half verified | Imported VPU modules have standalone unit tests, standalone RTL cosim, Makefile coverage, and `STATUS.md` counts | Done for the integrated real-VPU path |
| Real-VPU `minion_top` | Default `minion_top` instantiates translated `vpu_top`, keeps `null_vpu` only behind `VpuEn=0`, and has real-VPU unit/cosim coverage | Done; 118,468 cosim comparisons |
| `standalone_minion` complete | The shell instantiates the real-VPU `minion_top`, all shell outputs are compared in cosim, and status/coverage are updated | Done; 19 checks and 1,765,940 cosim comparisons |

Documentation and status entries should describe the top-level default path as
real-VPU-backed and reserve `null_vpu`/integer-only wording for the intentional
`VpuEn=0` configuration.
