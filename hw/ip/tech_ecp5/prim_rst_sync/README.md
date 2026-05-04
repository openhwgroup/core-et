# prim_rst_sync

ECP5-specific implementation of `prim_rst_sync`.

Why this exists
- Reset-domain handoff is a true technology seam in the project.
- Even when the functional behavior matches the generic model, the target still
  needs an explicit ownership point for FPGA-specific reset implementation.

How this implementation realizes the contract
- Reset is asynchronously asserted and synchronously released on `clk_i`.
- `dft_i.scanmode` bypasses the synchronizer and forwards `dft_i.scan_reset_n`.

Why it is implemented this way
- The primitive preserves the project reset contract while keeping the ECP5
  target on an explicit, auditable implementation path.

RTL: `rtl/prim_rst_sync.sv`
