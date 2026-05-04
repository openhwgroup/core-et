# tech_ecp5/prim_rf_1r1w_reg

ECP5 implementation of `prim_rf_1r1w_reg`.

Behavior is identical to the generic/original timing contract:
- falling edge: capture `wr_data_i` when `wr_data_en_1p_i` is high
- rising edge: update the addressed entry when `wr_en_i` is high
- rising edge: register `rd_addr_i` when `rd_en_i` is high
- output: combinational read from the registered address (`1` cycle read latency)

Implementation note:
- the storage array is marked `(* ram_style = "block" *)` to bias Yosys
  `synth_ecp5` toward `DP16KD` inference for the Minion dcache LRAM wrappers
  that instantiate this primitive
- the module intentionally keeps the same registered-address / combinational-read
  shape as the generic/original model rather than hand-instantiating `DP16KD`
