# prim_rf_1r1w_diff

Xilinx-specific implementation of `prim_rf_1r1w_diff`.

Why this exists
- The primitive exposes an overlapping, wrap-around read view at
  `RAlignment` granularity.
- That read contract is not a native Xilinx RAM port shape, even though the
  underlying storage is logically array-like.

How this implementation realizes the contract
- It preserves the same falling-edge capture and rising-edge commit protocol as
  the generic contract.
- Storage is an explicit FF bank tagged with `ram_style="registers"` to make
  the Xilinx resource choice intentional.
- The combinational flattened read view is unchanged, including wrap-around.

Why it is implemented this way
- Pretending this is a simple LUTRAM would hide a real target constraint: the
  read view is richer than a single Xilinx memory port.
- Owning the FF-bank realization here keeps the module explicitly Xilinx-aware
  without changing behavior.

RTL: `rtl/prim_rf_1r1w_diff.sv`
