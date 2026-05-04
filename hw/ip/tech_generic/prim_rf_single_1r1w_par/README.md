# tech_generic/prim_rf_single_1r1w_par

Behavioral/default implementation of `prim_rf_single_1r1w_par`.

Contract:
- falling edge: capture `wr_data_i` when `wr_data_en_1p_i` is high
- rising edge: update the single storage element when `wr_en_i` is high
- output: the stored value is exposed directly

This matches the original CORE-ET `rf_latch_single_1r_1w_par` Verilator timing model.
