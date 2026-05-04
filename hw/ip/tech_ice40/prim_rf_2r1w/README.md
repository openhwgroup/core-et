# prim_rf_2r1w

iCE40-specific implementation of `prim_rf_2r1w`.

Why this exists
- The original `rf_latch_2r_1w` is a latch-array RF with gated clocks.
- iCE40 needs an FF-bank realization that still preserves dual combinational
  reads, hardwired-zero rows, and parallel taps.

How this implementation realizes the contract
- A negedge capture register preserves the original half-cycle write staging.
- Storage is an explicit per-entry FF bank with decoded write enables.
- `Zero` rows remain hardwired to zero and `Parallel` taps are preserved.

Why it is implemented this way
- This is the clean iCE40 substitute for the original latch-and-clock-gate
  structure.
- It keeps the architectural RF behavior intact without pretending the block is
  a BRAM-like memory.

RTL: `rtl/prim_rf_2r1w.sv`
