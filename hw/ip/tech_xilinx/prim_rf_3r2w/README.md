# prim_rf_3r2w

Xilinx-specific implementation of `prim_rf_3r2w`.

Why this exists
- The primitive needs three combinational read ports and two phase-split write
  ports.
- A single Xilinx memory primitive does not provide that exact shape.

How this implementation realizes the contract
- Two negedge capture registers preserve the original half-cycle write staging.
- Storage is an explicit FF bank with direct combinational reads.
- Port B writes first and port A writes second, so same-address conflicts still
  resolve in favor of port A.

Why it is implemented this way
- This is the simplest faithful Xilinx-side mapping of the original RF
  contract until a more area-aware specialization is needed.

RTL: `rtl/prim_rf_3r2w.sv`
