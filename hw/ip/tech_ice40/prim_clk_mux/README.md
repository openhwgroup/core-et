# prim_clk_mux

iCE40-specific notes for `prim_clk_mux`.

Why there is no dedicated iCE40 RTL body
- iCE40 does not provide a general internal glitchless clock-mux primitive that
  this module can target as a drop-in replacement for the generic contract.
- Treating clocks like ordinary LUT data-path signals is not acceptable for a
  live functional clock network.
- In this tree, `prim_clk_mux` is only used for SRAM DFT clock override, where
  the select is expected to be static or changed only when the affected clock
  path is quiescent.

What iCE40 uses instead
- `mk/prim.mk` intentionally falls back to `tech_generic/prim_clk_mux`.
- That fallback is acceptable specifically because the contract is static
  selection for test/DFT use, not dynamic in-field clock switching.

Why it is documented here
- Clock muxing is still a technology seam, even when the correct iCE40 choice is
  to keep the generic behavioral body and narrow the usage contract.
- This README makes that choice explicit so the fallback is intentional rather
  than accidental.
