# prim_ram_1p

iCE40-specific implementation of `prim_ram_1p`.

Why this exists
- The logical behavior is the same as the generic wrapper, but the synthesis policy is not.
- For iCE40/Yosys, keeping an explicit block-RAM preference on the memory array is part of the target contract.

How this implementation realizes the contract
- The RTL body stays intentionally close to the generic model.
- The memory array carries `(* ram_style = "block" *)` so Yosys prefers `SB_RAM40_4K` resources instead of falling back to logic RAM or registers.

Why it is implemented this way
- The design uses many parameterized widths and depths.
- Directly instantiating and tiling `SB_RAM40_4K` inside this generic wrapper would hard-code packing policy and make odd geometries harder to maintain.
- Letting Yosys perform the legal tiling while the wrapper provides the target hint is the right compromise here.

RTL: `rtl/prim_ram_1p.sv`
