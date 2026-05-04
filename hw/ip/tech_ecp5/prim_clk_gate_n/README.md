# prim_clk_gate_n

ECP5-specific implementation of `prim_clk_gate_n`.

Why this exists
- ECP5 has no dedicated negative-phase clock-gate primitive, and the generic
  latch-based model infers latch feedback in FPGA fabric.

How this implementation realizes the contract
- It uses the same high-transparent enable latch and `clk_i | ~en_latch`
  waveform as the generic/original contract.
- The root clock is first gated with `DCCA.CE = en_latch`.
- The disabled-high waveform is formed as `clk_enabled | ~en_latch`.
- That waveform is then routed through a second ECP5 `DCCA` cell so the
  exported `clk_no` is a DCCA output.

Why it is implemented this way
- ECP5 has no exact primitive for a clock-when-enabled/held-high-when-disabled
  local clock, so this preserves the original waveform while keeping the final
  local clock on an explicit DCCA output.
- Prefer coarser phase/register primitives for new translated sites when the
  raw negative-phase local clock can be absorbed without changing visible
  timing.

RTL: `rtl/prim_clk_gate_n.sv`
