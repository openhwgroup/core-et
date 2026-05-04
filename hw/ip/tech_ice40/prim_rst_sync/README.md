# prim_rst_sync

iCE40-specific implementation of `prim_rst_sync`.

Why this exists
- Reset handoff is a real technology seam, even when the external interface is
  simple.
- The original CORE-ET `rst_repeat` drives the DFT-selected reset onto both
  the repeater flop's async reset input and the final bypass path.
- iCE40 has ordinary async-reset flops (`SB_DFFR`-class mapping), but no special
  hardened reset-repeater macro to target here.

How this implementation realizes the contract
- `rst_ni` asserts `rst_no` asynchronously and releases it synchronously.
- In scan mode, `dft_i.scan_reset_n` becomes the async reset source seen by the
  repeater flop.
- `rst_no` is still bypassed directly from `dft_i.scan_reset_n` while scan mode
  is active.

Why it is implemented this way
- The body stays as a single-flop repeater because that is the actual contract
  the translated RTL depends on.
- The implementation is written in ordinary synthesizable RTL so Verilator and
  Yosys can share the same source; Yosys can map it onto the iCE40 async-reset
  flip-flop resources.

RTL: `rtl/prim_rst_sync.sv`
