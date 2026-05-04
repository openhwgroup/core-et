# prim_rf_2r1w

Xilinx-specific implementation of `prim_rf_2r1w`.

Why this exists
- The primitive needs two combinational read ports and a phase-split write
  path.
- A single Xilinx distributed RAM instance does not provide that exact
  combination together with the additional `Parallel` tap view.

How this implementation realizes the contract
- The falling-edge capture stage is preserved exactly.
- Storage is duplicated into two `ram_style="distributed"` arrays so each read
  port keeps a native async-read path.
- `Parallel` taps are maintained in a small shadow register bank so the module
  does not create a third memory read port.
- `Zero` rows remain hardwired to zero.

Why it is implemented this way
- This is the closest Xilinx resource mapping to the original RF contract
  without collapsing the read-port behavior into a generic FF bank.
- The target-specific logic is still explicit about where Xilinx runs out of a
  native memory shape and needs auxiliary state.

RTL: `rtl/prim_rf_2r1w.sv`
