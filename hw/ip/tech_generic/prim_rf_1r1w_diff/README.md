# prim_rf_1r1w_diff

Generic/default implementation of `prim_rf_1r1w_diff`.

## System role

`prim_rf_1r1w_diff` is the mixed-width latch-timed RF seam used where the
original CORE-ET design relied on `rf_latch_1r_1w_diff_widths`. In the
current tree it is used by the minion frontend thread buffer.

## Contract

- The write path is phase-split, with per-subword enables.
- On the falling edge, each read-width slice of `wr_data_i` is captured when
  the corresponding `wr_data_en_1p_i` bit is asserted.
- On the rising edge, the captured slices are committed when the matching
  `wr_en_i` bits are asserted.
- The read path is combinational.
- The read address indexes at `RAlignment` granularity and wrap-around at the
  final entry boundary is part of the contract.
- RF contents are not reset.

## Why generic looks like this

- This is the direct behavioral reference for the original half-cycle contract.
- The flattened read view makes the wrap-around alignment rules explicit.
- Target-specific overrides may choose different storage structures, but not a
  different read-address interpretation or write phase ordering.

RTL: `rtl/prim_rf_1r1w_diff.sv`
