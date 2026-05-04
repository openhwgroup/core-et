# prim_clk_gate

iCE40-specific implementation of `prim_clk_gate`.

Why this exists
- iCE40 has no dedicated latch-based ICG cell.
- Inferring the generic low-transparent latch model is the wrong fit for iCE40 fabric and for Yosys `synth_ice40`.

How this implementation realizes the contract
- The enable is captured with a negedge FF at the start of the low phase.
- The captured enable is ANDed with the root clock to produce the following high-phase pulse.
- `dft_i.scanmode` still forces the clock on.

Why it is implemented this way
- This is the standard FPGA approximation of a positive-phase integrated clock gate.
- It maps naturally onto iCE40 FF plus LUT resources and avoids latch inference.
- It is slightly more restrictive than the generic latch model, but it preserves the intended phase split used by the design.

RTL: `rtl/prim_clk_gate.sv`
