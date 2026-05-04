# prim_ram_2p

ECP5-specific implementation of `prim_ram_2p`.

Why this exists
- RAM inference is a real technology seam.
- The generic dual-port RAM wrapper is the contract reference, but FPGA targets
  should steer inference toward the right on-chip memory resources.

How this implementation realizes the contract
- The body remains a dual-port RAM wrapper with the generic visible behavior.
- The storage array carries `(* ram_style = "block" *)` to bias inference
  toward ECP5 block RAM resources.

Why it is implemented this way
- This preserves the generic contract while giving ECP5 synthesis the correct
  implementation hint for SRAM-style dual-port storage.

RTL: `rtl/prim_ram_2p.sv`
