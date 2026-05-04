# prim_rf_1r1w

Xilinx-specific implementation of `prim_rf_1r1w`.

Why this exists
- The primitive contract is async-read, sync-write storage with a half-cycle
  write protocol.
- That is a good match for Xilinx distributed RAM, not for BRAM and not for
  the generic latch-array model.

How this implementation realizes the contract
- The falling-edge data capture stage is preserved exactly.
- Storage uses an async-read, sync-write array tagged with
  `ram_style="distributed"` to bias Xilinx synthesis toward LUTRAM.
- The read path stays combinational.

Why it is implemented this way
- This is the natural Xilinx resource mapping for a 1R1W RF with combinational
  reads.
- It keeps the target-specific change focused on resource selection, not on the
  externally visible RF timing contract.

RTL: `rtl/prim_rf_1r1w.sv`
