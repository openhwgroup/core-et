# prim_rf_1r1w

Generic/default implementation of `prim_rf_1r1w`.

## System role

`prim_rf_1r1w` is the latch-timed 1-read/1-write register-file seam used where
the original CORE-ET design relied on `rf_latch_1r_1w`. Current users include
minion TLB and dcache metadata/buffer arrays, plus shirecache data queues.

## Contract

- The write path is phase-split.
- On the falling edge, `wr_data_i` is captured when `wr_data_en_i=1`.
- On the following rising edge, the captured data is committed when `wr_en_i=1`.
- The read path is combinational and returns the addressed entry in the same
  cycle.
- RF contents are not reset. Power-up state is intentionally unspecified.

## Why generic looks like this

- This is the behavioral reference for the half-cycle write protocol.
- It matches the original Verilator branch closely, making standalone cosim
  straightforward.
- ASIC and FPGA targets are free to realize the storage differently, but they
  must preserve the same phase ordering and combinational read contract.

RTL: `rtl/prim_rf_1r1w.sv`
