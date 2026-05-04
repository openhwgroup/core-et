# prim_hot2bin

One-hot to binary encoder.

## Description

Converts a one-hot input vector to its binary index. If no bits are set, output is zero. If multiple bits are set, output is the OR of their indices (matching the original `hot2bin` + `onehot_mux` behavior from the CORE-ET library).

Pure combinational, no clock or reset.

## Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `HotW` | 8 | Width of the one-hot input |
| `BinW` | `$clog2(HotW)` | Width of the binary output (derived) |

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `hot_i` | input | `HotW` | One-hot input vector |
| `bin_o` | output | `BinW` | Binary-encoded index |

## Replaces

`hot2bin` + `onehot_mux` from the CORE-ET source tree (`rtl/libs/arbiters/`).
