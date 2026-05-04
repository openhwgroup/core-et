# prim_rf_1r1w

ECP5-specific implementation of `prim_rf_1r1w`.

Why this exists
- The primitive contract is async-read, sync-write storage with the original
  half-cycle write-capture timing.
- On ECP5, that is a better match for structured LUTRAM than for the generic
  FF-array fallback.

How this implementation realizes the contract
- The falling-edge write-data capture stage is preserved exactly.
- Storage is tagged with `ram_style="distributed"` so ECP5 synthesis can map
  the RF into LUTRAM-shaped memory instead of ordinary fabric flops.
- The read path remains combinational.

Why it is implemented this way
- It preserves the externally visible RF timing contract.
- It gives Yosys a cleaner ECP5 memory shape than the generic storage body.

RTL: `rtl/prim_rf_1r1w.sv`
