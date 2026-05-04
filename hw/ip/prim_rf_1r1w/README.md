# prim_rf_1r1w

Latch-timed 1R1W register-file primitive used by translated CORE-ET blocks.

This IP currently provides:

| Module | Selected by | Implementations |
|--------|-------------|-----------------|
| `prim_rf_1r1w` | `mk/prim.mk` via `PRIM_RF_1R1W` | `tech_generic/prim_rf_1r1w`, `tech_ice40/prim_rf_1r1w`, `tech_ecp5/prim_rf_1r1w`, `tech_xilinx/prim_rf_1r1w` |

The implementation preserves the original 2-phase write timing:
- falling edge: capture `wr_data_i` when `wr_data_en_i` is high
- rising edge: commit the captured data when `wr_en_i` is high

The read path is combinational, matching the original `rf_latch_1r_1w`.

For ASIC, it maps to a latch-based register file with a clock-gated write path.
The module has no reset port, so power-up contents are undefined until
software or the parent pipeline initializes the addressed entries.

## Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `Width` | `int unsigned` | 32 | Data width in bits |
| `Depth` | `int unsigned` | 32 | Number of entries |

## Ports

### `prim_rf_1r1w`

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_i` | input | 1 | Clock |
| `wr_data_en_i` | input | 1 | Write data capture enable (negedge: latch `wr_data_i` into holding register) |
| `wr_en_i` | input | 1 | Write enable (posedge: commit captured data to RF at `wr_addr_i`) |
| `wr_addr_i` | input | `$clog2(Depth)` | Write address |
| `wr_data_i` | input | `Width` | Write data |
| `rd_addr_i` | input | `$clog2(Depth)` | Read address |
| `rd_data_o` | output | `Width` | Read data (combinational, latency = 0) |

## 2-phase write timing

The write uses a 2-phase protocol that mirrors the ASIC latch-based implementation:

1. **Phase 1 (negedge):** When `wr_data_en_i` is high, `wr_data_i` is captured into an internal holding register (`wr_data_del`) on the falling clock edge.
2. **Phase 2 (posedge):** When `wr_en_i` is high, the previously captured data in `wr_data_del` is written to the register file at `wr_addr_i` on the rising clock edge.

In normal pipeline operation, `wr_data_en_i` is asserted one phase (half-cycle) before `wr_en_i`, `wr_addr_i`, and the actual write.

```
                     _______         _______
clk_i           ____/       \_______/       \____

wr_data_en_i    ----<  1  >---------< next >-----
wr_data_i       ----< val >---------< next >-----
wr_en_i         -------------------<  1  >-------
wr_addr_i       -------------------< addr >------
```

## Differences from CORE-ET Originals

| Aspect | Original | New | Rationale |
|--------|----------|-----|-----------|
| Module name | `rf_latch_1r_1w` | `prim_rf_1r1w` | Consistent naming convention |
| Port names | `clk`, `wr_data_a`, `wr_data_a_en_1p`, `wr_addr_a`, `wr_en_a`, `rd_addr_a`, `rd_data_a`, `test_en` | `clk_i`, `wr_data_i`, `wr_data_en_i`, `wr_addr_i`, `wr_en_i`, `rd_addr_i`, `rd_data_o` | Standard `_i`/`_o` suffixes |
| Parameters | `WIDTH`, `ENTRIES`, `LEVEL2_CLK_GATE` | `Width`, `Depth` | CamelCase parameters, clock gating handled by implementation |
| `test_en` port | Present | Removed | DFT handled externally |
| ASIC path | Foundry clock gates and latch arrays | Technology-selected primitive implementation | Keeps the RF behind the same target seam as other storage/clocking primitives |
| Verilator behavior | `ifdef VERILATOR` special path | Always uses the same edge-based model | Behavioral model matches the original Verilator timing exactly |

## Tests

```bash
make -C hw/ip/prim_rf_1r1w/dv test
```
