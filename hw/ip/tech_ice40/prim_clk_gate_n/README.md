# prim_clk_gate_n

iCE40-specific implementation of `prim_clk_gate_n`.

Why this exists
- iCE40 has no dedicated high-transparent negedge clock-gate cell.
- Falling back to the generic latch-based model would push latch inference into a target that is better served by FF-based clocking structures.

How this implementation realizes the contract
- The enable is captured with a posedge FF at the start of the high phase.
- The captured enable is ORed with the root clock to produce the following low-phase pulse on `clk_no`.

Why it is implemented this way
- This is the symmetric FPGA approximation of `prim_clk_gate`.
- It maps cleanly onto iCE40 FF plus LUT resources and avoids latch inference.
- It is more restrictive than the generic high-transparent latch model, but it preserves the phase-2 split expected by the translated logic.

RTL: `rtl/prim_clk_gate_n.sv`
