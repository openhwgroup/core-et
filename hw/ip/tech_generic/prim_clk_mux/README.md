# prim_clk_mux

Generic/default implementation of `prim_clk_mux`.

## System role

`prim_clk_mux` is the clock-selection seam for SRAM wrappers. In the current
design it is used primarily in shirecache and minion local-RAM wrappers to
switch between:
- the functional SRAM clock path
- the DFT SRAM override clock selected by `dft_i.sram_clk_override`

It is not currently used as a dynamic clock-tree mux, DVFS selector, or general
clock-domain switching element.

## Contract

- `clk_o` follows `clk0_i` when `sel_i=0` and `clk1_i` when `sel_i=1`.
- `UseTechClockMux=0` is an explicit opt-out for tied-off DFT paths that only
  use `clk0_i`; normal functional tests and cosims should leave it at `1`.
- The primitive does not synchronize `sel_i` and does not provide glitch
  filtering or mutual exclusion by itself.
- Callers must only change `sel_i` when the switch is safe:
  - both source clocks are stopped, or
  - the deselected source is already quiesced or gated off, or
  - the design is in a static DFT/test mode where the switch is controlled
    outside normal functional traffic
- The selected clock must preserve the source phase and pulse shape expected by
  the RAM wrapper behind it.

## Why generic looks like this

- The generic body is a plain mux because the contract is about the insertion
  point and safe-use rules, not about any one vendor primitive.
- This matches the original CORE-ET `et_clk_mux2` behavioral fallback.
- A target with a dedicated glitch-managed clock mux may override this module,
  but the caller contract remains the same.

RTL: `rtl/prim_clk_mux.sv`
