# tech_generic/prim_rf_1r1w_dec

Behavioral/default implementation of `prim_rf_1r1w_dec`.

Contract:
- falling edge: capture `wr_data_i` when `wr_data_en_1p_i` is high
- rising edge: update every decoded entry selected by `wr_addr_i` when `wr_en_i`
  is high
- output: combinational OR-reduction of all entries selected by `rd_addr_i`

This matches the original CORE-ET `rf_latch_1r_1w_dec` Verilator timing
model.
