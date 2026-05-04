# prim_rf_1r1w_dec

Latch-timed 1R1W register-file primitive with decoded one-hot read and write
addresses.

This IP currently provides:

| Module | Selected by | Implementations |
|--------|-------------|-----------------|
| `prim_rf_1r1w_dec` | `mk/prim.mk` via `PRIM_RF_1R1W_DEC` | `tech_generic/prim_rf_1r1w_dec` |

The implementation preserves the original `rf_latch_1r_1w_dec` timing:
- falling edge: capture `wr_data_i` when `wr_data_en_1p_i` is high
- rising edge: update every decoded entry selected by `wr_addr_i` when `wr_en_i`
  is high
- output: combinational OR-reduction of the entries selected by `rd_addr_i`

This primitive is used by the standalone-Minion micro-Icache data path.
