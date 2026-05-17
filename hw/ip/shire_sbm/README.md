// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

# shire_sbm

`shire_sbm` implements the compute-shire system-bus-master path that accepts the
native SYS AXI slave channel used by the mesh and emits APB requests toward the
compute-shire APB endpoints.

The IP contains three RTL modules:

- `mshire_axi_to_apb`: one-lane AXI-address/write-data to APB converter with AXI
  B/R response generation.
- `shire_bus_master`: AXI ingress, request decode, credit/overflow tracking,
  per-lane request buffering, broadcast write sequencing, and response
  arbitration for the 10 SBM APB lanes.
- `sbm_top`: native wrapper that exposes 11 shire APB ports and inserts the
  translated APB flop stage used at the original top boundary.

## Parameters

`shire_bus_master` parameters:

| Parameter | Default | Meaning |
| --- | --- | --- |
| `SbmInterfaces` | `shire_sbm_pkg::SbmInterfaces` | Number of internal SBM APB lanes (10). |
| `MaxConcReq` | `shire_sbm_pkg::MaxConcReq` | Request-buffer/FIFO depth used for queued AXI requests (16). |
| `ApbDataWidth` | `shire_sbm_pkg::ApbDataWidth` | APB data width (64). |

`mshire_axi_to_apb` parameters:

| Parameter | Default | Meaning |
| --- | --- | --- |
| `ApbAddrWidth` | `shire_sbm_pkg::SbmApbAddrWidth` | Raw AXI/APB address width used before lane-specific APB address packing. |
| `ApbDataWidth` | `shire_sbm_pkg::ApbDataWidth` | APB data width. |
| `ShiftDataEnRd` | `1'b1` | Replicate 64-bit APB read data across the 256-bit SYS R payload when set; otherwise zero-extend. |
| `ResponseControl` | `1'b0` | Preserve original optional response-hold stage behavior. |

`sbm_top` has no user parameters.

## Interfaces

### Native SYS AXI side

The AXI payload structs are defined in `hw/ip/axi/rtl/axi_pkg.sv`:

- `axi_pkg::sys_slave_ar_t`, `sys_axi_ar_valid_i`, `sys_axi_ar_ready_o`
- `axi_pkg::sys_slave_aw_t`, `sys_axi_aw_valid_i`, `sys_axi_aw_ready_o`
- `axi_pkg::sys_slave_w_t`, `sys_axi_w_ready_o`
- `axi_pkg::sys_slave_b_t`, `sys_axi_b_valid_o`, `sys_axi_b_ready_i`
- `axi_pkg::sys_slave_r_t`, `sys_axi_r_valid_o`, `sys_axi_r_ready_i`

Only single-beat accesses (`len == 0`) are accepted. AXI IDs are preserved on B/R
responses. Write data is selected from the 256-bit SYS W payload by `AWADDR[4:3]`
to match the original 64-bit SBM APB data lane.

### APB side

APB request/response structs are defined in `shire_sbm_pkg.sv`:

- `shire_apb_req_t`: `paddr`, `psel`, `penable`, `pwrite`, `pwdata`
- `shire_apb_rsp_t`: `prdata`, `pready`, `pslverr`

`shire_bus_master` exposes 10 internal lanes:

| Lane(s) | Destination |
| --- | --- |
| 0..3 | Neighborhood/minion APB windows |
| 4..7 | Shire-cache bank APB windows |
| 8 | Shire ESR/APB window |
| 9 | RBox APB window |

`sbm_top` exposes 11 APB ports. Port 10 is currently a native placeholder for the
old BPAM/UltraSoc debug APB path and remains idle.

### Reset and status inputs

- `rst_ni`: main active-low reset.
- `rst_neigh_ni[3:0]`: active-low per-neighborhood availability/reset inputs.
- `rst_shire_cache_ni`: active-low shire-cache/bank availability/reset input.
- `rst_rbox_ni`: active-low RBox availability/reset input.
- `shire_virtual_id_i`: shire ID used by address decode.
- `credit_return_write_o`: translated write-credit return pulse.

## Address decode

The implementation preserves the original SBM decode:

- `ADDR[32]` must select the shire/system APB region.
- `ADDR[29:22]` must match `shire_virtual_id_i[7:0]`.
- `ADDR[21:20]` selects minion/neighborhood/bank/shire-local classes.
- Neighborhood and bank write broadcasts use destination `4'hf` and fan out to
  all four lanes in the selected group.
- Bad encodings, reset endpoints, read broadcasts, wrong shire IDs, and nonzero
  AXI `LEN` return `SLVERR`.

## Integration notes

Instantiate `sbm_top` at compute-shire boundaries that need the native seam:
provide the SYS AXI structs from `axi_pkg`, wire each APB request to the matching
endpoint, and tie unused APB response fields low/ready as appropriate. Use
`shire_bus_master` directly only when the APB flop stage is not desired.

## Differences from original CORE-ET

| Difference | Rationale |
| --- | --- |
| Active-low `rst_*_ni` ports replace active-high `reset*` inputs. | Project reset convention; reset-domain separation is preserved. |
| Native SYS AXI structs from `axi_pkg` replace original include-file structs/macros. | Project-native typed interface. |
| APB ports use packed request/response structs rather than parallel arrays. | Project style and C++/Verilator-friendly integration. |
| Original BPAM/UltraSoc debug arbitration is not instantiated; `sbm_top` port 10 is idle. | Third-party UltraSoc debug IP is outside the clean compute-shire native seam and is dropped per project policy. |
| `prim_fifo_reg` and `prim_arb_rr` replace original `gen_fifo_reg` and `arb_rr_data`. | Project primitive/library equivalents with standalone cosims. |
| `sbm_top` implements the original APB flop stage directly in native SV. | Keeps the original top-visible APB staging without carrying raw include/macros. |
