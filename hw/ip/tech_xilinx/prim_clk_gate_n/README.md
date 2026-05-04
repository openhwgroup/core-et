# prim_clk_gate_n

Xilinx-specific implementation of `prim_clk_gate_n`.

Why this exists
- The generic primitive is a high-transparent latch plus OR gate.
- Xilinx fabric can realize latches, but using latch-based clock logic in an
  FPGA clock path is not the right target-specific tradeoff.

How this implementation realizes the contract
- The enable is captured with a posedge FF at the start of the high phase.
- The captured enable is ORed with the root clock to produce the following
  active-low pulse on `clk_no`.

Why it is implemented this way
- This is the Xilinx-side companion to `prim_clk_gate`: FPGA-friendly local
  clock shaping instead of latch inference.
- It preserves the low-phase pulse contract expected by the translated
  phase-2/latch-timed minion logic.

RTL: `rtl/prim_clk_gate_n.sv`
