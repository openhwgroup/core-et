# prim_rst_sync

Generic/default implementation of `prim_rst_sync`.

## System role

`prim_rst_sync` is the project reset repeater: async assert, sync release,
active low. It is not a spare utility primitive. In the current tree it is used
heavily in shirecache to derive synchronized cold, warm, debug, and local
subdomain resets, including reset paths that must be generated on ungated clocks
before a downstream clock gate can safely operate.

This is the replacement for the original CORE-ET `rst_repeat`, so translated
logic expects the same one-cycle delayed release semantics.

## Contract

- `rst_ni` asserts `rst_no` asynchronously.
- `rst_no` deasserts synchronously to `clk_i`.
- The primitive behaves as a one-stage reset repeater, not as a general
  multi-stage data synchronizer.
- `dft_i.scanmode=1` switches the repeater onto `dft_i.scan_reset_n` as its
  async reset source and also bypasses `rst_no` directly from
  `dft_i.scan_reset_n`.
- The module is intended for reset-domain handoff and DFT-visible reset control.
  Functional modules may rely on its release timing to match the original
  design's reset sequencing.

## Why generic looks like this

- The generic body is a single flop plus DFT bypass because that matches the
  original `rst_repeat` behavior the rest of the design was translated against.
- It is the simplest contract reference for future technologies.
- A target may override it if reset distribution, scan insertion, or hardened
  reset cells require a different implementation body.

RTL: `rtl/prim_rst_sync.sv`
