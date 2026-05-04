# prim_rf_1r1w

iCE40-specific implementation of `prim_rf_1r1w`.

Why this exists
- The original CORE-ET primitive is a latch-array RF with gated clocks.
- iCE40 fabric has no latch-based RF primitive and this is not a BRAM-shaped
  storage contract.

How this implementation realizes the contract
- The half-cycle write protocol is preserved with a negedge data-capture
  register followed by posedge storage flops.
- The storage is an explicit FF bank with decoded per-entry write enables.
- The read path remains combinational.

Why it is implemented this way
- This maps naturally onto iCE40 FF resources without inferring latches or
  internal gated clocks.
- The decoded write enables are a better fit for iCE40 synthesis than treating
  the RF like a generic memory.

RTL: `rtl/prim_rf_1r1w.sv`
