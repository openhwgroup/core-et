# prim_clk_gate_n

Generic/default implementation of `prim_clk_gate_n`.

## System role

`prim_clk_gate_n` is the negative-phase companion to `prim_clk_gate`, but its
use is much narrower. In the current tree it is used only by translated minion
intpipe phase-2 logic, specifically the mul/div datapath and CSR file paths
that originally used `et_clk_gate_n`.

Those consumers use `clk_no` as a phase-2 or low-phase clock for latch-timed
behavior. This primitive is therefore a timing-preservation seam, not a
general-purpose inverted clock helper.

## Contract

- `en_i` is sampled only while `clk_i` is high.
- If the sampled enable is 1, `clk_no` reproduces the low phase of `clk_i`.
- If the sampled enable is 0, `clk_no` remains high.
- The output is an inverted gated clock pulse intended for phase-2/latch-timed
  logic.
- The current interface has no reset and no DFT override port; any target
  implementation must preserve that module interface unless the project decides
  to widen the primitive contract globally.

## Why generic looks like this

- The generic body is a high-transparent latch plus OR because that matches the
  original CORE-ET `et_clk_gate_n` behavioral model.
- This is the cleanest semantic reference for ASIC-oriented phase-2 gating.
- FPGA targets may choose a different body if they need a latch-free
  approximation, but they must preserve the observable pulse contract seen by
  the translated intpipe logic.

RTL: `rtl/prim_clk_gate_n.sv`
