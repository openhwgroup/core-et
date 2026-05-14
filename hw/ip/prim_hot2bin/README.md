# prim_hot2bin

One-hot to binary encoder.

## Description

`prim_hot2bin` converts a one-hot input vector to the OR-reduced binary index
used by the original CORE-ET `hot2bin` helper. If no input bits are set,
`bin_o` is zero. If multiple input bits are set, the output is the bitwise OR
of every selected index, matching `hot2bin` plus `onehot_mux`; it is not a
lowest-priority or highest-priority encoder.

The module is pure combinational logic with no clock, reset, DFT, or RAM
configuration interface.

## Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `HotW` | 8 | Width of the one-hot input. Supported values are `HotW >= 2`. |
| `BinW` | `$clog2(HotW)` | Width of the binary output (derived localparam). |

`HotW=1` is not a supported instantiation because the inherited
`$clog2(HotW)` output-width convention would produce a zero-width `bin_o`.
Non-power-of-two widths are supported; indices are still OR-reduced into
`$clog2(HotW)` bits.

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `hot_i` | input | `HotW` | One-hot or multi-hot input vector. |
| `bin_o` | output | `BinW` | OR of the binary indices for all asserted `hot_i` bits; zero when `hot_i` is zero. |

## Original mapping

This primitive replaces the CORE-ET `rtl/libs/arbiters/hot2bin.v` helper, which
builds a per-entry index array and selects it through
`rtl/libs/arbiters/onehot_mux.v`. Multi-hot inputs intentionally preserve the
original `onehot_mux` OR semantics.

## Verification

Standalone DV is provided in `hw/ip/prim_hot2bin/dv/` and standalone RTL cosim
is provided in `dv/rtlcosim/prim_hot2bin/`.

- Unit test: 311 explicit checks covering zero input, exhaustive `HotW=2`,
  exhaustive non-power-of-two `HotW=3`, exhaustive default `HotW=8`, every
  single-hot index for wider `HotW=33`, and multi-hot OR-of-indices cases.
- RTL cosim: 12,168 output comparisons against original CORE-ET `hot2bin` +
  `onehot_mux` for `HotW=2`, `3`, `8`, and `33`, with directed zero,
  single-hot, multi-hot, lowest/highest-index, and random stimulus.

## Intentional differences from original CORE-ET

| Difference | Rationale |
|------------|-----------|
| Module name and ports use `prim_hot2bin`, `hot_i`, and `bin_o`. | Project naming convention for primitive library IP. |
| Original helper composition (`hot2bin` instantiating `onehot_mux`) is flattened into one combinational module. | Preserves the exact OR-of-indices behavior while exposing one reusable primitive. |
| Supported minimum is documented as `HotW=2`. | The original `$clog2(HOT_W)` convention also gives a zero-width binary output for `HOT_W=1`; this project does not rely on zero-width ports. |
