<!--
Copyright (c) 2026 Ainekko
SPDX-License-Identifier: Apache-2.0
-->

# sp_apb_mux

APB address decoder and response mux for the original `standalone_minion`
service-processor debug window.

## Description

`sp_apb_mux` takes a single BPAM APB master port and routes it to one of four
targets:

- Minion debug APB space
- SPIO ESR APB space
- I-cache debug APB space
- I-cache data RAM APB space

The decode and address rewrite behavior matches the original CORE-ET
`rtl/shire/standalone_minion/sp_apb_mux.v`.

## Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `master_apb_addr` | `16` | Width of the incoming APB address |
| `master_apb_data` | `64` | Width of the incoming APB data bus |
| `esr_apb_addr` | `4` | ESR APB address width |
| `esr_apb_data` | `64` | ESR APB data width |
| `icache_apb_addr` | `10` | I-cache APB address width |
| `icache_apb_data` | `64` | I-cache APB data width |
| `minion_apb_addr` | `13` | Minion debug APB address width |
| `minion_apb_data` | `64` | Minion debug APB data width |
| `icache_d_apb_addr` | `13` | I-cache data APB address width |
| `icache_d_apb_data` | `64` | I-cache data APB data width |

## Ports

The module keeps the original standalone-shell port names because it is a
direct translation target used by the future `standalone_minion` wrapper.

### Master APB

| Port | Direction | Description |
|------|-----------|-------------|
| `apb_paddr` | input | Incoming APB address |
| `apb_penable` | input | APB access phase |
| `apb_prdata` | output | Routed read data |
| `apb_pready` | output | Routed ready |
| `apb_psel` | input | APB select |
| `apb_pslverr` | output | Decode/target slave error |
| `apb_pwdata` | input | Incoming write data |
| `apb_pwrite` | input | Incoming write strobe |

### Routed target ports

The module exposes one APB master-facing port per downstream target:

- `esr_apb_*`
- `icache_apb_*`
- `minion_apb_*`
- `ic_data_apb_*`

Only one target is selected at a time. `pwrite` and `pwdata` are broadcast to
all targets exactly like the original.

## Usage Notes

- Invalid address encodings return `apb_pready=1`, `apb_pslverr=1`,
  `apb_prdata=0`.
- The Minion region performs the same address rewrite as the original shell for
  the register (`000`) and dcache (`100`) debug windows.
- This is a pure combinational block. There is no clock or reset.

## Differences From Original

None intentionally in behavior or interface semantics.

The only repository-level difference is the added Apache copyright header.
