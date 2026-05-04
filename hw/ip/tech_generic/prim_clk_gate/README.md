# prim_clk_gate

Generic/default implementation of `prim_clk_gate`.

## System role

`prim_clk_gate` is the project-wide positive-phase clock-gating primitive.
Current users include:
- shirecache local bank, trace, perfmon, request-queue, and SRAM clocks
- rbox local clock gating
- minion frontend, TLB, mul/div, and generated CSR latch clocks

In shirecache and rbox, the real `dft_i` bundle is passed through so scanmode
can force gated clocks on. Some minion blocks currently tie `dft_i` to `'0`,
but they still rely on the same clock-gate timing contract.

## Contract

- `en_i` is sampled only while `clk_i` is low.
- If the sampled enable is 1, `clk_o` reproduces the next high phase of
  `clk_i`; if it is 0, that high phase is suppressed.
- `dft_i.scanmode=1` bypasses functional gating and makes `clk_o` follow
  `clk_i`.
- The primitive has no reset and does not define any special power-up state
  beyond normal RTL semantics.
- This is a real clock primitive. Downstream logic may rely on true clock
  pulses, not just equivalent clock-enable behavior.

## Why generic looks like this

- The generic body is latch plus AND because that matches the original
  CORE-ET `et_clk_gate` behavioral model.
- It also matches the semantic shape of a standard ASIC ICG cell, so the
  generic version is the cleanest contract reference.
- FPGA targets that should avoid a latch-based implementation override this
  module in `tech_<target>/`.

RTL: `rtl/prim_clk_gate.sv`
