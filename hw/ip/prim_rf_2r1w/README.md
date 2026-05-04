# prim_rf_2r1w

Latch-timed 2R1W register-file primitive used by translated CORE-ET blocks.

Replaces: `rf_latch_2r_1w` from the CORE-ET source tree.

Implementation selection:
- `mk/prim.mk` exposes this primitive as `PRIM_RF_2R1W`
- Current implementations live under `tech_generic/prim_rf_2r1w` and
  `tech_ice40/prim_rf_2r1w` and `tech_ecp5/prim_rf_2r1w` and
  `tech_xilinx/prim_rf_2r1w`

## Contract

- Falling edge: capture `wr_data_i` when `wr_data_en_1p_i` is asserted.
- Rising edge: commit captured data when `wr_en_i` is asserted.
- Read ports A and B are combinational.
- `Zero` marks hardwired-zero rows.
- `Parallel` selects rows that are concatenated onto `rd_par_o`.
- RF contents are not reset.

## Current users

- `hw/ip/minion/intpipe/rtl/intpipe_rf.sv`

## Differences from CORE-ET

| Aspect | Original | Reimplementation | Rationale |
|--------|----------|------------------|-----------|
| Module name | `rf_latch_2r_1w` | `prim_rf_2r1w` | Repository primitive naming |
| `test_en` | Explicit port | Removed from public contract | DFT/clocking detail belongs in the target-specific implementation |
| Storage body | Latch array with gated clocks | Technology-selected RF primitive body | Keeps the latch-timed contract while allowing FPGA-friendly implementations |

## Verification

RTL co-simulation is available in `dv/rtlcosim/prim_rf_2r1w/`.
