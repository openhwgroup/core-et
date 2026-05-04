# prim_rf_3r2w

iCE40-specific implementation of `prim_rf_3r2w`.

Why this exists
- The original `rf_latch_3r_2w` is a latch-array RF with two write ports and
  three combinational read ports.
- iCE40 has no native memory primitive with that exact contract.

How this implementation realizes the contract
- Two negedge capture registers preserve the original half-cycle write staging.
- Storage is an explicit FF bank with direct combinational reads.
- Port B writes first and port A writes second, so same-address conflicts still
  resolve in favor of port A.

Why it is implemented this way
- This keeps the architectural 3R2W RF behavior intact without pretending the
  block is BRAM-like.

RTL: `rtl/prim_rf_3r2w.sv`
