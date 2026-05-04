# prim_rf_3r2w

Generic/default implementation of `prim_rf_3r2w`.

## System role

`prim_rf_3r2w` is the latch-timed 3-read/2-write register-file seam used where
the original CORE-ET design relied on `rf_latch_3r_2w`. In the current tree
it is the missing primitive needed before the translated VPU register file can
land.

## Contract

- Both write paths are phase-split: falling-edge data capture, rising-edge
  commit.
- All three read ports are combinational.
- If both write ports target the same entry in one cycle, port A wins.
- RF contents are not reset.

## Why generic looks like this

- This is the portable behavioral reference for the original latch-based RF.
- The B-then-A write ordering is architecturally visible and must be preserved.
- Target-specific overrides may change how storage is built, but not these
  visible semantics.

RTL: `rtl/prim_rf_3r2w.sv`
