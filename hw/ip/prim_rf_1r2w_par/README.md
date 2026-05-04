# prim_rf_1r2w_par

Latch-timed 1R2W register-file primitive with parallel readback.

This IP currently provides:

| Module | Selected by | Implementations |
|--------|-------------|-----------------|
| `prim_rf_1r2w_par` | `mk/prim.mk` via `PRIM_RF_1R2W_PAR` | `tech_generic/prim_rf_1r2w_par` |

The implementation preserves the original `rf_latch_1r_2w_par` timing:

- falling edge: capture `wr_data_a_i` and/or `wr_data_b_i` when their
  corresponding `*_en_1p_i` strobes are high
- rising edge: commit captured write-port B first, then write-port A
- output: expose every entry in parallel as a packed flat bus

If both write ports target the same entry in the same cycle, port A wins,
matching the original implementation.
