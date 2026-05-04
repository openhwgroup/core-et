# prim_rf_single_1r1w_par

Latch-timed single-entry write-capture primitive with direct readback.

This IP currently provides:

| Module | Selected by | Implementations |
|--------|-------------|-----------------|
| `prim_rf_single_1r1w_par` | `mk/prim.mk` via `PRIM_RF_SINGLE_1R1W_PAR` | `tech_generic/prim_rf_single_1r1w_par` |

The implementation preserves the original `rf_latch_single_1r_1w_par` timing:
- falling edge: capture `wr_data_i` when `wr_data_en_1p_i` is high
- rising edge: commit the captured data when `wr_en_i` is high
- output: expose the stored value directly
