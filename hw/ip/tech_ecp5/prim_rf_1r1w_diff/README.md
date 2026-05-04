# prim_rf_1r1w_diff

ECP5-specific implementation of `prim_rf_1r1w_diff`.

Why this exists
- The primitive exposes an overlapping, wrap-around read view at
  `RAlignment` granularity.
- The generic flattened readback form is functionally correct, but it is a
  poor FPGA storage shape because it turns one small RF into a large
  fabric-visible flattened bus.

How this implementation realizes the contract
- It currently uses the exact generic/original implementation.
- The falling-edge write-data capture stage and wrap-around aligned read view
  are therefore preserved identically.

Why it is implemented this way
- The previous ECP5-specific storage reshaping is a plausible source of the
  current frontend FPGA regression.
- Falling back to the exact generic/original implementation is the fastest safe
  way to remove that variable before attempting a new ECP5-specific shape.

RTL: `rtl/prim_rf_1r1w_diff.sv`
