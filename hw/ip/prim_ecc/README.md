# prim_ecc

SECDED (Single Error Correction, Double Error Detection) ECC primitives.

Standard Hamming code with three width configurations:

| DataWidth | EccWidth | Used for |
|-----------|----------|----------|
| 12–26 | 6 | Cache tags (23-bit) |
| 27–57 | 7 | Cache tag state (33-bit) |
| 58–64 | 8 | Data doublewords (64-bit) |

## Modules

### prim_ecc_enc

Encoder. Pure combinational. Generates ECC bits from input data.

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `data_i` | input | `DataWidth` | Data to protect |
| `data_and_ecc_o` | output | `DataWidth + EccWidth` | `{ecc, data}` concatenated |

### prim_ecc_dec

Decoder/corrector. Pure combinational. Corrects single-bit errors, detects double-bit errors.

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `data_and_ecc_i` | input | `DataWidth + EccWidth` | `{ecc, data}` input |
| `data_o` | output | `DataWidth` | Corrected data |
| `sbe_o` | output | 1 | Single-bit error detected (corrected) |
| `dbe_o` | output | 1 | Double-bit error detected (uncorrectable) |

## Differences from CORE-ET `ecc_gen` / `ecc_corr`

| Aspect | Original | New | Rationale |
|--------|----------|-----|-----------|
| Module names | `ecc_gen`, `ecc_corr` | `prim_ecc_enc`, `prim_ecc_dec` | Project naming convention |
| Port naming | `data`, `data_and_ecc`, `data_corr`, `sbe`, `dbe` | `data_i`, `data_and_ecc_o`, `data_o`, `sbe_o`, `dbe_o` | lowRISC `_i/_o` convention |
| Parameters | `DATA_SIZE`, `ECC_SIZE` | `DataWidth`, `EccWidth` | UpperCamelCase convention |
| Parity matrix | Identical | Identical | Bit-compatible for cosim |
