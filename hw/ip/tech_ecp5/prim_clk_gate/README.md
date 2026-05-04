# prim_clk_gate

ECP5-specific implementation of `prim_clk_gate`.

Why this exists
- ECP5 has dedicated clock-control hardware, so plain fabric-derived clock
  gating is the wrong ownership point for this primitive in the OSS flow.

How this implementation realizes the contract
- The enable is combined with `dft_i.scanmode`.
- The root clock is fed directly to the ECP5 `DCCA` clock-control primitive.
- The combined enable drives `DCCA.CE`; no LUT logic is placed on the clock.
- The internal `DCCA` output net is tagged with `ECP5_IS_GLOBAL`.
- This module is an ECP5 synthesis/PnR mapping, not a generic Verilator
  behavioral proxy.

Why it is implemented this way
- `prim_clk_gate` is a real clock primitive in this tree.
- Using `DCCA.CE` keeps the gate on the real ECP5 clock path instead of
  synthesizing a fabric-generated clock.

RTL: `rtl/prim_clk_gate.sv`
