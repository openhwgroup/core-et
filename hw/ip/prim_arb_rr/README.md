# prim_arb_rr -- Round-Robin Arbiter with Data Mux

## Description

Round-robin arbiter with data multiplexing. Arbitrates among `NumReqs`
requestors using round-robin fairness and selects data from the granted
requestor via a one-hot mux.

`grants_o` is one-hot when any request is active, zero otherwise.
`data_o` holds the last granted data when no request is active.
The round-robin pointer advances on `pop_i` (when at least one request is
active).

Replaces: `arb_rr_data` + `arb_rr_base` + `onehot_mux` from the CORE-ET
source tree (`rtl/libs/arbiters/`).

## Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `NumReqs` | 4 | Number of requestors |
| `Width`   | 32 | Data width in bits |

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_i` | input | 1 | Clock |
| `rst_ni` | input | 1 | Active-low async reset |
| `reqs_i` | input | NumReqs | Request vector |
| `pop_i` | input | 1 | Pop / advance round-robin pointer |
| `grants_o` | output | NumReqs | One-hot grant vector (0 when no reqs) |
| `data_i` | input | NumReqs x Width | Per-requestor data |
| `data_o` | output | Width | Data from granted (or last granted) requestor |

## Behaviour

- When one or more bits of `reqs_i` are asserted, `grants_o` is one-hot
  selecting the highest-priority requestor under the current round-robin mask.
- When `pop_i` is asserted and at least one request is active, the mask
  advances so the next cycle favours lower-priority requestors.
- When no request is active, `grants_o` is zero and `data_o` retains the
  data of the last granted requestor.
- After reset, the mask is zero -- requestor 0 has initial highest priority.

## Usage

```systemverilog
prim_arb_rr #(
  .NumReqs (4),
  .Width   (32)
) u_arb (
  .clk_i,
  .rst_ni,
  .reqs_i   (req_vec),
  .pop_i    (pop),
  .grants_o (grant_vec),
  .data_i   (data_array),
  .data_o   (selected_data)
);
```

## Differences from original

| Aspect | Original (`arb_rr_data`) | This module |
|--------|--------------------------|-------------|
| Reset | Active-high synchronous | Active-low async-assert/sync-deassert |
| Naming | `clock`/`reset`, `reqs`/`grants`/`data_in`/`data_out` | `clk_i`/`rst_ni`, `reqs_i`/`grants_o`/`data_i`/`data_o` |
| Structure | Three modules (`arb_rr_data`, `arb_rr_base`, `onehot_mux`) | Single self-contained module |
| Coding | `RST_EN_FF` macro | Explicit `always_ff` |

## Tests

```bash
make -C dv test          # run unit test
make -C dv test-trace    # run with VCD waveform
make -C dv lint          # lint RTL only
```
