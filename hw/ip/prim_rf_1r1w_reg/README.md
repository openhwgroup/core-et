# prim_rf_1r1w_reg

Latch-timed 1R1W register-file primitive with registered read address.

This IP currently provides:

| Module | Selected by | Implementations |
|--------|-------------|-----------------|
| `prim_rf_1r1w_reg` | `mk/prim.mk` via `PRIM_RF_1R1W_REG` | `tech_generic/prim_rf_1r1w_reg`, `tech_ecp5/prim_rf_1r1w_reg` |

The implementation preserves the original `rf_latch_1r_1w_reg` timing:
- falling edge: capture `wr_data_i` when `wr_data_en_1p_i` is high
- rising edge: commit the captured data when `wr_en_i` is high
- rising edge: capture `rd_addr_i` when `rd_en_i` is high
- output: combinational read from the registered address

This gives a 1-cycle read latency with hold-last-address behavior when `rd_en_i` is low.
