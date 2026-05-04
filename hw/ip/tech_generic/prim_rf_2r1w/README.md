# prim_rf_2r1w

Generic/default implementation of `prim_rf_2r1w`.

## System role

`prim_rf_2r1w` is the latch-timed 2-read/1-write register-file seam used where
the original CORE-ET design relied on `rf_latch_2r_1w`. In the current tree
it is used by the minion integer pipeline register file.

## Contract

- The write path is phase-split: falling-edge data capture, rising-edge
  commit.
- Both read ports are combinational.
- `Zero` marks hardwired-zero entries whose contents must always read as zero.
- `Parallel` selects entries that are also exposed on `rd_par_o` in index
  order.
- RF contents are not reset.

## Why generic looks like this

- This is the portable behavioral reference for the original latch-based RF.
- The explicit `Zero` and `Parallel` contracts matter architecturally and must
  survive any target-specific remapping.
- Target-specific overrides may change how storage is built, but not these
  visible semantics.

RTL: `rtl/prim_rf_2r1w.sv`
