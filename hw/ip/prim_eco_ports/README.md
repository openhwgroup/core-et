# prim_eco_ports

ECO spare-port wrapper primitive.

`prim_eco_ports` is the repo-native replacement for the original CORE-ET
`et_eco_ports` helper used in top-level wrappers. It is not functional logic:
it keeps spare/ECO input pins alive in the design cone and ties the spare/ECO
output pins low.

## Parameters

| Parameter | Meaning |
|-----------|---------|
| `InputWidth` | Width of `eco_i` |
| `OutputWidth` | Width of `eco_o` |

## Ports

| Port | Direction | Description |
|------|-----------|-------------|
| `eco_i` | input | Spare/ECO input pins to preserve |
| `eco_o` | output | Spare/ECO output pins, always tied low |

## Differences from CORE-ET `et_eco_ports`

| Aspect | Original | New | Rationale |
|--------|----------|-----|-----------|
| Module name | `et_eco_ports` | `prim_eco_ports` | Repo-native primitive naming |
| Parameter names | `INPUT_WIDTH`, `OUTPUT_WIDTH` | `InputWidth`, `OutputWidth` | lowRISC parameter style |
| Tech override seam | `ifdef ET_LIBS_USE_STD_CELLS` inside RTL | `tech_*` primitive selection via `mk/prim.mk` | keep functional RTL free of tech conditionals |

Functional generic behavior is the same as the original non-std-cell fallback:
`eco_o` is tied low and `eco_i` is preserved only to keep the ports alive.

## Tests

```bash
make -C dv test
make -C ../../dv/rtlcosim/prim_eco_ports test
```
