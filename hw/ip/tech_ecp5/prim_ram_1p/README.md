# prim_ram_1p

ECP5-specific implementation of `prim_ram_1p`.

Why this exists
- RAM inference is a real technology seam.
- The generic RAM wrapper is the contract reference, but FPGA targets should
  steer inference toward the right on-chip memory resources.

How this implementation realizes the contract
- The body is still a straightforward single-port RAM wrapper.
- The storage array carries `(* ram_style = "block" *)` to bias inference
  toward ECP5 block RAM resources.

Why it is implemented this way
- This preserves the generic visible behavior while giving ECP5 synthesis the
  correct implementation hint for banked SRAM-style storage.

RTL: `rtl/prim_ram_1p.sv`
