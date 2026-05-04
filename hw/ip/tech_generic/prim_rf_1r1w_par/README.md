# tech_generic/prim_rf_1r1w_par

Behavioral/default implementation of `prim_rf_1r1w_par`.

Contract:
- falling edge: capture `wr_data_i` when `wr_data_en_1p_i` is high
- rising edge: update the addressed entry when `wr_en_i` is high
- output: all entries are exposed in parallel as a flat packed bus

This matches the original CORE-ET `rf_latch_1r_1w_par` Verilator timing model.
