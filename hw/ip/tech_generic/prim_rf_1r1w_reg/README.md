# tech_generic/prim_rf_1r1w_reg

Behavioral/default implementation of `prim_rf_1r1w_reg`.

Contract:
- falling edge: capture `wr_data_i` when `wr_data_en_1p_i` is high
- rising edge: update the addressed entry when `wr_en_i` is high
- rising edge: register `rd_addr_i` when `rd_en_i` is high
- output: combinational read from the registered address (`1` cycle read latency)

This matches the original CORE-ET `rf_latch_1r_1w_reg` Verilator timing model.
