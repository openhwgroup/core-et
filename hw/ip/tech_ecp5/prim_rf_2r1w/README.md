# prim_rf_2r1w

ECP5-specific implementation of `prim_rf_2r1w`.

Why this exists
- The primitive needs two combinational read ports and the original phase-split
  write path.
- A single ECP5 LUTRAM-shaped memory does not provide that exact contract
  together with the extra `Parallel` tap view.

How this implementation realizes the contract
- The falling-edge write-data capture stage is preserved exactly.
- Storage is duplicated into two `ram_style="distributed"` arrays so each read
  port keeps a native async-read path.
- `Parallel` taps are kept in a small shadow bank so the implementation does
  not create a third memory read port.
- `Zero` rows remain hardwired to zero.

Why it is implemented this way
- It is the closest ECP5 structured-memory mapping to the original RF contract
  without collapsing the two-read-port behavior into a generic FF bank.

RTL: `rtl/prim_rf_2r1w.sv`
