# prim_rf_1r1w_diff

iCE40-specific implementation of `prim_rf_1r1w_diff`.

Why this exists
- The original primitive is a latch-based mixed-width RF with per-slice clock
  gating.
- The iCE40 target needs an FF-based realization, not latch inference.

How this implementation realizes the contract
- It preserves the same falling-edge data capture and rising-edge commit
  protocol as the generic contract.
- Storage is an explicit FF bank with decoded per-entry enables and per-slice
  write enables.
- The combinational wrap-around read view is unchanged.

Why it is implemented this way
- iCE40 logic fabric is a better fit for FF banks plus muxing than for a
  latch-array abstraction.
- Keeping the read flattening identical makes the target-specific change only a
  storage realization choice, not a semantic one.

RTL: `rtl/prim_rf_1r1w_diff.sv`
