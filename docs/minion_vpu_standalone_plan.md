# Minion VPU to `standalone_minion` dependency map

This document records the dependency chain from the current translated Minion
state to a full original-equivalent `standalone_minion` wrapper. It is a
planning map for translation and verification work; `STATUS.md` remains the
authoritative source for module-by-module completion counts.

## Current state

The translated `minion_top` path is intentionally integer-only for VPU behavior
at the moment. `hw/ip/minion/rtl/minion_top.sv` preserves the original top-level
core/VPU split, but the VPU instance is currently satisfied by
`hw/ip/minion/rtl/null_vpu.sv` instead of a translated `vpu_top`.

`null_vpu` is a bring-up helper, not a CORE-ET translation target. It keeps the
minion-side VPU boundary connected while failing closed for non-integer work:
VPU-facing outputs are idle where possible, FP/VPU/ML operations that reach the
boundary are reported as illegal, and debug data is tied to benign values. This
is useful for scalar Minion integration and for proving that the shell does not
hang when the VPU subtree is absent, but it is not original-equivalent VPU
behavior.

The bottom-up VPU subtree under `hw/ip/minion/vpu/` already contains many
translated and verified leaves. The missing dependency is not the final shell
wrapper alone: a full original-equivalent `standalone_minion` depends first on a
real translated `vpu_top` being integrated into `minion_top` and verified there.
The current `minion_top` cosim is therefore an integer-only integration check;
it should not be treated as evidence that full VPU/FP/tensor behavior has been
translated.

## Dependency graph

```text
translated VPU seed audit/adaptation
        |
        v
VPU top-half RTL import into hw/ip/minion/vpu/
        |
        v
standalone VPU unit tests and RTL cosims
        |
        v
real vpu_top integrated into minion_top
        |
        v
full-VPU minion_top unit tests and RTL cosim
        |
        v
standalone_minion wrapper RTL
        |
        v
standalone_minion unit tests and RTL cosim
```

The standalone shell modules that are independent of the VPU can continue to be
translated and verified in parallel. The final `standalone_minion` wrapper,
however, is blocked from being original-equivalent until the `minion_top` it
instantiates is no longer backed by `null_vpu` for VPU behavior.

## Safe work order

1. **Audit the translated VPU seed before importing.**
   Use the existing translated VPU seed repository as an adaptation source for
   top-half RTL and collateral instead of restarting from the original RTL.
   The seed already carries useful work for modules such as `vpu_ml`,
   `vpu_tensorfma`, `vpu_tensorquant`, `vpu_tensorreduce`, `vpu_lane`,
   `vpu_ctrl`, `vpu_txfma_trans_top`, the `txfma*_top` blocks, and `vpu_top`.

2. **Preserve target package/RF policy.**
   Do not blindly overwrite the target repository's
   `hw/ip/minion/vpu/rtl/vpu_pkg.sv` or `hw/ip/minion/vpu/rtl/vpu_rf.sv` when
   reusing seed RTL. The target tree currently keeps explicit MMI compatibility
   knobs for the VPU RF path and verifies both `UseMmi=0` and `UseMmi=1`
   behavior. If imported top-half RTL needs additional constants or types, merge
   them into the target policy deliberately and document any intentional
   divergence in the VPU README and, if applicable, `BUGS.md`.

3. **Import and normalize the VPU top-half RTL.**
   Bring over the top-half modules behind the existing package, primitive, reset,
   and naming conventions. Keep feature selection as explicit parameters or
   package defaults; do not reintroduce functional preprocessor switches.

4. **Port existing standalone VPU DV/cosims.**
   Where the seed already has standalone tests and cosims, adapt them to the
   target tree rather than writing shallow replacements. Each translated module
   still needs its own unit test and its own RTL cosim with every output compared.

5. **Add the missing standalone VPU DV/cosims.**
   Any imported module that only has RTL present remains incomplete until it has
   proportional standalone unit tests, standalone cosim, Makefile coverage, and
   `STATUS.md` entries. This includes the final `vpu_top` integration module.

6. **Update coverage and status after VPU collateral changes.**
   VPU cosim changes require refreshed checked-in cosim LCOV data under
   `dv/rtlcosim/coverage/`, and `STATUS.md` must reflect the exact test and
   cosim counts.

7. **Replace `null_vpu` in `minion_top` with the real `vpu_top`.**
   Only after `vpu_top` has standalone coverage should `minion_top` be changed
   to instantiate it. The `minion_top` unit tests and cosim must then expand
   beyond integer-only stimulus to exercise VPU/FP/tensor paths, DCache/VPU
   sidebands, debug monitor paths, and kill/replay interactions while comparing
   all top-level outputs.

8. **Add the final `standalone_minion` wrapper.**
   The final wrapper should be translated and cosimmed only after the real-VPU
   `minion_top` path is verified. Otherwise the wrapper would only prove the
   scalar/null-VPU shell, not full original-equivalent standalone Minion
   behavior.

## Completion gates

A task is not complete merely because `standalone_minion` elaborates. The staged
completion gates are:

| Gate | Required evidence |
|------|-------------------|
| VPU top-half imported | RTL follows target package/primitive policy and does not regress existing VPU leaf tests/cosims |
| VPU top-half verified | Every imported VPU module has standalone unit tests, standalone RTL cosim, Makefile coverage, and `STATUS.md` counts |
| Real-VPU `minion_top` | `minion_top` instantiates translated `vpu_top`, no longer relies on `null_vpu` for VPU behavior, and has full-path unit/cosim coverage |
| `standalone_minion` complete | The shell instantiates the real-VPU `minion_top`, all shell outputs are compared in cosim, and status/coverage are updated |

Until the real-VPU `minion_top` gate is complete, documentation and status
entries should describe the top-level path as integer-only or null-VPU-backed
for VPU behavior.
